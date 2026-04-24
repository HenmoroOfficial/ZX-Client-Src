#include "AUI\\CSplit.h"
#include "AUI\\AUIListBox.h"
#include "AUI\\AUITextArea.h"
#include "AUI\\AUIStillImageButton.h"
#include "AUI\\AUILabel.h"
#include "DlgBattleDungeonApplyList.h"
#include "DlgHost.h"
#include "DlgBattleDungeonTower.h"
#include "DlgBattleFactionWarControl.h"
#include "EC_Game.h"
#include "EC_GameRun.h"
#include "EC_GameUIMan.h"
#include "EC_GameSession.h"
#include "EC_World.h"
#include "EC_GPDataType.h"
#include "battlegetfield_re.hpp"
#include "battlegetlist_re.hpp"
#include "EC_HostPlayer.h"
#include "EC_Inventory.h"
#include "EC_GameUIMisc.h"
#include "EC_BattleDungeon.h"
#include "EC_BattleDungeonTower.h"

#include "raidjoinreq.hpp"
#include "raidopen_re.hpp"
#include "raidjoin_re.hpp"
#include "raidjoinapplylist.hpp"
#include "raidenter_re.hpp"
#include "raidjoinaccept_re.hpp"
#include "raidstart_re.hpp"
#include "raidappoint_re.hpp"
#include "raidappointnotify.hpp"
#include "raidstartnotify.hpp"
#include "raidkick_re.hpp"
#include "raidkicknotify.hpp"
#include "raidquit_re.hpp"
#include "raidgetroom_re.hpp"

#define new A_DEBUG_NEW

AUI_BEGIN_COMMAND_MAP(CDlgBattleDungeonApplyList, CDlgBase)

AUI_ON_COMMAND("Btn_Out",		OnCommandKick)
AUI_ON_COMMAND("Btn_Move",		OnCommandChange)
AUI_ON_COMMAND("Btn_Start",		OnCommandOpen)
AUI_ON_COMMAND("Btn_Yes",		OnCommandAccept)
AUI_ON_COMMAND("Btn_No",		OnCommandRefuse)
AUI_ON_COMMAND("Btn_Leave",		OnCommandLeave)
AUI_ON_COMMAND("IDCANCEL",		OnCommandCancel)

AUI_END_COMMAND_MAP()

AUI_BEGIN_EVENT_MAP(CDlgBattleDungeonApplyList, CDlgBase)

AUI_ON_EVENT("Lst_1",			WM_RBUTTONUP,	OnEventRButtonUpLstIn)
AUI_ON_EVENT("Lst_2",			WM_RBUTTONUP,	OnEventRButtonUpLstOut)

AUI_END_EVENT_MAP()


CDlgBattleDungeonApplyList::CDlgBattleDungeonApplyList()
{
}

CDlgBattleDungeonApplyList::~CDlgBattleDungeonApplyList()
{
}

