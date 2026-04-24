
#ifndef __GNET_INSTANCINGJOIN_HPP
#define __GNET_INSTANCINGJOIN_HPP

#include "rpcdefs.h"
#include "callid.hxx"
#include "state.hxx"


namespace GNET
{

class InstancingJoin : public GNET::Protocol
{
	#include "instancingjoin"

	void Process(Manager *manager, Manager::Session::ID sid)
	{
	}
};

};

#endif
