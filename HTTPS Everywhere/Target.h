#pragma once


namespace HTTPS
{
	class Target
	{
	public:
		Target(const char* host);
		~Target();

		bool IsMatch(char* domain);
		char* GetHost();

	private:
		char* host;
		bool isSearch;

		//char* HostToRegex(const char* host);

		// TODO: move to some string helper function
		int wildcmp(const char *wild, const char *string);
	};
}