bool CDlgBattleDungeonApplyList::OnInitDialog()
{
	m_mapFuncs.clear();
	m_mapFuncs["join_req"]			= &CDlgBattleDungeonApplyList::OnJoinReq;
	m_mapFuncs["open_re"]			= &CDlgBattleDungeonApplyList::OnOpenRe;
	m_mapFuncs["join_re"]			= &CDlgBattleDungeonApplyList::OnJoinRe;
	m_mapFuncs["join_apply"]		= &CDlgBattleDungeonApplyList::OnJoinApply;
	m_mapFuncs["join_accept_success"]		= &CDlgBattleDungeonApplyList::OnAcceptRe;
	m_mapFuncs["enter_re"]			= &CDlgBattleDungeonApplyList::OnEnterRe;
	m_mapFuncs["start_re"]			= &CDlgBattleDungeonApplyList::OnStartRe;
	m_mapFuncs["appoint_re"]		= &CDlgBattleDungeonApplyList::OnAppointRe;
	m_mapFuncs["appoint_notify"]	= &CDlgBattleDungeonApplyList::OnAppointNotify;
	m_mapFuncs["start_notify"]		= &CDlgBattleDungeonApplyList::OnStartNotify;
	m_mapFuncs["kick_re"]			= &CDlgBattleDungeonApplyList::OnKickRe;
	m_mapFuncs["kick_notify"]		= &CDlgBattleDungeonApplyList::OnKickNotify;
	m_mapFuncs["refuse_enter"]		= &CDlgBattleDungeonApplyList::OnQuitRe;
	m_mapFuncs["room_info"]			= &CDlgBattleDungeonApplyList::OnGetRoomRe;

	m_pLstIn = (PAUILISTBOX)GetDlgItem("Lst_1");
	m_pLstOut = (PAUILISTBOX)GetDlgItem("Lst_2");
	m_pLabTimeLeftStart = (PAUILABEL)GetDlgItem("Txt_TimeLeft");
	m_pLabOwnerName = (PAUILABEL)GetDlgItem("Lab_OwnerName");
	m_pLabOwnerProf = (PAUILABEL)GetDlgItem("Lab_OwnerProf");
	m_pLabOwnerLv = (PAUILABEL)GetDlgItem("Lab_OwnerLv");
	m_pLabPlayerNum = (PAUILABEL)GetDlgItem("Txt_PlayerNum");

	m_pBtnKick = GetDlgItem("Btn_Out");
	m_pBtnChange = GetDlgItem("Btn_Move");
	m_pBtnAccept = GetDlgItem("Btn_Yes");
	m_pBtnRefuse = GetDlgItem("Btn_No");
	return true;
}

void CDlgBattleDungeonApplyList::OnShowDialog()
{
	if (CECBattleDungeonMan::getSingletonPtr()->GetInTeamBattle())
	{
		AUILabel* pLab = (PAUILABEL)GetDlgItem("Txt_Hint");
		ACString strText;
		strText.Format(_AL("%d"), CECBattleDungeonMan::getSingletonPtr()->GetInTeamBattle()->m_pConfigData->room_active_time/60);
		pLab->SetText(strText);
	}
}

void CDlgBattleDungeonApplyList::OnTick()
{	
	ACString strText;
	int nTime = m_iApplyEndTime - GetGame()->GetServerGMTTime();
	if( nTime > 0 )
	{
		m_pLabTimeLeftStart->SetText(GetTimeText(nTime));
	}
	else
		m_pLabTimeLeftStart->SetText(_AL(""));

	strText.Format(_AL("%d/%d"), m_pLstIn->GetCount()+1, CECBattleDungeonMan::getSingletonPtr()->GetInTeamBattle()->m_pConfigData->max_player_num);
	m_pLabPlayerNum->SetText(strText);
	
	bool bOwner = CECBattleDungeonMan::getSingletonPtr()->m_iLeaderId==GetHostPlayer()->GetCharacterID();
	GetDlgItem("Btn_Out")->Enable(bOwner);
	GetDlgItem("Btn_Move")->Enable(bOwner);
	GetDlgItem("Btn_Start")->Enable(bOwner);
	GetDlgItem("Btn_Yes")->Enable(bOwner);
	GetDlgItem("Btn_No")->Enable(bOwner);
	if (bOwner)
	{
		m_pBtnKick->Enable(m_pLstIn->GetCurSel()>=0&&m_pLstIn->GetCurSel()<m_pLstIn->GetCount());
		m_pBtnChange->Enable(m_pLstIn->GetCurSel()>=0&&m_pLstIn->GetCurSel()<m_pLstIn->GetCount());
		
		m_pBtnAccept->Enable(m_pLstOut->GetCurSel()>=0&&m_pLstOut->GetCurSel()<m_pLstOut->GetCount());
		m_pBtnRefuse->Enable(m_pLstOut->GetCurSel()>=0&&m_pLstOut->GetCurSel()<m_pLstOut->GetCount());
	}
}

void CDlgBattleDungeonApplyList::OnCommandKick(const char * szCommand)
{
	GetGameSession()->raid_kick(CECBattleDungeonMan::getSingletonPtr()->m_iRoomId, m_pLstIn->GetItemData(m_pLstIn->GetCurSel()), RAID_KICKOUT);
}

