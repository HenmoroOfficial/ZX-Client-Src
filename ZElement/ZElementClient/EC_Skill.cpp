/*
 * FILE: EC_Skill.cpp
 *
 * DESCRIPTION: 
 *
 * CREATED BY: Duyuxin, 2005/1/17
 *
 * HISTORY: 
 *
 * Copyright (c) 2005 Archosaur Studio, All Rights Reserved.
 */

#include "EC_Global.h"
#ifndef _INVENTORY_DESC_MAN		//游戏客户端使用 Modified 2012-04-23.
#include "EC_Game.h"
#include "EC_GameRun.h"
#include "EC_HostPlayer.h"
#else
#include "EC_Game_HintTool.h"	//小工具使用
#endif
#include "EC_Inventory.h"
#include "EC_IvtrTalismanMain.h"
#include "EC_Skill.h"


#define new A_DEBUG_NEW

///////////////////////////////////////////////////////////////////////////
//	
//	Define and Macro
//	
///////////////////////////////////////////////////////////////////////////


///////////////////////////////////////////////////////////////////////////
//	
//	Reference to External variables and functions
//	
///////////////////////////////////////////////////////////////////////////


///////////////////////////////////////////////////////////////////////////
//	
//	Local Types and Variables and Global variables
//	
///////////////////////////////////////////////////////////////////////////

static CECSkillStr l_SkillStr;

///////////////////////////////////////////////////////////////////////////
//	
//	Local functions
//	
///////////////////////////////////////////////////////////////////////////

wchar_t* CECSkillStr::Find(int id) const
{
	CECStringTab* pStrTab = g_pGame->GetSkillDesc();
	const wchar_t* str = pStrTab->GetWideString(id);
	return str ? (wchar_t*)str : L"";
}

///////////////////////////////////////////////////////////////////////////
//	
//	Implement CECSkill
//	
///////////////////////////////////////////////////////////////////////////

CECSkill::CECSkill(int id, int iLevel)
{
	if (!(m_pSkillCore = GNET::ElementSkill::Create(id, iLevel)))
	{
		//ASSERT(m_pSkillCore);
		m_pSkillCore = GNET::ElementSkill::Create(1, 1);
	}

	m_idSkill		= id;
	m_iCoolTime		= m_pSkillCore->GetCoolingTime();
	m_iCoolCnt		= 0;
	m_bCooling		= false;
	m_iChargeCnt	= 0;
	m_iChargeMax	= 0;
	m_bCharging		= false;
	m_bChanting		= false;
	m_bEmission		= false;
	m_cStamp		= 0;
	m_bExtra		= false;
	m_pItem			= NULL;

	m_bCanCastTriggerSkill = false;
	m_iTriggerSkillCnt = 0;
}

CECSkill::~CECSkill()
{
	if (m_pSkillCore)
		m_pSkillCore->Destroy();
}

const wchar_t* CECSkill::GetName()
{
	return g_pGame->GetSkillDesc()->GetWideString(GetSkillID() * 10);
}

//	Start into cooling state
//	iTotalTime: total cooling time, 0 means to use cooling time in database
void CECSkill::StartCooling(int iTotalTime, int iStartCnt)
{
	m_iCoolTime	= iTotalTime ? iTotalTime : GetCoreCoolingTime();
	m_iCoolCnt	= iStartCnt;
	m_bCooling	= true;
}

//	Start charging
void CECSkill::StartCharging(int iChargeMax)
{
	if (m_pSkillCore->IsWarmup())
	{
		m_iChargeMax = iChargeMax;
		m_iChargeCnt = 0;
		m_bCharging	 = true;
		m_bChanting = false;
	}
}

void CECSkill::StartChanting()
{
	if ( m_pSkillCore->IsWarmup() )
	{
		m_bCharging = false;
		m_bEmission = false;
		m_bChanting = true;
	}
}

void CECSkill::EndCharging()
{
	m_bCharging = false;
	m_bEmission = true;
	m_bChanting = false;  
	m_iChargeCnt = m_iChargeMax = 0; 
}

//	Change full
bool CECSkill::ChargeFull()
{
	return m_pSkillCore->IsWarmup() && m_iChargeCnt >= m_iChargeMax;
}

