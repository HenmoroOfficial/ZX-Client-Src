#include "EC_Global.h"
#include "gnet.h"
#include "EC_NetRawProtocol.h"
#include "EC_GameSession.h"

bool CECNetRawProtocol::Filter(CECGameSession* pSession)
{
	return pSession->FilterProtocols(GetRawProtocol());
}

bool CECNetRawProtocol::Process(CECGameSession* pSession)
{
	return pSession->ProcessOneProtocol(this);
}
