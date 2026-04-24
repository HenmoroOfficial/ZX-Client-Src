/*
 * FILE: EC_HPWorkStand.cpp
 *
 * DESCRIPTION: 
 *
 * CREATED BY: Duyuxin, 2004/10/11
 *
 * HISTORY: 
 *
 * Copyright (c) 2004 Archosaur Studio, All Rights Reserved.
 */

#include "EC_Global.h"
#include "EC_HPWorkStand.h"
#include "EC_HostPlayer.h"
#include "EC_HPWorkMove.h"
#include "EC_HPWorkFall.h"
#include "EC_CDR.h"
#include "EC_Game.h"
#include "EC_GameSession.h"

#include "A3DGFXEx.h"
#include "A3DGFXExMan.h"

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
//	Implement CECHPWorkStand
//	
///////////////////////////////////////////////////////////////////////////

CECHPWorkStand::CECHPWorkStand(CECHPWorkMan* pWorkMan) :
CECHPWork(WORK_STAND, pWorkMan)
{
	m_dwMask		= MASK_STAND;
	m_dwTransMask	= MASK_MOVETOPOS | MASK_FLYOFF | MASK_FREEFALL | MASK_TRACEOBJECT |
					  MASK_SIT | MASK_SPELLOBJECT | MASK_FOLLOW | MASK_CONCENTRATE | MASK_USEITEM;

	CECHPWorkStand::Reset();
}

CECHPWorkStand::~CECHPWorkStand()
{
}

//	Reset work
void CECHPWorkStand::Reset()
{
	CECHPWork::Reset();

	m_bMeetSlide	= false;
	m_iPoseAction	= CECPlayer::ACT_STAND;
	m_bSession		= false;
	m_bWaterStop	= false;
	m_bMoving		= false;
	m_bStopSlide	= false;
	m_iCurAction	= CECPlayer::ACT_STAND;
}

//	Copy work data
bool CECHPWorkStand::CopyData(CECHPWork* pWork)
{
	if (!CECHPWork::CopyData(pWork))
		return false;

	CECHPWorkStand* pSrc = (CECHPWorkStand*)pWork;

	m_bMeetSlide	= pSrc->m_bMeetSlide;
	m_iPoseAction	= pSrc->m_iPoseAction;
	m_bSession		= pSrc->m_bSession;
	m_bWaterStop	= pSrc->m_bWaterStop;
	m_bMoving		= pSrc->m_bMoving;
	m_bStopSlide	= pSrc->m_bStopSlide;
	m_iCurAction	= pSrc->m_iCurAction;

	return true;
}

void CECHPWorkStand::SetPoseAction(int iAction, bool bSession)
{ 
	m_iPoseAction	= iAction;
	m_bSession		= bSession;

	m_pHost->PlayAction(m_iPoseAction, 1.0f, true);

	if (!bSession && iAction != CECPlayer::ACT_EXP_KISS)
		m_pHost->PlayAction(m_pHost->GetMoveStandAction(false, false), 1.0f, true, 300, true);
}

void CECHPWorkStand::SetActivePoseAction(int iAction)
{
	m_iPoseAction	= iAction;
	m_bSession		= true;

	m_pHost->PlayAction(m_iPoseAction, 1.0f, true);

	int iNextAction = m_pHost->GetMoveStandAction(false, false);
	switch(iAction)
	{
		case CECPlayer::ACT_PROPOSE:
			iNextAction = CECPlayer::ACT_PROPOSE_LOOP;
		break;

		default:
			iNextAction = CECPlayer::ACT_PROPOSE_LOOP;
	}

	m_pHost->PlayAction(iNextAction, 1.0f, true, 300, true);
}


//	On first tick
void CECHPWorkStand::OnFirstTick()
{
	m_pHost->m_iMoveMode = CECPlayer::MOVE_STAND;
	m_pHost->m_vAccel.Clear();

	if (m_pHost->m_iMoveEnv == CECPlayer::MOVEENV_AIR)
		m_pHost->m_vVelocity.x = m_pHost->m_vVelocity.z = 0.0f;
	else if (m_pHost->m_iMoveEnv == CECPlayer::MOVEENV_WATER)
		m_pHost->m_vVelocity.x = m_pHost->m_vVelocity.z = 0.0f;
	else
		m_pHost->m_vVelocity.Clear();

	//	Play appropriate actions
//	if (m_iPoseAction == CECPlayer::ACT_STAND)
//		m_pHost->PlayAction(m_pHost->GetMoveStandAction(false), 1.0f, false);

	if (m_pHost->m_pMoveTargetGFX)
		m_pHost->m_pMoveTargetGFX->Stop();
}

