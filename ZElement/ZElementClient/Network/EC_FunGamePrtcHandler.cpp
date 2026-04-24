/*
 * FILE: EC_GamePrtcHandler.cpp
 *
 * DESCRIPTION: 
 *
 * CREATED BY: Wangrui, 2009/3/11
 *
 * HISTORY: 
 *
 * Copyright (c) 2004 Archosaur Studio, All Rights Reserved.
 */

#include "EC_Global.h"
#include "EC_FunGamePrtcHandler.h"
#include "EC_FGPDataType.h"
#include "gnoctets.h"
#include "fungamedatasend.hpp"

#include <A3DMacros.h>
#include <ALog.h>
#include <AFile.h>

#define new A_DEBUG_NEW

///////////////////////////////////////////////////////////////////////////
//	
//	Define and Macro
//	
///////////////////////////////////////////////////////////////////////////

#define FIXFGCMD_SIZE_CHECK(cmd_type, ci) \
	return (sizeof (cmd_type) == ci.iDataSize);

#define VARFGCMD_SIZE_CHECK(cmd_type, ci) \
	return cmd_type::CheckValid(ci.pData, ci.iDataSize);

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
//	Implement CECGamePrtcHandler
//	
///////////////////////////////////////////////////////////////////////////

CECFunGamePrtcHandler::CECFunGamePrtcHandler()
{
	using namespace FG_S2C;
	
	m_pFGS2CCmdLog = NULL;
	m_pFGS2CCmdBinLog = NULL;	
	
	m_HandlerMap[GET_ROOM_LIST] = &CECFunGamePrtcHandler::OnFGCmd_GetRoomList;
	m_HandlerMap[NOTIFY_ROOM_INFO] = &CECFunGamePrtcHandler::OnFGCmd_NotifyRoomInfo;
	m_HandlerMap[PLAYER_INFO] = &CECFunGamePrtcHandler::OnFGCmd_PlayerInfo;
	m_HandlerMap[PLAYER_ENTER_ROOM] = &CECFunGamePrtcHandler::OnFGCmd_PlayerEnterRoom;
	m_HandlerMap[PLAYER_LEAVE_ROOM] = &CECFunGamePrtcHandler::OnFGCmd_PlayerLeaveRoom;

	m_HandlerMap[PLAYER_STATE_CHANGE] = &CECFunGamePrtcHandler::OnFGCmd_PlayerStateChange;
	m_HandlerMap[PLAYER_REENTER_INVITE] = &CECFunGamePrtcHandler::OnFGCmd_PlayerReenterInvite;
	m_HandlerMap[ROOM_GAME_IS_READY] = &CECFunGamePrtcHandler::OnFGCmd_RoomGameIsReady;
	m_HandlerMap[ROOM_DISPATCH_NORMAL_CARD] = &CECFunGamePrtcHandler::OnFGCmd_RoomDispatchNormalCard;
	m_HandlerMap[PLAYER_PICKUP_NORMAL_CARD] = &CECFunGamePrtcHandler::OnFGCmd_PlayerPickupNormalCard;

	m_HandlerMap[PLAYER_INTURN_NORMAL_CARD] = &CECFunGamePrtcHandler::OnFGCmd_PlayerInturnNormalCard;
	m_HandlerMap[PLAYER_THROW_NORMAL_CARD] = &CECFunGamePrtcHandler::OnFGCmd_PlayerThrowNormalCard;
	m_HandlerMap[PLAYER_GIVEUP_NORMAL_CARD] = &CECFunGamePrtcHandler::OnFGCmd_PlayerGiveupNormalCard;
	m_HandlerMap[PLAYER_AUCTION] = &CECFunGamePrtcHandler::OnFGCmd_PlayerAuction;
	m_HandlerMap[ROOM_GAME_END] = &CECFunGamePrtcHandler::OnFGCmd_RoomGameEnd;

	m_HandlerMap[LANDLORD_RESULT_BROADCAST] = &CECFunGamePrtcHandler::OnFGCmd_LandlordResultBroadcast;
	m_HandlerMap[LANDLORD_REONLINE_DATASEND] = &CECFunGamePrtcHandler::OnFGCmd_LandlordReonlineDataSend;
	m_HandlerMap[PLAYER_ENTER_HALL] = &CECFunGamePrtcHandler::OnFGCmd_PlayerEnterHall;
	m_HandlerMap[PLAYER_LEAVE_HALL] = &CECFunGamePrtcHandler::OnFGCmd_PlayerLeaveHall;
	m_HandlerMap[NOTIFY_HALL_MEMBERS] = &CECFunGamePrtcHandler::OnFGCmd_NotifyHallMembers;
}

