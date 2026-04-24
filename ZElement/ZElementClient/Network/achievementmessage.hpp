
#ifndef __GNET_ACHIEVEMENTMESSAGE_HPP
#define __GNET_ACHIEVEMENTMESSAGE_HPP

#include "rpcdefs.h"
#include "callid.hxx"
#include "state.hxx"


namespace GNET
{

class AchievementMessage : public GNET::Protocol
{
	#include "achievementmessage"

	void Process(Manager *manager, Manager::Session::ID sid)
	{
		// TODO
	}
};

};

#endif
