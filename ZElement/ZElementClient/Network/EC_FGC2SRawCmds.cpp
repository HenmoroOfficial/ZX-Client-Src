/*
 * FILE: EC_FGC2SRawCmds.cpp
 *
 * DESCRIPTION: Fun game command types
 *
 * CREATED BY: Wangrui, 2009/3/12
 *
 * HISTORY: 
 *
 * Copyright (c) 2004 Archosaur Studio, All Rights Reserved.
 */

#include "EC_Global.h"
#include "EC_GameSession.h"
#include "EC_FGPDataType.h"
#include "EC_Game.h"

///////////////////////////////////////////////////////////////////////////
//	
//	Define and Macro
//	
///////////////////////////////////////////////////////////////////////////

//	Fixed length fun game command structure
#define FIXLEN_FUNGAMECMD(byGame, byCmd, content_type) \
	using namespace FG_C2S; \
	using namespace FG_C2S::INFO; \
	using namespace FG_C2S::CMD; \
	struct \
	{ \
		fgc2s_cmd_header fghd; \
		char content[sizeof (content_type)]; \
	} fgcmd; \
	fgcmd.fghd.game_type = byGame; \
	fgcmd.fghd.cmd_type = byCmd;	\
    content_type* pFGCmd = (content_type*)(&fgcmd.content); \
	int iSize = sizeof (fgc2s_cmd_header) + sizeof (content_type);

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
//	Implement
//	
///////////////////////////////////////////////////////////////////////////

//	Implementation of poker cards commands

void fgc2s_SendCmdGetRoomList(BYTE byGameType, BYTE byIndex)
{
	FIXLEN_FUNGAMECMD(byGameType, GET_ROOM_LIST, fgcmd_get_room_list)

	pFGCmd->index = byIndex;

	g_pGame->GetGameSession()->SendFunGameData(&fgcmd, iSize);
}

void fgc2s_SendCmdPlayerEnterRoom(BYTE byGameType, WORD wRoomID, BYTE byDir)
{
	FIXLEN_FUNGAMECMD(byGameType, PLAYER_ENTER_ROOM, fgcmd_player_enter_room)

	pFGCmd->roomid = wRoomID;
	pFGCmd->direction = byDir;

	g_pGame->GetGameSession()->SendFunGameData(&fgcmd, iSize);
}

void fgc2s_SendCmdPlayerBeginGame(BYTE byGameType, WORD wRoomID)
{
	FIXLEN_FUNGAMECMD(byGameType, PLAYER_BEGIN_GAME, fgcmd_player_begin_game)

	pFGCmd->roomid = wRoomID;

	g_pGame->GetGameSession()->SendFunGameData(&fgcmd, iSize);
}

void fgc2s_SendCmdPlayerReEnterRoom(BYTE byGameType, WORD wRoomID)
{
	FIXLEN_FUNGAMECMD(byGameType, PLAYER_REENTER_ROOM, fgcmd_player_reenter_room)

	pFGCmd->roomid = wRoomID;

	g_pGame->GetGameSession()->SendFunGameData(&fgcmd, iSize);
}

void fgc2s_SendCmdPlayerLeaveRoom(BYTE byGameType, WORD wRoomID)
{
	FIXLEN_FUNGAMECMD(byGameType, PLAYER_LEAVE_ROOM, fgcmd_player_leave_room)

	pFGCmd->roomid = wRoomID;

	g_pGame->GetGameSession()->SendFunGameData(&fgcmd, iSize);
}

void fgc2s_SendCmdPlayerThrowNormalCard(BYTE byGameType, WORD wRoomID, BYTE byCardNum, const BYTE* aCards)
{
	using namespace FG_C2S;
	using namespace FG_C2S::INFO;
	using namespace FG_C2S::CMD;

	int iSize = sizeof(fgc2s_cmd_header) + sizeof(short) + sizeof(char) + byCardNum * sizeof(char);
	
	BYTE* pBuf = new BYTE[iSize];
	if (!pBuf)
		return;

	((fgc2s_cmd_header*)pBuf)->game_type = byGameType;
	((fgc2s_cmd_header*)pBuf)->cmd_type = PLAYER_THROW_NORMAL_CARD;

	fgcmd_player_throw_normal_card* pFGCmd = (fgcmd_player_throw_normal_card*)(pBuf + sizeof(fgc2s_cmd_header));
	pFGCmd->roomid = wRoomID;
	pFGCmd->cards.card_size = byCardNum;
		
	// Copy cards data
	if (byCardNum)
		memcpy(pFGCmd->cards.card, aCards, byCardNum * sizeof(char));

	g_pGame->GetGameSession()->SendFunGameData(pBuf, iSize);
	
	delete [] pBuf;
}

void fgc2s_SendCmdPlayerPickupNormalCard(BYTE byGameType, WORD wRoomID)
{
	FIXLEN_FUNGAMECMD(byGameType, PLAYER_PICKUP_NORMAL_CARD, fgcmd_player_pickup_normal_card)

	pFGCmd->roomid = wRoomID;

	g_pGame->GetGameSession()->SendFunGameData(&fgcmd, iSize);
}

void fgc2s_SendCmdPlayerGiveupNormalCard(BYTE byGameType, WORD wRoomID)
{
	FIXLEN_FUNGAMECMD(byGameType, PLAYER_GIVEUP_NORMAL_CARD, fgcmd_player_giveup_normal_card)

	pFGCmd->roomid = wRoomID;

	g_pGame->GetGameSession()->SendFunGameData(&fgcmd, iSize);
}

void fgc2s_SendCmdPlayerAuction(BYTE byGameType, WORD wRoomID, int idRole, BYTE byScore)
{
	FIXLEN_FUNGAMECMD(byGameType, PLAYER_AUCTION, fgcmd_player_auction)

	pFGCmd->roomid = wRoomID;
	pFGCmd->roleid = idRole;
	pFGCmd->score = byScore;

	g_pGame->GetGameSession()->SendFunGameData(&fgcmd, iSize);
}

void fgc2s_SendCmdPlayerEnterHall(BYTE byGameType)
{
//	FIXLEN_FUNGAMECMD(byGameType, PLAYER_ENTER_HALL, player_enter_hall)

	using namespace FG_C2S;
	using namespace FG_C2S::INFO;
	using namespace FG_C2S::CMD;
	struct
	{
	fgc2s_cmd_header fghd;
	} fgcmd;
	fgcmd.fghd.game_type = byGameType;
	fgcmd.fghd.cmd_type = PLAYER_ENTER_HALL;
	int iSize = sizeof (fgc2s_cmd_header);
		
	g_pGame->GetGameSession()->SendFunGameData(&fgcmd, iSize);
}

void fgc2s_SendCmdPlayerLeaveHall(BYTE byGameType)
{
//	FIXLEN_FUNGAMECMD(byGameType, PLAYER_LEAVE_HALL, player_leave_hall)
		
	using namespace FG_C2S;
	using namespace FG_C2S::INFO;
	using namespace FG_C2S::CMD;
	struct
	{
		fgc2s_cmd_header fghd;
	} fgcmd;
	fgcmd.fghd.game_type = byGameType;
	fgcmd.fghd.cmd_type = PLAYER_LEAVE_HALL;
	int iSize = sizeof (fgc2s_cmd_header);

	g_pGame->GetGameSession()->SendFunGameData(&fgcmd, iSize);
}