CECFunGamePrtcHandler::~CECFunGamePrtcHandler()
{

}

//	On session opening
bool CECFunGamePrtcHandler::OnSessionOpen()
{
	m_nFGS2CCmdSize = 0;

	//	Create cmd log
	A3DRELEASE(m_pFGS2CCmdLog);
	m_pFGS2CCmdLog = new ALog;

	//	Determine log file's path
	AString str = "fgs2ccmd.log";
	OFSTRUCT st;
	if (OpenFile("Logs\\fgs2ccmd.log", &st, OF_EXIST) != HFILE_ERROR)
	{
		//	文件存在
		HFILE file = OpenFile("Logs\\fgs2ccmd.log", &st, OF_READ | OF_SHARE_EXCLUSIVE);
		if (file != HFILE_ERROR)
			CloseHandle((HANDLE)file);
		else
			str.Format("fgs2ccmd%d.log", time(NULL));
	}

	if (!m_pFGS2CCmdLog->Init(str, "Fun game server to Client command log!"))
	{
		A3DRELEASE(m_pFGS2CCmdLog)
		return true;
	}

	//	Create command bin log
	if (m_pFGS2CCmdBinLog)
	{
		m_pFGS2CCmdBinLog->Close();
		delete m_pFGS2CCmdBinLog;
		m_pFGS2CCmdBinLog = NULL;
	}

	m_pFGS2CCmdBinLog = new AFile;

	//	Determine log file's path
	str = "logs\\fgs2ccmdlog.bin";
	if (OpenFile(str, &st, OF_EXIST) != HFILE_ERROR)
	{
		//文件存在
		HFILE file = OpenFile(str, &st, OF_READ | OF_SHARE_EXCLUSIVE);
		if (file!= HFILE_ERROR)
			CloseHandle((HANDLE)file);
		else
			str.Format("logs\\fgs2ccmdlog%d.bin", time(NULL));
	}

	if (!m_pFGS2CCmdBinLog->Open(str, AFILE_CREATENEW | AFILE_BINARY | AFILE_NOHEAD))
	{
		m_pFGS2CCmdBinLog->Close();
		delete m_pFGS2CCmdBinLog;
		m_pFGS2CCmdBinLog = NULL;
		return true;
	}

	return true;
}

//	On session closing
bool CECFunGamePrtcHandler::OnSessionClose()
{
	A3DRELEASE(m_pFGS2CCmdLog);

	if (m_pFGS2CCmdBinLog)
	{
		m_pFGS2CCmdBinLog->Close();
		delete m_pFGS2CCmdBinLog;
		m_pFGS2CCmdBinLog = NULL;
	}

	return true;
}

//	Log Fun game S2C command data
void CECFunGamePrtcHandler::LogFGS2CCmdData(BYTE byCmd, BYTE byGameType, const BYTE* pDataBuf, int nSize)
{
	using namespace FG_S2C;

	m_nFGS2CCmdSize += nSize;

	if (m_pFGS2CCmdLog)
	{
		int iPrintSize = nSize;
		a_ClampRoof(iPrintSize, 600);

		char* szStr = new char [iPrintSize * 3 + 1];
		int iCount = 0;
		for (int i=0; i < iPrintSize; i++)
		{
			sprintf(&szStr[iCount], "%02x ", pDataBuf[i]);
			iCount += 3;
		}

		m_pFGS2CCmdLog->Log("total: %d, size: %d, fgcmd: %d, fgtype: %d, data: %s", m_nFGS2CCmdSize, nSize, byCmd, byGameType, szStr);

		delete [] szStr;
	}

	// output to binary log
	if (m_pFGS2CCmdBinLog) 
	{
		DWORD dwWriteLen;
		m_pFGS2CCmdBinLog->Write(&nSize, sizeof(nSize), &dwWriteLen);
		SYSTEMTIME sysTime;
		GetLocalTime(&sysTime);
		m_pFGS2CCmdBinLog->Write(&sysTime, sizeof(sysTime), &dwWriteLen);
		m_pFGS2CCmdBinLog->Write((void*)pDataBuf, nSize, &dwWriteLen);
		m_pFGS2CCmdBinLog->Flush();
	}
}

