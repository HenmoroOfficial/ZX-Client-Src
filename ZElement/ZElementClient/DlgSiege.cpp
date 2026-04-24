// File		: DlgSiege.cpp
// Creator	: Xiao Zhou
// Date		: 2007/2/19

#include "AUI\\AUIListBox.h"
#include "AUI\\AUIRadioButton.h"
#include "DlgSiege.h"
#include "DlgHost.h"
#include "EC_Game.h"
#include "EC_GameRun.h"
#include "EC_GameUIMan.h"
#include "EC_GameSession.h"
#include "EC_Faction.h"
#include "EC_HostPlayer.h"
#include "EC_Inventory.h"
#include "siegeChallenge_re.hpp"
#include "siegesetassistant_re.hpp"
#include "siegeenter_re.hpp"
#include "siegeinfoget_re.hpp"
#include "siegetop_re.hpp"
#include "gfactionmaster"
#include "ids.hxx"

#define new A_DEBUG_NEW

static const A3DCOLOR MSGBOX_COLOR		= A3DCOLORRGBA(255, 255, 255, 160);
static const int CHALLENGE_ITEM_ID		= 16381;	// 城战报名令牌ID
static const int FACTION_LEVEL_LIMIT	= 3;		// 城战帮派级别限制
static const int CHALLENGE_MONEY_PERDAY = 1000000;	// 城战报名一天100金
static const ACHAR EMPTY_STR[]			= _AL("---");

DWORD CDlgSiege::m_dwLastJoinTime = 0;
bool CDlgSiege::m_bCanJoin = false;

AUI_BEGIN_COMMAND_MAP(CDlgSiege, CDlgBase)
// {
AUI_ON_COMMAND("setassist",		OnCommandSetAssist)
AUI_ON_COMMAND("confirmassist", OnCommandConfirmAssist)
AUI_ON_COMMAND("choice",		OnCommandChoice)
AUI_ON_COMMAND("challenge",		OnCommandChallenge)
AUI_ON_COMMAND("IDCANCEL",		OnCommandCancel)
AUI_ON_COMMAND("refreshsiegeinfo", OnCommandRefreshSiegeInfo)
AUI_ON_COMMAND("refreshguildsiege", OnCommandRefreshSiegeInfo)
AUI_ON_COMMAND("refreshsiegescore", OnCommandRefreshSiegeScore)
// }
AUI_END_COMMAND_MAP()

AUI_BEGIN_EVENT_MAP(CDlgSiege, CDlgBase)
// {
AUI_ON_EVENT("List_Member",	WM_RBUTTONUP,	OnEventRButtonUp)
// }
AUI_END_EVENT_MAP()

CDlgSiege::CDlgSiege()
{
}

CDlgSiege::~CDlgSiege()
{
}

bool CDlgSiege::OnInitDialog()
{
	if (stricmp(m_szName, "Win_SiegeInfo") == 0) {
		GetDlgItem("Lab_Name_DefenderGuild")->SetText(EMPTY_STR);
		GetDlgItem("Lab_Name_Defender")->SetText(EMPTY_STR);
		GetDlgItem("Lab_Name_AttackerGuild")->SetText(EMPTY_STR);
		GetDlgItem("Lab_Name_Attacker")->SetText(EMPTY_STR);
		GetDlgItem("Lab_Name_HelperGuild")->SetText(EMPTY_STR);
		GetDlgItem("Lab_Name_Helper")->SetText(EMPTY_STR);

		GetDlgItem("Btn_Join")->Enable(false);
		GetDlgItem("Btn_SiegeHelprCh")->Enable(false);
	}
	return true;
}

void CDlgSiege::OnCommandSetAssist(const char * szCommand)
{
	int fRoleID = GetHostPlayer()->GetFRoleID();
	bool bCanSetAssist = (fRoleID == GNET::_R_MASTER || fRoleID == GNET::_R_VICEMASTER);
	if (!bCanSetAssist) {
		GetGameUIMan()->MessageBox("", GetStringFromTable(960), MB_OK, MSGBOX_COLOR);
	} else {
		Faction_Info* pFInfo = GetGame()->GetFactionMan()->GetFaction(
									GetHostPlayer()->GetFactionID(), true);
		if (pFInfo && pFInfo->GetLevel()+1 < FACTION_LEVEL_LIMIT) {
			bCanSetAssist = false;
			GetGameUIMan()->MessageBox("", GetStringFromTable(958), MB_OK, MSGBOX_COLOR);
		}
	}
	if (bCanSetAssist)
		GetGameUIMan()->GetDialog("Win_SiegeHelperCh")->Show(true);
}

