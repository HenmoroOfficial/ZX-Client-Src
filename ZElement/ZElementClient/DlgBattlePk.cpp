// File		: DlgBattlePk.cpp
// Creator	: Fuchonggang
// Date		: 2009/04/28

#include "AUI\\AUIListBox.h"
#include "AUI\\AUITextArea.h"
#include "AUI\\AUIStillImageButton.h"
#include "AUI\\AUILabel.h"
#include "DlgBattlePk.h"
#include "DlgHost.h"
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

#define new A_DEBUG_NEW

AUI_BEGIN_COMMAND_MAP(CDlgBattlePk, CDlgBase)

AUI_ON_COMMAND("join",			OnCommandConfirm)
AUI_ON_COMMAND("IDCANCEL",		OnCommandCancel)

AUI_END_COMMAND_MAP()

AUI_BEGIN_EVENT_MAP(CDlgBattlePk, CDlgBase)

AUI_ON_EVENT("List_Battle",	WM_LBUTTONDOWN,	OnEventLButtonDownBattle)

AUI_END_EVENT_MAP()

const int a_maxNum = 100;
AUIStillImageButton* CDlgBattlePk::m_pBtnJoinEnter;
AUILabel* CDlgBattlePk::m_pLabTime;
int			CDlgBattlePk::m_iMapId = 0;


CDlgBattlePk::CDlgBattlePk()
{
	m_pLabTitle = NULL;
}

CDlgBattlePk::~CDlgBattlePk()
{
}

bool CDlgBattlePk::OnInitDialog()
{
	if (m_szName == "Win_BattleListPK")
	{
		m_pBtnJoinEnter = (PAUISTILLIMAGEBUTTON)GetDlgItem("Btn_Join");
		m_pLstInfo = (PAUILISTBOX)GetDlgItem("List_Member");
		m_pLabTitle = (PAUILABEL)GetDlgItem("Title2");
		PAUITEXTAREA pText = (PAUITEXTAREA)GetDlgItem("Txt_Description");
		pText->SetText(GetGameUIMan()->GetStringFromTable(9321));
	}
	else if (m_szName == "Win_BattleListPKAwait")
	{
		m_pLabTime = (PAUILABEL)GetDlgItem("Txt_Time");
		m_pLstInfo = (PAUILISTBOX)GetDlgItem("List_Member");
		m_pLabTitle = (PAUILABEL)GetDlgItem("Title2");
		PAUITEXTAREA pText = (PAUITEXTAREA)GetDlgItem("Txt_Description");
		pText->SetText(GetGameUIMan()->GetStringFromTable(9321));
	}
	else if (m_szName == "Win_BattleScorePK")
	{
		m_pLabTime = (PAUILABEL)GetDlgItem("Lab_Time");
		m_pLstInfo = (PAUILISTBOX)GetDlgItem("List_Score");
		m_pLabKill = (PAUILABEL)GetDlgItem("Txt_KillNum");
		m_pLabKillScore = (PAUILABEL)GetDlgItem("Txt_KillScore");
		m_pLabNumLeave = (PAUILABEL)GetDlgItem("Txt_LeftNum");
		m_pLabTitle = (PAUILABEL)GetDlgItem("Title2");
	}
	return true;
}

void CDlgBattlePk::OnShowDialog()
{
	if (m_pLabTitle)
		m_pLabTitle->SetText(GetGameUIMan()->GetStringFromTable(GetHostPlayer()->GetRebornInfo().size()==0 ? 9196 : 9197));
}

void CDlgBattlePk::OnTick()
{
	if (m_szName == "Win_BattleListPK")
	{
		ACHAR szText[256];
		tm tmBm;
		tmBm.tm_sec = 0;
		tmBm.tm_min = 30;
		tmBm.tm_hour = 19;
		tmBm.tm_mday = GetGame()->GetServerLocalTime().tm_mday;
		tmBm.tm_mon = GetGame()->GetServerLocalTime().tm_mon;
		tmBm.tm_year = GetGame()->GetServerLocalTime().tm_year;
		tmBm.tm_wday = GetGame()->GetServerLocalTime().tm_wday;
		tmBm.tm_yday = GetGame()->GetServerLocalTime().tm_yday;
		tmBm.tm_isdst = GetGame()->GetServerLocalTime().tm_isdst;
		time_t nStart = mktime(&tmBm);
		int nTime = nStart - GetGame()->GetServerGMTTime();
		if( nTime > 0 )
		{
			a_sprintf(szText,GetStringFromTable(7711),nTime/60,nTime%60);
			GetDlgItem("Txt_Time")->SetText(szText);
		}
		else
		{
			GetDlgItem("Txt_Time")->SetText(GetStringFromTable(7710));
		}
		if ( GetGameUIMan()->m_pDlgHost->GetBattleState() == CDlgHost::BATTLE_STATE_INBATTLE )
		{
			OnCommandCancel("");
		}
	}
	if (m_szName == "Win_BattleListPKAwait")
	{
		ACHAR szText[256];
		int nTime = GetGameUIMan()->m_pDlgHost->GetBattleTime() - GetGame()->GetServerGMTTime();
		if( nTime > 0 )
		{
			a_sprintf(szText,GetStringFromTable(7712),nTime/60,nTime%60);
			GetDlgItem("Txt_Time")->SetText(szText);
		}
		else
		{
			a_sprintf(szText,GetStringFromTable(7712),0,0);
			GetDlgItem("Txt_Time")->SetText(szText);
		}
		if ( GetGameUIMan()->m_pDlgHost->GetBattleState() == CDlgHost::BATTLE_STATE_INBATTLE  )
		{
			OnCommandCancel("");
		}
	}
	else if (m_szName == "Win_BattleScorePK")
	{
		ACString strText;
		int nTime = (GetGameUIMan()->m_pDlgHost->GetBattleTime() - GetGame()->GetServerGMTTime() ) / 60 + 1;
		if( nTime > 0 )
		{
			strText.Format(_AL("%d"), nTime);
			GetDlgItem("Txt_Time")->SetText(strText);
			GetDlgItem("Txt_Time")->Show(true);
		}
		else
			GetDlgItem("Txt_Time")->Show(true);
	}
}

