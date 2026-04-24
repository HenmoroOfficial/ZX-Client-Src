
#ifndef __GNET_CONSIGNPLAYER_RE_HPP
#define __GNET_CONSIGNPLAYER_RE_HPP

#include "rpcdefs.h"
#include "callid.hxx"
#include "state.hxx"


namespace GNET
{

class ConsignPlayer_Re : public GNET::Protocol
{
	#include "consignplayer_re"

	void Process(Manager *manager, Manager::Session::ID sid)
	{
		// TODO
	}
};

};

#endif
