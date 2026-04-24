/*
 * FILE: EC_HPWorkSit.cpp
 *
 * DESCRIPTION: 
 *
 * CREATED BY: Duyuxin, 2004/12/27
 *
 * HISTORY: 
 *
 * Copyright (c) 2004 Archosaur Studio, All Rights Reserved.
 */

#include "EC_Global.h"
#include "EC_HPWorkSit.h"
#include "EC_HostPlayer.h"
#include "EC_Game.h"
#include "EC_GameRun.h"
#include "EC_World.h"
#include "EC_AssureMove.h"
#include "EC_ManPlayer.h"

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
//	Implement CECHPWorkSit
//	
///////////////////////////////////////////////////////////////////////////

CECHPWorkSit::CECHPWorkSit(CECHPWorkMan* pWorkMan) :
CECHPWork(WORK_SIT, pWorkMan)
{
	m_dwMask		= MASK_SIT;
	m_dwTransMask	= MASK_STAND;

	CECHPWorkSit::Reset();
}

CECHPWorkSit::~CECHPWorkSit()
{
}

//	Reset work
void CECHPWorkSit::Reset()
{
	CECHPWork::Reset();

	m_bBeSitting = false;
}

//	Copy work data
bool CECHPWorkSit::CopyData(CECHPWork* pWork)
{
	if (!CECHPWork::CopyData(pWork))
		return false;

	CECHPWorkSit* pSrc = (CECHPWorkSit*)pWork;

	m_bBeSitting = pSrc->m_bBeSitting;

	return true;
}

//	On first tick
void CECHPWorkSit::OnFirstTick()
{
	m_pHost->m_iMoveMode = CECHostPlayer::MOVE_STAND;

	if (m_bBeSitting)
		m_pHost->PlayAction(CECHostPlayer::ACT_SITDOWN_LOOP, 1.0f);
	else
	{
		m_pHost->PlayAction(CECHostPlayer::ACT_SITDOWN, 1.0f);
		m_pHost->PlayAction(CECHostPlayer::ACT_SITDOWN_LOOP, 1.0f, true, 200, true);
	}
}

//	Tick routine
bool CECHPWorkSit::Tick(DWORD dwDeltaTime)
{
	CECHPWork::Tick(dwDeltaTime);

	return true;
}

///////////////////////////////////////////////////////////////////////////
//	
//	Implement CECHPWorkPick
//	
///////////////////////////////////////////////////////////////////////////

CECHPWorkPick::CECHPWorkPick(CECHPWorkMan* pWorkMan) :
CECHPWork(WORK_PICKUP, pWorkMan)
{
	m_dwMask		= MASK_PICKUP;
	m_dwTransMask	= MASK_STAND | MASK_MOVETOPOS;
	m_nOprt			= 0;

	m_TimeCnt.SetPeriod(500);

	CECHPWorkPick::Reset();
}

CECHPWorkPick::~CECHPWorkPick()
{
}

//	Reset work
void CECHPWorkPick::Reset()
{
	CECHPWork::Reset();

	m_TimeCnt.Reset();
}

//	Copy work data
bool CECHPWorkPick::CopyData(CECHPWork* pWork)
{
	if (!CECHPWork::CopyData(pWork))
		return false;

	CECHPWorkPick* pSrc = (CECHPWorkPick*)pWork;

	m_bIsGather	= pSrc->m_bIsGather;

	m_TimeCnt.SetPeriod(pSrc->m_TimeCnt.GetPeriod());
	m_TimeCnt.SetCounter(pSrc->m_TimeCnt.GetCounter());

	return true;
}

//	On first tick
void CECHPWorkPick::OnFirstTick()
{
	if (IsGather())
	{
		m_pHost->PlayAction(CECPlayer::ACT_PICKUP, 1.0f);
		m_pHost->PlayAction(CECPlayer::ACT_PICKUP_LOOP, 1.0f, false, 200, true);
	}
	else
	{
		m_pHost->PlayAction(CECPlayer::ACT_PICKUP_MATTER, 1.0f);
		m_pHost->PlayAction(CECPlayer::ACT_STAND, 1.0f, false, 200, true);
	}
}

//	Tick routine
bool CECHPWorkPick::Tick(DWORD dwDeltaTime)
{
	DWORD dwRealTime = g_pGame->GetRealTickTime();

	CECHPWork::Tick(dwRealTime);

	if (!IsGather() && m_TimeCnt.IncCounter(dwRealTime))
	{
		m_bFinished = true;
	}

	return true;
}

