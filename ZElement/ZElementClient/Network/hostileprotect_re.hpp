
#ifndef __GNET_HOSTILEPROTECT_RE_HPP
#define __GNET_HOSTILEPROTECT_RE_HPP

#include "rpcdefs.h"
#include "callid.hxx"
#include "state.hxx"


namespace GNET
{

class HostileProtect_Re : public GNET::Protocol
{
	#include "hostileprotect_re"

	void Process(Manager *manager, Manager::Session::ID sid)
	{
		// TODO
	}
};

};

#endif
