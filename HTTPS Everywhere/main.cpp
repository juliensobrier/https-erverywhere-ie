#include "common.h"
#include <Olectl.h>
#include "ClassFactory.h"

#include "debug.h"
#include <windows.h>

#include "FileSystem.h"
#include "Rules.h"
#include "Homepage.h"


volatile LONG DllRefCount=0;
HINSTANCE hInstance=NULL;





// Called by COM to get a reference to our CClassFactory object
STDAPI DllGetClassObject(REFIID rclsid,REFIID riid,LPVOID *ppv)
{
	// We only support one class, make sure rclsid matches CLSID_IEPlugin
	if(!IsEqualCLSID(rclsid,CLSID_IEPlugin)) 
		return CLASS_E_CLASSNOTAVAILABLE;
	// Make sure the ppv pointer is valid
	if(IsBadWritePtr(ppv,sizeof(LPVOID))) 
		return E_POINTER;

	(*ppv)=NULL;
	CClassFactory *pFactory=new CClassFactory;
	// If we couldn't allocate the new object, return an out-of-memory error
	if(pFactory==NULL) 
		return E_OUTOFMEMORY;

	// Query the pFactory object for the requested interface
	HRESULT hr = pFactory->QueryInterface(riid,ppv);

	// If the requested interface isn't supported by pFactory, delete the newly created object
	if(FAILED(hr)) 
		delete pFactory;

	// Return the same HRESULT as CClassFactory::QueryInterface
	return hr;
}

// This function is called by COM to determine if the DLL safe to unload.
// We return true if no objects from this DLL are being used and the DLL is unlocked.
STDAPI DllCanUnloadNow()
{
	//Debug::Log("main.cpp: DllCanUnloadNow");
	bho::FileSystem::Clear();

	if(DllRefCount>0) 
		return S_FALSE;

	return S_OK;
}

// We register ourselves with both COM and Internet Explorer
STDAPI DllRegisterServer()
{
	Debug::Log("Entering DllRegisterServer()");

	HKEY hk;
	TCHAR dllpath[1024];
	DWORD n;

	if (hInstance == NULL)
		hInstance = AfxGetResourceHandle();


	// Get the full path to this DLL's file so we can register it
	GetModuleFileName(hInstance,dllpath,1024);
	Debug::Log("DLL: %s", dllpath);

	// Create our key under HKCR\\CLSID
	if(RegCreateKeyEx(HKEY_CLASSES_ROOT,_T("CLSID\\") CLSID_IEPlugin_Str,0,NULL,0,KEY_ALL_ACCESS,NULL,&hk,NULL)!=ERROR_SUCCESS) return SELFREG_E_CLASS;
	Debug::Log("%s%s", "CLSID\\", CLSID_IEPlugin_Str);

	// Set the name of our BHO
	RegSetValueEx(hk,NULL,0,REG_SZ,(const BYTE*)_T(BHO_NAME),24*sizeof(TCHAR));
	RegCloseKey(hk);

	// Create the InProcServer32 key
	if(RegCreateKeyEx(HKEY_CLASSES_ROOT,_T("CLSID\\") CLSID_IEPlugin_Str _T("\\InProcServer32"),0,NULL,0,KEY_ALL_ACCESS,NULL,&hk,NULL)!=ERROR_SUCCESS) return SELFREG_E_CLASS;
	
	// Set the path to this DLL
	RegSetValueEx(hk,NULL,0,REG_SZ,(const BYTE*)dllpath,(_tcslen(dllpath)+1)*sizeof(TCHAR));

	// Set the ThreadingModel to Apartment
	RegSetValueEx(hk,_T("ThreadingModel"),0,REG_SZ,(const BYTE*)_T("Apartment"),10*sizeof(TCHAR));
	RegCloseKey(hk);

	// Now register the BHO with Internet Explorer
	LPCTSTR subkey = _T("Software\\Microsoft\\Windows\\CurrentVersion\\Explorer\\Browser Helper Objects\\") CLSID_IEPlugin_Str;
	Debug::Log("subkey registration: %s", subkey);
	

	if(RegCreateKeyEx(HKEY_LOCAL_MACHINE,subkey,0,NULL,0,KEY_ALL_ACCESS,NULL,&hk,NULL)!=ERROR_SUCCESS) 
	{
		Debug::Error("Could not create %s", subkey);
		return SELFREG_E_CLASS;
	}
	
	// Tells explorer.exe not to load our BHO
	n = 0x01;
	long result = RegSetValueEx(hk,_T("NoExplorer"),0,REG_DWORD,(const BYTE*)&n,sizeof(n));
	if (result != ERROR_SUCCESS)
	{
		Debug::Error("Could not set regitry NoExplorer: %i", result);
	}
	RegCloseKey(hk);

	HTTPS::Rules rules;
	rules.Add("default.rulesets");
	HTTPS::Homepage::TransformHomepages(rules);

	bho::FileSystem::Clear();

	return S_OK;
}

// Remove registration entries from both COM and Internet Explorer
STDAPI DllUnregisterServer()
{
	Debug::Log("DllUnregisterServer()");

	// Remove the Internet Explorer BHO registration
	RegDeleteKey(HKEY_LOCAL_MACHINE, _T("Software\\Microsoft\\Windows\\CurrentVersion\\Explorer\\Browser Helper Objects\\") CLSID_IEPlugin_Str);
	
	// Remove the COM registration, starting with the deeper key first since RegDeleteKey is not recursive
	RegDeleteKey(HKEY_CLASSES_ROOT,_T("CLSID\\") CLSID_IEPlugin_Str _T("\\InProcServer32"));
	RegDeleteKey(HKEY_CLASSES_ROOT,_T("CLSID\\") CLSID_IEPlugin_Str);

	Debug::Log("DllUnregisterServer finished");
	bho::FileSystem::Clear();
	return S_OK;
}
