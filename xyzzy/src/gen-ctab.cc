#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#define NOT_COMPILE_TIME
#include "chtype.h"

static void
dump (const char *b, int size, int hex = 1)
{
  for (int i = 0; i < size; i += 8)
    {
      printf (" ");
      for (int j = 0; j < 8; j++)
        if (hex)
          printf (" 0x%02x,", *b++ & 0xff);
        else
          printf (" %d,", *b++);
      printf ("\n");
    }
}

#ifdef UNICODE

#include <winnls.h>

static void
uctype ()
{
  printf ("unsigned char unichar_type_table[] = \n");
  printf ("{\n");

  for (int i = 0; i < 256; ++i)
    {
      WCHAR src[256];
      WORD typ[256];
      unsigned char buf[256];

      for (int j = 0; j < 256; ++j)
        src[j] = i * 256 + j;

      GetStringTypeW (CT_CTYPE1, src, _countof (src), typ);

      for (int j = 0; j < 256; ++j)
        {
          WORD t = typ[j];
          unsigned char v = 0;
          if (t & C1_UPPER)   v |= UC_UPPER;
          if (t & C1_LOWER)   v |= UC_LOWER;
          if (t & C1_SPACE)   v |= UC_SPACE;
          if (t & C1_PUNCT)   v |= UC_PUNCT;
          if (t & C1_CNTRL)   v |= UC_CNTRL;
          if (t & C1_BLANK)   v |= UC_BLANK;
          if (t & C1_ALPHA)   v |= UC_ALPHA;
          if (t & C1_DEFINED) v |= UC_DEFINED;
          buf[j] = v;
        }

      dump (reinterpret_cast<const char *> (buf), _countof (buf));
    }

  printf ("};\n\n");
}

#endif

static void
ctype ()
{
  char buf[256];
  bzero (buf, sizeof buf);
  for (int i = '0'; i <= '9'; i++)
    buf[i] |= _CTN;
  for (int i = 'A'; i <= 'Z'; i++)
    buf[i] |= _CTU;
  for (int i = 'a'; i <= 'z'; i++)
    buf[i] |= _CTL;
  for (int i = 0xa1; i <= 0xdf; i++)
    buf[i] |= _CTK;
  for (int i = 0x81; i <= 0x9f; i++)
    buf[i] |= _CTK1;
  for (int i = 0xe0; i <= 0xfc; i++)
    buf[i] |= _CTK1;
  for (int i = 0x40; i <= 0x7e; i++)
    buf[i] |= _CTK2;
  for (int i = 0x80; i <= 0xfc; i++)
    buf[i] |= _CTK2;

  printf ("unsigned char char_type_table[] =\n");
  printf ("{\n");
  printf ("  0,\n");
  dump (buf, sizeof buf);
  printf ("};\n\n");
}

static void
translate ()
{
  char buf[256];
  for (int i = 0; i < 256; i++)
    buf[i] = i;
  for (int i = 'a'; i <= 'z'; i++)
    buf[i] = i - 'a' + 'A';

  printf ("unsigned char char_translate_upcase_table[] =\n");
  printf ("{\n");
  dump (buf, sizeof buf);
  printf ("};\n\n");

  for (int i = 0; i < 256; i++)
    buf[i] = i;
  for (int i = 'A'; i <= 'Z'; i++)
    buf[i] = i - 'A' + 'a';

  printf ("unsigned char char_translate_downcase_table[] =\n");
  printf ("{\n");
  dump (buf, sizeof buf);
  printf ("};\n\n");

  for (int i = 0; i < 256; i++)
    buf[i] = i;

  printf ("unsigned char char_no_translate_table[] =\n");
  printf ("{\n");
  dump (buf, sizeof buf);
  printf ("};\n\n");
}

static void
numeric ()
{
  char buf[128];
  for (int i = 0; i < 128; i++)
    buf[i] = 36;
  for (int i = '0'; i <= '9'; i++)
    buf[i] = i - '0';
  for (int i = 'A'; i <= 'Z'; i++)
    buf[i] = i - 'A' + 10;
  for (int i = 'a'; i <= 'z'; i++)
    buf[i] = i - 'a' + 10;

  printf ("char char_numeric_table[] =\n");
  printf ("{\n");
  dump (buf, sizeof buf, 0);
  printf ("};\n\n");
}

