/*
 * FILE: EC_HPWorkSpell.cpp
 *
 * DESCRIPTION: 
 *
 * CREATED BY: Duyuxin, 2004/3/8
 *
 * HISTORY: 
 *
 * Copyright (c) 2004 Archosaur Studio, All Rights Reserved.
 */

#include "EC_Global.h"
#include "EC_HPWorkSpell.h"
#include "EC_HostPlayer.h"
#include "EC_GameRun.h"
#include "EC_InputCtrl.h"
#include "EC_HostInputFilter.h"
#include "EC_Skill.h"
#include "EC_Game.h"
#include "EC_GameSession.h"

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


///////////////////////////////////////////////////////////////////////////
//	
//	Local functions
//	
///////////////////////////////////////////////////////////////////////////


///////////////////////////////////////////////////////////////////////////
//	
//	Implement CECHPWorkSpell
//	
///////////////////////////////////////////////////////////////////////////

CECHPWorkSpell::CECHPWorkSpell(CECHPWorkMan* pWorkMan) :
CECHPWork(WORK_SPELLOBJECT, pWorkMan)
{
	m_dwMask		= MASK_SPELLOBJECT;
	m_dwTransMask	= MASK_STAND | MASK_MOVETOPOS | MASK_TRACEOBJECT;

	CECHPWorkSpell::Reset();
}

CECHPWorkSpell::~CECHPWorkSpell()
{
}

//	Reset work
void CECHPWorkSpell::Reset()
{
	CECHPWork::Reset();

	m_OverTimeCnt.Reset();

	m_pSkill	= NULL;
	m_iState	= ST_INCANT;
	m_idTarget	= 0;
	m_iChargeTime = -1;
}

//	Copy work data
bool CECHPWorkSpell::CopyData(CECHPWork* pWork)
{
	if (!CECHPWork::CopyData(pWork))
		return false;

	CECHPWorkSpell* pSrc = (CECHPWorkSpell*)pWork;

	m_pSkill	= pSrc->m_pSkill;
	m_iState	= pSrc->m_iState;
	m_idTarget	= pSrc->m_idTarget;
	m_iChargeTime = pSrc->m_iChargeTime;

	m_OverTimeCnt.SetPeriod(pSrc->m_OverTimeCnt.GetPeriod());
	m_OverTimeCnt.SetCounter(pSrc->m_OverTimeCnt.GetCounter());
	
	return true;
}

//	Prepare cast
void CECHPWorkSpell::PrepareCast(int idTarget, CECSkill* pSkill, int iIncantTime)
{
	ASSERT(pSkill);
	m_pSkill	= pSkill;
	m_iState	= ST_INCANT;
	m_idTarget	= idTarget;
//	m_iChargeTime = iChargeTime;

//	if(!pSkill->IsChargeable())
	{		
		if (iIncantTime < 0)
			iIncantTime = 0x7fffffff;
		else
			iIncantTime *= 2;
	}

	m_OverTimeCnt.SetPeriod(iIncantTime);
	m_OverTimeCnt.Reset();
}

//	Change state
void CECHPWorkSpell::ChangeState(int iState)
{
	m_iState = iState;
}

//	On first tick
void CECHPWorkSpell::OnFirstTick()
{
	m_pHost->m_iMoveMode = CECHostPlayer::MOVE_STAND;
}

