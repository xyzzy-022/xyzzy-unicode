#include "ed.h"
#include "except.h"
#include "oleconv.h"

#define MAX_LISP_CALL_STACK_DEPTH 64

struct lisp_call_stack
{
  int type;
  lisp object;
};

static lisp_call_stack lisp_call_stack_buf[MAX_LISP_CALL_STACK_DEPTH];

const Win32Exception::known_exception Win32Exception::known_excep[] =
{
  {EXCEPTION_ACCESS_VIOLATION, _T("Access violation")},
  {EXCEPTION_DATATYPE_MISALIGNMENT, _T("Data type misalignment")},
  {EXCEPTION_ARRAY_BOUNDS_EXCEEDED, _T("Array bounds exceeded")},
  {EXCEPTION_FLT_DIVIDE_BY_ZERO, _T("Floating point divide by zero")},
  {EXCEPTION_INT_DIVIDE_BY_ZERO, _T("Integer divide by zero")},
  {EXCEPTION_ILLEGAL_INSTRUCTION, _T("Illegal instruction")},
  {EXCEPTION_STACK_OVERFLOW, _T("Stack overflow")},
  {EXCEPTION_IN_PAGE_ERROR, _T("In page error")},
};

EXCEPTION_RECORD Win32Exception::r;
CONTEXT Win32Exception::c;
u_int Win32Exception::code;

inline
Win32Exception::Win32Exception (u_int code_, const EXCEPTION_POINTERS *ep)
{
  code = code_;
  r = *ep->ExceptionRecord;
  c = *ep->ContextRecord;
}

void __cdecl
se_handler (u_int code, EXCEPTION_POINTERS *ep)
{
  int i = 0;
  for (stack_trace *p = stack_trace::stp; p; p = p->last)
    if (p->type != stack_trace::empty)
      {
        if (i == MAX_LISP_CALL_STACK_DEPTH)
          break;
        lisp_call_stack_buf[i].type = p->type;
        lisp_call_stack_buf[i].object = p->fn;
        i++;
      }

#ifdef DEBUG
  if (code == EXCEPTION_IN_PAGE_ERROR)
    throw Win32Exception (code, ep);
#else
# if 0
  for (int i = 0; i < numberof (Win32Exception::known_excep); i++)
    if (code == Win32Exception::known_excep[i].code)
      throw Win32Exception (code, ep);
# else
  throw Win32Exception (code, ep);
# endif
#endif
}

static int
get_section_name (void *base, void *p, TCHAR *buf, int size)
{
  DWORD nread;
  IMAGE_DOS_HEADER dos;
  if (!ReadProcessMemory (GetCurrentProcess (),
                          base, &dos, sizeof dos, &nread))
    return 0;

  if (dos.e_magic != IMAGE_DOS_SIGNATURE)
    return 0;

  IMAGE_NT_HEADERS nt;
  if (!ReadProcessMemory (GetCurrentProcess (),
                          (char *)base + dos.e_lfanew,
                          &nt, sizeof nt, &nread))
    return 0;
  if (nt.Signature != IMAGE_NT_SIGNATURE)
    return 0;

  DWORD rva = DWORD (p) - DWORD (base);

  IMAGE_SECTION_HEADER *section =
    (IMAGE_SECTION_HEADER *)((char *)base + dos.e_lfanew
                             + offsetof (IMAGE_NT_HEADERS, OptionalHeader)
                             + nt.FileHeader.SizeOfOptionalHeader);
  for (int i = 0; i < nt.FileHeader.NumberOfSections; i++, section++)
    {
      IMAGE_SECTION_HEADER sec;
      if (!ReadProcessMemory (GetCurrentProcess (), section,
                              &sec, sizeof sec, &nread))
        continue;
      if (rva >= sec.VirtualAddress
          && rva < sec.VirtualAddress + max (sec.SizeOfRawData, sec.Misc.VirtualSize))
        {
          USES_CONVERSION;
          int l = min ((int)sizeof sec.Name, size - 1);
          _tmemcpy (buf, A2T ((char *)sec.Name), l);
          buf[l] = 0;
          return 1;
        }
    }
  return 0;
}

