
#ifndef __GNET_ACACCUSE_HPP
#define __GNET_ACACCUSE_HPP

#include "rpcdefs.h"
#include "callid.hxx"
#include "state.hxx"


namespace GNET
{

class ACAccuse : public GNET::Protocol
{
	#include "acaccuse"

	void Process(Manager *manager, Manager::Session::ID sid)
	{
		// TODO
	}
};

};

#endif
