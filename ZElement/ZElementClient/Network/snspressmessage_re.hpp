
#ifndef __GNET_SNSPRESSMESSAGE_RE_HPP
#define __GNET_SNSPRESSMESSAGE_RE_HPP

#include "rpcdefs.h"
#include "callid.hxx"
#include "state.hxx"


namespace GNET
{

class SNSPressMessage_Re : public GNET::Protocol
{
	#include "snspressmessage_re"

	void Process(Manager *manager, Manager::Session::ID sid)
	{
		// TODO
	}
};

};

#endif
