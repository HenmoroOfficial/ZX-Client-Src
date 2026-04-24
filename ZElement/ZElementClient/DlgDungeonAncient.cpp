#pragma once

#include "EC_GameUIMan.h"
#include "DlgDungeonAncient.h" 
#include "EC_GameSession.h"
#include "EC_GPDataType.h"
#include "AUI\\AUIImagePicture.h"
#include "EC_Game.h"
#include "elementdataman.h"
#include "EC_IvtrItem.h"
#include "A2DSprite.h"
#include "AUI\\AUICTranslate.h"
#include "AFI.h"
#include "EC_HostPlayer.h"
#define new A_DEBUG_NEW
 
AUI_BEGIN_COMMAND_MAP(CDlgAncientMini, CDlgBase)

AUI_ON_COMMAND("Btn_Max",		OnCommand_Open)

AUI_END_COMMAND_MAP()

AUI_BEGIN_EVENT_MAP(CDlgAncientMini, CDlgBase)



AUI_END_EVENT_MAP()

CDlgAncientMini::CDlgAncientMini()
{
}

CDlgAncientMini::~CDlgAncientMini()
{
}

void CDlgAncientMini::OnCommand_Open(const char * szCommand)
{		
	GetGameUIMan()->GetDialog("Win_Dungeon_Ling")->Show(!GetGameUIMan()->GetDialog("Win_Dungeon_Ling")->IsShow());
}

bool CDlgAncientMini::Tick()
{
	if(GetHostPlayer()->GetRaidBattleInfo() != 538)
		Show(false);
	
	return true;
}

bool CDlgAncientMini::OnInitDialog()
{

	return true;
}



void CDlgAncientMini::OnShowDialog()
{
}


AUI_BEGIN_COMMAND_MAP(CDlgAncientLevelInfo, CDlgBase)

AUI_ON_COMMAND("Btn_Prev",		OnCommandPrev)
AUI_ON_COMMAND("Btn_Next",		OnCommandNext)
AUI_ON_COMMAND("Btn_Get",		OnCommandGet)

AUI_END_COMMAND_MAP()

AUI_BEGIN_EVENT_MAP(CDlgAncientLevelInfo, CDlgBase)



AUI_END_EVENT_MAP()

CDlgAncientLevelInfo::CDlgAncientLevelInfo()
{
}

CDlgAncientLevelInfo::~CDlgAncientLevelInfo()
{
}

