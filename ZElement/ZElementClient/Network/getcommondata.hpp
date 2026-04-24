
#ifndef __GNET_GETCOMMONDATA_HPP
#define __GNET_GETCOMMONDATA_HPP

#include "rpcdefs.h"
#include "callid.hxx"
#include "state.hxx"


namespace GNET
{

class GetCommonData : public GNET::Protocol
{
	#include "getcommondata"

	void Process(Manager *manager, Manager::Session::ID sid)
	{
		// TODO
	}
};

};

#endif