int
get_module_base_name (HMODULE h, LPTSTR buf, DWORD size)
{
  if (!GetModuleFileName (h, buf, size))
    return 0;
  TCHAR *p = jrindex (buf, _T('\\'));
  if (p)
    _tcscpy (buf, p + 1);
  int l = _tcslen (buf);
  if (l >= 4 && !_tcsicmp (buf + l - 4, _T(".dll")))
    buf[l - 4] = 0;
  return 1;
}

static int
get_module_name (DWORD addr, MEMORY_BASIC_INFORMATION *bi, TCHAR *buf)
{
  switch (bi->AllocationProtect & ~(PAGE_GUARD | PAGE_NOCACHE))
    {
    case PAGE_READONLY:
    case PAGE_READWRITE:
    case PAGE_WRITECOPY:
    case PAGE_EXECUTE:
    case PAGE_EXECUTE_READ:
    case PAGE_EXECUTE_READWRITE:
    case PAGE_EXECUTE_WRITECOPY:
    case PAGE_NOACCESS:
      break;

    default:
      return 0;
    }

  TCHAR path[512];
  if (!get_module_base_name (HMODULE (bi->AllocationBase), path, _countof (path)))
    return 0;

  _tcscpy (buf, path);
  if (get_section_name (bi->AllocationBase,
                        bi->BaseAddress,
                        path, _countof (path)))
    _tcscpy (stpcpy (buf + _tcslen (buf), _T("!")), path);
  return 1;
}

static int
find_module_name (void *addr, TCHAR *buf)
{
  SYSTEM_INFO si;
  GetSystemInfo (&si);
  addr = (void *)(DWORD (addr) & ~si.dwPageSize);

  MEMORY_BASIC_INFORMATION bi;
  memset (&bi, 0, sizeof bi);
  return (VirtualQuery (addr, &bi, sizeof bi)
          && get_module_name (DWORD (addr), &bi, buf));
}

static void
print_modules (FILE *fp, DWORD addr, MEMORY_BASIC_INFORMATION *bi)
{
  switch (bi->AllocationProtect & ~(PAGE_GUARD | PAGE_NOCACHE))
    {
    case PAGE_READONLY:
    case PAGE_READWRITE:
    case PAGE_WRITECOPY:
    case PAGE_EXECUTE:
    case PAGE_EXECUTE_READ:
    case PAGE_EXECUTE_READWRITE:
    case PAGE_EXECUTE_WRITECOPY:
    case PAGE_NOACCESS:
      break;

    default:
      return;
    }

  TCHAR path[MAX_PATH + IMAGE_SIZEOF_SHORT_NAME + 2];
  if (!get_module_base_name (HMODULE (bi->AllocationBase), path, MAX_PATH))
    return;
  TCHAR *p = path + lstrlen (path);
  if (get_section_name (bi->AllocationBase, bi->BaseAddress, p + 1, path + _countof (path) - p - 1))
    *p = _T('!');
  _ftprintf (fp, _T("%08x - %08x: %s\n"), addr, addr + bi->RegionSize, path);
}

static void
print_module_allocation (FILE *fp)
{
  for (DWORD addr = 0; addr < 0xffffffff;)
    {
      MEMORY_BASIC_INFORMATION bi;
      if (VirtualQuery ((void *)addr, &bi, sizeof bi))
        print_modules (fp, addr, &bi);
      else
        bi.RegionSize = 0;
      DWORD oaddr = addr;
      addr += bi.RegionSize ? bi.RegionSize : 64 * 1024;
      if (addr < oaddr)
        break;
    }
  _puttc (_T('\n'), fp);
}

