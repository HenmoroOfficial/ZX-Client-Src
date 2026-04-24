
#ifndef __GNET_CRSSVRTEAMSRECRUIT_HPP
#define __GNET_CRSSVRTEAMSRECRUIT_HPP

#include "rpcdefs.h"
#include "callid.hxx"
#include "state.hxx"


namespace GNET
{

class CrssvrTeamsRecruit : public GNET::Protocol
{
	#include "crssvrteamsrecruit"

	void Process(Manager *manager, Manager::Session::ID sid)
	{
		// TODO
	}
};

};

#endif
