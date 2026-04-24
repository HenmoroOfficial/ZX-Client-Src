
#ifndef __GNET_KINGGETREWARD_RE_HPP
#define __GNET_KINGGETREWARD_RE_HPP

#include "rpcdefs.h"
#include "callid.hxx"
#include "state.hxx"


namespace GNET
{

class KingGetReward_Re : public GNET::Protocol
{
	#include "kinggetreward_re"

	void Process(Manager *manager, Manager::Session::ID sid)
	{
		// TODO
	}
};

};

#endif