#ifdef _M_IX86
static void
x86_print_registers (FILE *fp, const CONTEXT &c)
{
  _ftprintf (fp, _T("Registers:\n"));
  _ftprintf (fp, _T("EAX: %08x  EBX: %08x  ECX: %08x  EDX: %08x  ESI: %08x\n"),
           c.Eax, c.Ebx, c.Ecx, c.Edx, c.Esi);
  _ftprintf (fp, _T("EDI: %08x  ESP: %08x  EBP: %08x  EIP: %08x  EFL: %08x\n"),
           c.Edi, c.Esp, c.Ebp, c.Eip, c.EFlags);
  _ftprintf (fp, _T("CS: %04x  DS: %04x  ES: %04x  SS: %04x  FS: %04x  GS: %04x\n\n"),
           c.SegCs, c.SegDs, c.SegEs, c.SegSs, c.SegFs, c.SegGs);

  DWORD eip = c.Eip - 16;
  for (int j = 0; j < 2; j++)
    {
      _ftprintf (fp, _T("%08x:"), eip);
      for (int i = 0; i < 16; i++, eip++)
        {
          if (IsBadReadPtr ((void *)eip, 1))
            _ftprintf (fp, _T(" ??"));
          else
            _ftprintf (fp, _T(" %02x"), *(u_char *)eip);
        }
      _puttc ('\n', fp);
    }
  _puttc ('\n', fp);
}

static void
x86_stack_dump (FILE *fp, const CONTEXT &c)
{
  _ftprintf (fp, _T("Stack dump:\n"));
  DWORD esp = c.Esp, ebp = c.Ebp;

  for (int i = 0; i < 64; i++)
    {
      DWORD buf[16], nread;
      if (!ReadProcessMemory (GetCurrentProcess (), (void *)esp,
                              buf, sizeof buf, &nread)
          || nread != sizeof buf)
        break;
      for (int j = 0; j < 16; j += 4)
        _ftprintf (fp, _T("%08x: %08x %08x %08x %08x\n"),
                   esp + j * 4, buf[j], buf[j + 1], buf[j + 2], buf[j + 3]);
      _ftprintf (fp, _T("\n"));
      if (ebp <= esp || ebp & 3)
        break;
      esp = ebp;
      if (!ReadProcessMemory (GetCurrentProcess (), (void *)esp,
                              &ebp, sizeof ebp, &nread))
        ebp = 0;
//      esp += sizeof ebp;
    }
}

#endif /* _M_IX86 */

static int
bad_object_p (FILE *fp, lisp object)
{
  if (!IsBadReadPtr (object, sizeof object))
    return 0;
  _ftprintf (fp, _T("(???)\n"));
  return 1;
}

static void
print_object (FILE *fp, lisp object, int f)
{
  if (f)
    _puttc (_T('('), fp);
  if (!bad_object_p (fp, object))
    {
      if (closurep (object))
        object = xclosure_body (object);
      if (!bad_object_p (fp, object))
        {
          if (consp (object))
            {
              if (bad_object_p (fp, xcar (object)))
                ;
              else if (xcar (object) == Qlambda)
                _ftprintf (fp, _T("(lambda sexp)"));
              else if (xcar (object) == Qmacro)
                _ftprintf (fp, _T("(macro sexp)"));
              else
                _ftprintf (fp, _T("(...)"));
            }
          else
            {
              if (symbolp (object))
                object = xsymbol_name (object);
              else if (functionp (object))
                object = xfunction_name (object);
              if (bad_object_p (fp, object))
                ;
              else if (stringp (object))
                {
                  const Char *p = xstring_contents (object);
                  const Char *const pe = p + xstring_length (object);
                  if (IsBadStringPtr ((TCHAR *)p, sizeof *p * xstring_length (object)))
                    _ftprintf (fp, _T("(Invalid String)"));
                  else
                    for (; p < pe; p++)
                      {
#ifndef UNICODE
                        if (DBCP (*p))
                          putc (*p >> 8, fp);
#endif
                        _puttc (*p, fp);
                      }
                }
              else
                _ftprintf (fp, _T("..."));
            }
        }
    }

  if (f)
    _ftprintf (fp, _T(" calculating arguments...)"));
  _puttc (_T('\n'), fp);
}

static void
lisp_stack_trace (FILE *fp)
{
  _ftprintf (fp, _T("Lisp stack trace:\n"));
  for (lisp_call_stack *p = lisp_call_stack_buf, *pe = p + MAX_LISP_CALL_STACK_DEPTH;
       p < pe; p++)
    switch (p->type)
      {
      case stack_trace::empty:
        return;

      case stack_trace::special_form:
      case stack_trace::macro:
      case stack_trace::apply:
        print_object (fp, p->object, 0);
        break;

      case stack_trace::eval_args:
        print_object (fp, p->object, 1);
        break;
      }
}

