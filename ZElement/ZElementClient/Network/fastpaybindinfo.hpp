
#ifndef __GNET_FASTPAYBINDINFO_HPP
#define __GNET_FASTPAYBINDINFO_HPP

#include "rpcdefs.h"
#include "callid.hxx"
#include "state.hxx"


namespace GNET
{

class FastPayBindInfo : public GNET::Protocol
{
	#include "fastpaybindinfo"

	void Process(Manager *manager, Manager::Session::ID sid)
	{
		// TODO
	}
};

};

#endif
