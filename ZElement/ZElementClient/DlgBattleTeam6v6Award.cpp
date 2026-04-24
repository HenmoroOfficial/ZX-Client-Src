// Filename	: DlgBattleTeam6v6Award.cpp
// Creator	: Fu Chonggang
// Date		: 

#include "DlgBattleTeam6v6Award.h"
#include "A2DSprite.h"
#include "AUI\\AUIImagePicture.h"
#include "AUI\\AUILabel.h"
#include "EC_GameUIMan.h"
#include "EC_Game.h"
#include "EC_GameRun.h"
#include "EC_SendC2SCmds.h"
#include "EC_GPDataType.h"
#include "DlgSkill.h"

#define new A_DEBUG_NEW

AUI_BEGIN_COMMAND_MAP(CDlgBattleTeam6v6Award, CDlgBase)

AUI_ON_COMMAND("Btn_GetDailyAward",		OnCommand_GetDaily)
AUI_ON_COMMAND("Btn_GetScoreAward",		OnCommand_GetNormal)

AUI_END_COMMAND_MAP()


AUI_BEGIN_EVENT_MAP(CDlgBattleTeam6v6Award, CDlgBase)

AUI_ON_EVENT("Item_A*",			WM_LBUTTONDOWN,		OnEventLButtonDown)
AUI_ON_EVENT("Item_B*",			WM_LBUTTONDOWN,		OnEventLButtonDown)

AUI_END_EVENT_MAP()

CDlgBattleTeam6v6Award::CDlgBattleTeam6v6Award()
{
}

CDlgBattleTeam6v6Award::~CDlgBattleTeam6v6Award()
{
}

bool CDlgBattleTeam6v6Award::OnInitDialog()
{
	m_iCurSel = -1;
	m_pObjDaily = GetDlgItem("Btn_GetDailyAward");
	m_pObjScore = GetDlgItem("Btn_GetScoreAward");
	m_iDayWin = 0;
	BuildAwardsFromTmpl();

	m_pSprite = new A2DSprite;
	if( !m_pSprite ) return AUI_ReportError(__LINE__, __FILE__);
	
	bool bval = m_pSprite->Init(m_pA3DDevice, "icon\\Battle_Dungeon_6v6_SkillFocus.tga", 0);
	if( !bval ) return AUI_ReportError(__LINE__, __FILE__);
	return true;
}

bool CDlgBattleTeam6v6Award::Release()
{
	A3DRELEASE(m_pSprite);
	return CDlgBase::Release();
}

void CDlgBattleTeam6v6Award::OnTick()
{
	int iTimeZoneBias = -GetGame()->GetTimeZoneBias()*60;
	int t = GetGame()->GetServerGMTTime();
	m_pObjDaily->Enable((t+iTimeZoneBias)/86400==(m_iLast_collision_timestamp+iTimeZoneBias)/86400 &&
		(t+iTimeZoneBias)/86400!=(m_iDaily_award_timestamp+iTimeZoneBias)/86400 && m_iDayWin>0);
	COLLISION_RAID_AWARD_CONFIG& config = GetGameRun()->GetCollisionRaidAwardConfig();
	m_pObjScore->Enable(m_iCurSel>=0 && m_iScore_a>=config.award_list[m_iCurSel].score1_num
		&& m_iScore_b>=config.award_list[m_iCurSel].score2_num
		&& m_iWin_num>=config.award_list[m_iCurSel].win_num);
}

void CDlgBattleTeam6v6Award::OnCommand_GetDaily(const char * szCommand)
{
	c2s_SendCmdGetCollisionAward(1, 0);
}

void CDlgBattleTeam6v6Award::OnCommand_GetNormal(const char * szCommand)
{
	c2s_SendCmdGetCollisionAward(0, m_iCurSel);
}

void CDlgBattleTeam6v6Award::OnEventLButtonDown(WPARAM wParam, LPARAM lParam, AUIObject *pObj)
{
	if (m_iCurSel>=0)
	{
		int i;
		AString strName;
		PAUIIMAGEPICTURE pImg;
		for (i=0; i<8; ++i)
		{
			strName.Format("Item_A%02d", i+1);
			pImg = (PAUIIMAGEPICTURE)GetDlgItem(strName);
			pImg->SetCover(0, 0, 1);
			
			strName.Format("Item_B%02d", i+1);
			pImg = (PAUIIMAGEPICTURE)GetDlgItem(strName);
			pImg->SetCover(0, 0, 1);
			
		}
	}
	m_iCurSel = pObj->GetData("index");
	PAUIIMAGEPICTURE pImg = (PAUIIMAGEPICTURE)pObj;
	pImg->SetCover(m_pSprite, 0, 1);
}

