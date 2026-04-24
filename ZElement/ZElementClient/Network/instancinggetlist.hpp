
#ifndef __GNET_INSTANCINGGETLIST_HPP
#define __GNET_INSTANCINGGETLIST_HPP

#include "rpcdefs.h"
#include "callid.hxx"
#include "state.hxx"


namespace GNET
{

class InstancingGetList : public GNET::Protocol
{
	#include "instancinggetlist"

	void Process(Manager *manager, Manager::Session::ID sid)
	{
		// TODO
	}
};

};

#endif
