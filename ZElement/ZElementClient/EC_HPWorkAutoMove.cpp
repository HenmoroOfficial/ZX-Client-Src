/********************************************************************
	created:	2007/04/03
	author:		kuiwu
	
	purpose:	host player automove directed by a given path
	Copyright (C) 2007 - All Rights Reserved
*********************************************************************/

#include "EC_HPWorkAutoMove.h"

#include "AutoMove\MoveAgent.h"
#include "EC_HostPlayer.h"
#include "EC_Model.h"
#include "EC_CDR.h"
#include "EC_GameRecord.h"
#include "EC_Game.h"
#include "EC_FixedMsg.h"
#include "EC_GameRun.h"
#include "EC_AutoMove.h"
#include "EC_World.h"
#include "EC_ManNPC.h"
#include "EC_GameSession.h"
#include "EC_NPC.h"

#include "A3DFuncs.h"

///////////////////////////////////////////////////////////////////////////
//	
//	Define and Macro
//	
///////////////////////////////////////////////////////////////////////////
#define new A_DEBUG_NEW

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

ACString g_AutoMoveDestName = _AL("");
///////////////////////////////////////////////////////////////////////////
//	
//	Local functions
//	
///////////////////////////////////////////////////////////////////////////


///////////////////////////////////////////////////////////////////////////
//	
//	Implement CECHPWorkAutoMove
//	
///////////////////////////////////////////////////////////////////////////

CECHPWorkAutoMove::CECHPWorkAutoMove(CECHPWorkMan* pWorkMan) :
CECHPWork(WORK_AUTOMOVE, pWorkMan)
{
	m_dwMask		= MASK_AUTOMOVE;
	m_dwTransMask	= MASK_STAND | MASK_MOVETOPOS | MASK_TRACEOBJECT | MASK_AUTOMOVE| MASK_FOLLOW;

	m_Counter.SetPeriod(5000);
	CECHPWorkAutoMove::Reset();
}

CECHPWorkAutoMove::~CECHPWorkAutoMove()
{
}

//	Reset work
void CECHPWorkAutoMove::Reset()
{
	CECHPWork::Reset();

	m_bReach		= false;
	//m_bMeetSlide	= false;
	//m_PathIndex		= 1; // avoid turning back
	
	m_PathIndex = g_ECAutoMove.MoveOn(0);
	m_Counter.Reset();
	g_AutoMoveDestName = _AL("");
}

//	Copy work data
bool CECHPWorkAutoMove::CopyData(CECHPWork* pWork)
{
	if (!CECHPWork::CopyData(pWork))
		return false;

	CECHPWorkAutoMove* pSrc = (CECHPWorkAutoMove*)pWork;

	m_bReach	= pSrc->m_bReach;
	//m_bMeetSlide	= pSrc->m_bMeetSlide;
	m_vDestPos		= pSrc->m_vDestPos;
	m_vCurDirH		= pSrc->m_vCurDirH;
	m_PathIndex		= pSrc->m_PathIndex;

	return true;
}


//	On first tick
void CECHPWorkAutoMove::OnFirstTick()
{
	if (m_pHost->m_iMoveEnv != CECPlayer::MOVEENV_AIR)
		m_pHost->ShowWing(false);
}


//	Stop move
void CECHPWorkAutoMove::StopMove(const A3DVECTOR3& vPos, float fSpeed, int iMoveMode)
{
	m_pHost->m_iMoveMode = CECHostPlayer::MOVE_STAND;

	//	Play appropriate actions
	m_pHost->PlayAction(m_pHost->GetMoveStandAction(false), 1.0f, false);

	m_pHost->m_MoveCtrl.SendStopMoveCmd(vPos, fSpeed, iMoveMode);
}


