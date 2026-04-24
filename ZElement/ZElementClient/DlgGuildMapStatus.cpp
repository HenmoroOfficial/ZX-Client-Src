// Filename	: DlgGuildMapStatus.cpp
// Creator	: Xiao Zhou
// Date		: 2005/1/5

#include "DlgGuildMapStatus.h"
#include "DlgGuildMap.h"
#include "DlgWorldMap.h"
#include "AUI\\AUIListBox.h"
#include "EC_GameUIMan.h"
#include "EC_Game.h"
#include "EC_Faction.h"
#include "globaldataman.h"
#include "EC_GameSession.h"
#include "EC_HostPlayer.h"
#include "A2DSprite.h"

#define new A_DEBUG_NEW

AUI_BEGIN_COMMAND_MAP(CDlgGuildMapStatus, CDlgBase)

AUI_ON_COMMAND("IDCANCEL",		OnCommandCancel)
AUI_ON_COMMAND("confirm",		OnCommandConfirm)
AUI_ON_COMMAND("allbattletime",	OnCommandAllBattleTime)

AUI_END_COMMAND_MAP()

CDlgGuildMapStatus::CDlgGuildMapStatus()
{
}

CDlgGuildMapStatus::~CDlgGuildMapStatus()
{
}

void CDlgGuildMapStatus::OnCommandCancel(const char * szCommand)
{
	if( stricmp(m_szName, "Win_GMapChallenge") == 0 )
		Show(false);
// 	else if( GetGameUIMan()->m_pDlgGuildMap->IsShow() )
// 		GetGameUIMan()->m_pDlgGuildMap->OnCommandCancel("");
	else
		GetGameUIMan()->m_pDlgWorldMap->OnCommandCancel("");
}

void CDlgGuildMapStatus::OnCommandAllBattleTime(const char * szCommand)
{
	PAUIDIALOG pDlg = GetGameUIMan()->GetDialog("Win_GMapBattleTime");
	PAUILISTBOX pList = (PAUILISTBOX)pDlg->GetDlgItem("Lst_Time");
	pList->ResetContent();
	vector<DOMAIN_INFO> *info = globaldata_getdomaininfos();
	DWORD i;
	for(i = 0; i < info->size(); i++)
	{
// 		if( (*info)[i].battle_time2 != 0 )
// 		{
// 			ACString strText;
// 			long stime = (*info)[i].battle_time2;
// 			stime -= GetGame()->GetTimeZoneBias() * 60;	// localtime = UTC - bias, which bias is in minutes
// 			tm *gtime = gmtime(&stime);
// 			strText.Format(GetStringFromTable(779), GetStringFromTable(856), (*info)[i].name,
// 				gtime->tm_mon + 1, gtime->tm_mday, gtime->tm_hour, gtime->tm_min);
// 			pList->AddString(strText);
// 		}
	}
	for(i = 0; i < info->size(); i++)
	{
// 		if( (*info)[i].battle_time1 != 0 )
// 		{
// 			ACString strText;
// 			long stime = (*info)[i].battle_time1;
// 			stime -= GetGame()->GetTimeZoneBias() * 60;	// localtime = UTC - bias, which bias is in minutes
// 			tm *gtime = gmtime(&stime);
// 			strText.Format(GetStringFromTable(779), GetStringFromTable(857), (*info)[i].name,
// 				gtime->tm_mon + 1, gtime->tm_mday, gtime->tm_hour, gtime->tm_min);
// 			pList->AddString(strText);
// 		}
	}
	pDlg->Show(true, true);
	pDlg->SetPosEx(0, 0, alignCenter, alignCenter);
	pDlg->SetCanMove(false);
}

void CDlgGuildMapStatus::OnCommandConfirm(const char * szCommand)
{
//	int nDeposit = a_atoi(m_pTxt_NewMoney->GetText());
//	if( nDeposit >= (int)m_pTxt_NewMoney->GetData() )
//	{
//		if( nDeposit > GetHostPlayer()->GetMoneyAmount() )
//			GetGameUIMan()->MessageBox("", GetStringFromTable(656),
//				MB_OK, A3DCOLORRGBA(255, 255, 255, 160));
//		else
//		{
//		}
//	}
}