//	Tick routine
bool CECHPWorkSpell::Tick(DWORD dwDeltaTime)
{
	CECHPWork::Tick(dwDeltaTime);

	if(m_pSkill->IsChargeable() && m_pHost->GetIncantCnt().IsFull() && m_iChargeTime > 0)
	{
		CECInputFilter* pInputFilter = g_pGame->GetGameRun()->GetInputCtrl()->GetKBFilter();
         if(pInputFilter->IsLogicKeyPressed(m_pHost->GetCurSkillKeyCode()))
		//if (GetAsyncKeyState(m_pHost->GetCurSkillKeyCode()) & 0x8000)
		{
			m_pSkill->StartCharging(m_iChargeTime);
			m_pHost->PlayAttackEffect(m_idTarget, m_pSkill->GetSkillID(), m_pSkill->GetSkillLevel(), m_pSkill->GetStamp(), -1, 0);
		}
		else
		{	
			m_pSkill->EndCharging();
			m_pHost->PlayAttackEffect(m_idTarget, m_pSkill->GetSkillID(), m_pSkill->GetSkillLevel(), m_pSkill->GetStamp(), -1, 0);
			g_pGame->GetGameSession()->c2s_CmdContinueAction();
		}
		m_iChargeTime = -1;
	}
	// 蓄力结束，释放技能
	if (m_pSkill->IsChargeable() && m_pSkill->GetChargingCnt() &&  m_pSkill->ChargeFull())
	{
		m_pSkill->EndCharging();
		m_pHost->PlayAttackEffect(m_idTarget, m_pSkill->GetSkillID(), m_pSkill->GetSkillLevel(), m_pSkill->GetStamp(), -1, 0);
		g_pGame->GetGameSession()->c2s_CmdContinueAction();	
	}
/*	if (m_pSkill && m_idTarget)
	{
		//	Check whether target is still attackable when spelling negative
		//	effect skills
		if (m_pSkill->GetType() == CECSkill::TYPE_ATTACK || 
			m_pSkill->GetType() == CECSkill::TYPE_CURSE)
		{
			if (m_pHost->AttackableJudge(m_idTarget, true) != 1)
			{
				m_bFinished = true;
				return true;
			}
		}
	}
*/
//	if (m_iState == ST_INCANT)
	{
		if (m_OverTimeCnt.IncCounter(dwDeltaTime))
			m_bFinished = true;
	}

	if (m_idTarget && m_idTarget != m_pHost->GetCharacterID())
		m_pHost->TurnFaceTo(m_idTarget);

	return true;
}


///////////////////////////////////////////////////////////////////////////////
//
//CECHPWorkSpecialCharge类的定义 Added 2011-08-01.
//
///////////////////////////////////////////////////////////////////////////////
/**
*@param: CECHPWorkMan,传入work 管理器对象指针
*@return: void
*@usage: 简单的初始化工作
**/
CECHPWorkSpecialCharge::CECHPWorkSpecialCharge(CECHPWorkMan* pWorkMan) :
CECHPWork(WORK_SPECIAL_CHARGE, pWorkMan)
{
	m_dwMask		= MASK_SPECIAL_CHARGE;
	m_dwTransMask	= MASK_STAND | MASK_MOVETOPOS | MASK_TRACEOBJECT;

	CECHPWorkSpecialCharge::Reset();
}

CECHPWorkSpecialCharge::~CECHPWorkSpecialCharge()
{

}

//	Reset work
void CECHPWorkSpecialCharge::Reset()
{
	CECHPWork::Reset();
}

//	Copy work data
bool CECHPWorkSpecialCharge::CopyData(CECHPWork* pWork)
{
	if (!CECHPWork::CopyData(pWork))
	{
		return false;
	}

//	CECHPWorkSpecialCharge* pSrc = (CECHPWorkSpecialCharge*)pWork;
	
	return true;
}

//	On first tick
void CECHPWorkSpecialCharge::OnFirstTick()
{
	m_pHost->m_iMoveMode = CECHostPlayer::MOVE_STAND;

	// 播放拉扯循环动作
	m_pHost->PlayAction( CECPlayer::ACT_FIGHT_PULL_LOOP, 1.0f, true );
}

bool CECHPWorkSpecialCharge::Tick(DWORD dwDeltaTime)
{
	CECHPWork::Tick(dwDeltaTime);

	CECInputFilter* pInputFilter = g_pGame->GetGameRun()->GetInputCtrl()->GetKBFilter();
	if(pInputFilter->IsLogicKeyPressed(m_pHost->GetCurSkillKeyCode()))
	//if ( GetAsyncKeyState(m_pHost->GetCurSkillKeyCode()) & 0x8000 ) //如果当前按住了技能快捷键
	{

	}
	else	//如果没有按住技能快捷键，则直接通知gs该技能效果取消
	{
		g_pGame->GetGameSession()->c2s_CmdCancelPulling();
		m_bFinished = true;
		m_pHost->SetSpecialCharge( false );
	}

	//
	return true;
}