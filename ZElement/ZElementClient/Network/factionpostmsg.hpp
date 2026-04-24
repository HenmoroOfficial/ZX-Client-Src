
#ifndef __GNET_FACTIONPOSTMSG_HPP
#define __GNET_FACTIONPOSTMSG_HPP

#include "rpcdefs.h"
#include "callid.hxx"
#include "state.hxx"


namespace GNET
{

class FactionPostMsg : public GNET::Protocol
{
	#include "factionpostmsg"

	void Process(Manager *manager, Manager::Session::ID sid)
	{
		// TODO
	}
};

};

#endif
