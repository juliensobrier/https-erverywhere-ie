#include "Ruleset.h"
#include "debug.h"


HTTPS::Ruleset::Ruleset(const char* aName, const char* aDefault_off, const char* aPlatform)
	:name(NULL), default_off(NULL), platform(NULL), enabled(true)
{
	if (aDefault_off != NULL)
	{
		enabled = false;
	}
	else if (aPlatform != NULL && 
		(this->strpos(const_cast<char*>(aPlatform), "ie") == 0 || this->strpos(const_cast<char*>(aPlatform), " ie") >= 0))
		enabled = false;

	if (aName != NULL)
	{
		name = new char[strlen(aName) + 1];
		strcpy_s(name, strlen(aName) + 1, aName);
	}

	if (aPlatform != NULL)
	{
		platform = new char[strlen(aPlatform) + 1];
		strcpy_s(platform, strlen(aPlatform) + 1, aPlatform);
	}

	if (aDefault_off != NULL)
	{
		default_off = new char[strlen(aDefault_off) + 1];
		strcpy_s(default_off, strlen(aDefault_off) + 1, aDefault_off);
	}

}

HTTPS::Ruleset::~Ruleset()
{
	if (name != NULL)
		delete name;
	
	if (platform != NULL)
		delete platform;

	if (default_off != NULL)
		delete default_off;
}

void HTTPS::Ruleset::AddTarget(const char* host)
{
	this->targets.push_back(new HTTPS::Target(host));
}

void HTTPS::Ruleset::AddRule(const char* from, const char* to)
{
	this->rules.push_back(new HTTPS::Rule(from, to));
}

void HTTPS::Ruleset::AddSecurecookies(const char* host, const char* name)
{
	this->securecookies.push_back(new HTTPS::Securecookies(host, name));
}

void HTTPS::Ruleset::AddExclusion(const char* pattern)
{
	this->exclusions.push_back(HTTPS::Exclusion(pattern));
}

bool HTTPS::Ruleset::HostMatch(char* domain)
{
	if (enabled == false)
		return false;

	if (this->targets.size() == 0)
		return false;

	std::vector<HTTPS::Target*>::iterator target;
	for(target = this->targets.begin() ; target < this->targets.end(); target++ )
	{
		if ((*target)->IsMatch(domain) == true) {
			//Debug::Log("Rule %s match for %s: %s", this->name, domain, (*target)->GetHost());
			return true;
		}
	}

	return false;
}

char* HTTPS::Ruleset::Transform(char* url)
{
	if (this->rules.size() == 0)
		return NULL;

	std::vector<HTTPS::Rule*>::iterator rule;
	for(rule = this->rules.begin() ; rule < this->rules.end(); rule++ )
	{
		if (this->exclusions.size() > 0)
		{
			bool excluded = false;

			std::vector<HTTPS::Exclusion>::iterator exclusion;
			for(exclusion = this->exclusions.begin() ; exclusion < this->exclusions.end(); exclusion++)
			{
				if ((*exclusion).IsMatch(url))
				{
					excluded = true;
					Debug::Log("%s excluded", url);
					break;
				}
			}

			if(excluded)
				continue;
		}

		/*if (rule == this->rules.end())
		{
			return NULL;
		}*/

		char* newUrl = (*rule)->Transform(url);
		if(newUrl != NULL)
		{
			return newUrl;
		}
	}

	return NULL;
}

bool HTTPS::Ruleset::IsSecurecookies(char* domain, const char* name)
{
	Debug::Log("IsSecurecookies: %s %s", domain, name);

	if (this->securecookies.size() == 0)
	{
		Debug::Log("No secure cookie rule");
		return false;
	}

	std::vector<HTTPS::Securecookies*>::iterator cookie;
	for(cookie = this->securecookies.begin() ; cookie < this->securecookies.end(); cookie++ )
	{
		if ((*cookie)->IsMatch(domain, name))
			return true;
	}

	return false;
}

char* HTTPS::Ruleset::GetName()
{
	Debug::Log("Rule name: %s", this->name);
	return this->name;
}

int HTTPS::Ruleset::strpos(char *haystack, char *needle)
{
   char *p = strstr(haystack, needle);
   if (p)
      return p - haystack;
   return -1;   // Not found = -1.
}
