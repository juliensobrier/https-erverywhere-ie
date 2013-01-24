#pragma once

class Debug
{
	public:
		static void Log(const char* szError,...);
		static void Warn(const char* szError,...);
		static void Error(const char* szError,...);
};