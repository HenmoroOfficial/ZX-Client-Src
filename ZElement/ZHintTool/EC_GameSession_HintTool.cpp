/*
 * FILE: EC_GameSession.cpp
 *
 * DESCRIPTION: 
 *
 * CREATED BY: Duyuxin, 2004/9/8
 *
 * HISTORY: 
 *
 * Copyright (c) 2004 Archosaur Studio, All Rights Reserved.
 */

#include "EC_Global.h"
#include "gnet.h"
#include "gameclient.h"
//#include "pingclient.h"
#include "EC_GameSession_HintTool.h"
#include "EC_Game_HintTool.h"
#include "EC_GameRun_HintTool.h"
#include "EC_GPDataType.h"
#include "EC_RTDebug.h"
/*#include "EC_LoginUIMan.h"
#include "EC_UIManager.h"
#include "EC_World.h"
#include "EC_HostPlayer.h"
#include "EC_NPC.h"
#include "EC_MsgDef.h"
#include "EC_ManPlayer.h"
#include "EC_ManNPC.h"
#include "EC_Configs.h"
#include "EC_FixedMsg.h"
#include "EC_GameUIMan.h"
#include "EC_Team.h"
*/#include "EC_IvtrTypes.h"
#include "EC_DealInventory.h"
#include "EC_IvtrItem.h"
/*#include "EL_Precinct.h"
#include "EC_Faction.h"
#include "AUI\\AUICTranslate.h"
#include "DlgMailWrite.h"
#include "DlgGuildMap.h"
#include "DlgGoldTrade.h"
#include "EC_NetProtocolBase.h"
#include "EC_PrtcPBIProc.h"
#include "TaskTemplMan.h"
#include "EC_Friend.h"
#include "EC_FunGamePrtcHandler.h"
#include "EC_HomeTownPrtcHandler.h"
#include "EC_Circle.h"
#include "EC_Cutscene.h"

#include "cashlock.hpp"
#include "cashpasswordset.hpp"
#include "cashlock_re.hpp"
#include "cashpasswordset_re.hpp"
#include "factionchat.hpp"
#include "factionresign_re.hpp"
#include "factionnickname_re.hpp"
#include "factionleave_re.hpp"
#include "factionexpel_re.hpp"
#include "factionappoint_re.hpp"
#include "factionrecruit_re.hpp"
#include "acanswer.hpp"

#include "getfactionbaseinfo.hpp"
#include "gethelpstates.hpp"
#include "gethelpstates_re.hpp"
#include "sethelpstates.hpp"
#include "sethelpstates_re.hpp"
#include "announceforbidinfo.hpp"
#include "worldchat.hpp"
#include "fungamedatasend.hpp"
#include "hometowndatasend.hpp"

#include "getcirclebaseinfo_re.hpp"
#include "circlelist_re.hpp"
#include "circlenotify.hpp"
#include "gcirclechat.hpp"

#include "forbid.hxx"
#include "ids.hxx"

#include "AC.h"
#include "acremotecode.hpp"
#include "acreport.hpp"
#include "updateremaintime.hpp"
#include "defence/defence.h"
#include "defence/stackchecker.h"
#include "globaldataman.h"
#include "EC_GameRecord.h"
#include "LuaEvent.h"
#include "EC_Secretary.h"

extern game_record gr;
*/

//=============================================================================
#include "GetItemDesc.hpp"
#include "ToolAnnounceAid.hpp"
#include "GetItemDescResponse.hpp"
#include "NewKeepAlive.hpp"
#include "gnmarshal.h"
#include "GetItemDescForPlatform.hpp" //Added 2011-11-09.
#include "GetItemDescForPlatformRe.hpp"//Added 2011-11-09.
#include "GetRoleEquip.hpp" //Added 2011-11-23.
#include "GetSNSRolePet.hpp" //Added 2011-11-23.
#include "GetRoleEquipRe.hpp" //Added 2011-11-23.
#include "GetSNSRolePetRe.hpp" //Added 2011-11-23.
#include "GetRoleDesc.hpp"	//Added 2012-02-09.
#include "GetRoleDescSnS.hpp"
#include "GetRoleDescSnSRe.hpp"
#include "AXMLFile.h"	//Added 2012-02-09.
#include "ConfigFromLua.h"	//Added 2012-03-06.
#include "AUI\\AUICTranslate.h" //Added 2012-03-06.
#include "GetRoleDescRe.hpp"	//Added 2012-03-09.
#include "gconsignrole"
#include "elementdataman.h"		//Added 2012-03-12.
#include "EC_Skill.h"			//Added 2012-03-15.
#include "EC_IvtrConsume.h"		//Added 2012-03-19.
#include "EC_FixedMsg.h"		//Added 2012-03-19.
#include "EC_RoleDescXMLParser.h"
#include "gsnsrole"

//=============================================================================
#define new A_DEBUG_NEW

///////////////////////////////////////////////////////////////////////////
//	
//	Define and Macro
//	
///////////////////////////////////////////////////////////////////////////

//	Base index of link error message
#define BASEIDX_LINKSEVERROR	1150

///////////////////////////////////////////////////////////////////////////
//	
//	Reference to External variables and functions
//	
///////////////////////////////////////////////////////////////////////////

/*extern abase::vector<ACRemoteCode*> g_DefenceCodeVec;
extern CRITICAL_SECTION g_csDefence;

///////////////////////////////////////////////////////////////////////////
//	
//	Local Types and Variables and Global variables
//	
///////////////////////////////////////////////////////////////////////////
*/
static bool l_bExitConnect = false;
/*
///////////////////////////////////////////////////////////////////////////
//	
//	Local functions
//	
///////////////////////////////////////////////////////////////////////////

void OnTaskChatMessage(const void* pBuf, size_t sz)
{
	struct _data
	{
		int self_id;	//完成任务的玩家的角色ID
		int task_id;	//任务ID
		int channel;	//任务里选择的频道 ，目前只有 普通、交易、世界、组队有效
		int param;		//任务系统传过来的参数
		int line_id;
		char name[20];	//最大长度为20字节的名字
	};

	if (sz >= sizeof(_data))
	{
		_data* pData = (_data*)pBuf;
		CECGameUIMan* pGameUI = (CECGameUIMan*)g_pGame->GetGameRun()->GetUIManager()->GetInGameUIMan();
		ATaskTempl* pTempl = g_pGame->GetTaskTemplateMan()->GetTaskTemplByID(pData->task_id);

		if (pTempl && pTempl->GetTribute() )
		{
			wchar_t szName[11];
			szName[10] = 0;
			wcsncpy(szName, (wchar_t*)pData->name, 10);
			ACString strMsg = pTempl->GetTribute();
			int nSub = strMsg.Find(_AL("$name"));
			while (nSub != -1)
			{
				strMsg = strMsg.Left(nSub) + szName + strMsg.Right(strMsg.GetLength() - nSub - 5);
				nSub = strMsg.Find(_AL("$name"));
			}

			ACString strLine = pGameUI->GetLine(pData->line_id);
			nSub = strMsg.Find(_AL("$line"));
			while (nSub != -1)
			{
				strMsg = strMsg.Left(nSub) + strLine + strMsg.Right(strMsg.GetLength() - nSub - 5);
				nSub = strMsg.Find(_AL("$line"));
			}

*///			g_pGame->GetGameRun()->AddChatMessage(strMsg, GP_CHAT_SYSTEM/*pData->channel*/); // 策划需求任务不分频道2010.6.25 fuchonggang
/*		}
	}
}

///////////////////////////////////////////////////////////////////////////
//	
//	Implement CECGameSession
//	
///////////////////////////////////////////////////////////////////////////
*/
CECGameSession::CECGameSession() :
m_aNewProtocols(128, 128)/*,
m_aOldProtocols(128, 128),
m_Ping(50, 50)*/
{
	if (!(m_pNetMan = &GNET::GameClient::GetInstance()))
	{
		ASSERT(m_pNetMan);
		a_LogOutput(1, "CECGameSession::Open, Failed to create net manager");
	}

	m_pNetMan->SetZoneID(0);
	m_pNetMan->Attach(IOCallBack);

/*	GNET::PingClient::GetInstance().Attach(IOCallBack);

*/	m_idLink			= (DWORD)(-1);
/*	m_iUserID			= 0;
	m_iCharID			= 0;
*/	m_bLinkBroken		= false;
	m_bConnected		= false;
/*	m_pS2CCmdLog		= NULL;
	m_pS2CCmdBinLog		= NULL;
*/	m_hConnectThread	= NULL;
/*	m_bKickUser			= true;
	m_iiAccountTime		= 0;
	m_iiFreeGameTime	= 0;
	m_iFreeGameEnd		= 0;
	m_dwLastAccTime		= 0;
*/	m_bProcNewPtrc		= false;
/*
	m_pPBIProc	= new CECPrtcPBIProc;

	m_pfnHelpStatesCallBack = NULL;
*/	memset(&m_ot, 0, sizeof (m_ot));
/*	m_iDistrictID = 0;
	m_iReferrer	= 0;
	m_zoneID	= 0;
	m_pFunGamePrtcHandler = new CECFunGamePrtcHandler;
	m_pHomeTownPrtcHandler = new CECHomeTownPrtcHandler;
	InitializeCriticalSection(&m_csPingSection);*/
}

CECGameSession::~CECGameSession()
{
/*	GNET::PingClient::GetInstance().Detach();

	if (m_pPBIProc)
		delete m_pPBIProc;
*/
	if( m_pNetMan )
	{
		m_pNetMan->Detach();
		m_pNetMan = NULL;
	}
/*
	for (int i = 0; i < m_Ping.GetSize(); i++)
		delete m_Ping[i];

	m_ErrorMsgs.Release();
	delete m_pFunGamePrtcHandler;
	delete m_pHomeTownPrtcHandler;
	DeleteCriticalSection(&m_csPingSection);*/
}

//	Open session
bool CECGameSession::Open()
{
	//	Connect server
	Connect();
/*
	//	Load server error strings form file
	m_ErrorMsgs.Init("Configs\\server_error.txt", true);

	// create cmd log
	if(m_pS2CCmdLog) A3DRELEASE(m_pS2CCmdLog);
	m_pS2CCmdLog = new ALog;

	//determine log file's path
	AString str = "s2ccmd.log";
	OFSTRUCT st;
	if (OpenFile("Logs\\s2ccmd.log",&st,OF_EXIST) != HFILE_ERROR)
	{
		//文件存在
		HFILE file = OpenFile("Logs\\s2ccmd.log",&st,OF_READ|OF_SHARE_EXCLUSIVE);
		if(file!= HFILE_ERROR)
		{
			CloseHandle((HANDLE)file);
		}
		else
		{
			str.Format("s2ccmd%d.log",time(NULL));			
		}
	}

	if (!m_pS2CCmdLog->Init(str, "Server to Client command log!"))
	{
		A3DRELEASE(m_pS2CCmdLog)
		return true;
	}

	// create command bin log
	if(m_pS2CCmdBinLog) 
	{
		m_pS2CCmdBinLog->Close();
		delete m_pS2CCmdBinLog;
		m_pS2CCmdBinLog = NULL;
	}
	m_pS2CCmdBinLog = new AFile;

	//determine log file's path
	str = "logs\\s2ccmdlog.bin";
	if (OpenFile(str,&st,OF_EXIST) != HFILE_ERROR)
	{
		//文件存在
		HFILE file = OpenFile(str,&st,OF_READ|OF_SHARE_EXCLUSIVE);
		if(file!= HFILE_ERROR)
		{
			CloseHandle((HANDLE)file);
		}
		else
		{
			str.Format("logs\\s2ccmdlog%d.bin",time(NULL));
		}
	}

	if (!m_pS2CCmdBinLog->Open(str, AFILE_CREATENEW | AFILE_BINARY | AFILE_NOHEAD))
	{
		m_pS2CCmdBinLog->Close();
		delete m_pS2CCmdBinLog;
		m_pS2CCmdBinLog = NULL;
		return true;
	}
	m_pFunGamePrtcHandler->OnSessionOpen();
	m_pHomeTownPrtcHandler->OnSessionOpen();
	m_nS2CCmdSize = 0;*/
	return true;
}

//	Close sesssion
void CECGameSession::Close()
{
/*	m_pfnHelpStatesCallBack = NULL;

*/if (m_hConnectThread)
	{
		TerminateThread(m_hConnectThread, 0);
		CloseHandle(m_hConnectThread);
		m_hConnectThread = NULL;
	}

	//	Close net manager
	if (m_pNetMan)
	{
		m_pNetMan->Disconnect();
		m_idLink = 0;
		PollIO::WakeUp();
	}
/*
	if (m_pS2CCmdBinLog)
	{
		m_pS2CCmdBinLog->Close();
		delete m_pS2CCmdBinLog;
		m_pS2CCmdBinLog = NULL;
	}

	if (m_pS2CCmdLog)
	{
		m_pS2CCmdLog->Release();
		delete m_pS2CCmdLog;
		m_pS2CCmdLog = NULL;
	}

	//	Clear command cache
	m_CmdCache.RemoveAllCmds();

	if (m_pPBIProc)
		m_pPBIProc->Reset();
*/
	int i;

	EnterCriticalSection(&g_csSession);

	//	Release all unprocessed protocols
	for (i=0; i < m_aNewProtocols.GetSize(); i++)
	{
		GNET::Protocol* p = m_aNewProtocols[i];
		p->Destroy();
	}

	m_aNewProtocols.RemoveAll();

	LeaveCriticalSection(&g_csSession);
/*
	//	Release all processed protocols
	ClearOldProtocols();

	::EnterCriticalSection(&g_csDefence);
	for (size_t j = 0; j < g_DefenceCodeVec.size(); j++)
		g_DefenceCodeVec[j]->Destroy();
	g_DefenceCodeVec.clear();
	::LeaveCriticalSection(&g_csDefence);
*/
	m_bConnected		= false;
/*	m_iiAccountTime		= 0;
	m_iiFreeGameTime	= 0;
	m_iFreeGameEnd		= 0;
	m_pFunGamePrtcHandler->OnSessionClose();
	m_pHomeTownPrtcHandler->OnSessionClose();*/
}

DWORD WINAPI CECGameSession::ConnectThread(LPVOID pArg)
{
	CECGameSession* pGameSession = (CECGameSession*)pArg;

	const CECGame::GAMEINIT& gi = g_pGame->GetGameInit();
	if (gi.szIP[0] && gi.iPort)
	{
		if (pGameSession->GetNetManager()->Connect(gi.szIP, (WORD)gi.iPort) == -1)
		{
			AString strAddr = g_pGame->GetIPAddressByName(gi.szIP);

			if (!strAddr.IsEmpty())
				pGameSession->GetNetManager()->Connect(strAddr, (WORD)gi.iPort);
		}
	}
	else
		pGameSession->GetNetManager()->Connect(NULL, 0);

	l_bExitConnect = false;

	//	Start overtime check
	DWORD dwTotal, dwCount;
	if( !pGameSession->GetOvertimeCnt(dwTotal, dwCount) )
	{
		// we only activate the overtime check when the timer has not been activated
		pGameSession->DoOvertimeCheck(true, CECGameSession::OT_CHALLENGE, 40000);
	}

	//	Wait until connnect is established
	while (!pGameSession->IsConnected() && !l_bExitConnect)
	{
		Sleep(100);
	}

	return 0;
}

//	Connect server
bool CECGameSession::Connect()
{
	if (!m_pNetMan)
	{
		ASSERT(m_pNetMan);
		return false;
	}

	if (m_hConnectThread)
	{
		TerminateThread(m_hConnectThread, 0);
		CloseHandle(m_hConnectThread);
		m_hConnectThread = NULL;
	}
	
	// we use a thread to connect with the server, this can avoid the main thread blocked by 
	// firewalls when calling socket's connect
	DWORD dwThreadID;
	m_hConnectThread = CreateThread(NULL, 0, ConnectThread, this, 0, &dwThreadID);

	return true;
}

//	Send net data
bool CECGameSession::SendNetData(const GNET::Protocol& p, bool bUrg)
{
/*	StackChecker::ACTrace(8);

	if (gr.get_state() == game_record::enum_gr_replay)
		return true;
*/
	if (!m_pNetMan || m_idLink == (DWORD)(-1))
	{
		ASSERT(m_pNetMan && m_idLink != (DWORD)(-1));
		return false;
	}
	
	return m_pNetMan->Send(m_idLink, p, bUrg);
}
/*
//	Update
bool CECGameSession::Update(DWORD dwDeltaTime)
{
	//	Check overtime
	if (m_ot.bCheck)
	{
		if ((m_ot.dwTimeCnt += dwDeltaTime) >= m_ot.dwTime)
		{
			m_ot.bCheck = false;
			OnOvertimeHappen();
		}
	}

	m_CmdCache.Tick(dwDeltaTime);

	if (m_bLinkBroken)
		LinkBroken(true);

	//	Update left account game time
	if (m_iFreeGameEnd == -1)
	{
		// 当前是免费状态，不用计费
	}
	else if (m_iiFreeGameTime)
	{
		DWORD dwCurTime = a_GetTime();
		DWORD dwDeltaTime = dwCurTime - m_dwLastAccTime;
		if ((m_iiFreeGameTime -= dwDeltaTime) < 0)
			m_iiFreeGameTime = 0;
		
		m_dwLastAccTime = dwCurTime;
	}
	else if (m_iiAccountTime)
	{
		DWORD dwCurTime = a_GetTime();
		DWORD dwDeltaTime = dwCurTime - m_dwLastAccTime;
		if ((m_iiAccountTime -= dwDeltaTime) < 0)
			m_iiAccountTime = 0;

		m_dwLastAccTime = dwCurTime;
	}

	return true;
}

//	Net pulse
void CECGameSession::NetPulse()
{
	GNET::Timer::Update();
}
*/
//	Start / End overtime
void CECGameSession::DoOvertimeCheck(bool bStart, int iCheckID, DWORD dwTime)
{
/*	if (gr.get_state() == game_record::enum_gr_replay)
		return;
*/
	if (bStart)
	{
		m_ot.bCheck		= bStart;
		m_ot.iCheckID	= iCheckID;
		m_ot.dwTime		= dwTime;
		m_ot.dwTimeCnt	= 0;
	}
	else
	{
		ASSERT(m_ot.iCheckID == iCheckID);
		m_ot.bCheck	= false;
	}
}
/*
//	On overtime happens
void CECGameSession::OnOvertimeHappen()
{
	switch (m_ot.iCheckID)
	{
	case OT_CHALLENGE:

		l_bExitConnect = true;
		LinkBroken(false);
		break;

	case OT_ENTERGAME:

		LinkBroken(true);
		break;
	}
}
*/
//	Get overtime counter
bool CECGameSession::GetOvertimeCnt(DWORD& dwTotalTime, DWORD& dwCnt)
{
	if (m_ot.bCheck)
	{
		dwTotalTime = m_ot.dwTime;
		dwCnt = m_ot.dwTimeCnt;
		return true;
	}
	else
		return false;
}
/*
//	Freeze host's inventory item
void CECGameSession::FreezeHostItem(int iIvtr, int iIndex, bool bFreeze)
{
	CECHostPlayer* pHost = g_pGame->GetGameRun()->GetHostPlayer();
	if (pHost)
		pHost->FreezeItem(iIvtr, iIndex, bFreeze, true);
}

void CECGameSession::server_Ping(int idServer, in_addr * addr, int nPort)
{
	EnterCriticalSection(&m_csPingSection);
	if( m_Ping.GetSize() > idServer )
	{
		if( m_Ping[idServer]->idLink == (DWORD)-1 || GetTickCount() - m_Ping[idServer]->dwLastPingTime > 600000 )
		{
			m_Ping[idServer]->idLink = GNET::PingClient::GetInstance().Ping(addr, nPort);
			m_Ping[idServer]->dwLastPingTime = GetTickCount();
			m_Ping[idServer]->dwTime = 99999999;
			m_Ping[idServer]->dwStatus = 0;
		}
		LeaveCriticalSection(&m_csPingSection);
		return;
	}
	int i;
	for( i = m_Ping.GetSize(); i < idServer; i++ )
	{
		PINGTIME* pt = new PINGTIME;
		pt->idLink = (DWORD)-1;
		pt->dwTime = 99999999;
		pt->dwLastPingTime = 0;
		pt->dwStatus = 0;
		m_Ping.Add(pt);
	}
	PINGTIME* pt = new PINGTIME;
	pt->idLink = GNET::PingClient::GetInstance().Ping(addr, nPort);
	pt->dwTime = 99999999;
	pt->dwLastPingTime = GetTickCount();
	pt->dwStatus = 0;
	m_Ping.Add(pt);
	LeaveCriticalSection(&m_csPingSection);
}
*/
int CECGameSession::IOCallBack(void* pData, unsigned int idLink, int iEvent)
{
	using namespace GNET;

	CECGameSession* pSession = g_pGame->GetGameSession();
	if (!pSession)
		return 0;

	GNET::GameClient* pNetMan = pSession->GetNetManager();
	if (!pNetMan)
	{
		OutputDebugString(_AL("CECGameSession::IOCallBack, callback when pNetMan=NULL !"));
		return 0;
	}

	switch (iEvent)
	{
	case EVENT_ADDSESSION:
	{
		g_pGame->RuntimeDebugInfo(RTDCOL_NETWORK, _AL("NET - Connection established"));
		
		//test.
		a_LogOutput( 1, "Connection established! linkid = %d\n", idLink );
		pNetMan->ChangeState(idLink, &state_ElementHint);
		//test end.
		pSession->SetLinkID(idLink);
		pSession->m_bConnected	= true;
		pSession->m_bLinkBroken	= false;
		const CECGame::GAMEINIT& gi = g_pGame->GetGameInit();
		g_pGame->AddIPAddressByName(gi.szIP, pNetMan->GetPeer().c_str());
		if(g_pGame->GetToolType()==CECGame::TT_ZHUTI)
			pSession->SendAnnounceAid();
		return 0;
	}
	case EVENT_DELSESSION:
	{
		//	If we are in game state and were disconnected passively, 
		//	notify player and exit game
		if (pSession->m_bConnected)
			pSession->m_bLinkBroken = true;
	
		pSession->m_bConnected = false;
		g_pGame->RuntimeDebugInfo(RTDCOL_WARNING, _AL("NET - Server disconnected"));

		//test.
		a_LogOutput( 2, "Server disconnected\n" );

		//Re-Connect.
//		pSession->Close();
//		pSession->Open();
		//test end.

		return 0;
	}
	case EVENT_ABORTSESSION:
	{	
		// connect to server failed, so pop up a message window
		// make overtime check fire.
		pSession->DoOvertimeCheck(true, CECGameSession::OT_CHALLENGE, 0);
		
		pSession->m_bConnected = false;
		g_pGame->RuntimeDebugInfo(RTDCOL_NETWORK, _AL("NET - Connection Failed !"));

		//test.
		a_LogOutput( 1, "Can not connect to the server!\n" );
		//Re-Connect.
		Sleep(60000);//sleep 60 second and then re-connect.
		pSession->Close();
		pSession->Open();
		//test end.

		return 0;
	}
	case EVENT_PINGRETURN:
	{
/*		EnterCriticalSection(&pSession->m_csPingSection);
		int i;
		for( i = 0; i < pSession->m_Ping.GetSize(); i++ )
			if( pSession->m_Ping[i]->idLink == idLink )
			{
				ServerStatus* status = (ServerStatus*)pData;
				double newping = (double)status->ping;
				if( status->attr.load >= 200 )
					status->attr.load = 199;
				if( status->attr.lambda != 0 )
					newping = status->ping / ( 1 - exp( - status->attr.lambda * ( 1 - status->attr.load / 200.0) ) );
				if( status->attr.anything != 0 )
					newping *= 0.4 + status->attr.anything * 0.1;
				a_Clamp(newping, 5.0 , 3000.0);
				pSession->m_Ping[i]->dwTime = (DWORD)newping;
				pSession->m_Ping[i]->dwStatus = status->attr._attr;
				break;
			}
		LeaveCriticalSection(&pSession->m_csPingSection);
*/
		return 0;
	}
	case EVENT_DISCONNECT:
	{
		//test.
		a_LogOutput( 1, "Disconnected from server!\n" );
		Sleep(60000);//sleep 60 second and then re-connect.
		pSession->Close();
		pSession->Open();
		//test end.
		a_LogOutput(1, "CECGameSession::IOCallBack, EVENT_DELSESSION, error code = %s", pSession->m_pNetMan->GetLastError());
		return 0;
	}

	}

	GNET::Protocol* pProtocol = (GNET::Protocol*)pData;

/*	if (gr.get_state() == game_record::enum_gr_replay && g_pGame->GetGameRun()->GetGameState() == CECGameRun::GS_GAME)
		pProtocol->Destroy();
	else*/
		pSession->AddNewProtocol(pProtocol);

	//for test.
//	a_LogOutput( 1, "Receive a new protocol!\n" );//Canceled 2010-08-24.
	//test end.

	return 0;
}

