
#ifndef __GNET_TEAMRAIDMAPPINGSUCCESS_HPP
#define __GNET_TEAMRAIDMAPPINGSUCCESS_HPP

#include "rpcdefs.h"
#include "callid.hxx"
#include "state.hxx"
#include "gcrssvrteamsdata"
#include "teamraidrolebrief"

namespace GNET
{

class TeamRaidMappingSuccess : public GNET::Protocol
{
	#include "teamraidmappingsuccess"

	void Process(Manager *manager, Manager::Session::ID sid)
	{
		// TODO
	}
};

};

#endif
