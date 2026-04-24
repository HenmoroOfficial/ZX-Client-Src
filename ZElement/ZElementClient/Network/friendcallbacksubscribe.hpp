
#ifndef __GNET_FRIENDCALLBACKSUBSCRIBE_HPP
#define __GNET_FRIENDCALLBACKSUBSCRIBE_HPP

#include "rpcdefs.h"
#include "callid.hxx"
#include "state.hxx"


namespace GNET
{

class FriendCallbackSubscribe : public GNET::Protocol
{
	#include "friendcallbacksubscribe"

	void Process(Manager *manager, Manager::Session::ID sid)
	{
		// TODO
	}
};

};

#endif
