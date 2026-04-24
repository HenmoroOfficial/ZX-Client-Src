
#ifndef __GNET_FACTIONRESIGN_HPP
#define __GNET_FACTIONRESIGN_HPP

#include "rpcdefs.h"
#include "callid.hxx"
#include "state.hxx"


namespace GNET
{

class FactionResign : public GNET::Protocol
{
	#include "factionresign"

	void Process(Manager *manager, Manager::Session::ID sid)
	{
		// TODO
	}
};

};

#endif
