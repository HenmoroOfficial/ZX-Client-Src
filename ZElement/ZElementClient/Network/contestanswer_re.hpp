
#ifndef __GNET_CONTESTANSWER_RE_HPP
#define __GNET_CONTESTANSWER_RE_HPP

#include "rpcdefs.h"
#include "callid.hxx"
#include "state.hxx"


namespace GNET
{

class ContestAnswer_Re : public GNET::Protocol
{
	#include "contestanswer_re"

	void Process(Manager *manager, Manager::Session::ID sid)
	{
		// TODO
	}
};

};

#endif
