
#ifndef __GNET_CONSIGNQUERY_HPP
#define __GNET_CONSIGNQUERY_HPP

#include "rpcdefs.h"
#include "callid.hxx"
#include "state.hxx"


namespace GNET
{

class ConsignQuery : public GNET::Protocol
{
	#include "consignquery"

	void Process(Manager *manager, Manager::Session::ID sid)
	{
		// TODO
	}
};

};

#endif
