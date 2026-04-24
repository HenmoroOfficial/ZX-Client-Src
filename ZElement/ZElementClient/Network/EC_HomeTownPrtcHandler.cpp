/*
 * FILE: EC_HomeTownPrtcHandler.cpp
 *
 * DESCRIPTION: 
 *
 * CREATED BY: lihengju, 2009/8/19
 *
 * HISTORY: 
 *
 * Copyright (c) 2004 Archosaur Studio, All Rights Reserved.
 */

#include "EC_Global.h"
#include "EC_HomeTownPrtcHandler.h"
#include "EC_HTPDataType.h"
#include "gnoctets.h"
#include "hometowndatasend.hpp"

#include <A3DMacros.h>
#include <ALog.h>
#include <AFile.h>

#include "DlgModule.h"
#include "EC_Game.h"
#include "EC_GameRun.h"
#include "EC_UIManager.h"
#include "EC_GameUIMan.h"

#define new A_DEBUG_NEW

///////////////////////////////////////////////////////////////////////////
//	
//	Define and Macro
//	
///////////////////////////////////////////////////////////////////////////

#define FIXHTCMD_SIZE_CHECK(cmd_type, ci) \
	return (sizeof (cmd_type) == ci.iDataSize);

#define VARHTCMD_SIZE_CHECK(cmd_type, ci) \
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

CECHomeTownPrtcHandler::CECHomeTownPrtcHandler()
{
	using namespace HT_S2C;

	m_pHTS2CCmdLog = NULL;
	m_pHTS2CCmdBinLog = NULL;	
	m_HandlerMap[COMPO_LIST] = &CECHomeTownPrtcHandler::OnHTCmd_GetCompoList;
	m_HandlerMap[COMPO_ADD] = &CECHomeTownPrtcHandler::OnHTCmd_AddCompo;
	m_HandlerMap[EXCHG_MONEY] = &CECHomeTownPrtcHandler::OnHTCmd_ExgMoney;
	m_HandlerMap[LEVEL_UP] = &CECHomeTownPrtcHandler::OnHTCmd_LevelUp;
	m_HandlerMap[BUY_ITEM] = &CECHomeTownPrtcHandler::OnHTCmd_BuyItem;
	m_HandlerMap[USE_ITEM] = &CECHomeTownPrtcHandler::OnHTCmd_UseItem;
	m_HandlerMap[SELL_ITEM] = &CECHomeTownPrtcHandler::OnHTCmd_SellItem;
	m_HandlerMap[PACKAGE_LIST] = &CECHomeTownPrtcHandler::OnHTCmd_GetPackage;
	m_HandlerMap[FARM_VIEW] = &CECHomeTownPrtcHandler::OnHTCmd_ViewFarm;
	m_HandlerMap[FARM_VIEW_FIELD] = &CECHomeTownPrtcHandler::OnHTCmd_ViewFarmField;
	m_HandlerMap[FARM_PLOW_FIELD] = &CECHomeTownPrtcHandler::OnHTCmd_PlowField;
	m_HandlerMap[FARM_WATER_FIELD] = &CECHomeTownPrtcHandler::OnHTCmd_WaterField;
	m_HandlerMap[FARM_SOW_FIELD] = &CECHomeTownPrtcHandler::OnHTCmd_SowField;
	m_HandlerMap[FARM_PEST_FIELD] = &CECHomeTownPrtcHandler::OnHTCmd_PestField;
	m_HandlerMap[FARM_WEED_FIELD] = &CECHomeTownPrtcHandler::OnHTCmd_WeedField;
	m_HandlerMap[FARM_HARVEST_FIELD] = &CECHomeTownPrtcHandler::OnHTCmd_HarvestField;
	m_HandlerMap[FARM_STEAL_FIELD] = &CECHomeTownPrtcHandler::OnHTCmd_StealField;
	m_HandlerMap[FARM_LIST_LOVE_FIELD] = &CECHomeTownPrtcHandler::OnHTCmd_ListLoveFields;
	m_HandlerMap[FARM_LIST_MATURE_FIELD] = &CECHomeTownPrtcHandler::OnHTCmd_ListMatureFields;		
}

CECHomeTownPrtcHandler::~CECHomeTownPrtcHandler()
{

}

