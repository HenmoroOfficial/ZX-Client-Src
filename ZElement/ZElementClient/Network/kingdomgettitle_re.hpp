
#ifndef __GNET_KINGDOMGETTITLE_RE_HPP
#define __GNET_KINGDOMGETTITLE_RE_HPP

#include "rpcdefs.h"
#include "callid.hxx"
#include "state.hxx"


namespace GNET
{

class KingdomGetTitle_Re : public GNET::Protocol
{
	#include "kingdomgettitle_re"

	void Process(Manager *manager, Manager::Session::ID sid)
	{
		// TODO
	}
};

};

#endif
