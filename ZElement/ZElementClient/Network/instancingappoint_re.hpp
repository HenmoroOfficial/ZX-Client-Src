
#ifndef __GNET_INSTANCINGAPPOINT_RE_HPP
#define __GNET_INSTANCINGAPPOINT_RE_HPP

#include "rpcdefs.h"
#include "callid.hxx"
#include "state.hxx"


namespace GNET
{

class InstancingAppoint_Re : public GNET::Protocol
{
	#include "instancingappoint_re"

	void Process(Manager *manager, Manager::Session::ID sid)
	{
		// TODO
	}
};

};

#endif
