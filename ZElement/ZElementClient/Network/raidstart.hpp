
#ifndef __GNET_RAIDSTART_HPP
#define __GNET_RAIDSTART_HPP

#include "rpcdefs.h"
#include "callid.hxx"
#include "state.hxx"


namespace GNET
{

class RaidStart : public GNET::Protocol
{
	#include "raidstart"

	void Process(Manager *manager, Manager::Session::ID sid)
	{
		// TODO
	}
};

};

#endif
