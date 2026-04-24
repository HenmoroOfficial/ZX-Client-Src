// Filename	: DlgBAttleTeam6v6Score.cpp
// Creator	: Fu Chonggang
// Date		: 2012.8.8

#include "DlgBattleTeam6v6Score.h"
#include "AUI\\AUILabel.h"
#include "AUI\\AUIListBox.h"
#include "EC_GameUIMan.h"
#include "EC_BattleDungeon.h"
#include "EC_Game.h"
#include "EC_GameRun.h"
#include "EC_GameUIMisc.h"
#include "EC_GPDataType.h"
#include "EC_World.h"
#include "EC_GameSession.h"
#include "EC_ManPlayer.h"
#include "EC_Player.h"
#include "EC_HostPlayer.h"

#define new A_DEBUG_NEW

CDlgBattleTeam6v6Score::CDlgBattleTeam6v6Score()
{
}

CDlgBattleTeam6v6Score::~CDlgBattleTeam6v6Score()
{
}

bool CDlgBattleTeam6v6Score::OnInitDialog()
{
	m_pLabTime = (PAUILABEL)GetDlgItem("Txt_TimeLeft");
	PAUILABEL pLab = (PAUILABEL)GetDlgItem("Txt_SelfKill");
	pLab->SetText(_AL("0"));
	return true;
}

void CDlgBattleTeam6v6Score::OnTick()
{
	ACString strTime = GetTimeText(CECBattleDungeonMan::getSingletonPtr()->m_iEndTime-GetGame()->GetServerGMTTime());
	m_pLabTime->SetText(strTime);
}

AUI_BEGIN_COMMAND_MAP(CDlgBattleTeam6v6Record, CDlgBase)

AUI_ON_COMMAND("Btn_Exit",			OnCommandLeave)
AUI_ON_COMMAND("Btn_SortKill1",		OnCommandSortByKill)
AUI_ON_COMMAND("Btn_SortKill2",		OnCommandSortByDead)

AUI_END_COMMAND_MAP()

CDlgBattleTeam6v6Record::CDlgBattleTeam6v6Record()
{
}

CDlgBattleTeam6v6Record::~CDlgBattleTeam6v6Record()
{
}

bool CDlgBattleTeam6v6Record::OnInitDialog()
{
	m_pLabTime = (PAUILABEL)GetDlgItem("Txt_TimeLeft");
	m_pLstRecord = (PAUILISTBOX)GetDlgItem("List_Record");
	PAUILABEL pLab = (PAUILABEL)GetDlgItem("Txt_GuildAkill");
	pLab->SetText(_AL("0"));
	pLab = (PAUILABEL)GetDlgItem("Txt_GuildBkill");
	pLab->SetText(_AL("0"));
	return true;
}

void CDlgBattleTeam6v6Record::OnShowDialog()
{
	int i;
	for (i=0; i<m_pLstRecord->GetCount(); ++i)
	{
		if (m_pLstRecord->GetItemData(i, 0, "role_id")!=0)
		{
			if (GetGameRun()->GetPlayerName(m_pLstRecord->GetItemData(i, 0, "role_id"), false))
			{
				ACString strText;
				strText.Format(_AL("%s%s\t%d\t%d"), m_pLstRecord->GetItemData(i, 3, "faction")==1 ? _AL("^ef8080"):_AL("^8080ef"), 
					GetGameRun()->GetPlayerName(m_pLstRecord->GetItemData(i, 0, "role_id"), false), m_pLstRecord->GetItemData(i, 1), m_pLstRecord->GetItemData(i, 2));
				m_pLstRecord->SetText(i, strText);
				m_pLstRecord->SetItemData(i, 0, 0, "role_id");
			}
		}
	}

	PAUILABEL pLab = (PAUILABEL)GetDlgItem("Lab_FirstName");
	if (pLab->GetData("role_id")!=0)
	{
		if (GetGameRun()->GetPlayerName(pLab->GetData("role_id"), false))
		{
			pLab->SetText(GetGameRun()->GetPlayerName(pLab->GetData("role_id"), false));
			pLab->SetData(0, "role_id");
		}
	}
}

void CDlgBattleTeam6v6Record::OnTick()
{
	ACString strTime = GetTimeText(CECBattleDungeonMan::getSingletonPtr()->m_iEndTime-GetGame()->GetServerGMTTime());
	m_pLabTime->SetText(strTime);
}

void CDlgBattleTeam6v6Record::OnCommandLeave(const char * szCommand)
{
	GetGameUIMan()->MessageBox("Game_DungeonLeave", GetGameUIMan()->GetStringFromTable(12263), MB_YESNO);
}

