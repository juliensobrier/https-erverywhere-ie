//#include "WinUnit.h"
#include <cfixcc.h>

#include "Rule.h"
#include "debug.h"
#include <regex>

class Rule : public cfixcc::TestFixture
{
public:
  void Transform1() 
  {
	HTTPS::Rule* rule = new HTTPS::Rule("^(http://(www\\.)?|https://www\\.)1and1\\.com/", "https://order.1and1.com/");
	char* url = "http://www.1and1.com/";
	char* newURL = rule->Transform(url);
	CFIX_LOG(newURL);

	CFIX_LOG("New URL: %s", newURL);

	CFIX_ASSERT( strcmp("https://order.1and1.com/", newURL) == 0 );

	delete newURL;
	delete rule;
}
  
  void CreateRuleOK() 
  {
	HTTPS::Rule* rule = new HTTPS::Rule("foo", "bar");

	delete rule;
  }

  void Transform2() 
  {
	HTTPS::Rule* rule = new HTTPS::Rule("^(http://(www\\.)?|https://www\\.)1and1\\.com/", "https://order.1and1.com/");
	char* url = "http://www.1and1.com/index.html";
	char* newURL = rule->Transform(url);
	CFIX_ASSERT(newURL != NULL);

	CFIX_LOG("New URL: %s", newURL);

	CFIX_ASSERT( strcmp("https://order.1and1.com/index.html", newURL) == 0);

	delete newURL;
	delete rule;
  }

    void Transform3() 
  {
	HTTPS::Rule* rule = new HTTPS::Rule("^http://([^/:@]*)\\.4shared\\.com/", "https://$1.4shared.com/");
	char* url = "http://www.4shared.com/";
	char* newURL = rule->Transform(url);
	CFIX_ASSERT(newURL != NULL);

	CFIX_LOG("New URL: %s", newURL);

	CFIX_ASSERT( strcmp("https://www.4shared.com/", newURL) == 0);

	delete newURL;
	delete rule;
  }

	void TransformWikipediaOrg() 
  {
	HTTPS::Rule* rule = new HTTPS::Rule("^http://([^@:/]+\\.)?wik(ipedia|inews|isource|ibooks|iquote|iversity|tionary|imedia)\\.org/", "https://$1wik$2.org/");
	char* url = "http://www.wikipedia.org/";
	char* newURL = rule->Transform(url);
	CFIX_ASSERT(newURL != NULL);

	CFIX_LOG("New URL: %s", newURL);

	CFIX_ASSERT( strcmp("https://www.wikipedia.org/", newURL) == 0);

	delete newURL;
	delete rule;
  }

	void Test() 
	{
		std::string str("http://www.wikipedia.org/");
		std::regex fromRegex("http://([^a]+\\.)?wik(ipedia|imedia)\\.org/", std::regex_constants::icase);
		std::string fmt("https://$1wik$2.org/");
		std::string result = std::regex_replace(str, fromRegex, fmt);

		CFIX_ASSERT(strcmp("https://www.wikipedia.org/", result.c_str()) == 0);
	}

};

CFIXCC_BEGIN_CLASS( Rule )
  CFIXCC_METHOD( CreateRuleOK )
  CFIXCC_METHOD( Transform1 )
  CFIXCC_METHOD( Transform2 )
  CFIXCC_METHOD( Transform3 )
  //CFIXCC_METHOD( TransformWikipediaOrg )
  CFIXCC_METHOD( Test )
CFIXCC_END_CLASS()