static void
ctlchars ()
{
  char buf[128], buf2[256];
  memset (buf, 0, sizeof buf);
  memset (buf2, 0, sizeof buf2);
  buf['!'] = CCF_EXCLAM;
  buf['"'] = CCF_DQUOTE;
  buf['#'] = CCF_NUMBER;
  buf['$'] = CCF_DOLLAR;
  buf['%'] = CCF_PERCENT;
  buf['&'] = CCF_AMPER;
  buf['\''] = CCF_QUOTE;
  buf['('] = CCF_LPAREN;
  buf[')'] = CCF_RPAREN;
  buf['*'] = CCF_ASTER;
  buf['+'] = CCF_PLUS;
  buf[','] = CCF_COMMA;
  buf['-'] = CCF_MINUS;
  buf['.'] = CCF_DOT;
  buf['/'] = CCF_SLASH;
  buf['0'] = CCF_0;
  buf['1'] = CCF_1;
  buf['2'] = CCF_2;
  buf['3'] = CCF_3;
  buf['4'] = CCF_4;
  buf['5'] = CCF_5;
  buf['6'] = CCF_6;
  buf['7'] = CCF_7;
  buf['8'] = CCF_8;
  buf['9'] = CCF_9;
  buf[':'] = CCF_COLON;
  buf[';'] = CCF_SEMI;
  buf['<'] = CCF_LT;
  buf['='] = CCF_EQ;
  buf['>'] = CCF_GT;
  buf['`'] = CCF_BACKQ;
  buf['{'] = CCF_LBRACE;
  buf['|'] = CCF_VER;
  buf['}'] = CCF_RBRACE;
  buf['~'] = CCF_TILDE;

  for (int i = 0; i < sizeof buf; i++)
    if (buf[i])
      buf2[buf[i] & 0xff] = i;

  printf ("unsigned char pseudo_char2ctl_table[] =\n");
  printf ("{\n");
  dump (buf, sizeof buf);
  printf ("};\n\n");

  printf ("unsigned char pseudo_ctl2char_table[] =\n");
  printf ("{\n");
  dump (buf2, sizeof buf2);
  printf ("};\n\n");
}

static void
b64tab ()
{
  char buf[128];
  memset (buf, 65, sizeof buf);

  for (int i = 'A'; i <= 'Z'; i++)
    buf[i] = i - 'A';
  for (int i = 'a'; i <= 'z'; i++)
    buf[i] = i - 'a' + 26;
  for (int i = '0'; i <= '9'; i++)
    buf[i] = i - '0' + 52;
  buf['+'] = 62;
  buf['/'] = 63;
  buf['='] = 64;

  printf ("unsigned char base64_decode_table[] =\n");
  printf ("{\n");
  dump (buf, sizeof buf);
  printf ("};\n\n");

  buf['='] = 65;
  buf['/'] = 65;
  buf[','] = 63;

  printf ("unsigned char imap4_base64_decode_table[] =\n");
  printf ("{\n");
  dump (buf, sizeof buf);
  printf ("};\n\n");
}

static void
utf7tab ()
{
  char buf[128];
  memset (buf, 0, sizeof buf);

  for (int i = 'A'; i <= 'Z'; i++)
    buf[i] = UTF7_SET_D | UTF7_SET_B;
  for (int i = 'a'; i <= 'z'; i++)
    buf[i] = UTF7_SET_D | UTF7_SET_B;
  for (int i = '0'; i <= '9'; i++)
    buf[i] = UTF7_SET_D | UTF7_SET_B;
  for (const char *p = "'(),-./:?"; *p; p++)
    buf[*p] = UTF7_SET_D;

  for (const char *p = "!\"#$%&*;<=>@[]^_`{|}"; *p; p++)
    buf[*p] = UTF7_SET_O;

  buf['+'] |= UTF7_SET_B;
  buf['/'] |= UTF7_SET_B;

  buf[' '] = UTF7_WHITE;
  buf['\t'] = UTF7_WHITE;
  buf['\r'] = UTF7_WHITE;
  buf['\n'] = UTF7_WHITE;

  for (int i = 0x20; i <= 0x25; i++)
    buf[i] |= UTF7_IMAP4_MAILBOX_NAME;
  for (int i = 0x27; i <= 0x7e; i++)
    buf[i] |= UTF7_IMAP4_MAILBOX_NAME;

  buf['+'] |= UTF7_SHIFT_CHAR;
  buf['&'] |= UTF7_IMAP4_SHIFT_CHAR;

  printf ("unsigned char utf7_set_table[] =\n");
  printf ("{\n");
  dump (buf, sizeof buf);
  printf ("};\n\n");
}

static void
hqxtab ()
{
  char buf[128];
  memset (buf, 64, sizeof buf);

  for (int i = 0; i < 64; i++)
    buf["!\"#$%&'()*+,-012345689@ABCDEFGHIJKLMNPQRSTUVXYZ[`abcdefhijklmpqr"[i]] = i;

  printf ("unsigned char hqx_decode_table[] =\n");
  printf ("{\n");
  dump (buf, sizeof buf);
  printf ("};\n\n");
}

void
main ()
{
#ifdef UNICODE
  uctype ();
#endif
  ctype ();
  translate ();
  numeric ();
  ctlchars ();
  b64tab ();
  utf7tab ();
  hqxtab ();
  exit (0);
}

