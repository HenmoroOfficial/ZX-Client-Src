/*
 * FILE: EC_FunGame.cpp
 *
 * DESCRIPTION: 
 *
 * CREATED BY: Wangrui, 2009/03/17
 *
 * HISTORY: 
 *
 * Copyright (c) 2004 Archosaur Studio, All Rights Reserved.
 */

#include "EC_Global.h"
#include "EC_Game.h"
#include "EC_GameRun.h"
#include "EC_GameSession.h"
#include "EC_FunGame.h"
#include "EC_FGPDataType.h"
#include "EC_UIManager.h"
#include "EC_GameUIMan.h"

#include "DlgGamePlaza.h"
#include "DlgGamePoker.h"

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
//	Implement CECFunGame
//	
///////////////////////////////////////////////////////////////////////////

CECFunGamePlaza::CECFunGamePlaza()
{
	ClearFGPlaza();
}

CECFunGamePlaza::~CECFunGamePlaza()
{

}

void CECFunGamePlaza::ClearFGPlaza()
{
	m_vecHallPlayers.clear();
	for (int i=0; i<PLAZA_ROOM_MAX; i++)
		ClearFGRoomInfo(i); 
}

FG_ROOMINFO* CECFunGamePlaza::GetFGRoomInfo(int idRoom) 
{
	if (idRoom >=0 && idRoom < PLAZA_ROOM_MAX)
	{
		return &m_aRooms[idRoom];
	}
	return NULL; 
}

void CECFunGamePlaza::ClearFGRoomInfo(int idRoom)
{ 
	ASSERT(idRoom >=0 && idRoom < PLAZA_ROOM_MAX);

	m_aRooms[idRoom].bInit = false;
	m_aRooms[idRoom].iGameType = 0;
	m_aRooms[idRoom].iPlayerNum = 0;

	for (int i=0; i<ROOM_PLAYER_MAX; i++)
		ClearFGPlayerInfo(idRoom, i);
}

void CECFunGamePlaza::ClearFGPlayerInfo(int idRoom, int iDir)
{ 
	ASSERT(idRoom >=0 && idRoom < PLAZA_ROOM_MAX && iDir >= 0 && iDir < ROOM_PLAYER_MAX); 

	m_aRooms[idRoom].players[iDir].idPlayer = 0;
	m_aRooms[idRoom].players[iDir].iState = 0;
	m_aRooms[idRoom].players[iDir].cGender = 0;
	m_aRooms[idRoom].players[iDir].iScore = 0;
	m_aRooms[idRoom].players[iDir].iWinCount = 0;
	m_aRooms[idRoom].players[iDir].iLoseCount = 0;
	m_aRooms[idRoom].players[iDir].iDrawCount = 0;
	m_aRooms[idRoom].players[iDir].iEscapeCount = 0;
	m_aRooms[idRoom].players[iDir].strName = _AL("");
}

void CECFunGamePlaza::SetFGRoomInfo(int iGameType, const FG_S2C::INFO::room_info* pInfo)
{
	using namespace FG_S2C;
	using namespace FG_S2C::INFO;

	int idRoom = pInfo->roomid;
	if (idRoom < 0 || idRoom >= PLAZA_ROOM_MAX)
	{
		ASSERT(0);
		return;
	}

	ClearFGRoomInfo(idRoom);
	m_vecHallPlayers.clear();
	
	int iDir;

	m_aRooms[idRoom].bInit	= true;
	m_aRooms[idRoom].iGameType = iGameType;
	m_aRooms[idRoom].iPlayerNum = pInfo->player_size;
	for (int i=0; i<pInfo->player_size; i++)
	{
		FG_PLAYERINFO playerInfo;
		playerInfo.idPlayer = pInfo->players[i].roleid;
		playerInfo.iState = pInfo->players[i].state;
		playerInfo.cGender = pInfo->players[i].gender;
		playerInfo.iScore = pInfo->players[i].score;
		playerInfo.iWinCount = pInfo->players[i].win_count;
		playerInfo.iLoseCount = pInfo->players[i].lose_count;
		playerInfo.iDrawCount = pInfo->players[i].draw_count;
		playerInfo.iEscapeCount = pInfo->players[i].escape_count;
		AWString strTmp((ACHAR*)pInfo->players[i].name, pInfo->players[i].name_size / sizeof(ACHAR));
		playerInfo.strName = strTmp;
		if (playerInfo.iState == 5 )	//´óÌü
		{
			m_vecHallPlayers.push_back(playerInfo);
		}
		else
		{
			iDir = pInfo->players[i].direction;
			
			ClearFGPlayerInfo(idRoom, iDir);
			
			m_aRooms[idRoom].players[iDir] = playerInfo;
		}
	}
}

