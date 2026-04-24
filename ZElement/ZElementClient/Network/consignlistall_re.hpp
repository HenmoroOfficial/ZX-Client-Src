
#ifndef __GNET_CONSIGNLISTALL_RE_HPP
#define __GNET_CONSIGNLISTALL_RE_HPP

#include "rpcdefs.h"
#include "callid.hxx"
#include "state.hxx"

#include "gconsignlistnode"

namespace GNET
{

class ConsignListAll_Re : public GNET::Protocol
{
	#include "consignlistall_re"

	void Process(Manager *manager, Manager::Session::ID sid)
	{
		// TODO
	}
};

};

#endif
