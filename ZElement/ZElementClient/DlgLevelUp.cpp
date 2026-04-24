// Filename	:	DlgLevelUp.cpp
// Creator	:	Lei Dongyu
// Date		:	Mar 30, 2012

//#include "ElementStdHeaders.h"
#include "DlgLevelUp.h"
#include "EC_Hostplayer.h"
#include "elementdataman.h"
#include "EC_Game.h"
#include "EC_GameRun.h"
#include "EC_GameUIMan.h"
#include "AFI.h"
#include "A2DSprite.h"
#include "AUI\\AUI.h"

#include "DlgSystem.h"
#include "DlgSkillMan.h"

extern CECGame* g_pGame;

#define new A_DEBUG_NEW

AUI_BEGIN_COMMAND_MAP(CDlgLevelUp, CDlgBase)

AUI_ON_COMMAND("IDCANCEL",				OnCommand_CANCEL)
AUI_ON_COMMAND("Btn_Close",				OnCommand_CANCEL)
AUI_ON_COMMAND("Btn_Skillzx2",			OnCommand_SkillWindow)
AUI_ON_COMMAND("Btn_Talent",			OnCommand_TalentWindow)

AUI_END_COMMAND_MAP()

AUI_BEGIN_EVENT_MAP(CDlgLevelUp, CDlgBase)

AUI_END_EVENT_MAP()

static unsigned int iSkillPtConfig = 0;
static unsigned int iTalentPtConfig = 0;

float CalcAttriBonus(int level , int requiredLevel , int initial , float lvlup)
{
	if(level < requiredLevel)
	{
		//ASSERT(0);
		OutputDebugStringA("Error! Player level is lower than required level!");
		return 0;
	}
	return initial + (level - requiredLevel) * lvlup;
}

float CalcRebornBonus(const int iLevel[4] , const float attri[15] , int n/*n:reborn_count*/)
{
	//公式抄自服务器
	float result = 0;
	const float *args = &(attri[-1]);
	float f1[] = {0 ,1 , args[13] , args[14]};
	float f2[] = {0 ,0 , 1,args[15]};
	float f3[] = {0 ,0 , 0 ,1};
	int lvl = iLevel[0];
	
	float part1 = (args[1]*(lvl*lvl*lvl) + args[2] *(lvl*lvl) + args[3] * lvl + args[4]);
	float part2 = 1.0f + (f1[n] *(iLevel[1] - args[7])*args[8] + f2[n]*(iLevel[2] - args[9])*args[10] + f3[n]*(iLevel[3] - args[11]) * args[12])/100.f;
	
	result = part1 *  a_Min(args[5], a_Max(args[6] ,  part2));
	
	return result;
}

