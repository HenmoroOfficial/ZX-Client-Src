
#ifndef __GNET_CRSSVRTEAMSGETTEAMBASE_HPP
#define __GNET_CRSSVRTEAMSGETTEAMBASE_HPP

#include "rpcdefs.h"
#include "callid.hxx"
#include "state.hxx"


namespace GNET
{

class CrssvrTeamsGetTeamBase : public GNET::Protocol
{
	#include "crssvrteamsgetteambase"

	void Process(Manager *manager, Manager::Session::ID sid)
	{
		// TODO
	}
};

};

#endif
