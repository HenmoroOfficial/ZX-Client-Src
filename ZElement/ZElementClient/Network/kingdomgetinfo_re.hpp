
#ifndef __GNET_KINGDOMGETINFO_RE_HPP
#define __GNET_KINGDOMGETINFO_RE_HPP

#include "rpcdefs.h"
#include "callid.hxx"
#include "state.hxx"

#include "kingdomclientinfo"

namespace GNET
{

class KingdomGetInfo_Re : public GNET::Protocol
{
	#include "kingdomgetinfo_re"

	void Process(Manager *manager, Manager::Session::ID sid)
	{
		// TODO
	}
};

};

#endif
