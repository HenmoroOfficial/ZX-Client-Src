
#ifndef __GNET_FACTIONWITHDRAWBONUS_RE_HPP
#define __GNET_FACTIONWITHDRAWBONUS_RE_HPP

#include "rpcdefs.h"
#include "callid.hxx"
#include "state.hxx"


namespace GNET
{

class FactionWithDrawBonus_Re : public GNET::Protocol
{
	#include "factionwithdrawbonus_re"

	void Process(Manager *manager, Manager::Session::ID sid)
	{
		// TODO
	}
};

};

#endif
