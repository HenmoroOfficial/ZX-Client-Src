
#ifndef __GNET_TEAMRAIDAPPLYNOTICE_HPP
#define __GNET_TEAMRAIDAPPLYNOTICE_HPP

#include "rpcdefs.h"
#include "callid.hxx"
#include "state.hxx"


namespace GNET
{

class TeamRaidApplyNotice : public GNET::Protocol
{
	#include "teamraidapplynotice"

	void Process(Manager *manager, Manager::Session::ID sid)
	{
		// TODO
	}
};

};

#endif
