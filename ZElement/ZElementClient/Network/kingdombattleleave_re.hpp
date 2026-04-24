
#ifndef __GNET_KINGDOMBATTLELEAVE_RE_HPP
#define __GNET_KINGDOMBATTLELEAVE_RE_HPP

#include "rpcdefs.h"
#include "callid.hxx"
#include "state.hxx"


namespace GNET
{

class KingdomBattleLeave_Re : public GNET::Protocol
{
	#include "kingdombattleleave_re"

	void Process(Manager *manager, Manager::Session::ID sid)
	{
		// TODO
	}
};

};

#endif
