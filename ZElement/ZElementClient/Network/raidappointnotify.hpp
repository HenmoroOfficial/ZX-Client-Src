
#ifndef __GNET_RAIDAPPOINTNOTIFY_HPP
#define __GNET_RAIDAPPOINTNOTIFY_HPP

#include "rpcdefs.h"
#include "callid.hxx"
#include "state.hxx"


namespace GNET
{

class RaidAppointNotify : public GNET::Protocol
{
	#include "raidappointnotify"

	void Process(Manager *manager, Manager::Session::ID sid)
	{
		// TODO
	}
};

};

#endif
