// File		: DlgBattleInfo.cpp
// Creator	: Xiao Zhou
// Date		: 2007/8/6

#include "AUI\\AUIListBox.h"
#include "AUI\\AUIRadioButton.h"
#include "DlgBattleInfo.h"
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
#include "EC_GPDataType.h"

#define new A_DEBUG_NEW

AUI_BEGIN_COMMAND_MAP(CDlgBattleInfo, CDlgBase)

AUI_ON_COMMAND("giveup",		OnCommandGiveup)
AUI_ON_COMMAND("choice",		OnCommandChoice)
AUI_ON_COMMAND("join",			OnCommandJoin)
AUI_ON_COMMAND("refresh",		OnCommandRefresh)
AUI_ON_COMMAND("IDCANCEL",		OnCommandCancel)
AUI_ON_COMMAND("Btn_Exit",		OnCommandExit)
AUI_END_COMMAND_MAP()

AUI_BEGIN_EVENT_MAP(CDlgBattleInfo, CDlgBase)

AUI_ON_EVENT("List_MemberA",	WM_RBUTTONDOWN,	OnEventRButtonDown)
AUI_ON_EVENT("List_MemberB",	WM_RBUTTONDOWN,	OnEventRButtonDown)
AUI_ON_EVENT(NULL,		WM_LBUTTONDOWN,		OnEvent)
AUI_ON_EVENT(NULL,		WM_LBUTTONUP,		OnEvent)
AUI_ON_EVENT(NULL,		WM_LBUTTONDBLCLK,	OnEvent)
AUI_ON_EVENT(NULL,		WM_RBUTTONUP,		OnEvent)
AUI_ON_EVENT(NULL,		WM_RBUTTONDBLCLK,	OnEvent)
AUI_ON_EVENT(NULL,		WM_MOUSEMOVE,		OnEvent)

AUI_END_EVENT_MAP()

int CDlgBattleInfo::m_iJoinType = 1;
int CDlgBattleInfo::m_iEnterTimes = 0;
int CDlgBattleInfo::m_iBattleType = 0;

CDlgBattleInfo::CDlgBattleInfo()
{
	m_iMaxNum = 0;

	m_pTxtScore01	= NULL;
	m_pTxtScore02	= NULL;
	m_pTxtPlayer01	= NULL;
	m_pTxtPlayer02	= NULL;
	m_pTxtKillOwn	= NULL;
	m_pTxtKillEnemy	= NULL;
	m_pTxtKill		= NULL;
	m_pTxtDead		= NULL;
	m_TxtTimeLeft	= NULL;
	m_pTxtOffLineEnemy = NULL;
	m_pTxtOffLineOwn = NULL;
	
	m_bisBattleUIType = false;
	m_iMinPlayerDeduceScore = 99999;

	m_pLabTimeLeft	= NULL;

	m_pLabKillOwn = NULL;
	m_pLabKillEnemy = NULL;
	m_pLabKill = NULL;
	m_pLabDead = NULL;
}

CDlgBattleInfo::~CDlgBattleInfo()
{
}

bool CDlgBattleInfo::OnInitDialog()
{
	if (stricmp(m_szName, "Win_PKScoreHover2013") == 0)
	{
		DDX_Control("Txt_Score01", m_pTxtScore01);		// 我方分数
		DDX_Control("Txt_Score02", m_pTxtScore02);	// 敌方分数

		DDX_Control("Txt_Player01", m_pTxtPlayer01);		// 我方玩家人数
		DDX_Control("Txt_Player02", m_pTxtPlayer02);	// 敌方玩家人数

		DDX_Control("Txt_Dead", m_pTxtDead);			// 个人死亡
		DDX_Control("Txt_PerKill", m_pTxtKill);			// 个人击杀
		
		DDX_Control("Txt_Punish01", m_pTxtOffLineEnemy);	// 我方离线
		DDX_Control("Txt_Punish02", m_pTxtOffLineOwn);	// 敌方离线


		DDX_Control("Txt_Kill01", m_pTxtKillOwn);		// 我方杀人总数
		DDX_Control("Txt_Kill02", m_pTxtKillEnemy);		// 敌方杀人总数
	
		DDX_Control("Txt_TimeLeft", m_TxtTimeLeft);
	}

	if (stricmp(m_szName, "Win_PkScore") == 0)
	{
		DDX_Control("Lab_KillOwn", m_pLabKillOwn);
		DDX_Control("Lab_KillEnemy", m_pLabKillEnemy);
		DDX_Control("Lab_Kill", m_pLabKill);
		DDX_Control("Lab_Dead", m_pLabDead);
		DDX_Control("Txt_KillOwn", m_pTxtKillOwn);
		DDX_Control("Txt_KillEnemy", m_pTxtKillEnemy);
		DDX_Control("Txt_Kill", m_pTxtKill);
		DDX_Control("Txt_Dead", m_pTxtDead);
		DDX_Control("Txt_TimeLeft", m_pLabTimeLeft);
	}
	return true;
}

void CDlgBattleInfo::OnShowDialog()
{
	if(stricmp(m_szName, "Win_PkScore2013") == 0)
	{
		if(m_bisBattleUIType == true)
		{
			GetDlgItem("Title")->SetText(GetStringFromTable(19950));
			GetDlgItem("Lab_Time")->Show(true);
			GetDlgItem("Txt_Time")->Show(true);
			GetDlgItem("Btn_Exit")->Show(false);
		}
		else
		{
			GetDlgItem("Lab_Time")->Show(false);
			GetDlgItem("Txt_Time")->Show(false);
			GetDlgItem("Btn_Exit")->Show(true);
		}
	}
}

void CDlgBattleInfo::OnCommandGiveup(const char * szCommand)
{
	Show(false);
	GetGameSession()->battle_leave();
}

void CDlgBattleInfo::OnEventRButtonDown(WPARAM wParam, LPARAM lParam, AUIObject * pObj)
{
	if (stricmp(m_szName, "Win_PKScoreHover2013") == 0)
	{
		GetGameUIMan()->RespawnMessage();
	}
	else if(stricmp(m_szName, "Win_PkScore") == 0)
	{
		GetGameUIMan()->RespawnMessage();
	}
	else if(stricmp(m_szName, "Win_BattleInfo") == 0 || stricmp(m_szName, "Win_BattleScore") == 0)
	{
		PAUILISTBOX pList = (PAUILISTBOX)pObj;
		int nSel = pList->GetCurSel();
		if( nSel < pList->GetCount() )
			GetGameUIMan()->PopupPlayerContextMenu(pList->GetItemData(nSel));
	}
}

void CDlgBattleInfo::OnEvent(WPARAM wParam, LPARAM lParam, AUIObject * pObj)
{
	if (stricmp(m_szName, "Win_PKScoreHover2013") == 0)
	{
		GetGameUIMan()->RespawnMessage();
	}
	else 	if (stricmp(m_szName, "Win_PkScore") == 0)
	{
		GetGameUIMan()->RespawnMessage();
	}
}

void CDlgBattleInfo::OnCommandChoice(const char * szCommand)
{
	PAUIRADIOBUTTON pRadio1 = (PAUIRADIOBUTTON)GetDlgItem("Rdo_Choice1");
	PAUIRADIOBUTTON pRadio2 = (PAUIRADIOBUTTON)GetDlgItem("Rdo_Choice2");
	if( pRadio1->IsChecked() )
	{
		GetGameSession()->battle_join(GetGameRun()->GetCurLine(), GetData(), 1);
		Show(false);
	}
	else if( pRadio2->IsChecked() )
	{
		GetGameSession()->battle_join(GetGameRun()->GetCurLine(), GetData(), 2);
		Show(false);
	}
}

