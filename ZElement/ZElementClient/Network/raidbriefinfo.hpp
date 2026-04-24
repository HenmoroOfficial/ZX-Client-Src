
#ifndef __GNET_RAIDBRIEFINFO_HPP
#define __GNET_RAIDBRIEFINFO_HPP

#include "rpcdefs.h"
#include "callid.hxx"
#include "state.hxx"


namespace GNET
{

class RaidBriefInfo : public GNET::Protocol
{
	#include "raidbriefinfo"

	void Process(Manager *manager, Manager::Session::ID sid)
	{
		// TODO
	}
};

};

#endif
