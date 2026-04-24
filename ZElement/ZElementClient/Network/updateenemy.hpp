
#ifndef __GNET_UPDATEENEMY_HPP
#define __GNET_UPDATEENEMY_HPP

#include "rpcdefs.h"
#include "callid.hxx"
#include "state.hxx"


namespace GNET
{

class UpdateEnemy : public GNET::Protocol
{
	#include "updateenemy"

	void Process(Manager *manager, Manager::Session::ID sid)
	{
		// TODO
	}
};

};

#endif
