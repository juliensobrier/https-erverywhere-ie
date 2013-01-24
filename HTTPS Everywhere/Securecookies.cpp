#include "Securecookies.h"
#include "debug.h"

#include <regex>

HTTPS::Securecookies::Securecookies(const char* aHost, const char* aName)
{
	this->host.assign(aHost);
	this->name.assign(aName);
}

bool HTTPS::Securecookies::IsMatch(char* aDomain, const char* aName)
{
	//Debug::Log("Securecookies::IsMatch");
	try
	{
		std::regex domainRegex(this->host, std::regex_constants::icase);
		if (! std::regex_search(std::string(aDomain), domainRegex))
		{
			//Debug::Log("Host %s does not match %s", aDomain, this->host);
			return false;
		}

		if (this->name.compare(".+") == 0 || this->name.compare(".*") == 0)
		{
			//Debug::Log("Securecookies::IsMatch: match for all names");
			return true;
		}

		std::regex nameRegex(this->name, std::regex_constants::icase);
		if (std::regex_search(std::string(aName), nameRegex))
		{
			//Debug::Log("Name match: %s => %s", aName, this->name.c_str());
			return true;
		}
		else
		{
			//Debug::Log("Nme %s does not match %s", aName, this->name.c_str());
			return false;
		}
	}
	catch(std::exception& e)
	{
		Debug::Error("Error inside Securecookies::IsMatch: %s", e.what());
		return NULL;
	}
}