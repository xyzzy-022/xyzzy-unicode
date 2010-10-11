#include "targetver.h"
#include <windows.h>
#include <malloc.h>
#include <tchar.h>

#pragma data_seg (".text")

typedef unsigned long u_long;

static TCHAR *
skip_token (TCHAR *p)
{
  if (*p == _T('"'))
    {
      for (p++; *p && *p != _T('"'); p++)
        {
#ifndef UNICODE
          if (IsDBCSLeadByte (*p) && p[1])
            p++;
#endif
        }
      if (*p == _T('"'))
        p++;
    }
  else
    {
      for (; *p && *p != _T(' ') && *p != _T('\t'); p++)
        {
#ifndef UNICODE
          if (IsDBCSLeadByte (*p) && p[1])
            p++;
#endif
        }
    }
  return p;
}

static TCHAR *
skip_white (TCHAR *p)
{
  for (; *p == _T(' ') || *p == _T('\t'); p++)
    ;
  return p;
}

static TCHAR *
split (TCHAR *&beg)
{
  TCHAR *p = skip_token (beg);
  if (*beg == _T('"'))
    {
      beg++;
      if (*CharPrev (beg, p) == _T('"'))
        p[-1] = 0;
    }
  else if (*p)
    *p++ = 0;
  return skip_white (p);
}

static TCHAR *
split (TCHAR *&beg, int &l)
{
  TCHAR *p = skip_token (beg);
  if (*beg == _T('"'))
    {
      beg++;
      l = p - beg;
      if (*CharPrev (beg, p) == _T('"'))
        l--;
    }
  else
    l = p - beg;
  return skip_white (p);
}

static u_long
parse_long (const TCHAR *p)
{
  u_long val = 0;
  for (; *p >= _T('0') && *p <= _T('9'); p++)
    val = val * 10 + *p - _T('0');
  return val;
}

static inline int
char_upcase (int c)
{
  return c >= _T('a') && c <= _T('Z') ? c - (_T('a') - _T('A')) : c;
}

static int
bcasecmp (const void *b1, const void *b2, int size)
{
  const _TUCHAR *p = (const _TUCHAR *)b1, *const pe = p + size;
  const _TUCHAR *q = (const _TUCHAR *)b2;
  int f;
  for (f = 0; p < pe && !(f = char_upcase (*p) - char_upcase (*q)); p++, q++)
    ;
  return f;
}

static void
doprint (const char *fmt, ...)
{
  char buf[1024];
  va_list ap;
  va_start (ap, fmt);
  wvsprintfA (buf, fmt, ap);
  va_end (ap);
  DWORD n;
  WriteFile (GetStdHandle (STD_ERROR_HANDLE), buf, lstrlenA (buf), &n, 0);
}

static void
syserror (int e, char *buf, int size)
{
  if (!FormatMessageA ((FORMAT_MESSAGE_FROM_SYSTEM
                        | FORMAT_MESSAGE_IGNORE_INSERTS
                        | FORMAT_MESSAGE_MAX_WIDTH_MASK),
                       0, e, GetUserDefaultLangID (),
                       buf, size, 0))
    wsprintfA (buf, "error %d", e);
}

static int
cmdmatch (const TCHAR *p, const TCHAR *pe, const TCHAR *s)
{
  if (pe - p >= 4 && (!bcasecmp (pe - 4, _T(".exe"), 4 * sizeof TCHAR)
                      || !bcasecmp (pe - 4, _T(".com"), 4 * sizeof TCHAR)))
    pe -= 4;
  int l = lstrlen (s);
  return pe - p >= l && !bcasecmp (pe - l, s, l * sizeof TCHAR);
}

static void
set_title (TCHAR *cmd)
{
  int cmdl;
  TCHAR *opt = split (cmd, cmdl);
  if (cmdmatch (cmd, cmd + cmdl, _T("cmd"))
      || cmdmatch (cmd, cmd + cmdl, _T("command")))
    {
      int optl;
      TCHAR *arg = split (opt, optl);
      if (optl == 2 && !bcasecmp (opt, _T("/c"), 2))
        {
          cmd = arg;
          split (cmd, cmdl);
        }
    }

  TCHAR *title = (TCHAR *)_alloca ((cmdl + 1) * sizeof TCHAR);
  memcpy (title, cmd, cmdl * sizeof TCHAR);
  title[cmdl] = 0;
  SetConsoleTitle (title);
}

extern "C" void
process_startup ()
{
  char buf[256];
  TCHAR *myname = skip_white (GetCommandLine ());
  TCHAR *event = split (myname);
  TCHAR *cmdline = split (event);
  TCHAR *dir = 0;
  int no_events = !lstrcmp (event, _T("--"));

  if (no_events)
    {
      dir = cmdline;
      cmdline = split (dir);
    }

  set_title (cmdline);

  PROCESS_INFORMATION pi;
  STARTUPINFO si = {sizeof si};

  si.dwFlags = STARTF_USESTDHANDLES;
  si.hStdInput = GetStdHandle (STD_INPUT_HANDLE);
  si.hStdOutput = GetStdHandle (STD_OUTPUT_HANDLE);
  si.hStdError = GetStdHandle (STD_ERROR_HANDLE);

  if (!CreateProcess (0, cmdline, 0, 0, 1, CREATE_NEW_PROCESS_GROUP,
                      0, dir, &si, &pi))
    {
      syserror (GetLastError (), buf, _countof (buf));
      doprint ("%ls: %ls: %s\n", myname, cmdline, buf);
      ExitProcess (2);
    }

  CloseHandle (pi.hThread);

  if (no_events)
    {
      if (WaitForSingleObject (pi.hProcess, INFINITE) == WAIT_FAILED)
        {
          syserror (GetLastError (), buf, _countof (buf));
          doprint ("%ls: %s\n", myname, buf);
          ExitProcess (2);
        }
    }
  else
    {
      HANDLE hevent = HANDLE (parse_long (event));

      HANDLE objects[2];
      objects[0] = hevent;
      objects[1] = pi.hProcess;
      while (1)
        {
          DWORD r = WaitForMultipleObjects (2, objects, 0, INFINITE);
          if (r == WAIT_FAILED)
            {
              syserror (GetLastError (), buf, _countof (buf));
              doprint ("%ls: %s\n", myname, buf);
              ExitProcess (2);
            }
          if (r == WAIT_OBJECT_0 + 1)
            break;

          GenerateConsoleCtrlEvent (CTRL_BREAK_EVENT, pi.dwProcessId);
          if (WaitForSingleObject (pi.hProcess, 3000) == WAIT_TIMEOUT)
            GenerateConsoleCtrlEvent (CTRL_C_EVENT, pi.dwProcessId);
          ResetEvent (hevent);
        }
    }

  DWORD code;
  GetExitCodeProcess (pi.hProcess, &code);
  ExitProcess (code);
}
