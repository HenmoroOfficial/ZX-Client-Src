
#ifndef __GNET_KINGDOMPOINTCHANGE_HPP
#define __GNET_KINGDOMPOINTCHANGE_HPP

#include "rpcdefs.h"
#include "callid.hxx"
#include "state.hxx"


namespace GNET
{

class KingdomPointChange : public GNET::Protocol
{
	#include "kingdompointchange"

	void Process(Manager *manager, Manager::Session::ID sid)
	{
		// TODO
	}
};

};

#endif
