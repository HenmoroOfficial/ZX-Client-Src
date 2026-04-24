
#ifndef __GNET_FACTIONPKRAIDSTOPFIGHT_HPP
#define __GNET_FACTIONPKRAIDSTOPFIGHT_HPP

#include "rpcdefs.h"
#include "callid.hxx"
#include "state.hxx"


namespace GNET
{

class FactionPkRaidStopFight : public GNET::Protocol
{
	#include "factionpkraidstopfight"

	void Process(Manager *manager, Manager::Session::ID sid)
	{
		// TODO
	}
};

};

#endif
