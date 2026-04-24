
#ifndef __GNET_FACTIONAPPOINT_HPP
#define __GNET_FACTIONAPPOINT_HPP

#include "rpcdefs.h"
#include "callid.hxx"
#include "state.hxx"


namespace GNET
{

class FactionAppoint : public GNET::Protocol
{
	#include "factionappoint"

	void Process(Manager *manager, Manager::Session::ID sid)
	{
		// TODO
	}
};

};

#endif
