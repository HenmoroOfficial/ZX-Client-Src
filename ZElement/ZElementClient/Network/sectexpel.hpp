
#ifndef __GNET_SECTEXPEL_HPP
#define __GNET_SECTEXPEL_HPP

#include "rpcdefs.h"
#include "callid.hxx"
#include "state.hxx"


namespace GNET
{

class SectExpel : public GNET::Protocol
{
	#include "sectexpel"

	void Process(Manager *manager, Manager::Session::ID sid)
	{
		// TODO
	}
};

};

#endif
