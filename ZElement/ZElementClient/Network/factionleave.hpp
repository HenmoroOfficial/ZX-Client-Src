
#ifndef __GNET_FACTIONLEAVE_HPP
#define __GNET_FACTIONLEAVE_HPP

#include "rpcdefs.h"
#include "callid.hxx"
#include "state.hxx"


namespace GNET
{

class FactionLeave : public GNET::Protocol
{
	#include "factionleave"

	void Process(Manager *manager, Manager::Session::ID sid)
	{
		// TODO
	}
};

};

#endif
