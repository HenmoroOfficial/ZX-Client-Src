
#ifndef __GNET_GETCIRCLEBASEINFO_HPP
#define __GNET_GETCIRCLEBASEINFO_HPP

#include "rpcdefs.h"
#include "callid.hxx"
#include "state.hxx"


namespace GNET
{

class GetCircleBaseInfo : public GNET::Protocol
{
	#include "getcirclebaseinfo"

	void Process(Manager *manager, Manager::Session::ID sid)
	{
		// TODO
	}
};

};

#endif