void CECHPWorkAutoMove::Cancel()
{

	g_ECAutoMove.ClearPath(!m_bReach);
	int iMsg = m_bReach? (FIXMSG_AUTOMOVE_END):(FIXMSG_AUTOMOVE_CANCEL);
	g_pGame->GetGameRun()->AddFixedMessage(iMsg);
	
	if (!g_AutoMoveDestName.IsEmpty())
	{
		CECNPCMan* pNPCMan = g_pGame->GetGameRun()->GetWorld()->GetNPCMan();
		int idTarget;
		
		if (idTarget = pNPCMan->GetNPCIDbyName(g_AutoMoveDestName))//如果目标点是一个ServerNPC，尝试打开NPC对话框
		{
			CECNPC* pNPC = pNPCMan->GetNPC(idTarget);
			if (!pNPC || !pNPC->IsServerNPC())
				return;
			if (m_pHost->CanTouchTarget(pNPC->GetPos(), pNPC->GetTouchRadius(), 3))
				g_pGame->GetGameSession()->c2s_CmdNPCSevHello(idTarget);	
		}	
	}
	
	Reset();
}

//	Tick routine
bool CECHPWorkAutoMove::Tick(DWORD dwDeltaTime)
{
	CECHPWork::Tick(dwDeltaTime);

	//assume move agent  has searched and updated path elsewhere.
	if (m_PathIndex >= (int)g_AutoMoveAgent.GetPathCount())
	{
		m_bFinished = true;
		m_bReach  = true;
		int iMoveMode = m_pHost->m_bWalkRun ? GP_MOVE_RUN : GP_MOVE_WALK;		
		m_pHost->m_MoveCtrl.SendStopMoveCmd(m_pHost->GetPos(), m_pHost->GetGroundSpeed(), iMoveMode);
		
		return true;
	}


	m_vDestPos = g_AutoMoveAgent.Get3DPathNode(m_PathIndex);

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

		Follow_Walk(dwDeltaTime);
	}
	else
	{
		m_pHost->ResetJump();

		//	Play appropriate actions
		int iAction = m_pHost->GetMoveStandAction(true);
		m_pHost->PlayAction(iAction, 1.0f, false);

		Follow_FlySwim(dwDeltaTime);
	}
	return true;
}

//#define  LOCAL_DEGREE    30
//#define  LOCAL_DEGREE    15
//#define  LOCAL_DEGREE    10
#define  LOCAL_DEGREE    5

int CECHPWorkAutoMove::_GetNextPathIndex(int curNext)
{

	if (curNext >= (int)g_AutoMoveAgent.GetPathCount())
	{
		return curNext;
	}


	
	//return curNext+1; //test


	int max_try = g_AutoMoveAgent.GetOptimizeCatchCount();
	
	A3DVECTOR3 dir;
	A3DVECTOR3 cur_pos = m_pHost->GetPos();
	int  next_index = curNext;
	A3DVECTOR3 next_pos = g_AutoMoveAgent.Get3DPathNode(next_index);
	dir = next_pos - cur_pos;
	dir.y = 0.0f;
	dir.Normalize();

	int  i = 1;
	next_index++;

	float threshold = (float)cos(LOCAL_DEGREE*A3D_PI/180.0);

	while (i < max_try && next_index < (int)g_AutoMoveAgent.GetPathCount())
	{
		next_pos = g_AutoMoveAgent.Get3DPathNode(next_index);
		A3DVECTOR3   tmp_dir(next_pos - cur_pos);
		tmp_dir.y = 0.0f;
		tmp_dir.Normalize();

		float dtp = DotProduct(tmp_dir, dir);
		if (dtp < threshold)
		{
			//fail
			break;
			
		}
		i++;
		next_index++;
	}

	return next_index -1;

}


bool CECHPWorkAutoMove::_CheckBlock(DWORD dwDeltaTime)
{
	bool blocked = false;
	if (m_pHost->m_MoveCtrl.MoveBlocked() >=3 )
	{
		blocked = m_Counter.IncCounter(dwDeltaTime);
	}
	else
	{
		m_Counter.Reset();
		
	}

	return blocked;
}

