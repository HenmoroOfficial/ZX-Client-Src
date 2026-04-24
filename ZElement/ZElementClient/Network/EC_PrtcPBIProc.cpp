/*
 * FILE: EC_PrtcPBIProc.cpp
 *
 * DESCRIPTION: 
 *
 * CREATED BY: Duyuxin, 2007/3/9
 *
 * HISTORY: 
 *
 * Copyright (c) 2007 Archosaur Studio, All Rights Reserved.
 */

#include "EC_Global.h"
#include "EC_PrtcPBIProc.h"
#include "EC_Game.h"
#include "EC_GameRun.h"
#include "EC_StringTab.h"
#include "EC_Team.h"
#include "EC_HostPlayer.h"
#include "EC_NetDef.h"
#include "EC_FixedMsg.h"
#include "EC_Faction.h"
#include "EC_BattleDungeon.h"
#include "EC_NetProtocolBase.h"
#include "EC_GameSession.h"

#include "AUI\\AUICommon.h"

#include "gnetdef.h"
#include "factionchat.hpp"
#include "factionresign_re.hpp"
#include "factionnickname_re.hpp"
#include "factionleave_re.hpp"
#include "factionexpel_re.hpp"
#include "factionappoint_re.hpp"
#include "factionrecruit_re.hpp"
#include "getplayerbriefinfo_re.hpp"

#define new A_DEBUG_NEW

///////////////////////////////////////////////////////////////////////////
//	
//	Define and Macro
//	
///////////////////////////////////////////////////////////////////////////


///////////////////////////////////////////////////////////////////////////
//	
//	Reference to External variables and functions
//	
///////////////////////////////////////////////////////////////////////////


///////////////////////////////////////////////////////////////////////////
//	
//	Local Types and Variables and Global variables
//	
///////////////////////////////////////////////////////////////////////////


///////////////////////////////////////////////////////////////////////////
//	
//	Local functions
//	
///////////////////////////////////////////////////////////////////////////


///////////////////////////////////////////////////////////////////////////
//	
//	Implement CECPrtcPBIProc
//	
///////////////////////////////////////////////////////////////////////////

CECPrtcPBIProc::CECPrtcPBIProc()
{
	//	Initialize valid protocol map
	using namespace GNET;
	m_ValidPrtcMap[PROTOCOL_FACTIONCHAT] = true;
	m_ValidPrtcMap[PROTOCOL_FACTIONRECRUIT_RE] = true;
	m_ValidPrtcMap[PROTOCOL_FACTIONRESIGN_RE] = true;
	m_ValidPrtcMap[PROTOCOL_FACTIONNICKNAME_RE]	= true;
	m_ValidPrtcMap[PROTOCOL_FACTIONLEAVE_RE] = true;
	m_ValidPrtcMap[PROTOCOL_FACTIONEXPEL_RE] = true;
	m_ValidPrtcMap[PROTOCOL_FACTIONAPPOINT_RE] = true;
	m_ValidPrtcMap[PROTOCOL_CHATMESSAGE] = true;
	m_ValidPrtcMap[PROTOCOL_GAMEDATASEND] = true;
	m_ValidPrtcMap[PROTOCOL_GCIRCLECHAT]  = true;
}

CECPrtcPBIProc::~CECPrtcPBIProc()
{
	CECPrtcPBIProc::Reset();
}

//	Release object
void CECPrtcPBIProc::Reset()
{
	//	Release player brief info protocols
	int i, iNumInfo = m_aBriefInfoPrtcs.GetSize();
	for (i=0; i < iNumInfo; i++)
	{
		GetPlayerBriefInfo_Re* p = m_aBriefInfoPrtcs[i];
		p->Destroy();
	}

	m_aBriefInfoPrtcs.RemoveAll();

	//	Release unprocessed protocols
	ALISTPOSITION pos = m_UnprocPrtcsList.GetHeadPosition();
	while (pos)
	{
		CECNetProtocolBase* p = m_UnprocPrtcsList.GetNext(pos);
		p->Destroy();
	}

	m_UnprocPrtcsList.RemoveAll();

	m_BriefInfoMap.clear();
}

//	Add player brief info protocol
void CECPrtcPBIProc::AddPlayerBriefInfoPrtc(GNET::GetPlayerBriefInfo_Re* pProtocol)
{
	if (pProtocol->retcode == ERR_SUCCESS)
		m_aBriefInfoPrtcs.Add(pProtocol);
}

