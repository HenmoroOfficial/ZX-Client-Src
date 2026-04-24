
#ifndef __GNET_RAIDJOINACCEPT_HPP
#define __GNET_RAIDJOINACCEPT_HPP

#include "rpcdefs.h"
#include "callid.hxx"
#include "state.hxx"


namespace GNET
{

class RaidJoinAccept : public GNET::Protocol
{
	#include "raidjoinaccept"

	void Process(Manager *manager, Manager::Session::ID sid)
	{
		// TODO
	}
};

};

#endif
