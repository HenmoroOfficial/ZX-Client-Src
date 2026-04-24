
#ifndef __GNET_CIRCLENOTIFY_HPP
#define __GNET_CIRCLENOTIFY_HPP

#include "rpcdefs.h"
#include "callid.hxx"
#include "state.hxx"


namespace GNET
{

class CircleNotify : public GNET::Protocol
{
	#include "circlenotify"

	void Process(Manager *manager, Manager::Session::ID sid)
	{
		// TODO
	}
};

};

#endif
