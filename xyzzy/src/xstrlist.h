#ifndef _xstrlist_h_
#define _xstrlist_h_

class xstring_node: public xlist_node <xstring_node>
{
public:
  TCHAR data[1];
  operator const TCHAR * () const {return data;}
};

class xstring_list: public xlist <xstring_node>
{
public:
  ~xstring_list ()
    {
      while (!empty_p ())
        delete [] (char *)remove_head ();
    }
  static xstring_node *alloc (const TCHAR *s)
    {
      xstring_node *p = (xstring_node *)new char [sizeof *p + _tcslen (s) * sizeof TCHAR];
      _tcscpy (p->data, s);
      return p;
    }
  void add (const TCHAR *s) {add_head (alloc (s));}
  lisp make_list () const
    {
      lisp r = Qnil;
      for (const xstring_node *p = head (); p; p = p->next ())
        r = xcons (make_string (*p), r);
      return r;
    }
};

class xstring_pair_node: public xlist_node <xstring_pair_node>
{
public:
  TCHAR *str2;
  TCHAR str1[2];
};

class xstring_pair_list: public xlist <xstring_pair_node>
{
public:
  ~xstring_pair_list ()
    {
      while (!empty_p ())
        delete [] (char *)remove_head ();
    }
  static xstring_pair_node *alloc (const TCHAR *s1, const TCHAR *s2)
    {
      int l1 = _tcslen (s1);
      xstring_pair_node *p =
        (xstring_pair_node *)new char [sizeof *p + (l1 + _tcslen (s2)) * sizeof TCHAR];
      p->str2 = p->str1 + l1 + 1;
      _tcscpy (p->str1, s1);
      _tcscpy (p->str2, s2);
      return p;
    }
  void add (const TCHAR *s1, const TCHAR *s2) {add_head (alloc (s1, s2));}
  lisp make_list (int pair) const
    {
      lisp r = Qnil;
      if (pair)
        for (const xstring_pair_node *p = head (); p; p = p->next ())
          r = xcons (xcons (make_string (p->str1),
                            xcons (make_string (p->str2), Qnil)),
                     r);
      else
        for (const xstring_pair_node *p = head (); p; p = p->next ())
          r = xcons (make_string (p->str1), r);
      return r;
    }
};

#endif /* _xstrlist_h_ */
