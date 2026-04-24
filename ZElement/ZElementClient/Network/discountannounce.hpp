
#ifndef __GNET_DISCOUNTANNOUNCE_HPP
#define __GNET_DISCOUNTANNOUNCE_HPP

#include "rpcdefs.h"
#include "callid.hxx"
#include "state.hxx"

#include "merchantdiscount"

namespace GNET
{

class DiscountAnnounce : public GNET::Protocol
{
	#include "discountannounce"

	void Process(Manager *manager, Manager::Session::ID sid)
	{
		// TODO
	}
};

};

#endif
