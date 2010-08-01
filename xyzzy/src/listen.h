#ifndef _listen_h_
#define _listen_h_

static const TCHAR xyzzysrv_name[] = _T("xyzzy-server v2");

// wm_private_xyzzysrv
//  wparam: process ID
//  lparam: handle

struct xyzzysrv_param
{
  int size;
  DWORD pid;
  HANDLE hevent;
  HWND hwnd;
  int kill_ok;
  TCHAR data[1];
};

#endif
