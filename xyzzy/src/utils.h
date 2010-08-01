// -*-C++-*-
#ifndef _utils_h_
# define _utils_h_

# include <stddef.h>

void *xmalloc (size_t);
void *xrealloc (void *, size_t);
void xfree (void *);
TCHAR *xstrdup (const TCHAR *);
void *xmemdup (const void *, size_t);
TCHAR *stpcpy (TCHAR *, const TCHAR *);
TCHAR *stpncpy (TCHAR *, const TCHAR *, int);
long log2 (u_long);

# define NF_BAD 0
# define NF_INTEGER 1
# define NF_INTEGER_DOT 2
# define NF_FRACTION 3
# define NF_FLOAT 0x100
# define  NF_FLOAT_E (NF_FLOAT | 'e')
# define  NF_FLOAT_S (NF_FLOAT | 's')
# define  NF_FLOAT_F (NF_FLOAT | 'f')
# define  NF_FLOAT_D (NF_FLOAT | 'd')
# define  NF_FLOAT_L (NF_FLOAT | 'l')

int parse_number_format (const Char *, const Char *, int);
int check_integer_format (const TCHAR *, int *);
int default_float_format ();

int streq (const Char *, int, const TCHAR *);
int strequal (const TCHAR *, const Char *);
int strequal (const TCHAR *, const Char *, int);
#ifdef UNICODE
static inline int
strcaseeq (const TCHAR *w1, const TCHAR *w2)
{
  return !_tcsicmp (w1, w2);
}
#else
int strcasecmp (const char *, const char *);
static inline int
strcaseeq (const char *s1, const char *s2)
{
  return !strcasecmp (s1, s2);
}
#endif

TCHAR *jindex (const TCHAR *, int);
TCHAR *jrindex (const TCHAR *, int);
TCHAR *find_last_slash (const TCHAR *);
TCHAR *find_slash (const TCHAR *);
void convert_backsl_with_sl (TCHAR *, int, int);

inline void
map_backsl_to_sl (TCHAR *s)
{
  convert_backsl_with_sl (s, _T('\\'), _T('/'));
}

inline void
map_sl_to_backsl (TCHAR *s)
{
  convert_backsl_with_sl (s, _T('/'), _T('\\'));
}

inline TCHAR *
strappend (TCHAR *d, const TCHAR *s)
{
  return stpcpy (d + _tcslen (d), s);
}

static inline int
dir_separator_p (Char c)
{
  return c == _T('/') || c == _T('\\');
}

static inline int
dir_separator_p (int c)
{
  return c == _T('/') || c == _T('\\');
}

void paint_button_off (HDC, const RECT &);
void paint_button_on (HDC, const RECT &);
void fill_rect (HDC, int, int, int, int, COLORREF);
void fill_rect (HDC, const RECT &, COLORREF);
void draw_hline (HDC, int, int, int, COLORREF);
void draw_vline (HDC, int, int, int, COLORREF);

class find_handle
{
  HANDLE h;
public:
  find_handle (HANDLE h_) : h (h_) {}
  ~find_handle () {FindClose (h);}
};

class wnet_enum_handle
{
  HANDLE h;
public:
  wnet_enum_handle (HANDLE h_) : h (h_) {}
  ~wnet_enum_handle () {WNetCloseEnum (h);}
};

class frameDC
{
  HWND f_hwnd;
  HDC f_hdc;
  HGDIOBJ f_obr;
  enum {WIDTH = 2};
public:
  frameDC (HWND, int = 0);
  ~frameDC ();
  void frame_rect (const RECT &, int = WIDTH) const;
  void paint (const RECT &r) const
    {PatBlt (f_hdc, r.left, r.top,
             r.right - r.left, r.bottom - r.top, PATINVERT);}
};

#endif
