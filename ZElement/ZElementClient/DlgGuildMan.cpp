// Filename	: DlgGuildMan.cpp
// Creator	: Tom Zhou
// Date		: October 25, 2005

#include "DlgGuildMan.h"
#include "DlgInputName.h"
#include "DlgProclaim.h"
#include "DlgSkill.h"
#include "EC_Skill.h"
#include "EC_Game.h"
#include "EC_GameUIMan.h"
#include "EC_Faction.h"
#include "EC_HostPlayer.h"
#include "EC_GameSession.h"
#include "EC_GameRun.h"
#include "AUI\\CSplit.h"
#include "AUI\\AUIEditBox.h"
#include "Network\\ids.hxx"
#include "combattop_re.hpp"
#include "combatstatus.hpp"
#include "gt_overlay.h"

#define new A_DEBUG_NEW

#define PAGE_MAX 20

const int familyskillability[] =
{
	0,
	100,
	400,
	1000,
	2000,
	4000,
	6000,
	9000,
	12000,
	17000,
	22000,
	29000,
	37000,
	50000,
	72000,
	INT_MAX,
};

const int honorlevel[] =
{
	0,
	5000,
	20000,
	60000,
	120000,
	300000,
	INT_MAX
};

const int terrscorelevel[] =
{
	0,
	10,
	20,
	40,
	70,
	100,
	130,
	160,
	190,
	230,
	270,
	310,
	350,
	390,
	760,
	INT_MAX
};

AUI_BEGIN_COMMAND_MAP(CDlgGuildMan, CDlgBase)

AUI_ON_COMMAND("recover",		OnCommand_recover)
AUI_ON_COMMAND("gappoint",		OnCommand_gappoint)
AUI_ON_COMMAND("confirm",		OnCommand_confirm)
AUI_ON_COMMAND("IDCANCEL",		OnCommand_CANCEL)
AUI_ON_COMMAND("sortname",		OnCommand_sortname)
AUI_ON_COMMAND("sortjob",		OnCommand_sortjob)
AUI_ON_COMMAND("sorttitle",		OnCommand_sorttitle)
AUI_ON_COMMAND("sortlevel",		OnCommand_sortlevel)
AUI_ON_COMMAND("sortprof",		OnCommand_sortprof)
AUI_ON_COMMAND("sortol",		OnCommand_sortol)
AUI_ON_COMMAND("sortcon",		OnCommand_sortcon)
AUI_ON_COMMAND("sortdov",		OnCommand_sortdov)
AUI_ON_COMMAND("sortfamily",	OnCommand_sortfamily)
AUI_ON_COMMAND("Btn_Camp",		OnCommand_sortcamp)
AUI_ON_COMMAND("Edit",			OnCommand_Edit)
AUI_ON_COMMAND("gquit",			OnCommand_gquit)
AUI_ON_COMMAND("gpass",			OnCommand_gpass)
AUI_ON_COMMAND("gdemiss",		OnCommand_gdemiss)
AUI_ON_COMMAND("gdemote",		OnCommand_gdemote)
AUI_ON_COMMAND("gkick",			OnCommand_gkick)
AUI_ON_COMMAND("gtitle",		OnCommand_gtitle)
AUI_ON_COMMAND("ginvite",		OnCommand_ginvite)
AUI_ON_COMMAND("genemy",		OnCommand_genemy)
AUI_ON_COMMAND("refresh*",		OnCommand_refresh)
AUI_ON_COMMAND("gwar",			OnCommand_gwar)
AUI_ON_COMMAND("gsiegeinfo",	OnCommand_gsiegeinfo)
AUI_ON_COMMAND("showfaction",	OnCommand_showfaction)
AUI_ON_COMMAND("showfamily",	OnCommand_showfamily)
AUI_ON_COMMAND("help",			OnCommand_help)
//AUI_ON_COMMAND("Btn_GetMoney",  OnCommand_getmoney)
AUI_ON_COMMAND("Btn_Mic",       OnCommandGTEnterFaction)
AUI_ON_COMMAND("Btn_GuildMoney",       OnCommand_money)
AUI_END_COMMAND_MAP()

AUI_BEGIN_EVENT_MAP(CDlgGuildMan, CDlgBase)

AUI_ON_EVENT("List_Member",	WM_RBUTTONUP,	OnEventRButtonUp)
AUI_ON_EVENT("List_Family",	WM_LBUTTONUP,	OnEventLButtonUp)
AUI_ON_EVENT("Img_Skill*",	WM_LBUTTONDOWN,	OnEventLButtonDownSkill)

AUI_END_EVENT_MAP()

CDlgGuildMan::CDlgGuildMan()
{
	m_dwLastRefreshTime = 0;
	m_nSortMode = SORT_BY_ORDER;
	m_bFirstOpen = true;
	m_idCurFamily = 0;
}

CDlgGuildMan::~CDlgGuildMan()
{
}

void CDlgGuildMan::SetLastRefreshTime(DWORD dwTime)
{
	m_dwLastRefreshTime = dwTime;
}

void CDlgGuildMan::OnCommand_gappoint(const char * szCommand)
{
	int nCurSel = m_pList_Member->GetCurSel();
	if( nCurSel < 0 || nCurSel >= m_pList_Member->GetCount() ) return;

	CSplit s(m_pList_Member->GetText(nCurSel));
	CSPLIT_STRING_VECTOR vec = s.Split(_AL("\t"));

	PAUIDIALOG pDlg = GetGameUIMan()->GetDialog("Win_GuildAppoint");
	pDlg->SetData(m_pList_Member->GetItemData(nCurSel));
	pDlg->GetDlgItem("Txt_Name")->SetText(vec[0]);
	pDlg->GetDlgItem("Txt_CurPosition")->SetText(vec[1]);
	int idFRole = GetHostPlayer()->GetFRoleID();
	PAUILISTBOX pList = (PAUILISTBOX)pDlg->GetDlgItem("Lst_Position");
	int i;
	pList->ResetContent();
	for(i = 1301 + idFRole; i < 1306; i++)
	{
		ACString szPosition = GetStringFromTable(i);
		if( szPosition == _AL("") )
			break;

		pList->AddString(szPosition);
		pList->SetItemData(pList->GetCount() - 1, i - 1300);
	}
	pDlg->Show(true, true);
}

void CDlgGuildMan::OnCommand_recover(const char * szCommand)
{
	CECFactionMan *pFMan = GetGame()->GetFactionMan();
	int nDeleteTime = 0;
	if( stricmp(m_szName, "Win_GuildManage") == 0 )
	{
		if( pFMan->GetDeleteTime() != 0 )
		{
			GetGameUIMan()->MessageBox("Faction_Recover", GetStringFromTable(867),
					MB_OKCANCEL, A3DCOLORRGBA(255, 255, 255, 160));
		}
	}
	else
	{
		if( pFMan->GetFamilyDeleteTime() != 0 )
		{
			GetGameUIMan()->MessageBox("Family_Recover", GetStringFromTable(885),
					MB_OKCANCEL, A3DCOLORRGBA(255, 255, 255, 160));
		}
	}
}

