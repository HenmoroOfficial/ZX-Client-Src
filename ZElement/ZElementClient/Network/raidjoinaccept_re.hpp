
#ifndef __GNET_RAIDJOINACCEPT_RE_HPP
#define __GNET_RAIDJOINACCEPT_RE_HPP

#include "rpcdefs.h"
#include "callid.hxx"
#include "state.hxx"


namespace GNET
{

class RaidJoinAccept_Re : public GNET::Protocol
{
	#include "raidjoinaccept_re"

	void Process(Manager *manager, Manager::Session::ID sid)
	{
		// TODO
	}
};

};

#endif
