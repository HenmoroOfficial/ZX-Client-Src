
#ifndef __GNET_GETKINGTASK_HPP
#define __GNET_GETKINGTASK_HPP

#include "rpcdefs.h"
#include "callid.hxx"
#include "state.hxx"


namespace GNET
{

class GetKingTask : public GNET::Protocol
{
	#include "getkingtask"

	void Process(Manager *manager, Manager::Session::ID sid)
	{
		// TODO
	}
};

};

#endif
