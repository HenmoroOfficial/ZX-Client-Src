// File		: CDlgBattleInstance.cpp
// Creator	: Fuchonggang
// Date		: 2008/12/12

#include "AUI\\AUIListBox.h"
#include "AUI\\AUIRadioButton.h"
#include "AUI\\AUIComboBox.h"
#include "DlgBattleInstance.h"
#include "DlgHost.h"
#include "EC_Game.h"
#include "EC_GameRun.h"
#include "EC_GameUIMan.h"
#include "EC_GameSession.h"
#include "EC_World.h"
#include "EC_GPDataType.h"
#include "EC_Inventory.h"
#include "EC_Global.h"
#include "luastate.h"
#include "instancinggetlist_re.hpp"
#include "instancingavailablelist_re.hpp"
#include "instancingenter_re.hpp"
#include "instancingappoint_re.hpp"
#include "instancingkick_re.hpp"
#include "instancingkicknotify.hpp"
#include "instancingstart_re.hpp"
#include "instancingappointnotify.hpp"
#include "instancingjoin_re.hpp"
#include "instancingjoinreq.hpp"
#include "instancingleave_re.hpp"
#include "InstancingCloseNotify.hpp"
#include "instancingstartnotify.hpp"
#include "EC_HostPlayer.h"
#include "elementdataman.h"

#define new A_DEBUG_NEW

AUI_BEGIN_COMMAND_MAP(CDlgBattleInstance, CDlgBase)

AUI_ON_COMMAND("active",		OnCommandActive)
AUI_ON_COMMAND("join",			OnCommandJoin)
AUI_ON_COMMAND("start",			OnCommandStart)
AUI_ON_COMMAND("leave",			OnCommandLeave)
AUI_ON_COMMAND("kick",			OnCommandKick)
AUI_ON_COMMAND("change",		OnCommandChange)
AUI_ON_COMMAND("accept",		OnCommandAccept)
AUI_ON_COMMAND("refuse",		OnCommandRefuse)
AUI_ON_COMMAND("jointype",		OnCommandJoinType)
AUI_ON_COMMAND("Chk_MiniHint",	OnCommandMiniHint)
AUI_ON_COMMAND("Btn_Close",		OnCommandCancel)
AUI_ON_COMMAND("IDCANCEL",		OnCommandCancel)

AUI_END_COMMAND_MAP()

AUI_BEGIN_EVENT_MAP(CDlgBattleInstance, CDlgBase)

AUI_ON_EVENT("List_BattleField",	WM_LBUTTONDOWN,	OnEventLButtonDownBattleField)
AUI_ON_EVENT("Lst_Stage",			WM_LBUTTONDOWN,	OnEventLButtonDownBattleState)

AUI_END_EVENT_MAP()

abase::hash_map<int, int>	CDlgBattleInstance::m_mapMap2Tid;
int			CDlgBattleInstance::m_nJoinTID = 0;
int			CDlgBattleInstance::m_nTID = 0;
int			CDlgBattleInstance::m_iScore=0;
int			CDlgBattleInstance::m_iCurStage=0;
int			CDlgBattleInstance::m_iStateTime=0;
CLuaState*	CDlgBattleInstance::m_pState = NULL;

#define  INSTANCING_DESC_TBL_NAME    "InstancingBattleDesc"
const char szBattleInstancingFile[] = "Interfaces\\script\\InstancingBattleDesc.lua";

CDlgBattleInstance::CDlgBattleInstance()
{
}

CDlgBattleInstance::~CDlgBattleInstance()
{
}

bool CDlgBattleInstance::OnInitDialog()
{
	if( stricmp(m_szName, "Win_BattleRaidStage") == 0 )
	{
		m_pState = g_LuaStateMan.GetAIState();
		if (!m_pState)
		{
			a_LogOutput(1, "CDlgBattleInstance::OnInitDialog, null lua state\n");
			return false;
		}

		CLuaScript * pScript = m_pState->RegisterFile(szBattleInstancingFile);
		if (!pScript)
		{
			assert(0);
			a_LogOutput(1, "CDlgBattleInstance::OnInitDialog, register file %s error\n", szBattleInstancingFile);
			return false;
		}
		m_mapMap2Tid.clear();
	}
	return true; 
}

bool CDlgBattleInstance::Release()
{
	if( stricmp(m_szName, "Win_BattleRaidStage") == 0 )
	{
		if (m_pState)
		{
			m_pState->ReleaseTbl(INSTANCING_DESC_TBL_NAME);
			m_pState->UnRegister(szBattleInstancingFile);
		}
	}
	
	return CDlgBase::Release();
}

void CDlgBattleInstance::OnShowDialog()
{
	if( stricmp(m_szName, "Win_BattleRaid") == 0 )
	{
	}
	else if( stricmp(m_szName, "Win_BattleRaidApliList") == 0 )
	{
	}
	else if( stricmp(m_szName, "Win_BattleRaidStage") == 0 )
	{
	}
	else if( stricmp(m_szName, "Win_BattleRaidMiniHint") == 0 )
	{
	}
}

