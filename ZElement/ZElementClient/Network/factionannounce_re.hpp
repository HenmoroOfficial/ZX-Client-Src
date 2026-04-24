
#ifndef __GNET_FACTIONANNOUNCE_RE_HPP
#define __GNET_FACTIONANNOUNCE_RE_HPP

#include "rpcdefs.h"
#include "callid.hxx"
#include "state.hxx"


namespace GNET
{

class FactionAnnounce_Re : public GNET::Protocol
{
	#include "factionannounce_re"

	void Process(Manager *manager, Manager::Session::ID sid)
	{
		// TODO
	}
};

};

#endif
