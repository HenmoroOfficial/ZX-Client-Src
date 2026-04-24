
#ifndef __GNET_FACTIONPKRAIDINVITE_RE_HPP
#define __GNET_FACTIONPKRAIDINVITE_RE_HPP

#include "rpcdefs.h"
#include "callid.hxx"
#include "state.hxx"


namespace GNET
{

class FactionPkRaidInvite_Re : public GNET::Protocol
{
	#include "factionpkraidinvite_re"

	void Process(Manager *manager, Manager::Session::ID sid)
	{
		// TODO
	}
};

};

#endif
