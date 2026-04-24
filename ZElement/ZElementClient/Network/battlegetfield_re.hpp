
#ifndef __GNET_BATTLEGETFIELD_RE_HPP
#define __GNET_BATTLEGETFIELD_RE_HPP

#include "rpcdefs.h"
#include "callid.hxx"
#include "state.hxx"

#include "gbattlerole"
#include "gbattlerole"

namespace GNET
{

class BattleGetField_Re : public GNET::Protocol
{
	#include "battlegetfield_re"

	void Process(Manager *manager, Manager::Session::ID sid)
	{
		// TODO
	}
};

};

#endif
