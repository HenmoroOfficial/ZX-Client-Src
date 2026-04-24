
#ifndef __GNET_CLAIMCIRCLEGRADBONUS_HPP
#define __GNET_CLAIMCIRCLEGRADBONUS_HPP

#include "rpcdefs.h"
#include "callid.hxx"
#include "state.hxx"


namespace GNET
{

class ClaimCircleGradBonus : public GNET::Protocol
{
	#include "claimcirclegradbonus"

	void Process(Manager *manager, Manager::Session::ID sid)
	{
		// TODO
	}
};

};

#endif
