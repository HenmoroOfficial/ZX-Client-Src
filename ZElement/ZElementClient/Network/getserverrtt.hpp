
#ifndef __GNET_GETSERVERRTT_HPP
#define __GNET_GETSERVERRTT_HPP

#include "rpcdefs.h"
#include "callid.hxx"
#include "state.hxx"


namespace GNET
{

class GetServerRTT : public GNET::Protocol
{
	#include "getserverrtt"

	void Process(Manager *manager, Manager::Session::ID sid)
	{
		// TODO
	}
};

};

#endif
