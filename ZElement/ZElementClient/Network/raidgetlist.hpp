
#ifndef __GNET_RAIDGETLIST_HPP
#define __GNET_RAIDGETLIST_HPP

#include "rpcdefs.h"
#include "callid.hxx"
#include "state.hxx"


namespace GNET
{

class RaidGetList : public GNET::Protocol
{
	#include "raidgetlist"

	void Process(Manager *manager, Manager::Session::ID sid)
	{
		// TODO
	}
};

};

#endif