void CDlgBattleInstance::OnTick()
{
	if( stricmp(m_szName, "Win_BattleRaid") == 0 )
	{
		RefreshBattleList(true);
		PAUILISTBOX pList = (PAUILISTBOX)GetDlgItem("List_Member");
		int nSel = pList->GetCurSel();
		if( nSel >= 0 && nSel < pList->GetCount() )
		{
			GetDlgItem("Btn_Activiate")->Enable(pList->GetItemData(nSel,3)==BATTLEINSTANCING_STATE_UNACTIVE && GetGameRun()->GetCurLine()==(int)pList->GetItemData(nSel,0));
			GetDlgItem("Btn_Apply")->Enable(pList->GetItemData(nSel,3)==BATTLEINSTANCING_STATE_ACTIVE	&& 
											GetGameRun()->GetCurLine()==(int)pList->GetItemData(nSel,0)	&&
											pList->GetItemData(nSel,4)==0);
		}
	}
	else if( stricmp(m_szName, "Win_BattleRaidApliList") == 0 )
	{
		if (m_mapFieldsGot.find(m_nJoinTID) != m_mapFieldsGot.end() && !m_mapFieldsGot[m_nJoinTID])
		{
			GetGameSession()->instancing_getfield(GetGameRun()->GetCurLine(), GetData());
		}

		ACString strText;
		int nTime = (GetGameUIMan()->m_pDlgHost->GetInstanceTime() - GetGame()->GetServerGMTTime() );
		nTime = nTime > 0 ? nTime : 0;
		strText.Format(_AL("^ffcb4a%d^ffffff        ^ffcb4a%d^ffffff"), nTime/60, nTime%60);
		GetDlgItem("Txt_TimeLeftNum")->SetText(strText);
	}
	else if( stricmp(m_szName, "Win_BattleRaidStage") == 0 )
	{
		ACString strText;
		int nTime = (GetGameUIMan()->m_pDlgHost->GetInstanceTime() - GetGame()->GetServerGMTTime() );
		nTime = nTime > 0 ? nTime : 0;
		strText.Format(_AL("^ffcb4a%d^ffffff        ^ffcb4a%d^ffffff"), nTime/60, nTime%60);
		GetDlgItem("Txt_TimeLeftNum")->SetText(strText);

		PAUILISTBOX pLst = (PAUILISTBOX)GetDlgItem("Lst_Stage");
		pLst->SetCurSel(m_iCurStage);
	}
	else if( stricmp(m_szName, "Win_BattleRaidMiniHint") == 0 )
	{
		const INSTANCING_BATTLE_CONFIG *pInstance = GetInstanceConfig(m_nJoinTID);
		ACString strText;
		int nTime = (GetGameUIMan()->m_pDlgHost->GetInstanceTime() - GetGame()->GetServerGMTTime() );
		
		abase::vector<CScriptValue> args, results;
		args.push_back((double)m_nJoinTID);
		args.push_back((double)(m_iCurStage+1));
		m_pState->Call(INSTANCING_DESC_TBL_NAME, "GetBattleLevelDesc", args, results);
		ACString strDesc = _AL("");
		if (results.size() == 1 && results[0].m_Type == CScriptValue::SVT_STRING)
		{
			results[0].RetrieveAWString(strDesc);
		}

		PAUILABEL pLabScore = (PAUILABEL)GetDlgItem("Txt_List");
		strText.Format(GetGameUIMan()->GetStringFromTable(8586),
			pInstance->tollgate[m_iCurStage].tollgate_name,
			m_iStateTime/60,m_iStateTime%60,
			nTime/60, nTime%60,
			m_iScore,
			strDesc);
		pLabScore->SetText(strText);
	}
}

void CDlgBattleInstance::OnCommandActive(const char * szCommand)
{
	PAUILISTBOX pList = (PAUILISTBOX)GetDlgItem("List_Member");
	int nSel = pList->GetCurSel();
	if( nSel<0 || nSel >= pList->GetCount() )
		return;
	
	int map_id = pList->GetItemData(nSel, 1);
	m_nJoinTID = m_nTID;
	const INSTANCING_BATTLE_CONFIG *pInstance = GetInstanceConfig(m_nJoinTID);
	if( pInstance )
	{
		PAUIDIALOG pDlg = GetGameUIMan()->GetDialog("Win_BattleRaidApliList");
		ACString strItem;
		if( GetHostPlayer()->GetBasicProps().iLevel < pInstance->level_lower_limit || 
			pInstance->level_upper_limit > 0 && GetHostPlayer()->GetBasicProps().iLevel > pInstance->level_upper_limit )
		{
			GetGameUIMan()->MessageBox("", GetStringFromTable(918), MB_OK, A3DCOLORRGBA(255, 255, 255, 160));
			return;
		}
		if( pInstance->renascence_count>=0 && (int)(GetHostPlayer()->GetRebornInfo().size()) != pInstance->renascence_count )
		{
			GetGameUIMan()->MessageBox("", GetStringFromTable(8570), MB_OK, A3DCOLORRGBA(255, 255, 255, 160));
			return;
		}
		if( pInstance->active_fee>0 && GetHostPlayer()->GetMoneyAmount() < pInstance->active_fee )
		{
			GetGameUIMan()->MessageBox("", GetStringFromTable(7191), MB_OK, A3DCOLORRGBA(255, 255, 255, 160));
			return;
		}
		if( pInstance->active_item>0 && GetHostPlayer()->GetPack()->GetItemTotalNum(pInstance->active_item) <=0 )
		{
			GetGameUIMan()->MessageBox("", GetStringFromTable(8484), MB_OK, A3DCOLORRGBA(255, 255, 255, 160));
			return;
		}
		GetGameSession()->c2s_CmdJoinInstancing(map_id);
		pDlg->SetData(map_id);
	}
}