//	Tick routine
void CECSkill::Tick()
{
	if (m_bCooling)
	{
		//	In cooling state
		m_iCoolCnt -= (int)g_pGame->GetRealTickTime();
		if (m_iCoolCnt <= 0)
		{
			m_iCoolCnt = 0;
			m_bCooling = false;

			//	TODO: do something here ?
		}
	}

	if (m_bCharging)
	{
		//	In charging state
		m_iChargeCnt += (int)g_pGame->GetRealTickTime();
		if (m_iChargeCnt >= m_iChargeMax)
		{
			m_iChargeCnt = m_iChargeMax;
			m_bCharging	 = false;

			//	TODO: do something here ?
		}
	}
	
	if(GetCategory() == GNET::CATEGORY_TRIGGERED)
	{
		m_iTriggerSkillCnt -= (int)g_pGame->GetRealTickTime();
		if(m_iTriggerSkillCnt <=0)
		{
			m_iTriggerSkillCnt = 0;
			m_bCanCastTriggerSkill = false;
		}
		else
		{
			m_bCanCastTriggerSkill = true;
		}
	}
}

const wchar_t* CECSkill::GetDesc(int iLevel, bool bNextLv)
{
	//	Note: use static buffer to receive description, so this function isn't
	//		thread safe.
	//-------------------------------------------------------------------------
	//比较恶心的处理模式，技能系统里面并不能实时更新HostPlayer的信息，因此可能出现不同步，这里强制设置一遍玩家属性Added 2012-09-17.
#ifndef _INVENTORY_DESC_MAN
	CECHostPlayer* pHost = g_pGame->GetGameRun()->GetHostPlayer();
	if ( pHost )
	{
		pHost->SetPlayerProps( this );
	}
#endif
	//-------------------------------------------------------------------------
	wchar_t szDesc[2048];
	m_pSkillCore->GetIntroduction(szDesc, 2048, l_SkillStr, bNextLv);
	m_strDesc = szDesc;
	if (IsCategory(GNET::CATEGORY_TALISMAN))
	{
		if (m_pItem && m_pItem->GetFixSkill()==GetSkillID() && bNextLv)
		{
			// 基本技能描述
			m_pSkillCore->GetIntroduction(szDesc, 2048, l_SkillStr, false);
			m_strDesc = szDesc;
			int i;
			// 相关的基本技能描述
			for (i=CECIvtrTalismanMain::TYPE_REFINE_1; i<=CECIvtrTalismanMain::TYPE_REFINE_8; ++i)
			{
				if (m_pItem->GetInfo(i)==0)
				{
					break;
				}
				CECSkill *pSkill = new CECSkill(m_pItem->GetInfo(i), m_pSkillCore->GetLevel());
				m_strDesc += pSkill->GetDesc(m_pItem->GetInfo(i), false);
				delete pSkill;
			}
			
			// 下一阶基本技能描述
			szDesc[0] = 0;
			m_pSkillCore->GetNextLevelDescription(szDesc, 2048, l_SkillStr);
			if (szDesc[0]!=0)
			{
				m_strDesc += szDesc;
				// 相关的基本技能下一阶描述
				for (i=CECIvtrTalismanMain::TYPE_REFINE_1; i<=CECIvtrTalismanMain::TYPE_REFINE_8; ++i)
				{
					if (m_pItem->GetInfo(i)==0)
					{
						break;
					}
					CECSkill *pSkill = new CECSkill(m_pItem->GetInfo(i), m_pSkillCore->GetLevel());
					szDesc[0] = 0;
					pSkill->m_pSkillCore->GetNextLevelDescription(szDesc, 2048, l_SkillStr, false);
					m_strDesc += _AL("\r");
					m_strDesc += szDesc;
					delete pSkill;
				}
			}

			// learn condition
//			m_pSkillCore->GetLearnConditionDescrition(szDesc, 2048, l_SkillStr);
//			m_strDesc += szDesc;
		}
	}
	return m_strDesc;
}

//	Skill level up
void CECSkill::LevelUp()
{
	int iLevel = m_pSkillCore->GetBaseLevel();
	m_pSkillCore->SetLevel(++iLevel);
}

int CECSkill::GetSkillLevel()
{
	return m_pSkillCore->GetBaseLevel();
}

int CECSkill::GetSkillMaxLevel()
{
	return m_pSkillCore->GetMaxLevel();
}

void CECSkill::SetAddonLevel(int iLevel)
{
	m_pSkillCore->SetAddonLevel(iLevel);
}

int CECSkill::GetAddonLevel()
{
	return m_pSkillCore->GetAddonLevel();
}

void CECSkill::SetCommonLevel(int iLevel)
{
	m_pSkillCore->SetCommonLevel(iLevel);
}

//	Check cast condition
int CECSkill::CheckCastCondition(GNET::UseRequirement& req)
{
	if (GetSkillLevel()==0)
	{
		return 13;
	}
	return m_pSkillCore->Condition(req);
}

void CECSkill::SetPlayerProps(GNET::PlayerProps& props)
{
	m_pSkillCore->SetPlayerLevel(props.level);
	m_pSkillCore->SetPlayerOccupation( props.occupation ); //Added 2011-04-18
	m_pSkillCore->SetPlayerDarklight(props.darklight);
}