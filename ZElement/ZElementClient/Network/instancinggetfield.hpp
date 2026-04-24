
#ifndef __GNET_INSTANCINGGETFIELD_HPP
#define __GNET_INSTANCINGGETFIELD_HPP

#include "rpcdefs.h"
#include "callid.hxx"
#include "state.hxx"


namespace GNET
{

class InstancingGetField : public GNET::Protocol
{
	#include "instancinggetfield"

	void Process(Manager *manager, Manager::Session::ID sid)
	{
		// TODO
	}
};

};

#endif
