
#ifndef __GNET_SIEGEENTER_HPP
#define __GNET_SIEGEENTER_HPP

#include "rpcdefs.h"
#include "callid.hxx"
#include "state.hxx"


namespace GNET
{

class SiegeEnter : public GNET::Protocol
{
	#include "siegeenter"

	void Process(Manager *manager, Manager::Session::ID sid)
	{
		// TODO
	}
};

};

#endif
