/*
 * FILE: EC_HPWorkFollow.cpp
 *
 * DESCRIPTION: 
 *
 * CREATED BY: Duyuxin, 2004/11/4
 *
 * HISTORY: 
 *
 * Copyright (c) 2004 Archosaur Studio, All Rights Reserved.
 */

#include "EC_Global.h"
#include "EC_HPWorkFollow.h"
#include "EC_HostPlayer.h"
#include "EC_ManPlayer.h"
#include "EC_ElsePlayer.h"
#include "EC_Model.h"
#include "EC_CDR.h"
#include "EC_GameRecord.h"

#include "AC.h"
#include "A3DFuncs.h"

#define new A_DEBUG_NEW

///////////////////////////////////////////////////////////////////////////
//	
//	Define and Macro
//	
///////////////////////////////////////////////////////////////////////////

#define STOP_RADIUS		5.0f

///////////////////////////////////////////////////////////////////////////
//	
//	Reference to External variables and functions
//	
///////////////////////////////////////////////////////////////////////////

extern game_record gr;

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
//	Implement CECHPWorkFollow
//	
///////////////////////////////////////////////////////////////////////////

CECHPWorkFollow::CECHPWorkFollow(CECHPWorkMan* pWorkMan) :
CECHPWork(WORK_FOLLOW, pWorkMan)
{
	m_dwMask		= MASK_FOLLOW;
	m_dwTransMask	= MASK_STAND | MASK_MOVETOPOS | MASK_TRACEOBJECT | MASK_FOLLOW;

	CECHPWorkFollow::Reset();
}

CECHPWorkFollow::~CECHPWorkFollow()
{
}

//	Change trace target
void CECHPWorkFollow::ChangeTarget(int idTarget)
{
	ASSERT(ISPLAYERID(idTarget) && idTarget != m_pHost->GetCharacterID());
	m_idTarget = idTarget;
}

//	Reset work
void CECHPWorkFollow::Reset()
{
	CECHPWork::Reset();

	m_idTarget		= 0;
	m_bStopMove		= false;
	m_bMeetSlide	= false;
}

//	Copy work data
bool CECHPWorkFollow::CopyData(CECHPWork* pWork)
{
	if (!CECHPWork::CopyData(pWork))
		return false;

	CECHPWorkFollow* pSrc = (CECHPWorkFollow*)pWork;

	m_idTarget		= pSrc->m_idTarget;
	m_bStopMove		= pSrc->m_bStopMove;
	m_bMeetSlide	= pSrc->m_bMeetSlide;
	m_vDestPos		= pSrc->m_vDestPos;
	m_vOffset		= pSrc->m_vOffset;
	m_vCurDirH		= pSrc->m_vCurDirH;

	return true;
}

bool CECHPWorkFollow::gen_record_msg(ECMSG& msg)
{
	msg.dwMsg = MSG_HST_FOLLOW_RECORD;
	msg.iManager = MAN_PLAYER;
	msg.iSubID = 0;
	msg.dwParam1 = m_idTarget;
	msg.dwParam2 = 0;
	msg.dwParam3 = 0;
	msg.dwParam4 = 0;
	return true;
}

//	On first tick
void CECHPWorkFollow::OnFirstTick()
{
	if (!ISPLAYERID(m_idTarget) || m_idTarget == m_pHost->GetCharacterID())
		return;
	
	CECElsePlayer* pPlayer = m_pHost->m_pPlayerMan->GetElsePlayer(m_idTarget);
	if (!pPlayer)
		return;
	
	//	Calculate distance between target and host
	m_vCurDirH = m_pHost->GetPos() - pPlayer->GetPos();
	m_vCurDirH.y = 0.0f;
	float fCurDist = m_vCurDirH.Normalize();

	m_vOffset = m_vCurDirH * a_Random(2.0f, STOP_RADIUS);

	if (fCurDist < STOP_RADIUS)
	{
		m_bStopMove = true;
		m_pHost->m_iMoveMode = CECHostPlayer::MOVE_STAND;
		
		//	Play appropriate actions
		m_pHost->PlayAction(m_pHost->GetMoveStandAction(false), 1.0f, false);
	
		//	If this work was transfered from a work which 
		//	needs moving (such as 'move to' work) and we reach target position
		//	immediately, we must need to send 'stop move' command
		if (!m_pHost->m_MoveCtrl.IsStop())
			m_pHost->m_MoveCtrl.SendStopMoveCmd();
	}
	else
	{
		m_bStopMove = false;
		m_pHost->m_iMoveMode = CECHostPlayer::MOVE_MOVE;
	}
}

