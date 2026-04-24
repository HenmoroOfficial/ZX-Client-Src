
#ifndef __GNET_RAIDGETROOM_HPP
#define __GNET_RAIDGETROOM_HPP

#include "rpcdefs.h"
#include "callid.hxx"
#include "state.hxx"


namespace GNET
{

class RaidGetRoom : public GNET::Protocol
{
	#include "raidgetroom"

	void Process(Manager *manager, Manager::Session::ID sid)
	{
		// TODO
	}
};

};

#endif
