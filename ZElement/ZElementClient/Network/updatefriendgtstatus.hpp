
#ifndef __GNET_UPDATEFRIENDGTSTATUS_HPP
#define __GNET_UPDATEFRIENDGTSTATUS_HPP

#include "rpcdefs.h"
#include "callid.hxx"
#include "state.hxx"

#include "ggaterolegtstatus"

namespace GNET
{

class UpdateFriendGTStatus : public GNET::Protocol
{
	#include "updatefriendgtstatus"

	void Process(Manager *manager, Manager::Session::ID sid)
	{
		// TODO
	}
};

};

#endif