///////////////////////////////////////////////////////////////////////////
//	
//	Implement CECHPWorkGeneralOprt
//	
///////////////////////////////////////////////////////////////////////////

CECHPWorkGeneralOprt::CECHPWorkGeneralOprt(CECHPWorkMan* pWorkMan) :
CECHPWork(WORK_GENERAL_OPRT, pWorkMan)
{
	m_dwMask		= MASK_GENERAL_OPRT;
	m_dwTransMask	= MASK_STAND | MASK_MOVETOPOS;

	CECHPWorkGeneralOprt::Reset();
}

CECHPWorkGeneralOprt::~CECHPWorkGeneralOprt()
{
}

//	Reset work
void CECHPWorkGeneralOprt::Reset()
{
	CECHPWork::Reset();
}

//	Copy work data
bool CECHPWorkGeneralOprt::CopyData(CECHPWork* pWork)
{
	if (!CECHPWork::CopyData(pWork))
		return false;

	return true;
}

//	On first tick
void CECHPWorkGeneralOprt::OnFirstTick()
{
	if( m_pHost->GetAttachMode() == CECPlayer::enumAttachNone && !m_pHost->GetPetModel() )
		m_pHost->PlayAction(CECPlayer::ACT_GENERAL_OPRT, 1.0f);
}

//	Tick routine
bool CECHPWorkGeneralOprt::Tick(DWORD dwDeltaTime)
{
	DWORD dwRealTime = g_pGame->GetRealTickTime();
	CECHPWork::Tick(dwRealTime);
	return true;
}

///////////////////////////////////////////////////////////////////////////
//	
//	Implement CECHPWorkConcentrate
//	
///////////////////////////////////////////////////////////////////////////

CECHPWorkConcentrate::CECHPWorkConcentrate(CECHPWorkMan* pWorkMan) :
CECHPWork(WORK_CONCENTRATE, pWorkMan)
{
	m_dwMask		= MASK_CONCENTRATE;
	m_dwTransMask	= MASK_STAND;
	m_iDoWhat		= DO_SUMMONPET;

	CECHPWorkConcentrate::Reset();
}

CECHPWorkConcentrate::~CECHPWorkConcentrate()
{
}

//	Reset work
void CECHPWorkConcentrate::Reset()
{
	CECHPWork::Reset();
}

//	Copy work data
bool CECHPWorkConcentrate::CopyData(CECHPWork* pWork)
{
	if (!CECHPWork::CopyData(pWork))
		return false;

	CECHPWorkConcentrate* pSrc = (CECHPWorkConcentrate*)pWork;
	m_iDoWhat = pSrc->m_iDoWhat;

	return true;
}

//	On first tick
void CECHPWorkConcentrate::OnFirstTick()
{
}

//	Tick routine
bool CECHPWorkConcentrate::Tick(DWORD dwDeltaTime)
{
	DWORD dwRealTime = g_pGame->GetRealTickTime();

	CECHPWork::Tick(dwRealTime);

	return true;
}

///////////////////////////////////////////////////////////////////////////
//	
//	Implement CECHPWorkBeBound
//	
///////////////////////////////////////////////////////////////////////////

CECHPWorkBeBound::CECHPWorkBeBound(CECHPWorkMan* pWorkMan) :
CECHPWork(WORK_BEBOUND, pWorkMan)
{
	m_dwMask		= MASK_BEBOUND;
	m_dwTransMask	= MASK_STAND;

	CECHPWorkBeBound::Reset();
}

CECHPWorkBeBound::~CECHPWorkBeBound()
{
}

//	Reset work
void CECHPWorkBeBound::Reset()
{
	CECHPWork::Reset();
}

//	Copy work data
bool CECHPWorkBeBound::CopyData(CECHPWork* pWork)
{
	if (!CECHPWork::CopyData(pWork))
		return false;

	CECHPWorkBeBound* pSrc = (CECHPWorkBeBound*)pWork;

	return true;
}

//	On first tick
void CECHPWorkBeBound::OnFirstTick()
{
}