bool CDlgGuildMapStatus::OnInitDialog()
{
	m_pTxt_Name = (PAUILABEL)GetDlgItem("Txt_Name");
	m_pTxt_Attach = (PAUILABEL)GetDlgItem("Txt_Attach");
	m_pTxt_Attack1 = (PAUILABEL)GetDlgItem("Txt_Attack1");
	m_pTxt_Time1 = (PAUILABEL)GetDlgItem("Txt_Time1");
	m_pTxt_Attack2 = (PAUILABEL)GetDlgItem("Txt_Attack2");
	m_pTxt_Time2 = (PAUILABEL)GetDlgItem("Txt_Time2");
	m_pTxt_Time = (PAUILABEL)GetDlgItem("Txt_Time");
	m_nIndex = -1;

	return true;
}

void CDlgGuildMapStatus::OnShowDialog()
{
	m_pTxt_Name->SetText(_AL(""));
	m_pTxt_Attack1->SetText(_AL(""));
	m_pTxt_Attack2->SetText(_AL(""));
	m_pTxt_Attach->SetText(_AL(""));
	if( m_pTxt_Time )
		m_pTxt_Time->SetText(_AL(""));
	if( m_pTxt_Time1 )
		m_pTxt_Time1->SetText(_AL(""));
	if( m_pTxt_Time2 )
		m_pTxt_Time2->SetText(_AL(""));
}

void CDlgGuildMapStatus::SetIndex(int n)
{
	if( n != -1 && n != m_nIndex )
	{
		vector<DOMAIN_INFO> *info = globaldata_getdomaininfos();
		CECFactionMan *pMan = GetGame()->GetFactionMan();
		if( (*info)[n].id_owner )
			pMan->GetFaction((*info)[n].id_owner, true);
// 		if( (*info)[n].id_challenger1 )
// 			pMan->GetFaction((*info)[n].id_challenger1, true);
// 		if( (*info)[n].id_challenger2 )
// 			pMan->GetFaction((*info)[n].id_challenger2, true);
// 		if( (*info)[n].id_tempchallenger1 )
// 			pMan->GetFaction((*info)[n].id_tempchallenger1, true);
// 		if( (*info)[n].id_tempchallenger2 )
// 			pMan->GetFaction((*info)[n].id_tempchallenger2, true);
	}
	m_nIndex = n;
}

