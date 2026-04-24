
#ifndef __GNET_CRSSVRTEAMSGETROLENOTIFY_HPP
#define __GNET_CRSSVRTEAMSGETROLENOTIFY_HPP

#include "rpcdefs.h"
#include "callid.hxx"
#include "state.hxx"


namespace GNET
{

class CrssvrTeamsGetRoleNotify : public GNET::Protocol
{
	#include "crssvrteamsgetrolenotify"

	void Process(Manager *manager, Manager::Session::ID sid)
	{
		// TODO
	}
};

};

#endif
