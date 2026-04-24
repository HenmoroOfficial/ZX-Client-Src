
#ifndef __GNET_CRSSVRTEAMSGETROLELIST_HPP
#define __GNET_CRSSVRTEAMSGETROLELIST_HPP

#include "rpcdefs.h"
#include "callid.hxx"
#include "state.hxx"


namespace GNET
{

class CrssvrTeamsGetRoleList : public GNET::Protocol
{
	#include "crssvrteamsgetrolelist"

	void Process(Manager *manager, Manager::Session::ID sid)
	{
		// TODO
	}
};

};

#endif
