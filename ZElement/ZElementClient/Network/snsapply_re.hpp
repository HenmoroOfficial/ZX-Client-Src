
#ifndef __GNET_SNSAPPLY_RE_HPP
#define __GNET_SNSAPPLY_RE_HPP

#include "rpcdefs.h"
#include "callid.hxx"
#include "state.hxx"


namespace GNET
{

class SNSApply_Re : public GNET::Protocol
{
	#include "snsapply_re"

	void Process(Manager *manager, Manager::Session::ID sid)
	{
		// TODO
	}
};

};

#endif
