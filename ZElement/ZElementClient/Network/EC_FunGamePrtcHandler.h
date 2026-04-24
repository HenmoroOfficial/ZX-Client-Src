/*
 * FILE: EC_GamePrtcHandler.h
 *
 * DESCRIPTION: 
 *
 * CREATED BY: Wangrui, 2009/3/11
 *
 * HISTORY: 
 *
 * Copyright (c) 2004 Archosaur Studio, All Rights Reserved.
 */

#pragma once

#include "hashmap.h"

///////////////////////////////////////////////////////////////////////////
//	
//	Define and Macro
//	
///////////////////////////////////////////////////////////////////////////


///////////////////////////////////////////////////////////////////////////
//	
//	Types and Global variables
//	
///////////////////////////////////////////////////////////////////////////

namespace GNET
{
	class Protocol;
}

class ALog;
class AFile;

///////////////////////////////////////////////////////////////////////////
//	
//	Declare of Global functions
//	
///////////////////////////////////////////////////////////////////////////


///////////////////////////////////////////////////////////////////////////
//	
//	Class CECFunGamePrtcHandler
//	
///////////////////////////////////////////////////////////////////////////

class CECFunGamePrtcHandler
{
public:		//	Types

	struct FGCMDINFO
	{
		BYTE	byCmd;		//	Fun game command header
		BYTE	byGameType;	//  Fun game type
		BYTE*	pData;		//	Fun game command data
		int		iDataSize;	//	command data size
	};

	typedef int (CECFunGamePrtcHandler::*LPFNFUNGAMECMDHANDLER)(const FGCMDINFO& ci);
	typedef abase::hash_map<int, LPFNFUNGAMECMDHANDLER> FGCmdHandlerMap;

public:		//	Constructor and Destructor

	CECFunGamePrtcHandler();
	virtual ~CECFunGamePrtcHandler();

public:		//	Attributes

public:		//	Operations

	//	On session opening
	bool OnSessionOpen();
	//	On session closing
	bool OnSessionClose();

	/*	Process protocol.

		Return value:			
			0: process fail.
			1: process successfully
			2: protocol processing should be postponed
			3: process finished but don't remove this protocol
	*/
	int Process(GNET::Protocol* pProtocol);

protected:	//	Attributes

	FGCmdHandlerMap	m_HandlerMap;

	int		m_nFGS2CCmdSize;		//	total server to client command size
	ALog*	m_pFGS2CCmdLog;			//	Command log file
	AFile*	m_pFGS2CCmdBinLog;		//	Command log in binary format
	
public:	//	Operations

	//	Log Fun game S2C command data
	void LogFGS2CCmdData(BYTE byCmd, BYTE byGameType, const BYTE* pDataBuf, int nSize);
	//	Check data size
	bool CheckDataSize(FGCMDINFO& fgci);

	//	Fun game command handler functions
	int OnFGCmd_GetRoomList(const FGCMDINFO& fgci);
	int OnFGCmd_NotifyRoomInfo(const FGCMDINFO& fgci);
	int OnFGCmd_PlayerInfo(const FGCMDINFO& fgci);
	int OnFGCmd_PlayerEnterRoom(const FGCMDINFO& fgci);
	int OnFGCmd_PlayerLeaveRoom(const FGCMDINFO& fgci);
	int OnFGCmd_PlayerStateChange(const FGCMDINFO& fgci);
	int OnFGCmd_PlayerReenterInvite(const FGCMDINFO& fgci);
	int OnFGCmd_RoomGameIsReady(const FGCMDINFO& fgci);
	int OnFGCmd_RoomDispatchNormalCard(const FGCMDINFO& fgci);
	int OnFGCmd_PlayerInturnNormalCard(const FGCMDINFO& fgci);
	int OnFGCmd_PlayerThrowNormalCard(const FGCMDINFO& fgci);
	int OnFGCmd_PlayerPickupNormalCard(const FGCMDINFO& fgci);
	int OnFGCmd_PlayerGiveupNormalCard(const FGCMDINFO& fgci);
	int OnFGCmd_PlayerAuction(const FGCMDINFO& fgci);
	int OnFGCmd_RoomGameEnd(const FGCMDINFO& fgci);
	int OnFGCmd_LandlordResultBroadcast(const FGCMDINFO& fgci);
	int OnFGCmd_LandlordReonlineDataSend(const FGCMDINFO& fgci);
	int OnFGCmd_PlayerEnterHall(const FGCMDINFO& fgci);
	int OnFGCmd_PlayerLeaveHall(const FGCMDINFO& fgci);
	int OnFGCmd_NotifyHallMembers(const FGCMDINFO& fgci);
};