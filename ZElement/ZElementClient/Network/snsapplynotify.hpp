
#ifndef __GNET_SNSAPPLYNOTIFY_HPP
#define __GNET_SNSAPPLYNOTIFY_HPP

#include "rpcdefs.h"
#include "callid.hxx"
#include "state.hxx"


namespace GNET
{

class SNSApplyNotify : public GNET::Protocol
{
	#include "snsapplynotify"

	void Process(Manager *manager, Manager::Session::ID sid)
	{
		// TODO
	}
};

};

#endif
