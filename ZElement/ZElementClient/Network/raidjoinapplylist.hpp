
#ifndef __GNET_RAIDJOINAPPLYLIST_HPP
#define __GNET_RAIDJOINAPPLYLIST_HPP

#include "rpcdefs.h"
#include "callid.hxx"
#include "state.hxx"


namespace GNET
{

class RaidJoinApplyList : public GNET::Protocol
{
	#include "raidjoinapplylist"

	void Process(Manager *manager, Manager::Session::ID sid)
	{
		// TODO
	}
};

};

#endif