//	Should player start move ?
int CECHPWorkStand::ShouldMove()
{
	int iMoveReason = 0;

	if (m_pHost->IsJumping())
	{
		iMoveReason = 1;
	}
	else if (!m_pHost->m_GndInfo.bOnGround)
	{
		if (m_pHost->m_iMoveEnv != CECPlayer::MOVEENV_AIR &&
			m_pHost->m_iMoveEnv != CECPlayer::MOVEENV_WATER)		
			iMoveReason = 2;
	}

	return iMoveReason;
}

//	On work shift
void CECHPWorkStand::OnWorkShift()
{
	m_bWaterStop	= false;
	m_bMoving		= false;
	m_bMeetSlide	= false;
	m_bStopSlide	= false;
}

//	Work is cancel
void CECHPWorkStand::Cancel()
{
	m_iPoseAction = CECPlayer::ACT_STAND;
	m_bSession = false;

	/*
	if (m_pHost->m_AttachMode == CECPlayer::enumAttachActiveEmote)
	{
		m_pHost->StopActiveEmote();
		g_pGame->GetGameSession()->c2s_CmdStopActiveEmote();
	}
	*/
}

//	Doing session pose ?
bool CECHPWorkStand::DoingSessionPose()
{
	return (m_iPoseAction != CECPlayer::ACT_STAND && m_bSession);
}

//	Tick routine
bool CECHPWorkStand::Tick(DWORD dwDeltaTime)
{
	CECHPWork::Tick(dwDeltaTime);

	int iMoveReason = ShouldMove();
	if (iMoveReason)
	{
		if (iMoveReason == 1)
		{
			CECHPWorkMove* pWork = (CECHPWorkMove*)m_pWorkMan->CreateWork(WORK_MOVETOPOS);
			pWork->SetDestination(CECHPWorkMove::DEST_STANDJUMP, g_vOrigin);

			m_pWorkMan->StartWork_p1(pWork);
			//	Tick move work immediately, so that m_pHost->m_fVertSpeed can be
			//	used before it is cleared at the end of CECHostPlayer::Tick().
			m_pWorkMan->GetWorkByID(WORK_MOVETOPOS, false)->Tick(dwDeltaTime);
		}
		else if (iMoveReason == 2)
		{
			CECHPWorkFall* pWork = (CECHPWorkFall*)m_pWorkMan->CreateWork(WORK_FREEFALL);
			pWork->SetFallType(CECHPWorkFall::TYPE_FREEFALL);
			m_pWorkMan->StartWork_p1(pWork);
		}
		else
		{
			ASSERT(0);
		}

		return true;
	}

	//	Play appropriate actions
	if (m_iPoseAction == CECPlayer::ACT_STAND)
	{
		bool bFight = m_pHost->IsFighting();
		int iAction = m_pHost->GetMoveStandAction(false, bFight);
		if (iAction == CECPlayer::ACT_FIGHTSTAND)
		//	m_pHost->PlayAction(iAction, 1.0f, false, 300);
			m_iCurAction = iAction;
		else
		//	m_pHost->PlayAction(iAction, 1.0f, false);
			m_iCurAction = iAction;
	}

	float fDeltaTime = dwDeltaTime * 0.001f;

	if (!m_pHost->IsRooting())
	{
		if (m_pHost->m_iMoveEnv == CECPlayer::MOVEENV_GROUND)
			Tick_Walk(fDeltaTime);
		else	//	(m_pHost->m_iMoveEnv == CECPlayer::MOVEENV_AIR || m_pHost->m_iMoveEnv == CECPlayer::MOVEENV_WATER)
			Tick_FlySwim(fDeltaTime);
	}

	if (m_iPoseAction == CECPlayer::ACT_STAND)
		m_pHost->PlayAction(m_iCurAction, 1.0f, false, 300);
	
	//	Force to update object's direction and up
	m_pHost->m_vecGroundNormal = m_pHost->m_vecGroundNormalSet;
	m_pHost->SetDirAndUp(m_pHost->GetDir(), m_pHost->GetUp());

	return true;
}

