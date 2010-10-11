#include "cdecl.h"
#include "chtype.h"

struct print_char_name
{
  const TCHAR *name;
  int l;
  Char code;
};

#define X(A, B) {A, _countof (A) - 1, B}

static const print_char_name standard_char_names[] =
{
  X (_T("TAB"), CC_TAB),
  X (_T("SPC"), CC_SPC),
  X (_T("LFD"), CC_NL),
  X (_T("RET"), CC_CR),
  X (_T("ESC"), CC_ESC),
  X (_T("DEL"), CC_DEL),
  X (_T("NUL"), 0),
};

static const print_char_name function_char_names[] =
{
  X (_T("PageUp"), CCF_PRIOR),
  X (_T("PageDown"), CCF_NEXT),
  X (_T("End"), CCF_END),
  X (_T("Home"), CCF_HOME),
  X (_T("Left"), CCF_LEFT),
  X (_T("Up"), CCF_UP),
  X (_T("Right"), CCF_RIGHT),
  X (_T("Down"), CCF_DOWN),
  X (_T("Pause"), CCF_PAUSE),
  X (_T("MouseMove"), CCF_MOUSEMOVE),
  X (_T("Scroll"), CCF_SCROLL),
  X (_T("Apps"), CCF_APPS),
  X (_T("Insert"), CCF_INSERT),
  X (_T("Delete"), CCF_DELETE),
  X (_T("Help"), CCF_HELP),
  X (_T("F24"), CCF_F24),
  X (_T("F23"), CCF_F23),
  X (_T("F22"), CCF_F22),
  X (_T("F21"), CCF_F21),
  X (_T("F20"), CCF_F20),
  X (_T("F19"), CCF_F19),
  X (_T("F18"), CCF_F18),
  X (_T("F17"), CCF_F17),
  X (_T("F16"), CCF_F16),
  X (_T("F15"), CCF_F15),
  X (_T("F14"), CCF_F14),
  X (_T("F13"), CCF_F13),
  X (_T("F12"), CCF_F12),
  X (_T("F11"), CCF_F11),
  X (_T("F10"), CCF_F10),
  X (_T("F9"), CCF_F9),
  X (_T("F8"), CCF_F8),
  X (_T("F7"), CCF_F7),
  X (_T("F6"), CCF_F6),
  X (_T("F5"), CCF_F5),
  X (_T("F4"), CCF_F4),
  X (_T("F3"), CCF_F3),
  X (_T("F2"), CCF_F2),
  X (_T("F1"), CCF_F1),
  X (_T("LBtnDown"), CCF_LBTNDOWN),
  X (_T("LBtnUp"), CCF_LBTNUP),
  X (_T("LBtnMove"), CCF_LBTNMOVE),
  X (_T("RBtnDown"), CCF_RBTNDOWN),
  X (_T("RBtnUp"), CCF_RBTNUP),
  X (_T("RBtnMove"), CCF_RBTNMOVE),
  X (_T("MBtnDown"), CCF_MBTNDOWN),
  X (_T("MBtnUp"), CCF_MBTNUP),
  X (_T("MBtnMove"), CCF_MBTNMOVE),
  X (_T("XBtn1Down"), CCF_XBTN1DOWN),
  X (_T("XBtn1Up"), CCF_XBTN1UP),
  X (_T("XBtn1Move"), CCF_XBTN1MOVE),
  X (_T("XBtn2Down"), CCF_XBTN2DOWN),
  X (_T("XBtn2Up"), CCF_XBTN2UP),
  X (_T("XBtn2Move"), CCF_XBTN2MOVE),
};

static const print_char_name char_bit_names[] =
{
  X (_T("C-"), CCF_CTRL_BIT),
  X (_T("M-"), CCF_META),
  X (_T("S-"), CCF_SHIFT_BIT),
};

static inline int
seql (const Char *p1, const _TUCHAR *p2)
{
  for (; *p2; p1++, p2++)
#ifdef UNICODE
    if (char_upcase (*p1) != char_upcase (*p2))
#else
    if (char_upcase (*p1) != _char_upcase (*p2))
#endif
      return 0;
  return 1;
}

static inline int
sequal (const Char *p1, const _TUCHAR *p2)
{
  for (; *p2; p1++, p2++)
    if (*p1 != *p2)
      return 0;
  return 1;
}

Char
standard_char_name2Char (const Char *name, int l)
{
  for (const print_char_name *p = standard_char_names,
       *pe = p + _countof (standard_char_names);
       p < pe; p++)
    if (l == p->l && seql (name, (const _TUCHAR *)p->name))
      return p->code;
  return Char (-1);
}

Char
function_char_name2Char (const Char *name, int l)
{
  for (const print_char_name *p = function_char_names,
       *pe = p + _countof (function_char_names);
       p < pe; p++)
    if (l == p->l && sequal (name, (const _TUCHAR *)p->name))
      return p->code;
  return Char (-1);
}

Char
char_bit_name2Char (const Char *name, int l, int &xl)
{
  for (const print_char_name *p = char_bit_names,
       *pe = p + _countof (char_bit_names);
       p < pe; p++)
    if (l >= p->l && seql (name, (const _TUCHAR *)p->name))
      {
        xl = p->l;
        return p->code;
      }
  return 0;
}

const TCHAR *
function_Char2name (Char c)
{
  for (const print_char_name *p = function_char_names,
       *pe = p + _countof (function_char_names);
       p < pe; p++)
    if (p->code == c)
      return p->name;
  return 0;
}

const TCHAR *
standard_Char2name (Char c)
{
  for (const print_char_name *p = standard_char_names,
       *pe = p + _countof (standard_char_names);
       p < pe; p++)
    if (p->code == c)
      return p->name;
  return 0;
}
