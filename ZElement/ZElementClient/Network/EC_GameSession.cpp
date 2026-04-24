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

#include "AIniFile.h"
#include "EC_Global.h"
#include "gnet.h"
#include "gameclient.h"
#include "pingclient.h"
#include "EC_GameSession.h"
#include "EC_Game.h"
#include "EC_GameRun.h"
#include "EC_GPDataType.h"
#include "EC_RTDebug.h"
#include "EC_LoginUIMan.h"
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
#include "EC_IvtrTypes.h"
#include "EC_DealInventory.h"
#include "EC_IvtrItem.h"
#include "EL_Precinct.h"
#include "EC_Faction.h"
#include "AUI\\AUICTranslate.h"
#include "DlgMailWrite.h"
#include "DlgGuildMap.h"
#include "DlgGoldTrade.h"
#include "DlgLoginServerList.h"
#include "DlgFriendList.h"
#include "EC_NetProtocolBase.h"
#include "EC_PrtcPBIProc.h"
#include "TaskTemplMan.h"
#include "EC_Friend.h"
#include "EC_FunGamePrtcHandler.h"
#include "EC_HomeTownPrtcHandler.h"
#include "EC_Circle.h"
#include "EC_Cutscene.h"
#include "EC_GameUIMisc.h"
#include "EC_BattleDungeon.h"
#include "EC_Achievement.h"
#include "gt_overlay.h"

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
#include "globaldropbroadcast.hpp"
#include "globaldropremainbroadcast.hpp"
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

#include "changeds_re.hpp"
#include "playerchangeds_re.hpp"
#include "keyreestablish.hpp"
#include "gateofflinechat.hpp"
#include "ssogetticketrep.hpp"
#include "changerolename_re.hpp"
// 2013-3-1 by zy add 服务器消息
#include "topflower_err.hpp"
#include "topflower_re.hpp"
#include "topflowermsg_re.hpp"

// 2013-4-10 by ay add 
#include "OpenBanquetJoin_Re.hpp"
#include "openbanquetenter_re.hpp"
#include "openbanquetdrag_re.hpp"
#include "openbanquetqueuecount_re.hpp"
#include "openbanquetgetfield_re.hpp"
#include "openbanquetnotify_re.hpp"

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

#include "A3DConfig.h" //Added 2011-08-18.

#include "discountannounce.hpp"
#include "fastpay_re.hpp"
#include "fastpaybindinfo.hpp"
#include "teamraidapply_re.hpp"
#include "teamraidmappingsuccess.hpp"
#include "teamraidapplynotice.hpp"

#include "kingdombattleenter_re.hpp"
#include "kingdombattleleave_re.hpp"
#include "kingdomappoint_re.hpp"
#include "kingdomdischarge_re.hpp"
#include "kingdomgetinfo_re.hpp"
#include "kingdomsetgameattri_re.hpp"
#include "kingdomannounce_re.hpp"
#include "kingdomgettitle_re.hpp"
#include "kingdomtitlechange.hpp"
#include "kingdompointchange.hpp"
#include "kingdomkingchange.hpp"
#include "touchpointexchange_re.hpp"
#include "gettouchpoint_re.hpp"
#include "getpassportcashadd_re.hpp"
#include "kingcallguards_re.hpp"
#include "kingguardinvite.hrp"
#include "queenopenbath_re.hpp"
#include "queenopenbathinvite.hrp"
#include "kingissuetask_re.hpp"
#include "getkingtask_re.hpp"
#include "kinggetreward_re.hpp"

#include "acaccuse.hpp"
#include "ssogetticketreq.hpp"

#include "getfactiondynamic_re.hpp"
#include "getfactionmsg_re.hpp"
#include "factionpostmsg_re.hpp"
#include "factiondynamic.h"
#include "createfacbase_re.hpp"
#include "facbaseenter_re.hpp"
#include "getfacacttoptable_re.hpp"

#include "crssvrteamschgcaptain.hpp"
#include "crssvrteamsdismiss.hpp"
#include "crssvrteamsgetrolelist.hpp"
#include "crssvrteamsrolelist_re.hpp"
#include "crssvrteamsinvite.hrp"
#include "crssvrteamsleave.hpp"
#include "crssvrteamsnotify_re.hpp"
#include "crssvrteamsrecruit.hpp"
#include "crssvrteamsrename.hpp"
#include "crssvrteamskick.hpp"
#include "crssvrteamsteambase_re.hpp"
#include "teamraidactiveapplynotice.hpp"
#include "crssvrteamssearch_re.hpp"
#include "crssvrteamsrolenotify_re.hpp"
#include "getkdctoken_re.hpp"
#include "facbasestopnotice.hpp"

#include "DlgFastPay.h"
#include "DlgFastPayCode.h"
#include "DlgBaseExplorer.h"
#include "DlgFlower.h"
#include "DlgKingWar.h"
#include "DlgBattleFlow.h"
#include "DlgHost.h"
#include "DlgCrssvrTeams.h"
#include "DlgGuildMain.h"
#include "DlgCrssvrTeamsSub.h"
#include "DlgGuildShop.h"
extern game_record gr;

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

extern abase::vector<ACRemoteCode*> g_DefenceCodeVec;
extern CRITICAL_SECTION g_csDefence;

///////////////////////////////////////////////////////////////////////////
//	
//	Local Types and Variables and Global variables
//	
///////////////////////////////////////////////////////////////////////////

static bool l_bExitConnect = false;

///////////////////////////////////////////////////////////////////////////
//	
//	Local functions
//	
///////////////////////////////////////////////////////////////////////////

//消息字符串解析Added 2011-03-31.
void MsgStrParser( ACString& strMsg, const ACString strPattern, long value )
{
	//如果value值为0，则此时没有解析和替换的模式
	if( !value )
	{
		return;
	}

	//设置value为格式化字符串
	ACString strValue;
	strValue.Format( _AL("%d"), value );

	//查找模式（需要替换的字符串子串）
	int nSub = strMsg.Find( strPattern );
	while ( nSub != -1 )
	{
		strMsg = strMsg.Left(nSub) + strValue + strMsg.Right(strMsg.GetLength() - nSub - strPattern.GetLength());
		nSub = strMsg.Find( strPattern );
	}

}
//Added end.

//-----------------------------------------------------------------------------
//怪物喊话内容解析，处理相应表情变量 Added 2012-04-25.
void MonsterStrParser( ACString& strMsg )
{
	if ( strMsg.IsEmpty() )
	{
		return;
	}

	const int MAX_VALUE_LENGTH = 2;
	ACString strPattern = _AL("&");
	int nSubLeft = -1, nSubRight = -1, nStart = 0;
	while ( nStart != -1 )
	{
		nSubLeft = strMsg.Find( strPattern, nStart );
		if ( nSubLeft != -1 )	//左边&查找成功，开始查找右边&并进一步处理
		{
			nStart = nSubLeft + 1;
			nSubRight = strMsg.Find( strPattern, nStart );
			if ( nSubRight != -1 )	//右边&查找成功，判断&&之间的数字合法性并插入表情控制代码
			{
				int nSubLength = nSubRight - nSubLeft - 1;
				if ( nSubLength > 0 && nSubLength <= MAX_VALUE_LENGTH )	//数字最多2个
				{
					ACString strIndex( (ACHAR*)(&strMsg[nStart]), nSubLength );
					ACHAR ch = 57344;
					ACString strCode = _AL("");
					strCode += ch;
					ACString strFace;
					strFace.Format( _AL("<0><0:%s>"), strIndex );
					strCode += strFace;	//固定使用第0套表情 _AL("<0><0:10>");
					strMsg = strMsg.Left( nSubLeft ) + strCode + strMsg.Right( strMsg.GetLength() - nSubLeft - nSubLength - 2 );
					nStart = nSubLeft + strCode.GetLength();	//计算调整后的整体字符串的开始搜索位置
				}
				else	//数字个数为0或者超过2个
				{
					nStart = nSubRight + 1;
				}
			}
			else	//右边查找失败，说明不配对，跳出查找
			{
				nStart = -1;
			}
		}
		else	//左边&查找失败
		{
			nStart = -1;	//跳出循环
		}
	}

}

//-----------------------------------------------------------------------------

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
		long lVariables[MAX_VARIABLE_NUM];	//服务器传过来的变量值，当前暂定义为全局key/value的value值 Added 2011-03-31
		char factionname[20];
	};

	if (sz >= sizeof(_data))
	{
		_data* pData = (_data*)pBuf;
		CECGameUIMan* pGameUI = (CECGameUIMan*)g_pGame->GetGameRun()->GetUIManager()->GetInGameUIMan();
		ATaskTempl* pTempl = g_pGame->GetTaskTemplateMan()->GetTaskTemplByID(pData->task_id);

		//首先初始化喊话内容变量
		ACString strMsg = _AL("");

		//检查到底是普通奖励的系统喊话还是额外奖励的系统喊话
		if( !pData->param && pTempl && pTempl->GetTribute() )//目前商议：以data中传过来的param为0：普通奖励 最高位为1：额外奖励
		{
			strMsg = pTempl->GetTribute();
		}
		else if( (0x80000000 & pData->param) && pTempl )//最高位为1，这说明需要计算额外奖励的情况
		{
			AWARD_DATA ad;
			pTempl->GetAwardData(pData->param, &ad);//根据服务器对当前任务的完成情况的判断（通过param）来得到奖励数据

			//如果奖励中的额外奖励系统喊话内容不为空
			if( ad.m_pwstrExtraTribute )
			{
				strMsg = ad.m_pwstrExtraTribute;
			}
		}

		//判断是否有喊话内容
		if( !strMsg.IsEmpty() )
		{
			wchar_t szName[11];
			szName[10] = 0;
			wcsncpy(szName, (wchar_t*)pData->name, 10);
			//ACString strMsg = pTempl->GetTribute();
			int nSub = strMsg.Find(_AL("$name"));
			while (nSub != -1)
			{
				strMsg = strMsg.Left(nSub) + szName + strMsg.Right(strMsg.GetLength() - nSub - 5);
				nSub = strMsg.Find(_AL("$name"));
			}

			wchar_t szFactionName[11];
			szFactionName[10] = 0;
			wcsncpy(szFactionName, (wchar_t*)pData->factionname, 10);
			nSub = strMsg.Find(_AL("$faction"));
			while (nSub != -1)
			{
				strMsg = strMsg.Left(nSub) + szFactionName + strMsg.Right(strMsg.GetLength() - nSub - 8);
				nSub = strMsg.Find(_AL("$faction"));
			}

			ACString strLine = pGameUI->GetLine(pData->line_id);
			nSub = strMsg.Find(_AL("$line"));
			while (nSub != -1)
			{
				strMsg = strMsg.Left(nSub) + strLine + strMsg.Right(strMsg.GetLength() - nSub - 5);
				nSub = strMsg.Find(_AL("$line"));
			}

			//解析全局变量相关通配符字符串 Added 2011-03-31
			ACString strPattern[] = { _AL("&var1&"), _AL("&var2&"), _AL("&var3&") };
			for ( int i = 0; i < MAX_VARIABLE_NUM; ++i )
			{
				MsgStrParser( strMsg, strPattern[i], pData->lVariables[i] );
			}

			g_pGame->GetGameRun()->AddChatMessage(strMsg, GP_CHAT_SYSTEM/*pData->channel*/); // 策划需求任务不分频道2010.6.25 fuchonggang
		}
	}
}

///////////////////////////////////////////////////////////////////////////
//	
//	Implement CECGameSession
//	
///////////////////////////////////////////////////////////////////////////

CECGameSession::CECGameSession() :
m_aNewProtocols(128, 128),
m_aOldProtocols(128, 128),
m_Ping(50, 50)
{
	if (!(m_pNetMan = &GNET::GameClient::GetInstance()))
	{
		ASSERT(m_pNetMan);
		a_LogOutput(1, "CECGameSession::Open, Failed to create net manager");
	}

	m_pNetMan->SetZoneID(0);
	m_pNetMan->Attach(IOCallBack);

	GNET::PingClient::GetInstance().Attach(IOCallBack);

	m_idLink			= (DWORD)(-1);
	m_iUserID			= 0;
	m_iCharID			= 0;
	m_bLinkBroken		= false;
	m_bConnected		= false;
	m_pS2CCmdLog		= NULL;
	m_pS2CCmdBinLog		= NULL;
	m_hConnectThread	= NULL;
	m_bKickUser			= true;
	m_iiAccountTime		= 0;
	m_iiFreeGameTime	= 0;
	m_iFreeGameEnd		= 0;
	m_dwLastAccTime		= 0;
	m_bProcNewPtrc		= false;

	m_pPBIProc	= new CECPrtcPBIProc;

	m_pfnHelpStatesCallBack = NULL;
	memset(&m_ot, 0, sizeof (m_ot));
	m_iDistrictID	= 0;
	m_iReferrer		= 0;
	m_zoneID		= 0;
	m_iSrcZoneID	= 0;
	m_iDstZoneID	= 0;
	m_iSrcLine		= 0;
	m_iDstLine		= 0;
	m_bCrossZone	= false;
	m_cCrossZoneFlag	= 0;
	m_pFunGamePrtcHandler = new CECFunGamePrtcHandler;
	m_pHomeTownPrtcHandler = new CECHomeTownPrtcHandler;
	InitializeCriticalSection(&m_csPingSection);
}

CECGameSession::~CECGameSession()
{
	GNET::PingClient::GetInstance().Detach();

	if (m_pPBIProc)
		delete m_pPBIProc;

	if( m_pNetMan )
	{
		m_pNetMan->Detach();
		m_pNetMan = NULL;
	}

	for (int i = 0; i < m_Ping.GetSize(); i++)
		delete m_Ping[i];

	m_ErrorMsgs.Release();
	delete m_pFunGamePrtcHandler;
	delete m_pHomeTownPrtcHandler;
	DeleteCriticalSection(&m_csPingSection);
}

//	Open session
bool CECGameSession::Open()
{
	//	Connect server
	Connect();

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
	m_nS2CCmdSize = 0;
	return true;
}

//	Close sesssion
void CECGameSession::Close()
{
	m_pfnHelpStatesCallBack = NULL;
	m_bCrossZone		= false;
	m_iSrcLine			= 0;
	m_iDstLine			= 0;

	// 恢复源服务器地址
	if (m_iSrcZoneID)
	{
		CECGame::SERVERINFO* pInfo = g_pGame->GetSrcServerInfo(m_iSrcZoneID);	
		CECGame::GAMEINIT& gameinit = g_pGame->GetGameInit();
		
		int index = a_Random(0, pInfo->vecAddress.size()-1);
		strncpy(gameinit.szIP, pInfo->vecAddress[index].GetBuffer(0), pInfo->vecAddress[index].GetLength());
		gameinit.szIP[pInfo->vecAddress[index].GetLength()] = 0;
		
		index = a_Random(0, pInfo->vecPorts.size()-1);
		gameinit.iPort = pInfo->vecPorts[index];
		gameinit.iServerID = pInfo->iServerID;
		strncpy(gameinit.szServerName, pInfo->szServerName, 256);
		// 有可能是小退，需要更新服务器显示，也可能是大退，GameRun已经被释放了
		CECGameRun* pGameRun = g_pGame->GetGameRun();
		if (pGameRun)
		{
			CECLoginUIMan * pLoginUIMan = pGameRun->GetUIManager()->GetLoginUIMan();
			if (pLoginUIMan)
			{
				CDlgLoginServerList * pDlg = (CDlgLoginServerList*)pLoginUIMan->GetDialog("Win_Login");
				if (pDlg)
					pDlg->GetDlgItem("Txt_CurrentServer")->SetText(AS2AC(gameinit.szServerName));
			}
		}	
	}

	if (m_hConnectThread)
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

	//	Release all processed protocols
	ClearOldProtocols();

	::EnterCriticalSection(&g_csDefence);
	for (size_t j = 0; j < g_DefenceCodeVec.size(); j++)
		g_DefenceCodeVec[j]->Destroy();
	g_DefenceCodeVec.clear();
	::LeaveCriticalSection(&g_csDefence);

	m_bConnected		= false;
	m_iiAccountTime		= 0;
	m_iiFreeGameTime	= 0;
	m_iFreeGameEnd		= 0;
	m_pFunGamePrtcHandler->OnSessionClose();
	m_pHomeTownPrtcHandler->OnSessionClose();
}

DWORD WINAPI CECGameSession::ConnectThread(LPVOID pArg)
{
	CECGameSession* pGameSession = (CECGameSession*)pArg;

	const CECGame::GAMEINIT& gi = g_pGame->GetGameInit();
	if (gi.szIP[0] && gi.iPort)
	{
		g_pGame->GetGameRun()->RegisterIP(gi.szIP);
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
	StackChecker::ACTrace(8);

	if (gr.get_state() == game_record::enum_gr_replay)
		return true;

	if (!m_pNetMan || m_idLink == (DWORD)(-1))
	{
		ASSERT(m_pNetMan && m_idLink != (DWORD)(-1));
		return false;
	}
	
	return m_pNetMan->Send(m_idLink, p, bUrg);
}

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

//	Start / End overtime
void CECGameSession::DoOvertimeCheck(bool bStart, int iCheckID, DWORD dwTime)
{
	if (gr.get_state() == game_record::enum_gr_replay)
		return;

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
			m_Ping[idServer]->ui64Status = 0;
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
		pt->ui64Status = 0;
		m_Ping.Add(pt);
	}
	PINGTIME* pt = new PINGTIME;
	pt->idLink = GNET::PingClient::GetInstance().Ping(addr, nPort);
	pt->dwTime = 99999999;
	pt->dwLastPingTime = GetTickCount();
	pt->ui64Status = 0;
	m_Ping.Add(pt);
	LeaveCriticalSection(&m_csPingSection);
}

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
		pSession->SetLinkID(idLink);
		pSession->m_bConnected	= true;
		pSession->m_bLinkBroken	= false;
		const CECGame::GAMEINIT& gi = g_pGame->GetGameInit();
		g_pGame->AddIPAddressByName(gi.szIP, pNetMan->GetPeer().c_str());
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
		return 0;
	}
	case EVENT_ABORTSESSION:
	{	
		// connect to server failed, so pop up a message window
		// make overtime check fire.
		pSession->DoOvertimeCheck(true, CECGameSession::OT_CHALLENGE, 0);
		
		pSession->m_bConnected = false;
		g_pGame->RuntimeDebugInfo(RTDCOL_NETWORK, _AL("NET - Connection Failed !"));
		return 0;
	}
	case EVENT_PINGRETURN:
	{
		EnterCriticalSection(&pSession->m_csPingSection);
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
				pSession->m_Ping[i]->ui64Status = status->attr._attr;
				break;
			}
		LeaveCriticalSection(&pSession->m_csPingSection);

		return 0;
	}
	case EVENT_DISCONNECT:
	{
		a_LogOutput(1, "CECGameSession::IOCallBack, EVENT_DELSESSION, error code = %s", pSession->m_pNetMan->GetLastError());
		return 0;
	}

	}

	GNET::Protocol* pProtocol = (GNET::Protocol*)pData;

	if (gr.get_state() == game_record::enum_gr_replay && g_pGame->GetGameRun()->GetGameState() == CECGameRun::GS_GAME)
		pProtocol->Destroy();
	else
		pSession->AddNewProtocol(pProtocol);

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

