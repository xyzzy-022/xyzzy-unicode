#include <ctype.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <tchar.h>

typedef unsigned char u_char;
typedef unsigned short u_short;
typedef unsigned int u_int;

#if 0
static const u_char h_20_7e[] = (" !\"#$%&'()*+,-./"
                                 "0123456789:;<=>?"
                                 "@ABCDEFGHIJKLMNO"
                                 "PQRSTUVWXYZ[\\]^_"
                                 "`abcdefghijklmno"
                                 "pqrstuvwxyz{|}~");

//                                0123456789abcdef
static const u_char h_a1_df[] = ( "｡｢｣､･ｦｧｨｩｪｫｬｭｮｯ"   // A
                                 "ｰｱｲｳｴｵｶｷｸｹｺｻｼｽｾｿ"   // B
                                 "ﾀﾁﾂﾃﾄﾅﾆﾇﾈﾉﾊﾋﾌﾍﾎﾏ"   // C
                                 "ﾐﾑﾒﾓﾔﾕﾖﾗﾘﾙﾚﾛﾜﾝﾞﾟ"); // D
#endif

#define HK_START _TUCHAR (_T('｡'))
#define HK_END   _TUCHAR (_T('ﾟ'))

static const TCHAR f_20_7e[] = _T("　！”＃＄％＆’（）＊＋，－．／")
                               _T("０１２３４５６７８９：；＜＝＞？")
                               _T("＠ＡＢＣＤＥＦＧＨＩＪＫＬＭＮＯ")
                               _T("ＰＱＲＳＴＵＶＷＸＹＺ［￥］＾＿")
                               _T("‘ａｂｃｄｅｆｇｈｉｊｋｌｍｎｏ")
                               _T("ｐｑｒｓｔｕｖｗｘｙｚ｛｜｝￣");

static const TCHAR fk_a1_df[] = _T(  "。「」、・ヲァィゥェォャュョッ")
                                _T("ーアイウエオカキクケコサシスセソ")
                                _T("タチツテトナニヌネノハヒフヘホマ")
                                _T("ミムメモヤユヨラリルレロワン゛゜");

static const TCHAR fh_a1_df[] = _T(  "。「」、・をぁぃぅぇぉゃゅょっ")
                                _T("ーあいうえおかきくけこさしすせそ")
                                _T("たちつてとなにぬねのはひふへほま")
                                _T("みむめもやゆよらりるれろわん゛゜");

static const TCHAR fh_voiced_b6_ce[] = _T("がぎぐげござじずぜぞだぢづでど")
                                       _T("　　　　　ばびぶべぼ");

static const TCHAR fk_voiced_b3_ce[] = _T("ヴ　　ガギグゲゴザジズゼゾ")
                                       _T("ダヂヅデド　　　　　バビブベボ");

static const TCHAR fh_semi_voiced_ca_ce[] = _T("ぱぴぷぺぽ");

static const TCHAR fk_semi_voiced_ca_ce[] = _T("パピプペポ");

#ifdef UNICODE
#define F(T, N) T[N]
#else
#define F(T, N) (u_short ((u_char ((T) [(N) * 2]) << 8) + u_char ((T) [(N) * 2 + 1])))
#endif

static char *upper (const char *s)
{
  char *r = _strdup (s), *p = r;
  while (*p) *p++ = toupper (*p);
  return r;
}

static char *lower (const char *s)
{
  char *r = _strdup (s), *p = r;
  while (*p) *p++ = tolower (*p);
  return r;
}

static void
soutput (u_short *buf, int n, const char *name)
{
  int min, max;

  for (min = 0; min < 256 && !buf[min]; min++)
    ;
  for (max = 255; max > min && !buf[max]; max--)
    ;
  printf ("#define VOICED_SOUND_%s_MIN 0x%04x\n", upper (name), (n << 8) + min);
  printf ("#define VOICED_SOUND_%s_MAX 0x%04x\n", upper (name), (n << 8) + max);
#ifdef UNICODE
  printf ("static const Char voiced_sound_%s[] = {\n", lower (name));
#else
  printf ("static const u_char voiced_sound_%s[] = {\n", lower (name));
#endif
  for (int i = min, j = 0; i <= max; i++, j++)
    {
#ifdef UNICODE
      printf ("0x%04x,", buf[i]);
#else
      printf ("0x%02x,", buf[i]);
#endif
      if ((j & 7) == 7)
        printf ("\n");
    }
  printf ("};\n\n");
}

