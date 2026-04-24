
#ifndef __GNET_FACTIONLIST_HPP
#define __GNET_FACTIONLIST_HPP

#include "rpcdefs.h"
#include "callid.hxx"
#include "state.hxx"


namespace GNET
{

class FactionList : public GNET::Protocol
{
	#include "factionlist"

	void Process(Manager *manager, Manager::Session::ID sid)
	{
		// TODO
	}
};

};

#endif