void CDlgGuildMan::OnCommand_confirm(const char * szCommand)
{
	PAUILISTBOX pList = (PAUILISTBOX)GetDlgItem("Lst_Position");
	int nCurSel = pList->GetCurSel();
	if( nCurSel < 0 || nCurSel >= pList->GetCount() ) return;
	GetGameSession()->faction_appoint(GetData(), (char)pList->GetItemData(nCurSel));
}

void CDlgGuildMan::OnEventLButtonUp(WPARAM wParam, LPARAM lParam, AUIObject * pObj)
{
	if( m_pList_Family->GetCurSel() < m_pList_Family->GetCount() )
	{
		int idCurFamily = m_pList_Family->GetItemData(m_pList_Family->GetCurSel());
		if( idCurFamily != m_idCurFamily )
		{
			m_idCurFamily = idCurFamily;
			GetGame()->GetFactionMan()->RefreshMemList(0, m_idCurFamily);
		}
	}
}

void CDlgGuildMan::OnEventRButtonUp(WPARAM wParam, LPARAM lParam, AUIObject * pObj)
{
	POINT ptPos = pObj->GetPos();
	int nCurSel = m_pList_Member->GetCurSel();
	A3DVIEWPORTPARAM *p = m_pA3DEngine->GetActiveViewport()->GetParam();
	int x = GET_X_LPARAM(lParam) - ptPos.x - p->X;
	int y = GET_Y_LPARAM(lParam) - ptPos.y - p->Y;

	if( nCurSel >= 0 && nCurSel < m_pList_Member->GetCount() &&
		m_pList_Member->GetHitArea(x, y) == AUILISTBOX_RECT_TEXT )
	{
		int idPlayer = m_pList_Member->GetItemData(nCurSel);

		x = GET_X_LPARAM(lParam) - p->X;
		y = GET_Y_LPARAM(lParam) - p->Y;
		GetGameUIMan()->PopupPlayerContextMenu(idPlayer, x, y);
	}
}

bool CDlgGuildMan::OnInitDialog()
{
	m_pList_Member = (PAUILISTBOX)GetDlgItem("List_Member");
	m_pList_Family = (PAUILISTBOX)GetDlgItem("List_Family");
	//m_pTxt_GuildName = (PAUILABEL)GetDlgItem("Txt_GuildName");
	//m_pTxt_GuildLevel = (PAUILABEL)GetDlgItem("Txt_GuildLevel");
	//m_pTxt_GuildProsperity = (PAUILABEL)GetDlgItem("Txt_GuildProsperity");
	//m_pTxt_GuildHonor = (PAUILABEL)GetDlgItem("Txt_GuildHonor");
	//m_pPro_GuildHonor= (PAUIPROGRESS)GetDlgItem("Pro_GuildHonor");
	//m_pTxt_GuildTerrScore = (PAUILABEL)GetDlgItem("Txt_LandOccupy");
	//m_pPro_GuildTerrScore= (PAUIPROGRESS)GetDlgItem("Pro_LandOccupy");

	m_pTxt_DeleteTime = (PAUILABEL)GetDlgItem("Txt_DeleteTime");
	//m_pTxt_Message = (PAUITEXTAREA)GetDlgItem("Txt_Message");
	m_pBtn_Recover = (PAUISTILLIMAGEBUTTON)GetDlgItem("Btn_Recover");
	m_pBtn_GuildPass = (PAUISTILLIMAGEBUTTON)GetDlgItem("Btn_GuildPass");
	m_pBtn_GuildDemission = (PAUISTILLIMAGEBUTTON)GetDlgItem("Btn_GuildDemission");
	m_pBtn_GuildAppoint = (PAUISTILLIMAGEBUTTON)GetDlgItem("Btn_GuildAppoint");
	m_pBtn_GuildKick = (PAUISTILLIMAGEBUTTON)GetDlgItem("Btn_GuildKick");
	m_pBtn_GuildTitle = (PAUISTILLIMAGEBUTTON)GetDlgItem("Btn_GuildTitle");
	m_pBtn_GuildQuit = (PAUISTILLIMAGEBUTTON)GetDlgItem("Btn_GuildQuit");
	m_pBtn_GuildInvite = (PAUISTILLIMAGEBUTTON)GetDlgItem("Btn_GuildInvite");
	//m_pBtn_Edit = (PAUISTILLIMAGEBUTTON)GetDlgItem("Btn_Edit");
	m_pBtn_Refresh = (PAUISTILLIMAGEBUTTON)GetDlgItem("Btn_Refresh");
	//m_pBtn_ShowFaction = (PAUISTILLIMAGEBUTTON)GetDlgItem("Btn_ShowFaction");
	//m_pBtn_ShowFamily = (PAUISTILLIMAGEBUTTON)GetDlgItem("Btn_ShowFamily");
	//m_pImg_GuildIcon = (PAUIIMAGEPICTURE)GetDlgItem("Img_GuildIcon");

	//if( m_pTxt_GuildName )
	//	m_pTxt_GuildName->SetAlign(AUIFRAME_ALIGN_CENTER);

	//if( m_pTxt_GuildHonor )
	//	m_pTxt_GuildHonor->SetAlign(AUIFRAME_ALIGN_CENTER);
	
	if( stricmp(m_szName, "Win_FamilyManage") == 0 )
	{
		char szName[20];
		for(int i = 0; i < CDLGGUILDMAN_FAMILYSKILL_MAX; i++)
		{
			sprintf(szName, "Img_Skill%02d", i + 1);
			m_pImg_Skill[i] = (PAUIIMAGEPICTURE)GetDlgItem(szName);
			sprintf(szName, "Lab_Lev_Skill%02d", i + 1);
			m_pLab_Lev_Skill[i] = (PAUILABEL)GetDlgItem(szName);
			m_pLab_Lev_Skill[i]->SetAlign(AUIFRAME_ALIGN_CENTER);
			sprintf(szName, "Lab_Exp_Skill%02d", i + 1);
			m_pLab_Exp_Skill[i] = (PAUILABEL)GetDlgItem(szName);
			m_pLab_Exp_Skill[i]->SetAlign(AUIFRAME_ALIGN_CENTER);
		}
		m_pTxt_PplCap = (PAUILABEL)GetDlgItem("Txt_PplCap");
		m_pTxt_PplOnline = (PAUILABEL)GetDlgItem("Txt_PplOnline");
	}
	
	return true;
}

