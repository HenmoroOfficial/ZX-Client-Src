
#ifndef __GNET_HOMETOWNDATASEND_HPP
#define __GNET_HOMETOWNDATASEND_HPP

#include "rpcdefs.h"
#include "callid.hxx"
#include "state.hxx"


namespace GNET
{

class HometowndataSend : public GNET::Protocol
{
	#include "hometowndatasend"

	void Process(Manager *manager, Manager::Session::ID sid)
	{
		// TODO
	}
};

};

#endif
