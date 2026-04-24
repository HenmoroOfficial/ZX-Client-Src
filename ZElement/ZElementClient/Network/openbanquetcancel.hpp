
#ifndef __GNET_OPENBANQUETCANCEL_HPP
#define __GNET_OPENBANQUETCANCEL_HPP

#include "rpcdefs.h"
#include "callid.hxx"
#include "state.hxx"


namespace GNET
{

class OpenBanquetCancel : public GNET::Protocol
{
	#include "openbanquetcancel"

	void Process(Manager *manager, Manager::Session::ID sid)
	{
		// TODO
	}
};

};

#endif
