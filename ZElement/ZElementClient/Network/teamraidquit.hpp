
#ifndef __GNET_TEAMRAIDQUIT_HPP
#define __GNET_TEAMRAIDQUIT_HPP

#include "rpcdefs.h"
#include "callid.hxx"
#include "state.hxx"


namespace GNET
{

class TeamRaidQuit : public GNET::Protocol
{
	#include "teamraidquit"

	void Process(Manager *manager, Manager::Session::ID sid)
	{
		// TODO
	}
};

};

#endif
