
#ifndef __GNET_FACTIONRECRUIT_HPP
#define __GNET_FACTIONRECRUIT_HPP

#include "rpcdefs.h"
#include "callid.hxx"
#include "state.hxx"


namespace GNET
{

class FactionRecruit : public GNET::Protocol
{
	#include "factionrecruit"

	void Process(Manager *manager, Manager::Session::ID sid)
	{
		// TODO
	}
};

};

#endif
