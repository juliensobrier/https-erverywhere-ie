#pragma once

#include "Rules.h"


namespace HTTPS
{
	class Homepage
	{
	public:
		static char* GetHomePage();
		static std::vector<char*>* GetSecondaryHomePages(std::vector<char*>* defaultUrls);
		static std::vector<char*>* GetHomePages(std::vector<char*>* defaultUrls);

		static void SetHomepages(std::vector<char*> urls);
		static int TransformHomepages(HTTPS::Rules rules);
	};
}