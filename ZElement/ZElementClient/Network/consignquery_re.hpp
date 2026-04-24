
#ifndef __GNET_CONSIGNQUERY_RE_HPP
#define __GNET_CONSIGNQUERY_RE_HPP

#include "rpcdefs.h"
#include "callid.hxx"
#include "state.hxx"

#include "gconsignitem"

namespace GNET
{

class ConsignQuery_Re : public GNET::Protocol
{
	#include "consignquery_re"

	void Process(Manager *manager, Manager::Session::ID sid)
	{
		// TODO
	}
};

};

#endif
