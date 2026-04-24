
#ifndef __GNET_TOPFLOWER_RE_HPP
#define __GNET_TOPFLOWER_RE_HPP

#include "rpcdefs.h"
#include "callid.hxx"
#include "state.hxx"

#include "gtopflowerdata"

namespace GNET
{

class TopFlower_Re : public GNET::Protocol
{
	#include "topflower_re"

	void Process(Manager *manager, Manager::Session::ID sid)
	{
		// TODO
	}
};

};

#endif
