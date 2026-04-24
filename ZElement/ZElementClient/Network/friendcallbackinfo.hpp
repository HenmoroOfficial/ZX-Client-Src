
#ifndef __GNET_FRIENDCALLBACKINFO_HPP
#define __GNET_FRIENDCALLBACKINFO_HPP

#include "rpcdefs.h"
#include "callid.hxx"
#include "state.hxx"


namespace GNET
{

class FriendCallbackInfo : public GNET::Protocol
{
	#include "friendcallbackinfo"

	void Process(Manager *manager, Manager::Session::ID sid)
	{
		// TODO
	}
};

};

#endif
