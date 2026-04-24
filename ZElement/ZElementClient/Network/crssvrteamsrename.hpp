
#ifndef __GNET_CRSSVRTEAMSRENAME_HPP
#define __GNET_CRSSVRTEAMSRENAME_HPP

#include "rpcdefs.h"
#include "callid.hxx"
#include "state.hxx"


namespace GNET
{

class CrssvrTeamsRename : public GNET::Protocol
{
	#include "crssvrteamsrename"

	void Process(Manager *manager, Manager::Session::ID sid)
	{
		// TODO
	}
};

};

#endif
