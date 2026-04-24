
#ifndef __GNET_FACTIONPKRAIDGETSCORE_RE_HPP
#define __GNET_FACTIONPKRAIDGETSCORE_RE_HPP

#include "rpcdefs.h"
#include "callid.hxx"
#include "state.hxx"

#include "gfactionpkraidrecord"

namespace GNET
{

class FactionPkRaidGetScore_Re : public GNET::Protocol
{
	#include "factionpkraidgetscore_re"

	void Process(Manager *manager, Manager::Session::ID sid)
	{
		// TODO
	}
};

};

#endif
