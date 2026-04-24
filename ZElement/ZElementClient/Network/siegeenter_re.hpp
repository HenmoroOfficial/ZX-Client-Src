
#ifndef __GNET_SIEGEENTER_RE_HPP
#define __GNET_SIEGEENTER_RE_HPP

#include "rpcdefs.h"
#include "callid.hxx"
#include "state.hxx"

#include "gfactionmaster"
#include "gfactionmaster"
#include "gfactionmaster"

namespace GNET
{

class SiegeEnter_Re : public GNET::Protocol
{
	#include "siegeenter_re"

	void Process(Manager *manager, Manager::Session::ID sid)
	{
		// TODO
	}
};

};

#endif
