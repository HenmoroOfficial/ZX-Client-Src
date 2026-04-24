
#ifndef __GNET_GSHOPNOTIFYSCHEME_HPP
#define __GNET_GSHOPNOTIFYSCHEME_HPP

#include "rpcdefs.h"
#include "callid.hxx"
#include "state.hxx"


namespace GNET
{

class GShopNotifyScheme : public GNET::Protocol
{
	#include "gshopnotifyscheme"

	void Process(Manager *manager, Manager::Session::ID sid)
	{
		// TODO
	}
};

};

#endif
