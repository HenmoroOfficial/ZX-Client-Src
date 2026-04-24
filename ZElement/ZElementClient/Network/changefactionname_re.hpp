
#ifndef __GNET_CHANGEFACTIONNAME_RE_HPP
#define __GNET_CHANGEFACTIONNAME_RE_HPP

#include "rpcdefs.h"
#include "callid.hxx"
#include "state.hxx"


namespace GNET
{

class ChangeFactionName_Re : public GNET::Protocol
{
	#include "changefactionname_re"

	void Process(Manager *manager, Manager::Session::ID sid)
	{
		// TODO
	}
};

};

#endif