//	Stop move
void CECHPWorkFollow::StopMove(const A3DVECTOR3& vPos, float fSpeed, int iMoveMode)
{
	m_bStopMove = true;
	m_pHost->m_iMoveMode = CECHostPlayer::MOVE_STAND;

	//	Play appropriate actions
	m_pHost->PlayAction(m_pHost->GetMoveStandAction(false), 1.0f, false);

	m_pHost->m_MoveCtrl.SendStopMoveCmd(vPos, fSpeed, iMoveMode);
}

//	Tick routine
bool CECHPWorkFollow::Tick(DWORD dwDeltaTime)
{
	CECHPWork::Tick(dwDeltaTime);

	ASSERT(ISPLAYERID(m_idTarget) && m_idTarget != m_pHost->GetCharacterID());

	CECElsePlayer* pPlayer = m_pHost->m_pPlayerMan->GetElsePlayer(m_idTarget);
	if (!pPlayer)
	{
		//	Taget has missed ?
		m_bFinished = true;
		return true;
	}

	float fDeltaTime = dwDeltaTime * 0.001f;
	A3DVECTOR3 vDist = m_pHost->GetPos() - pPlayer->GetPos();
	vDist.y = 0.0f;
	float fDist = vDist.Normalize();

	if (m_bStopMove && !m_pHost->IsJumping())
	{
		if (fDist < STOP_RADIUS)
		{
			m_vCurDirH.Clear();
			return true;
		}

		//	Start moving again
		m_bStopMove	= false;
		m_vOffset	= vDist * a_Random(2.0f, STOP_RADIUS);

		m_pHost->m_iMoveMode = CECHostPlayer::MOVE_MOVE;
	}
	else
	{
		m_bStopMove	= false;
		m_vDestPos = pPlayer->GetPos() + m_vOffset;

		if (!m_pHost->IsRooting())
		{	
			//	Continue following target
			if (m_pHost->m_iMoveEnv == CECPlayer::MOVEENV_GROUND)
			{
				//	Play appropriate actions
				int iCurAction = m_pHost->GetPlayerModel() ? (int)m_pHost->GetPlayerModel()->GetCurActionUserData(ACT_CHANNEL_BODY) : 0;
				if (!m_pHost->IsJumping() && iCurAction != CECPlayer::ACT_TRICK_RUN &&
					m_pHost->m_iMoveMode != CECPlayer::MOVE_SLIDE)
				{
					int iAction = m_pHost->GetMoveStandAction(true);
					m_pHost->PlayAction(iAction, 1.0f, false);
				}

				Follow_Walk(fDeltaTime);
			}
			else	//	(m_pHost->m_iMoveEnv == CECPlayer::MOVEENV_AIR || m_pHost->m_iMoveEnv == CECPlayer::MOVEENV_WATER)
			{
				m_pHost->ResetJump();

				//	Play appropriate actions
				int iAction = m_pHost->GetMoveStandAction(true);
				m_pHost->PlayAction(iAction, 1.0f, false);

				Follow_FlySwim(fDeltaTime);
			}
		}
	}

	return true;
}

