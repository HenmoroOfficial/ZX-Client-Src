
#ifndef __GNET_SELECTROLE_RE_HPP
#define __GNET_SELECTROLE_RE_HPP

#include "rpcdefs.h"
#include "callid.hxx"
#include "state.hxx"

#include "roleinfo"

namespace GNET
{

class SelectRole_Re : public GNET::Protocol
{
	#include "selectrole_re"

	void Process(Manager *manager, Manager::Session::ID sid)
	{
		// TODO
	}
};

};

#endif
