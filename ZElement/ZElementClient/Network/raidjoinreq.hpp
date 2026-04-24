
#ifndef __GNET_RAIDJOINREQ_HPP
#define __GNET_RAIDJOINREQ_HPP

#include "rpcdefs.h"
#include "callid.hxx"
#include "state.hxx"


namespace GNET
{

class RaidJoinReq : public GNET::Protocol
{
	#include "raidjoinreq"

	void Process(Manager *manager, Manager::Session::ID sid)
	{
		// TODO
	}
};

};

#endif