//	Add a protocol to m_NewPrtcList and prepare to be processed
int CECGameSession::AddNewProtocol(GNET::Protocol* pProtocol)
{
	EnterCriticalSection(&g_csSession);
	int iSize = m_aNewProtocols.Add(pProtocol);
	LeaveCriticalSection(&g_csSession);
	return iSize;
}
/*
//	When connection was broken, this function is called
void CECGameSession::LinkBroken(bool bDisconnect)
{
	if (g_pGame->GetGameRun()->GetUIManager())
	{
		const static char* szDlgName = "MsgBox_LinkBroken";
		CECGameRun* pGameRun = g_pGame->GetGameRun();
		CECUIManager* pUIMan = pGameRun->GetUIManager();

		bool bPopup = false;
		if (pGameRun->GetGameState() == CECGameRun::GS_LOGIN)
		{
			if (!pUIMan->GetLoginUIMan()->GetDialog(szDlgName))
				bPopup = true;
		}
		else if (pGameRun->GetGameState() == CECGameRun::GS_GAME)
		{
			if (!pUIMan->GetInGameUIMan()->GetDialog(szDlgName))
				bPopup = true;
		}

		if (bPopup)
		{
			int iMsg = bDisconnect ? FIXMSG_DISCONNECTED : FIXMSG_FAILEDTOCONN;
			pUIMan->ShowErrorMsg(g_pGame->GetFixedMsgTab()->GetWideString(iMsg), szDlgName);
		}
	}

	m_bLinkBroken = false;
}

//	Process one protocol
bool CECGameSession::ProcessOneProtocol(CECNetProtocolBase* pRaw)
{
	GNET::Protocol* pProtocol = pRaw->GetRawProtocol();
	bool bAddToOld = true;

	switch (pProtocol->GetType())
	{
	case PROTOCOL_CHALLENGE:				OnPrtcChallenge(pProtocol);				break;
	case PROTOCOL_KEYEXCHANGE:				OnPrtcKeyExchange(pProtocol);			break;
	case PROTOCOL_ONLINEANNOUNCE:			OnPrtcOnlineAnnounce(pProtocol);		break;
	case PROTOCOL_ROLELIST_RE:				OnPrtcRoleListRe(pProtocol);			break;
	case PROTOCOL_SELECTROLE_RE:			OnPrtcSelectRoleRe(pProtocol);			break;
	case PROTOCOL_CREATEROLE_RE:			OnPrtcCreateRoleRe(pProtocol);			break;
	case PROTOCOL_DELETEROLE_RE:			OnPrtcDeleteRoleRe(pProtocol);			break;
	case PROTOCOL_UNDODELETEROLE_RE:		OnPtrcUndoDeleteRoleRe(pProtocol);		break;

	case PROTOCOL_GAMEDATASEND:	
		
		ProcessGameData(((GamedataSend*)pProtocol)->data);
		break;

	case PROTOCOL_FUNGAMEDATASEND:
		ProcessFunGameData(pProtocol);
		break;
	case PROTOCOL_HOMETOWNDATASEND:
		ProcessHomeTownData(pProtocol);
		break;

	case PROTOCOL_CHATMESSAGE:				OnPrtcChatMessage(pProtocol, bAddToOld);break;
	case PROTOCOL_ERRORINFO:				OnPrtcErrorInfo(pProtocol);				break;
	case PROTOCOL_PLAYERLOGOUT:				OnPrtcPlayerLogout(pProtocol);			break;
	case PROTOCOL_PRIVATECHAT:				OnPrtcPrivateChat(pProtocol);			break;
	case PROTOCOL_PLAYERBASEINFO_RE:		OnPrtcPlayerBaseInfoRe(pProtocol);		break;
	case PROTOCOL_PLAYERBASEINFO2_RE:		OnPrtcPlayerBaseInfo2Re(pProtocol);		break;
	case PROTOCOL_GETUICONFIG_RE:			OnPrtcGetConfigRe(pProtocol);			break;
	case PROTOCOL_SETUICONFIG_RE:			OnPrtcSetConfigRe(pProtocol);			break;
	case PROTOCOL_GETHELPSTATES_RE:			OnPrtcGetHelpStatesRe(pProtocol);		break;
	case PROTOCOL_SETHELPSTATES_RE:			OnPrtcSetHelpStatesRe(pProtocol);		break;
	case PROTOCOL_SETCUSTOMDATA_RE:			OnPrtcSetCustomDataRe(pProtocol);		break;
	case PROTOCOL_GETCUSTOMDATA_RE:			OnPrtcGetCustomDataRe(pProtocol);		break;
	case PROTOCOL_GETPLAYERIDBYNAME_RE:		OnPrtcGetPlayerIDByNameRe(pProtocol);	break;
	case PROTOCOL_ROLESTATUSANNOUNCE:		OnPrtcRoleStatusAnnounce(pProtocol);	break;
	case PROTOCOL_ANNOUNCEFORBIDINFO:		OnPrtcAnnounceForbidInfo(pProtocol);	break;
	case PROTOCOL_UPDATEREMAINTIME:			OnPrtcUpdateRemainTime(pProtocol);		break;
	case PROTOCOL_WORLDCHAT:				OnPrtcWorldChat(pProtocol);				break;
	
	case PROTOCOL_GETPLAYERBRIEFINFO_RE:	
		
		m_pPBIProc->AddPlayerBriefInfoPrtc((GNET::GetPlayerBriefInfo_Re*)pProtocol);
		bAddToOld = false;
		break;

	//	Trade protocols
	case PROTOCOL_TRADESTART_RE:
	case PROTOCOL_TRADEMOVEOBJ_RE:
	case PROTOCOL_TRADEADDGOODS_RE:
	case PROTOCOL_TRADEREMOVEGOODS_RE:
	case PROTOCOL_TRADEDISCARD_RE:
	case PROTOCOL_TRADESUBMIT_RE:
	case PROTOCOL_TRADECONFIRM_RE:
	case PROTOCOL_TRADEEND:

		OnTradeProtocols(pProtocol);
		break;

	//	GM protocols
	case PROTOCOL_GMONLINENUM_RE:		OnPrtcGMOnlineNumberRe(pProtocol);		break;	
	case PROTOCOL_GMLISTONLINEUSER_RE:	OnPrtcGMListOnlineUserRe(pProtocol);	break;
	case PROTOCOL_GMKICKOUTUSER_RE:		OnPrtcGMKickOutUserRe(pProtocol);		break;	
	case PROTOCOL_GMSHUTUP_RE:			OnPrtcGMShutupUserRe(pProtocol);		break;
	case PROTOCOL_GMRESTARTSERVER_RE:	OnPrtcGMRestartServerRe(pProtocol);		break;
	case PROTOCOL_GMRESTARTSERVER:		OnPrtcGMRestartServer(pProtocol);		break;
	case PROTOCOL_GMKICKOUTROLE_RE:		OnPrtcGMKickOutRoleRe(pProtocol);		break;
	case PROTOCOL_GMSHUTUPROLE_RE:		OnPrtcGMShutupRoleRe(pProtocol);		break;
	case PROTOCOL_GMFORBIDROLE_RE:		OnPrtcGMForbidRoleRe(pProtocol);		break;
	case PROTOCOL_GMTOGGLECHAT_RE:		OnPrtcGMToggleChatRe(pProtocol);		break;

	//	Friend protocols
	case PROTOCOL_ADDFRIEND_RE:
	case PROTOCOL_GETFRIENDS_RE:
	case PROTOCOL_SETGROUPNAME_RE:
	case PROTOCOL_SETFRIENDGROUP_RE:
	case PROTOCOL_DELFRIEND_RE:
	case PROTOCOL_FRIENDSTATUS:
	case PROTOCOL_GETSAVEDMSG_RE:

		OnFriendProtocols(pProtocol);
		break;
		
	//	Faction protocols
	case PROTOCOL_FACTIONCHAT:

		if (g_pGame->GetFactionMan()->CanProcessPtrc(pProtocol))
		{
			g_pGame->GetFactionMan()->OnChat(static_cast<FactionChat*>(pProtocol));
		}
		else
		{
			AddIDForBriefInfo(static_cast<FactionChat*>(pProtocol)->src);

			bAddToOld = false;
		}

		break;

	case PROTOCOL_FACTIONRECRUIT_RE:

		if (g_pGame->GetFactionMan()->CanProcessPtrc(pProtocol))
		{
			g_pGame->GetFactionMan()->OnAcceptJoin(static_cast<FactionRecruit_Re*>(pProtocol));
		}
		else
		{
			AddIDForBriefInfo(static_cast<FactionRecruit_Re*>(pProtocol)->inviter);

			bAddToOld = false;
		}

		break;

	case PROTOCOL_FACTIONRESIGN_RE:

		if (g_pGame->GetFactionMan()->CanProcessPtrc(pProtocol))
		{
			g_pGame->GetFactionMan()->OnResign(static_cast<FactionResign_Re*>(pProtocol));
		}
		else
		{
			AddIDForBriefInfo(static_cast<FactionResign_Re*>(pProtocol)->roleid);

			bAddToOld = false;
		}

		break;

	case PROTOCOL_FACTIONNICKNAME_RE:

		if (g_pGame->GetFactionMan()->CanProcessPtrc(pProtocol))
		{
			g_pGame->GetFactionMan()->OnRename(static_cast<FactionNickname_Re*>(pProtocol));
		}
		else
		{
			AddIDForBriefInfo(static_cast<FactionNickname_Re*>(pProtocol)->roleid);
			AddIDForBriefInfo(static_cast<FactionNickname_Re*>(pProtocol)->receiver);

			bAddToOld = false;
		}

		break;

	case PROTOCOL_FACTIONLEAVE_RE:

		g_pGame->GetFactionMan()->OnLeave(static_cast<FactionLeave_Re*>(pProtocol));

		break;

	case PROTOCOL_FACTIONEXPEL_RE:

		if (g_pGame->GetFactionMan()->CanProcessPtrc(pProtocol))
		{
			g_pGame->GetFactionMan()->OnExpel(static_cast<FactionExpel_Re*>(pProtocol));
		}
		else
		{
			AddIDForBriefInfo(static_cast<FactionExpel_Re*>(pProtocol)->roleid);

			bAddToOld = false;
		}

		break;

	case PROTOCOL_FACTIONAPPOINT_RE:

		if (g_pGame->GetFactionMan()->CanProcessPtrc(pProtocol))
		{
			g_pGame->GetFactionMan()->OnAppoint(static_cast<FactionAppoint_Re*>(pProtocol));
		}
		else
		{
			AddIDForBriefInfo(static_cast<FactionAppoint_Re*>(pProtocol)->roleid);
			AddIDForBriefInfo(static_cast<FactionAppoint_Re*>(pProtocol)->candidate);

			bAddToOld = false;
		}

		break;

	case PROTOCOL_FACTIONCREATE_RE:
	case PROTOCOL_FACTIONANNOUNCE_RE:
	case PROTOCOL_FACTIONUPGRADE_RE:
	case PROTOCOL_FACTIONLIST_RE:
	case PROTOCOL_GETFACTIONBASEINFO_RE:
	case PROTOCOL_FACTIONDISMISS_RE:
	case PROTOCOL_TERRITORYSCOREUPDATE:

		OnFactionProtocols(pProtocol);
		break;

	//	Chatroom protocols
	case PROTOCOL_CHATROOMCREATE_RE:
	case PROTOCOL_CHATROOMINVITE:
	case PROTOCOL_CHATROOMINVITE_RE:
	case PROTOCOL_CHATROOMJOIN_RE:
	case PROTOCOL_CHATROOMLEAVE:
	case PROTOCOL_CHATROOMEXPEL:
	case PROTOCOL_CHATROOMSPEAK:
	case PROTOCOL_CHATROOMLIST_RE:

		OnChatRoomProtocols(pProtocol);
		break;

	//	Mail protocols
	case PROTOCOL_ANNOUNCENEWMAIL:
	case PROTOCOL_GETMAILLIST_RE:
	case PROTOCOL_GETMAIL_RE:
	case PROTOCOL_GETMAILATTACHOBJ_RE:
	case PROTOCOL_DELETEMAIL_RE:
	case PROTOCOL_PRESERVEMAIL_RE:
	case PROTOCOL_PLAYERSENDMAIL_RE:

		OnMailProtocols(pProtocol);
		break;

	//	Vendue protocols
	case PROTOCOL_AUCTIONOPEN_RE:
	case PROTOCOL_AUCTIONBID_RE:
	case PROTOCOL_AUCTIONLIST_RE:
	case PROTOCOL_AUCTIONCLOSE_RE:
	case PROTOCOL_AUCTIONGET_RE:
	case PROTOCOL_AUCTIONATTENDLIST_RE:
	case PROTOCOL_AUCTIONEXITBID_RE:
	case PROTOCOL_AUCTIONGETITEM_RE:

		OnVendueProtocols(pProtocol);
		break;

	//	Battle protocols
	case PROTOCOL_BATTLEGETLIST_RE:
	case PROTOCOL_BATTLEGETFIELD_RE:
	case PROTOCOL_BATTLEENTER_RE:
	case PROTOCOL_BATTLEJOIN_RE:
	case PROTOCOL_BATTLELEAVE_RE:
	case PROTOCOL_BATTLESTARTNOTIFY:

		OnBattleProtocols(pProtocol);
		break;
	
	// Instancing protocols
	case PROTOCOL_INSTANCINGAVAILABLELIST_RE:
	case PROTOCOL_INSTANCINGGETLIST_RE:
	case PROTOCOL_INSTANCINGGETFIELD_RE:
    case PROTOCOL_INSTANCINGKICK_RE:
	case PROTOCOL_INSTANCINGSTART_RE:
	case PROTOCOL_INSTANCINGJOINREQ:
    case PROTOCOL_INSTANCINGJOIN_RE:
    case PROTOCOL_INSTANCINGLEAVE_RE:
	case PROTOCOL_INSTANCINGAPPOINT_RE:
	case PROTOCOL_INSTANCINGAPPOINTNOTIFY:
    case PROTOCOL_INSTANCINGENTER_RE:
	case PROTOCOL_INSTANCINGACCEPT_RE:
    case PROTOCOL_INSTANCINGSTARTNOTIFY: 
    case PROTOCOL_INSTANCINGCLOSENOTIFY:
	case PROTOCOL_INSTANCINGKICKNOTIFY:
		OnInstancingProtocols(pProtocol);
		break;

	//	Account protocols
	case PROTOCOL_SELLPOINT_RE:
	case PROTOCOL_GETSELLLIST_RE:
	case PROTOCOL_FINDSELLPOINTINFO_RE:
	case PROTOCOL_SELLCANCEL_RE:
	case PROTOCOL_BUYPOINT_RE:
	case PROTOCOL_ANNOUNCESELLRESULT:
		
		OnAccountProtocols(pProtocol);
		break;

	//	RPCs...
	case RPC_FACTIONINVITE:
	case RPC_COMBATINVITE:
		OnFactionProtocols(pProtocol);
		bAddToOld = false;
		break;

	case RPC_TRADESTARTRQST:

		OnTradeProtocols(pProtocol);
		bAddToOld = false;
		break;

	case RPC_ADDFRIENDRQST:

		OnFriendProtocols(pProtocol);
		bAddToOld = false;
		break;

	case RPC_SECTINVITE:

		OnSectProtocols(pProtocol);
		bAddToOld = false;
		break;

	case PROTOCOL_KEEPALIVE:

		//	Do nothing ...
		break;

	case PROTOCOL_MATRIXCHALLENGE:

		OnPrtcMatrixChallenge(pProtocol);
		break;

	case PROTOCOL_CASHLOCK_RE:
	case PROTOCOL_CASHPASSWORDSET_RE:

		OnPrtcCashOpt(pProtocol);
		break;

	case PROTOCOL_ACREMOTECODE:

#ifdef USING_DEFENCE
		::EnterCriticalSection(&g_csDefence);
		g_DefenceCodeVec.push_back(static_cast<ACRemoteCode*>(pProtocol));
		::LeaveCriticalSection(&g_csDefence);
		bAddToOld = false;
#endif

		break;

	case PROTOCOL_TERRITORYMAPGET_RE:
	case PROTOCOL_TERRITORYCHALLENGE_RE:
	case PROTOCOL_TERRITORYENTER_RE:
	case PROTOCOL_TERRITORYLEAVE_RE:
	case PROTOCOL_TERRITORYENTERREMIND:

		OnTerritoryProtocols(pProtocol);
		break;

	case PROTOCOL_CIRCLELIST_RE:
	case PROTOCOL_GETCIRCLEBASEINFO_RE:
	case PROTOCOL_CIRCLENOTIFY:

		OnCircleProtocols(pProtocol);
		break;
	case PROTOCOL_GCIRCLECHAT:

		if (g_pGame->GetCircleMan()->CanProcessPtrc(pProtocol))
		{
			g_pGame->GetCircleMan()->OnChat(static_cast<GCircleChat*>(pProtocol));
		}
		else
		{
			AddIDForBriefInfo(static_cast<GCircleChat*>(pProtocol)->src);

			bAddToOld = false;
		}
		break;

	default:
		g_pGame->RuntimeDebugInfo(RTDCOL_WARNING, _AL("NET - Unknown protocol (%d)"), pProtocol->GetType());
		break;
	}

	//	Add this protocol to old list
	return bAddToOld;
}
*/
//	Process protocols in m_NewPrtcList
bool CECGameSession::ProcessNewProtocols()
{
	using namespace GNET;

	int i;

	EnterCriticalSection(&g_csSession);

	for (i=0; i < m_aNewProtocols.GetSize(); i++)
	{
		GNET::Protocol* pSrc = m_aNewProtocols[i];

		//
//		a_LogOutput( 1, "Enter Process Protocol!, the Protocol type = %d\n", pSrc->GetType() );//Canceled 2010-08-24.
		//process the protocol.
		if( PROTOCOL_GETITEMDESC == pSrc->GetType() )
		{
			a_LogOutput( 1, "Receive Procotol Item Desc!" );
			OnPrtcGetItemDesc( pSrc );
		}

		//供平台中心小工具使用的协议
		if ( PROTOCOL_GETITEMDESC_PLTFM == pSrc->GetType() )
		{
			OnPrtcGetItemDescForPlatform( pSrc );
		}

		//寻宝网小工具 角色交易
		if ( PROTOCOL_GET_ROLE_DESC == pSrc->GetType() )
		{
			//------------------------------
			//Test
			a_LogOutput( 1, "Receive Procotol Role Desc!" );
			//------------------------------
			OnPrtcGetRoleDesc( pSrc );
		}
		//主题站获取角色，类似寻宝网角色交易
		if ( PROTOCOL_GET_ROLE_DESC_SNS== pSrc->GetType() )
		{
			//------------------------------
			//Test
			a_LogOutput( 1, "Receive Procotol SNS Desc!" );
			//------------------------------
			OnPrtcGetRoleDescSnS( pSrc );
		}

		//test.
//		a_LogOutput( 1, "The Protocol type: %d\n", pSrc->GetType() );//Canceled 2010-08-24.
		//test end.
/*		CECNetProtocolBase* p = CECNetProtocolBase::CreateFrom(pSrc);

		if (p)
		{
			if (gr.get_state() == game_record::enum_gr_record
			 && g_pGame->GetGameRun()->GetGameState() == CECGameRun::GS_GAME
			 && !gr.filter_protocol(pSrc->GetType()))
			{
				frame_record* fr = gr.get_cur_frame();

				if (fr)
				{
					// the last init protocol
					if (pSrc->GetType() == PROTOCOL_GETUICONFIG_RE)
					{
						fr->set_delta_time(255);
						gr.push_cur_frame();
						fr = gr.inc_frame();
					}

					fr->push_event(new event_record_protocol(pSrc));
				}
			}

			m_aTempProtocols.Add(p);
		}
		else
			pSrc->Destroy();
*/
	}

// test code
/*	if(GetAsyncKeyState(VK_CONTROL)&0x8000)
	{
		if(GetAsyncKeyState('1')&0x8000)
		{
			GConsignRole roleInfo;
			CECRoleDescXMLParser roleDescXMLParser;
			ACString strXMLText = roleDescXMLParser.GetXMLText( &roleInfo );

			FILE* pFile = fopen( "./rolexunbao.xml", "w+t" );
			if ( pFile )
			{
				char* result = new char[ strXMLText.GetLength()*2 ];
				a_WideCharToMultiByte( strXMLText, strXMLText.GetLength(), result, strXMLText.GetLength()*2 );//wcslen(pszHint)
				fwrite( result, sizeof( char ), strXMLText.GetLength()*2, pFile );
				delete []result;
				fclose(pFile);
			}
		}
		if(GetAsyncKeyState('2')&0x8000)
		{
			int iIndex=1;
			while(1)
			{
				AString strFile;
				strFile.Format("E:\\2013913\\20130930\\Logs\\role%03d", iIndex++);
				FILE *pRoleFile = fopen(strFile, "rb");
				if ( !pRoleFile )
					break;
				{
					GetRoleDescSnS getRoleDesc;
					char* szRead = new char[9999];
					int iReadLen = fread(szRead, 1, 9999, pRoleFile);
					getRoleDesc.detailInfo.replace(szRead,iReadLen);
					delete []szRead;
					fclose(pRoleFile);
					OnPrtcGetRoleDescSnS(&getRoleDesc);
				}
			}
		}
	}
*/
	m_aNewProtocols.RemoveAll(false);

	LeaveCriticalSection(&g_csSession);

	m_bProcNewPtrc = true;

/*	for (i=0; i < m_aTempProtocols.GetSize(); i++)
	{
		CECNetProtocolBase* pProtocol = m_aTempProtocols[i];

		if (gr.get_state() != game_record::enum_gr_replay)
		{
			if (pProtocol->Filter(this))
			{
				m_aOldProtocols.Add(pProtocol);
				continue;
			}
		}

		if (pProtocol->Process(this))
		{
			m_aOldProtocols.Add(pProtocol);
		}
		else if (m_pPBIProc->AddUnprocessedPrtc(pProtocol))
		{
		}
		else
		{
			pProtocol->Detach();
			pProtocol->Destroy();
		}
	}

	m_aTempProtocols.RemoveAll(false);

	m_bProcNewPtrc = false;

	//	If there are some players who said something but we still don't 
	//	know their names, try to get that from server
	if (m_aPBIPlayers.GetSize())
	{
		//	Filter ids of players whose name we have known before
		int iAllCnt = m_aPBIPlayers.GetSize();
		AArray<int, int> aChatPlayers(0, iAllCnt);
		
		for (i=0; i < iAllCnt; i++)
		{
			int id = m_aPBIPlayers[i];
			if (!g_pGame->GetGameRun()->GetPlayerName(id, false))
				aChatPlayers.Add(id);
		}

		if (aChatPlayers.GetSize())
			GetPlayerBriefInfo(aChatPlayers.GetSize(), aChatPlayers.GetData(), 1);

		m_aPBIPlayers.RemoveAll(false);
	}

	if (m_pPBIProc)
		m_pPBIProc->Process();
*/
	return true;
}
/*
//	Clear all processed protocols in m_OldPrtcList
void CECGameSession::ClearOldProtocols()
{
	EnterCriticalSection(&g_csSession);

	for (int i=0; i < m_aOldProtocols.GetSize(); i++)
	{
		CECNetProtocolBase* p = m_aOldProtocols[i];
		p->Destroy();
	}

	m_aOldProtocols.RemoveAll(false);

	LeaveCriticalSection(&g_csSession);
}

//	Filter protocols. Return true if we shouldn't handle this protocol in
//	current state
bool CECGameSession::FilterProtocols(GNET::Protocol* pProtocol)
{
	bool bFilter = false;

	switch (pProtocol->GetType())
	{
	case PROTOCOL_CHALLENGE:
	case PROTOCOL_KEYEXCHANGE:
	case PROTOCOL_ONLINEANNOUNCE:
	case PROTOCOL_ROLELIST_RE:	
	case PROTOCOL_SELECTROLE_RE:
	case PROTOCOL_CREATEROLE_RE:
	case PROTOCOL_DELETEROLE_RE:
	case PROTOCOL_UNDODELETEROLE_RE:
	case PROTOCOL_MATRIXCHALLENGE:

		//	If we aren't in login state, ignore this data
		if (g_pGame->GetGameRun()->GetGameState() != CECGameRun::GS_LOGIN)
			bFilter = true;

		break;

	case PROTOCOL_GAMEDATASEND:
	case PROTOCOL_CHATMESSAGE:
	case PROTOCOL_WORLDCHAT:
	case PROTOCOL_PLAYERLOGOUT:
	case PROTOCOL_PRIVATECHAT:
	case PROTOCOL_PLAYERBASEINFO_RE:
	case PROTOCOL_PLAYERBASEINFO2_RE:
	case PROTOCOL_GETUICONFIG_RE:
	case PROTOCOL_SETUICONFIG_RE:
	case PROTOCOL_SETHELPSTATES_RE:
	case PROTOCOL_GETHELPSTATES_RE:
	case PROTOCOL_TRADESTART_RE:
	case PROTOCOL_TRADEMOVEOBJ_RE:
	case PROTOCOL_TRADEADDGOODS_RE:
	case PROTOCOL_TRADEREMOVEGOODS_RE:
	case PROTOCOL_TRADEDISCARD_RE:
	case PROTOCOL_TRADESUBMIT_RE:
	case PROTOCOL_TRADECONFIRM_RE:
	case PROTOCOL_TRADEEND:
	case PROTOCOL_GETPLAYERBRIEFINFO_RE:
	case PROTOCOL_GETPLAYERIDBYNAME_RE:
	case PROTOCOL_ROLESTATUSANNOUNCE:
	case RPC_TRADESTARTRQST:

	case PROTOCOL_GMONLINENUM_RE:	
	case PROTOCOL_GMLISTONLINEUSER_RE:
	case PROTOCOL_GMKICKOUTUSER_RE:
	case PROTOCOL_GMSHUTUP_RE:
	case PROTOCOL_GMRESTARTSERVER_RE:
	case PROTOCOL_GMKICKOUTROLE_RE:
	case PROTOCOL_GMSHUTUPROLE_RE:

	case PROTOCOL_ADDFRIEND_RE:
	case PROTOCOL_GETFRIENDS_RE:
	case PROTOCOL_SETGROUPNAME_RE:
	case PROTOCOL_SETFRIENDGROUP_RE:
	case PROTOCOL_DELFRIEND_RE:
	case PROTOCOL_FRIENDSTATUS:
	case PROTOCOL_GETSAVEDMSG_RE:
	case RPC_ADDFRIENDRQST:
	case RPC_FACTIONINVITE:
	case RPC_SECTINVITE:

	case PROTOCOL_FACTIONCREATE_RE:
	case PROTOCOL_FACTIONANNOUNCE_RE:
	case PROTOCOL_FACTIONUPGRADE_RE:
	case PROTOCOL_FACTIONLIST_RE:
	case PROTOCOL_GETFACTIONBASEINFO_RE:
	case PROTOCOL_FACTIONDISMISS_RE:
	case PROTOCOL_FACTIONCHAT:
	case PROTOCOL_FACTIONRESIGN_RE:
	case PROTOCOL_FACTIONNICKNAME_RE:
	case PROTOCOL_FACTIONLEAVE_RE:
	case PROTOCOL_FACTIONEXPEL_RE:
	case PROTOCOL_FACTIONAPPOINT_RE:

	case PROTOCOL_CHATROOMCREATE_RE:
	case PROTOCOL_CHATROOMINVITE:
	case PROTOCOL_CHATROOMINVITE_RE:
	case PROTOCOL_CHATROOMJOIN_RE:
	case PROTOCOL_CHATROOMLEAVE:
	case PROTOCOL_CHATROOMEXPEL:
	case PROTOCOL_CHATROOMSPEAK:
	case PROTOCOL_CHATROOMLIST_RE:

	case PROTOCOL_ANNOUNCENEWMAIL:
	case PROTOCOL_GETMAILLIST_RE:
	case PROTOCOL_GETMAIL_RE:
	case PROTOCOL_GETMAILATTACHOBJ_RE:
	case PROTOCOL_DELETEMAIL_RE:
	case PROTOCOL_PRESERVEMAIL_RE:
	case PROTOCOL_PLAYERSENDMAIL_RE:

	case PROTOCOL_AUCTIONOPEN_RE:
	case PROTOCOL_AUCTIONBID_RE:
	case PROTOCOL_AUCTIONLIST_RE:
	case PROTOCOL_AUCTIONCLOSE_RE:
	case PROTOCOL_AUCTIONGET_RE:
	case PROTOCOL_AUCTIONATTENDLIST_RE:
	case PROTOCOL_AUCTIONEXITBID_RE:
	case PROTOCOL_AUCTIONGETITEM_RE:

//	case PROTOCOL_BATTLECHALLENGE_RE:
//	case PROTOCOL_BATTLECHALLENGEMAP_RE:
	case PROTOCOL_BATTLEGETMAP_RE:
	case PROTOCOL_BATTLEENTER_RE:
	case PROTOCOL_BATTLESTATUS_RE:

	case PROTOCOL_SELLPOINT_RE:
	case PROTOCOL_GETSELLLIST_RE:
	case PROTOCOL_FINDSELLPOINTINFO_RE:
	case PROTOCOL_SELLCANCEL_RE:
	case PROTOCOL_BUYPOINT_RE:
	case PROTOCOL_ANNOUNCESELLRESULT:
		
	case PROTOCOL_SNSSETPLAYERINFO_RE:
	case PROTOCOL_SNSPRESSMESSAGE_RE:
	case PROTOCOL_SNSCANCELMESSAGE_RE:
	case PROTOCOL_SNSAPPLY_RE:
	case PROTOCOL_SNSLISTMESSAGE_RE:
	case PROTOCOL_SNSGETMESSAGE_RE:
	case PROTOCOL_SNSGETPLAYERINFO_RE:
	case PROTOCOL_SNSVOTE_RE:
	case PROTOCOL_SNSACCEPTAPPLY_RE:
	case PROTOCOL_SNSAPPLYNOTIFY:
	case PROTOCOL_SNSCANCELLEAVEMESSAGE_RE:

	case PROTOCOL_INSTANCINGAVAILABLELIST_RE:
	case PROTOCOL_INSTANCINGGETLIST_RE:
	case PROTOCOL_INSTANCINGGETFIELD_RE:
    case PROTOCOL_INSTANCINGKICK_RE:
	case PROTOCOL_INSTANCINGSTART_RE:
	case PROTOCOL_INSTANCINGJOINREQ:
    case PROTOCOL_INSTANCINGJOIN_RE:
    case PROTOCOL_INSTANCINGLEAVE_RE:
	case PROTOCOL_INSTANCINGAPPOINT_RE:
	case PROTOCOL_INSTANCINGAPPOINTNOTIFY:
    case PROTOCOL_INSTANCINGENTER_RE:
	case PROTOCOL_INSTANCINGACCEPT_RE:
    case PROTOCOL_INSTANCINGSTARTNOTIFY: 
    case PROTOCOL_INSTANCINGCLOSENOTIFY:
	case PROTOCOL_INSTANCINGKICKNOTIFY:

    case PROTOCOL_REFLISTREFERRALS_RE:
    case PROTOCOL_REFWITHDRAWEXP_RE:
    case PROTOCOL_REFWITHDRAWBONUS_RE:
    case PROTOCOL_REFGETREFERENCECODE_RE:

	case PROTOCOL_FUNGAMEDATASEND:

	case PROTOCOL_HOMETOWNDATASEND:
	case PROTOCOL_HOMETOWNEXCHGMONEY_RE:

	case PROTOCOL_ACHIEVEMENTMESSAGE:
	case PROTOCOL_GSHOPNOTIFYSCHEME:
	case PROTOCOL_PKMESSAGE:
	case PROTOCOL_REFINEMESSAGE:

	case PROTOCOL_TERRITORYMAPGET_RE:
	case PROTOCOL_TERRITORYCHALLENGE_RE:
	case PROTOCOL_TERRITORYENTER_RE:
	case PROTOCOL_TERRITORYLEAVE_RE:
	case PROTOCOL_TERRITORYENTERREMIND:
	case PROTOCOL_TERRITORYSCOREUPDATE:
		
	case PROTOCOL_UNIQUEBIDHISTORY_RE:

	case PROTOCOL_CIRCLELIST_RE:
	case PROTOCOL_GETCIRCLEBASEINFO_RE:
	case PROTOCOL_CIRCLENOTIFY:
	case PROTOCOL_GCIRCLECHAT:

		//	If we aren't in game state, ignore this data
		if (g_pGame->GetGameRun()->GetGameState() != CECGameRun::GS_GAME)
			bFilter = true;

		break;

	case PROTOCOL_ERRORINFO:
	case PROTOCOL_KEEPALIVE:
	case PROTOCOL_UPDATEREMAINTIME:
	case PROTOCOL_SETCUSTOMDATA_RE:
	case PROTOCOL_GETCUSTOMDATA_RE:
	case PROTOCOL_ANNOUNCEFORBIDINFO:
		
		break;

	default:
		g_pGame->RuntimeDebugInfo(RTDCOL_WARNING, _AL("NET - Unfiltered protocol (%d)"), pProtocol->GetType());
		break;
	}

	if (bFilter)
		g_pGame->RuntimeDebugInfo(RTDCOL_WARNING, _AL("NET - Protocol (%d) is filtered"), pProtocol->GetType());

	return bFilter;
}

//	Output link server error
void CECGameSession::OutputLinkSevError(int iRetCode)
{
	wchar_t szUnknown[128];

	int iIdx = iRetCode + BASEIDX_LINKSEVERROR;
	const wchar_t* szMsg = m_ErrorMsgs.GetWideString(iIdx);
	if (!szMsg)
	{
		a_sprintf(szUnknown, L"Unknown error (%d)", iRetCode);
		szMsg = szUnknown;
	}

	CECUIManager* pUIMan = g_pGame->GetGameRun()->GetUIManager();
	if (pUIMan)
	{
		AString strName;
		strName.Format("LinkSevError%d",iRetCode);
		pUIMan->ShowErrorMsg(szMsg, strName);
	}
	else
		g_pGame->RuntimeDebugInfo(RTDCOL_ERROR, szMsg);
}

void CECGameSession::cash_Lock(char cLock, const ACHAR* szPassword)
{
	GNET::CashLock p;
	p.userid = m_iCharID;
	p.lock = cLock;

	if (szPassword && szPassword[0])
	{
		int iLen = a_strlen(szPassword) * sizeof (ACHAR);
		p.cash_password.replace(szPassword, iLen);
	}

	SendNetData(p);
}

void CECGameSession::cash_SetPassword(const ACHAR* szPassword)
{
	GNET::CashPasswordSet p;
	p.userid = m_iCharID;

	if (szPassword && szPassword[0])
	{
		int iLen = a_strlen(szPassword) * sizeof (ACHAR);
		p.cash_password.replace(szPassword, iLen);
	}

	SendNetData(p);
}

void CECGameSession::send_acanswer(int type, int seq, int reserved, int answer)
{
	StackChecker::ACTrace(0);

	GNET::ACAnswer p;
	p.roleid = m_iCharID;
	p.qtype = type;
	p.seq = seq;
	p.reserved = reserved;
	p.answer = answer;
	SendNetData(p, true);
}

//	Send game data
bool CECGameSession::SendGameData(void* pData, int iSize)
{
	StackChecker::ACTrace(7);
	using namespace GNET;

	GamedataSend p;
	p.data.replace(pData, iSize);
	return SendNetData(p);
}

//	Send fun game data
bool CECGameSession::SendFunGameData(void* pData, int iSize)
{
	using namespace GNET;

	FunGamedataSend p;
	p.data.replace(pData, iSize);

	return SendNetData(p);
}

bool CECGameSession::SendHomeTownData(void* pData, int iSize)
{
	using namespace GNET;
	HometowndataSend p;
	p.data.replace(pData, iSize);
	return SendNetData(p);
	
}
//	Send chat data
bool CECGameSession::SendChatData(char cChannel, const ACHAR* szMsg, int nPack, int nSlot)
{
	using namespace GNET;
	using namespace CHAT_C2S;

	if (!szMsg || !szMsg[0])
		return true;

	PublicChat p;
	p.channel	= cChannel;
	p.roleid	= m_iCharID;

	if (nPack >= 0)
	{
		chat_equip_item c;
		c.cmd_id = CHAT_EQUIP_ITEM;
		c.where = nPack;
		c.index = nSlot;
		p.data.replace(&c, sizeof(c));
	}

	//	Fill message string
	int iMsgLen = a_strlen(szMsg);
	if (iMsgLen * sizeof (ACHAR) > 280)
		iMsgLen = 280 / sizeof (ACHAR);

	int iSize = iMsgLen * sizeof (ACHAR);
	p.msg.replace(szMsg, iSize);

	SendNetData(p);

	return true;
}
*/
//	Send private chat data
//bool CECGameSession::SendPrivateChatData(const ACHAR* szDstName, const ACHAR* szMsg,
//								BYTE byFlag/* 0 */, int idPlayer/* 0 */, int nPack, int nSlot)
/*{
	using namespace GNET;
	using namespace CHAT_C2S;

	if (!szMsg || !szMsg[0])
		return true;
	// 竞技场中，屏蔽私聊
	CECHostPlayer* pHost = g_pGame->GetGameRun()->GetHostPlayer();
	if(pHost->GetBattleType() == BATTLE_TYPE_ARENA || pHost->GetBattleType() == BATTLE_TYPE_ARENAREBORN || pHost->GetBattleType() == BATTLE_TYPE_CRSSVR)
		return true;

	PrivateChat p;
	p.srcroleid = m_iCharID;
	p.dstroleid = idPlayer;
	p.channel	= byFlag;
	p.srcrolelevel = 0;

	if (nPack >= 0)
	{
		chat_equip_item c;
		c.cmd_id = CHAT_EQUIP_ITEM;
		c.where = nPack;
		c.index = nSlot;
		p.data.replace(&c, sizeof(c));
	}

	//	Fill host player name
	const ACHAR* szSrcName = pHost->GetName();
	int iSize = a_strlen(szSrcName) * sizeof (ACHAR);
	ASSERT(iSize);
	p.src_name.replace(szSrcName, iSize);

	//	Fill destination player's name
	iSize = a_strlen(szDstName) * sizeof (ACHAR);
	p.dst_name.replace(szDstName, iSize);

	//	Fill message string
	int iMsgLen = a_strlen(szMsg);
	if (iMsgLen * sizeof (ACHAR) > 160)
		iMsgLen = 160 / sizeof (ACHAR);

	iSize = iMsgLen * sizeof (ACHAR);
	p.msg.replace(szMsg, iSize);

	//  发送密语消息,并添加对象到临时好友列表中
	int ChatID = g_pGame->GetGameRun()->GetPlayerID(szDstName);
	
    if (ChatID)
	{
		CECFriendMan* pFriendMan = g_pGame->GetGameRun()->GetHostPlayer()->GetFriendMan();
        pFriendMan->AddTempFriend(ChatID, szDstName);
	}
	return SendNetData(p);
}

//	Create a role
bool CECGameSession::CreateRole(const GNET::RoleInfo& Info, const ACHAR* szReferID)
{
	GNET::CreateRole cr;
	cr.userid	= m_iUserID;
	cr.roleinfo	= Info;
	if(szReferID)
		cr.referid.replace(szReferID, a_strlen(szReferID)*sizeof(ACHAR));
	return SendNetData(cr);
}

//	Select role
bool CECGameSession::SelectRole(int cid, int line_id)
{
	SetCharacterID(cid);
	return SendNetData(GNET::SelectRole(cid, line_id));
}

//	Delete role
bool CECGameSession::DeleteRole(int cid)
{
	return SendNetData(GNET::DeleteRole(cid, 0));
}

//	Restore role
bool CECGameSession::RestoreRole(int cid)
{
	return SendNetData(GNET::UndoDeleteRole(cid, 0));
}

//	Start game
bool CECGameSession::StartGame()
{
	m_pNetMan->ChangeState(m_idLink, &GNET::state_GDataExchgClient);
	bool bRet = SendNetData(GNET::EnterWorld(m_iCharID, _SID_INVALID));
	DoOvertimeCheck(true, OT_ENTERGAME, 30000);
	return bRet;
}

//	Re-login
bool CECGameSession::ReLogin(bool bSelRole)
{
	if (!bSelRole)
	{
		//	Return to login interface, Close connect
		Close();
	}
	else	//	Return to role selecting interface
	{	
		m_pNetMan->ChangeState(m_idLink, &state_GSelectRoleClient);
		SendNetData(RoleList(m_iUserID, _SID_INVALID, _HANDLE_BEGIN));
	}

	return true;
}

//	Get player's bese info (including custom data)
void CECGameSession::GetRoleBaseInfo(int iNumRole, const int* aRoleIDs)
{
	using namespace GNET;

	if (!iNumRole || !aRoleIDs)
		return;

	static const int iNumLimit = 240;
//	static const int iNumLimit = 100;
	int iCount = 0;

	while (iCount < iNumRole)
	{
		PlayerBaseInfo p;
		p.roleid = m_iCharID;

		int iNumSend = iNumLimit;
		if (iCount + iNumLimit > iNumRole)
			iNumSend = iNumRole - iCount;

		for (int i=0; i < iNumSend; i++)
			p.playerlist.add(aRoleIDs[iCount+i]);

		SendNetData(p);

		iCount += iNumSend;
	}
}

//	Get clone's bese info by player's id
void CECGameSession::GetRoleBaseInfo2(int iPlayerId, int iCloneId)
{
	using namespace GNET;

	if (!iPlayerId || !iCloneId)
		return;

	PlayerBaseInfo2 p;
	p.roleid	= m_iCharID;
	p.playerid	= iPlayerId;
	p.cloneid	= iCloneId;

	SendNetData(p);	
}


//	Get player's customized data
void CECGameSession::GetRoleCustomizeData(int iNumRole, const int* aRoleIDs)
{
	using namespace GNET;

	if (!iNumRole || !aRoleIDs)
		return;

	static const int iNumLimit = 240;
//	static const int iNumLimit = 100;
	int iCount = 0;

	while (iCount < iNumRole)
	{
		GetCustomData p;
		p.roleid = m_iCharID;

		int iNumSend = iNumLimit;
		if (iCount + iNumLimit > iNumRole)
			iNumSend = iNumRole - iCount;

		for (int i=0; i < iNumSend; i++)
			p.playerlist.add(aRoleIDs[iCount+i]);

		SendNetData(p);

		iCount += iNumSend;
	}
}

//	Get player's faction info
void CECGameSession::GetFactionInfo(int iNumFaction, const int* aFactinoIDs, char scale)
{
	using namespace GNET;
	const int iNumLimit = 240;
	int iCount = 0;

	while (iCount < iNumFaction)
	{
		GetFactionBaseInfo p;
		p.roleid = m_iCharID;
		p.scale = scale;
		int iNumSend = iNumLimit;

		if (iCount + iNumLimit > iNumFaction)
			iNumSend = iNumFaction - iCount;

		for (int i = 0; i < iNumSend; i++)
			p.factionlist.add(aFactinoIDs[iCount+i]);

		SendNetData(p);
		iCount += iNumSend;
	}
}

//	Set custom data
void CECGameSession::SetRoleCustomizeData(int idRole, void* pData, int iDataSize)
{
	using namespace GNET;

	if (!pData || !iDataSize)
		return;
	
	SetCustomData p;
	p.roleid = idRole;
	p.custom_data.replace(pData, iDataSize);
	SendNetData(p);
}

//	Save necessary user configs (UI, shortcut, accelerate keys) to server
void CECGameSession::SaveConfigData(const void* pDataBuf, int iSize)
{
	using namespace GNET;

	if (!pDataBuf || !iSize)
		return;
	
	SetUIConfig p;
	p.roleid = m_iCharID;
	p.ui_config.replace(pDataBuf, iSize);
	SendNetData(p);
}

//	Load necessary user configs (UI, shortcut, accelerate keys) from server
void CECGameSession::LoadConfigData()
{
	using namespace GNET;

	GetUIConfig p;
	p.roleid = m_iCharID;
	SendNetData(p);

	// 放在这里是因为不能太早的发该协议
	if(m_iReferrer)
		GetPlayerBriefInfo(1, &m_iReferrer, 2);  //获取推广人的名字

}

//	Save help states data to server
void CECGameSession::SaveHelpStates(const void* pDataBuf, int iSize)
{
	using namespace GNET;

	if (!pDataBuf || !iSize)
		return;
	
	SetHelpStates p;
	p.roleid = m_iCharID;
	p.help_states.replace(pDataBuf, iSize);
	SendNetData(p);
}

//	Load help states data from server, and when help states received, we will call callback function to
//	let the caller get the data
void CECGameSession::LoadHelpStates()
{
//	m_pfnHelpStatesCallBack = pfnHelpStatesCallBack;

	using namespace GNET;

	GetHelpStates p;
	p.roleid = m_iCharID;
	SendNetData(p);
}
*/
/*	Get player's brief information

	iReason: reason to get player's information

		0: try to get host player's team member's information.
		1: try to get someone's name who just said something.
		2: just try to get players' names
		3: get player's brief infomation to handle team invitation
*/
void CECGameSession::GetPlayerBriefInfo(int iNumPlayer, int* aIDs, int iReason)
{
/*	if (!iNumPlayer || !aIDs)
		return;

	static const int iNumLimit = 100;
	int iCount = 0;

	while (iCount < iNumPlayer)
	{
		GNET::GetPlayerBriefInfo p;
		p.roleid = m_iCharID;
		p.reason = (BYTE)iReason;

		int iNumSend = iNumLimit;
		if (iCount + iNumLimit > iNumPlayer)
			iNumSend = iNumPlayer - iCount;

		for (int i=0; i < iNumSend; i++)
		{
			ASSERT(aIDs[iCount+i]);
			p.playerlist.add(aIDs[iCount+i]);
		}

		SendNetData(p);

		iCount += iNumSend;
	}*/
}

