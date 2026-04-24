
#ifndef __GNET_ACACCUSE_RE_HPP
#define __GNET_ACACCUSE_RE_HPP

#include "rpcdefs.h"
#include "callid.hxx"
#include "state.hxx"


namespace GNET
{

class ACAccuse_Re : public GNET::Protocol
{
	#include "acaccuse_re"

	void Process(Manager *manager, Manager::Session::ID sid)
	{
		// TODO
	}
};

};

#endif
