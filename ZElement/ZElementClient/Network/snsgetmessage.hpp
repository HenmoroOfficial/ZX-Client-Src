
#ifndef __GNET_SNSGETMESSAGE_HPP
#define __GNET_SNSGETMESSAGE_HPP

#include "rpcdefs.h"
#include "callid.hxx"
#include "state.hxx"


namespace GNET
{

class SNSGetMessage : public GNET::Protocol
{
	#include "snsgetmessage"

	void Process(Manager *manager, Manager::Session::ID sid)
	{
		// TODO
	}
};

};

#endif
