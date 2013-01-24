#include "WinInetHook.h"
#include "MTServer.h"

#include <algorithm>


static CWinInetHook * pHook = NULL;
static HTTPS::Rules *pRules = NULL;
static Server* pServer = NULL;

void WinInetInstallHooks(HTTPS::Rules &rules)
{
	if( !pHook )
	{
		pServer = new Server();
		int port = pServer->Start(5000, 15000); // should be configurable
		Debug::Log("Port number used: %i", port);

		pHook = new CWinInetHook(port);
	}

	if (! pRules )
		pRules = new HTTPS::Rules(rules);
	else
		Debug::Log("Rules already loaded");
}

void WinInetRemoveHooks(void)
{
	if( pHook )
	{
		delete pHook;
		pHook = NULL;
	}
}


HINTERNET __stdcall InternetOpenW_Hook(LPCWSTR lpszAgent,DWORD dwAccessType,LPCWSTR lpszProxy,LPCWSTR lpszProxyBypass,DWORD dwFlags)
{
	HINTERNET ret = NULL;
	__try{
		if(pHook)
			ret = pHook->InternetOpenW(lpszAgent, dwAccessType, lpszProxy, lpszProxyBypass, dwFlags);
	}__except(1){}
	return ret;
}

HINTERNET __stdcall InternetOpenA_Hook(LPCSTR lpszAgent,DWORD dwAccessType,LPCSTR lpszProxy,LPCSTR lpszProxyBypass,DWORD dwFlags)
{
	HINTERNET ret = NULL;
	__try{
		if(pHook)
			ret = pHook->InternetOpenA(lpszAgent, dwAccessType, lpszProxy, lpszProxyBypass, dwFlags);
	}__except(1){}
	return ret;
}

BOOL __stdcall InternetCloseHandle_Hook(HINTERNET hInternet)
{
	BOOL ret = FALSE;
	__try{
		if(pHook)
			ret = pHook->InternetCloseHandle(hInternet);
	}__except(1){}
	return ret;
}

INTERNET_STATUS_CALLBACK __stdcall InternetSetStatusCallback_Hook(HINTERNET hInternet, INTERNET_STATUS_CALLBACK lpfnInternetCallback)
{
	INTERNET_STATUS_CALLBACK ret = NULL;
	__try{
		if(pHook)
			ret = pHook->InternetSetStatusCallback(hInternet, lpfnInternetCallback);
	}__except(1){}
	return ret;
}

HINTERNET __stdcall InternetConnectW_Hook(HINTERNET hInternet, LPCWSTR lpszServerName, INTERNET_PORT nServerPort, LPCWSTR lpszUserName, LPCWSTR lpszPassword, DWORD dwService, DWORD dwFlags, DWORD_PTR dwContext)
{
	HINTERNET ret = NULL;
	__try{
		if(pHook)
			ret = pHook->InternetConnectW(hInternet, lpszServerName, nServerPort, lpszUserName, lpszPassword, dwService, dwFlags, dwContext);
	}__except(1){}
	return ret;
}

HINTERNET __stdcall InternetConnectA_Hook(HINTERNET hInternet, LPCSTR lpszServerName, INTERNET_PORT nServerPort, LPCSTR lpszUserName, LPCSTR lpszPassword, DWORD dwService, DWORD dwFlags, DWORD_PTR dwContext)
{
	Debug::Log("InternetConnectA_Hook: %ls", lpszServerName);
	HINTERNET ret = NULL;
	__try{
		if(pHook)
			ret = pHook->InternetConnectA(hInternet, lpszServerName, nServerPort, lpszUserName, lpszPassword, dwService, dwFlags, dwContext);
	}__except(1){}
	return ret;
}

HINTERNET __stdcall HttpOpenRequestW_Hook(HINTERNET hConnect, LPCWSTR lpszVerb, LPCWSTR lpszObjectName, LPCWSTR lpszVersion, LPCWSTR lpszReferrer, LPCWSTR FAR * lplpszAcceptTypes, DWORD dwFlags, DWORD_PTR dwContext)
{
	HINTERNET ret = NULL;
	__try{
		if(pHook)
			ret = pHook->HttpOpenRequestW(hConnect, lpszVerb, lpszObjectName, lpszVersion, lpszReferrer, lplpszAcceptTypes, dwFlags, dwContext);
	}__except(1){}
	return ret;
}