void CDlgBattleInfo::OnCommandJoin(const char * szCommand)
{
	PAUILISTBOX pList = (PAUILISTBOX)GetDlgItem("List_Member");
	int nSel = pList->GetCurSel();
	if( nSel >= pList->GetCount() )
		return;

	int gs_id = pList->GetItemData(nSel, 0);
	int map_id = pList->GetItemData(nSel, 1);
	int redmember = pList->GetItemData(nSel, 2);
	int bluemember = pList->GetItemData(nSel, 3);

	m_iMaxNum = GetData();
	PAUIDIALOG pMsgBox;
	if (GetHostPlayer()->IsGM())
	{
		GetGameSession()->battle_join(GetGameRun()->GetCurLine(), map_id, 1);
	}
	else if (m_iBattleType==BATTLE_TYPE_CHALLENGE)	// 挑战空间
	{
		if(m_iEnterTimes==0)
			GetGameSession()->battle_join(GetGameRun()->GetCurLine(), map_id, 1);
		else
			GetGameUIMan()->MessageBox(5290);
	}
	else if ( 20!= GetData() && GetHostPlayer()->GetRebornInfo().size()>0)//不是跨服pk,飞升战场
	{
		GetGameSession()->battle_join(GetGameRun()->GetCurLine(), map_id, 0);
	}
	else if( redmember >= m_iMaxNum && bluemember >= m_iMaxNum )
		GetGameUIMan()->MessageBox("", GetStringFromTable(911), MB_YESNO, A3DCOLORRGBA(255, 255, 255, 160));
	else if( redmember < m_iMaxNum && bluemember >= m_iMaxNum )
	{
		GetGameUIMan()->MessageBox("Game_BattleJoin1", GetStringFromTable(913), MB_YESNO, A3DCOLORRGBA(255, 255, 255, 160), &pMsgBox);
		pMsgBox->SetData(map_id);
	}
	else if( redmember >= m_iMaxNum && bluemember < m_iMaxNum )
	{
		GetGameUIMan()->MessageBox("Game_BattleJoin2", GetStringFromTable(912), MB_YESNO, A3DCOLORRGBA(255, 255, 255, 160), &pMsgBox);
		pMsgBox->SetData(map_id);
	}
	else
	{
		GetGameUIMan()->GetDialog("Win_BattleJoinChoice")->Show(true, true);
		GetGameUIMan()->GetDialog("Win_BattleJoinChoice")->SetData(map_id);
	}
}

void CDlgBattleInfo::OnCommandRefresh(const char * szCommand)
{
	((CDlgHost*)GetGameUIMan()->GetDialog("Win_CharHead"))->OnCommand_battleinfo("");
}

void CDlgBattleInfo::OnCommandCancel(const char * szCommand)
{
	if( stricmp(m_szName, "Win_BattleList") == 0 || stricmp(m_szName, "Win_BattleList2") == 0 )
		GetGameUIMan()->EndNPCService();
	if (stricmp(m_szName, "Win_PKScoreHover2013") == 0)
	{
		m_pAUIManager->RespawnMessage();
	}
	else if (stricmp(m_szName, "Win_PkScore") == 0)
	{
		m_pAUIManager->RespawnMessage();
	}
	else
		Show(false);
}

void CDlgBattleInfo::OnCommandExit(const char * szCommand)
{
//	GetGameSession()->battle_leave();
	Show(false);
}

void CDlgBattleInfo::OnTick()
{
	if( stricmp(m_szName, "Win_BattleInfo") == 0 || 
		stricmp(m_szName, "Win_BattleInfo2") == 0 || 
		stricmp(m_szName, "Win_BattleInfo_Challenge") == 0 || 
		stricmp(m_szName, "Win_Battle6v6") == 0 )
	{
		if (GetHostPlayer()->GetBattleType()>=0)
		{
			Show(false);
		}
		ACString strText;
		int nTime = (GetGameUIMan()->m_pDlgHost->GetBattleTime() - GetGame()->GetServerGMTTime() + 59) / 60;
		if( nTime > 0 )
		{
			strText.Format(_AL("%d"), nTime);
			GetDlgItem("Txt_Time")->SetText(strText);
			GetDlgItem("Txt_Time")->Show(true);
		}
		else
			GetDlgItem("Txt_Time")->Show(false);
	}
	else if( stricmp(m_szName, "Win_BattleScore") == 0 )
	{
		ACString strText;
		int nTime = (GetGameUIMan()->m_pDlgHost->GetBattleTime() - GetGame()->GetServerGMTTime() + 59) / 60;
		if( nTime > 0 )
		{
			strText.Format(_AL("%d"), nTime);
			GetDlgItem("Txt_Time")->SetText(strText);
			GetDlgItem("Txt_Time")->Show(true);
		}
		else
			GetDlgItem("Txt_Time")->Show(false);
		PAUILISTBOX pListA = (PAUILISTBOX)GetDlgItem("List_MemberA");
		PAUILISTBOX pListB = (PAUILISTBOX)GetDlgItem("List_MemberB");
		int i;
		for(i = 0; i < pListA->GetCount(); i++)
			if( pListA->GetItemData(i, 1) != 0 )
			{
				if( GetGameRun()->GetPlayerName(pListA->GetItemData(i), false) )
				{
					ACString strText = pListA->GetText(i);
					strText = GetGameRun()->GetPlayerName(pListA->GetItemData(i), true) + strText.Mid(1);
					pListA->SetText(i, strText);
					pListA->SetItemData(i, 0, 1);
				}
			}
		for(i = 0; i < pListB->GetCount(); i++)
			if( pListB->GetItemData(i, 1) != 0 )
			{
				if( GetGameRun()->GetPlayerName(pListB->GetItemData(i), false) )
				{
					ACString strText = pListB->GetText(i);
					strText = GetGameRun()->GetPlayerName(pListB->GetItemData(i), true) + strText.Mid(1);
					pListB->SetText(i, strText);
					pListB->SetItemData(i, 0, 1);
				}
			}
	}
	else if( stricmp(m_szName, "Win_PkScore2013") == 0 )
	{
		ACString strText;
		int nTime = GetGameUIMan()->m_pDlgHost->GetBattleTime() - GetGame()->GetServerGMTTime();
		int nMin = nTime/60;
		int nSec = nTime%60;
		if( nTime > 0 )
		{
			strText.Format(_AL("%d:%02d"), nMin,nSec);
			GetDlgItem("Txt_Time")->SetText(strText);
			GetDlgItem("Txt_Time")->Show(m_bisBattleUIType);
		}
		else
		{
			GetDlgItem("Txt_Time")->Show(false);
			GetDlgItem("Lab_Time")->Show(false);
		}
		PAUILISTBOX pListA = (PAUILISTBOX)GetDlgItem("List_MemberA");
		PAUILISTBOX pListB = (PAUILISTBOX)GetDlgItem("List_MemberB");
		int i;
		for(i = 0; i < pListA->GetCount(); i++)
			if( pListA->GetItemData(i, 1) != 0 )
			{
				if( GetGameRun()->GetPlayerName(pListA->GetItemData(i), false) )
				{
					ACString strText = pListA->GetText(i);
					strText = GetGameRun()->GetPlayerName(pListA->GetItemData(i), true) + strText.Mid(1);
					pListA->SetText(i, strText);
					pListA->SetItemData(i, 0, 1);
				}
			}
		for(i = 0; i < pListB->GetCount(); i++)
			if( pListB->GetItemData(i, 1) != 0 )
			{
				if( GetGameRun()->GetPlayerName(pListB->GetItemData(i), false) )
				{
					ACString strText = pListB->GetText(i);
					strText = GetGameRun()->GetPlayerName(pListB->GetItemData(i), true) + strText.Mid(1);
					pListB->SetText(i, strText);
					pListB->SetItemData(i, 0, 1);
				}
			}
	}
	else if( stricmp(m_szName, "Win_BattleList") == 0 )
	{
		if(!GetHostPlayer()->IsGM())
			GetDlgItem("Btn_Join")->Enable(false);
		PAUILISTBOX pList = (PAUILISTBOX)GetDlgItem("List_Member");
		int nSel = pList->GetCurSel();
		if( nSel < pList->GetCount() )
		{
			int gs_id = pList->GetItemData(nSel, 0);
			int redmember = pList->GetItemData(nSel, 2);
			int bluemember = pList->GetItemData(nSel, 3);

			if( gs_id == GetGameRun()->GetCurLine() && (redmember < m_iMaxNum || bluemember < m_iMaxNum) )
				GetDlgItem("Btn_Join")->Enable(true);
		}
	}
	else if( stricmp(m_szName, "Win_BattleList2") == 0 )
	{
		if(!GetHostPlayer()->IsGM())
			GetDlgItem("Btn_Join")->Enable(false);
		PAUILISTBOX pList = (PAUILISTBOX)GetDlgItem("List_Member");
		int nSel = pList->GetCurSel();
		if( nSel < pList->GetCount() )
		{
			int gs_id = pList->GetItemData(nSel, 0);
			int member = pList->GetItemData(nSel, 2);

			if( gs_id == GetGameRun()->GetCurLine() && member < m_iMaxNum )
				GetDlgItem("Btn_Join")->Enable(true);
		}
	}
	else if ( stricmp(m_szName, "Win_PKScoreHover2013") == 0 )
	{
		ACString strText;
		int nTime = GetGameUIMan()->m_pDlgHost->GetBattleTime() - GetGame()->GetServerGMTTime();
		int nMin = nTime/60;
		int nSec = nTime%60;
		if( nTime>0)
		{
			strText.Format(_AL("%d:%02d"), nMin,nSec);
			m_TxtTimeLeft->SetText(strText);
		}
		else
		{
			m_TxtTimeLeft->SetText(_AL("0:0"));
		}

		if(m_nSlefAttacker_count < m_iMinPlayerDeduceScore)
		{
			GetDlgItem("Img_Hover")->Show(true);
		}
		else
		{
			GetDlgItem("Img_Hover")->Show(false);
		}
	}
	else if ( stricmp(m_szName, "Win_PkScore") == 0 )
	{
		ACString strText;
		int nTime = GetGameUIMan()->m_pDlgHost->GetBattleTime() - GetGame()->GetServerGMTTime();
		int nMin = nTime/60;
		int nSec = nTime%60;
		if( nTime>0)
		{
			strText.Format(_AL("%d:%02d"), nMin,nSec);
			m_pLabTimeLeft->SetText(strText);
		}
		else
		{
			m_pLabTimeLeft->SetText(_AL("0:0"));
		}
	}
	else if ( stricmp(m_szName, "Win_BattleResult6v6") == 0 )
	{
		PAUILISTBOX pList = (PAUILISTBOX)GetDlgItem("List_Result");
		int i;
		for(i = 0; i < pList->GetCount(); i++ )
		{
			if (pList->GetItemData(i, 0)>0)
			{
				int id = pList->GetItemData(i, 0);
				if (GetGameRun()->GetPlayerName(id, false))
				{
					A3DCOLOR c = pList->GetItemTextColor(i);
					ACString strText = pList->GetText(i);
					strText = GetGameRun()->GetPlayerName(id, false) + strText;
					pList->DeleteString(i);
					pList->InsertString(i, strText);
					pList->SetItemTextColor(i, c);
				}
			}

		}
	}
}

