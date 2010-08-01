#include <tchar.h>
#include <locale.h>
#include <stdio.h>
#include <stdlib.h>
#include "cdecl.h"
#include "chtype.h"

#include "chtab.gen.cc"

struct msgdef
{
  const char *ident;
  const TCHAR *text;
};

#define MSG(a, b) {#a, b}

static const msgdef msg[] =
{
#include "msgdef.h"
};

static void
print_quote (const TCHAR *p)
{
  while (*p)
    {
      if (_TUCHAR (*p) < _T(' '))
        printf ("\\%03o", *p++);
      else
        {
          if (*p == _T('\\') || *p == _T('"'))
            _puttchar (_T('\\'));
          _puttchar (*p);
#ifdef UNICODE
          p++;
#else
          if (SJISP (*p++ & 0xff))
            putchar (*p++);
#endif
        }
    }
}

static void
print_quote_rc (const TCHAR *p)
{
  while (*p)
    {
      if (_TUCHAR (*p) < _T(' '))
        printf ("\\%03o", *p++);
      else
        {
          if (*p == _T('\\') || *p == _T('"'))
            _puttchar (*p);
          _puttchar (*p);
#ifdef UNICODE
          p++;
#else
          if (SJISP (*p++ & 0xff))
            putchar (*p++);
#endif
        }
    }
}

void
main (int argc, char **argv)
{
  _tsetlocale(LC_ALL, _T(""));

  if (argc == 1)
    exit (2);
  if (!strcmp (argv[1], "-def"))
    {
      for (int i = 0; i < _countof (msg); i++)
        printf ("#define %s %d\n", msg[i].ident, i);
    }
  else if (!strcmp (argv[1], "-enum"))
    {
      printf ("enum message_code\n{\n");
      for (int i = 0; i < _countof (msg) - 1; i++)
        printf ("  %s,\n", msg[i].ident);
      printf ("  %s\n", msg[i].ident);
      printf ("};\n");
    }
  else if (!strcmp (argv[1], "-c"))
    {
      printf ("#include <tchar.h>\n\n");
      printf ("const TCHAR SSM[] =\n");
      for (int i = 0; i < _countof (msg); i++)
        {
          printf ("  _T(\"");
          print_quote (msg[i].text);
          printf ("\\0\")\n");
        }
      printf (";\n\n");


      printf ("static const TCHAR *const message_string[] =\n");
      printf ("{\n");
      int l = 0;
      for (i = 0; i < _countof (msg); i++)
        {
          printf ("  SSM + %d,\n", l);
          l += _tcslen (msg[i].text) + 1;
        }
      printf ("};\n\n");

      printf ("const TCHAR *\n"
              "get_message_string (int code)\n"
              "{return message_string[code];}\n");
    }
  else if (!strcmp (argv[1], "-stbl"))
    {
      printf ("STRINGTABLE DISCARDABLE\n");
      printf ("BEGIN\n");
      for (int i = 0; i < _countof (msg); i++)
        {
          printf ("  %d \"", i + 1024);
          print_quote_rc (msg[i].text);
          printf ("\"\n");
        }
      printf ("END\n");
    }
  else if (!strcmp (argv[1], "-rc"))
    {
      printf ("#include \"ed.h\"\n"
              "const char *\n"
              "get_message_string (int code)\n"
              "{\n"
              "  static char buf[256];\n"
              "  if (!LoadString (app.hinst, 1024 + code, buf, sizeof buf))\n"
              "    sprintf (buf, \"String resource %%d not found.\", code);\n"
              "  return buf;\n"
              "}\n");
    }
  exit (0);
}
