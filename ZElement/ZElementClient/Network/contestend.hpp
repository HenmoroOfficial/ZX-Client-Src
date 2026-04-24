
#ifndef __GNET_CONTESTEND_HPP
#define __GNET_CONTESTEND_HPP

#include "rpcdefs.h"
#include "callid.hxx"
#include "state.hxx"


namespace GNET
{

class ContestEnd : public GNET::Protocol
{
	#include "contestend"

	void Process(Manager *manager, Manager::Session::ID sid)
	{
		// TODO
	}
};

};

#endif