void CDlgSiege::OnCommandConfirmAssist(const char * szCommand)
{
	if (stricmp(m_szName, "Win_SiegeHelperCh") != 0) 
		return;

	const ACHAR* strFName = GetDlgItem("DEFAULT_Txt_Input")->GetText();
	// SELF
	const ACHAR* hostFName = NULL;
	Faction_Info* pFInfo = GetGame()->GetFactionMan()->GetFaction(GetHostPlayer()->GetFactionID(), true);		
	if (pFInfo)	hostFName = pFInfo->GetName();	
	if (hostFName && a_strcmp(hostFName, strFName) == 0) {
		GetGameUIMan()->MessageBox("", GetStringFromTable(957), MB_OK, MSGBOX_COLOR);
		return;
	}
	// OWNER
	const ACHAR* OwnerName = NULL;
	PAUIOBJECT pLab = GetGameUIMan()->GetDialog("Win_SiegeInfo")->GetDlgItem("Lab_Name_DefenderGuild");
	if (pLab && pLab->GetData())
		OwnerName = pLab->GetText();
	if (OwnerName && a_strcmp(OwnerName, strFName) == 0) {
		GetGameUIMan()->MessageBox("", GetStringFromTable(987), MB_OK, MSGBOX_COLOR);
		return;
	}

	GetGameSession()->war_assistant(1, strFName);
	Show(false);
}

void CDlgSiege::OnWarAssistRe(const GNET::SiegeSetAssistant_Re* p)
{
	CECGameUIMan *pGameUI = GetGameUIMan();
	switch(p->retcode) {
	case ERR_SUCCESS:
		pGameUI->MessageBox("", GetStringFromTable(954), MB_OK, MSGBOX_COLOR);
		// 指定协助帮成功之后，刷新列表
		OnCommandRefreshSiegeInfo("refreshsiegeinfo");
		break;
	case ERR_SIEGE_TIMEERROR:
		pGameUI->MessageBox("", GetStringFromTable(956), MB_OK, MSGBOX_COLOR);
		break;
	case ERR_SIEGE_ASSISTANT_SELF:
		pGameUI->MessageBox("", GetStringFromTable(957), MB_OK, MSGBOX_COLOR);
		break;
	case ERR_SIEGE_FACTIONLIMIT:
		pGameUI->MessageBox("", GetStringFromTable(958), MB_OK, MSGBOX_COLOR);
		break;
	case ERR_SIEGE_FACTIONNOTFOUND:
		pGameUI->MessageBox("", GetStringFromTable(959), MB_OK, MSGBOX_COLOR);
		break;
	case ERR_SIEGE_ASSISTANT_TIME:
		pGameUI->MessageBox("", GetStringFromTable(985), MB_OK, MSGBOX_COLOR);
		break;
	case ERR_SIEGE_ASSISTANT_LEVEL:
		pGameUI->MessageBox("", GetStringFromTable(986), MB_OK, MSGBOX_COLOR);
		break;
	case ERR_SIEGE_ASSISTANT_OWNER:
		pGameUI->MessageBox("", GetStringFromTable(987), MB_OK, MSGBOX_COLOR);
		break;
	default:
		pGameUI->MessageBox("", GetStringFromTable(955), MB_OK, MSGBOX_COLOR);
	}		
}

void CDlgSiege::OnEventRButtonUp(WPARAM wParam, LPARAM lParam, AUIObject * pObj)
{
	if (stricmp(m_szName, "Win_SiegeInfo") != 0)
		return;

	POINT ptPos = pObj->GetPos();
	PAUILISTBOX pList = (PAUILISTBOX)pObj;
	int nCurSel = pList->GetCurSel();
	A3DVIEWPORTPARAM *p = m_pA3DEngine->GetActiveViewport()->GetParam();
	int x = GET_X_LPARAM(lParam) - ptPos.x - p->X;
	int y = GET_Y_LPARAM(lParam) - ptPos.y - p->Y;

	if( nCurSel >= 0 && nCurSel < pList->GetCount() &&
		pList->GetHitArea(x, y) == AUILISTBOX_RECT_TEXT )
	{
		int idPlayer = pList->GetItemData(nCurSel, 1);
		x = GET_X_LPARAM(lParam) - p->X;
		y = GET_Y_LPARAM(lParam) - p->Y;

		GetGameUIMan()->PopupPlayerContextMenu(idPlayer, x, y);
	}
}

void CDlgSiege::OnCommandChoice(const char * szCommand)
{
	PAUIRADIOBUTTON pRadio1 = (PAUIRADIOBUTTON)GetDlgItem("Rdo_Choice1");
	PAUIRADIOBUTTON pRadio2 = (PAUIRADIOBUTTON)GetDlgItem("Rdo_Choice2");
	PAUIRADIOBUTTON pRadio3 = (PAUIRADIOBUTTON)GetDlgItem("Rdo_Choice3");

	int nDay = pRadio1->IsChecked() ? 1 : 
				(pRadio2->IsChecked() ? 2 : (pRadio3->IsChecked() ? 3 : 0));
	if (nDay < 1)
		return;

	CECInventory *pPack = GetHostPlayer()->GetPack();
	int count = pPack->GetItemTotalNum(CHALLENGE_ITEM_ID);
	if (count < nDay) {
		GetGameUIMan()->MessageBox("", GetStringFromTable(946), MB_OK, MSGBOX_COLOR);
	} else if (GetHostPlayer()->GetMoneyAmount() < nDay*CHALLENGE_MONEY_PERDAY) {
		GetGameUIMan()->MessageBox("", GetStringFromTable(947), MB_OK, MSGBOX_COLOR);
	} else {
		GetGameSession()->war_challege(1, nDay);
		Show(false);
	}
}

