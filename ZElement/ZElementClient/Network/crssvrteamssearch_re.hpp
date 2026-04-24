
#ifndef __GNET_CRSSVRTEAMSSEARCH_RE_HPP
#define __GNET_CRSSVRTEAMSSEARCH_RE_HPP

#include "rpcdefs.h"
#include "callid.hxx"
#include "state.hxx"

#include "gcrssvrteamsdata"

namespace GNET
{

class CrssvrTeamsSearch_Re : public GNET::Protocol
{
	#include "crssvrteamssearch_re"

	void Process(Manager *manager, Manager::Session::ID sid)
	{
		// TODO
	}
};

};

#endif
