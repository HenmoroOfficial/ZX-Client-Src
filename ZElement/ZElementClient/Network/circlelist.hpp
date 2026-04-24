
#ifndef __GNET_CIRCLELIST_HPP
#define __GNET_CIRCLELIST_HPP

#include "rpcdefs.h"
#include "callid.hxx"
#include "state.hxx"


namespace GNET
{

class CircleList : public GNET::Protocol
{
	#include "circlelist"

	void Process(Manager *manager, Manager::Session::ID sid)
	{
		// TODO
	}
};

};

#endif
