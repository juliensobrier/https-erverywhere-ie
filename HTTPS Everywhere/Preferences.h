#pragma once

#include "common.h"
#include <vector>

namespace bho
{
	class Preferences
	{
	public:
		static long getLastSoftwareUpdate();
		static void setLastSoftwareUpdate(long seconds);

		static long getSoftwareUpdateInterval();
		static void setSoftwareUpdateInterval(long seconds);

		static char* Retrieve(HKEY hKey, char* path, char* key, char* defaultValue);
		static std::vector<char*>* RetrieveMultiple(HKEY hKey, char* path, char* key, std::vector<char*>* defaults);

		static void SaveMultiple(HKEY hKey, char* path, char* key, std::vector<char*> values);
		static void Save(char* key, const char* value);
		static void Save(HKEY hKey, char* path, char* key, const char* value);

	private:
		static char* RegistryPath();
		static char* RegistryPathCLSID();

		static char* Retrieve(char* key, char* defaultValue);
		
	};
}