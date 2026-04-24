/*
 * FILE: EC_HPWorkMoveBezier.cpp
 *
 * DESCRIPTION: 
 *
 * CREATED BY: Fu Chonggang, 2013/09/16
 *
 * HISTORY: 
 *
 * Copyright (c) 2013 Archosaur Studio, All Rights Reserved.
 */

#include "EC_Global.h"
#include "EC_HPWorkMoveBezier.h"
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
#include "EC_Bezier.h"
#include "EC_SendC2SCmds.h"

#define new A_DEBUG_NEW


CECHPWorkBezierMove::CECHPWorkBezierMove(CECHPWorkMan* pWorkMan) :
CECHPWork(WORK_MOVEBEZIEL, pWorkMan)
{
	m_dwMask		= MASK_MOVEBEZIEL;
	m_dwTransMask	= MASK_STAND;
	
	CECHPWorkBezierMove::Reset();
}

CECHPWorkBezierMove::~CECHPWorkBezierMove()
{
	if(m_pBezierWalker)
		delete m_pBezierWalker;
}

//	Reset work
void CECHPWorkBezierMove::Reset()
{
	m_fSpeed = .0f;
	m_iIdBezier = -1;
	m_pBezierWalker = 0;
	m_iRunState = 0;
	CECHPWork::Reset();

}

//	Copy work data
bool CECHPWorkBezierMove::CopyData(CECHPWork* pWork)
{
	if (!CECHPWork::CopyData(pWork))
	{
		return false;
	}

	CECHPWorkBezierMove* pSrc = (CECHPWorkBezierMove*)pWork;

	m_fSpeed = pSrc->m_fSpeed;
	m_iIdBezier = pSrc->m_iIdBezier;
	m_iRunState = pSrc->m_iRunState;
	return true;
}

void CECHPWorkBezierMove::SetBezierIdAndSpeed(const int iId, const float fSpeed)
{
	m_iIdBezier = iId;
	m_fSpeed = fSpeed;
}

//	On first tick
void CECHPWorkBezierMove::OnFirstTick()
{
	if(!m_pBezierWalker)
	{
		m_pBezierWalker = new CECBezierWalker;
	}

	ChangeBezier(m_iIdBezier);
	m_pHost->ShowWing(true);
}

//	Tick routine
bool CECHPWorkBezierMove::Tick(DWORD dwDeltaTime)
{
	CECHPWork::Tick(dwDeltaTime);
	if(!m_pBezier)
	{
		return false;
	}

	m_pBezierWalker->Tick(dwDeltaTime);
	if(!m_pBezierWalker->IsWalking())
	{
		if(m_pBezier->GetNextGlobalID()>=0)
		{
			ChangeBezier(m_pBezier->GetNextGlobalID());
		}
		else
		{
			Finish();
			return true;
		}
	}

	if(m_iRunState==1)
	{
		int iMaxLoopNum = 9;
		while(iMaxLoopNum>0 && m_pBezier->GetNextGlobalID()>0)
		{
			iMaxLoopNum--;
			ChangeBezier(m_pBezier->GetNextGlobalID());
		}
		iMaxLoopNum = 500;
		m_pBezierWalker->SetSpeed(500);
		while(m_pBezierWalker->IsWalking() && iMaxLoopNum>0)
		{
			iMaxLoopNum--;
			m_pBezierWalker->Tick(500000);
		}
		m_pBezierWalker->SetSpeed(m_fSpeed);
		m_pBezierWalker->StartWalk(false, false);
		m_pBezierWalker->Tick(300);
		m_pBezierWalker->Tick(300);
		m_pBezierWalker->Tick(300);
		m_pBezierWalker->Tick(300);
		m_pBezierWalker->SetForwardFlag(true);
		m_iRunState = 2;
	}

/*	if ( GetAsyncKeyState('9')&0x8000 )//0 && m_pHost->m_iMoveEnv == CECPlayer::MOVEENV_GROUND )
	{
		Finish();
		//	Play appropriate actions
		// m_pHost->PlayAction(CECPlayer::ACT_RUN, 1.0f, false);

		// Tick_Move(fDeltaTime);
	}
	else*/
	//{
		m_pHost->ResetJump();

		//	Play appropriate actions
		int iAction = m_pHost->GetMoveStandAction(true);
		m_pHost->PlayAction(iAction, 1.0f, false);

		Tick_Move(dwDeltaTime);
	//}
	return true;
}

void CECHPWorkBezierMove::ChangeBezier(const int iId)
{
	m_pBezier = (CECBezier*)g_pGame->GetGameRun()->GetWorld()->GetCutsceneBezier(iId);
	if(!m_pBezier)
	{
		Finish();
		return;
	}
	m_pBezierWalker->BindBezier(m_pBezier); 
	m_pBezierWalker->SetSpeed(m_fSpeed);
	m_pBezierWalker->StartWalk(false, true);
}

//	Tick routine of walking on ground
bool CECHPWorkBezierMove::Tick_Move(DWORD dwDeltaTime)
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
	

	A3DVECTOR3 vDestPos = m_pBezierWalker->GetPos();
	A3DVECTOR3 vVel2 = m_pHost->GetDir();
	vVel2 *= m_fSpeed;
	A3DVECTOR3 vDir = vDestPos - vCurPos;
	
	// 模拟行走，tick时间，实际目标位置采用贝塞尔曲线上的位置，
	m_pHost->m_MoveCtrl.GroundMove(vDir, m_fSpeed, fDeltaTime, m_pHost->m_fVertSpeed);
	
	m_pHost->m_MoveCtrl.SendMoveCmd(vCurPos, 0, vDestPos, vVel2, iMoveMode | GP_MOVE_RUN);

	m_pHost->SetPos(vDestPos);
	m_pHost->SetDirAndUp(vDir, g_vAxisY);
	return true;
}

//	Finish work
void CECHPWorkBezierMove::Finish()
{
	m_bFinished = true;
	A3DVECTOR3 vDestPos = m_pBezierWalker->GetPos();
	m_pHost->SetPos(vDestPos);
	m_pHost->m_MoveCtrl.SendStopMoveCmd(vDestPos, m_fSpeed, GP_MOVE_RUN);
	c2s_SendCmdStopFreeFly();
	//m_pHost->StartLanding();
}

//	Work is cancel
void CECHPWorkBezierMove::Cancel()
{
	A3DVECTOR3 vDir = m_pHost->GetDir();
	vDir.y = 0;
	vDir.Normalize();
	if (!vDir.IsZero())
	{
		m_pHost->SetDirAndUp(vDir, g_vAxisY);
	}
}