void CDlgBattleInfo::UpdateBattleInfo(void *pData)
{
	if ( stricmp(m_szName,"Win_BattleInfo")==0)
	{
		BattleGetField_Re *p = (BattleGetField_Re*)pData;
		if( p->retcode != ERR_SUCCESS )
			return;
		bool bisSlef = false;
		UINT i;
		ACString strText;
		PAUILISTBOX pListA = (PAUILISTBOX)GetDlgItem("List_MemberA");
		PAUILISTBOX pListB = (PAUILISTBOX)GetDlgItem("List_MemberB");
		pListA->ResetContent();
		pListB->ResetContent();
		GetHostPlayer()->SetBattleFlag(p->iscross);
		for(i = 0; i < p->red_team.size(); i++ )
		{
			ACString strName = ACString((const ACHAR*)p->red_team[i].name.begin(), p->red_team[i].name.size() / sizeof (ACHAR));
			strText.Format(_AL("%s\t%d\t%s\t%d"), strName,
				p->red_team[i].level, GetStringFromTable(PROFESSION_START + p->red_team[i].occupation), p->red_team[i].score);
			pListA->AddString(strText);
			pListA->SetItemData(pListA->GetCount() - 1, p->red_team[i].roleid);
	
			CECGame::SERVERINFO* pServerInfo = GetGame()->GetSrcServerInfo(p->red_team[i].src_zoneid);
			if(pServerInfo)
			{
				ACString strTemp;
				strTemp.Format(GetStringFromTable(15300),AS2AC(pServerInfo->szServerGroup),AS2AC(pServerInfo->szServerName));
 				pListA->SetItemHint(pListA->GetCount() - 1,strTemp );
  			}
			GetGameRun()->AddPlayerName(p->red_team[i].roleid, strName);
			if(p->red_team[i].roleid == GetHostPlayer()->GetCharacterID())
			{
				bisSlef = true;
			}
		}
		for(i = 0; i < p->blue_team.size(); i++ )
		{
			ACString strName = ACString((const ACHAR*)p->blue_team[i].name.begin(), p->blue_team[i].name.size() / sizeof (ACHAR));
			strText.Format(_AL("%s\t%d\t%s\t%d"), strName,
				p->blue_team[i].level, GetStringFromTable(PROFESSION_START + p->blue_team[i].occupation), p->blue_team[i].score);
			pListB->AddString(strText);
			pListB->SetItemData(pListB->GetCount() - 1, p->blue_team[i].roleid);
	
			CECGame::SERVERINFO* pServerInfo = GetGame()->GetSrcServerInfo(p->blue_team[i].src_zoneid);
			if(pServerInfo)
			{
				ACString strTemp;
				strTemp.Format(GetStringFromTable(15300),AS2AC(pServerInfo->szServerGroup),AS2AC(pServerInfo->szServerName));
 				pListB->SetItemHint(pListB->GetCount() - 1,strTemp );
  			}
			GetGameRun()->AddPlayerName(p->blue_team[i].roleid, strName);
			if(p->blue_team[i].roleid == GetHostPlayer()->GetCharacterID())
			{
				bisSlef = true;
			}
		}
		GetGameUIMan()->m_pDlgHost->SetBattleTime(p->starttime);

		if(GetHostPlayer()->GetBattleFlag() != 0) /// 2012-8-22 add by zy 判断是否跨服
		{
			GetDlgItem("Title2")->Show(true);
			GetDlgItem("Title")->Show(false);
		}
		else
		{
			GetDlgItem("Title2")->Show(false);
			GetDlgItem("Title")->Show(true);
		}
		GetDlgItem("Btn_Cancel")->Show(bisSlef);
		
		Show(true);
	}
	else if ( stricmp(m_szName,"Win_BattleInfo2")==0)
	{
		BattleGetField_Re *p = (BattleGetField_Re*)pData;
		if( p->retcode != ERR_SUCCESS )
			return;
		
		UINT i;
		ACString strText;
		PAUILISTBOX pList = (PAUILISTBOX)GetDlgItem("List_Member");
		pList->ResetContent();
		for(i = 0; i < p->red_team.size(); i++ )
		{
			ACString strName = ACString((const ACHAR*)p->red_team[i].name.begin(), p->red_team[i].name.size() / sizeof (ACHAR));
			strText.Format(_AL("%s\t%d\t%s\t%d"), strName,
				p->red_team[i].level, GetStringFromTable(PROFESSION_START + p->red_team[i].occupation), p->red_team[i].score);
			pList->AddString(strText);
			pList->SetItemData(pList->GetCount() - 1, p->red_team[i].roleid);
			//	
			CECGame::SERVERINFO* pServerInfo = GetGame()->GetSrcServerInfo(p->red_team[i].src_zoneid);
			if(pServerInfo)
			{
				ACString strTemp;
				strTemp.Format(GetStringFromTable(15300),AS2AC(pServerInfo->szServerGroup),AS2AC(pServerInfo->szServerName));
 				pList->SetItemHint(pList->GetCount() - 1,strTemp );
  			}
			GetGameRun()->AddPlayerName(p->red_team[i].roleid, strName);
		}
		for(i = 0; i < p->blue_team.size(); i++ )
		{
			ACString strName = ACString((const ACHAR*)p->blue_team[i].name.begin(), p->blue_team[i].name.size() / sizeof (ACHAR));
			strText.Format(_AL("%s\t%d\t%s\t%d"), strName,
				p->blue_team[i].level, GetStringFromTable(PROFESSION_START + p->blue_team[i].occupation), p->blue_team[i].score);
			pList->AddString(strText);
			pList->SetItemData(pList->GetCount() - 1, p->blue_team[i].roleid);
			//	
			CECGame::SERVERINFO* pServerInfo = GetGame()->GetSrcServerInfo(p->blue_team[i].src_zoneid);
			if(pServerInfo)
			{
				ACString strTemp;
				strTemp.Format(GetStringFromTable(15300),AS2AC(pServerInfo->szServerGroup),AS2AC(pServerInfo->szServerName));
 				pList->SetItemHint(pList->GetCount() - 1,strTemp );
  			}
			GetGameRun()->AddPlayerName(p->blue_team[i].roleid, strName);
		}

		GetGameUIMan()->m_pDlgHost->SetBattleTime(p->starttime);
		GetHostPlayer()->SetBattleFlag(p->iscross);
		if(GetHostPlayer()->GetBattleFlag() != 0) /// 2012-8-22 add by zy 判断是否跨服
		{
			GetDlgItem("Title2")->Show(true);
			GetDlgItem("Title")->Show(false);
		}
		else
		{
			GetDlgItem("Title2")->Show(false);
			GetDlgItem("Title")->Show(true);
		}
		Show(true);
	}
	else if ( stricmp(m_szName,"Win_BattleInfo_Challenge")==0)
	{
		BattleGetField_Re *p = (BattleGetField_Re*)pData;
		if( p->retcode != ERR_SUCCESS )
			return;
		
		UINT i;
		ACString strText;
		PAUILISTBOX pList = (PAUILISTBOX)GetDlgItem("List_Member");
		pList->ResetContent();
		for(i = 0; i < p->red_team.size(); i++ )
		{
			ACString strName = ACString((const ACHAR*)p->red_team[i].name.begin(), p->red_team[i].name.size() / sizeof (ACHAR));
			strText.Format(_AL("%s\t%d\t%s"), strName,
				p->red_team[i].level, GetStringFromTable(PROFESSION_START + p->red_team[i].occupation));
			pList->AddString(strText);
			pList->SetItemData(pList->GetCount() - 1, p->red_team[i].roleid);
			GetGameRun()->AddPlayerName(p->red_team[i].roleid, strName);
		}
		for(i = 0; i < p->blue_team.size(); i++ )
		{
			ACString strName = ACString((const ACHAR*)p->blue_team[i].name.begin(), p->blue_team[i].name.size() / sizeof (ACHAR));
			strText.Format(_AL("%s\t%d\t%s"), strName,
				p->blue_team[i].level, GetStringFromTable(PROFESSION_START + p->blue_team[i].occupation));
			pList->AddString(strText);
			pList->SetItemData(pList->GetCount() - 1, p->blue_team[i].roleid);
			GetGameRun()->AddPlayerName(p->blue_team[i].roleid, strName);
		}
		GetGameUIMan()->m_pDlgHost->SetBattleTime(p->starttime);
		Show(true);
	}
	else if ( stricmp(m_szName,"Win_Battle6v6")==0)
	{
		BattleGetField_Re *p = (BattleGetField_Re*)pData;
		if( p->retcode != ERR_SUCCESS )
			return;

		PAUILISTBOX pList = (PAUILISTBOX)GetDlgItem("List_Member");
		pList->ResetContent();
		UINT i;
		ACString strText;
		for(i = 0; i < p->red_team.size(); i++ )
		{
			ACString strName = ACString((const ACHAR*)p->red_team[i].name.begin(), p->red_team[i].name.size() / sizeof (ACHAR));
			strText.Format(_AL("%s\t%d\t%s%s"), strName, p->red_team[i].level, 
				GetStringFromTable(7000 + p->red_team[i].cultivation), GetStringFromTable(PROFESSION_START + p->red_team[i].occupation));
			pList->AddString(strText);
			pList->SetItemData(pList->GetCount() - 1, p->red_team[i].roleid);
			GetGameRun()->AddPlayerName(p->red_team[i].roleid, strName);
		}
		for(i = 0; i < p->blue_team.size(); i++ )
		{
			ACString strName = ACString((const ACHAR*)p->blue_team[i].name.begin(), p->blue_team[i].name.size() / sizeof (ACHAR));
			strText.Format(_AL("%s\t%d\t%s%s"), strName, p->blue_team[i].level, 
				GetStringFromTable(7000 + p->blue_team[i].cultivation), GetStringFromTable(PROFESSION_START + p->blue_team[i].occupation));
			pList->AddString(strText);
			pList->SetItemData(pList->GetCount() - 1, p->blue_team[i].roleid);
			GetGameRun()->AddPlayerName(p->blue_team[i].roleid, strName);
		}
		GetGameUIMan()->m_pDlgHost->SetBattleTime(p->starttime);
		Show(true);
	}
}

