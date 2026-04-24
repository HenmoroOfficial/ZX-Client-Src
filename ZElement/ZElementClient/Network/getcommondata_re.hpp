
#ifndef __GNET_GETCOMMONDATA_RE_HPP
#define __GNET_GETCOMMONDATA_RE_HPP

#include "rpcdefs.h"
#include "callid.hxx"
#include "state.hxx"


namespace GNET
{

class GetCommonData_Re : public GNET::Protocol
{
	#include "getcommondata_re"

	void Process(Manager *manager, Manager::Session::ID sid)
	{
		// TODO
	}
};

};

#endif
