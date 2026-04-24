
#ifndef __GNET_CONSIGNLISTALL_HPP
#define __GNET_CONSIGNLISTALL_HPP

#include "rpcdefs.h"
#include "callid.hxx"
#include "state.hxx"


namespace GNET
{

class ConsignListAll : public GNET::Protocol
{
	#include "consignlistall"

	void Process(Manager *manager, Manager::Session::ID sid)
	{
		// TODO
	}
};

};

#endif
