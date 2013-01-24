#pragma once

#include "Ruleset.h"
#include "common.h"


namespace HTTPS
{
	class Rules
	{
	public:
		HRESULT Add(char* file);
		HRESULT AddAbsolute(char* rulesPath);
		bool HostMatch(char* domain);
		bool HostMatchFromUrl(char* url);
		char* Transform(char* url);
		bool SecureCookie(char* url, const char* name);
		bool IsEmpty();

	private:
		std::vector<HTTPS::Ruleset*> rules;
		char* DomainFromUrl(char* url);
	};
}