
#ifndef __GNET_OPENBANQUETGETFIELD_HPP
#define __GNET_OPENBANQUETGETFIELD_HPP

#include "rpcdefs.h"
#include "callid.hxx"
#include "state.hxx"


namespace GNET
{

class OpenBanquetGetField : public GNET::Protocol
{
	#include "openbanquetgetfield"

	void Process(Manager *manager, Manager::Session::ID sid)
	{
		// TODO
	}
};

};

#endif
