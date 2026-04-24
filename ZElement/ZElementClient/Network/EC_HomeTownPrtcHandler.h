/*
 * FILE: EC_HomeTownPrtcHandler.h
 *
 * DESCRIPTION: 
 *
 * CREATED BY: lihengju, 2009/8/19
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
//	Class CECHomeTownPrtcHandler
//	
///////////////////////////////////////////////////////////////////////////

class CECHomeTownPrtcHandler
{
public:		//	Types

	struct HTCMDINFO
	{
		int 	retCode;
		int 	cmd_type;	//  home town command type
		int 	compo_id;	//	home town component id
		BYTE*	pData;		//	home town command data
		int		iDataSize;	//	command data size
	};

	typedef int (CECHomeTownPrtcHandler::*LPFNHOMETOWNCMDHANDLER)(const HTCMDINFO& ci);
	typedef abase::hash_map<int, LPFNHOMETOWNCMDHANDLER> HTCmdHandlerMap;

public:		//	Constructor and Destructor

	CECHomeTownPrtcHandler();
	virtual ~CECHomeTownPrtcHandler();

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

	HTCmdHandlerMap	m_HandlerMap;

	int		m_nHTS2CCmdSize;		//	total server to client command size
	ALog*	m_pHTS2CCmdLog;			//	Command log file
	AFile*	m_pHTS2CCmdBinLog;		//	Command log in binary format
	
public:	//	Operations

	//	Log Fun game S2C command data
	void LogHTS2CCmdData(BYTE byCmd, BYTE byGameType, const BYTE* pDataBuf, int nSize);
	//	Check data size
	bool CheckDataSize(HTCMDINFO& htci);

	//	Fun game command handler functions
	int OnHTCmd_GetCompoList(const HTCMDINFO& htci);
	int OnHTCmd_AddCompo(const HTCMDINFO& htci);
	int OnHTCmd_ExgMoney(const HTCMDINFO& htci);
	int OnHTCmd_LevelUp(const HTCMDINFO& htci);
	int OnHTCmd_BuyItem(const HTCMDINFO& htci);
	int OnHTCmd_UseItem(const HTCMDINFO& htci);
	int OnHTCmd_SellItem(const HTCMDINFO& htci);
	int OnHTCmd_GetPackage(const HTCMDINFO& htci);
	int OnHTCmd_ViewFarm(const HTCMDINFO& htci);
	int OnHTCmd_ViewFarmField(const HTCMDINFO& htci);
	int OnHTCmd_SowField(const HTCMDINFO& htci);
	int OnHTCmd_PestField(const HTCMDINFO& htci);
	int OnHTCmd_WaterField(const HTCMDINFO& htci);
	int OnHTCmd_PlowField(const HTCMDINFO& htci);
	int OnHTCmd_WeedField(const HTCMDINFO& htci);
	int OnHTCmd_HarvestField(const HTCMDINFO& htci);
	int OnHTCmd_StealField(const HTCMDINFO& htci);
	int OnHTCmd_ListLoveFields(const HTCMDINFO& htci);
	int OnHTCmd_ListMatureFields(const HTCMDINFO& htci);
};