void CDlgBattleDungeonApplyList::OnCommandChange(const char * szCommand)
{
	GetGameSession()->raid_appoint(CECBattleDungeonMan::getSingletonPtr()->m_iRoomId, m_pLstIn->GetItemData(m_pLstIn->GetCurSel()));
}

void CDlgBattleDungeonApplyList::OnCommandOpen(const char * szCommand)
{
	if ((m_pLstIn->GetCount()+1)<CECBattleDungeonMan::getSingletonPtr()->GetInTeamBattle()->m_pConfigData->min_player_num)
	{
		CECBattleDungeonMan::getSingletonPtr()->ReportError(ERR_RAID_START_TEAMNUMBER);
		return;
	}
	GetGameSession()->raid_start(CECBattleDungeonMan::getSingletonPtr()->m_iRoomId, CECBattleDungeonMan::getSingletonPtr()->GetInTeamBattle()->m_iMapId);
}

void CDlgBattleDungeonApplyList::OnCommandAccept(const char * szCommand)
{
	GetGameSession()->raid_accept(CECBattleDungeonMan::getSingletonPtr()->m_iRoomId, CECBattleDungeonMan::getSingletonPtr()->GetInTeamBattle()->m_iMapId, m_pLstOut->GetItemData(m_pLstOut->GetCurSel()), 1);
}

void CDlgBattleDungeonApplyList::OnCommandRefuse(const char * szCommand)
{
	GetGameSession()->raid_accept(CECBattleDungeonMan::getSingletonPtr()->m_iRoomId, CECBattleDungeonMan::getSingletonPtr()->GetInTeamBattle()->m_iMapId, m_pLstOut->GetItemData(m_pLstOut->GetCurSel()), 0);
}

void CDlgBattleDungeonApplyList::OnCommandLeave(const char * szCommand)
{
	GetGameSession()->raid_quit(CECBattleDungeonMan::getSingletonPtr()->m_iRoomId);
}

void CDlgBattleDungeonApplyList::OnCommandCancel(const char * szCommand)
{
	int i;
	for (i=0; i<m_pLstOut->GetCount(); ++i)
	{
		if ((int)m_pLstOut->GetItemData(i)==GetHostPlayer()->GetCharacterID())
		{
			GetGameUIMan()->MessageBox(12261);
			return;
		}
	}
	Show(false);
}

void CDlgBattleDungeonApplyList::OnEventRButtonUpLstIn(WPARAM wParam, LPARAM lParam, AUIObject * pObj)
{
	if (m_pLstIn->GetCurSel()>=0 && m_pLstIn->GetCurSel()<m_pLstIn->GetCount())
	{
		int x = GET_X_LPARAM(lParam);
		int y = GET_Y_LPARAM(lParam);
		GetGameUIMan()->PopupPlayerContextMenu(m_pLstIn->GetItemData(m_pLstIn->GetCurSel()), x, y);	
	}
}

void CDlgBattleDungeonApplyList::OnEventRButtonUpLstOut(WPARAM wParam, LPARAM lParam, AUIObject * pObj)
{
	if (m_pLstOut->GetCurSel()>=0 && m_pLstOut->GetCurSel()<m_pLstOut->GetCount())
	{
		int x = GET_X_LPARAM(lParam);
		int y = GET_Y_LPARAM(lParam);
		GetGameUIMan()->PopupPlayerContextMenu(m_pLstOut->GetItemData(m_pLstOut->GetCurSel()), x, y);
	}
}

int CDlgBattleDungeonApplyList::CommonMessage(AString strMsg, WPARAM wParam, LPARAM lParam)
{
/*	int retcod = *((int*)wParam);
	if (retcod!=0)
	{
		GetGameUIMan()->MessageBox(11200 + retcod);
		return;
	}
*/
	if (m_mapFuncs.find(strMsg)!=m_mapFuncs.end())
	{
		(this->*m_mapFuncs[strMsg])((void*)wParam);
	}

	return 0;
}

