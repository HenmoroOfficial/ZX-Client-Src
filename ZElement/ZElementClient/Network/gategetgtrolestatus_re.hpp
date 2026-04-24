
#ifndef __GNET_GATEGETGTROLESTATUS_RE_HPP
#define __GNET_GATEGETGTROLESTATUS_RE_HPP

#include "rpcdefs.h"
#include "callid.hxx"
#include "state.hxx"

#include "ggaterolegtstatus"

namespace GNET
{

class GateGetGTRoleStatus_Re : public GNET::Protocol
{
	#include "gategetgtrolestatus_re"

	void Process(Manager *manager, Manager::Session::ID sid)
	{
		// TODO
	}
};

};

#endif