void CECFunGamePlaza::SetFGPlayerInfo(int idRoom, const FG_S2C::INFO::player_base_info* pInfo)
{
	using namespace FG_S2C;
	using namespace FG_S2C::INFO;
	
	FG_PLAYERINFO* pPlayerInfo = GetFGPlayerInfo(idRoom, pInfo->direction);
	if (!pPlayerInfo)
		return;

	pPlayerInfo->idPlayer = pInfo->roleid;
	pPlayerInfo->iState = pInfo->state;
	pPlayerInfo->cGender = pInfo->gender;
	pPlayerInfo->iScore = pInfo->score;
	pPlayerInfo->iWinCount = pInfo->win_count;
	pPlayerInfo->iLoseCount = pInfo->lose_count;
	pPlayerInfo->iDrawCount = pInfo->draw_count;
	pPlayerInfo->iEscapeCount = pInfo->escape_count;
	AWString strName((ACHAR*)pInfo->name, pInfo->name_size/sizeof(ACHAR));
	pPlayerInfo->strName = strName;
}

FG_PLAYERINFO* CECFunGamePlaza::GetFGPlayerInfo(int idRoom, int iDir)
{
	if (idRoom < 0 || idRoom >= PLAZA_ROOM_MAX || iDir<0)
	{
		ASSERT(0);
		return NULL;
	}
	
	return &(m_aRooms[idRoom].players[iDir]);
}

int CECFunGamePlaza::FindPlayerInRoom(int idRoom, int idPlayer)
{
	const FG_ROOMINFO* pRoomInfo = GetFGRoomInfo(idRoom);
	
	int iDir;
	for (iDir=0; iDir<ROOM_PLAYER_MAX; iDir++)
	{
		if (pRoomInfo->players[iDir].idPlayer == idPlayer)
			break;
	}

	if (iDir == ROOM_PLAYER_MAX)
		return -1;
	else
		return iDir;
}

void CECFunGamePlaza::PlayerEnterHall(FG_S2C::INFO::player_base_info *pData)
{
	FG_S2C::INFO::player_base_info* pCmd = (FG_S2C::INFO::player_base_info*)pData;

	FG_PLAYERINFO info;
	info.idPlayer = pCmd->roleid;
	info.iState = pCmd->state;
	info.cGender = pCmd->gender;
	info.iScore = pCmd->score;
	info.iWinCount = pCmd->win_count;
	info.iLoseCount = pCmd->lose_count;
	info.iDrawCount = pCmd->draw_count;
	info.iEscapeCount = pCmd->escape_count;
	info.strName = AWString((ACHAR*)pCmd->name, pCmd->name_size/sizeof(ACHAR));
	m_vecHallPlayers.push_back(info);
	((CDlgGamePlaza *)g_pGame->GetGameRun()->GetUIManager()->GetInGameUIMan()->GetDialog("Win_GamePoker_Hall"))->PlayerEnterHall(&info);
}

void CECFunGamePlaza::PlayerEnterHall(FG_PLAYERINFO *pData)
{
	FG_PLAYERINFO info = *pData;
	m_vecHallPlayers.push_back(info);
	((CDlgGamePlaza *)g_pGame->GetGameRun()->GetUIManager()->GetInGameUIMan()->GetDialog("Win_GamePoker_Hall"))->PlayerEnterHall(&info);
}

void CECFunGamePlaza::PlayerLeaveHall(int playerId)
{
	abase::vector<FG_PLAYERINFO>::iterator it = m_vecHallPlayers.begin();
	while (it!=m_vecHallPlayers.end())
	{
		if (it->idPlayer == playerId)
		{
			m_vecHallPlayers.erase(it);
			break;
		}
		it++;
	}
}

void CECFunGamePlaza::NotifyHallMember(void *pData)
{
	FG_S2C::CMD::fgcmd_notify_hall_members* pCmd = (FG_S2C::CMD::fgcmd_notify_hall_members*)pData;
	m_vecHallPlayers.clear();
	for (int i=0; i<pCmd->member_num; i++)
	{
		FG_PLAYERINFO info;
		info.idPlayer = pCmd->players[i].roleid;
		info.iState = pCmd->players[i].state;
		info.cGender = pCmd->players[i].gender;
		info.iScore = pCmd->players[i].score;
		info.iWinCount = pCmd->players[i].win_count;
		info.iLoseCount = pCmd->players[i].lose_count;
		info.iDrawCount = pCmd->players[i].draw_count;
		info.iEscapeCount = pCmd->players[i].escape_count;
		info.strName = AWString((ACHAR*)pCmd->players[i].name, pCmd->players[i].name_size/sizeof(ACHAR));
		m_vecHallPlayers.push_back(info);
	}
	((CDlgGamePlaza *)g_pGame->GetGameRun()->GetUIManager()->GetInGameUIMan()->GetDialog("Win_GamePoker_Hall"))->UpdateHallMember();
}