void CDlgBattleDungeonApplyList::OnJoinReq(void* pData)
{
	using namespace GNET;
	RaidJoinReq* p = (RaidJoinReq*)pData;
	if (p->raidroom_id!=CECBattleDungeonMan::getSingletonPtr()->m_iRoomId)
	{
		return;
	}
	GetGameSession()->raid_getroom(CECBattleDungeonMan::getSingletonPtr()->m_iRoomId);
}

void CDlgBattleDungeonApplyList::OnOpenRe(void* pData)		
{	
	using namespace GNET;
	RaidOpen_Re* p = (RaidOpen_Re*)pData;
	if (p->retcode!=0)
	{
		GetGameUIMan()->MessageBox(11200 + p->retcode);
		return;
	}
	CECBattleDungeonMan::getSingletonPtr()->m_iRoomId = p->raidroom_id;
	if (p->map_id!=GetHostPlayer()->GetBattleDungeonTower()->GetMapId())
	{
		CECBattleDungeonMan::getSingletonPtr()->JoinBattle(p->map_id);
		GRaidRoomInfo info;
		info.raidroom_id = p->raidroom_id;
		info.map_id = CECBattleDungeonMan::getSingletonPtr()->GetInTeamBattle()->m_iMapId;
		info.player_number = 1;
		ACString strName = GetHostPlayer()->GetName();
		info.leadername = Octets(strName.GetBuffer(0), strName.GetLength()*2);
		info.leaderid = GetHostPlayer()->GetCharacterID();
		info.roomname = p->roomname;
		info.apply_endtime = p->apply_endtime;
		info.difficulty = p->difficulty;
		CECBattleDungeonMan::getSingletonPtr()->GetInTeamBattle()->m_vecRoomInfo.push_back(info);
	}

	GetGameSession()->raid_getroom(CECBattleDungeonMan::getSingletonPtr()->m_iRoomId);
}

void CDlgBattleDungeonApplyList::OnJoinRe(void* pData)		
{
	using namespace GNET;
	RaidJoin_Re* p = (RaidJoin_Re*)pData;
	//帮战错误处理
	{
		
		if(p->map_id == CDlgBattleFactionWarControl::GetFactionPKRaidMapId())
		{
		   if(p->retcode != 0)
		   CECBattleDungeonMan::getSingletonPtr()->ReportFactionPKError(p->retcode);
		   return;
		}
	}
	CECBattleDungeonMan::getSingletonPtr()->m_iRoomId = p->raidroom_id;
	if (p->retcode==0)
	{
		CECBattleDungeonMan::getSingletonPtr()->JoinBattle(p->map_id);
		int iTime = 0;	
		for (UINT i=0; i<CECBattleDungeonMan::getSingletonPtr()->GetInTeamBattle()->m_vecRoomInfo.size(); ++i)
		{
			if (CECBattleDungeonMan::getSingletonPtr()->GetInTeamBattle()->m_vecRoomInfo[i].raidroom_id==p->raidroom_id)
			{
				iTime = CECBattleDungeonMan::getSingletonPtr()->GetInTeamBattle()->m_vecRoomInfo[i].apply_endtime;
				break;
			}
		}
		GetGameSession()->raid_getroom(CECBattleDungeonMan::getSingletonPtr()->m_iRoomId);
	}
	else if (p->retcode==14)	// 申请者遭拒绝
	{
		OnBattleQuitClear();
	}
	else
	{
		GetGameUIMan()->MessageBox(11200 + p->retcode);
		return;
	}	
}

void CDlgBattleDungeonApplyList::OnJoinApply(void* pData)
{
	using namespace GNET;
	RaidJoinApplyList* p = (RaidJoinApplyList*)pData;
	
	if (p->retcode!=0)
	{
		GetGameUIMan()->MessageBox(11200 + p->retcode);
		return;
	}	
	CECBattleDungeonMan::getSingletonPtr()->m_iRoomId = p->raidroom_id;
	GetGameSession()->raid_getroom(CECBattleDungeonMan::getSingletonPtr()->m_iRoomId);
	
}

