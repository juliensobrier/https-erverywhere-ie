#ifndef __COMMON_H__
#define __COMMON_H__

//#define SUPPORT_64BIT_OFFSET
//#define _AFXDLL

#define _BIND_TO_CURRENT_CRT_VERSION 1
#define _CRT_SECURE_CPP_OVERLOAD_SECURE_NAMES 1
#define _CRT_SECURE_NO_WARNINGS 1

#ifndef VC_EXTRALEAN
#define VC_EXTRALEAN            // Exclude rarely-used stuff from Windows headers
#endif

// turns off MFC's hiding of some common and often safely ignored warning messages
#define _AFX_ALL_WARNINGS

#include "targetver.h"


#include <afxcoll.h>
#include <AFX.H>
#include "afxwin.h"

/*#define _WIN32_WINNT _WIN32_WINNT_MAXVER
#include "targetver.h"*/

//#include <windows.h>
#include <tchar.h>
#include <assert.h>



// Our main CLSID in string format
#define CLSID_IEPlugin_Str _T("{44D1BC7D-F859-4CB3-9E1D-D1ED52181916}")
#define GUID_IE_Str _T("44D1BC7D-F859-4CB3-9E1D-D1ED52181916")
const CLSID CLSID_IEPlugin = { 0x44D1BC7D, 0xF859, 0x4CB3, { 0x9E, 0x1D, 0xD1, 0xED, 0x52, 0x18, 0x19, 0x16 } }; // The CLSID in binary format

#define BHO_NAME _T("HTTPS Everywhere")
#define BHO_FILENAME _T("HTTPSEverywhere")
#ifdef _WIN64
#define BHO_FILENAME_DLL _T("HTTPSEverywhere64.dll")
#else
#define BHO_FILENAME_DLL _T("HTTPSEverywhere.dll")
#endif
#define BHO_UPDATE _T("https://www.zscaler.com/research/plugins/ie/https-everywhere/version.xml")


extern const CLSID CLSID_IEPlugin;
extern volatile LONG DllRefCount;
extern HINSTANCE hInstance;

#endif // __COMMON_H__
