/*
 * FILE: EC_HPWorkMove.cpp
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
#include "EC_HPWorkMove.h"
#include "EC_HostPlayer.h"
#include "EC_World.h"
#include "EC_Game.h"
#include "EC_GameRun.h"
#include "EC_GameSession.h"
#include "EC_Model.h"
#include "EC_CDR.h"
#include "A3DGFXEx.h"
#include "A3DFuncs.h"
#include "A3DTerrain2.h"
#include "EC_GameRecord.h"
#include "EC_ManPlayer.h"
#include "EC_ElsePlayer.h"

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

extern game_record gr;

///////////////////////////////////////////////////////////////////////////
//	
//	Local Types and Variables and Global variables
//	
///////////////////////////////////////////////////////////////////////////

static const float pitch_ang_wing = DEG2RAD(15.0f);
static const float pitch_ang_fly_sword = DEG2RAD(25.0f);
static const float lean_max_ang = DEG2RAD(45.0f);
static const float ang_vel_fly = 1.0f / DEG2RAD(60.0f);
static const float ang_vel_swim = 1.0f / DEG2RAD(180.0f);
static const float pitch_co_wing = pitch_ang_wing / A3D_PI;
static const float pitch_co_fly_sword = pitch_ang_fly_sword / A3D_PI;
static const float sidle_co = .5f;
static const float sidle_co_r = 1.0f - sidle_co;
static const float push_pitch_vel_wing = pitch_ang_wing / 0.5f;
static const float push_pitch_vel_fly_sword = pitch_ang_fly_sword / 0.5f;

#define FORCE_FACTOR		100.0f		//注意：碰撞角色的质量最大设定为100
#define BACKWARD_FACTOR		1.6f		//反向时，增加一个减速因子

///////////////////////////////////////////////////////////////////////////
//	
//	Local functions
//	
///////////////////////////////////////////////////////////////////////////


///////////////////////////////////////////////////////////////////////////
//	
//	Implement CECHPWorkMove
//	
///////////////////////////////////////////////////////////////////////////

CECHPWorkMove::CECHPWorkMove(CECHPWorkMan* pWorkMan) :
CECHPWork(WORK_MOVETOPOS, pWorkMan)
{
	m_dwMask		= MASK_MOVETOPOS;
	m_dwTransMask	= MASK_STAND | MASK_TRACEOBJECT | MASK_FOLLOW;

	CECHPWorkMove::Reset();
}

CECHPWorkMove::~CECHPWorkMove()
{
}

//	Reset work
void CECHPWorkMove::Reset()
{
	CECHPWork::Reset();

	m_iDestType		= DEST_2D;
	m_bHaveDest		= false;
	m_bMeetSlide	= false;
	m_bReadyCancel	= false;
	m_bGliding		= false;
	m_fGlideTime	= 0;
	m_fCurPitch		= 0;
	m_fPushPitch	= 0;
	m_fPushLean		= 0;
}

//	Copy work data
bool CECHPWorkMove::CopyData(CECHPWork* pWork)
{
	if (!CECHPWork::CopyData(pWork))
		return false;

	CECHPWorkMove* pSrc = (CECHPWorkMove*)pWork;

	m_iDestType		= pSrc->m_iDestType;
	m_bHaveDest		= pSrc->m_bHaveDest;
	m_bMeetSlide	= pSrc->m_bMeetSlide;
	m_bReadyCancel	= pSrc->m_bReadyCancel;
	m_bGliding		= pSrc->m_bGliding;
	m_fGlideTime	= pSrc->m_fGlideTime;
	m_fGlideSpan	= pSrc->m_fGlideSpan;
	m_fGlideAng		= pSrc->m_fGlideAng;
	m_fGlideVel		= pSrc->m_fGlideVel;	
	m_fGlidePitch	= pSrc->m_fGlidePitch;
	m_fCurPitch		= pSrc->m_fCurPitch;
	m_fPushPitch	= pSrc->m_fPushPitch;
	m_fPushLean		= pSrc->m_fPushLean;
	m_iDestType		= pSrc->m_iDestType;
	m_vMoveDest		= pSrc->m_vMoveDest;
	m_vCurDir		= pSrc->m_vCurDir;
		
	return true;
}

//	On first tick
void CECHPWorkMove::OnFirstTick()
{
	if (m_pHost->m_iMoveMode == CECPlayer::MOVE_STAND)
		m_pHost->m_iMoveMode = CECPlayer::MOVE_MOVE;

	//PlayMoveTargetGFX();

	if (m_pHost->m_iMoveEnv != CECPlayer::MOVEENV_AIR)
		m_pHost->ShowWing(false);

/*	if (!m_pHost->IsJumping())
	{
		int iAction = m_pHost->GetMoveStandAction(true);
		m_pHost->PlayAction(iAction, 1.0f, false);
	}	*/
}

//	Set destination position or direction
void CECHPWorkMove::SetDestination(int iDestType, const A3DVECTOR3& vMoveDest)
{
	m_iDestType	= iDestType;
	m_vMoveDest	= vMoveDest;
	m_bHaveDest	= true;
	m_bGliding	= false;
	m_pHost->SetAdjustOrient(false);

	if (iDestType == DEST_DIR)
		m_vCurDir = vMoveDest;
	else if (iDestType == DEST_2D || iDestType == DEST_3D)
	{
		m_vCurDir = vMoveDest - m_pHost->GetPos();
		m_vCurDir.y = 0.0f;
		m_vCurDir.Normalize();
	}

	if (m_pHost->m_pMoveTargetGFX)
	{
		if (iDestType == DEST_DIR)
			m_pHost->m_pMoveTargetGFX->Stop();
		//else
			//PlayMoveTargetGFX();
	}
}

//	Play walk target effect
void CECHPWorkMove::PlayMoveTargetGFX(const A3DVECTOR3& vPos, const A3DVECTOR3& vNormal)
{
	/*
	if (!m_bHaveDest || m_iDestType != DEST_2D || !m_pHost->m_pMoveTargetGFX)
		return;
	
	A3DVECTOR3 vTestPos = m_vMoveDest;
	vTestPos.y += 0.2f;

	A3DVECTOR3 vNormal, vHitPos;
	if (!VertRayTrace(vTestPos, vHitPos, vNormal, 5.0f))
		vNormal = g_vAxisY;
	*/

	A3DVECTOR3 vRight;
	vRight.CrossProduct(vNormal, g_vAxisX);
	vRight.Normalize();

	A3DMATRIX4 mat = a3d_TransformMatrix(vRight, vNormal, vPos + vNormal * 0.2f);

	if (m_pHost->m_pMoveTargetGFX)
	{
		float dh = vPos.y - g_pGame->GetGameRun()->GetWorld()->GetTerrain()->GetPosHeight(vPos);
		if( dh < 0.05f )
			m_pHost->m_pMoveTargetGFX->SetMatchGround(true);
		else
			m_pHost->m_pMoveTargetGFX->SetMatchGround(false);

		m_pHost->m_pMoveTargetGFX->SetParentTM(mat);
		m_pHost->m_pMoveTargetGFX->Start(true);
	}
}

//	Tick routine
bool CECHPWorkMove::Tick(DWORD dwDeltaTime)
{
	CECHPWork::Tick(dwDeltaTime);

	if (m_pHost->IsRooting())
		return true;

	float fDeltaTime = dwDeltaTime * 0.001f;
#ifdef CDR_HL2_MOVE
	bool bJumpInWater = m_pHost->m_iMoveEnv == CECPlayer::MOVEENV_WATER && m_pHost->IsJumping() && (m_pHost->m_pCDRInfo->vAbsVelocity.y > 0 || m_pHost->m_pCDRInfo->vClipVel.y > 0);
#else
	bool bJumpInWater = m_pHost->m_iMoveEnv == CECPlayer::MOVEENV_WATER && m_pHost->IsJumping() && (m_pHost->m_pCDRInfo->vAbsVelocity.y > 0 || m_pHost->m_pCDRInfo->fYVel > 0);
#endif
	if (m_pHost->m_iMoveEnv == CECPlayer::MOVEENV_GROUND || bJumpInWater)
	{
		//	Play appropriate actions
		int iCurAction = m_pHost->GetPlayerModel() ? (int)m_pHost->GetPlayerModel()->GetCurActionUserData(ACT_CHANNEL_BODY) : 0;
		if (!m_pHost->IsJumping() && iCurAction != CECPlayer::ACT_TRICK_RUN &&
			m_pHost->m_iMoveMode != CECPlayer::MOVE_SLIDE &&
			m_pHost->m_iMoveMode != CECPlayer::MOVE_FREEFALL && !m_bMeetSlide)
		{
			int iAction = m_pHost->GetMoveStandAction(true);
			m_pHost->PlayAction(iAction, 1.0f, false);
		}

		Tick_Walk(fDeltaTime);
	}
	else	//	(m_pHost->m_iMoveEnv == CECPlayer::MOVEENV_AIR || m_pHost->m_iMoveEnv == CECPlayer::MOVEENV_WATER)
	{
		m_pHost->ResetJump();

		//	Play appropriate actions
		if (!m_bGliding)
		{
			int iAction = m_pHost->GetMoveStandAction(true);
			m_pHost->PlayAction(iAction, 1.0f, false);
		}

		Tick_FlySwim(fDeltaTime);
	}

	return true;
}

