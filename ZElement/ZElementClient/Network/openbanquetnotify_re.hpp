
#ifndef __GNET_OPENBANQUETNOTIFY_RE_HPP
#define __GNET_OPENBANQUETNOTIFY_RE_HPP

#include "rpcdefs.h"
#include "callid.hxx"
#include "state.hxx"


namespace GNET
{

class OpenBanquetNotify_Re : public GNET::Protocol
{
	#include "openbanquetnotify_re"

	void Process(Manager *manager, Manager::Session::ID sid)
	{
		// TODO
	}
};

};

#endif
