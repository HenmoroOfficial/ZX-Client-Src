
#ifndef __GNET_CRSSVRTEAMSTEAMBASE_RE_HPP
#define __GNET_CRSSVRTEAMSTEAMBASE_RE_HPP

#include "rpcdefs.h"
#include "callid.hxx"
#include "state.hxx"


namespace GNET
{

class CrssvrTeamsTeamBase_Re : public GNET::Protocol
{
	#include "crssvrteamsteambase_re"

	void Process(Manager *manager, Manager::Session::ID sid)
	{
		// TODO
	}
};

};

#endif
