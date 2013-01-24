#pragma once

#include "Rules.h"

#include <WinInet.h>  
#pragma comment( lib, "WinInet.lib")  

#include "ncodehook/NCodeHookInstantiation.h"


/* Function Prototypes */
typedef HINTERNET(__stdcall * LPINTERNETOPENW)(LPCWSTR lpszAgent,DWORD dwAccessType,LPCWSTR lpszProxy,LPCWSTR lpszProxyBypass,DWORD dwFlags);
typedef HINTERNET(__stdcall * LPINTERNETOPENA)(LPCSTR lpszAgent,DWORD dwAccessType,LPCSTR lpszProxy,LPCSTR lpszProxyBypass,DWORD dwFlags);
typedef BOOL(__stdcall * LPINTERNETCLOSEHANDLE)(HINTERNET hInternet);
typedef INTERNET_STATUS_CALLBACK(__stdcall * LPINTERNETSETSTATUSCALLBACK)(HINTERNET hInternet, INTERNET_STATUS_CALLBACK lpfnInternetCallback);
typedef HINTERNET(__stdcall * LPINTERNETCONNECTW)(HINTERNET hInternet, LPCWSTR lpszServerName, INTERNET_PORT nServerPort, LPCWSTR lpszUserName, LPCWSTR lpszPassword, DWORD dwService, DWORD dwFlags, DWORD_PTR dwContext);
typedef HINTERNET(__stdcall * LPINTERNETCONNECTA)(HINTERNET hInternet, LPCSTR lpszServerName, INTERNET_PORT nServerPort, LPCSTR lpszUserName, LPCSTR lpszPassword, DWORD dwService, DWORD dwFlags, DWORD_PTR dwContext);
typedef HINTERNET(__stdcall * LPHTTPOPENREQUESTW)(HINTERNET hConnect, LPCWSTR lpszVerb, LPCWSTR lpszObjectName, LPCWSTR lpszVersion, LPCWSTR lpszReferrer, LPCWSTR FAR * lplpszAcceptTypes, DWORD dwFlags, DWORD_PTR dwContext);
typedef HINTERNET(__stdcall * LPHTTPOPENREQUESTA)(HINTERNET hConnect, LPCSTR lpszVerb, LPCSTR lpszObjectName, LPCSTR lpszVersion, LPCSTR lpszReferrer, LPCSTR FAR * lplpszAcceptTypes, DWORD dwFlags, DWORD_PTR dwContext);
typedef BOOL(__stdcall * LPHTTPSENDREQUESTW)(HINTERNET hRequest, LPCWSTR lpszHeaders, DWORD dwHeadersLength, LPVOID lpOptional, DWORD dwOptionalLength);
typedef BOOL(__stdcall * LPHTTPSENDREQUESTA)(HINTERNET hRequest, LPCSTR lpszHeaders, DWORD dwHeadersLength, LPVOID lpOptional, DWORD dwOptionalLength);
typedef HINTERNET(__stdcall * LPFTPOPENFILEW)(HINTERNET hConnect, LPCWSTR lpszFileName, DWORD dwAccess, DWORD dwFlags, DWORD_PTR dwContext);
typedef HINTERNET(__stdcall * LPFTPOPENFILEA)(HINTERNET hConnect, LPCSTR lpszFileName, DWORD dwAccess, DWORD dwFlags, DWORD_PTR dwContext);
typedef BOOL(__stdcall * LPINTERNETREADFILE)(HINTERNET hFile, LPVOID lpBuffer, DWORD dwNumberOfBytesToRead, LPDWORD lpdwNumberOfBytesRead);
typedef BOOL(__stdcall * LPINTERNETREADFILEEXW)(HINTERNET hFile, LPINTERNET_BUFFERSW lpBuffersOut, DWORD dwFlags, DWORD_PTR dwContext);
typedef BOOL(__stdcall * LPINTERNETREADFILEEXA)(HINTERNET hFile, LPINTERNET_BUFFERSA lpBuffersOut, DWORD dwFlags, DWORD_PTR dwContext);
typedef BOOL(__stdcall * LPHTTPADDREQUESTHEADERSW)(HINTERNET hRequest, LPCWSTR lpszHeaders, DWORD dwHeadersLength, DWORD dwModifiers);
typedef BOOL(__stdcall * LPHTTPADDREQUESTHEADERSA)(HINTERNET hRequest, LPCSTR lpszHeaders, DWORD dwHeadersLength, DWORD dwModifiers);


void WinInetInstallHooks(HTTPS::Rules &rules);
void WinInetRemoveHooks(void);

typedef struct _connectionInfo
{
	char* domain;
	BOOL https;
	char* url;
	HINTERNET connect;
	HINTERNET connect2;
} connectionInfo;

typedef struct _connectionCount
{
	int count;
	time_t  time;
} connectionCount;

struct cmp_str
{
   bool operator()(char const *a, char const *b)
   {
      return std::strcmp(a, b) < 0;
   }
};

const int MAX_COUNT = 5;
const int MAX_DELAY = 5;
    
class CWinInetHook
{
public:
	CWinInetHook(int port);
	virtual ~CWinInetHook(void);
	void InstallHooks(void);

