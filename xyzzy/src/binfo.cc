#include "ed.h"
#include "binfo.h"

const TCHAR *const buffer_info::b_eol_name[] = {_T("lf"), _T("crlf"), _T("cr")};

TCHAR *
buffer_info::modified (TCHAR *b, int pound) const
{
  if (!pound)
    {
      int c1 = _T('-'), c2 = _T('-');
      if (b_bufp->b_modified)
        c1 = c2 = _T('*');
      if (b_bufp->read_only_p ())
        {
          c1 = _T('%');
          if (c2 == _T('-'))
            c2 = c1;
        }
      if (b_bufp->b_truncated)
        c2 = _T('#');
      *b++ = c1;
      *b++ = c2;
    }
  else
    *b++ = b_bufp->b_modified ? _T('*') : _T(' ');
  return b;
}

TCHAR *
buffer_info::read_only (TCHAR *b, int pound) const
{
  if (b_bufp->read_only_p ())
    *b++ = _T('%');
  else if (!pound && b_bufp->b_truncated)
    *b++ = _T('#');
  else
    *b++ = _T(' ');
  return b;
}

TCHAR *
buffer_info::buffer_name (TCHAR *b, TCHAR *be) const
{
  b = b_bufp->buffer_name (b, be);
  if (b == be - 1)
    *b++ = _T(' ');
  return b;
}

TCHAR *
buffer_info::file_name (TCHAR *b, TCHAR *be, int pound) const
{
  lisp name;
  if (stringp (name = b_bufp->lfile_name)
      || stringp (name = b_bufp->lalternate_file_name))
    {
      if (!pound)
        b = stpncpy (b, _T("File: "), be - b);
      b = w2s (b, be, name);
      if (b == be - 1)
        *b++ = _T(' ');
    }
  return b;
}

TCHAR *
buffer_info::file_or_buffer_name (TCHAR *b, TCHAR *be, int pound) const
{
  TCHAR *bb = b;
  b = file_name (b, be, pound);
  if (b == bb)
    b = buffer_name (b, be);
  return b;
}

static TCHAR *
docopy (TCHAR *d, TCHAR *de, const TCHAR *s, int &f)
{
  *d++ = f ? _T(' ') : _T(':');
  f = 1;
  return stpncpy (d, s, de - d);
}

TCHAR *
buffer_info::minor_mode (lisp x, TCHAR *b, TCHAR *be, int &f) const
{
  for (int i = 0; i < 10; i++)
    if (consp (x) && symbolp (xcar (x))
        && symbol_value (xcar (x), b_bufp) != Qnil)
      {
        x = xcdr (x);
        if (symbolp (x))
          {
            x = symbol_value (x, b_bufp);
            if (!stringp (x))
              break;
          }
        if (stringp (x))
          {
            *b++ = f ? _T(' ') : _T(':');
            f = 1;
            return w2s (b, be, x);
          }
      }
    else
      break;
  return b;
}

TCHAR *
buffer_info::mode_name (TCHAR *b, TCHAR *be, int c) const
{
  int f = 0;
  lisp mode = symbol_value (Vmode_name, b_bufp);
  if (stringp (mode))
    b = w2s (b, be, mode);

  if (c == _T('M'))
    {
      if (b_bufp->b_narrow_depth)
        b = docopy (b, be, _T("Narrow"), f);
      if (Fkbd_macro_saving_p () != Qnil)
        b = docopy (b, be, _T("Def"), f);
      for (lisp al = xsymbol_value (Vminor_mode_alist);
           consp (al); al = xcdr (al))
        b = minor_mode (xcar (al), b, be, f);
    }

  if (processp (b_bufp->lprocess))
    switch (xprocess_status (b_bufp->lprocess))
      {
      case PS_RUN:
        b = stpncpy (b, _T(":Run"), be - b);
        break;

      case PS_EXIT:
        b = stpncpy (b, _T(":Exit"), be - b);
        break;
      }
  return b;
}

TCHAR *
buffer_info::ime_mode (TCHAR *b, TCHAR *be) const
{
  if (!b_ime)
    return b;
  *b_ime = 1;
  return stpncpy (b, (app.ime_open_mode == kbd_queue::IME_MODE_ON
                      ? _T("‚ ") : _T("--")),
                  be - b);
}

TCHAR *
buffer_info::position (TCHAR *b, TCHAR *be) const
{
  if (b_posp)
    *b_posp = b;
  else if (b_wp)
    {
      TCHAR tem[64];
      _stprintf_s (tem, _T("%d:%d"), b_wp->w_plinenum, b_wp->w_column);
      b = stpncpy (b, tem, be - b);
    }
  return b;
}

TCHAR *
buffer_info::host_name (TCHAR *b, TCHAR *be, int pound) const
{
  if (*sysdep.host_name)
    {
      if (pound)
        *b++ = _T('@');
      b = stpncpy (b, sysdep.host_name, be - b);
    }
  return b;
}

TCHAR *
buffer_info::format (lisp fmt, TCHAR *b, TCHAR *be) const
{
  if (b_posp)
    *b_posp = 0;
  if (b_ime)
    *b_ime = 0;

  const Char *p = xstring_contents (fmt);
  const Char *const pe = p + xstring_length (fmt);

  while (p < pe && b < be)
    {
      Char c = *p++;
      if (c != _T('%'))
        {
        normal_char:
#ifndef UNICODE
          if (DBCP (c))
            *b++ = c >> 8;
#endif
          *b++ = TCHAR (c);
        }
      else
        {
          if (p == pe)
            break;

          c = *p++;
          int pound = 0;
          if (c == _T('#'))
            {
              pound = 1;
              if (p == pe)
                break;
              c = *p++;
            }

          switch (c)
            {
            default:
              goto normal_char;

            case _T('*'):
              b = modified (b, pound);
              break;

            case _T('r'):
              b = read_only (b, pound);
              break;

            case _T('p'):
              b = progname (b, be);
              break;

            case _T('v'):
              b = version (b, be);
              break;

            case _T('h'):
              b = host_name (b, be, pound);
              break;

            case _T('b'):
              b = buffer_name (b, be);
              break;

            case _T('f'):
              b = file_name (b, be, pound);
              break;

            case _T('F'):
              b = file_or_buffer_name (b, be, pound);
              break;

            case _T('M'):
            case _T('m'):
              b = mode_name (b, be, c);
              break;

            case _T('k'):
              b = encoding (b, be);
              break;

            case _T('l'):
              b = eol_code (b, be);
              break;

            case _T('i'):
              b = ime_mode (b, be);
              break;

            case _T('P'):
              b = position (b, be);
              break;
            }
        }
    }

  return b;
}
