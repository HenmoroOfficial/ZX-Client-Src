
#ifndef __GNET_GETPASSPORTCASHADD_HPP
#define __GNET_GETPASSPORTCASHADD_HPP

#include "rpcdefs.h"
#include "callid.hxx"
#include "state.hxx"


namespace GNET
{

class GetPassportCashAdd : public GNET::Protocol
{
	#include "getpassportcashadd"

	void Process(Manager *manager, Manager::Session::ID sid)
	{
		// TODO
	}
};

};

#endif
