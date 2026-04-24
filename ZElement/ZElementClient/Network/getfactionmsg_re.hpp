
#ifndef __GNET_GETFACTIONMSG_RE_HPP
#define __GNET_GETFACTIONMSG_RE_HPP

#include "rpcdefs.h"
#include "callid.hxx"
#include "state.hxx"

#include "factionmsgitem"

namespace GNET
{

class GetFactionMsg_Re : public GNET::Protocol
{
	#include "getfactionmsg_re"

	void Process(Manager *manager, Manager::Session::ID sid)
	{
		// TODO
	}
};

};

#endif
