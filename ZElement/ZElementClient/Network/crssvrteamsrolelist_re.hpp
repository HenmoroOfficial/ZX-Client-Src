
#ifndef __GNET_CRSSVRTEAMSROLELIST_RE_HPP
#define __GNET_CRSSVRTEAMSROLELIST_RE_HPP

#include "rpcdefs.h"
#include "callid.hxx"
#include "state.hxx"

#include "gcrssvrteamsrole"

namespace GNET
{

class CrssvrTeamsRoleList_Re : public GNET::Protocol
{
	#include "crssvrteamsrolelist_re"

	void Process(Manager *manager, Manager::Session::ID sid)
	{
		// TODO
	}
};

};

#endif
