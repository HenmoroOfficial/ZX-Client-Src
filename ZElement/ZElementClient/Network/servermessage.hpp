
#ifndef __GNET_SERVERMESSAGE_HPP
#define __GNET_SERVERMESSAGE_HPP

#include "rpcdefs.h"
#include "callid.hxx"
#include "state.hxx"


namespace GNET
{

class ServerMessage : public GNET::Protocol
{
	#include "servermessage"

	void Process(Manager *manager, Manager::Session::ID sid)
	{
		// TODO
	}
};

};

#endif