void CDlgBattleInstance::OnCommandJoin(const char * szCommand)
{
	PAUILISTBOX pList = (PAUILISTBOX)GetDlgItem("List_Member");
	int nSel = pList->GetCurSel();
	if( nSel<0 || nSel >= pList->GetCount() )
		return;
	
	int map_id = pList->GetItemData(nSel, 1);
	int member = pList->GetItemData(nSel, 2);
	m_nJoinTID = m_nTID;
	const INSTANCING_BATTLE_CONFIG *pInstance = GetInstanceConfig(m_nJoinTID);
	if( pInstance )
	{
		PAUIDIALOG pDlg = GetGameUIMan()->GetDialog("Win_BattleRaidApliList");
		ACString strItem;
		if( GetHostPlayer()->GetBasicProps().iLevel < pInstance->level_lower_limit || 
			pInstance->level_upper_limit > 0 && GetHostPlayer()->GetBasicProps().iLevel > pInstance->level_upper_limit )
		{
			GetGameUIMan()->MessageBox("", GetStringFromTable(918), MB_OK, A3DCOLORRGBA(255, 255, 255, 160));
			return;
		}
		if( pInstance->renascence_count>=0 && (int)(GetHostPlayer()->GetRebornInfo().size()) != pInstance->renascence_count )
		{
			GetGameUIMan()->MessageBox("", GetStringFromTable(8570), MB_OK, A3DCOLORRGBA(255, 255, 255, 160));
			return;
		}
		if( member >= pInstance->max_player )
		{
			GetGameUIMan()->MessageBox("", GetStringFromTable(8477), MB_OK, A3DCOLORRGBA(255, 255, 255, 160));
			return;
		}
		GetGameSession()->c2s_CmdJoinInstancing(map_id);
		GetGameUIMan()->AddChatMessage(GetStringFromTable(8488), GP_CHAT_MISC);
		pDlg->SetData(map_id);
	}
}

void CDlgBattleInstance::OnCommandStart(const char * szCommand)
{
	GetGameSession()->instancing_start(GetData());
}

void CDlgBattleInstance::OnCommandLeave(const char * szCommand)
{
	GetGameSession()->instancing_leave(GetData());
}

void CDlgBattleInstance::OnCommandKick(const char * szCommand)
{
	PAUILISTBOX pLstMember = (PAUILISTBOX)GetDlgItem("Lst_Member1");
	int nSel = pLstMember->GetCurSel();
	if (nSel<0 || nSel>=pLstMember->GetCount())
	{
		return;
	}
	GetGameSession()->instancing_kick(GetData(), pLstMember->GetItemData(nSel));
}

void CDlgBattleInstance::OnCommandChange(const char * szCommand)
{
	PAUILISTBOX pLstMember = (PAUILISTBOX)GetDlgItem("Lst_Member1");
	int nSel = pLstMember->GetCurSel();
	if (nSel<0 || nSel>=pLstMember->GetCount())
	{
		return;
	}
	GetGameSession()->instancing_appoint(GetData(), pLstMember->GetItemData(nSel));
}

void CDlgBattleInstance::OnCommandAccept(const char * szCommand)
{
	PAUILISTBOX pLstApplyer = (PAUILISTBOX)GetDlgItem("Lst_Member2");
	int nSel = pLstApplyer->GetCurSel();
	if (nSel<0 || nSel>=pLstApplyer->GetCount())
	{
		return;
	}
	GetGameSession()->instancing_accept(1, GetData(), pLstApplyer->GetItemData(nSel));
}

void CDlgBattleInstance::OnCommandRefuse(const char * szCommand)
{
	PAUILISTBOX pLstApplyer = (PAUILISTBOX)GetDlgItem("Lst_Member2");
	int nSel = pLstApplyer->GetCurSel();
	if (nSel<0 || nSel>=pLstApplyer->GetCount())
	{
		return;
	}
	GetGameSession()->instancing_accept(0, GetData(), pLstApplyer->GetItemData(nSel));
}

void CDlgBattleInstance::OnCommandJoinType(const char * szCommand)
{
}

void CDlgBattleInstance::OnCommandMiniHint(const char * szCommand)
{
	PAUIDIALOG pDlg = GetGameUIMan()->GetDialog("Win_BattleRaidMiniHint");
	pDlg->Show(!pDlg->IsShow());
}

void CDlgBattleInstance::OnCommandCancel(const char * szCommand)
{
	GetGameUIMan()->EndNPCService();
	Show(false);
	if (stricmp(m_szName, "Win_BattleRaidMiniHint")==0)
	{
		(PAUICHECKBOX(GetGameUIMan()->GetDialog("Win_BattleRaidStage")->GetDlgItem("Chk_MiniHint")))->Check(false);
	}
}

void CDlgBattleInstance::OnEventLButtonDownBattleField(WPARAM wParam, LPARAM lParam, AUIObject * pObj)
{
	PAUILISTBOX pList = (PAUILISTBOX)GetDlgItem("List_BattleField");
	int nSel = pList->GetCurSel();
	PAUILABEL pLabName = (PAUILABEL)GetDlgItem("Lab_Choice");
	pLabName->SetText(pList->GetText(nSel));
	
	if( nSel >= 0 && nSel < pList->GetCount() )
	{
		m_nTID = pList->GetItemData(nSel);
		if (m_mapFieldsGot.find(m_nTID) != m_mapFieldsGot.end())
		{
			RefreshBattleList();
		}
		else
		{
			GetGameSession()->instancing_getlist(m_nTID);
		}
	}
}

void CDlgBattleInstance::OnEventLButtonDownBattleState(WPARAM wParam, LPARAM lParam, AUIObject * pObj)
{
	
}

