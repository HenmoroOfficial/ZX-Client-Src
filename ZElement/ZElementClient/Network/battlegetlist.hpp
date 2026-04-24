
#ifndef __GNET_BATTLEGETLIST_HPP
#define __GNET_BATTLEGETLIST_HPP

#include "rpcdefs.h"
#include "callid.hxx"
#include "state.hxx"


namespace GNET
{

class BattleGetList : public GNET::Protocol
{
	#include "battlegetlist"

	void Process(Manager *manager, Manager::Session::ID sid)
	{
		// TODO
	}
};

};

#endif
