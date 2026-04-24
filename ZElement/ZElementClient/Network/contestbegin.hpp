
#ifndef __GNET_CONTESTBEGIN_HPP
#define __GNET_CONTESTBEGIN_HPP

#include "rpcdefs.h"
#include "callid.hxx"
#include "state.hxx"


namespace GNET
{

class ContestBegin : public GNET::Protocol
{
	#include "contestbegin"

	void Process(Manager *manager, Manager::Session::ID sid)
	{
		// TODO
	}
};

};

#endif
