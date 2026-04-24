
#ifndef __GNET_SSOGETTICKETREQ_HPP
#define __GNET_SSOGETTICKETREQ_HPP

#include "rpcdefs.h"
#include "callid.hxx"
#include "state.hxx"

#include "userssoinfo"

namespace GNET
{

class SSOGetTicketReq : public GNET::Protocol
{
	#include "ssogetticketreq"

	void Process(Manager *manager, Manager::Session::ID sid)
	{
		// TODO
	}
};

};

#endif
