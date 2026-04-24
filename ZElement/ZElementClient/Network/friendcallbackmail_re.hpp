
#ifndef __GNET_FRIENDCALLBACKMAIL_RE_HPP
#define __GNET_FRIENDCALLBACKMAIL_RE_HPP

#include "rpcdefs.h"
#include "callid.hxx"
#include "state.hxx"


namespace GNET
{

class FriendCallbackMail_Re : public GNET::Protocol
{
	#include "friendcallbackmail_re"

	void Process(Manager *manager, Manager::Session::ID sid)
	{
		// TODO
	}
};

};

#endif
