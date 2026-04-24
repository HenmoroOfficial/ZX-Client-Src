
#ifndef __GNET_GETFACACTTOPTABLE_RE_HPP
#define __GNET_GETFACACTTOPTABLE_RE_HPP

#include "rpcdefs.h"
#include "callid.hxx"
#include "state.hxx"

#include "gtopitem"

namespace GNET
{

class GetFacActTopTable_Re : public GNET::Protocol
{
	#include "getfacacttoptable_re"

	void Process(Manager *manager, Manager::Session::ID sid)
	{
		// TODO
	}
};

};

#endif
