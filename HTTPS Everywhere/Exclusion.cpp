#include "Exclusion.h"
#include <regex>


HTTPS::Exclusion::Exclusion(const char* aPattern)
{
	this->pattern.assign(aPattern);
}

bool HTTPS::Exclusion::IsMatch(char* url)
{
	std::regex urlRegex(this->pattern, std::regex_constants::icase);
	if (! std::regex_search(std::string(url), urlRegex))
	{
		//Debug::Log("URL %s does not match %s", url, this->pattern);
		return false;
	}
	else
	{
		return true;
	}
}