#include "Convert.h"

int bho::Convert::ToMultiBytes( char* &pStr, const WCHAR* pwStr, int len, BOOL IsEnd)
{
    //assert_pointer(pwStr, WCHAR) ;
    assert( len > 0 || len == -1 ) ;
    int nChars = WideCharToMultiByte(CP_ACP, 0, pwStr, len, NULL, 0, NULL, NULL);
    if (len == -1)
    {
        --nChars;
    }
    if (nChars == 0)
    {
        return 0;
    }
    if(IsEnd)
    {
        pStr = new char[nChars+1];
        ZeroMemory(pStr, nChars+1);
    }
    else
    {
        pStr = new char[nChars];
        ZeroMemory(pStr, nChars);
    }
    WideCharToMultiByte(CP_ACP, 0, pwStr, len, pStr, nChars, NULL, NULL);
    return nChars;
}