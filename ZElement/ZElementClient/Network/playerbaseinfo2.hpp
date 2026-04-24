
#ifndef __GNET_PLAYERBASEINFO2_HPP
#define __GNET_PLAYERBASEINFO2_HPP

#include "rpcdefs.h"
#include "callid.hxx"
#include "state.hxx"


namespace GNET
{

class PlayerBaseInfo2 : public GNET::Protocol
{
	#include "playerbaseinfo2"

	void Process(Manager *manager, Manager::Session::ID sid)
	{
		// TODO
	}
};

};

#endif
