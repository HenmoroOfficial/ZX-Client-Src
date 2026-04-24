
#ifndef __GNET_BATTLELEAVE_HPP
#define __GNET_BATTLELEAVE_HPP

#include "rpcdefs.h"
#include "callid.hxx"
#include "state.hxx"


namespace GNET
{

class BattleLeave : public GNET::Protocol
{
	#include "battleleave"

	void Process(Manager *manager, Manager::Session::ID sid)
	{
		// TODO
	}
};

};

#endif
