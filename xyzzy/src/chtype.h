// -*-C++-*-
#ifndef _chtype_h_
#define _chtype_h_

#include "cdecl.h"

#define CC_BEL 7
#define CC_BS 8
#define CC_HT 9
#define CC_TAB CC_HT
#define CC_NL 10
#define CC_LFD CC_NL
#define CC_VT 11
#define CC_FF 12
#define CC_CR 13
#define CC_RET CC_CR
#define CC_SO 14
#define CC_SI 15
#define CC_ESC 27
#define CC_SPC 32
#define CC_DEL 127
#define CC_SS2 142
#define CC_SS3 143

#define CCF_SHIFT_BIT 0x0040
#define CCF_CTRL_BIT  0x0080
#define CC_META_BIT   0x8000

#define LCHAR_MOUSE 0x10000
#define LCHAR_MENU  0x20000

#ifdef UNICODE
#define CCF_FUNCTION_MASK 0xf700
#define CCF_META 0xf600
#else
#define CCF_FUNCTION_MASK 0xff00
#define CCF_META 0xfe00
#endif

#define CCF_CHAR_MIN CCF_PRIOR
// See WINUSER.H
#define  CCF_PRIOR     (CCF_FUNCTION_MASK | 0x00)
#define  CCF_NEXT      (CCF_FUNCTION_MASK | 0x01)
#define  CCF_END       (CCF_FUNCTION_MASK | 0x02)
#define  CCF_HOME      (CCF_FUNCTION_MASK | 0x03)
#define  CCF_LEFT      (CCF_FUNCTION_MASK | 0x04)
#define  CCF_UP        (CCF_FUNCTION_MASK | 0x05)
#define  CCF_RIGHT     (CCF_FUNCTION_MASK | 0x06)
#define  CCF_DOWN      (CCF_FUNCTION_MASK | 0x07)
#define  CCF_SCROLL    (CCF_FUNCTION_MASK | 0x08)
#define  CCF_MOUSEMOVE (CCF_FUNCTION_MASK | 0x09)
#define  CCF_PAUSE     (CCF_FUNCTION_MASK | 0x0a)
#define  CCF_APPS      (CCF_FUNCTION_MASK | 0x0b)
#define  CCF_INSERT    (CCF_FUNCTION_MASK | 0x0c)
#define  CCF_DELETE    (CCF_FUNCTION_MASK | 0x0d)
#define  CCF_HELP      (CCF_FUNCTION_MASK | 0x0e)

#define  CCF_F1  (CCF_FUNCTION_MASK | 0x0f)
#define  CCF_F2  (CCF_FUNCTION_MASK | 0x10)
#define  CCF_F3  (CCF_FUNCTION_MASK | 0x11)
#define  CCF_F4  (CCF_FUNCTION_MASK | 0x12)
#define  CCF_F5  (CCF_FUNCTION_MASK | 0x13)
#define  CCF_F6  (CCF_FUNCTION_MASK | 0x14)
#define  CCF_F7  (CCF_FUNCTION_MASK | 0x15)
#define  CCF_F8  (CCF_FUNCTION_MASK | 0x16)
#define  CCF_F9  (CCF_FUNCTION_MASK | 0x17)
#define  CCF_F10 (CCF_FUNCTION_MASK | 0x18)
#define  CCF_F11 (CCF_FUNCTION_MASK | 0x19)
#define  CCF_F12 (CCF_FUNCTION_MASK | 0x1a)
#define  CCF_F13 (CCF_FUNCTION_MASK | 0x1b)
#define  CCF_F14 (CCF_FUNCTION_MASK | 0x1c)
#define  CCF_F15 (CCF_FUNCTION_MASK | 0x1d)
#define  CCF_F16 (CCF_FUNCTION_MASK | 0x1e)
#define  CCF_F17 (CCF_FUNCTION_MASK | 0x1f)
#define  CCF_F18 (CCF_FUNCTION_MASK | 0x20)
#define  CCF_F19 (CCF_FUNCTION_MASK | 0x21)
#define  CCF_F20 (CCF_FUNCTION_MASK | 0x22)
#define  CCF_F21 (CCF_FUNCTION_MASK | 0x23)
#define  CCF_F22 (CCF_FUNCTION_MASK | 0x24)
#define  CCF_F23 (CCF_FUNCTION_MASK | 0x25)
#define  CCF_F24 (CCF_FUNCTION_MASK | 0x26)
#define CCF_Fn_MAX CCF_F24

