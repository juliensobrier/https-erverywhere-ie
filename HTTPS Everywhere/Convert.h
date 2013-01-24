#pragma once

#include "common.h"


namespace bho
{
	class Convert
	{
	public:
		static int ToMultiBytes( char* &pStr, const WCHAR* pwStr, int len, BOOL IsEnd);
	};
}