void CDlgGuildMan::OnCommand_CANCEL(const char * szCommand)
{
	if( m_szName == "Win_GuildWarStatus" && m_nGuildWarTime != 0 )
		return;
	
	Show(false);
}

void CDlgGuildMan::OnCommand_sortname(const char * szCommand)
{
	m_nSortMode = SORT_BY_NAME;
	BuildFactionList();
}

void CDlgGuildMan::OnCommand_sortjob(const char * szCommand)
{
	m_nSortMode = SORT_BY_JOB;
	BuildFactionList();
}

void CDlgGuildMan::OnCommand_sorttitle(const char * szCommand)
{
	m_nSortMode = SORT_BY_TITLE;
	BuildFactionList();
}

void CDlgGuildMan::OnCommand_sortlevel(const char * szCommand)
{
	m_nSortMode = SORT_BY_LEVEL;
	BuildFactionList();
}

void CDlgGuildMan::OnCommand_sortprof(const char * szCommand)
{
	m_nSortMode = SORT_BY_PROFESSION;
	BuildFactionList();
}

void CDlgGuildMan::OnCommand_sortol(const char * szCommand)
{
	m_nSortMode = SORT_BY_ONLINE;
	BuildFactionList();
}

void CDlgGuildMan::OnCommand_sortcon(const char * szCommand)
{
	m_nSortMode = SORT_BY_CONTRIBUTION;
	BuildFactionList();
}

void CDlgGuildMan::OnCommand_sortdov(const char * szCommand)
{
	m_nSortMode = SORT_BY_DEVOTION;
	BuildFactionList();
}

void CDlgGuildMan::OnCommand_sortfamily(const char * szCommand)
{
	m_nSortMode = SORT_BY_FAMILY;
	BuildFactionList();
}

void CDlgGuildMan::OnCommand_sortcamp(const char * szCommand)
{
	m_nSortMode = SORT_BY_CAMP;
	BuildFactionList();
}

void CDlgGuildMan::OnCommand_money(const char* szCommand)
{
	m_nSortMode = SORT_BY_MONEY;
	BuildFactionList();
}

void CDlgGuildMan::OnCommand_Edit(const char * szCommand)
{
	ACString strText = GetDlgItem("Txt_Message")->GetText();
	GetGameUIMan()->m_pDlgProclaim->Show(true);
	GetGameUIMan()->m_pDlgProclaim->m_pDEFAULT_Txt_Input->SetText(strText);
	GetGameUIMan()->m_pDlgProclaim->SetData(1);
}

void CDlgGuildMan::OnCommand_gquit(const char * szCommand)
{
	if( stricmp(m_szName, "Win_GuildManage") == 0 )
	{
		GetGameUIMan()->MessageBox("Faction_Quit", GetStringFromTable(590),
			MB_OKCANCEL, A3DCOLORRGBA(255, 255, 255, 160));
	}
	else
	{
		GetGameUIMan()->MessageBox("Family_Quit", GetStringFromTable(855),
			MB_OKCANCEL, A3DCOLORRGBA(255, 255, 255, 160));
	}
}

void CDlgGuildMan::OnCommand_gpass(const char * szCommand)
{
	int nCurSel = m_pList_Member->GetCurSel();
	if( nCurSel < 0 || nCurSel >= m_pList_Member->GetCount() ) return;

	CSplit s(m_pList_Member->GetText(nCurSel));
	CSPLIT_STRING_VECTOR vec = s.Split(_AL("\t"));

	ACString strMsg;
	PAUIDIALOG pMsgBox;
	if( stricmp(m_szName, "Win_GuildManage") == 0 )
	{
		strMsg.Format(GetStringFromTable(584), vec[0]);
		GetGameUIMan()->MessageBox("Faction_Pass", strMsg,
			MB_OKCANCEL, A3DCOLORRGBA(255, 255, 255, 160), &pMsgBox);
		pMsgBox->SetData(m_pList_Member->GetItemData(nCurSel));
	}
	else
	{
		if( GetHostPlayer()->GetFRoleID() < GNET::_R_HEADER )
			GetGameUIMan()->MessageBox("", GetStringFromTable(869), MB_OK, A3DCOLORRGBA(255, 255, 255, 160));
		else
		{
			strMsg.Format(GetStringFromTable(856), vec[0]);
			GetGameUIMan()->MessageBox("Family_Pass", strMsg,
				MB_OKCANCEL, A3DCOLORRGBA(255, 255, 255, 160), &pMsgBox);
			pMsgBox->SetData(m_pList_Member->GetItemData(nCurSel));
		}
	}
}

void CDlgGuildMan::OnCommand_gdemiss(const char * szCommand)
{
	GetGameUIMan()->MessageBox("Faction_Demiss", GetStringFromTable(585),
		MB_OKCANCEL, A3DCOLORRGBA(255, 255, 255, 160));
}

void CDlgGuildMan::OnCommand_gdemote(const char * szCommand)
{
	int nCurSel = m_pList_Member->GetCurSel();
	if( nCurSel < 0 || nCurSel >= m_pList_Member->GetCount() ) return;

	CSplit s(m_pList_Member->GetText(nCurSel));
	CSPLIT_STRING_VECTOR vec = s.Split(_AL("\t"));

	ACString strMsg;
	PAUIDIALOG pMsgBox;
	strMsg.Format(GetStringFromTable(587), vec[0]);
	GetGameUIMan()->MessageBox("Faction_Demote", strMsg,
		MB_OKCANCEL, A3DCOLORRGBA(255, 255, 255, 160), &pMsgBox);
	pMsgBox->SetData(m_pList_Member->GetItemData(nCurSel));
}

void CDlgGuildMan::OnCommand_gkick(const char * szCommand)
{
	ACString strMsg;
	PAUIDIALOG pMsgBox;
	if( stricmp(m_szName, "Win_GuildManage") == 0 )
	{
		int nCurSel = m_pList_Family->GetCurSel();
		if( nCurSel <= 0 || nCurSel >= m_pList_Family->GetCount() ) return;

		strMsg.Format(GetStringFromTable(588), m_pList_Family->GetText(m_pList_Family->GetCurSel()));
		GetGameUIMan()->MessageBox("Faction_Kick", strMsg,
			MB_OKCANCEL, A3DCOLORRGBA(255, 255, 255, 160), &pMsgBox);
		pMsgBox->SetData(m_pList_Family->GetItemData(nCurSel));
	}
	else
	{
		int nCurSel = m_pList_Member->GetCurSel();
		if( nCurSel < 0 || nCurSel >= m_pList_Member->GetCount() ) return;

		CSplit s(m_pList_Member->GetText(nCurSel));
		CSPLIT_STRING_VECTOR vec = s.Split(_AL("\t"));

		strMsg.Format(GetStringFromTable(857), vec[0]);
		GetGameUIMan()->MessageBox("Family_Kick", strMsg,
			MB_OKCANCEL, A3DCOLORRGBA(255, 255, 255, 160), &pMsgBox);
		pMsgBox->SetData(m_pList_Member->GetItemData(nCurSel));
	}
}

