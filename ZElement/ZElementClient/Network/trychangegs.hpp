
#ifndef __GNET_TRYCHANGEGS_HPP
#define __GNET_TRYCHANGEGS_HPP

#include "rpcdefs.h"
#include "callid.hxx"
#include "state.hxx"


namespace GNET
{

class TryChangeGS : public GNET::Protocol
{
	#include "trychangegs"

	void Process(Manager *manager, Manager::Session::ID sid)
	{
		// TODO
	}
};

};

#endif