void CDlgBattleInfo::UpdateBattleScore(void *pData)
{
	S2C::cmd_battle_info *p = (S2C::cmd_battle_info*)pData;
	UINT i;
	ACString strText;
	PAUILABEL pLab1 = (PAUILABEL)GetDlgItem("Lab_Abuild");
	PAUILABEL pLab2 = (PAUILABEL)GetDlgItem("Lab_Bbuild");
	if (GetHostPlayer()->GetRebornInfo().size()>0)
	{
		pLab1->SetText(GetStringFromTable(9182));
		pLab2->SetText(GetStringFromTable(9182));
	}
	else
	{
		pLab1->SetText(GetStringFromTable(9181));
		pLab2->SetText(GetStringFromTable(9181));
	}
	PAUILISTBOX pListA = (PAUILISTBOX)GetDlgItem("List_MemberA");
	PAUILISTBOX pListB = (PAUILISTBOX)GetDlgItem("List_MemberB");
	pListA->ResetContent();
	pListB->ResetContent();
	int killa = 0, killb = 0;
	bool bPk = GetHostPlayer()->GetBattleType()==BATTLE_TYPE_NAMED_CRSSVR;
	for(i = 0; i < (UINT)p->player_count; i++ )
	{
		const ACHAR *pszName = GetGameRun()->GetPlayerName(p->info[i].id, false);
		int id = p->info[i].id;
		if (bPk)
		{
			if( !pszName )
			{
				strText.Format(_AL(" \t%d\t%d"), p->info[i].kill, p->info[i].death);
				GetGameSession()->GetPlayerBriefInfo(1, &p->info[i].id, 2);
			}
			else
				strText.Format(_AL("%s\t%d\t%d"), pszName, p->info[i].kill, p->info[i].death);
		}
		else
		{
			if( !pszName )
			{
				strText.Format(_AL(" \t%d\t%d\t%d"), p->info[i].kill, p->info[i].death, p->info[i].battle_score);
				GetGameSession()->GetPlayerBriefInfo(1, &p->info[i].id, 2);
			}
			else
				strText.Format(_AL("%s\t%d\t%d\t%d"), pszName, p->info[i].kill, p->info[i].death, p->info[i].battle_score);
		}
		if( p->info[i].battle_faction == 1 )
		{
			if(bPk)
				killb += p->info[i].death;
			else
				killa += p->info[i].kill;
			pListA->AddString(strText);
			pListA->SetItemData(pListA->GetCount() - 1, id);
			if( !pszName )
				pListA->SetItemData(pListA->GetCount() - 1, 1, 1);
		}
		else
		{
			if(bPk)
				killa += p->info[i].death;
			else
				killb += p->info[i].kill;
			pListB->AddString(strText);
			pListB->SetItemData(pListB->GetCount() - 1, id);
			if( !pszName )
				pListB->SetItemData(pListB->GetCount() - 1, 1, 1);
		}
	}



	strText.Format(_AL("%d"), killa);
	GetDlgItem(bPk ? "Txt_Abuild" : "Txt_Akill")->SetText(strText);
	strText.Format(_AL("%d"), killb);
	GetDlgItem(bPk ? "Txt_Bbuild" : "Txt_Bkill")->SetText(strText);
	if (!bPk)
	{
		strText.Format(_AL("%d"), p->attacker_building_left);
		GetDlgItem("Txt_Abuild")->SetText(strText);
		strText.Format(_AL("%d"), p->defender_building_left);
		GetDlgItem("Txt_Bbuild")->SetText(strText);
	}
	
	GetDlgItem("Lab_Akill")->Show(!bPk);
	GetDlgItem("Lab_Bkill")->Show(!bPk);
	GetDlgItem("Txt_Akill")->Show(!bPk);
	GetDlgItem("Txt_Bkill")->Show(!bPk);




	GetDlgItem("Btn_ScoreA")->Show(!bPk);
	GetDlgItem("Btn_ScoreB")->Show(!bPk);
	GetDlgItem("Lab_Abuild")->SetText(GetGameUIMan()->GetStringFromTable(bPk ? 10259 : 10258));
	GetDlgItem("Lab_Bbuild")->SetText(GetGameUIMan()->GetStringFromTable(bPk ? 10259 : 10258));
	Show(true);
}