void CalcAttriBonus(float* result , int level , const CHARACTER_CLASS_CONFIG* pClassConfig , const RENASCENCE_PROP_CONFIG* pRebornConfig ,
					const CECHostPlayer::RebornInfoVec* pInfo)
{
	result[0] = CalcAttriBonus(level , pClassConfig->level_required , pClassConfig->hp , pClassConfig->lvlup_hp);
	result[1] = CalcAttriBonus(level , pClassConfig->level_required , pClassConfig->mp , pClassConfig->lvlup_mp);
	result[2] = CalcAttriBonus(level , pClassConfig->level_required , pClassConfig->dmg , pClassConfig->lvlup_dmg);
	result[3] = CalcAttriBonus(level , pClassConfig->level_required , pClassConfig->defense , pClassConfig->lvlup_defense);
	result[4] = CalcAttriBonus(level , pClassConfig->level_required , pClassConfig->anti_stunt , pClassConfig->lvlup_anti_stunt);
	result[5] = CalcAttriBonus(level , pClassConfig->level_required , pClassConfig->anti_weak , pClassConfig->lvlup_anti_weak);
	result[6] = CalcAttriBonus(level , pClassConfig->level_required , pClassConfig->anti_slow , pClassConfig->lvlup_anti_slow);		//这项是定身
	result[7] = CalcAttriBonus(level , pClassConfig->level_required , pClassConfig->anti_silence , pClassConfig->lvlup_anti_silence);
	result[8] = CalcAttriBonus(level , pClassConfig->level_required , pClassConfig->anti_sleep , pClassConfig->lvlup_anti_sleep);
	result[9] = CalcAttriBonus(level , pClassConfig->level_required , pClassConfig->anti_twist , pClassConfig->lvlup_anti_twist);	//这项是减速
	result[10] = CalcAttriBonus(level , pClassConfig->level_required , pClassConfig->attack , pClassConfig->lvlup_attack);
	result[11] = CalcAttriBonus(level , pClassConfig->level_required , pClassConfig->armor , pClassConfig->lvlup_armor);

	if(pRebornConfig)
	{
		int iLevel[4];
		iLevel[0] = level;
		size_t i;
		for(i = 1; i <= pInfo->size();i++)
		{
			iLevel [i] = (*pInfo)[i -1].level;
		}
		
		//i[]
		//i++;
		for(i;i<4;i++)
		{
			iLevel[i] = 0;
		}

		int count = pInfo->size();

		result[0] += CalcRebornBonus(iLevel , pRebornConfig->hp , count);
		result[1] += CalcRebornBonus(iLevel , pRebornConfig->mp , count);
		result[2] += CalcRebornBonus(iLevel , pRebornConfig->dmg , count);
		result[3] += CalcRebornBonus(iLevel , pRebornConfig->def , count);
		result[4] += CalcRebornBonus(iLevel , pRebornConfig->anti[0] , count);
		result[5] += CalcRebornBonus(iLevel , pRebornConfig->anti[1] , count);
		result[6] += CalcRebornBonus(iLevel , pRebornConfig->anti[2] , count);
		result[7] += CalcRebornBonus(iLevel , pRebornConfig->anti[3] , count);
		result[8] += CalcRebornBonus(iLevel , pRebornConfig->anti[4] , count);
		result[9] += CalcRebornBonus(iLevel , pRebornConfig->anti[5] , count);
		result[10] += CalcRebornBonus(iLevel , pRebornConfig->attack , count);
		result[11] += CalcRebornBonus(iLevel , pRebornConfig->armor , count);

	}
}

bool CheckRebornConfig(const RENASCENCE_PROP_CONFIG* pConfig ,const CECHostPlayer::RebornInfoVec* pInfo , short iClass)
{
	for(size_t i= 0;i <= pInfo->size() ;i++)
	{
		UInt64 mask0,mask1;
		switch (i)
		{
		case 0:
			mask0 = pConfig->occup_lev_0;
			mask1 = pConfig->occup_lev_0_2;
			break;
		case 1:
			mask0 = pConfig->occup_lev_1;
			mask1 = pConfig->occup_lev_1_2;
			break;
		case 2:
			mask0 = pConfig->occup_lev_2;
			mask1 = pConfig->occup_lev_2_2;
			break;
		case 3:
			mask0 = pConfig->occup_lev_3;
			mask1 = pConfig->occup_lev_3_2;
			break;
		default:
			ASSERT(0);
			break;
		}

		short prof;
		if(i < pInfo->size()) prof= (*pInfo)[i].prof;
		else prof = iClass;
		if((prof < 64 && (mask0 & UINT64(1)<< prof) == 0) 
				|| (prof >= 64 && (mask1 & UINT64(1) << (prof -64))==0))
		{
			return false;
		}
	}
	return true;
}

