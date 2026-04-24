
#ifndef __GNET_SIEGECHALLENGE_RE_HPP
#define __GNET_SIEGECHALLENGE_RE_HPP

#include "rpcdefs.h"
#include "callid.hxx"
#include "state.hxx"


namespace GNET
{

class SiegeChallenge_Re : public GNET::Protocol
{
	#include "siegechallenge_re"

	void Process(Manager *manager, Manager::Session::ID sid)
	{
		// TODO
	}
};

};

#endif
