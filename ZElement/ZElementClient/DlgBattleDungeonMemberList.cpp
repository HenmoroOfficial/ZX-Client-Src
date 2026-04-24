#include "AUI\\AUIListBox.h"
#include "AUI\\AUILabel.h"
#include "DlgBattleDungeonMemberList.h"
#include "DlgBattleFactionWarControl.h"
#include "EC_SendC2SCmds.h"
#include "EC_BattleDungeon.h"
#include "EC_GPDataType.h"
#include "EC_HostPlayer.h"
#include "EC_GameUIMan.h"
#include "EC_GameUIMisc.h"
#include "EC_GameRun.h"
#include "EC_Game.h"
#include "EC_GameSession.h"
#include "EC_BattleDungeonTower.h"

#include "ExpTypes.h"
#include "elementdataman.h"

#define new A_DEBUG_NEW

AUI_BEGIN_COMMAND_MAP(CDlgBattleDungeonMemberList, CDlgBase)

AUI_ON_COMMAND("Btn_Leave",		OnCommandLeave)
AUI_ON_COMMAND("Btn_Kickout",	OnCommandKick)

AUI_END_COMMAND_MAP()

AUI_BEGIN_EVENT_MAP(CDlgBattleDungeonMemberList, CDlgBase)

AUI_ON_EVENT("Lst_1",			WM_RBUTTONUP,	OnEventRButtonUp)

AUI_END_EVENT_MAP()


CDlgBattleDungeonMemberList::CDlgBattleDungeonMemberList()
{
}

CDlgBattleDungeonMemberList::~CDlgBattleDungeonMemberList()
{
}

bool CDlgBattleDungeonMemberList::OnInitDialog()
{
	m_pLabTimeLeft = (PAUILABEL)GetDlgItem("Txt_TimeLeft");
	m_pLstMember = (PAUILISTBOX)GetDlgItem("Lst_1");

	return true;
}

void CDlgBattleDungeonMemberList::OnShowDialog()
{
	if (!CECBattleDungeonMan::getSingletonPtr()->GetCurBattle())
	{
		elementdataman* pDB = GetGame()->GetElementDataMan();
		DATA_TYPE DataType = DT_INVALID;
		int tid = pDB->get_first_data_id(ID_SPACE_CONFIG, DataType);
		while (tid)
		{
			const void* pData = pDB->get_data_ptr(tid, ID_SPACE_CONFIG, DataType);
			
			if (DataType == DT_TRANSCRIPTION_CONFIG)
			{
				const TRANSCRIPTION_CONFIG* pConf = (const TRANSCRIPTION_CONFIG*)pData;
				if (pConf->map_id==GetHostPlayer()->GetRaidBattleInfo())
				{
					CECBattleDungeonMan::getSingletonPtr()->Init(tid);
					break;
				}
			}
			
			//	Get next item
			tid = pDB->get_next_data_id(ID_SPACE_CONFIG, DataType);
		}

	}
	PAUILABEL pLab = (PAUILABEL)GetDlgItem("Title");
	if (CECBattleDungeonMan::getSingletonPtr()->GetCurBattle())
	{
		pLab->SetText(CECBattleDungeonMan::getSingletonPtr()->GetCurBattle()->m_pConfigData->name);
	}
	else	// 爬塔副本
		pLab->SetText(GetHostPlayer()->GetBattleDungeonTower()->GetConfig()->name);
}

void CDlgBattleDungeonMemberList::OnTick()
{
	GetDlgItem("Btn_Kickout")->Enable(GetHostPlayer()->GetCharacterID()==CECBattleDungeonMan::getSingletonPtr()->m_iLeaderId
		&& m_pLstMember->GetCurSel()>=0 && m_pLstMember->GetCurSel()<m_pLstMember->GetCount()
		&& (int)m_pLstMember->GetItemData(m_pLstMember->GetCurSel())!=GetHostPlayer()->GetCharacterID());

	ACString strTime = GetTimeText(CECBattleDungeonMan::getSingletonPtr()->m_iEndTime-GetGame()->GetServerGMTTime());
	m_pLabTimeLeft->SetText(strTime);
	int i;
	// 刷新玩家名字
	for(i = 0; i < m_pLstMember->GetCount(); i++)
		if( m_pLstMember->GetItemData(i, 1) != 0 )
		{
			if( GetGameRun()->GetPlayerName(m_pLstMember->GetItemData(i, 1), false) )
			{
				ACString strText = m_pLstMember->GetText(i);
				strText = GetGameRun()->GetPlayerName(m_pLstMember->GetItemData(i), true) + strText.Mid(1);
				m_pLstMember->SetText(i, strText);
				m_pLstMember->SetItemData(i, 0, 1);
			}
		}
}

