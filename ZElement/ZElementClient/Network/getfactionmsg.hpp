
#ifndef __GNET_GETFACTIONMSG_HPP
#define __GNET_GETFACTIONMSG_HPP

#include "rpcdefs.h"
#include "callid.hxx"
#include "state.hxx"


namespace GNET
{

class GetFactionMsg : public GNET::Protocol
{
	#include "getfactionmsg"

	void Process(Manager *manager, Manager::Session::ID sid)
	{
		// TODO
	}
};

};

#endif
