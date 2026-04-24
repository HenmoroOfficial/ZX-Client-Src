
#ifndef __GNET_INSTANCINGAPPOINT_HPP
#define __GNET_INSTANCINGAPPOINT_HPP

#include "rpcdefs.h"
#include "callid.hxx"
#include "state.hxx"


namespace GNET
{

class InstancingAppoint : public GNET::Protocol
{
	#include "instancingappoint"

	void Process(Manager *manager, Manager::Session::ID sid)
	{
		// TODO
	}
};

};

#endif
