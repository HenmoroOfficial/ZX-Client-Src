
#ifndef __GNET_RAIDKICK_HPP
#define __GNET_RAIDKICK_HPP

#include "rpcdefs.h"
#include "callid.hxx"
#include "state.hxx"


namespace GNET
{

class RaidKick : public GNET::Protocol
{
	#include "raidkick"

	void Process(Manager *manager, Manager::Session::ID sid)
	{
		// TODO
	}
};

};

#endif
