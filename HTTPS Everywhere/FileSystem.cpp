#include "FileSystem.h"
#include "debug.h"
#include "Convert.h"

#include "Shlobj.h"


typedef HRESULT (WINAPI* lpSHGetKnownFolderPath)(
    REFKNOWNFOLDERID rfid,
    DWORD dwFlags,
    HANDLE hToken,
    PWSTR *ppszPath
);


PSTR bho::FileSystem::localLow = NULL;

char* bho::FileSystem::BHOLocalLowFile(PSTR file)
{
	PSTR localLow = BHOLocalLow();

	const int size = strlen(localLow) + strlen("\\") + strlen(file) + 1;
	char* path = new char[size + 1];
	strcpy_s( path, size, localLow);
	strcat_s( path, size, "\\");
	strcat_s( path, size, file);

	Debug::Log(path);
	return path;
}

// TODO: duplicate code with Preferences.cpp (OS version)
PSTR bho::FileSystem::AppLocalLow()
{
	if (localLow != NULL)
		return localLow;

	// Get OS version
	OSVERSIONINFO osvi;
	ZeroMemory(&osvi, sizeof(OSVERSIONINFO));
    osvi.dwOSVersionInfoSize = sizeof(OSVERSIONINFO);

    GetVersionEx(&osvi);

	HMODULE hndl_shell32;
    lpSHGetKnownFolderPath pSHGetKnownFolderPath;
	hndl_shell32 = LoadLibrary("shell32");
    if (NULL != hndl_shell32)
    {
		Debug::Log("Windows Vista or Above");
		pSHGetKnownFolderPath = (lpSHGetKnownFolderPath) GetProcAddress(hndl_shell32, "SHGetKnownFolderPath");
		if(pSHGetKnownFolderPath != NULL)  // Windows Vista and above
        {
			PWSTR path = nullptr;
			HRESULT hr = pSHGetKnownFolderPath(FOLDERID_LocalAppDataLow, 0, NULL, &path);

			if(!SUCCEEDED(hr))  {
				Debug::Error("Could not find FOLDERID_LocalAppDataLow");
				return PSTR("");
			}

			char *pStr;
			bho::Convert::ToMultiBytes(pStr, path, -1, TRUE);

			localLow = PSTR(pStr);
			Debug::Log("%s %s", "AppLocalLow: ", localLow);

			FreeLibrary(hndl_shell32);
			return localLow;
		}
		else // Windows XP
		{

			Debug::Log("Windows XP");
			TCHAR szPath[_MAX_PATH];
			HRESULT hr = SHGetFolderPath(NULL, CSIDL_LOCAL_APPDATA, NULL, 0, szPath);


			if(!SUCCEEDED(hr)) 
			{
				Debug::Error("Could not find CSIDL_LOCAL_APPDATA");
				return PSTR("");
			}

			localLow = PSTR(szPath);
			Debug::Log("%s %s", "AppLocalLow: ", localLow);
			FreeLibrary(hndl_shell32);
			return localLow;
		}
	}

	Debug::Error("Could not access SHGetKnownFolderPath");
	return NULL;
}

PSTR bho::FileSystem::bhoLocalLow = NULL;
PSTR bho::FileSystem::BHOLocalLow()
{
	if (bhoLocalLow != NULL)
		return bhoLocalLow;

	PSTR local = AppLocalLow();
	//AppLocalLow();
	//PSTR folder = PSTR(BHO_FILENAME);
	
	//const int size = sizeof(local) / sizeof(local[0]) + sizeof(folder) / sizeof(folder[0]) + 1;
	const int size = strlen(local) + strlen(BHO_FILENAME) + 2;
	Debug::Log("%s %i", "Compile char size: ", size );
	char* path = new char[size + 1];
	memset(path, 0x0, size + 1);
	strcpy_s( path, size, local);
	strcat_s( path, size, "\\");
	strcat_s( path, size, BHO_FILENAME);

	int created = SHCreateDirectoryEx(NULL, _T(path), NULL);
	if (created == ERROR_SUCCESS || created == ERROR_FILE_EXISTS || created == ERROR_ALREADY_EXISTS) {
		bhoLocalLow = path;
		Debug::Log("%s %s", "BHOLocalLow: ", bhoLocalLow);
		return bhoLocalLow;
	}

	Debug::Log("%s %s", "BHOLocalLow directory creation failed ", path);
	return PSTR(local);
}

void bho::FileSystem::Clear() 
{
	if (localLow != NULL)
		delete(localLow);

	if (bhoLocalLow != NULL)
		delete(bhoLocalLow);
}