//	When connection was broken, this function is called
void CECGameSession::LinkBroken(bool bDisconnect)
{
	if (g_pGame->GetGameRun()->GetUIManager())
	{
		const static char* szDlgName = "MsgBox_LinkBroken";
		CECGameRun* pGameRun = g_pGame->GetGameRun();
		CECUIManager* pUIMan = pGameRun->GetUIManager();
		

		if (pGameRun->GetGameState() == CECGameRun::GS_LOGIN)
		{
// 			if (pGameRun->IsCrossing())
// 			{
// 				pGameRun->EndCrossAnimate();
// 				pGameRun->SetLogoutFlag(2);
// 			}
			if (m_bCrossZone)
			{
				pGameRun->SetLogoutFlag(2);
			}
			else if (!pUIMan->GetLoginUIMan()->GetDialog(szDlgName))
			{
				int iMsg = bDisconnect ? FIXMSG_DISCONNECTED : FIXMSG_FAILEDTOCONN;
				pUIMan->ShowErrorMsg(g_pGame->GetFixedMsgTab()->GetWideString(iMsg), szDlgName);
			}
		}
		else if (pGameRun->GetGameState() == CECGameRun::GS_GAME)
		{
// 			if (pGameRun->IsCrossing())
// 			{
// 				pGameRun->EndCrossAnimate();
// 				pGameRun->SetLogoutFlag(2);
// 			}
			if (m_bCrossZone)
			{
				pGameRun->SetLogoutFlag(2);
			}
			else if (!pUIMan->GetInGameUIMan()->GetDialog(szDlgName))
			{
				if (bDisconnect)
					pUIMan->GetInGameUIMan()->MessageBox(szDlgName, g_pGame->GetFixedMsgTab()->GetWideString(FIXMSG_DISCONNECTED), MB_YESNO);
				else
					pUIMan->ShowErrorMsg(g_pGame->GetFixedMsgTab()->GetWideString(FIXMSG_FAILEDTOCONN), szDlgName);
			}

		}
		else if (pGameRun->GetGameState() == CECGameRun::GS_NONE)
		{
// 			if (pGameRun->IsCrossing())
// 				pGameRun->EndCrossAnimate();
			pGameRun->SetLogoutFlag(2);
		}
	}
	
	m_bLinkBroken = false;
}
void CECGameSession::SetUserName(const char* szName,int suffix /*= USERNAME_SUFFIX_NONE*/)
{
	m_strRawUserName = szName;
	m_strUserName = szName; 
	
	if(suffix == USERNAME_SUFFIX_XGXQQ)
	{
	   m_strUserName += "@xgxqq";
	}
	else if(suffix == USERNAME_SUFFIX_XGXTW)
	{
	   m_strUserName += "@xgxtw";
	}
	
}
//	Process one protocol
bool CECGameSession::ProcessOneProtocol(CECNetProtocolBase* pRaw)
{
	GNET::Protocol* pProtocol = pRaw->GetRawProtocol();
	bool bAddToOld = true;

	CECGameUIMan* pGameUI = (CECGameUIMan*)g_pGame->GetGameRun()->GetUIManager()->GetInGameUIMan();

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
	case PROTOCOL_PLAYERCHANGEDS_RE:		OnPrtcPlayerChangeDsRe(pProtocol);		break;
	case PROTOCOL_CHANGEDS_RE:				OnPrtcChangeDsRe(pProtocol);			break;

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
	case PROTOCOL_FRIENDNAMECHANGE:

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
	case PROTOCOL_CHANGEFACTIONNAME_RE:

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

	// 副本相关 Added 2011-07-21.
	case PROTOCOL_RAIDJOINREQ:
	case PROTOCOL_RAIDOPEN_RE:
	case PROTOCOL_RAIDJOIN_RE:
	case PROTOCOL_RAIDJOINAPPLYLIST:
	case PROTOCOL_RAIDJOINACCEPT_RE:
	case PROTOCOL_RAIDENTER_RE:
	case PROTOCOL_RAIDGETLIST_RE:
	case PROTOCOL_RAIDSTART_RE:
	case PROTOCOL_RAIDAPPOINT_RE:
	case PROTOCOL_RAIDAPPOINTNOTIFY:
	case PROTOCOL_RAIDSTARTNOTIFY:
	case PROTOCOL_RAIDKICK_RE:
	case PROTOCOL_RAIDKICKNOTIFY:
	case PROTOCOL_RAIDQUIT_RE:
	case PROTOCOL_RAIDGETROOM_RE:
	case PROTOCOL_RAIDOPENVOTE_RE:
	case PROTOCOL_RAIDBEGINVOTES:
	case PROTOCOL_RAIDVOTERESULT:
	case PROTOCOL_RAIDBRIEFINFO:
		OnRaidProtocols(pProtocol);
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
	case RPC_CRSSVRTEAMSINVITE:
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

	case RPC_KINGGUARDINVITE:
	case RPC_QUEENOPENBATHINVITE:

		OnKingdomPkOpt(pProtocol);
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

	case PROTOCOL_CONSIGNCANCELPOST_RE:
		{
			CECGameUIMan* pGameUI = (CECGameUIMan*)g_pGame->GetGameRun()->GetUIManager()->GetInGameUIMan();
			pGameUI->CommonMessage("Win_ShopConsign", "calcel_re", (int)pProtocol, 0);
			break;
		}
	case PROTOCOL_CONSIGNQUERY_RE:
		{
			CECGameUIMan* pGameUI = (CECGameUIMan*)g_pGame->GetGameRun()->GetUIManager()->GetInGameUIMan();
			pGameUI->CommonMessage("Win_ShopConsign", "query_re", (int)pProtocol, 0);
			break;
		}
	case PROTOCOL_CONSIGNSTARTERR:
		{
			CECGameUIMan* pGameUI = (CECGameUIMan*)g_pGame->GetGameRun()->GetUIManager()->GetInGameUIMan();
			pGameUI->CommonMessage("Win_ShopConsign", "server_error", (int)pProtocol, 0);
			break;
		}
	case PROTOCOL_CONSIGNLISTALL_RE:
		{
			CECGameUIMan* pGameUI = (CECGameUIMan*)g_pGame->GetGameRun()->GetUIManager()->GetInGameUIMan();
			pGameUI->CommonMessage("Win_ShopConsignView", "listall_re", (int)pProtocol, 0);
			break;
		}
	case PROTOCOL_CONSIGNGETITEM_RE:
		{
			CECGameUIMan* pGameUI = (CECGameUIMan*)g_pGame->GetGameRun()->GetUIManager()->GetInGameUIMan();
			pGameUI->CommonMessage("Win_ShopConsignView", "getitem_re", (int)pProtocol, 0);
			break;	
		}
	case PROTOCOL_CONSIGNLISTLARGECATEGORY_RE:
		{
			CECGameUIMan* pGameUI = (CECGameUIMan*)g_pGame->GetGameRun()->GetUIManager()->GetInGameUIMan();
			pGameUI->CommonMessage("Win_ShopConsignView" , "getLargeCategory_re" , (int)pProtocol , 0);
			break;
		}
	//好友召回相关协议处理
	case PROTOCOL_FRIENDCALLBACKINFO_RE:
	case PROTOCOL_FRIENDCALLBACKMAIL_RE:
	case PROTOCOL_FRIENDCALLBACKAWARD_RE:
		OnFriendCallBckPrtcls(pProtocol);
		break;
	case PROTOCOL_GATEOFFLINECHAT:
		{
			OnPrtcGTOffLineChat(pProtocol);
			break;
		}
	case PROTOCOL_GATEGETGTROLESTATUS_RE:
		{
			CECGameUIMan* pGameUI = (CECGameUIMan*)g_pGame->GetGameRun()->GetUIManager()->GetInGameUIMan();
			pGameUI->CommonMessage("Win_FriendList" , "get_status" , (int)pProtocol , 0);
		
			break;
		}
	case PROTOCOL_UPDATEFRIENDGTSTATUS:
		{
			CECGameUIMan* pGameUI = (CECGameUIMan*)g_pGame->GetGameRun()->GetUIManager()->GetInGameUIMan();
			pGameUI->CommonMessage("Win_FriendList" , "update_status" , (int)pProtocol , 0);
		
			break;
		}
	case PROTOCOL_GETKDCTOKEN_RE:
		{
			CECGameUIMan* pGameUI = (CECGameUIMan*)g_pGame->GetGameRun()->GetUIManager()->GetInGameUIMan();
			pGameUI->CommonMessage("Win_ZhuxianKnows" , "set_token" , (int)pProtocol , 0);
			pGameUI->CommonMessage("Win_Guild_Bbs" , "set_token" , (int)pProtocol , 0);
			AString szURL = AString("http://www.17y.com/zx2web/embedded/pushinfo?t=%s&time=%d");
			AString strUrl;
			AString m_strToken;
			GNET::GetKDCToken_Re* p = (GNET::GetKDCToken_Re*)pProtocol;
			if (p->retcode==0)
			{
				m_strToken =  AString((const char*)p->token.begin(), p->token.size() / sizeof (char));
				strUrl.Format(szURL,m_strToken,g_pGame->GetServerGMTTime());
				CDlgBaseExplorer* pDlg = (CDlgBaseExplorer*)pGameUI->GetDialog("win_explorer");
				pDlg->NavigateUrl(strUrl);
			}
			break;
		}
	case PROTOCOL_CONSIGNPLAYER_RE:
		{
			CECGameUIMan* pGameUI = (CECGameUIMan*)g_pGame->GetGameRun()->GetUIManager()->GetInGameUIMan();
			pGameUI->CommonMessage("Win_ShopConsign" , "consign_player_re" , (int)pProtocol , 0);
			
			break;
		}
	case PROTOCOL_CONSIGNLISTROLE_RE:
		{
			CECGameUIMan* pGameUI = (CECGameUIMan*)g_pGame->GetGameRun()->GetUIManager()->GetInGameUIMan();
			pGameUI->CommonMessage("Win_ShopConsignView" , "role_list" , (int)pProtocol , 0);
			
			break;
		}
	case PROTOCOL_SSOGETTICKETREP:
		{
			GNET::SSOGetTicketRep* p = (GNET::SSOGetTicketRep*)pProtocol;
			AIniFile theIni;
			AString szURL = "";
			AString strContext;
			strContext = AString((char*)p->local_context.begin(), p->local_context.size());
			if (strContext=="gt")
			{				
				AString strTicket((char*)p->ticket.begin(), p->ticket.size());
				overlay::GTOverlay::Instance().Login(g_pGame->GetGameSession()->GetZoneID(), 
					g_pGame->GetGameRun()->GetHostPlayer()->GetCharacterID(), g_pGame->GetGameSession()->GetUserName(), strTicket);
				break;
			}
			if( theIni.Open("configs\\url.ini") )
			{
				if (strContext=="accountcomplete")
				{
					if((g_pGame->GetGameRun()->GetPlatformMask()&0x01) > 0)
						szURL = theIni.GetValueAsString("ingame", "accountcomplete1", "");
					else
						szURL = theIni.GetValueAsString("ingame", "accountcomplete2", "");
				}
				else if (strContext=="payment")
					szURL = theIni.GetValueAsString("ingame", "payment", "");
				else if (strContext== "fastpay")
					szURL = theIni.GetValueAsString("ingame", "fastpay", "");
				else if (strContext== "webhot")
					szURL = theIni.GetValueAsString("ingame", "webhot", "");
				else if(strContext == "touch")
					szURL = theIni.GetValueAsString("ingame", "touch", "");
				else if(strContext == "webshop")
					szURL = theIni.GetValueAsString("ingame", "webshop", "");
				else if(strContext == "themelink")
					szURL = theIni.GetValueAsString("ingame", "zhutizhan", "");
				theIni.Close();
			}
			if( szURL != "" )
			{
				AString strUrl;
				AString strTicket, strAccount;
				strTicket = AString((char*)p->ticket.begin(), p->ticket.size());
				strAccount = AString((char*)p->user.account.begin(), p->user.account.size());
				strUrl.Format(szURL, strTicket, strAccount);
				CECGameUIMan* pGameUI = (CECGameUIMan*)g_pGame->GetGameRun()->GetUIManager()->GetInGameUIMan();
				if (strContext=="accountcomplete")
				{
					PAUIDIALOG pDlg;
					pGameUI->MessageBox("Game_FinishInfo", pGameUI->GetStringFromTable(15000), MB_YESNO,
						A3DCOLORRGBA(255, 255, 255, 160), &pDlg);
					pDlg->SetDataPtr(new AString(strUrl), "url");
				}
				else if (strContext=="webhot")
				{
					CDlgBaseExplorer* pDlg = (CDlgBaseExplorer*)pGameUI->GetDialog("win_explorer_ChargeAward");
					pDlg->Show(true);
					pDlg->NavigateUrl(strUrl);
				}
				else if (strContext=="payment" || strContext=="fastpay")
					pGameUI->CommonMessage("Win_QShop", "open_url", int(&strUrl), 0);
				else if(strContext == "touch")
				{
					AString strUrl;
					strUrl.Format(szURL,strAccount,strTicket);
				    pGameUI->OpenUrl(strUrl);
				}
				else if(strContext == "webshop")
				{
					pGameUI->OpenUrl(strUrl);
				}
				else if(strContext == "themelink")
				{
					//strUrl.Format(szURL,strTicket);
					//CDlgBaseExplorer* pDlg = (CDlgBaseExplorer*)pGameUI->GetDialog("win_explorer");
					//pDlg->Show(true);
					//pDlg->NavigateUrl(strUrl);
				}
			}

			break;
		}

	case PROTOCOL_DISCOUNTANNOUNCE:
		{
			GNET::DiscountAnnounce* p = (GNET::DiscountAnnounce*)pProtocol;

			CECGameUIMan* pGameUI = (CECGameUIMan*)g_pGame->GetGameRun()->GetUIManager()->GetInGameUIMan();
			CDlgFastPay* pDlg = (CDlgFastPay*)pGameUI->GetDialog("Win_Qshop_QuickInputMoney");
			DISCOUNT_GRADE discountgrade;
			
			for (int i=0; i < (int)p->discount.size(); i++)
			{
				GNET::MerchantDiscount& merchantdiscount = p->discount[i];
				abase::vector<DISCOUNT_GRADE> *pvecdiscountgrade = new abase::vector<DISCOUNT_GRADE>; 
				for(int j = 0; j < (int)merchantdiscount.discount.size(); j++)
				{
					discountgrade.amount_begin = merchantdiscount.discount[j].amount_begin;
					discountgrade.discount = merchantdiscount.discount[j].discount;
					pvecdiscountgrade->push_back(discountgrade);
				}
					
				pDlg->AddMerchantDiscount(merchantdiscount.id, pvecdiscountgrade);			
			}
			
			break;
		}
	case PROTOCOL_FASTPAY_RE:
		{
			GNET::FastPay_Re* p = (GNET::FastPay_Re*)pProtocol;
			g_pGame->GetGameRun()->AddChatMessage(pGameUI->GetStringFromTable(15110+p->retcode), GP_CHAT_MISC);
			
			CECGameUIMan* pGameUI = (CECGameUIMan*)g_pGame->GetGameRun()->GetUIManager()->GetInGameUIMan();
			CDlgFastPayCode* pDlg = (CDlgFastPayCode*)pGameUI->GetDialog("Win_Qshop_QuickInputCode");
			if(p->retcode == 0) // success
				pDlg->Show(true);
			else
				pDlg->Show(false);

			break;
		}
	case PROTOCOL_FASTPAYBINDINFO:
		{
			GNET::FastPayBindInfo* p = (GNET::FastPayBindInfo*)pProtocol;
			CDlgFastPay* pDlg = (CDlgFastPay*)pGameUI->GetDialog("Win_Qshop_QuickInputMoney");
			
			for(int i=0;i<(int)p->merchantlist.size();i++)
			{
				pDlg->AddBindInfo(p->merchantlist[i]);
			}

			break;
		}
	case PROTOCOL_GLOBALDROPBROADCAST:
		{
			GNET::GlobalDropBroadCast* p = (GNET::GlobalDropBroadCast*)pProtocol;
			g_pGame->GetGameRun()->DropSpeaking(p->itemid, p->remainnumber, 1234);
			break;
		}
	case PROTOCOL_GLOBALDROPREMAINBROADCAST:
		{
			GNET::GlobalDropRemainBroadCast* p = (GNET::GlobalDropRemainBroadCast*)pProtocol;
			g_pGame->GetGameRun()->DropSpeaking(p->itemid, p->remainnumber);
			break;			
		}

	case PROTOCOL_QUERYROLENAMEHISTORY_RE:
		{
			CECGameUIMan* pGameUI = (CECGameUIMan*)g_pGame->GetGameRun()->GetUIManager()->GetInGameUIMan();
			pGameUI->CommonMessage("Win_Char_ChangeName_History", "hist_info", (int)pProtocol, 0);
			break;
		}
	case PROTOCOL_CHANGEROLENAME_RE:
		{
			GNET::ChangeRolename_Re *p = (GNET::ChangeRolename_Re*)pProtocol;
			if (p->retcode!=0)
			{
				OutputLinkSevError(p->retcode);
			}
			else
			{
				ACString strName((const ACHAR*)p->newname.begin(), p->newname.size()/sizeof(ACHAR));
				CECHostPlayer* pHost = g_pGame->GetGameRun()->GetHostPlayer();
				pHost->SetName(strName);
			}
			break;
		}
	// 6v6副本
	case PROTOCOL_TEAMRAIDAPPLY_RE:			// 加入回复
	case PROTOCOL_TEAMRAIDMAPPINGSUCCESS:	// 配对成功，弹出选择界面
	case PROTOCOL_TEAMRAIDBEGINCHOOSECHAR:	// 
	case PROTOCOL_TEAMRAIDACKTIMEOUT:
	case PROTOCOL_TEAMRAIDUPDATECHAR:
	case PROTOCOL_TEAMRAIDAPPLYNOTICE:
	case PROTOCOL_TEAMRAIDACTIVEAPPLYNOTICE:
		OnPrtcTeamRade(pProtocol);
		break;
	case PROTOCOL_FACTIONPKRAIDCHALLENGE_RE:
	case PROTOCOL_FACTIONPKRAIDINVITE:    
	case PROTOCOL_FACTIONPKRAIDBEGIN:   
	case PROTOCOL_FACTIONPKRAIDGETSCORE_RE:       
	case PROTOCOL_FACTIONPKRAIDGETLIST_RE:      
	case PROTOCOL_FACTIONPKRAIDSTOPFIGHT:  
    case PROTOCOL_FACTIONPKRAIDCHALLENGECANCEL_RE:
	case PROTOCOL_FACTIONWITHDRAWBONUS_RE:
		OnFactionPKRaid(pProtocol);
		break;
	case PROTOCOL_KINGDOMBATTLEENTER_RE:
	case PROTOCOL_KINGDOMBATTLELEAVE_RE:
	case PROTOCOL_KINGDOMANNOUNCE_RE:
	case PROTOCOL_KINGDOMAPPOINT_RE:
	case PROTOCOL_KINGDOMDISCHARGE_RE:
	case PROTOCOL_KINGDOMSETGAMEATTRI_RE:
	case PROTOCOL_KINGDOMGETINFO_RE:
	case PROTOCOL_KINGDOMGETTITLE_RE:
	case PROTOCOL_KINGDOMTITLECHANGE:
	case PROTOCOL_KINGDOMPOINTCHANGE:
	case PROTOCOL_KINGDOMKINGCHANGE:
	case PROTOCOL_KINGCALLGUARDS_RE:
	case PROTOCOL_QUEENOPENBATH_RE:
	case PROTOCOL_KINGISSUETASK_RE:
	case PROTOCOL_GETKINGTASK_RE:
	case PROTOCOL_KINGGETREWARD_RE:
		OnKingdomPkOpt(pProtocol);
		break;
	case PROTOCOL_GETTOUCHPOINT_RE:
	case PROTOCOL_GETPASSPORTCASHADD_RE:
	case PROTOCOL_TOUCHPOINTEXCHANGE_RE:
		OnTouchOpt(pProtocol);
        break;
	case PROTOCOL_GETCOMMONDATA_RE:
		{
			CECGameUIMan* pGameUI = (CECGameUIMan*)g_pGame->GetGameRun()->GetUIManager()->GetInGameUIMan();
			pGameUI->CommonMessage("Win_Fly_Prgs", "fly_prg_info", (int)pProtocol, 0);
			break;
		}
		break;
	case PROTOCOL_TOPFLOWER_RE://2013-3-1 by zy 添加服务器消息
		{
			OnTopFlower(pProtocol);
		}
		break;
	case PROTOCOL_TOPFLOWER_ERR: //2013-3-1 by zy 添加服务器消息
		{
			GNET::TopFlower_Err* p = (GNET::TopFlower_Err*)pProtocol;
			int nErr = p->retcode;
			CECGameUIMan* pGameUIMan = g_pGame->GetGameRun()->GetUIManager()->GetInGameUIMan();
			pGameUIMan->MessageBox(17800 + nErr);
		}
		break;
	case PROTOCOL_TOPFLOWERMSG_RE:
		{
			OnTopFlowerMsg(pProtocol);
		}
		break;
	case PROTOCOL_OPENBANQUETJOIN_RE:
		{
			OnPrtcFlowBattleJoinRe(pProtocol);
		}
		break;
	case PROTOCOL_OPENBANQUETENTER_RE:
		{
			OnPrtcFlowBattleEnterRe(pProtocol);
		}
		break;
	case PROTOCOL_OPENBANQUETDRAG_RE:
		{
			OnPrtcFlowBattleDragEnterRe(pProtocol);
		}
		break;
	case PROTOCOL_OPENBANQUETQUEUECOUNT_RE:
		{
			OnPrtcFlowBattleQueueCountRe(pProtocol);
		}
		break;
	case PROTOCOL_OPENBANQUETGETFIELD_RE:
		{
			OnPrtcFlowBattleGetFieldRe(pProtocol);
		}
		break;
	case PROTOCOL_OPENBANQUETNOTIFY_RE:
		{
			OnPrtcFlowBattleNotifyRe(pProtocol);
		}
		break;
	case PROTOCOL_ACACCUSE_RE:
		{
			CECGameUIMan* pGameUI = (CECGameUIMan*)g_pGame->GetGameRun()->GetUIManager()->GetInGameUIMan();
			pGameUI->AddChatMessage(pGameUI->GetStringFromTable(18211), GP_CHAT_MISC);
			break;
		}
		break;
	case PROTOCOL_GETFACTIONDYNAMIC_RE:
		{
			OnPrtcFaciondynamicRe(pProtocol);
		}
		break;
	case PROTOCOL_GETFACTIONMSG_RE:
		{
			OnPrtcFaciondMsgRe(pProtocol);
		}
		break;
	case PROTOCOL_FACTIONPOSTMSG_RE:
		{
			OnPrtcFactionPostMsg(pProtocol);
		}
		break;
	case PROTOCOL_CREATEFACBASE_RE:
		{
			OnPrtcCreateFacBase_Re(pProtocol);
		}
		break;
	case PROTOCOL_FACBASEENTER_RE:
		{
			OnPrtcFacBaseEnter_Re(pProtocol);
		}
		break;
	case PROTOCOL_GETFACACTTOPTABLE_RE:
		{	
			OnPrtcGetFacActTopTable_Re(pProtocol);
		}
		break;
	case PROTOCOL_CRSSVRTEAMSNOTIFY_RE:
		{
			OnPrtcCrssvrTeamsNotifyRe(pProtocol);		//  
		}
		break;
	case PROTOCOL_CRSSVRTEAMSROLELIST_RE:
		{
			OnPrtcCrssvrTeamsListRe(pProtocol);		//  
		}
		break;
	case PROTOCOL_CRSSVRTEAMSTEAMBASE_RE:
		{
			OnPrtcCrssvrTeamsBaseRe(pProtocol);	
		}
		break;
	case PROTOCOL_CRSSVRTEAMSSEARCH_RE:
		{
			OnPrtcCrssvrTeamsSearchRe(pProtocol);
		}
		break;
	case PROTOCOL_CRSSVRTEAMSROLENOTIFY_RE:
		{
			OnPrtcCrssvrTeamsRoleNotifyRe(pProtocol);
		}
		break;
	case PROTOCOL_FACBASESTOPNOTICE:
		{
			OnPrtcFacBaseStopNotice(pProtocol);
		}
		break;
	default:
		g_pGame->RuntimeDebugInfo(RTDCOL_WARNING, _AL("NET - Unknown protocol (%d)"), pProtocol->GetType());
		break;


	}

	//	Add this protocol to old list
	return bAddToOld;
}


