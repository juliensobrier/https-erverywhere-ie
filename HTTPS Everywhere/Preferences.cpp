#include "Preferences.h"
#include "debug.h"
#include <sstream>



static char* registryPath = NULL;

char* bho::Preferences::RegistryPath()
{
	if (registryPath != NULL) {
		return registryPath;
	}

	// Get OS version
	OSVERSIONINFO osvi;
	ZeroMemory(&osvi, sizeof(OSVERSIONINFO));
    osvi.dwOSVersionInfoSize = sizeof(OSVERSIONINFO);

    GetVersionEx(&osvi);

	// Windows XP
	if ((osvi.dwMajorVersion <= 5) && (osvi.dwPlatformId == VER_PLATFORM_WIN32_NT )) {
		registryPath = "Software\\Classes\\Software";
		return registryPath;
	}
	else { // Windows Vista and higher
		registryPath = "Software\\AppDataLow\\Software";
		return registryPath;
	}
}

static char* registryPathCLSID = NULL;

char* bho::Preferences::RegistryPathCLSID()
{
	if (registryPathCLSID != NULL) {
		return registryPathCLSID;
	}

	char* registryPathCLSID = new char[_MAX_PATH];
	strcpy_s(registryPathCLSID, _MAX_PATH, RegistryPath());
	strcat_s(registryPathCLSID, _MAX_PATH, "\\");
	strcat_s(registryPathCLSID, _MAX_PATH, GUID_IE_Str);
	Debug::Log(registryPathCLSID);

	return registryPathCLSID;
}

char* bho::Preferences::Retrieve(char* key, char* defaultValue)
{
	char* path = RegistryPathCLSID();
	return  Retrieve(HKEY_CURRENT_USER, path, key, defaultValue);
}


char* bho::Preferences::Retrieve(HKEY hKey, char* path, char* key, char* defaultValue)
{
	HKEY hk;

	if( RegOpenKeyEx(hKey,_T(path), 0, KEY_ALL_ACCESS, &hk) != ERROR_SUCCESS) {
		Debug::Warn("Could not open Registry key at %s", path);

		// Crreate it if it does not exist
		if(RegCreateKeyEx(hKey,_T(path),0,NULL,0,KEY_ALL_ACCESS,NULL,&hk,NULL) != ERROR_SUCCESS) {
			Debug::Error("Could not open Registry key in HKEY_CURRENT_USER");
			return defaultValue;
		}
	}
	
	char buf[1024] = {0};
	DWORD dwBufSize = sizeof(buf);
	DWORD dwType = REG_SZ;

	if( RegQueryValueEx(hk,_T(key), 0, &dwType, (BYTE*)buf, &dwBufSize) != ERROR_SUCCESS) {
		Debug::Error("Could not open key in HKEY_CURRENT_USER 5: %s", key);

		// Create it if it does not exist
		RegSetValueEx(hk,_T(key),0,REG_SZ,(const BYTE*)_T(defaultValue),strlen(defaultValue)*sizeof(TCHAR));

		RegCloseKey(hk);
		return defaultValue;
	}


	RegCloseKey(hk);

	char* value = new char[1024];
	strcpy_s(value, 1024, buf);
	return value;
}