void CDlgBattleDungeonApplyList::OnAcceptRe(void* pData)	
{
	using namespace GNET;
	RaidJoinAccept_Re* p = (RaidJoinAccept_Re*)pData;
	
	if (p->retcode!=0)
	{
		GetGameUIMan()->MessageBox(11200 + p->retcode);
		return;
	}
	
	// p->accept	原样发过来
	// 可修改为不从服务器取
	GetGameSession()->raid_getroom(CECBattleDungeonMan::getSingletonPtr()->m_iRoomId);
}

void CDlgBattleDungeonApplyList::OnEnterRe(void* pData)	
{
	using namespace GNET;
	RaidEnter_Re* p = (RaidEnter_Re*)pData;
	if (p->retcode!=0)
	{
		GetGameUIMan()->MessageBox(11200 + p->retcode);
		return;
	}	
}

void CDlgBattleDungeonApplyList::OnStartRe(void* pData)	
{
	using namespace GNET;
	RaidStart_Re* p = (RaidStart_Re*)pData;
	if (p->retcode!=0)
	{
		GetGameUIMan()->MessageBox(11200 + p->retcode);
		return;
	}	
}

void CDlgBattleDungeonApplyList::OnAppointRe(void* pData)
{
	using namespace GNET;
	RaidAppoint_Re* p = (RaidAppoint_Re*)pData;
	if (p->retcode!=0)
	{
		GetGameUIMan()->MessageBox(11200 + p->retcode);
		return;
	}
	
	ACString strText;
	strText.Format(_AL("%s\t%s\t%s"), m_pLabOwnerName->GetText(), 
		m_pLabOwnerProf->GetText(), m_pLabOwnerLv->GetText());
	m_pLstIn->AddString(strText);
	m_pLstIn->SetItemData(m_pLstIn->GetCount()-1, CECBattleDungeonMan::getSingletonPtr()->m_iLeaderId);
	
	int i;
	for (i=0; i<m_pLstIn->GetCount(); ++i)
	{
		if ((int)m_pLstIn->GetItemData(i)==p->target)
		{
			CSplit s(m_pLstIn->GetText(i));
			CSplit::VectorAWString vec = s.Split(_AL("\t"));
			if (vec.size()==3)
			{
				m_pLabOwnerName->SetText(vec[0]);
				m_pLabOwnerProf->SetText(vec[1]);
				m_pLabOwnerLv->SetText(vec[2]);
			}
			m_pLstIn->DeleteString(i);
			break;
		}
	}
	CECBattleDungeonMan::getSingletonPtr()->m_iLeaderId = p->target;
	
	for (i=0; i<(int)CECBattleDungeonMan::getSingletonPtr()->GetInTeamBattle()->m_vecRoomInfo.size(); ++i)
	{
		if (CECBattleDungeonMan::getSingletonPtr()->GetInTeamBattle()->m_vecRoomInfo[i].raidroom_id==p->raidroom_id)
		{
			CECBattleDungeonMan::getSingletonPtr()->GetInTeamBattle()->m_vecRoomInfo[i].leaderid = CECBattleDungeonMan::getSingletonPtr()->m_iLeaderId;
			break;
		}
	}
}

void CDlgBattleDungeonApplyList::OnAppointNotify(void* pData)
{
	using namespace GNET;
	RaidAppointNotify* p = (RaidAppointNotify*)pData;

	ACString strText;
	strText.Format(_AL("%s\t%s\t%s"), m_pLabOwnerName->GetText(), 
			m_pLabOwnerProf->GetText(), m_pLabOwnerLv->GetText());
	m_pLstIn->AddString(strText);
	m_pLstIn->SetItemData(m_pLstIn->GetCount()-1, CECBattleDungeonMan::getSingletonPtr()->m_iLeaderId);

	int i;
	for (i=0; i<m_pLstIn->GetCount(); ++i)
	{
		if ((int)m_pLstIn->GetItemData(i)==p->new_master_id)
		{
			CSplit s(m_pLstIn->GetText(i));
			CSplit::VectorAWString vec = s.Split(_AL("\t"));
			if (vec.size()==3)
			{
				m_pLabOwnerName->SetText(vec[0]);
				m_pLabOwnerProf->SetText(vec[1]);
				m_pLabOwnerLv->SetText(vec[2]);
			}
			m_pLstIn->DeleteString(i);
			break;
		}
	}
	CECBattleDungeonMan::getSingletonPtr()->m_iLeaderId = p->new_master_id;
	
	for (i=0; i<(int)CECBattleDungeonMan::getSingletonPtr()->GetInTeamBattle()->m_vecRoomInfo.size(); ++i)
	{
		if (CECBattleDungeonMan::getSingletonPtr()->GetInTeamBattle()->m_vecRoomInfo[i].raidroom_id==p->raidroom_id)
		{
			CECBattleDungeonMan::getSingletonPtr()->GetInTeamBattle()->m_vecRoomInfo[i].leaderid = CECBattleDungeonMan::getSingletonPtr()->m_iLeaderId;
			break;
		}
	}
}

