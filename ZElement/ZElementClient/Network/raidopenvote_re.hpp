
#ifndef __GNET_RAIDOPENVOTE_RE_HPP
#define __GNET_RAIDOPENVOTE_RE_HPP

#include "rpcdefs.h"
#include "callid.hxx"
#include "state.hxx"


namespace GNET
{

class RaidOpenVote_Re : public GNET::Protocol
{
	#include "raidopenvote_re"

	void Process(Manager *manager, Manager::Session::ID sid)
	{
		// TODO
	}
};

};

#endif
