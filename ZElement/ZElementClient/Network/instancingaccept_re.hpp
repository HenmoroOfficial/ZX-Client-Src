
#ifndef __GNET_INSTANCINGACCEPT_RE_HPP
#define __GNET_INSTANCINGACCEPT_RE_HPP

#include "rpcdefs.h"
#include "callid.hxx"
#include "state.hxx"


namespace GNET
{

class InstancingAccept_Re : public GNET::Protocol
{
	#include "instancingaccept_re"

	void Process(Manager *manager, Manager::Session::ID sid)
	{
		// TODO
	}
};

};

#endif
