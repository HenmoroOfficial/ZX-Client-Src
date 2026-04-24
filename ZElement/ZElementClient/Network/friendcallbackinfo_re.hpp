
#ifndef __GNET_FRIENDCALLBACKINFO_RE_HPP
#define __GNET_FRIENDCALLBACKINFO_RE_HPP

#include "rpcdefs.h"
#include "callid.hxx"
#include "state.hxx"

#include "gcallbackinfo"

namespace GNET
{

class FriendCallbackInfo_Re : public GNET::Protocol
{
	#include "friendcallbackinfo_re"

	void Process(Manager *manager, Manager::Session::ID sid)
	{
		// TODO
	}
};

};

#endif
