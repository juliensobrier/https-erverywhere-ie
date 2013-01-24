#pragma once

#include <sstream>


namespace bho
{
	class SoftwareUpdate
	{
	public:
		static const char* Check();
		static const char* Check(bool visible);
		static const char* Check(bool tab_visible, bool force);

	private:
		static void Parse(int result[4], const std::string& input);
		static bool LessThanVersion(const std::string& a,const std::string& b);
		static bool LessThanVersion(int* parsedA, int*  parsedB);
	};
}