void CDlgGuildMan::OnCommand_gtitle(const char * szCommand)
{
	int nCurSel = m_pList_Member->GetCurSel();
	if( nCurSel < 0 || nCurSel >= m_pList_Member->GetCount() ) return;

	GetGameUIMan()->m_pDlgInputName->Show(true, true);
	GetGameUIMan()->m_pDlgInputName->m_pTxtInput->SetMaxLength(4);
	GetGameUIMan()->m_pDlgInputName->m_pTxtInput->SetText(_AL(""));
	GetGameUIMan()->m_pDlgInputName->SetData(CDlgInputName::INPUTNAME_FACTION_TITLE);
	GetGameUIMan()->m_pDlgInputName->SetDataPtr(
		(void *)m_pList_Member->GetItemData(nCurSel),"int");
}

void CDlgGuildMan::OnCommand_ginvite(const char * szCommand)
{
	CDlgInputName *pDlg;
	if( stricmp(m_szName, "Win_GuildManage") == 0 )
	{
		pDlg = (CDlgInputName*)GetGameUIMan()->GetDialog("Win_InputString10");
		pDlg->SetData(CDlgInputName::INPUTNAME_ADD_GUILD);
	}
	else
	{
		pDlg = GetGameUIMan()->m_pDlgInputName;
		pDlg->SetData(CDlgInputName::INPUTNAME_ADD_FAMILY);
	}
	pDlg->Show(true, true);
	pDlg->m_pTxtInput->SetText(_AL(""));
}

void CDlgGuildMan::OnCommand_genemy(const char * szCommand)
{
	GetGameUIMan()->GetDialog("Win_GuildEnemies")->Show(true);
	GetGameUIMan()->GetDialog("Win_GuildEnemies")->OnCommand("refresh");
}

void CDlgGuildMan::OnCommand_refresh(const char * szCommand)
{
	if (GetHostPlayer()->GetFactionID() == 0)
		return;

	int nCode = atoi(szCommand + strlen("refresh"));

	if( m_szName == "Win_GuildManage" )
	{
		if( m_idCurFamily == 0 )
		{
			m_dwLastRefreshTime = GetTickCount();
			if( nCode == -3 )
				GetGame()->GetFactionMan()->RefreshMemList(-1);
			else if( nCode == -2 )
				GetGame()->GetFactionMan()->RefreshMemList(0);
			else if( nCode == 2 )
			{
				Faction_Info *pinfo = GetGame()->GetFactionMan()->GetFaction(GetHostPlayer()->GetFactionID());
				if( pinfo )
					GetGame()->GetFactionMan()->RefreshMemList(999);
			}
			else
			{
				int nPage = GetGame()->GetFactionMan()->GetCurPage();
				nPage += nCode;
				a_ClampFloor(nCode, 0);
				Faction_Info *pinfo = GetGame()->GetFactionMan()->GetFaction(GetHostPlayer()->GetFactionID());
				if( pinfo )
					a_ClampRoof(nCode, 999);
				GetGame()->GetFactionMan()->RefreshMemList(nPage);
			}
		}
	}
	else if( m_szName == "Win_FamilyManage" )
	{
		GetGame()->GetFactionMan()->RefreshMemList(0, GetHostPlayer()->GetFamilyID());
	}
	else if( m_szName == "Win_GuildWar" )
	{
		if( nCode == -2 )
			m_nGuildWarPage = 0;
		else if( nCode == 2 )
			m_nGuildWarPage = -1;
		else
		{
			m_nGuildWarPage += nCode;
			if( m_nGuildWarPage < 0 )
				m_nGuildWarPage = 0;
		}
		GetGameSession()->combat_Top(GetHostPlayer()->GetCharacterID(),
			GetHostPlayer()->GetFactionID(), m_nGuildWarPage);
	}
	else if( m_szName == "Win_GuildEnemies" )
	{
	}
}

void CDlgGuildMan::OnCommand_gwar(const char * szCommand)
{
	PAUIDIALOG pDlg = GetGameUIMan()->GetDialog("Win_GuildWar");
	if( pDlg )
	{
		PAUILISTBOX pList = (PAUILISTBOX)pDlg->GetDlgItem("List_Member");
		pList->ResetContent();
		pDlg->Show(true);
		m_nGuildWarPage = 0;
		GetGameSession()->combat_Top(GetHostPlayer()->GetCharacterID(),
			GetHostPlayer()->GetFactionID(), 0);
	}
}

void CDlgGuildMan::OnCommand_gsiegeinfo(const char * szCommand)
{
	GetGameUIMan()->GetDialog("Win_GuildSiege")->Show(true);
	GetGameSession()->war_infoget(1);
}

void CDlgGuildMan::OnCommand_showfaction(const char * szCommand)
{
	if( GetHostPlayer()->GetFactionID() && stricmp(m_szName, "Win_GuildManage") != 0  )
	{
		CDlgGuildMan *pDlg = GetGameUIMan()->m_pDlgGuildMan;
		pDlg->Show(true);
		pDlg->SetPosEx(GetPos().x, GetPos().y);
		if( pDlg->IsFirstOpen() || (int)pDlg->GetDlgItem("List_Family")->GetData() != GetHostPlayer()->GetFactionID() )
		{
			GetGame()->GetFactionMan()->RefreshMemList(0, 0, true);
			pDlg->SetFirstOpen(false);
		}
		else
			GetGame()->GetFactionMan()->RefreshMemList(0, 0, false);
		pDlg->m_idCurFamily = 0;
		pDlg->m_pList_Family->SetCurSel(0);
		Show(false);
	}
}

void CDlgGuildMan::OnCommand_showfamily(const char * szCommand)
{
	if( GetHostPlayer()->GetFamilyID() && stricmp(m_szName, "Win_FamilyManage") != 0 )
	{
		CDlgGuildMan *pDlg = GetGameUIMan()->m_pDlgFamilyMan;
		pDlg->Show(true);
		pDlg->SetPosEx(GetPos().x, GetPos().y);
		if( pDlg->IsFirstOpen() )
		{
			GetGame()->GetFactionMan()->RefreshMemList(0, GetHostPlayer()->GetFamilyID(), true);
			pDlg->SetFirstOpen(false);
		}
		else
			GetGame()->GetFactionMan()->RefreshMemList(0, GetHostPlayer()->GetFamilyID(), false);
		Show(false);
	}
}