//	Follow on ground
bool CECHPWorkAutoMove::Follow_Walk(DWORD dwDeltaTime)
{
	float fDeltaTime = dwDeltaTime * 0.001f;

	A3DVECTOR3 vCurPos = m_pHost->GetPos();
	CDR_INFO& cdr = *m_pHost->m_pCDRInfo;

	if (m_pHost->m_iMoveMode == CECPlayer::MOVE_SLIDE)
	{
		m_pHost->PlayAction(CECPlayer::ACT_JUMP_LOOP, 1.0f, false);

		A3DVECTOR3 vDir = m_vDestPos - vCurPos;
		vDir.y = 0;
		vDir.Normalize();
		
		//float fMaxSpeedV;
		//m_bMeetSlide = m_pHost->m_MoveCtrl.MeetSlope(vDir, fMaxSpeedV);
		//a_ClampFloor(cdr.vClipVel.y, -fMaxSpeedV);	
		m_vCurDirH = vDir;
		vCurPos = m_pHost->m_MoveCtrl.GroundMove(m_vCurDirH, m_pHost->GetGroundSpeed(), fDeltaTime);


		if (_CheckBlock(dwDeltaTime))
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
	//note: NEED thoroughly test
	//else if (!m_bMeetSlide)
	else
	{
		int iMoveMode = m_pHost->m_bWalkRun ? GP_MOVE_RUN : GP_MOVE_WALK;
		if (m_pHost->IsJumping())
			iMoveMode = GP_MOVE_JUMP;
		else if (!m_pHost->m_GndInfo.bOnGround)
			iMoveMode = GP_MOVE_FALL;

		float fDist, fSpeed = m_pHost->GetGroundSpeed();
		if (m_pHost->m_GndInfo.bOnGround)
		{
			//	Adjust direction only when player on ground
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

		if (_CheckBlock(dwDeltaTime))
		{
			m_bFinished	= true;
			m_pHost->m_MoveCtrl.SendStopMoveCmd(vCurPos, fSpeed, iMoveMode);
		}
		//Added and Modified 2011-07-12.对于囚牢技能，由于并不对囚牢进行碰撞检测，因此需要额外执行范围检测。
		else if ( m_pHost->IsNeedCheckCurPosValid() && !m_pHost->CheckCurrentPosValid(vCurPos) )
		{
			m_bFinished	= true;
			m_pHost->m_MoveCtrl.SendStopMoveCmd(vPrevPos, fSpeed, iMoveMode);
		}
		else
		{
			//	Check whether we have arrived destination
			A3DVECTOR3 vMoveDelta = vCurPos - m_pHost->GetPos();
			vMoveDelta.y = 0.0f;
			float fMoveDist = vMoveDelta.Normalize();

			m_pHost->SetPos(vCurPos);
			m_pHost->m_MoveCtrl.SendMoveCmd(vCurPos, 0, m_vDestPos, cdr.vAbsVelocity, iMoveMode);	

			if (fMoveDist >= fDist)
			{
				//m_PathIndex++;
				//m_PathIndex = g_ECAutoMove.MoveOn(m_PathIndex);
				  
				int nearest_next = g_ECAutoMove.MoveOn(m_PathIndex);
				m_PathIndex = _GetNextPathIndex(nearest_next);

				//m_PathIndex = _GetNextPathIndex(m_PathIndex+1);
			}
		}
	}

	return true;
}

//	Follow in air and water
bool CECHPWorkAutoMove::Follow_FlySwim(DWORD dwDeltaTime)
{
	float fDeltaTime = dwDeltaTime * 0.001f;

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
	if ( (fDistH>s - 0.01f) || m_PathIndex < (int)g_AutoMoveAgent.GetPathCount() )
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
// 	if (fMoveDistH >= fDistH)
// 		vVel2.Clear();

	m_pHost->SetPos(vCurPos);
	m_pHost->m_vVelocity = vVel2;

	m_pHost->m_MoveCtrl.SendMoveCmd(vCurPos, 0, m_vDestPos, vVel2, iMoveMode | GP_MOVE_RUN);
	if (fMoveDistH >= fDistH)
	{
		//m_PathIndex++;
		//m_PathIndex = g_ECAutoMove.MoveOn(m_PathIndex);

		int nearest_next = g_ECAutoMove.MoveOn(m_PathIndex);
		m_PathIndex = _GetNextPathIndex(nearest_next);
		
		//m_PathIndex = _GetNextPathIndex(m_PathIndex+1);
	}	
	
		

	return true;
}

//	Calculate vertical speed when fly or swim
float CECHPWorkAutoMove::CalcFlySwimVertSpeed(float fSpeed1, float fPushDir, float fPushAccel,
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