	HINTERNET	InternetOpenW(LPCWSTR lpszAgent,DWORD dwAccessType,LPCWSTR lpszProxy,LPCWSTR lpszProxyBypass,DWORD dwFlags);
	HINTERNET	InternetOpenA(LPCSTR lpszAgent,DWORD dwAccessType,LPCSTR lpszProxy,LPCSTR lpszProxyBypass,DWORD dwFlags);
	BOOL		InternetCloseHandle(HINTERNET hInternet);
	INTERNET_STATUS_CALLBACK	InternetSetStatusCallback(HINTERNET hInternet, INTERNET_STATUS_CALLBACK lpfnInternetCallback);
	void		InternetStatusCallback(HINTERNET hInternet, DWORD_PTR dwContext, DWORD dwInternetStatus, LPVOID lpvStatusInformation, DWORD dwStatusInformationLength);
	HINTERNET	InternetConnectW(HINTERNET hInternet, LPCWSTR lpszServerName, INTERNET_PORT nServerPort, LPCWSTR lpszUserName, LPCWSTR lpszPassword, DWORD dwService, DWORD dwFlags, DWORD_PTR dwContext);
	HINTERNET	InternetConnectA(HINTERNET hInternet, LPCSTR lpszServerName, INTERNET_PORT nServerPort, LPCSTR lpszUserName, LPCSTR lpszPassword, DWORD dwService, DWORD dwFlags, DWORD_PTR dwContext);
	HINTERNET	HttpOpenRequestW(HINTERNET hConnect, LPCWSTR lpszVerb, LPCWSTR lpszObjectName, LPCWSTR lpszVersion, LPCWSTR lpszReferrer, LPCWSTR FAR * lplpszAcceptTypes, DWORD dwFlags, DWORD_PTR dwContext);
	HINTERNET	HttpOpenRequestA(HINTERNET hConnect, LPCSTR lpszVerb, LPCSTR lpszObjectName, LPCSTR lpszVersion, LPCSTR lpszReferrer, LPCSTR FAR * lplpszAcceptTypes, DWORD dwFlags, DWORD_PTR dwContext);
	BOOL		HttpSendRequestW(HINTERNET hRequest, LPCWSTR lpszHeaders, DWORD dwHeadersLength, LPVOID lpOptional, DWORD dwOptionalLength);
	BOOL		HttpSendRequestA(HINTERNET hRequest, LPCSTR lpszHeaders, DWORD dwHeadersLength, LPVOID lpOptional, DWORD dwOptionalLength);
	HINTERNET	FtpOpenFileW(HINTERNET hConnect, LPCWSTR lpszFileName, DWORD dwAccess, DWORD dwFlags, DWORD_PTR dwContext);
	HINTERNET	FtpOpenFileA(HINTERNET hConnect, LPCSTR lpszFileName, DWORD dwAccess, DWORD dwFlags, DWORD_PTR dwContext);
	BOOL		InternetReadFile(HINTERNET hFile, LPVOID lpBuffer, DWORD dwNumberOfBytesToRead, LPDWORD lpdwNumberOfBytesRead);
	BOOL		InternetReadFileExW(HINTERNET hFile, LPINTERNET_BUFFERSW lpBuffersOut, DWORD dwFlags, DWORD_PTR dwContext);
	BOOL		InternetReadFileExA(HINTERNET hFile, LPINTERNET_BUFFERSA lpBuffersOut, DWORD dwFlags, DWORD_PTR dwContext);
	BOOL		HttpAddRequestHeadersW(HINTERNET hRequest, LPCWSTR lpszHeaders, DWORD dwHeadersLength, DWORD dwModifiers);
	BOOL		HttpAddRequestHeadersA(HINTERNET hRequest, LPCSTR lpszHeaders, DWORD dwHeadersLength, DWORD dwModifiers);
	
private:
	int port;
	std::map<HINTERNET, connectionInfo> connections;
	std::map<char*, connectionCount, cmp_str> connectionCounts;
	CRITICAL_SECTION	cs;
	BOOL hookA;

#ifdef _WIN64
	NCodeHookX64	hook;
#else
	NCodeHookIA32	hook;
#endif

	LPINTERNETOPENW				_InternetOpenW;
	LPINTERNETOPENA				_InternetOpenA;
	LPINTERNETCLOSEHANDLE		_InternetCloseHandle;
	LPINTERNETSETSTATUSCALLBACK	_InternetSetStatusCallback;
	LPINTERNETCONNECTW			_InternetConnectW;
	LPINTERNETCONNECTA			_InternetConnectA;
	LPHTTPOPENREQUESTW			_HttpOpenRequestW;
	LPHTTPOPENREQUESTA			_HttpOpenRequestA;
	LPHTTPSENDREQUESTW			_HttpSendRequestW;
	LPHTTPSENDREQUESTA			_HttpSendRequestA;
	LPFTPOPENFILEW				_FtpOpenFileW;
	LPFTPOPENFILEA				_FtpOpenFileA;
	LPINTERNETREADFILE			_InternetReadFile;
	LPINTERNETREADFILEEXW		_InternetReadFileExW;
	LPINTERNETREADFILEEXA		_InternetReadFileExA;
	LPHTTPADDREQUESTHEADERSW	_HttpAddRequestHeadersW;
	LPHTTPADDREQUESTHEADERSA	_HttpAddRequestHeadersA;

	void RegisterConnection(HINTERNET hConnect, LPCWSTR lpszServerName);
	HINTERNET ModifyConnection(HINTERNET hConnect, LPCWSTR lpszVerb, LPCWSTR lpszObjectName, /*LPCWSTR lpszVersion, LPCWSTR lpszReferrer,*/ LPCWSTR lplpszAcceptTypes, DWORD dwFlags, DWORD_PTR dwContext, BOOL w);
};
