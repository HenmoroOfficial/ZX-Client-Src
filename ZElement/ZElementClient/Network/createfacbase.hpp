
#ifndef __GNET_CREATEFACBASE_HPP
#define __GNET_CREATEFACBASE_HPP

#include "rpcdefs.h"
#include "callid.hxx"
#include "state.hxx"


namespace GNET
{

class CreateFacBase : public GNET::Protocol
{
	#include "createfacbase"

	void Process(Manager *manager, Manager::Session::ID sid)
	{
		// TODO
	}
};

};

#endif
