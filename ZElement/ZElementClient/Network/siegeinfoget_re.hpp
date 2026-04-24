
#ifndef __GNET_SIEGEINFOGET_RE_HPP
#define __GNET_SIEGEINFOGET_RE_HPP

#include "rpcdefs.h"
#include "callid.hxx"
#include "state.hxx"

#include "gcityinfo"
#include "gchallenger"

namespace GNET
{

class SiegeInfoGet_Re : public GNET::Protocol
{
	#include "siegeinfoget_re"

	void Process(Manager *manager, Manager::Session::ID sid)
	{
		// TODO
	}
};

};

#endif
