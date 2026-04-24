
#ifndef __GNET_FASTPAY_HPP
#define __GNET_FASTPAY_HPP

#include "rpcdefs.h"
#include "callid.hxx"
#include "state.hxx"


namespace GNET
{

class FastPay : public GNET::Protocol
{
	#include "fastpay"

	void Process(Manager *manager, Manager::Session::ID sid)
	{
		// TODO
	}
};

};

#endif
