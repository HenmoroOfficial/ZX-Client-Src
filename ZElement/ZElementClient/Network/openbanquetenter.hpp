
#ifndef __GNET_OPENBANQUETENTER_HPP
#define __GNET_OPENBANQUETENTER_HPP

#include "rpcdefs.h"
#include "callid.hxx"
#include "state.hxx"


namespace GNET
{

class OpenBanquetEnter : public GNET::Protocol
{
	#include "openbanquetenter"

	void Process(Manager *manager, Manager::Session::ID sid)
	{
		// TODO
	}
};

};

#endif
