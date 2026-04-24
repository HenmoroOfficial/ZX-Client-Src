
#ifndef __GNET_GETENEMIES_HPP
#define __GNET_GETENEMIES_HPP

#include "rpcdefs.h"
#include "callid.hxx"
#include "state.hxx"


namespace GNET
{

class GetEnemies : public GNET::Protocol
{
	#include "getenemies"

	void Process(Manager *manager, Manager::Session::ID sid)
	{
		// TODO
	}
};

};

#endif
