#include "EventSink.h"

#include "HTTP.h"
#include "WininetHook.h"


CEventSink EventSink;


STDMETHODIMP CEventSink::QueryInterface(REFIID riid,void **ppvObject)
{
	// Check if ppvObject is a valid pointer
	if(IsBadWritePtr(ppvObject,sizeof(void*))) 
		return E_POINTER;
	// Set *ppvObject to NULL
	(*ppvObject)=NULL;
	// See if the requested IID matches one that we support
	// If it doesn't return E_NOINTERFACE
	if(!IsEqualIID(riid,IID_IUnknown) && !IsEqualIID(riid,IID_IDispatch) && !IsEqualIID(riid,DIID_DWebBrowserEvents2)) 
		return E_NOINTERFACE;
	// If it's a matching IID, set *ppvObject to point to the global EventSink object
	(*ppvObject)=(void*)&EventSink;

	return S_OK;
}

STDMETHODIMP_(ULONG) CEventSink::AddRef()
{
	//Debug::Log("CEventSink::AddRef");
	return 1; // We always have just one static object
}

STDMETHODIMP_(ULONG) CEventSink::Release()
{
	//Debug::Log("CEventSink::Release");
	return 1;
}

STDMETHODIMP CEventSink::GetTypeInfoCount(UINT *pctinfo)
{
	UNREFERENCED_PARAMETER(pctinfo);

	return E_NOTIMPL;
}

STDMETHODIMP CEventSink::GetTypeInfo(UINT iTInfo,LCID lcid,ITypeInfo **ppTInfo)
{
	UNREFERENCED_PARAMETER(iTInfo);
	UNREFERENCED_PARAMETER(lcid);
	UNREFERENCED_PARAMETER(ppTInfo);

	return E_NOTIMPL;
}

STDMETHODIMP CEventSink::GetIDsOfNames(REFIID riid,LPOLESTR *rgszNames,UINT cNames,LCID lcid,DISPID *rgDispId)
{
	UNREFERENCED_PARAMETER(riid);
	UNREFERENCED_PARAMETER(rgszNames);
	UNREFERENCED_PARAMETER(cNames);
	UNREFERENCED_PARAMETER(lcid);
	UNREFERENCED_PARAMETER(rgDispId);

	return E_NOTIMPL;
}

void CEventSink::Init(IWebBrowser2* pSite) {

	this->pSite = pSite;

	if (rules.IsEmpty())
	{
		rules.Add("default.rulesets");
		rules.Add("custom.rulesets");
	}

	LoadLibrary(BHO_FILENAME_DLL);


	//rules.Add("default.rulesets"); // Check if necessary

	WinInetInstallHooks(rules);
	pDomEvent = NULL;

	Debug::Log("CEventSink::Init finished");
}

void CEventSink::Exit()
{
	if (pDomEvent != NULL)
	{
		delete pDomEvent;
		pDomEvent = NULL;
	}

	//WinInetRemoveHooks();
}


