
#ifndef __GNET_GMFORBIDSELLPOINT_RE_HPP
#define __GNET_GMFORBIDSELLPOINT_RE_HPP

#include "rpcdefs.h"
#include "callid.hxx"
#include "state.hxx"


namespace GNET
{

class GMForbidSellPoint_Re : public GNET::Protocol
{
	#include "gmforbidsellpoint_re"

	void Process(Manager *manager, Manager::Session::ID sid)
	{
		// TODO
	}
};

};

#endif