void CalcSkillAndTalentPoint(int* result , int level , int count , const CECHostPlayer::RebornInfoVec* v)
{
	elementdataman* pDataman = g_pGame->GetElementDataMan();
	DATA_TYPE dtType;
	const PLAYER_SKILL_POINT_CONFIG* pSkillPtConfig = (const PLAYER_SKILL_POINT_CONFIG*)pDataman->get_data_ptr(iSkillPtConfig , ID_SPACE_CONFIG , dtType);
	if(dtType != DT_PLAYER_SKILL_POINT_CONFIG)
	{
		pSkillPtConfig = NULL;
		return;
	}
	const PLAYER_TALENT_CONFIG* pTalentPtConfig = (const PLAYER_TALENT_CONFIG*)pDataman->get_data_ptr(iTalentPtConfig , ID_SPACE_CONFIG ,dtType);
	if(dtType != DT_PLAYER_TALENT_CONFIG)
	{
		pTalentPtConfig = NULL;
		return;
	}
	result[0] = 0;
	result[1] = 0;
	for(int i= 0 ;i<= count;i++)
	{
		const int* skillpoint;
		const int* talentpoint;
		switch (i)
		{

		case 0:
			skillpoint = pSkillPtConfig->skill_point;
			talentpoint = pTalentPtConfig->talent_point;
			break;
		case 1:
			skillpoint = pSkillPtConfig->skill_point_1;
			talentpoint = pTalentPtConfig->talent_point_1;
			break;
		case 2:
			skillpoint = pSkillPtConfig->skill_point_2;
			talentpoint = pTalentPtConfig->talent_point_2;
			break;
		case 3:
			skillpoint = pSkillPtConfig->skill_point_3;
			talentpoint = pTalentPtConfig->talent_point_3;
			break;
		default:
			ASSERT(0);
			break;
		}
		if(i < count)
		{
			result[0] += skillpoint[(*v)[i].level - 1];
			result[1] += talentpoint[(*v)[i].level - 1];
		}
		else
		{
			result[0] += skillpoint[level - 1];
			result[1] += talentpoint[level - 1];
		}
	}
}

CDlgLevelUp::CDlgLevelUp()
{
}

CDlgLevelUp::~CDlgLevelUp()
{
}

bool CDlgLevelUp::Release()
{
	return CDlgBase::Release();
}

bool CDlgLevelUp::OnInitDialog()
{
	if (!CDlgBase::OnInitDialog())
		return false;

	m_pLst_Skill = (AUIListBox*) GetDlgItem("Lst_Skill");
	m_pLst_Detail = (AUIListBox *)GetDlgItem("Lst_Basic");
	m_pBtn_Skill = (AUIStillImageButton*) GetDlgItem("Btn_Skillzx2");
	m_pBtn_Talent = (AUIStillImageButton*) GetDlgItem("Btn_Talent");
	m_pLab_LevelAfter = (AUILabel*)GetDlgItem("Lab_LvAfter");
	m_pLab_LevelBefore = (AUILabel*)GetDlgItem("Lab_LvBefore");

	m_iNewLevel = 0;
	m_iNewClass = 0;
	m_iOldLevel = 0;
	m_iOldClass = 0;
	m_iRebornCount = 0;
	m_iOldRebornCount = 0;
	return true;
}

void CDlgLevelUp::OnShowDialog()
{
	
}