void CDlgBattleTeam6v6Record::OnCommandSortByKill(const char * szCommand)
{
	static bool bAscent = false;
	bAscent = !bAscent;
	m_pLstRecord->SortItemsByData(bAscent ? AUILISTBOX_SORT_ASCENT:AUILISTBOX_SORT_DESCENT, AUILISTBOX_DATASORTTYPE_INT, 1);
}

void CDlgBattleTeam6v6Record::OnCommandSortByDead(const char * szCommand)
{
	static bool bAscent = false;
	bAscent = !bAscent;
	m_pLstRecord->SortItemsByData(bAscent ? AUILISTBOX_SORT_ASCENT:AUILISTBOX_SORT_DESCENT, AUILISTBOX_DATASORTTYPE_INT, 2);
}

int CDlgBattleTeam6v6Record::CommonMessage(AString strMsg, WPARAM wParam, LPARAM lParam)
{
	if (strMsg=="score")
	{
		PAUIDIALOG pDlgScore = GetGameUIMan()->GetDialog("Win_Battle_Dungeon_6v6_Score");
		S2C::cmd_collision_raid_score* pCmd = (S2C::cmd_collision_raid_score*)wParam;
		ACString strText;
		strText.Format(_AL("%d"), CECBattleDungeonMan::getSingletonPtr()->m_iFaction==1 ? pCmd->attacker_score:pCmd->defender_score);
		PAUILABEL pLab = (PAUILABEL)GetDlgItem("Txt_GuildAkill");
		pLab->SetText(strText);
		pLab = (PAUILABEL)pDlgScore->GetDlgItem("Txt_GuildAkill");
		pLab->SetText(strText);
		strText.Format(_AL("%d"), CECBattleDungeonMan::getSingletonPtr()->m_iFaction==1 ? pCmd->defender_score:pCmd->attacker_score);
		pLab = (PAUILABEL)GetDlgItem("Txt_GuildBkill"); 
		pLab->SetText(strText);
		pLab = (PAUILABEL)pDlgScore->GetDlgItem("Txt_GuildBkill");
		pLab->SetText(strText);

		if (pCmd->count>0)
		{
			int i, j;
			for (i=0; i<pCmd->count-1; ++i)
			{
				for (j=i+1; j<pCmd->count; ++j)
				{
					if (pCmd->member_info[i].kill_count<pCmd->member_info[j].kill_count)
					{
						a_Swap(pCmd->member_info[i].roleid, pCmd->member_info[j].roleid);
						a_Swap(pCmd->member_info[i].faction, pCmd->member_info[j].faction);
						a_Swap(pCmd->member_info[i].kill_count, pCmd->member_info[j].kill_count);
						a_Swap(pCmd->member_info[i].dead_count, pCmd->member_info[j].dead_count);
					}
				}
			}
			const ACHAR *szName = GetGameRun()->GetPlayerName(pCmd->max_killer_id, false);
			pLab = (PAUILABEL)GetDlgItem("Lab_FirstName");
			if (szName)
			{
				pLab->SetText(szName);
				pLab->SetData(0, "role_id");
			}
			else
			{
				pLab->SetText(_AL(""));
				pLab->SetData(pCmd->max_killer_id, "role_id");
			}
			pLab = (PAUILABEL)GetDlgItem("Txt_FirstKill");
			strText.Format(_AL("%d"), pCmd->member_info[0].kill_count);
			pLab->SetText(strText);
			m_pLstRecord->ResetContent();
			for (i=0; i<pCmd->count; ++i)
			{
				const ACHAR *szName = GetGameRun()->GetPlayerName(pCmd->member_info[i].roleid, false);
				strText.Format(_AL("%s%s\t%d\t%d"), pCmd->member_info[i].faction==1 ? _AL("^ef8080"):_AL("^8080ef"), szName?szName:_AL(" "), pCmd->member_info[i].kill_count, pCmd->member_info[i].dead_count);
				m_pLstRecord->AddString(strText);
				m_pLstRecord->SetItemData(m_pLstRecord->GetCount()-1, pCmd->member_info[i].kill_count, 1);
				m_pLstRecord->SetItemData(m_pLstRecord->GetCount()-1, pCmd->member_info[i].dead_count, 2);
				m_pLstRecord->SetItemData(m_pLstRecord->GetCount()-1, pCmd->member_info[i].faction, 3, "faction");
				if (!szName)
				{
					m_pLstRecord->SetItemData(m_pLstRecord->GetCount()-1, pCmd->member_info[i].roleid, 0, "role_id");
					GetGameSession()->GetPlayerBriefInfo(1, &pCmd->member_info[i].roleid, 2);
				}
				if (pCmd->member_info[i].roleid==GetHostPlayer()->GetCharacterID())
				{					
					pLab = (PAUILABEL)pDlgScore->GetDlgItem("Txt_SelfKill");
					strText.Format(_AL("%d"), pCmd->member_info[i].kill_count);
					pLab->SetText(strText);
				}
			}
		}
	}
	return 0;
}
