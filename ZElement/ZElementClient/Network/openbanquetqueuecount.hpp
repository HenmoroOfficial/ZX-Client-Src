
#ifndef __GNET_OPENBANQUETQUEUECOUNT_HPP
#define __GNET_OPENBANQUETQUEUECOUNT_HPP

#include "rpcdefs.h"
#include "callid.hxx"
#include "state.hxx"


namespace GNET
{

class OpenBanquetQueueCount : public GNET::Protocol
{
	#include "openbanquetqueuecount"

	void Process(Manager *manager, Manager::Session::ID sid)
	{
		// TODO
	}
};

};

#endif