void CDlgBattleDungeonMemberList::OnCommandLeave(const char * szCommand)
{
	GetGameUIMan()->MessageBox("Game_DungeonLeave", GetGameUIMan()->GetStringFromTable(12263), MB_YESNO);
}

void CDlgBattleDungeonMemberList::OnCommandKick(const char * szCommand)
{
	if(m_pLstMember->GetCurSel()<0 && m_pLstMember->GetCurSel()>=m_pLstMember->GetCount())
		return;
	ACString strText;
	ACString strName = GetGameRun()->GetPlayerName(m_pLstMember->GetItemData(m_pLstMember->GetCurSel()), false);
	strText.Format(GetGameUIMan()->GetStringFromTable(15017), strName);
	PAUIDIALOG pDlg;
	GetGameUIMan()->MessageBox("Game_DungeonKick", strText, MB_YESNO, A3DCOLORRGBA(255, 255, 255, 160), &pDlg);
	pDlg->SetData(m_pLstMember->GetItemData(m_pLstMember->GetCurSel()));
}

void CDlgBattleDungeonMemberList::OnEventRButtonUp(WPARAM wParam, LPARAM lParam, AUIObject * pObj)
{
	if (m_pLstMember->GetCurSel()>=0 && m_pLstMember->GetCurSel()<m_pLstMember->GetCount())
	{
		int x = GET_X_LPARAM(lParam);
		int y = GET_Y_LPARAM(lParam);
		GetGameUIMan()->PopupPlayerContextMenu(m_pLstMember->GetItemData(m_pLstMember->GetCurSel()), x, y);	
	}
}

int CDlgBattleDungeonMemberList::CommonMessage(AString strMsg, WPARAM wParam, LPARAM lParam)
{
	if (strMsg=="member_list")
	{
		S2C::raid_player_info* pCmd = (S2C::raid_player_info*)wParam;
		m_pLstMember->ResetContent();
		int i;
		ACString strText;
		for (i=0; i<pCmd->player_count; ++i)
		{
			const ACHAR *pszName = GetGameRun()->GetPlayerName(pCmd->player_info[i].player_id, false);
			if( pszName )
			{
				strText.Format(_AL("%s\t%s\t%s%d"), pszName, GetGameUIMan()->GetStringFromTable(PROFESSION_START + pCmd->player_info[i].profession),
				pCmd->player_info[i].sec_level>0?GetGameUIMan()->GetStringFromTable(7012):_AL(" "), pCmd->player_info[i].player_level);
			}
			else
			{
				strText.Format(_AL(" \t%s\t%s%d"), GetGameUIMan()->GetStringFromTable(PROFESSION_START + pCmd->player_info[i].profession),
				pCmd->player_info[i].sec_level>0?GetGameUIMan()->GetStringFromTable(7012):_AL(" "), pCmd->player_info[i].player_level);
				GetGameSession()->GetPlayerBriefInfo(1, &pCmd->player_info[i].player_id, 2);
			}
			m_pLstMember->AddString(strText);
			m_pLstMember->SetItemData(i, pCmd->player_info[i].player_id);
			if (!pszName)
			{
				m_pLstMember->SetItemData(i, pCmd->player_info[i].player_id, 1);
			}
		}
		if (!IsShow())
		{
			Show(true);
		}		
		
		if (CECBattleDungeonMan::getSingletonPtr()->GetCurBattle())
		{
			strText.Format(_AL("%d/%d"), m_pLstMember->GetCount(), CECBattleDungeonMan::getSingletonPtr()->GetCurBattle()->m_pConfigData->max_player_num);
		}
		else	// 爬塔副本
			strText = _AL("1/1");
		PAUILABEL pLabPlayerNum = (PAUILABEL)GetDlgItem("Txt_PlayerNum");
		pLabPlayerNum->SetText(strText);

        GetDlgItem("Btn_Kickout")->Show(true);
		//帮战副本中，将“移除副本”按钮隐藏
		if(CECBattleDungeonMan::getSingletonPtr()->GetCurBattle())
		{
			if(CECBattleDungeonMan::getSingletonPtr()->GetCurBattle()->m_pConfigData->map_id == CDlgBattleFactionWarControl::GetFactionPKRaidMapId())
			{
			   GetDlgItem("Btn_Kickout")->Show(false);
			}
		}
	
	}
	return 0;
}