//	Check data size
bool CECFunGamePrtcHandler::CheckDataSize(FGCMDINFO& fgci)
{
	using namespace FG_S2C;
	using namespace FG_S2C::CMD;

	switch (fgci.byCmd)
	{
	case GET_ROOM_LIST:					VARFGCMD_SIZE_CHECK(fgcmd_get_room_list, fgci)					
	case NOTIFY_ROOM_INFO:				VARFGCMD_SIZE_CHECK(fgcmd_notify_room_info, fgci)			
	case PLAYER_INFO:					FIXFGCMD_SIZE_CHECK(fgcmd_player_info, fgci)
	case PLAYER_ENTER_ROOM:				VARFGCMD_SIZE_CHECK(fgcmd_player_enter_room, fgci)
	case PLAYER_LEAVE_ROOM:				FIXFGCMD_SIZE_CHECK(fgcmd_player_leave_room, fgci)
	case PLAYER_STATE_CHANGE:			FIXFGCMD_SIZE_CHECK(fgcmd_player_state_change, fgci)
	case PLAYER_REENTER_INVITE:			FIXFGCMD_SIZE_CHECK(fgcmd_player_state_change, fgci)
	case ROOM_GAME_IS_READY:			FIXFGCMD_SIZE_CHECK(fgcmd_room_game_is_ready, fgci)
	case ROOM_DISPATCH_NORMAL_CARD:		VARFGCMD_SIZE_CHECK(fgcmd_room_dispatch_normal_card, fgci)
	case PLAYER_PICKUP_NORMAL_CARD:		VARFGCMD_SIZE_CHECK(fgcmd_player_pickup_normal_card, fgci)
	case PLAYER_INTURN_NORMAL_CARD:		FIXFGCMD_SIZE_CHECK(fgcmd_player_inturn_normal_card, fgci)
	case PLAYER_THROW_NORMAL_CARD:		VARFGCMD_SIZE_CHECK(fgcmd_player_throw_normal_card, fgci)
	case PLAYER_GIVEUP_NORMAL_CARD:		FIXFGCMD_SIZE_CHECK(fgcmd_player_giveup_normal_card, fgci)
	case PLAYER_AUCTION:				FIXFGCMD_SIZE_CHECK(fgcmd_player_auction, fgci)
	case ROOM_GAME_END:					FIXFGCMD_SIZE_CHECK(fgcmd_room_game_end, fgci)
	case LANDLORD_RESULT_BROADCAST:		VARFGCMD_SIZE_CHECK(fgcmd_landlord_result_broadcast, fgci)
	case LANDLORD_REONLINE_DATASEND:	VARFGCMD_SIZE_CHECK(fgcmd_landlord_reonline_datasend, fgci)
	case PLAYER_ENTER_HALL:				VARFGCMD_SIZE_CHECK(fgcmd_player_enter_hall, fgci)
	case PLAYER_LEAVE_HALL:				FIXFGCMD_SIZE_CHECK(fgcmd_player_leave_hall, fgci)
	case NOTIFY_HALL_MEMBERS:			VARFGCMD_SIZE_CHECK(fgcmd_notify_hall_members, fgci)
	
		
	default:
		ASSERT(0);
		break;
	}
	
	return false;
}

/*	Process protocol.

	Return value:			
		0: process fail.
		1: process successfully
		2: protocol processing should be postponed
		3: process finished but don't remove this protocol
*/
int CECFunGamePrtcHandler::Process(GNET::Protocol* pProtocol)
{
	using namespace GNET;
	using namespace FG_S2C;

	FGCMDINFO fgci;

	GNET::Octets& Data = static_cast<FunGamedataSend*>(pProtocol)->data;
	fgci.byGameType = static_cast<fgs2c_cmd_header*>(Data.begin())->game_type;
	fgci.byCmd = static_cast<fgs2c_cmd_header*>(Data.begin())->cmd;
	fgci.pData = (BYTE*)Data.begin() + sizeof (fgs2c_cmd_header);
	fgci.iDataSize = Data.size() - sizeof (fgs2c_cmd_header);

#ifdef _DEBUG

	if (!CheckDataSize(fgci))
	{
		ASSERT(0 && ("Fun game command data size error."));
		return 0;
	}

#endif

	LogFGS2CCmdData(fgci.byCmd, fgci.byGameType, fgci.pData, fgci.iDataSize);
	
#ifdef _DEBUG
	char msg[200];
	sprintf(msg, "fgtype: %d\n", fgci.byCmd);
	OutputDebugStringA(msg);
#endif

	FGCmdHandlerMap::iterator it = m_HandlerMap.find(fgci.byCmd);
	if (it != m_HandlerMap.end())
	{
		//	Call handler function
		LPFNFUNGAMECMDHANDLER lpfnHandler = it->second;
		return (this->*(lpfnHandler))(fgci);
	}
	else	//	No handler was found
	{
		//	Output a warning ?
		ASSERT(0 && ("Couldn't find fun game command handler."));
		return 0;
	}
}
