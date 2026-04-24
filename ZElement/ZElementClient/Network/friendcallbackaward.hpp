
#ifndef __GNET_FRIENDCALLBACKAWARD_HPP
#define __GNET_FRIENDCALLBACKAWARD_HPP

#include "rpcdefs.h"
#include "callid.hxx"
#include "state.hxx"


namespace GNET
{

class FriendCallbackAward : public GNET::Protocol
{
	#include "friendcallbackaward"

	void Process(Manager *manager, Manager::Session::ID sid)
	{
		// TODO
	}
};

};

#endif