void CDlgLevelUp::OnTick()
{
	int oldLevel = GetLevel();
	int oldClass = GetClass();
	
	int newLevel = 0;
	int newClass = 0;
	if(m_bRebornType)
	{
		newLevel = GetHostPlayer()->GetBasicProps().iLevel;
		newClass = GetHostPlayer()->GetProfession();
	}
	else
	{
		newLevel = m_iRebornLevel;
		newClass = m_iRebornClass;
	}


	//不需要更新界面，直接返回
	if(oldLevel == newLevel && oldClass == newClass
		|| newLevel == m_iNewLevel && newClass == m_iNewClass)
	{
		return;
	}

	//int iLevelBeforeReborn = oldLevel;
	if(GetHostPlayer()->GetRebornCount() != m_iRebornCount)	//飞升了
	{
		//飞升时，关闭界面
		oldLevel = newLevel;
		oldClass = newClass;
		OnCommand_CANCEL("");
		return;
	}
	m_iNewClass = newClass;
	m_iNewLevel = newLevel;

	ACString strNewLevel;
	ACString strOldLevel;
	
	if ( 0 == m_iRebornCount )
	{
		strOldLevel.Format(GetStringFromTable(769), oldLevel,
			m_pAUIManager->GetStringFromTable(PROFESSION_START + oldClass));
		strNewLevel.Format(GetStringFromTable(769), newLevel,
			m_pAUIManager->GetStringFromTable(PROFESSION_START + newClass));
	}
	else
	{
		if(m_bRebornType)		// 2012-10-26 add 模式判断 by zy
		{
			strOldLevel.Format(GetStringFromTable(7010), oldLevel,
				m_pAUIManager->GetStringFromTable(PROFESSION_START + oldClass) , "");
			strNewLevel.Format(GetStringFromTable(7010), newLevel,
				m_pAUIManager->GetStringFromTable(PROFESSION_START + newClass) , "");
		}
		else
		{
			strOldLevel.Format(GetStringFromTable(7010), oldLevel,
				m_pAUIManager->GetStringFromTable(PROFESSION_START + oldClass) , GetStringFromTable(18600));
			strNewLevel.Format(GetStringFromTable(7010), newLevel,
				m_pAUIManager->GetStringFromTable(PROFESSION_START + newClass) ,GetStringFromTable(18600));
		}
	}
	//OutputDebugStringW(strOldLevel);
	//OutputDebugStringW(strNewLevel);
	m_pLab_LevelBefore->SetText(strOldLevel);
	m_pLab_LevelAfter->SetText(strNewLevel);
	
	const CHARACTER_CLASS_CONFIG* pOldClassConfig;
	const CHARACTER_CLASS_CONFIG* pNewClassConfig;
	const RENASCENCE_PROP_CONFIG* pOldRebornConfig;
	const RENASCENCE_PROP_CONFIG* pNewRebornConfig;

	DATA_TYPE dtType;
	elementdataman* pDataman = g_pGame->GetElementDataMan();
	unsigned int id = pDataman->get_first_data_id(ID_SPACE_CONFIG , dtType);

	//计算属性增益
	int rebornCount = GetHostPlayer()->GetRebornCount();
	CECHostPlayer::RebornInfoVec v;
	pNewRebornConfig = NULL;
	pOldRebornConfig = NULL;
	if(rebornCount)
	{
		v = GetHostPlayer()->GetRebornInfo();
		//获取飞升增益模板
		unsigned int id = pDataman->get_first_data_id(ID_SPACE_CONFIG , dtType);
		while(id && !(pNewRebornConfig && pOldRebornConfig))
		{
			if(dtType == DT_RENASCENCE_PROP_CONFIG)
			{
				const RENASCENCE_PROP_CONFIG* pConfig = (const RENASCENCE_PROP_CONFIG*)pDataman->get_data_ptr(id, ID_SPACE_CONFIG , dtType);
				if(CheckRebornConfig(pConfig , &v , m_iNewClass))
				{
					pNewRebornConfig = pConfig;
				}
				if(CheckRebornConfig(pConfig , &v , oldClass))
				{
					pOldRebornConfig = pConfig;
				}
			}
			id = pDataman->get_next_data_id(ID_SPACE_CONFIG , dtType);
		}
	}
	//获取职业升级属性变化模板
	id = pDataman->get_first_data_id(ID_SPACE_CONFIG , dtType);
	while(id)
	{
		if(dtType == DT_CHARACTER_CLASS_CONFIG)
		{
			const CHARACTER_CLASS_CONFIG* pTempConfig = (const CHARACTER_CLASS_CONFIG*)pDataman->get_data_ptr(id , ID_SPACE_CONFIG , dtType);
			if((int)pTempConfig->character_class_id == oldClass)
			{
				pOldClassConfig = pTempConfig;
				break;
			}
		}
		id = pDataman->get_next_data_id(ID_SPACE_CONFIG , dtType);
	}
	if(!id)
	{
		pOldClassConfig = NULL;
	}

	if(m_iNewClass == oldClass)
	{
		pNewClassConfig = pOldClassConfig;
	}
	else
	{
		id = pDataman->get_first_data_id(ID_SPACE_CONFIG , dtType);
		while(id)
		{
			if(dtType == DT_CHARACTER_CLASS_CONFIG)
			{
				const CHARACTER_CLASS_CONFIG* pTempConfig = (const CHARACTER_CLASS_CONFIG*)pDataman->get_data_ptr(id , ID_SPACE_CONFIG , dtType);
				if((int)pTempConfig->character_class_id == m_iNewClass)
				{
					pNewClassConfig = pTempConfig;
					break;
				}
			}
			id = pDataman->get_next_data_id(ID_SPACE_CONFIG , dtType);
		}
		if(!id)
		{
			pNewClassConfig = NULL;
		}
	}
	
	if(!pOldClassConfig || !pNewClassConfig) return;

	//计算属性增益
	float oldAttribute[12];
	float newAttribute[12];
	memset(oldAttribute , 0 , sizeof(oldAttribute));
	memset(newAttribute , 0 , sizeof(newAttribute));
	CalcAttriBonus(oldAttribute , oldLevel , pOldClassConfig , pOldRebornConfig , &v);
	CalcAttriBonus(newAttribute , newLevel , pNewClassConfig , pNewRebornConfig , &v);

	//获得技能点和天书点模板
	if(!iSkillPtConfig || !iTalentPtConfig)
	{
		id = pDataman->get_first_data_id(ID_SPACE_CONFIG , dtType);
		while(id)
		{
			if(dtType == DT_PLAYER_SKILL_POINT_CONFIG)
			{
				iSkillPtConfig = id;
				if(iTalentPtConfig)
				{
					break;
				}
			}
			if(dtType == DT_PLAYER_TALENT_CONFIG)
			{
				iTalentPtConfig = id;
				if(iSkillPtConfig)
				{
					break;
				}
			}
			id = pDataman->get_next_data_id(ID_SPACE_CONFIG ,dtType);
		}	
	}

	if(!iSkillPtConfig || !iTalentPtConfig)
	{
		OutputDebugStringA("Error! Cannot find Skill point config or Talent point config");
	}
	//计算技能点和天书点
	int oldSkillPoint[2];
	int newSkillPoint[2];
	memset(oldSkillPoint , 0 , sizeof(oldSkillPoint));
	memset(newSkillPoint , 0 , sizeof(newSkillPoint));
	int count = GetHostPlayer()->GetRebornCount();
	if(count != m_iRebornCount)//飞升
	{
		CalcSkillAndTalentPoint(oldSkillPoint , oldLevel , count -1 ,&v);
	}
	else
	{
		CalcSkillAndTalentPoint(oldSkillPoint , oldLevel , count ,&v);
	}
	CalcSkillAndTalentPoint(newSkillPoint , newLevel , count ,&v);
	
	if(!m_bRebornType)
	{
		memset(oldSkillPoint , 0 , sizeof(oldSkillPoint));
		memset(newSkillPoint , 0 , sizeof(newSkillPoint));
	}
	//更新list
	m_pLst_Detail->ResetContent();
	m_pLst_Skill->ResetContent();
	ACString strText;
	int iDeltaValue;

	iDeltaValue = (int)newAttribute[0] - (int)oldAttribute[0];
	strText.Format(_AL("^80ffff%s\t^2bfffa%d"), GetGameUIMan()->GetStringFromTable(11100), iDeltaValue);
	m_pLst_Detail->AddString(strText);
	iDeltaValue = (int)newAttribute[1] - (int)oldAttribute[1];
	strText.Format(_AL("^80ffff%s\t^2bfffa%d"), GetGameUIMan()->GetStringFromTable(11101), iDeltaValue);
	m_pLst_Detail->AddString(strText);
	iDeltaValue = (int)newAttribute[2] - (int)oldAttribute[2];
	strText.Format(_AL("^80ffff%s\t^2bfffa%d"), GetGameUIMan()->GetStringFromTable(11102), iDeltaValue);
	m_pLst_Detail->AddString(strText);
	iDeltaValue = (int)newAttribute[3] - (int)oldAttribute[3];
	strText.Format(_AL("^80ffff%s\t^2bfffa%d"), GetGameUIMan()->GetStringFromTable(11103), iDeltaValue);
	m_pLst_Detail->AddString(strText);
	iDeltaValue = (int)newAttribute[10] - (int)oldAttribute[10];
	strText.Format(_AL("^80ffff%s\t^2bfffa%d"), GetGameUIMan()->GetStringFromTable(11115), iDeltaValue);
	m_pLst_Detail->AddString(strText);
	iDeltaValue = (int)newAttribute[11] - (int)oldAttribute[11];
	strText.Format(_AL("^80ffff%s\t^2bfffa%d"), GetGameUIMan()->GetStringFromTable(11116), iDeltaValue);
	m_pLst_Detail->AddString(strText);
	iDeltaValue = (int)newAttribute[4] - (int)oldAttribute[4];
	strText.Format(_AL("^80ffff%s\t^2bfffa%d"), GetGameUIMan()->GetStringFromTable(11106), iDeltaValue);
	m_pLst_Detail->AddString(strText);
	iDeltaValue = (int)newAttribute[5] - (int)oldAttribute[5];
	strText.Format(_AL("^80ffff%s\t^2bfffa%d"), GetGameUIMan()->GetStringFromTable(11107), iDeltaValue);
	m_pLst_Detail->AddString(strText);
	iDeltaValue = (int)newAttribute[9] - (int)oldAttribute[9];
	strText.Format(_AL("^80ffff%s\t^2bfffa%d"), GetGameUIMan()->GetStringFromTable(11112), iDeltaValue);
	m_pLst_Detail->AddString(strText);
	iDeltaValue = (int)newAttribute[7] - (int)oldAttribute[7];
	strText.Format(_AL("^80ffff%s\t^2bfffa%d"), GetGameUIMan()->GetStringFromTable(11109), iDeltaValue);
	m_pLst_Detail->AddString(strText);
	iDeltaValue = (int)newAttribute[8] - (int)oldAttribute[8];
	strText.Format(_AL("^80ffff%s\t^2bfffa%d"), GetGameUIMan()->GetStringFromTable(11110), iDeltaValue);
	m_pLst_Detail->AddString(strText);
	iDeltaValue = (int)newAttribute[6] - (int)oldAttribute[6];
	strText.Format(_AL("^80ffff%s\t^2bfffa%d"), GetGameUIMan()->GetStringFromTable(11111), iDeltaValue);
	m_pLst_Detail->AddString(strText);

	strText.Format(_AL("^80ffff%s\t^2bfffa%d"), GetGameUIMan()->GetStringFromTable(11114), newSkillPoint[0] - oldSkillPoint[0]);
	m_pLst_Skill->AddString(strText);
	strText.Format(_AL("^80ffff%s\t^2bfffa%d"), GetGameUIMan()->GetStringFromTable(11113), newSkillPoint[1] - oldSkillPoint[1]);
	m_pLst_Skill->AddString(strText);

}