// This is called by IE to notify us of events
// http://msdn.microsoft.com/en-us/library/aa768283(VS.85).aspx
STDMETHODIMP CEventSink::Invoke(DISPID dispIdMember,REFIID riid,LCID lcid,WORD wFlags,DISPPARAMS *pDispParams,VARIANT *pVarResult,EXCEPINFO *pExcepInfo,UINT *puArgErr)
{
	UNREFERENCED_PARAMETER(lcid);
	UNREFERENCED_PARAMETER(wFlags);
	UNREFERENCED_PARAMETER(pVarResult);
	UNREFERENCED_PARAMETER(pExcepInfo);
	UNREFERENCED_PARAMETER(puArgErr);

	if(!IsEqualIID(riid,IID_NULL)) return DISP_E_UNKNOWNINTERFACE; // riid should always be IID_NULL

	/*
Order:
[6184] HTTPSEverywhere DISPID_COMMANDSTATECHANGE
[6184] HTTPSEverywhere DISPID_COMMANDSTATECHANGE
[6184] HTTPSEverywhere DISPID_DOWNLOADCOMPLETE
[6184] HTTPSEverywhere DISPID_COMMANDSTATECHANGE
[6184] HTTPSEverywhere DISPID_NAVIGATECOMPLETE2
*/
	
	if(dispIdMember == DISPID_DOCUMENTCOMPLETE) { // DWebBrowserEvents2::DocumentComplete
		//Debug::Log("DISPID_DOCUMENTCOMPLETE");
		CheckForUpdate();
	}
	else if(dispIdMember==DISPID_NAVIGATECOMPLETE2) { // DWebBrowserEvents2::NavigateComplete2
		Debug::Log("DISPID_NAVIGATECOMPLETE2");
		AttachEvent(pDispParams->rgvarg[1].pdispVal); // helps a bit but not enough for HTTP script
	}
	else if (dispIdMember==DISPID_WINDOWSTATECHANGED) {
		//Debug::Log("DISPID_WINDOWSTATECHANGED");

		DWORD dwMask = pDispParams->rgvarg[0].lVal;
		DWORD dwFlags = pDispParams->rgvarg[1].lVal;

		// Is tab visible?
		if (dwMask & OLECMDIDF_WINDOWSTATE_USERVISIBLE) {
			visible = dwFlags & OLECMDIDF_WINDOWSTATE_USERVISIBLE;
		}
	}
	/*else if (dispIdMember==DISPID_ONQUIT) {
		Debug::Log("ONQUIT");
		//WinInetRemoveHooks();
		//Debug::Log("Wininet hook removed");
	}*/

	return S_OK;
}



void CEventSink::CheckForUpdate() {
	const char* urlUpdate = bho::SoftwareUpdate::Check(this->visible);
	if (urlUpdate != NULL) 
	{
		Debug::Log("Update URL: %s", urlUpdate);
		bho::HTTP::OpenNewTab(urlUpdate, pSite);
	}
}

void CEventSink::AttachEvent(IDispatch *pDisp)
{
	CComQIPtr<IWebBrowser2> pBrowser(pDisp);
	if (! pBrowser)
	{
		Debug::Warn("Could not retrieve IWebBrowser2");
		return;
	}

	CComPtr<IDispatch> pUnk = NULL;
	HRESULT hr = pBrowser->get_Document(&pUnk);
	if (FAILED(hr) || pUnk == NULL)
	{
		Debug::Warn("get_Document failed");
		return;
	}

	CComQIPtr<IHTMLDocument2> pHtmlDoc2(pUnk);
	if (! pHtmlDoc2)
	{
		Debug::Warn("Could not retrieve IHTMLDocument2");
		return;
	}

	if (Page::Modify::ShouldModify(pHtmlDoc2, rules) == FALSE)
	{
		Debug::Log("Do not attach event");
		return;
	}


	CComQIPtr<IHTMLWindow2> pWindow2;
	hr = pHtmlDoc2->get_parentWindow((IHTMLWindow2**) &pWindow2);
	if(FAILED(hr))
	{
		Debug::Warn("Could not get pWindow2");
		return;
	}

	CComQIPtr<IHTMLWindow3> pWindow3(pWindow2);
	if(! pWindow3)
	{
		Debug::Warn("Could not get pWindow3");
		return;
	}


	CComQIPtr<IHTMLDocument3> pDoc3(pHtmlDoc2);
	if(! pDoc3)
	{
		Debug::Warn("Could not create pDoc3");
		return;
	}

	VARIANT_BOOL result = VARIANT_TRUE;
	CComBSTR onEvent(TEXT("onreadystatechange")); // DOMContentLoaded onclick onload onreadystatechange

	if (pDomEvent != NULL)
	{
		delete pDomEvent;
		pDomEvent = NULL;
	}
	pDomEvent = new Page::DOMLoaded(pBrowser, rules);

	hr = pDoc3->attachEvent(onEvent, pDomEvent, &result); // DOMContentLoaded FAIL
	//pDoc3->detachEvent(onEvent, pDomEvent);
	//hr = pWindow3->attachEvent(onEvent, pDomEvent, &result); // onload OK

	if(FAILED(hr))
	{
		Debug::Error("Error in attachEvent result");
		return;
	}
	if (result != VARIANT_TRUE )
	{
		Debug::Error("Error in attachEvent result");
		return;
	}
}
