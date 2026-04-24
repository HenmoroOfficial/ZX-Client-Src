
#ifndef __GNET_GLOBALDROPBROADCAST_HPP
#define __GNET_GLOBALDROPBROADCAST_HPP

#include "rpcdefs.h"
#include "callid.hxx"
#include "state.hxx"


namespace GNET
{

class GlobalDropBroadCast : public GNET::Protocol
{
	#include "globaldropbroadcast"

	void Process(Manager *manager, Manager::Session::ID sid)
	{
		// TODO
	}
};

};

#endif
