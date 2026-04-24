
#ifndef __GNET_TEAMRAIDMAPPINGSUCCESS_RE_HPP
#define __GNET_TEAMRAIDMAPPINGSUCCESS_RE_HPP

#include "rpcdefs.h"
#include "callid.hxx"
#include "state.hxx"


namespace GNET
{

class TeamRaidMappingSuccess_Re : public GNET::Protocol
{
	#include "teamraidmappingsuccess_re"

	void Process(Manager *manager, Manager::Session::ID sid)
	{
		// TODO
	}
};

};

#endif
