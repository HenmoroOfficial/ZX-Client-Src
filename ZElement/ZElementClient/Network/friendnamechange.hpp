
#ifndef __GNET_FRIENDNAMECHANGE_HPP
#define __GNET_FRIENDNAMECHANGE_HPP

#include "rpcdefs.h"
#include "callid.hxx"
#include "state.hxx"


namespace GNET
{

class FriendNameChange : public GNET::Protocol
{
	#include "friendnamechange"

	void Process(Manager *manager, Manager::Session::ID sid)
	{
		// TODO
	}
};

};

#endif
