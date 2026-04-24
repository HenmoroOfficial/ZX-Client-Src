
#ifndef __GNET_GCIRCLECHAT_HPP
#define __GNET_GCIRCLECHAT_HPP

#include "rpcdefs.h"
#include "callid.hxx"
#include "state.hxx"


namespace GNET
{

class GCircleChat : public GNET::Protocol
{
	#include "gcirclechat"

	void Process(Manager *manager, Manager::Session::ID sid)
	{
		// TODO
	}
};

};

#endif
