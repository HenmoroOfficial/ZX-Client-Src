
#ifndef __GNET_SSOGETTICKETREP_HPP
#define __GNET_SSOGETTICKETREP_HPP

#include "rpcdefs.h"
#include "callid.hxx"
#include "state.hxx"

#include "userssoinfo"

namespace GNET
{

class SSOGetTicketRep : public GNET::Protocol
{
	#include "ssogetticketrep"

	void Process(Manager *manager, Manager::Session::ID sid)
	{
		// TODO
	}
};

};

#endif
