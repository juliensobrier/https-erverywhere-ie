#pragma once

#define VC_EXTRALEAN		// Exclude rarely-used stuff from Windows headers

#include <iostream>
#include <conio.h>
#include <winsock2.h>



class Server
{
	public:
		BOOL Start(int portStart, int portEnd);
		void Stop();

	private:
		static UINT  MTServerThread(LPVOID pParam);
		static UINT  ClientThread(LPVOID pParam);
};

