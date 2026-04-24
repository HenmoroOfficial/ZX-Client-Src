
#ifndef __GNET_QUERYROLENAMEHISTORY_HPP
#define __GNET_QUERYROLENAMEHISTORY_HPP

#include "rpcdefs.h"
#include "callid.hxx"
#include "state.hxx"


namespace GNET
{

class QueryRolenameHistory : public GNET::Protocol
{
	#include "queryrolenamehistory"

	void Process(Manager *manager, Manager::Session::ID sid)
	{
		// TODO
	}
};

};

#endif
