// -*-C++-*-
#ifndef _cdecl_h_
# define _cdecl_h_

# include "targetver.h"

# define NOMINMAX
# define _USE_MATH_DEFINES

# include <windows.h>
# include <winreg.h>
# include <commctrl.h>

# include <algorithm>
# include <array>
# include <climits>
# include <cmath>
# include <cstddef>
# include <cstdint>
# include <cstdio>
# include <cstdlib>
# include <cstring>
# include <limits>
# include <utility>

# include <malloc.h>
# include <mbstring.h>
# include <tchar.h>

# pragma warning (disable: 4510)
# pragma warning (disable: 4514)
# pragma warning (disable: 4610)

# define alloca _alloca

# define BITS_PER_SHORT (sizeof (short) * CHAR_BIT)
# define BITS_PER_INT (sizeof (int) * CHAR_BIT)
# define BITS_PER_LONG (sizeof (long) * CHAR_BIT)

# define PATH_MAX 1024
# define BUFFER_NAME_MAX PATH_MAX

typedef unsigned char u_char;
typedef unsigned short u_short;
typedef unsigned int u_int;
typedef unsigned long u_long;

typedef uintptr_t pointer_t;

typedef uint16_t Char;
# define CHAR_LIMIT 0x10000
typedef u_long lChar;
const lChar lChar_EOF = lChar (-1);

typedef wchar_t ucs2_t;
typedef uint32_t ucs4_t;

typedef long point_t;

using std::min;
using std::max;
using std::swap;

inline int
bcmp (const void *p1, const void *p2, size_t size)
{
  return memcmp (p1, p2, size);
}

inline void *
bzero (void *dst, size_t size)
{
  return memset (dst, 0, size);
}

inline void
bcopy (const Char *src, Char *dst, size_t size)
{
  memcpy (dst, src, sizeof (Char) * size);
}

inline int
bcmp (const Char *p1, const Char *p2, size_t size)
{
  return memcmp (p1, p2, sizeof (Char) * size);
}

template <class T>
inline T *
bfill (T *p0, int start, int end, T x)
{
  for (T *p = p0 + start, *pe = p0 + end; p < pe; p++)
    *p = x;
  return p0;
}

template <class T>
inline T *
bfill (T *p0, int size, T x)
{
  return bfill (p0, 0, size, x);
}

# ifdef DEBUG
int assert_failed (const char *, int);
#  define assert(f) \
  ((void)((f) || assert_failed (__FILE__, __LINE__)))
# else
#  define assert(f) /* empty */
# endif

# ifdef DEBUG
#  define DBG_PRINT(a) (printf a, fflush (stdout))
# else
#  define DBG_PRINT(a) /* empty */
# endif

# define __CONCAT(X, Y) X ## Y
# define CONCAT(X, Y) __CONCAT (X, Y)

# define __CONCAT3(X, Y, Z) X ## Y ## Z
# define CONCAT3(X, Y, Z) __CONCAT3 (X, Y, Z)

# define __TOSTR(X) #X
# define _TOSTR(X) __TOSTR(X)
# define _TOTSTR(X) _T(__TOSTR(X))

# define THREADLOCAL __declspec (thread)

# ifdef UNICODE
#  define _tmemcmp  wmemcmp
#  define _tmemcpy  wmemcpy
#  define _tmemset  wmemset
#  define _tmemmove wmemmove
# else
#  define _tmemcmp  memcmp
#  define _tmemcpy  memcpy
#  define _tmemset  memset
#  define _tmemmove memmove
# endif

#endif
