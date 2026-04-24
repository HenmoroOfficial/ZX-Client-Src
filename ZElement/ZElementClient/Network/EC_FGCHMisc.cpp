/*
 * FILE: EC_FGCHMisc.cpp
 *
 * DESCRIPTION: 
 *
 * CREATED BY: Wangrui, 2009/4/1
 *
 * HISTORY: 
 *
 * Copyright (c) 2004 Archosaur Studio, All Rights Reserved.
 */

#include "EC_Global.h"
#include "EC_FunGamePrtcHandler.h"
#include "EC_FGPDataType.h"
#include "EC_Game.h"
#include "EC_FunGame.h"
#include "EC_GameRun.h"
#include "EC_UIManager.h"
#include "EC_GameUIMan.h"
#include "DlgGamePlaza.h"
#include "DlgGamePoker.h"

#include "gnoctets.h"
#include "fungamedatasend.hpp"

///////////////////////////////////////////////////////////////////////////
//	
//	Define and Macro
//	
///////////////////////////////////////////////////////////////////////////

#define new A_DEBUG_NEW

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
//	Implement CECFunGamePrtcHandler
//	
///////////////////////////////////////////////////////////////////////////



int CECFunGamePrtcHandler::OnFGCmd_GetRoomList(const FGCMDINFO& fgci)
{
	using namespace FG_S2C;
	using namespace FG_S2C::INFO;
	using namespace FG_S2C::CMD;

	CECFunGamePlaza* pPlaza = g_pGame->GetFunGamePlaza();

	fgcmd_get_room_list cmd;
	if (!cmd.FillData(fgci.pData, fgci.iDataSize))
		return 0;

	for (int i=0; i<cmd.room_size; i++)
		pPlaza->SetFGRoomInfo(fgci.byGameType, &(cmd.data[i]));
	
	((CDlgGamePlaza *)g_pGame->GetGameRun()->GetUIManager()->GetInGameUIMan()->GetDialog("Win_GamePoker_Hall"))->UpdatePlaza();

	return 1;
}

int CECFunGamePrtcHandler::OnFGCmd_NotifyRoomInfo(const FGCMDINFO& fgci)
{
	using namespace FG_S2C;
	using namespace FG_S2C::INFO;
	using namespace FG_S2C::CMD;

	CECFunGamePlaza* pPlaza = g_pGame->GetFunGamePlaza();

	fgcmd_notify_room_info cmd;
	if (!cmd.FillData(fgci.pData, fgci.iDataSize))
		return 0;
	
	pPlaza->SetFGRoomInfo(fgci.byGameType, &(cmd.data));

	CECGameUIMan* pUIMan = g_pGame->GetGameRun()->GetUIManager()->GetInGameUIMan();

	((CDlgGamePoker *)pUIMan->GetDialog("Win_GamePoker_Match"))->PokerCmd_NotifyRoomInfo(cmd.data);
	pUIMan->GetDialog("Win_GamePoker_Match")->Show(true);

	return 1;
}

int CECFunGamePrtcHandler::OnFGCmd_PlayerInfo(const FGCMDINFO& fgci)
{
	using namespace FG_S2C;
	using namespace FG_S2C::INFO;
	using namespace FG_S2C::CMD;

	CECFunGamePlaza* pPlaza = g_pGame->GetFunGamePlaza();

	fgcmd_player_info cmd;
	if (!cmd.FillData(fgci.pData, fgci.iDataSize))
		return 0;

	//	This protocol is not finished yet
	pPlaza->SetFGPlayerInfo(0, &(cmd.data));
	CECGameUIMan* pUIMan = g_pGame->GetGameRun()->GetUIManager()->GetInGameUIMan();
	((CDlgGamePlaza *)pUIMan->GetDialog("Win_GamePoker_Hall"))->UpdatePlaza();

	return 1;
}

int CECFunGamePrtcHandler::OnFGCmd_PlayerEnterRoom(const FGCMDINFO& fgci)
{
	using namespace FG_S2C;
	using namespace FG_S2C::INFO;
	using namespace FG_S2C::CMD;

	CECFunGamePlaza* pPlaza = g_pGame->GetFunGamePlaza();

	fgcmd_player_enter_room cmd;
	if (!cmd.FillData(fgci.pData, fgci.iDataSize))
		return 0;

	pPlaza->SetFGPlayerInfo(cmd.roomid, &(cmd.data));

	CECGameUIMan* pUIMan = g_pGame->GetGameRun()->GetUIManager()->GetInGameUIMan();
	CDlgGamePoker *pDlgPoker = (CDlgGamePoker *)pUIMan->GetDialog("Win_GamePoker_Match");
	((CDlgGamePlaza *)g_pGame->GetGameRun()->GetUIManager()->GetInGameUIMan()->GetDialog("Win_GamePoker_Hall"))->PlayerLeaveHall(cmd.data.roleid);
	if ( pDlgPoker->IsInRoom() && pDlgPoker->GetRoomId()==cmd.roomid )
	{
		((CDlgGamePoker *)pUIMan->GetDialog("Win_GamePoker_Match"))->PokerCmd_PlayerEnterRoom(cmd.data);
	}

	return 1;
}