int CDlgBattleTeam6v6Award::CommonMessage(AString strMsg, WPARAM wParam, LPARAM lParam)
{
	if (strMsg=="info")
	{
		S2C::collision_raid_info *p = (S2C::collision_raid_info*)wParam;
		m_iScore_a = p->score_a;
		m_iScore_b = p->score_b;
		m_iWin_num = p->win_num;
		m_iLost_num = p->lost_num;
		m_iDraw_num = p->draw_num;
		m_iDaily_award_timestamp = p->daily_award_timestamp;
		m_iLast_collision_timestamp = p->last_collision_timestamp;
		m_iDayWin = p->day_win;
		ACString strText;
		PAUILABEL pLab = (PAUILABEL)GetDlgItem("Txt_PerInfoScore_01");
		strText.Format(_AL("%d"), p->score_a);
		pLab->SetText(strText);
		pLab = (PAUILABEL)GetDlgItem("Txt_PerInfoScore_02");
		strText.Format(_AL("%d"), p->score_b);
		pLab->SetText(strText);
		pLab = (PAUILABEL)GetDlgItem("Txt_PerInfoScore_03");
		strText.Format(_AL("%d"), p->win_num);
		pLab->SetText(strText);

	}
	return 0;
}

void CDlgBattleTeam6v6Award::BuildAwardsFromTmpl()
{
	COLLISION_RAID_AWARD_CONFIG& config = GetGameRun()->GetCollisionRaidAwardConfig();
	PAUIIMAGEPICTURE pImg;
	PAUILABEL pLab;
	pImg = (PAUIIMAGEPICTURE)GetDlgItem("Item_Daily");
	GetGameUIMan()->SetIvtrInfo(pImg, config.daily_award_item);
	int i;
	int iScoreNum1 = 0;
	int iScoreNum2 = 0;
	AString strName;
	ACString strText;
	for (i=0; i<20; ++i)
	{
		if (config.award_list[i].item_id==0)
		{
			break;
		}
		if (iScoreNum1<8 && config.award_list[i].score1_num>0)
		{
			strName.Format("Item_A%02d", iScoreNum1+1);
			pImg = (PAUIIMAGEPICTURE)GetDlgItem(strName);
			GetGameUIMan()->SetIvtrInfo(pImg, config.award_list[i].item_id,config.award_list[i].item_num);
			pImg->SetData(i, "index");
			strName.Format("Txt_Score_A%02d", iScoreNum1+1);
			pLab = (PAUILABEL)GetDlgItem(strName);
			strText.Format(_AL("%d"), config.award_list[i].score1_num);
			pLab->SetText(strText);
			
			iScoreNum1++;
		}
		else if (iScoreNum2<8 && config.award_list[i].score2_num>0)
		{
			strName.Format("Item_B%02d", iScoreNum2+1);
			pImg = (PAUIIMAGEPICTURE)GetDlgItem(strName);
			GetGameUIMan()->SetIvtrInfo(pImg, config.award_list[i].item_id,config.award_list[i].item_num);
			pImg->SetData(i, "index");
			strName.Format("Txt_Score_B%02d", iScoreNum2+1);
			pLab = (PAUILABEL)GetDlgItem(strName);
			strText.Format(_AL("%d"), config.award_list[i].score2_num);
			pLab->SetText(strText);
			strName.Format("Txt_Victory_B%02d", iScoreNum2+1);
			pLab = (PAUILABEL)GetDlgItem(strName);
			strText.Format(_AL("%d"), config.award_list[i].win_num);
			pLab->SetText(strText);
			
			iScoreNum2++;
		}
	}
	for (i=iScoreNum1; i<8; ++i)
	{
		strName.Format("Item_A%02d", i+1);
		pImg = (PAUIIMAGEPICTURE)GetDlgItem(strName);
		pImg->Show(false);
		strName.Format("Txt_Score_A%02d", i+1);
		pLab = (PAUILABEL)GetDlgItem(strName);
		pLab->Show(false);
		strName.Format("Lab_Score_A%02d", i+1);
		pLab = (PAUILABEL)GetDlgItem(strName);
		pLab->Show(false);
	}
	for (i=iScoreNum2; i<8; ++i)
	{
		strName.Format("Item_B%02d", i+1);
		pImg = (PAUIIMAGEPICTURE)GetDlgItem(strName);
		pImg->Show(false);
		strName.Format("Txt_Score_B%02d", i+1);
		pLab = (PAUILABEL)GetDlgItem(strName);
		pLab->Show(false);
		strName.Format("Lab_Score_B%02d", i+1);
		pLab = (PAUILABEL)GetDlgItem(strName);
		pLab->Show(false);
		strName.Format("Txt_Victory_B%02d", i+1);
		pLab = (PAUILABEL)GetDlgItem(strName);
		pLab->Show(false);
		strName.Format("Lab_Victory_B%02d", i+1);
		pLab = (PAUILABEL)GetDlgItem(strName);
		pLab->Show(false);
	}
}