#define  CCF_LBTNDOWN (CCF_FUNCTION_MASK | 0x27)
#define  CCF_LBTNMOVE (CCF_FUNCTION_MASK | 0x28)
#define  CCF_LBTNUP   (CCF_FUNCTION_MASK | 0x29)
#define  CCF_RBTNDOWN (CCF_FUNCTION_MASK | 0x2a)
#define  CCF_RBTNMOVE (CCF_FUNCTION_MASK | 0x2b)
#define  CCF_RBTNUP   (CCF_FUNCTION_MASK | 0x2c)
#define  CCF_MBTNDOWN (CCF_FUNCTION_MASK | 0x2d)
#define  CCF_MBTNMOVE (CCF_FUNCTION_MASK | 0x2e)
#define  CCF_MBTNUP   (CCF_FUNCTION_MASK | 0x2f)

#define CCF_CTLCHAR_MIN CCF_EXCLAM
#define  CCF_EXCLAM  (CCF_FUNCTION_MASK | 0x30)
#define  CCF_DQUOTE  (CCF_FUNCTION_MASK | 0x31)
#define  CCF_NUMBER  (CCF_FUNCTION_MASK | 0x32)
#define  CCF_DOLLAR  (CCF_FUNCTION_MASK | 0x33)
#define  CCF_PERCENT (CCF_FUNCTION_MASK | 0x34)
#define  CCF_AMPER   (CCF_FUNCTION_MASK | 0x35)
#define  CCF_QUOTE   (CCF_FUNCTION_MASK | 0x36)
#define  CCF_LPAREN  (CCF_FUNCTION_MASK | 0x37)
#define  CCF_RPAREN  (CCF_FUNCTION_MASK | 0x38)
#define CCF_CTLCHAR_MAX CCF_RPAREN

#define  CCF_ASTER (CCF_FUNCTION_MASK | 0x70)
#define  CCF_PLUS  (CCF_FUNCTION_MASK | 0x71)
#define  CCF_COMMA (CCF_FUNCTION_MASK | 0x72)
#define  CCF_MINUS (CCF_FUNCTION_MASK | 0x73)
#define  CCF_DOT   (CCF_FUNCTION_MASK | 0x74)
#define  CCF_SLASH (CCF_FUNCTION_MASK | 0x75)
#define  CCF_0     (CCF_FUNCTION_MASK | 0x76)
#define  CCF_1     (CCF_FUNCTION_MASK | 0x77)
#define  CCF_2     (CCF_FUNCTION_MASK | 0x78)

#define  CCF_3     (CCF_FUNCTION_MASK | 0xb0)
#define  CCF_4     (CCF_FUNCTION_MASK | 0xb1)
#define  CCF_5     (CCF_FUNCTION_MASK | 0xb2)
#define  CCF_6     (CCF_FUNCTION_MASK | 0xb3)
#define  CCF_7     (CCF_FUNCTION_MASK | 0xb4)
#define  CCF_8     (CCF_FUNCTION_MASK | 0xb5)
#define  CCF_9     (CCF_FUNCTION_MASK | 0xb6)
#define  CCF_COLON (CCF_FUNCTION_MASK | 0xb7)
#define  CCF_SEMI  (CCF_FUNCTION_MASK | 0xb8)

#define  CCF_LT (CCF_FUNCTION_MASK | 0xf0)
#define  CCF_EQ (CCF_FUNCTION_MASK | 0xf1)
#define  CCF_GT (CCF_FUNCTION_MASK | 0xf2)
//#define  CCF_QUESTION  --> DEL
#define  CCF_BACKQ      (CCF_FUNCTION_MASK | 0xf3)
#define  CCF_LBRACE     (CCF_FUNCTION_MASK | 0xf4)
#define  CCF_VER        (CCF_FUNCTION_MASK | 0xf5)
#define  CCF_RBRACE     (CCF_FUNCTION_MASK | 0xf6)
#define  CCF_TILDE      (CCF_FUNCTION_MASK | 0xf7)
#define  CCF_EMPTY_CHAR (CCF_FUNCTION_MASK | 0xf8) // XXX