bool CDlgAncientLevelInfo::Tick()
{
	if(GetHostPlayer()->GetRaidBattleInfo() != 538)
		Show(false);

	m_pImg_Item->Show(false);

	DATA_TYPE DataType;
	elementdataman* pDataMan = GetGame()->GetElementDataMan();
	const void* pBuf = pDataMan->get_data_ptr(726, ID_SPACE_CONFIG, DataType);
	if (pBuf && DataType == DT_CHALLENGE_2012_CONFIG)
	{
		CHALLENGE_2012_CONFIG* pData = (CHALLENGE_2012_CONFIG*)pBuf;
		
		// 关卡奖励信息
		CECIvtrItem* pItem = NULL;		
		if(m_nCurPage >=0 && m_nCurPage <=6 && pData->lv_config[m_nCurPage].award_item_id)
				pItem = CECIvtrItem::CreateItem(pData->lv_config[m_nCurPage].award_item_id, 0, 1);	
		else if(m_nCurPage == -1 && pData->main_config.award_item_id)
				pItem = CECIvtrItem::CreateItem(pData->lv_config[m_nCurPage].award_item_id, 0, 1);	
		if(pItem)
		{
			AString strFile;
			af_GetFileTitle(pItem->GetIconFile(), strFile);
			strFile.MakeLower();
			m_pImg_Item->SetCover(GetGameUIMan()->m_pA2DSpriteIcons[CECGameUIMan::ICONS_INVENTORY],
			GetGameUIMan()->m_IconMap[CECGameUIMan::ICONS_INVENTORY][strFile]);
			AUICTranslate trans;
			pItem->GetDetailDataFromLocal();
			const ACHAR* pszDesc = pItem->GetDesc();
			if(pszDesc)
				m_pImg_Item->SetHint(trans.Translate(pszDesc));
			else
				m_pImg_Item->SetHint(pItem->GetName());
				
			delete pItem;
			m_pImg_Item->Show(true);
		}
	}
	
	//关卡 时间、开矿信息
	ACString strText;			
	if(m_nCurPage >= 0 && m_nCurPage <= 6 && m_nCurPage == m_nCurLevel)
	{
		if(time)
		{
		
			m_iTime = GetGameUIMan()->m_pDlgAncientTime->GetTime();
			strText.Format(GetGameUIMan()->GetStringFromTable(11198), m_iTime / 60, m_iTime % 60);
			m_pTv_Conditions->SetItemText(time, strText);
		}
		if(count)
		{
			strText.Format(GetGameUIMan()->GetStringFromTable(11197), m_nCnt, m_nTotal);
			m_pTv_Conditions->SetItemText(count, strText);
		}
	}

	if(m_nCurPage == -1 && (m_nFinalState == 1 || m_nFinalState == 2))
	{
		if(time)
		{
		
			if(pBuf && DataType == DT_CHALLENGE_2012_CONFIG)
			{
				int iTime = 0;
				CHALLENGE_2012_CONFIG* pData = (CHALLENGE_2012_CONFIG*)pBuf;
				iTime = m_nFinalStartTime + pData->main_config.time_limit - GetGame()->GetServerGMTTime();
				if(iTime < 0)
					iTime = 0;
				strText.Format(GetGameUIMan()->GetStringFromTable(11198), iTime / 60, iTime % 60);
				m_pTv_Conditions->SetItemText(time, strText);
			}

		}
		if(count)
		{
			strText.Format(GetGameUIMan()->GetStringFromTable(11197), m_nFinalCnt, m_nFinalTotal);
			m_pTv_Conditions->SetItemText(count, strText);
		}
	}

	//玩家挑战状态
	if(m_nCurPage >= 0 && m_nCurPage <=6)
	{
		if(m_nLevelState[m_nCurPage] == 2)
			if(level_result[m_nCurPage])
				m_pLabel_Stage->SetText(GetGameUIMan()->GetStringFromTable(11187));
			else 
				m_pLabel_Stage->SetText(GetGameUIMan()->GetStringFromTable(11186));
		else if(m_nLevelState[m_nCurPage] == 1)
			m_pLabel_Stage->SetText(GetGameUIMan()->GetStringFromTable(11184));
		else if(m_nLevelState[m_nCurPage] == 0)
			m_pLabel_Stage->SetText(GetGameUIMan()->GetStringFromTable(11185));
	}
	
	if(m_nCurPage == -1)
	{
		if(m_nFinalState == 2)
			if(final)
				m_pLabel_Stage->SetText(GetGameUIMan()->GetStringFromTable(11187));
			else
				m_pLabel_Stage->SetText(GetGameUIMan()->GetStringFromTable(11186));
		else if(m_nFinalState == 1)
			m_pLabel_Stage->SetText(GetGameUIMan()->GetStringFromTable(11184));
		else if(m_nFinalState == 0)
			m_pLabel_Stage->SetText(GetGameUIMan()->GetStringFromTable(11185));
		if(final)
			m_pLabel_Stage->SetText(GetGameUIMan()->GetStringFromTable(11187));	
	}
	
	//领奖按钮状态控制
	if(m_nCurPage >= 0 && m_nCurPage <=6)
		if(level_result[m_nCurPage] && !level_award_deliveried[m_nCurPage])
			m_pBtn_Get->Enable(true);
		else
			m_pBtn_Get->Enable(false);
	
	if(m_nCurPage == -1)
		if(final && !final_award_deliveried)
			m_pBtn_Get->Enable(true);
		else 
			m_pBtn_Get->Enable(false);
	
	// 翻页按钮状态控制
	if(m_nCurPage == 6)
		m_pNextPage->Enable(false);
	else 
		m_pNextPage->Enable(true);

	if(m_nCurPage == -1)
		m_pPrevPage->Enable(false);
	else
		m_pPrevPage->Enable(true);

	return CDlgBase::Tick();
}

bool CDlgAncientLevelInfo::OnInitDialog()
{
	m_nCurLevel = -1;
	m_nFinalState = 2;
	m_pPrevPage = (AUIStillImageButton*)GetDlgItem("Btn_Prev");
	m_pNextPage = (AUIStillImageButton*)GetDlgItem("Btn_Next");
	m_pBtn_Get = (AUIStillImageButton*)GetDlgItem("Btn_Get");
	m_pTxt_Content = (AUITextArea *)GetDlgItem("Txt_Stage1");
	m_pImg_Item = (AUIImagePicture*)GetDlgItem("Img_Item");
	m_pTv_Conditions = (AUITreeView*)GetDlgItem("Tv_Stage2");
	m_pLabel_Name = (AUILabel*)GetDlgItem("Txt_Boss");
	m_pLabel_Stage = (AUILabel*)GetDlgItem("Lab_Stage2");
	m_nTotal = 0;
	m_nFinalTotal = 0;
	m_nCnt = 0;
	m_nFinalCnt = 0;
	m_nFinalStartTime = 0;
	m_nLevelStartTime = 0;
	m_bFinalTimerFlag = false;
	m_bLevelTimerFlag = false;
	final = false;
	final_award_deliveried = false;
	for(int i = 0; i < 10; i++)
	{
		level_result[i] = false;
		level_award_deliveried[i] = false;
		m_nLevelState[i] = 0;
	}

	return true;
}

