
#ifndef __GNET_KINGDOMGETTITLE_HPP
#define __GNET_KINGDOMGETTITLE_HPP

#include "rpcdefs.h"
#include "callid.hxx"
#include "state.hxx"


namespace GNET
{

class KingdomGetTitle : public GNET::Protocol
{
	#include "kingdomgettitle"

	void Process(Manager *manager, Manager::Session::ID sid)
	{
		// TODO
	}
};

};

#endif
