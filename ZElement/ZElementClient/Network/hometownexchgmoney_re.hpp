
#ifndef __GNET_HOMETOWNEXCHGMONEY_RE_HPP
#define __GNET_HOMETOWNEXCHGMONEY_RE_HPP

#include "rpcdefs.h"
#include "callid.hxx"
#include "state.hxx"


namespace GNET
{

class HometownExchgMoney_Re : public GNET::Protocol
{
	#include "hometownexchgmoney_re"

	void Process(Manager *manager, Manager::Session::ID sid)
	{
		// TODO
	}
};

};

#endif
