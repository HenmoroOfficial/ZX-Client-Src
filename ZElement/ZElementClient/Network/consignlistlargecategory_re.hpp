
#ifndef __GNET_CONSIGNLISTLARGECATEGORY_RE_HPP
#define __GNET_CONSIGNLISTLARGECATEGORY_RE_HPP

#include "rpcdefs.h"
#include "callid.hxx"
#include "state.hxx"

#include "gconsignlistnode"

namespace GNET
{

class ConsignListLargeCategory_Re : public GNET::Protocol
{
	#include "consignlistlargecategory_re"

	void Process(Manager *manager, Manager::Session::ID sid)
	{
		// TODO
	}
};

};

#endif
