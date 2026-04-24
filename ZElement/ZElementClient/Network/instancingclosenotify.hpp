
#ifndef __GNET_INSTANCINGCLOSENOTIFY_HPP
#define __GNET_INSTANCINGCLOSENOTIFY_HPP

#include "rpcdefs.h"
#include "callid.hxx"
#include "state.hxx"


namespace GNET
{

class InstancingCloseNotify : public GNET::Protocol
{
	#include "instancingclosenotify"

	void Process(Manager *manager, Manager::Session::ID sid)
	{
		// TODO
	}
};

};

#endif
