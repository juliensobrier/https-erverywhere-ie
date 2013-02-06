#include "Rules.h"

#include <tinyxml2.h>
using namespace tinyxml2;

HRESULT HTTPS::Rules::Add(char* file)
{
	// TODO: Put this code in a library
	TCHAR dllpath[_MAX_PATH];

	HMODULE hMod = GetModuleHandle(BHO_FILENAME_DLL);

	DWORD length = GetModuleFileName(hMod, dllpath, _MAX_PATH);
	dllpath[length] = NULL;

	//From complete path, get directory
	TCHAR tszDrive[_MAX_DRIVE], tszPath[_MAX_DIR], tszFilename[_MAX_FNAME], tszExtension[_MAX_EXT];

	_tsplitpath_s(dllpath, tszDrive, _MAX_DRIVE, tszPath, _MAX_DIR, tszFilename, _MAX_FNAME, tszExtension, _MAX_EXT);

	TCHAR* rulesPath = new TCHAR[_MAX_PATH];
	strcpy_s(rulesPath, _MAX_PATH, tszDrive);
	strcat_s(rulesPath, _MAX_PATH, tszPath);
	strcat_s(rulesPath, _MAX_PATH, "rules\\");
	strcat_s(rulesPath, _MAX_PATH, file);
	//Debug::Log("Rule file path: %s", rulesPath);

	HRESULT hr = this->AddAbsolute(rulesPath);

	delete rulesPath;
	return hr;
}



HRESULT HTTPS::Rules::AddAbsolute(char* rulesPath)
{
	Debug::Log("Rules file: %s", rulesPath);

	// Check if file exists
	DWORD dwAttrib = GetFileAttributes(rulesPath);
	if (dwAttrib == INVALID_FILE_ATTRIBUTES)
	{
		Debug::Warn("File %s does not exist", rulesPath);
		return E_FAIL;
	}
	else if (dwAttrib & FILE_ATTRIBUTE_DIRECTORY) {
		Debug::Warn("%s is a directory, not a file", rulesPath);
		return E_FAIL;
	}


	tinyxml2::XMLDocument doc;
	XMLError loaded = doc.LoadFile(rulesPath);
	if (loaded == XML_SUCCESS) {
		Debug::Log("Rules loaded");

		XMLElement* child = doc.FirstChildElement("rulesetlibrary")->FirstChildElement("ruleset");

		for( child; child; child = child->NextSiblingElement("ruleset") )
		{
			const char* name = child->Attribute("name");
			//Debug::Log("Rule name: %s", name);

			const char* default_off = child->Attribute("default_off");
			/*if (default_off != NULL)
				Debug::Log("Rule default_off: %s", default_off);
			else
				Debug::Log("No default_off attribute");*/

			const char* platform = child->Attribute("platform");
			/*if (platform != NULL)
				Debug::Log("Rule platform: %s", platform);*/

			HTTPS::Ruleset* ruleset = new Ruleset(name, default_off, platform);

			XMLElement* target = child->FirstChildElement("target");
			for( target; target; target=target->NextSiblingElement("target") )
			{
				const char* host = target->Attribute("host");
				//Debug::Log("Rule host: %s", host);
				ruleset->AddTarget(host);
			}

			XMLElement* rule = child->FirstChildElement("rule");
			for( rule; rule; rule=rule->NextSiblingElement("rule") )
			{
				const char* from = rule->Attribute("from");
				const char* to = rule->Attribute("to");

				//Debug::Log("Rule from: %s - to :%s", from, to);
				ruleset->AddRule(from, to);
			}

			XMLElement* cookies = child->FirstChildElement("securecookie");
			for( cookies; cookies; cookies=cookies->NextSiblingElement("securecookie") )
			{
				const char* host = cookies->Attribute("host");
				const char* nameCookie = cookies->Attribute("name");

				//Debug::Log("Secure cookies host: %s", host);
				ruleset->AddSecurecookies(host, nameCookie);
			}

			XMLElement* exclusion = child->FirstChildElement("exclusion");
			for( exclusion; exclusion; exclusion=exclusion->NextSiblingElement("exclusion") )
			{
				const char* pattern = exclusion->Attribute("pattern");

				//Debug::Log("Exclusion pattern: %s", pattern);
				ruleset->AddExclusion(pattern);
			}

			this->rules.push_back(ruleset);
		}

		Debug::Log("All rules loaded");
		return S_OK;
	}
	else {
		Debug::Error("Rules NOT loaded");
		return E_FAIL;
	}
}