/*	Get player's ID by name

	iReason: the reason to get player's ID
		
		0: no special reason, just get it
		1: for invite a player who is very far from us to a team
*//*
void CECGameSession::GetPlayerIDByName(const ACHAR* szName, int iReason)
{
	GNET::GetPlayerIDByName p;

	int iSize = a_strlen(szName) * sizeof (ACHAR);
	p.rolename.replace(szName, iSize);
	p.reason = (BYTE)iReason;
	SendNetData(p);
}

//	Report problems to GM
void CECGameSession::SubmitProblemToGM(const ACHAR* szContent)
{
	if (!szContent || !szContent[0])
		return;

	CECHostPlayer* pHost = g_pGame->GetGameRun()->GetHostPlayer();
	CELPrecinct* pPrecinct = g_pGame->GetGameRun()->GetWorld()->GetCurPrecinct();
	A3DVECTOR3 v = pHost->GetPos();
	
	GNET::Report2GM p;

	p.roleid = m_iCharID;
	p.rolename.replace(pHost->GetName(), a_strlen(pHost->GetName()) * sizeof (ACHAR));
	p.posx = v.x;
	p.posy = v.y;
	p.posz = v.z;

	//	Fill content string
	int iMsgLen = a_strlen(szContent);
	if (iMsgLen * sizeof (ACHAR) > 160)
		iMsgLen = 160 / sizeof (ACHAR);

	int iSize = iMsgLen * sizeof (ACHAR);
	p.content.replace(szContent, iSize);

	if (pPrecinct)
		p.mapzone.replace(pPrecinct->GetName(), a_strlen(pPrecinct->GetName()) * sizeof (ACHAR));

	SendNetData(p);
}

//	Impeach else player to GM
void CECGameSession::ImpeachPlayerToGM(const ACHAR* szName, const ACHAR* szContent)
{
	if (!szName || !szName[0] || !szContent || !szContent[0])
		return;

	CECHostPlayer* pHost = g_pGame->GetGameRun()->GetHostPlayer();
	CELPrecinct* pPrecinct = g_pGame->GetGameRun()->GetWorld()->GetCurPrecinct();
	A3DVECTOR3 v = pHost->GetPos();

	GNET::Complain2GM p;

	p.roleid = m_iCharID;
	p.rolename.replace(pHost->GetName(), a_strlen(pHost->GetName()) * sizeof (ACHAR));
	p.posx = v.x;
	p.posy = v.y;
	p.posz = v.z;
	p.comp_rolename.replace(szName, a_strlen(szName) * sizeof (ACHAR));

	//	Fill content string
	int iMsgLen = a_strlen(szContent);
	if (iMsgLen * sizeof (ACHAR) > 160)
		iMsgLen = 160 / sizeof (ACHAR);

	int iSize = iMsgLen * sizeof (ACHAR);
	p.content.replace(szContent, a_strlen(szContent) * sizeof (ACHAR));

	if (pPrecinct)
		p.mapzone.replace(pPrecinct->GetName(), a_strlen(pPrecinct->GetName()) * sizeof (ACHAR));

	SendNetData(p);
}

DWORD CECGameSession::GetPingTime(int idServer, DWORD& dwStatus)
{
	EnterCriticalSection(&m_csPingSection);
	DWORD dwRet = 99999999;
	dwStatus = 0;
	if( m_Ping.GetSize() > idServer && m_Ping[idServer] != NULL &&
		m_Ping[idServer]->idLink != (DWORD)-1)
	{
		dwRet = m_Ping[idServer]->dwTime;
		dwStatus = m_Ping[idServer]->dwStatus;
	}
	LeaveCriticalSection(&m_csPingSection);
	return dwRet;
}

void CECGameSession::OnPrtcChallenge(GNET::Protocol* pProtocol)
{
	using namespace GNET;

	g_pGame->RuntimeDebugInfo(RTDCOL_NETWORK, _AL("NET - protocol challenge"));
	DoOvertimeCheck(false, OT_CHALLENGE, 0);

	Challenge* p = (Challenge*)pProtocol;
	AString str((const char *)p->edition.begin(), p->edition.size());
	bool bVerStringOk = !stricmp(g_pGame->GetVersionString(), str);

	if (g_pGame->GetGameInit().bVerCheck && (p->version != g_pGame->GetGameVersion() || !bVerStringOk))
	{
		a_LogOutput(1, "local ver: %s", g_pGame->GetVersionString());
		a_LogOutput(1, "svr ver: %s", str);		
		CECUIManager* pUIMan = g_pGame->GetGameRun()->GetUIManager();
		CECStringTab* pStrTab = g_pGame->GetFixedMsgTab();
		if (pUIMan)
		{
			if (p->version > g_pGame->GetGameVersion())
				pUIMan->ShowErrorMsg(pStrTab->GetWideString(FIXMSG_WRONGVERSION));
			else
				pUIMan->ShowErrorMsg(pStrTab->GetWideString(FIXMSG_SERVERUPDATE));
		}
		else
			::MessageBox(g_pGame->GetGameInit().hWnd, _AL("Element client"), pStrTab->GetWideString(FIXMSG_WRONGVERSION), MB_OK);

		//	Disconnect from server
		m_bConnected = false;
		l_bExitConnect = true;
		m_pNetMan->Disconnect();
		return;
	}

	Response res;
	m_pNetMan->SetupResponse(res, m_strUserName, m_strPassword, p->nonce, p->algo);

	// clear password
	char * szBuffer = m_strPassword.LockBuffer();
	memset(szBuffer, 0, strlen(szBuffer));
	m_strPassword.UnlockBuffer();

	SendNetData(res);
}

void CECGameSession::OnPrtcKeyExchange(GNET::Protocol* pProtocol)
{
	using namespace GNET;

	g_pGame->RuntimeDebugInfo(RTDCOL_NETWORK, _AL("NET - protocol keyexchange"));

	KeyExchange* p = (KeyExchange*)pProtocol;
	p->Setup(m_pNetMan, m_idLink, m_strUserName, m_bKickUser ? true : false);

	// clear user name
	char * szBuffer = m_strUserName.LockBuffer();
	memset(szBuffer, 0, strlen(szBuffer));
	m_strUserName.UnlockBuffer();

	SendNetData(p);
}

void CECGameSession::OnPrtcOnlineAnnounce(GNET::Protocol* pProtocol)
{
	using namespace GNET;

	g_pGame->RuntimeDebugInfo(RTDCOL_NETWORK, _AL("NET - protocol online announce"));

	OnlineAnnounce* p = (OnlineAnnounce*)pProtocol;
	m_iUserID = p->userid;
	m_pNetMan->SetZoneID(p->zoneid);
	m_zoneID = p->zoneid;
	m_pNetMan->ChangeState(m_idLink, &state_GSelectRoleClient);
	SendNetData(RoleList(m_iUserID, _SID_INVALID, _HANDLE_BEGIN));

	m_iiAccountTime		= ((INT64)p->remain_time) * 1000;
	m_iiFreeGameTime	= ((INT64)p->free_time_left) * 1000;
	m_iFreeGameEnd		= p->free_time_end;
	m_iDistrictID		= p->district_id;	  // 设置大区id
	m_dwLastAccTime		= a_GetTime();

	//	Enter select role interface
	CECLoginUIMan* pLoginUIMan = g_pGame->GetGameRun()->GetUIManager()->GetLoginUIMan();
	ASSERT(pLoginUIMan);
	pLoginUIMan->LaunchCharacter();
}

void CECGameSession::OnPrtcRoleListRe(GNET::Protocol* pProtocol)
{
	using namespace GNET;
	
	g_pGame->RuntimeDebugInfo(RTDCOL_NETWORK, _AL("NET - protocol rolelist re"));

	CECLoginUIMan* pLoginUIMan = g_pGame->GetGameRun()->GetUIManager()->GetLoginUIMan();
	ASSERT(pLoginUIMan);

	RoleList_Re* p = (RoleList_Re*)pProtocol;
	if (p->result != ERR_SUCCESS)
	{
		g_pGame->RuntimeDebugInfo(RTDCOL_ERROR, _AL("NET - protocol rolelist re return error"));

		if (p->handle != _HANDLE_END)
			SendNetData(RoleList(m_iUserID, _SID_INVALID, p->handle));
		else
		{
			OutputLinkSevError(p->result);
			pLoginUIMan->AddCharacter(NULL);
		}

		return;
	}

	ASSERT(m_iUserID == p->userid);

	int i, iNumRole = p->rolelist.size();
	for (i=0; i < iNumRole; i++)
		pLoginUIMan->AddCharacter(&p->rolelist[i]);

	m_iReferrer = p->referrer;

	if (p->handle != _HANDLE_END)
		SendNetData(RoleList(m_iUserID, _SID_INVALID, p->handle));
	else
		pLoginUIMan->AddCharacter(NULL);
}

void CECGameSession::OnPrtcSelectRoleRe(GNET::Protocol* pProtocol)
{
	using namespace GNET;

	g_pGame->RuntimeDebugInfo(RTDCOL_NETWORK, _AL("NET - protocol selectrole re"));
	
	SelectRole_Re* p = (SelectRole_Re*)pProtocol;
	if (p->result != ERR_SUCCESS)
	{
		OutputLinkSevError(p->result);
		g_pGame->RuntimeDebugInfo(RTDCOL_ERROR, _AL("NET - protocol selectrole re return error"));
		return;
	}
	g_pGame->ResetPrivilege(&p->auth.GetVector()[0], p->auth.size());

	CECLoginUIMan* pLoginUIMan = g_pGame->GetGameRun()->GetUIManager()->GetLoginUIMan();
	if (pLoginUIMan)
		pLoginUIMan->LaunchLoading();
	else	//	Start game directly
		StartGame();
}

void CECGameSession::OnPrtcCreateRoleRe(GNET::Protocol* pProtocol)
{
	using namespace GNET;

	g_pGame->RuntimeDebugInfo(RTDCOL_NETWORK, _AL("NET - protocol createrole re"));

	CreateRole_Re* p = (CreateRole_Re*)pProtocol;
	if (p->result != ERR_SUCCESS)
	{
		if(p->result == 8)
		{
			OutputLinkSevError(140);
			g_pGame->RuntimeDebugInfo(RTDCOL_ERROR, _AL("NET - protocol createrole re return error"));
			return;
		}
		if(p->result == 9)
		{
			OutputLinkSevError(141);
			g_pGame->RuntimeDebugInfo(RTDCOL_ERROR, _AL("NET - protocol createrole re return error"));
			return;
		}
		if(p->result == 11)
		{
			OutputLinkSevError(143);
			g_pGame->RuntimeDebugInfo(RTDCOL_ERROR, _AL("NET - protocol createrole re return error"));
			return;
		}
		OutputLinkSevError(p->result);
		g_pGame->RuntimeDebugInfo(RTDCOL_ERROR, _AL("NET - protocol createrole re return error"));
		return;
	}
	else
	{
		if(p->refretcode == 9 )
		{
			OutputLinkSevError(142);
			g_pGame->RuntimeDebugInfo(RTDCOL_ERROR, _AL("NET - protocol createrole re return error"));
		}
		if(p->refretcode == 11)
		{
			OutputLinkSevError(143);
			g_pGame->RuntimeDebugInfo(RTDCOL_ERROR, _AL("NET - protocol createrole re return error"));
		}
	}
	m_iReferrer = p->referrer;
	CECLoginUIMan* pLoginUIMan = g_pGame->GetGameRun()->GetUIManager()->GetLoginUIMan();
	ASSERT(pLoginUIMan);
	pLoginUIMan->CreateCharacter(p->roleinfo);
}

void CECGameSession::OnPrtcDeleteRoleRe(GNET::Protocol* pProtocol)
{
	using namespace GNET;

	g_pGame->RuntimeDebugInfo(RTDCOL_NETWORK, _AL("NET - protocol deleterole re"));

	DeleteRole_Re* p = (DeleteRole_Re*)pProtocol;
	if (p->result != ERR_SUCCESS)
	{
		OutputLinkSevError(p->result);
		g_pGame->RuntimeDebugInfo(RTDCOL_ERROR, _AL("NET - protocol deleterole re return error"));
		return;
	}
	
	CECLoginUIMan* pLoginUIMan = g_pGame->GetGameRun()->GetUIManager()->GetLoginUIMan();
	ASSERT(pLoginUIMan);
	pLoginUIMan->DelCharacter(p->result, p->roleid);
}

void CECGameSession::OnPtrcUndoDeleteRoleRe(GNET::Protocol* pProtocol)
{
	using namespace GNET;

	g_pGame->RuntimeDebugInfo(RTDCOL_NETWORK, _AL("NET - protocol undodeleterole re"));

	UndoDeleteRole_Re* p = (UndoDeleteRole_Re*)pProtocol;
	if (p->result != ERR_SUCCESS)
	{
		OutputLinkSevError(p->result);
		g_pGame->RuntimeDebugInfo(RTDCOL_ERROR, _AL("NET - protocol undodeleterole re return error"));
		return;
	}
	
	CECLoginUIMan* pLoginUIMan = g_pGame->GetGameRun()->GetUIManager()->GetLoginUIMan();
	ASSERT(pLoginUIMan);
	pLoginUIMan->RestoreCharacter(p->result, p->roleid);
}

//	Process chat data
void CECGameSession::OnPrtcChatMessage(GNET::Protocol* pProtocol, bool& bAddToOld)
{
	CECGameUIMan* pGameUI = (CECGameUIMan*)g_pGame->GetGameRun()->GetUIManager()->GetInGameUIMan();

	using namespace GNET;
	using namespace CHAT_S2C;

	ChatMessage* p = (ChatMessage*)pProtocol;
	CECIvtrItem* pIvtrItem = NULL;

	ACHAR szMsg[1024];
	ACString strTemp((const ACHAR*)p->msg.begin(), p->msg.size() / sizeof (ACHAR));

	if( pGameUI )
		pGameUI->FilterBadWords(strTemp);
	AUI_ConvertChatString(strTemp, szMsg, false);

	chat_equip_item* pItemInfo = (chat_equip_item*)p->data.begin();
	const size_t sz = sizeof(chat_equip_item) - sizeof(pItemInfo->content);

	if (p->data.size() >= sz && sz + pItemInfo->content_length == p->data.size())
	{
		if (pItemInfo->cmd_id == CHAT_EQUIP_ITEM)
		{
			pIvtrItem = CECIvtrItem::CreateItem(pItemInfo->type, pItemInfo->expire_date, 1);
		
			if (pIvtrItem)
			{
				pIvtrItem->SetItemInfo((unsigned char*)pItemInfo->content, pItemInfo->content_length);
				pIvtrItem->SetStateFlag(pItemInfo->state);
				pIvtrItem->UpdateInfoMD5();
			}
		}
	}

	bAddToOld = true;

	if (p->channel == GP_CHAT_BROADCAST || p->channel == GP_CHAT_SYSTEM ||
		!p->srcroleid)
	{
		if( p->channel == GP_CHAT_SYSTEM && p->srcroleid > 0 )
		{
			vector<DOMAIN_INFO> *info = globaldata_getdomaininfos();
			ACString strMsg = _AL("");
			switch(p->srcroleid)
			{
			case 3:
			{
				char id, result;
				if( p->msg.size() >= 2 * sizeof(char) )
					Marshal::OctetsStream(p->msg) >> id >> result;
				for(DWORD i = 0; i < info->size(); i++ )
					if((*info)[i].id == id )
					{
						strMsg.Format(pGameUI->GetStringFromTable(4000 + p->srcroleid), (*info)[i].name);
						strMsg += pGameUI->GetStringFromTable(3900 + result);
						break;
					}
				break;
			}
			case 4:
			case 5:
			{
				char id;
				if( p->msg.size() >= sizeof(char) )
					Marshal::OctetsStream(p->msg) >> id;
				for(DWORD i = 0; i < info->size(); i++ )
					if((*info)[i].id == id )
					{
						strMsg.Format(pGameUI->GetStringFromTable(4000 + p->srcroleid), (*info)[i].name);
						break;
					}
				break;
			}
			case 6:
			case 7:
			{
				unsigned int amount;
				if( p->msg.size() >= sizeof(unsigned int))
					Marshal::OctetsStream(p->msg) >> amount;
				strMsg.Format(pGameUI->GetStringFromTable(4000 + p->srcroleid), amount);
				break;
			}
			case 8:
			case 9:
			{
				unsigned int amount;
				unsigned char id;
				if( p->msg.size() >= sizeof(unsigned int) + sizeof(unsigned char))
					Marshal::OctetsStream(p->msg) >> id >> amount;
				for(DWORD i = 0; i < info->size(); i++ )
					if((*info)[i].id == id )
					{
						strMsg.Format(pGameUI->GetStringFromTable(4000 + p->srcroleid), (*info)[i].name, amount);
						break;
					}
				break;
			}
			case 10:
			{
				int id1, id2;
				Octets name1, name2;
				try{ 
					Marshal::OctetsStream(p->msg)>>id1>>id2>>name1>>name2; 
					strMsg.Format(pGameUI->GetStringFromTable(4000 + p->srcroleid), 
						ACString((const ACHAR*)name1.begin(), name1.size() / sizeof(ACHAR)), 
						ACString((const ACHAR*)name2.begin(), name2.size() / sizeof(ACHAR)));
				}catch(...) {}
				break;
			}
			case 12:
			case 13:
			case 14:
			case 16:
			{
				int id1, id2;
				Octets name1, name2;
				try{ 
					Marshal::OctetsStream(p->msg)>>id1>>id2>>name1>>name2; 
					if( p->srcroleid == 12 || p->srcroleid == 14 || p->srcroleid == 16 )
					{
						strMsg.Format(pGameUI->GetStringFromTable(4000 + p->srcroleid), 
							ACString((const ACHAR*)name1.begin(), name1.size() / sizeof(ACHAR)), 
							ACString((const ACHAR*)name2.begin(), name2.size() / sizeof(ACHAR)));
					}
					else
					{
						strMsg.Format(pGameUI->GetStringFromTable(4000 + p->srcroleid), 
							ACString((const ACHAR*)name2.begin(), name2.size() / sizeof(ACHAR)), 
							ACString((const ACHAR*)name1.begin(), name1.size() / sizeof(ACHAR)));
					}
				}catch(...) {}
				break;
			}
			case 15:
			{
				int id1, id2;
				int kill1, kill2;
				Octets name1, name2;
				try{ 
					Marshal::OctetsStream(p->msg)>>id1>>id2>>name1>>name2>>kill1>>kill2; 
					strMsg.Format(pGameUI->GetStringFromTable(4000 + p->srcroleid), 
						ACString((const ACHAR*)name1.begin(), name1.size() / sizeof(ACHAR)), 
						ACString((const ACHAR*)name2.begin(), name2.size() / sizeof(ACHAR)),
						ACString((const ACHAR*)name1.begin(), name1.size() / sizeof(ACHAR)), kill1, 
						ACString((const ACHAR*)name2.begin(), name2.size() / sizeof(ACHAR)), kill2);
				}catch(...) {}
				break;
			}
			case 20:
			case 23:
			{
				int id;
				try{ 
					Marshal::OctetsStream(p->msg)>>id; 
					strMsg.Format(pGameUI->GetStringFromTable(4000 + p->srcroleid), id);
					if( p->srcroleid == 23 )
					{
						Faction_Info *pinfo = g_pGame->GetFactionMan()->GetFaction(g_pGame->GetGameRun()->GetHostPlayer()->GetFactionID());
						if( pinfo )
							pinfo->SetNimbus(id);
					}
				}catch(...) {}
				break;
			}
			case 21:
			case 22:
			{
				int id, data;
				try{ 
					Marshal::OctetsStream(p->msg)>>id>>data;
					strMsg.Format(pGameUI->GetStringFromTable(4000 + p->srcroleid), 
						pGameUI->GetStringFromTable(3451 + id), data);
				}catch(...) {}
				break;
			}
			case 24:
			{
				OnTaskChatMessage(p->msg.begin(), p->msg.size());
				delete pIvtrItem;
				return;
			}
			case 25:
			{
				#pragma pack(1)
						struct _data
						{
								unsigned char type;             // 类型 0 第一次击杀 1 杀神 2 阻止杀戮
								unsigned char battle_faction;   // killer的阵营 1 攻方 2 守方
								int battle_id;                  // 战场id
								int gs_id;                      // 线号
								int killer;                     // 杀人玩家id
								int deadman;                    // 被杀玩家id
						};
				#pragma pack()
				if( p->msg.size() >= sizeof(_data))
				{
					_data* pData = (_data*)p->msg.begin();
					ACString strName = g_pGame->GetGameRun()->GetPlayerName(pData->killer, true);
					ACString strLine = pGameUI->GetLine(pData->gs_id);
					ACString strBattle = pGameUI->GetStringFromTable(1900 + pData->battle_id);
					switch(pData->type)
					{
					case 0:
					{
						ACString strName1 = g_pGame->GetGameRun()->GetPlayerName(pData->deadman, true);
						strMsg.Format(pGameUI->GetStringFromTable(4101), strBattle, strLine, strName, strName1);
						break;
					}
					case 1:
					{
						strMsg.Format(pGameUI->GetStringFromTable(4102), strBattle, strLine, strName);
						break;
					}
					case 2:
					{
						ACString strName1 = g_pGame->GetGameRun()->GetPlayerName(pData->deadman, true);
						strMsg.Format(pGameUI->GetStringFromTable(4103), strBattle, strLine, strName, strName1);
						break;
					}
					}
				}
				break;
			}
			case 26:
			{
				int lineid;
				try{ 
					Marshal::OctetsStream(p->msg)>>lineid; 
					ACString strLine = pGameUI->GetLine(lineid);
					strMsg.Format(pGameUI->GetStringFromTable(4000 + p->srcroleid), strLine);
				}catch(...) {}
				break;
			}
			case 27:
			case 28:
			case 29:
			{
				long stime;
				try{ 
					Marshal::OctetsStream(p->msg)>>stime; 
					ACString strTime;
					stime -= g_pGame->GetTimeZoneBias() * 60; // localtime = UTC - bias, in which bias is in minutes
					tm *gtime = gmtime(&stime);
					strTime.Format(pGameUI->GetStringFromTable(781), 
						gtime->tm_mon + 1, gtime->tm_mday, gtime->tm_hour, gtime->tm_min);
					strMsg.Format(pGameUI->GetStringFromTable(4000 + p->srcroleid), strTime);
				}catch(...) {}
				break;
			}
			case 30:
			{
				struct battlefiled_msg_t
				{
						int type;               //消息类型 0 捐献 1 建设 2 购买弓箭手
						int player_id;          //事件发起人
						char player_name[20];	//名字
						int data1;              //type 为 0 时 表示 捐献了什么(物品id)；type 为 1 时表示建设了什么(key从1开始)； type 为 2 时表示是否成功(0/1)
						int data2;              //type 为 0 时 表示 捐献了多少        ；type 为 1 时表示建筑的级别(level从0开始)
						int data3;              //type 为 1 时 表示 类型(从0开始)
				};
				if(p->msg.size() >= sizeof(battlefiled_msg_t))
				{ 
					battlefiled_msg_t* pData = (battlefiled_msg_t*)p->msg.begin();
					ACHAR szName[ 20/sizeof(ACHAR) + 1 ];
					szName[ 20/sizeof(ACHAR) ] = 0;					
					memcpy(szName, pData->player_name, 20);

					ACString strText;
					switch(pData->type) {
					case 0: {
						CECIvtrItem *pItem = CECIvtrItem::CreateItem(pData->data1, 0, 1);
						ACString itemName = pItem ? pItem->GetName() : _AL(" ");
						strText.Format(pGameUI->GetStringFromTable(4105), szName, itemName, pData->data2);
						delete pItem;}
						strMsg.Format(pGameUI->GetStringFromTable(4000 + p->srcroleid), strText);
						break;
					case 1: {
						static const int TOWER_NUM = 7;
						static const int GATE_NUM = 3;
						ACString bldingName;
						bldingName.Format(_AL("%s%d"), 
							pData->data1 <= TOWER_NUM ? pGameUI->GetStringFromTable(4111) : pGameUI->GetStringFromTable(4112), 
							pData->data1 <= TOWER_NUM ? pData->data1: pData->data1 - TOWER_NUM);
						if (pData->data3 == 4) { // 血量("%s 将“%s”的 %s 提升为 %d")
							strText.Format(pGameUI->GetStringFromTable(4109), szName, bldingName, 
								pGameUI->GetStringFromTable(4121 + pData->data3), pData->data2);
						} else { // 其他
							strText.Format(pGameUI->GetStringFromTable(4108), szName, bldingName, 
								pGameUI->GetStringFromTable(4121 + pData->data3), pData->data2+1);
						}

						strMsg.Format(pGameUI->GetStringFromTable(4000 + p->srcroleid), strText);}
						break;
					case 2:
						if (pData->data1) {
							strText.Format(pGameUI->GetStringFromTable(4126), szName);
							strMsg.Format(pGameUI->GetStringFromTable(4000 + p->srcroleid), strText);
						}
						break;						
					default:
						break;
					}
				}
				break;
			}
			case 33:
			{
				int startTime;
				try{
					Marshal::OctetsStream(p->msg)>>startTime;
					ACString strTime;
					strTime.Format(pGameUI->GetStringFromTable(1602), (startTime+59) / 60);
					strMsg.Format(pGameUI->GetStringFromTable(4033), strTime);
				}catch(...) {}
				break;
			}
			case 34:
			{
				strMsg = pGameUI->GetStringFromTable(4034);
				break;
			}
			case 35:
			{
				Octets rolename;
				char msg_type;
				try{
				
					Marshal::OctetsStream(p->msg)>>rolename>>msg_type;
					strMsg.Format(pGameUI->GetStringFromTable(7265 + msg_type), ACString((const ACHAR*)rolename.begin(), rolename.size() / sizeof(ACHAR)));
				}catch(...){ }
				break;
			}
			case 36:
			{
				Octets rolename,apply_rolename;
				char msg_type;
				try
				{
					Marshal::OctetsStream(p->msg)>>rolename>>apply_rolename>>msg_type;
					strMsg.Format(pGameUI->GetStringFromTable(7315 + msg_type), ACString((const ACHAR*)rolename.begin(), rolename.size() / sizeof(ACHAR)), ACString((const ACHAR*)apply_rolename.begin(), apply_rolename.size() / sizeof(ACHAR)));
				}catch(...){}
				break;
			}
			case 37:
			{
				int type;
				try
				{
					Marshal::OctetsStream(p->msg)>>type;
					strMsg = pGameUI->GetStringFromTable(9429+type);
				}catch(...){}
				break;
			}
			case 38:
			{
				strMsg = pGameUI->GetStringFromTable(9434);
				break;
			}
			case 39:
			{
				int terrId, itemId;
				try
				{
					Marshal::OctetsStream(p->msg)>>terrId>>itemId;
					abase::vector<DOMAIN_INFO> *info = globaldata_getdomaininfos2();
					for (UINT i=0; i<(*info).size(); i++)
					{
						if ((*info)[i].id == terrId)
						{
							CECIvtrItem* pItem = CECIvtrItem::CreateItem(itemId, 0, 1);
							if (pItem)
							{
								strMsg.Format(pGameUI->GetStringFromTable(9435), (*info)[i].name, pItem->GetName());
								delete pItem;
							}
						}
					}
				}catch(...){}
				
				break;
			}
			case 40:
			{
				int winItemId;
				try
				{
					Marshal::OctetsStream(p->msg)>>winItemId;
					CECIvtrItem* pItem = CECIvtrItem::CreateItem(winItemId, 0, 1);
					if (pItem)
					{
						strMsg.Format(pGameUI->GetStringFromTable(9880), pItem->GetName());
						delete pItem;
					}
				}catch(...){}
				break;
			}
			case 41:
			{
				strMsg = pGameUI->GetStringFromTable(9881);
				break;
			}
			case 42:
				{
					int winId, price, id;
					Octets name;
					ACString strMoney;
					try{ 
						Marshal::OctetsStream(p->msg)>>winId>>name>>price>>id;
						if (winId==-1)
						{
							strMsg = pGameUI->GetStringFromTable(9879);
						}
						else if (winId==0)
						{
							strMsg = pGameUI->GetStringFromTable(9882);
						}
						else
						{
							pGameUI->FormatMoney(price, strMoney);
							CECIvtrItem* pItem = CECIvtrItem::CreateItem(id, 0, 1);
							if (pItem)
							{
								strMsg.Format(pGameUI->GetStringFromTable(9883), ACString((const ACHAR*)name.begin(), name.size() / sizeof(ACHAR)), 
									strMoney, pItem->GetName());
								delete pItem;
							}
						}
					}catch(...) {}
				break;
				}
			case 43:
				{
					int winItemId;
					try
					{
						Marshal::OctetsStream(p->msg)>>winItemId;
						CECIvtrItem* pItem = CECIvtrItem::CreateItem(winItemId, 0, 1);
						if (pItem)
						{
							strMsg.Format(pGameUI->GetStringFromTable(9878), pItem->GetName());
							delete pItem;
						}
					}catch(...){}
					break;
				}
			case 44:
				{
					int terrId, defenceId, attackerId;
					Octets defenceName, attackerName;
					int result;	// 1 攻方胜 2 守方胜
					int time;	// 0 慢慢结束 1 快速结束
					try
					{
						Marshal::OctetsStream(p->msg)>>terrId>>defenceId>>defenceName>>attackerId>>attackerName>>result>>time;
						
						abase::vector<DOMAIN_INFO> *info = globaldata_getdomaininfos2();
						for (UINT i=0; i<(*info).size(); i++)
						{
							if ((*info)[i].id == terrId)
							{
								int idTxt = 9375;
								idTxt += (result==2 ? 1 : 0);
								idTxt += (time==0 ? 2 : 0);
								idTxt += 4*(3-(*info)[i].type);
								ACString strDefencer, strAttacker;
								strDefencer = defenceId>0 ? ACString((const ACHAR*)defenceName.begin(), defenceName.size() / sizeof(ACHAR)) : pGameUI->GetStringFromTable(9372 - (*info)[i].type);
								strAttacker = attackerId>0 ? ACString((const ACHAR*)attackerName.begin(), attackerName.size() / sizeof(ACHAR)) : pGameUI->GetStringFromTable(9372 - (*info)[i].type);
								if (result==1)
								{
									strMsg.Format(pGameUI->GetStringFromTable(idTxt), strAttacker, strDefencer, (*info)[i].name);
								}
								else
								{
									strMsg.Format(pGameUI->GetStringFromTable(idTxt), strDefencer, strAttacker, (*info)[i].name);
								}
								break;
							}
						}
					}catch(...){}
					break;
				}
			default:
				strMsg = pGameUI->GetStringFromTable(4000 + p->srcroleid);
			}
			if( strMsg != _AL("") )
			{
				if( p->srcroleid == 10 || p->srcroleid == 13 || p->srcroleid == 15 )
					g_pGame->GetGameRun()->AddChatMessage(strMsg, GP_CHAT_BROADCAST, p->srcroleid, NULL, 0);
				else
					g_pGame->GetGameRun()->AddChatMessage(strMsg, GP_CHAT_BROADCAST, p->srcroleid, NULL, 1);
			}
		}
		else
			g_pGame->GetGameRun()->AddChatMessage(szMsg, p->channel, p->srcroleid, NULL, 0, p->emotion);
	}
	else
	{
		CECStringTab* pStrTab = g_pGame->GetFixedMsgTab();

		EC_VIDEO_SETTING vs = g_pGame->GetConfigs()->GetVideoSettings();
		if (ISPLAYERID(p->srcroleid))
		{
			CECHostPlayer *pHost = g_pGame->GetGameRun()->GetHostPlayer();
			EC_LOCAL_SETTING setting = g_pGame->GetConfigs()->GetLocalSettings();
			if ( (pHost->GetFactionID()>0&&(int)p->srcfactionid == pHost->GetFactionID()) ||(pHost->GetMaster()>0&&pHost->GetMaster()==p->srcsectid) ||
				pHost->IsInSociety(p->srcroleid) || 
				!(p->channel == GP_CHAT_LOCAL && setting.local_chat.bFilterLocalChat && p->srcrolelevel>0 && p->srcrolelevel <= setting.local_chat.nLocalChatLevel))
			{
				//	Try to get player's name
				const ACHAR* szName = g_pGame->GetGameRun()->GetPlayerName(p->srcroleid, false);
				if (!szName)
				{
					//	Add protocol to waiting list
					AddIDForBriefInfo(p->srcroleid);
					bAddToOld = false;
				}
				else
				{
					ACHAR szText[80];
					AUI_ConvertChatString(szName, szText);
					
					ACString str;
					ACString strMsg = szMsg;
					ACHAR ch = 0xdbff;
					ACString strMark = ACString(ch,2);
					if((0 == strMsg.Find(_AL(":")+strMark)||0 == strMsg.Find(g_pGame->GetGameRun()->GetUIManager()->GetInGameUIMan()->GetStringFromTable(8493)+strMark)) && 0 <= strMsg.Find(_AL("#N")))
					{
						strMsg = strMsg.Mid(3);
						int iLeft = strMsg.Find(_AL("#N"));
						while (iLeft>=0)
						{
							strMsg.Format(_AL("%s%s%s"), strMsg.Left(iLeft), szText, strMsg.Mid(iLeft+2));
							iLeft = strMsg.Find(_AL("#N"));
						}
						str.Format(_AL("%s%s"),g_pGame->GetGameRun()->GetUIManager()->GetInGameUIMan()->GetStringFromTable(8494),strMsg);
					}
					else
					{
						str.Format(pStrTab->GetWideString(FIXMSG_CHAT), szText, szMsg);
						//	Set player's last said words
						CECPlayer* pPlayer = g_pGame->GetGameRun()->GetWorld()->GetPlayerMan()->GetPlayer(p->srcroleid);
						if (pPlayer && !vs.bPlayerBubble)
						{
							CECIvtrItem* p2 = pIvtrItem ? pIvtrItem->Clone() : 0;
							pPlayer->SetLastSaidWords(strTemp, p->emotion, p2);
						}
					}
					g_pGame->GetGameRun()->AddChatMessage(str, p->channel, p->srcroleid, NULL, 0, p->emotion, pIvtrItem);
					
					pIvtrItem = NULL;
				}
			}
		}
		else if (ISNPCID(p->srcroleid))
		{
			CECNPC* pNPC = g_pGame->GetGameRun()->GetWorld()->GetNPCMan()->GetNPC(p->srcroleid);

			if (pNPC)
			{
				ACString str;
				str.Format(pStrTab->GetWideString(FIXMSG_CHAT2), pNPC->GetName(), szMsg);
				g_pGame->GetGameRun()->AddChatMessage(str, p->channel, p->srcroleid, NULL, 0, 0, pIvtrItem);
				pIvtrItem = 0;
				if (!vs.bPlayerBubble)
					pNPC->SetLastSaidWords(strTemp);
			}
		}
	}

	delete pIvtrItem;
}

void CECGameSession::OnPrtcWorldChat(GNET::Protocol* pProtocol)
{
	using namespace GNET;
	using namespace CHAT_S2C;

	WorldChat* p = (WorldChat*)pProtocol;
	CECIvtrItem* pIvtrItem = NULL;

	ACHAR szMsg[1024];
	ACString strTemp((const ACHAR*)p->msg.begin(), p->msg.size() / sizeof (ACHAR));
	CECGameUIMan* pGameUI = g_pGame->GetGameRun()->GetUIManager()->GetInGameUIMan();
	if( pGameUI )
		pGameUI->FilterBadWords(strTemp);
	AUI_ConvertChatString(strTemp, szMsg, false);

	chat_equip_item* pItemInfo = (chat_equip_item*)p->data.begin();
	const size_t sz = sizeof(chat_equip_item) - sizeof(pItemInfo->content);

	if (p->data.size() >= sz && sz + pItemInfo->content_length == p->data.size())
	{
		if (pItemInfo->cmd_id == CHAT_EQUIP_ITEM)
		{
			pIvtrItem = CECIvtrItem::CreateItem(pItemInfo->type, pItemInfo->expire_date, 1);
		
			if (pIvtrItem)
			{
				pIvtrItem->SetItemInfo((unsigned char*)pItemInfo->content, pItemInfo->content_length);
				pIvtrItem->SetStateFlag(pItemInfo->state);
				pIvtrItem->UpdateInfoMD5();
			}
		}
	}

	if (p->channel == GP_CHAT_FARCRY && p->roleid == 24)
	{
		OnTaskChatMessage(p->msg.begin(), p->msg.size());
		delete pIvtrItem;
		return;
	}

	if (p->channel == GP_CHAT_SYSTEM )
	{
		CECGameUIMan* pGameUI = (CECGameUIMan*)g_pGame->GetGameRun()->GetUIManager()->GetInGameUIMan();
		CDlgGuildMap* pDlgGuildMap = (CDlgGuildMap*)pGameUI->GetDialog("GuildMap");
		
		int idFaction = g_pGame->GetGameRun()->GetHostPlayer()->GetFactionID();
		CECFactionMan *pMan = g_pGame->GetFactionMan();
		vector<DOMAIN_INFO> *info = globaldata_getdomaininfos();
		ACString strName((const ACHAR*)p->name.begin(), p->name.size() / sizeof (ACHAR));
		ACString strMsg = _AL("");
		ACString strAttacker = _AL("");
		ACString strDefender = _AL("");
		switch(p->roleid)
		{
		case 3:
		{
			if( p->msg.size() >= sizeof(unsigned char) + sizeof(unsigned int))
			{
				unsigned char id;
				unsigned int fid;
				Marshal::OctetsStream(p->msg) >> id >> fid;
				Faction_Info *finfo = pMan->GetFaction(fid, false);
				if( !finfo )
					pMan->AddFaction(fid, false, strName, strName.GetLength() * sizeof(ACHAR), 3, 0, 200);

				if( (int)fid == idFaction )
					strAttacker = pGameUI->GetStringFromTable(4500);
				else
					strAttacker.Format(pGameUI->GetStringFromTable(4499), strName);

				for(DWORD i = 0; i < info->size(); i++ )
					if((*info)[i].id == id )
					{
						bool bAttack = false;
//						if( (int)fid == (*info)[i].id_challenger1 )
//							bAttack = true;

						if( pDlgGuildMap )
							if( (*info)[i].id_owner && (*info)[i].id_owner == idFaction ||
								(int)fid == idFaction )
								pDlgGuildMap->m_vecAreaType[i] |= CDlgGuildMap::AREA_TYPE_CANENTER;
						if( (*info)[i].id_owner )
						{
							if((*info)[i].id_owner == idFaction )
								strDefender = pGameUI->GetStringFromTable(4500);
							else
							{
								Faction_Info *finfo = pMan->GetFaction((*info)[i].id_owner, false);
								if( finfo )
									strDefender.Format(pGameUI->GetStringFromTable(4499), 
										finfo->GetName());
							}
						}
						if( bAttack )
							strMsg.Format(pGameUI->GetStringFromTable(4501), strAttacker, strDefender );
						else
							strMsg.Format(pGameUI->GetStringFromTable(4502), strAttacker, strDefender, (*info)[i].name);
						break;
					}
			}
			break;
		}
		case 4:
		{
			if( p->msg.size() >= 2 * sizeof(unsigned int) + 3 * sizeof(unsigned char) )
			{
				unsigned char id, result, color;
				unsigned int amount, fid;
				Marshal::OctetsStream(p->msg) >> id >> result >> amount >> fid >> color;
				Faction_Info *finfo = pMan->GetFaction(fid, false);
				if( !finfo )
					pMan->AddFaction(fid, false, strName, strName.GetLength() * sizeof(ACHAR), 3, 0, 200);

				if( (int)fid == idFaction )
					strAttacker = pGameUI->GetStringFromTable(4500);
				else
					strAttacker.Format(pGameUI->GetStringFromTable(4499), strName);

				for(DWORD i = 0; i < info->size(); i++ )
					if((*info)[i].id == id )
					{
						bool bAttack = false;
//						if( (int)fid == (*info)[i].id_challenger1 )
//							bAttack = true;

						if( (*info)[i].id_owner )
						{
							if((*info)[i].id_owner == idFaction )
								strDefender = pGameUI->GetStringFromTable(4500);
							else
							{
								Faction_Info *finfo = pMan->GetFaction((*info)[i].id_owner, false);
								if( finfo )
									strDefender.Format(pGameUI->GetStringFromTable(4499), 
										finfo->GetName());
							}
						}
						if( result == 4 )
							strMsg.Format(pGameUI->GetStringFromTable(4520), (*info)[i].name );
						else if( !bAttack )
						{
							if( result == 1 )
								strMsg.Format(pGameUI->GetStringFromTable(4503), strAttacker, strDefender, (*info)[i].name);
							else
								strMsg.Format(pGameUI->GetStringFromTable(4504), strAttacker, strDefender, (*info)[i].name);
						}
						else
						{
							if( (*info)[i].id_owner )
							{
								if( result == 1 )
									strMsg.Format(pGameUI->GetStringFromTable(4506), strAttacker, strDefender, (*info)[i].name);
								else 
									strMsg.Format(pGameUI->GetStringFromTable(4507), strAttacker, strDefender, (*info)[i].name);
							}
							else
								strMsg.Format(pGameUI->GetStringFromTable(4505), strAttacker, (*info)[i].name);
						}
						if( bAttack )
						{
//							(*info)[i].id_challenger1 = 0;
//							(*info)[i].battle_time1 = 0;
							if( result == 1 )
							{
								(*info)[i].id_owner = fid;
								(*info)[i].color = color;
							}
						}
						else
						{
//							(*info)[i].id_challenger2 = 0;
//							(*info)[i].battle_time2 = 0;
						}
						if( pDlgGuildMap )
							pDlgGuildMap->UpdateNormalMap();
						break;
					}
			}
			break;
		}
		case 5:
		{
			if( p->msg.size() >= 4 * sizeof(unsigned int) + sizeof(unsigned char) )
			{
				unsigned char id;
				unsigned int fid, amount, loser, cutofftime;
				Marshal::OctetsStream(p->msg) >> id >> fid >> amount >> loser >> cutofftime;
				Faction_Info *finfo = pMan->GetFaction(fid, false);
				if( !finfo )
					pMan->AddFaction(fid, false, strName, strName.GetLength() * sizeof(ACHAR), 3, 0, 200);

				if( (int)fid == idFaction )
					strAttacker = pGameUI->GetStringFromTable(4500);
				else
					strAttacker.Format(pGameUI->GetStringFromTable(4499), strName);

				bool bAttack = false;
				for(DWORD i = 0; i < info->size(); i++ )
					if((*info)[i].id_owner == (int)fid )
					{
						bAttack = true;
						break;
					}
					
				for(i = 0; i < info->size(); i++ )
					if((*info)[i].id == id )
					{
//						if( bAttack )
//							(*info)[i].id_tempchallenger1 = fid;
//						else
//							(*info)[i].id_tempchallenger2 = fid;
						(*info)[i].deposit = amount;
//						(*info)[i].cutoff_time = cutofftime;
						if( pDlgGuildMap )
							pDlgGuildMap->UpdateChallengeMap();

						if( (*info)[i].id_owner )
						{
							if((*info)[i].id_owner == idFaction )
								strDefender = pGameUI->GetStringFromTable(4500);
							else
							{
								Faction_Info *finfo = pMan->GetFaction((*info)[i].id_owner, false);
								if( finfo )
									strDefender.Format(pGameUI->GetStringFromTable(4499), 
										finfo->GetName());
							}
						}
						if( (int)loser == idFaction )
							strMsg.Format(pGameUI->GetStringFromTable(4508), (*info)[i].name, strAttacker );
						else if( strDefender == _AL("") )
							strMsg.Format(pGameUI->GetStringFromTable(4509), strAttacker, (*info)[i].name );
						else
							strMsg.Format(pGameUI->GetStringFromTable(4510), strAttacker, strDefender, (*info)[i].name);

						break;
					}
			}
			break;
		}
		case 24:
		{
			OnTaskChatMessage(p->msg.begin(), p->msg.size());
			delete pIvtrItem;
			return;
		}

		}
		if( strMsg != _AL("") )
			g_pGame->GetGameRun()->AddChatMessage(strMsg, GP_CHAT_BROADCAST, 0, NULL, 1, p->emotion);
	}
	else if (!p->roleid)
	{
		g_pGame->GetGameRun()->AddChatMessage(szMsg, p->channel, p->roleid, NULL, 0, p->emotion);
	}
	else
	{
		CECStringTab* pStrTab = g_pGame->GetFixedMsgTab();

		ACHAR szText[80];
		ACString strName((const ACHAR*)p->name.begin(), p->name.size() / sizeof (ACHAR));
		AUI_ConvertChatString(strName, szText);

		ACString str;
		ACString strMsg = szMsg;
		ACHAR ch = 0xdbff;
		ACString strMark = ACString(ch,2);
		if((0 == strMsg.Find(_AL(":")+strMark)||0 == strMsg.Find(g_pGame->GetGameRun()->GetUIManager()->GetInGameUIMan()->GetStringFromTable(8493)+strMark)) && 0 <= strMsg.Find(_AL("#N")))
		{
			strMsg = strMsg.Mid(3);
			int iLeft = strMsg.Find(_AL("#N"));
			while (iLeft>=0)
			{
				strMsg.Format(_AL("%s%s%s"), strMsg.Left(iLeft), szText, strMsg.Mid(iLeft+2));
				iLeft = strMsg.Find(_AL("#N"));
			}
			str.Format(_AL("%s%s"),g_pGame->GetGameRun()->GetUIManager()->GetInGameUIMan()->GetStringFromTable(8494),strMsg);
		}
		else
		{
			str.Format(pStrTab->GetWideString(FIXMSG_CHAT), szText, szMsg);
			if (ISPLAYERID(p->roleid))
				g_pGame->GetGameRun()->AddPlayerName(p->roleid, strName);
		}
		g_pGame->GetGameRun()->AddChatMessage(str, p->channel, p->roleid, NULL, 0, p->emotion, pIvtrItem);
		pIvtrItem = 0;

	}

	delete pIvtrItem;
}

void CECGameSession::OnPrtcErrorInfo(GNET::Protocol* pProtocol)
{
	using namespace GNET;
	ErrorInfo* p = (ErrorInfo*)pProtocol;
	OutputLinkSevError(p->errcode);
*/
/*	if( p->errcode == ERR_INVALID_PASSWORD ||
		p->errcode == ERR_COMMUNICATION ||
		p->errcode == ERR_ACCOUNTLOCKED ||
		p->errcode == ERR_MULTILOGIN ||
		p->errcode == ERR_LINKISFULL)
*//*	{
		m_bConnected = false;
		l_bExitConnect = true;
		m_pNetMan->Disconnect();
	}
}

void CECGameSession::OnPrtcPlayerLogout(GNET::Protocol* pProtocol)
{
	using namespace GNET;
	PlayerLogout* p = (PlayerLogout*)pProtocol;

	m_CmdCache.RemoveAllCmds();
	
	int iFlag;
	switch (p->result)
	{
	case _PLAYER_LOGOUT_FULL:	iFlag = 0;	break;
	case _PLAYER_LOGOUT_HALF:	iFlag = 1;	break;
	default: 					iFlag = 2;	break;
	}

	g_pGame->GetGameRun()->SetLogoutFlag(iFlag);
}

void CECGameSession::OnPrtcPrivateChat(GNET::Protocol* pProtocol)
{
	using namespace GNET;
	using namespace CHAT_S2C;

	PrivateChat* p = (PrivateChat*)pProtocol;
	ACString strMsg((const ACHAR*)p->msg.begin(), p->msg.size() / sizeof (ACHAR));
	ACString strSrcName((const ACHAR*)p->src_name.begin(), p->src_name.size() / sizeof (ACHAR));

	EC_LOCAL_SETTING setting = g_pGame->GetConfigs()->GetLocalSettings();
	// 屏蔽低级别玩家信息
	
	CECHostPlayer *pHost = g_pGame->GetGameRun()->GetHostPlayer();
	static int lvIndex[] = {1, 5, 10, 20, 30, 40, 50, 60};
	if (!((pHost->GetFactionID()>0&&(int)p->srcfactionid == pHost->GetFactionID()) ||(pHost->GetMaster()>0&&pHost->GetMaster()==p->srcsectid) ||
		pHost->IsInSociety(p->srcroleid)))
	{
		if (setting.local_chat.bFilterWhisper && p->srcrolelevel>0 && p->srcrolelevel <= lvIndex[setting.local_chat.nWhisperLevel])
		{
			SendPrivateChatData(strSrcName, g_pGame->GetGameRun()->GetUIManager()->GetInGameUIMan()->GetStringFromTable(1056));
			return;
		}
	}
	CECIvtrItem* pIvtrItem = NULL;
	ASSERT(p->dstroleid == m_iCharID);


	CECGameUIMan* pGameUI = g_pGame->GetGameRun()->GetUIManager()->GetInGameUIMan();
	if( pGameUI )
		pGameUI->FilterBadWords(strMsg);

	chat_equip_item* pItemInfo = (chat_equip_item*)p->data.begin();
	const size_t sz = sizeof(chat_equip_item) - sizeof(pItemInfo->content);

	if (p->data.size() >= sz && sz + pItemInfo->content_length == p->data.size())
	{
		if (pItemInfo->cmd_id == CHAT_EQUIP_ITEM)
		{
			pIvtrItem = CECIvtrItem::CreateItem(pItemInfo->type, pItemInfo->expire_date, 1);
		
			if (pIvtrItem)
			{
				pIvtrItem->SetItemInfo((unsigned char*)pItemInfo->content, pItemInfo->content_length);
				pIvtrItem->SetStateFlag(pItemInfo->state);
				pIvtrItem->UpdateInfoMD5();
			}
		}
	}

	if (p->channel == CHANNEL_NORMAL || p->channel == CHANNEL_NORMALRE)
	{
		CECStringTab* pStrTab = g_pGame->GetFixedMsgTab();

		ACString str;
		ACString strTemp;
		ACHAR szText[80], szText1[1024];
		AUI_ConvertChatString(strSrcName, szText);
		AUI_ConvertChatString(strMsg, szText1, false);
		
		ACHAR ch = 0xdbff;
		ACString strMark = ACString(ch,2);
		if((0 == strMsg.Find(_AL(":")+strMark)||0 == strMsg.Find(g_pGame->GetGameRun()->GetUIManager()->GetInGameUIMan()->GetStringFromTable(8493)+strMark)) && 0 <= strMsg.Find(_AL("#N")))
		{
			strMsg = strMsg.Mid(3);
			int iLeft = strMsg.Find(_AL("#N"));
			while (iLeft>=0)
			{
				strMsg[iLeft] = _AL('%');
				strMsg[iLeft+1] = _AL('s');
				strTemp = strMsg;
				strMsg.Format(strTemp, strSrcName);
				iLeft = strMsg.Find(_AL("#N"));
			}
			iLeft = strMsg.Find(_AL("#n"));
			while (iLeft>=0)
			{
				strMsg[iLeft] = _AL('%');
				strMsg[iLeft+1] = _AL('s');
				strTemp = strMsg;
				strMsg.Format(strTemp, g_pGame->GetGameRun()->GetHostPlayer()->GetName());
				iLeft = strMsg.Find(_AL("#N"));
			}
			str = g_pGame->GetGameRun()->GetUIManager()->GetInGameUIMan()->GetStringFromTable(8494) + strMsg;
		}
		else
			str.Format(pStrTab->GetWideString(FIXMSG_PRIVATECHAT1), szText, szText1);
		g_pGame->GetGameRun()->AddChatMessage(str, GP_CHAT_WHISPER, p->srcroleid, strSrcName, p->channel, p->emotion, pIvtrItem);
	}
	else
	{
		g_pGame->GetGameRun()->AddChatMessage(strMsg, GP_CHAT_WHISPER, p->srcroleid, strSrcName, p->channel, p->emotion, pIvtrItem);
	}

	//	Save player name.
	g_pGame->GetGameRun()->AddPlayerName(p->srcroleid, strSrcName);
}

void CECGameSession::OnPrtcPlayerBaseInfoRe(GNET::Protocol* pProtocol)
{
	g_pGame->GetGameRun()->PostMessage(MSG_PM_PLAYERBASEINFO, MAN_PLAYER, -1, (DWORD)pProtocol);
}

void CECGameSession::OnPrtcPlayerBaseInfo2Re(GNET::Protocol* pProtocol)
{
	g_pGame->GetGameRun()->PostMessage(MSG_PM_PLAYERBASEINFO2, MAN_PLAYER, -1, (DWORD)pProtocol);
}

void CECGameSession::OnPrtcGetConfigRe(GNET::Protocol* pProtocol)
{
	using namespace GNET;
	GetUIConfig_Re* p = (GetUIConfig_Re*)pProtocol;
	if( p->result != ERR_SUCCESS )
		a_LogOutput(1, "CECGameSession::OnPrtcGetConfigRe, link return error code of %d", p->result);
	else
	{
		g_pGame->GetGameRun()->LoadConfigsFromServer(p->ui_config.begin(), p->ui_config.size());

		//	Now, Get config data request is sent after all host initial data ready. 
		//		so when we receive this reply, we can do some last work before game
		//		really starts. Maybe it's not the best place to do these work, but
		//		now we do it here.
		//	Enalbe game UI
		CECGameUIMan* pGameUI = (CECGameUIMan*)g_pGame->GetGameRun()->GetUIManager()->GetInGameUIMan();
		if (pGameUI)
			pGameUI->EnableUI(true);
	
		CECHostPlayer* pHost = g_pGame->GetGameRun()->GetHostPlayer();
		BYTE pvp_mask = pHost->BuildPVPMask(false);
		c2s_CmdUpatePVPMask(pvp_mask);
		pHost->OnAllInitDataReady();

		if(pHost->GetCutscene())
			pHost->GetCutscene()->InitMaskData();
		
		//for test
		if(pHost->GetProfession() <= 30)	//人族
			pHost->StartCutscene(4);
		else if(pHost->GetProfession() >= 32 && pHost->GetProfession() <= 55)	// 神族
			pHost->StartCutscene(6);
	}
}

void CECGameSession::OnPrtcSetConfigRe(GNET::Protocol* pProtocol)
{
	using namespace GNET;
	SetUIConfig_Re* p = (SetUIConfig_Re*)pProtocol;
	if (p->result != ERR_SUCCESS)
		a_LogOutput(1, "CECGameSession::OnPrtcSetConfigRe, link return error code of %d", p->result);
}

void CECGameSession::OnPrtcGetHelpStatesRe(GNET::Protocol* pProtocol)
{
	using namespace GNET;
	GetHelpStates_Re* p = (GetHelpStates_Re*)pProtocol;

	if( p->result != ERR_SUCCESS )
		a_LogOutput(1, "CECGameSession::OnPrtcGetHelpStatesRe, link return error code of %d", p->result);
	else
	{
		// set the data to the object that needs them.
// 		if( m_pfnHelpStatesCallBack )
// 		{	
// 			(*m_pfnHelpStatesCallBack)(p->help_states.begin(), p->help_states.size());
// 			m_pfnHelpStatesCallBack = NULL;
// 		}
		g_pGame->GetGameRun()->GetSecretary()->LoadStatesFromServer(p->help_states.begin(), p->help_states.size());
	}
}

void CECGameSession::OnPrtcSetHelpStatesRe(GNET::Protocol* pProtocol)
{
	using namespace GNET;
	SetHelpStates_Re* p = (SetHelpStates_Re*)pProtocol;
	if (p->result != ERR_SUCCESS)
		a_LogOutput(1, "CECGameSession::OnPrtcSetHelpStatesRe, link return error code of %d", p->result);
}

void CECGameSession::OnPrtcSetCustomDataRe(GNET::Protocol* pProtocol)
{
	using namespace GNET;
	SetCustomData_Re* p = (SetCustomData_Re*)pProtocol;
	CECUIManager* pUIMan = g_pGame->GetGameRun()->GetUIManager();
	int gs = g_pGame->GetGameRun()->GetGameState();

	if (p->result == ERR_SUCCESS)
	{
		if (gs == CECGameRun::GS_LOGIN)
		{
			pUIMan->GetLoginUIMan()->ChangeCustomizeData(p->roleid);
		}
		else if (gs == CECGameRun::GS_GAME)
		{
		//	CECHostPlayer* pHost = g_pGame->GetGameRun()->GetHostPlayer();
		//	if (pHost && pHost->IsAllResReady())
		//	{
		//	}
		}
	}
	else
	{
		if (gs == CECGameRun::GS_GAME)
		{
			g_pGame->GetGameRun()->PostMessage(MSG_HST_CHANGEFACE, MAN_PLAYER, 0, (DWORD)pProtocol, S2C::PROTOCOL_COMMAND, PROTOCOL_SETCUSTOMDATA_RE);
		}

		pUIMan->ShowErrorMsg(g_pGame->GetFixedMsgTab()->GetWideString(FIXMSG_WRONGCUSTOMDATA));
		a_LogOutput(1, "CECGameSession::OnPrtcSetCustomDataRe, Failed to set custom data");
	}
}

void CECGameSession::OnPrtcGetCustomDataRe(GNET::Protocol* pProtocol)
{
	g_pGame->GetGameRun()->PostMessage(MSG_PM_PLAYERCUSTOM, MAN_PLAYER, -1, (DWORD)pProtocol);
}

void CECGameSession::OnPrtcGetPlayerIDByNameRe(GNET::Protocol* pProtocol)
{
	using namespace GNET;
	GetPlayerIDByName_Re* p = (GetPlayerIDByName_Re*)pProtocol;

	if (p->retcode != ERR_SUCCESS)
	{
		if( p->reason != 3)
			g_pGame->GetGameRun()->AddFixedMessage(FIXMSG_WRONGROLE);
		else
		{
			CECGameUIMan* pGameUI = (CECGameUIMan*)g_pGame->GetGameRun()->GetUIManager()->GetInGameUIMan();
			if (pGameUI)
			{
				CDlgMailWrite *pDlg = ((CDlgMailWrite *)pGameUI->GetDialog("Win_MailWrite"));
				if( pDlg )
					pDlg->SendMail(-1);
			}
		}
		return;
	}

	switch (p->reason)
	{
	case 0:		//	no special reason, just get it

		break;

	case 1:		//	for invite a player who is very far from us to a team

		c2s_CmdTeamInvite(p->roleid);
		break;
		
	case 2:		//	for invite a player who is very far from us to family
	{
		CECGameUIMan* pGameUI = (CECGameUIMan*)g_pGame->GetGameRun()->GetUIManager()->GetInGameUIMan();
		if (pGameUI)
			pGameUI->AddChatMessage(pGameUI->GetStringFromTable(858), GP_CHAT_MISC);
		faction_accept_join(p->roleid, 1);
		break;
	}

	case 3:		//	for send mail to other players
	{
		CECGameUIMan* pGameUI = (CECGameUIMan*)g_pGame->GetGameRun()->GetUIManager()->GetInGameUIMan();
		if (pGameUI)
		{
			CDlgMailWrite *pDlg = ((CDlgMailWrite *)pGameUI->GetDialog("Win_MailWrite"));
			if( pDlg )
				pDlg->SendMail(p->roleid);
		}
		break;
	}

	case 4:		//	for invite a player who is very far from us to faction
	{
		CECGameUIMan* pGameUI = (CECGameUIMan*)g_pGame->GetGameRun()->GetUIManager()->GetInGameUIMan();
		if (pGameUI)
			pGameUI->AddChatMessage(pGameUI->GetStringFromTable(601), GP_CHAT_MISC);
		faction_accept_join(p->roleid, 0);
		break;
	}
	case 5:		//	for invite a player who is very far from us to sect
	{
		CECGameUIMan* pGameUI = (CECGameUIMan*)g_pGame->GetGameRun()->GetUIManager()->GetInGameUIMan();
		if (pGameUI)
			pGameUI->AddChatMessage(pGameUI->GetStringFromTable(999), GP_CHAT_MISC);
		sect_recruit(p->roleid);
		break;
	}

	}

	//	Save ID and name pair. Name-id pair got this time always be right,
	//	so we force to overwrite old pair
	ACString strName((const ACHAR*)p->rolename.begin(), p->rolename.size() / sizeof (ACHAR));
	g_pGame->GetGameRun()->AddPlayerName(p->roleid, strName, true);
}

void CECGameSession::OnPrtcRoleStatusAnnounce(GNET::Protocol* pProtocol)
{
	using namespace GNET;
	RoleStatusAnnounce* p = (RoleStatusAnnounce*)pProtocol;

	if (p->status == _STATUS_OFFLINE)
	{
		ACHAR szName[80];
		ACString strTemp((const ACHAR*)p->rolename.begin(), p->rolename.size() / sizeof (ACHAR));
		AUI_ConvertChatString(strTemp, szName);

		CECStringTab* pStrTab = g_pGame->GetFixedMsgTab();
		g_pGame->GetGameRun()->AddFixedMessage(FIXMSG_OFFLINE, szName);
	}
}

void CECGameSession::OnPrtcAnnounceForbidInfo(GNET::Protocol* pProtocol)
{
	using namespace GNET;
	AnnounceForbidInfo* p = (AnnounceForbidInfo*)pProtocol;
	const GRoleForbid& rf = p->forbid;
	CECStringTab* pStrTab = g_pGame->GetFixedMsgTab();

	if (rf.type == Forbid::FBD_FORBID_SELLPTS)
	{
		//	Game points locking notify
		g_pGame->GetGameRun()->LockGamePoints(rf.time ? true : false);
		return;
	}

	ACString str1, str2, str3, str;

	//	Forbid type
	switch (rf.type)
	{
	case Forbid::FBD_FORBID_LOGIN:	str1 = pStrTab->GetWideString(FIXMSG_FORBIDLOGIN);	break;
	case Forbid::FBD_FORBID_TALK:	str1 = pStrTab->GetWideString(FIXMSG_FORBIDTALK);	break;
	case Forbid::FBD_FORBID_TRADE:	str1 = pStrTab->GetWideString(FIXMSG_FORBIDTRADE);	break;
	case Forbid::FBD_FORBID_SELL:	str1 = pStrTab->GetWideString(FIXMSG_FORBIDSELL);	break;
	}

	//	Forbid reason
	if (rf.reason.size())
	{
		AUICTranslate trans;
		str2 = ACString((const ACHAR*)rf.reason.begin(), rf.reason.size() / sizeof (ACHAR));
		str = trans.Translate(str2);
		str2.Format(pStrTab->GetWideString(FIXMSG_FORBIDREASON), str);
	}
	else
		str2 = pStrTab->GetWideString(FIXMSG_FORBIDREASON);

	//	Forbid time
	str3.Format(pStrTab->GetWideString(FIXMSG_FORBIDTIME), (rf.time + 59) / 60);

	str = str1 + _AL("\r") + str2 + _AL("\r") + str3;

	CECUIManager* pUIMan = g_pGame->GetGameRun()->GetUIManager();
	if (pUIMan)
		pUIMan->ShowErrorMsg(str);
}

void CECGameSession::OnPrtcUpdateRemainTime(GNET::Protocol* pProtocol)
{
	using namespace GNET;
	UpdateRemainTime* p = (UpdateRemainTime*)pProtocol;
	m_iiAccountTime		= ((INT64)p->remain_time) * 1000;
	m_iiFreeGameTime	= ((INT64)p->free_time_left) * 1000;
	m_iFreeGameEnd		= p->free_time_end;
	m_dwLastAccTime		= a_GetTime();
}

void CECGameSession::OnPrtcSubmitProblemToGMRe(GNET::Protocol* pProtocol)
{
	using namespace GNET;
	Report2GM_Re* p = (Report2GM_Re*)pProtocol;

	if (p->retcode == ERR_GENERAL)
	{
		g_pGame->GetGameRun()->AddFixedMessage(FIXMSG_MSGSENDWAIT);
	}
	else if (p->retcode == ERR_COMMUNICATION)
	{
		g_pGame->GetGameRun()->AddFixedMessage(FIXMSG_MSGSENDFAIL);
	}
}

void CECGameSession::OnPrtcImpeachPlayerToGMRe(GNET::Protocol* pProtocol)
{
	using namespace GNET;
	Complain2GM_Re* p = (Complain2GM_Re*)pProtocol;

	if (p->retcode == ERR_GENERAL)
	{
		g_pGame->GetGameRun()->AddFixedMessage(FIXMSG_MSGSENDWAIT);
	}
	else if (p->retcode == ERR_COMMUNICATION)
	{
		g_pGame->GetGameRun()->AddFixedMessage(FIXMSG_MSGSENDFAIL);
	}
}

void CECGameSession::OnPrtcMatrixChallenge(GNET::Protocol* pProtocol)
{
	CECLoginUIMan* pLoginUIMan = g_pGame->GetGameRun()->GetUIManager()->GetLoginUIMan();
	if( pLoginUIMan )
		pLoginUIMan->LaunchMatrixChallenge(pProtocol);
}

void CECGameSession::OnPrtcCashOpt(GNET::Protocol* pProtocol)
{
	CECGameUIMan* pGameUIMan = g_pGame->GetGameRun()->GetUIManager()->GetInGameUIMan();
	if( pProtocol->GetType() == PROTOCOL_CASHLOCK_RE )
	{
		CashLock_Re* p = (CashLock_Re*)pProtocol;
		if( p->retcode == ERR_SUCCESS )
		{
			pGameUIMan->GetDialog("Win_GoldPwdInput")->Show(false);
			((CDlgGoldTrade*)pGameUIMan->GetDialog("Win_GoldTrade"))->SetLocked(false);
		}
		else
		{
			pGameUIMan->MessageBox("", pGameUIMan->GetStringFromTable(895), MB_OK, A3DCOLORRGBA(255, 255, 255, 160));
			pGameUIMan->GetDialog("Win_SoftkbGame")->Show(true);
		}
	}
	else if( pProtocol->GetType() == PROTOCOL_CASHPASSWORDSET_RE )
	{
		CashPasswordSet_Re* p = (CashPasswordSet_Re*)pProtocol;
		if( p->retcode == ERR_SUCCESS )
		{
			pGameUIMan->GetDialog("Win_GoldPwdChange")->Show(false);
		}
		else if( p->retcode == ERR_STOCK_CASHLOCKED )
		{
			pGameUIMan->MessageBox("", pGameUIMan->GetStringFromTable(897), MB_OK, A3DCOLORRGBA(255, 255, 255, 160));
		}
		else
		{
			pGameUIMan->MessageBox("", pGameUIMan->GetStringFromTable(896), MB_OK, A3DCOLORRGBA(255, 255, 255, 160));
		}
	}
}

void CECGameSession::c2s_CmdBindPlayerInviteRe(int idWho, int iReply)
{
	if(iReply == 0) // 同意
	{
		const ACHAR* pName = g_pGame->GetGameRun()->GetPlayerName(idWho,false);
		g_pGame->GetGameRun()->GetHostPlayer()->GetFriendMan()->AddTempFriend(idWho, pName);
	}
	::c2s_SendCmdBindPlayerInviteRe(idWho, iReply); 
}

void CECGameSession::c2s_CmdMultiBindPlayerInviteRe(int idWho, int iReply)
{	
	if(iReply ==0) // 同意
	{
		const ACHAR* pName = g_pGame->GetGameRun()->GetPlayerName(idWho,false);
		g_pGame->GetGameRun()->GetHostPlayer()->GetFriendMan()->AddTempFriend(idWho, pName);
	}
	::c2s_SendCmdMultiBindPlayerInviteRe(idWho, iReply); 
}

// 组队邀请，如果在竞技场中，屏蔽组队功能
void CECGameSession::c2s_CmdTeamInvite(int idPlayer)
{
	CECHostPlayer* pHost = g_pGame->GetGameRun()->GetHostPlayer();
	if(pHost)
	{
		if(pHost->GetBattleType() == BATTLE_TYPE_ARENA || pHost->GetBattleType() == BATTLE_TYPE_ARENAREBORN || pHost->GetBattleType() == BATTLE_TYPE_CRSSVR)
			return;
	}
	LuaEvent::FirstTeam();
	::c2s_SendCmdTeamInvite(idPlayer); 
}

void CECGameSession::c2s_CmdMoveIvtrItem(int iSrc, int iDest, int iAmount)
{
	FreezeHostItem(IVTRTYPE_PACK, iSrc, true);
	FreezeHostItem(IVTRTYPE_PACK, iDest, true);
	::c2s_SendCmdMoveIvtrItem(iSrc, iDest, iAmount); 
}

void CECGameSession::c2s_CmdExgIvtrItem(int iIndex1, int iIndex2)
{ 
	FreezeHostItem(IVTRTYPE_PACK, iIndex1, true);
	FreezeHostItem(IVTRTYPE_PACK, iIndex2, true);
	::c2s_SendCmdExgIvtrItem(iIndex1, iIndex2); 
}

void CECGameSession::c2s_CmdDropIvtrItem(int iIndex, int iAmount)
{ 
	FreezeHostItem(IVTRTYPE_PACK, iIndex, true);
	::c2s_SendCmdDropIvtrItem(iIndex, iAmount); 
}

void CECGameSession::c2s_CmdDropEquipItem(int iIndex)
{ 
	FreezeHostItem(IVTRTYPE_EQUIPPACK, iIndex, true);
	::c2s_SendCmdDropEquipItem(iIndex); 
}

void CECGameSession::c2s_CmdExgEquipItem(int iIndex1, int iIndex2)
{ 
	FreezeHostItem(IVTRTYPE_EQUIPPACK, iIndex1, true);
	FreezeHostItem(IVTRTYPE_EQUIPPACK, iIndex2, true);
	::c2s_SendCmdExgEquipItem(iIndex1, iIndex2); 
}

void CECGameSession::c2s_CmdEquipItem(int iIvtrIdx, int iEquipIdx)
{ 
	FreezeHostItem(IVTRTYPE_PACK, iIvtrIdx, true);
	FreezeHostItem(IVTRTYPE_EQUIPPACK, iEquipIdx, true);
	::c2s_SendCmdEquipItem(iIvtrIdx, iEquipIdx); 
}

void CECGameSession::c2s_CmdMoveItemToEquip(int iIvtrIdx, int iEquipIdx)
{ 
	FreezeHostItem(IVTRTYPE_PACK, iIvtrIdx, true);
	FreezeHostItem(IVTRTYPE_EQUIPPACK, iEquipIdx, true);
	::c2s_SendCmdMoveItemToEquip(iIvtrIdx, iEquipIdx); 
}

void CECGameSession::c2s_CmdExgTrashBoxItem(bool bFaction, int iIndex1, int iIndex2)
{ 
	int pack = bFaction ? IVTRTYPE_FACTIONTRASHBOX : IVTRTYPE_TRASHBOX;
	FreezeHostItem(pack, iIndex1, true);
	FreezeHostItem(pack, iIndex2, true);
	::c2s_SendCmdExgTrashBoxItem(iIndex1, iIndex2); 
}

void CECGameSession::c2s_CmdMoveTrashBoxItem(bool bFaction, int iSrc, int iDst, int iAmount)
{ 
	int pack = bFaction ? IVTRTYPE_FACTIONTRASHBOX : IVTRTYPE_TRASHBOX;
	FreezeHostItem(pack, iSrc, true);
	FreezeHostItem(pack, iDst, true);
	::c2s_SendCmdMoveTrashBoxItem(iSrc, iDst, iAmount); 
}

void CECGameSession::c2s_CmdExgTrashBoxIvtrItem(bool bFaction, int iTrashIdx, int iIvtrIdx)
{ 
	int pack = bFaction ? IVTRTYPE_FACTIONTRASHBOX : IVTRTYPE_TRASHBOX;
	FreezeHostItem(IVTRTYPE_PACK, iIvtrIdx, true);
	FreezeHostItem(pack, iTrashIdx, true);
	::c2s_SendCmdExgTrashBoxIvtrItem(iTrashIdx, iIvtrIdx); 
}

void CECGameSession::c2s_CmdMoveTrashBoxToIvtr(bool bFaction, int iTrashIdx, int iIvtrIdx, int iAmount)
{ 
	int pack = bFaction ? IVTRTYPE_FACTIONTRASHBOX : IVTRTYPE_TRASHBOX;
	FreezeHostItem(IVTRTYPE_PACK, iIvtrIdx, true);
	FreezeHostItem(pack, iTrashIdx, true);
	::c2s_SendCmdMoveTrashBoxToIvtr(iTrashIdx, iIvtrIdx, iAmount); 
}

void CECGameSession::c2s_CmdMoveIvtrToTrashBox(bool bFaction, int iTrashIdx, int iIvtrIdx, int iAmount)
{ 
	int pack = bFaction ? IVTRTYPE_FACTIONTRASHBOX : IVTRTYPE_TRASHBOX;
	FreezeHostItem(IVTRTYPE_PACK, iIvtrIdx, true);
	FreezeHostItem(pack, iTrashIdx, true);
	::c2s_SendCmdMoveIvtrToTrashBox(iTrashIdx, iIvtrIdx, iAmount); 
}

// 新增随身包裹
void CECGameSession::c2s_CmdExgPocketItem(int iIndex1, int iIndex2)
{
	FreezeHostItem(IVTRTYPE_POCKETPACK, iIndex1, true);
	FreezeHostItem(IVTRTYPE_POCKETPACK, iIndex2, true);
	::c2s_SendCmdExgPocketItem(iIndex1, iIndex2);
}

void CECGameSession::c2s_CmdExgPocketIvtrItem(int iPocIdx, int iIvtrIdx)
{
	FreezeHostItem(IVTRTYPE_POCKETPACK, iPocIdx, true);
	FreezeHostItem(IVTRTYPE_PACK, iIvtrIdx, true);
	::c2s_SendCmdExgPocketIvtrItem(iPocIdx, iIvtrIdx);
}

void CECGameSession::c2s_CmdMovePocketItem(int iSrc, int iDst, int iAmount)
{
	FreezeHostItem(IVTRTYPE_POCKETPACK, iSrc, true);
	FreezeHostItem(IVTRTYPE_POCKETPACK, iDst, true);
	::c2s_SendCmdMovePocketItem(iSrc, iDst, iAmount);
}

void CECGameSession::c2s_CmdMovePocketToIvtr(int iPocIdx, int iIvtrIdx, int iAmount)
{
	FreezeHostItem(IVTRTYPE_POCKETPACK, iPocIdx, true);
	FreezeHostItem(IVTRTYPE_PACK, iIvtrIdx, true);
	::c2s_SendCmdMovePocketToIvtr(iPocIdx, iIvtrIdx, iAmount);
}
void CECGameSession::c2s_CmdMoveIvtrToPocket(int iPocIdx, int iIvtrIdx, int iAmount)
{
	FreezeHostItem(IVTRTYPE_POCKETPACK, iPocIdx, true);
	FreezeHostItem(IVTRTYPE_PACK, iIvtrIdx, true);
	::c2s_SendCmdMoveIvtrToPocket(iPocIdx, iIvtrIdx, iAmount);
}

void CECGameSession::c2s_CmdMovePocketAlltoIvtr()
{
//	CECHostPlayer* pHost = g_pGame->GetGameRun()->GetHostPlayer();
// 	for (int i=0; i< pHost->GetPack(IVTRTYPE_POCKETPACK)->GetSize(); ++i)
	::c2s_SendCmdMovePocketAllToIvtr();
}

void CECGameSession::c2s_CmdUpdateFashionHotkey()
{
	int i;
	C2S::cmd_update_fashion_hotkey::fashion_hotkey aEntry[CECHostPlayer::FASHION_HOTKEY_NUM];
	for(i=0; i< CECHostPlayer::FASHION_HOTKEY_NUM; ++i)
	{
		CECHostPlayer::FASHION_HOTKEY& hotkey = g_pGame->GetGameRun()->GetHostPlayer()->GetFashionHotkey(i);
		aEntry[i].index = i;
		aEntry[i].id_head = hotkey.id_head;
		aEntry[i].id_cloth = hotkey.id_body;
		aEntry[i].id_shoe = hotkey.id_shoe;
	}
	::c2s_SendCmdUpdateFashionHotkey(CECHostPlayer::FASHION_HOTKEY_NUM, aEntry);
}

void CECGameSession::c2s_CmdExgFashionItem(int iIndex1, int iIndex2)
{
	FreezeHostItem(IVTRTYPE_FASHION, iIndex1, true);
	FreezeHostItem(IVTRTYPE_FASHION, iIndex1, true);
	::c2s_SendCmdExgFashionItem(iIndex1, iIndex2);
}

void CECGameSession::c2s_CmdExgIvtrFashionItem(int iIvtrIdx, int iFasIdx)
{
	FreezeHostItem(IVTRTYPE_PACK, iIvtrIdx, true);
	FreezeHostItem(IVTRTYPE_FASHION, iFasIdx, true);
	::c2s_SendCmdExgIvtrFashionItem(iIvtrIdx, iFasIdx);
}

void CECGameSession::c2s_CmdExgEquFashionItem(int iEquIdx, int iFasIdx)
{
	FreezeHostItem(IVTRTYPE_EQUIPPACK, iEquIdx, true);
	FreezeHostItem(IVTRTYPE_FASHION, iFasIdx, true);
	::c2s_SendCmdExgEquFashionItem(iEquIdx, iFasIdx);
}

void CECGameSession::c2s_CmdHotkeyChangeFashion(int iKeyIdx)
{
	CECInventory *pFashionPack = g_pGame->GetGameRun()->GetHostPlayer()->GetFashionPack();
	int idx1, idx2, idx3;
	idx1 = idx2 = idx3 = 0xff;
	CECHostPlayer::FASHION_HOTKEY& hotkey = g_pGame->GetGameRun()->GetHostPlayer()->GetFashionHotkey(iKeyIdx);
	if(hotkey.id_head)
	{
		idx1 = pFashionPack->FindItem(hotkey.id_head);
		FreezeHostItem(IVTRTYPE_FASHION, idx1, true);
	}
	if(hotkey.id_body)
	{
		idx2 = pFashionPack->FindItem(hotkey.id_body);
		FreezeHostItem(IVTRTYPE_FASHION, idx2, true);
	}
	if(hotkey.id_shoe)
	{
		idx3 = pFashionPack->FindItem(hotkey.id_shoe);
		FreezeHostItem(IVTRTYPE_FASHION, idx3, true);
	}
	FreezeHostItem(IVTRTYPE_EQUIPPACK, EQUIPIVTR_FASHION_HEAD, true);
	FreezeHostItem(IVTRTYPE_EQUIPPACK, EQUIPIVTR_FASHION_BODY, true);
	FreezeHostItem(IVTRTYPE_EQUIPPACK, EQUIPIVTR_FASHION_FOOT, true);

	::c2s_SendCmdHotkeyChangeFashion(iKeyIdx, idx1, idx2, idx3);
}

void CECGameSession::c2s_CmdChargeEquipFlySword(int iEleIdx, int iCount)
{ 
//	FreezeHostItem(IVTRTYPE_PACK, iEleIdx, true);
//	FreezeHostItem(IVTRTYPE_EQUIPPACK, EQUIPIVTR_FLYSWORD, true);
//	::c2s_SendCmdChargeEquipFlySword(iEleIdx, iCount);
}

void CECGameSession::c2s_CmdChargeFlySword(int iEleIdx, int iFSIdx, int iCount, int idFlySword)
{ 
	FreezeHostItem(IVTRTYPE_PACK, iEleIdx, true);
	FreezeHostItem(IVTRTYPE_PACK, iFSIdx, true);
	::c2s_SendCmdChargeFlySword(iEleIdx, iFSIdx, iCount, idFlySword); 
}

void CECGameSession::c2s_CmdNPCSevSell(int iItemNum, C2S::npc_trade_item* aItems)
{
	for (int i=0; i < iItemNum; i++)
		FreezeHostItem(IVTRTYPE_PACK, aItems[i].index, true);
	
	::c2s_SendCmdNPCSevSell(iItemNum, aItems);
}

void CECGameSession::c2s_CmdNPCSevBoothSell(int idBooth, int iItemNum, C2S::npc_booth_item* aItems)
{ 
	for (int i=0; i < iItemNum; i++)
		FreezeHostItem(IVTRTYPE_PACK, aItems[i].inv_index, true);
	
	::c2s_SendCmdNPCSevBoothSell(idBooth, iItemNum, aItems); 
}

void CECGameSession::c2s_CmdNPCSevPetAdopt(int inv_index)
{
	using namespace C2S;

#pragma pack(1)

	struct CONTENT
	{
		int iIvtrIdx;
	};

#pragma pack()

	int iSize = sizeof (cmd_header) + sizeof (cmd_sevnpc_serve) + sizeof (CONTENT);
	BYTE* pBuf = new BYTE [iSize];
	if (!pBuf)
		return;

	((cmd_header*)pBuf)->cmd = C2S::SEVNPC_SERVE;

	cmd_sevnpc_serve* pCmd = (cmd_sevnpc_serve*)(pBuf + sizeof (cmd_header));
	pCmd->service_type = GP_NPCSEV_ADOPT_PET;
	pCmd->len = sizeof (CONTENT);

	BYTE* pData = pBuf + sizeof (cmd_header) + sizeof (cmd_sevnpc_serve);
	CONTENT* pContent = (CONTENT*)pData;
	pContent->iIvtrIdx = inv_index;

	g_pGame->GetGameSession()->SendGameData(pBuf, iSize);

	delete [] pBuf;
}

void CECGameSession::c2s_CmdNPCSevFree(int inv_index)
{
	using namespace C2S;

#pragma pack(1)

	struct CONTENT
	{
		int iIvtrIdx;
	};

#pragma pack()

	int iSize = sizeof (cmd_header) + sizeof (cmd_sevnpc_serve) + sizeof (CONTENT);
	BYTE* pBuf = new BYTE [iSize];
	if (!pBuf)
		return;

	((cmd_header*)pBuf)->cmd = C2S::SEVNPC_SERVE;

	cmd_sevnpc_serve* pCmd = (cmd_sevnpc_serve*)(pBuf + sizeof (cmd_header));
	pCmd->service_type = GP_NPCSEV_FREE_PET;
	pCmd->len = sizeof (CONTENT);

	BYTE* pData = pBuf + sizeof (cmd_header) + sizeof (cmd_sevnpc_serve);
	CONTENT* pContent = (CONTENT*)pData;
	pContent->iIvtrIdx = inv_index;

	g_pGame->GetGameSession()->SendGameData(pBuf, iSize);

	delete [] pBuf;
}

void CECGameSession::c2s_CmdNPCSevCombine(int inv_index_pet, int inv_index_c, int inv_index_a)
{
	using namespace C2S;

#pragma pack(1)

	struct CONTENT
	{
		int inv_index_pet;      //物品 包裹栏索引
        int inv_index_c;        //修炼道具 包裹索引栏
        int inv_index_a;        //辅助道具 包裹索引栏
	};

#pragma pack()

	int iSize = sizeof (cmd_header) + sizeof (cmd_sevnpc_serve) + sizeof (CONTENT);
	BYTE* pBuf = new BYTE [iSize];
	if (!pBuf)
		return;

	((cmd_header*)pBuf)->cmd = C2S::SEVNPC_SERVE;

	cmd_sevnpc_serve* pCmd = (cmd_sevnpc_serve*)(pBuf + sizeof (cmd_header));
	pCmd->service_type = GP_NPCSEV_COMBINE_PET;
	pCmd->len = sizeof (CONTENT);

	BYTE* pData = pBuf + sizeof (cmd_header) + sizeof (cmd_sevnpc_serve);
	CONTENT* pContent = (CONTENT*)pData;
	pContent->inv_index_pet = inv_index_pet;
	pContent->inv_index_c = inv_index_c;
	pContent->inv_index_a = inv_index_a;

	g_pGame->GetGameSession()->SendGameData(pBuf, iSize);

	delete [] pBuf;
}

void CECGameSession::c2s_CmdNPCSevRename(int inv_index, const ACHAR* szName)
{
	using namespace C2S;

#pragma pack(1)

	struct CONTENT
	{
		int inv_index;  //物品 包裹栏索引
		char name[20];
		int size;
	};

#pragma pack()

	int iSize = sizeof (cmd_header) + sizeof (cmd_sevnpc_serve) + sizeof (CONTENT);
	BYTE* pBuf = new BYTE [iSize];
	if (!pBuf)
		return;

	((cmd_header*)pBuf)->cmd = C2S::SEVNPC_SERVE;

	cmd_sevnpc_serve* pCmd = (cmd_sevnpc_serve*)(pBuf + sizeof (cmd_header));
	pCmd->service_type = GP_NPCSEV_RENAME_PET;
	pCmd->len = sizeof (CONTENT);

	BYTE* pData = pBuf + sizeof (cmd_header) + sizeof (cmd_sevnpc_serve);
	CONTENT* pContent = (CONTENT*)pData;
	pContent->inv_index = inv_index;
	a_strncpy((ACHAR*)pContent->name, szName, 20 / sizeof(ACHAR));
	pContent->size = wcslen(szName) * sizeof(ACHAR);

	g_pGame->GetGameSession()->SendGameData(pBuf, iSize);

	delete [] pBuf;
}

void CECGameSession::c2s_CmdNPCSevVehicleUpgrade(int inv_mount, int inv_material)
{
	using namespace C2S;

#pragma pack(1)

	struct CONTENT
	{
		int inv_mount;            //要驯服骑乘物品在包裹栏中的索引
        int inv_material;	   //需要的材料在包裹栏中的索引
	};

#pragma pack()

	int iSize = sizeof (cmd_header) + sizeof (cmd_sevnpc_serve) + sizeof (CONTENT);
	BYTE* pBuf = new BYTE [iSize];
	if (!pBuf)
		return;

	((cmd_header*)pBuf)->cmd = C2S::SEVNPC_SERVE;

	cmd_sevnpc_serve* pCmd = (cmd_sevnpc_serve*)(pBuf + sizeof (cmd_header));
	pCmd->service_type = GP_NPCSEV_VEHICLE_UPGRADE;
	pCmd->len = sizeof (CONTENT);

	BYTE* pData = pBuf + sizeof (cmd_header) + sizeof (cmd_sevnpc_serve);
	CONTENT* pContent = (CONTENT*)pData;
	pContent->inv_mount = inv_mount;
	pContent->inv_material = inv_material;

	g_pGame->GetGameSession()->SendGameData(pBuf, iSize);

	delete [] pBuf;
}

void CECGameSession::c2s_CmdNPCSevLockItem(int item_index, int item_id)
{
	using namespace C2S;

#pragma pack(1)

	struct CONTENT
	{
		int item_index;
        int item_id;
	};

#pragma pack()

	int iSize = sizeof (cmd_header) + sizeof (cmd_sevnpc_serve) + sizeof (CONTENT);
	BYTE* pBuf = new BYTE [iSize];
	if (!pBuf)
		return;

	((cmd_header*)pBuf)->cmd = C2S::SEVNPC_SERVE;

	cmd_sevnpc_serve* pCmd = (cmd_sevnpc_serve*)(pBuf + sizeof (cmd_header));
	pCmd->service_type = GP_NPCSEV_LOCK_ITEM;
	pCmd->len = sizeof (CONTENT);

	BYTE* pData = pBuf + sizeof (cmd_header) + sizeof (cmd_sevnpc_serve);
	CONTENT* pContent = (CONTENT*)pData;
	pContent->item_index = item_index;
	pContent->item_id = item_id;

	g_pGame->GetGameSession()->SendGameData(pBuf, iSize);

	delete [] pBuf;
}

void CECGameSession::c2s_CmdNPCSevUnlockItem(int item_index, int item_id)
{
	using namespace C2S;

#pragma pack(1)

	struct CONTENT
	{
		int item_index;
        int item_id;
	};

#pragma pack()

	int iSize = sizeof (cmd_header) + sizeof (cmd_sevnpc_serve) + sizeof (CONTENT);
	BYTE* pBuf = new BYTE [iSize];
	if (!pBuf)
		return;

	((cmd_header*)pBuf)->cmd = C2S::SEVNPC_SERVE;

	cmd_sevnpc_serve* pCmd = (cmd_sevnpc_serve*)(pBuf + sizeof (cmd_header));
	pCmd->service_type = GP_NPCSEV_UNLOCK_ITEM;
	pCmd->len = sizeof (CONTENT);

	BYTE* pData = pBuf + sizeof (cmd_header) + sizeof (cmd_sevnpc_serve);
	CONTENT* pContent = (CONTENT*)pData;
	pContent->item_index = item_index;
	pContent->item_id = item_id;

	g_pGame->GetGameSession()->SendGameData(pBuf, iSize);

	delete [] pBuf;
}

void CECGameSession::c2s_CmdNPCSevItemTrade(const C2S::item_trade* pItemList, size_t count)
{
	using namespace C2S;

#pragma pack(1)

	struct CONTENT
	{
		size_t count;
		item_trade p[1];
	};

#pragma pack()

	int sz =  sizeof (size_t) + sizeof(item_trade) * count;
	int iSize = sizeof (cmd_header) + sizeof (cmd_sevnpc_serve) + sz;
	BYTE* pBuf = new BYTE[iSize];

	((cmd_header*)pBuf)->cmd = C2S::SEVNPC_SERVE;

	cmd_sevnpc_serve* pCmd = (cmd_sevnpc_serve*)(pBuf + sizeof (cmd_header));
	pCmd->service_type = GP_NPCSEV_ITEM_TRADE;
	pCmd->len = sz;

	BYTE* pData = pBuf + sizeof (cmd_header) + sizeof (cmd_sevnpc_serve);
	CONTENT* pContent = (CONTENT*)pData;
	pContent->count = count;
	memcpy(pContent->p, pItemList, sizeof(item_trade) * count);

	g_pGame->GetGameSession()->SendGameData(pBuf, iSize);
	delete[] pBuf;
}


void CECGameSession::c2s_CmdPetSetRank(unsigned char pet_index, int pet_tid, unsigned char rank)
{
	using namespace C2S;

	int iSize = sizeof (cmd_header) + sizeof (set_pet_rank);
	BYTE* pBuf = new BYTE [iSize];
	if (!pBuf)
		return;
	
	((cmd_header*)pBuf)->cmd = C2S::SET_PET_RANK;

	set_pet_rank* pCmd = (set_pet_rank*)(pBuf + sizeof (cmd_header));
	pCmd->pet_index	= pet_index;
	pCmd->pet_tid = pet_tid;
	pCmd->rank = rank;

	g_pGame->GetGameSession()->SendGameData(pBuf, iSize);

	delete [] pBuf;
}

void CECGameSession::c2s_CmdPetMoveBedgeItem( unsigned char src_index, unsigned char dst_index)
{
	using namespace C2S;

	int iSize = sizeof (cmd_header) + sizeof (move_pet_bedge_item);
	BYTE* pBuf = new BYTE [iSize];
	if (!pBuf)
		return;
	
	((cmd_header*)pBuf)->cmd = C2S::MOVE_PET_BEDGE_ITEM;

	move_pet_bedge_item* pCmd = (move_pet_bedge_item*)(pBuf + sizeof (cmd_header));
	pCmd->src_index	= src_index;
	pCmd->dst_index = dst_index;

	g_pGame->GetGameSession()->SendGameData(pBuf, iSize);

	delete [] pBuf;

}

void CECGameSession::c2s_CmdPetEquipBedge(unsigned char inv_index, unsigned char pet_index)
{
	using namespace C2S;

	int iSize = sizeof (cmd_header) + sizeof (equip_pet_bedge);
	BYTE* pBuf = new BYTE [iSize];
	if (!pBuf)
		return;
	
	((cmd_header*)pBuf)->cmd = C2S::EQUIP_PET_BEDGE;

	equip_pet_bedge* pCmd = (equip_pet_bedge*)(pBuf + sizeof (cmd_header));
	pCmd->inv_index = inv_index;
	pCmd->pet_index = pet_index;

	g_pGame->GetGameSession()->SendGameData(pBuf, iSize);

	delete [] pBuf;
}

void CECGameSession::c2s_CmdPetEquip(unsigned char inv_index, unsigned char pet_index)
{
	using namespace C2S;

	int iSize = sizeof (cmd_header) + sizeof (equip_pet_equip);
	BYTE* pBuf = new BYTE [iSize];
	if (!pBuf)
		return;

	((cmd_header*)pBuf)->cmd = C2S::EQUIP_PET_EQUIP;

	equip_pet_equip* pCmd = (equip_pet_equip*)(pBuf + sizeof (cmd_header));
	pCmd->inv_index = inv_index;
	pCmd->pet_index = pet_index;

	g_pGame->GetGameSession()->SendGameData(pBuf, iSize);

	delete [] pBuf;
}

void CECGameSession::c2s_CmdPetCtrlCmd(unsigned char pet_index, int target, int pet_cmd, void* buf, int len)
{
	using namespace C2S;

#pragma pack(1)
	
	struct CONTENT
	{
		unsigned char pet_index;
		int target;
        int pet_cmd;
	};   

#pragma pack()

	int iSize = sizeof (cmd_header) + sizeof (CONTENT) + len;
	BYTE* pBuf = new BYTE [iSize];

	if (!pBuf)
		return;
	
	((cmd_header*)pBuf)->cmd = C2S::PET_CTRL_CMD;

	CONTENT* pCmd = (CONTENT*)(pBuf + sizeof (cmd_header));
	pCmd->pet_index = pet_index;
	pCmd->target = target;
	pCmd->pet_cmd = pet_cmd;

	BYTE* pData = pBuf + sizeof (cmd_header) + sizeof (CONTENT);

	if (len)
		memcpy(pData, buf, len);

	g_pGame->GetGameSession()->SendGameData(pBuf, iSize);

	delete [] pBuf;
}

void CECGameSession::c2s_CmdPetCombine(unsigned char pet_index, unsigned char combine_type)
{
	using namespace C2S;

	int iSize = sizeof (cmd_header) + sizeof (combine_pet);
	BYTE* pBuf = new BYTE [iSize];
	if (!pBuf)
		return;

	((cmd_header*)pBuf)->cmd = C2S::COMBINE_PET;

	combine_pet* pCmd = (combine_pet*)(pBuf + sizeof (cmd_header));
	pCmd->pet_index = pet_index;
	pCmd->combine_type = combine_type;

	g_pGame->GetGameSession()->SendGameData(pBuf, iSize);

	delete [] pBuf;
}

void CECGameSession::c2s_CmdPetUncombine(unsigned char pet_index)
{
	using namespace C2S;

	int iSize = sizeof (cmd_header) + sizeof (un_combine_pet);
	BYTE* pBuf = new BYTE [iSize];
	if (!pBuf)
		return;

	((cmd_header*)pBuf)->cmd = C2S::UN_COMBINE_PET;

	un_combine_pet* pCmd = (un_combine_pet*)(pBuf + sizeof (cmd_header));
	pCmd->pet_index = pet_index;

	g_pGame->GetGameSession()->SendGameData(pBuf, iSize);

	delete [] pBuf;
}

void CECGameSession::c2s_CmdPetSetStatus(unsigned char pet_index, int pet_tid, unsigned char main_status, unsigned char sub_status)
{
	using namespace C2S;

	int iSize = sizeof (cmd_header) + sizeof (set_pet_status);
	BYTE* pBuf = new BYTE [iSize];
	if (!pBuf)
		return;

	((cmd_header*)pBuf)->cmd = C2S::SET_PET_STATUS;

	set_pet_status* pCmd = (set_pet_status*)(pBuf + sizeof (cmd_header));
	pCmd->pet_index = pet_index;
	pCmd->pet_tid = pet_tid;
	pCmd->main_status = main_status;
	pCmd->sub_status = sub_status;

	g_pGame->GetGameSession()->SendGameData(pBuf, iSize);
	LuaEvent::FirstChangePetMode();
	if(main_status == 1)
		LuaEvent::FirstPetCollect();
	delete [] pBuf;
}

void CECGameSession::c2s_CmdStartFly()
{
	using namespace C2S;

	int iSize = sizeof (cmd_header);
	BYTE* pBuf = new BYTE [iSize];
	if (!pBuf)
		return;
	
	((cmd_header*)pBuf)->cmd = START_FLY;
	g_pGame->GetGameSession()->SendGameData(pBuf, iSize);

	delete [] pBuf;
}

void CECGameSession::c2s_CmdStopFly()
{
	using namespace C2S;

	int iSize = sizeof (cmd_header);
	BYTE* pBuf = new BYTE [iSize];
	if (!pBuf)
		return;
	
	((cmd_header*)pBuf)->cmd = STOP_FLY;
	g_pGame->GetGameSession()->SendGameData(pBuf, iSize);

	delete [] pBuf;
}

void CECGameSession::c2s_CmdStartTransfigure()
{
	using namespace C2S;

	int iSize = sizeof (cmd_header);
	BYTE* pBuf = new BYTE [iSize];
	if (!pBuf)
		return;
	
	((cmd_header*)pBuf)->cmd = START_TRANSFIGURE;
	g_pGame->GetGameSession()->SendGameData(pBuf, iSize);

	delete [] pBuf;
}

void CECGameSession::c2s_CmdStopTransfigure()
{
	using namespace C2S;

	int iSize = sizeof (cmd_header);
	BYTE* pBuf = new BYTE [iSize];
	if (!pBuf)
		return;
	
	((cmd_header*)pBuf)->cmd = STOP_TRANSFIGURE;
	g_pGame->GetGameSession()->SendGameData(pBuf, iSize);

	delete [] pBuf;
}*/