void CDlgBattleInstance::UpdateBattleList(void *pData)
{
	InstancingAvailableList_Re *p = (InstancingAvailableList_Re*)pData;
	if (p->retcode !=0)
	{
		GetGameUIMan()->MessageBox("", GetGameUIMan()->GetStringFromTable(8400+p->retcode), MB_OK, A3DCOLORRGBA(255, 255, 255, 160));
		return;
	}
	ACString strText;
	int i;
	PAUILISTBOX pList = (PAUILISTBOX)GetDlgItem("List_Member");
	pList->ResetContent();
	pList = (PAUILISTBOX)GetDlgItem("List_BattleField");
	pList->ResetContent();
	m_vecFieldList.clear();
	m_mapFieldsGot.clear();
	m_mapFieldsGot.clear();
	int k=0;
	m_mapMap2Tid.clear();
	for(i = 0; i < (int)p->fields.size(); i++ )
	{
		const INSTANCING_BATTLE_CONFIG *pInstance = GetInstanceConfig(p->fields[i]);
		if (pInstance)
		{
			pList->AddString(pInstance->name);
			pList->SetItemData(k,p->fields[i]);
			m_mapMap2Tid[pInstance->map_id] = p->fields[i];
			k++;
		}
	}
	OnEventLButtonDownBattleField(0,0,NULL);
	
	Show(true);
}

void CDlgBattleInstance::UpdateBattleListInfo(void *pData)
{
	InstancingGetList_Re *p = (InstancingGetList_Re*)pData;
	if (p->retcode!=0)
	{
		GetGameUIMan()->MessageBox("", GetGameUIMan()->GetStringFromTable(8400+p->retcode), MB_OK, A3DCOLORRGBA(255, 255, 255, 160));
		return;
	}
	int i;
	LOCAL_GFieldInfo FieldInfo;
	m_nTID = FieldInfo.tid = p->battle_tid;
	m_mapFieldsGot[m_nTID] = true;
	m_mapEnterTime[m_nTID] = p->enter_count;
	for(i = 0; i < (int)p->fields.size(); i++ )
	{
		FieldInfo.info = p->fields[i];
		FieldInfo.info.time_left = FieldInfo.info.time_left * 1000 + GetTickCount();
		m_vecFieldList.push_back(FieldInfo);
	}

	RefreshBattleList();	
}

