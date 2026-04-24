
#ifndef __GNET_FACTIONPKRAIDINVITE_HPP
#define __GNET_FACTIONPKRAIDINVITE_HPP

#include "rpcdefs.h"
#include "callid.hxx"
#include "state.hxx"


namespace GNET
{

class FactionPkRaidInvite : public GNET::Protocol
{
	#include "factionpkraidinvite"

	void Process(Manager *manager, Manager::Session::ID sid)
	{
		// TODO
	}
};

};

#endif