void CECGameSession::OnPrtcGetItemDesc(GNET::Protocol* pProtocol)
{
	using namespace GNET;

	//implement type cast to the protocol.
	GetItemDesc* getItemDescProtocol = ( GetItemDesc* )pProtocol;
	CECIvtrItem* pIvtrItem = NULL;

	//create the GetItemDescRe protocol.
	GetItemDescResponse getItemDescResponse;
	//copy the sn and timestamp value from the source protocol.
	getItemDescResponse.sn = getItemDescProtocol->sn;
	getItemDescResponse.timestamp = getItemDescProtocol->timestamp;

	//get the detailInfo field of the GetItemDesc protocol as GRoleInventory.
//	GRoleInventory* pItemInfo = ( GRoleInventory* )getItemDescProtocol->detailInfo.begin();
	GRoleInventory* pItemInfo, itemInfo;
	Marshal::OctetsStream os( getItemDescProtocol->detailInfo );
	os >> itemInfo;
	pItemInfo = &itemInfo;
	
	//
//	a_LogOutput( 1, "Can marshal the GRoleInventory!, the item id= %d\n", pItemInfo->id );//Canceled 2010-08-24.

	//check the detailInfo.
	if( !pItemInfo )
	{
		getItemDescResponse.retCode = 1;//it means Fail to parse the protocol.
		SendNetData( getItemDescResponse );//send the response protocol to the server.
		//test.
		a_LogOutput( 1, "Return code = 1, Fail to parse protocol!\n" );
		//test end.
		return;
	}

	//create the intrItem based on the pItemInfo.
	pIvtrItem = CECIvtrItem::CreateItem( pItemInfo->id, pItemInfo->expire_date, pItemInfo->count );
	if (!pIvtrItem)
	{
		a_LogOutput(1, "CECGameSession::OnPrtcGetItemDesc, Failed to create the inventory item.");
		getItemDescResponse.retCode = 2;//it means Can not create the relative ivtrItem.
		SendNetData( getItemDescResponse );//send the response protocol to the server.
		//test.
		a_LogOutput( 1, "Return code = 2, Fail to create ivtrItem!\n" );
		//test end.
		return;
	}

	//check the data field of the pItemInfo and set the ItemInfo.
	if( pItemInfo->data.begin() && pItemInfo->data.size() )
	{
		pIvtrItem->SetItemInfo((BYTE*)pItemInfo->data.begin(), pItemInfo->data.size());
	}
	else//New Added, used when the content and the content length is 0.
	{
		pIvtrItem->SetEmptyInfo();
	}
	pIvtrItem->UpdateInfoMD5();

	//
//	a_LogOutput( 1, "Can create the ivtrItem!\n" );//Canceled 2010-08-24.

	//get the detailed description info of the item.
//	const wchar_t *pszHint = pIvtrItem->GetDesc();//use the default parameter.	//Canceled 2012-09-17.
	//-------------------------------------------------------------------------特殊字符转义
	//Added 2012-09-17.
	CECRoleDescXMLParser roleDescXMLParser;
	ACString strItemDesc;
	roleDescXMLParser.TranslateStr( pIvtrItem->GetDesc(), strItemDesc );
	const wchar_t *pszHint = (const wchar_t*)strItemDesc;
	//-------------------------------------------------------------------------

	//set the incon file name.
	char iconName[512];
	wchar_t iconNameWide[1024];
	sprintf( iconName, "%d.png", pItemInfo->id );
//	a_LogOutput( 1, "item id = %d\n", pItemInfo->id );//Canceled 2010-08-24.
	a_MultiByteToWideChar( iconName, 512, iconNameWide, 1024 );

	//set the GetItemDesc Response message protocol.
	getItemDescResponse.retCode = 0;//it means success to parse the protocol.
	getItemDescResponse.templateID = pItemInfo->id;
	getItemDescResponse.level = pIvtrItem->GetEstone(); //use the m_Essence.old_data.num_estone of the equapment to spacify the level.
	
/*	size_t strLength = wcslen(pIvtrItem->GetName()) * sizeof (wchar_t);
	char* name = new char[ strLength ];
	a_WideCharToMultiByte( pIvtrItem->GetName(), wcslen(pIvtrItem->GetName()), name, strLength );		
	char* name = "测试";
	getItemDescResponse.name.replace( name, strlen(name) );
*/
	//-------------------------------------------------------------------------特殊字符转义
	//Added 2012-09-17.
	ACString strItemName;
	//roleDescXMLParser.TranslateStr( pIvtrItem->GetName(), strItemName );
	strItemName = pIvtrItem->GetName();
	const wchar_t *pszItemName = (const wchar_t*)strItemName;
	//-------------------------------------------------------------------------
	getItemDescResponse.name.replace( pszItemName, wcslen(pszItemName) * sizeof (wchar_t) ); //pIvtrItem->GetName()
//	getItemDescResponse.icon.replace( pIvtrItem->GetIconFile(), strlen(pIvtrItem->GetIconFile()) );
	getItemDescResponse.icon.replace( iconNameWide, wcslen(iconNameWide)*sizeof(wchar_t) );
/*
	size_t strLength = wcslen(pszHint) * sizeof (wchar_t);
	char* resultHint = new char[ strLength ];
	a_WideCharToMultiByte( pszHint, wcslen(pszHint), resultHint, strLength );	
	getItemDescResponse.detailInfo.replace( resultHint, strlen(resultHint) );	
*/
	if( pszHint )//make sure the hint text is valid.
	{
		getItemDescResponse.detailInfo.replace( pszHint, wcslen(pszHint) * sizeof (wchar_t) );
	}
	else
	{
		a_LogOutput( 1, "Hint text is NULL!\n" );
	}

//	printf( "name = %s, icon = %s, detail = %s\n", name, (const char*)iconName, resultHint );

	//set the data to server.
	SendNetData( getItemDescResponse );

	//free the memory.
//	delete []name;
//	delete []resultHint;

	//free the memory.
	delete pIvtrItem;

	//test.
//	a_LogOutput( 1, "finish process the current protocol, and send the info to server!\n The sn = %d\n", getItemDescProtocol->sn );//Canceled 2010-08-24.
	//test end.
}

