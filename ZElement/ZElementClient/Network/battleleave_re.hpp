
#ifndef __GNET_BATTLELEAVE_RE_HPP
#define __GNET_BATTLELEAVE_RE_HPP

#include "rpcdefs.h"
#include "callid.hxx"
#include "state.hxx"


namespace GNET
{

class BattleLeave_Re : public GNET::Protocol
{
	#include "battleleave_re"

	void Process(Manager *manager, Manager::Session::ID sid)
	{
		// TODO
	}
};

};

#endif