static void
make_ss_table (void)
{
  u_short buf[256];

#ifdef UNICODE
  memset (buf, 0, sizeof buf);
  for (int i = 0xb6; i <= 0xce; i++)
    {
      if (F (fh_voiced_b6_ce, i - 0xb6) != 0x3000)
        buf[F (fh_voiced_b6_ce, i - 0xb6) - 0x3000] = i - 0xb6 + 0xff76;
    }
  for (int i = 0xca; i <= 0xce; i++)
    buf[F (fh_semi_voiced_ca_ce, i - 0xca) - 0x3000] = (i - 0xca + 0xff8a) & 0x7fff;
  soutput (buf, 0x30, "hira");

  memset (buf, 0, sizeof buf);
  for (int i = 0xb3; i <= 0xce; i++)
    {
      if (F (fk_voiced_b3_ce, i - 0xb3) != 0x3000)
        buf[F (fk_voiced_b3_ce, i - 0xb3) - 0x3000] = i - 0xb3 + 0xff73;
    }
  for (int i = 0xca; i <= 0xce; i++)
    buf[F (fk_semi_voiced_ca_ce, i - 0xca) - 0x3000] = (i - 0xca + 0xff8a) & 0x7fff;
  soutput (buf, 0x30, "kata");

#else
  memset (buf, 0, sizeof buf);
  for (int i = 0xb6; i <= 0xce; i++)
    {
      if (F (fh_voiced_b6_ce, i - 0xb6) >= 0x8200
          && F (fh_voiced_b6_ce, i - 0xb6) < 0x8300)
        buf[F (fh_voiced_b6_ce, i - 0xb6) - 0x8200] = i;
    }
  for (int i = 0xca; i <= 0xce; i++)
    buf[F (fh_semi_voiced_ca_ce, i - 0xca) - 0x8200] = i & 0x7f;
  soutput (buf, 0x82, "hira");

  memset (buf, 0, sizeof buf);
  for (int i = 0xb3; i <= 0xce; i++)
    {
      if (F (fk_voiced_b3_ce, i - 0xb3) >= 0x8300
          && F (fk_voiced_b3_ce, i - 0xb3) < 0x8400)
        buf[F (fk_voiced_b3_ce, i - 0xb3) - 0x8300] = i;
    }
  for (int i = 0xca; i <= 0xce; i++)
    buf[F (fk_semi_voiced_ca_ce, i - 0xca) - 0x8300] = i & 0x7f;
  soutput (buf, 0x83, "kata");
#endif
}

static void
fill (u_short *buf, int from, int to)
{
  memset (buf, 0, sizeof *buf * 256);
  for (int i = 0x20; i <= 0x7e; i++)
    {
      if (F (f_20_7e, i - 0x20) >= from
          && F (f_20_7e, i - 0x20) < to)
        buf[F (f_20_7e, i - 0x20) - from] = i;
    }
  for (int i = HK_START; i <= HK_END; i++)
    {
      if (F (fk_a1_df, i - HK_START) >= from
          && F (fk_a1_df, i - HK_START) < to)
        buf[F (fk_a1_df, i - HK_START) - from] = i;
    }
  for (int i = HK_START; i <= HK_END; i++)
    {
      if (F (fh_a1_df, i - HK_START) >= from
          && F (fh_a1_df, i - HK_START) < to)
        buf[F (fh_a1_df, i - HK_START) - from] = i;
    }
}

static void
output (u_short *buf, int from, const char *name)
{
  int min, max;

  for (min = 0; min < 256 && !buf[min]; min++)
    ;
  for (max = 255; max > min && !buf[max]; max--)
    ;
  printf ("#define TO_HALF_WIDTH_%s_MIN 0x%04x\n", upper (name), from + min);
  printf ("#define TO_HALF_WIDTH_%s_MAX 0x%04x\n", upper (name), from + max);
#ifdef UNICODE
  printf ("static const Char to_half_width_%s[] = {\n", lower (name));
#else
  printf ("static const u_char to_half_width_%s[] = {\n", lower (name));
#endif
  for (int i = min, j = 0; i <= max; i++, j++)
    {
#ifdef UNICODE
      printf ("0x%04x,", buf[i]);
#else
      printf ("0x%02x,", buf[i]);
#endif
      if ((j & 7) == 7)
        printf ("\n");
    }
  printf ("};\n\n");
}