void CECGameSession::OnPrtcGetItemDescForPlatform( GNET::Protocol* pProtocol )
{
	using namespace GNET;

	//强制类型转换协议指针
	GetItemDescForPlatform* pGetItemDescForPlatform = (GetItemDescForPlatform*)pProtocol;
	if ( !pGetItemDescForPlatform )
	{
		a_LogOutput( 1, "The protocol is NULL!\n" );
		return;
	}
	CECIvtrItem* pIvtrItem = NULL;

	//初始化新的返回协议
	GetItemDescForPlatformRe getItemDescForPlatformRe;
	getItemDescForPlatformRe.sn = pGetItemDescForPlatform->sn;
	getItemDescForPlatformRe.timestamp = pGetItemDescForPlatform->timestamp;

	//设置key
	Octets key;
	unsigned char szKey[] = {0xbf,0x61,0x09,0x72,0x1d,0xbb,0x6e,0xe9,0x9a,0x01,
					0x3d,0x47,0x0a,0x11,0x60,0xbe,0xf8,0x48,0x36,0x3f,
					0xdf,0xf5,0x9a,0xc5,0xca,0x1c,0x79,0xcb,0xbb,0x90,
					0x96,0xde,0x58,0xff,0x60,0xbe,0x8d,0x4e,0xd8,0xe1,
					0x83,0x47,0xd0,0x57,0x6d,0x62,0x49,0x66,0x71,0x0d,
					0x6a,0xfe,0x3b,0xdc,0xd2,0x33,0xd7,0x8f,0x5c,0x5e,
					0x4b,0x86,0x25,0xd0,0x6f,0xd2,0xd0,0x8c,0x53,0x46,
					0xd0,0xc4,0x3a,0x10,0xae,0xcf,0x75,0xcd,0xfd,0x74,
					0xe8,0xfe,0x80,0x0b,0x14,0xf9,0x94,0x66,0xcc,0x19,
					0x7d,0xda,0x89,0x27,0x91,0xab,0x5c,0x73,0xb0,0x78,
					0xc9,0x1c,0x48,0xd9,0xe2,0xc3,0xfc,0xfa,0x3e,0x74,
					0xb9,0xec,0x81,0x66,0x5f,0x5b,0x3e,0x73,0x1e,0xd2,
					0x31,0x99,0xc7,0xb5,0xe7,0x3e,0xbc,0x02};
	key.replace( szKey, 128 );

	//解密物品详细信息
	Security* security = Security::Create(ARCFOURSECURITY);
	security->SetParameter(key);
	Octets detailInfo = pGetItemDescForPlatform->detailInfo;
	security->Update( detailInfo );

	//计算解密后的详细信息的md5值，并且判断是否与发送过来的md5值相等
	Octets md5Info = MD5Hash::Digest( detailInfo );
	if ( md5Info != pGetItemDescForPlatform->md5Info ) //如果数据不一致，则返回协议中包含错误码
	{
		getItemDescForPlatformRe.retCode = 1;//md5校验错误
		SendNetData( getItemDescForPlatformRe );//send the response protocol to the server.
		a_LogOutput( 1, "Return code = 1, MD5 information assertion failed!\n" );
		return;
	}

	//解析detailInfo 到 GRoleInventory
	GRoleInventory itemInfo;
	Marshal::OctetsStream os( detailInfo );
	os >> itemInfo;
	
	//通过itemInfo构建物品实例
	pIvtrItem = CECIvtrItem::CreateItem( itemInfo.id, itemInfo.expire_date, itemInfo.count );
	if (!pIvtrItem)
	{
		getItemDescForPlatformRe.retCode = 2;//构建物品失败，物品信息错误
		SendNetData( getItemDescForPlatformRe );//send the response protocol to the server.
		a_LogOutput( 1, "Return code = 2, Fail to create ivtrItem!\n" );
		return;
	}

	//设置物品的详细信息
	if( itemInfo.data.begin() && itemInfo.data.size() )
	{
		pIvtrItem->SetItemInfo((BYTE*)itemInfo.data.begin(), itemInfo.data.size());
	}
	else//New Added, used when the content and the content length is 0.
	{
		pIvtrItem->SetEmptyInfo();
	}
	pIvtrItem->UpdateInfoMD5();

	//计算并获得物品解析完之后的属性信息
	const wchar_t *pszHint = pIvtrItem->GetDesc();//use the default parameter.

	//set the incon file name.
	char iconName[512];
	wchar_t iconNameWide[1024];
	sprintf( iconName, "%d.png", itemInfo.id );
//	a_LogOutput( 1, "item id = %d\n", pItemInfo->id );//Canceled 2010-08-24.
	a_MultiByteToWideChar( iconName, 512, iconNameWide, 1024 );

	//set the GetItemDesc Response message protocol.
	getItemDescForPlatformRe.retCode = 0;//it means success to parse the protocol.
	getItemDescForPlatformRe.templateID = itemInfo.id;
	getItemDescForPlatformRe.level = pIvtrItem->GetEstone(); //use the m_Essence.old_data.num_estone of the equapment to spacify the level.
	
	getItemDescForPlatformRe.name.replace( pIvtrItem->GetName(), wcslen(pIvtrItem->GetName()) * sizeof (wchar_t) );
	getItemDescForPlatformRe.icon.replace( iconNameWide, wcslen(iconNameWide)*sizeof(wchar_t) );

	if( pszHint )//make sure the hint text is valid.
	{
		getItemDescForPlatformRe.detailInfo.replace( pszHint, wcslen(pszHint) * sizeof (wchar_t) );
	}
	else
	{
		a_LogOutput( 1, "Hint text is NULL!\n" );
	}

	//set the data to server.
	SendNetData( getItemDescForPlatformRe );

	//free the memory.
	delete pIvtrItem;
}

