#ifndef _binfo_h_
#define _binfo_h_

class buffer_info
{
  const Window *const b_wp;
  const Buffer *const b_bufp;
  TCHAR **const b_posp;
  int *const b_ime;
  static const TCHAR *const b_eol_name[];

  TCHAR *minor_mode (lisp, TCHAR *, TCHAR *, int &) const;
public:
  buffer_info (const Window *wp, const Buffer *bp, TCHAR **posp, int *ime)
       : b_wp (wp), b_bufp (bp), b_posp (posp), b_ime (ime) {}
  TCHAR *format (lisp, TCHAR *, TCHAR *) const;
  TCHAR *modified (TCHAR *, int) const;
  TCHAR *read_only (TCHAR *, int) const;
  TCHAR *progname (TCHAR *b, TCHAR *be) const
    {return stpncpy (b, ProgramName, be - b);}
  TCHAR *version (TCHAR *b, TCHAR *be) const
    {return stpncpy (b, VersionString, be - b);}
  TCHAR *buffer_name (TCHAR *, TCHAR *) const;
  TCHAR *file_name (TCHAR *, TCHAR *, int) const;
  TCHAR *file_or_buffer_name (TCHAR *, TCHAR *, int) const;
  TCHAR *mode_name (TCHAR *, TCHAR *, int) const;
  TCHAR *encoding (TCHAR *b, TCHAR *be) const
    {return w2s (b, be, xchar_encoding_name (b_bufp->lchar_encoding));}
  TCHAR *eol_code (TCHAR *b, TCHAR *be) const
    {return stpncpy (b, b_eol_name[b_bufp->b_eol_code], be - b);}
  TCHAR *ime_mode (TCHAR *, TCHAR *) const;
  TCHAR *position (TCHAR *, TCHAR *) const;
  TCHAR *host_name (TCHAR *, TCHAR *, int) const;
};

#endif