//	Follow on ground
bool CECHPWorkFollow::Follow_Walk(float fDeltaTime)
{
	A3DVECTOR3 vCurPos = m_pHost->GetPos();
	CDR_INFO& cdr = *m_pHost->m_pCDRInfo;

	if (m_pHost->m_iMoveMode == CECPlayer::MOVE_SLIDE)
	{
		m_pHost->PlayAction(CECPlayer::ACT_JUMP_LOOP, 1.0f, false);

		A3DVECTOR3 vDir = m_vDestPos - vCurPos;
		vDir.y = 0;
		vDir.Normalize();
		
		float fMaxSpeedV;
		m_bMeetSlide = m_pHost->m_MoveCtrl.MeetSlope(vDir, fMaxSpeedV);
#ifdef CDR_HL2_MOVE
		a_ClampFloor(cdr.vClipVel.y, -fMaxSpeedV);	
#else
		a_ClampFloor(cdr.fYVel, -fMaxSpeedV);
#endif
		m_vCurDirH = vDir;
		vCurPos = m_pHost->m_MoveCtrl.GroundMove(m_vCurDirH, m_pHost->GetGroundSpeed(), fDeltaTime);

		// 由于回放时会被误判为Blocked状态，所以忽略此判断
		if (m_pHost->m_MoveCtrl.MoveBlocked() >= 3 && gr.get_state() != game_record::enum_gr_replay)
		{
			m_pHost->m_MoveCtrl.SetSlideLock(true);
			m_pHost->m_MoveCtrl.SendStopMoveCmd(vCurPos, m_pHost->GetGroundSpeed(), GP_MOVE_SLIDE);
			m_bFinished = true;
		}
		else
		{
			m_pHost->SetPos(vCurPos);
			m_pHost->m_MoveCtrl.SendMoveCmd(vCurPos, 2, g_vOrigin, cdr.vAbsVelocity, GP_MOVE_SLIDE);
		}
	}
	else if (!m_bMeetSlide)
	{
		int iMoveMode = m_pHost->m_bWalkRun ? GP_MOVE_RUN : GP_MOVE_WALK;
		if (m_pHost->IsJumping())
			iMoveMode = GP_MOVE_JUMP;
		else if (!m_pHost->m_GndInfo.bOnGround)
			iMoveMode = GP_MOVE_FALL;

		float fDist, fSpeed = m_pHost->GetGroundSpeed();
		if (m_pHost->m_GndInfo.bOnGround)
		{
			//	Ajust direction only when player on ground
			m_vCurDirH = m_vDestPos - vCurPos;
			m_vCurDirH.y = 0.0f;
			fDist = m_vCurDirH.Normalize();
		}
		else
			fDist = a3d_MagnitudeH(m_vDestPos-vCurPos);

		//	Ground move
		A3DVECTOR3 vPrevPos = m_pHost->GetPos(); // Added 2011-07-12. 仅用于焚香火焰囚牢技能，在判断范围前先记录之前位置
		vCurPos = m_pHost->m_MoveCtrl.GroundMove(m_vCurDirH, fSpeed, fDeltaTime, m_pHost->m_fVertSpeed);
		if (!m_vCurDirH.IsZero())
			m_pHost->SetDestDirAndUp(m_vCurDirH, g_vAxisY, 100);

		// 由于回放时会被误判为Blocked状态，所以忽略此判断
		if (m_pHost->m_MoveCtrl.MoveBlocked() >= 3 && gr.get_state() != game_record::enum_gr_replay)
		{
			m_bFinished = true;
			m_pHost->m_MoveCtrl.SendStopMoveCmd(vCurPos, fSpeed, iMoveMode);
		}
	//	else if (m_pHost->m_GndInfo.bOnGround)
	//	else if (!cdr.vTPNormal.IsZero())
		else if (cdr.bOnSurface)
		{
			//	Check whether we have arrived destination
			A3DVECTOR3 vMoveDelta = vCurPos - m_pHost->GetPos();
			vMoveDelta.y = 0.0f;
			float fMoveDist = vMoveDelta.Normalize();

			if (fMoveDist >= fDist)
			{
				StopMove(vCurPos, fSpeed, iMoveMode);
			}
			//Added and Modified 2011-07-12.对于囚牢技能，由于并不对囚牢进行碰撞检测，因此需要额外执行范围检测。
			else if ( m_pHost->IsNeedCheckCurPosValid() && !m_pHost->CheckCurrentPosValid(vCurPos) )
			{
				m_bFinished = true;
				m_pHost->m_MoveCtrl.SendStopMoveCmd(vPrevPos, fSpeed, iMoveMode);
			}
			else
			{
				m_pHost->SetPos(vCurPos);
				m_pHost->m_MoveCtrl.SendMoveCmd(vCurPos, 0, m_vDestPos, cdr.vAbsVelocity, iMoveMode);
			}
		}
		else
		{
			m_pHost->SetPos(vCurPos);
			m_pHost->m_MoveCtrl.SendMoveCmd(vCurPos, 1, m_vDestPos, cdr.vAbsVelocity, iMoveMode);
		}
	}
	else
	{
		m_bFinished = true;
		m_pHost->m_MoveCtrl.SendStopMoveCmd(vCurPos, m_pHost->GetGroundSpeed(), GP_MOVE_SLIDE);
	}

	return true;
}

