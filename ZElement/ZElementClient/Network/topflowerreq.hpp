
#ifndef __GNET_TOPFLOWERREQ_HPP
#define __GNET_TOPFLOWERREQ_HPP

#include "rpcdefs.h"
#include "callid.hxx"
#include "state.hxx"


namespace GNET
{

class TopFlowerReq : public GNET::Protocol
{
	#include "topflowerreq"

	void Process(Manager *manager, Manager::Session::ID sid)
	{
		// TODO
	}
};

};

#endif
