#ifndef _vfs_h_
#define _vfs_h_

class WINFS
{
protected:
  typedef BOOL (WINAPI *GETDISKFREESPACEEX)(LPCTSTR, PULARGE_INTEGER,
                                            PULARGE_INTEGER, PULARGE_INTEGER);
  static const GETDISKFREESPACEEX GetDiskFreeSpaceEx;

  static DWORD WINAPI internal_GetFullPathName (LPCTSTR lpFileName, DWORD nBufferLength,
                                                LPTSTR lpBuffer, LPTSTR *lpFilePart);
  static DWORD WINAPI internal_GetFileAttributes (LPCTSTR lpFileName);
public:
  static TCHAR wfs_share_cache[MAX_PATH * 2];

  static void clear_share_cache () {*wfs_share_cache = 0;}

  static BOOL WINAPI CreateDirectory (LPCTSTR lpPathName, LPSECURITY_ATTRIBUTES lpSecurityAttributes);
  static HANDLE WINAPI CreateFile (LPCTSTR lpFileName, DWORD dwDesiredAccess, DWORD dwShareMode,
                                   LPSECURITY_ATTRIBUTES lpSecurityAttributes, DWORD dwCreationDisposition,
                                   DWORD dwFlagsAndAttributes, HANDLE hTemplateFile);
  static BOOL WINAPI DeleteFile (LPCTSTR lpFileName);
  static HANDLE WINAPI FindFirstFile (LPCTSTR lpFileName, LPWIN32_FIND_DATA lpFindFileData);
  static BOOL WINAPI FindNextFile (HANDLE hFindFile, LPWIN32_FIND_DATA lpFindFileData);
  static BOOL WINAPI GetDiskFreeSpace (LPCTSTR lpRootPathName, LPDWORD lpSectorsPerCluster,
                                       LPDWORD lpBytesPerSector, LPDWORD lpNumberOfFreeClusters,
                                       LPDWORD lpTotalNumberOfClusters);
  static DWORD WINAPI GetFileAttributes (LPCTSTR lpFileName);
  static DWORD WINAPI GetFullPathName (LPCTSTR lpFileName, DWORD nBufferLength, LPTSTR lpBuffer, LPTSTR *lpFilePart);
  static UINT WINAPI GetTempFileName (LPCTSTR lpPathName, LPCTSTR lpPrefixString, UINT uUnique, LPTSTR lpTempFileName);
  static BOOL WINAPI GetVolumeInformation (LPCTSTR lpRootPathName, LPTSTR lpVolumeNameBuffer,
                                           DWORD nVolumeNameSize, LPDWORD lpVolumeSerialNumber,
                                           LPDWORD lpMaximumComponentLength, LPDWORD lpFileSystemFlags,
                                           LPTSTR lpFileSystemNameBuffer, DWORD nFileSystemNameSize);
  static HMODULE WINAPI LoadLibrary (LPCTSTR lpLibFileName);
  static BOOL WINAPI MoveFile (LPCTSTR lpExistingFileName, LPCTSTR lpNewFileName);
  static BOOL WINAPI RemoveDirectory (LPCTSTR lpPathName);
  static BOOL WINAPI SetCurrentDirectory (LPCTSTR lpPathName);
  static BOOL WINAPI SetFileAttributes (LPCTSTR lpFileName, DWORD dwFileAttributes);
  static DWORD WINAPI WNetOpenEnum (DWORD dwScope, DWORD dwType, DWORD dwUsage,
                                    LPNETRESOURCE lpNetResource, LPHANDLE lphEnum);

  static int WINAPI get_file_data (const TCHAR *, WIN32_FIND_DATA &);
};

#endif /* _vfs_h_ */
