
#ifndef __GNET_FACTIONPKRAIDGETLIST_RE_HPP
#define __GNET_FACTIONPKRAIDGETLIST_RE_HPP

#include "rpcdefs.h"
#include "callid.hxx"
#include "state.hxx"

#include "gfactionpkroominfo"

namespace GNET
{

class FactionPkRaidGetList_Re : public GNET::Protocol
{
	#include "factionpkraidgetlist_re"

	void Process(Manager *manager, Manager::Session::ID sid)
	{
		// TODO
	}
};

};

#endif
