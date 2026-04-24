
#ifndef __GNET_ACCLIENTTIMEOUT_HPP
#define __GNET_ACCLIENTTIMEOUT_HPP

#include "rpcdefs.h"
#include "callid.hxx"
#include "state.hxx"


namespace GNET
{

class ACClientTimeout : public GNET::Protocol
{
	#include "acclienttimeout"

	void Process(Manager *manager, Manager::Session::ID sid)
	{
		// TODO
	}
};

};

#endif
