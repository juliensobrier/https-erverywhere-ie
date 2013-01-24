#include "Homepage.h"
#include "Preferences.h"


char* HTTPS::Homepage::GetHomePage()
{
	return bho::Preferences::Retrieve(HKEY_CURRENT_USER, "Software\\Microsoft\\Internet Explorer\\Main", "Start Page", "");

}

std::vector<char*>* HTTPS::Homepage::GetSecondaryHomePages(std::vector<char*>* defaultUrls)
{
	//std::vector<char*> pages;
	return bho::Preferences::RetrieveMultiple(HKEY_CURRENT_USER, "Software\\Microsoft\\Internet Explorer\\Main", "Secondary Start Pages", defaultUrls);
}

std::vector<char*>* HTTPS::Homepage::GetHomePages(std::vector<char*>* defaultUrls)
{
	char* home = GetHomePage();
	std::vector<char*>* urls = GetSecondaryHomePages(defaultUrls);
	urls->insert(urls->begin(), home);

	return urls;
}



void  HTTPS::Homepage::SetHomepages(std::vector<char*> urls)
{
	//
}

int HTTPS::Homepage::TransformHomepages(HTTPS::Rules rules)
{
	char* home = GetHomePage();
	if (home == NULL)
		return 0;

	int changes = 0;
	char* newUrl = rules.Transform(home);
	if (newUrl != NULL) 
	{
			Debug::Log("%s => %s", home, newUrl);
			bho::Preferences::Save(HKEY_CURRENT_USER, "Software\\Microsoft\\Internet Explorer\\Main","Start Page", newUrl);
			changes++;
	}

	delete home;

	std::vector<char*> defaultUrls;
	std::vector<char*>* urls = GetSecondaryHomePages(&defaultUrls);
	std::vector<char*> newUrls;
	int secondaryChanges = 0;
	
	Debug::Log("Size: %i", urls->size());
	if (urls->size() == 0)
		return changes;

	std::vector<char*>::iterator url;
	for(url = urls->begin() ; url < urls->end(); url++ )
	{
		Debug::Log("%s", *url);

		char* newUrl = rules.Transform(*url);
		if (newUrl != NULL) {
			newUrls.push_back(newUrl);
			secondaryChanges++;

			Debug::Log("%s => %s", *url, newUrl);
		}
		else
		{
			Debug::Log("no change: %s", *url);
			newUrls.push_back(*url);
		}
	}

	if (secondaryChanges > 0)
		bho::Preferences::SaveMultiple(HKEY_CURRENT_USER, "Software\\Microsoft\\Internet Explorer\\Main","Secondary Start Pages", newUrls);

	delete urls;

	return changes + secondaryChanges;
}