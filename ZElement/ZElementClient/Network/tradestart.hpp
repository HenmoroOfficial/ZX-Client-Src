
#ifndef __GNET_TRADESTART_HPP
#define __GNET_TRADESTART_HPP

#include "rpcdefs.h"
#include "callid.hxx"
#include "state.hxx"


namespace GNET
{

class TradeStart : public GNET::Protocol
{
	#include "tradestart"

	void Process(Manager *manager, Manager::Session::ID sid)
	{
		// TODO
	}
};

};

#endif
