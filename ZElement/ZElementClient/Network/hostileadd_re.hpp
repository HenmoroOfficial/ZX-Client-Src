
#ifndef __GNET_HOSTILEADD_RE_HPP
#define __GNET_HOSTILEADD_RE_HPP

#include "rpcdefs.h"
#include "callid.hxx"
#include "state.hxx"


namespace GNET
{

class HostileAdd_Re : public GNET::Protocol
{
	#include "hostileadd_re"

	void Process(Manager *manager, Manager::Session::ID sid)
	{
		// TODO
	}
};

};

#endif
