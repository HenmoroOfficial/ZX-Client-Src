
#ifndef __GNET_BATTLEJOIN_RE_HPP
#define __GNET_BATTLEJOIN_RE_HPP

#include "rpcdefs.h"
#include "callid.hxx"
#include "state.hxx"


namespace GNET
{

class BattleJoin_Re : public GNET::Protocol
{
	#include "battlejoin_re"

	void Process(Manager *manager, Manager::Session::ID sid)
	{
		// TODO
	}
};

};

#endif
