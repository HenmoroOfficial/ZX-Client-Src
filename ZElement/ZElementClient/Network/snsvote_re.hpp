
#ifndef __GNET_SNSVOTE_RE_HPP
#define __GNET_SNSVOTE_RE_HPP

#include "rpcdefs.h"
#include "callid.hxx"
#include "state.hxx"


namespace GNET
{

class SNSVote_Re : public GNET::Protocol
{
	#include "snsvote_re"

	void Process(Manager *manager, Manager::Session::ID sid)
	{
		// TODO
	}
};

};

#endif