std::vector<char*>* bho::Preferences::RetrieveMultiple(HKEY hKey, char* path, char* key, std::vector<char*>* defaults)
{
	std::vector<char*>* urls = new std::vector<char*>();
	HKEY hk;

	if( RegOpenKeyEx(hKey,_T(path), 0, KEY_ALL_ACCESS, &hk) != ERROR_SUCCESS) {
		Debug::Warn("Could not open Registry key at %s", path);

		// Crreate it if it does not exist
		if(RegCreateKeyEx(hKey,_T(path),0,NULL,0,KEY_ALL_ACCESS,NULL,&hk,NULL) != ERROR_SUCCESS) {
			Debug::Error("Could not open Registry key in HKEY_CURRENT_USER");

			return defaults;
		}
	}

	DWORD dwType = REG_MULTI_SZ;
	DWORD dwSize;

	if( RegQueryValueEx(hk,_T(key),0,0,0,&dwSize) != ERROR_SUCCESS) {
		Debug::Error("Could not open key in RetrieveMultiple 1 : %s", key);

		// Create it if it does not exist
		// Todo: FIX
		//RegSetValueEx(hk,_T(key),0,REG_MULTI_SZ,(const BYTE*)_T(defaultValue),strlen(defaultValue)*sizeof(TCHAR));

		RegCloseKey(hk);
		return defaults;
	}

	LPBYTE lpBuffer = LPBYTE(GlobalAlloc(GPTR, dwSize));
    if( lpBuffer == NULL )
    {
		Debug::Error("GlobalAlloc failed (%d)\n", GetLastError());
        RegCloseKey(hk);
        return defaults;
    }


    if (RegQueryValueEx(hk,_T(key),0,&dwType,lpBuffer,&dwSize) != ERROR_SUCCESS) {
		Debug::Error("Could not open key in RetrieveMultiple 2 : %s", key);

		RegCloseKey(hk);
		return defaults;
	}
    

    register LPTSTR p = LPTSTR(lpBuffer);
    for(; *p; p += _tcslen(p) + 1)
	{
          //Debug::Log("Home page: %s\n",p); 

		  int size = strlen(p) + 1;
		  char* url = new char[size];
		  strcpy_s(url, size, p);

		  urls->push_back(url);

	}

    GlobalFree(HGLOBAL(lpBuffer));
	RegCloseKey(hk);

	return urls;
}

// TODO: no duplicate code with Retrieve
void bho::Preferences::Save(char* key, const char* value)
{
	char* path = RegistryPathCLSID();
	Save(HKEY_CURRENT_USER, path, key, value);
}

void bho::Preferences::Save(HKEY hKey, char* path, char* key, const char* value)
{
	HKEY hk;

	if( RegOpenKeyEx(hKey,_T(path), 0, KEY_ALL_ACCESS, &hk) != ERROR_SUCCESS) {
		Debug::Warn("Could not open Registry key in HKEY_CURRENT_USER 1");

		// Create it if it does not exist
		if(RegCreateKeyEx(hKey,_T(path),0,NULL,0,KEY_ALL_ACCESS,NULL,&hk,NULL) != ERROR_SUCCESS) {
			Debug::Error("Could not open Registry key in HKEY_CURRENT_USER 2");
			return;
		}
	}

	RegSetValueEx(hk,_T(key),0,REG_SZ,(const BYTE*)_T(value),strlen(value)*sizeof(TCHAR));

	RegCloseKey(hk);
}

void bho::Preferences::SaveMultiple(HKEY hKey, char* path, char* key, std::vector<char*> values)
{
	HKEY hk;

	if( RegOpenKeyEx(hKey,_T(path), 0, KEY_ALL_ACCESS, &hk) != ERROR_SUCCESS) {
		Debug::Warn("Could not open Registry key at %s in SaveMultiple", path);

		// Create it if it does not exist
		if(RegCreateKeyEx(hKey,_T(path),0,NULL,0,KEY_ALL_ACCESS,NULL,&hk,NULL) != ERROR_SUCCESS) {
			Debug::Error("Could not open Registry key in SaveMultiple 2");
			return;
		}
	}

	int len=1;
	std::vector<char*>::iterator value;

	for(value = values.begin() ; value < values.end(); value++ )
	{
		len += strlen(*value) + 1;
		Debug::Log("value: %s", (*value));
	}

	char* multi_sz = new char[len];
	char* ptr = multi_sz;
	memset(multi_sz, 0, len);
	for(value = values.begin() ; value < values.end(); value++ )
	{
		strcpy_s(ptr, len, *value);
		ptr += strlen(*value)+1;
	}

	RegSetValueEx(hk,_T(key),0,REG_MULTI_SZ,(LPBYTE)multi_sz,len*sizeof(TCHAR));
	delete multi_sz;
}

long bho::Preferences::getLastSoftwareUpdate()
{
	return atoi( Retrieve("LastSoftwareUpdate", "0"));
}

void bho::Preferences::setLastSoftwareUpdate(long seconds)
{
	std::ostringstream oss;
	oss << seconds;

	bho::Preferences::Save("LastSoftwareUpdate", oss.str().c_str());
}

long bho::Preferences::getSoftwareUpdateInterval()
{
	return atoi( Retrieve("SoftwareUpdateInterval", "86400")); // default: 24 hours
}

void bho::Preferences::setSoftwareUpdateInterval(long seconds)
{
	std::ostringstream oss;
	oss << seconds;

	bho::Preferences::Save("SoftwareUpdateInterval", oss.str().c_str());
}