#include "common.h"

#include <urlmon.h>
#pragma comment(lib, "urlmon.lib")

#include <Exdisp.h>

namespace bho
{
	class HTTP
	{
	public:
		static HRESULT Download(LPCSTR szURL, LPTSTR szFileName);
		static HRESULT DownloadLow(LPCSTR szURL, LPTSTR szFileName);
		static HRESULT OpenNewTab(const char* url, IWebBrowser2 *pSite);
	};
}