bool CDlgLevelUp::Render()
{
	if( !CDlgBase::Render() )
		return false;
	return true;
}

//关闭窗口，更新人物级别和职业
void CDlgLevelUp::OnCommand_CANCEL(const char * szCommand)
{
	Show(false);
	SetLevel(GetHostPlayer()->GetBasicProps().iLevel);
	SetClass(GetHostPlayer()->GetProfession());
	m_iNewClass = 0;
	m_iNewLevel = 0;
	m_iRebornCount = GetHostPlayer()->GetRebornCount();
}

void CDlgLevelUp::OnCommand_SkillWindow(const char * szCommand)
{
	CDlgSystem* pDlg = (CDlgSystem*)GetGameUIMan()->GetDialog("Win_Main");
	pDlg->OnCommandSkill(szCommand);
}

void CDlgLevelUp::OnCommand_TalentWindow(const char * szCommand)
{
	CDlgSkillMan* pDlg = (CDlgSkillMan*)GetGameUIMan()->GetDialog("Win_Skillzx2");
	pDlg->OnCommand_talent(szCommand);
}

void CDlgLevelUp::SetRebornCount(int i)
{
	m_iRebornCount = i;
}

int CDlgLevelUp::GetRebornCount()
{

	return m_iRebornCount;
}

void CDlgLevelUp::SetClass(int i)
{
	m_iOldClass = i;
}

int CDlgLevelUp::GetClass()
{
	return m_iOldClass;
}

void CDlgLevelUp::SetLevel(int i)
{
	m_iOldLevel = i;
}

int CDlgLevelUp::GetLevel()
{
	return m_iOldLevel;
}

void CDlgLevelUp::SetRebornType(bool b)  // 2012-10-18 add 弥补飞升值 by zy	
{
	m_bRebornType = b;
}
bool CDlgLevelUp::GetRebornType()
{
	return m_bRebornType;
} 

void CDlgLevelUp::SetRebornLevel(int i)  // 2012-10-18 add 弥补飞升值 by zy	
{
	m_iRebornLevel = i;
}

int CDlgLevelUp::GetLRebornLevel()
{
	return m_iRebornLevel;
}


void CDlgLevelUp::SetRebornClass(int i)  // 2012-10-18 add 弥补飞升值 by zy	
{
	m_iRebornClass = i;
}

int CDlgLevelUp::GetLRebornClass()
{
	return m_iRebornClass;
}
