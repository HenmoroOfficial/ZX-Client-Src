
#ifndef __GNET_KINGISSUETASK_RE_HPP
#define __GNET_KINGISSUETASK_RE_HPP

#include "rpcdefs.h"
#include "callid.hxx"
#include "state.hxx"


namespace GNET
{

class KingIssueTask_Re : public GNET::Protocol
{
	#include "kingissuetask_re"

	void Process(Manager *manager, Manager::Session::ID sid)
	{
		// TODO
	}
};

};

#endif
