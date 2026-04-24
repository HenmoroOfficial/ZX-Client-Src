
#ifndef __GNET_SNSLISTMESSAGE_RE_HPP
#define __GNET_SNSLISTMESSAGE_RE_HPP

#include "rpcdefs.h"
#include "callid.hxx"
#include "state.hxx"

#include "snsmessagebrief"

namespace GNET
{

class SNSListMessage_Re : public GNET::Protocol
{
	#include "snslistmessage_re"

	void Process(Manager *manager, Manager::Session::ID sid)
	{
		// TODO
	}
};

};

#endif
