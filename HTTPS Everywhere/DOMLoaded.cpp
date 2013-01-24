#include "DOMLoaded.h"
#include "debug.h"

Page::DOMLoaded::DOMLoaded(IWebBrowser2* inRootFrame, HTTPS::Rules &aRules)
{
	webBrowser2 = inRootFrame;
	rules = aRules;
}

// IUnknown
HRESULT STDMETHODCALLTYPE Page::DOMLoaded::GetTypeInfo(UINT iTInfo, LCID  lcid, ITypeInfo **ppTInfo)
{
	UNREFERENCED_PARAMETER(iTInfo); 
	UNREFERENCED_PARAMETER(lcid); 
	UNREFERENCED_PARAMETER(ppTInfo); 
	
	return E_NOTIMPL;
}

HRESULT STDMETHODCALLTYPE Page::DOMLoaded::GetIDsOfNames(REFIID riid, LPOLESTR *rgszNames, UINT cNames, LCID lcid, DISPID *rgDispId)
{ 
	UNREFERENCED_PARAMETER(riid); 
	UNREFERENCED_PARAMETER(rgszNames); 
	UNREFERENCED_PARAMETER(cNames); 
	UNREFERENCED_PARAMETER(lcid); 
	UNREFERENCED_PARAMETER(rgDispId); 

	return E_NOTIMPL; 
}




HRESULT STDMETHODCALLTYPE Page::DOMLoaded::Invoke(DISPID dispIdMember, REFIID riid, LCID lcid, WORD wFlags,
	DISPPARAMS *pDispParams, VARIANT *pVarResult, EXCEPINFO *pExcepInfo, UINT *puArgErr)
{
	UNREFERENCED_PARAMETER(pExcepInfo);
	UNREFERENCED_PARAMETER(pDispParams);
	UNREFERENCED_PARAMETER(wFlags);
	UNREFERENCED_PARAMETER(dispIdMember);
	UNREFERENCED_PARAMETER(lcid);


	UINT			uArgErr;
	VARIANT		varResultDummy;

	Debug::Log("Page::DOMLoaded::Invoke");

	if(webBrowser2 == NULL)
	{
		Debug::Error("Page::DOMLoaded::Invoke: webbrowser is NULL");
		return E_FAIL;
	}

	CComPtr<IDispatch> pUnk = NULL;
	HRESULT hr = webBrowser2->get_Document(&pUnk);
	if (FAILED(hr) || pUnk == NULL)
	{
		Debug::Warn("get_Document failed");
		return E_FAIL;
	}

	CComQIPtr<IHTMLDocument2> pHtmlDoc2(pUnk);
	if (! pHtmlDoc2)
	{
		Debug::Warn("Could not retrieve IHTMLDocument2");
		return E_FAIL;
	}


	// Should we modify this page?
	if (Page::Modify::ShouldModify(pHtmlDoc2, rules) == FALSE)
	{
		Debug::Log("Do not modify document");
		return E_FAIL;
	}
	
	// Modify Head
	hr = Page::Modify::UpdateHead(pHtmlDoc2, rules);

	// Modify body if available
	hr = Page::Modify::UpdateBody(pHtmlDoc2, rules);


	if (riid != IID_NULL) {
		return ResultFromScode(DISP_E_UNKNOWNINTERFACE);
	}

	if (puArgErr == NULL)
		puArgErr = &uArgErr;
	if (pVarResult == NULL)
		pVarResult = &varResultDummy;

	VariantInit(pVarResult);
	
	return S_OK;
}
