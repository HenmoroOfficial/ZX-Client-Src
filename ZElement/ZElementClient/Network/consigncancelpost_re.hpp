
#ifndef __GNET_CONSIGNCANCELPOST_RE_HPP
#define __GNET_CONSIGNCANCELPOST_RE_HPP

#include "rpcdefs.h"
#include "callid.hxx"
#include "state.hxx"


namespace GNET
{

class ConsignCancelPost_Re : public GNET::Protocol
{
	#include "consigncancelpost_re"

	void Process(Manager *manager, Manager::Session::ID sid)
	{
		// TODO
	}
};

};

#endif