void CDlgBattleInfo::UpdateBattleScorePK2013(void *pData)
{
	S2C::cmd_battle_info *p = (S2C::cmd_battle_info*)pData;
	UINT i;
	ACString strText;
// 	PAUILABEL pLab1 = (PAUILABEL)GetDlgItem("Lab_Abuild");
// 	PAUILABEL pLab2 = (PAUILABEL)GetDlgItem("Lab_Bbuild");
// 	if (GetHostPlayer()->GetRebornInfo().size()>0)
// 	{
// 		pLab1->SetText(GetStringFromTable(9182));
// 		pLab2->SetText(GetStringFromTable(9182));
// 	}
// 	else
// 	{
// 		pLab1->SetText(GetStringFromTable(9181));
// 		pLab2->SetText(GetStringFromTable(9181));
// 	}
	PAUILISTBOX pListA = (PAUILISTBOX)GetDlgItem("List_MemberA");
	PAUILISTBOX pListB = (PAUILISTBOX)GetDlgItem("List_MemberB");
	pListA->ResetContent();
	pListB->ResetContent();
	int killa = 0, killb = 0;
	bool bPk = GetHostPlayer()->GetBattleType()==BATTLE_TYPE_NAMED_CRSSVR;
	for(i = 0; i < (UINT)p->player_count; i++ )
	{
		const ACHAR *pszName = GetGameRun()->GetPlayerName(p->info[i].id, false);
		int id = p->info[i].id;
		if (bPk)
		{
			if( !pszName )
			{
				strText.Format(_AL(" \t%d\t%d"), p->info[i].kill, p->info[i].death);
				GetGameSession()->GetPlayerBriefInfo(1, &p->info[i].id, 2);
			}
			else
				strText.Format(_AL("%s\t%d\t%d"), pszName, p->info[i].kill, p->info[i].death);
		}
		else
		{
			if( !pszName )
			{
				strText.Format(_AL(" \t%d\t%d\t%d"), p->info[i].kill, p->info[i].death, p->info[i].battle_score);
				GetGameSession()->GetPlayerBriefInfo(1, &p->info[i].id, 2);
			}
			else
				strText.Format(_AL("%s\t%d\t%d\t%d"), pszName, p->info[i].kill, p->info[i].death, p->info[i].battle_score);
		}
		if( p->info[i].battle_faction == 1 )
		{
			if(bPk)
				killb += p->info[i].death;
			else
				killa += p->info[i].kill;
			pListA->AddString(strText);
			pListA->SetItemData(pListA->GetCount() - 1, id);
			if( !pszName )
				pListA->SetItemData(pListA->GetCount() - 1, 1, 1);
		}
		else
		{
			if(bPk)
				killa += p->info[i].death;
			else
				killb += p->info[i].kill;
			pListB->AddString(strText);
			pListB->SetItemData(pListB->GetCount() - 1, id);
			if( !pszName )
				pListB->SetItemData(pListB->GetCount() - 1, 1, 1);
		}
	}

	strText.Format(_AL("%d"), killb);
	GetDlgItem("Txt_KillB")->SetText(strText);
	strText.Format(_AL("%d"), killa);
	GetDlgItem("Txt_KillA")->SetText(strText);

// 	strText.Format(_AL("%d"), killa);
// 	GetDlgItem(bPk ? "Txt_Abuild" : "Txt_Akill")->SetText(strText);
// 	strText.Format(_AL("%d"), killb);
// 	GetDlgItem(bPk ? "Txt_Bbuild" : "Txt_Bkill")->SetText(strText);
// 	if (!bPk)
// 	{
// 		strText.Format(_AL("%d"), p->attacker_building_left);
// 		GetDlgItem("Txt_Abuild")->SetText(strText);
// 		strText.Format(_AL("%d"), p->defender_building_left);
// 		GetDlgItem("Txt_Bbuild")->SetText(strText);
// 	}
// 	
// 	GetDlgItem("Lab_Akill")->Show(!bPk);
// 	GetDlgItem("Lab_Bkill")->Show(!bPk);
// 	GetDlgItem("Txt_Akill")->Show(!bPk);
// 	GetDlgItem("Txt_Bkill")->Show(!bPk);

// 	GetDlgItem("Btn_ScoreA")->Show(!bPk);
// 	GetDlgItem("Btn_ScoreB")->Show(!bPk);
// 	GetDlgItem("Lab_Abuild")->SetText(GetGameUIMan()->GetStringFromTable(bPk ? 10259 : 10258));
// 	GetDlgItem("Lab_Bbuild")->SetText(GetGameUIMan()->GetStringFromTable(bPk ? 10259 : 10258));
//	Show(true);
}

void CDlgBattleInfo::UpdateBattleScorePK2011(void *pData)
{
	S2C::cmd_battle_info *p = (S2C::cmd_battle_info*)pData;
	PAUILISTBOX pList = (PAUILISTBOX)GetDlgItem("List_Result");
	pList->ResetContent();
	UINT i;
	int index = 0;
	ACString strText;
	for(i = 0; i < (UINT)p->player_count; i++ )
	{
		int id = p->info[i].id;
		const ACHAR *pszName = GetGameRun()->GetPlayerName(id, false);
		if( !pszName )
		{
			strText.Format(_AL(" \t%d\t%d"), p->info[i].kill, p->info[i].death);
			GetGameSession()->GetPlayerBriefInfo(1, &p->info[i].id, 2);
		}
		else
			strText.Format(_AL("%s\t%d\t%d"), pszName, p->info[i].kill, p->info[i].death);

		index = pList->GetCount();
		int j;
		for (j=0; j<pList->GetCount(); ++j)
		{
			if (p->info[i].kill>pList->GetItemData(j, 1) ||
				(p->info[i].kill==pList->GetItemData(j, 1) && p->info[i].death<=pList->GetItemData(j, 2)))
			{
				index = j;
				break;
			}
		}
	
		pList->InsertString(index, strText);
		if( !pszName )
			pList->SetItemData(index, id, 0);
		else
			pList->SetItemData(index, 0, 0);
		pList->SetItemData(index, p->info[i].kill, 1);
		pList->SetItemData(index, p->info[i].death, 2);
		pList->SetItemTextColor(index, p->info[i].battle_faction==1 ? 0xfff13349:0xff33acf1);
	}

	Show(true);
}

