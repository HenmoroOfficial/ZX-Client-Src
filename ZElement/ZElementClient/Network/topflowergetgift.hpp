
#ifndef __GNET_TOPFLOWERGETGIFT_HPP
#define __GNET_TOPFLOWERGETGIFT_HPP

#include "rpcdefs.h"
#include "callid.hxx"
#include "state.hxx"


namespace GNET
{

class TopFlowerGetGift : public GNET::Protocol
{
	#include "topflowergetgift"

	void Process(Manager *manager, Manager::Session::ID sid)
	{
		// TODO
	}
};

};

#endif
