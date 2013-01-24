#include "Modify.h"

#include <Exdisp.h>
#include <Exdispid.h>


namespace Page
{
	class DOMLoaded : public IDispatch
	{
	public:
		IWebBrowser2* webBrowser2;
		HTTPS::Rules rules;
		DOMLoaded(IWebBrowser2* inRootFrame, HTTPS::Rules &aRules);

		// IUnknown
		HRESULT STDMETHODCALLTYPE QueryInterface(REFIID riid, LPVOID FAR* ppvObj)
		{
			UNREFERENCED_PARAMETER(riid);

			*ppvObj = 0;
			return E_NOTIMPL;
		}
		ULONG STDMETHODCALLTYPE AddRef(void) { return 1; }
		ULONG STDMETHODCALLTYPE Release(void) { return 1; }

		// IDispatch
		HRESULT STDMETHODCALLTYPE GetTypeInfoCount(UINT *pctinfo)
		{
			*pctinfo = 0;
			return E_NOTIMPL;
		}
		HRESULT STDMETHODCALLTYPE GetTypeInfo(UINT iTInfo, LCID  lcid, ITypeInfo **ppTInfo);
		HRESULT STDMETHODCALLTYPE GetIDsOfNames(REFIID riid, LPOLESTR *rgszNames, UINT cNames, LCID lcid, DISPID *rgDispId);
		HRESULT STDMETHODCALLTYPE Invoke(DISPID dispIdMember, REFIID riid, LCID lcid, WORD wFlags,
			DISPPARAMS *pDispParams, VARIANT *pVarResult, EXCEPINFO *pExcepInfo, UINT *puArgErr);
	};
}