void CDlgAncientLevelInfo::ClearState()
{
	for(int i = 0; i < 10; i++)
		m_nLevelState[i] = 0;
	m_nFinalState = 0;
}

void CDlgAncientLevelInfo::SetState(int level)
{
	if(level >=0 && level <= 6)
	{
		for(int i = 0; i <= 6; i++)
		{
			if(i == level)
				m_nLevelState[i] = 1;
			else if(i < level)
				m_nLevelState[i] = 2;
			else if(i > level)
				m_nLevelState[i] = 0;
		}		
	}
	else if(level == -1)
		m_nFinalState = 1;
}

void CDlgAncientLevelInfo::SetStartTime(int level, int StartTime)
{
	if(level >= 0 && level <= 6)
		m_nLevelStartTime = StartTime;
	else if(level == -1)
		m_nFinalStartTime = StartTime;
}

//请求关卡通过、奖励发放信息
void CDlgAncientLevelInfo::OnShowDialog()
{
	m_nCurPage = m_nCurLevel;
	GetGameSession()->c2s_CmdAncientLevelResult();
	BuildConditions();
}

bool CDlgAncientLevelInfo::SetCurLevel(int level)
{
	m_iTime = 0;
	
	SetTimerFlag(level, true);
	DATA_TYPE DataType;
	CHALLENGE_2012_CONFIG* pData = NULL;
	elementdataman* pDataMan = GetGame()->GetElementDataMan();
	const void* pBuf = pDataMan->get_data_ptr(726, ID_SPACE_CONFIG, DataType);
	if (pBuf && DataType == DT_CHALLENGE_2012_CONFIG)
		pData = (CHALLENGE_2012_CONFIG*)pBuf;
	if(level >= 0 && level <=6)
	{
		m_nCurLevel = level;
		if(pData)
		{
			m_iTime = pData->lv_config[m_nCurLevel].time_limit;
			m_nTotal = pData->lv_config[m_nCurLevel].mine_count_min;
		}
	}
	else if(level == -1)
	{
		if(pData)
		{
			m_iTime = pData->main_config.time_limit; 
			m_nFinalTotal = pData->main_config.mine_count_min;
		}
	}
	if(level >= 0 && level <= 6) //显示分挑战计时器
		if(m_iTime)   
		{
			GetGameUIMan()->m_pDlgAncientTime->Show(true);
			GetGameUIMan()->m_pDlgAncientTime->SetTime(m_iTime);
		}
		else
			GetGameUIMan()->m_pDlgAncientTime->Show(false);
		
	
	return true;
}

int CDlgAncientLevelInfo::GetCurLevel()
{
	return m_nCurLevel;
}

void CDlgAncientLevelInfo::SetLevelInfo(const S2C::ancient_raid_level_result *p)
{
	final = p->final;
	final_award_deliveried = p->final_award_deliveried;
	for(int i = 0; i < 10; i++)
	{
		level_result[i] = p->level_result[i];
		level_award_deliveried[i] = p->level_award_deliveried[i];
	}
}

void CDlgAncientLevelInfo::OnCommandPrev(const char* szCommand)
{
	m_nCurPage--;
	BuildConditions();
}

void CDlgAncientLevelInfo::OnCommandNext(const char* szCommand)
{
	m_nCurPage++;
	BuildConditions();
}


void CDlgAncientLevelInfo::SetMatterCnt(int level, int Cnt)
{
	if(level >=0 && level <= 9)
		m_nCnt = Cnt;
	else if(level ==-1)
		m_nFinalCnt = Cnt;
}

void CDlgAncientLevelInfo::SetLevelState(int level, int state)
{
	if(level == -1)
		m_nFinalState = state;
	else if(level >=0 && level <= 9)
		m_nLevelState[level] = state;
}