HINTERNET __stdcall HttpOpenRequestA_Hook(HINTERNET hConnect, LPCSTR lpszVerb, LPCSTR lpszObjectName, LPCSTR lpszVersion, LPCSTR lpszReferrer, LPCSTR FAR * lplpszAcceptTypes, DWORD dwFlags, DWORD_PTR dwContext)
{
	HINTERNET ret = NULL;
	//Debug::Log("HttpOpenRequestA_Hook");
	__try{
		if(pHook)
			ret = pHook->HttpOpenRequestA(hConnect, lpszVerb, lpszObjectName, lpszVersion, lpszReferrer, lplpszAcceptTypes, dwFlags, dwContext);
	}__except(1){
		Debug::Error("Error in HttpOpenRequestA_Hook: %i", GetExceptionCode());
	}
	//Debug::Log("HttpOpenRequestA_Hook finished");
	return ret;
}

BOOL __stdcall HttpSendRequestW_Hook(HINTERNET hRequest, LPCWSTR lpszHeaders, DWORD dwHeadersLength, LPVOID lpOptional, DWORD dwOptionalLength)
{
	BOOL ret = FALSE;
	__try{
		if(pHook)
			ret = pHook->HttpSendRequestW(hRequest, lpszHeaders, dwHeadersLength, lpOptional, dwOptionalLength);
	}__except(1){}
	return ret;
}

BOOL __stdcall HttpSendRequestA_Hook(HINTERNET hRequest, LPCSTR lpszHeaders, DWORD dwHeadersLength, LPVOID lpOptional, DWORD dwOptionalLength)
{
	BOOL ret = FALSE;
	__try{
		if(pHook)
			ret = pHook->HttpSendRequestA(hRequest, lpszHeaders, dwHeadersLength, lpOptional, dwOptionalLength);
	}__except(1){}
	return ret;
}

HINTERNET __stdcall FtpOpenFileW_Hook(HINTERNET hConnect, LPCWSTR lpszFileName, DWORD dwAccess, DWORD dwFlags, DWORD_PTR dwContext)
{
	HINTERNET ret = NULL;
	__try{
		if(pHook)
			ret = pHook->FtpOpenFileW(hConnect, lpszFileName, dwAccess, dwFlags, dwContext);
	}__except(1){}
	return ret;
}

HINTERNET __stdcall FtpOpenFileA_Hook(HINTERNET hConnect, LPCSTR lpszFileName, DWORD dwAccess, DWORD dwFlags, DWORD_PTR dwContext)
{
	HINTERNET ret = NULL;
	__try{
		if(pHook)
			ret = pHook->FtpOpenFileA(hConnect, lpszFileName, dwAccess, dwFlags, dwContext);
	}__except(1){}
	return ret;
}

BOOL __stdcall InternetReadFile_Hook(HINTERNET hFile, LPVOID lpBuffer, DWORD dwNumberOfBytesToRead, LPDWORD lpdwNumberOfBytesRead)
{
	BOOL ret = FALSE;
	__try{
		if(pHook)
			ret = pHook->InternetReadFile(hFile, lpBuffer, dwNumberOfBytesToRead, lpdwNumberOfBytesRead);
	}__except(1){}
	return ret;
}

BOOL __stdcall InternetReadFileExW_Hook(HINTERNET hFile, LPINTERNET_BUFFERSW lpBuffersOut, DWORD dwFlags, DWORD_PTR dwContext)
{
	BOOL ret = FALSE;
	__try{
		if(pHook)
			ret = pHook->InternetReadFileExW(hFile, lpBuffersOut, dwFlags, dwContext);
	}__except(1){}
	return ret;
}

BOOL __stdcall InternetReadFileExA_Hook(HINTERNET hFile, LPINTERNET_BUFFERSA lpBuffersOut, DWORD dwFlags, DWORD_PTR dwContext)
{
	BOOL ret = FALSE;
	__try{
		if(pHook)
			ret = pHook->InternetReadFileExA(hFile, lpBuffersOut, dwFlags, dwContext);
	}__except(1){}
	return ret;
}

BOOL __stdcall HttpAddRequestHeadersW_Hook(HINTERNET hRequest, LPCWSTR lpszHeaders, DWORD dwHeadersLength, DWORD dwModifiers)
{
	BOOL ret = FALSE;
	__try{
		if(pHook)
			ret = pHook->HttpAddRequestHeadersW(hRequest, lpszHeaders, dwHeadersLength, dwModifiers);
	}__except(1){}
	return ret;
}

BOOL __stdcall HttpAddRequestHeadersA_Hook(HINTERNET hRequest, LPCSTR lpszHeaders, DWORD dwHeadersLength, DWORD dwModifiers)
{
	BOOL ret = FALSE;
	__try{
		if(pHook)
			ret = pHook->HttpAddRequestHeadersA(hRequest, lpszHeaders, dwHeadersLength, dwModifiers);
	}__except(1){}
	return ret;
}



