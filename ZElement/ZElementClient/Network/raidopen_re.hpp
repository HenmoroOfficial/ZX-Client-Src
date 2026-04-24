
#ifndef __GNET_RAIDOPEN_RE_HPP
#define __GNET_RAIDOPEN_RE_HPP

#include "rpcdefs.h"
#include "callid.hxx"
#include "state.hxx"


namespace GNET
{

class RaidOpen_Re : public GNET::Protocol
{
	#include "raidopen_re"

	void Process(Manager *manager, Manager::Session::ID sid)
	{
		// TODO
	}
};

};

#endif
