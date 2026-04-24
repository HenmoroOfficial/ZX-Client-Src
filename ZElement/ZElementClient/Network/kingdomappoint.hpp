
#ifndef __GNET_KINGDOMAPPOINT_HPP
#define __GNET_KINGDOMAPPOINT_HPP

#include "rpcdefs.h"
#include "callid.hxx"
#include "state.hxx"


namespace GNET
{

class KingdomAppoint : public GNET::Protocol
{
	#include "kingdomappoint"

	void Process(Manager *manager, Manager::Session::ID sid)
	{
		// TODO
	}
};

};

#endif
