#include "debug.h"
#include "common.h"


void Debug::Log(const char* szError,...)
{
     char                        szBuff[2000];
     va_list                     vl;

     va_start(vl, szError);
	 vsnprintf_s(szBuff,sizeof(szBuff), _TRUNCATE, szError, vl);
     szBuff[sizeof(szBuff)-1]=0;

	 const int size =  2048 + strlen(BHO_FILENAME) + 1;
	 char* logMsg = new char[size];
	 strcpy_s(logMsg, size, BHO_FILENAME);
	 strcat_s(logMsg, size, " ");
	 strcat_s(logMsg, size, szBuff);
	 strcat_s(logMsg, size, "\n");

     OutputDebugString(LPCSTR(logMsg));
     va_end(vl);
	 delete logMsg;
}

void Debug::Warn(const char* szError,...)
{
     char                        szBuff[2000];
     va_list                     vl;

     va_start(vl, szError);
	 vsnprintf_s(szBuff,sizeof(szBuff), _TRUNCATE, szError, vl);
     szBuff[sizeof(szBuff)-1]=0;

	 char* warning = " WARNING: ";
	 const int size =  2048 + strlen(BHO_FILENAME) + strlen(warning) + 1;
	 char* errorMsg = new char[size];
	 strcpy_s(errorMsg, size, BHO_FILENAME);
	 strcat_s(errorMsg, size, warning);
	 strcat_s(errorMsg, size, szBuff);
	 strcat_s(errorMsg, size, "\n");
	 OutputDebugString(LPCSTR(errorMsg));

     va_end(vl);
	 delete errorMsg;
}

void Debug::Error(const char* szError,...)
{
     char                        szBuff[2000];
     va_list                     vl;


     va_start(vl, szError);
	 vsnprintf_s(szBuff,sizeof(szBuff), _TRUNCATE, szError, vl);
     szBuff[sizeof(szBuff)-1]=0;

	 char* error = " ERROR: ";
	 const int size =  2048 + strlen(BHO_FILENAME) + strlen(error) + 1;
	 char* errorMsg = new char[size];

	 strcpy_s(errorMsg, size, BHO_FILENAME);
	 strcat_s(errorMsg, size, error);
	 strcat_s(errorMsg, size, szBuff);
	 strcat_s(errorMsg, size, "\n");
	 OutputDebugString(LPCSTR(errorMsg));

     va_end(vl);
	 delete errorMsg;
}