int CECFunGamePrtcHandler::OnFGCmd_PlayerLeaveRoom(const FGCMDINFO& fgci)
{
	using namespace FG_S2C;
	using namespace FG_S2C::INFO;
	using namespace FG_S2C::CMD;

	CECFunGamePlaza* pPlaza = g_pGame->GetFunGamePlaza();

	fgcmd_player_leave_room* pCmd = (fgcmd_player_leave_room*)fgci.pData;

	int iDir = pPlaza->FindPlayerInRoom(int(pCmd->roomid), pCmd->roleid);
	if (iDir != -1)
	{
		const FG_ROOMINFO* pRoom = pPlaza->GetFGRoomInfo(pCmd->roomid);
		FG_PLAYERINFO info = pRoom->players[iDir];
		pPlaza->PlayerEnterHall(&info);
		pPlaza->ClearFGPlayerInfo(int(pCmd->roomid), iDir);	
	}

	CECGameUIMan* pUIMan = g_pGame->GetGameRun()->GetUIManager()->GetInGameUIMan();
	CDlgGamePoker *pDlgPoker = (CDlgGamePoker *)pUIMan->GetDialog("Win_GamePoker_Match");
	if ( pDlgPoker->IsInRoom() && pDlgPoker->GetRoomId()==pCmd->roomid )
		((CDlgGamePoker *)pUIMan->GetDialog("Win_GamePoker_Match"))->PokerCmd_PlayerLeaveRoom(pCmd->roleid);

	return 1;
}

int CECFunGamePrtcHandler::OnFGCmd_PlayerStateChange(const FGCMDINFO& fgci)
{
	using namespace FG_S2C;
	using namespace FG_S2C::INFO;
	using namespace FG_S2C::CMD;

	fgcmd_player_state_change* pCmd = (fgcmd_player_state_change*)fgci.pData;
	CECFunGamePlaza* pPlaza = g_pGame->GetFunGamePlaza();
	
	FG_PLAYERINFO* pInfo = pPlaza->GetFGPlayerInfo(pCmd->roomid, pCmd->data.direction);
	if (pInfo)
	{
		if (pInfo->idPlayer != pCmd->data.roleid)
			return 0;

		pInfo->iState = pCmd->data.state;
	}
	
	CECGameUIMan* pUIMan = g_pGame->GetGameRun()->GetUIManager()->GetInGameUIMan();
	((CDlgGamePoker *)pUIMan->GetDialog("Win_GamePoker_Match"))->UpdatePlayState(pCmd->data.direction, pCmd->data.state);
	
	return 1;
}

int CECFunGamePrtcHandler::OnFGCmd_PlayerReenterInvite(const FGCMDINFO& fgci)
{
	return 1;
}

int CECFunGamePrtcHandler::OnFGCmd_RoomGameIsReady(const FGCMDINFO& fgci)
{
	using namespace FG_S2C;
	using namespace FG_S2C::CMD;

	fgcmd_room_game_is_ready* pCmd = (fgcmd_room_game_is_ready*)fgci.pData;
	CECGameUIMan* pUIMan = g_pGame->GetGameRun()->GetUIManager()->GetInGameUIMan();
	
	((CDlgGamePoker *)pUIMan->GetDialog("Win_GamePoker_Match"))->PokerCmd_RoomIsReady(pCmd->roomid);
	return 1;
}

int CECFunGamePrtcHandler::OnFGCmd_RoomDispatchNormalCard(const FGCMDINFO& fgci)
{
	using namespace FG_S2C;
	using namespace FG_S2C::INFO;
	using namespace FG_S2C::CMD;

	fgcmd_room_dispatch_normal_card cmd;
	if (!cmd.FillData(fgci.pData, fgci.iDataSize))
		return 0;

	CECGameUIMan* pUIMan = g_pGame->GetGameRun()->GetUIManager()->GetInGameUIMan();

	switch(fgci.byGameType)
	{
	case 1:		//	斗地主
	{	
		((CDlgGamePoker *)pUIMan->GetDialog("Win_GamePoker_Match"))->PokerCmd_RoomDispatchNormalCard(cmd.cards.card_size, cmd.cards.cards, cmd.next_roleid);
		break;
	}
	default:
	    break;
	}

	return 1;
}