//	Process protocols in m_NewPrtcList
bool CECGameSession::ProcessNewProtocols()
{
	using namespace GNET;

	int i;

	EnterCriticalSection(&g_csSession);

	for (i=0; i < m_aNewProtocols.GetSize(); i++)
	{
		GNET::Protocol* pSrc = m_aNewProtocols[i];
		CECNetProtocolBase* p = CECNetProtocolBase::CreateFrom(pSrc);

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
	}

	m_aNewProtocols.RemoveAll(false);

	LeaveCriticalSection(&g_csSession);

	m_bProcNewPtrc = true;

	for (i=0; i < m_aTempProtocols.GetSize(); i++)
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

	return true;
}

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
//	case PROTOCOL_CHALLENGE:
	case PROTOCOL_KEYEXCHANGE:
//	case PROTOCOL_ONLINEANNOUNCE:
	case PROTOCOL_ROLELIST_RE:	
//	case PROTOCOL_SELECTROLE_RE:
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
	case RPC_CRSSVRTEAMSINVITE:

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
	case PROTOCOL_CHANGEFACTIONNAME_RE:

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

	case PROTOCOL_CONSIGNCANCELPOST_RE:
	case PROTOCOL_CONSIGNQUERY_RE:
	case PROTOCOL_CONSIGNSTARTERR:
		
	//  Cross Server Protocol
//	case PROTOCOL_PLAYERCHANGEDS_RE:
	case PROTOCOL_CHANGEDS_RE:
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
	if (m_bCrossZone)
		return true;
	GamedataSend p;
	p.data.replace(pData, iSize);
	return SendNetData(p);
}

//	Send fun game data
bool CECGameSession::SendFunGameData(void* pData, int iSize)
{
	using namespace GNET;
	if (m_bCrossZone)
		return true;
	FunGamedataSend p;
	p.data.replace(pData, iSize);

	return SendNetData(p);
}

