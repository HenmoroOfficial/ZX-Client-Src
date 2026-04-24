
#ifndef __GNET_INSTANCINGENTER_HPP
#define __GNET_INSTANCINGENTER_HPP

#include "rpcdefs.h"
#include "callid.hxx"
#include "state.hxx"


namespace GNET
{

class InstancingEnter : public GNET::Protocol
{
	#include "instancingenter"

	void Process(Manager *manager, Manager::Session::ID sid)
	{
		// TODO
	}
};

};

#endif