CWinInetHook::CWinInetHook(int port): hookA(TRUE)
{
	InitializeCriticalSection(&cs);

	this->port = port;

	// initialize the hash tables
	_InternetOpenW = hook.createHookByName("wininet.dll", "InternetOpenW", InternetOpenW_Hook);
	_InternetOpenA = hook.createHookByName("wininet.dll", "InternetOpenA", InternetOpenA_Hook);
	_InternetCloseHandle = hook.createHookByName("wininet.dll", "InternetCloseHandle", InternetCloseHandle_Hook); // used
	_InternetSetStatusCallback = hook.createHookByName("wininet.dll", "InternetSetStatusCallback", InternetSetStatusCallback_Hook);
	_InternetConnectW = hook.createHookByName("wininet.dll", "InternetConnectW", InternetConnectW_Hook); // used
	_InternetConnectA = hook.createHookByName("wininet.dll", "InternetConnectA", InternetConnectA_Hook);
	_HttpOpenRequestW = hook.createHookByName("wininet.dll", "HttpOpenRequestW", HttpOpenRequestW_Hook); // used
	_HttpOpenRequestA = hook.createHookByName("wininet.dll", "HttpOpenRequestA", HttpOpenRequestA_Hook);
	_HttpSendRequestW = hook.createHookByName("wininet.dll", "HttpSendRequestW", HttpSendRequestW_Hook);
	_HttpSendRequestA = hook.createHookByName("wininet.dll", "HttpSendRequestA", HttpSendRequestA_Hook);
	_FtpOpenFileW = hook.createHookByName("wininet.dll", "FtpOpenFileW", FtpOpenFileW_Hook);
	_FtpOpenFileA = hook.createHookByName("wininet.dll", "FtpOpenFileA", FtpOpenFileA_Hook);
	_InternetReadFile = hook.createHookByName("wininet.dll", "InternetReadFile", InternetReadFile_Hook);
	_InternetReadFileExW = hook.createHookByName("wininet.dll", "InternetReadFileExW", InternetReadFileExW_Hook);
	_InternetReadFileExA = hook.createHookByName("wininet.dll", "InternetReadFileExA", InternetReadFileExA_Hook);
	_HttpAddRequestHeadersW = hook.createHookByName("wininet.dll", "HttpAddRequestHeadersW", HttpAddRequestHeadersW_Hook);
	_HttpAddRequestHeadersA = hook.createHookByName("wininet.dll", "HttpAddRequestHeadersA", HttpAddRequestHeadersA_Hook);
}

CWinInetHook::~CWinInetHook(void)
{
	if( pHook == this )
	{
		if (connections.size() > 0)
		{
			Debug::Log("Number of connections still tracked: %i", connections.size());
			connections.clear(); // might need to clean connection better
		}

		if (connectionCounts.size() > 0)
		{
			Debug::Log("Number of connection counts still tracked: %i", connectionCounts.size());
			connectionCounts.clear();
		}

		// Stop server?
		pHook = NULL;
	}
		
	DeleteCriticalSection(&cs);
}

HINTERNET CWinInetHook::InternetOpenW(LPCWSTR lpszAgent,DWORD dwAccessType,LPCWSTR lpszProxy,LPCWSTR lpszProxyBypass,DWORD dwFlags)
{
	HINTERNET ret = NULL;
		
	if( _InternetOpenW )
		ret = _InternetOpenW(lpszAgent, dwAccessType, lpszProxy, lpszProxyBypass, dwFlags);
	
	return ret;
}

HINTERNET CWinInetHook::InternetOpenA(LPCSTR lpszAgent,DWORD dwAccessType,LPCSTR lpszProxy,LPCSTR lpszProxyBypass,DWORD dwFlags)
{
	HINTERNET ret = NULL;

		
	if( _InternetOpenA )
		ret = _InternetOpenA(lpszAgent, dwAccessType, lpszProxy, lpszProxyBypass, dwFlags);
	
	return ret;
}