//	Tick routine of walking on ground
bool CECHPWorkMove::Tick_Walk(float fDeltaTime)
{
	A3DVECTOR3 vCurPos = m_pHost->GetPos();
	CDR_INFO& cdr = *m_pHost->m_pCDRInfo;

	if (m_pHost->m_iMoveMode == CECPlayer::MOVE_SLIDE)
	{
		m_pHost->PlayAction(CECPlayer::ACT_JUMP_LOOP, 1.0f, false);

		float fSpeedH = m_pHost->GetGroundSpeed();

		//	This will cause stop moming after we slide down.
		A3DVECTOR3 vDir;
		if (m_iDestType == DEST_DIR)
			vDir = m_vCurDir;
		else if (m_iDestType == DEST_PUSH)
		{
			A3DVECTOR3 vMoveDir;
			if (m_pHost->GetPushDir(vMoveDir, CECHostPlayer::MD_FORWARD | CECHostPlayer::MD_BACK | CECHostPlayer::MD_LEFT | CECHostPlayer::MD_RIGHT, fDeltaTime)
				&& (!vMoveDir.IsZero()))
			{
				m_pHost->SetDirAndUp(vMoveDir, g_vAxisY);
				vDir = vMoveDir;
			}
			else
			{
				vDir = m_pHost->GetDir();
			}

			if (!(m_pHost->m_dwMoveRelDir & (CECHostPlayer::MD_LEFT | CECHostPlayer::MD_RIGHT | CECHostPlayer::MD_FORWARD | CECHostPlayer::MD_BACK)))
				fSpeedH = 0.0f;
		}
		else if (m_iDestType == DEST_STANDJUMP)
		{
			vDir = m_pHost->GetDir();
			fSpeedH = 0.0f;
		}
		else
		{
			vDir = m_vMoveDest - vCurPos;
			vDir.y = 0;
			vDir.Normalize();
		}

		float fMaxSpeedV;
		m_bMeetSlide = m_pHost->m_MoveCtrl.MeetSlope(vDir, fMaxSpeedV);
#ifdef CDR_HL2_MOVE
		a_ClampFloor(cdr.vClipVel.y, -fMaxSpeedV);
#else
		a_ClampFloor(cdr.fYVel, -fMaxSpeedV);
#endif	

		if (m_pHost->m_GndInfo.bOnGround)
			m_vCurDir = vDir;

		//	Ground move
	//	vCurPos = m_pHost->m_MoveCtrl.GroundMove(m_vCurDir, m_pHost->GetGroundSpeed(), fDeltaTime);
		vCurPos = m_pHost->m_MoveCtrl.GroundMove(m_vCurDir, fSpeedH, fDeltaTime);
		// 由于回放时会被误判为Blocked状态，所以忽略此判断
		if (m_pHost->m_MoveCtrl.MoveBlocked() >= 3 && gr.get_state() != game_record::enum_gr_replay)
		{
			m_pHost->m_MoveCtrl.SetSlideLock(true);

			//	blocked, so we clear the fYVel of cdr!
			//cdr.fYVel = 0.0f;

			Finish();
			m_pHost->m_MoveCtrl.SendStopMoveCmd(vCurPos, m_pHost->GetGroundSpeed(), GP_MOVE_SLIDE);
		}		
		else
		{
			m_pHost->SetPos(vCurPos);
			m_pHost->m_MoveCtrl.SendMoveCmd(vCurPos, 2, g_vOrigin, cdr.vAbsVelocity, GP_MOVE_SLIDE);
		}
	}
	else if (/*m_pHost->m_iMoveMode == CECPlayer::MOVE_FREEFALL || */!m_bMeetSlide)
	{
		float fSpeed = m_pHost->GetGroundSpeed();

		int iMoveMode = m_pHost->m_bWalkRun ? GP_MOVE_RUN : GP_MOVE_WALK;
		if (m_pHost->IsJumping())
			iMoveMode = GP_MOVE_JUMP;
		else if (!m_pHost->m_GndInfo.bOnGround)
			iMoveMode = GP_MOVE_FALL;

		if (m_bReadyCancel && m_pHost->m_GndInfo.bOnGround)
		{
			Finish();
			m_pHost->m_MoveCtrl.SendStopMoveCmd(vCurPos, fSpeed, iMoveMode);
			return true;
		}

		if (m_pHost->m_dwMoveRelDir & (CECHostPlayer::MD_LEFT | CECHostPlayer::MD_RIGHT | CECHostPlayer::MD_FORWARD | CECHostPlayer::MD_BACK))
			m_iDestType = DEST_PUSH;

		if (m_iDestType == DEST_2D)
		{
			float fDist;
			if (m_pHost->m_GndInfo.bOnGround)
			{
				//	Ajust direction only when player on ground
				m_vCurDir = m_vMoveDest - vCurPos;
				m_vCurDir.y = 0.0f;
				fDist = m_vCurDir.Normalize();
			}
			else
				fDist = a3d_MagnitudeH(m_vMoveDest-vCurPos);

			//	Ground move
			vCurPos = m_pHost->m_MoveCtrl.GroundMove(m_vCurDir, fSpeed, fDeltaTime, m_pHost->m_fVertSpeed);
			if (!m_vCurDir.IsZero())
				m_pHost->SetDirAndUp(m_vCurDir, g_vAxisY);

			// 由于回放时会被误判为Blocked状态，所以忽略此判断
			if (m_pHost->m_MoveCtrl.MoveBlocked() >= 3 && gr.get_state() != game_record::enum_gr_replay)
			{
				// blocked, so we clear the fYVel of cdr!
				//cdr.fYVel = 0.0f;

				Finish();
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
#ifdef CDR_HL2_MOVE
				   cdr.vClipVel.x = cdr.vClipVel.z = 0.0f;
#endif
				

					Finish();
					m_pHost->SetPos(vCurPos);
					m_pHost->m_MoveCtrl.SendStopMoveCmd(vCurPos, fSpeed, iMoveMode);
				}
				else
				{
					//Added and Modified 2011-07-11.对于囚牢技能，由于并不对囚牢进行碰撞检测，因此需要额外执行范围检测。
					if ( m_pHost->IsNeedCheckCurPosValid() && !m_pHost->CheckCurrentPosValid(vCurPos) )
					{
#ifdef CDR_HL2_MOVE
						cdr.vClipVel.x = cdr.vClipVel.z = 0.0f;
#endif
						Finish();
						m_pHost->m_vVelocity.Clear();
						m_pHost->m_MoveCtrl.SendStopMoveCmd(m_pHost->GetPos(), fSpeed, iMoveMode);
					}
					else
					{
						m_pHost->SetPos(vCurPos);
						m_pHost->m_MoveCtrl.SendMoveCmd(vCurPos, 0, m_vMoveDest, cdr.vAbsVelocity, iMoveMode);
					}
				}
			}
			else
			{
				m_pHost->SetPos(vCurPos);
				m_pHost->m_MoveCtrl.SendMoveCmd(vCurPos, 1, m_vMoveDest, cdr.vAbsVelocity, iMoveMode);
			}
		}
		else if (m_iDestType == DEST_DIR)
		{
			vCurPos = m_pHost->m_MoveCtrl.GroundMove(m_vCurDir, fSpeed, fDeltaTime, m_pHost->m_fVertSpeed);
			m_pHost->SetPos(vCurPos);
			if (!m_vCurDir.IsZero())
				m_pHost->SetDirAndUp(m_vCurDir, g_vAxisY);

			// 由于回放时会被误判为Blocked状态，所以忽略此判断
			if (m_pHost->m_MoveCtrl.MoveBlocked() >= 3 && gr.get_state() != game_record::enum_gr_replay)
			{
				// blocked, so we clear the fYVel of cdr!
				//cdr.fYVel = 0.0f;

				Finish();
				m_pHost->m_MoveCtrl.SendStopMoveCmd(vCurPos, fSpeed, iMoveMode);
			}
			else if (m_pHost->m_GndInfo.bOnGround)
			//	m_pHost->m_MoveCtrl.SendMoveCmd(vCurPos, 2, g_vOrigin, m_vCurDir * fSpeed, GP_MOVE_RUN);
				m_pHost->m_MoveCtrl.SendMoveCmd(vCurPos, 2, g_vOrigin, cdr.vAbsVelocity, iMoveMode);
			else
				m_pHost->m_MoveCtrl.SendMoveCmd(vCurPos, 1, m_vMoveDest, cdr.vAbsVelocity, iMoveMode);
		}
		else if (m_iDestType == DEST_STANDJUMP)
		{
			if (!m_pHost->IsJumping())
			{
				Finish();
#ifdef CDR_HL2_MOVE
				cdr.vClipVel.x = cdr.vClipVel.z = 0.0f;
#endif
				
				m_pHost->m_MoveCtrl.SendStopMoveCmd(vCurPos, fSpeed, iMoveMode);
			}
			else
			{
				vCurPos = m_pHost->m_MoveCtrl.GroundMove(g_vOrigin, 0.0f, fDeltaTime, m_pHost->m_fVertSpeed);
				m_pHost->SetPos(vCurPos);

				// 由于回放时会被误判为Blocked状态，所以忽略此判断
				if (m_pHost->m_MoveCtrl.MoveBlocked() >= 3 && gr.get_state() != game_record::enum_gr_replay)
				{
					m_pHost->ResetJump();	//	Reset jump counter
					Finish();
					m_pHost->m_MoveCtrl.SendStopMoveCmd(vCurPos, fSpeed, iMoveMode);
				}
				else
					m_pHost->m_MoveCtrl.SendMoveCmd(vCurPos, 0, m_vMoveDest, cdr.vAbsVelocity, iMoveMode);
			}
		}
		else if (m_iDestType == DEST_PUSH)
		{
			A3DVECTOR3 vMoveDir;
			A3DVECTOR3 vPrevPos = m_pHost->GetPos(); //Added 2011-07-08.先保存玩家上一次的位置，仅用于囚牢技能
			float fSpeed = m_pHost->GetGroundSpeed();
			bool bFinish = false;

			if (m_pHost->GetPushDir(vMoveDir, CECHostPlayer::MD_FORWARD | CECHostPlayer::MD_BACK | CECHostPlayer::MD_LEFT | CECHostPlayer::MD_RIGHT, fDeltaTime))
			{
				if (!vMoveDir.IsZero())
					m_pHost->SetDirAndUp(vMoveDir, g_vAxisY);

				vCurPos = m_pHost->m_MoveCtrl.GroundMove(vMoveDir, fSpeed, fDeltaTime, m_pHost->m_fVertSpeed);
				m_pHost->SetPos(vCurPos);
			}
			else
				bFinish = true;

			// 由于回放时会被误判为Blocked状态，所以忽略此判断
			if (bFinish || m_pHost->m_MoveCtrl.MoveBlocked() >= 3 && gr.get_state() != game_record::enum_gr_replay)
			{
				if (m_pHost->m_MoveCtrl.MoveBlocked() >= 3)
				{
					//	blocked case, so we clear the fYVel of cdr!
					//cdr.fYVel = 0.0f;
				}
#ifdef CDR_HL2_MOVE
				//cdr.vClipVel.Clear();
				cdr.vClipVel.x  = cdr.vClipVel.z = 0.0f;
#endif				
				Finish();
				m_pHost->m_vVelocity.Clear();
				m_pHost->m_MoveCtrl.SendStopMoveCmd(vCurPos, fSpeed, iMoveMode);
			}
			else
			{
				//Added and Modified 2011-07-08.对于囚牢技能，由于并不对囚牢进行碰撞检测，因此需要额外执行范围检测。
				if ( m_pHost->IsNeedCheckCurPosValid() && !m_pHost->CheckCurrentPosValid(vCurPos) )
				{
					Finish();
					m_pHost->m_vVelocity.Clear();
					m_pHost->m_MoveCtrl.SendStopMoveCmd(vPrevPos, fSpeed, iMoveMode);
					m_pHost->SetPos(vPrevPos);
				}
				else
				{
					m_pHost->m_vVelocity = vMoveDir * fSpeed;
					m_pHost->m_MoveCtrl.SendMoveCmd(vCurPos, 2, g_vOrigin, m_pHost->m_vVelocity, iMoveMode);
				}
			}
		}
	}
	else	//	m_bMeetSlide == true
	{
		m_pHost->m_MoveCtrl.SendStopMoveCmd(vCurPos, m_pHost->GetGroundSpeed(), GP_MOVE_SLIDE);
#ifdef CDR_HL2_MOVE
		cdr.vClipVel.x = cdr.vClipVel.z = 0.0f;
#endif
		
		Finish();
	}

	return true;
}

