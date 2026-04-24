
#ifndef __GNET_GLOBALDROPREMAINBROADCAST_HPP
#define __GNET_GLOBALDROPREMAINBROADCAST_HPP

#include "rpcdefs.h"
#include "callid.hxx"
#include "state.hxx"


namespace GNET
{

class GlobalDropRemainBroadCast : public GNET::Protocol
{
	#include "globaldropremainbroadcast"

	void Process(Manager *manager, Manager::Session::ID sid)
	{
		// TODO
	}
};

};

#endif
