
#ifndef __GNET_SENDFLOWERREQ_HPP
#define __GNET_SENDFLOWERREQ_HPP

#include "rpcdefs.h"
#include "callid.hxx"
#include "state.hxx"


namespace GNET
{

class SendFlowerReq : public GNET::Protocol
{
	#include "sendflowerreq"

	void Process(Manager *manager, Manager::Session::ID sid)
	{
		// TODO
	}
};

};

#endif
