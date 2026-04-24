
#ifndef __GNET_LINELIST_HPP
#define __GNET_LINELIST_HPP

#include "rpcdefs.h"
#include "callid.hxx"
#include "state.hxx"

#include "lineinfo"

namespace GNET
{

class LineList : public GNET::Protocol
{
	#include "linelist"

	void Process(Manager *manager, Manager::Session::ID sid)
	{
		// TODO
	}
};

};

#endif
