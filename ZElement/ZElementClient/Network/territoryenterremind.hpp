
#ifndef __GNET_TERRITORYENTERREMIND_HPP
#define __GNET_TERRITORYENTERREMIND_HPP

#include "rpcdefs.h"
#include "callid.hxx"
#include "state.hxx"


namespace GNET
{

class TerritoryEnterRemind : public GNET::Protocol
{
	#include "territoryenterremind"

	void Process(Manager *manager, Manager::Session::ID sid)
	{
		// TODO
	}
};

};

#endif