void CDlgBattleInstance::InstancingAppointRe(void *pData)
{
	InstancingAppoint_Re *p = (InstancingAppoint_Re*)pData;
	if (p->retcode!=0)
	{
		GetGameUIMan()->MessageBox("", GetGameUIMan()->GetStringFromTable(8400+p->retcode), MB_OK, A3DCOLORRGBA(255, 255, 255, 160));
		return;
	}
	if (m_mapMap2Tid.find(p->map_id)!=m_mapMap2Tid.end())
	{
		m_nJoinTID = m_mapMap2Tid[p->map_id];
	}
	UINT i;
	PAUILISTBOX pLstMember = (PAUILISTBOX)GetDlgItem("Lst_Member1");
	pLstMember->ResetContent();
	int k=0;
	ACString strText, strName;
	for (i=0; i<m_vecMembers.size(); i++)
	{
		GBattleRole role = m_vecMembers[i];
		if (role.roleid == p->target)
		{
			PAUILABEL pLabLevel = (PAUILABEL)GetDlgItem("Lab_LvTeamLeader");
			PAUILABEL pLabName = (PAUILABEL)GetDlgItem("Lab_TeamLeaderName");
			strText.Format(_AL("%s%d%s"),GetStringFromTable(PROFESSION_START + role.occupation),role.level, role.reborn_cnt>0 ? GetStringFromTable(7012) : _AL(""));
			pLabLevel->SetText(strText);
			strText = ACString((const ACHAR*)role.name.begin(), role.name.size() / sizeof (ACHAR));
			pLabName->SetText(strText);
			continue;
		}
		strName = ACString((const ACHAR*)role.name.begin(), role.name.size() / sizeof (ACHAR));
		strText.Format(_AL("%s%d%s\t%s"),GetStringFromTable(PROFESSION_START + role.occupation),role.level, role.reborn_cnt>0 ? GetStringFromTable(7012) : _AL(""), strName);
		pLstMember->AddString(strText);
		pLstMember->SetItemData(k, role.roleid);
		k++;
	}
	bool bOwner = p->target == GetHostPlayer()->GetCharacterID();
	GetDlgItem("Btn_KickOut")->Enable(bOwner);
	GetDlgItem("Btn_ChangeLeader")->Enable(bOwner);
	GetDlgItem("Btn_Agree")->Enable(bOwner);
	GetDlgItem("Btn_Refuse")->Enable(bOwner);
	GetDlgItem("Btn_Start")->Enable(bOwner);
}
void CDlgBattleInstance::InstancingAppointNotifyAction(void *pData)
{
	InstancingAppointNotify *p = (InstancingAppointNotify*)pData;
	if (m_mapMap2Tid.find(p->map_id)!=m_mapMap2Tid.end())
	{
		m_nJoinTID = m_mapMap2Tid[p->map_id];
	}
	UINT i;
	PAUILISTBOX pLstMember = (PAUILISTBOX)GetDlgItem("Lst_Member1");
	pLstMember->ResetContent();
	int k=0;
	ACString strText, strName;
	int hostId = GetHostPlayer()->GetCharacterID();
	for (i=0; i<m_vecMembers.size(); i++)
	{
		GBattleRole role = m_vecMembers[i];
		if (role.roleid == hostId)
		{
			PAUILABEL pLabLevel = (PAUILABEL)GetDlgItem("Lab_LvTeamLeader");
			PAUILABEL pLabName = (PAUILABEL)GetDlgItem("Lab_TeamLeaderName");
			strText.Format(_AL("%s%d%s"),GetStringFromTable(PROFESSION_START + role.occupation),role.level,role.reborn_cnt>0 ? GetStringFromTable(7012) : _AL(""));
			pLabLevel->SetText(strText);
			strText = ACString((const ACHAR*)role.name.begin(), role.name.size() / sizeof (ACHAR));
			pLabName->SetText(strText);
			continue;
		}
		strName = ACString((const ACHAR*)role.name.begin(), role.name.size() / sizeof (ACHAR));
		strText.Format(_AL("%s%d%s\t%s"),GetStringFromTable(PROFESSION_START + role.occupation),role.level,role.reborn_cnt>0 ? GetStringFromTable(7012) : _AL(""), strName);
		pLstMember->AddString(strText);
		pLstMember->SetItemData(k, role.roleid);
		k++;
	}
	
	bool bOwner = true;
	GetDlgItem("Btn_KickOut")->Enable(bOwner);
	GetDlgItem("Btn_ChangeLeader")->Enable(bOwner);
	GetDlgItem("Btn_Agree")->Enable(bOwner);
	GetDlgItem("Btn_Refuse")->Enable(bOwner);
	GetDlgItem("Btn_Start")->Enable(bOwner);
}
void CDlgBattleInstance::InstancingEnterRe(void *pData)
{
	//显示距离战场结束时间
	InstancingEnter_Re *p = (InstancingEnter_Re*)pData;
	if (p->retcode!=0)
	{
		GetGameUIMan()->MessageBox("", GetGameUIMan()->GetStringFromTable(8400+p->retcode), MB_OK, A3DCOLORRGBA(255, 255, 255, 160));
		return;
	}
}
void CDlgBattleInstance::InstancingGetFieldRe(void *pData)
{
	InstancingGetField_Re *p = (InstancingGetField_Re*)pData;
	if (p->retcode !=0)
	{
		GetGameUIMan()->MessageBox("", GetGameUIMan()->GetStringFromTable(8400+p->retcode), MB_OK, A3DCOLORRGBA(255, 255, 255, 160));
		return;
	}
	UINT i;
	std::vector<GBattleRole> vecMembers  = p->attacker;
	m_vecMembers.clear();
	for (i=0; i<vecMembers.size(); i++)
	{
		m_vecMembers.push_back(vecMembers[i]);
	}
	if (m_vecMembers.size()<1)
	{
		assert(0);
		return;
	}
	vecMembers  = p->req_attacker;
	m_vecApplyers.clear();
	for (i=0; i<vecMembers.size(); i++)
	{
		m_vecApplyers.push_back(vecMembers[i]);
	}
	PAUILISTBOX pLstMember = (PAUILISTBOX)GetDlgItem("Lst_Member1");
	pLstMember->ResetContent();
	PAUILISTBOX pLstApplyer = (PAUILISTBOX)GetDlgItem("Lst_Member2");
	pLstApplyer->ResetContent();
	ACString strText;
	ACString strName;
	PAUILABEL pLabel = (PAUILABEL)GetDlgItem("Lab_LvTeamLeader");
	strText.Format(_AL("%s%d%s"),GetStringFromTable(PROFESSION_START + m_vecMembers[0].occupation),m_vecMembers[0].level, m_vecMembers[0].reborn_cnt>0 ? GetStringFromTable(7012) : _AL(""));
	pLabel->SetText(strText);
	pLabel->SetData(m_vecMembers[0].roleid);
	pLabel = (PAUILABEL)GetDlgItem("Lab_TeamLeaderName");
	strName = ACString((const ACHAR*)m_vecMembers[0].name.begin(), m_vecMembers[0].name.size() / sizeof (ACHAR));
	pLabel->SetText(strName);
	for (i=0; i<(m_vecMembers.size()-1); i++)
	{
		GBattleRole role = m_vecMembers[i+1];
		strName = ACString((const ACHAR*)role.name.begin(), role.name.size() / sizeof (ACHAR));
		strText.Format(_AL("%s%d%s\t%s"),GetStringFromTable(PROFESSION_START + role.occupation),role.level,role.reborn_cnt>0 ? GetStringFromTable(7012) : _AL(""), strName);
		pLstMember->AddString(strText);
		pLstMember->SetItemData(i, role.roleid);
	}
	for (i=0; i<m_vecApplyers.size(); i++)
	{
		GBattleRole role = m_vecApplyers[i];
		strName = ACString((const ACHAR*)role.name.begin(), role.name.size() / sizeof (ACHAR));
		strText.Format(_AL("%s%d%s\t%s"),GetStringFromTable(PROFESSION_START + role.occupation),role.level,role.reborn_cnt>0 ? GetStringFromTable(7012) : _AL(""), strName);
		pLstApplyer->AddString(strText);
		pLstApplyer->SetItemData(i, role.roleid);
	}

	PAUILABEL pLabNum1 = (PAUILABEL)GetDlgItem("Txt_Num1");
	PAUILABEL pLabNum2 = (PAUILABEL)GetDlgItem("Txt_Num2");
	const INSTANCING_BATTLE_CONFIG *pInstance = GetInstanceConfig(m_nJoinTID);
	strText.Format(_AL("%d/%d"),m_vecMembers.size(),pInstance->max_player);
	pLabNum1->SetText(strText);
	strText.Format(_AL("%d/%d"),m_vecApplyers.size(),pInstance->max_player);
	pLabNum2->SetText(strText);
	PAUILABEL pLabNotice = (PAUILABEL)GetDlgItem("Txt_Notice");
	strText.Format(GetGameUIMan()->GetStringFromTable(8499), pInstance->apply_time/60);
	pLabNotice->SetText(strText);

	bool bOwner = m_vecMembers[0].roleid == GetHostPlayer()->GetCharacterID();
	GetDlgItem("Btn_KickOut")->Enable(bOwner);
	GetDlgItem("Btn_ChangeLeader")->Enable(bOwner);
	GetDlgItem("Btn_Agree")->Enable(bOwner);
	GetDlgItem("Btn_Refuse")->Enable(bOwner);
	GetDlgItem("Btn_Start")->Enable(bOwner);

	Show(true);
}
void CDlgBattleInstance::InstancingJoinRe(void *pData)
{
	InstancingJoin_Re *p = (InstancingJoin_Re*)pData;
	if (p->retcode!=0)
	{
		GetGameUIMan()->MessageBox("", GetGameUIMan()->GetStringFromTable(8400+p->retcode), MB_OK, A3DCOLORRGBA(255, 255, 255, 160));
		return;
	}
	if (m_mapMap2Tid.find(p->map_id)!=m_mapMap2Tid.end())
	{
		m_nJoinTID = m_mapMap2Tid[p->map_id];
	}
// 	else
// 	{
// 		GetGameUIMan()->MessageBox("", GetGameUIMan()->GetStringFromTable(8510), MB_OK, A3DCOLORRGBA(255, 255, 255, 160));
// 	}

// 	PAUILABEL pLabLevel = (PAUILABEL)GetDlgItem("Lab_LvTeamLeader");
// 	PAUILABEL pLabName = (PAUILABEL)GetDlgItem("Lab_TeamLeaderName");
// 	ACString strText;
// 	strText.Format(_AL("%s%d%s"),GetStringFromTable(101 + GetHostPlayer()->GetReputation()), GetHostPlayer()->GetBasicProps().iLevel,
// 		GetHostPlayer()->GetRebornInfo().size()>0 ? GetStringFromTable(7012) : _AL(""));
// 	pLabLevel->SetText(strText);
// 	pLabName->SetText(GetHostPlayer()->GetName());
 	PAUILABEL pLabInstanceName = (PAUILABEL)GetDlgItem("Title");
 	const INSTANCING_BATTLE_CONFIG *pInstance = GetInstanceConfig(m_nJoinTID);
	if (pInstance)
	{
		pLabInstanceName->SetText(pInstance->name);
	}
	GetGameUIMan()->m_pDlgHost->SetInstanceInfo(CDlgHost::BATTLE_STATE_INTEAM, 
		p->starttime, p->gs_id, p->map_id);
	GetGameSession()->instancing_getfield(GetGameRun()->GetCurLine(), GetData());
	
	((CDlgBattleInstance*)GetGameUIMan()->GetDialog("Win_BattleRaid"))->OnCommandCancel("");
//	Show(true);
}
void CDlgBattleInstance::InstancingJoinReqAction(void *pData)
{
	InstancingJoinReq *p = (InstancingJoinReq*)pData;
	if (m_mapMap2Tid.find(p->map_id)!=m_mapMap2Tid.end())
	{
		m_nJoinTID = m_mapMap2Tid[p->map_id];
	}
	GetGameSession()->instancing_getfield(GetGameRun()->GetCurLine(), GetData());
}
void CDlgBattleInstance::InstancingKickRe(void *pData)
{
	InstancingKick_Re *p = (InstancingKick_Re*)pData;
	if (p->retcode!=0)
	{
		GetGameUIMan()->MessageBox("", GetGameUIMan()->GetStringFromTable(8400+p->retcode), MB_OK, A3DCOLORRGBA(255, 255, 255, 160));
		return;
	}
	if (m_mapMap2Tid.find(p->map_id)!=m_mapMap2Tid.end())
	{
		m_nJoinTID = m_mapMap2Tid[p->map_id];
	}
	PAUILISTBOX pLstMember = (PAUILISTBOX)GetDlgItem("Lst_Member1");
	int k=0;
	ACString strText;
	for (int i=0; i<pLstMember->GetCount(); i++)
	{
		if (p->target == (int)pLstMember->GetItemData(i))
		{
			pLstMember->DeleteString(i);
			break;
		}
	}
	PAUILABEL pLabNum1 = (PAUILABEL)GetDlgItem("Txt_Num1");
	const INSTANCING_BATTLE_CONFIG *pInstance = GetInstanceConfig(m_nJoinTID);
	strText.Format(_AL("%d/%d"),m_vecMembers.size(),pInstance->max_player);
	pLabNum1->SetText(strText);
}
void CDlgBattleInstance::InstancingKickNotifyAction(void *pData)
{
	InstancingKickNotify *p = (InstancingKickNotify*)pData;
	if (m_mapMap2Tid.find(p->map_id)!=m_mapMap2Tid.end())
	{
		m_nTID = m_mapMap2Tid[p->map_id];
	}
	PAUILISTBOX pLstMember = (PAUILISTBOX)GetDlgItem("Lst_Member1");
	pLstMember->ResetContent();
	int k=0;
	ACString strText;
	for (UINT i=0; i<m_vecMembers.size(); i++)
	{
		GBattleRole role = m_vecMembers[i];
		if (role.roleid == p->target)
		{
			m_vecMembers.erase(&m_vecMembers[i]);
			continue;
		}
		ACString strName = ACString((const ACHAR*)role.name.begin(), role.name.size() / sizeof (ACHAR));
		strText.Format(_AL("%s%d\t%s"),GetStringFromTable(PROFESSION_START + role.occupation),role.level, strName);
		pLstMember->AddString(strText);
		pLstMember->SetItemData(k, role.roleid);
		k++;
	}
	PAUILABEL pLabNum1 = (PAUILABEL)GetDlgItem("Txt_Num1");
	const INSTANCING_BATTLE_CONFIG *pInstance = GetInstanceConfig(m_nTID);
	strText.Format(_AL("%d/%d"),m_vecMembers.size(),pInstance->max_player);
	pLabNum1->SetText(strText);
}
void CDlgBattleInstance::InstancingLeaveRe(void *pData)
{
	InstancingLeave_Re *p = (InstancingLeave_Re*)pData;
	if (p->retcode!=0)
	{
		GetGameUIMan()->MessageBox("", GetGameUIMan()->GetStringFromTable(8400+p->retcode), MB_OK, A3DCOLORRGBA(255, 255, 255, 160));
		return;
	}
	Show(false);
}
void CDlgBattleInstance::InstancingStartRe(void *pData)
{
	// 通知战场开启者，战场开启成功，提示或不做处理
	InstancingStart_Re *p = (InstancingStart_Re*)pData;
	if (p->retcode!=0)
		GetGameUIMan()->MessageBox("", GetGameUIMan()->GetStringFromTable(8400+p->retcode), MB_OK, A3DCOLORRGBA(255, 255, 255, 160));
	if (m_mapMap2Tid.find(p->map_id)!=m_mapMap2Tid.end())
	{
		m_nJoinTID = m_mapMap2Tid[p->map_id];
	}
}

