#ifndef _glob_h_
# define _glob_h_

int wild_pathname_p (const TCHAR *);
int pathname_match_p (const TCHAR *, const TCHAR *);

# define GLOB_NOT '|'

class file_masks
{
  TCHAR **fm_masks;
  static TCHAR **build_masks (lisp);

  file_masks (const file_masks &);
  file_masks &operator = (const file_masks &);
public:
  file_masks ();
  file_masks (lisp);
  ~file_masks ();
  void operator = (lisp);
  int match (const TCHAR *) const;
  int empty_p () const;
  void set_text (HWND) const;
};


inline
file_masks::file_masks ()
     : fm_masks (0)
{
}

inline
file_masks::file_masks (lisp lmasks)
     : fm_masks (build_masks (lmasks))
{
}

inline
file_masks::~file_masks ()
{
  xfree (fm_masks);
}

inline void
file_masks::operator = (lisp lmasks)
{
  TCHAR **new_masks = build_masks (lmasks);
  xfree (fm_masks);
  fm_masks = new_masks;
}

inline int
file_masks::empty_p () const
{
  return !fm_masks;
}

#endif
