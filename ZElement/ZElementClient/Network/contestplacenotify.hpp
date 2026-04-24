
#ifndef __GNET_CONTESTPLACENOTIFY_HPP
#define __GNET_CONTESTPLACENOTIFY_HPP

#include "rpcdefs.h"
#include "callid.hxx"
#include "state.hxx"

#include "contestplace"

namespace GNET
{

class ContestPlaceNotify : public GNET::Protocol
{
	#include "contestplacenotify"

	void Process(Manager *manager, Manager::Session::ID sid)
	{
		// TODO
	}
};

};

#endif
