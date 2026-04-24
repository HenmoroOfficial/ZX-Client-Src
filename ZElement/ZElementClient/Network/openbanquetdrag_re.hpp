
#ifndef __GNET_OPENBANQUETDRAG_RE_HPP
#define __GNET_OPENBANQUETDRAG_RE_HPP

#include "rpcdefs.h"
#include "callid.hxx"
#include "state.hxx"


namespace GNET
{

class OpenBanquetDrag_Re : public GNET::Protocol
{
	#include "openbanquetdrag_re"

	void Process(Manager *manager, Manager::Session::ID sid)
	{
		// TODO
	}
};

};

#endif