void CECGameSession::SendNewKeepAlivePrtc( void )
{
	NewKeepAlive keepAliveProtocol;
	keepAliveProtocol.reserved = 1;

	//set the data to server.
	SendNetData( keepAliveProtocol );
}

void CECGameSession::SendAnnounceAid( void )
{
	ToolAnnounceAid aidProtocol(11);

	//set the data to server.
	SendNetData(aidProtocol);
}

void CECGameSession::OnPrtcProcessGetItemDescForSNS( GNET::Protocol* pProtocol )
{
	using namespace GNET;

	//判断合法性
	if ( !pProtocol )
	{
		a_LogOutput( 1, "Invalid protocol!\n" );
		return;
	}

	//定义所需变量
	CECIvtrItem* pIvtrItem = NULL;
	Octets detailInfo;
	int		nZoneId = 0;
	INT64	lRoleId = 0;
	INT64	lTimeStamp = 0;
	byte	retCode = 0;
	
	switch ( pProtocol->GetType() )
	{
		case PROTOCOL_GET_ROLE_EQUIP:
		{
			GetRoleEquip* pSrc = (GetRoleEquip*)pProtocol;
			nZoneId = pSrc->zone_id;
			lRoleId = pSrc->role_id;
			lTimeStamp = pSrc->timestamp;
			detailInfo = pSrc->detailInfo;
			break;
		}
		case PROTOCOL_GET_SNS_ROLE_PET:
		{
			GetSNSRolePet* pSrc = (GetSNSRolePet*)pProtocol;
			nZoneId = pSrc->zone_id;
			lRoleId = pSrc->role_id;
			lTimeStamp = pSrc->timestamp;
			detailInfo = pSrc->detailInfo;
			break;
		}
		default:
			break;
	}

	//通过协议相关信息构造并获得物品实例，同时从物品实例中获得物品信息
	//解析detailInfo 到 GRoleInventory
	GRoleInventory itemInfo;
	Marshal::OctetsStream os( detailInfo );
	os >> itemInfo;
	
	//通过itemInfo构建物品实例
	pIvtrItem = CECIvtrItem::CreateItem( itemInfo.id, itemInfo.expire_date, itemInfo.count );
	if (!pIvtrItem)
	{
		retCode = 1;//构建物品失败，物品信息错误
		a_LogOutput( 1, "Return code = 1, Fail to create ivtrItem!\n" );
	}
	else	//构建物品成功，继续处理
	{

		//设置物品的详细信息
		if( itemInfo.data.begin() && itemInfo.data.size() )
		{
			pIvtrItem->SetItemInfo((BYTE*)itemInfo.data.begin(), itemInfo.data.size());
		}
		else//New Added, used when the content and the content length is 0.
		{
			pIvtrItem->SetEmptyInfo();
		}
		pIvtrItem->UpdateInfoMD5();

		//计算并获得物品解析完之后的属性信息
		const wchar_t *pszHint = pIvtrItem->GetDesc();//use the default parameter.
		if( pszHint )//make sure the hint text is valid.
		{
			detailInfo.replace( pszHint, wcslen(pszHint) * sizeof (wchar_t) );
		}
		else
		{
			a_LogOutput( 1, "Hint text is NULL!\n" );
		}	
	}

	//将所有计算的结果值存入协议后，发出协议
	switch ( pProtocol->GetType() )
	{
		case PROTOCOL_GET_ROLE_EQUIP:
		{
			GetRoleEquipRe getRoleEquipRe;
			getRoleEquipRe.retCode = retCode;
			getRoleEquipRe.zone_id = nZoneId;
			getRoleEquipRe.role_id = lRoleId;
			getRoleEquipRe.timestamp = lTimeStamp;
			if ( !retCode )
			{
				getRoleEquipRe.detailInfo = detailInfo;
			}
			//set the data to server.
			SendNetData( getRoleEquipRe );
			break;
		}
		case PROTOCOL_GET_SNS_ROLE_PET:
		{
			GetRolePetRe getSNSRolePetRe;
			getSNSRolePetRe.retCode = retCode;
			getSNSRolePetRe.zone_id = nZoneId;
			getSNSRolePetRe.role_id = lRoleId;
			getSNSRolePetRe.timestamp = lTimeStamp;
			if ( !retCode )
			{
				getSNSRolePetRe.detailInfo = detailInfo;
			}
			//set the data to server.
			SendNetData( getSNSRolePetRe );
			break;
		}
		default:
			break;
	}

	//清理资源
	if ( pIvtrItem )
	{
		delete pIvtrItem;
	}

}

