
#ifndef __GNET_OPENBANQUETGETFIELD_RE_HPP
#define __GNET_OPENBANQUETGETFIELD_RE_HPP

#include "rpcdefs.h"
#include "callid.hxx"
#include "state.hxx"

#include "gopenbanquetroleinfo"

namespace GNET
{

class OpenBanquetGetField_Re : public GNET::Protocol
{
	#include "openbanquetgetfield_re"

	void Process(Manager *manager, Manager::Session::ID sid)
	{
		// TODO
	}
};

};

#endif
