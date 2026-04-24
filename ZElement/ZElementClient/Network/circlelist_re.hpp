
#ifndef __GNET_CIRCLELIST_RE_HPP
#define __GNET_CIRCLELIST_RE_HPP

#include "rpcdefs.h"
#include "callid.hxx"
#include "state.hxx"

#include "circlememberinfo"
#include "gcirclehistory"

namespace GNET
{

class CircleList_Re : public GNET::Protocol
{
	#include "circlelist_re"

	void Process(Manager *manager, Manager::Session::ID sid)
	{
		// TODO
	}
};

};

#endif
