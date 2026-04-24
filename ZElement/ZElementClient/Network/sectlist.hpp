
#ifndef __GNET_SECTLIST_HPP
#define __GNET_SECTLIST_HPP

#include "rpcdefs.h"
#include "callid.hxx"
#include "state.hxx"


namespace GNET
{

class SectList : public GNET::Protocol
{
	#include "sectlist"

	void Process(Manager *manager, Manager::Session::ID sid)
	{
		// TODO
	}
};

};

#endif