#define  CCF_XBTN1DOWN (CCF_FUNCTION_MASK | 0x39)
#define  CCF_XBTN1UP   (CCF_FUNCTION_MASK | 0x3a)
#define  CCF_XBTN1MOVE (CCF_FUNCTION_MASK | 0x3b)
#define  CCF_XBTN2DOWN (CCF_FUNCTION_MASK | 0x3c)
#define  CCF_XBTN2UP   (CCF_FUNCTION_MASK | 0x3d)
#define  CCF_XBTN2MOVE (CCF_FUNCTION_MASK | 0x3e)

#define CCF_CHAR_MAX CCF_XBTN2MOVE

#define CCF_CHAR_MASK 0x003f

#define NFUNCTION_KEYS (CCF_CHAR_MAX - CCF_CHAR_MIN + 1)

#define _CTN 1
#define _CTU 2
#define _CTL 4
#define _CTK 8
#define _CTK1 0x10
#define _CTK2 0x20

#define UTF7_SET_D 1
#define UTF7_SET_O 2
#define UTF7_SET_B 4
#define UTF7_WHITE 8
#define UTF7_IMAP4_MAILBOX_NAME 16
#define UTF7_SHIFT_CHAR 32
#define UTF7_IMAP4_SHIFT_CHAR 64

#ifndef NOT_COMPILE_TIME

inline int
_char_type (int c)
{
  extern u_char char_type_table[];
  return (char_type_table + 1) [c];
}

inline int digit_char_p (int c) {return _char_type (c) & _CTN;}
inline int upper_char_p (int c) {return _char_type (c) & _CTU;}
inline int lower_char_p (int c) {return _char_type (c) & _CTL;}
inline int alpha_char_p (int c) {return _char_type (c) & (_CTL | _CTU);}
inline int alphanumericp (int c) {return _char_type (c) & (_CTL | _CTU | _CTN);}
inline int kana_char_p (int c) {return _char_type (c) & _CTK;}
inline int kanji_char_p (int c) {return _char_type (c) & _CTK1;}
inline int kanji2_char_p (int c) {return _char_type (c) & _CTK2;}
inline int SJISP (int c) {return kanji_char_p (c);}
inline int SJIS2P (int c) {return kanji2_char_p (c);}

inline int ascii_char_p (int c) {return u_int (c) < 128;}

inline int SBCP (Char c) {return c < 256;}
inline int DBCP (Char c) {return c >= 256;}
inline int digit_char_p (Char c)
  {return ascii_char_p (c) && digit_char_p (int (c));}
inline int upper_char_p (Char c)
  {return ascii_char_p (c) && upper_char_p (int (c));}
inline int lower_char_p (Char c)
  {return ascii_char_p (c) && lower_char_p (int (c));}
inline int alpha_char_p (Char c)
  {return ascii_char_p (c) && alpha_char_p (int (c));}
inline int alphanumericp (Char c)
  {return ascii_char_p (c) && alphanumericp (int (c));}
inline int kana_char_p (Char c)
  {return SBCP (c) && kana_char_p (int (c));}
inline int kanji_char_p (Char c)
  {return DBCP (c);}

inline int SBCP (lChar c)
  {return c < 256;}
inline int DBCP (lChar c)
  {return c >= 256 && c < CHAR_LIMIT;}
inline int digit_char_p (lChar c)
  {return ascii_char_p (c) && digit_char_p (int (c));}
inline int upper_char_p (lChar c)
  {return ascii_char_p (c) && upper_char_p (int (c));}
inline int lower_char_p (lChar c)
  {return ascii_char_p (c) && lower_char_p (int (c));}
inline int alpha_char_p (lChar c)
  {return ascii_char_p (c) && alpha_char_p (int (c));}
inline int alphanumericp (lChar c)
  {return ascii_char_p (c) && alphanumericp (int (c));}