void CECHPWorkMove::Glide(float fMoveTime, const A3DVECTOR3& vMoveDirH,  float fDeltaTime, bool bFly)
{
	A3DVECTOR3 vHostDir = m_pHost->GetDir();
	float fAngle = DotProduct(vMoveDirH, vHostDir);
	a_Clamp(fAngle, -1.0f, 1.0f);
	fAngle = acosf(fAngle);

	if (m_pHost->IsShapeChanged())
	{
		if (!m_pHost->IsAdjustOrient())
		{
			float fTime = fAngle * (bFly ? ang_vel_fly : ang_vel_swim);
			m_pHost->SetDestDirAndUp(vMoveDirH, g_vAxisY, DWORD(fTime * 1000.f));
		}

		return;
	}

	m_fGlideSpan += fDeltaTime;
	float fDeltaRot = fabsf(m_fGlideVel * fDeltaTime);

	if (fAngle < DEG2RAD(5.0f) ||
	   (!m_bGliding && fMoveTime < 1.0f) ||
	   ( m_bGliding && (m_fGlideSpan >= m_fGlideTime || fDeltaRot >= fAngle)))
	{
		if (!vMoveDirH.IsZero())
			m_pHost->SetDirAndUp(vMoveDirH, g_vAxisY);

		if (m_bGliding)
		{
			m_bGliding = false;
			m_fCurPitch = 0;

			if (bFly)
			{
				m_pHost->PlayAction(m_pHost->GetMoveStandAction(true), 1.0f, false);
				//m_pHost->PlayAction(CECPlayer::ACT_FLY, 1.0f, false);
			}
		}

		return;
	}

	float pitch_co = pitch_co_wing;

	if (!m_bGliding)
	{
		m_fGlideSpan = 0;
		m_fGlideTime = fAngle * (bFly ? ang_vel_fly : ang_vel_swim);
		if (m_fGlideTime > fMoveTime) m_fGlideTime = fMoveTime;

		if (CrossProduct(vHostDir, vMoveDirH).y > 0) // turn right
		{
			m_fGlidePitch = (-pitch_co) * fAngle;
			m_fGlideAng = fAngle;
		}
		else
		{
			m_fGlidePitch = pitch_co * fAngle;
			m_fGlideAng = -fAngle;
		}

//		if (bFly) m_pHost->PlayAction(CECPlayer::ACT_FLY_GLIDE, 1.0f, false);

		m_fGlideVel = m_fGlideAng / m_fGlideTime;
		m_fGlideVel *= 1.5f;
		m_bGliding = true;
	}

	float fRatio = 1.0f - fabsf(fAngle / m_fGlideAng);
	A3DVECTOR3 vNewDir = a3d_RotatePosAroundY(vHostDir, m_fGlideVel * fDeltaTime);
	vNewDir.y = 0.0f;
	vNewDir.Normalize();

	if (fRatio > sidle_co)
	{
		fRatio -= sidle_co;
		fRatio = 1.0f - fRatio / sidle_co_r;
		m_fCurPitch = m_fGlidePitch * fRatio;
	}
	else
	{
		fRatio /= sidle_co;
		m_fCurPitch = m_fCurPitch * (1.0f - fRatio) + m_fGlidePitch * fRatio;
	}

	if (m_pHost->IsShapeChanged())
		m_pHost->SetDirAndUp(vNewDir, g_vAxisY);
	else
	{
		A3DVECTOR3 vNewUp = a3d_RotatePosAroundAxis(g_vAxisY, vNewDir, m_fCurPitch);
		m_pHost->SetDirAndUp(vNewDir, vNewUp);
	}
}

