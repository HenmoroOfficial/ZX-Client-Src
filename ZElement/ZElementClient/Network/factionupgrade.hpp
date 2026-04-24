
#ifndef __GNET_FACTIONUPGRADE_HPP
#define __GNET_FACTIONUPGRADE_HPP

#include "rpcdefs.h"
#include "callid.hxx"
#include "state.hxx"


namespace GNET
{

class FactionUpgrade : public GNET::Protocol
{
	#include "factionupgrade"

	void Process(Manager *manager, Manager::Session::ID sid)
	{
		// TODO
	}
};

};

#endif
