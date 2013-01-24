#include "HTTP.h"
#include "FileSystem.h"
#include "debug.h"

HRESULT bho::HTTP::Download(LPCSTR szURL, LPTSTR szFileName)
{
	return URLDownloadToFile (0, szURL,  szFileName, 0, 0);
}

HRESULT bho::HTTP::DownloadLow(LPCSTR szURL, LPTSTR szFileName)
{
	Debug::Log("bho::HTTP::DownloadLow");

	PSTR file = PSTR(szFileName);
	char* path = bho::FileSystem::BHOLocalLowFile(file);
	Debug::Log(path);

	HRESULT hr = Download(szURL, _T(path));
	delete(path);
	return hr;
}

HRESULT bho::HTTP::OpenNewTab(const char* url, IWebBrowser2 *pSite)
{
	VARIANT vEmpty = {0};

	VARIANT vFlags = { 0 };
	VariantInit(&vFlags);
	vFlags.vt   = VT_I4;
	vFlags.intVal |= navOpenInNewTab;

	VARIANT vWindow;
	vWindow.bstrVal = (BSTR)("_blank");
	vWindow.vt = VT_BSTR;

	CComBSTR goTo=_T(url);
	HRESULT hr = pSite->Navigate(goTo, &vFlags, &vWindow, &vEmpty, &vEmpty); 
	SysFreeString(goTo);

	return hr;
}