void CDlgBattleInstance::InstancingStartNotifyAction(void *pData)
{
	// 通知进入战场
	InstancingStartNotify *p = (InstancingStartNotify*)pData;
	if (m_mapMap2Tid.find(p->map_id)!=m_mapMap2Tid.end())
	{
		m_nJoinTID = m_mapMap2Tid[p->map_id];
	}
	PAUIDIALOG pMsgBox;
	ACString strText;
	strText.Format(GetGameUIMan()->GetStringFromTable(914), GetGameUIMan()->GetStringFromTable(1900 + p->map_id));
	GetGameUIMan()->MessageBox("Game_BattleInstancingEnter", strText, MB_YESNO, A3DCOLORRGBA(255, 255, 255, 160), &pMsgBox);
	pMsgBox->SetData(p->map_id);
	((CDlgHost*)GetGameUIMan()->GetDialog("Win_CharHead"))->SetInstanceInfo(CDlgHost::BATTLE_STATE_NORMAL, 0);

}

void CDlgBattleInstance::InstancingCloseNotifyAction(void *pData)
{
	InstancingCloseNotify *p = (InstancingCloseNotify*)pData;
	if (p->reason !=0)
		GetGameUIMan()->MessageBox("",GetStringFromTable(8400+p->reason), MB_OK, A3DCOLORRGBA(255, 255, 255, 160));

	((CDlgHost*)GetGameUIMan()->GetDialog("Win_CharHead"))->SetInstanceInfo(CDlgHost::BATTLE_STATE_NORMAL, 0);
}

