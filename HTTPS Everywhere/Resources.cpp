#include "Resources.h"
#include "debug.h"

#pragma comment(lib,"Version.lib")


char* bho::Resources::Extract(char* name, char* type) {
	HMODULE hMod = GetModuleHandle(BHO_FILENAME_DLL);

	HRSRC hres = FindResource(hMod, _T(name), _T(type));
	if (hres == 0) {
		Debug::Log("resource NOT found");
		return "";
	}

	Debug::Log("resource found");
	HGLOBAL hbytes = LoadResource(hMod, hres);

	//Convert the resource text file to data we can use
	LPVOID pdata = LockResource(hbytes);

	LPTSTR sString = (LPTSTR)pdata;

	//PSTR test = PSTR(sString);
	//size_t size = strlen(test);
	size_t size = strlen(sString);
	Debug::Log("data length: %i", size);

    char* pBuffer = new char[size + 1];
    //strcpy_s(pBuffer, size + 1, test);
	strcpy_s(pBuffer, size + 1, sString);


	//Debug::Log("test: %s", pBuffer);

	FreeResource(hbytes);
	return pBuffer;
}


int* bho::Resources::IEVersion() {
	char fileName[_MAX_PATH]; // Not sure where _MAX_PATH si defined
	DWORD size = GetModuleFileName(NULL, fileName, _MAX_PATH);
	fileName[size] = NULL;
	DWORD handle = 0;
	size = GetFileVersionInfoSize(fileName, &handle);
	BYTE* versionInfo = new BYTE[size];
	if (!GetFileVersionInfo(fileName, handle, size, versionInfo))
	{
		Debug::Warn("IE version could not be found");
		delete[] versionInfo;
		return NULL;
	}

	UINT len = 0;
	VS_FIXEDFILEINFO* vsfi = NULL;
	VerQueryValue(versionInfo, LPCSTR("\\"), (void**)&vsfi, &len);

	int *aVersion = new int[4];
	aVersion[0] = HIWORD(vsfi->dwFileVersionMS);
	aVersion[1] = LOWORD(vsfi->dwFileVersionMS);
	aVersion[2] = HIWORD(vsfi->dwFileVersionLS);
	aVersion[3] = LOWORD(vsfi->dwFileVersionLS);
	delete[] versionInfo;
	return aVersion;
}

int* bho::Resources::Version() {
	char fileName[_MAX_PATH];
	HMODULE hMod = GetModuleHandle(BHO_FILENAME_DLL);
	DWORD size = GetModuleFileName(hMod, fileName, _MAX_PATH);
	fileName[size] = NULL;
	DWORD handle = 0;
	size = GetFileVersionInfoSize(fileName, &handle);
	BYTE* versionInfo = new BYTE[size];
	if (!GetFileVersionInfo(fileName, handle, size, versionInfo))
	{
		Debug::Warn("File version could not be found");
		delete[] versionInfo;
		return NULL;
	}
	// we have version information
	UINT len = 0;
	VS_FIXEDFILEINFO* vsfi = NULL;
	VerQueryValue(versionInfo, LPCSTR("\\"), (void**)&vsfi, &len);

	int *aVersion = new int[4];
	aVersion[0] = HIWORD(vsfi->dwFileVersionMS);
	aVersion[1] = LOWORD(vsfi->dwFileVersionMS);
	aVersion[2] = HIWORD(vsfi->dwFileVersionLS);
	aVersion[3] = LOWORD(vsfi->dwFileVersionLS);
	delete[] versionInfo;
	return aVersion;
}