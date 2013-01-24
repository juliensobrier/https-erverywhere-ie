#pragma once

#include <string>


namespace HTTPS
{
	class Rule
	{
	public:
		Rule(const char* from, const char* to);
		~Rule();

		 char* Transform(char* url);

	private:
		char* from;
		char* to;
	};
}