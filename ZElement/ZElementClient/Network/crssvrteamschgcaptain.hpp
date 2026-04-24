
#ifndef __GNET_CRSSVRTEAMSCHGCAPTAIN_HPP
#define __GNET_CRSSVRTEAMSCHGCAPTAIN_HPP

#include "rpcdefs.h"
#include "callid.hxx"
#include "state.hxx"


namespace GNET
{

class CrssvrTeamsChgCaptain : public GNET::Protocol
{
	#include "crssvrteamschgcaptain"

	void Process(Manager *manager, Manager::Session::ID sid)
	{
		// TODO
	}
};

};

#endif
