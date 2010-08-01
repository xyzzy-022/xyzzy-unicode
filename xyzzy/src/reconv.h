#ifndef _reconv_h_
#define _reconv_h_

#ifndef WM_IME_REQUEST
#define WM_IME_REQUEST 0x0288
#endif

#ifndef IMR_RECONVERTSTRING
#define IMR_RECONVERTSTRING 4

typedef struct tagRECONVERTSTRING
{
  DWORD dwSize;
  DWORD dwVersion;
  DWORD dwStrLen;
  DWORD dwStrOffset;
  DWORD dwCompStrLen;
  DWORD dwCompStrOffset;
  DWORD dwTargetStrLen;
  DWORD dwTargetStrOffset;
}
  RECONVERTSTRING;

#endif

#define WM_MSIME_RECONVERTW L"MSIMEReconvert"
#define WM_ATOK_RECONVERTW L"Atok Message for ReconvertString"
#define WM_MSIME_RECONVERTA "MSIMEReconvert"
#define WM_ATOK_RECONVERTA "Atok Message for ReconvertString"
#ifdef UNICODE
#define WM_MSIME_RECONVERT WM_MSIME_RECONVERTW
#define WM_ATOK_RECONVERT WM_ATOK_RECONVERTW
#else
#define WM_MSIME_RECONVERT WM_MSIME_RECONVERTA
#define WM_ATOK_RECONVERT WM_ATOK_RECONVERTA
#endif

#endif /* _reconv_h_ */
