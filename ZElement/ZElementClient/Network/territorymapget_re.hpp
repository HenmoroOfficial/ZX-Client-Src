
#ifndef __GNET_TERRITORYMAPGET_RE_HPP
#define __GNET_TERRITORYMAPGET_RE_HPP

#include "rpcdefs.h"
#include "callid.hxx"
#include "state.hxx"

#include "territorybrief"

namespace GNET
{

class TerritoryMapGet_Re : public GNET::Protocol
{
	#include "territorymapget_re"

	void Process(Manager *manager, Manager::Session::ID sid)
	{
		// TODO
	}
};

};

#endif
