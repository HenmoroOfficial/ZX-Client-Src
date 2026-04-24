
#ifndef __GNET_COMBATCHALLENGE_RE_HPP
#define __GNET_COMBATCHALLENGE_RE_HPP

#include "rpcdefs.h"
#include "callid.hxx"
#include "state.hxx"


namespace GNET
{

class CombatChallenge_Re : public GNET::Protocol
{
	#include "combatchallenge_re"

	void Process(Manager *manager, Manager::Session::ID sid)
	{
		// TODO
	}
};

};

#endif