void CDlgGuildMapStatus::OnTick()
{
	PAUIDIALOG pDlg = NULL;
	if( stricmp(m_szName, "Win_GMapStatus1") == 0 )
	{
		pDlg = GetGameUIMan()->GetDialog("Win_WorldMap");
		if( !pDlg->IsShow() )
			pDlg = GetGameUIMan()->GetDialog("GuildMap");
	}
	else if( stricmp(m_szName, "Win_GMapStatus2") == 0 )
		pDlg = GetGameUIMan()->GetDialog("GuildMap");
	else
		pDlg = NULL;
	if( pDlg )
		SetPosEx(pDlg->GetSize().cx / 20 + pDlg->GetPos().x, 0, AUIDialog::alignNone, AUIDialog::alignCenter);

	if( m_nIndex == -1 )
	{
		OnShowDialog();
		return;
	}
	vector<DOMAIN_INFO> *info = globaldata_getdomaininfos();
	CECFactionMan *pMan = GetGame()->GetFactionMan();
	
	ACString strText;
	m_pTxt_Name->SetText((*info)[m_nIndex].name);
	strText.Format( _AL("%d"), (*info)[m_nIndex].reward);
	if( (*info)[m_nIndex].id_owner != 0 )
	{
		Faction_Info *finfo = pMan->GetFaction((*info)[m_nIndex].id_owner, false);
		if( finfo )
			m_pTxt_Attach->SetText(finfo->GetName());
		else
			m_pTxt_Attach->SetText(_AL(""));
	}
	else
		m_pTxt_Attach->SetText(GetStringFromTable(780));
	if( stricmp(GetName(), "Win_GMapStatus1") == 0 )
	{
// 		if( (*info)[m_nIndex].id_challenger1 != 0 )
// 		{
// 			Faction_Info *finfo = pMan->GetFaction((*info)[m_nIndex].id_challenger1, false);
// 			if( finfo )
// 				m_pTxt_Attack1->SetText(finfo->GetName());
// 			else
// 				m_pTxt_Attack1->SetText(_AL(""));
// 		}
// 		else
			m_pTxt_Attack1->SetText(GetStringFromTable(780));
// 		if( (*info)[m_nIndex].id_challenger2 != 0 )
// 		{
// 			Faction_Info *finfo = pMan->GetFaction((*info)[m_nIndex].id_challenger2, false);
// 			if( finfo )
// 				m_pTxt_Attack2->SetText(finfo->GetName());
// 			else
// 				m_pTxt_Attack2->SetText(_AL(""));
// 		}
// 		else
			m_pTxt_Attack2->SetText(GetStringFromTable(780));
// 		if( (*info)[m_nIndex].battle_time1 != 0 )
// 		{
// 			long stime = (*info)[m_nIndex].battle_time1;
// 			stime -= GetGame()->GetTimeZoneBias() * 60;	// localtime = UTC - bias, which bias is in minutes
// 			tm *gtime = gmtime(&stime);
// 			strText.Format(GetStringFromTable(781), 
// 				gtime->tm_mon + 1, gtime->tm_mday, gtime->tm_hour, gtime->tm_min);
// 			m_pTxt_Time1->SetText(strText);
// 		}
// 		else
 			m_pTxt_Time1->SetText(_AL(""));
// 		if( (*info)[m_nIndex].battle_time2 != 0 )
// 		{
// 			long stime = (*info)[m_nIndex].battle_time2;
// 			stime -= GetGame()->GetTimeZoneBias() * 60;	// localtime = UTC - bias, which bias is in minutes
// 			tm *gtime = gmtime(&stime);
// 			strText.Format(GetStringFromTable(781), 
// 				gtime->tm_mon + 1, gtime->tm_mday, gtime->tm_hour, gtime->tm_min);
// 			m_pTxt_Time2->SetText(strText);
// 		}
// 		else
 			m_pTxt_Time2->SetText(_AL(""));
	}
	else
	{
// 		if( (*info)[m_nIndex].id_tempchallenger1 != 0 )
// 		{
// 			Faction_Info *finfo = pMan->GetFaction((*info)[m_nIndex].id_tempchallenger1, true);
// 			if( finfo )
// 				m_pTxt_Attack1->SetText(finfo->GetName());
// 			else
// 				m_pTxt_Attack1->SetText(_AL(""));
// 		}
// 		else
			m_pTxt_Attack1->SetText(GetStringFromTable(780));
// 		if( (*info)[m_nIndex].id_tempchallenger2 != 0 )
// 		{
// 			Faction_Info *finfo = pMan->GetFaction((*info)[m_nIndex].id_tempchallenger2, true);
// 			if( finfo )
// 				m_pTxt_Attack2->SetText(finfo->GetName());
// 			else
// 				m_pTxt_Attack2->SetText(_AL(""));
// 		}
// 		else
			m_pTxt_Attack2->SetText(GetStringFromTable(780));
// 		if( GetGame()->GetServerGMTTime() <= (*info)[m_nIndex].cutoff_time )
// 		{
// 			long stime = (*info)[m_nIndex].cutoff_time;
// 			stime -= GetGame()->GetTimeZoneBias() * 60;	// localtime = UTC - bias, which bias is in minutes
// 			tm *gtime = gmtime(&stime);
// 			strText.Format(GetStringFromTable(781), 
// 				gtime->tm_mon + 1, gtime->tm_mday, gtime->tm_hour, gtime->tm_min);
// 			m_pTxt_Time->SetText(strText);
// 		}
// 		else
			m_pTxt_Time->SetText(GetStringFromTable(782));
	}
}