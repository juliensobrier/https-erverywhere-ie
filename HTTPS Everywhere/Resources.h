#pragma once

#include "common.h"

namespace bho
{
	class Resources
	{
	public:
		static char* Extract(char* name, char* type);
		static int* bho::Resources::IEVersion();
		static int* bho::Resources::Version();
	};
}