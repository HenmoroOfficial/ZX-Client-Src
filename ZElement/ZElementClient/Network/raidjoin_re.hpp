
#ifndef __GNET_RAIDJOIN_RE_HPP
#define __GNET_RAIDJOIN_RE_HPP

#include "rpcdefs.h"
#include "callid.hxx"
#include "state.hxx"


namespace GNET
{

class RaidJoin_Re : public GNET::Protocol
{
	#include "raidjoin_re"

	void Process(Manager *manager, Manager::Session::ID sid)
	{
		// TODO
	}
};

};

#endif