//	Add unprocessed protocol
bool CECPrtcPBIProc::AddUnprocessedPrtc(CECNetProtocolBase* pProtocol)
{
	int idPrtc = pProtocol->GetRawProtocol()->GetType();

	ValidPrtcMap::iterator it = m_ValidPrtcMap.find(idPrtc);
	if (it != m_ValidPrtcMap.end())
	{
		m_UnprocPrtcsList.AddTail(pProtocol);
		return true;
	}

	return false;
}

//	Tick process
void CECPrtcPBIProc::Process()
{
	using namespace GNET;

	int i, iNumInfo = m_aBriefInfoPrtcs.GetSize();
	if (!iNumInfo)
		return;

	m_BriefInfoMap.clear();

	for (i=0; i < iNumInfo; i++)
	{
		GetPlayerBriefInfo_Re* p = m_aBriefInfoPrtcs[i];
		ProcessPBIPrtc(p);
	}

	CECGameSession* pSession = g_pGame->GetGameSession();

	ALISTPOSITION pos = m_UnprocPrtcsList.GetHeadPosition();
	while (pos)
	{
		ALISTPOSITION posTemp = pos;
		CECNetProtocolBase* p = m_UnprocPrtcsList.GetNext(pos);

		if (p->Process(pSession))
		{
			p->Destroy();
			m_UnprocPrtcsList.RemoveAt(posTemp);
		}
	}

	//	Release player brief info protocols
	for (i=0; i < iNumInfo; i++)
	{
		GetPlayerBriefInfo_Re* p = m_aBriefInfoPrtcs[i];
		p->Destroy();
	}

	m_aBriefInfoPrtcs.RemoveAll(false);
}

//	Process one player brief info protocol
void CECPrtcPBIProc::ProcessPBIPrtc(GNET::GetPlayerBriefInfo_Re* pProtocol)
{
	using namespace GNET;

	if (pProtocol->retcode != ERR_SUCCESS)
		return;

	CECGameRun* pGameRun = g_pGame->GetGameRun();

	switch (pProtocol->reason)
	{
	case 0:		//	We try to get host player's team member's information.
	{
		CECTeam* pTeam = g_pGame->GetGameRun()->GetHostPlayer()->GetTeam();
		if (pTeam)
			pTeam->UpdateMemberInfo(*pProtocol);

		break;
	}
	case 1:		//	Wait other protocols to handle
	{
		//	Update host's team member information
		for (int i=0; i < (int)pProtocol->playerlist.size(); i++)
		{
			PlayerBriefInfo* pInfo = &pProtocol->playerlist[i];
			m_BriefInfoMap[pInfo->roleid] = pInfo;

			//	Name-id pair got this time always be right, so we force to
			//	overwrite old pair
			ACString strName = ACString((const ACHAR*)pInfo->name.begin(), pInfo->name.size() / sizeof (ACHAR));
			pGameRun->AddPlayerName(pInfo->roleid, strName, true);
		}

		break;
	}
	case 2:		//	We just try to get players' names
	{
		for (int i=0; i < (int)pProtocol->playerlist.size(); i++)
		{
			//	Name-id pair got this time always be right, so we force to
			//	overwrite old pair
			const PlayerBriefInfo& Info = pProtocol->playerlist[i];
			ACString strName = ACString((const ACHAR*)Info.name.begin(), Info.name.size() / sizeof (ACHAR));
			pGameRun->AddPlayerName(Info.roleid, strName, true);
		}

		break;
	}
	case 3:		//	Get player's brief infomation to handle team invitation
	{
		CECHostPlayer* pHost = pGameRun->GetHostPlayer();
		if (!pHost)
			return;

		for (int i=0; i < (int)pProtocol->playerlist.size(); i++)
			pHost->TeamInvite(pProtocol->playerlist[i]);
		
		break;
	}
	case 4:		// ¸±±¾ÌßÈË
	{
		const PlayerBriefInfo& Info = pProtocol->playerlist[0];
		ACString strName = ACString((const ACHAR*)Info.name.begin(), Info.name.size() / sizeof (ACHAR));
		pGameRun->AddPlayerName(Info.roleid, strName, true);
		CECBattleDungeonMan::getSingletonPtr()->DoKickPlayer(Info.roleid);

	}
	default:

		ASSERT(0);
		break;
	}
}



