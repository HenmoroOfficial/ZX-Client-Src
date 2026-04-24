
#ifndef __GNET_CONTESTINVITE_HPP
#define __GNET_CONTESTINVITE_HPP

#include "rpcdefs.h"
#include "callid.hxx"
#include "state.hxx"


namespace GNET
{

class ContestInvite : public GNET::Protocol
{
	#include "contestinvite"

	void Process(Manager *manager, Manager::Session::ID sid)
	{
		// TODO
	}
};

};

#endif
