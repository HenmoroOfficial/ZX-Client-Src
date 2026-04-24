
#ifndef __GNET_FACTIONPKRAIDGETSCORE_HPP
#define __GNET_FACTIONPKRAIDGETSCORE_HPP

#include "rpcdefs.h"
#include "callid.hxx"
#include "state.hxx"


namespace GNET
{

class FactionPkRaidGetScore : public GNET::Protocol
{
	#include "factionpkraidgetscore"

	void Process(Manager *manager, Manager::Session::ID sid)
	{
		// TODO
	}
};

};

#endif