BOOL CWinInetHook::InternetCloseHandle(HINTERNET hInternet)
{

	EnterCriticalSection(&cs);
	Debug::Log("_InternetCloseHandle"); // seen

	
	// Remove HINTERNET from connections
	std::map< HINTERNET, connectionInfo>::iterator iter;
	iter = connections.find(hInternet);
	if( iter != connections.end() )
	{
		//Debug::Log("_InternetCloseHandle: connection found");
		try
		{
			Debug::Log("CLean count 1");
			if (iter->second.url != NULL)
			{
				std::map<char*, connectionCount, cmp_str>::iterator iter_count;
				iter_count = connectionCounts.find(iter->second.url);

				time_t  current;
				time(&current);

				if( iter_count != connectionCounts.end() )
				{
					Debug::Log("CLean count 2");
					time_t last = iter_count->second.time;

					if (current - last > MAX_DELAY) { // 5s
						Debug::Log("Clean count 3");
						EnterCriticalSection(&cs);
						connectionCounts.erase(iter_count);
						LeaveCriticalSection(&cs);
						Debug::Log("CLean count 4");
					}
				}
				Debug::Log("CLean count - done");
			}

			if (iter->second.https)
			{
				TCHAR tszTemp[10240] = {0};
				DWORD dwSize = sizeof(tszTemp);
				DWORD index = 0;
				BOOL ok = true;

				while (ok && index != ERROR_HTTP_HEADER_NOT_FOUND)
				{
					ok = HttpQueryInfo( hInternet, HTTP_QUERY_SET_COOKIE, tszTemp, &dwSize, &index);

					if (ok && index != ERROR_HTTP_HEADER_NOT_FOUND)
					{
						//Debug::Log("cookie: %s, URL: %s", tszTemp, iter->second.url);
						std::string cookie(tszTemp);
						std::string cookieLc(tszTemp);
						std::transform(cookieLc.begin(), cookieLc.end(), cookieLc.begin(), tolower);


						// Is cookie secure? (lower case)
						size_t pos = cookieLc.find("; secure"); // ; has to be encoded
						if (pos != std::string::npos)
						{
							//Debug::Log("Cookie secure already");
						}
						else
						{
							// get cookie name
							pos = cookieLc.find(";");
							if (pos == std::string::npos)
								pos = cookieLc.length();

							std::string name("");	
							size_t before = cookieLc.find("=");
							if (before != std::string::npos && before  < pos)
							{
								// foo=bar
								name = cookieLc.substr(0, before);
							}
							else {
								// foo
								name = cookieLc.substr(0, pos);
							}

							//Debug::Log("Cookie name: %s", name.c_str());

							// Match on name?
							if (pRules->SecureCookie(iter->second.url, name.c_str()))
							{
								//Debug::Log("Cookie must be secure");
								size_t httponly = cookieLc.find("httponly", pos);
								cookie.append("; Secure");
								DWORD test;

								if (httponly == std::string::npos)
								{
									//cookie.append("; Secure");
									test = InternetSetCookieEx(TEXT(iter->second.url), NULL, TEXT(cookie.c_str()), INTERNET_COOKIE_HTTPONLY, NULL);
								}
								else
								{
									//cookie.replace(httponly, strlen("httponly"), "Secure; HttpOnly");
									test = InternetSetCookieEx(TEXT(iter->second.url), NULL, TEXT(cookie.c_str()), INTERNET_COOKIE_HTTPONLY, NULL);
								}

								//Debug::Log("Cookie added? %i %i", test, GetLastError());
							}
						}
					}
				}

				if (iter->second.connect != NULL && _InternetCloseHandle)
					_InternetCloseHandle(iter->second.connect);

				if (iter->second.connect2 != NULL && _InternetCloseHandle)
					_InternetCloseHandle(iter->second.connect2);

				if (iter->second.url != NULL)
					delete iter->second.url;

				if (iter->second.domain != NULL)
					delete iter->second.domain;
			}


			connections.erase(iter);
		}
		catch(std::exception &e)
		{
			Debug::Error(e.what());
		}
		catch(...)
		{
			Debug::Error("CWininetHook::_InternetCloseHandle SEH error");
		}
	}

	//Debug::Log("Number of connections tracked: %i", connections.size());

	LeaveCriticalSection(&cs);

	if( _InternetCloseHandle )
		return _InternetCloseHandle(hInternet);
	else
		return FALSE;
}

void CALLBACK InternetStatusCallback(HINTERNET hInternet, DWORD_PTR dwContext, DWORD dwInternetStatus, LPVOID lpvStatusInformation, DWORD dwStatusInformationLength)
{
	__try{
		if( pHook )
			pHook->InternetStatusCallback(hInternet, dwContext, dwInternetStatus, lpvStatusInformation, dwStatusInformationLength);
	}__except(1){}
}

INTERNET_STATUS_CALLBACK CWinInetHook::InternetSetStatusCallback(HINTERNET hInternet, INTERNET_STATUS_CALLBACK lpfnInternetCallback)
{
	INTERNET_STATUS_CALLBACK ret = NULL;
	
	if( _InternetSetStatusCallback )
	{
		ret = _InternetSetStatusCallback( hInternet,lpfnInternetCallback);
	}
	
	return ret;
}

void CWinInetHook::InternetStatusCallback(HINTERNET hInternet, DWORD_PTR dwContext, DWORD dwInternetStatus, LPVOID lpvStatusInformation, DWORD dwStatusInformationLength)
{

	// get the original callback
	INTERNET_STATUS_CALLBACK cb = NULL;

	EnterCriticalSection(&cs);
	/*HINTERNET h = hInternet;
	while( !cb && h )
	{
		statusCallbacks.Lookup(h, cb);
		if( !cb )
		{
			HINTERNET parent = NULL;
			parents.Lookup(h, parent);
			h = parent;
		}
	}*/
	LeaveCriticalSection(&cs);
	
	//if( cb )
		cb(hInternet, dwContext, dwInternetStatus, lpvStatusInformation, dwStatusInformationLength);
}

