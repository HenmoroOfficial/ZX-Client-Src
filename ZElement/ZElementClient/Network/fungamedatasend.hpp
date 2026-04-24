
#ifndef __GNET_FUNGAMEDATASEND_HPP
#define __GNET_FUNGAMEDATASEND_HPP

#include "rpcdefs.h"
#include "callid.hxx"
#include "state.hxx"


namespace GNET
{

class FunGamedataSend : public GNET::Protocol
{
	#include "fungamedatasend"

	void Process(Manager *manager, Manager::Session::ID sid)
	{
		// TODO
	}
};

};

#endif
