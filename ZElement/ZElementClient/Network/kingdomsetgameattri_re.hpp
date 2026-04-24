
#ifndef __GNET_KINGDOMSETGAMEATTRI_RE_HPP
#define __GNET_KINGDOMSETGAMEATTRI_RE_HPP

#include "rpcdefs.h"
#include "callid.hxx"
#include "state.hxx"


namespace GNET
{

class KingdomSetGameAttri_Re : public GNET::Protocol
{
	#include "kingdomsetgameattri_re"

	void Process(Manager *manager, Manager::Session::ID sid)
	{
		// TODO
	}
};

};

#endif