void CDlgBattlePk::OnCommandConfirm(const char * szCommand)
{
	int state = GetGameUIMan()->m_pDlgHost->GetBattleState();
	if( stricmp(m_szName, "Win_BattleListPK") == 0 )
	{
		int nSel = m_pLstInfo->GetCurSel();
		if( nSel >= m_pLstInfo->GetCount() )
			return;
		m_iMapId = m_pLstInfo->GetItemData(nSel, 1);
		int idEnter;
		if (GetHostPlayer()->GetRebornInfo().size()==0)
		{
			idEnter = GetGameRun()->GetSpecialIDConfig().id_enter_arena_item;
		}
		else
			idEnter = GetGameRun()->GetSpecialIDConfig().id_enter_arena_reborn_item;
		int iPos = -1;
		if ( (iPos = GetHostPlayer()->GetPack()->FindItem(idEnter)) >0)
		{
			GetGameSession()->c2s_CmdNPCSevArenaJoin(m_iMapId, iPos);
		//	GetGameSession()->battle_join(GetGameRun()->GetCurLine(), m_iMapId, 0);
		}
		else
			GetGameUIMan()->MessageBox("",GetStringFromTable(9320), MB_OK, A3DCOLORRGBA(255, 255, 255, 160));
	}
	else if ( stricmp(m_szName, "Win_BattleListPKAwait") == 0 )
	{
		GetGameSession()->battle_leave();
	}
}

void CDlgBattlePk::OnCommandCancel(const char * szCommand)
{
	GetGameUIMan()->EndNPCService();
	Show(false);
}

void CDlgBattlePk::OnEventLButtonDownBattle(WPARAM wParam, LPARAM lParam, AUIObject * pObj)
{
}

void CDlgBattlePk::UpdatePkBattleInfo(void *pData)
{
	BattleGetField_Re *p = (BattleGetField_Re*)pData;
	if( p->retcode != ERR_SUCCESS )
		return;
	ACString strText;
	m_pLstInfo->ResetContent();
	ACString strLine = GetGameUIMan()->GetLine(GetGame()->GetGameRun()->GetCurLine());
	strText.Format(ACString(_AL("%s\t%d")), strLine, (int)(p->red_team.size()+p->blue_team.size()));
	m_pLstInfo->AddString(strText);
	GetGameUIMan()->m_pDlgHost->SetBattleTime(p->starttime);
	Show(true);
}

void CDlgBattlePk::UpdatePkBattleList(void *pData)
{
	BattleGetList_Re *p = (BattleGetList_Re*)pData;
	if( p->retcode != ERR_SUCCESS )
		return;

	int k = 0;
	ACString strText;
	PAUILISTBOX pList = m_pLstInfo;
	pList->ResetContent();
	for(int i = 0; i < (int)p->fields.size(); i++ )
	{
		ACString strLine = GetGameUIMan()->GetLine(p->fields[i].gs_id);
//		if (p->fields[i].starttime >= GetGame()->GetServerGMTTime())
//		{
//			strText.Format(ACString(_AL("%s\t%s\t%d/%d\t")) + GetStringFromTable(1602), 
//				strLine, GetStringFromTable(1900 + p->fields[i].map_id), p->fields[i].red_number+p->fields[i].blue_number,a_maxNum,
//				(p->fields[i].starttime - GetGame()->GetServerGMTTime()) / 60 + 1);
//		}
//		else
//		{
			strText.Format(ACString(_AL("%s\t%d/%d")), strLine, p->fields[i].red_number+p->fields[i].blue_number,a_maxNum);
			m_iMapId = p->fields[i].map_id;
//		}
		if( p->fields[i].gs_id == GetGameRun()->GetCurLine() )
		{
			pList->InsertString(k, strText);
			pList->SetItemData(k, p->fields[i].gs_id, 0);
			pList->SetItemData(k, p->fields[i].map_id, 1);
			pList->SetItemData(k, p->fields[i].red_number+p->fields[i].blue_number, 2);
			k++;
		}
		else
		{
			pList->AddString(strText);
			pList->SetItemData(pList->GetCount() - 1, p->fields[i].gs_id, 0);
			pList->SetItemData(pList->GetCount() - 1, p->fields[i].map_id, 1);
			pList->SetItemData(pList->GetCount() - 1, p->fields[i].red_number+p->fields[i].blue_number, 2);
		}
	}
	if (p->fields.size()>0)
	{
		GetGameUIMan()->m_pDlgHost->SetBattleTime(p->fields[0].starttime);
		m_iEnterTime = p->fields[0].starttime;
	}

	Show(true);
}

void CDlgBattlePk::UpdatePkBattleScore(void *pData)
{
	S2C::arena_info *p = (S2C::arena_info*)pData;

	ACString strText;
	strText.Format(_AL("%d"), p->kill_count);
	m_pLabKill->SetText(strText);
	strText.Format(_AL("%d"), p->score);
	m_pLabKillScore->SetText(strText);
	strText.Format(_AL("%d"), p->remain_count);
	m_pLabNumLeave->SetText(strText);

	m_pLstInfo->ResetContent();
	for (int i=0; i<p->player_count; i++)
	{
		strText.Format(_AL("%d\t%d"), p->info[i].kill_count, p->info[i].score);
		m_pLstInfo->AddString(strText);
	}
	Show(true);
}