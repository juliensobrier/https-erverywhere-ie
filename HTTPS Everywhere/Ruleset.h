#pragma once

#include "Target.h"
#include "Rule.h"
#include "Securecookies.h"
#include "Exclusion.h"

#include <vector>


namespace HTTPS
{
	class Ruleset
	{
	public:
		Ruleset(const char* name, const char* default_off, const char* platform);
		~Ruleset();

		void AddTarget(const char* host);
		void AddRule(const char* from, const char* to);
		void AddSecurecookies(const char* host, const char* name);
		void AddExclusion(const char* pattern);

		bool HostMatch(char* domain);
		char* Transform(char* url);
		bool IsSecurecookies(char* domain, const char* name);

		bool enabled;
		char* GetName();

	private:
		char* default_off;
		//std::string default_off;
		char* platform;
		//std::string platform;
		char* name;

		std::vector<HTTPS::Target*> targets;
		std::vector<HTTPS::Rule*> rules;
		std::vector<HTTPS::Securecookies*> securecookies;
		std::vector<HTTPS::Exclusion> exclusions;

		// TODO: move to some string helper function
		int strpos(char *haystack, char *needle);
	};
}