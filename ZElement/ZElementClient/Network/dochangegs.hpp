
#ifndef __GNET_DOCHANGEGS_HPP
#define __GNET_DOCHANGEGS_HPP

#include "rpcdefs.h"
#include "callid.hxx"
#include "state.hxx"


namespace GNET
{

class DoChangeGS : public GNET::Protocol
{
	#include "dochangegs"

	void Process(Manager *manager, Manager::Session::ID sid)
	{
		// TODO
	}
};

};

#endif