HINTERNET CWinInetHook::InternetConnectW(HINTERNET hInternet, LPCWSTR lpszServerName, INTERNET_PORT nServerPort, LPCWSTR lpszUserName, LPCWSTR lpszPassword, DWORD dwService, DWORD dwFlags, DWORD_PTR dwContext)
{
	Debug::Log("InternetConnectW: %ls", lpszServerName);
	hookA = FALSE;
	HINTERNET hConnect = NULL;
	if( _InternetConnectW )
		hConnect = _InternetConnectW( hInternet, lpszServerName, nServerPort, lpszUserName, lpszPassword, dwService, dwFlags, dwContext);  
	
	RegisterConnection(hConnect, lpszServerName);

	return hConnect;
}

HINTERNET CWinInetHook::InternetConnectA(HINTERNET hInternet, LPCSTR lpszServerName, INTERNET_PORT nServerPort, LPCSTR lpszUserName, LPCSTR lpszPassword, DWORD dwService, DWORD dwFlags, DWORD_PTR dwContext)
{
	Debug::Log("InternetConnectA: %s", lpszServerName);
	HINTERNET hConnect = NULL;

	if( _InternetConnectA )
		hConnect = _InternetConnectA(hInternet, lpszServerName, nServerPort, lpszUserName, lpszPassword, dwService, dwFlags, dwContext);

	if(hookA)
	{
		USES_CONVERSION;
		LPCWSTR wlpszServerName = A2W(lpszServerName);
		RegisterConnection(hConnect, wlpszServerName);
	}

	return hConnect;
}

HINTERNET CWinInetHook::HttpOpenRequestW(HINTERNET hConnect, LPCWSTR lpszVerb, LPCWSTR lpszObjectName, LPCWSTR lpszVersion, LPCWSTR lpszReferrer, LPCWSTR FAR * lplpszAcceptTypes, DWORD dwFlags, DWORD_PTR dwContext)
{
	Debug::Log("HttpOpenRequestW %ls", lpszObjectName);
	LPCWSTR acceptType = NULL;
	if (lplpszAcceptTypes != NULL)
		acceptType = *lplpszAcceptTypes;

	HINTERNET hRequest = ModifyConnection(hConnect, lpszVerb, lpszObjectName, /*lpszVersion, lpszReferrer,*/ acceptType, dwFlags, dwContext, TRUE);
	
	if(_HttpOpenRequestW && ! hRequest)
	{
		Debug::Log("HttpOpenRequestW - Create hrequest to same URL");
		hRequest = _HttpOpenRequestW(hConnect, lpszVerb, lpszObjectName, lpszVersion, lpszReferrer, lplpszAcceptTypes, dwFlags, dwContext);
	}
			
	return hRequest;
}

HINTERNET CWinInetHook::HttpOpenRequestA(HINTERNET hConnect, LPCSTR lpszVerb, LPCSTR lpszObjectName, LPCSTR lpszVersion, LPCSTR lpszReferrer, LPCSTR FAR * lplpszAcceptTypes, DWORD dwFlags, DWORD_PTR dwContext)
{
	Debug::Log("HttpOpenRequestA %s %s", lpszVerb, lpszObjectName);
	HINTERNET hRequest = NULL;
		
	if (hookA &&
		(lplpszAcceptTypes != NULL
			|| strcmp(lpszObjectName, "/redirect") != 0
			|| strcmp(lpszVersion, "HTTP/1.0") != 0
			|| strcmp(lpszReferrer, "https://internal/") != 0
		)
		&& strcmp(lpszVerb, "CONNECT") != 0
		)
	{
		USES_CONVERSION;
		//LPCWSTR wlpszVerb = A2W(lpszVerb);
		//LPCWSTR wlpszObjectName = A2W(lpszObjectName);
		//LPCWSTR wlpszVersion = A2W(lpszVersion);
		//LPCWSTR wlpszReferrer = A2W(lpszReferrer);
		LPCWSTR wlplpszAcceptTypes = NULL;
		if (lplpszAcceptTypes != NULL)
			wlplpszAcceptTypes = A2W(*lplpszAcceptTypes);

		//EnterCriticalSection(&cs);
		hRequest = ModifyConnection(hConnect, A2W(lpszVerb), A2W(lpszObjectName), /*wlpszVersion, wlpszReferrer,*/ wlplpszAcceptTypes, dwFlags, dwContext, FALSE);
		//LeaveCriticalSection(&cs);
	}

	if(_HttpOpenRequestA  && ! hRequest)
	{
		Debug::Log("HttpOpenRequestA - Create hrequest to same URL");
		hRequest = _HttpOpenRequestA(hConnect, lpszVerb, lpszObjectName, lpszVersion, lpszReferrer, lplpszAcceptTypes, dwFlags, dwContext);
		Debug::Log("HttpOpenRequestA - done");
	}
	
	return hRequest;
}

