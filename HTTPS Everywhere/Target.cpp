#include "Target.h"
#include "debug.h"
#include <string>


HTTPS::Target::Target(const char* aHost)
	:host(NULL), isSearch(false)
{
	if (strchr(aHost, '*') != NULL)
	{
		//Debug::Log("%s has a *", host);
		this->isSearch = true;
		//Debug::Log("isSearch %i", this->isSearch);
	}
	else
	{
		//Debug::Log("%s does NOT have *", host);
		this->isSearch = false;
	}

	if (aHost != NULL)
	{
		host = new char[strlen(aHost) + 1];
		strcpy_s(host, strlen(aHost) + 1, aHost);
	}
}

HTTPS::Target::~Target()
{
	if (host != NULL)
		delete host;
}

bool HTTPS::Target::IsMatch(char* domain)
{
	int result = 0;
	//Debug::Log("IsMatch: %i", this->isSearch);
	//Debug::Log("IsMatch: %s", this->host);

	if (this->isSearch) {
		//Debug::Log("Search %s in %s", this->host, domain);
		result = 1 - this->wildcmp(this->host, domain);
	}
	else {
		result = strcmp(this->host, domain);
	}

	if (result == 0)
		return true;
	else
		return false;
}

char* HTTPS::Target::GetHost()
{
	return this->host;
}

int HTTPS::Target::wildcmp(const char* wild, const char* string) {
  const char *cp = NULL, *mp = NULL;

  while ((*string) && (*wild != '*')) {
    if ((*wild != *string) && (*wild != '?')) {
      return 0;
    }
    wild++;
    string++;
  }

  while (*string) {
    if (*wild == '*') {
      if (!*++wild) {
        return 1;
      }
      mp = wild;
      cp = string+1;
    } else if ((*wild == *string) || (*wild == '?')) {
      wild++;
      string++;
    } else {
      wild = mp;
      string = cp++;
    }
  }

  while (*wild == '*') {
    wild++;
  }
  return !*wild;
}