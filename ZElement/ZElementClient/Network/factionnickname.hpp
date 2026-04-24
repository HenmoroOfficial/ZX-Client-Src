
#ifndef __GNET_FACTIONNICKNAME_HPP
#define __GNET_FACTIONNICKNAME_HPP

#include "rpcdefs.h"
#include "callid.hxx"
#include "state.hxx"


namespace GNET
{

class FactionNickname : public GNET::Protocol
{
	#include "factionnickname"

	void Process(Manager *manager, Manager::Session::ID sid)
	{
		// TODO
	}
};

};

#endif
