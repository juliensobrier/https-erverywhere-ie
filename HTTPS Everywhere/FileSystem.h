#pragma once

#include "common.h"


namespace bho
{
	class FileSystem
	{
	public:
		static PSTR AppLocalLow();
		static PSTR BHOLocalLow();
		static char* BHOLocalLowFile(PSTR file);

		static void Clear();

		static int ToMultiBytes( char* &pStr, const WCHAR* pwStr, int len, BOOL IsEnd);

	private:
		static PSTR localLow;
		static PSTR bhoLocalLow;
	};
}