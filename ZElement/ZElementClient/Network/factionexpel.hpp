
#ifndef __GNET_FACTIONEXPEL_HPP
#define __GNET_FACTIONEXPEL_HPP

#include "rpcdefs.h"
#include "callid.hxx"
#include "state.hxx"


namespace GNET
{

class FactionExpel : public GNET::Protocol
{
	#include "factionexpel"

	void Process(Manager *manager, Manager::Session::ID sid)
	{
		// TODO
	}
};

};

#endif
