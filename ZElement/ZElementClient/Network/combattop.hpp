
#ifndef __GNET_COMBATTOP_HPP
#define __GNET_COMBATTOP_HPP

#include "rpcdefs.h"
#include "callid.hxx"
#include "state.hxx"


namespace GNET
{

class CombatTop : public GNET::Protocol
{
	#include "combattop"

	void Process(Manager *manager, Manager::Session::ID sid)
	{
		// TODO
	}
};

};

#endif
