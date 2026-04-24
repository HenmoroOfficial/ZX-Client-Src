
#ifndef __GNET_SNSGETMESSAGE_RE_HPP
#define __GNET_SNSGETMESSAGE_RE_HPP

#include "rpcdefs.h"
#include "callid.hxx"
#include "state.hxx"

#include "gleavemessage"
#include "gapplyinfo"
#include "gapplyinfo"
#include "gapplyinfo"

namespace GNET
{

class SNSGetMessage_Re : public GNET::Protocol
{
	#include "snsgetmessage_re"

	void Process(Manager *manager, Manager::Session::ID sid)
	{
		// TODO
	}
};

};

#endif
