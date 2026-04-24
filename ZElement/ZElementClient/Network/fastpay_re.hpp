
#ifndef __GNET_FASTPAY_RE_HPP
#define __GNET_FASTPAY_RE_HPP

#include "rpcdefs.h"
#include "callid.hxx"
#include "state.hxx"


namespace GNET
{

class FastPay_Re : public GNET::Protocol
{
	#include "fastpay_re"

	void Process(Manager *manager, Manager::Session::ID sid)
	{
		// TODO
	}
};

};

#endif
