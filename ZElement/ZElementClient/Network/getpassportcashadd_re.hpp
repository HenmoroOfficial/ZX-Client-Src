
#ifndef __GNET_GETPASSPORTCASHADD_RE_HPP
#define __GNET_GETPASSPORTCASHADD_RE_HPP

#include "rpcdefs.h"
#include "callid.hxx"
#include "state.hxx"


namespace GNET
{

class GetPassportCashAdd_Re : public GNET::Protocol
{
	#include "getpassportcashadd_re"

	void Process(Manager *manager, Manager::Session::ID sid)
	{
		// TODO
	}
};

};

#endif
