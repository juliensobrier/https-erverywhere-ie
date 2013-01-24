#ifndef __EVENTSINK_H__
#define __EVENTSINK_H__

#include "DOMLoaded.h"

#include "SoftwareUpdate.h"
#include "Rules.h"



class CEventSink : public DWebBrowserEvents2 {
	public:
		// IUnknown methods
		STDMETHODIMP QueryInterface(REFIID riid,void **ppvObject);
		STDMETHODIMP_(ULONG) AddRef();
		STDMETHODIMP_(ULONG) Release();

		// IDispatch methods
		STDMETHODIMP GetTypeInfoCount(UINT *pctinfo);
		STDMETHODIMP GetTypeInfo(UINT iTInfo,LCID lcid,ITypeInfo **ppTInfo);
		STDMETHODIMP GetIDsOfNames(REFIID riid,LPOLESTR *rgszNames,UINT cNames,LCID lcid,DISPID *rgDispId);
		STDMETHODIMP Invoke(DISPID dispIdMember,REFIID riid,LCID lcid,WORD wFlags,DISPPARAMS *pDispParams,VARIANT *pVarResult,EXCEPINFO *pExcepInfo,UINT *puArgErr);

		IWebBrowser2 *pSite; // the currently set site
		void Init(IWebBrowser2* pSite); // initialize EventSink with the pSite reference
		void Exit();
		Page::DOMLoaded* pDomEvent;

		HTTPS::Rules rules;

	protected:
		void AttachEvent(IDispatch *pDisp);
		void CheckForUpdate();

		bool visible;
};

// We only have one global object of this
extern CEventSink EventSink;


#endif