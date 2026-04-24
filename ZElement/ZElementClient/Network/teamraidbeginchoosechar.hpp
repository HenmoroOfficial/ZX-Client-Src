
#ifndef __GNET_TEAMRAIDBEGINCHOOSECHAR_HPP
#define __GNET_TEAMRAIDBEGINCHOOSECHAR_HPP

#include "rpcdefs.h"
#include "callid.hxx"
#include "state.hxx"

#include "teamraidmatchdata"

namespace GNET
{

class TeamRaidBeginChooseChar : public GNET::Protocol
{
	#include "teamraidbeginchoosechar"

	void Process(Manager *manager, Manager::Session::ID sid)
	{
		// TODO
	}
};

};

#endif
