//#include "WinUnit.h"
#include <cfixcc.h>

#include "Securecookies.h"
#include "debug.h"


class Securecookies : public cfixcc::TestFixture
{
public:
  void MatchOK() 
  {
	HTTPS::Securecookies cookies("^.*\\.google\\.com$", ".+");
	bool match = cookies.IsMatch("www.google.com", "foo");

	CFIX_ASSERT(match);
  }

  void MatchFail() 
  {
	HTTPS::Securecookies cookies("^.*\\.google\\.org$", ".+");
	bool match = cookies.IsMatch("www.google.com", "foo");

	CFIX_ASSERT(match == false);
  }

    void MatchFail1() 
  {
	HTTPS::Securecookies cookies("^.*\\.google\\.com$", "bar");
	bool match = cookies.IsMatch("www.google.com", "foo");

	CFIX_ASSERT(match == false);
  }
};

CFIXCC_BEGIN_CLASS( Securecookies )
  CFIXCC_METHOD( MatchOK )
  CFIXCC_METHOD( MatchFail )
  CFIXCC_METHOD( MatchFail1 )
CFIXCC_END_CLASS()
