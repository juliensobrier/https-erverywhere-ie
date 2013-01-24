#include <afxcoll.h>
#define _ATL_ALL_WARNINGS

//#include "WinUnit.h"
#include <cfixcc.h>

#include "Rules.h"
#include "debug.h"


class Rules : public cfixcc::TestFixture
{
public:
  void LoadAllRulesOK() 
  {
	HTTPS::Rules rules;
	HRESULT hr = rules.Add("default.rulesets");

	//WIN_ASSERT_TRUE( SUCCEEDED(hr) );
	CFIX_ASSERT_SUCCEEDED(hr);
	//Debug::Log("\n\n");
}
  
  void LoadAllRulesFail() 
  {
	HTTPS::Rules rules;
	HRESULT hr = rules.Add("default.rulesets.other");

	CFIX_ASSERT_FAILED(hr);
  }

  void HostMatchOK() 
  {
	HTTPS::Rules rules;
	HRESULT hr = rules.Add("default.rulesets");
	bool match = rules.HostMatch("www.google.com");

	CFIX_ASSERT(match);
  }

  void HostMatchFail() 
  {
		HTTPS::Rules rules;
		HRESULT hr = rules.Add("default.rulesets");
		bool match = rules.HostMatch("foo.www.google.com.sobrier.net");

		CFIX_ASSERT( match == false );
  }

  void TransformOK()
  {
	HTTPS::Rules rules;
	HRESULT hr = rules.Add("default.rulesets");

	char* url = "http://www.1and1.com/index.html";
	char* newURL = rules.Transform(url);
	CFIX_ASSERT( newURL != NULL );

	CFIX_LOG("New URL: %s", newURL);

	CFIX_ASSERT( strcmp("https://order.1and1.com/index.html", newURL) == 0 );

	delete newURL;
  }

  void ExclusionTransform()
  {
	HTTPS::Rules rules;
	HRESULT hr = rules.Add("default.rulesets");

	char* url = "http://static.via.me.s3.amazonaws.com/";
	char* newURL = rules.Transform(url);

	CFIX_ASSERT( newURL == NULL );
  }

  void ExclusionTransform1()
  {
	HTTPS::Rules rules;
	HRESULT hr = rules.Add("default.rulesets");

	char* url = "http://via.me.s3.amazonaws.com/";
	char* newURL = rules.Transform(url);

	CFIX_ASSERT( newURL != NULL );
  }
};

CFIXCC_BEGIN_CLASS( Rules )
  CFIXCC_METHOD( LoadAllRulesOK )
  CFIXCC_METHOD( LoadAllRulesFail )
  CFIXCC_METHOD( HostMatchOK )
  CFIXCC_METHOD( HostMatchFail )
  CFIXCC_METHOD( TransformOK )
  CFIXCC_METHOD( ExclusionTransform )
  CFIXCC_METHOD( ExclusionTransform1 )
CFIXCC_END_CLASS()

/*
BEGIN_TEST(LoadAllRulesOK)
{
	HTTPS::Rules rules;
	HRESULT hr = rules.AddAbsolute("C:\\depot\\research_team\\HTTS EveryWhere\\Release\\rules\\default.rulesets");

	WIN_ASSERT_TRUE( SUCCEEDED(hr) );
	Debug::Log("\n\n");
}
END_TEST

BEGIN_TEST(LoadAllRulesFail)
{
	HTTPS::Rules rules;
	HRESULT hr = rules.Add("default.rulesets");

	WIN_ASSERT_FALSE( SUCCEEDED(hr) );
	Debug::Log("\n\n");
}
END_TEST

BEGIN_TEST(HostMatchOK)
{
	HTTPS::Rules rules;
	HRESULT hr = rules.AddAbsolute("C:\\depot\\research_team\\HTTS EveryWhere\\Release\\rules\\default.rulesets");
	bool match = rules.IsMatch("www.google.com");

	WIN_ASSERT_TRUE( match );
	Debug::Log("\n\n");
}
END_TEST

BEGIN_TEST(HostMatchFail)
{
	HTTPS::Rules rules;
	HRESULT hr = rules.AddAbsolute("C:\\depot\\research_team\\HTTS EveryWhere\\Release\\rules\\default.rulesets");
	bool match = rules.IsMatch("foo.www.google.com.sobrier.net");

	WIN_ASSERT_FALSE( match );
	Debug::Log("\n\n");
}
END_TEST


BEGIN_TEST(TransformOK)
{
	HTTPS::Rules rules;
	HRESULT hr = rules.AddAbsolute("C:\\depot\\research_team\\HTTS EveryWhere\\Release\\rules\\default.rulesets");

	char* url = "http://www.1and1.com/index.html";
	char* newURL = rules.Transform(url);
	WIN_ASSERT_NOT_NULL(newURL);

	Debug::Log("New URL: %s", newURL);

	WIN_ASSERT_STRING_EQUAL("https://order.1and1.com/index.html", newURL);
	Debug::Log("\n\n");

	delete newURL;
}
END_TEST*/