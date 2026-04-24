
#ifndef __GNET_RESPONSE_HPP
#define __GNET_RESPONSE_HPP

#include "rpcdefs.h"
#include "callid.hxx"
#include "state.hxx"

namespace GNET
{

class Response : public GNET::Protocol
{
	#include "response"
	void Process(Manager *manager, Manager::Session::ID sid)
	{
	}
};

};

#endif