//	Tick routine
bool CECHPWorkBeBound::Tick(DWORD dwDeltaTime)
{
	DWORD dwRealTime = g_pGame->GetRealTickTime();

	CECHPWork::Tick(dwRealTime);

	if( g_pGame->GetGameRun()->GetWorld()->GetAssureMove() )
	{
		g_pGame->GetGameRun()->GetWorld()->GetAssureMove()->AssureMove(m_pHost->GetPos(), m_pHost->GetPos());
		CECPlayer * pPlayer = g_pGame->GetGameRun()->GetWorld()->GetPlayerMan()->GetPlayer(m_pHost->GetBuddyID());

		if( pPlayer && pPlayer->IsFlying() )
			g_pGame->GetGameRun()->GetWorld()->GetAssureMove()->NoAssureMove();
	}

	return true;
}

///////////////////////////////////////////////////////////////////////////
//	
//	Implement CECHPWorkBlewUp
//	
///////////////////////////////////////////////////////////////////////////

CECHPWorkBlewUp::CECHPWorkBlewUp(CECHPWorkMan* pWorkMan) :
CECHPWork(WORK_BLEW_UP, pWorkMan)
{
	m_dwMask		= MASK_BLEW_UP;
	m_dwTransMask	= MASK_STAND;
	CECHPWorkBlewUp::Reset();
}

CECHPWorkBlewUp::~CECHPWorkBlewUp()
{
}

//	Reset work
void CECHPWorkBlewUp::Reset()
{
	CECHPWork::Reset();
	m_dwPastTime = 0;
	m_dwDuration = 0;
	m_fSpeed = 0;
	m_fAccelerate = 0;
}

//	Copy work data
bool CECHPWorkBlewUp::CopyData(CECHPWork* pWork)
{
	if (!CECHPWork::CopyData(pWork))
		return false;

	CECHPWorkBlewUp* pSrc = (CECHPWorkBlewUp*)pWork;
	
	m_dwPastTime = pSrc->m_dwPastTime;
	m_dwDuration = pSrc->m_dwDuration;
	m_fSpeed = pSrc->m_fSpeed;
	m_fAccelerate = pSrc->m_fAccelerate;
	m_vecPostion = pSrc->m_vecPostion;
	m_fHeight = pSrc->m_fHeight;

	return true;
}

//	On first tick
void CECHPWorkBlewUp::OnFirstTick()
{
	m_pHost->PlayAction(CECPlayer::ACT_WOUNDED, 1.0f);;
}

//	Tick routine
bool CECHPWorkBlewUp::Tick(DWORD dwDeltaTime)
{
	CECHPWork::Tick(dwDeltaTime);

	m_dwPastTime += 50;
	if(m_dwPastTime > m_dwDuration)
	{
		m_bFinished = true;
		m_vecPostion.y = m_fHeight;
		m_pHost->SetPos(m_vecPostion);
		return true;
	}
	else
	{
		if(m_dwPastTime < m_dwDuration / 2)   //up
		{
			m_fSpeed = m_fSpeed - m_fAccelerate * 0.05;
			if(m_fSpeed > 0)
			{
				m_vecPostion.y += m_fSpeed * 0.05;
				m_pHost->SetPos(m_vecPostion);
			}
		}
		else						//down
		{
			m_fSpeed = m_fSpeed + m_fAccelerate * 0.05;
			if(m_fSpeed > 0)
			{
				m_vecPostion.y -= m_fSpeed * 0.05;
				if(m_vecPostion.y > m_fHeight)
					m_pHost->SetPos(m_vecPostion);
				else
				{
					m_bFinished = true;
					m_vecPostion.y = m_fHeight;
					m_pHost->SetPos(m_vecPostion);
					return true;
				}
			}
		}
	}
	return true;
}


void CECHPWorkBlewUp::PrepareBlew(DWORD DWDuration, float fSpeed)
{
	m_dwDuration = DWDuration;
	m_fSpeed = fSpeed;
	m_fAccelerate = (float)m_fSpeed / (float)(m_dwDuration * 0.0005);

	m_fHeight = m_pHost->GetPos().y;
	m_vecPostion = m_pHost->GetPos();
}

///////////////////////////////////////////////////////////////////////////
//	
//	Implement CECHPWorkParabolic
//	
///////////////////////////////////////////////////////////////////////////

CECHPWorkParabolic::CECHPWorkParabolic(CECHPWorkMan* pWorkMan) :
CECHPWork(WORK_PARABOLIC, pWorkMan)
{
	m_dwMask		= MASK_PARABOLIC;
	m_dwTransMask	= MASK_STAND;
	CECHPWorkParabolic::Reset();
}

CECHPWorkParabolic::~CECHPWorkParabolic()
{
}