void CECGameSession::OnPrtcGetRoleDesc( GNET::Protocol* pProtocol )
{
	using namespace GNET;

	//判断输入参数合法性
	if ( !pProtocol )
	{
		a_LogOutput( 1, "Invalid protocol!\n" );
		return;
	}

	//转化协议，并根据协议值得到最终xml格式字符串
	GetRoleDesc* pGetRoleDesc = (GetRoleDesc*)pProtocol;
	GetRoleDescRe getRoleDescRe;
	getRoleDescRe.sn		= pGetRoleDesc->sn;
	getRoleDescRe.timestamp	= pGetRoleDesc->timestamp;

	//将协议中的详细信息压入到GConsignRole结构中
	GConsignRole roleInfo;
	Marshal::OctetsStream os( pGetRoleDesc->detailInfo );
	os >> roleInfo;

	CECRoleDescXMLParser roleDescXMLParser;
	ACString strXMLText = roleDescXMLParser.GetXMLText( &roleInfo );

	//==========================================================================
/*	FILE* pFile = fopen( "./role.xml", "w+t" );
	if ( !pFile )
	{
		a_LogOutput( 1, "Can not open role.xml!" );
		return;
	}
	char* result = new char[ strXMLText.GetLength()*2 ];
	a_WideCharToMultiByte( strXMLText, strXMLText.GetLength(), result, strXMLText.GetLength()*2 );//wcslen(pszHint)
	fwrite( result, sizeof( char ), strXMLText.GetLength()*2, pFile );
	delete []result;
	fclose(pFile);
*/
	//==========================================================================

	//初始化并设置回复协议，之后发送该协议到server
	if ( strXMLText.GetLength() == 0 )
	{
		getRoleDescRe.retCode = 1;	//解析错误
		SendNetData( getRoleDescRe );
		a_LogOutput( 1, "XML Parser Error!" );
		return;
	}

	getRoleDescRe.cls = roleInfo.gsroleinfo.rolebasicinfo.cls;
	getRoleDescRe.gender = roleInfo.gsroleinfo.rolebasicinfo.gender;
	getRoleDescRe.level = roleInfo.gsroleinfo.rolebasicinfo.level;
	ACString strClsName = roleDescXMLParser.GetClsName(roleInfo.gsroleinfo.rolebasicinfo.cls);
	getRoleDescRe.name.replace( strClsName, wcslen(strClsName) * sizeof (wchar_t) );
	ACString strIcon;
	strIcon.Format( _AL("head/%d.png"), roleInfo.gsroleinfo.rolebasicinfo.cls );
	getRoleDescRe.icon.replace( strIcon, strIcon.GetLength()*sizeof(wchar_t) );

	//basic_desc
	ACString strBasicDesc;
	strBasicDesc.Format( _AL("职业 %s\\r性别 %s\\r飞升前等级 %d %s\\r等级 %d\\r阵营 %s\\r元神等级 %d\\r攻击 %d-%d\\r防御 %d\\r气血 %d/%d\\r真气 %d/%d"),  //\\r声望 %d //---去掉声望
						strClsName,	//roleDescXMLParser.GetClsName(roleInfo.gsroleinfo.rolebasicinfo.cls),
						((roleInfo.gsroleinfo.rolebasicinfo.gender == 0) ? _AL("男") : _AL("女")),
						roleInfo.gsroleinfo.rolebasicinfo.level_before_reborn,
						((roleInfo.gsroleinfo.rolebasicinfo.level_before_reborn == 0) ? _AL("未飞升") : roleDescXMLParser.GetClsName(roleInfo.gsroleinfo.rolebasicinfo.cls_before_reborn)),
						/*roleInfo.gsroleinfo.rolebasicinfo.reputation,*/
						roleInfo.gsroleinfo.rolebasicinfo.level,
						roleDescXMLParser.GetCultivationName(roleInfo.gsroleinfo.rolebasicinfo.cultivation),
						roleInfo.gsroleinfo.deityinfo.deity_level,
						roleInfo.gsroleinfo.rolebasicinfo.damage_low,
						roleInfo.gsroleinfo.rolebasicinfo.damage_high,
						roleInfo.gsroleinfo.rolebasicinfo.defense,
						roleInfo.gsroleinfo.rolebasicinfo.max_hp,
						roleInfo.gsroleinfo.rolebasicinfo.max_hp,
						roleInfo.gsroleinfo.rolebasicinfo.max_mp,
						roleInfo.gsroleinfo.rolebasicinfo.max_mp
						);
	getRoleDescRe.basicDetailInfo.replace( strBasicDesc, strBasicDesc.GetLength()*sizeof(wchar_t) );

	//detial_desc
	getRoleDescRe.detailDesc.replace( strXMLText, strXMLText.GetLength()*sizeof(wchar_t) );

	//--------------------
//	ACString strBasicDesc(_AL("1111"));
//	getRoleDescRe.basicDetailInfo.replace( strBasicDesc, strBasicDesc.GetLength()*sizeof(wchar_t) );
	//--------------------
	//返回协议
	SendNetData( getRoleDescRe );

	//清理资源
}