inline int kana_char_p (lChar c)
  {return SBCP (c) && kana_char_p (int (c));}
inline int kanji_char_p (lChar c)
  {return DBCP (c);}

inline int
_char_downcase (int c)
{
  extern u_char char_translate_downcase_table[];
  return char_translate_downcase_table[c];
}

inline int
_char_upcase (int c)
{
  extern u_char char_translate_upcase_table[];
  return char_translate_upcase_table[c];
}

inline int
_char_transpose_case (int c)
{
  return c ^ 0x20;
}

inline int char_downcase (int c)
  {return ascii_char_p (c) ? _char_downcase (c) : c;}
inline int char_upcase (int c)
  {return ascii_char_p (c) ? _char_upcase (c) : c;}
inline int char_transpose_case (int c)
  {return alpha_char_p (c) ? _char_transpose_case (c) : c;}
inline Char char_downcase (Char c)
  {return ascii_char_p (c) ? Char (_char_downcase (c)) : c;}
inline Char char_upcase (Char c)
  {return ascii_char_p (c) ? Char (_char_upcase (c)) : c;}
inline Char char_transpose_case (Char c)
  {return alpha_char_p (c) ? (Char)_char_transpose_case (c) : c;}

inline int
_digit_char (int c)
{
  extern char char_numeric_table[];
  return char_numeric_table[c];
}

inline int
digit_char (int c)
{
  return ascii_char_p (c) ? _digit_char (c) : 36;
}

inline int
_digit_char_p (int c, int base)
{
  int n = _digit_char (c);
  return n < base ? n : -1;
}

inline int
digit_char_p (int c, int base)
{
  return ascii_char_p (c) ? _digit_char_p (c, base) : -1;
}

extern TCHAR upcase_digit_char[];
extern TCHAR downcase_digit_char[];

inline int
meta_char_p (Char c)
{
  return (c & CC_META_BIT && c <= CC_META_BIT + 127
          && ascii_char_p (c & ~CC_META_BIT));
}

inline Char
char_to_meta_char (Char c)
{
  return Char (c | CC_META_BIT);
}

inline Char
meta_char_to_char (Char c)
{
  return Char (c & ~CC_META_BIT);
}

inline Char
function_to_meta_function (Char c)
{
  return Char ((c & ~CCF_FUNCTION_MASK) | CCF_META);
}

inline Char
meta_function_to_function (Char c)
{
  return Char (c | CCF_FUNCTION_MASK);
}

inline int
function_char_p (Char c)
{
  if (c == CCF_EMPTY_CHAR)
    return 0;
  c &= ~(CCF_CTRL_BIT | CCF_SHIFT_BIT);
  return c >= CCF_CHAR_MIN && c <= CCF_CHAR_MAX;
}

inline int
meta_function_char_p (Char c)
{
  return ((c & CCF_FUNCTION_MASK) == CCF_META
          && function_char_p (meta_function_to_function (c)));
}

inline int
pseudo_ctlchar_p (Char c)
{
  c &= ~(CCF_CTRL_BIT | CCF_SHIFT_BIT);
  return c >= CCF_CTLCHAR_MIN && c <= CCF_CTLCHAR_MAX;
}

inline int
meta_pseudo_ctlchar_p (Char c)
{
  return ((c & CCF_FUNCTION_MASK) == CCF_META
          && pseudo_ctlchar_p (meta_function_to_function (c)));
}

inline int
base64_decode (int c)
{
  extern u_char base64_decode_table[];
  return c < 128 ? base64_decode_table[c] : 65;
}

inline int
imap4_base64_decode (int c)
{
  extern u_char imap4_base64_decode_table[];
  return c < 128 ? imap4_base64_decode_table[c] : 65;
}

inline u_char
utf7_set (int c)
{
  extern u_char utf7_set_table[];
  return utf7_set_table[c];
}

inline int
hqx_decode (int c)
{
  extern u_char hqx_decode_table[];
  return c < 128 ? hqx_decode_table[c] : 64;
}

extern u_char pseudo_char2ctl_table[];
extern u_char pseudo_ctl2char_table[];

#endif /* not NOT_COMPILE_TIME */

#endif
