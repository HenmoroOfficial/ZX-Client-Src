
#ifndef __GNET_KINGDOMSETGAMEATTRI_HPP
#define __GNET_KINGDOMSETGAMEATTRI_HPP

#include "rpcdefs.h"
#include "callid.hxx"
#include "state.hxx"


namespace GNET
{

class KingdomSetGameAttri : public GNET::Protocol
{
	#include "kingdomsetgameattri"

	void Process(Manager *manager, Manager::Session::ID sid)
	{
		// TODO
	}
};

};

#endif