void CDlgSiege::OnCommandChallenge(const char * szCommand)
{
	// 必须是帮主/副帮主
	int fRoleID = GetHostPlayer()->GetFRoleID();		
	if (fRoleID != GNET::_R_MASTER && fRoleID != GNET::_R_VICEMASTER) {
		GetGameUIMan()->MessageBox("", GetStringFromTable(952), MB_OK, MSGBOX_COLOR);
		return;
	}
	// 帮派级别必须达到要求
	Faction_Info* pFInfo = GetGame()->GetFactionMan()->GetFaction(
								GetHostPlayer()->GetFactionID(), true);
	if (pFInfo && pFInfo->GetLevel()+1 < FACTION_LEVEL_LIMIT) {
		GetGameUIMan()->MessageBox("", GetStringFromTable(951), MB_OK, MSGBOX_COLOR);
		return;
	}

	GetGameUIMan()->GetDialog("Win_SiegeJoinChoice")->Show(true);
}

void CDlgSiege::OnWarChallengeRe(const GNET::SiegeChallenge_Re* p)
{
	CECGameUIMan *pGameUI = GetGameUIMan();
	switch(p->retcode) {
	case ERR_SUCCESS:
		pGameUI->MessageBox("", GetStringFromTable(944), MB_OK, MSGBOX_COLOR);
		m_dwLastJoinTime = GetTickCount();
		// 报名成功之后，刷新报名列表
		OnCommandRefreshSiegeInfo("refreshsiegeinfo");
		break;
	case ERR_SIEGE_QUEUEFULL:
		pGameUI->MessageBox("", GetStringFromTable(953), MB_OK, MSGBOX_COLOR);
		break;
	case ERR_SIEGE_FACTIONLIMIT:
		pGameUI->MessageBox("", GetStringFromTable(951), MB_OK, MSGBOX_COLOR);
		break;
	case ERR_SIEGE_CHALLENGED:{
		ACString str;
		str.Format(GetStringFromTable(950), GetStringFromTable(943));
		pGameUI->MessageBox("", str, MB_OK, MSGBOX_COLOR);}
		break;
	case ERR_SIEGE_SELF:{
		ACString str;
		str.Format(GetStringFromTable(949), GetStringFromTable(943));
		pGameUI->MessageBox("", str, MB_OK, MSGBOX_COLOR);}
		break;
	case ERR_BS_NEWBIE_BANNED:
		pGameUI->MessageBox("", GetStringFromTable(948), MB_OK, MSGBOX_COLOR);
		break;
	case ERR_SIEGE_MONEYLIMIT:
		pGameUI->MessageBox("", GetStringFromTable(947), MB_OK, MSGBOX_COLOR);
		break;
	case ERR_SIEGE_ITEMLIMIT:
		pGameUI->MessageBox("", GetStringFromTable(946), MB_OK, MSGBOX_COLOR);
		break;
	case ERR_SIEGE_COOLING:
		pGameUI->MessageBox("", GetStringFromTable(931), MB_OK, MSGBOX_COLOR);
		break;
	default:
		pGameUI->MessageBox("", GetStringFromTable(945), MB_OK, MSGBOX_COLOR);
	}			
}

void CDlgSiege::OnWarEnterService()
{
	/*int hostFID = GetHostPlayer()->GetFactionID();
	// NOFACTION
	if (hostFID == 0) {
		ACString str;
		str.Format(GetStringFromTable(977), GetStringFromTable(943));
		GetGameUIMan()->MessageBox("", str, MB_OK, MSGBOX_COLOR);
		return;
	}
	// LEVEL
	Faction_Info* pFInfo = GetGame()->GetFactionMan()->GetFaction(hostFID, true);
	if (pFInfo && pFInfo->GetLevel()+1 < FACTION_LEVEL_LIMIT) {
		GetGameUIMan()->MessageBox("", GetStringFromTable(988), MB_OK, MSGBOX_COLOR);
		return;
	}*/

	GetGameSession()->war_enter(1);
}

void CDlgSiege::OnWarEnterRe(const GNET::SiegeEnter_Re* p)
{
	CECGameUIMan *pGameUI = GetGameUIMan();
	switch(p->retcode) {
	case ERR_SUCCESS:
		// Do Nothing.
		break;
	case ERR_SIEGE_NOFACTION:{
		ACString str;
		str.Format(GetStringFromTable(977), GetStringFromTable(943));
		pGameUI->MessageBox("", str, MB_OK, MSGBOX_COLOR);}
		break;
	case ERR_SIEGE_FACTIONLIMIT:
		GetGameUIMan()->MessageBox("", GetStringFromTable(988), MB_OK, MSGBOX_COLOR);
		break;
	case ERR_BS_NEWBIE_BANNED:{
		ACString str;
		str.Format(GetStringFromTable(963), GetStringFromTable(943));
		pGameUI->MessageBox("", str, MB_OK, MSGBOX_COLOR);}
		break;
	case ERR_SIEGE_CANTENTER_NORMALCITY:
		pGameUI->MessageBox("", GetStringFromTable(964), MB_OK, MSGBOX_COLOR);
		break;
	case ERR_SIEGE_CANTENTER_BATTLECITY:
		pGameUI->MessageBox("", GetStringFromTable(965), MB_OK, MSGBOX_COLOR);
		break;
	default:
		pGameUI->MessageBox("", GetStringFromTable(962), MB_OK, MSGBOX_COLOR);
	}
}

