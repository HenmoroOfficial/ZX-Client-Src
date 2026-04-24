
#ifndef __GNET_ANNOUNCEDISTRICTID_HPP
#define __GNET_ANNOUNCEDISTRICTID_HPP

#include "rpcdefs.h"
#include "callid.hxx"
#include "state.hxx"


namespace GNET
{

class AnnounceDistrictId : public GNET::Protocol
{
	#include "announcedistrictid"

	void Process(Manager *manager, Manager::Session::ID sid)
	{
		// TODO
	}
};

};

#endif
