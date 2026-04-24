
#ifndef __GNET_GETTOUCHPOINT_HPP
#define __GNET_GETTOUCHPOINT_HPP

#include "rpcdefs.h"
#include "callid.hxx"
#include "state.hxx"


namespace GNET
{

class GetTouchPoint : public GNET::Protocol
{
	#include "gettouchpoint"

	void Process(Manager *manager, Manager::Session::ID sid)
	{
		// TODO
	}
};

};

#endif