bool CECGameSession::SendHomeTownData(void* pData, int iSize)
{
	using namespace GNET;
	HometowndataSend p;
	if (m_bCrossZone)
		return true;
	p.data.replace(pData, iSize);
	return SendNetData(p);
	
}
//	Send chat data
bool CECGameSession::SendChatData(char cChannel, const ACHAR* szMsg, int nPack, int nSlot, int nSpeaker, int nAchievement)
{
	using namespace GNET;
	using namespace CHAT_C2S;

	if (!szMsg || !szMsg[0])
		return true;

	PublicChat p;
	p.channel	= cChannel;
	p.roleid	= m_iCharID;
	p.item_pos	= nSpeaker;

	if (nPack >= 0)
	{
		chat_equip_item c;
		c.cmd_id = CHAT_EQUIP_ITEM;
		c.where = nPack;
		c.index = nSlot;
		p.data.replace(&c, sizeof(c));
	}
	else if (nAchievement >= 0)
	{
		chat_achievement c;
		c.cmd_id = CHAT_ACHIEVEMENT;
		c.achieve_id = nAchievement;
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

//	Send private chat data
bool CECGameSession::SendPrivateChatData(const ACHAR* szDstName, const ACHAR* szMsg, BYTE byFlag/* 0 */, 
										 int idPlayer/* 0 */, int nPack, int nSlot, bool bGT, int nAchievement)
{
	using namespace GNET;
	using namespace CHAT_C2S;

	if (!szMsg || !szMsg[0])
		return true;
	// 竞技场中，屏蔽私聊
	CECHostPlayer* pHost = g_pGame->GetGameRun()->GetHostPlayer();
	if(pHost->GetBattleType() == BATTLE_TYPE_ARENA || pHost->GetBattleType() == BATTLE_TYPE_ARENAREBORN || pHost->GetBattleType() == BATTLE_TYPE_CRSSVR)
		return true;

	int id = idPlayer;
	if(id==0)
		id = g_pGame->GetGameRun()->GetPlayerID(szDstName);
	PrivateChat p;
	p.srcroleid = m_iCharID;
	p.dstroleid = id;
	p.channel	= byFlag;
	p.srcrolelevel = 0;
	p.route = TO_GAME;
	if (bGT && id!=0)
	{
		CECFriendMan *pMan = pHost->GetFriendMan();
		if (pMan)
		{
			p.route = pMan->GetChatRouteType(id);
		}
	}

	if (nPack >= 0)
	{
		chat_equip_item c;
		c.cmd_id = CHAT_EQUIP_ITEM;
		c.where = nPack;
		c.index = nSlot;
		p.data.replace(&c, sizeof(c));
	}
	else if (nAchievement >= 0)
	{
		chat_achievement c;
		c.cmd_id = CHAT_ACHIEVEMENT;
		c.achieve_id = nAchievement;
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
		CECFriendMan* pFriendMan = pHost->GetFriendMan();
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
bool CECGameSession::SelectRole(int cid, int line_id, char flag)
{
	SetCharacterID(cid);
	return SendNetData(GNET::SelectRole(cid, line_id, flag));
//	return SendNetData(GNET::SelectRole(cid, line_id));
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
	m_bCrossZone = false;
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

	if (!iNumRole || !aRoleIDs || m_bCrossZone)
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

	if (!iPlayerId || !iCloneId || m_bCrossZone)
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

/*	Get player's brief information

	iReason: reason to get player's information

		0: try to get host player's team member's information.
		1: try to get someone's name who just said something.
		2: just try to get players' names
		3: get player's brief infomation to handle team invitation
*/
void CECGameSession::GetPlayerBriefInfo(int iNumPlayer, int* aIDs, int iReason)
{
	if (!iNumPlayer || !aIDs)
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
	}
}

/*	Get player's ID by name

	iReason: the reason to get player's ID
		
		0: no special reason, just get it
		1: for invite a player who is very far from us to a team
*/
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

void CECGameSession::SubmitAccuse(int id)
{	
	GNET::ACAccuse p;
	
	CECHostPlayer* pHost = g_pGame->GetGameRun()->GetHostPlayer();
	p.zoneid = 0;
	p.roleid = pHost->GetCharacterID();
	p.accid = GetUserID();
	p.accusation_roleid = id;
	p.accusation_accid = 0;
	p.reason.replace(_AL("举报外挂"), a_strlen(_AL("举报外挂")) * sizeof (ACHAR));
	
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

DWORD CECGameSession::GetPingTime(int idServer, UINT64& ui64Status)
{
	EnterCriticalSection(&m_csPingSection);
	DWORD dwRet = 99999999;
	ui64Status = 0;
	if( m_Ping.GetSize() > idServer && m_Ping[idServer] != NULL &&
		m_Ping[idServer]->idLink != (DWORD)-1)
	{
		dwRet = m_Ping[idServer]->dwTime;
		ui64Status = m_Ping[idServer]->ui64Status;
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
	if (m_bCrossZone)
	{
		if (m_cCrossZoneFlag == DS_TO_CENTRALDS || m_cCrossZoneFlag == DIRECT_TO_CENTRALDS || m_cCrossZoneFlag == DS_TO_BATTLEMAP)
			SendNetData(GNET::KeyReestablish(m_iCharID, m_iUserID, m_cCrossZoneFlag, m_iSrcZoneID, m_RandomKey));
		else if (m_cCrossZoneFlag == CENTRALDS_TO_DS)
			SendNetData(GNET::KeyReestablish(m_iCharID, m_iUserID, m_cCrossZoneFlag, m_iDstZoneID, m_RandomKey));
		// 这里重建输入加密会导致上面的协议被加密后才发送过去，原因是发送协议走的是多线程
		//		m_pNetMan->RestoreOSecurity();	
		m_pNetMan->RestoreISecurity();
		return;
	}
	
	Response res;
//	m_pNetMan->SetupResponse(res, m_strUserName, m_strPassword, p->nonce, p->algo);
	if (g_pGame->GetGameInit().szUserName[0] && g_pGame->GetGameInit().szToken[0])
		res.use_token = 1;
	else
		res.use_token = 0;

	m_pNetMan->SetupResponse(res, m_strUserName, m_strRawUserName,m_strPassword, g_pGame->GetConfigs()->GetCountryRegion(), p->nonce, p->algo, res.use_token == 1);

	// clear password
	char * szBuffer = m_strPassword.LockBuffer();
	memset(szBuffer, 0, strlen(szBuffer));
	m_strPassword.UnlockBuffer();
	
	// clear token and user
	if (res.use_token)
	{
		g_pGame->GetGameInit().szUserName[0] = '\0';
        g_pGame->GetGameInit().szToken[0] = '\0';	
	}

	// clear username and password for multi-platform login. Added 2012-05-24.
	if ( g_pGame->GetGameInit().szUserName[0] && g_pGame->GetGameInit().szPassword[0] )
	{
		g_pGame->GetGameInit().szUserName[0] = '\0';
		g_pGame->GetGameInit().szPassword[0] = '\0';
	}
	AString szTemp = g_pGame->LoadMid();
	res.mid = Octets((const char*)szTemp, szTemp.GetLength() * sizeof(char)); 
	SendNetData(res);
}

void CECGameSession::OnPrtcKeyExchange(GNET::Protocol* pProtocol)
{
	using namespace GNET;

	g_pGame->RuntimeDebugInfo(RTDCOL_NETWORK, _AL("NET - protocol keyexchange"));

	KeyExchange* p = (KeyExchange*)pProtocol;
	p->Setup(m_pNetMan, m_idLink, m_strUserName, m_bKickUser ? true : false);
	
	m_strUserName.MakeLower();
	// system账号标识
/* gt功能需要username，故取消清除
	if (m_strUserName!="system"&&m_strUserName!="system2")
	{
		// clear user name
		char * szBuffer = m_strUserName.LockBuffer();
		memset(szBuffer, 0, strlen(szBuffer));
		m_strUserName.UnlockBuffer();
	}
*/
	SendNetData(p);
}

void CECGameSession::OnPrtcOnlineAnnounce(GNET::Protocol* pProtocol)
{
	using namespace GNET;

	g_pGame->RuntimeDebugInfo(RTDCOL_NETWORK, _AL("NET - protocol online announce"));
	OnlineAnnounce* p = (OnlineAnnounce*)pProtocol;

	if (m_bCrossZone)
	{	
		m_pNetMan->RestoreOSecurity();
		SelectRole(m_iCharID, m_iDstLine, m_cCrossZoneFlag);
		g_pGame->GetGameRun()->SetCurLine(m_iDstLine);
		// 清掉正在跨服标志？
//		m_bCrossZone = false;	
		m_pNetMan->ChangeState(m_idLink, &state_GSelectRoleClient);
	}
	else
	{
		m_iUserID = p->userid;
		m_pNetMan->SetZoneID(p->zoneid);
		m_zoneID = p->zoneid;
//		m_iSrcZoneID = p->zoneid;	// 如果玩家跨服，这个值会用上，如果失败重连，保证重连的是源服务器
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
	if(p->auth.size() == 0)
		g_pGame->ResetPrivilege(NULL, 0);
	else
		g_pGame->ResetPrivilege(&p->auth.GetVector()[0], p->auth.size());
	g_pGame->GetGameRun()->SetLastLoginIp(p->lastlogin_ip);
	
	if (m_bCrossZone)	//跨服状态下，直接进入跨服gs
	{
		g_pGame->GetGameRun()->SetSelectedRoleInfo(p->select_roleinfo);
		const GNET::RoleInfo& ri = g_pGame->GetGameRun()->GetSelectedRoleInfo();
		A3DVECTOR3 vecPos(ri.posx, ri.posy, ri.posz);
		g_pGame->GetGameRun()->StartGame(ri.worldtag, vecPos);
		StartGame();
		m_bCrossZone = false;

		// Added 2011-08-18.
//		g_pA3DConfig->SetFlagNewBoneScale(false);
		// Added end.
		return;
	}

	if ( p->direct_enter_raid )	//副本状态下，直接进入副本gs Added 2011-08-10.
	{
		//在客户端收到SelectRoleRe之前，必然已经给服务器发了SelectRole协议（点击进入游戏按键enterworld）
		//而那个时候，已经设置了CECGameRun::SetSelectedRoleInfo函数
		GNET::RoleInfo ri = g_pGame->GetGameRun()->GetSelectedRoleInfo();
		
		//副本状态下，只需要从协议中更新地图坐标和地图ID.
		ri.posx = p->select_roleinfo.posx;
		ri.posy = p->select_roleinfo.posy;
		ri.posz = p->select_roleinfo.posz;
		ri.worldtag = p->select_roleinfo.worldtag;
		//更新
		g_pGame->GetGameRun()->SetSelectedRoleInfo(ri);
		
		A3DVECTOR3 vecPos(ri.posx, ri.posy, ri.posz);
		g_pGame->GetGameRun()->StartGame(ri.worldtag, vecPos);
		StartGame();

		// Added 2011-08-18.
//		g_pA3DConfig->SetFlagNewBoneScale(false);
		// Added end.
		return;
	}

	CECLoginUIMan* pLoginUIMan = g_pGame->GetGameRun()->GetUIManager()->GetLoginUIMan();
	if (pLoginUIMan)
		pLoginUIMan->LaunchLoading();
	else	//	Start game directly
		StartGame();

	// 如果是通过登陆界面进入游戏，那么在收到SelectRoleRe协议之后，将进入游戏世界
	// 而在这次更新中，选择角色界面的逻辑和实现发生了一些变化，并为了实现效果使用
	// 了缩放角色模型的功能，这需要设置g_pA3DConfig->SetFlagNewBoneScale(true);但
	// 游戏中还存在一些老的骨骼模型，因此使用FlagNewBoneScale会出现异常，因此在此
	// 恢复FlagNewBoneScale标志为false //Added 2011-08-18. 
//	g_pA3DConfig->SetFlagNewBoneScale(false);
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

void CECGameSession::OnPrtcChangeDsRe(GNET::Protocol* pProtocol)
{
	using namespace GNET;
	ChangeDS_Re* p = (ChangeDS_Re*)pProtocol;
	if (p->retcode != ERR_SUCCESS)
	{
		OutputLinkSevError(p->retcode);
		g_pGame->RuntimeDebugInfo(RTDCOL_ERROR, _AL("NET - protocol playerchangeds re return error"));
		return;
	}
}

void CECGameSession::OnPrtcPlayerChangeDsRe(GNET::Protocol* pProtocol)
{
	using namespace GNET;
	PlayerChangeDS_Re* p = (PlayerChangeDS_Re*)pProtocol;

	if (p->retcode != ERR_SUCCESS)
	{
		OutputLinkSevError(p->retcode);
		g_pGame->RuntimeDebugInfo(RTDCOL_ERROR, _AL("NET - protocol playerchangeds re return error"));
		return;
	}

	overlay::GTOverlay::Instance().Logout();

	// 清除该列表，防止跨服中发送对应的协议
	m_aPBIPlayers.RemoveAll(false);
	m_pPBIProc->Reset();

// 	g_pGame->GetGameRun()->CachePlayerInfo(g_pGame->GetGameRun()->GetHostPlayer());
// 	g_pGame->GetGameRun()->EndGameState();
	// 断开原来的连接，建立新连接
	m_pNetMan->Disconnect();
	m_bConnected = false;
	CECGame::SERVERINFO* pInfo = NULL;
	if (p->flag == DS_TO_CENTRALDS || p->flag == DIRECT_TO_CENTRALDS
		|| p->flag == DS_TO_BATTLEMAP)
	{
		pInfo = g_pGame->GetCrossServerInfo(p->dst_zoneid);
		if (!pInfo)
			return;
	}
	else if (p->flag == CENTRALDS_TO_DS)
	{
		pInfo = g_pGame->GetSrcServerInfo(p->dst_zoneid);
		if (!pInfo)
			return;
	}
	else
		return;

	CECGame::GAMEINIT& gameinit = g_pGame->GetGameInit();
	
	int index = a_Random(0, pInfo->vecAddress.size()-1);
	strncpy(gameinit.szIP, pInfo->vecAddress[index].GetBuffer(0), pInfo->vecAddress[index].GetLength());
	gameinit.szIP[pInfo->vecAddress[index].GetLength()] = 0;
	
	index = a_Random(0, pInfo->vecPorts.size()-1);
	gameinit.iPort = pInfo->vecPorts[index];
	gameinit.iServerID = pInfo->iServerID;
	strncpy(gameinit.szServerName, pInfo->szServerName, 256);

	Connect();

	m_iCharID	= p->roleid;
	m_iUserID	= p->userid;
	if (p->flag == DS_TO_CENTRALDS || p->flag == DIRECT_TO_CENTRALDS
			|| p->flag == DS_TO_BATTLEMAP)
	{
		m_iSrcZoneID = m_zoneID;
		m_iDstZoneID = p->dst_zoneid;
	}	
	else if (p->flag == CENTRALDS_TO_DS)
	{
		m_iSrcZoneID = p->dst_zoneid;
		m_iDstZoneID = m_zoneID;
	}
	//  线路信息
	m_iSrcLine	 = g_pGame->GetGameRun()->GetCurLine();
	m_iDstLine	 = p->gsid;
//	g_pGame->GetGameRun()->SetCurLine(p->gsid);
	
	m_pNetMan->SetZoneID(p->dst_zoneid);
	m_zoneID	= p->dst_zoneid;
	m_bCrossZone	= true;
	m_cCrossZoneFlag= p->flag;
	m_RandomKey		= p->random;
}

//	Process chat data
void CECGameSession::OnPrtcChatMessage(GNET::Protocol* pProtocol, bool& bAddToOld)
{
	CECGameUIMan* pGameUI = (CECGameUIMan*)g_pGame->GetGameRun()->GetUIManager()->GetInGameUIMan();

	using namespace GNET;
	using namespace CHAT_S2C;

	ChatMessage* p = (ChatMessage*)pProtocol;
	CECIvtrItem* pIvtrItem = NULL;
	sAchievement* pAchievement = NULL;

	ACHAR szMsg[1024];
	ACString strTemp((const ACHAR*)p->msg.begin(), p->msg.size() / sizeof (ACHAR));

	if( pGameUI )
	{
		pGameUI->FilterBadWords(strTemp);
		pGameUI->FilterImages(strTemp);
	}
	AUI_ConvertChatString(strTemp, szMsg, false);

	chat_equip_item* pItemInfo = (chat_equip_item*)p->data.begin();
	chat_achievement* pAchievementInfo = (chat_achievement*)p->data.begin();
	
	if (pItemInfo && pItemInfo->cmd_id == CHAT_EQUIP_ITEM)
	{
		const size_t sz = sizeof(chat_equip_item) - sizeof(pItemInfo->content);
		if (p->data.size() >= sz && sz + pItemInfo->content_length == p->data.size())
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
	else if (pAchievementInfo && pAchievementInfo->cmd_id == CHAT_ACHIEVEMENT)
	{
		const size_t sz = sizeof(chat_achievement) - sizeof(pAchievementInfo->player_name);
		if (p->data.size() >= sz && sz + pAchievementInfo->player_name_len == p->data.size())
		{
			CECAchievementMan* pMan = g_pGame->GetGameRun()->GetHostPlayer()->GetAchievementMan();

			if (pMan->GetAchievement(pAchievementInfo->achievement_id))
			{
				pAchievement = new sAchievement(*pMan->GetAchievement(pAchievementInfo->achievement_id));
				pAchievement->iFinishTime = pAchievementInfo->finish_time;
				ACString playerName((ACHAR*)pAchievementInfo->player_name, pAchievementInfo->player_name_len/2);
				pAchievement->strOwnerName = playerName;
				pAchievement->SetFinish(pAchievement->iFinishTime != 0);
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
				delete pAchievement;
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
					tm *gtime = gmtime((const time_t*)&stime);
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
			case 45:
				{
					char type;
					int defender_id;
                    Octets defender_name,attacker1_name,attacker2_name,attacker3_name,attacker4_name;
					try
					{
						Marshal::OctetsStream(p->msg)>>type>>defender_id>>defender_name>>attacker1_name>>attacker2_name>>attacker3_name>>attacker4_name;
						if(type == 1 || type == 2)
						{
						   ACString strDefender = pGameUI->GetStringFromTable(15644); 
						   if(defender_id != 0) 
						       strDefender = ACString((const ACHAR*)defender_name.begin(), 
							                          defender_name.size() / sizeof(ACHAR));
				           strMsg.Format(pGameUI->GetStringFromTable(15622+type)
							           ,strDefender
									   ,ACString((const ACHAR*)attacker1_name.begin(), attacker1_name.size() / sizeof(ACHAR))
									   ,ACString((const ACHAR*)attacker2_name.begin(), attacker2_name.size() / sizeof(ACHAR))
									   ,ACString((const ACHAR*)attacker3_name.begin(), attacker3_name.size() / sizeof(ACHAR))
									   ,ACString((const ACHAR*)attacker4_name.begin(), attacker4_name.size() / sizeof(ACHAR))
									   ,g_pGame->GetGameRun()->GetCurGroup()
									   ,g_pGame->GetGameRun()->GetServerName());
						}
						
					}catch(...){}
					break;
				}
			case 46:
				{
					Octets win_fac_name,win_king_name;
					try
					{
						Marshal::OctetsStream(p->msg)>>win_fac_name>>win_king_name;

						strMsg.Format(pGameUI->GetStringFromTable(15625)
							,ACString((const ACHAR*)win_fac_name.begin(), win_fac_name.size() / sizeof(ACHAR))
						    ,ACString((const ACHAR*)win_king_name.begin(), win_king_name.size() / sizeof(ACHAR)));
						
					}catch(...){}
					break;
				}
			case 47:
				{
					try
					{
						ACString strTxt = ACString((const ACHAR*)p->msg.begin(),p->msg.size()/sizeof(ACHAR));
						CDlgKingAnnounce* pDlg = (CDlgKingAnnounce*)pGameUI->GetDialog("Win_King_Anounce");
						pDlg->SetMessage(strTxt);
						pDlg->Show(true);
						strMsg = _AL("");
					}catch(...){}
				    break;
				}
			case 48:
				{
					int flag,score;
					try
					{
						Marshal::OctetsStream(p->msg)>>flag>>score;
						strMsg.Format(pGameUI->GetStringFromTable(15634+flag),score);
						
					}catch(...){}
					break;
				}
			case 49:
				{
					char   title;
					Octets rolename;
					try
					{
					    Marshal::OctetsStream(p->msg)>>title>>rolename;
						//神将、天官和亲卫职务
						//assert(title >= 3 && title <= 5);
						int vocation = 0;
						if(title == 3 || title == 4)
							vocation = 15642+title;
						else if(title == 6)
						    vocation = 15651;
						 strMsg.Format(pGameUI->GetStringFromTable(vocation)
						               ,ACString((const ACHAR*)rolename.begin(), rolename.size() / sizeof(ACHAR)));

					}catch(...){}
				    break;
				}
			case 50:
				{
					Octets queenname;
					try
					{
						Marshal::OctetsStream(p->msg)>>queenname;
						
						strMsg.Format(pGameUI->GetStringFromTable(15659)
							,ACString((const ACHAR*)queenname.begin(), queenname.size() / sizeof(ACHAR)));

						pGameUI->PopupKingBathFlyToNpc();
						
					}catch(...){}
					break;
				}
			case 51:
				{
					Octets kingname; int type;
					try
					{
						Marshal::OctetsStream(p->msg)>>kingname>>type;
						ACString strLevel = pGameUI->GetStringFromTable(15665+type);
						
						strMsg.Format(pGameUI->GetStringFromTable(15665)
							,ACString((const ACHAR*)kingname.begin(), kingname.size() / sizeof(ACHAR)),strLevel);
						
					}catch(...){}
					break;
				}
			default:
				strMsg = pGameUI->GetStringFromTable(4000 + p->srcroleid);
			}
			if( strMsg != _AL("") )
			{
				if( p->srcroleid == 10 || 
					p->srcroleid == 13 || 
					p->srcroleid == 15 || 
					p->srcroleid == 48 || 
					p->srcroleid == 49)
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
		// 屏蔽30级以下12字符以上信息
		if (g_pGame->GetConfigs()->Limit30Words())
		{
			int nLen = GetLengthExpItem(szMsg);
			if (p->channel == GP_CHAT_LOCAL && nLen>12 && p->srcrolelevel>0 && p->srcrolelevel < 30)
			{
				delete pIvtrItem;
				delete pAchievement;
				return;
			}
		}
		
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
							sAchievement* p3 = pAchievement ? new sAchievement(*pAchievement) : 0;
							pPlayer->SetLastSaidWords(strTemp, p->emotion, p2, p3);
						}
					}
					g_pGame->GetGameRun()->AddChatMessage(str, p->channel, p->srcroleid, NULL, 0, p->emotion, pIvtrItem, pAchievement);
					
					pIvtrItem = NULL;
					pAchievement = NULL;
				}
			}
		}
		else if (ISNPCID(p->srcroleid))
		{
			CECNPC* pNPC = g_pGame->GetGameRun()->GetWorld()->GetNPCMan()->GetNPC(p->srcroleid);
			
			if (pNPC)
			{
				ACString str;
				if(pNPC->GetName()&&a_strlen(pNPC->GetName())>0)
					str.Format(pStrTab->GetWideString(FIXMSG_CHAT2), pNPC->GetName(), strTemp);	//szMsg通过函数将& 转换成^&,但对怪物喊话当前没有使用到
				else
					str = strTemp;	//szMsg
				//表情-----------------------------------------
/*				ACHAR ch = 57344;
				ACString strCode = _AL("");
				strCode += ch;
				strCode += _AL("<0><8:10>");	//固定使用第八套表情
				str += strCode;

				ACString strTest = _AL( "测试&1&cs测试&2&12&" );
				str += strTest;
*/
				const int nDefaultEmotionType = 0;
				MonsterStrParser( str );
				g_pGame->GetGameRun()->AddChatMessage(str, p->channel, p->srcroleid, NULL, 0, nDefaultEmotionType, pIvtrItem);
				//---------------------------------------------
				pIvtrItem = 0;
				if (!vs.bPlayerBubble && pNPC->GetName() && a_strlen(pNPC->GetName())>0)
				{			
					ACString strWords = _AL("");
					strWords = strTemp;
					MonsterStrParser( strWords );
					//
					pNPC->SetLastSaidWords(strWords, nDefaultEmotionType);//Modified 2012-04-25.
				}
			}
		}
	}

	delete pIvtrItem;
	delete pAchievement;
}

void CECGameSession::OnPrtcWorldChat(GNET::Protocol* pProtocol)
{
	using namespace GNET;
	using namespace CHAT_S2C;

	WorldChat* p = (WorldChat*)pProtocol;
	CECIvtrItem* pIvtrItem = NULL;
	sAchievement* pAchievement = NULL;

	ACHAR szMsg[1024];
	ACString strTemp((const ACHAR*)p->msg.begin(), p->msg.size() / sizeof (ACHAR));
	CECGameUIMan* pGameUI = g_pGame->GetGameRun()->GetUIManager()->GetInGameUIMan();
	if( pGameUI )
	{
		pGameUI->FilterBadWords(strTemp);
		pGameUI->FilterImages(strTemp);
	}
	AUI_ConvertChatString(strTemp, szMsg, false);

	chat_equip_item* pItemInfo = (chat_equip_item*)p->data.begin();
	chat_achievement* pAchievementInfo = (chat_achievement*)p->data.begin();

	if (pItemInfo && pItemInfo->cmd_id == CHAT_EQUIP_ITEM)
	{
		const size_t sz = sizeof(chat_equip_item) - sizeof(pItemInfo->content);
		if (p->data.size() >= sz && sz + pItemInfo->content_length == p->data.size())
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
	else if (pAchievementInfo && pAchievementInfo->cmd_id == CHAT_ACHIEVEMENT)
	{
		const size_t sz = sizeof(chat_achievement) - sizeof(pAchievementInfo->player_name);
		if (p->data.size() >= sz && sz + pAchievementInfo->player_name_len == p->data.size())
		{
			CECAchievementMan* pMan = g_pGame->GetGameRun()->GetHostPlayer()->GetAchievementMan();
			
			if (pMan->GetAchievement(pAchievementInfo->achievement_id))
			{
				pAchievement = new sAchievement(*pMan->GetAchievement(pAchievementInfo->achievement_id));
				pAchievement->iFinishTime = pAchievementInfo->finish_time;
				ACString playerName((ACHAR*)pAchievementInfo->player_name, pAchievementInfo->player_name_len/2);
				pAchievement->strOwnerName = playerName;
				pAchievement->SetFinish(pAchievement->iFinishTime != 0);
			}
		}
	}

	if (p->channel == GP_CHAT_FARCRY && p->roleid == 24)
	{
		OnTaskChatMessage(p->msg.begin(), p->msg.size());
		delete pIvtrItem;
		delete pAchievement;
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
				DWORD i;
				for( i= 0; i < info->size(); i++ )
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
			delete pAchievement;
			return;
		}
		case 25: // 汤圆彩票中大奖喊话
		{			
			struct _data
			{
				int player_id;
				int lineno;
				char name[20];
				int item_id;
			};
			
			if (p->msg.size() >= sizeof(_data))
			{				
				_data* pData = (_data*)p->msg.begin();
				wchar_t szName[11];
				szName[10] = 0;
				wcsncpy(szName, (wchar_t*)pData->name, 10);
				pIvtrItem = CECIvtrItem::CreateItem(pData->item_id, 0, 1);
				if (pIvtrItem)
				{
					EditBoxItemBase item(enumEIIvtrlItem);
					ACString strItem = (ACHAR)AUICOMMON_ITEM_CODE_START + item.Serialize();
					strMsg.Format(pGameUI->GetStringFromTable(14004), szName, strItem);
				}
			}
			g_pGame->GetGameRun()->AddChatMessage(strMsg, GP_CHAT_BROADCAST, 0, NULL, 1, p->emotion, pIvtrItem);
			return;
		}
		case 26:  //国王使用积分购买物品
		{
			struct _data 
			{
               int item_id;
			   int item_num;
			   int score;
			};

			if (p->msg.size() >= sizeof(_data))
			{				
				_data* pData = (_data*)p->msg.begin();
				pIvtrItem = CECIvtrItem::CreateItem(pData->item_id, 0, 1);
				if (pIvtrItem)
				{
					EditBoxItemBase item(enumEIIvtrlItem);
					ACString strItem = (ACHAR)AUICOMMON_ITEM_CODE_START + item.Serialize();
					strMsg.Format(pGameUI->GetStringFromTable(15639),pData->score,pData->item_num,strItem);
				}
			}
			g_pGame->GetGameRun()->AddChatMessage(strMsg, GP_CHAT_BROADCAST, 0, NULL, 1, p->emotion, pIvtrItem);
			return;
			
		}
		case 27: //抽到紫色符文的全服喊话
		{
			struct _data
			{
				int player_id;
				char name[20];
				int item_id;
			};

						
			if (p->msg.size() >= sizeof(_data))
			{				
				_data* pData = (_data*)p->msg.begin();
				wchar_t szName[11];
				szName[10] = 0;
				wcsncpy(szName, (wchar_t*)pData->name, 10);
				pIvtrItem = CECIvtrItem::CreateItem(pData->item_id, 0, 1);
				if (pIvtrItem)
				{
					pIvtrItem->SetNeedUpdate(false);
					EditBoxItemBase item(enumEIIvtrlItem);
					ACString strItem = (ACHAR)AUICOMMON_ITEM_CODE_START + item.Serialize();
					strMsg.Format(pGameUI->GetStringFromTable(18337), szName, strItem);
				}
			}
			g_pGame->GetGameRun()->AddChatMessage(strMsg, GP_CHAT_BROADCAST, 0, NULL, 1, p->emotion, pIvtrItem);
			return;
		}

		}
		if( strMsg != _AL("") )
			g_pGame->GetGameRun()->AddChatMessage(strMsg, GP_CHAT_BROADCAST, 0, NULL, 1, p->emotion);
	}
	else if (!p->roleid && p->channel!=GP_CHAT_RUMOR)
	{
		g_pGame->GetGameRun()->AddChatMessage(szMsg, p->channel, p->roleid, NULL, 0, p->emotion);
	}
	else
	{
		int iLength = EditBoxItemBase::GetUnserializedLength(szMsg);
		if (pIvtrItem)
		{
			iLength += a_strlen(pIvtrItem->GetName());
			iLength += 1;	// +2-1
		}
		if ((p->channel==GP_CHAT_SPEAKER||p->channel==GP_CHAT_RUMOR) && iLength>30)	// 大喇叭不能超过30字符，防外挂
		{
			return;
		}
		CECStringTab* pStrTab = g_pGame->GetFixedMsgTab();

		ACHAR szText[80];
		ACString strName((const ACHAR*)p->name.begin(), p->name.size() / sizeof (ACHAR));
		if (ISPLAYERID(p->roleid))
			g_pGame->GetGameRun()->AddPlayerName(p->roleid, strName);
		if (p->channel==GP_CHAT_RUMOR)
		{
			strName = g_pGame->GetGameRun()->GetUIManager()->GetInGameUIMan()->GetStringFromTable(15025);
		}
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
			str.Format(_AL("%s%s"),pGameUI->GetStringFromTable(8494),strMsg);
		}
		else
		{
			if (p->sender_zoneid>0 && p->channel==GP_CHAT_FARCRY)
			{
				CECGame::SERVERINFO* pInfo = g_pGame->GetSrcServerInfo(p->sender_zoneid);
				if (pInfo)
				{
					str.Format(pGameUI->GetStringFromTable(1057), szText, AS2AC(pInfo->szServerName), szMsg);
				}
				else
					str.Format(pStrTab->GetWideString(FIXMSG_CHAT), szText, szMsg);
			}
			else if (p->channel==GP_CHAT_RUMOR)
				str.Format(_AL("%s:%s"), szText, szMsg);
			else
				str.Format(pStrTab->GetWideString(FIXMSG_CHAT), szText, szMsg);
		}
		g_pGame->GetGameRun()->AddChatMessage(str, p->channel, p->roleid, NULL, 0, p->emotion, pIvtrItem, pAchievement);
		pIvtrItem = 0;
		pAchievement = 0;
	}

	delete pIvtrItem;
	delete pAchievement;
}

void CECGameSession::OnPrtcGTOffLineChat(GNET::Protocol* pProtocol)
{
	using namespace GNET;
	using namespace CHAT_S2C;
	
	GateOfflineChat* p = (GateOfflineChat*)pProtocol;
	CECGameUIMan* pGameUI = g_pGame->GetGameRun()->GetUIManager()->GetInGameUIMan();
	if( !pGameUI )
		return;
		CECFriendMan* pFriendMan = g_pGame->GetGameRun()->GetHostPlayer()->GetFriendMan();
	if (!pFriendMan)
		return;

	for (UINT i=0; i<p->messages.size(); ++i)
	{
		GGTOfflineMsg chat = p->messages[i];
		ACString strMsg((const ACHAR*)chat.msg.begin(), chat.msg.size() / sizeof (ACHAR));
		ACString strSrcName((const ACHAR*)chat.src_name.begin(), chat.src_name.size() / sizeof (ACHAR));
		pGameUI->FilterBadWords(strMsg);
		pGameUI->FilterImages(strMsg);
		EditBoxItemBase item(enumEIImage);
		item.SetImageIndex(CECGameUIMan::ICONLIST_GTICON);
		item.SetImageFrame(0);
		strMsg = ACString(_AL(" ")) + (ACHAR)AUICOMMON_ITEM_CODE_START + item.Serialize() + ACString(_AL(" ")) + strMsg;

		ACString str;
		CECStringTab* pStrTab = g_pGame->GetFixedMsgTab();
		str.Format(pStrTab->GetWideString(FIXMSG_PRIVATECHAT1), strSrcName, strMsg);
		CECFriendMan::FRIEND *pFriend = pFriendMan->GetFriendByID(chat.srcroleid);
		if (pFriend)
			g_pGame->GetGameRun()->AddChatMessage(strMsg, GP_CHAT_WHISPER, chat.srcroleid, strSrcName, CHANNEL_FRIEND, chat.emotion);
		else
			g_pGame->GetGameRun()->AddChatMessage(str, GP_CHAT_WHISPER, chat.srcroleid, strSrcName, 0, chat.emotion);
		
		//	Save player name.
		g_pGame->GetGameRun()->AddPlayerName(chat.srcroleid, strSrcName);
	}
}


void CECGameSession::OnPrtcErrorInfo(GNET::Protocol* pProtocol)
{
	using namespace GNET;
	ErrorInfo* p = (ErrorInfo*)pProtocol;
	OutputLinkSevError(p->errcode);
	if (m_bCrossZone)
	{
//		g_pGame->GetGameRun()->EndCrossAnimate();
		g_pGame->GetGameRun()->SetLogoutFlag(2);
		m_bCrossZone = false;
	}
/*	if( p->errcode == ERR_INVALID_PASSWORD ||
		p->errcode == ERR_COMMUNICATION ||
		p->errcode == ERR_ACCOUNTLOCKED ||
		p->errcode == ERR_MULTILOGIN ||
		p->errcode == ERR_LINKISFULL)
*/	{
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

	// 屏蔽30级以下12字符以上信息
	int nLen = GetLengthExpItem(strMsg);
	if (p->channel == GP_CHAT_LOCAL && nLen>12 && p->srcrolelevel>0 && p->srcrolelevel < 30)
	{
		return;
	}

	// 屏蔽低级别玩家信息
	EC_LOCAL_SETTING setting = g_pGame->GetConfigs()->GetLocalSettings();
	
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
	sAchievement* pAchievement = NULL;
	ASSERT(p->dstroleid == m_iCharID);


	CECGameUIMan* pGameUI = g_pGame->GetGameRun()->GetUIManager()->GetInGameUIMan();
	if( pGameUI )
	{
		pGameUI->FilterImages(strMsg);
		pGameUI->FilterBadWords(strMsg);
	}
	g_pGame->GetGameRun()->AddLstMsgType(p->srcroleid, p->route==FROM_GT ? 2:1);
	if(p->route==FROM_GT)
	{
		// 聊天信息前面加标记
		EditBoxItemBase item(enumEIImage);
		item.SetImageIndex(CECGameUIMan::ICONLIST_GTICON);
		item.SetImageFrame(0);
		strMsg = ACString(_AL(" ")) + (ACHAR)AUICOMMON_ITEM_CODE_START + item.Serialize() + ACString(_AL(" ")) + strMsg;
		// 隐身好友现身
		CECFriendMan* pFriendMan = g_pGame->GetGameRun()->GetHostPlayer()->GetFriendMan();
		if (pFriendMan)
		{
			CECFriendMan::FRIEND *pFriend = pFriendMan->GetFriendByID(p->srcroleid);
			if (pFriend)
			{
				CECGameUIMan* pGameUI = g_pGame->GetGameRun()->GetUIManager()->GetInGameUIMan();
				pFriendMan->ChangeFriendGTStatus(p->srcroleid, 1);
				pGameUI->FriendAction(p->srcroleid, -1, CDlgFriendList::FRIEND_ACTION_INFO_GT, 1);
				p->channel = CHANNEL_FRIEND;
			}
		}
	}

	chat_equip_item* pItemInfo = (chat_equip_item*)p->data.begin();
	chat_achievement* pAchievementInfo = (chat_achievement*)p->data.begin();

	if (pItemInfo && pItemInfo->cmd_id == CHAT_EQUIP_ITEM)
	{
		const size_t sz = sizeof(chat_equip_item) - sizeof(pItemInfo->content);
		if (p->data.size() >= sz && sz + pItemInfo->content_length == p->data.size())
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
	else if (pAchievementInfo && pAchievementInfo->cmd_id == CHAT_ACHIEVEMENT)
	{
		const size_t sz = sizeof(chat_achievement) - sizeof(pAchievementInfo->player_name);
		if (p->data.size() >= sz && sz + pAchievementInfo->player_name_len == p->data.size())
		{
			CECAchievementMan* pMan = g_pGame->GetGameRun()->GetHostPlayer()->GetAchievementMan();

			if (pMan->GetAchievement(pAchievementInfo->achievement_id))
			{
				pAchievement = new sAchievement(*pMan->GetAchievement(pAchievementInfo->achievement_id));
				pAchievement->iFinishTime = pAchievementInfo->finish_time;
				ACString playerName((ACHAR*)pAchievementInfo->player_name, pAchievementInfo->player_name_len/2);
				pAchievement->strOwnerName = playerName;
				pAchievement->SetFinish(pAchievement->iFinishTime != 0);
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
		g_pGame->GetGameRun()->AddChatMessage(str, GP_CHAT_WHISPER, p->srcroleid, strSrcName, p->channel, p->emotion, pIvtrItem, pAchievement);
	}
	else
	{
		g_pGame->GetGameRun()->AddChatMessage(strMsg, GP_CHAT_WHISPER, p->srcroleid, strSrcName, p->channel, p->emotion, pIvtrItem, pAchievement);
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
		CECHostPlayer* pHost = g_pGame->GetGameRun()->GetHostPlayer();
		BYTE pvp_mask = pHost->BuildPVPMask(false);
		c2s_CmdUpatePVPMask(pvp_mask);
		pHost->OnAllInitDataReady();

		CECGameUIMan* pGameUI = (CECGameUIMan*)g_pGame->GetGameRun()->GetUIManager()->GetInGameUIMan();
		if (pGameUI)
		{
			pGameUI->EnableUI(true);
			//			g_pGame->GetGameRun()->EndCrossAnimate();
			pGameUI->LoginPromp();
		}

		if(pHost->GetCutscene())
			pHost->GetCutscene()->InitMaskData();
		
		//if( pHost->GetProfession()<=30 || (pHost->GetProfession()>=64 && pHost->GetProfession()<=69) )	//人族
		//	pHost->StartCutscene(4);
		//else	// 神族
		//	pHost->StartCutscene(6);
		int race = pHost->GetRace();
		if(race == RACE_HUMAN)
		{
		   pHost->StartAnimation(4);
		}
		else if(race == RACE_ORC)
		{
		   pHost->StartAnimation(6);
		}
		else if(race == RACE_EVILKIND)
		{
		   pHost->StartAnimation(8);
		}

		// 再次的同步时间
		g_pGame->GetGameSession()->c2s_CmdGetServerTime();
		//bool isTrue = false;
		//CECGameRun::LineInfoVec lineinfo = g_pGame->GetGameRun()->GetLinesInfo();
		//for (size_t n = 0; n < lineinfo.size(); n++)
		//{
		//	CECGameRun::LineInfo& info = lineinfo[n];
		//	
		//	if (info.id == m_iDstLine)
		//	{
		//		isTrue = true;
		//		break;
		//	}
		//}
		//if (m_iSrcLine > 0 && m_iDstLine > 0 && m_iSrcLine != m_iDstLine && isTrue == true)
		//{
		//	ACString str;
		//	str.Format(g_pGame->GetFixedMsgTab()->GetWideString(FIXMSG_RANDOM_LINE), m_iDstLine);
		//	pGameUI->ShowErrorMsg(str);
		//	m_iDstLine = 0;		// avoid unnecessary tip in the next change line operation
		//}
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
	case Forbid::FBD_FORBID_LOGIN:		str1 = pStrTab->GetWideString(FIXMSG_FORBIDLOGIN);	break;
	case Forbid::FBD_FORBID_TALK:		str1 = pStrTab->GetWideString(FIXMSG_FORBIDTALK);	break;
	case Forbid::FBD_FORBID_TRADE:		str1 = pStrTab->GetWideString(FIXMSG_FORBIDTRADE);	break;
	case Forbid::FBD_FORBID_SELL:		str1 = pStrTab->GetWideString(FIXMSG_FORBIDSELL);	break;
	case Forbid::FBD_FORBID_SELLROLE:	str1 = pStrTab->GetWideString(FIXMSG_FORBIDSELLROLE);	break;
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

	if ( rf.type == Forbid::FBD_FORBID_SELLROLE )
		str = str1 + _AL("\r") + str3;
	else
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

void CECGameSession::OnPrtcTeamRade(GNET::Protocol* pProtocol)
{			
	CECGameUIMan* pGameUI = (CECGameUIMan*)g_pGame->GetGameRun()->GetUIManager()->GetInGameUIMan();
	if( pProtocol->GetType() == PROTOCOL_TEAMRAIDAPPLY_RE )				// 加入回复
	{
		TeamRaidApply_Re* p = (TeamRaidApply_Re*)pProtocol;
		if (p->retcode!=0)
		{
			if(p->retcode==40)
				pGameUI->CommonMessage("Win_Battle_Dungeon_6v6_CharSelect", "apply_error", p->err_roleid, 0);
			else
				CECBattleDungeonMan::getSingletonPtr()->ReportError(p->retcode);
		}
/*		else
		{
			//pGameUI->GetDialog("Win_Battle_Dungeon_6v6_Min")->GetDlgItem("Btn_Max")->SetHint(pGameUI->GetStringFromTable(13139));
			pGameUI->GetDialog("Win_Battle_Dungeon_6v6_Wait")->Show(true);
			pGameUI->GetDialog("Win_Battle_Dungeon_6v6_MapSelect")->GetDlgItem("Txt_Wait")->Show(true);
			pGameUI->GetDialog("Win_Battle_Dungeon_6v6_MapSelect")->GetDlgItem("Btn_quit")->Show(true);
			CECBattleDungeonMan::getSingletonPtr()->Init(642);	// 目前只有一个，暂时写死
			pGameUI->MessageBox(13140);
		}
*/	}
	else if( pProtocol->GetType() == PROTOCOL_TEAMRAIDMAPPINGSUCCESS )	// 配对成功，弹出选择是否进入界面
	{
		TeamRaidMappingSuccess* p = (TeamRaidMappingSuccess*)pProtocol;
		
		int Iscross = p->iscross;
		DlgCrssvrTeamsApply *pDlgApply = (DlgCrssvrTeamsApply*)pGameUI->GetDialog("Win_Battle_Dungeon_6v6_GroupList");
		if(pDlgApply && Iscross == 1)
		{
			pDlgApply->UpdateCrssvrTeamsApplyInfo(pProtocol);
		}
		g_pGame->GetGameRun()->SaveConfigsToServer();
		ACString strText(pGameUI->GetStringFromTable(14000));		
		pGameUI->CommonMessage("Win_Message9", AC2AS(strText), 20, 2);
		PAUIDIALOG pMsgBox = pGameUI->GetDialog("Win_Message9");
		pMsgBox->SetData(p->raidroom_id);
	
		pMsgBox->SetDataPtr((void*)Iscross);
		pGameUI->GetDialog("Win_Battle_Dungeon_6v6_Wait")->Show(false);
		pGameUI->GetDialog("Win_Battle_Dungeon_6v6_MapSelect")->GetDlgItem("Txt_Wait")->Show(false);
		pGameUI->GetDialog("Win_Battle_Dungeon_6v6_MapSelect")->GetDlgItem("Btn_quit")->Show(false);
	}
	else if( pProtocol->GetType() == PROTOCOL_TEAMRAIDBEGINCHOOSECHAR )	// 确定进入，弹出选择界面
	{
		if(pGameUI)
			pGameUI->CommonMessage("Win_Battle_Dungeon_6v6_CharSelect", "begin", (int)pProtocol, 0);
	}
	else if( pProtocol->GetType() == PROTOCOL_TEAMRAIDUPDATECHAR )		// 通知单个玩家选择，s2c、c2s都是这个协议
	{
		if(pGameUI)
			pGameUI->CommonMessage("Win_Battle_Dungeon_6v6_CharSelect", "single", (int)pProtocol, 0);
	}
	else if( pProtocol->GetType() == PROTOCOL_TEAMRAIDAPPLYNOTICE )	
	{
		TeamRaidApplyNotice* p = (TeamRaidApplyNotice*)pProtocol;
		pGameUI->GetDialog("Win_Battle_Dungeon_6v6_Wait")->AlignTo(	pGameUI->GetDialog("Win_MiniMap"),
			AUIDIALOG_ALIGN_INSIDE, AUIDIALOG_ALIGN_RIGHT,
			AUIDIALOG_ALIGN_OUTSIDE, AUIDIALOG_ALIGN_BOTTOM);
		if(p->isapplying == 1 || p->isapplying == 0)
		{
			pGameUI->GetDialog("Win_Battle_Dungeon_6v6_Wait")->Show(p->isapplying>0);
			pGameUI->GetDialog("Win_Battle_Dungeon_6v6_MapSelect")->GetDlgItem("Txt_Wait")->Show(p->isapplying>0);
			pGameUI->GetDialog("Win_Battle_Dungeon_6v6_MapSelect")->GetDlgItem("Btn_quit")->Show(p->isapplying>0);
			CECBattleDungeonMan::getSingletonPtr()->Init(642);	// 目前只有一个，暂时写死
		}
		else if(p->isapplying == 2)
		{
			pGameUI->GetDialog("Win_Battle_Dungeon_6v6_Wait")->Show(p->isapplying>0);
			CECBattleDungeonMan::getSingletonPtr()->Init(795);	// 目前只有一个，暂时写死
		}
		pGameUI->MessageBox(13140);
	}
	else if(pProtocol->GetType() == PROTOCOL_TEAMRAIDACTIVEAPPLYNOTICE)
	{
		TeamRaidActiveApplyNotice* p = (TeamRaidActiveApplyNotice*)pProtocol;
		//int active_roleid;
		//int isapplying;
		//unsigned int localsid;
		//int roleid;
		DlgCrssvrTeamsApply *pDlgApply = (DlgCrssvrTeamsApply*)pGameUI->GetDialog("Win_Battle_Dungeon_6v6_GroupList");
		if(pDlgApply )
		{
			pDlgApply->UpdateQueueUpState(p->active_roleid);
		}
	}
}
void CECGameSession::OnFactionPKRaid(GNET::Protocol* pProtocol)
{
	CECGameUIMan* pGameUI = (CECGameUIMan*)g_pGame->GetGameRun()->GetUIManager()->GetInGameUIMan();
	if(pProtocol->GetType() == PROTOCOL_FACTIONPKRAIDCHALLENGE_RE)
	{
	    pGameUI->CommonMessage("Win_Battle_GuildWar_Apply","challenge",(int)pProtocol,0);
	}
	else if(pProtocol->GetType() == PROTOCOL_FACTIONPKRAIDCHALLENGECANCEL_RE)
	{
	    pGameUI->CommonMessage("Win_Battle_GuildWar_Apply","challengecancel",(int)pProtocol,0);
	}
	else if(pProtocol->GetType() == PROTOCOL_FACTIONPKRAIDINVITE)
	{
		pGameUI->CommonMessage("Win_Battle_GuildWar_Apply","invite",(int)pProtocol,0);
	}
	else if(pProtocol->GetType() == PROTOCOL_FACTIONPKRAIDBEGIN)
	{
		pGameUI->CommonMessage("Win_Battle_GuildWar_Applylist","begin",(int)pProtocol,0);
	}
	else if(pProtocol->GetType() == PROTOCOL_FACTIONPKRAIDGETSCORE_RE)
	{
		pGameUI->CommonMessage("Win_Battle_GuildWar_Applylist","getscore",(int)pProtocol,0);
	}
	else if(pProtocol->GetType() == PROTOCOL_FACTIONPKRAIDGETLIST_RE)
	{
		pGameUI->CommonMessage("Win_Battle_GuildWarList","getlist",(int)pProtocol,0);
	}
	else if(pProtocol->GetType() == PROTOCOL_FACTIONPKRAIDSTOPFIGHT)
	{
		pGameUI->CommonMessage("Win_Battle_GuildWar_Applylist","stopfight",(int)pProtocol,0);
	}
	else if(pProtocol->GetType() == PROTOCOL_FACTIONWITHDRAWBONUS_RE)
	{
	    pGameUI->CommonMessage("Win_Battle_GuildWarList","withdrawbonus",(int)pProtocol,0);
	}

}
void CECGameSession::OnKingdomPkOpt(GNET::Protocol* pProtocol)
{
	int errorcode = ERR_SUCCESS;
	CECGameUIMan* pGameUI = (CECGameUIMan*)g_pGame->GetGameRun()->GetUIManager()->GetInGameUIMan();
    if(pProtocol->GetType() == PROTOCOL_KINGDOMBATTLEENTER_RE)
	{
         KingdomBattleEnter_Re* p = (KingdomBattleEnter_Re*)pProtocol;
		 errorcode = p->retcode;

	}
	else if(pProtocol->GetType() == PROTOCOL_KINGDOMBATTLELEAVE_RE)
	{
	     KingdomBattleLeave_Re* p = (KingdomBattleLeave_Re*)pProtocol;
		 errorcode = p->retcode;
	}
	else if(pProtocol->GetType() == PROTOCOL_KINGDOMANNOUNCE_RE)
	{
	    KingdomAnnounce_Re* p = (KingdomAnnounce_Re*)pProtocol;
		pGameUI->CommonMessage("Win_King_Info","announce",(int)pProtocol,0);
		errorcode = p->retcode;
	}
	else if(pProtocol->GetType() == PROTOCOL_KINGDOMAPPOINT_RE)
	{
		KingdomAppoint_Re* p = (KingdomAppoint_Re*)pProtocol;
		pGameUI->CommonMessage("Win_King_Info","appoint",(int)pProtocol,0);
		errorcode = p->retcode;
	}
	else if(pProtocol->GetType() == PROTOCOL_KINGDOMDISCHARGE_RE)
	{
		KingdomDischarge_Re* p =(KingdomDischarge_Re*)pProtocol;
		pGameUI->CommonMessage("Win_King_Info","discharge",(int)pProtocol,0);
		errorcode = p->retcode;
	}
	else if(pProtocol->GetType() == PROTOCOL_KINGDOMSETGAMEATTRI_RE)
	{
		KingdomSetGameAttri_Re* p =(KingdomSetGameAttri_Re*)pProtocol;
		pGameUI->CommonMessage("Win_King_Info","setgameattri",(int)pProtocol,0);
		errorcode = p->retcode;
	}
	else if(pProtocol->GetType() == PROTOCOL_KINGDOMGETINFO_RE)
	{
		KingdomGetInfo_Re* p =(KingdomGetInfo_Re*)pProtocol;
		pGameUI->CommonMessage("Win_King_Info","getinfo",(int)pProtocol,0);
		errorcode = p->retcode;
	}
	else if(pProtocol->GetType() == PROTOCOL_KINGDOMGETTITLE_RE)
	{
	    KingdomGetTitle_Re* p =(KingdomGetTitle_Re*)pProtocol;
		errorcode = p->retcode;
		if(errorcode == ERR_SUCCESS)
		{
		   g_pGame->GetGameRun()->GetHostPlayer()->SetKingdomTitle(p->self_title);
		   g_pGame->GetGameRun()->GetHostPlayer()->SetKingdomKingId(p->king);
		   g_pGame->GetGameRun()->GetHostPlayer()->SetKingdomPoints(p->points);
		   g_pGame->GetGameRun()->GetHostPlayer()->SetKingdomHasInit();
		}
	}
	else if(pProtocol->GetType() == PROTOCOL_KINGDOMTITLECHANGE)
	{
		pGameUI->CommonMessage("Win_King_Info","titlechanged",(int)pProtocol,0);

	}
	else if(pProtocol->GetType() == PROTOCOL_KINGDOMPOINTCHANGE)
	{
		pGameUI->CommonMessage("Win_King_Info","pointchanged",(int)pProtocol,0);
	}
	else if (pProtocol->GetType() == PROTOCOL_KINGDOMKINGCHANGE)
	{
		pGameUI->CommonMessage("Win_King_Info","kingchanged",(int)pProtocol,0);
	}
	else if(pProtocol->GetType() == PROTOCOL_KINGCALLGUARDS_RE)
	{
	    KingCallGuards_Re* p = (KingCallGuards_Re*)pProtocol;
		errorcode = p->retcode;
		pGameUI->CommonMessage("Win_King_Info","callguard",(int)pProtocol,0);
	}
	else if(pProtocol->GetType() == PROTOCOL_KINGGETREWARD_RE)
	{
		KingGetReward_Re* p = (KingGetReward_Re*)pProtocol;
		errorcode = p->retcode;
		pGameUI->CommonMessage("Win_King_Info","getaward",(int)pProtocol,0);
	}
	else if(pProtocol->GetType() == RPC_KINGGUARDINVITE)
	{
		KingGuardInvite* p = (KingGuardInvite*)pProtocol;
		KingGuardInviteArg* pArg = (KingGuardInviteArg*)(p->GetArgument());
		if(pArg->kingname.size())
		{
		   ACString strKingName = ACString((const ACHAR*)(pArg->kingname.begin()),pArg->kingname.size()/sizeof(ACHAR));
	       pGameUI->PopupKingGuardTransmit(strKingName,(DWORD)p);
		}
	}
	else if(pProtocol->GetType() == RPC_QUEENOPENBATHINVITE)
	{
	    QueenOpenBathInvite* p = (QueenOpenBathInvite*)pProtocol; 
		pGameUI->PopupKingOpenBathInvite((DWORD)p);

	}
	else if(pProtocol->GetType() == PROTOCOL_QUEENOPENBATH_RE)
	{
	     QueenOpenBath_Re* p = (QueenOpenBath_Re*)pProtocol;
		 errorcode = p->retcode;
	}
	else if(pProtocol->GetType() == PROTOCOL_KINGISSUETASK_RE)
	{
	     KingIssueTask_Re* p = (KingIssueTask_Re*)pProtocol;
		 errorcode = p->retcode;
		 //发布成功，重新获取任务状态
		 if(errorcode == ERR_SUCCESS)
		  kingwar_getkingtask(g_pGame->GetGameRun()->GetHostPlayer()->GetCharacterID());
	}
	else if(pProtocol->GetType() == PROTOCOL_GETKINGTASK_RE)
	{
	     GetKingTask_Re* p = (GetKingTask_Re*)pProtocol;
		 pGameUI->CommonMessage("Win_King_Info","gettask",(int)pProtocol,0);
		 errorcode = p->retcode;
	}
	if(errorcode != ERR_SUCCESS)
	{
	     CECGameUIMan* pGameUIMan = g_pGame->GetGameRun()->GetUIManager()->GetInGameUIMan();
         pGameUIMan->MessageBox("", pGameUIMan->GetStringFromTable(15551+errorcode), MB_OK, A3DCOLORRGBA(255, 255, 255, 160));
	}
}
void CECGameSession::OnTouchOpt(GNET::Protocol* pProtocol)
{
   int errorcode = ERR_SUCCESS;
   CECGameUIMan* pGameUI = (CECGameUIMan*)g_pGame->GetGameRun()->GetUIManager()->GetInGameUIMan();
   if(pProtocol->GetType() == PROTOCOL_GETTOUCHPOINT_RE)
   {
       GetTouchPoint_Re* p = (GetTouchPoint_Re*)pProtocol;
	   errorcode = p->retcode;
	   pGameUI->CommonMessage("Win_Game_Touch","getpoint",(int)pProtocol,0);
   }
   else if(pProtocol->GetType() == PROTOCOL_GETPASSPORTCASHADD_RE)
   {
       GetPassportCashAdd_Re* p = (GetPassportCashAdd_Re*)pProtocol;
	   errorcode = p->retcode;
	   pGameUI->CommonMessage("Win_Game_Touch","getpassport",(int)pProtocol,0);
   }
   else if(pProtocol->GetType() == PROTOCOL_TOUCHPOINTEXCHANGE_RE)
   {
	   TouchPointExchange_Re* p= (TouchPointExchange_Re*)pProtocol;
	   errorcode = p->retcode;
	   pGameUI->CommonMessage("Win_Game_Touch","pointexchange",(int)pProtocol,0);
   }
   if(errorcode != ERR_SUCCESS)
   {
	   pGameUI->MessageBox("", pGameUI->GetStringFromTable(17920+errorcode), MB_OK, A3DCOLORRGBA(255, 255, 255, 160));
   }
}
void CECGameSession::OnTopFlower(GNET::Protocol* pProtocol)
{
	CECGameUIMan* pGameUI = (CECGameUIMan*)g_pGame->GetGameRun()->GetUIManager()->GetInGameUIMan();
	GNET::TopFlower_Re* p = (GNET::TopFlower_Re*)pProtocol;
	DlgFlowersList* pDlg = (DlgFlowersList*)pGameUI->GetDialog("Win_Roserank");
	if((int)p->isRecv == 1)
	{
		pDlg->ClearReceiveMap();
	}
	else
	{
		pDlg->ClearSend();
	}
	
	
	pDlg->UpdateUiData((int)p->isRecv,p->page + 1,p->my_pos + 1,p->my_count,p->all_count);
	
	for(int i=0;i<(int)p->data.size();i++)
	{
		GNET::GTopFlowerData& pData = p->data[i];
		FLOWER_DATA pRece;
		pRece.nIndex = (p->page * 20) + i + 1;
		pRece.roleid = pData.roleid;
		pRece.rolename =ACString((const ACHAR*)pData.rolename.begin(), pData.rolename.size() / sizeof(ACHAR));
		pRece.stamp = pData.stamp;
		pRece.count = pData.count;
		pRece.gender = pData.gender;
// 		
// 		ACString strText;
// 		strText.Format( _AL("序列%dID%d名字%s总数%d男女%d接或收%d\n"), pRece.nIndex,pRece.roleid,pRece.rolename,pRece.count,pRece.gender,(int)p->isRecv);
// 		OutputDebugString(strText);


		pDlg->AddMap((int)p->isRecv,pRece);
		
	
	}
// 	ACString strText;
// 	strText.Format( _AL("-------------------------\n"));
// 	OutputDebugString(strText);	
	pDlg->UpdateFlowerList((int)p->isRecv);
}

void CECGameSession::OnTopFlowerMsg(GNET::Protocol* pProtocol)
{
	CECGameUIMan* pGameUI = (CECGameUIMan*)g_pGame->GetGameRun()->GetUIManager()->GetInGameUIMan();
	GNET::TopFlowerMsg_Re* p = (GNET::TopFlowerMsg_Re*)pProtocol;
	int nZoneID = p->zoneid;
	int nHostID = p->myself_roleid;
	int nFlowerNum =p->count;
	int ndistrictID = p->district_id;
	ACString szAName = ACString((const ACHAR*)p->send_rolename.begin(), p->send_rolename.size() / sizeof(ACHAR));
	ACString szBName = ACString((const ACHAR*)p->recv_rolename.begin(), p->recv_rolename.size() / sizeof(ACHAR));
	ACString szMsg = ACString((const ACHAR*)p->msg.begin(), p->msg.size() / sizeof(ACHAR));
	pGameUI->FilterBadWords(szMsg);
// 	ACString strText;
// 	strText.Format( _AL("Msg nZoneID=%d nHostID=%d nFlowerNum=%d ndistrictID=%d szSName=%s szRName=%s\n"), nZoneID,nHostID,nFlowerNum,ndistrictID,szAName,szBName,szMsg);
// 	OutputDebugString(strText);
	if(nFlowerNum < 9999)
	{
		if(GetZoneID() == nZoneID)
		{
			CECHostPlayer * pHost = (CECHostPlayer*)g_pGame->GetGameRun()->GetHostPlayer();
			if(pHost->GetCharacterID() == nHostID)
			{
				ACString szTemp;
				szTemp.Format(pGameUI->GetStringFromTable(17832),szAName,nFlowerNum);
				CDlgFlowerGfx* pDlgGfx = (CDlgFlowerGfx*)pGameUI->GetDialog("Win_Rose_Gfx");
				pDlgGfx->setContentToChat(GP_CHAT_WHISPER,szTemp);
				if(nFlowerNum >= 99)
				{
					CDlgFlowerManifesto* pDlg = (CDlgFlowerManifesto*)pGameUI->GetDialog("Win_RosePopup");
					pDlg->setCloseTime(5000); 
					szTemp.Format(pGameUI->GetStringFromTable(17831),szAName,nFlowerNum);
					pDlg->setWordsText(szTemp);
					pDlg->Show(!pDlg->IsShow());
					pDlgGfx->setCloseTime(5000);
					pDlgGfx->setPlayGfx(nFlowerNum);
					pDlgGfx->Show(true);
				}
				
			}
			else
			{	
				
// 				CDlgFlowerGfx* pDlgGfx = (CDlgFlowerGfx*)pGameUI->GetDialog("Win_Rose_Gfx");
// 				pDlgGfx->setCloseTime(3000);
// 				pDlgGfx->setPlayGfx(nFlowerNum);
// 				pDlgGfx->Show(!pDlgGfx->IsShow());
			}
			if(nFlowerNum >= 520)
			{
				ACString szTemp;
				szTemp.Format(pGameUI->GetStringFromTable(17827),szAName,szBName,nFlowerNum);
				CDlgFlowerAnounce* pDlg = (CDlgFlowerAnounce*)pGameUI->GetDialog("Win_RoseAnounce");
				ACString szContent;
				szContent.Format(pGameUI->GetStringFromTable(17828),szAName,szBName,szMsg);
				ACString szSS;
				szSS.Format(_AL("%s%s"),szTemp,szContent);
				pDlg->addMsg(szContent);
				
				CDlgFlowerGfx* pDlgGfx = (CDlgFlowerGfx*)pGameUI->GetDialog("Win_Rose_Gfx");
				pDlgGfx->setContentToChat(GP_CHAT_SYSTEM,szTemp);
			}
		}
	}
	else
	{
		CECHostPlayer * pHost = (CECHostPlayer*)g_pGame->GetGameRun()->GetHostPlayer();
		if(pHost->GetCharacterID() == nHostID)
		{
			ACString szTemp;
			szTemp.Format(pGameUI->GetStringFromTable(17832),szAName,nFlowerNum);
			CDlgFlowerGfx* pDlgGfx = (CDlgFlowerGfx*)pGameUI->GetDialog("Win_Rose_Gfx");
			pDlgGfx->setContentToChat(GP_CHAT_WHISPER,szTemp);
			if(nFlowerNum >= 99)
			{
				CDlgFlowerManifesto* pDlg = (CDlgFlowerManifesto*)pGameUI->GetDialog("Win_RosePopup");
				pDlg->setCloseTime(5000); 
				szTemp.Format(pGameUI->GetStringFromTable(17831),szAName,nFlowerNum);
				pDlg->setWordsText(szTemp);
				pDlg->Show(true);
			}
			
		}

		ACString szTemp;
		szTemp.Format(pGameUI->GetStringFromTable(17827),szAName,szBName,nFlowerNum);
		CECGame::SERVERINFO* pInfo = g_pGame->GetSrcServerInfo(nZoneID);
// 		ACString strText;
// 		strText.Format( _AL("%d\n"),nZoneID);
// 		OutputDebugString(strText);	
		ACString szContent;
		szContent.Format(pGameUI->GetStringFromTable(17828),szAName,szBName,szMsg);
		ACString szServer;
		if (pInfo)
		{	
			szServer.Format(pGameUI->GetStringFromTable(17829),AS2AC(pInfo->szServerGroup), AS2AC(pInfo->szServerName), szTemp,szContent);	
		}
		else
		{	
			szServer = szContent;
		}
		CDlgFlowerAnounce* pDlg = (CDlgFlowerAnounce*)pGameUI->GetDialog("Win_RoseAnounce");
		
		pDlg->addMsg(szServer);
		
		CDlgFlowerGfx* pDlgGfx = (CDlgFlowerGfx*)pGameUI->GetDialog("Win_Rose_Gfx");
		pDlgGfx->setCloseTime(5000);
		pDlgGfx->setPlayGfx(nFlowerNum);
		pDlgGfx->setContentToChat(GP_CHAT_SYSTEM,szServer);
		pDlgGfx->Show(true);
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

//搓澡邀请回复
void CECGameSession::c2s_SendCmdKingBathInviteReply(int who,int param)
{
    if (param == 0) //同意
    {
		const ACHAR* pName = g_pGame->GetGameRun()->GetPlayerName(who,false);
		g_pGame->GetGameRun()->GetHostPlayer()->GetFriendMan()->AddTempFriend(who, pName);
    }
	::c2s_SendCmdKingBathInviteReply(who,param);
}

void CECGameSession::c2s_CmdQilinInviteReply(int who, int param)
{
	if(param == 0) //同意
	{
		const ACHAR* pName = g_pGame->GetGameRun()->GetPlayerName(who, false);
		g_pGame->GetGameRun()->GetHostPlayer()->GetFriendMan()->AddTempFriend(who, pName);
	}
	::c2s_SendCmdQilinInviteReply(who, param);
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

void CECGameSession::c2s_CmdExgMountWingItem(int iIndex1, int iIndex2)
{
	FreezeHostItem(IVTRTYPE_MOUNTWING, iIndex1, true);
	FreezeHostItem(IVTRTYPE_MOUNTWING, iIndex1, true);
	::c2s_SendCmdExgMountWingItem(iIndex1, iIndex2);
}

void CECGameSession::c2s_CmdExgIvtrMountWingItem(int iIvtrIdx, int iMwIdx)
{
	FreezeHostItem(IVTRTYPE_PACK, iIvtrIdx, true);
	FreezeHostItem(IVTRTYPE_MOUNTWING, iMwIdx, true);
	::c2s_SendCmdExgIvtrMountWingItem(iIvtrIdx, iMwIdx);
}

void CECGameSession::c2s_CmdExgEquMountWingItem(int iEquIdx, int iMwIdx)
{
	FreezeHostItem(IVTRTYPE_EQUIPPACK, iEquIdx, true);
	FreezeHostItem(IVTRTYPE_MOUNTWING, iMwIdx, true);
	::c2s_SendCmdExgEquMountWingItem(iEquIdx, iMwIdx);
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
	
	::c2s_SendCmdNPCSevSellBuyBack(0, iItemNum, aItems);
}

void CECGameSession::c2s_CmdNPCSevSellBuyBack(int iItemNum, C2S::npc_trade_item* aItems)
{
	for (int i=0; i < iItemNum; i++)
		FreezeHostItem(IVTRTYPE_CLIENT_BUYBACK, aItems[i].index, true);
	
	::c2s_SendCmdNPCSevSellBuyBack(1, iItemNum, aItems);
}

void CECGameSession::c2s_CmdNPCSevBoothSell(int idBooth, int iItemNum, C2S::npc_booth_item* aItems)
{ 
	for (int i=0; i < iItemNum; i++)
		FreezeHostItem(IVTRTYPE_PACK, aItems[i].inv_index, true);
	
	::c2s_SendCmdNPCSevBoothSell(idBooth, iItemNum, aItems); 
}

void CECGameSession::c2s_CmdNPCSevPetUpgradeWithXianDou(int inv_index_pet, int type)
{
	using namespace C2S;
	
#pragma pack(1)
	
	struct CONTENT
	{
		int inv_index_pet;
		int type;
	};
	
#pragma pack()
	
	int iSize = sizeof (cmd_header) + sizeof (cmd_sevnpc_serve) + sizeof (CONTENT);
	BYTE* pBuf = new BYTE [iSize];
	if (!pBuf)
		return;
	
	((cmd_header*)pBuf)->cmd = C2S::SEVNPC_SERVE;
	
	cmd_sevnpc_serve* pCmd = (cmd_sevnpc_serve*)(pBuf + sizeof (cmd_header));
	pCmd->service_type = GP_PET_UPGRADE_WITH_XIANDOU;
	pCmd->len = sizeof (CONTENT);
	
	BYTE* pData = pBuf + sizeof (cmd_header) + sizeof (cmd_sevnpc_serve);
	CONTENT* pContent = (CONTENT*)pData;
	pContent->inv_index_pet = inv_index_pet;
	pContent->type = type;
	
	g_pGame->GetGameSession()->SendGameData(pBuf, iSize);
	
	delete [] pBuf;
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

void CECGameSession::c2s_CmdNPCSevItemTrade(int shop_index, const C2S::item_trade* pItemList, size_t count)
{
	using namespace C2S;

#pragma pack(1)

	struct CONTENT
	{
		int index;
		size_t count;
		item_trade p[1];
	};

#pragma pack()

	int sz =  sizeof(int) + sizeof (size_t) + sizeof(item_trade) * count;
	int iSize = sizeof (cmd_header) + sizeof (cmd_sevnpc_serve) + sz;
	BYTE* pBuf = new BYTE[iSize];

	((cmd_header*)pBuf)->cmd = C2S::SEVNPC_SERVE;

	cmd_sevnpc_serve* pCmd = (cmd_sevnpc_serve*)(pBuf + sizeof (cmd_header));
	pCmd->service_type = GP_NPCSEV_ITEM_TRADE;
	pCmd->len = sz;

	BYTE* pData = pBuf + sizeof (cmd_header) + sizeof (cmd_sevnpc_serve);
	CONTENT* pContent = (CONTENT*)pData;
	pContent->index = shop_index;
	pContent->count = count;
	memcpy(pContent->p, pItemList, sizeof(item_trade) * count);

	g_pGame->GetGameSession()->SendGameData(pBuf, iSize);
	delete[] pBuf;
}

void CECGameSession::c2s_CmdNPCSevChangeName(int nItemSlot, int nItemId, const ACHAR* szName)
{
	using namespace C2S;
	
#pragma pack(1)
	
	struct CONTENT
	{
		int item_index;
		int item_id;
		size_t name_len;
		char name[1];
	};
	
#pragma pack()
	
	int sz =  2*sizeof(int) + sizeof (size_t) + a_strlen(szName) * sizeof(ACHAR);
	int iSize = sizeof (cmd_header) + sizeof (cmd_sevnpc_serve) + sz;
	BYTE* pBuf = new BYTE[iSize];
	
	((cmd_header*)pBuf)->cmd = C2S::SEVNPC_SERVE;
	
	cmd_sevnpc_serve* pCmd = (cmd_sevnpc_serve*)(pBuf + sizeof (cmd_header));
	pCmd->service_type = GP_CHANGE_NAME;
	pCmd->len = sz;
	
	BYTE* pData = pBuf + sizeof (cmd_header) + sizeof (cmd_sevnpc_serve);
	CONTENT* pContent = (CONTENT*)pData;
	pContent->item_index = nItemSlot;
	pContent->item_id = nItemId;
	pContent->name_len = sizeof(ACHAR)*a_strlen(szName);
	memcpy(pContent->name, szName, pContent->name_len);
	
	g_pGame->GetGameSession()->SendGameData(pBuf, iSize);
	delete[] pBuf;
}

void CECGameSession::c2s_CmdNPCSevChangeFamilyGuildName(int nItemSlot, int nItemId, const ACHAR* szName, char cType)
{
	using namespace C2S;
	
#pragma pack(1)
	
	struct CONTENT
	{
		int item_index;
		int item_id;
		char type;			// 0-帮派   1-家族
		size_t name_len;
		char name[1];
	};
	
#pragma pack()
	
	int sz =  2*sizeof(int) + sizeof(char) + sizeof (size_t) + a_strlen(szName) * sizeof(ACHAR);
	int iSize = sizeof (cmd_header) + sizeof (cmd_sevnpc_serve) + sz;
	BYTE* pBuf = new BYTE[iSize];
	
	((cmd_header*)pBuf)->cmd = C2S::SEVNPC_SERVE;
	
	cmd_sevnpc_serve* pCmd = (cmd_sevnpc_serve*)(pBuf + sizeof (cmd_header));
	pCmd->service_type = GP_NPCSEV_CHANGE_FAMILYGUILD_NAME;
	pCmd->len = sz;
	
	BYTE* pData = pBuf + sizeof (cmd_header) + sizeof (cmd_sevnpc_serve);
	CONTENT* pContent = (CONTENT*)pData;
	pContent->item_index = nItemSlot;
	pContent->item_id = nItemId;
	pContent->type = cType;
	pContent->name_len = sizeof(ACHAR)*a_strlen(szName);
	memcpy(pContent->name, szName, pContent->name_len);
	
	g_pGame->GetGameSession()->SendGameData(pBuf, iSize);
	delete[] pBuf;
}

void CECGameSession::c2s_CmdNPCSevActiveUiTransid(int transid)
{
	using namespace C2S;

#pragma pack(1)
    struct  CONTENT
    {
        int index;  // 激活点索引
    };
#pragma pack()

	int iSize = sizeof (cmd_header) + sizeof (cmd_sevnpc_serve) + sizeof(CONTENT);
	BYTE* pBuf = new BYTE[iSize];

	((cmd_header*)pBuf)->cmd = C2S::SEVNPC_SERVE;

	cmd_sevnpc_serve* pCmd = (cmd_sevnpc_serve*)(pBuf + sizeof (cmd_header));
	pCmd->service_type = GP_NPCSEV_ACTIVE_UI_TRANSFER;
	pCmd->len = sizeof(CONTENT);

	BYTE* pData = pBuf + sizeof (cmd_header) + sizeof (cmd_sevnpc_serve);
	CONTENT* pContent = (CONTENT*)pData;
	pContent->index = transid;

	g_pGame->GetGameSession()->SendGameData(pBuf, iSize);
	delete[] pBuf;
}

void CECGameSession::c2s_CmdNPCSevUseUiTransfer(int transid)
{
	using namespace C2S;

#pragma pack(1)
    struct  CONTENT
    {
        int index;  // 目标点索引
    };
#pragma pack()

	int iSize = sizeof (cmd_header) + sizeof (cmd_sevnpc_serve) + sizeof(CONTENT);
	BYTE* pBuf = new BYTE[iSize];

	((cmd_header*)pBuf)->cmd = C2S::SEVNPC_SERVE;

	cmd_sevnpc_serve* pCmd = (cmd_sevnpc_serve*)(pBuf + sizeof (cmd_header));
	pCmd->service_type = GP_NPCSEV_USE_UI_TRANSER;
	pCmd->len = sizeof(CONTENT);

	BYTE* pData = pBuf + sizeof (cmd_header) + sizeof (cmd_sevnpc_serve);
	CONTENT* pContent = (CONTENT*)pData;
	pContent->index = transid;

	g_pGame->GetGameSession()->SendGameData(pBuf, iSize);
	delete[] pBuf;
}

void CECGameSession::c2s_CmdNPCSevEquipSlot(int item_id, int item_index, int stone_id, int stone_index)
{
	using namespace C2S;

#pragma pack(1)
	struct CONTENT
	{
		int item_index;		// 物品 包裹装备
		int item_id;		// 物品 id
		int stone_index;	// 道具 index
		int stone_id;		// 道具 id
	};
#pragma pack()

	int iSize = sizeof (cmd_header) + sizeof (cmd_sevnpc_serve) + sizeof(CONTENT);
	BYTE* pBuf = new BYTE[iSize];

	((cmd_header*)pBuf)->cmd = C2S::SEVNPC_SERVE;

	cmd_sevnpc_serve* pCmd = (cmd_sevnpc_serve*)(pBuf + sizeof (cmd_header));
	pCmd->service_type = GP_NPCSEV_EQUIP_SLOT2;
	pCmd->len = sizeof(CONTENT);

	BYTE* pData = pBuf + sizeof (cmd_header) + sizeof (cmd_sevnpc_serve);
	CONTENT* pContent = (CONTENT*)pData;
	pContent->item_index = item_index;
	pContent->item_id = item_id;
	pContent->stone_index = stone_index;
	pContent->stone_id = stone_id;

	g_pGame->GetGameSession()->SendGameData(pBuf, iSize);
	delete[] pBuf;
}

void CECGameSession::c2s_CmdNPCSevAstrologyInstall(int item_id, int item_index, int stone_id, int stone_index, int slot_index)
{
	using namespace C2S;

#pragma pack(1)
	struct CONTENT
	{
		int item_index;		// 物品 包裹装备
		int item_id;		// 物品 id
		int stone_index;	// 道具 index
		int stone_id;		// 道具 id
		int slot_index;		// 
	};
#pragma pack()

	int iSize = sizeof (cmd_header) + sizeof (cmd_sevnpc_serve) + sizeof(CONTENT);
	BYTE* pBuf = new BYTE[iSize];

	((cmd_header*)pBuf)->cmd = C2S::SEVNPC_SERVE;

	cmd_sevnpc_serve* pCmd = (cmd_sevnpc_serve*)(pBuf + sizeof (cmd_header));
	pCmd->service_type = GP_NPCSEV_ASTROLOGY_INSTALL;
	pCmd->len = sizeof(CONTENT);

	BYTE* pData = pBuf + sizeof (cmd_header) + sizeof (cmd_sevnpc_serve);
	CONTENT* pContent = (CONTENT*)pData;
	pContent->item_index = item_index;
	pContent->item_id = item_id;
	pContent->stone_index = stone_index;
	pContent->stone_id = stone_id;
	pContent->slot_index = slot_index;

	g_pGame->GetGameSession()->SendGameData(pBuf, iSize);
	delete[] pBuf;
}

void CECGameSession::c2s_CmdNPCSevAstrologyUninstall(int item_id, int item_index, int slot_index)
{
	using namespace C2S;

#pragma pack(1)
	struct CONTENT
	{
		int item_index;		// 物品 包裹装备
		int item_id;		// 物品 id
		int slot_index;		// 
	};
#pragma pack()

	int iSize = sizeof (cmd_header) + sizeof (cmd_sevnpc_serve) + sizeof(CONTENT);
	BYTE* pBuf = new BYTE[iSize];

	((cmd_header*)pBuf)->cmd = C2S::SEVNPC_SERVE;

	cmd_sevnpc_serve* pCmd = (cmd_sevnpc_serve*)(pBuf + sizeof (cmd_header));
	pCmd->service_type = GP_NPCSEV_ASTROLOGY_UNINSTALL;
	pCmd->len = sizeof(CONTENT);

	BYTE* pData = pBuf + sizeof (cmd_header) + sizeof (cmd_sevnpc_serve);
	CONTENT* pContent = (CONTENT*)pData;
	pContent->item_index = item_index;
	pContent->item_id = item_id;
	pContent->slot_index = slot_index;

	g_pGame->GetGameSession()->SendGameData(pBuf, iSize);
	delete[] pBuf;
}
void CECGameSession::c2s_CmdNPCSevKingdomEnter(char field_type)
{
    using namespace C2S;
#pragma pack(1)
	struct CONTENT
	{
		char field_type;
	};
#pragma pack()

	int iSize = sizeof (cmd_header) + sizeof (cmd_sevnpc_serve) + sizeof(CONTENT);
	BYTE* pBuf = new BYTE[iSize];
	
	((cmd_header*)pBuf)->cmd = C2S::SEVNPC_SERVE;
	
	cmd_sevnpc_serve* pCmd = (cmd_sevnpc_serve*)(pBuf + sizeof (cmd_header));
	pCmd->service_type = GP_NPCSEV_KINGDOM_ENTER;
	pCmd->len = sizeof(CONTENT);
	
	BYTE* pData = pBuf + sizeof (cmd_header) + sizeof (cmd_sevnpc_serve);
	CONTENT* pContent = (CONTENT*)pData;
	pContent->field_type = field_type;

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
void CECGameSession::c2s_CmdBuyKingdomItem(char type,int index)
{
    using namespace C2S;
	
	int iSize = sizeof (cmd_header) + sizeof (buy_kingdom_item);
	BYTE* pBuf = new BYTE [iSize];
	if (!pBuf)
		return;
	
	((cmd_header*)pBuf)->cmd = C2S::BUY_KINGDOM_ITEM;
	
	buy_kingdom_item* pCmd = (buy_kingdom_item*)(pBuf + sizeof (cmd_header));
	pCmd->type = type;
	pCmd->index = index;
	g_pGame->GetGameSession()->SendGameData(pBuf, iSize);
	
	delete [] pBuf;
}
void CECGameSession::c2s_CmdGetTouchAward(char type,int count)
{
	using namespace C2S;
	
	int iSize = sizeof (cmd_header) + sizeof (get_touch_award);
	BYTE* pBuf = new BYTE [iSize];
	if (!pBuf)
		return;
	
	((cmd_header*)pBuf)->cmd = C2S::GET_TOUCH_AWARD;
	
	get_touch_award* pCmd = (get_touch_award*)(pBuf + sizeof (cmd_header));
	pCmd->type = type;
	pCmd->count = count;
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
void CECGameSession::c2s_CmdPetChangeShape(int iPetIndex, int iShapeId)
{
	using namespace C2S;

	int iSize = sizeof (cmd_header) + sizeof (pet_change_shape);
	BYTE* pBuf = new BYTE [iSize];
	if (!pBuf)
		return;

	((cmd_header*)pBuf)->cmd = C2S::PET_CHANGE_SHAPE;

	pet_change_shape* pCmd = (pet_change_shape*)(pBuf + sizeof (cmd_header));
	pCmd->pet_index	= iPetIndex;
	pCmd->shape_id = iShapeId;

	g_pGame->GetGameSession()->SendGameData(pBuf, iSize);

	delete [] pBuf;

}
void CECGameSession::c2s_CmdPetFlyRefineAttr(int iPetIndex, int iAttrIndex, int iAssistId, int iAssistCount)
{
	using namespace C2S;

	int iSize = sizeof (cmd_header) + sizeof (pet_refine_attr);
	BYTE* pBuf = new BYTE [iSize];
	if (!pBuf)
		return;

	((cmd_header*)pBuf)->cmd = C2S::PET_REFINE_ATTR;

	pet_refine_attr* pCmd = (pet_refine_attr*)(pBuf + sizeof (cmd_header));
	pCmd->pet_index	= iPetIndex;
	pCmd->attr_index = iAttrIndex;
	pCmd->assist_id = iAssistId;
	pCmd->assist_count = iAssistCount;

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
}

void CECGameSession::get_platform_token(AString strType, int toaid, int tozoneid)
{
	using namespace GNET;
	
	if (strType=="gt")
	{
		static DWORD dwTime = GetTickCount()-60100;
		DWORD dwTimeNow = GetTickCount();
		if (dwTimeNow<(dwTime+60000))	// 10s冷却
		{
			CECGameUIMan* pGameUI = (CECGameUIMan*)g_pGame->GetGameRun()->GetUIManager()->GetInGameUIMan();
			pGameUI->AddChatMessage(pGameUI->GetStringFromTable(8738), GP_CHAT_MISC);
			return;
		}
		
		CECHostPlayer* pHost = g_pGame->GetGameRun()->GetHostPlayer();
		// 天界屏蔽GT
		if(pHost->GetSrcServerID()!=0)
			return;
		
		dwTime = dwTimeNow;
	}
	
	SSOGetTicketReq p;
	p.toaid = toaid;
	p.user.userid = GetUserID();
	p.tozoneid = tozoneid;
	p.local_context = Octets(strType.GetBuffer(0), strType.GetLength());
	SendNetData(p);
}

void CECGameSession::open_url_by_token(AString strType, int toaid, int tozoneid)
{
	const char *szCountry = g_pGame->GetConfigs()->GetCountryRegion();
	if ( 0 == stricmp(szCountry, "cn") || 0 == stricmp(szCountry, "sg") )
	{
		get_platform_token(strType, toaid, tozoneid);
		return;
	}

	AString strText = "";
	AIniFile iniFile;
	if(iniFile.Open("configs\\url.ini"))
	{
		strText = iniFile.GetValueAsString("ingame", strType, "");
		iniFile.Close();
	}
	if( strText != "" )
		g_pGame->OpenURL(strText);
}


void CECGameSession::OnPrtcFlowBattleJoinRe(GNET::Protocol* pProtocol)
{
	CECGameUIMan* pGameUI = (CECGameUIMan*)g_pGame->GetGameRun()->GetUIManager()->GetInGameUIMan();
	GNET::OpenBanquetJoin_Re* p = (GNET::OpenBanquetJoin_Re*)pProtocol;
	if(p->retcode == 1)
	{
		DlgBattleFlowInfoWaitMax* pDlgMax = (DlgBattleFlowInfoWaitMax*)pGameUI->GetDialog("Win_Battle_SeniorPK_WaitLine");
		if(pDlgMax)
		{
			pDlgMax->UpDateFlowWait(p->queue_number,p->gs_id,p->map_id);
			pDlgMax->Show(true);
		}
	}
	else if(p->retcode == 0)
	{
		c2s_CmdSendFlowBattleEnter(p->gs_id,p->map_id,0);
	}
}


void CECGameSession::OnPrtcFlowBattleEnterRe(GNET::Protocol* pProtocol)
{
	CECGameUIMan* pGameUI = (CECGameUIMan*)g_pGame->GetGameRun()->GetUIManager()->GetInGameUIMan();
	GNET::OpenBanquetEnter_Re* p = (GNET::OpenBanquetEnter_Re*)pProtocol;
// 		unsigned short retcode;
// 		int roleid;
// 		int endtime;
// 		unsigned int localsid;
// 		char iscross;
	if(p->retcode == 1)
	{
			
	}
}


void CECGameSession::OnPrtcFlowBattleDragEnterRe(GNET::Protocol* pProtocol)
{
	CECGameUIMan* pGameUI = (CECGameUIMan*)g_pGame->GetGameRun()->GetUIManager()->GetInGameUIMan();
	GNET::OpenBanquetDrag_Re* p = (GNET::OpenBanquetDrag_Re*)pProtocol;

	DlgBattleFlowInfoPrompt* pDlgPrompt = (DlgBattleFlowInfoPrompt*)pGameUI->GetDialog("Win_Battle_SeniorPK_Enter");
	if(pDlgPrompt)
	{
		pDlgPrompt->UpDateTime(GetTickCount()+30*1000,p->gs_id,p->map_id,p->iscross);
		pDlgPrompt->Show(true);
	}
	DlgBattleFlowInfoWaitMax* pDlgMax = (DlgBattleFlowInfoWaitMax*)pGameUI->GetDialog("Win_Battle_SeniorPK_WaitLine");
	if(pDlgMax)
	{
		pDlgMax->Show(false);
	}
}


void CECGameSession::OnPrtcFlowBattleQueueCountRe(GNET::Protocol* pProtocol)
{
	CECGameUIMan* pGameUI = (CECGameUIMan*)g_pGame->GetGameRun()->GetUIManager()->GetInGameUIMan();
	GNET::OpenBanquetQueueCount_Re* p = (GNET::OpenBanquetQueueCount_Re*)pProtocol;
	DlgBattleFlowInfoWaitMax* pDlgMax = (DlgBattleFlowInfoWaitMax*)pGameUI->GetDialog("Win_Battle_SeniorPK_WaitLine");
	if(pDlgMax)
	{
		pDlgMax->UpDateFlowWait(p->queue_number,p->gs_id,p->map_id);
		//pDlgMax->Show(true);
	}
}

void CECGameSession::OnPrtcFlowBattleGetFieldRe(GNET::Protocol* pProtocol)
{
	CECGameUIMan* pGameUI = (CECGameUIMan*)g_pGame->GetGameRun()->GetUIManager()->GetInGameUIMan();
	GNET::OpenBanquetGetField_Re* p = (GNET::OpenBanquetGetField_Re*)pProtocol;
	DlgBattleFlowInfoMax* pDlgMax = (DlgBattleFlowInfoMax*)pGameUI->GetDialog("Win_Battle_SeniorPK_Score");
	if(p->retcode == 0)
	{
		if(pDlgMax)
		{
			int i;
			for(i = 0; i < (int)p->red_team.size(); i++ )
			{
				ACString strName = ACString((const ACHAR*)p->red_team[i].name.begin(), p->red_team[i].name.size() / sizeof (ACHAR));
				pDlgMax->addFlowName(p->red_team[i].roleid,strName);
			}
			for( i = 0; i < (int)p->blue_team.size(); i++ )
			{
				ACString strName = ACString((const ACHAR*)p->blue_team[i].name.begin(), p->blue_team[i].name.size() / sizeof (ACHAR));
				pDlgMax->addFlowName(p->red_team[i].roleid,strName);
			}
		}
	}
	else
	{
		
	}
}

void CECGameSession::OnPrtcFlowBattleNotifyRe(GNET::Protocol* pProtocol)
{
	GNET::OpenBanquetNotify_Re* p = (GNET::OpenBanquetNotify_Re*)pProtocol;

	int nErr = p->retcode;
	CECGameUIMan* pGameUIMan = g_pGame->GetGameRun()->GetUIManager()->GetInGameUIMan();
	pGameUIMan->MessageBox(18601 + nErr);
}


void CECGameSession::OnPrtcFaciondynamicRe(GNET::Protocol* pProtocol)
{
	CECGameUIMan* pGameUI = (CECGameUIMan*)g_pGame->GetGameRun()->GetUIManager()->GetInGameUIMan();
	CDlgGuildInfo* pDlg = (CDlgGuildInfo*)pGameUI->GetDialog("Win_Guild_Info");
	if(pDlg)
	{
		pDlg->UpDateGuildInfo(pProtocol);
	}
}

void CECGameSession::OnPrtcFaciondMsgRe(GNET::Protocol* pProtocol)
{
	GNET::GetFactionMsg_Re* p = (GNET::GetFactionMsg_Re*)pProtocol;
}


void CECGameSession::OnPrtcFactionPostMsg(GNET::Protocol* pProtocol)
{
	GNET::FactionPostMsg_Re* p = (GNET::FactionPostMsg_Re*)pProtocol;
}

void  CECGameSession::OnPrtcCreateFacBase_Re(GNET::Protocol* pProtocol)
{
	GNET::CreateFacBase_Re* p = (GNET::CreateFacBase_Re*)pProtocol; 
	//int retcode;
	//int roleid;
	//char enterable;
		int nErr = p->retcode;
		CECGameUIMan* pGameUIMan = g_pGame->GetGameRun()->GetUIManager()->GetInGameUIMan();
		pGameUIMan->MessageBox(20150 + nErr);
}
void  CECGameSession::OnPrtcFacBaseEnter_Re(GNET::Protocol* pProtocol)
{
	GNET::FacBaseEnter_Re* p = (GNET::FacBaseEnter_Re*)pProtocol;
	int nErr = p->retcode;
	CECGameUIMan* pGameUIMan = g_pGame->GetGameRun()->GetUIManager()->GetInGameUIMan();
	pGameUIMan->MessageBox(20150 + nErr);
	//int retcode;
	//int roleid;
}

void CECGameSession::OnPrtcGetFacActTopTable_Re(GNET::Protocol* pProtocol)
{
	CECGameUIMan* pGameUI = (CECGameUIMan*)g_pGame->GetGameRun()->GetUIManager()->GetInGameUIMan();
	CDlgGuildLandCreate* pDlg = (CDlgGuildLandCreate*)pGameUI->GetDialog("Win_Guild_LandCreate");
	if(pDlg)
	{
		pDlg->UpDateList(pProtocol);
	}

}

void CECGameSession::OnPrtcFacBaseStopNotice(GNET::Protocol* pProtocol)
{
	CECGameUIMan* pGameUI = (CECGameUIMan*)g_pGame->GetGameRun()->GetUIManager()->GetInGameUIMan();
	CDlgGuildBuild* pDlg = (CDlgGuildBuild*)pGameUI->GetDialog("Win_Guild_Build");
	CDlgGuildMain* pGuildMain = (CDlgGuildMain*)pGameUI->GetDialog("Win_GuildMain");
	if(pDlg)
	{
		pDlg->SetIsSend(false);
		if(pGuildMain->IsShow())
		{
			pGuildMain->Show(true);
		}
	}
	DlgGuildShop* pShopDlg = (DlgGuildShop*)pGameUI->GetDialog("Win_Guild_Shop");
	if(pShopDlg)
	{
		pShopDlg->SetIsLoadAuction(false);
	}
	
}

void CECGameSession::OnPrtcCrssvrTeamsNotifyRe(GNET::Protocol* pProtocol)		//  PROTOCOL_CRSSVRTEAMSNOTIFY_RE
{
	GNET::CrssvrTeamsNotify_Re* p = (GNET::CrssvrTeamsNotify_Re*)pProtocol;
	int nErr = p->retcode - 300;
	if(nErr ==  21)	// 解散成功
	{
		CECGameUIMan* pGameUI = (CECGameUIMan*)g_pGame->GetGameRun()->GetUIManager()->GetInGameUIMan();
		DlgCrssvrTeamsGroupInfo* pDlg = (DlgCrssvrTeamsGroupInfo*)pGameUI->GetDialog("Win_Battle_Dungeon_6v6_GroupInfo");
		pDlg->ClearData();
		CECGameUIMan* pGameUIMan = g_pGame->GetGameRun()->GetUIManager()->GetInGameUIMan();
		pGameUIMan->MessageBox(20032);
		DlgCrssvrTeamsMain* pCrssvrTeamsMainDlg = (DlgCrssvrTeamsMain*)pGameUI->GetDialog("Win_Battle_Dungeon_6v6_GroupMain");
		pCrssvrTeamsMainDlg->ClearData();
	}
	else if(p->retcode == 327)
	{
		CECGameUIMan* pGameUI = (CECGameUIMan*)g_pGame->GetGameRun()->GetUIManager()->GetInGameUIMan();
		ACString strName((const ACHAR*)p->name.begin(), p->name.size()/sizeof(ACHAR));
		ACString szTemp;
		szTemp.Format(pGameUI->GetStringFromTable(20047),strName);
		pGameUI->MessageBox("", szTemp, MB_OK, A3DCOLORRGBA(255, 255, 255, 160));
	}
	else if(p->retcode == 328)
	{
		CECGameUIMan* pGameUIMan = g_pGame->GetGameRun()->GetUIManager()->GetInGameUIMan();
		pGameUIMan->MessageBox(20048);
	}
	else if(p->retcode == 329)
	{
		CECGameUIMan* pGameUI = (CECGameUIMan*)g_pGame->GetGameRun()->GetUIManager()->GetInGameUIMan();
		ACString strName((const ACHAR*)p->name.begin(), p->name.size()/sizeof(ACHAR));
		ACString szTemp;
		szTemp.Format(pGameUI->GetStringFromTable(20049),strName);
		pGameUI->MessageBox("", szTemp, MB_OK, A3DCOLORRGBA(255, 255, 255, 160));
	}
	else
	{
		CECGameUIMan* pGameUIMan = g_pGame->GetGameRun()->GetUIManager()->GetInGameUIMan();
		pGameUIMan->MessageBox(20011 + nErr);
	}
	//int retcode;
	//int roleid;
	//int teamid;
}
void CECGameSession::OnPrtcCrssvrTeamsListRe(GNET::Protocol* pProtocol)		//  PROTOCOL_CRSSVRTEAMSNOTIFY_RE
{
	GNET::CrssvrTeamsRoleList_Re* p = (GNET::CrssvrTeamsRoleList_Re*)pProtocol;
	int nErr = 0;
	if(p->retcode != 0)
	{
		nErr = p->retcode  - 300;
	}
	if(nErr != ERR_SUCCESS && nErr != 12)
	{
		CECGameUIMan* pGameUIMan = g_pGame->GetGameRun()->GetUIManager()->GetInGameUIMan();
		pGameUIMan->MessageBox(20011 + nErr );
	}
	else if( nErr == 12) // 实际枚举值为 312  , 没有队伍
	{
		//CECGameUIMan* pGameUI = (CECGameUIMan*)g_pGame->GetGameRun()->GetUIManager()->GetInGameUIMan();
		//DlgCrssvrTeamsGroupInfo* pDlg = (DlgCrssvrTeamsGroupInfo*)pGameUI->GetDialog("Win_Battle_Dungeon_6v6_GroupInfo");
	}
	else
	{
		CECGameUIMan* pGameUI = (CECGameUIMan*)g_pGame->GetGameRun()->GetUIManager()->GetInGameUIMan();
		DlgCrssvrTeamsGroupInfo* pDlg = (DlgCrssvrTeamsGroupInfo*)pGameUI->GetDialog("Win_Battle_Dungeon_6v6_GroupInfo");
		pDlg->UpDateGroupInfoList(p);
	}
}

void CECGameSession::OnPrtcCrssvrTeamsBaseRe(GNET::Protocol* pProtocol)		//  PROTOCOL_CRSSVRTEAMSLIST_RE
{
	GNET::CrssvrTeamsTeamBase_Re* p = (GNET::CrssvrTeamsTeamBase_Re*)pProtocol;
	int nErr = 0;
	if(p->retcode != 0)
	{
		nErr = p->retcode  - 300;
	}
	if(nErr != ERR_SUCCESS && nErr != 12)
	{
		CECGameUIMan* pGameUIMan = g_pGame->GetGameRun()->GetUIManager()->GetInGameUIMan();
		pGameUIMan->MessageBox(20011 + nErr );
	}
	else if( nErr == 12) // 实际枚举值为 312  , 没有队伍
	{
		//CECGameUIMan* pGameUI = (CECGameUIMan*)g_pGame->GetGameRun()->GetUIManager()->GetInGameUIMan();
		//DlgCrssvrTeamsGroupInfo* pDlg = (DlgCrssvrTeamsGroupInfo*)pGameUI->GetDialog("Win_Battle_Dungeon_6v6_GroupInfo");
	}
	else
	{
		CECGameUIMan* pGameUI = (CECGameUIMan*)g_pGame->GetGameRun()->GetUIManager()->GetInGameUIMan();
		DlgCrssvrTeamsGroupInfo* pDlg = (DlgCrssvrTeamsGroupInfo*)pGameUI->GetDialog("Win_Battle_Dungeon_6v6_GroupInfo");
		pDlg->UpDateGroupInfoBase(p);
		DlgCrssvrTeamsMain* pTeamsMainDlg = (DlgCrssvrTeamsMain*)pGameUI->GetDialog("Win_Battle_Dungeon_6v6_GroupMain");
		pTeamsMainDlg->UpData(p);
		DlgCrssvrTeamsAward_Stage *pDlgStage = (DlgCrssvrTeamsAward_Stage*)pGameUI->GetDialog("Win_Battle_Dungeon_6v6_GroupAward03");
		if(pDlgStage)
		{
			pDlgStage->SetTeamPos(p->team_pos);
		}
	}
}

void CECGameSession::OnPrtcCrssvrTeamsSearchRe(GNET::Protocol* pProtocol)
{
	GNET::CrssvrTeamsSearch_Re* p = (GNET::CrssvrTeamsSearch_Re*)pProtocol;
	CECGameRun* pGameRun = g_pGame->GetGameRun();
	if(p->retcode != 0)
	{
		CECUIManager* pUIMan = pGameRun->GetUIManager();
		CECGameUIMan* pGameUI = (CECGameUIMan*)g_pGame->GetGameRun()->GetUIManager()->GetInGameUIMan();
		pUIMan->GetInGameUIMan()->MessageBox("", pGameUI->GetStringFromTable(20009), MB_OK, A3DCOLORRGBA(255, 255, 255, 160));
		return;
	}
	CECGameUIMan* pGameUI = (CECGameUIMan*)pGameRun->GetUIManager()->GetInGameUIMan();
	DlgCrssvrTeamsGroupRank* pDlg = (DlgCrssvrTeamsGroupRank*)pGameUI->GetDialog("Win_Battle_Dungeon_6v6_GroupRank");
	pDlg->UpDateGroupSearchRank(p);
}

void CECGameSession::OnPrtcCrssvrTeamsRoleNotifyRe(GNET::Protocol* pProtocol)
{
	GNET::CrssvrTeamsRoleNotify_Re* p = (GNET::CrssvrTeamsRoleNotify_Re*)pProtocol;
	CECGameRun* pGameRun = g_pGame->GetGameRun();
	CECGameUIMan* pGameUI = (CECGameUIMan*)pGameRun->GetUIManager()->GetInGameUIMan();
	if(p->retcode == 0)
	{
		DlgCrssvrTeamsAward_Week *pDlgWeek = (DlgCrssvrTeamsAward_Week*)pGameUI->GetDialog("Win_Battle_Dungeon_6v6_GroupAward01");
		DlgCrssvrTeamsAward_Exchange *pDlgExchange = (DlgCrssvrTeamsAward_Exchange*)pGameUI->GetDialog("Win_Battle_Dungeon_6v6_GroupAward02");
		DlgCrssvrTeamsAward_Stage *pDlgStage = (DlgCrssvrTeamsAward_Stage*)pGameUI->GetDialog("Win_Battle_Dungeon_6v6_GroupAward03");
		//DlgCrssvrTeamsAward_Stage *pDlgStage = (DlgCrssvrTeamsAward_Stage*)pGameUI->GetDialog("Win_Battle_Dungeon_6v6_GroupAward03");
		if(pDlgWeek)
		{
			pDlgWeek->SetUpDate(p->ct_last_max_score);
		}
		if(pDlgExchange)
		{
			pDlgExchange->SetUpDate(p->ct_last_max_score);
		}
		if(pDlgStage)
		{
			pDlgStage->SetUpDate(p->ct_last_max_score);
		}
	}
}
