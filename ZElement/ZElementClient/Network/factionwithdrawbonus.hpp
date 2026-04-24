
#ifndef __GNET_FACTIONWITHDRAWBONUS_HPP
#define __GNET_FACTIONWITHDRAWBONUS_HPP

#include "rpcdefs.h"
#include "callid.hxx"
#include "state.hxx"


namespace GNET
{

class FactionWithDrawBonus : public GNET::Protocol
{
	#include "factionwithdrawbonus"

	void Process(Manager *manager, Manager::Session::ID sid)
	{
		// TODO
	}
};

};

#endif