BOOL CWinInetHook::HttpSendRequestW(HINTERNET hRequest, LPCWSTR lpszHeaders, DWORD dwHeadersLength, LPVOID lpOptional, DWORD dwOptionalLength)
{
	BOOL ret = FALSE;
	
	if( _HttpSendRequestW )
		ret = _HttpSendRequestW(hRequest, lpszHeaders, dwHeadersLength, lpOptional, dwOptionalLength);

	return ret;
}

BOOL CWinInetHook::HttpSendRequestA(HINTERNET hRequest, LPCSTR lpszHeaders, DWORD dwHeadersLength, LPVOID lpOptional, DWORD dwOptionalLength)
{
	BOOL ret = FALSE;
	
	if( _HttpSendRequestA )
		ret = _HttpSendRequestA(hRequest, lpszHeaders, dwHeadersLength, lpOptional, dwOptionalLength);

	return ret;
}

HINTERNET CWinInetHook::FtpOpenFileW(HINTERNET hConnect, LPCWSTR lpszFileName, DWORD dwAccess, DWORD dwFlags, DWORD_PTR dwContext)
{
	HINTERNET ret = NULL;

	if( _FtpOpenFileW )
		ret = _FtpOpenFileW(hConnect, lpszFileName, dwAccess, dwFlags, dwContext);
	
	return ret;
}

HINTERNET CWinInetHook::FtpOpenFileA(HINTERNET hConnect, LPCSTR lpszFileName, DWORD dwAccess, DWORD dwFlags, DWORD_PTR dwContext)
{
	HINTERNET ret = NULL;

	if( _FtpOpenFileA )
		ret = _FtpOpenFileA(hConnect, lpszFileName, dwAccess, dwFlags, dwContext);
	
	return ret;
}

BOOL CWinInetHook::InternetReadFile(HINTERNET hFile, LPVOID lpBuffer, DWORD dwNumberOfBytesToRead, LPDWORD lpdwNumberOfBytesRead)
{
	BOOL ret = FALSE;

	if( _InternetReadFile )
		ret = _InternetReadFile(hFile, lpBuffer, dwNumberOfBytesToRead, lpdwNumberOfBytesRead);

	return ret;
}

BOOL CWinInetHook::InternetReadFileExW(HINTERNET hFile, LPINTERNET_BUFFERSW lpBuffersOut, DWORD dwFlags, DWORD_PTR dwContext)
{
	BOOL ret = FALSE;

	if( _InternetReadFileExW )
		ret = _InternetReadFileExW(hFile, lpBuffersOut, dwFlags, dwContext);
	
	return ret;
}

BOOL CWinInetHook::InternetReadFileExA(HINTERNET hFile, LPINTERNET_BUFFERSA lpBuffersOut, DWORD dwFlags, DWORD_PTR dwContext)
{
	BOOL ret = FALSE;

	if( _InternetReadFileExA )
		ret = _InternetReadFileExA(hFile, lpBuffersOut, dwFlags, dwContext);
	
	return ret;
}

BOOL CWinInetHook::HttpAddRequestHeadersW(HINTERNET hRequest, LPCWSTR lpszHeaders, DWORD dwHeadersLength, DWORD dwModifiers)
{
	BOOL ret = FALSE;

	if( _HttpAddRequestHeadersW )
		ret = _HttpAddRequestHeadersW(hRequest, lpszHeaders, dwHeadersLength, dwModifiers);

	return ret;
}

BOOL CWinInetHook::HttpAddRequestHeadersA(HINTERNET hRequest, LPCSTR lpszHeaders, DWORD dwHeadersLength, DWORD dwModifiers)
{
	BOOL ret = FALSE;

	if( _HttpAddRequestHeadersA )
		ret = _HttpAddRequestHeadersA(hRequest, lpszHeaders, dwHeadersLength, dwModifiers);

	return ret;
}

