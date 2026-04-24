
#ifndef __GNET_TRYCHANGEGS_RE_HPP
#define __GNET_TRYCHANGEGS_RE_HPP

#include "rpcdefs.h"
#include "callid.hxx"
#include "state.hxx"


namespace GNET
{

class TryChangeGS_Re : public GNET::Protocol
{
	#include "trychangegs_re"

	void Process(Manager *manager, Manager::Session::ID sid)
	{
		// TODO
	}
};

};

#endif
