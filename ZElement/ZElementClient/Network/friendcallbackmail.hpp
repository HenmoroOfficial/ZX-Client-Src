
#ifndef __GNET_FRIENDCALLBACKMAIL_HPP
#define __GNET_FRIENDCALLBACKMAIL_HPP

#include "rpcdefs.h"
#include "callid.hxx"
#include "state.hxx"


namespace GNET
{

class FriendCallbackMail : public GNET::Protocol
{
	#include "friendcallbackmail"

	void Process(Manager *manager, Manager::Session::ID sid)
	{
		// TODO
	}
};

};

#endif