void CDlgSiege::OnWarTopRe(const GNET::SiegeTop_Re* p)
{
	if (!GetHostPlayer()->IsInWar()) {
		assert(0);
		return;
	}

	const CECHostPlayer::WAR_INFO& warInfo = GetHostPlayer()->GetWarInfo();
	const std::vector<SiegeTopRecord>& assistTop = p->assistant_top;

	int fid = 0;
	ACString factionName;
	Faction_Info *pFInfo = NULL;
	PAUIOBJECT pLabA = GetDlgItem("Txt_A"),
		pLabB = GetDlgItem("Txt_B"), pLabC = GetDlgItem("Txt_C");
	// 守城帮派名
	fid = warInfo.iWarDefenceFID;
	factionName = EMPTY_STR;
	pFInfo = GetGame()->GetFactionMan()->GetFaction(fid, true);
	if (pFInfo) {
		factionName = pFInfo->GetName();
		fid = 0;
	}
	pLabA->SetText(factionName); 
	pLabA->SetData(fid);
	// 攻城帮派名
	fid = warInfo.iWarAttackFID;
	factionName = EMPTY_STR;
	pFInfo = GetGame()->GetFactionMan()->GetFaction(fid, true);
	if (pFInfo) {
		factionName = pFInfo->GetName();
		fid = 0;
	}
	pLabB->SetText(factionName); 
	pLabB->SetData(fid);
	// 协助帮派名
	fid = warInfo.iWarAssistFID;
	factionName = EMPTY_STR;
	pFInfo = GetGame()->GetFactionMan()->GetFaction(fid, true);
	if (pFInfo) {
		factionName = pFInfo->GetName();
		fid = 0;
	}
	pLabC->SetText(factionName); 
	pLabC->SetData(fid);

	// 城战结束时间点
	GetDlgItem("Txt_Time")->SetData(warInfo.iEndTime);
	// 剩余时间（分钟）
	int timeLeftMin = (warInfo.iEndTime - GetGame()->GetServerGMTTime() + 59) / 60;
	if (timeLeftMin > 0) {
		ACString strTime;
		strTime.Format(_AL("%d"), timeLeftMin);
		GetDlgItem("Txt_Time")->SetText(strTime);
		GetDlgItem("Txt_Time")->Show(true);
	} else {
		GetDlgItem("Txt_Time")->Show(false);
	}

	// 排名
	PAUILISTBOX pListA = (PAUILISTBOX)GetDlgItem("List_MemberA");
	PAUILISTBOX pListB = (PAUILISTBOX)GetDlgItem("List_MemberB");
	PAUILISTBOX pListC = (PAUILISTBOX)GetDlgItem("List_MemberC");
	pListA->ResetContent();
	pListB->ResetContent();
	pListC->ResetContent();
	size_t i;
	ACString strText;
	int killa = 0, killb = 0, killc = 0;
	for (i = 0; i < p->owner_top.size(); ++i)
	{
		const SiegeTopRecord& record = p->owner_top[i];
		ACString name((const ACHAR*)record.role_name.begin(), (int)record.role_name.size() / sizeof(ACHAR));
		strText.Format(_AL("%s\t%d\t%d"), name, record.kills, record.deaths);
		pListA->AddString(strText);
		pListA->SetItemData(pListA->GetCount() - 1, record.roleid);
		killa += record.kills;
	}
	for (i = 0; i < p->attacker_top.size(); ++i)
	{
		const SiegeTopRecord& record = p->attacker_top[i];
		ACString name((const ACHAR*)record.role_name.begin(), (int)record.role_name.size() / sizeof(ACHAR));
		strText.Format(_AL("%s\t%d\t%d"), name, record.kills, record.deaths);
		pListB->AddString(strText);
		pListB->SetItemData(pListB->GetCount() - 1, record.roleid);
		killb += record.kills;
	}
	for (i = 0; i < p->assistant_top.size(); ++i)
	{
		const SiegeTopRecord& record = p->assistant_top[i];
		ACString name((const ACHAR*)record.role_name.begin(), (int)record.role_name.size() / sizeof(ACHAR));
		strText.Format(_AL("%s\t%d\t%d"), name, record.kills, record.deaths);
		pListC->AddString(strText);
		pListC->SetItemData(pListC->GetCount() - 1, record.roleid);
		killc += record.kills;
	}
	strText.Format(_AL("%d"), killa);
	GetDlgItem("Txt_Akill")->SetText(strText);
	strText.Format(_AL("%d"), killb);
	GetDlgItem("Txt_Bkill")->SetText(strText);
	strText.Format(_AL("%d"), killc);
	GetDlgItem("Txt_Ckill")->SetText(strText);
	Show(true);
}

