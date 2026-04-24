
#ifndef __GNET_FACTIONPKRAIDCHALLENGE_RE_HPP
#define __GNET_FACTIONPKRAIDCHALLENGE_RE_HPP

#include "rpcdefs.h"
#include "callid.hxx"
#include "state.hxx"


namespace GNET
{

class FactionPkRaidChallenge_Re : public GNET::Protocol
{
	#include "factionpkraidchallenge_re"

	void Process(Manager *manager, Manager::Session::ID sid)
	{
		// TODO
	}
};

};

#endif
