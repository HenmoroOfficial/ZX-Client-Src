
#ifndef __GNET_PLAYERBASEINFO2_RE_HPP
#define __GNET_PLAYERBASEINFO2_RE_HPP

#include "rpcdefs.h"
#include "callid.hxx"
#include "state.hxx"

#include "grolebase"

namespace GNET
{

class PlayerBaseInfo2_Re : public GNET::Protocol
{
	#include "playerbaseinfo2_re"

	void Process(Manager *manager, Manager::Session::ID sid)
	{
		// TODO
	}
};

};

#endif
