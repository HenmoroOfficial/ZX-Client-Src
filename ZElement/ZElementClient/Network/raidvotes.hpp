
#ifndef __GNET_RAIDVOTES_HPP
#define __GNET_RAIDVOTES_HPP

#include "rpcdefs.h"
#include "callid.hxx"
#include "state.hxx"


namespace GNET
{

class RaidVotes : public GNET::Protocol
{
	#include "raidvotes"

	void Process(Manager *manager, Manager::Session::ID sid)
	{
		// TODO
	}
};

};

#endif
