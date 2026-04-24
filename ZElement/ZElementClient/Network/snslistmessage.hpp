
#ifndef __GNET_SNSLISTMESSAGE_HPP
#define __GNET_SNSLISTMESSAGE_HPP

#include "rpcdefs.h"
#include "callid.hxx"
#include "state.hxx"


namespace GNET
{

class SNSListMessage : public GNET::Protocol
{
	#include "snslistmessage"

	void Process(Manager *manager, Manager::Session::ID sid)
	{
		// TODO
	}
};

};

#endif
