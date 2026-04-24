
#ifndef __GNET_GETFACACTTOPTABLE_HPP
#define __GNET_GETFACACTTOPTABLE_HPP

#include "rpcdefs.h"
#include "callid.hxx"
#include "state.hxx"


namespace GNET
{

class GetFacActTopTable : public GNET::Protocol
{
	#include "getfacacttoptable"

	void Process(Manager *manager, Manager::Session::ID sid)
	{
		// TODO
	}
};

};

#endif
