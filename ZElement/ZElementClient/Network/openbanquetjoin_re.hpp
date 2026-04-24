
#ifndef __GNET_OPENBANQUETJOIN_RE_HPP
#define __GNET_OPENBANQUETJOIN_RE_HPP

#include "rpcdefs.h"
#include "callid.hxx"
#include "state.hxx"


namespace GNET
{

class OpenBanquetJoin_Re : public GNET::Protocol
{
	#include "openbanquetjoin_re"

	void Process(Manager *manager, Manager::Session::ID sid)
	{
		// TODO
	}
};

};

#endif
