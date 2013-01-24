//#include "WinUnit.h"
#include <cfixcc.h>

#include "Exclusion.h"
#include "debug.h"


class Exclusion : public cfixcc::TestFixture
{
public:
  void MatchOK() 
  {
	HTTPS::Exclusion exclusion("^http://(?:www\\.)?afsp\\.org/indiana$");
	bool match = exclusion.IsMatch("http://www.afsp.org/indiana");

	CFIX_ASSERT(match);
  }

  void MatchFail() 
  {
	HTTPS::Exclusion exclusion("^http://(?:www\\.)?afsp\\.org/$");
	bool match = exclusion.IsMatch("http://www.afsp.org/indiana");

	CFIX_ASSERT(match == false);
  }
};

CFIXCC_BEGIN_CLASS( Exclusion )
  CFIXCC_METHOD( MatchOK )
  CFIXCC_METHOD( MatchFail )
CFIXCC_END_CLASS()
