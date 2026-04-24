
#ifndef __GNET_FACTIONANNOUNCE_HPP
#define __GNET_FACTIONANNOUNCE_HPP

#include "rpcdefs.h"
#include "callid.hxx"
#include "state.hxx"


namespace GNET
{

class FactionAnnounce : public GNET::Protocol
{
	#include "factionannounce"

	void Process(Manager *manager, Manager::Session::ID sid)
	{
		// TODO
	}
};

};

#endif
