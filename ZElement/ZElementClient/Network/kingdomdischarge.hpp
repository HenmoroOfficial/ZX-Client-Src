
#ifndef __GNET_KINGDOMDISCHARGE_HPP
#define __GNET_KINGDOMDISCHARGE_HPP

#include "rpcdefs.h"
#include "callid.hxx"
#include "state.hxx"


namespace GNET
{

class KingdomDischarge : public GNET::Protocol
{
	#include "kingdomdischarge"

	void Process(Manager *manager, Manager::Session::ID sid)
	{
		// TODO
	}
};

};

#endif
