
#ifndef __GNET_CONSIGNLISTROLE_HPP
#define __GNET_CONSIGNLISTROLE_HPP

#include "rpcdefs.h"
#include "callid.hxx"
#include "state.hxx"


namespace GNET
{

class ConsignListRole : public GNET::Protocol
{
	#include "consignlistrole"

	void Process(Manager *manager, Manager::Session::ID sid)
	{
		// TODO
	}
};

};

#endif