void CDlgGuildMan::OnCommand_help(const char * szCommand)
{
	/*PAUIDIALOG pDlg;
	if( stricmp(m_szName, "Win_GuildManage") == 0 )
		pDlg = GetGameUIMan()->GetDialog("Win_Guild_Help");
	else
		pDlg = GetGameUIMan()->GetDialog("Win_Family_Help");
	pDlg->Show(!pDlg->IsShow());*/
	AString strDlg;
	if( stricmp(m_szName, "Win_GuildManage") == 0 )
	     strDlg = "Win_Guild_Help";
	else
		strDlg = "Win_Family_Help";
	GetGameUIMan()->CommonMessage("Win_HelpInfo","open",(WPARAM)(&strDlg),0);
}

void CDlgGuildMan::OnCommand_getmoney(const char* szCommand)
{
    if( stricmp(m_szName, "Win_GuildManage") == 0 )
	{
		Faction_Info *pFactionInfo= NULL;
		pFactionInfo = GetGame()->GetFactionMan()->GetFaction(GetHostPlayer()->GetFactionID());
		if (NULL != pFactionInfo && pFactionInfo->GetMoney() > 0)
		{
	       GetGameSession()->factionpkraid_withdrawbonus(GetHostPlayer()->GetCharacterID());
		}
	}
}

void CDlgGuildMan::OnCommandGTEnterFaction(const char* szCommand)
{
	if (!GetGameRun()->IsGTLogin())
	{
		GetGame()->GetGameSession()->get_platform_token("gt", 24, 2400);
	}
	
	overlay::GTOverlay::Instance().EnterFaction();
}

