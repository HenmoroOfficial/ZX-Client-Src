
#ifndef __GNET_CHANGEROLENAME_RE_HPP
#define __GNET_CHANGEROLENAME_RE_HPP

#include "rpcdefs.h"
#include "callid.hxx"
#include "state.hxx"


namespace GNET
{

class ChangeRolename_Re : public GNET::Protocol
{
	#include "changerolename_re"

	void Process(Manager *manager, Manager::Session::ID sid)
	{
		// TODO
	}
};

};

#endif