bool HTTPS::Rules::HostMatch(char* domain)
{
	//Debug::Log("Check match for %s", domain);

	if (this->rules.size() == 0)
		return false;

	std::vector<HTTPS::Ruleset*>::iterator ruleset;
	for(ruleset = this->rules.begin() ; ruleset < this->rules.end(); ruleset++ )
	{
		//Debug::Log("Check rule %s", (*ruleset)->GetName());

		if ((*ruleset)->HostMatch(domain) == true)
		{
			//Debug::Log("Host match found for %s", domain);
			return true;
		}
	}

	//Debug::Log("No host match for %s", domain);
	return false;
}

bool HTTPS::Rules::HostMatchFromUrl(char* url)
{
	char* domain = this->DomainFromUrl(url);
	bool result = this->HostMatch(domain);

	delete domain;
	return result;
}

char* HTTPS::Rules::Transform(char* url)
{
	//get domain
	char* domain = this->DomainFromUrl(url);
	if (domain == NULL || strcmp(domain, "") == 0)
	{
		Debug::Log("No domain found for %s", url);
		return NULL;
	}

	if (this->rules.size() == 0)
	{
		delete domain;
		return NULL;
	}

	std::vector<HTTPS::Ruleset*>::iterator ruleset;
	for(ruleset = this->rules.begin() ; ruleset < this->rules.end(); ruleset++ )
	{
		if ((*ruleset)->HostMatch(domain) == true)
		{
			char* newUrl = (*ruleset)->Transform(url);

			if (newUrl != NULL) {
				delete domain;
				return newUrl;
			}
		}
	}

	delete domain;
	return NULL;
}

bool HTTPS::Rules::SecureCookie(char* url, const char* name)
{
	char* domain = this->DomainFromUrl(url);
	//Debug::Log("DomainFromUrl finished: %s", domain);

	if (domain == NULL || strcmp(domain, "") == 0)
	{
		Debug::Log("No domain found for %s", url);
		return false;
	}

	if (this->rules.size() == 0)
		return false;

	std::vector<HTTPS::Ruleset*>::iterator ruleset;
	for(ruleset = this->rules.begin() ; ruleset < this->rules.end(); ruleset++ )
	{
		if ((*ruleset)->HostMatch(domain) == true)
		{
			if ((*ruleset)->IsSecurecookies(domain, name))
				return true;
		}
	}

	return false;
}

char* HTTPS::Rules::DomainFromUrl(char* url) {
	//Debug::Log("DomainFromUrl: %s", url);
	if (strlen(url) == 0)
		return "";

	std::string sUrl = std::string(url);
	size_t begin = sUrl.find("://");
	if (begin == std::string::npos)
	{
		Debug::Log("%s %s", "No schema found in",  url);
		return "";
	}
	begin += 3;

	size_t end = sUrl.find("/", begin + 1);
	if (end == std::string::npos)
	{
		Debug::Log("%s %s, using string last position", "No / found in",  url);
		end = sUrl.length() - 0;
	}

	std::string domain;
	domain = sUrl.substr(begin, end - begin);
	//Debug::Log("begin: %i - end: %i", begin, end);
	//Debug::Log("%s has domain %s", url, domain.c_str());

	int size = domain.length() + 1;
	char* value = new char[size];
	strcpy_s(value, size, domain.c_str());
	return value;
}

bool HTTPS::Rules::IsEmpty()
{
	if(rules.size() == 0)
		return true;
	else
		return false;
}