void CDlgBattleDungeonApplyList::OnStartNotify(void* pData)
{
	using namespace GNET;
	RaidStartNotify* p = (RaidStartNotify*)pData;

	ACString strText;
	strText.Format(GetGameUIMan()->GetStringFromTable(914), GetGameUIMan()->GetStringFromTable(1900 + p->map_id));
	GetGameRun()->SaveConfigsToServer();
	GetGameUIMan()->CommonMessage("Win_Message9", AC2AS(strText), 20, 0);
	PAUIDIALOG pMsgBox = GetGameUIMan()->GetDialog("Win_Message9");
	pMsgBox->SetData(p->raidroom_id);
	pMsgBox->SetDataPtr((void*)(p->map_id));
	OnBattleQuitClear();	
}

void CDlgBattleDungeonApplyList::OnKickRe(void* pData)
{
	using namespace GNET;
	RaidKick_Re* p = (RaidKick_Re*)pData;
	if (p->retcode!=0)
	{
		GetGameUIMan()->MessageBox(11200 + p->retcode);
		return;
	}

	for (int i=0; i<m_pLstIn->GetCount(); ++i)
	{
		if ((int)m_pLstIn->GetItemData(i)==p->target)
		{
			m_pLstIn->DeleteString(i);
			break;
		}
	}
}

void CDlgBattleDungeonApplyList::OnKickNotify(void* pData)
{
	using namespace GNET;
	RaidKickNotify* p = (RaidKickNotify*)pData;

	GetGameUIMan()->MessageBox(12250+p->reason);
	OnBattleQuitClear();
}

void CDlgBattleDungeonApplyList::OnQuitRe(void* pData)
{
	using namespace GNET;
	RaidQuit_Re* p = (RaidQuit_Re*)pData;
	
	OnBattleQuitClear();
	
	if (p->retcode!=0)
	{
		GetGameUIMan()->MessageBox(11200 + p->retcode);
	}
	else
	{
		GetGameUIMan()->GetDialog("Win_Battle_Dungeon_6v6_Wait")->Show(false);
		GetGameUIMan()->GetDialog("Win_Battle_Dungeon_6v6_MapSelect")->GetDlgItem("Txt_Wait")->Show(false);
		GetGameUIMan()->GetDialog("Win_Battle_Dungeon_6v6_MapSelect")->GetDlgItem("Btn_quit")->Show(false);
	}
}

