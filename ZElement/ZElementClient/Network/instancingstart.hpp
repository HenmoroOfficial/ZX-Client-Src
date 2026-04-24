
#ifndef __GNET_INSTANCINGSTART_HPP
#define __GNET_INSTANCINGSTART_HPP

#include "rpcdefs.h"
#include "callid.hxx"
#include "state.hxx"


namespace GNET
{

class InstancingStart : public GNET::Protocol
{
	#include "instancingstart"

	void Process(Manager *manager, Manager::Session::ID sid)
	{
		// TODO
	}
};

};

#endif
