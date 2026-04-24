
#ifndef __GNET_FACTIONPOSTMSG_RE_HPP
#define __GNET_FACTIONPOSTMSG_RE_HPP

#include "rpcdefs.h"
#include "callid.hxx"
#include "state.hxx"


namespace GNET
{

class FactionPostMsg_Re : public GNET::Protocol
{
	#include "factionpostmsg_re"

	void Process(Manager *manager, Manager::Session::ID sid)
	{
		// TODO
	}
};

};

#endif
