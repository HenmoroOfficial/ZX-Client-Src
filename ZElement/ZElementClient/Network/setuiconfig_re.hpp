
#ifndef __GNET_SETUICONFIG_RE_HPP
#define __GNET_SETUICONFIG_RE_HPP

#include "rpcdefs.h"
#include "callid.hxx"
#include "state.hxx"


namespace GNET
{

class SetUIConfig_Re : public GNET::Protocol
{
	#include "setuiconfig_re"

	void Process(Manager *manager, Manager::Session::ID sid)
	{
		// TODO
	}
};

};

#endif
