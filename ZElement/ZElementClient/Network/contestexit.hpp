
#ifndef __GNET_CONTESTEXIT_HPP
#define __GNET_CONTESTEXIT_HPP

#include "rpcdefs.h"
#include "callid.hxx"
#include "state.hxx"


namespace GNET
{

class ContestExit : public GNET::Protocol
{
	#include "contestexit"

	void Process(Manager *manager, Manager::Session::ID sid)
	{
		// TODO
	}
};

};

#endif