int CECFunGamePrtcHandler::OnFGCmd_PlayerPickupNormalCard(const FGCMDINFO& fgci)
{
	using namespace FG_S2C;
	using namespace FG_S2C::INFO;
	using namespace FG_S2C::CMD;

	fgcmd_player_pickup_normal_card cmd;
	if (!cmd.FillData(fgci.pData, fgci.iDataSize))
		return 0;

	CECGameUIMan* pUIMan = g_pGame->GetGameRun()->GetUIManager()->GetInGameUIMan();

	switch(fgci.byGameType)
	{
	case 1:		//	斗地主
	{	
		((CDlgGamePoker *)pUIMan->GetDialog("Win_GamePoker_Match"))->PokerCmd_PlayerPickupNormalCard(cmd.cards.card_size, cmd.cards.cards, cmd.roleid);
		break;
	}
	default:
	    break;
	}

	return 1;
}

int CECFunGamePrtcHandler::OnFGCmd_PlayerInturnNormalCard(const FGCMDINFO& fgci)
{
	using namespace FG_S2C;
	using namespace FG_S2C::INFO;
	using namespace FG_S2C::CMD;

	fgcmd_player_inturn_normal_card* pCmd = (fgcmd_player_inturn_normal_card*)fgci.pData;
	CECGameUIMan* pUIMan = g_pGame->GetGameRun()->GetUIManager()->GetInGameUIMan();

	switch(fgci.byGameType)
	{
	case 1:		//	斗地主
	{	
		((CDlgGamePoker *)pUIMan->GetDialog("Win_GamePoker_Match"))->PokerCmd_PlayerInturnNormalCard(pCmd->roleid);
		break;
	}
	default:
	    break;
	}

	return 1;
}

int CECFunGamePrtcHandler::OnFGCmd_PlayerThrowNormalCard(const FGCMDINFO& fgci)
{
	using namespace FG_S2C;
	using namespace FG_S2C::INFO;
	using namespace FG_S2C::CMD;

	fgcmd_player_throw_normal_card cmd;
	if (!cmd.FillData(fgci.pData, fgci.iDataSize))
		return 0;

	CECGameUIMan* pUIMan = g_pGame->GetGameRun()->GetUIManager()->GetInGameUIMan();

	switch(fgci.byGameType)
	{
	case 1:		//	斗地主
	{	
		((CDlgGamePoker *)pUIMan->GetDialog("Win_GamePoker_Match"))->PokerCmd_PlayerThrowNormalCard(cmd.roleid, cmd.next_roleid, cmd.cards.card_size, cmd.cards.cards);
		break;
	}
	default:
	    break;
	}

	return 1;
}

int CECFunGamePrtcHandler::OnFGCmd_PlayerGiveupNormalCard(const FGCMDINFO& fgci)
{
	using namespace FG_S2C;
	using namespace FG_S2C::INFO;
	using namespace FG_S2C::CMD;

	fgcmd_player_giveup_normal_card* pCmd = (fgcmd_player_giveup_normal_card*)fgci.pData;
	CECGameUIMan* pUIMan = g_pGame->GetGameRun()->GetUIManager()->GetInGameUIMan();

	switch(fgci.byGameType)
	{
	case 1:		//	斗地主
	{	
		((CDlgGamePoker *)pUIMan->GetDialog("Win_GamePoker_Match"))->PokerCmd_PlayerGiveupNormalCard(pCmd->roleid, pCmd->next_roleid);
		break;
	}
	default:
	    break;
	}

	return 1;
}

int CECFunGamePrtcHandler::OnFGCmd_PlayerAuction(const FGCMDINFO& fgci)
{
	using namespace FG_S2C;
	using namespace FG_S2C::INFO;
	using namespace FG_S2C::CMD;

	fgcmd_player_auction* pCmd = (fgcmd_player_auction*)fgci.pData;
	CECGameUIMan* pUIMan = g_pGame->GetGameRun()->GetUIManager()->GetInGameUIMan();

	switch(fgci.byGameType)
	{
	case 1:		//	斗地主
	{	
		((CDlgGamePoker *)pUIMan->GetDialog("Win_GamePoker_Match"))->PokerCmd_PlayerAuction(pCmd->roleid, pCmd->next_roleid, pCmd->score);
		break;
	}
	default:
	    break;
	}

	return 1;
}

