
#ifndef __GNET_CRSSVRTEAMSSEARCH_HPP
#define __GNET_CRSSVRTEAMSSEARCH_HPP

#include "rpcdefs.h"
#include "callid.hxx"
#include "state.hxx"


namespace GNET
{

class CrssvrTeamsSearch : public GNET::Protocol
{
	#include "crssvrteamssearch"

	void Process(Manager *manager, Manager::Session::ID sid)
	{
		// TODO
	}
};

};

#endif
