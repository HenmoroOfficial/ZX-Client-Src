
#ifndef __GNET_CONSIGNLISTROLE_RE_HPP
#define __GNET_CONSIGNLISTROLE_RE_HPP

#include "rpcdefs.h"
#include "callid.hxx"
#include "state.hxx"

#include "gconsignlistrolenode"

namespace GNET
{

class ConsignListRole_Re : public GNET::Protocol
{
	#include "consignlistrole_re"

	void Process(Manager *manager, Manager::Session::ID sid)
	{
		// TODO
	}
};

};

#endif
