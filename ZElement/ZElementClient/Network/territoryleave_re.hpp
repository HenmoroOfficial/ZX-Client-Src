
#ifndef __GNET_TERRITORYLEAVE_RE_HPP
#define __GNET_TERRITORYLEAVE_RE_HPP

#include "rpcdefs.h"
#include "callid.hxx"
#include "state.hxx"


namespace GNET
{

class TerritoryLeave_Re : public GNET::Protocol
{
	#include "territoryleave_re"

	void Process(Manager *manager, Manager::Session::ID sid)
	{
		// TODO
	}
};

};

#endif