//	On session opening
bool CECHomeTownPrtcHandler::OnSessionOpen()
{
	m_nHTS2CCmdSize = 0;

	//	Create cmd log
	A3DRELEASE(m_pHTS2CCmdLog);
	m_pHTS2CCmdLog = new ALog;

	//	Determine log file's path
	AString str = "hts2ccmd.log";
	OFSTRUCT st;
	if (OpenFile("Logs\\hts2ccmd.log", &st, OF_EXIST) != HFILE_ERROR)
	{
		//	文件存在
		HFILE file = OpenFile("Logs\\hts2ccmd.log", &st, OF_READ | OF_SHARE_EXCLUSIVE);
		if (file != HFILE_ERROR)
			CloseHandle((HANDLE)file);
		else
			str.Format("hts2ccmd%d.log", time(NULL));
	}

	if (!m_pHTS2CCmdLog->Init(str, "home town server to Client command log!"))
	{
		A3DRELEASE(m_pHTS2CCmdLog)
		return true;
	}

	//	Create command bin log
	if (m_pHTS2CCmdBinLog)
	{
		m_pHTS2CCmdBinLog->Close();
		delete m_pHTS2CCmdBinLog;
		m_pHTS2CCmdBinLog = NULL;
	}

	m_pHTS2CCmdBinLog = new AFile;

	//	Determine log file's path
	str = "logs\\hts2ccmdlog.bin";
	if (OpenFile(str, &st, OF_EXIST) != HFILE_ERROR)
	{
		//文件存在
		HFILE file = OpenFile(str, &st, OF_READ | OF_SHARE_EXCLUSIVE);
		if (file!= HFILE_ERROR)
			CloseHandle((HANDLE)file);
		else
			str.Format("logs\\hts2ccmdlog%d.bin", time(NULL));
	}

	if (!m_pHTS2CCmdBinLog->Open(str, AFILE_CREATENEW | AFILE_BINARY | AFILE_NOHEAD))
	{
		m_pHTS2CCmdBinLog->Close();
		delete m_pHTS2CCmdBinLog;
		m_pHTS2CCmdBinLog = NULL;
		return true;
	}

	return true;
}

//	On session closing
bool CECHomeTownPrtcHandler::OnSessionClose()
{
	A3DRELEASE(m_pHTS2CCmdLog);

	if (m_pHTS2CCmdBinLog)
	{
		m_pHTS2CCmdBinLog->Close();
		delete m_pHTS2CCmdBinLog;
		m_pHTS2CCmdBinLog = NULL;
	}

	return true;
}

//	Log Fun game S2C command data
void CECHomeTownPrtcHandler::LogHTS2CCmdData(BYTE byCmd, BYTE byGameType, const BYTE* pDataBuf, int nSize)
{
	return; 

	using namespace HT_S2C;

	m_nHTS2CCmdSize += nSize;

	if (m_pHTS2CCmdLog)
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

		m_pHTS2CCmdLog->Log("total: %d, size: %d, htretcode: %d, htcmd: %d, data: %s", m_nHTS2CCmdSize, nSize, byCmd, byGameType, szStr);

		delete [] szStr;
	}

	// output to binary log
	if (m_pHTS2CCmdBinLog) 
	{
		DWORD dwWriteLen;
		m_pHTS2CCmdBinLog->Write(&nSize, sizeof(nSize), &dwWriteLen);
		SYSTEMTIME sysTime;
		GetLocalTime(&sysTime);
		m_pHTS2CCmdBinLog->Write(&sysTime, sizeof(sysTime), &dwWriteLen);
		m_pHTS2CCmdBinLog->Write((void*)pDataBuf, nSize, &dwWriteLen);
		m_pHTS2CCmdBinLog->Flush();
	}
}