int CECFunGamePrtcHandler::OnFGCmd_RoomGameEnd(const FGCMDINFO& fgci)
{
	using namespace FG_S2C;
	using namespace FG_S2C::INFO;
	using namespace FG_S2C::CMD;

	fgcmd_room_game_end* pCmd = (fgcmd_room_game_end*)fgci.pData;
	CECGameUIMan* pUIMan = g_pGame->GetGameRun()->GetUIManager()->GetInGameUIMan();

	switch(fgci.byGameType)
	{
	case 1:		//	斗地主
	{	
		((CDlgGamePoker *)pUIMan->GetDialog("Win_GamePoker_Match"))->PokerCmd_RoomGameEnd(pCmd->roleid, pCmd->reason);
		break;
	}
	default:
	    break;
	}

	return 1;
}

int CECFunGamePrtcHandler::OnFGCmd_LandlordResultBroadcast(const FGCMDINFO& fgci)
{
	using namespace FG_S2C;
	using namespace FG_S2C::INFO;
	using namespace FG_S2C::CMD;

	fgcmd_landlord_result_broadcast cmd;
	if (!cmd.FillData(fgci.pData, fgci.iDataSize))
		return 0;
	
	CECFunGamePlaza* pPlaza = g_pGame->GetFunGamePlaza();

	CECGameUIMan* pUIMan = g_pGame->GetGameRun()->GetUIManager()->GetInGameUIMan();

	switch(fgci.byGameType)
	{
	case 1:		//	斗地主
	{	
		pUIMan->AddChatMessage(pUIMan->GetStringFromTable(9194), GP_CHAT_MISC);
		for (int i=0; i<cmd.size; i++)
		{
			ACHAR szResult[50] = {_AL("")};
			int iDir = pPlaza->FindPlayerInRoom(cmd.roomid, cmd.results[i].roleid);
			FG_PLAYERINFO* pInfo = pPlaza->GetFGPlayerInfo(cmd.roomid, iDir);
			pInfo->iScore += cmd.results[i].score;
			if (cmd.results[i].score>0)
				pInfo->iWinCount++;
			else
				pInfo->iLoseCount++;
			a_sprintf(szResult, pUIMan->GetStringFromTable(9195), pInfo->strName, cmd.results[i].score);
			pUIMan->AddChatMessage(szResult, GP_CHAT_MISC);
		}
		
		((CDlgGamePoker *)pUIMan->GetDialog("Win_GamePoker_Match"))->RefreshResult(cmd);
		break;
	}
	default:
	    break;
	}

	return 1;
}

int CECFunGamePrtcHandler::OnFGCmd_LandlordReonlineDataSend(const FGCMDINFO& fgci)
{
	return 1;
}

int CECFunGamePrtcHandler::OnFGCmd_PlayerEnterHall(const FGCMDINFO& fgci)
{
	using namespace FG_S2C;
	using namespace FG_S2C::INFO;
	using namespace FG_S2C::CMD;
	
	fgcmd_player_enter_hall cmd;
	if (!cmd.FillData(fgci.pData, fgci.iDataSize))
		return 0;

	CECFunGamePlaza* pPlaza = g_pGame->GetFunGamePlaza();
	player_base_info baseInfo = cmd.data;
	pPlaza->PlayerEnterHall(&baseInfo);
	return 1;
}

int CECFunGamePrtcHandler::OnFGCmd_PlayerLeaveHall(const FGCMDINFO& fgci)
{
	using namespace FG_S2C;
	using namespace FG_S2C::INFO;
	using namespace FG_S2C::CMD;
	
	fgcmd_player_leave_hall* pCmd = (fgcmd_player_leave_hall*)fgci.pData;

	CECFunGamePlaza* pPlaza = g_pGame->GetFunGamePlaza();
	pPlaza->PlayerLeaveHall(pCmd->roleid);
	((CDlgGamePlaza *)g_pGame->GetGameRun()->GetUIManager()->GetInGameUIMan()->GetDialog("Win_GamePoker_Hall"))->PlayerLeaveHall(pCmd->roleid);
	return 1;
}

int CECFunGamePrtcHandler::OnFGCmd_NotifyHallMembers(const FGCMDINFO& fgci)
{
	using namespace FG_S2C;
	using namespace FG_S2C::INFO;
	using namespace FG_S2C::CMD;
	
	fgcmd_notify_hall_members cmd;
	if (!cmd.FillData(fgci.pData, fgci.iDataSize))
		return 0;

	CECFunGamePlaza* pPlaza = g_pGame->GetFunGamePlaza();
	pPlaza->NotifyHallMember(&cmd);
	return 1;
}
