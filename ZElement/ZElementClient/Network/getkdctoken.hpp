
#ifndef __GNET_GETKDCTOKEN_HPP
#define __GNET_GETKDCTOKEN_HPP

#include "rpcdefs.h"
#include "callid.hxx"
#include "state.hxx"


namespace GNET
{

class GetKDCToken : public GNET::Protocol
{
	#include "getkdctoken"

	void Process(Manager *manager, Manager::Session::ID sid)
	{
		// TODO
	}
};

};

#endif
