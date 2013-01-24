#pragma once

#include "debug.h"
#include <string>

namespace HTTPS
{
	class Exclusion
	{
	public:
		Exclusion(const char* pattern);

		bool IsMatch(char* url);

	private:
		std::string pattern;
	};
}