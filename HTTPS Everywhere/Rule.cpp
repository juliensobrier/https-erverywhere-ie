#include "Rule.h"
#include "debug.h"
#include <regex>


HTTPS::Rule::Rule(const char* aFrom, const char* aTo)
	:from(NULL), to(NULL) 
{ 
	if (aFrom != NULL)
	{
		from = new char[strlen(aFrom) + 1];
		strcpy_s(from, strlen(aFrom) + 1, aFrom);
	}

	if (aTo != NULL)
	{
		to = new char[strlen(aTo) + 1];
		strcpy_s(to, strlen(aTo) + 1, aTo);
	}
}

HTTPS::Rule::~Rule()
{
	if (from != NULL)
		delete from;

	if (to != NULL)
		delete to;
}


char* HTTPS::Rule::Transform(char* url)
{
	if(this->from == NULL || this->to == NULL)
		return NULL;

	try
	{
		std::regex fromRegex(this->from, std::regex_constants::icase);
		/*const std::regex esc("\\.");
		const std::string rep("\\\\.");
		std::string regexp = std::regex_replace(std::string(this->from), esc, rep);
		std::regex fromRegex(regexp, std::regex_constants::icase); // Regex should be saved for later*/

		if (! std::regex_search(std::string(url), fromRegex))
		{
			//Debug::Log("%s does not match %s", url, this->from);
			return NULL;
		}

		Debug::Log("%s matches %s (%s)", url, this->from, this->to);

		std::string result;
		std::string str(url);
		std::string fmt(this->to);
		//std::regex_constants::match_flag_type flag =  std::tr1::regex_constants::icase; 
		//std::regex_constants::match_flag_type fonly = std::regex_constants::format_no_copy; 
		/*const std::regex esc("\\.");
		const std::string rep("\\\\.");
		std::string regexp = std::regex_replace(std::string(this->from), esc, rep);
		std::regex fromRegex2(regexp, std::regex_constants::icase);*/
		//result = std::regex_replace(str, fromRegex, fmt, std::tr1::regex_constants::match_not_null);
		//result = std::regex_replace(str, std::regex(this->from, std::regex_constants::icase | std::regex_constants::format_first_only), fmt, std::tr1::regex_constants::format_first_only);
		result = std::regex_replace(str, fromRegex, fmt);

		if(result.compare(str) == 0) {
			// URLs are actually the same
			return NULL;
		}

		int size = result.length() + 1;
		char* newUrl = new char[size];
		strcpy_s(newUrl, size, result.c_str());

		return newUrl;
	}
	catch(std::exception& e)
	{
		Debug::Error("Error inside Rule->Transform: %s", e.what());
		return NULL;
	}
	catch(...)
	{
		Debug::Error("Error inside Rule->Transform: SEH errror");
		return NULL;
	}
}