void CDlgGuildMan::OnTick()
{
	if( stricmp(m_szName, "Win_GuildEnemies") == 0 )
		return;

	if( stricmp(m_szName, "Win_GuildManage") == 0 && GetHostPlayer()->GetFactionID() <= 0 ||
		stricmp(m_szName, "Win_FamilyManage") == 0 && GetHostPlayer()->GetFamilyID() <= 0)
	{
		Show(false);
		return;
	}

	if( stricmp(m_szName, "Win_GuildWarStatus") == 0 )
	{
		DWORD dwTime = GetTickCount();
		m_nGuildWarTime -= dwTime - m_dwLastRefreshTime;
		m_dwLastRefreshTime = dwTime;
		if( m_nGuildWarTime < 0 )
			m_nGuildWarTime = 0;
		ACString strText;
		if( m_nGuildWarTime >= 60000 )
			strText.Format(GetStringFromTable(1602), m_nGuildWarTime / 60000);
		else
			strText.Format(GetStringFromTable(1601), m_nGuildWarTime / 1000);
		GetDlgItem("Txt_LastTime")->SetAlign(AUIFRAME_ALIGN_RIGHT);
		GetDlgItem("Txt_LastTime")->SetText(strText);
		//GetDlgItem("Btn_Close")->Show(m_nGuildWarTime == 0);
		if( GetHostPlayer()->GetCombatFaction() )
		{
			Faction_Info *finfo = GetGame()->GetFactionMan()->GetFaction(GetHostPlayer()->GetCombatFaction());
			if( finfo )
			{
				ACHAR szFName[20];
				AUI_ConvertChatString(finfo->GetName(), szFName);
				GetDlgItem("Txt_EGName")->SetText(szFName);
			}
		}
	}

	if( stricmp(m_szName, "Win_GuildManage") != 0 && stricmp(m_szName, "Win_FamilyManage") != 0 )
		return;

	bool bFaction = true;
	if( stricmp(m_szName, "Win_GuildManage") != 0 )
		bFaction = false;
	
	//m_pBtn_ShowFaction->Show(GetHostPlayer()->GetFactionID() != 0);
	//m_pBtn_ShowFamily->Show(GetHostPlayer()->GetFactionID() != 0);
	//m_pBtn_ShowFaction->SetPushed(stricmp(m_szName, "Win_GuildManage") == 0);
	//m_pBtn_ShowFamily->SetPushed(stricmp(m_szName, "Win_FamilyManage") == 0);

	CECFactionMan *pFMan = GetGame()->GetFactionMan();
	ACString strText;
	int nCurSel = m_pList_Member->GetCurSel();
	int idFRole = GetHostPlayer()->GetFRoleID();
	int idPlayer = -1, idFRoleU = GNET::_R_MASTER;

	if( nCurSel >= 0 && nCurSel < m_pList_Member->GetCount() )
	{
		idPlayer = m_pList_Member->GetItemData(nCurSel);
		idFRoleU = m_pList_Member->GetItemData(nCurSel, 1);
	}

	if( idPlayer <= 0 || idPlayer == GetHostPlayer()->GetCharacterID() )
		m_pBtn_GuildPass->Enable(false);
	else
	{
		if( bFaction && idFRole == GNET::_R_MASTER && idFRoleU == GNET::_R_VICEMASTER ||
			!bFaction && idFRole <= GNET::_R_HEADER && idFRoleU == GNET::_R_MEMBER )
			m_pBtn_GuildPass->Enable(true);
		else
			m_pBtn_GuildPass->Enable(false);
	}

	if( bFaction )
	{
		if( idFRole >= GNET::_R_HEADER || m_pList_Family->GetCurSel() <= 0 || m_pList_Family->GetCurSel() >= m_pList_Family->GetCount() )
			m_pBtn_GuildKick->Enable(false);
		else
		{
			FactionMemList &fml = GetGame()->GetFactionMan()->GetMemList();

			// 空家族可以踢出
			if (fml.size()==0)
				m_pBtn_GuildKick->Enable(true);

			for(DWORD i = 0; i < fml.size(); i++)
				if( fml[i]->GetFRoleID() <= GNET::_R_HEADER )
				{
					if( fml[i]->GetFRoleID() == GNET::_R_HEADER )
						m_pBtn_GuildKick->Enable(true);
					else
						m_pBtn_GuildKick->Enable(false);
					break;
				}
		}

		//领奖金
		{
			 //if(GetHostPlayer()->GetFRoleID() == GNET::_R_MASTER)
			 // GetDlgItem("Btn_GetMoney")->Enable(true);
		  //  else
		  //	  GetDlgItem("Btn_GetMoney")->Enable(false);
		}
	}
	else if( idFRole <= GNET::_R_HEADER && idFRoleU == GNET::_R_MEMBER )
		m_pBtn_GuildKick->Enable(true);
	else
		m_pBtn_GuildKick->Enable(false);
	
	if( bFaction && idFRole <= GNET::_R_BODYGUARD ||
		!bFaction && idFRole <= GNET::_R_HEADER )
		m_pBtn_GuildInvite->Enable(true);
	else
		m_pBtn_GuildInvite->Enable(false);

	if( !bFaction && idFRole == GNET::_R_MEMBER ||
		bFaction && idFRole == GNET::_R_HEADER )
		m_pBtn_GuildQuit->Enable(true);
	else
		m_pBtn_GuildQuit->Enable(false);

	//if( bFaction && (idFRole == GNET::_R_MASTER || idFRole == GNET::_R_VICEMASTER) ||
	//	!bFaction && idFRole <= GNET::_R_HEADER )
	//	m_pBtn_Edit->Enable(true);
	//else
	//	m_pBtn_Edit->Enable(false);

	if( bFaction )
	{
		if( idPlayer < 0 )
			m_pBtn_GuildTitle->Enable(false);
		else
		{
			if( idFRole == GNET::_R_MASTER || idFRole == GNET::_R_VICEMASTER )
				m_pBtn_GuildTitle->Enable(true);
			else
				m_pBtn_GuildTitle->Enable(false);
		}

		if( idFRole == GNET::_R_VICEMASTER || idFRole == GNET::_R_BODYGUARD )
			m_pBtn_GuildDemission->Enable(true);
		else
			m_pBtn_GuildDemission->Enable(false);

		if( idPlayer <= 0 || idPlayer == GetHostPlayer()->GetCharacterID() )
			m_pBtn_GuildAppoint->Enable(false);
		else
		{
			if( idFRoleU - idFRole >= 1 && idFRoleU != GNET::_R_MEMBER &&
				(idFRole == GNET::_R_MASTER ||
				idFRole == GNET::_R_VICEMASTER) )
			{
				m_pBtn_GuildAppoint->Enable(true);
			}
			else
			{
				m_pBtn_GuildAppoint->Enable(false);
			}
		}
	}
	int nDeleteTime = 0;
	if( bFaction )
	{
		nDeleteTime = pFMan->GetDeleteTime();
		m_pBtn_Recover->Show(nDeleteTime != 0 && idFRole == GNET::_R_MASTER);
	}
	else
	{
		nDeleteTime = pFMan->GetFamilyDeleteTime();
		m_pBtn_Recover->Show(nDeleteTime != 0 && idFRole <= GNET::_R_HEADER);
	}
	if( nDeleteTime == 0 )
		m_pTxt_DeleteTime->Show(false);
	else
	{
		ACString strTime;
		int nRestTime = (nDeleteTime - GetGame()->GetServerGMTTime()) / 60 + 1;
		if( nRestTime >= 60 * 24 )
			strTime.Format(GetStringFromTable(1604), nRestTime / (60 * 24));
		else if( nRestTime >= 60 )
			strTime.Format(GetStringFromTable(1611), nRestTime / 60, nRestTime % 60);
		else
			strTime.Format(GetStringFromTable(1602), nRestTime);
		if( bFaction )
			strText.Format(GetStringFromTable(868), strTime);
		else
			strText.Format(GetStringFromTable(884), strTime);
		m_pTxt_DeleteTime->Show(true);
		m_pTxt_DeleteTime->SetText(strText);
	}

	int idFaction = GetHostPlayer()->GetFactionID();
	int idFamily = GetHostPlayer()->GetFamilyID();
	Faction_Info *pInfo;
	if( bFaction )
		pInfo = pFMan->GetFaction(idFaction);
	else
		pInfo = pFMan->GetFaction(idFamily, 0 ,true);

	if( pInfo )
	{
		//ACHAR szFName[20];
		//AUI_ConvertChatString(pInfo->GetName(), szFName);
		//m_pTxt_GuildName->SetText(szFName);

		if( !bFaction )
		{
			if( stricmp(m_szName, "Win_GuildManage") != 0 )
			{
				strText = _AL("15");
				m_pTxt_PplCap->SetText(strText);
			}
		}

		if( bFaction )
		{
			//strText.Format(_AL("%d"), pInfo->GetLevel() + 1);
			//m_pTxt_GuildLevel->SetText(strText);
			//strText.Format(_AL("%d"), pInfo->GetProsperity());
			//m_pTxt_GuildProsperity->SetText(strText);
			//int nHonor = pInfo->GetNimbus();
			//int level = 0;
			//while( nHonor >= honorlevel[level + 1] )
			//	level++;
			//int nNowHonor = nHonor - honorlevel[level];
			//int nMaxHonor = honorlevel[level + 1] - honorlevel[level];
			//m_pPro_GuildHonor->SetProgress(int(double(nNowHonor) / nMaxHonor * AUIPROGRESS_MAX));
			//strText.Format(_AL("%d/%d"), nHonor, honorlevel[level + 1]);
			//m_pPro_GuildHonor->SetHint(strText);
			//m_pTxt_GuildHonor->SetText(GetStringFromTable(1200 + level));
			if( stricmp(m_szName, "Win_GuildManage") != 0 )
			{
				strText.Format(_AL("%d/%d"), pFMan->GetFamilyList().size(), pInfo->GetLevel() * 2 + 5);
				m_pTxt_PplCap->SetText(strText);
			}
			//int nTerr = pInfo->GetTerritoryScore();
			//level = 0;
			//while( nTerr >= terrscorelevel[level + 1] )
			//	level++;
			//int nNowTerr = nTerr - terrscorelevel[level];
			//int nMaxTerr = terrscorelevel[level + 1] - terrscorelevel[level];
			//m_pPro_GuildTerrScore->SetProgress(int(double(nNowTerr) / nMaxTerr * AUIPROGRESS_MAX));
			//strText.Format(_AL("%d/%d"), nTerr, terrscorelevel[level + 1]);
			//m_pPro_GuildTerrScore->SetHint(strText);
			//m_pTxt_GuildTerrScore->SetText(GetStringFromTable(1210 + level));
		}
	}
	if( bFaction )
	{
		//if( 0 != a_strcmp(pFMan->GetProclaim(), m_pTxt_Message->GetText()) )
		//{
		//	strText = pFMan->GetProclaim();
		//	GetGameUIMan()->FilterBadWords(strText);
		//	m_pTxt_Message->SetText(strText);
		//}
	}
	else
	{
		if( 0 != a_strcmp(pFMan->GetFamilyProclaim(), GetDlgItem("Txt_Message")->GetText()) )
		{
			strText = pFMan->GetFamilyProclaim();
			GetGameUIMan()->FilterBadWords(strText);
			GetDlgItem("Txt_Message")->SetText(strText);
		}
	}

	if( !bFaction )
	{
		DWORD i;
		for(i = 0; i < CDLGGUILDMAN_FAMILYSKILL_MAX; i++)
		{
			if( i < pFMan->GetFamilySkillList().size() && 
				GetHostPlayer()->GetSkill(pFMan->GetFamilySkillList()[i].id) )
			{
				CECSkill* pSkill = GetHostPlayer()->GetSkill(pFMan->GetFamilySkillList()[i].id);
				m_pImg_Skill[i]->Show(true);
				m_pLab_Lev_Skill[i]->Show(true);
				m_pLab_Exp_Skill[i]->Show(true);
				GetGameUIMan()->m_pDlgSkill->SetImage(m_pImg_Skill[i], pSkill, true);
				if( pSkill->GetType() == CECSkill::TYPE_PASSIVE || pSkill->GetType() == CECSkill::TYPE_LIVE )
					m_pImg_Skill[i]->SetDataPtr(NULL);
				ACHAR szText[100];
				a_sprintf(szText, GetStringFromTable(887), pSkill->GetSkillLevel());
				m_pLab_Lev_Skill[i]->SetText(szText);
				if( pSkill->GetSkillLevel() > 0 && familyskillability[pSkill->GetSkillLevel()] != INT_MAX )
				{
					a_sprintf(szText, _AL("%.2f%%"), double(pFMan->GetFamilySkillList()[i].ability - familyskillability[pSkill->GetSkillLevel() - 1]) * 100.0 / 
						(familyskillability[pSkill->GetSkillLevel()] - familyskillability[pSkill->GetSkillLevel() - 1]));
				}
				else
					a_strcpy(szText, _AL("----"));
				m_pLab_Exp_Skill[i]->SetText(szText);
			}
			else
			{
				m_pImg_Skill[i]->Show(false);
				m_pLab_Lev_Skill[i]->Show(false);
				m_pLab_Exp_Skill[i]->Show(false);
			}
		}

		for(i = 0; i < FAMILY_RECORD_COUNT; i++)
		{
			char szName[100];
			sprintf(szName, "Lab_FamilyHonor%02d", i + 1);
			if( GetDlgItem(szName) && a_strlen(GetDlgItem(szName)->GetText()) > 0 )
			{
				sprintf(szName, "Lab_Num_FamilyHonor%02d", i + 1);
				ACHAR szText[20];
				a_sprintf(szText, _AL("%d"), pFMan->GetFamilyRecord(i));
				GetDlgItem(szName)->SetText(szText);
			}
		}
	}

//	if( GetTickCount() - m_dwLastRefreshTime >= 30 * 1000 )
//		m_pBtn_Refresh->Show(true);
//	else
//		m_pBtn_Refresh->Show(false);
}