void CWinInetHook::RegisterConnection(HINTERNET hConnect, LPCWSTR lpszServerName)
{
	Debug::Log("RegisterConnection: %ls", lpszServerName);
	
	if( hConnect )
	{
		try
		{
			// Do not track it if there is no host match
			int length = WideCharToMultiByte(CP_ACP, 0, lpszServerName, -1, 0, 0, NULL, NULL);
			char* host = new char[length];
			WideCharToMultiByte(CP_ACP, 0, lpszServerName, -1, host , length, NULL, NULL);

			if (pRules->HostMatch(host))
			{
				EnterCriticalSection(&cs);
				//LPWSTR domain = NULL;
				//wcscpy(domain, lpszServerName);
				const connectionInfo connection = { host, false, NULL, NULL, NULL };
				//const connectionInfo connection = { host, false, NULL, NULL, NULL };

				connections.insert(std::pair<HINTERNET, connectionInfo>(hConnect, connection));
				LeaveCriticalSection(&cs);
			}
			else if (host != NULL)
				delete host;
		}
		catch(std::exception &e)
		{
			Debug::Error(e.what());
		}
		catch(...)
		{
			Debug::Error("CWininetHook::RegisterConnection SEH error");
		}
	}
	else
		Debug::Warn("RegisterConnection: hConnect is NULL");
	
	
}