void CDlgBattleInstance::InstancingSelfInfo(void *pData)
{
}

void CDlgBattleInstance::InstancingInfo(void *pData)
{
	S2C::instancing_battle_info* info = (S2C::instancing_battle_info*)pData;
	int i;
	ACString strText;
	const static int stageColor[4] = 
	{
		0xFFFFFF,
		0xFFFF00,
		0x00FF00,	
		0x00FFFF,	
	};
	const INSTANCING_BATTLE_CONFIG *pInstance = GetInstanceConfig(m_nJoinTID);
	PAUILISTBOX pLst = (PAUILISTBOX)GetDlgItem("Lst_Stage");
	pLst->ResetContent();
	for (i=0; i<pInstance->valid_tollgate_num; i++)
	{
		strText.Format(GetGameUIMan()->GetStringFromTable(8495+info->level_status[i]),i+1,pInstance->tollgate[i].tollgate_name);
		pLst->AddString(strText);
		pLst->SetItemTextColor(i,stageColor[info->level_status[i]]);
		if (info->level_status[i] == 1)
		{
			int nSel = i;
			ACString strText;
			const INSTANCING_BATTLE_CONFIG *pInstance = GetInstanceConfig(m_nJoinTID);
			strText.Format(_AL("%d"),pInstance->tollgate[nSel].score);
			PAUILABEL pLabAimScore = (PAUILABEL)GetDlgItem("Txt_AimScore");
			pLabAimScore->SetText(strText);
			PAUITEXTAREA pTxtDesc = (PAUITEXTAREA)GetDlgItem("Txt_Detail");
			abase::vector<CScriptValue> args, results;
			args.push_back((double)m_nJoinTID);
			args.push_back((double)(nSel+1));
			m_pState->Call(INSTANCING_DESC_TBL_NAME, "GetBattleLevelDesc", args, results);
			ACString strDesc = _AL("");
			if (results.size() == 1 && results[0].m_Type == CScriptValue::SVT_STRING)
			{
				results[0].RetrieveAWString(strDesc);
			}
			pTxtDesc->SetText(strDesc);
			m_iCurStage = i;
			m_iStateTime = pInstance->tollgate[m_iCurStage].time_limit;
		}
	}
	for (; i<pInstance->valid_tollgate_num; i++)
	{
		pLst->AddString(_AL("??????????"));
		pLst->SetItemTextColor(i,stageColor[0]);
	}

	PAUILABEL pLab = (PAUILABEL)GetDlgItem("Title");
	pLab->SetText(pInstance->name);
}

const INSTANCING_BATTLE_CONFIG* CDlgBattleInstance::GetInstanceConfig(int tid)
{
	DATA_TYPE DataType;
	const void* pData = g_pGame->GetElementDataMan()->get_data_ptr(tid, ID_SPACE_CONFIG, DataType);
	if (pData && DataType == DT_INSTANCING_BATTLE_CONFIG)
		return (const INSTANCING_BATTLE_CONFIG*)pData;
	return NULL;
}

