/*
 * FILE: EC_HPWorkFly.cpp
 *
 * DESCRIPTION: 
 *
 * CREATED BY: Duyuxin, 2004/11/23
 *
 * HISTORY: 
 *
 * Copyright (c) 2004 Archosaur Studio, All Rights Reserved.
 */

#include "EC_Global.h"
#include "EC_HPWorkFly.h"
#include "EC_HostPlayer.h"
#include "EC_Configs.h"
#include "EC_Game.h"
#include "EC_GameRun.h"
#include "EC_FullGlowRender.h"
#include "EC_ComboSkill.h"
#include "EC_CDR.h"
#include "A3DFuncs.h"
#include "EC_World.h"
#include "A3DTerrain2.h"

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

DWORD fly_act = 1400;
DWORD fly_prepare = 1820;
DWORD fly_launch = 2800;
float lauch_hei = 2.f;
float launch_vel = lauch_hei * 1000.0f / (fly_prepare - fly_act);

///////////////////////////////////////////////////////////////////////////
//	
//	Local functions
//	
///////////////////////////////////////////////////////////////////////////

extern const int fly_takeoff_act[3] = 
{
	CECPlayer::ACT_TAKEOFF_SWORD,
	CECPlayer::ACT_TAKEOFF_WING,
	CECPlayer::ACT_TAKEOFF_FEIQI,
};

extern const int fly_hang_act[3] =
{
	CECPlayer::ACT_HANGINAIR_SWORD,
	CECPlayer::ACT_HANGINAIR_WING,
	CECPlayer::ACT_HANGINAIR_FEIQI,
};


///////////////////////////////////////////////////////////////////////////
//	
//	Implement CECHPWorkFly
//	
///////////////////////////////////////////////////////////////////////////

CECHPWorkFly::CECHPWorkFly(CECHPWorkMan* pWorkMan) :
CECHPWork(WORK_FLYOFF, pWorkMan)
{
	m_dwMask		= MASK_FLYOFF;
	m_dwTransMask	= MASK_STAND | MASK_MOVETOPOS;

	CECHPWorkFly::Reset();
}

CECHPWorkFly::~CECHPWorkFly()
{
}

//	Reset work
void CECHPWorkFly::Reset()
{
	CECHPWork::Reset();

	m_dwLaunchTime	= 0;
	m_bAddSpeed		= true;
}

//	Copy work data
bool CECHPWorkFly::CopyData(CECHPWork* pWork)
{
	if (!CECHPWork::CopyData(pWork))
		return false;

	CECHPWorkFly* pSrc = (CECHPWorkFly*)pWork;

	m_vDestPos		= pSrc->m_vDestPos;
	m_dwLaunchTime	= pSrc->m_dwLaunchTime;
	m_bContinueFly	= pSrc->m_bContinueFly;
	m_bSkillFly		= pSrc->m_bSkillFly;
	m_bAddSpeed		= pSrc->m_bAddSpeed;

	return true;
}

//	Work is cancel
void CECHPWorkFly::Cancel()
{
	m_pHost->m_pCDRInfo->vAbsVelocity.Clear();
#ifdef CDR_HL2_MOVE
	m_pHost->m_pCDRInfo->vClipVel.Clear();
#else
	m_pHost->m_pCDRInfo->fYVel = 0.0f;
#endif
		
}

