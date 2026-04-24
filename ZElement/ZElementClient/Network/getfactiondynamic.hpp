
#ifndef __GNET_GETFACTIONDYNAMIC_HPP
#define __GNET_GETFACTIONDYNAMIC_HPP

#include "rpcdefs.h"
#include "callid.hxx"
#include "state.hxx"


namespace GNET
{

class GetFactionDynamic : public GNET::Protocol
{
	#include "getfactiondynamic"

	void Process(Manager *manager, Manager::Session::ID sid)
	{
		// TODO
	}
};

};

#endif