void CDlgSiege::OnWarEnd(const S2C::war_end* p)
{
	if (p && p->result == 1) { // 1:攻方获胜

	} else if (p && p->result == 2) { // 2:守方获胜

	}

	Sleep(3000); // 延迟3s
	GetGameSession()->war_top(1); // 显示排名
}

void CDlgSiege::OnCommandCancel(const char * szCommand)
{
	Show(false);
	if( stricmp(m_szName, "Win_SiegeInfo") == 0 )
	{
		GetGameUIMan()->EndNPCService();
		GetGameUIMan()->GetDialog("Win_SiegeJoinChoice")->Show(false);
	}
}

void CDlgSiege::OnCommandRefreshSiegeInfo(const char * szCommand)
{
	GetGameSession()->war_infoget(1);
}

void CDlgSiege::OnCommandRefreshSiegeScore(const char * szCommand)
{
	if (stricmp(m_szName, "Win_SiegeScore") == 0)
		GetGameSession()->war_top(1);
}

void CDlgSiege::OnWarInfoGetService()
{
	Show(true);
	GetGameSession()->war_infoget(1);
}

void CDlgSiege::OnWarInfoGetRe(const GNET::SiegeInfoGet_Re* p)
{
	CECGameUIMan* pGameUI = GetGameUIMan();
	CDlgSiege* pDlg1 = (CDlgSiege*)pGameUI->GetDialog("Win_GuildSiege");
	CDlgSiege* pDlg2 = (CDlgSiege*)pGameUI->GetDialog("Win_SiegeInfo");
	if (pDlg1 && pDlg1->IsShow())
		pDlg1->UpdateGuildSiege(p);
	if (pDlg2 && pDlg2->IsShow())
		pDlg2->UpdateSiegeList(p);
}

void CDlgSiege::UpdateGuildSiege(const GNET::SiegeInfoGet_Re* p)
{
	ACString strText;
	size_t i;
	int fid = 0, fid1 = 0;
	ACString str, str1;
	Faction_Info *pFInfo = NULL;
	int hostFaction = GetHostPlayer()->GetFactionID();
	PAUILISTBOX pList = (PAUILISTBOX)GetDlgItem("List_Member");
	pList->ResetContent();
	for (i = 0; i < p->challengers.size(); ++i)
	{
		const GNET::GChallenger& curChall = p->challengers[i];
		// 过滤掉与本帮派无关的条目
		if (p->city.owner.fid != hostFaction &&
			curChall.challenger.fid != hostFaction &&
			curChall.assistant.fid != hostFaction)
			continue;

		time_t beginTime = curChall.begin_time;
		tm *timeTM = gmtime(&beginTime);

		str = str1 = EMPTY_STR;
		fid = curChall.challenger.fid;
		pFInfo = GetGame()->GetFactionMan()->GetFaction(fid, true);
		if (pFInfo) str = pFInfo->GetName();
		fid1 = curChall.assistant.fid;
		pFInfo = GetGame()->GetFactionMan()->GetFaction(fid1, true);
		if (pFInfo) str1 = pFInfo->GetName();

		// 攻城日期 攻方 辅助方 
		ACString strFormat = GetStringFromTable(1612);
		strText.Format(strFormat + ACString(_AL("\t%s\t%s")),
			timeTM->tm_mon+1, timeTM->tm_mday, str, str1);
		pList->AddString(strText);
		int curIndex = pList->GetCount() - 1;
		pList->SetItemData(curIndex, curChall.begin_time, 0);
		pList->SetItemData(curIndex, fid,	1);
		pList->SetItemData(curIndex, fid1,	2);
	}

	// 今日守城方
	PAUILABEL pLab = (PAUILABEL)GetDlgItem("Lab_Name_DefenderGuild");
	str = EMPTY_STR;
	fid = p->city.owner.fid;
	// 帮派名
	pFInfo = GetGame()->GetFactionMan()->GetFaction(fid, true);
	if (pFInfo) {
		str = pFInfo->GetName();
		fid = 0;
	}
	pLab->SetData(fid);
	pLab->SetText(str);
	// 帮主名
	pLab = (PAUILABEL)GetDlgItem("Lab_Name_Defender");
	str1 = EMPTY_STR;
	fid1 = p->city.owner.master;
	const ACHAR* pszName = GetGame()->GetGameRun()->GetPlayerName(fid1, false);
	if (!pszName && fid1 != 0)
		GetGameSession()->GetPlayerBriefInfo(1, &fid1, 2);
	else { // 以后不需更新
		if (pszName) str1 = pszName;
		fid1 = 0;
	}
	pLab->SetData(fid1);
	pLab->SetText(str1);
}