void CDlgGuildMan::BuildFamilyList()
{
	DWORD i;
	CECFactionMan *pFMan = GetGame()->GetFactionMan();
	m_pList_Family->ResetContent();
	m_pList_Family->AddString(GetStringFromTable(865));
	m_pList_Family->SetItemTextColor(0, A3DCOLORRGB(43, 255, 250));
	m_pList_Family->SetData(GetHostPlayer()->GetFactionID());
	for(i = 0; i < pFMan->GetFamilyList().size(); i++)
	{
		Faction_Info *pInfo = pFMan->GetFaction(pFMan->GetFamilyList()[i], false, true);
		if( pInfo )
		{
			m_pList_Family->AddString(pInfo->GetName());
			m_pList_Family->SetItemData(i + 1, pInfo->GetID());
			if( (int)pInfo->GetID() == m_idCurFamily )
				m_pList_Family->SetCurSel(i + 1);
		}
	}
}

void CDlgGuildMan::BuildFactionList()
{
	ACHAR szName[100];
	int nCurSel = m_pList_Member->GetCurSel();
	if( nCurSel >= 0 && nCurSel < m_pList_Member->GetCount() )
	{
		const ACHAR *szText = m_pList_Member->GetText(nCurSel);
		ACHAR *pszName = szName;
		while( *szText != '\0' )
		{
			if( *szText == '\t' )
			{
				*pszName = '\0';
				break;
			}
			*(pszName++) = *(szText++);
		}
		if( *pszName != '\0')
			szName[0] = '\0';
	}

	ACString strText;
	int i;
	FactionMemList &fml = GetGame()->GetFactionMan()->GetMemList();

	if( m_nSortMode != SORT_BY_ORDER )
	{
		bool bExchanged;
		Faction_Mem_Info *pTemp;

		do
		{
			bExchanged = false;
			for( i = 0; i < (int)fml.size() - 1; i++ )
			{
				if( (m_nSortMode == SORT_BY_NAME &&
					a_stricmp(fml[i]->GetName(), fml[i + 1]->GetName()) > 0) ||
					(m_nSortMode == SORT_BY_JOB &&
					fml[i]->GetFRoleID() > fml[i + 1]->GetFRoleID()) ||
					(m_nSortMode == SORT_BY_TITLE &&
					a_stricmp(fml[i]->GetNickName(), fml[i + 1]->GetNickName()) > 0) ||
					(m_nSortMode == SORT_BY_LEVEL &&
					fml[i]->GetLev() < fml[i + 1]->GetLev()) ||
					(m_nSortMode == SORT_BY_PROFESSION &&
					fml[i]->GetProf() > fml[i + 1]->GetProf()) ||
					(m_nSortMode == SORT_BY_ONLINE &&
					fml[i]->GetLineId() < fml[i + 1]->GetLineId()) ||
					(m_nSortMode == SORT_BY_FAMILY &&
					fml[i]->GetFamilyID() < fml[i + 1]->GetFamilyID()) ||
					(m_nSortMode == SORT_BY_DEVOTION &&
					fml[i]->GetDevotion() < fml[i + 1]->GetDevotion()) ||
					(m_nSortMode == SORT_BY_CONTRIBUTION &&
					fml[i]->GetContribution() < fml[i + 1]->GetContribution()) ||
					(m_nSortMode == SORT_BY_CAMP &&
					fml[i]->GetCultivation() < fml[i + 1]->GetCultivation())||
					(SORT_BY_MONEY == m_nSortMode && 
					fml[i]->GetFacCouponAdd() < fml[i + 1]->GetFacCouponAdd() ))
				{
					pTemp = fml[i];
					fml[i] = fml[i + 1];
					fml[i + 1] = pTemp;
					bExchanged = true;
				}
			}
		}
		while( bExchanged );
	}

	m_pList_Member->ResetContent();
	ACString strLine;
	for( i = 0; i < (int)fml.size(); i++ )
	{
		strLine = GetGameUIMan()->GetLine(fml[i]->GetLineId());

		if( stricmp(m_szName, "Win_GuildManage") == 0 )
		{
			ACString strFamily;
			Faction_Info *finfo = GetGame()->GetFactionMan()->GetFaction(fml[i]->GetFamilyID(), true, true);
			if( finfo )
				strFamily = finfo->GetName();
			else
				strFamily = _AL(" ");
			if( fml[i]->GetFRoleID() == GNET::_R_MEMBER )
			{
				strText.Format(_AL("%s\t%s\t%s\t%s\t%d\t%s\t%d\t%I64d\t%s\t%s"),
					fml[i]->GetName(), strFamily, _AL(" "),
					a_strlen(fml[i]->GetNickName()) > 0 ? fml[i]->GetNickName() : _AL(" "),
					fml[i]->GetLev(), GetStringFromTable(2101 + fml[i]->GetProf()),
					fml[i]->GetContribution(), fml[i]->GetFacCouponAdd(),strLine, GetGameUIMan()->GetStringFromTable(7000 + fml[i]->GetCultivation()) );
			}
			else
			{
				strText.Format(_AL("%s\t%s\t%s\t%s\t%d\t%s\t%d\t%I64d\t%s\t%s"),
					fml[i]->GetName(), strFamily, GetStringFromTable(1300 + fml[i]->GetFRoleID()),
					a_strlen(fml[i]->GetNickName()) > 0 ? fml[i]->GetNickName() : _AL(" "),
					fml[i]->GetLev(), GetStringFromTable(2101 + fml[i]->GetProf()),
					fml[i]->GetContribution(), fml[i]->GetFacCouponAdd(),strLine, GetGameUIMan()->GetStringFromTable(7000 + fml[i]->GetCultivation()));
			}
		}
		else
		{
			if( fml[i]->GetFRoleID() == GNET::_R_MEMBER )
			{
				strText.Format(_AL("%s\t%d\t%s\t%s\t%d\t%s\t%s"),
					fml[i]->GetName(), fml[i]->GetLev(), GetStringFromTable(2101 + fml[i]->GetProf()), _AL(" "),
					fml[i]->GetDevotion(), strLine, GetGameUIMan()->GetStringFromTable(7000 + fml[i]->GetCultivation()));
			}
			else
			{
				strText.Format(_AL("%s\t%d\t%s\t%s\t%d\t%s\t%s"),
					fml[i]->GetName(), fml[i]->GetLev(), GetStringFromTable(2101 + fml[i]->GetProf()), 
					GetStringFromTable(1308), fml[i]->GetDevotion(), strLine, GetGameUIMan()->GetStringFromTable(7000 + fml[i]->GetCultivation()));
			}
		}
		m_pList_Member->AddString(strText);
		m_pList_Member->SetItemData(i, fml[i]->GetID());
		m_pList_Member->SetItemData(i, fml[i]->GetFRoleID(), 1);

		if( a_strcmp(szName, fml[i]->GetName()) == 0 )
			m_pList_Member->SetCurSel(i);
	}

	Faction_Info *finfo;
	if( stricmp(m_szName, "Win_GuildManage") == 0 )
		finfo = GetGame()->GetFactionMan()->GetFaction(GetHostPlayer()->GetFactionID());
	else
		finfo = GetGame()->GetFactionMan()->GetFaction(GetHostPlayer()->GetFamilyID(), false, true);
	if( stricmp(m_szName, "Win_GuildManage") != 0 )
	{
		if( finfo )
			strText.Format(_AL("%d/%d"), finfo->GetOnlineMemNum(), finfo->GetMemNum());
		else
			strText = _AL("");
		m_pTxt_PplOnline->SetText(strText);
	}
	//if( stricmp(m_szName, "Win_GuildManage") == 0 )
	//{
		//int nIndex = 0;
		//A2DSprite *sprite = GetGameUIMan()->GetFactionIcon(GetGame()->GetGameInit().iServerID, GetHostPlayer()->GetFactionID(), &nIndex);
		//m_pImg_GuildIcon->SetCover(sprite, nIndex);


		//Faction_Info *pFactionInfo= NULL;
		//pFactionInfo = GetGame()->GetFactionMan()->GetFaction(GetHostPlayer()->GetFactionID());
		//if (NULL != pFactionInfo)
		//{
		//   strText = _AL("");
		//   strText.Format(GetStringFromTable(16001),pFactionInfo->GetMoney()/10000);
		//   ((PAUILABEL)(GetDlgItem("Txt_GetGuildwarMoney")))->SetText(strText);
		//}

//	}
}

