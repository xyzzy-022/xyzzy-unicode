#ifndef _environ_h_
# define _environ_h_

class environ
{
public:
  static int save_window_size;
  static int save_window_position;
  static int restore_window_size;
  static int restore_window_position;
  static int load_geometry (int, POINT *, SIZE *);
  static void save_geometry ();
};

class Registry
{
protected:
  static const TCHAR base[];
  HKEY hkey;
  Registry ();
  ~Registry ();
public:
  static const TCHAR Settings[];
  int fail () const;
};

inline
Registry::Registry ()
     : hkey (0)
{
}

inline
Registry::~Registry ()
{
  if (hkey)
    RegCloseKey (hkey);
}

inline int
Registry::fail () const
{
  return !hkey;
}

class ReadRegistry: public Registry
{
protected:
  void open_local (const TCHAR *);
public:
  int get (const TCHAR *, void *, DWORD, DWORD) const;
  int get (const TCHAR *, int *) const;
  int get (const TCHAR *, long *) const;
  int get (const TCHAR *, TCHAR *, int) const;
  int get (const TCHAR *, void *, int) const;
  int query (const TCHAR *, DWORD *) const;
  ReadRegistry (const TCHAR *);
  ReadRegistry (HKEY, const TCHAR *);
};

inline
ReadRegistry::ReadRegistry (const TCHAR *subkey)
{
  open_local (subkey);
}

inline int
ReadRegistry::get (const TCHAR *key, int *x) const
{
  return get (key, x, sizeof *x, REG_DWORD) == sizeof *x;
}

inline int
ReadRegistry::get (const TCHAR *key, long *x) const
{
  return get (key, x, sizeof *x, REG_DWORD) == sizeof *x;
}

inline int
ReadRegistry::get (const TCHAR *key, TCHAR *buf, int size) const
{
  return get (key, buf, size * sizeof *buf, REG_SZ);
}

inline int
ReadRegistry::get (const TCHAR *key, void *buf, int size) const
{
  return get (key, buf, size, REG_BINARY);
}

class WriteRegistry: public Registry
{
public:
  int set (const TCHAR *, DWORD, const void *, int) const;
  int set (const TCHAR *, long) const;
  int set (const TCHAR *, const TCHAR *) const;
  int set (const TCHAR *, const TCHAR *, int) const;
  int set (const TCHAR *, const void *, int) const;
  int remove (const TCHAR *) const;
  WriteRegistry (const TCHAR *);
};

inline int
WriteRegistry::set (const TCHAR *key, long val) const
{
  return set (key, REG_DWORD, &val, sizeof val);
}

inline int
WriteRegistry::set (const TCHAR *key, const TCHAR *val) const
{
  return set (key, REG_SZ, val, (_tcslen (val) + 1) * sizeof *val);
}

inline int
WriteRegistry::set (const TCHAR *key, const TCHAR *val, int size) const
{
  return set (key, REG_SZ, val, size * sizeof *val);
}

inline int
WriteRegistry::set (const TCHAR *key, const void *val, int size) const
{
  return set (key, REG_BINARY, val, size);
}

class EnumRegistry: public ReadRegistry
{
public:
  EnumRegistry (const TCHAR *subkey) : ReadRegistry (subkey) {}
  EnumRegistry (HKEY h, const TCHAR *subkey) : ReadRegistry (h, subkey) {}
  operator HKEY () const {return hkey;}
};

struct decoded_time
{
  int year;
  int mon;
  int day;
  int hour;
  int min;
  int sec;
  int dow;
  int timezone;
  int daylight;
};

void decode_universal_time (lisp, decoded_time *);
lisp decoded_time_to_universal_time (int, int, int, int, int, int, int);
lisp file_time_to_universal_time (const FILETIME &);

#endif
