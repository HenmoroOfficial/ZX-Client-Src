
#ifndef __GNET_TEAMRAIDUPDATECHAR_HPP
#define __GNET_TEAMRAIDUPDATECHAR_HPP

#include "rpcdefs.h"
#include "callid.hxx"
#include "state.hxx"

#include "teamraidroledata"

namespace GNET
{

class TeamRaidUpdateChar : public GNET::Protocol
{
	#include "teamraidupdatechar"

	void Process(Manager *manager, Manager::Session::ID sid)
	{
		// TODO
	}
};

};

#endif