void CDlgBattleInfo::UpdateBattleList(void *pData, int type)
{
	m_iBattleType = type;
	BattleGetList_Re *p = (BattleGetList_Re*)pData;
	ACString strText;
	UINT i;
	PAUILISTBOX pList = (PAUILISTBOX)GetDlgItem("List_Member");
	pList->ResetContent();
	int k = 0;
	m_iMaxNum = GetData();
	int numEnterBattleMax = 10;
	if (type==BATTLE_TYPE_CHALLENGE)
	{
		numEnterBattleMax = 1;
	}
	if ( stricmp(m_szName,"Win_BattleList")==0 )
	{
		for(i = 0; i < p->fields.size(); i++ )
		{
			ACString strLine = GetGameUIMan()->GetLine(p->fields[i].gs_id);
			if (p->fields[i].starttime >= GetGame()->GetServerGMTTime())
			{
				strText.Format(ACString(_AL("%s\t%s\t%d/%d\t%d/%d\t")) + GetStringFromTable(1602), 
					strLine, GetStringFromTable(1900 + p->fields[i].map_id), p->fields[i].red_number,m_iMaxNum, p->fields[i].blue_number,m_iMaxNum,
					(p->fields[i].starttime - GetGame()->GetServerGMTTime()) / 60 + 1);
			}
			else
			{
				strText.Format(ACString(_AL("%s\t%s\t%d/%d\t%d/%d\t")) + GetStringFromTable(1012), 
					strLine, GetStringFromTable(1900 + p->fields[i].map_id), p->fields[i].red_number,m_iMaxNum, p->fields[i].blue_number,m_iMaxNum);
			}
			if( p->fields[i].gs_id == GetGameRun()->GetCurLine() )
			{
				pList->InsertString(k, strText);
				pList->SetItemData(k, p->fields[i].gs_id, 0);
				pList->SetItemData(k, p->fields[i].map_id, 1);
				pList->SetItemData(k, p->fields[i].red_number, 2);
				pList->SetItemData(k, p->fields[i].blue_number, 3);
				k++;
			}
			else
			{
				pList->AddString(strText);
				pList->SetItemData(pList->GetCount() - 1, p->fields[i].gs_id, 0);
				pList->SetItemData(pList->GetCount() - 1, p->fields[i].map_id, 1);
				pList->SetItemData(pList->GetCount() - 1, p->fields[i].red_number, 2);
				pList->SetItemData(pList->GetCount() - 1, p->fields[i].blue_number, 3);
			}
		}
	}
	else if ( stricmp(m_szName,"Win_BattleList2")==0 )
	{
		for(i = 0; i < p->fields.size(); i++ )
		{
			ACString strLine = GetGameUIMan()->GetLine(p->fields[i].gs_id);
			if (p->fields[i].starttime >= GetGame()->GetServerGMTTime())
			{
				strText.Format(ACString(_AL("%s\t%s\t%d/%d\t")) + GetStringFromTable(1602), 
					strLine, GetStringFromTable((type==BATTLE_TYPE_REBORN?1900:1850) + p->fields[i].map_id), p->fields[i].red_number+p->fields[i].blue_number,m_iMaxNum,
					(p->fields[i].starttime - GetGame()->GetServerGMTTime()) / 60 + 1);
			}
			else
			{
				strText.Format(ACString(_AL("%s\t%s\t%d/%d\t")) + GetStringFromTable(1012), 
					strLine, GetStringFromTable((type==BATTLE_TYPE_REBORN?1900:1850) + p->fields[i].map_id), p->fields[i].red_number+p->fields[i].blue_number,m_iMaxNum);
			}
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
	}

	m_iEnterTimes = p->enter_times;
	ACHAR szText[200];
	a_sprintf(szText, GetStringFromTable(1011), p->enter_times, numEnterBattleMax);
	GetDlgItem("Txt_Num")->SetText(szText);
	if(GetHostPlayer()->GetSrcServerID() != 0) // 跨服
	{
		GetDlgItem("Title")->Show(false);
		GetDlgItem("Title2")->Show(true);	
	}
	else
	{
		GetDlgItem("Title")->Show(true);
		GetDlgItem("Title2")->Show(false);	
	}
	Show(true);
}

void CDlgBattleInfo::UpdataCrossServerPKBattleScore(void *pData)
{
	S2C::crossserver_battle_score *p = (S2C::crossserver_battle_score*)pData;
	ACString strText;

	strText.Format(_AL("%d"),p->self_kill_num);
	m_pTxtKill->SetText(strText);
	strText.Format(_AL("%d"),p->self_death_num);
	m_pTxtDead->SetText(strText);
	if (GetHostPlayer()->GetBattleCamp() == GP_BATTLE_CAMP_INVADER)
	{
		strText.Format(_AL("%d"),p->attacker_kill_num);
		m_pTxtKillOwn->SetText(strText);
		strText.Format(_AL("%d"),p->defender_kill_num);
		m_pTxtKillEnemy->SetText(strText);
		m_pLabKillOwn->SetColor(A3DCOLORRGB(255, 0, 0));
		m_pTxtKillOwn->SetColor(A3DCOLORRGB(255, 0, 0));
		m_pLabKill->SetColor(A3DCOLORRGB(255, 0, 0));
		m_pTxtKill->SetColor(A3DCOLORRGB(255, 0, 0));
		m_pLabKillEnemy->SetColor(A3DCOLORRGB(0, 128, 255));
		m_pTxtKillEnemy->SetColor(A3DCOLORRGB(0, 128, 255));
		m_pLabDead->SetColor(A3DCOLORRGB(0, 128, 255));
		m_pTxtDead->SetColor(A3DCOLORRGB(0, 128, 255));
	}
	else
	{
		strText.Format(_AL("%d"),p->defender_kill_num);
		m_pTxtKillOwn->SetText(strText);
		strText.Format(_AL("%d"),p->attacker_kill_num);
		m_pTxtKillEnemy->SetText(strText);
		m_pLabKillOwn->SetColor(A3DCOLORRGB(0, 128, 255));
		m_pTxtKillOwn->SetColor(A3DCOLORRGB(0, 128, 255));
		m_pLabKill->SetColor(A3DCOLORRGB(0, 128, 255));
		m_pTxtKill->SetColor(A3DCOLORRGB(0, 128, 255));
		m_pLabKillEnemy->SetColor(A3DCOLORRGB(255, 0, 0));
		m_pTxtKillEnemy->SetColor(A3DCOLORRGB(255, 0, 0));
		m_pLabDead->SetColor(A3DCOLORRGB(255, 0, 0));
		m_pTxtDead->SetColor(A3DCOLORRGB(255, 0, 0));

	}
}


void CDlgBattleInfo::UpdataCrossServerBattleScore(void *pData)
{
	S2C::crossserver_battle_score *p = (S2C::crossserver_battle_score*)pData;
	ACString strText;

	if(m_iJoinType==2)
	{
		strText.Format(_AL("%d"),p->defender_kill_num + p->attacker_off_line_score);
		m_pTxtScore01->SetText(strText);
		strText.Format(_AL("%d"),p->defender_off_line_score + p->attacker_kill_num);
		m_pTxtScore02->SetText(strText);
		
		m_nSlefAttacker_count = (int)p->attacker_count;
		strText.Format(_AL("%d"),m_nSlefAttacker_count);
		m_pTxtPlayer02->SetText(strText);
		strText.Format(_AL("%d"),(int)p->defender_count);
		m_pTxtPlayer01->SetText(strText);
		
		strText.Format(_AL("%d"),p->attacker_kill_num);
		m_pTxtKillEnemy->SetText(strText);
		strText.Format(_AL("%d"),p->defender_kill_num);
		m_pTxtKillOwn->SetText(strText);
		
		
		strText.Format(_AL("%d"),p->defender_off_line_score);
		m_pTxtOffLineEnemy->SetText(strText);
		strText.Format(_AL("%d"),p->attacker_off_line_score);
		m_pTxtOffLineOwn->SetText(strText);


		m_pTxtScore01->SetColor(A3DCOLORRGB(0, 128, 255));
		m_pTxtScore02->SetColor(A3DCOLORRGB(255, 0, 0));
		m_pTxtPlayer01->SetColor(A3DCOLORRGB(0, 128, 255));
		GetDlgItem("Lab_Player01")->SetColor(A3DCOLORRGB(0, 128, 255));
		m_pTxtPlayer02->SetColor(A3DCOLORRGB(255, 0, 0));
		GetDlgItem("Lab_Player02")->SetColor(A3DCOLORRGB(255, 0, 0));
		m_pTxtKillOwn->SetColor(A3DCOLORRGB(0, 128, 255));
		GetDlgItem("Lab_Kill01")->SetColor(A3DCOLORRGB(0, 128, 255));
		m_pTxtKillEnemy->SetColor(A3DCOLORRGB(255, 0, 0));
		GetDlgItem("Lab_Kill02")->SetColor(A3DCOLORRGB(255, 0, 0));
		m_pTxtOffLineOwn->SetColor(A3DCOLORRGB(255, 0, 0));
		GetDlgItem("Lab_Punish01")->SetColor(A3DCOLORRGB(0, 128, 255));
		m_pTxtOffLineEnemy->SetColor(A3DCOLORRGB(0, 128, 255));
		GetDlgItem("Lab_Punish02")->SetColor(A3DCOLORRGB(255, 0, 0));
	}
	else
	{		
		strText.Format(_AL("%d"),p->defender_kill_num + p->attacker_off_line_score);
		m_pTxtScore02->SetText(strText);
		strText.Format(_AL("%d"),p->defender_off_line_score + p->attacker_kill_num);
		m_pTxtScore01->SetText(strText);
		
		m_nSlefAttacker_count = (int)p->attacker_count;
		strText.Format(_AL("%d"),m_nSlefAttacker_count);
		m_pTxtPlayer01->SetText(strText);
		strText.Format(_AL("%d"),(int)p->defender_count);
		m_pTxtPlayer02->SetText(strText);
		
		strText.Format(_AL("%d"),p->attacker_kill_num);
		m_pTxtKillOwn->SetText(strText);
		strText.Format(_AL("%d"),p->defender_kill_num);
		m_pTxtKillEnemy->SetText(strText);
		
		
		strText.Format(_AL("%d"),p->attacker_off_line_score);
		m_pTxtOffLineEnemy->SetText(strText);
		strText.Format(_AL("%d"),p->defender_off_line_score);
		m_pTxtOffLineOwn->SetText(strText);

		m_pTxtScore01->SetColor(A3DCOLORRGB(255, 0, 0));
		m_pTxtScore02->SetColor(A3DCOLORRGB(0, 128, 255));
		m_pTxtPlayer01->SetColor(A3DCOLORRGB(255, 0, 0));
		GetDlgItem("Lab_Player01")->SetColor(A3DCOLORRGB(255, 0, 0));
		m_pTxtPlayer02->SetColor(A3DCOLORRGB(0, 128, 255));
		GetDlgItem("Lab_Player02")->SetColor(A3DCOLORRGB(0, 128, 255));
		m_pTxtKillOwn->SetColor(A3DCOLORRGB(255, 0, 0));
		GetDlgItem("Lab_Kill01")->SetColor(A3DCOLORRGB(255, 0, 0));
		m_pTxtKillEnemy->SetColor(A3DCOLORRGB(0, 128, 255));
		GetDlgItem("Lab_Kill02")->SetColor(A3DCOLORRGB(0, 128, 255));
		m_pTxtOffLineOwn->SetColor(A3DCOLORRGB(0, 128, 255));
		GetDlgItem("Lab_Punish01")->SetColor(A3DCOLORRGB(255, 0, 0));
		m_pTxtOffLineEnemy->SetColor(A3DCOLORRGB(255, 0, 0));
		GetDlgItem("Lab_Punish02")->SetColor(A3DCOLORRGB(0, 128, 255));
	}

	strText.Format(_AL("%d"),p->self_kill_num);
	m_pTxtKill->SetText(strText);
	strText.Format(_AL("%d"),p->self_death_num);
	m_pTxtDead->SetText(strText);
	m_iMinPlayerDeduceScore = p->min_player_deduct_score;
}

void CDlgBattleInfo::UpDataPKScoreBattleInfo(void* pData)
{
	S2C::crossserver_battle_score *p = (S2C::crossserver_battle_score*)pData;
	ACString strText;
	
	strText.Format(_AL("%d"), p->defender_off_line_score + p->attacker_kill_num);
	GetDlgItem("Txt_ScoreA")->SetText(strText);
	strText.Format(_AL("%d"), p->defender_kill_num + p->attacker_off_line_score);
	GetDlgItem("Txt_ScoreB")->SetText(strText);
	
	strText.Format(_AL("%d"), p->attacker_off_line_score);
	GetDlgItem("Txt_PunishA")->SetText(strText);
	strText.Format(_AL("%d"), p->defender_off_line_score);
	GetDlgItem("Txt_PunishB")->SetText(strText);
	
	strText.Format(_AL("%d"), (int)p->attacker_count);
	GetDlgItem("Txt_NumA")->SetText(strText);
	strText.Format(_AL("%d"), (int)p->defender_count);
	GetDlgItem("Txt_NumB")->SetText(strText);	
}


void CDlgBattleInfo::UpDateBattleOffLineInfo(void* pData)
{	
	ACString strText;
	PAUILISTBOX pListA = (PAUILISTBOX)GetDlgItem("Lst_PunishA");
	PAUILISTBOX pListB = (PAUILISTBOX)GetDlgItem("Lst_PunishB");
	pListA->ResetContent();
	pListB->ResetContent();
	S2C::battle_off_line_info *p = (S2C::battle_off_line_info*)pData;
	UINT i;
	for(i = 0; i < (UINT)p->off_line_count; i++ )
	{
		int nTmMin = p->info[i].ntime/60;
		int nTmSec = p->info[i].ntime%60;
		strText.Format(_AL("%d:%02d\t%d\t%d"), nTmMin,nTmSec,p->info[i].off_line, p->info[i].off_line_score);
		if( p->info[i].battle_faction == 1)
		{
			pListA->AddString(strText);
			pListA->SetItemData(pListA->GetCount() - 1, p->info[i].ntime);
		}
		else
		{
			pListB->AddString(strText);
			pListB->SetItemData(pListB->GetCount() - 1,p->info[i].ntime);
		}
	}
}



AUI_BEGIN_COMMAND_MAP(CDlgBattlePkScore, CDlgBase)

AUI_ON_COMMAND("Btn_Expand",		OnCommandExpand)

AUI_END_COMMAND_MAP()

int CDlgBattlePkScore::m_iTimeEnd = 0;

CDlgBattlePkScore::CDlgBattlePkScore()
{
}

CDlgBattlePkScore::~CDlgBattlePkScore()
{
}

bool CDlgBattlePkScore::OnInitDialog()
{
	m_pLabScoreStage	= (PAUILABEL)GetDlgItem("Lab_FinalScore");
	m_pLabStageNo		= (PAUILABEL)GetDlgItem("Lab_No");
	m_pLabScoreCurRound	= (PAUILABEL)GetDlgItem("Lab_Survive");
	m_pLabTime			= (PAUILABEL)GetDlgItem("Lab_TimeLeft");
	AString strName;
	for (int i=0; i<TOTAL_ROUND_NUM2011; ++i)
	{
		strName.Format("Lab_RedScore%d", i+1);
		m_pLabRedScore[i]	= (PAUILABEL)GetDlgItem(strName);
		strName.Format("Lab_BlueScore%d", i+1);
		m_pLabBlueScore[i]	= (PAUILABEL)GetDlgItem(strName);
	}
	m_pImgState			= (PAUIIMAGEPICTURE)GetDlgItem("Img_Status");
	return true;
}

void CDlgBattlePkScore::OnTick()
{
	ACString strText;
	int nTime = m_iTimeEnd - GetGame()->GetServerGMTTime();
	if( nTime > 0 )
	{
		strText.Format(_AL("%d:%02d"), nTime/60, nTime%60);
		m_pLabTime->SetText(strText);
	}
	else
		m_pLabTime->SetText(_AL("0:00"));
}

void CDlgBattlePkScore::OnCommandExpand(const char * szCommand)
{
	PAUIDIALOG pDlgSwitch;
	if (stricmp(m_szName, "Win_Battle6V6ScoreBoardL")==0)
		pDlgSwitch = GetGameUIMan()->GetDialog("Win_Battle6V6ScoreBoardS");
	else
		pDlgSwitch = GetGameUIMan()->GetDialog("Win_Battle6V6ScoreBoardL");

	Show(false);
	pDlgSwitch->AlignTo(this, AUIDIALOG_ALIGN_INSIDE, AUIDIALOG_ALIGN_LEFT,
			AUIDIALOG_ALIGN_INSIDE, AUIDIALOG_ALIGN_TOP);
	pDlgSwitch->Show(true);
}

void CDlgBattlePkScore::UpdateBattleScore2011OnLine(void *pData)
{
	using namespace S2C;
	crossserver_team_battle_info* pInfo = (crossserver_team_battle_info*)pData;
	m_iTimeEnd = pInfo->cur_state_end_time;
	ACString strText;
	int i;
	int attScore = 0;
	int defScore = 0;
	for (i=0; i<pInfo->history_round_count; ++i)
	{	
		if (pInfo->score_info[i].attacker_score>pInfo->score_info[i].defender_score)
			attScore++;
		else if(pInfo->score_info[i].attacker_score<pInfo->score_info[i].defender_score)
			defScore++;
	}
	// 这个函数会调用多次，但只有Win_Battle6V6ScoreBoardL有m_pLabRedScore[0]
	if (m_pLabRedScore[0])
	{
		for (i=0; i<pInfo->history_round_count; ++i)
		{	
			strText.Format(_AL("%d"), pInfo->score_info[i].attacker_score);
			m_pLabRedScore[i]->SetText(strText);
			strText.Format(_AL("%d"), pInfo->score_info[i].defender_score);
			m_pLabBlueScore[i]->SetText(strText);
		}
		for (i=pInfo->history_round_count; i<TOTAL_ROUND_NUM2011; ++i)
		{
			m_pLabRedScore[i]->SetText(_AL(""));
			m_pLabBlueScore[i]->SetText(_AL(""));
		}
/* 每局结束弹出比分界面，考虑到可能的问题和不太重要，暂时删掉
   需要用的话进入战场需要调用m_pImgState->FixFrame(0);
		if (pInfo->battle_state==0 && m_pImgState->GetFixFrame()==1)
		{
			PAUIDIALOG pDlg = GetGameUIMan()->GetDialog("Win_PKRecord2011");
			int resultString = pInfo->score_info[pInfo->history_round_count-1].attacker_score==pInfo->score_info[pInfo->history_round_count-1].defender_score
				? 8480 : (pInfo->score_info[pInfo->history_round_count-1].attacker_score>pInfo->score_info[pInfo->history_round_count-1].defender_score ? 8003 : 8004);
			pDlg->GetDlgItem("Lab_Victor")->SetText(GetGameUIMan()->GetStringFromTable(resultString));
			if (GetGameRun()->GetPlayerName(pInfo->best_killer_id, false))
			{
				pDlg->GetDlgItem("Txt_Winner")->SetText(GetGameRun()->GetPlayerName(pInfo->best_killer_id, false));
			}
			else
			{
				pDlg->GetDlgItem("Txt_Winner")->SetText(_AL(""));
				pDlg->GetDlgItem("Txt_Winner")->SetData(pInfo->best_killer_id, "VIP_Name");
				GetGameSession()->GetPlayerBriefInfo(1, &pInfo->best_killer_id, 2);
			}
			strText.Format(_AL("%d"), attScore);
			pDlg->GetDlgItem("Txt_RedScore1")->SetText(strText);
			strText.Format(_AL("%d"), defScore);
			pDlg->GetDlgItem("Txt_BlueScore1")->SetText(strText);
			strText.Format(_AL("%d"), pInfo->score_info[pInfo->history_round_count-1].attacker_score);
			pDlg->GetDlgItem("Txt_RedScore2")->SetText(strText);
			strText.Format(_AL("%d"), pInfo->score_info[pInfo->history_round_count-1].defender_score);
			pDlg->GetDlgItem("Txt_BlueScore2")->SetText(strText);
			pDlg->Show(true);
		}
*/	}
	m_pImgState->FixFrame(pInfo->battle_state);
	strText.Format(_AL("^f13349%d ^ffffff: ^33acf1%d"), attScore, defScore);
	m_pLabScoreStage->SetText(strText);
	if(pInfo->cur_state_end_time==0)	// 战斗已经结束
	{
		m_pLabStageNo->SetText(GetGameUIMan()->GetStringFromTable(10610+pInfo->history_round_count));
		strText.Format(_AL("^f13349%d ^00ff84: ^33acf1%d"), pInfo->score_info[pInfo->history_round_count-1].attacker_score, pInfo->score_info[pInfo->history_round_count-1].defender_score);
		m_pLabScoreCurRound->SetText(strText);
	}
	else
	{
		m_pLabStageNo->SetText(GetGameUIMan()->GetStringFromTable(10610+pInfo->history_round_count+1));
		strText.Format(_AL("^f13349%d ^00ff84: ^33acf1%d"), pInfo->attacker_score, pInfo->defender_score);
		m_pLabScoreCurRound->SetText(strText);
	}
}



const int	CDlgBattlePkScoreResult2011::SHOW_TIME = 30000;
const int	CDlgBattlePkScoreResult2011::APPEAR_TIME = 300;

CDlgBattlePkScoreResult2011::CDlgBattlePkScoreResult2011()
{
}

CDlgBattlePkScoreResult2011::~CDlgBattlePkScoreResult2011()
{
}

void CDlgBattlePkScoreResult2011::OnShowDialog()
{
	m_iShowTime = GetTickCount();
	m_bNormal = false;
	m_iVip = GetDlgItem("Txt_Winner")->GetData("VIP_Name");
}

void CDlgBattlePkScoreResult2011::OnTick()
{
	// 更新名字显示
	ACString strName = GetDlgItem("Txt_Winner")->GetText();
	if (strName.IsEmpty() && GetGameRun()->GetPlayerName(m_iVip, false))
	{
		GetDlgItem("Txt_Winner")->SetText(GetGameRun()->GetPlayerName(m_iVip, false));
	}
	// 5s后消失
	DWORD dwNow = GetTickCount();
	if (dwNow>(m_iShowTime + SHOW_TIME))
	{
		Show(false);
	}
	else if (dwNow<(m_iShowTime + APPEAR_TIME))
	{
		SetScale((dwNow - m_iShowTime)/(float)APPEAR_TIME);
	}
	else if (!m_bNormal)
	{
		SetScale(1.0f);
		m_bNormal = true;
	}
}

