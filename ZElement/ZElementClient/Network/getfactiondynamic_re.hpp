
#ifndef __GNET_GETFACTIONDYNAMIC_RE_HPP
#define __GNET_GETFACTIONDYNAMIC_RE_HPP

#include "rpcdefs.h"
#include "callid.hxx"
#include "state.hxx"


namespace GNET
{

class GetFactionDynamic_Re : public GNET::Protocol
{
	#include "getfactiondynamic_re"

	void Process(Manager *manager, Manager::Session::ID sid)
	{
		// TODO
	}
};

};

#endif
