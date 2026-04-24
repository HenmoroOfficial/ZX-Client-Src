
#ifndef __GNET_REFINEMESSAGE_HPP
#define __GNET_REFINEMESSAGE_HPP

#include "rpcdefs.h"
#include "callid.hxx"
#include "state.hxx"


namespace GNET
{

class RefineMessage : public GNET::Protocol
{
	#include "refinemessage"

	void Process(Manager *manager, Manager::Session::ID sid)
	{
		// TODO
	}
};

};

#endif
