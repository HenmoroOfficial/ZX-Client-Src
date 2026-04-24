
#ifndef __GNET_FRIENDCALLBACKAWARD_RE_HPP
#define __GNET_FRIENDCALLBACKAWARD_RE_HPP

#include "rpcdefs.h"
#include "callid.hxx"
#include "state.hxx"


namespace GNET
{

class FriendCallbackAward_Re : public GNET::Protocol
{
	#include "friendcallbackaward_re"

	void Process(Manager *manager, Manager::Session::ID sid)
	{
		// TODO
	}
};

};

#endif
