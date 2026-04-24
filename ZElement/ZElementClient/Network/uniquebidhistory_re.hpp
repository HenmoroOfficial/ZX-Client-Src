
#ifndef __GNET_UNIQUEBIDHISTORY_RE_HPP
#define __GNET_UNIQUEBIDHISTORY_RE_HPP

#include "rpcdefs.h"
#include "callid.hxx"
#include "state.hxx"

#include "uniquewinner"
#include "uniquepriceinfo"

namespace GNET
{

class UniqueBidHistory_Re : public GNET::Protocol
{
	#include "uniquebidhistory_re"

	void Process(Manager *manager, Manager::Session::ID sid)
	{
		// TODO
	}
};

};

#endif
