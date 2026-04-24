
#ifndef __GNET_FACTIONCREATE_HPP
#define __GNET_FACTIONCREATE_HPP

#include "rpcdefs.h"
#include "callid.hxx"
#include "state.hxx"

namespace GNET
{

class FactionCreate : public GNET::Protocol
{
	#include "factioncreate"

	void Process(Manager *manager, Manager::Session::ID sid)
	{
		// TODO
	}
};

};

#endif
