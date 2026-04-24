
#ifndef __GNET_RAIDAPPOINT_HPP
#define __GNET_RAIDAPPOINT_HPP

#include "rpcdefs.h"
#include "callid.hxx"
#include "state.hxx"


namespace GNET
{

class RaidAppoint : public GNET::Protocol
{
	#include "raidappoint"

	void Process(Manager *manager, Manager::Session::ID sid)
	{
		// TODO
	}
};

};

#endif
