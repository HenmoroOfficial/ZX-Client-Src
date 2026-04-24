
#ifndef __GNET_SIEGECHALLENGE_HPP
#define __GNET_SIEGECHALLENGE_HPP

#include "rpcdefs.h"
#include "callid.hxx"
#include "state.hxx"


namespace GNET
{

class SiegeChallenge : public GNET::Protocol
{
	#include "siegechallenge"

	void Process(Manager *manager, Manager::Session::ID sid)
	{
		// TODO
	}
};

};

#endif
