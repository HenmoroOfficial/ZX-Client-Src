
#ifndef __GNET_INSTANCINGSTARTNOTIFY_HPP
#define __GNET_INSTANCINGSTARTNOTIFY_HPP

#include "rpcdefs.h"
#include "callid.hxx"
#include "state.hxx"


namespace GNET
{

class InstancingStartNotify : public GNET::Protocol
{
	#include "instancingstartnotify"

	void Process(Manager *manager, Manager::Session::ID sid)
	{
		// TODO
	}
};

};

#endif