//	Reset work
void CECHPWorkParabolic::Reset()
{
	CECHPWork::Reset();
	m_dwStartTime = 0;
	m_dwDuration  = 0;
	m_fSpeedH	  = 0;
	m_fSpeedV     = 0;
	m_fAccelerate = 0;
	m_fHeight     = 0;
	m_fDistance   = 0;
}

//	Copy work data
bool CECHPWorkParabolic::CopyData(CECHPWork* pWork)
{
	if (!CECHPWork::CopyData(pWork))
		return false;

	CECHPWorkParabolic* pSrc = (CECHPWorkParabolic*)pWork;

	m_dwStartTime = pSrc->m_dwStartTime;
	m_dwDuration  = pSrc->m_dwDuration;
	m_fSpeedH	  = pSrc->m_fSpeedH;
	m_fSpeedV	  = pSrc->m_fSpeedV;
	m_fAccelerate = pSrc->m_fAccelerate;
	m_vecPostion  = pSrc->m_vecPostion;
	m_vecDest	  = pSrc->m_vecDest;
	m_fHeight     = pSrc->m_fHeight;
	m_fDistance   = pSrc->m_fDistance;

	return true;
}

//	On first tick  改变飞行状态下速度、离地最小高度的限制
void CECHPWorkParabolic::OnFirstTick()
{
	m_fRunSpeedBackup = m_pHost->m_ExtProps.run_speed;
	m_pHost->m_ExtProps.run_speed  = 40;

	m_fMinAirHeiBackup = m_pHost->m_MoveConst.fMinAirHei; 
	m_pHost->m_MoveConst.fMinAirHei = 0.05;
}

bool CECHPWorkParabolic::Tick(DWORD dwDeltaTime)
{
	CECHPWork::Tick(dwDeltaTime);

	DWORD dwPastTime = GetTickCount() - m_dwStartTime;
	float fDeltaTime = dwDeltaTime * 0.001f; 
	if(dwPastTime > m_dwDuration)
	{
		Finish();
		return true;
	}
	
	int dirV = 1;

	if(dwPastTime < m_dwDuration / 2)   //up
	{
		if(m_fSpeedV - m_fAccelerate * fDeltaTime > 0)
			m_fSpeedV = m_fSpeedV - m_fAccelerate * fDeltaTime;
		dirV = 1;
	}
	else
	{
		if(m_fSpeedV + m_fAccelerate * fDeltaTime > 0)
			m_fSpeedV = m_fSpeedV + m_fAccelerate * fDeltaTime;
		dirV = -1;
	}
		
	A3DVECTOR3 vCurPos = m_pHost->GetPos();
	A3DVECTOR3 vMoveDir = m_vecDest - vCurPos;
	vMoveDir.y = 0.0f;
	vMoveDir.Normalize();
	if (!vMoveDir.IsZero())
	{
		m_pHost->SetDirAndUp(vMoveDir, g_vAxisY);
	}
	
	A3DVECTOR3 vVel = vMoveDir * m_fSpeedH + g_vAxisY * m_fSpeedV * dirV;

	vCurPos = m_pHost->m_MoveCtrl.AirWaterMove(vVel, fDeltaTime, true);
	m_pHost->SetPos(vCurPos);

	if (m_pHost->m_MoveCtrl.MoveBlocked() >= 3)
	{
		Finish();
		return true;
	}
	return true;		
}

void CECHPWorkParabolic::PrepareParabolic(DWORD dwDuration, float height, A3DVECTOR3 vecDest)
{
	m_vecPostion  = m_pHost->GetPos();
	float distance = a3d_MagnitudeH(vecDest - m_vecPostion);
	
	m_dwStartTime = GetTickCount();
	m_dwDuration  = dwDuration;
	m_fSpeedH	  = distance / (float) (dwDuration * 0.001f);
	m_fSpeedV	  = height / (float) (dwDuration * 0.0005f) * 2.0f;
	m_fAccelerate = m_fSpeedV / (float)(m_dwDuration * 0.0005f); 
	m_vecDest	  = vecDest;
	m_fHeight	  = height;
	m_fDistance	  = distance;
}

