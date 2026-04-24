
#ifndef __GNET_CRSSVRTEAMSNOTIFY_RE_HPP
#define __GNET_CRSSVRTEAMSNOTIFY_RE_HPP

#include "rpcdefs.h"
#include "callid.hxx"
#include "state.hxx"


namespace GNET
{

class CrssvrTeamsNotify_Re : public GNET::Protocol
{
	#include "crssvrteamsnotify_re"

	void Process(Manager *manager, Manager::Session::ID sid)
	{
		// TODO
	}
};

};

#endif
