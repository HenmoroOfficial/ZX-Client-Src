
#ifndef __GNET_CONSIGNSTARTERR_HPP
#define __GNET_CONSIGNSTARTERR_HPP

#include "rpcdefs.h"
#include "callid.hxx"
#include "state.hxx"


namespace GNET
{

class ConsignStartErr : public GNET::Protocol
{
	#include "consignstarterr"

	void Process(Manager *manager, Manager::Session::ID sid)
	{
		// TODO
	}
};

};

#endif
