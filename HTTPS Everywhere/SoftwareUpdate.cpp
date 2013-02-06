#include "SoftwareUpdate.h"
#include "Preferences.h"
#include "Resources.h"
#include "FileSystem.h"
#include "HTTP.h"
#include "debug.h"

#include <tinyxml2.h>
using namespace tinyxml2;


const char* bho::SoftwareUpdate::Check()
{
	return Check(true, false);
}

const char* bho::SoftwareUpdate::Check(bool tab_visible)
{
	return Check(tab_visible, false);
}

const char*  bho::SoftwareUpdate::Check(bool tab_visible, bool force)
{
	if (force == false)
	{
		if (tab_visible == false) 
		{
			Debug::Log("Tab should be visible");
			return NULL;
		}
		else
		{
			Debug::Log("Tab is visible");
		}


		time_t current;
		time(&current);

		int lastUpdate = bho::Preferences::getLastSoftwareUpdate();
		int interval = bho::Preferences::getSoftwareUpdateInterval();

		if (lastUpdate + interval > (long)current) {
			Debug::Log("Too early to check for a new version");
			return NULL;
		}
	}


	bho::HTTP::DownloadLow(_T(BHO_UPDATE), _T("version.xml"));
	char* path = bho::FileSystem::BHOLocalLowFile("version.xml");


	tinyxml2::XMLDocument doc;
	XMLError loaded = doc.LoadFile(path);
	if (loaded == XML_SUCCESS) {
		Debug::Log("XML loaded");

		XMLElement* pElem = doc.FirstChildElement( "version" )->FirstChildElement( "current" );
		if (pElem) {
			const char *pText=pElem->GetText();
			Debug::Log("New version found: %s", pText);

			int newVersion[4];
			std::string myString(pText);
			Parse(newVersion, myString);

			int* currentVersion = bho::Resources::Version();
			if (currentVersion == NULL) {
				Debug::Error("Could not find internal version number");
				return NULL;
			}

			Debug::Log("Current version: %i.%i.%i.%i", currentVersion[0], currentVersion[1], currentVersion[2], currentVersion[3]);
			Debug::Log("New version: %i.%i.%i.%i", newVersion[0], newVersion[1], newVersion[2], newVersion[3]);

			// Update registry
			time_t current;
			time(&current);
			bho::Preferences::setLastSoftwareUpdate((long)current);

			bool newer = LessThanVersion(currentVersion, newVersion);
			delete currentVersion;

			if (newer)
			{
				Debug::Log("Newer version available");

				XMLElement* pUrlElem = doc.FirstChildElement( "version" )->FirstChildElement( "url" );
				if (pUrlElem) 
				{
					const char *pUrlText = pUrlElem->GetText();
					Debug::Log("URL: %s", pUrlText);

					return pUrlText;
				}
				else
				{
					Debug::Error("Could not retrieve URL from XML file");
					return NULL;
				}
			}
			else
			{
				Debug::Log("Running the latest version");
				return NULL;
			}
		}
		else {
			Debug::Error("version not found");
			return NULL;
		}
	}
	else {
		Debug::Error("XML NOT loaded");
		return NULL;
	}

	return NULL;
}


// Works with 1-digit numbers only?
void bho::SoftwareUpdate::Parse(int result[4], const std::string& input)
{
    std::istringstream parser(input);
    parser >> result[0];
    for(int idx = 1; idx < 4; idx++)
    {
        parser.get(); //Skip period
        parser >> result[idx];
    }
}


bool bho::SoftwareUpdate::LessThanVersion(const std::string& a,const std::string& b)
{
    int parsedA[4], parsedB[4];
    Parse(parsedA, a);
    Parse(parsedB, b);
    return std::lexicographical_compare(parsedA, parsedA + 4, parsedB, parsedB + 4);
}

bool bho::SoftwareUpdate::LessThanVersion(int* parsedA, int*  parsedB)
{
    return std::lexicographical_compare(parsedA, parsedA + 4, parsedB, parsedB + 4);
}