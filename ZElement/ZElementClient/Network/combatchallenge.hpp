
#ifndef __GNET_COMBATCHALLENGE_HPP
#define __GNET_COMBATCHALLENGE_HPP

#include "rpcdefs.h"
#include "callid.hxx"
#include "state.hxx"


namespace GNET
{

class CombatChallenge : public GNET::Protocol
{
	#include "combatchallenge"

	void Process(Manager *manager, Manager::Session::ID sid)
	{
		// TODO
	}
};

};

#endif
