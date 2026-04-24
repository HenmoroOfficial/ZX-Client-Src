
#ifndef __GNET_SECTLIST_RE_HPP
#define __GNET_SECTLIST_RE_HPP

#include "rpcdefs.h"
#include "callid.hxx"
#include "state.hxx"

#include "gdiscipleinfo"
#include "gsectskill"

namespace GNET
{

class SectList_Re : public GNET::Protocol
{
	#include "sectlist_re"

	void Process(Manager *manager, Manager::Session::ID sid)
	{
		// TODO
	}
};

};

#endif
