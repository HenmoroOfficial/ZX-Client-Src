
#ifndef __GNET_RAIDENTER_RE_HPP
#define __GNET_RAIDENTER_RE_HPP

#include "rpcdefs.h"
#include "callid.hxx"
#include "state.hxx"


namespace GNET
{

class RaidEnter_Re : public GNET::Protocol
{
	#include "raidenter_re"

	void Process(Manager *manager, Manager::Session::ID sid)
	{
		// TODO
	}
};

};

#endif