//	On first tick
void CECHPWorkFly::OnFirstTick()
{
	m_pHost->m_iMoveEnv		= CECPlayer::MOVEENV_AIR;
	m_pHost->m_iMoveMode	= CECPlayer::MOVE_MOVE;

	m_pHost->m_vVelocity.Clear();
	m_pHost->m_vAccel.Clear();
	m_pHost->ResetJump();

	m_vDestPos = m_pHost->GetPos();
	m_vDestPos.y += lauch_hei;

	m_bAddSpeed = true;

	m_pHost->m_bFlyingOff = true;
	m_pHost->m_FlyingOffCnt.Reset();

	//m_pHost->PlayAction(m_pHost->GetMoveStandAction(false), 1.0f, true, 300);
	// 根据飞行方式选择起飞动作
	int fly_mode = m_pHost->GetFlyMode();

	if(m_bSkillFly)	// 技能导致的飞行效果，则起飞播放翅膀动作
	{
		fly_mode = CECPlayer::FLYMODE_WING;

		fly_act = 10;
		fly_prepare = 12;
		fly_launch = 20;
		lauch_hei = 2.f;
		launch_vel =  4.76f;
	}
	else
	{
		fly_act = 1400;
		fly_prepare = 1820;
		fly_launch = 2800;
		lauch_hei = 2.f;
		launch_vel = lauch_hei * 1000.0f / (fly_prepare - fly_act);
	}


	m_pHost->PlayAction(fly_takeoff_act[fly_mode], 1.0f);
	m_pHost->PlayAction(fly_hang_act[fly_mode], 1.0f, false, 2000, true);
	if (fly_mode == CECPlayer::FLYMODE_WING)
	{
		if(m_bSkillFly)	// 技能导致的飞行效果，则不显示脚底下的飞剑
		{
			m_pHost->m_bFlyingOff = false;	// 无需修正飞剑不显示的bug
			m_pHost->ShowWing(false);
		}
		else
			m_pHost->ShowWing(true);
	}
}

//	Tick routine
bool CECHPWorkFly::Tick(DWORD dwDeltaTime)
{
	CECHPWork::Tick(dwDeltaTime);
	m_dwLaunchTime += dwDeltaTime;

	if (m_dwLaunchTime < fly_act)
		return true;
	else if (m_dwLaunchTime < fly_prepare)
	{
		m_pHost->ShowWing(true);

		if (m_bContinueFly)
			return true;
		else
		{
			float fDeltaTime = dwDeltaTime * 0.001f;
			A3DVECTOR3 vCurPos = m_pHost->m_MoveCtrl.AirWaterMove(g_vAxisY * launch_vel, fDeltaTime, true);
			/*
			float fVerSpeed = m_bAddSpeed ? launch_vel : 0.0f;
			m_bAddSpeed = false;
			A3DVECTOR3 vCurPos = m_pHost->m_MoveCtrl.GroundMove(g_vOrigin, 0.0f, fDeltaTime, fVerSpeed, 0.0f);
			*/
			m_pHost->SetPos(vCurPos);
		}
	}
	else if (m_dwLaunchTime < fly_launch && !m_bContinueFly)
		return true;
	else
	{
		m_bFinished = true;
		A3DVECTOR3 vCurPos = m_pHost->GetPos();
		m_pHost->m_MoveCtrl.SendStopMoveCmd(vCurPos, 0, GP_MOVE_AIR | GP_MOVE_RUN);

		int fly_mode = m_pHost->GetFlyMode();
	
		if(m_bSkillFly)	// 技能导致的飞行效果，则下落播放翅膀动作
			fly_mode = CECPlayer::FLYMODE_WING;

		m_pHost->PlayAction(fly_hang_act[fly_mode], 1.0f, true, 300);
	}

	return true;
}

///////////////////////////////////////////////////////////////////////////
//	
//	Implement CECHPWorkFMove
//	
///////////////////////////////////////////////////////////////////////////

CECHPWorkFMove::CECHPWorkFMove(CECHPWorkMan* pWorkMan) :
CECHPWork(WORK_FLASHMOVE, pWorkMan)
{
	m_dwMask		= MASK_FLYOFF;
	m_dwTransMask	= MASK_STAND;

	CECHPWorkFMove::Reset();
}

CECHPWorkFMove::~CECHPWorkFMove()
{
}

//	Reset work
void CECHPWorkFMove::Reset()
{
	CECHPWork::Reset();

	m_fSpeed	= 0.0f;
	m_fDist		= 0.0f;
	m_fDistCnt	= 0.0f;
	m_fYDelta	= 0.0f;
}

