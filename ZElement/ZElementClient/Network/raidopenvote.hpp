
#ifndef __GNET_RAIDOPENVOTE_HPP
#define __GNET_RAIDOPENVOTE_HPP

#include "rpcdefs.h"
#include "callid.hxx"
#include "state.hxx"


namespace GNET
{

class RaidOpenVote : public GNET::Protocol
{
	#include "raidopenvote"

	void Process(Manager *manager, Manager::Session::ID sid)
	{
		// TODO
	}
};

};

#endif
