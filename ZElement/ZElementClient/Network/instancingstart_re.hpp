
#ifndef __GNET_INSTANCINGSTART_RE_HPP
#define __GNET_INSTANCINGSTART_RE_HPP

#include "rpcdefs.h"
#include "callid.hxx"
#include "state.hxx"


namespace GNET
{

class InstancingStart_Re : public GNET::Protocol
{
	#include "instancingstart_re"

	void Process(Manager *manager, Manager::Session::ID sid)
	{
		// TODO
	}
};

};

#endif
