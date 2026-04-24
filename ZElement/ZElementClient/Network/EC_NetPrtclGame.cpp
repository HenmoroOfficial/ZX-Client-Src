#include "EC_Global.h"
#include "gnet.h"
#include "EC_Game.h"
#include "EC_GameRun.h"
#include "EC_GPDataType.h"
#include "EC_GameSession.h"
#include "EC_NetPrtclGame.h"

void CECNetPrtclGame::PreProcess(CECGameSession* pSession)
{
	GNET::Octets& Data = static_cast<GamedataSend*>(m_pRaw)->data;
	m_Cmd = static_cast<S2C::cmd_header*>(Data.begin())->cmd;
	m_pData = (BYTE*)Data.begin() + sizeof(S2C::cmd_header);
	m_nDataSize = Data.size() - sizeof(S2C::cmd_header);
	pSession->LogS2CCmdData(m_Cmd, (BYTE*)Data.begin(), Data.size());
}

bool CECNetPrtclGame::Filter(CECGameSession* pSession)
{
	return g_pGame->GetGameRun()->GetGameState() != CECGameRun::GS_GAME;
}
