
#ifndef __GNET_CRSSVRTEAMSKICK_HPP
#define __GNET_CRSSVRTEAMSKICK_HPP

#include "rpcdefs.h"
#include "callid.hxx"
#include "state.hxx"


namespace GNET
{

class CrssvrTeamsKick : public GNET::Protocol
{
	#include "crssvrteamskick"

	void Process(Manager *manager, Manager::Session::ID sid)
	{
		// TODO
	}
};

};

#endif