void
cleanup_exception ()
{
  const TCHAR *desc = _T("Unknown exception");
  for (int i = 0; i < numberof (Win32Exception::known_excep); i++)
    if (Win32Exception::code == Win32Exception::known_excep[i].code)
      {
        desc = Win32Exception::known_excep[i].desc;
        break;
      }

  TCHAR path[PATH_MAX];
  GetModuleFileName (0, path, PATH_MAX);
  int l = _tcslen (path);
  if (l >= 4 && !_tcsicmp (path + l - 4, _T(".exe")))
    _tcscpy (path + l - 4, _T(".BUG"));
  else
    _tcscat (path, _T(".BUG"));

  TCHAR module[1024];
  if (!find_module_name (Win32Exception::r.ExceptionAddress, module))
    *module = 0;

  FILE *fp = _tfopen (path, _T("w"));
  if (!fp && GetTempPath (_countof (path), path))
    {
      TCHAR *p = find_last_slash (path);
      if (!p || p[1])
        _tcscat (path, _T("\\"));
      _tcscat (path, _T("xyzzy.BUG"));
      fp = _tfopen (path, _T("w"));
    }
  if (fp)
    {
      _ftprintf (fp, _T("%s %s Crash log:\n\n"), ProgramName, VersionString);

      _ftprintf (fp, _T("Windows %s %d.%02d.%d %s\n\n"),
                 sysdep.windows_name,
                 sysdep.os_ver.dwMajorVersion,
                 sysdep.os_ver.dwMinorVersion,
                 sysdep.os_ver.dwBuildNumber,
                 sysdep.os_ver.szCSDVersion);

      _ftprintf (fp, _T("%08x: %s\n"), Win32Exception::code, desc);
      _ftprintf (fp, _T("at %08x"), Win32Exception::r.ExceptionAddress);
      if (*module)
        _ftprintf (fp, _T(" (%s)"), module);
      _ftprintf (fp, _T("\n\n"));

#ifdef _M_IX86
      x86_print_registers (fp, Win32Exception::c);
      x86_stack_dump (fp, Win32Exception::c);
#else
# error "yet"
#endif
      _ftprintf (fp, _T("Initial stack: %08x  GC: %d\n\n"),
                 app.initial_stack, app.in_gc);

      print_module_allocation (fp);
      lisp_stack_trace (fp);
#ifdef DEBUG_GC
      _puttc (_T('\n'), fp);
      output_funcall_mark (fp);
#endif
      fclose (fp);
    }

  TCHAR msg[1024], *p = msg;
  p += _stprintf (p, _T("致命的な例外(%s)が発生しました。\nat %08x"),
                  desc, Win32Exception::r.ExceptionAddress);
  if (*module)
    p += _stprintf (p, _T(" (%s)"), module);
  *p++ = _T('\n');
  *p++ = _T('\n');
  if (fp)
    p += _stprintf (p,
                    _T("気が向いたら以下のファイルを添えて作者に報告してください。\n")
                    _T("\n%s\n\n")
                    _T("その際、どのような操作をしたか、また、同じ操作をして再現す\n")
                    _T("るかどうかなども合わせて報告してください。なお、IMEの操作\n")
                    _T("中に発生したもので再現性がある場合は、他のアプリケーション\n")
                    _T("(メモ帳など)でも再現するかどうかを確認してみてください。")
                    _T("\n\n"),
                    path);
  _tcscpy (p,
           _T("運がよければ、書きかけのファイルが救えるかもしれません。\n")
           _T("試しに自動セーブしてみますか?"));

  if (MsgBox (get_active_window (), msg, TitleBarString,
              MB_ICONHAND | MB_YESNO, 1) != IDYES)
    return;

  try
    {
      do_auto_save (0, 1);
    }
  catch (nonlocal_jump &)
    {
    }
}
