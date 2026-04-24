
#ifndef __GNET_SNSGETPLAYERINFO_HPP
#define __GNET_SNSGETPLAYERINFO_HPP

#include "rpcdefs.h"
#include "callid.hxx"
#include "state.hxx"


namespace GNET
{

class SNSGetPlayerInfo : public GNET::Protocol
{
	#include "snsgetplayerinfo"

	void Process(Manager *manager, Manager::Session::ID sid)
	{
		// TODO
	}
};

};

#endif
