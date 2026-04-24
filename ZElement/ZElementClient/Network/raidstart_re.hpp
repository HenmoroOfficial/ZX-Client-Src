
#ifndef __GNET_RAIDSTART_RE_HPP
#define __GNET_RAIDSTART_RE_HPP

#include "rpcdefs.h"
#include "callid.hxx"
#include "state.hxx"


namespace GNET
{

class RaidStart_Re : public GNET::Protocol
{
	#include "raidstart_re"

	void Process(Manager *manager, Manager::Session::ID sid)
	{
		// TODO
	}
};

};

#endif
