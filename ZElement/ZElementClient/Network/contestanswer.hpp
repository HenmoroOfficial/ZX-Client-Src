
#ifndef __GNET_CONTESTANSWER_HPP
#define __GNET_CONTESTANSWER_HPP

#include "rpcdefs.h"
#include "callid.hxx"
#include "state.hxx"


namespace GNET
{

class ContestAnswer : public GNET::Protocol
{
	#include "contestanswer"

	void Process(Manager *manager, Manager::Session::ID sid)
	{
		// TODO
	}
};

};

#endif