//	Follow in air and water
bool CECHPWorkFollow::Follow_FlySwim(float fDeltaTime)
{
	A3DVECTOR3 vCurPos = m_pHost->GetPos();
	int iMoveMode = (m_pHost->m_iMoveEnv == CECPlayer::MOVEENV_AIR) ? GP_MOVE_AIR : GP_MOVE_WATER;

	float na=0.0f, pa=0.0f, fMaxSpeed;
	bool bInAir;
	if (m_pHost->m_iMoveEnv == CECPlayer::MOVEENV_AIR)
	{
		bInAir = true;
		na = EC_NACCE_AIR;
		fMaxSpeed = m_pHost->GetFlySpeed();
	}
	else
	{
		bInAir = false;
		na = EC_NACCE_WATER;
		fMaxSpeed = m_pHost->GetSwimSpeed();
	}

	if (m_bMeetSlide)
	{
		m_pHost->m_MoveCtrl.SendStopMoveCmd(vCurPos, fMaxSpeed, iMoveMode | GP_MOVE_RUN);
		m_bFinished = true;
		return true;
	}

	A3DVECTOR3 vPushDir(0.0f);
	m_pHost->GetPushDir(vPushDir, CECHostPlayer::MD_ALL);
	vPushDir.x = vPushDir.z = 0.0f;

	float fSpeed1H = m_pHost->m_vVelocity.MagnitudeH();
	float fSpeed1V = m_pHost->m_vVelocity.y;
	ON_AIR_CDR_INFO& cdr = *m_pHost->m_pAirCDRInfo;

	m_vCurDirH = m_vDestPos - vCurPos;
	m_vCurDirH.y = 0.0f;
	float fDistH = m_vCurDirH.Normalize();

	//	Calculate the distance to reduce velocity to 0
	float s = -0.5f * fSpeed1H * fSpeed1H / na;
	if (fDistH > s - 0.01f)
		pa = EC_PUSH_ACCE;

	float fSpeed2H = fSpeed1H + (pa + na) * fDeltaTime;
	if (!pa && fSpeed2H < 0.0f)
		fSpeed2H = 0.0f;	//	Only resistance couldn't generate negative speed
	else if (fSpeed2H > fMaxSpeed)
		fSpeed2H = fMaxSpeed;

	//	Vertical speed
	float fSpeed2V = CalcFlySwimVertSpeed(fSpeed1V, vPushDir.y, EC_PUSH_ACCE, fDeltaTime);
	A3DVECTOR3 vVel2 = m_vCurDirH * fSpeed2H + g_vAxisY * fSpeed2V;

	if (!m_vCurDirH.IsZero())
		m_pHost->SetDestDirAndUp(m_vCurDirH, g_vAxisY, 100);

	//	Air/water move
	vCurPos = m_pHost->m_MoveCtrl.AirWaterMove(vVel2, fDeltaTime, bInAir);

	//	Reached destination ?
	A3DVECTOR3 vMoveDelta = vCurPos - m_pHost->GetPos();
	vMoveDelta.y = 0.0f;
	float fMoveDistH = vMoveDelta.Normalize();
	if (fMoveDistH >= fDistH)
		vVel2.Clear();

	m_pHost->SetPos(vCurPos);
	m_pHost->m_vVelocity = vVel2;

	if (fMoveDistH >= fDistH)
		StopMove(vCurPos, fMaxSpeed, iMoveMode | GP_MOVE_RUN);
	else
		m_pHost->m_MoveCtrl.SendMoveCmd(vCurPos, 0, m_vDestPos, vVel2, iMoveMode | GP_MOVE_RUN);

	return true;
}

//	Calculate vertical speed when fly or swim
float CECHPWorkFollow::CalcFlySwimVertSpeed(float fSpeed1, float fPushDir, float fPushAccel,
										  float fDeltaTime)
{
	float pa = fPushDir ? fPushAccel : 0.0f;

	float na=0.0f, fMaxSpeed;
	if (m_pHost->m_iMoveEnv == CECPlayer::MOVEENV_AIR)
	{
		na = EC_NACCE_AIR;
		fMaxSpeed = m_pHost->GetFlySpeed();
	}
	else
	{
		na = EC_NACCE_WATER;
		fMaxSpeed = m_pHost->GetSwimSpeed();
	}

	//	Vertical accelerate
	float fAccel = 0.0f;
	if (fPushDir > 0.0f)
		fAccel = pa + na;
	else if (fPushDir < 0.0f)
		fAccel = -(pa + na);
	else if (fSpeed1 > 0.0f)
		fAccel = na;
	else if (fSpeed1 < 0.0f)
		fAccel = -na;

	//	Vertical speed
	float fSpeed2 = fSpeed1 + fAccel * fDeltaTime;
	if (!pa && fSpeed2 * fSpeed1 < 0.0f)
		fSpeed2 = 0.0f;
	
	a_Clamp(fSpeed2, -fMaxSpeed, fMaxSpeed);

	return fSpeed2;
}
