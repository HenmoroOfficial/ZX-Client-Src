
#ifndef __GNET_TOPFLOWER_ERR_HPP
#define __GNET_TOPFLOWER_ERR_HPP

#include "rpcdefs.h"
#include "callid.hxx"
#include "state.hxx"


namespace GNET
{

class TopFlower_Err : public GNET::Protocol
{
	#include "topflower_err"

	void Process(Manager *manager, Manager::Session::ID sid)
	{
		// TODO
	}
};

};

#endif
