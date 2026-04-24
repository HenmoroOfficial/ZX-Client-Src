
#ifndef __GNET_SNSACCEPTAPPLY_HPP
#define __GNET_SNSACCEPTAPPLY_HPP

#include "rpcdefs.h"
#include "callid.hxx"
#include "state.hxx"


namespace GNET
{

class SNSAcceptApply : public GNET::Protocol
{
	#include "snsacceptapply"

	void Process(Manager *manager, Manager::Session::ID sid)
	{
		// TODO
	}
};

};

#endif
