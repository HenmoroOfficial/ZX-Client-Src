
#ifndef __GNET_SNSCANCELMESSAGE_RE_HPP
#define __GNET_SNSCANCELMESSAGE_RE_HPP

#include "rpcdefs.h"
#include "callid.hxx"
#include "state.hxx"


namespace GNET
{

class SNSCancelMessage_Re : public GNET::Protocol
{
	#include "snscancelmessage_re"

	void Process(Manager *manager, Manager::Session::ID sid)
	{
		// TODO
	}
};

};

#endif