void CDlgSiege::UpdateSiegeList(const GNET::SiegeInfoGet_Re* p)
{
	time_t curTime = GetGame()->GetServerGMTTime();
	tm *curTimeTM = gmtime(&curTime);
	int curYear = curTimeTM->tm_year, curYDay = curTimeTM->tm_yday;

	GNET::GChallenger todayChall;
	todayChall.begin_time = 0;

	size_t i;
	bool bJoined = false; // 是否已经报名城战
	ACString strText;
	const ACHAR* pszName = NULL;
	Faction_Info *pFInfo = NULL;
	int id = 0, id1 = 0;
	ACString str, str1;
	int hostFaction = GetHostPlayer()->GetFactionID();

	PAUILISTBOX pList = (PAUILISTBOX)GetDlgItem("List_Member");
	pList->ResetContent();
	for (i = 0; i < p->challengers.size(); ++i)
	{
		// 报名帮派 帮主 攻城日期
		const GNET::GChallenger& curChall = p->challengers[i];
		time_t beginTime = curChall.begin_time;
		tm *timeTM = gmtime(&beginTime);

		if (curChall.challenger.fid == hostFaction)
			bJoined = true;

		bool bGot[2] = {false, false};
		str = str1 = EMPTY_STR;
		id = curChall.challenger.fid;
		pFInfo = GetGame()->GetFactionMan()->GetFaction(id, true);
		if (pFInfo) {
			str = pFInfo->GetName();
			bGot[0] = true;
		}
		id1 = curChall.challenger.master;
		pszName = GetGame()->GetGameRun()->GetPlayerName(id1, false);
		if (!pszName && id1 != 0)
			GetGameSession()->GetPlayerBriefInfo(1, &id1, 2);
		else { // 以后不需更新
			if (pszName) str1 = pszName;
			bGot[1] = true;
		}
		if (bGot[0] && bGot[1]) { // 两者都非"---"时，第一个id设为-1，以后不用更新了
			id = -1;
		}

		strText.Format(ACString(_AL("%s\t%s\t"))+GetStringFromTable(1612), 
			str, str1, timeTM->tm_mon+1, timeTM->tm_mday);
		
		pList->AddString(strText);
		int curIndex = pList->GetCount() - 1;
		pList->SetItemData(curIndex, id, 0);
		pList->SetItemData(curIndex, id1, 1);
		pList->SetItemData(curIndex, curChall.begin_time, 2);
		
		// 是否今日城战
		if (curYear == timeTM->tm_year && curYDay == timeTM->tm_yday)
			todayChall = curChall;
	}

	PAUILABEL pLab = NULL;
	// 守城方帮派
	pLab = (PAUILABEL)GetDlgItem("Lab_Name_DefenderGuild");
	str = EMPTY_STR;
	id = p->city.owner.fid;
	if (id == hostFaction)
		bJoined = true;
	pFInfo = GetGame()->GetFactionMan()->GetFaction(id, true);
	if (pFInfo) { // 以后不需更新
		str = pFInfo->GetName();
		id = 0;
	}
	pLab->SetData(id);
	pLab->SetText(str);
	// 守城方帮主
	pLab = (PAUILABEL)GetDlgItem("Lab_Name_Defender");
	str = EMPTY_STR;
	id = p->city.owner.master;	
	pszName = GetGame()->GetGameRun()->GetPlayerName(id, false);
	if( !pszName && id != 0)
		GetGameSession()->GetPlayerBriefInfo(1, &id, 2);
	else { // 以后不需更新
		if (pszName) str = pszName;
		id = 0;	
	}
	pLab->SetData(id);
	pLab->SetText(str);

	// 是否可以报名城战
	m_bCanJoin = !bJoined;
	if (m_bCanJoin) {
		int fRoleID = GetHostPlayer()->GetFRoleID();
		if (fRoleID == GNET::_R_MASTER || fRoleID == GNET::_R_VICEMASTER) {
			Faction_Info* pFInfo = GetGame()->GetFactionMan()->GetFaction(
										GetHostPlayer()->GetFactionID(), true);
			if (pFInfo && pFInfo->GetLevel()+1 < FACTION_LEVEL_LIMIT)
				m_bCanJoin = false;
		} else {
			m_bCanJoin = false;
		}
	}

	// 今日攻城方、辅助方
	ACString attackGuildName(EMPTY_STR);
	ACString attackMasterName(EMPTY_STR);
	ACString assistGuildName(EMPTY_STR);
	ACString assistMasterName(EMPTY_STR);
	bool bCanSetAssist = false; // 是否可以指定辅助帮
	if (todayChall.begin_time > 0)
	{
		// 只能在城战当日的0:00-14:00指定辅助攻城方
		curTime -= GetGame()->GetTimeZoneBias() * 60; // localtime = UTC - bias
		tm *localTM = gmtime(&curTime);
		bCanSetAssist = ((localTM->tm_hour < 14) && (todayChall.challenger.fid == hostFaction));

		id = todayChall.challenger.fid;
		pFInfo = GetGame()->GetFactionMan()->GetFaction(id, true);
		if (pFInfo) { // 以后不需更新
			attackGuildName = pFInfo->GetName();
			id = 0;
		}
		GetDlgItem("Lab_Name_AttackerGuild")->SetData(id);
		
		id = todayChall.challenger.master;
		pszName = GetGame()->GetGameRun()->GetPlayerName(id, false);
		if (!pszName && id != 0)
			GetGameSession()->GetPlayerBriefInfo(1, &id, 2);
		else { // 以后不需更新
			if (pszName) attackMasterName = pszName;
			id = 0;
		}
		GetDlgItem("Lab_Name_Attacker")->SetData(id);

		id = todayChall.assistant.fid;
		pFInfo = GetGame()->GetFactionMan()->GetFaction(id, true);
		if (pFInfo) { // 以后不需更新
			assistGuildName = pFInfo->GetName();
			id = 0;
		}
		GetDlgItem("Lab_Name_HelperGuild")->SetData(id);
		
		id = todayChall.assistant.master;
		pszName = GetGame()->GetGameRun()->GetPlayerName(id, false);
		if (!pszName && id != 0)
			GetGameSession()->GetPlayerBriefInfo(1, &id, 2);
		else { // 以后不需更新
			if (pszName) assistMasterName = pszName;
			id = 0;
		}
		GetDlgItem("Lab_Name_Helper")->SetData(id);
	}
	GetDlgItem("Btn_SiegeHelprCh")->Enable(bCanSetAssist);

	GetDlgItem("Lab_Name_AttackerGuild")->SetText(attackGuildName); 
	GetDlgItem("Lab_Name_Attacker")->SetText(attackMasterName); 
	GetDlgItem("Lab_Name_HelperGuild")->SetText(assistGuildName);
	GetDlgItem("Lab_Name_Helper")->SetText(assistMasterName);

	if (!IsShow())
		Show(true);
}

