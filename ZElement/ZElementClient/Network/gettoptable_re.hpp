
#ifndef __GNET_GETTOPTABLE_RE_HPP
#define __GNET_GETTOPTABLE_RE_HPP

#include "rpcdefs.h"
#include "callid.hxx"
#include "state.hxx"

#include "gtopitem"
#include "gtopdetail"

namespace GNET
{

class GetTopTable_Re : public GNET::Protocol
{
	#include "gettoptable_re"

	void Process(Manager *manager, Manager::Session::ID sid)
	{
		// TODO
	}
};

};

#endif