//关卡挑战条件
void CDlgAncientLevelInfo::BuildConditions()
{
	//通关攻略
	m_pTxt_Content->SetText(GetGameUIMan()->GetStringFromTable(m_nCurPage + 11167));	
	m_pLabel_Name->SetText(GetGameUIMan()->GetStringFromTable(m_nCurPage + 17301));
	m_pTv_Conditions->DeleteAllItems();
	ACString str;
	MONSTER_ESSENCE* pMonster;
	DATA_TYPE DataType;
	elementdataman* pDataMan = GetGame()->GetElementDataMan();
	const void* pBuf = pDataMan->get_data_ptr(726, ID_SPACE_CONFIG, DataType);
	count = NULL;
	time =NULL;
	if (pBuf && DataType == DT_CHALLENGE_2012_CONFIG)
	{
		CHALLENGE_2012_CONFIG* pData = (CHALLENGE_2012_CONFIG*)pBuf;
		if(m_nCurPage >=0 && m_nCurPage <=6)
		{
			if(pData->lv_config[m_nCurPage].monster_id)
			{
				pMonster = (MONSTER_ESSENCE*)pDataMan->get_data_ptr(pData->lv_config[m_nCurPage].monster_id, ID_SPACE_ESSENCE, DataType);
				if(pMonster && DataType == DT_MONSTER_ESSENCE)
				{
					m_pTv_Conditions->InsertItem(GetGameUIMan()->GetStringFromTable(11188));
				}

			}
			if(pData->lv_config[m_nCurPage].mine_id)
			{
				str.Format(GetGameUIMan()->GetStringFromTable(11189), pData->lv_config[m_nCurPage].mine_count_min);
				Item = m_pTv_Conditions->InsertItem(str);
				if(m_nCurPage == m_nCurLevel)
				{
					str.Format(GetGameUIMan()->GetStringFromTable(11197), m_nCnt, pData->main_config.mine_count_min);
					count = m_pTv_Conditions->InsertItem(str, Item);
				}
			}
			if(pData->lv_config[m_nCurPage].time_limit)
			{
				str.Format(GetGameUIMan()->GetStringFromTable(11199), pData->lv_config[m_nCurPage].time_limit);
				Item = m_pTv_Conditions->InsertItem(str);
				if(m_nCurPage == m_nCurLevel)
				{
					str.Format(GetGameUIMan()->GetStringFromTable(11198), pData->lv_config[m_nCurPage].time_limit/60, pData->lv_config[m_nCurPage].time_limit%60);
					time = m_pTv_Conditions->InsertItem(str, Item);
					m_pTv_Conditions->Expand(Item, AUITREEVIEW_EXPAND_EXPAND);
				}
			}
			if(pData->lv_config[m_nCurPage].medic_limit_hp)
			{
				str.Format(GetGameUIMan()->GetStringFromTable(11191), pData->lv_config[m_nCurPage].medic_limit_hp / 10000);
				m_pTv_Conditions->InsertItem(str);
			}
			if(pData->lv_config[m_nCurPage].medic_limit_mp)
			{
				str.Format(GetGameUIMan()->GetStringFromTable(11192), pData->lv_config[m_nCurPage].medic_limit_mp / 10000);
				m_pTv_Conditions->InsertItem(str);
			}
			if(pData->lv_config[m_nCurPage].dead_count)
			{
				str.Format(GetGameUIMan()->GetStringFromTable(11193), pData->lv_config[m_nCurPage].dead_count);
				m_pTv_Conditions->InsertItem(str);
			}
			if(pData->lv_config[m_nCurPage].controller_id_challenge == 180021)
			{
				str.Format(GetGameUIMan()->GetStringFromTable(11194), pData->lv_config[m_nCurPage].controller_id_min);
				m_pTv_Conditions->InsertItem(str);
			}
			if(pData->lv_config[m_nCurPage].controller_id_challenge == 180017)
			{
				str.Format(GetGameUIMan()->GetStringFromTable(11195), pData->lv_config[m_nCurPage].controller_id_min);
				m_pTv_Conditions->InsertItem(str);
			}
			if(pData->lv_config[m_nCurPage].controller_id_win == 82692)
			{
				str.Format(GetGameUIMan()->GetStringFromTable(11196),pData->lv_config[m_nCurPage].controller_id_win);
				m_pTv_Conditions->InsertItem(str);
			}

		} 
		else if(m_nCurPage == -1)
		{
			if(pData->main_config.monster_id)
			{
				pMonster = (MONSTER_ESSENCE*)pDataMan->get_data_ptr(pData->main_config.monster_id, ID_SPACE_ESSENCE, DataType);
				if(pMonster && DataType == DT_MONSTER_ESSENCE)
				{
					m_pTv_Conditions->InsertItem(GetGameUIMan()->GetStringFromTable(11188));
				}
			}
			if(pData->main_config.mine_id && pData->main_config.time_limit)
			{
				str.Format(GetGameUIMan()->GetStringFromTable(11189), pData->main_config.time_limit / 60, pData->main_config.mine_count_min);
				Item = m_pTv_Conditions->InsertItem(str);
				if(m_nFinalState == 1 || m_nFinalState == 2)
				{
					str.Format(GetGameUIMan()->GetStringFromTable(11197), m_nFinalCnt, pData->main_config.mine_count_min);
					count = m_pTv_Conditions->InsertItem(str, Item);
					str.Format(GetGameUIMan()->GetStringFromTable(11198), pData->main_config.time_limit / 60, pData->main_config.time_limit % 60);
					time = m_pTv_Conditions->InsertItem(str, Item);
					m_pTv_Conditions->Expand(Item, AUITREEVIEW_EXPAND_EXPAND);
				}
			}

			if(pData->main_config.medic_limit_hp)
			{
				str.Format(GetGameUIMan()->GetStringFromTable(11191), pData->main_config.medic_limit_hp / 10000);
				m_pTv_Conditions->InsertItem(str);
			}
			if(pData->main_config.medic_limit_mp)
			{
				str.Format(GetGameUIMan()->GetStringFromTable(11192), pData->main_config.medic_limit_mp / 10000);
				m_pTv_Conditions->InsertItem(str);
			}
			if(pData->main_config.dead_count)
			{
				str.Format(GetGameUIMan()->GetStringFromTable(11193), pData->main_config.dead_count);
				m_pTv_Conditions->InsertItem(str);
			}
			if(pData->main_config.controller_id_challenge)
			{
				str.Format(GetGameUIMan()->GetStringFromTable(11194), pData->main_config.controller_id_challenge, pData->main_config.controller_id_min);
				m_pTv_Conditions->InsertItem(str);
			}
			if(pData->main_config.controller_id_win)
			{
				str.Format(GetGameUIMan()->GetStringFromTable(11195),pData->main_config.controller_id_win);
				m_pTv_Conditions->InsertItem(str);
			}

		}
	}
}

