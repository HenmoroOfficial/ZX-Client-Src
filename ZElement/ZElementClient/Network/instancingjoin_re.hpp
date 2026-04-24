
#ifndef __GNET_INSTANCINGJOIN_RE_HPP
#define __GNET_INSTANCINGJOIN_RE_HPP

#include "rpcdefs.h"
#include "callid.hxx"
#include "state.hxx"


namespace GNET
{

class InstancingJoin_Re : public GNET::Protocol
{
	#include "instancingjoin_re"

	void Process(Manager *manager, Manager::Session::ID sid)
	{
		// TODO
	}
};

};

#endif
