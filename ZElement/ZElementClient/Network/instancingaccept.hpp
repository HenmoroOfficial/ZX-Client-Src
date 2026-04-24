
#ifndef __GNET_INSTANCINGACCEPT_HPP
#define __GNET_INSTANCINGACCEPT_HPP

#include "rpcdefs.h"
#include "callid.hxx"
#include "state.hxx"


namespace GNET
{

class InstancingAccept : public GNET::Protocol
{
	#include "instancingaccept"

	void Process(Manager *manager, Manager::Session::ID sid)
	{
		// TODO
	}
};

};

#endif
