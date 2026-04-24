
#ifndef __GNET_KINGDOMANNOUNCE_HPP
#define __GNET_KINGDOMANNOUNCE_HPP

#include "rpcdefs.h"
#include "callid.hxx"
#include "state.hxx"


namespace GNET
{

class KingdomAnnounce : public GNET::Protocol
{
	#include "kingdomannounce"

	void Process(Manager *manager, Manager::Session::ID sid)
	{
		// TODO
	}
};

};

#endif