//	Tick routine of flying or swimming
bool CECHPWorkMove::Tick_FlySwim(float fDeltaTime)
{
	A3DVECTOR3 vCurPos = m_pHost->GetPos();
 	int iMoveMode = (m_pHost->m_iMoveEnv == CECPlayer::MOVEENV_AIR) ? GP_MOVE_AIR : GP_MOVE_WATER;

	float na, fMaxSpeed;
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

	if (m_bReadyCancel || m_bMeetSlide)
	{
		m_pHost->m_MoveCtrl.SendStopMoveCmd(vCurPos, fMaxSpeed, iMoveMode | GP_MOVE_RUN);
		Finish();
		return true;
	}

	if (m_pHost->m_dwMoveRelDir & (CECHostPlayer::MD_LEFT | CECHostPlayer::MD_RIGHT | CECHostPlayer::MD_FORWARD | CECHostPlayer::MD_BACK))
		m_iDestType = DEST_PUSH;

	ON_AIR_CDR_INFO& cdr = *m_pHost->m_pAirCDRInfo;

	if (m_iDestType == DEST_DIR)
	{
		A3DVECTOR3 vPushDir(0.0f);
		m_pHost->GetPushDir(vPushDir, CECHostPlayer::MD_ALL);
		vPushDir.x = vPushDir.z = 0.0f;

		float fSpeed1H = m_pHost->m_vVelocity.MagnitudeH();
		float fSpeed1V = m_pHost->m_vVelocity.y;

		A3DVECTOR3 vMoveDirH = m_vMoveDest;
		float pa=EC_PUSH_ACCE;

		float fSpeed2H = fSpeed1H + (pa + na) * fDeltaTime;
		if (!pa && fSpeed2H < 0.0f)
			fSpeed2H = 0.0f;	//	Only resistance couldn't generate negative speed
		else if (fSpeed2H > fMaxSpeed)
			fSpeed2H = fMaxSpeed;

		Glide(5.0f, vMoveDirH, fDeltaTime, bInAir);

		vMoveDirH = m_pHost->GetDir();
		vMoveDirH.y = 0;
		vMoveDirH.Normalize();

		//	Vertical speed
		float fSpeed2V = CalcFlySwimVertSpeed(fSpeed1V, vPushDir.y, EC_PUSH_ACCE, fDeltaTime);
		A3DVECTOR3 vVel2 = vMoveDirH * fSpeed2H + g_vAxisY * fSpeed2V;

		//	Air/water move
		vCurPos = m_pHost->m_MoveCtrl.AirWaterMove(vVel2, fDeltaTime, bInAir);
		m_pHost->SetPos(vCurPos);
		m_pHost->m_vVelocity = vVel2;

		if (m_pHost->m_MoveCtrl.MoveBlocked() >= 3)
		{
			Finish();
			m_pHost->m_MoveCtrl.SendStopMoveCmd(vCurPos, fMaxSpeed, iMoveMode | GP_MOVE_RUN);
		}
		else
			m_pHost->m_MoveCtrl.SendMoveCmd(vCurPos, 0, m_vMoveDest, vVel2, iMoveMode | GP_MOVE_RUN);
	}
	else if (m_iDestType == DEST_2D)
	{
		A3DVECTOR3 vPushDir(0.0f);
		m_pHost->GetPushDir(vPushDir, CECHostPlayer::MD_ALL);
		vPushDir.x = vPushDir.z = 0.0f;

		float fSpeed1H = m_pHost->m_vVelocity.MagnitudeH();
		float fSpeed1V = m_pHost->m_vVelocity.y;

		A3DVECTOR3 vMoveDirH = m_vMoveDest - vCurPos;
		vMoveDirH.y = 0.0f;
		float fDistH = vMoveDirH.Normalize();

		float pa=0.0f;

		//	Calculate the distance to reduce velocity to 0
		float s = -0.5f * fSpeed1H * fSpeed1H / na;
		if (fDistH > s - 0.01f)
			pa = EC_PUSH_ACCE;

		float fSpeed2H = fSpeed1H + (pa + na) * fDeltaTime;
		if (!pa && fSpeed2H < 0.0f)
			fSpeed2H = 0.0f;	//	Only resistance couldn't generate negative speed
		else if (fSpeed2H > fMaxSpeed)
			fSpeed2H = fMaxSpeed;

		Glide(fDistH / fMaxSpeed, vMoveDirH, fDeltaTime, bInAir);

		vMoveDirH = m_pHost->GetDir();
		vMoveDirH.y = 0;
		vMoveDirH.Normalize();

		//	Vertical speed
		float fSpeed2V = CalcFlySwimVertSpeed(fSpeed1V, vPushDir.y, EC_PUSH_ACCE, fDeltaTime);
		A3DVECTOR3 vVel2 = vMoveDirH * fSpeed2H + g_vAxisY * fSpeed2V;

		//	Air/water move
		vCurPos = m_pHost->m_MoveCtrl.AirWaterMove(vVel2, fDeltaTime, bInAir);

		if (m_pHost->m_MoveCtrl.MoveBlocked() >= 3)
		{
			vVel2.Clear();
			Finish();
		}
		else
		{
			//	Reached destination ?
			A3DVECTOR3 vMoveDelta = vCurPos - m_pHost->GetPos();
			vMoveDelta.y = 0.0f;
			float fMoveDistH = vMoveDelta.Normalize();
			if (fMoveDistH >= fDistH)
			{
				vVel2.x = vVel2.z = 0.0f;
				if (!vVel2.y)
					Finish();
				else
					m_iDestType = DEST_PUSH;
			}
		}

		m_pHost->SetPos(vCurPos);
		m_pHost->m_vVelocity = vVel2;

		if (m_bFinished)
			m_pHost->m_MoveCtrl.SendStopMoveCmd(vCurPos, vVel2.Magnitude(), iMoveMode | GP_MOVE_RUN);
		else
			m_pHost->m_MoveCtrl.SendMoveCmd(vCurPos, 0, m_vMoveDest, vVel2, iMoveMode | GP_MOVE_RUN);
	}
	else if (m_iDestType == DEST_3D)
	{
		float fSpeed1 = m_pHost->m_vVelocity.Magnitude();
		A3DVECTOR3 vMoveDir = m_vMoveDest - vCurPos;
		float fDist = vMoveDir.Normalize();

		float pa=0.0f;

		//	Calculate the distance to reduce velocity to 0
		float s = -0.5f * fSpeed1 * fSpeed1 / na;
		if (fDist > s - 0.01f)
			pa = EC_PUSH_ACCE;

		float fSpeed2 = fSpeed1 + (pa + na) * fDeltaTime;
		if (!pa && fSpeed2 < 0.0f)
			fSpeed2 = 0.0f;	//	Only resistance couldn't generate negative speed

		a_Clamp(fSpeed2, -fMaxSpeed, fMaxSpeed);

		A3DVECTOR3 vMoveDirH(vMoveDir.x, 0.0f, vMoveDir.z);
		if (!vMoveDirH.IsZero())
			m_pHost->SetDestDirAndUp(vMoveDirH, g_vAxisY, 100);

		//	Air/water move
	//	A3DVECTOR3 vVel1 = vMoveDir * fSpeed1;
		A3DVECTOR3 vVel2 = vMoveDir * fSpeed2;
		vCurPos = m_pHost->m_MoveCtrl.AirWaterMove(vMoveDir, fSpeed2, fDeltaTime, bInAir);

		if (m_pHost->m_MoveCtrl.MoveBlocked() >= 3)
		{
			vVel2.Clear();
			Finish();
		}
		else
		{
			//	Reached destination ?
			A3DVECTOR3 vMoveDelta = vCurPos - m_pHost->GetPos();
			float fMoveDist = vMoveDelta.Normalize();
			if (fMoveDist >= fDist)
			{
				vVel2.Clear();
				Finish();
			}
		}

		m_pHost->SetPos(vCurPos);
		m_pHost->m_vVelocity = vVel2;

		if (m_bFinished)
			m_pHost->m_MoveCtrl.SendStopMoveCmd(vCurPos, fMaxSpeed, iMoveMode | GP_MOVE_RUN);
		else
			m_pHost->m_MoveCtrl.SendMoveCmd(vCurPos, 1, m_vMoveDest, vVel2, iMoveMode | GP_MOVE_RUN);
	}
	else if (m_iDestType == DEST_PUSH)
	{
		A3DVECTOR3 vPushDir(0.0f), vUp;
		bool bPush = m_pHost->GetPushDir(vPushDir, CECHostPlayer::MD_FORWARD | CECHostPlayer::MD_BACK | CECHostPlayer::MD_LEFT | CECHostPlayer::MD_RIGHT, fDeltaTime);

		if (!bPush)
		{
			vPushDir = m_pHost->GetCameraCoord()->GetDir();
			vPushDir.y = 0;
			vPushDir.Normalize();
		}
	
  		int nPitchDir = 0;
		if ((m_pHost->m_dwMoveRelDir & (CECHostPlayer::MD_LEFT | CECHostPlayer::MD_RIGHT)) == 0)
		{
			A3DVECTOR3 vOldDir = m_pHost->GetDir();
			vOldDir.y = 0;
			vOldDir.Normalize();

			A3DVECTOR3 vNewDir = vPushDir;
			vNewDir.y = 0;
			vNewDir.Normalize();

			float fAngle = DotProduct(vOldDir, vNewDir);
			if (fAngle < 1.0f - 1e-4)
			{
				A3DVECTOR3 vUp = CrossProduct(vOldDir, vNewDir);
				if (vUp.y > 0) nPitchDir = 1;
				else nPitchDir = -1;
				if (m_pHost->m_dwMoveRelDir & CECHostPlayer::MD_BACK)
					nPitchDir = -nPitchDir;
			}
		}

		if (m_pHost->m_dwMoveRelDir)
		{
			if (m_pHost->m_dwMoveRelDir & ~(CECHostPlayer::MD_ABSDOWN | CECHostPlayer::MD_ABSUP))
			{
				float fPitchDelta = push_pitch_vel_wing * fDeltaTime;

				if ((m_pHost->m_dwMoveRelDir & CECHostPlayer::MD_LEFT) || nPitchDir == -1)
				{
					if (m_pHost->m_dwMoveRelDir & CECHostPlayer::MD_BACK)
						m_fPushPitch -= fPitchDelta;
					else
						m_fPushPitch += fPitchDelta;
				}
				else if ((m_pHost->m_dwMoveRelDir & CECHostPlayer::MD_RIGHT) || nPitchDir == 1)
				{
					if (m_pHost->m_dwMoveRelDir & CECHostPlayer::MD_BACK)
						m_fPushPitch += fPitchDelta;
					else
						m_fPushPitch -= fPitchDelta;
				}
				else if (m_fPushPitch > fPitchDelta)
					m_fPushPitch -= fPitchDelta;
				else if (m_fPushPitch < -fPitchDelta)
					m_fPushPitch += fPitchDelta;
				else
					m_fPushPitch = 0;

				A3DVECTOR3 vRight = m_pHost->GetCameraCoord()->GetRight();
				float fLean = -asinf(m_pHost->GetCameraCoord()->GetDir().y);
				a_Clamp(fLean, -lean_max_ang, lean_max_ang);

				if (fabsf(fLean) > DEG2RAD(3.0f))
				{
					vPushDir = a3d_RotatePosAroundAxis(vPushDir, vRight, fLean);
					vUp = a3d_RotatePosAroundAxis(g_vAxisY, vRight, fLean);
				}
				else
					vUp = g_vAxisY;

				float pitch_ang = pitch_ang_wing;
				a_Clamp(m_fPushPitch, -pitch_ang, pitch_ang);

				if (fabsf(m_fPushPitch) > DEG2RAD(4.0f))
					vUp = a3d_RotatePosAroundAxis(vUp, vPushDir, m_fPushPitch);

				m_pHost->SetDirAndUp(vPushDir, vUp);
			}

		//	if (bPush)
			if (bPush || (m_pHost->m_dwMoveRelDir & (CECHostPlayer::MD_ABSDOWN | CECHostPlayer::MD_ABSUP)))
			{
			//	float pa = bPush ? EC_PUSH_ACCE : 0.0f;
				float pa = EC_PUSH_ACCE;
				float na = m_pHost->m_iMoveEnv == CECPlayer::MOVEENV_AIR ? EC_NACCE_AIR : EC_NACCE_WATER;
				float fAccel = pa + na;

				float fSpeed1 = m_pHost->m_vVelocity.Magnitude();
				float fSpeed2 = fSpeed1 + fAccel * fDeltaTime;
				a_Clamp(fSpeed2, 0.0f, fMaxSpeed);

				//	Air/water move
				A3DVECTOR3 vVelDir(0.0f);
				if (bPush)
					vVelDir = vPushDir;

				if (m_pHost->m_dwMoveRelDir & CECHostPlayer::MD_ABSDOWN)
				{
					vVelDir += -g_vAxisY;
					vVelDir.Normalize();
				}
				else if (m_pHost->m_dwMoveRelDir & CECHostPlayer::MD_ABSUP)
				{
					vVelDir += g_vAxisY;
					vVelDir.Normalize();
				}

			//	A3DVECTOR3 vVel = vPushDir * fSpeed2;
				A3DVECTOR3 vVel = vVelDir * fSpeed2;
				vCurPos = m_pHost->m_MoveCtrl.AirWaterMove(vVel, fDeltaTime, bInAir);

				if (m_pHost->m_MoveCtrl.MoveBlocked() >= 3)
				{
					Finish();
					m_pHost->m_vVelocity.Clear();
					m_pHost->m_MoveCtrl.SendStopMoveCmd(vCurPos, fMaxSpeed, iMoveMode | GP_MOVE_RUN);
				}
				else
				{
					//Added and Modified 2011-07-11.对于囚牢技能，由于并不对囚牢进行碰撞检测，因此需要额外执行范围检测。
					if ( m_pHost->IsNeedCheckCurPosValid() && !m_pHost->CheckCurrentPosValid(vCurPos) )
					{
						Finish();
						m_pHost->m_vVelocity.Clear();
						m_pHost->m_MoveCtrl.SendStopMoveCmd(m_pHost->GetPos(), fMaxSpeed, iMoveMode | GP_MOVE_RUN);
					}
					else
					{
						m_pHost->SetPos(vCurPos);
						m_pHost->m_vVelocity = vVel;
						m_pHost->m_MoveCtrl.SendMoveCmd(vCurPos, 2, g_vOrigin, vVel, iMoveMode | GP_MOVE_RUN);
					}
				}
			}
		}
		else
		{
			Finish();
			m_fPushPitch = 0;
			m_pHost->m_vVelocity.Clear();
			m_pHost->m_MoveCtrl.SendStopMoveCmd(vCurPos, fMaxSpeed, iMoveMode | GP_MOVE_RUN);			
		}
	}
	else if (m_iDestType == DEST_STANDJUMP)
	{
		//	If host player fly off when jumping up, code will go here. In the
		//	case, just stop move work is well.
		Finish();
	}

	return true;
}

