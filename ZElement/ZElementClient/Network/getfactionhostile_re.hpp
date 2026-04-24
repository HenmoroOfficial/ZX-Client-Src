
#ifndef __GNET_GETFACTIONHOSTILE_RE_HPP
#define __GNET_GETFACTIONHOSTILE_RE_HPP

#include "rpcdefs.h"
#include "callid.hxx"
#include "state.hxx"

#include "hostileinfo"

namespace GNET
{

class GetFactionHostile_Re : public GNET::Protocol
{
	#include "getfactionhostile_re"

	void Process(Manager *manager, Manager::Session::ID sid)
	{
		// TODO
	}
};

};

#endif