void CECHPWorkParabolic::Finish()
{
	m_bFinished = true;

	m_pHost->m_ExtProps.run_speed  = m_fRunSpeedBackup;
	m_pHost->m_MoveConst.fMinAirHei = m_fMinAirHeiBackup;
		
	m_pHost->SetPos(m_vecDest);
	m_pHost->m_MoveCtrl.Reset();
	m_pHost->m_MoveCtrl.SetLastSevPos(m_vecDest);
	m_pHost->m_MoveCtrl.SetHostLastPos(m_vecDest);
	m_pHost->m_MoveCtrl.ResetMoveTime();
}



///////////////////////////////////////////////////////////////////////////
//	
//	Implement CECHPWorkMoveNoCollision
//	
///////////////////////////////////////////////////////////////////////////

CECHPWorkMoveNoCollision::CECHPWorkMoveNoCollision(CECHPWorkMan* pWorkMan) :
CECHPWork(WORK_MOVENOCOLLISION, pWorkMan)
{
	m_dwMask		= MASK_MOVENOCOLLISION;
	m_dwTransMask	= MASK_STAND;
	CECHPWorkMoveNoCollision::Reset();
}

CECHPWorkMoveNoCollision::~CECHPWorkMoveNoCollision()
{
}

//	Reset work
void CECHPWorkMoveNoCollision::Reset()
{
	CECHPWork::Reset();
	
	m_dwStartTime    = 0;
	m_dwDuration     = 0;
	m_fSpeed         = 0.0f;
	m_vecDest        = A3DVECTOR3(0.0f);
	m_idMoveToTarget = 0;
}

//	Copy work data
bool CECHPWorkMoveNoCollision::CopyData(CECHPWork* pWork)
{
	if (!CECHPWork::CopyData(pWork))
		return false;

	CECHPWorkMoveNoCollision* pSrc = (CECHPWorkMoveNoCollision*)pWork;

	m_dwStartTime     = pSrc->m_dwStartTime;
	m_dwDuration      = pSrc->m_dwDuration;
	m_fSpeed	      = pSrc->m_fSpeed;
	m_vecDest         = pSrc->m_vecDest;
	m_idMoveToTarget  = pSrc->m_idMoveToTarget;

	return true;
}


void CECHPWorkMoveNoCollision::OnFirstTick()
{
	if(m_pHost->m_bQilinFlashMove)
		m_pHost->PlayAction(CECPlayer::ACT_QINGGONG_LOOP, 1.0f, true, 0, false);
}

void CECHPWorkMoveNoCollision::SetMoveInfo(DWORD dwStartTime, DWORD dwDuration, float fSpeed, A3DVECTOR3 vecDest, int idTarget)
{
	m_dwStartTime    = dwStartTime;
	m_dwDuration     = dwDuration;
	m_fSpeed		 = fSpeed;
	m_vecDest	     = vecDest;
	m_idMoveToTarget = idTarget;
}

bool CECHPWorkMoveNoCollision::Tick(DWORD dwDeltaTime)
{
	CECHPWork::Tick(dwDeltaTime);
	
	
	if(m_pHost->m_bQilinFlashMove)
	{
		CECPlayer * pPlayer = g_pGame->GetGameRun()->GetWorld()->GetPlayerMan()->GetPlayer(m_idMoveToTarget);
		if(pPlayer)
		{
			m_vecDest = pPlayer->GetPos() + g_vAxisY * 5.0f;
		}
	}

	float fDeltaTime = dwDeltaTime * 0.001;
	A3DVECTOR3 vCurPos = m_pHost->GetPos();
	A3DVECTOR3 vMoveDir = m_vecDest - vCurPos;
	A3DVECTOR3 vMoveDirH = A3DVECTOR3(vMoveDir.x, 0.0f, vMoveDir.z);
	vMoveDirH.y = 0.0f;
	
	float fDist = vMoveDir.Normalize();

	vCurPos = vCurPos + vMoveDir * m_fSpeed * fDeltaTime;
	A3DVECTOR3 vMoveDelta = vCurPos - m_pHost->GetPos();
	float fMovDelta = vMoveDelta.Normalize();
	if (fMovDelta >= fDist)
	{
		Finish();
		return true;
	}
	else
	{
		m_pHost->SetPos(vCurPos);
		if(!vMoveDir.IsZero())
			m_pHost->SetDirAndUp(vMoveDirH, g_vAxisY);
	}
}

void CECHPWorkMoveNoCollision::Finish()
{
	m_bFinished = true;

	if(m_pHost->m_bQilinFlashMove)
	{
		m_pHost->ReAttachQilin();
		m_pHost->m_bQilinFlashMove = false;
	}
}