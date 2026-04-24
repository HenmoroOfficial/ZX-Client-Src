
#ifndef __GNET_TEAMRAIDACKTIMEOUT_HPP
#define __GNET_TEAMRAIDACKTIMEOUT_HPP

#include "rpcdefs.h"
#include "callid.hxx"
#include "state.hxx"


namespace GNET
{

class TeamRaidAckTimeout : public GNET::Protocol
{
	#include "teamraidacktimeout"

	void Process(Manager *manager, Manager::Session::ID sid)
	{
		// TODO
	}
};

};

#endif
