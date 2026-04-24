/*
 * FILE: EC_HPWorkTrace.cpp
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
#include "EC_HPWorkTrace.h"
#include "EC_HostPlayer.h"
#include "EC_Game.h"
#include "EC_GameRun.h"
#include "EC_World.h"
#include "EC_ManNPC.h"
#include "EC_NPC.h"
#include "EC_Matter.h"
#include "EC_Interactor.h"
#include "EC_ManMatter.h"
#include "EC_ElsePlayer.h"
#include "EC_GameSession.h"
#include "EC_ManPlayer.h"
#include "EC_Skill.h"
#include "EC_Model.h"
#include "EC_FixedMsg.h"
#include "EC_Inventory.h"
#include "EC_CDR.h"
#include "elementdataman.h"
#include "EC_GameRecord.h"
#include "EC_UIManager.h"
#include "EC_GameUIMan.h"
#include "DlgSkillSpeOperate.h"

#include "A3DFuncs.h"

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


///////////////////////////////////////////////////////////////////////////
//	
//	Local functions
//	
///////////////////////////////////////////////////////////////////////////


///////////////////////////////////////////////////////////////////////////
//	
//	Implement CECHPWorkTrace
//	
///////////////////////////////////////////////////////////////////////////

CECHPWorkTrace::CECHPWorkTrace(CECHPWorkMan* pWorkMan) :
CECHPWork(WORK_TRACEOBJECT, pWorkMan)
{
	m_dwMask		= MASK_TRACEOBJECT;
	m_dwTransMask	= MASK_STAND | MASK_MOVETOPOS | MASK_FLYOFF | MASK_FREEFALL |
						MASK_FOLLOW | MASK_USEITEM;

	CECHPWorkTrace::Reset();
}

CECHPWorkTrace::~CECHPWorkTrace()
{
}

//	Reset work
void CECHPWorkTrace::Reset()
{
	CECHPWork::Reset();

	m_idTarget		= 0;
	m_bHaveMoved	= false;
	m_bMeetSlide	= false;
	m_iReason		= TRACE_NONE;
	m_bCheckTouch	= true;
	m_bReadyCancel	= false;
	m_bMoreClose	= false;
	m_pPrepSkill	= NULL;
	m_bForceAttack	= false;
}

//	Copy work data
bool CECHPWorkTrace::CopyData(CECHPWork* pWork)
{
	if (!CECHPWork::CopyData(pWork))
		return false;

	CECHPWorkTrace* pSrc = (CECHPWorkTrace*)pWork;

	m_idTarget		= pSrc->m_idTarget;
	m_bHaveMoved	= pSrc->m_bHaveMoved;
	m_bMeetSlide	= pSrc->m_bMeetSlide;
	m_iReason		= pSrc->m_iReason;
	m_bCheckTouch	= pSrc->m_bCheckTouch;
	m_bReadyCancel	= pSrc->m_bReadyCancel;
	m_bMoreClose	= pSrc->m_bMoreClose;
	m_vCurDirH		= pSrc->m_vCurDirH;
	m_vTargetPos	= pSrc->m_vTargetPos;
	m_pPrepSkill	= pSrc->m_pPrepSkill;
	m_bForceAttack	= pSrc->m_bForceAttack;

	return true;
}

//	On first tick
void CECHPWorkTrace::OnFirstTick()
{
	m_pHost->m_iMoveMode = CECPlayer::MOVE_MOVE;
	m_bHaveMoved = false;

	if (!m_idTarget || m_idTarget == m_pHost->GetCharacterID())
	{
		m_pHost->m_pPrepSkill = m_pPrepSkill;
		TouchedTarget(NULL);
	}
}

//	Change trace target
void CECHPWorkTrace::ChangeTarget(int idTarget, int iReason)
{
	if (!idTarget || idTarget == m_pHost->GetCharacterID())
	{
		//	This is special case
		m_idTarget	= idTarget;
		m_iReason	= iReason;
		return;
	}
	
	CECObject* pObject = g_pGame->GetGameRun()->GetWorld()->GetObject(idTarget, 0);
	if (!pObject)
		return;

	if (ISPLAYERID(idTarget))
	{
		ASSERT(pObject->GetClassID() == CECObject::OCID_ELSEPLAYER);
		m_vTargetPos = ((CECElsePlayer*)pObject)->GetPos();
	}
	else if (ISNPCID(idTarget))
		m_vTargetPos = ((CECNPC*)pObject)->GetPos();
	else if (ISMATTERID(idTarget))
		m_vTargetPos = ((CECMatter*)pObject)->GetPos();
	else
	{
		ASSERT(0);
		return;
	}

	A3DVECTOR3 vDirH = m_vTargetPos - m_pHost->GetPos();
	vDirH.y = 0.0f;
	vDirH.Normalize();
	if (!vDirH.IsZero())
		m_vCurDirH = vDirH;
	else
		m_vCurDirH = g_vAxisZ;

	m_idTarget	= idTarget;
	m_iReason	= iReason;
}

//	Stop move when touch target
void CECHPWorkTrace::StopMove(bool bFinish)
{
	//	If 'trace' work was transfered from a work which 
	//	needs moving (such as 'move to' work) and we touch the target 
	//	immediately (m_bHaveMoved = false), we must need to send 'stop move'
	//	command
	if (m_bHaveMoved || !m_pHost->m_MoveCtrl.IsStop())
		m_pHost->m_MoveCtrl.SendStopMoveCmd();

	m_pHost->m_vVelocity.Clear();

	if (bFinish)
		m_bFinished = true;
}

//	Tick routine
bool CECHPWorkTrace::Tick(DWORD dwDeltaTime)
{
	CECHPWork::Tick(dwDeltaTime);

	if (!m_bReadyCancel)
	{
		//	Check prepare skill
		CheckPrepSkill();
	}
	
	//	m_bFinished flag may be set both in OnFirstTick() and CheckPrepSkill(),
	//	so check it here !
	if (m_bFinished)
		return true;

	float fDeltaTime = dwDeltaTime * 0.001f;

	if (m_idTarget && m_bCheckTouch)
	{
		CECObject* pObject = g_pGame->GetGameRun()->GetWorld()->GetObject(m_idTarget, 0);
		if (!pObject)
		{
			//	Taget has missed ?
			m_idTarget = 0;

			if (m_iReason == TRACE_SPELL)
				m_pHost->m_pPrepSkill = NULL;

			return true;
		}

		int iTouchReason = 0;
		float fMaxCut = m_bMoreClose ? -1.0f : 1.0f;

		if (m_iReason == TRACE_ATTACK)
			iTouchReason = 1;
		else if (m_iReason == TRACE_SPELL)
			iTouchReason = 2;
		else if (m_iReason == TRACE_TALK)
			iTouchReason = 3;
		else if (m_iReason == TRACE_INTERACT)
			iTouchReason = 3;

		if (ISPLAYERID(m_idTarget))
		{
			CECElsePlayer* pPlayer = (CECElsePlayer*)pObject;
			const A3DVECTOR3& vPlayerPos = pPlayer->GetServerPos();

			if (pPlayer->IsDead() && OnTargetDied(pPlayer))
				return true;

			if (m_iReason == TRACE_TALK)
			{
				if (IsGoodTimeToTouch() && 
					m_pHost->CanTouchTarget(vPlayerPos, 0.0f, iTouchReason, fMaxCut))
				{
					TouchedTarget(pPlayer);
					return true;
				}
			}
			else	//	Check whether it enters our attack area
			{
				if (IsGoodTimeToTouch() && 
					m_pHost->CanTouchTarget(vPlayerPos, pPlayer->GetTouchRadius(), iTouchReason, fMaxCut))
				{
					TouchedTarget(pPlayer);
					return true;
				}
			}

			//	Trace the player
			m_vTargetPos = vPlayerPos;
		}
		else if (ISNPCID(m_idTarget))
		{
			CECNPC* pNPC = (CECNPC*)pObject;
			const A3DVECTOR3& vNPCPos = pNPC->GetServerPos();

			if (pNPC->IsDead() && OnTargetDied(pNPC))
				return true;

			//	Get NPC's position, check whether it enters touch radius
			if (IsGoodTimeToTouch() &&
				m_pHost->CanTouchTarget(vNPCPos, pNPC->GetTouchRadius(), iTouchReason, fMaxCut))
			{
				TouchedTarget(pNPC);
				return true;
			}

			//	Trace the NPC
			m_vTargetPos = vNPCPos;
		}
		else if (ISMATTERID(m_idTarget))
		{
			CECMatter* pMatter = (CECMatter*)pObject;

			//	Get distance between matter and host player
			if (IsGoodTimeToTouch() &&
				pMatter->CalcDist(m_pHost->GetPos(), true) < 3.0f)
			{
				TouchedTarget(pMatter);
				return true;
			}

			//	Trace the matter
			m_vTargetPos = pMatter->GetPos();
		}
		else
		{
			ASSERT(0);
			return true;
		}
	}
	else if (!m_idTarget)	//	Miss target, finish work
	{
		StopMove(true);
		return true;
	}

	m_bCheckTouch = true;

	if (!m_pHost->IsRooting())
	{
		//	Continue tracing object
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

			Trace_Walk(fDeltaTime);
		}
		else	//	(m_pHost->m_iMoveEnv == CECPlayer::MOVEENV_AIR || m_pHost->m_iMoveEnv == CECPlayer::MOVEENV_WATER)
		{
			m_pHost->ResetJump();

			//	Play appropriate actions
			int iAction = m_pHost->GetMoveStandAction(true);
			m_pHost->PlayAction(iAction, 1.0f, false);

			Trace_FlySwim(fDeltaTime);
		}

		m_bHaveMoved = true;
	}

	return true;
}

//	Touched target
void CECHPWorkTrace::TouchedTarget(CECObject* pObject)
{
	if (!m_idTarget || ISPLAYERID(m_idTarget))
	{
		//	Stop moving before attack ask is sent
		StopMove(true);

		if (!m_idTarget || m_idTarget == m_pHost->GetCharacterID())
		{
			//	Handle special case
			ASSERT(m_iReason == TRACE_SPELL);
			if (!m_pHost->CannotAttack())
				m_pHost->CastSkill(m_idTarget, m_bForceAttack, pObject);
			else
				m_pHost->m_pPrepSkill = NULL;
	
			return;
		}

		if (m_iReason == TRACE_ATTACK)
		{
			if (m_idTarget == m_pHost->m_idSelTarget && 
				m_pHost->AttackableJudge(m_idTarget, m_bForceAttack) == 1)
			{
				BYTE byPVPMask = m_pHost->BuildPVPMask(m_bForceAttack);
				g_pGame->GetGameSession()->c2s_CmdNormalAttack(byPVPMask);
				m_pHost->m_bFight = true;
				m_pHost->m_bAboutToFight = true;
			}
		}
		else if (m_iReason == TRACE_SPELL)
		{
			if (!m_pHost->CastSkill(m_idTarget, m_bForceAttack, pObject))
				m_pHost->m_pPrepSkill = NULL;
		}
		else if (m_iReason == TRACE_TALK)
		{
			//	Visite other's booth, send hello message to him
			g_pGame->GetGameSession()->c2s_CmdNPCSevHello(m_idTarget);
		}
	}
	else if (ISNPCID(m_idTarget))
	{
		//	Stop moving before attack ask is sent
		StopMove(true);

		CECNPC* pNPC = (CECNPC*)pObject;

		if (m_iReason == TRACE_TALK)
		{
			if (!m_pHost->IsInBattle() || m_pHost->InSameBattleCamp(pNPC))
				g_pGame->GetGameSession()->c2s_CmdNPCSevHello(m_idTarget);
		}
		else if (m_iReason == TRACE_ATTACK)
		{
			if (m_idTarget == m_pHost->m_idSelTarget && 
				m_pHost->AttackableJudge(m_idTarget, m_bForceAttack) == 1)
			{
				BYTE byPVPMask = m_pHost->BuildPVPMask(m_bForceAttack);
				g_pGame->GetGameSession()->c2s_CmdNormalAttack(byPVPMask);
				m_pHost->m_bFight = true;
				m_pHost->m_bAboutToFight = true;
			}
		}
		else if (m_iReason == TRACE_SPELL)
		{
			if (!m_pHost->CannotAttack())
			{
				m_pHost->CastSkill(m_idTarget, m_bForceAttack);
				m_pHost->m_bFight = true;
			}
			else
				m_pHost->m_pPrepSkill = NULL;
		}
		else if(m_iReason == TRACE_TELEPORT)
		{	
			CECGameUIMan* pGameUI = g_pGame->GetGameRun()->GetUIManager()->GetInGameUIMan();
			if(!pGameUI || !pNPC)
				return;
			if(m_pHost->IsTeamMember(pNPC->GetSummonerID()) || pNPC->GetSummonerID() == m_pHost->GetPlayerInfo().cid) 
			{
				 if(m_pHost->GetCoolTime(COOLDOWN_INDEX_TELEPORT1) > 0)
				 {
					pGameUI->AddChatMessage(pGameUI->GetStringFromTable(21711), GP_CHAT_MISC);
				 }
				 else if(pNPC->GetTeleportNum() != 2)
				 {
					pGameUI->AddChatMessage(pGameUI->GetStringFromTable(21712), GP_CHAT_MISC);
				 }
				 else
				 {
					 CDlgSkillTeleport* pDlgSkillTeleport = (CDlgSkillTeleport*)pGameUI->GetDialog("Win_Prof13_Skill_Transfer");
					 if(pDlgSkillTeleport)
					 {
						 pDlgSkillTeleport->SetNpcId(m_idTarget);
						 pDlgSkillTeleport->Show(true);
					 }
				 }
			}
		}
		else if(m_iReason == TRACE_INTERACT && pNPC->IsInteractor())
		{
			CECInteractor* pObj = (CECInteractor *) pNPC;
			g_pGame->GetGameSession()->c2s_CmdInteractWithObj(m_idTarget, pObj->GetDBEssence()->require_task_id, pObj->GetDBEssence()->require_item_id);
		}
	}
	else if (ISMATTERID(m_idTarget))
	{
		StopMove(true);

		CECMatter* pMatter = (CECMatter*)pObject;

		if (m_iReason == TRACE_PICKUP)
		{
			//	Check whether we have enougth place to hold this item or money
			if (m_pHost->CanTakeItem(pMatter->GetTemplateID(), 1))
			{
				//	Send pickup asking and wait response command
				g_pGame->GetGameSession()->c2s_CmdPickup(m_idTarget, pMatter->GetTemplateID());
			}
			else
			{
				//	Print a notify message
				g_pGame->GetGameRun()->AddFixedMessage(FIXMSG_PACKISFULL);
			}
		}
		else	//	m_iReason == TRACE_GATHER
		{
			int tidMatter = pMatter->GetTemplateID();
			int stateid = pMatter->GetMineStateID();

			//	Check mine level requirement
			if (m_pHost->GetBasicProps().iLevel < pMatter->GetLevelReq())
			{
				g_pGame->GetGameRun()->AddFixedMessage(FIXMSG_LEVELTOOLOW);
				return;
			}

			//	Check whether we have a mine tool
			int iPack, iIndex, idTool;
			if (m_pHost->FindMineTool(tidMatter, &iPack, &iIndex, &idTool, stateid))
			{
				DATA_TYPE DataType;
				const void* pDataPtr = g_pGame->GetElementDataMan()->get_data_ptr(pMatter->GetTemplateID(), ID_SPACE_ESSENCE, DataType);

				const MINE_ESSENCE* pData = NULL;
				if (DataType == DT_MINE_ESSENCE)
				{
					pData = (const MINE_ESSENCE*)pDataPtr;
				}
				else if (DataType == DT_COMPOUND_MINE_ESSENCE)
				{
					DATA_TYPE MineDataType;
					const MINE_ESSENCE* pMineData = (const MINE_ESSENCE*) g_pGame->GetElementDataMan()->get_data_ptr(stateid, ID_SPACE_ESSENCE, MineDataType);
					if(MineDataType == DT_MINE_ESSENCE)
						pData = pMineData;
				}

				if (!pData)
				{
					ASSERT(false);
					return;
				}

				//	Send gather asking and wait response command
				g_pGame->GetGameSession()->c2s_CmdGatherMaterial(m_idTarget, iPack, iIndex, idTool, pData->task_in);
			}
			else
			{
				g_pGame->GetGameRun()->AddFixedMessage(FIXMSG_NEEDTOOL);
			}
		}
	}
}

//	Trace on ground
bool CECHPWorkTrace::Trace_Walk(float fDeltaTime)
{
	A3DVECTOR3 vCurPos = m_pHost->GetPos();
	CDR_INFO& cdr = *m_pHost->m_pCDRInfo;

	if (m_pHost->m_iMoveMode == CECPlayer::MOVE_SLIDE)
	{
		m_pHost->PlayAction(CECPlayer::ACT_JUMP_LOOP, 1.0f, false);

		//	This will cause stop moming after we slide down.
		A3DVECTOR3 vDir = m_vTargetPos - vCurPos;
		vDir.y = 0;
		vDir.Normalize();
		
		float fMaxSpeedV;
		m_bMeetSlide = m_pHost->m_MoveCtrl.MeetSlope(vDir, fMaxSpeedV);
#ifdef CDR_HL2_MOVE
	a_ClampFloor(cdr.vClipVel.y, -fMaxSpeedV);
#else
	a_ClampFloor(cdr.fYVel, -fMaxSpeedV);
#endif
		

		if (!vDir.IsZero())
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

		if (m_pHost->m_GndInfo.bOnGround)
		{
			if (m_bReadyCancel)
			{
				StopMove(true);
				return true;
			}

			//	Ajust direction only when player on ground
			A3DVECTOR3 vDirH = m_vTargetPos - vCurPos;
			A3DVECTOR3 v = a3d_Normalize(vDirH);
			if (fabs(v.y) > 0.9848f)
			{
				PressCancel();
				return true;
			}

			vDirH.y = 0.0f;
			vDirH.Normalize();
			if (!vDirH.IsZero())
				m_vCurDirH = vDirH;
		}

		A3DVECTOR3 vPrevPos = m_pHost->GetPos(); //Added 2011-09-05.专用于焚香火焰囚牢技能，在检查前先保留上一次位置
		vCurPos = m_pHost->m_MoveCtrl.GroundMove(m_vCurDirH, m_pHost->GetGroundSpeed(), fDeltaTime, m_pHost->m_fVertSpeed);
		m_pHost->SetPos(vCurPos);

	//	if (cdr.vTPNormal.IsZero())
		if (!cdr.bOnSurface)
			m_bCheckTouch = false;

		if (!m_vCurDirH.IsZero())
			m_pHost->SetDirAndUp(m_vCurDirH, g_vAxisY);

		// 由于回放时会被误判为Blocked状态，所以忽略此判断
		if (m_pHost->m_MoveCtrl.MoveBlocked() >= 3 && gr.get_state() != game_record::enum_gr_replay)
		{
		//	m_pHost->m_MoveCtrl.SendStopMoveCmd(vCurPos, m_pHost->GetGroundSpeed(), iMoveMode);
			PressCancel();
		}
		//Added and Modified 2011-09-05.对于囚牢技能，由于并不对囚牢进行碰撞检测，因此需要额外执行范围检测。
		else if ( m_pHost->IsNeedCheckCurPosValid() && !m_pHost->CheckCurrentPosValid(vCurPos) )
		{
			PressCancel();
//			m_pHost->m_MoveCtrl.SendStopMoveCmd(vPrevPos, m_pHost->GetGroundSpeed(), iMoveMode);
			m_pHost->SetPos(vPrevPos);
		}
		else
			m_pHost->m_MoveCtrl.SendMoveCmd(vCurPos, 0, m_vTargetPos, cdr.vAbsVelocity, iMoveMode);
	}
	else	//	m_bMeetSlide == true
	{
		if (m_bHaveMoved)
			m_pHost->m_MoveCtrl.SendStopMoveCmd(vCurPos, m_pHost->GetGroundSpeed(), GP_MOVE_SLIDE);

		m_bFinished = true;
	}

	return true;
}

//	Trace in air and water
bool CECHPWorkTrace::Trace_FlySwim(float fDeltaTime)
{
	if (m_bMeetSlide || m_bReadyCancel)
	{
		StopMove(true);
		return true;
	}

	A3DVECTOR3 vCurPos = m_pHost->GetPos();

	float fSpeed1 = m_pHost->m_vVelocity.Magnitude();
	A3DVECTOR3 vMoveDir = m_vTargetPos - vCurPos;
	float fDist = vMoveDir.Normalize();
	int iMoveMode;

	float na=0.0f, pa=0.0f, fMaxSpeed;
	bool bInAir;
	if (m_pHost->m_iMoveEnv == CECPlayer::MOVEENV_AIR)
	{
		bInAir = true;
		na = EC_NACCE_AIR;
		fMaxSpeed = m_pHost->GetFlySpeed();
		iMoveMode = GP_MOVE_AIR | GP_MOVE_RUN;
	}
	else
	{
		bInAir = false;
		na = EC_NACCE_WATER;
		fMaxSpeed = m_pHost->GetSwimSpeed();
		iMoveMode = GP_MOVE_WATER | GP_MOVE_RUN;
	}

	ON_AIR_CDR_INFO& cdr = *m_pHost->m_pAirCDRInfo;

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

	A3DVECTOR3 vVel2 = vMoveDir * fSpeed2;
	vCurPos = m_pHost->m_MoveCtrl.AirWaterMove(vMoveDir, fSpeed2, fDeltaTime, bInAir, true);

	m_pHost->SetPos(vCurPos);
	m_pHost->m_vVelocity = vVel2;

	if (fabs(vMoveDir.y) > 0.9848f)
	{
		A3DVECTOR3 v1 = vMoveDir;
		A3DVECTOR3 v2 = m_vTargetPos - vCurPos;
		v1.y = v2.y = 0.0f;
		if (DotProduct(v1, v2) < 0.0f)
		{
			PressCancel();
			return true;
		}
	}
	
	if (m_pHost->m_MoveCtrl.MoveBlocked() >= 3)
	{
		m_pHost->m_MoveCtrl.SendStopMoveCmd(vCurPos, fMaxSpeed, iMoveMode);
		m_bFinished = true;
	}
	else
		m_pHost->m_MoveCtrl.SendMoveCmd(vCurPos, 1, m_vTargetPos, vVel2, iMoveMode);

	return true;
}

//	Handle the case that target died when host is tracing it
//	Return true if host should stop tracing
bool CECHPWorkTrace::OnTargetDied(CECObject* pTarget)
{
	bool bRet = false;

	if (m_iReason == TRACE_ATTACK)
		bRet = true;
	else if (m_iReason == TRACE_SPELL)
	{
		CECSkill* pSkill = m_pHost->m_pPrepSkill;
		if (!pSkill || pSkill->GetTargetType() != 2)
		{
			bRet = true;
			m_pHost->m_pPrepSkill = NULL;
		}
	}

	if (bRet)
		StopMove(true);

	return bRet;
}

//	Is valid time to touch target ?
bool CECHPWorkTrace::IsGoodTimeToTouch()
{
	//	When jumping, host can do nothing
	if (m_pHost->IsJumping())
		return false;

	if (m_iReason == TRACE_ATTACK)
	{
	}
	else if (m_iReason == TRACE_SPELL)
	{
	}
	else
	{
	}
	
	return true;
}

//	User press cancel button
void CECHPWorkTrace::PressCancel()
{
	m_bReadyCancel = true; 

	if (m_iReason == TRACE_SPELL)
		m_pHost->m_pPrepSkill = NULL;
}

//	Check prepare skill
void CECHPWorkTrace::CheckPrepSkill()
{
	if (m_iReason == TRACE_SPELL)
	{
		ASSERT(m_pPrepSkill);
		m_pHost->m_pPrepSkill = m_pPrepSkill;
	}
	else if (m_pHost->m_pPrepSkill)
		m_pHost->m_pPrepSkill = NULL;
}

//	On work shift
void CECHPWorkTrace::Cancel()
{
	if (m_pHost->m_pPrepSkill && m_iReason == TRACE_SPELL)
		m_pHost->m_pPrepSkill = NULL;
}

bool CECHPWorkTrace::gen_record_msg(ECMSG& msg)
{
	msg.dwMsg = MSG_HST_TRACE_RECORD;
	msg.iManager = MAN_PLAYER;
	msg.iSubID = 0;
	msg.dwParam1 = m_idTarget;
	msg.dwParam2 = m_iReason;
	msg.dwParam3 = m_bForceAttack;
	msg.dwParam4 = 0;

	if (m_iReason == TRACE_SPELL && m_pPrepSkill)
		msg.dwParam4 = m_pPrepSkill->GetSkillID();
	else
		msg.dwParam4 = m_bMoreClose;

	return true;
}