void CECGameSession::OnPrtcGetRoleDescSnS( GNET::Protocol* pProtocol )
{
	using namespace GNET;

	//判断输入参数合法性
	if ( !pProtocol )
	{
		a_LogOutput( 1, "Invalid protocol!\n" );
		return;
	}

	//转化协议，并根据协议值得到最终xml格式字符串
	GetRoleDescSnS* pGetRoleDesc = (GetRoleDescSnS*)pProtocol;
	GetRoleDescSnSRe getRoleDescRe;
	getRoleDescRe.sn		= pGetRoleDesc->sn;
	getRoleDescRe.timestamp	= pGetRoleDesc->timestamp;

	//将协议中的详细信息压入到GConsignRole结构中
	GSNSRole roleInfo;
	Marshal::OctetsStream os( pGetRoleDesc->detailInfo );
	os >> roleInfo;

/*	static int iIndex = 0;
	iIndex++;
	AString strFile;
	strFile.Format("./logs/role%03d", iIndex);
	FILE *pRoleFile = fopen(strFile, "wb");
	if(pRoleFile)
	{
		fwrite(pGetRoleDesc->detailInfo.begin(), 1, pGetRoleDesc->detailInfo.size(), pRoleFile);
		fclose(pRoleFile);
	}
*/
	CECRoleDescXMLParser roleDescXMLParser;
	ACString strXMLText = roleDescXMLParser.GetXMLText( &roleInfo );

	//==========================================================================
/*	FILE* pFile = fopen( "./role.xml", "w+t" );
	if ( !pFile )
	{
		a_LogOutput( 1, "Can not open role.xml!" );
		return;
	}
	char* result = new char[ strXMLText.GetLength()*2 ];
	a_WideCharToMultiByte( strXMLText, strXMLText.GetLength(), result, strXMLText.GetLength()*2 );//wcslen(pszHint)
	fwrite( result, sizeof( char ), strXMLText.GetLength()*2, pFile );
	delete []result;
	fclose(pFile);
*/
	//==========================================================================

	//初始化并设置回复协议，之后发送该协议到server
	if ( strXMLText.GetLength() == 0 )
	{
		getRoleDescRe.retCode = 1;	//解析错误
		SendNetData( getRoleDescRe );
		a_LogOutput( 1, "XML Parser Error!" );
		return;
	}

	getRoleDescRe.cls = roleInfo.gsroleinfo.rolebasicinfo.cls;
	getRoleDescRe.gender = roleInfo.gsroleinfo.rolebasicinfo.gender;
	getRoleDescRe.level = roleInfo.gsroleinfo.rolebasicinfo.level;
	ACString strClsName = roleDescXMLParser.GetClsName(roleInfo.gsroleinfo.rolebasicinfo.cls);
	getRoleDescRe.name.replace( strClsName, wcslen(strClsName) * sizeof (wchar_t) );
	ACString strIcon;
	strIcon.Format( _AL("head/%d.png"), roleInfo.gsroleinfo.rolebasicinfo.cls );
	getRoleDescRe.icon.replace( strIcon, strIcon.GetLength()*sizeof(wchar_t) );

	//basic_desc
	ACString strBasicDesc;
	strBasicDesc.Format( _AL("职业 %s\\r性别 %s\\r飞升前等级 %d %s\\r等级 %d\\r阵营 %s\\r元神等级 %d\\r攻击 %d-%d\\r防御 %d\\r气血 %d/%d\\r真气 %d/%d"),  //\\r声望 %d //---去掉声望
						strClsName,	//roleDescXMLParser.GetClsName(roleInfo.gsroleinfo.rolebasicinfo.cls),
						((roleInfo.gsroleinfo.rolebasicinfo.gender == 0) ? _AL("男") : _AL("女")),
						roleInfo.gsroleinfo.rolebasicinfo.level_before_reborn,
						((roleInfo.gsroleinfo.rolebasicinfo.level_before_reborn == 0) ? _AL("未飞升") : roleDescXMLParser.GetClsName(roleInfo.gsroleinfo.rolebasicinfo.cls_before_reborn)),
						/*roleInfo.gsroleinfo.rolebasicinfo.reputation,*/
						roleInfo.gsroleinfo.rolebasicinfo.level,
						roleDescXMLParser.GetCultivationName(roleInfo.gsroleinfo.rolebasicinfo.cultivation),
						roleInfo.gsroleinfo.deityinfo.deity_level,
						roleInfo.gsroleinfo.rolebasicinfo.damage_low,
						roleInfo.gsroleinfo.rolebasicinfo.damage_high,
						roleInfo.gsroleinfo.rolebasicinfo.defense,
						roleInfo.gsroleinfo.rolebasicinfo.max_hp,
						roleInfo.gsroleinfo.rolebasicinfo.max_hp,
						roleInfo.gsroleinfo.rolebasicinfo.max_mp,
						roleInfo.gsroleinfo.rolebasicinfo.max_mp
						);
	getRoleDescRe.basicDetailInfo.replace( strBasicDesc, strBasicDesc.GetLength()*sizeof(wchar_t) );

	//detial_desc
	getRoleDescRe.detailDesc.replace( strXMLText, strXMLText.GetLength()*sizeof(wchar_t) );
	a_LogOutput( 1, "detailDesc size:%d", strXMLText.GetLength()*sizeof(wchar_t));

	//--------------------
//	ACString strBasicDesc(_AL("1111"));
//	getRoleDescRe.basicDetailInfo.replace( strBasicDesc, strBasicDesc.GetLength()*sizeof(wchar_t) );
	//--------------------
	//返回协议
	SendNetData( getRoleDescRe );

	//清理资源
}


//=============================================================================