//	Copy work data
bool CECHPWorkFMove::CopyData(CECHPWork* pWork)
{
	if (!CECHPWork::CopyData(pWork))
		return false;

	CECHPWorkFMove* pSrc = (CECHPWorkFMove*)pWork;

	m_vDestPos	= pSrc->m_vDestPos;
	m_fSpeed	= pSrc->m_fSpeed;
	m_fDist		= pSrc->m_fDist;
	m_fDistCnt	= pSrc->m_fDistCnt;
	m_vMoveDir	= pSrc->m_vMoveDir;
	m_fYDelta	= pSrc->m_fYDelta;

	return true;
}

//	Prepare to move
void CECHPWorkFMove::PrepareMove(const A3DVECTOR3& vDestPos, float fMoveTime)
{
	if (fMoveTime <= 0.0f)
	{
		ASSERT(fMoveTime > 0.0f);
		return;
	}

	m_vDestPos	= vDestPos;
	m_fDistCnt	= 0.0f;
	m_vMoveDir	= vDestPos - m_pHost->GetPos();
	m_vMoveDir.y = 0.0f; //
	m_fDist		= m_vMoveDir.Normalize();
	m_fSpeed	= m_fDist / fMoveTime;
	//Test
	float fCurTerrainHeight = g_pGame->GetGameRun()->GetWorld()->GetTerrainHeight(m_pHost->GetPos());
	m_fYDelta = m_pHost->GetPos().y - fCurTerrainHeight;
	//Test end
}

//	On first tick
void CECHPWorkFMove::OnFirstTick()
{
//	m_pHost->SetPos(m_pHost->GetPos() + A3DVECTOR3(0.0f, 0.7f, 0.0f));
//	m_pHost->m_MoveCtrl.SetHostLastPos(m_pHost->GetPos() + A3DVECTOR3(0.0f, 0.7f, 0.0f));

//	m_pHost->PlayAction(CECPlayer::ACT_JUMP_START, 1.0f, true, 0);
//	m_pHost->PlayAction(CECPlayer::ACT_JUMP_LOOP, 1.0f, true, 0, true);
	m_pHost->PlayAction(CECPlayer::ACT_FIGHT_CHARGE, 1.0f, true, 0, false);

	CECFullGlowRender * pGlow = g_pGame->GetGameRun()->GetFullGlowRender();
	if( pGlow && pGlow->IsGlowOn() )
	{
		pGlow->SetMotionBlur(0.8f, 0.8f, 1);
	}
}