void CDlgBattleDungeonApplyList::OnGetRoomRe(void* pData)
{
	using namespace GNET;
	RaidGetRoom_Re* p = (RaidGetRoom_Re*)pData;
	
	if (p->retcode!=0)
	{
		GetGameUIMan()->MessageBox(11200 + p->retcode);
		return;
	}

	if (GetHostPlayer()->GetBattleDungeonTower()->GetMapId()==p->map_id)
	{
		PAUILISTBOX pLst = (PAUILISTBOX)GetGameUIMan()->GetDialog("Win_Battle_Dungeon_Tower")->GetDlgItem("Lst_Info");
		if (pLst->GetCurSel()<0)
		{
			pLst->SetCurSel(0);
		}
		int lv = pLst->GetCurSel()*GetHostPlayer()->GetBattleDungeonTower()->GetConfig()->tower_pass_count;
		GetGameSession()->raid_start_tower(p->raidroom_id, p->map_id, lv);
		return;
	}

	CECBattleDungeonMan::getSingletonPtr()->JoinBattle(p->map_id);

	UINT i;
	GRaidRoomInfo info;
	for (i=0; i<CECBattleDungeonMan::getSingletonPtr()->GetInTeamBattle()->m_vecRoomInfo.size(); ++i)
	{
		if (CECBattleDungeonMan::getSingletonPtr()->GetInTeamBattle()->m_vecRoomInfo[i].raidroom_id==p->raidroom_id)
		{
			info = CECBattleDungeonMan::getSingletonPtr()->GetInTeamBattle()->m_vecRoomInfo[i];
			break;
		}
	}
	if (i==CECBattleDungeonMan::getSingletonPtr()->GetInTeamBattle()->m_vecRoomInfo.size())
	{
		return;
	}

	PAUILABEL pLabTitle = (PAUILABEL)GetDlgItem("Title");
	ACString strTitle(CECBattleDungeonMan::getSingletonPtr()->GetInTeamBattle()->m_pConfigData->name);
	if (info.difficulty>0)
		pLabTitle->SetText(strTitle + _AL("-") + GetGameUIMan()->GetStringFromTable(13141+info.difficulty));
	else
		pLabTitle->SetText(strTitle);
	PAUILABEL pLabRoomName = (PAUILABEL)GetDlgItem("Tiitle2");
	pLabRoomName->SetText(ACString((ACHAR*)info.roomname.begin(), info.roomname.size()/sizeof(ACHAR)));
	m_iApplyEndTime = info.apply_endtime;
	CECBattleDungeonMan::getSingletonPtr()->m_iLeaderId = info.leaderid;

	ACString strText;
	m_pLstIn->ResetContent();
	for (i=0; i<p->group1_player.size(); ++i)
	{
		GBattleRole& role = p->group1_player[i];
		ACString strName((ACHAR*)role.name.begin(), role.name.size()/sizeof(ACHAR));
		GetGameRun()->AddPlayerName(role.roleid, strName);
		if (info.leaderid==role.roleid)
		{
			m_pLabOwnerName->SetText(strName);
			ACString strText = GetStringFromTable(7000 + role.cultivation);
			m_pLabOwnerProf->SetText(strText+GetGameUIMan()->GetStringFromTable(PROFESSION_START + role.occupation));
			strText.Format(_AL("%d"), role.level);
			m_pLabOwnerLv->SetText(strText);
		}
		else
		{
			strText.Format(_AL("%s\t%s%s\t%d"), strName, GetStringFromTable(7000 + role.cultivation),
				GetGameUIMan()->GetStringFromTable(PROFESSION_START + role.occupation), role.level);
			m_pLstIn->AddString(strText);
			m_pLstIn->SetItemData(m_pLstIn->GetCount()-1, role.roleid);
		}
	}

	m_pLstOut->ResetContent();
	for (i=0; i<p->group1_applyer.size(); ++i)
	{
		GBattleRole& role = p->group1_applyer[i];
		strText.Format(_AL("%s\t%s%s\t%d"), ACString((ACHAR*)role.name.begin(), role.name.size()/sizeof(ACHAR)), 
			GetStringFromTable(7000 + role.cultivation),
			GetGameUIMan()->GetStringFromTable(PROFESSION_START + role.occupation), role.level);
		m_pLstOut->AddString(strText);
		m_pLstOut->SetItemData(i, role.roleid);
	}

	CECBattleDungeonMan::getSingletonPtr()->m_iRoomId = p->raidroom_id;
	if (!IsShow())
	{
		Show(true);
	}
}

void CDlgBattleDungeonApplyList::OnBattleQuitClear()
{
	m_iApplyEndTime = 0;
	CECBattleDungeonMan::getSingletonPtr()->JoinBattle(-1);
	if (IsShow())
	{
		Show(false);
	}
}