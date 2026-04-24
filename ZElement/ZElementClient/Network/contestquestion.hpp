
#ifndef __GNET_CONTESTQUESTION_HPP
#define __GNET_CONTESTQUESTION_HPP

#include "rpcdefs.h"
#include "callid.hxx"
#include "state.hxx"


namespace GNET
{

class ContestQuestion : public GNET::Protocol
{
	#include "contestquestion"

	void Process(Manager *manager, Manager::Session::ID sid)
	{
		// TODO
	}
};

};

#endif
