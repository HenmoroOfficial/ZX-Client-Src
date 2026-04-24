
#ifndef __GNET_KINGDOMKINGCHANGE_HPP
#define __GNET_KINGDOMKINGCHANGE_HPP

#include "rpcdefs.h"
#include "callid.hxx"
#include "state.hxx"


namespace GNET
{

class KingdomKingChange : public GNET::Protocol
{
	#include "kingdomkingchange"

	void Process(Manager *manager, Manager::Session::ID sid)
	{
		// TODO
	}
};

};

#endif
