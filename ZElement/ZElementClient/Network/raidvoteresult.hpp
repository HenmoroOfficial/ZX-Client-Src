
#ifndef __GNET_RAIDVOTERESULT_HPP
#define __GNET_RAIDVOTERESULT_HPP

#include "rpcdefs.h"
#include "callid.hxx"
#include "state.hxx"


namespace GNET
{

class RaidVoteResult : public GNET::Protocol
{
	#include "raidvoteresult"

	void Process(Manager *manager, Manager::Session::ID sid)
	{
		// TODO
	}
};

};

#endif
