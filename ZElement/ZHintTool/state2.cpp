//
//
//
//-----------------------------------------------------------------------------
#include <winsock2.h>

#include "callid2.hpp"
#include "gnproto.h"
#include "gncompress.h"

namespace GNET
{
	static GNET::Protocol::Type _state_ElementHint[] =
	{
		PROTOCOL_GETITEMDESC,
		PROTOCOL_GETITEMDESC_RE,
		PROTOCOL_NEWKEEPALIVE,
		PROTOCOL_GETITEMDESC_PLTFM,
		PROTOCOL_GETITEMDESC_PLTFM_RE,
		PROTOCOL_TOOL_ANNOUNCE_AID,
		PROTOCOL_GET_ROLE_EQUIP,
		PROTOCOL_GET_ROLE_EQUIP_RE,
		PROTOCOL_GET_SNS_ROLE_PET,
		PROTOCOL_GET_SNS_ROLE_PET_RE,
		PROTOCOL_GET_ROLE_DESC,
		PROTOCOL_GET_ROLE_DESC_RE,
		PROTOCOL_GET_ROLE_DESC_SNS,
		PROTOCOL_GET_ROLE_DESC_SNS_RE,
	};

	//
	GNET::Protocol::Manager::Session::State state_ElementHint(_state_ElementHint,
                        sizeof(_state_ElementHint)/sizeof(GNET::Protocol::Type), 0);
};