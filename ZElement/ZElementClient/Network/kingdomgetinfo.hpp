
#ifndef __GNET_KINGDOMGETINFO_HPP
#define __GNET_KINGDOMGETINFO_HPP

#include "rpcdefs.h"
#include "callid.hxx"
#include "state.hxx"


namespace GNET
{

class KingdomGetInfo : public GNET::Protocol
{
	#include "kingdomgetinfo"

	void Process(Manager *manager, Manager::Session::ID sid)
	{
		// TODO
	}
};

};

#endif
