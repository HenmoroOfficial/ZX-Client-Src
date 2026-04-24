
#ifndef __GNET_FAMILYRECORD_HPP
#define __GNET_FAMILYRECORD_HPP

#include "rpcdefs.h"
#include "callid.hxx"
#include "state.hxx"


namespace GNET
{

class FamilyRecord : public GNET::Protocol
{
	#include "familyrecord"

	void Process(Manager *manager, Manager::Session::ID sid)
	{
		// TODO
	}
};

};

#endif
