
#ifndef __GNET_RAIDQUIT_RE_HPP
#define __GNET_RAIDQUIT_RE_HPP

#include "rpcdefs.h"
#include "callid.hxx"
#include "state.hxx"


namespace GNET
{

class RaidQuit_Re : public GNET::Protocol
{
	#include "raidquit_re"

	void Process(Manager *manager, Manager::Session::ID sid)
	{
		// TODO
	}
};

};

#endif
