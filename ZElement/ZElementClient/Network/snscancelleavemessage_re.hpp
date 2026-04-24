
#ifndef __GNET_SNSCANCELLEAVEMESSAGE_RE_HPP
#define __GNET_SNSCANCELLEAVEMESSAGE_RE_HPP

#include "rpcdefs.h"
#include "callid.hxx"
#include "state.hxx"


namespace GNET
{

class SNSCancelLeaveMessage_Re : public GNET::Protocol
{
	#include "snscancelleavemessage_re"

	void Process(Manager *manager, Manager::Session::ID sid)
	{
		// TODO
	}
};

};

#endif
