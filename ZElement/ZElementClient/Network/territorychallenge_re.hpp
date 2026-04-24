
#ifndef __GNET_TERRITORYCHALLENGE_RE_HPP
#define __GNET_TERRITORYCHALLENGE_RE_HPP

#include "rpcdefs.h"
#include "callid.hxx"
#include "state.hxx"


namespace GNET
{

class TerritoryChallenge_Re : public GNET::Protocol
{
	#include "territorychallenge_re"

	void Process(Manager *manager, Manager::Session::ID sid)
	{
		// TODO
	}
};

};

#endif
