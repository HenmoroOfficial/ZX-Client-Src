
#ifndef __GNET_RAIDAPPOINT_RE_HPP
#define __GNET_RAIDAPPOINT_RE_HPP

#include "rpcdefs.h"
#include "callid.hxx"
#include "state.hxx"


namespace GNET
{

class RaidAppoint_Re : public GNET::Protocol
{
	#include "raidappoint_re"

	void Process(Manager *manager, Manager::Session::ID sid)
	{
		// TODO
	}
};

};

#endif
