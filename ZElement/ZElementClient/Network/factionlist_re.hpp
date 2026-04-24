
#ifndef __GNET_FACTIONLIST_RE_HPP
#define __GNET_FACTIONLIST_RE_HPP

#include "rpcdefs.h"
#include "callid.hxx"
#include "state.hxx"

#include "memberinfo"
#include "gfamilybrief"
#include "familyskill"
#include "hostilefaction"

namespace GNET
{

class FactionList_Re : public GNET::Protocol
{
	#include "factionlist_re"

	void Process(Manager *manager, Manager::Session::ID sid)
	{
		// TODO
	}
};

};

#endif
