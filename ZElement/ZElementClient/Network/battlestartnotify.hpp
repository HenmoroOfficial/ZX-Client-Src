
#ifndef __GNET_BATTLESTARTNOTIFY_HPP
#define __GNET_BATTLESTARTNOTIFY_HPP

#include "rpcdefs.h"
#include "callid.hxx"
#include "state.hxx"


namespace GNET
{

class BattleStartNotify : public GNET::Protocol
{
	#include "battlestartnotify"

	void Process(Manager *manager, Manager::Session::ID sid)
	{
		// TODO
	}
};

};

#endif
