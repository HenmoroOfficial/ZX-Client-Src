
#ifndef __GNET_COMBATCONTROL_HPP
#define __GNET_COMBATCONTROL_HPP

#include "rpcdefs.h"
#include "callid.hxx"
#include "state.hxx"


namespace GNET
{

class CombatControl : public GNET::Protocol
{
	#include "combatcontrol"

	void Process(Manager *manager, Manager::Session::ID sid)
	{
		// TODO
	}
};

};

#endif
