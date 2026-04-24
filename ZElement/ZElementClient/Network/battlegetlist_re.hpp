
#ifndef __GNET_BATTLEGETLIST_RE_HPP
#define __GNET_BATTLEGETLIST_RE_HPP

#include "rpcdefs.h"
#include "callid.hxx"
#include "state.hxx"

#include "gfieldinfo"

namespace GNET
{

class BattleGetList_Re : public GNET::Protocol
{
	#include "battlegetlist_re"

	void Process(Manager *manager, Manager::Session::ID sid)
	{
		// TODO
	}
};

};

#endif
