
#ifndef __GNET_HOSTILEDELETE_HPP
#define __GNET_HOSTILEDELETE_HPP

#include "rpcdefs.h"
#include "callid.hxx"
#include "state.hxx"


namespace GNET
{

class HostileDelete : public GNET::Protocol
{
	#include "hostiledelete"

	void Process(Manager *manager, Manager::Session::ID sid)
	{
		// TODO
	}
};

};

#endif
