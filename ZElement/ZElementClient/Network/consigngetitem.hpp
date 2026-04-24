
#ifndef __GNET_CONSIGNGETITEM_HPP
#define __GNET_CONSIGNGETITEM_HPP

#include "rpcdefs.h"
#include "callid.hxx"
#include "state.hxx"


namespace GNET
{

class ConsignGetItem : public GNET::Protocol
{
	#include "consigngetitem"

	void Process(Manager *manager, Manager::Session::ID sid)
	{
		// TODO
	}
};

};

#endif
