
#ifndef __GNET_SECTRECRUIT_HPP
#define __GNET_SECTRECRUIT_HPP

#include "rpcdefs.h"
#include "callid.hxx"
#include "state.hxx"


namespace GNET
{

class SectRecruit : public GNET::Protocol
{
	#include "sectrecruit"

	void Process(Manager *manager, Manager::Session::ID sid)
	{
		// TODO
	}
};

};

#endif