//	Check data size
bool CECHomeTownPrtcHandler::CheckDataSize(HTCMDINFO& htci)
{
	using namespace HT_S2C;
	using namespace HT_S2C::CMD;

	switch (htci.cmd_type)
	{
	case COMPO_LIST:
		return true;
		break;
	case COMPO_ADD:
		return true;
		break;
	case EXCHG_MONEY:
		FIXHTCMD_SIZE_CHECK(exchg_money, htci);
		break;
	case LEVEL_UP:
		return true;
		break;
	case BUY_ITEM:
		FIXHTCMD_SIZE_CHECK(buy_item, htci);
		break;
	case USE_ITEM:
		FIXHTCMD_SIZE_CHECK(use_item, htci);
		break;
	case SELL_ITEM:
		FIXHTCMD_SIZE_CHECK(sell_item, htci);
		break;
	case PACKAGE_LIST:
		return true;
		break;
	case FARM_VIEW:
		return true;
		break;
	case FARM_VIEW_FIELD:
		return true;
		break;
	case FARM_PLOW_FIELD:
		FIXHTCMD_SIZE_CHECK(farm_plow_field, htci);
		break;
	case FARM_SOW_FIELD:
		FIXHTCMD_SIZE_CHECK(farm_sow_field, htci);
		break;
	case FARM_WATER_FIELD:
		FIXHTCMD_SIZE_CHECK(farm_water_field, htci);
		break;
	case FARM_PEST_FIELD:
		FIXHTCMD_SIZE_CHECK(farm_pest_field, htci);
		break;
	case FARM_WEED_FIELD:
		FIXHTCMD_SIZE_CHECK(farm_weed_field,htci);
		break;
	case FARM_HARVEST_FIELD:
		FIXHTCMD_SIZE_CHECK(farm_harvest_field, htci);
		break;
	case FARM_STEAL_FIELD:
		FIXHTCMD_SIZE_CHECK(farm_steal_field, htci);
		break;
	case FARM_LIST_LOVE_FIELD:
		return true;
		break;
	case FARM_LIST_MATURE_FIELD:
		return true;
		break;
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
int CECHomeTownPrtcHandler::Process(GNET::Protocol* pProtocol)
{
	using namespace GNET;
	using namespace HT_S2C;

	HTCMDINFO htci;

	GNET::Octets& Data = static_cast<HometowndataSend*>(pProtocol)->data;
	htci.retCode = static_cast<hometown_s2c_cmd_header*>(Data.begin())->retcode;
	htci.cmd_type = static_cast<hometown_s2c_cmd_header*>(Data.begin())->cmd_type;
	htci.compo_id = static_cast<hometown_s2c_cmd_header*>(Data.begin())->compo_id;
	htci.pData = (BYTE*)Data.begin() + sizeof (hometown_s2c_cmd_header);
	htci.iDataSize = Data.size() - sizeof (hometown_s2c_cmd_header);

	CECGameUIMan *pGameUI = g_pGame->GetGameRun()->GetUIManager()->GetInGameUIMan();
	if (htci.retCode!=0 )
	{
		if (htci.retCode==9)
			pGameUI->MessageBox("",pGameUI->GetStringFromTable(10159+rand()%6), MB_OK, A3DCOLORRGBA(255, 255, 255, 160));
		else if (htci.retCode==10)
			pGameUI->AddChatMessage(pGameUI->GetStringFromTable(9060+htci.cmd_type-FARM_PLOW_FIELD), GP_CHAT_LOCAL);
		else if (htci.retCode==12)
		{
			if (pGameUI->GetDialog("Win_Garden_Friendlist")->IsShow())
				pGameUI->AddChatMessage(pGameUI->GetStringFromTable(8670), GP_CHAT_LOCAL);
			else if (((CDlgModule*)pGameUI->GetDialog("Win_Option"))->GetModuleNum()>0 || ((CDlgModule*)pGameUI->GetDialog("Win_Option"))->IsHost())
				pGameUI->AddChatMessage(pGameUI->GetStringFromTable(8673), GP_CHAT_LOCAL);
			else
				pGameUI->AddChatMessage(pGameUI->GetStringFromTable(8670), GP_CHAT_LOCAL);
		}
		else if (htci.retCode==38 && htci.cmd_type==EXCHG_MONEY)
		{
			pGameUI->AddChatMessage(pGameUI->GetStringFromTable(9104), GP_CHAT_LOCAL);
		}
		else
			pGameUI->AddChatMessage(pGameUI->GetStringFromTable(8700+htci.retCode), GP_CHAT_LOCAL);
		return 0;
	}
	if (htci.cmd_type==COMPO_ADD)
	{
		((CDlgModule*)pGameUI->GetDialog("Win_Option"))->OnPrtc_AddModule(htci.compo_id);
	}
#ifdef _DEBUG

	if (!CheckDataSize(htci))
	{
		ASSERT(0);
		return 0;
	}

#endif

	LogHTS2CCmdData(htci.retCode, htci.cmd_type, htci.pData, htci.iDataSize);
	

	HTCmdHandlerMap::iterator it = m_HandlerMap.find(htci.cmd_type);
	if (it != m_HandlerMap.end())
	{
		//	Call handler function
		LPFNHOMETOWNCMDHANDLER lpfnHandler = it->second;
		return (this->*(lpfnHandler))(htci);
	}
	else	//	No handler was found
	{
		//	Output a warning ?
		ASSERT(0 && ("Couldn't find fun game command handler."));
		return 0;
	}
}