// Tick
void CDlgSiege::OnTick()
{
	time_t curTime = GetGame()->GetServerGMTTime();
	tm *curTimeTM = gmtime(&curTime);

	if( stricmp(m_szName, "Win_SiegeInfo") == 0 )
	{
		GetDlgItem("Btn_Join")->Enable(m_bCanJoin && (GetTickCount()-m_dwLastJoinTime)>60000);

		PAUILABEL pLab = NULL;
		GNET::GChallenger todayChall;
		todayChall.begin_time = 0;
		ACString strText;
		int i;
		PAUILISTBOX pList = (PAUILISTBOX)GetDlgItem("List_Member");
		Faction_Info *pFInfo = NULL;
		for (i = 0; i < pList->GetCount(); ++i)
		{
			if (pList->GetItemData(i, 0) == -1) // 没有"---", 不需更新
				continue;

			ACString factionName(EMPTY_STR), masterName(EMPTY_STR);
			bool bGot[2] = {false, false};
			// 帮派名
			pFInfo = GetGame()->GetFactionMan()->GetFaction(pList->GetItemData(i, 0), false);
			if (pFInfo) {
				factionName = pFInfo->GetName();
				bGot[0] = true;
			}
			// 帮主名
			const ACHAR* master = GetGame()->GetGameRun()->GetPlayerName((int)pList->GetItemData(i, 1), false);
			if (master) {
				masterName = master;
				bGot[1] = true;
			}
			if (bGot[0] && bGot[1]) { // 两者都非"---"时，第一个id设为-1，以后不用更新了
				pList->SetItemData(i, -1, 0);
			}

			time_t beginTime = pList->GetItemData(i, 2);
			tm *timeTM = gmtime(&beginTime);
			
			strText.Format(ACString(_AL("%s\t%s\t"))+GetStringFromTable(1612), 
				factionName, masterName, timeTM->tm_mon+1, timeTM->tm_mday);
			
			pList->SetText(i, strText);
		}
		// 守城方
		int fid = 0, mid = 0;
		pLab = (PAUILABEL)GetDlgItem("Lab_Name_DefenderGuild");
		fid = pLab->GetData();
		if (fid) { 
			pFInfo = GetGame()->GetFactionMan()->GetFaction(fid, false);
			if (pFInfo) { // 以后不需更新
				pLab->SetText(pFInfo->GetName());
				pLab->GetData(0);
			}
		}
		pLab = (PAUILABEL)GetDlgItem("Lab_Name_Defender");		
		mid = pLab->GetData();
		if (mid) {
			const ACHAR* masterName = GetGame()->GetGameRun()->GetPlayerName(mid, false);
			if (masterName) { // 以后不需更新
				pLab->SetText(masterName);
				pLab->GetData(0);
			}
		}
		// 今日攻城方、辅助方
		pLab = (PAUILABEL)GetDlgItem("Lab_Name_AttackerGuild");
		fid = pLab->GetData();
		if (fid) {
			pFInfo = GetGame()->GetFactionMan()->GetFaction(fid, false);
			if (pFInfo) { // 以后不需更新
				pLab->SetText(pFInfo->GetName());
				pLab->SetData(0);
			}
		}
		pLab = (PAUILABEL)GetDlgItem("Lab_Name_Attacker");
		mid = pLab->GetData();
		if (mid) {
			const ACHAR* name = GetGame()->GetGameRun()->GetPlayerName(mid, false);
			if (name) { // 以后不需更新
				pLab->SetText(name);
				pLab->SetData(0);
			}
		}
		pLab = (PAUILABEL)GetDlgItem("Lab_Name_HelperGuild");
		fid = pLab->GetData();
		if (fid) {
			pFInfo = GetGame()->GetFactionMan()->GetFaction(fid, false);
			if (pFInfo) { // 以后不需更新
				pLab->SetText(pFInfo->GetName());
				pLab->SetData(0);
			}
		}
		pLab = (PAUILABEL)GetDlgItem("Lab_Name_Helper");
		mid = pLab->GetData();
		if (mid) {
			const ACHAR* name = GetGame()->GetGameRun()->GetPlayerName(mid, false);
			if (name) { // 以后不需更新
				pLab->SetText(name);
				pLab->SetData(0);
			}
		}
	}
	else if (stricmp(m_szName, "Win_GuildSiege") == 0)
	{
		int hostFaction = GetHostPlayer()->GetFactionID();
		Faction_Info *pFInfo = NULL;
		ACString strText;
		int i;
		PAUILISTBOX pList = (PAUILISTBOX)GetDlgItem("List_Member");
		for (i = 0; i < pList->GetCount(); ++i)
		{
			if (a_strstr(pList->GetText(i), EMPTY_STR) == 0) { // 没有"---", 不需更新
				continue;
			}

			time_t beginTime = pList->GetItemData(i, 0);
			tm *timeTM = gmtime(&beginTime);

			ACString attackGuildName(EMPTY_STR), assistGuildName(EMPTY_STR);
			pFInfo = GetGame()->GetFactionMan()->GetFaction(pList->GetItemData(i, 1), false);
			if (pFInfo) attackGuildName = pFInfo->GetName();
			pFInfo = GetGame()->GetFactionMan()->GetFaction(pList->GetItemData(i, 2), false);
			if (pFInfo) assistGuildName = pFInfo->GetName();

			// 攻城日期 攻方 辅助方 
			ACString strFormat = GetStringFromTable(1612);
			strText.Format(strFormat + ACString(_AL("\t%s\t%s")),
				timeTM->tm_mon+1, timeTM->tm_mday, attackGuildName, assistGuildName);
			pList->SetText(i, strText);
		}

		// 今日守城方
		PAUILABEL pLab = (PAUILABEL)GetDlgItem("Lab_Name_DefenderGuild");
		int fid = pLab->GetData();
		if (fid != 0) {
			pFInfo = GetGame()->GetFactionMan()->GetFaction(fid, false);
			if (pFInfo) {
				pLab->SetText(pFInfo->GetName());
				pLab->SetData(0); // id设为0，以后不用更新了
			}
		}
		pLab = (PAUILABEL)GetDlgItem("Lab_Name_Defender");
		int mid = pLab->GetData();
		if (mid != 0) {
			const ACHAR* masterName = GetGame()->GetGameRun()->GetPlayerName(mid, false);
			if (masterName) {
				pLab->SetText(masterName);
				pLab->SetData(0); // id设为0，以后不用更新了
			}
		}
	}
	else if( stricmp(m_szName, "Win_SiegeScore") == 0 )
	{
		Faction_Info *pFInfo = NULL;
		PAUIOBJECT pLabA = GetDlgItem("Txt_A"),
			pLabB = GetDlgItem("Txt_B"), pLabC = GetDlgItem("Txt_C");
		if (pLabA->GetData() != 0) // 含有"---"
		{ // 守城帮派名
			pFInfo = GetGame()->GetFactionMan()->GetFaction(pLabA->GetData(), false);
			if (pFInfo) {
				pLabA->SetText(pFInfo->GetName());
				pLabA->SetData(0);
			}			
		}
		if (pLabB->GetData() != 0) // 含有"---"
		{ // 攻城帮派名
			pFInfo = GetGame()->GetFactionMan()->GetFaction(pLabB->GetData(), false);
			if (pFInfo) {
				pLabB->SetText(pFInfo->GetName());
				pLabB->SetData(0);
			}
		}
		if (pLabC->GetData() != 0) // 含有"---"
		{ // 协助帮派名
			pFInfo = GetGame()->GetFactionMan()->GetFaction(pLabC->GetData(), false);
			if (pFInfo) {
				pLabC->SetText(pFInfo->GetName());
				pLabC->SetData(0);
			}
		}
		// 剩余时间（分钟）
		PAUIOBJECT pLab = GetDlgItem("Txt_Time");
		int timeLeftMin = (pLab->GetData() - GetGame()->GetServerGMTTime() + 59) / 60;
		if (timeLeftMin > 0) {
			ACString strTime;
			strTime.Format(_AL("%d"), timeLeftMin);
			pLab->SetText(strTime);
			pLab->Show(true);
		} else {
			pLab->Show(false);
		}
	}
}