
#ifndef __GNET_BATTLEJOIN_HPP
#define __GNET_BATTLEJOIN_HPP

#include "rpcdefs.h"
#include "callid.hxx"
#include "state.hxx"


namespace GNET
{

class BattleJoin : public GNET::Protocol
{
	#include "battlejoin"

	void Process(Manager *manager, Manager::Session::ID sid)
	{
		// TODO
	}
};

};

#endif
