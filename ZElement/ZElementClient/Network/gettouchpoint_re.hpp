
#ifndef __GNET_GETTOUCHPOINT_RE_HPP
#define __GNET_GETTOUCHPOINT_RE_HPP

#include "rpcdefs.h"
#include "callid.hxx"
#include "state.hxx"


namespace GNET
{

class GetTouchPoint_Re : public GNET::Protocol
{
	#include "gettouchpoint_re"

	void Process(Manager *manager, Manager::Session::ID sid)
	{
		// TODO
	}
};

};

#endif
