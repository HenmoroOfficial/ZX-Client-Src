
#ifndef __GNET_CREATEFACBASE_RE_HPP
#define __GNET_CREATEFACBASE_RE_HPP

#include "rpcdefs.h"
#include "callid.hxx"
#include "state.hxx"


namespace GNET
{

class CreateFacBase_Re : public GNET::Protocol
{
	#include "createfacbase_re"

	void Process(Manager *manager, Manager::Session::ID sid)
	{
		// TODO
	}
};

};

#endif