//	Tick routine of walking on ground
bool CECHPWorkStand::Tick_Walk(float fDeltaTime)
{
	A3DVECTOR3 vCurPos = m_pHost->GetPos();
	CDR_INFO& cdr = *m_pHost->m_pCDRInfo;

	if (m_pHost->m_iMoveMode == CECPlayer::MOVE_SLIDE)
	{
		if (m_bStopSlide)
			return true;

	//	m_pHost->PlayAction(CECPlayer::ACT_JUMP_LOOP, 1.0f, false);
		m_iCurAction = CECPlayer::ACT_JUMP_LOOP;

		m_bMeetSlide = true;
		m_bMoving = true;	

		A3DVECTOR3 vPrevPos = m_pHost->GetPos(); //Added 2011-07-12.专用于焚香火焰囚牢技能，在检查前先保留上一次位置
		vCurPos = m_pHost->m_MoveCtrl.GroundMove(g_vOrigin, 0.0f, fDeltaTime); 
		m_pHost->SetPos(vCurPos);

		if (m_pHost->m_MoveCtrl.MoveBlocked() >= 3)
		{
			m_bStopSlide = true;
			m_bMoving = false;
			m_pHost->m_MoveCtrl.SendStopMoveCmd(vCurPos, m_pHost->GetGroundSpeed(), GP_MOVE_SLIDE);
		}
		//Added and Modified 2011-07-12.对于囚牢技能，由于并不对囚牢进行碰撞检测，因此需要额外执行范围检测。
		else if ( m_pHost->IsNeedCheckCurPosValid() && !m_pHost->CheckCurrentPosValid(vCurPos) )
		{
			m_bStopSlide = true;
			m_bMoving = false;
			m_pHost->m_MoveCtrl.SendStopMoveCmd(vPrevPos, m_pHost->GetGroundSpeed(), GP_MOVE_SLIDE);
			m_pHost->SetPos(vPrevPos);
		}
		else
		{
			m_pHost->m_MoveCtrl.SendMoveCmd(vCurPos, 2, g_vOrigin, cdr.vAbsVelocity, GP_MOVE_SLIDE);
		}
	}
	else if (m_bMeetSlide)
	{
		m_bMeetSlide = false;
		m_bMoving = false;
		m_pHost->m_MoveCtrl.SendStopMoveCmd(vCurPos, m_pHost->GetGroundSpeed(), GP_MOVE_SLIDE);
	}

	return true;
}

//	Tick routine of flying or swimming
bool CECHPWorkStand::Tick_FlySwim(float fDeltaTime)
{
	m_bMoving = false;

	if (m_pHost->m_iMoveEnv == CECPlayer::MOVEENV_WATER && !m_bWaterStop)
	{
		//	When player fell from a high place into water and change to stand
		//	state immediately, he may float at water surface not deep enough, 
		//	handle this case here.
		A3DVECTOR3 vCurPos = m_pHost->GetPos();
		ON_AIR_CDR_INFO& cdr = *m_pHost->m_pAirCDRInfo;
		float fSpeed = m_pHost->GetSwimSpeed();
		
		if (m_pHost->m_GndInfo.fWaterHei - vCurPos.y < cdr.vExtent.y + m_pHost->m_MoveConst.fWaterSurf)
		{
			m_bMoving = true;

			vCurPos = m_pHost->m_MoveCtrl.AirWaterMove(-g_vAxisY, fSpeed, fDeltaTime, false);
			m_pHost->SetPos(vCurPos);

			if (m_pHost->m_MoveCtrl.MoveBlocked() >= 3 ||
				m_pHost->m_GndInfo.fWaterHei - vCurPos.y >= cdr.vExtent.y + m_pHost->m_MoveConst.fWaterSurf)
			{
				m_bWaterStop = true;
				m_pHost->m_MoveCtrl.SendStopMoveCmd(vCurPos, fSpeed, GP_MOVE_WATER | GP_MOVE_RUN);
			}
			else
				m_pHost->m_MoveCtrl.SendMoveCmd(vCurPos, 2, g_vOrigin, -g_vAxisY * fSpeed, GP_MOVE_WATER | GP_MOVE_RUN);
		}
	}

	return true;
}