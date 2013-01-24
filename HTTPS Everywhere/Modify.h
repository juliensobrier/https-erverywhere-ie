#pragma once

#include "Rules.h"

#include <MsHTML.h>
#include <comutil.h>

namespace Page
{
	class Modify
	{
	public:
		static HRESULT UpdateHead(IHTMLDocument2* pHtmlDoc2, HTTPS::Rules &rules);
		static HRESULT UpdateBody(IHTMLDocument2* pHtmlDoc2, HTTPS::Rules &rules);
		static BOOL ShouldModify(IHTMLDocument2* pHtmlDoc2, HTTPS::Rules &rules);

	private:
		static HRESULT DocumentLocation(BSTR &docLocation, IHTMLDocument2* pHtmlDoc2);
		static BOOL IsHTTPS(BSTR location); 
		static HRESULT UpdateDOM(IHTMLDOMNode* spNode, BOOL recursive, HTTPS::Rules &rules);
	};
}