//	Calculate vertical speed when fly or swim
float CECHPWorkMove::CalcFlySwimVertSpeed(float fSpeed1, float fPushDir, float fPushAccel,
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

	//	When player free fall into water, fSpeed1 may be >= fMaxSpeed
	if (fabs(fSpeed1) > fMaxSpeed)
	{
		na *= 2.0f;
		if (fPushDir * fSpeed1 > 0.0f)
			pa = 0.0f;
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
	
	//	If accelerate and speed on same direction, limit speed
	if (fAccel * fSpeed2 > 0.0f)
		a_Clamp(fSpeed2, -fMaxSpeed, fMaxSpeed);

	return fSpeed2;
}

//	Finish work
void CECHPWorkMove::Finish()
{
	m_bFinished = true;
	if (m_pHost->m_pMoveTargetGFX)
		m_pHost->m_pMoveTargetGFX->Stop();

	A3DVECTOR3 vDir = m_pHost->GetDir();
	vDir.y = 0;
	vDir.Normalize();
	if (!vDir.IsZero())
		m_pHost->SetDirAndUp(vDir, g_vAxisY);

#ifdef CDR_HL2_MOVE
	//m_pHost->m_pCDRInfo->vClipVel.Clear();
	m_pHost->m_pCDRInfo->vClipVel.x = 0;
	m_pHost->m_pCDRInfo->vClipVel.z = 0;
#endif
}

//	Work is cancel
void CECHPWorkMove::Cancel()
{
	if (m_pHost->m_pMoveTargetGFX)
		m_pHost->m_pMoveTargetGFX->Stop();

	A3DVECTOR3 vDir = m_pHost->GetDir();
	vDir.y = 0;
	vDir.Normalize();
	if (!vDir.IsZero())
		m_pHost->SetDirAndUp(vDir, g_vAxisY);
}

bool CECHPWorkMove::gen_record_msg(ECMSG& msg)
{
	msg.dwMsg = MSG_HST_MOVE_RECORD;
	msg.iManager = MAN_PLAYER;
	msg.iSubID = 0;
	msg.dwParam1 = m_iDestType;
	msg.dwParam2 = *(DWORD*)&m_vMoveDest.x;
	msg.dwParam3 = *(DWORD*)&m_vMoveDest.y;
	msg.dwParam4 = *(DWORD*)&m_vMoveDest.z;
	return true;
}


///////////////////////////////////////////////////////////////////////////////
//
//CECHPWorkBeMoved类的定义 Added 2011-07-28.
//
///////////////////////////////////////////////////////////////////////////////
CECHPWorkBeMoved::CECHPWorkBeMoved(CECHPWorkMan* pWorkMan) :
CECHPWork(WORK_BE_MOVED, pWorkMan)
{
	m_dwMask		= MASK_BE_MOVED;
	m_dwTransMask	= MASK_STAND | MASK_TRACEOBJECT | MASK_FOLLOW;

	CECHPWorkBeMoved::Reset();
}

CECHPWorkBeMoved::~CECHPWorkBeMoved()
{
}

//	Reset work
void CECHPWorkBeMoved::Reset()
{
	CECHPWork::Reset();

	m_bReadyCancel	= false;
	m_fSpeed		= 0.0f;
}

//	Copy work data
bool CECHPWorkBeMoved::CopyData(CECHPWork* pWork)
{
	if (!CECHPWork::CopyData(pWork))
	{
		return false;
	}

	CECHPWorkBeMoved* pSrc = (CECHPWorkBeMoved*)pWork;

	m_bReadyCancel	= pSrc->m_bReadyCancel;

	m_vDestPos		= pSrc->m_vDestPos;
	m_vCurDir		= pSrc->m_vCurDir;
	m_fSpeed		= pSrc->m_fSpeed;
		
	return true;
}

//	On first tick
void CECHPWorkBeMoved::OnFirstTick()
{
	if (m_pHost->m_iMoveEnv != CECPlayer::MOVEENV_AIR)
	{
		m_pHost->ShowWing(false);
	}
}

//	Set destination position or direction
void CECHPWorkBeMoved::SetDestination(const A3DVECTOR3& vDestPos, float fSpeed)
{
	m_vDestPos	= vDestPos;
	m_fSpeed	= fSpeed;

	m_pHost->SetAdjustOrient(false);

	m_vCurDir = m_vDestPos - m_pHost->GetPos();
	m_vCurDir.y = 0.0f;
	m_vCurDir.Normalize();
}

//	Tick routine
bool CECHPWorkBeMoved::Tick(DWORD dwDeltaTime)
{
	CECHPWork::Tick(dwDeltaTime);

	float fDeltaTime = dwDeltaTime * 0.001f;
#ifdef CDR_HL2_MOVE
	bool bJumpInWater = m_pHost->m_iMoveEnv == CECPlayer::MOVEENV_WATER && m_pHost->IsJumping() && (m_pHost->m_pCDRInfo->vAbsVelocity.y > 0 || m_pHost->m_pCDRInfo->vClipVel.y > 0);
#else
	bool bJumpInWater = m_pHost->m_iMoveEnv == CECPlayer::MOVEENV_WATER && m_pHost->IsJumping() && (m_pHost->m_pCDRInfo->vAbsVelocity.y > 0 || m_pHost->m_pCDRInfo->fYVel > 0);
#endif
	if (m_pHost->m_iMoveEnv == CECPlayer::MOVEENV_GROUND || bJumpInWater)
	{
		//	Play appropriate actions
		m_pHost->PlayAction(CECPlayer::ACT_STAND, 1.0f, false);

		Tick_Move(fDeltaTime);
	}

	return true;
}

//	Tick routine of walking on ground
bool CECHPWorkBeMoved::Tick_Move(float fDeltaTime)
{
	A3DVECTOR3 vCurPos = m_pHost->GetPos();
	CDR_INFO& cdr = *m_pHost->m_pCDRInfo;

	//取消该work
	if (m_bReadyCancel && m_pHost->m_GndInfo.bOnGround)
	{
		Finish();
		return true;
	}

	float fDist = 0.0f;
	if (m_pHost->m_GndInfo.bOnGround)
	{
		//	Ajust direction only when player on ground
		m_vCurDir = m_vDestPos - vCurPos;
		m_vCurDir.y = 0.0f;
		fDist = m_vCurDir.Normalize();
	}
	else
	{
		fDist = a3d_MagnitudeH(m_vDestPos - vCurPos);
	}

	//	Ground move
	vCurPos = m_pHost->m_MoveCtrl.GroundMove(m_vCurDir, m_fSpeed, fDeltaTime, m_pHost->m_fVertSpeed);
	if (!m_vCurDir.IsZero())
	{
		m_pHost->SetDirAndUp(m_vCurDir, g_vAxisY);
	}
	
	//
	if (m_pHost->m_MoveCtrl.MoveBlocked() >= 3)
	{
		Finish();
	}
	else if ( cdr.bOnSurface )
	{
		//	Check whether we have arrived destination
		A3DVECTOR3 vMoveDelta = vCurPos - m_pHost->GetPos();
		vMoveDelta.y = 0.0f;
		float fMoveDist = vMoveDelta.Normalize();

		if (fMoveDist >= fDist)//已经到达目标位置（再走就超过了）
		{
#ifdef CDR_HL2_MOVE
		   cdr.vClipVel.x = cdr.vClipVel.z = 0.0f;
#endif
			Finish();
			m_pHost->SetPos(m_vDestPos);//vCurPos 设置为目标位置？不应该是更新后的最后位置？？！
			m_pHost->m_MoveCtrl.SetLastSevPos(m_vDestPos);
			//需要设置？
			if (m_pHost->m_pCDRInfo->bOnSurface && !m_pHost->m_pCDRInfo->vTPNormal.IsZero())
			{
				m_pHost->SetGroundNormal(m_pHost->m_pCDRInfo->vTPNormal);
			}
		}
		else
		{
			//Added and Modified 2011-07-11.对于囚牢技能，由于并不对囚牢进行碰撞检测，因此需要额外执行范围检测。
			if ( m_pHost->IsNeedCheckCurPosValid() && !m_pHost->CheckCurrentPosValid(vCurPos) )
			{
#ifdef CDR_HL2_MOVE
				cdr.vClipVel.x = cdr.vClipVel.z = 0.0f;
#endif
				Finish();
				m_pHost->m_vVelocity.Clear();
			}
			else
			{
				m_pHost->SetPos(vCurPos);
			}
		}
	}
	else
	{
		m_pHost->SetPos(vCurPos);
	}
	//
	return true;
}

//	Finish work
void CECHPWorkBeMoved::Finish()
{
	m_bFinished = true;

	A3DVECTOR3 vDir = m_pHost->GetDir();
	vDir.y = 0;
	vDir.Normalize();
	if (!vDir.IsZero())
	{
		m_pHost->SetDirAndUp(vDir, g_vAxisY);
	}

	//重置HostMove中的变量值，因为在移动时，使用了HostMove中的GroundMove等函数接口，否则，在移动时可能会出现错误
	m_pHost->m_MoveCtrl.ResetMoveTime();

#ifdef CDR_HL2_MOVE
	m_pHost->m_pCDRInfo->vClipVel.x = 0;
	m_pHost->m_pCDRInfo->vClipVel.z = 0;
#endif
}

//	Work is cancel
void CECHPWorkBeMoved::Cancel()
{
	A3DVECTOR3 vDir = m_pHost->GetDir();
	vDir.y = 0;
	vDir.Normalize();
	if (!vDir.IsZero())
	{
		m_pHost->SetDirAndUp(vDir, g_vAxisY);
	}
}



///////////////////////////////////////////////////////////////////////////////
//
//CECHPWorkSpecialMove类的定义 Added 2012-06-12.
//
///////////////////////////////////////////////////////////////////////////////
CECHPWorkSpecialMove::CECHPWorkSpecialMove(CECHPWorkMan* pWorkMan) :
CECHPWork(WORK_SPECIAL_MOVE, pWorkMan)
{
	m_dwMask		= MASK_SPECIAL_MOVE;
	m_dwTransMask	= MASK_STAND;

	CECHPWorkSpecialMove::Reset();
}

CECHPWorkSpecialMove::~CECHPWorkSpecialMove()
{
}

//	Reset work
void CECHPWorkSpecialMove::Reset()
{
	CECHPWork::Reset();

	m_bReadyCancel		= false;
	m_vCurrentVelocity	= A3DVECTOR3(0.0f);
	m_vAcceleration		= A3DVECTOR3(0.0f);
}

//	Copy work data
bool CECHPWorkSpecialMove::CopyData(CECHPWork* pWork)
{
	if (!CECHPWork::CopyData(pWork))
	{
		return false;
	}

	CECHPWorkSpecialMove* pSrc = (CECHPWorkSpecialMove*)pWork;

	m_bReadyCancel		= pSrc->m_bReadyCancel;

	m_vCurrentVelocity	= pSrc->m_vCurrentVelocity;
	m_vAcceleration		= pSrc->m_vAcceleration;
		
	return true;
}

//	Set move velocity and acceleration.
void CECHPWorkSpecialMove::SetVelocityAndAcceleration(const A3DVECTOR3& vVelocity, const A3DVECTOR3& vAcceleration)
{
	m_vCurrentVelocity	= vVelocity;
	m_vAcceleration		= vAcceleration;		
}

//	On first tick
void CECHPWorkSpecialMove::OnFirstTick()
{
	A3DVECTOR3 vAccelerationDir;
	if (m_pHost->GetPushDir(vAccelerationDir, CECHostPlayer::MD_FORWARD | CECHostPlayer::MD_BACK | CECHostPlayer::MD_LEFT | CECHostPlayer::MD_RIGHT, 20))	//获得方向键
	{
		if (!vAccelerationDir.IsZero() && m_vCurrentVelocity.IsZero())	//按键方向 == 加速度方向
		{
			//计算新的加速度
			vAccelerationDir.Normalize();
			m_vCurrentVelocity = vAccelerationDir;
		}
	}

	// Get current acceleration
	UpdateCurrentAcceleration( 10 );

	// Send the Start special move cmd to server.
	m_pHost->m_MoveCtrl.SendStartSpecialMoveCmd( m_vCurrentVelocity, m_vAcceleration, m_pHost->GetPos(), m_pHost->GetCollisionState() );

	//----Test------
	if ( m_pHost->GetCollisionState() )
	{
		a_LogOutput( 1, "PlayerID=%d----------HostPlayer发送被撞开始移动消息！----------", m_pHost->GetCharacterID() );
	}
	//----Test end--
	// Reset the host player collision state to false
	m_pHost->SetCollisionState( false );

	// !!We should set the clip velocity of the CDR
#ifdef CDR_HL2_MOVE
			m_pHost->m_pCDRInfo->vClipVel.x = m_vCurrentVelocity.x;
			m_pHost->m_pCDRInfo->vClipVel.z = m_vCurrentVelocity.z;
#endif	
}

//	Update the current acceleration
void CECHPWorkSpecialMove::UpdateCurrentAcceleration( float fDeltaTime )
{
	//	Get the current direction and velocity.
	A3DVECTOR3 vAccelerationDir;
	if (m_pHost->GetPushDir(vAccelerationDir, CECHostPlayer::MD_FORWARD | CECHostPlayer::MD_BACK | CECHostPlayer::MD_LEFT | CECHostPlayer::MD_RIGHT, fDeltaTime))	//获得方向键
	{
		if (!vAccelerationDir.IsZero())	//按键方向 == 加速度方向
		{
			//计算新的加速度
			vAccelerationDir.Normalize();
			float fAccelerationRatio = FORCE_FACTOR / (float)(m_pHost->GetRealMass());
			if ( DotProduct( vAccelerationDir, m_vCurrentVelocity ) < 0.0f )
			{
				fAccelerationRatio *= BACKWARD_FACTOR;//1.6f
			}
			m_vAcceleration = vAccelerationDir * fAccelerationRatio;
			//m_pHost->SetDirAndUp(vMoveDir, g_vAxisY);	//需要设置为速度方向
			if ( m_pHost->GetCollisionSkillState() & COLLISION_STATE_CHAOS ) //如果处于混乱机能状态，则按键方向全部相反
			{
				m_vAcceleration = -m_vAcceleration;
			}

		}
	}
	else	//无按键方向，则重新确定加速度
	{
		//如果当前速度为0，且无按键操作，则，无加速度
		if ( m_vCurrentVelocity.IsZero() || m_vCurrentVelocity.Magnitude() <= 0.01f )
		{
			m_vAcceleration.Clear();
		}
		else	//加速度方向为当前速度的相反方向
		{
			A3DVECTOR3 vDir = m_vCurrentVelocity;
			vDir.Normalize();
			float fAccelerationRatio = FORCE_FACTOR / (float)(m_pHost->GetRealMass());
			m_vAcceleration = -vDir * fAccelerationRatio;
		}
	}	
}

//	Tick routine
bool CECHPWorkSpecialMove::Tick(DWORD dwDeltaTime)
{
	CECHPWork::Tick(dwDeltaTime);

	float fDeltaTime = dwDeltaTime * 0.001f;

	if (m_pHost->m_iMoveEnv == CECPlayer::MOVEENV_GROUND)
	{
		//	Play appropriate actions
		m_pHost->PlayAction(CECPlayer::ACT_RUN, 1.0f, false);

		Tick_Move(fDeltaTime);
	}

	return true;
}

//	Tick routine of walking on ground
bool CECHPWorkSpecialMove::Tick_Move(float fDeltaTime)
{
	// If the HostPlayer is in collision rooting state,then NO special move.
	if ( m_pHost->IsCollisionRooting() )	//处于不可移动状态
	{
		Finish();
		m_pHost->m_MoveCtrl.SendStopSpecialMoveCmd();
		return true;
	}

	//-------------------------------------------------------------------------
	//Test!!!!!!!!!!
//	static unsigned int uFrameCount = 0;
//	uFrameCount++;
	//-------------------------------------------------------------------------

	// Set the Host player special moving flag to true;
	m_pHost->m_bSpecialMoving = true;

	// Set the NoDelay flag to indicate that sending the move message immediately.
	bool bNoDelay = false;
	A3DVECTOR3 vPrevAccelerationDir = m_vAcceleration;

	// Update the current acceleration.
	UpdateCurrentAcceleration(fDeltaTime);

	// Calculate the Difference between the Previous acceleration and the current one.
	// If the Difference is big enough, then set the NoDelay flag to true.
	A3DVECTOR3 vCurAccelerationDir = m_vAcceleration;
	vPrevAccelerationDir.Normalize();
	vCurAccelerationDir.Normalize();
	if ( DotProduct( vPrevAccelerationDir, vCurAccelerationDir ) < 0.85f )	//0.85 is a Threshold.
	{
		bNoDelay = true;
	}

	// store the previous value.
	A3DVECTOR3 vPrevVelocity = m_vCurrentVelocity;
	// vt = v0 + at.
	m_vCurrentVelocity += m_vAcceleration * fDeltaTime;
	float fCurrentSpeed = m_vCurrentVelocity.Magnitude();
	// if the current velocity is in the opposite direction, then it means invalide.
	if ( DotProduct( vPrevVelocity, m_vCurrentVelocity ) < 0.0f || fCurrentSpeed <= 0.01f)
	{
		A3DVECTOR3 vDir = vPrevVelocity;
		float iMag = vDir.Normalize();
		A3DVECTOR3 vCurPos = m_pHost->m_MoveCtrl.GroundMove(vDir, fCurrentSpeed/2, fCurrentSpeed/m_vAcceleration.Normalize(), m_pHost->m_fVertSpeed);
		
		m_pHost->SetPos(vCurPos);
		if(iMag>0.5f)
			m_pHost->SetDirAndUp(vDir, g_vAxisY);

		Finish();
		m_pHost->m_MoveCtrl.SendStopSpecialMoveCmd();
		return true;
	}

	//---------------------------------------------------
	//如果当前速度大于设定阈值，则设置到最大速度
	if ( m_vCurrentVelocity.Magnitude() > (float)COLLISION_MAX_SPEED )
	{
		m_vCurrentVelocity.Normalize();
		m_vCurrentVelocity *= (float)COLLISION_MAX_SPEED;
	}
	//Set the current velocity and acceleration to the host player,Modified 2012-08-15.
	m_pHost->SetVelocity( m_vCurrentVelocity );
	m_pHost->SetAcceleration( m_vAcceleration );
	//---------------------------------------------------

	A3DVECTOR3 vCurDir = m_vCurrentVelocity;
	vCurDir.Normalize();
	A3DVECTOR3 vCurPos;

/*	if ( fCurrentSpeed <= 0.01f )	//0.01f
	{
		Finish();
		m_pHost->m_MoveCtrl.SendStopSpecialMoveCmd();
		return true;
	}
	else
*/	{
//		vCurPos = m_pHost->m_MoveCtrl.GroundMove(vCurDir, fCurrentSpeed, fDeltaTime, m_pHost->m_fVertSpeed);
		//TEST
/*		a_LogOutput( 1, "==========vCurPos = (%f, %f, %f)", vCurPos.x, vCurPos.y, vCurPos.z );
		m_pHost->GetCDRInfo()->vClipVel = vPrevClipVelocity;
		A3DVECTOR3 vNewCurPos = m_pHost->m_MoveCtrl.GroundMove(vCurDir, fCurrentSpeed, fDeltaTime, m_pHost->m_fVertSpeed);
		a_LogOutput( 1, "==========vNewCurPos = (%f, %f, %f)", vNewCurPos.x, vNewCurPos.y, vNewCurPos.z );*/
		//TEST END
/*		APtrArray<CECElsePlayer*>& tracePlayers = g_pGame->GetGameRun()->GetWorld()->GetPlayerMan()->GetTracePlayers();
		if ( tracePlayers.GetSize() > 0 )	//collide with certain else players.
		{
			for ( int i = 0; i < tracePlayers.GetSize(); ++i )
			{
				CECElsePlayer* pPlayer = tracePlayers[i];
				A3DVECTOR3 vVelocityElsePlayer = pPlayer->GetVelocity();
				UpdateVelocityAfterCD( m_pHost->GetPos(), pPlayer->GetPos(), 1, 1, m_vCurrentVelocity, vVelocityElsePlayer );
				//compute the new velocity after collision.
				if ( pPlayer->GetVelocity().IsZero() )
				{
					pPlayer->StartSpecialMove();
					A3DVECTOR3 vAcceleration = -vVelocityElsePlayer;
					vAcceleration.Normalize();
					vAcceleration *= ACCELERATION_FACTOR;
					pPlayer->SetAcceleration( vAcceleration );
				}
				pPlayer->SetVelocity( vVelocityElsePlayer );
			}
			vCurPos = m_pHost->GetPos();
		}
*/
		//m_pHost->SetPos(vCurPos);
		bool bCollide = g_pGame->GetGameRun()->GetWorld()->GetPlayerMan()->UpdateHostCollisionStates( fDeltaTime );
		if ( bCollide )
		{
//			a_LogOutput( 1, "----------PlayerID=%d, PrevCDVelocity=(%f, %f, %f), AfterCDVelocity=(%f, %f, %f)", m_pHost->GetCharacterID(), m_vCurrentVelocity.x, m_vCurrentVelocity.y, m_vCurrentVelocity.z, 
//								m_pHost->GetVelocity().x, m_pHost->GetVelocity().y, m_pHost->GetVelocity().z);
			m_vCurrentVelocity = m_pHost->GetVelocity();
			//-----------------------------------------------------------------
/*			static unsigned int uCollideFrameCount = 0;
			if ( uFrameCount == (uCollideFrameCount+1) )
			{
				Finish();
				m_pHost->m_MoveCtrl.SendStopSpecialMoveCmd();
				a_LogOutput( 1, "----------PlayerID=%d, 连续两次碰撞了！uCollideFrameCount = %d, uFrameCount = %d", m_pHost->GetCharacterID(), uCollideFrameCount, uFrameCount );
				uCollideFrameCount = uFrameCount;
				return true;
			}
			else
			{
				uCollideFrameCount = uFrameCount;
			}*/
			//-----------------------------------------------------------------
#ifdef CDR_HL2_MOVE
			m_pHost->m_pCDRInfo->vClipVel.x = m_vCurrentVelocity.x;
			m_pHost->m_pCDRInfo->vClipVel.z = m_vCurrentVelocity.z;
#endif
			m_pHost->m_MoveCtrl.SendSpecialMoveCmd( m_vCurrentVelocity, m_vAcceleration, m_pHost->GetPos(), true, 1 );//1:标示碰撞状态
			return true;
		}

		vCurPos = m_pHost->m_MoveCtrl.GroundMove(vCurDir, fCurrentSpeed, fDeltaTime, m_pHost->m_fVertSpeed);
		//---------------------------------------------------------------------
		//Test
		A3DVECTOR3 vDistTest = vCurPos - m_pHost->GetPos();
//		a_LogOutput( 1, "----------RealValue--->PlayerID=%d: vDistTest=(%f, %f, %f)----------", m_pHost->GetCharacterID(), vDistTest.x, vDistTest.y, vDistTest.z );
		//---------------------------------------------------------------------
		//---------------------------------------------------------------------
	}


	if (!vCurDir.IsZero())
	{
		m_pHost->SetDirAndUp(vCurDir, g_vAxisY);
	}
	
	//
	if (m_pHost->m_MoveCtrl.MoveBlocked() >= 3 /*|| Magnitude(vCurPos - m_pHost->GetPos()) < 0.00001f*/)	//block了，反弹？重新计算
	{
		//Finish();
//		m_vCurrentVelocity = -m_vCurrentVelocity;
		return true;
	}
	else
	{
		m_pHost->SetPos(vCurPos);
		//m_pHost->m_vVelocity = m_vCurrentVelocity;
		m_pHost->m_MoveCtrl.SendSpecialMoveCmd( m_vCurrentVelocity, m_vAcceleration, vCurPos, bNoDelay );
	}
	//
	return true;
}

//	Finish work
void CECHPWorkSpecialMove::Finish()
{
	m_bFinished = true;

/*	A3DVECTOR3 vDir = m_pHost->GetDir();
	vDir.y = 0;
	vDir.Normalize();
	if (!vDir.IsZero())
	{
		m_pHost->SetDirAndUp(vDir, g_vAxisY);
	}*/
	
	m_pHost->m_bSpecialMoving = false;	//Reset the special moving flag to false;

	m_pHost->m_vVelocity.Clear();

	m_pHost->m_vAccel.Clear();	//Reset the current acceleration of the host player. --Disgusting data synchronization.

	//重置HostMove中的变量值，因为在移动时，使用了HostMove中的GroundMove等函数接口，否则，在移动时可能会出现错误
	m_pHost->m_MoveCtrl.ResetMoveTime();

#ifdef CDR_HL2_MOVE
	m_pHost->m_pCDRInfo->vClipVel.x = 0;
	m_pHost->m_pCDRInfo->vClipVel.z = 0;
#endif
}

//	Work is cancel
void CECHPWorkSpecialMove::Cancel()
{
	A3DVECTOR3 vDir = m_pHost->GetDir();
	vDir.y = 0;
	vDir.Normalize();
	if (!vDir.IsZero())
	{
		m_pHost->SetDirAndUp(vDir, g_vAxisY);
	}
}





