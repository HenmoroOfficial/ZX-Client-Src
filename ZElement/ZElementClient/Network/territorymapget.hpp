
#ifndef __GNET_TERRITORYMAPGET_HPP
#define __GNET_TERRITORYMAPGET_HPP

#include "rpcdefs.h"
#include "callid.hxx"
#include "state.hxx"


namespace GNET
{

class TerritoryMapGet : public GNET::Protocol
{
	#include "territorymapget"

	void Process(Manager *manager, Manager::Session::ID sid)
	{
		// TODO
	}
};

};

#endif
