#pragma once

#include <string>

namespace HTTPS
{
	class Securecookies
	{
	public:
		Securecookies(const char* host, const char* name);
		bool IsMatch(char* domain, const char* name);


	private:
		std::string host;
		std::string name;
	};
}