void CDlgBattleInstance::RefreshBattleList(bool bRefresh)
{
	PAUILISTBOX pList = (PAUILISTBOX)GetDlgItem("List_BattleField");
	int nSel = pList->GetCurSel();
	if( nSel >= 0 && nSel < pList->GetCount() )
	{
		m_nTID = pList->GetItemData(nSel);
		const INSTANCING_BATTLE_CONFIG *pInstance = GetInstanceConfig(m_nTID);
		if( pInstance )
		{
			PAUILABEL pLabNum = (PAUILABEL)GetDlgItem("Txt_Num");
			ACString strText;
			if (m_mapEnterTime.find(m_nTID) != m_mapEnterTime.end())
				strText.Format(GetGameUIMan()->GetStringFromTable(8489),m_mapEnterTime[m_nTID],3);
			else
				strText.Format(_AL("error"));
			pLabNum->SetText(strText);

			abase::vector<CScriptValue> args, results;
			args.push_back((double)m_nTID);
			m_pState->Call(INSTANCING_DESC_TBL_NAME, "GetBattleDesc", args, results);
			ACString strDesc = _AL("");
			if (results.size() == 1 && results[0].m_Type == CScriptValue::SVT_STRING)
			{
				results[0].RetrieveAWString(strDesc);
			}
			PAUIOBJECT pObj = GetDlgItem("Txt_BattleInfo");
			if (strDesc != pObj->GetText())
			{
				pObj->SetText(strDesc);
			}
			
			pList = (PAUILISTBOX)GetDlgItem("List_Member");
			if (!bRefresh)
			{
				pList->ResetContent();
			}
			int i, j;
			int k = 0;
			int num = 0;
			for(i = 0; i < (int)m_vecFieldList.size(); i++ )
			{
				if( m_vecFieldList[i].tid != m_nTID )
					continue;

				ACString strLine = _AL("");
				ACString strStatus = _AL(" ");
				ACString strOwner = ACString((const ACHAR*)m_vecFieldList[i].info.leader.begin(), m_vecFieldList[i].info.leader.size() / sizeof (ACHAR));
				ACString strText;
				ACString strTime;
				A3DCOLOR Color1 = A3DCOLORRGB(255, 255, 255);
				A3DCOLOR Color2 = A3DCOLORRGB(255, 255, 255);

				CECGameRun::LineInfoVec lineinfo = GetGameRun()->GetLinesInfo();
				for(j = 0; j < (int)lineinfo.size(); j++ )
					if( lineinfo[j].id == m_vecFieldList[i].info.gs_id )
					{
						strLine = lineinfo[j].name;
						break;
					}
				if (strLine.IsEmpty())
				{
					strLine = GetStringFromTable(574);
				}
					
				strStatus = GetStringFromTable(8499 + m_vecFieldList[i].info.status);
				if (m_vecFieldList[i].info.status==1 ||m_vecFieldList[i].info.status==3)
				{
					strTime = GetFormatTime((m_vecFieldList[i].info.time_left - GetTickCount())/1000);				
				}
				else
					strTime.Format(_AL("---"));
				


				if (m_vecFieldList[i].info.player_limit > 0)
				{
					Color1 = A3DCOLORRGB(255, 255, 0);
					if (pInstance->max_player == (m_vecFieldList[i].info.player_number))
					{
						Color1 = A3DCOLORRGB(255, 0, 0);
					}
				}
				strText.Format(ACString(_AL("%s\t%s\t%d-%d\t%s\t%s")), strLine, strStatus, m_vecFieldList[i].info.player_number, pInstance->max_player, strTime, strOwner);
				
				if( m_vecFieldList[i].info.gs_id == GetGameRun()->GetCurLine() )
				{
					if (bRefresh)
					{
						//模拟listbox
						for (j=num; j>k; j--)
						{
							pList->SetText(j, pList->GetText(j-1));
						}
						pList->SetText(k, strText);
					}
					else
					{
						pList->InsertString(k, strText);
						pList->SetItemData(k, m_vecFieldList[i].info.gs_id, 0);
						pList->SetItemData(k, m_vecFieldList[i].info.map_id, 1);
						pList->SetItemData(k, m_vecFieldList[i].info.player_number, 2);
						pList->SetItemData(k, m_vecFieldList[i].info.status, 3);
						pList->SetItemData(k, strOwner == GetHostPlayer()->GetName(), 4);
						pList->SetItemTextColor(k, Color1, 1);
					}
					k++;
					num++;
				}
				else
				{
					if (bRefresh)
					{
						pList->SetText(num, strText);
					}
					else
					{
						pList->AddString(strText);
						pList->SetItemData(pList->GetCount() - 1, m_vecFieldList[i].info.gs_id, 0);
						pList->SetItemData(pList->GetCount() - 1, m_vecFieldList[i].info.map_id, 1);
						pList->SetItemData(pList->GetCount() - 1, m_vecFieldList[i].info.player_number, 2);
						pList->SetItemData(pList->GetCount() - 1, m_vecFieldList[i].info.status, 3);
						pList->SetItemData(pList->GetCount() - 1, strOwner == GetHostPlayer()->GetName(), 4);
						pList->SetItemTextColor(pList->GetCount() - 1, Color1, 1);
					}
					num++;
				}
			}
		}
	}
}

ACString CDlgBattleInstance::GetFormatTime(int nTime)
{
	ACString szTime;
	if( nTime <= 0 )
		szTime.Format(_AL("0"));
	else if( nTime > 3600)
		szTime.Format(GetStringFromTable(1603), nTime / 3600 + 1);
	else if( nTime > 60 )
		szTime.Format(GetStringFromTable(1602), nTime / 60 + 1);
	else if( nTime > 0 )
		szTime.Format(GetStringFromTable(1601), nTime);
	else
		szTime.Format(GetStringFromTable(1601), 0);
	
	return szTime;
}

#undef INSTANCING_DESC_TBL_NAME