HINTERNET CWinInetHook::ModifyConnection(HINTERNET hConnect, LPCWSTR lpszVerb, LPCWSTR lpszObjectName, /*LPCWSTR lpszVersion, LPCWSTR lpszReferrer,*/ LPCWSTR lpszAcceptTypes, DWORD dwFlags, DWORD_PTR dwContext, BOOL w)
{
	HINTERNET hRequest = NULL;

	try
	{
		Debug::Log("ModifyConnection: %ls %ls", lpszVerb, lpszObjectName); // seen
		BOOL https = ((dwFlags & INTERNET_FLAG_SECURE) == INTERNET_FLAG_SECURE);  


		EnterCriticalSection(&cs);

		std::map< HINTERNET, connectionInfo>::iterator iter;
		iter = connections.find(hConnect);

		LeaveCriticalSection(&cs);

		if( iter != connections.end() )
		{
			Debug::Log("ModifyConnection: found connection");
			connectionInfo connection = iter->second;
			connection.https = https;
			connection.url = NULL;
			connection.connect = NULL;
			connection.connect2 = NULL;


			/*size_t converted = 0;
			int size = wcslen(iter->second.domain) + 1;
			char* domain = new char[size];
			memset(domain, 0x0, size);
			wcstombs_s(&converted, domain, size, iter->second.domain, _TRUNCATE);
			Debug::Log("Domain: %s (from %ls)", domain, iter->second.domain);*/

			size_t converted = 0;
			int size = wcslen(lpszObjectName) + 1;
			char* path = new char[size];
			memset(path, 0x0, size);
			wcstombs_s(&converted, path, size, lpszObjectName, _TRUNCATE);
			//Debug::Log("Path: %s", path);

			size = strlen("http://") + strlen(iter->second.domain) + strlen(path) + 1;
			if (https)
				size += 1;

			char* url = new char[size + 1];
			memset(url, 0x0, size + 1);
			strcpy_s(url, size, "http");
			if (https)
				strcat_s(url, size, "s");
			strcat_s(url, size, "://");
			strcat_s(url, size, iter->second.domain);
			strcat_s(url, size, path);
			Debug::Log("URL: %s", url);

			//delete domain;
			delete path;

			connection.url = url;

			std::map<char*, connectionCount, cmp_str>::iterator iter_count;
			iter_count = connectionCounts.find(url);

			time_t  current;
			time(&current);
			int count = 1;

			if( iter_count != connectionCounts.end() )
			{
				Debug::Log("Found previous count for %s", url);
				count += iter_count->second.count;
				time_t last = iter_count->second.time;

				if (current - last > MAX_DELAY) { // 5s
					count = 1;
				}

				EnterCriticalSection(&cs);
				connectionCounts.erase(iter_count);
				LeaveCriticalSection(&cs);
			}
			else
				Debug::Log("No count info for %s", url);
			
			const connectionCount infoCount = { count, current };
			Debug::Log("%s - %i", url, count);
			
			if (count > MAX_COUNT) {
				Debug::Warn("More than %i requests to %s within %i seconds, skip", MAX_COUNT, url, MAX_DELAY);
				return NULL;
			}


			char *newUrl = pRules->Transform(url);
			if (newUrl != NULL)
			{
				Debug::Log("New URL: %s (%i)", newUrl, strlen(newUrl));

				EnterCriticalSection(&cs);
				connectionCounts.insert(std::pair<char*, connectionCount>(url, infoCount));
				LeaveCriticalSection(&cs);


				int size = strlen(newUrl) + 20;
				char *header = new char[size];
				strcpy_s(header, size, "X-Redirect: ");
				strcat_s(header, size, newUrl);
				strcat_s(header, size, "\r\n");
				Debug::Log("Wininet redirection to %s", newUrl);
				//Debug::Log("%s", header);
				

				DWORD dwOpenRequestFlags = 
					dwFlags |
					//INTERNET_FLAG_IGNORE_REDIRECT_TO_HTTP |
					INTERNET_FLAG_IGNORE_REDIRECT_TO_HTTPS |
					INTERNET_FLAG_KEEP_CONNECTION |
					//INTERNET_FLAG_NO_AUTO_REDIRECT | // => works for google.org image, but not 123systems.net main URL
					INTERNET_FLAG_NO_COOKIES |
					INTERNET_FLAG_NO_CACHE_WRITE |
					INTERNET_FLAG_NO_UI |
					INTERNET_FLAG_RELOAD;


				if (lpszAcceptTypes != NULL)
				{
					Debug::Log("%ls (%s)", lpszAcceptTypes, url);
					// text/html: length = 9

					if (wcscmp(lpszAcceptTypes, L"text/html") == 0 // this is (most likely?) the main TAB URL (IE 9 + 10)
						|| wcscmp(lpszAcceptTypes, L"image/gif") == 0 // IE 6 & 7, check for IE version
						|| wcscmp(lpszAcceptTypes, L"image/jpeg") == 0 // IE 8, check for IE version
						)
					{
						Debug::Log("Add WININET flag for text/html");
						dwOpenRequestFlags = dwOpenRequestFlags | INTERNET_FLAG_NO_AUTO_REDIRECT;
					}
				}
				else
					Debug::Log("Content type is null: %s", url);


				HINTERNET initialize = InternetOpen("HTTPSEVERYWHERE",INTERNET_OPEN_TYPE_DIRECT,NULL,NULL,0);
				if (initialize == NULL)
				{
					Debug::Error("initialize is NULL");
					Debug::Log("Last error: %i", GetLastError());
				}
				/*HINTERNET connect = InternetConnect(initialize,"127.0.0.1",5000,NULL,NULL,INTERNET_SERVICE_HTTP,0,0);
				//hRequest = HttpOpenRequest(connect,"GET","/","HTTP/1.0",NULL, NULL,dwOpenRequestFlags, 0);
				hRequest = HttpOpenRequest(connect,"GET","/","HTTP/1.0",NULL, NULL,dwOpenRequestFlags, dwContext);*/

				HINTERNET connect = NULL;
				if (w == TRUE && _InternetConnectW)
					connect = _InternetConnectW(initialize,L"127.0.0.1",(INTERNET_PORT)pHook->port,NULL,NULL,INTERNET_SERVICE_HTTP,0,0);
				else if(w == FALSE && _InternetConnectA)
					connect = _InternetConnectA(initialize,"127.0.0.1",(INTERNET_PORT)pHook->port,NULL,NULL,INTERNET_SERVICE_HTTP,0,0);

				if (connect == NULL)
				{
					Debug::Error("connect is NULL");
					return NULL;
				}


				if(w == TRUE && _HttpOpenRequestW )
					hRequest = _HttpOpenRequestW(connect,L"GET",L"/redirect",L"HTTP/1.0",NULL, NULL,dwOpenRequestFlags, dwContext);
				else if(w == FALSE && _HttpOpenRequestA)
					hRequest = _HttpOpenRequestA(connect,"GET","/redirect","HTTP/1.0","https://internal/", NULL,dwOpenRequestFlags, dwContext);

				if (hRequest == NULL)
				{
					Debug::Error("hrequest is NULL");
					return NULL;
				}
				Debug::Log("ModifyConnection: hrequest is good");

				BOOL result = FALSE;
				if (_HttpAddRequestHeadersW || _HttpAddRequestHeadersA)
					result = HttpAddRequestHeaders(hRequest, header, strlen(header), HTTP_ADDREQ_FLAG_ADD);

				if (hRequest == NULL)
				{
					Debug::Error("hrquest is NULL (2)");
					return NULL;
				}

				if (result == FALSE)
				{
					Debug::Error("HttpAddRequestHeaders failed");
					return NULL;
				}
				Debug::Log("ModifyConnection: HttpAddRequestHeaders is good");

				connection.connect = connect;
				connection.connect2 = initialize;

				delete newUrl;
				Debug::Log("ModifyConnection: delete 1");
				delete header;
				Debug::Log("ModifyConnection: delete 2");
			}
			else
			{
				Debug::Log("URL is safe");
			}

			EnterCriticalSection(&cs);

			connections.erase(iter);
			Debug::Log("ModifyConnection: erase done");
			connections.insert(std::pair<HINTERNET, connectionInfo>(hRequest, connection));
			Debug::Log("ModifyConnection: insert done");

			LeaveCriticalSection(&cs);
		}
		else
		{
			Debug::Log("Connection not found in cache");
		}
	}
	catch(std::exception &e)
	{
		Debug::Error(e.what());
	}
	catch(...)
	{
		Debug::Error("CWininetHook::ModifyConnection SEH error");
	}

	return hRequest;
}