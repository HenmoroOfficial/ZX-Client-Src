
#ifndef __GNET_RAIDGETLIST_RE_HPP
#define __GNET_RAIDGETLIST_RE_HPP

#include "rpcdefs.h"
#include "callid.hxx"
#include "state.hxx"

#include "graidroominfo"

namespace GNET
{

class RaidGetList_Re : public GNET::Protocol
{
	#include "raidgetlist_re"

	void Process(Manager *manager, Manager::Session::ID sid)
	{
		// TODO
	}
};

};

#endif
