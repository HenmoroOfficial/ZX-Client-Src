
#ifndef __GNET_INSTANCINGJOINREQ_HPP
#define __GNET_INSTANCINGJOINREQ_HPP

#include "rpcdefs.h"
#include "callid.hxx"
#include "state.hxx"


namespace GNET
{

class InstancingJoinReq : public GNET::Protocol
{
	#include "instancingjoinreq"

	void Process(Manager *manager, Manager::Session::ID sid)
	{
		// TODO
	}
};

};

#endif