//	Tick routine
bool CECHPWorkFMove::Tick(DWORD dwDeltaTime)
{
	CECHPWork::Tick(dwDeltaTime);

	//Added 2011-07-19.
	float fMoveSpeed;
	if(m_pHost->m_iBeChargeType == 4)
		fMoveSpeed = 5.0f;
	else if(m_pHost->m_iBeChargeType == 3 || m_pHost->m_iBeChargeType == 5)
		fMoveSpeed = 10.0f;
	else 
		fMoveSpeed = 25.0f;
	float fDeltaTime = dwDeltaTime * 0.001f;
	A3DVECTOR3 vCurPos = m_pHost->GetPos();
	m_vMoveDir = m_vDestPos - vCurPos;
	m_vMoveDir.y = 0.0f;
	if (!m_vMoveDir.IsZero())
	{
		m_pHost->SetDirAndUp(m_vMoveDir, g_vAxisY);
	}
	float fDist = m_vMoveDir.Normalize();
	vCurPos = m_pHost->m_MoveCtrl.GroundMove(m_vMoveDir, fMoveSpeed, fDeltaTime, m_pHost->m_fVertSpeed);
	if (m_pHost->m_MoveCtrl.MoveBlocked() >= 3)
	{
		// blocked, so we clear the fYVel of cdr!
		//cdr.fYVel = 0.0f;

		m_pHost->m_MoveCtrl.Reset();
		Finish();
		return true;
//		m_pHost->m_MoveCtrl.SendStopMoveCmd(vCurPos, fSpeed, iMoveMode);
	}
	float fMoveDelta = a3d_MagnitudeH(vCurPos - m_pHost->GetPos());
	if ( fMoveDelta >= fDist )
	{
		m_pHost->SetPos(m_vDestPos);
		m_pHost->m_MoveCtrl.Reset();
		m_pHost->m_MoveCtrl.SetLastSevPos(m_vDestPos);
		m_pHost->m_MoveCtrl.SetHostLastPos(m_vDestPos);
		m_pHost->m_pCDRInfo->vClipVel.Clear();
		if (m_pHost->m_pCDRInfo->bOnSurface && !m_pHost->m_pCDRInfo->vTPNormal.IsZero())
		{
			m_pHost->SetGroundNormal(m_pHost->m_pCDRInfo->vTPNormal);
		}

		CECFullGlowRender* pGlow = g_pGame->GetGameRun()->GetFullGlowRender();
		if (pGlow && pGlow->IsGlowOn())
		{
			pGlow->SetMotionBlur(0.8f, 0.0f, 3000);
		}

		//	Finish work
		Finish();

		//send move cmd to gs.
//		m_pHost->m_MoveCtrl.SendStopMoveCmd(vCurPos, fMoveSpeed, GP_MOVE_QINGGONG);
	}
	else
	{
		m_pHost->SetPos(vCurPos);
		m_pHost->m_MoveCtrl.SetHostLastPos(vCurPos);
//		m_pHost->m_MoveCtrl.SendMoveCmd(vCurPos, 0, m_vDestPos, m_pHost->m_pCDRInfo->vAbsVelocity, GP_MOVE_QINGGONG);
	}
	//Added end.
/*	m_fSpeed = 15.0f;
	float fMoveDelta = m_fSpeed * dwDeltaTime * 0.001f;
	A3DVECTOR3 vCurPos = m_pHost->GetPos();

	if (m_fDistCnt + fMoveDelta >= m_fDist)
	{
		fMoveDelta = m_fDist - m_fDistCnt;
		m_fDistCnt = m_fDist;

		m_pHost->SetPos(m_vDestPos);
		m_pHost->m_MoveCtrl.Reset();
		m_pHost->m_MoveCtrl.SetLastSevPos(m_vDestPos);
		m_pHost->m_MoveCtrl.SetHostLastPos(m_vDestPos);

	//	if (!m_pHost->m_pCDRInfo->vTPNormal.IsZero())
		if (m_pHost->m_pCDRInfo->bOnSurface && !m_pHost->m_pCDRInfo->vTPNormal.IsZero())
			m_pHost->SetGroundNormal(m_pHost->m_pCDRInfo->vTPNormal);

		CECFullGlowRender* pGlow = g_pGame->GetGameRun()->GetFullGlowRender();
		if (pGlow && pGlow->IsGlowOn())
			pGlow->SetMotionBlur(0.8f, 0.0f, 3000);

		//	Finish work
		Finish();
	}
	else
	{
		m_fDistCnt += fMoveDelta;
		vCurPos += m_vMoveDir * fMoveDelta;
		float fTerrHeight = g_pGame->GetGameRun()->GetWorld()->GetTerrainHeight(vCurPos);
		vCurPos.y = m_fYDelta + fTerrHeight;
		m_pHost->SetPos(vCurPos);
		m_pHost->m_MoveCtrl.SetHostLastPos(vCurPos);
	}
*/
	return true;
}

//	Finish work
void CECHPWorkFMove::Finish()
{
	m_bFinished = true;

	CECComboSkill* pcs = m_pHost->m_pComboSkill;
	if (pcs && !pcs->IsStop())
		pcs->Continue(false);

	//Added 2011-09-05.
	m_pHost->m_MoveCtrl.ResetMoveTime();
	//Added end.
	m_pHost->m_iBeChargeType = 0;
}