void CDlgGuildMan::BuildGuildWarRankList(void *pData)
{
	CombatTop_Re *p = (CombatTop_Re*)pData;
	DWORD i;
	PAUILISTBOX pList = (PAUILISTBOX)GetDlgItem("List_Member");
	pList->ResetContent();
	for(i = 0; i < p->list.size(); i++)
	{
		ACString strName((const ACHAR*)p->list[i].rolename.begin(), p->list[i].rolename.size() / sizeof(ACHAR));
		ACString strText;
	
		strText.Format(_AL("%s\t%d\t%d"), strName, p->list[i].kills, p->list[i].deaths);
		pList->AddString(strText);
		if( (int)p->list[i].factionid == GetHostPlayer()->GetFactionID() )
			pList->SetItemTextColor(pList->GetCount() - 1, A3DCOLORRGB(167, 205, 255));
		else
			pList->SetItemTextColor(pList->GetCount() - 1, A3DCOLORRGB(255, 0, 0));
	}
	m_nGuildWarPage = p->handle;
}

void CDlgGuildMan::RefreshGuildWarStatus(void *pData)
{
	CombatStatus *p = (CombatStatus*)pData;
	ACHAR szText[200];
	GetDlgItem("Txt_MKNum")->SetAlign(AUIFRAME_ALIGN_CENTER);
	GetDlgItem("Txt_EKNum")->SetAlign(AUIFRAME_ALIGN_CENTER);
	if( p->attacker == GetHostPlayer()->GetFactionID() )
	{
		a_sprintf(szText, _AL("%d"), p->attacker_kills);
		GetDlgItem("Txt_MKNum")->SetText(szText);
		a_sprintf(szText, _AL("%d"), p->defender_kills);
		GetDlgItem("Txt_EKNum")->SetText(szText);
	}
	else
	{
		a_sprintf(szText, _AL("%d"), p->attacker_kills);
		GetDlgItem("Txt_EKNum")->SetText(szText);
		a_sprintf(szText, _AL("%d"), p->defender_kills);
		GetDlgItem("Txt_MKNum")->SetText(szText);
	}
	GetDlgItem("Txt_EGName")->SetText(_AL(""));
	m_nGuildWarTime = p->remain_time * 1000;
	m_dwLastRefreshTime = GetTickCount();
	if( !IsShow() )
		Show(true);
}

void CDlgGuildMan::RefreshGuildEnemies(void *pData)
{
}

void CDlgGuildMan::OnEventLButtonDownSkill(WPARAM wParam, LPARAM lParam, AUIObject * pObj)
{
	if( !pObj->GetDataPtr("ptr_CECSkill"))
		return;

	A3DVIEWPORTPARAM *p = m_pA3DEngine->GetActiveViewport()->GetParam();
	POINT pt = 
	{
		GET_X_LPARAM(lParam) - p->X,
		GET_Y_LPARAM(lParam) - p->Y,
	};
	
	GetGameUIMan()->m_ptLButtonDown = pt;
	GetGameUIMan()->InvokeDragDrop(this, pObj, pt);
}
