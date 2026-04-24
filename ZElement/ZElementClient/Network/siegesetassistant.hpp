
#ifndef __GNET_SIEGESETASSISTANT_HPP
#define __GNET_SIEGESETASSISTANT_HPP

#include "rpcdefs.h"
#include "callid.hxx"
#include "state.hxx"


namespace GNET
{

class SiegeSetAssistant : public GNET::Protocol
{
	#include "siegesetassistant"

	void Process(Manager *manager, Manager::Session::ID sid)
	{
		// TODO
	}
};

};

#endif
