
#ifndef __GNET_FRIENDCALLBACKSUBSCRIBE_RE_HPP
#define __GNET_FRIENDCALLBACKSUBSCRIBE_RE_HPP

#include "rpcdefs.h"
#include "callid.hxx"
#include "state.hxx"


namespace GNET
{

class FriendCallbackSubscribe_Re : public GNET::Protocol
{
	#include "friendcallbacksubscribe_re"

	void Process(Manager *manager, Manager::Session::ID sid)
	{
		// TODO
	}
};

};

#endif
