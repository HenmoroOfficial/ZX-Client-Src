
#ifndef __GNET_FACTIONDISMISS_HPP
#define __GNET_FACTIONDISMISS_HPP

#include "rpcdefs.h"
#include "callid.hxx"
#include "state.hxx"


namespace GNET
{

class FactionDismiss : public GNET::Protocol
{
	#include "factiondismiss"

	void Process(Manager *manager, Manager::Session::ID sid)
	{
		// TODO
	}
};

};

#endif
