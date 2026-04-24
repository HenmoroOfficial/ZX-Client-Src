
#ifndef __GNET_PKMESSAGE_HPP
#define __GNET_PKMESSAGE_HPP

#include "rpcdefs.h"
#include "callid.hxx"
#include "state.hxx"


namespace GNET
{

class PKMessage : public GNET::Protocol
{
	#include "pkmessage"

	void Process(Manager *manager, Manager::Session::ID sid)
	{
		// TODO
	}
};

};

#endif
