
#ifndef __GNET_QUEENTRYOPENBATH_HPP
#define __GNET_QUEENTRYOPENBATH_HPP

#include "rpcdefs.h"
#include "callid.hxx"
#include "state.hxx"


namespace GNET
{

class QueenTryOpenBath : public GNET::Protocol
{
	#include "queentryopenbath"

	void Process(Manager *manager, Manager::Session::ID sid)
	{
		// TODO
	}
};

};

#endif