int
main ()
{
  printf ("static const Char to_full_20_7e[] = {\n");
  for (int i = 0x20; i <= 0x7e; i++)
    {
      printf ("0x%04x,", F (f_20_7e, i - 0x20));
      if ((i & 7) == 7)
	printf ("\n");
    }
  printf ("};\n\n");

  printf ("#define HALF_WIDTH_KATAKANA_MIN 0x%04x\n", _TUCHAR (_T('｡')));
  printf ("#define HALF_WIDTH_KATAKANA_MAX 0x%04x\n", _TUCHAR (_T('ﾟ')));
  printf ("\n");

  printf ("static const Char to_fullkata[] = {\n");
  for (int i = 0xa1; i <= 0xdf; i++)
    {
      printf ("0x%04x,", F (fk_a1_df, i - 0xa1));
      if ((i & 7) == 7)
	printf ("\n");
    }
  printf ("};\n\n");

  printf ("static const Char to_fullhira[] = {\n");
  for (int i = 0xa1; i <= 0xdf; i++)
    {
      printf ("0x%04x,", F (fh_a1_df, i - 0xa1));
      if ((i & 7) == 7)
	printf ("\n");
    }
  printf ("};\n\n");

  printf ("#define TO_FULLHIRA_VOICED_MIN 0x%04x\n", _TUCHAR (_T('ｶ')));
  printf ("#define TO_FULLHIRA_VOICED_MAX 0x%04x\n", _TUCHAR (_T('ﾎ')));
  printf ("static const Char to_fullhira_voiced[] = {\n");
  for (int i = 0xb6; i <= 0xce; i++)
    {
      int uc = F (fh_voiced_b6_ce, i - 0xb6);
      printf ("0x%04x,",
              (uc == F (_T("　"), 0)) ? 0 : uc);
      if ((i & 7) == 7)
	printf ("\n");
    }
  printf ("};\n\n");

  printf ("#define TO_FULLKATA_VOICED_MIN 0x%04x\n", _TUCHAR (_T('ｳ')));
  printf ("#define TO_FULLKATA_VOICED_MAX 0x%04x\n", _TUCHAR (_T('ﾎ')));
  printf ("static const Char to_fullkata_voiced[] = {\n");
  for (int i = 0xb3; i <= 0xce; i++)
    {
      int uc = F (fk_voiced_b3_ce, i - 0xb3);
      printf ("0x%04x,",
              (uc == F (_T("　"), 0)) ? 0 : uc);
      if ((i & 7) == 7)
	printf ("\n");
    }
  printf ("};\n\n");

  printf ("#define TO_FULLHIRA_SEMI_VOICED_MIN 0x%04x\n", _TUCHAR (_T('ﾊ')));
  printf ("#define TO_FULLHIRA_SEMI_VOICED_MAX 0x%04x\n", _TUCHAR (_T('ﾎ')));
  printf ("static const Char to_fullhira_semi_voiced[] = {\n");
  for (int i = 0xca; i <= 0xce; i++)
    printf ("0x%04x,", F (fh_semi_voiced_ca_ce, i - 0xca));
  printf ("};\n\n");

  printf ("#define TO_FULLKATA_SEMI_VOICED_MIN 0x%04x\n", _TUCHAR (_T('ﾊ')));
  printf ("#define TO_FULLKATA_SEMI_VOICED_MAX 0x%04x\n", _TUCHAR (_T('ﾎ')));
  printf ("static const Char to_fullkata_semi_voiced[] = {\n");
  for (int i = 0xca; i <= 0xce; i++)
    printf ("0x%04x,", F (fk_semi_voiced_ca_ce, i - 0xca));
  printf ("};\n\n");

  printf ("#define VOICED_SOUND_MARK 0x%04x\n", F (_T("゛"), 0));
  printf ("#define SEMI_VOICED_SOUND_MARK 0x%04x\n", F (_T("゜"), 0));
  printf ("#define HALF_WIDTH_VOICED_SOUND_MARK 0x%04x\n", _TUCHAR (_T('ﾞ')));
  printf ("#define HALF_WIDTH_SEMI_VOICED_SOUND_MARK 0x%04x\n", _TUCHAR (_T('ﾟ')));
  printf ("\n");

#ifdef UNICODE
  {
    u_short buf[256];
    fill (buf, 0x2000, 0x2100);
    output (buf, 0x2000, "punct_0");
    fill (buf, 0x3000, 0x3040);
    output (buf, 0x3000, "punct_1");
    fill (buf, 0xff00, 0x10000);
    output (buf, 0xff00, "punct_2");

    fill (buf, 0x3040, 0x30a0);
    output (buf, 0x3040, "hira");
    fill (buf, 0x30a0, 0x3100);
    output (buf, 0x30a0, "kata");
  }
#else
  {
    u_short buf[256];

    fill (buf, 0x8100, 0x8200);
    output (buf, 0x8100, "punct");
    fill (buf, 0x8200, 0x8300);
    output (buf, 0x8200, "hira");
    fill (buf, 0x8300, 0x8400);
    output (buf, 0x8300, "kata");
  }
#endif

  make_ss_table ();

  printf ("#define FULL_WIDTH_HIRAGANA_MIN 0x%04x\n", F (_T("ぁ"), 0));
  printf ("#define FULL_WIDTH_HIRAGANA_MAX 0x%04x\n", F (_T("ん"), 0));
  printf ("#define FULL_WIDTH_KATAKANA_MIN 0x%04x\n", F (_T("ァ"), 0));
  printf ("#define FULL_WIDTH_KATAKANA_MAX 0x%04x\n", F (_T("ヶ"), 0));

  return 0;
}

