#include "EC_Global.h"
#include "gnet.h"
#include "EC_NetPrtclNPCInfo.h"
#include "EC_GameSession.h"
#include "EC_GPDataType.h"
#include "EC_Game.h"
#include "EC_GameRun.h"
#include "EC_World.h"
#include "EC_ManNPC.h"

using namespace S2C;

CECNetProtocolBase* CECNetPrtclNPCInfo::_Create(GNET::Protocol* pSrc)
{
	_CREATE_PRTCL_FROM(CECNetPrtclNPCInfo)
}

bool CECNetPrtclNPCInfo::Process(CECGameSession* pSession)
{
	PreProcess(pSession);

	ECMSG msg;
	msg.dwMsg		= MSG_NM_NPCINFO;
	msg.iManager	= MAN_NPC;
	msg.iSubID		= 0;
	msg.dwParam1	= (DWORD)m_pData;
	msg.dwParam2	= m_Cmd;
	msg.dwParam3	= m_nDataSize;

	g_pGame->GetGameRun()->PostMessage(msg);
	return true;
}

CECNetProtocolBase* CECNetPrtclNPCDie::_Create(GNET::Protocol* pSrc)
{
	_CREATE_PRTCL_FROM(CECNetPrtclNPCDie)
}

bool CECNetPrtclNPCDie::Process(CECGameSession* pSession)
{
	PreProcess(pSession);

	ECMSG msg;
	msg.dwMsg		= MSG_NM_NPCDIED;
	msg.iManager	= MAN_NPC;
	msg.iSubID		= 0;
	msg.dwParam1	= (DWORD)m_pData;
	msg.dwParam2	= m_Cmd;
	msg.dwParam3	= m_nDataSize;

	g_pGame->GetGameRun()->PostMessage(msg);
	return true;
}

CECNetProtocolBase* CECNetPrtclNPCDie2::_Create(GNET::Protocol* pSrc)
{
	_CREATE_PRTCL_FROM(CECNetPrtclNPCDie2)
}

bool CECNetPrtclNPCDie2::Process(CECGameSession* pSession)
{
	PreProcess(pSession);

	ECMSG msg;
	msg.dwMsg		= MSG_NM_NPCDIED;
	msg.iManager	= MAN_NPC;
	msg.iSubID		= 0;
	msg.dwParam1	= (DWORD)m_pData;
	msg.dwParam2	= m_Cmd;
	msg.dwParam3	= m_nDataSize;

	g_pGame->GetGameRun()->PostMessage(msg);
	return true;
}