void CDlgAncientLevelInfo::OnCommandGet(const char* szCommand)
{
	if(m_nCurPage >=0 && m_nCurPage <=6)
	{
		GetGameSession()->c2s_CmdAncientLevelAward(m_nCurPage);
		level_award_deliveried[m_nCurPage] = true;
	}
	else if(m_nCurPage == -1)
	{
		GetGameSession()->c2s_CmdAncientLevelAward(-1);
		final_award_deliveried = true;
	}
}

int CDlgAncientLevelInfo::GetLevelStartTime()
{
	return m_nLevelStartTime;
}

void CDlgAncientLevelInfo::SetTimerFlag(int level, bool bFlag)
{
	if(level >= 0 && level <= 6)
		m_bLevelTimerFlag = bFlag;
	else if(level == -1)
		m_bFinalTimerFlag = bFlag;
}

void CDlgAncientLevelInfo::SetResult(int level, bool bResult)
{
	if(level >= 0 && level <=6)
		level_result[level] = bResult;
	else if(level == -1)
		final = bResult;
}


//显示当前关卡的剩余时间

CDlgAncientTime::CDlgAncientTime()
{
}

CDlgAncientTime::~CDlgAncientTime()
{
}

bool CDlgAncientTime::OnInitDialog()
{
	m_pLabTime = (PAUILABEL)GetDlgItem("Txt_TimeLeft");
	
	m_nTime = 0;
	m_bTimer = false;
	return true;
}

void CDlgAncientTime::OnTick()
{
	if(GetHostPlayer()->GetRaidBattleInfo() != 538)
		Show(false);

	ACString strText;


	int iTime = 0;	
	iTime = GetGameUIMan()->m_pDlgAncientLevelInfo->GetLevelStartTime() + m_nTime -  GetGame()->GetServerGMTTime();

	if(iTime < 0)
		iTime = 0;
	m_nLeftTime = iTime;
	strText.Format(_AL("%02d:%02d"), iTime / 60, iTime % 60);
	m_pLabTime->SetText(strText);


}

void CDlgAncientTime::SetTime(int nTime)
{
	m_nTime = nTime;
}



int CDlgAncientTime::GetTime()
{
	return  m_nLeftTime;
}