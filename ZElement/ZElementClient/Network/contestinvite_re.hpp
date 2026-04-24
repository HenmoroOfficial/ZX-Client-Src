
#ifndef __GNET_CONTESTINVITE_RE_HPP
#define __GNET_CONTESTINVITE_RE_HPP

#include "rpcdefs.h"
#include "callid.hxx"
#include "state.hxx"


namespace GNET
{

class ContestInvite_Re : public GNET::Protocol
{
	#include "contestinvite_re"

	void Process(Manager *manager, Manager::Session::ID sid)
	{
		// TODO
	}
};

};

#endif
