
#ifndef __GNET_INSTANCINGAVAILABLELIST_RE_HPP
#define __GNET_INSTANCINGAVAILABLELIST_RE_HPP

#include "rpcdefs.h"
#include "callid.hxx"
#include "state.hxx"


namespace GNET
{

class InstancingAvailableList_Re : public GNET::Protocol
{
	#include "instancingavailablelist_re"

	void Process(Manager *manager, Manager::Session::ID sid)
	{
		// TODO
	}
};

};

#endif
