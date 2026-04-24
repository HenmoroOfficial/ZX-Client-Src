
#ifndef __GNET_FACTIONRECRUIT_RE_HPP
#define __GNET_FACTIONRECRUIT_RE_HPP

#include "rpcdefs.h"
#include "callid.hxx"
#include "state.hxx"


namespace GNET
{

class FactionRecruit_Re : public GNET::Protocol
{
	#include "factionrecruit_re"

	void Process(Manager *manager, Manager::Session::ID sid)
	{
		// TODO
	}
};

};

#endif
