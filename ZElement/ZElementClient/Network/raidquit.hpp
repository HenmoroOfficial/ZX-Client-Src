
#ifndef __GNET_RAIDQUIT_HPP
#define __GNET_RAIDQUIT_HPP

#include "rpcdefs.h"
#include "callid.hxx"
#include "state.hxx"


namespace GNET
{

class RaidQuit : public GNET::Protocol
{
	#include "raidquit"

	void Process(Manager *manager, Manager::Session::ID sid)
	{
		// TODO
	}
};

};

#endif
