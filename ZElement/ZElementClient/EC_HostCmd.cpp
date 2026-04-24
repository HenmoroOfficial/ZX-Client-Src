/*
 * FILE: EC_HostPlayer.cpp
 *
 * DESCRIPTION: 
 *
 * CREATED BY: Duyuxin, 2004/9/1
 *
 * HISTORY: 
 *
 * Copyright (c) 2004 Archosaur Studio, All Rights Reserved.
 */

#include "EC_Global.h"
#include "EC_HostPlayer.h"
#include "EC_Game.h"
#include "EC_GameRun.h"
#include "EC_Utility.h"
#include "EC_World.h"
#include "EC_GameSession.h"
#include "EC_IvtrTypes.h"
#include "EC_IvtrItem.h"
#include "EC_ElsePlayer.h"
#include "EC_Shortcut.h"
#include "EC_InputCtrl.h"
#include "EC_Inventory.h"
#include "EC_Matter.h"
#include "EC_ManMatter.h"
#include "EC_ManPlayer.h"
#include "EC_Team.h"
#include "EC_GameUIMan.h"
#include "EC_UIManager.h"
#include "EC_FixedMsg.h"

#include "EC_HPWorkStand.h"
#include "EC_HPWorkSit.h"
#include "EC_HPWorkTrace.h"
#include "EC_GameRecord.h"
#include "defence/stackchecker.h"
#include "LuaEvent.h"

#include "A3DMacros.h"

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
//	Implement CECHostPlayer
//	
///////////////////////////////////////////////////////////////////////////

//	Sit down / Stand up
bool CECHostPlayer::CmdSitDown(bool bSitDown)
{
	if (!CanDo(CANDO_SITDOWN))
		return false;

	if (bSitDown)
	{
		g_pGame->GetGameSession()->c2s_CmdSitDown();
		LuaEvent::FirstSitDown();
	}
	else
		g_pGame->GetGameSession()->c2s_CmdStandUp();

/*	if (bSitDown)
	{
		if (m_iMoveEnv == MOVEENV_AIR || m_iMoveEnv == MOVEENV_WATER || IsTrading())
			return false;

		CECHPWork* pCurWork = m_pWorkMan->GetCurWork();
		if (!pCurWork || pCurWork->CanTransferTo(CECHPWork::MASK_SIT))
		{
			CECHPWorkSit* pWork = (CECHPWorkSit*)m_pWorkMan->GetWorkByID(CECHPWork::WORK_SIT, true);
			m_pWorkMan->StartWork_p1(CECHPWork::WORK_SIT);
		}
		else
			return false;
	}
	else
	{
		CECHPWork* pCurWork = m_pWorkMan->GetCurWork();
		if (!pCurWork || pCurWork->GetWorkID() == CECHPWork::WORK_SIT)
		{
			CECHPWorkStand* pWork = (CECHPWorkStand*)m_pWorkMan->GetWorkByID(CECHPWork::WORK_STAND, true);
			m_pWorkMan->StartWork_p1(CECHPWork::WORK_STAND);
		}
		else
			return false;
	}
*/
	return true;
}

//	Switch walk and run state
bool CECHostPlayer::CmdWalkRun(bool bRun)
{
	if (gr.get_state() == game_record::enum_gr_replay)
		return false;

	m_bWalkRun = bRun;

	if (gr.get_state() == game_record::enum_gr_record)
	{
		ECMSG msg;
		msg.dwMsg = MSG_HST_WALK_RECORED;
		msg.iManager = MAN_PLAYER;
		msg.iSubID = 0;
		msg.dwParam1 = bRun;
		msg.dwParam2 = 0;
		msg.dwParam3 = 0;
		msg.dwParam4 = 0;		
		gr.inc_frame()->push_event(new event_record_msg(msg));
	}

	return true;
}

//	Start normal attacking to selected target
bool CECHostPlayer::CmdNormalAttack(bool bMoreClose/* false */, bool bCombo/* false */,
							int idTarget/* 0 */, int iForceAtk/* -1 */)
{
	StackChecker::ACTrace(2);

	if (gr.get_state() == game_record::enum_gr_replay)
		return false;

	// first of all see if we need to cancel sitdown work.
	if (m_pWorkMan->GetCurWorkID() == CECHPWork::WORK_SIT)
	{
		g_pGame->GetGameSession()->c2s_CmdStandUp();
		return false;
	}

	if (!CanDo(CANDO_MELEE))
		return false;

	if (!bCombo)
		ClearComboSkill();

/*	CECHPWork* pP1Work = m_pWorkMan->GetWork_p1();

	//	Trace a object
	if (pP1Work && pP1Work->GetWorkID() == CECHPWork::WORK_TRACEOBJECT)
	{
		CECHPWorkTrace* pWork = (CECHPWorkTrace*)pP1Work;
		pWork->ChangeTarget(m_idSelTarget, CECHPWorkTrace::TRACE_ATTACK);
	}
	else if (pP1Work && pP1Work->GetWorkID() == CECHPWork::WORK_HACKOBJECT)
	{
		if (((CECHPWorkMelee*)pP1Work)->GetTarget() == m_idSelTarget)
			return true;	//	Host is attacking the target

		CECHPWorkTrace* pWork = (CECHPWorkTrace*)m_pWorkMan->GetWorkByID(CECHPWork::WORK_TRACEOBJECT, true);
		pWork->ChangeTarget(m_idSelTarget, CECHPWorkTrace::TRACE_ATTACK);
		m_pWorkMan->StartWork_p1(CECHPWork::WORK_TRACEOBJECT);
	}
	else if (!pP1Work || pP1Work->CanTransferTo(CECHPWork::MASK_TRACEOBJECT))
	{
		CECHPWorkTrace* pWork = (CECHPWorkTrace*)m_pWorkMan->GetWorkByID(CECHPWork::WORK_TRACEOBJECT, true);
		pWork->ChangeTarget(m_idSelTarget, CECHPWorkTrace::TRACE_ATTACK);
		m_pWorkMan->StartWork_p1(CECHPWork::WORK_TRACEOBJECT);
	}
*/
	if (!idTarget)
		idTarget = m_idSelTarget;

	bool bForceAttack;
	if (iForceAtk < 0)
		bForceAttack = GetForceAttackFlag(NULL);
	else
		bForceAttack = iForceAtk > 0 ? true : false;

	if (AttackableJudge(idTarget, bForceAttack) == 1)
	{
		NormalAttackObject(idTarget, bForceAttack, bMoreClose);

		if (gr.get_state() == game_record::enum_gr_record)
		{
			ECMSG msg;
			msg.dwMsg = MSG_HST_TRACE_RECORD;
			msg.iManager = MAN_PLAYER;
			msg.iSubID = 0;
			msg.dwParam1 = idTarget;
			msg.dwParam2 = CECHPWorkTrace::TRACE_ATTACK;
			msg.dwParam3 = bForceAttack;
			msg.dwParam4 = bMoreClose;
			gr.inc_frame()->push_event(new event_record_msg(msg));
		}
	}

	return true;
}

//	Find a near target
bool CECHostPlayer::CmdFindTarget()
{
	return true;
}

//	Select other player's attacked target
bool CECHostPlayer::CmdAssistAttack()
{
	// first of all see if we need to cancel sitdown work.
	if (m_pWorkMan->GetCurWorkID() == CECHPWork::WORK_SIT)
	{
		g_pGame->GetGameSession()->c2s_CmdStandUp();
		return false;
	}

	if (CanDo(CANDO_ASSISTSEL))
		g_pGame->GetGameSession()->c2s_CmdTeamAssistSel(m_idSelTarget);

	return true;
}

//	Start / Stop flying
bool CECHostPlayer::CmdFly()
{
	return true;
}

bool CECHostPlayer::CmdPickup()
{
	if (gr.get_state() == game_record::enum_gr_replay)
		return false;

	// first of all see if we need to cancel sitdown work.
	if (m_pWorkMan->GetCurWorkID() == CECHPWork::WORK_SIT)
	{
		g_pGame->GetGameSession()->c2s_CmdStandUp();
		return false;
	}

	if (!CanDo(CANDO_PICKUP))
		return false;

	//	If host is going to pickup an item already, return directly
	CECHPWork* pP1Work = m_pWorkMan->GetWork_p1();
	if (pP1Work && pP1Work->GetWorkID() == CECHPWork::WORK_TRACEOBJECT)
	{
		if (((CECHPWorkTrace*)pP1Work)->GetTraceReason() == CECHPWorkTrace::TRACE_PICKUP)
			return true;
	}

	//	Find a matter which is near enough to us
	CECMatterMan* pMatterMan = g_pGame->GetGameRun()->GetWorld()->GetMatterMan();
	CECMatter* pMatter = pMatterMan->FindMatterNearHost(10.0f, true);
	if (pMatter)
	{
		// 太虚幻境中，只能拾取对方阵营掉落的物品（系统掉落）
		if( pMatter->IsBattleFlag() )
		{
			if(IsInBattle() )
			{
				if( GetBattleCamp() == GP_BATTLE_CAMP_INVADER )
				{
					if( pMatter->GetBattleFlagState() == 1 )
						return false;
				}
				else if( GetBattleCamp() == GP_BATTLE_CAMP_DEFENDER )
				{
					if( pMatter->GetBattleFlagState() == 2 )
						return false;
				}
			}
		}

		PickupObject(pMatter->GetMatterID(), false);

		if (gr.get_state() == game_record::enum_gr_record)
		{
			ECMSG msg;
			msg.dwMsg = MSG_HST_TRACE_RECORD;
			msg.iManager = MAN_PLAYER;
			msg.iSubID = 0;
			msg.dwParam1 = pMatter->GetMatterID();
			msg.dwParam2 = CECHPWorkTrace::TRACE_PICKUP;
			msg.dwParam3 = 0;
			msg.dwParam4 = 0;
			gr.inc_frame()->push_event(new event_record_msg(msg));
		}
	}

	return true;
}

bool CECHostPlayer::CmdGather()
{
	if (gr.get_state() == game_record::enum_gr_replay)
		return false;

	// first of all see if we need to cancel sitdown work.
	if (m_pWorkMan->GetCurWorkID() == CECHPWork::WORK_SIT)
	{
		g_pGame->GetGameSession()->c2s_CmdStandUp();
		return false;
	}

	if (!CanDo(CANDO_GATHER))
		return false;

	//	If host is going to pickup an item already, return directly
	CECHPWork* pP1Work = m_pWorkMan->GetWork_p1();
	if (pP1Work && pP1Work->GetWorkID() == CECHPWork::WORK_TRACEOBJECT)
	{
		if (((CECHPWorkTrace*)pP1Work)->GetTraceReason() == CECHPWorkTrace::TRACE_GATHER)
			return true;
	}

	PickupObject(m_idSelTarget, true);

	if (gr.get_state() == game_record::enum_gr_record)
	{
		ECMSG msg;
		msg.dwMsg = MSG_HST_TRACE_RECORD;
		msg.iManager = MAN_PLAYER;
		msg.iSubID = 0;
		msg.dwParam1 = m_idSelTarget;
		msg.dwParam2 = CECHPWorkTrace::TRACE_GATHER;
		msg.dwParam3 = 0;
		msg.dwParam4 = 0;
		gr.inc_frame()->push_event(new event_record_msg(msg));
	}

	return true;
}

bool CECHostPlayer::CmdRushFly()
{
	if (m_bAboutToDie || IsDead() || !IsFlying())
		return false;

	g_pGame->GetGameSession()->c2s_CmdActiveRushFly(!m_bRushFly);
	
	return true;
}

bool CECHostPlayer::CmdBindBuddy(int idTarget)
{
	if (!m_BindCmdCoolCnt.IsFull())
	{
		g_pGame->GetGameRun()->AddFixedMessage(FIXMSG_CMD_INCOOLTIME);
		return false;
	}
	
	if (!CanDo(CANDO_BINDBUDDY) || !ISPLAYERID(idTarget) || 
		idTarget == GetCharacterID())
		return false;

	CECElsePlayer* pPlayer = m_pPlayerMan->GetElsePlayer(idTarget);
	if (!pPlayer || pPlayer->GetGender() == GetGender())
		return false;

	A3DVECTOR3 vDist = pPlayer->GetServerPos() - GetPos();
	if (vDist.Magnitude() >= 2.8f)
	{
		g_pGame->GetGameRun()->AddFixedMessage(FIXMSG_TARGETISFAR);
		return false;
	}

	if (GetGender() == GENDER_MALE)
		g_pGame->GetGameSession()->c2s_CmdBindPlayerInvite(idTarget);
	else
		g_pGame->GetGameSession()->c2s_CmdBindPlayerRequest(idTarget);

	//	Start cool time
	m_BindCmdCoolCnt.Reset();

	return true;
}

bool CECHostPlayer::CmdBindQilinBuddy(int idTarget)
{
	if(!CanDo(CANDO_QILINBUDDY) || !ISPLAYERID(idTarget) || idTarget == GetCharacterID())
		return false;

	CECElsePlayer* pPlayer = m_pPlayerMan->GetElsePlayer(idTarget);
	if(!pPlayer)
		return false;

	A3DVECTOR3 vDist = pPlayer->GetServerPos() - GetPos();
	if(vDist.Magnitude() >= 6.0f)
	{
		g_pGame->GetGameRun()->AddFixedMessage(FIXMSG_TARGETISFAR);
		return  true;
	}

	g_pGame->GetGameSession()->c2s_CmdQilinInvite(idTarget);
	
	return true;
}

bool CECHostPlayer::CmdBindMultiBuddy(int idTarget)
{
//	if (!m_BindCmdCoolCnt.IsFull())
//	{
//		g_pGame->GetGameRun()->AddFixedMessage(FIXMSG_CMD_INCOOLTIME);
//		return false;
// 	}
	
	if (!CanDo(CANDO_BINDMULTIBUDDY) || !ISPLAYERID(idTarget) || 
		idTarget == GetCharacterID())
		return false;

	CECElsePlayer* pPlayer = m_pPlayerMan->GetElsePlayer(idTarget);
	if (!pPlayer)
		return false;

	A3DVECTOR3 vDist = pPlayer->GetServerPos() - GetPos();
	if (vDist.Magnitude() >= 6.0f)
	{
		g_pGame->GetGameRun()->AddFixedMessage(FIXMSG_TARGETISFAR);
		return false;
	}

	g_pGame->GetGameSession()->c2s_CmdMultiBindPlayerInvite(idTarget);

	//	Start cool time
// 	m_BindCmdCoolCnt.Reset();

	return true;
}

bool CECHostPlayer::CmdMultiBindKick(int idTarget)
{
	
	if (!m_bMultiRideLeader || !ISPLAYERID(idTarget) || 
		idTarget == GetCharacterID())
		return false;

	CECElsePlayer* pPlayer = m_pPlayerMan->GetElsePlayer(idTarget);
	if (!pPlayer)
		return false;
	
	g_pGame->GetGameSession()->c2s_CmdMultiBindKick(pPlayer->GetMultiRidePos());

	return true;
}

bool CECHostPlayer::CmdViewOtherEquips(int idTarget)
{
	// 竞技场中，屏蔽查看他人信息，如果是跨服pk战场的观战者，则可以查看他人信息
	if(	(m_iBattleType == BATTLE_TYPE_ARENA || m_iBattleType == BATTLE_TYPE_ARENAREBORN || m_iBattleType == BATTLE_TYPE_CRSSVR) && m_iBattleCamp != GP_BATTLE_CAMP_OBSERVER)
		return false;
	// 匿名地图中，屏蔽查看他人信息
	if (m_bAnonymous)
		return false;

	if (GetCoolTime(GP_CT_VIEWOTHEREQUIP))
	{
		g_pGame->GetGameRun()->AddFixedMessage(FIXMSG_CMD_INCOOLTIME);
		return false;
	}

	if (!ISPLAYERID(idTarget) || idTarget == GetCharacterID())
		return false;

	CECElsePlayer* pPlayer = m_pPlayerMan->GetElsePlayer(idTarget);
	if (!pPlayer)
		return false;

	A3DVECTOR3 vDist = pPlayer->GetServerPos() - GetPos();
	if (vDist.Magnitude() >= 50.0f)
	{
		g_pGame->GetGameRun()->AddFixedMessage(FIXMSG_TARGETISFAR);
		return false;
	}

	g_pGame->GetGameSession()->c2s_CmdGetOtherEquipDetail(idTarget);

	return true;
}

bool CECHostPlayer::CmdAskDuel(int idTarget)
{
	if (!CanDo(CANDO_DUEL))
	{
		if (IsFighting())
			g_pGame->GetGameRun()->AddFixedMessage(FIXMSG_DUEL_INFIGHT);

		return false;
	}

	if (!ISPLAYERID(idTarget) || idTarget == GetCharacterID())
		return false;

	CECElsePlayer* pPlayer = m_pPlayerMan->GetElsePlayer(idTarget);
	if (!pPlayer)
		return false;

	A3DVECTOR3 vDist = pPlayer->GetServerPos() - GetPos();
	if (vDist.Magnitude() >= 16.0f)
	{
		g_pGame->GetGameRun()->AddFixedMessage(FIXMSG_TARGETISFAR);
		return false;
	}
	
	g_pGame->GetGameSession()->c2s_CmdDuelRequest(idTarget);
	g_pGame->GetGameRun()->AddFixedMessage(FIXMSG_DUEL_ASKSENT);
	LuaEvent::FirstPK();
	return true;
}

//	Invite selected player to join team
bool CECHostPlayer::CmdInviteToTeam()
{
	if (IsDead() || !ISPLAYERID(m_idSelTarget) || m_idSelTarget == GetCharacterID())
		return false;

	g_pGame->GetGameSession()->c2s_CmdTeamInvite(m_idSelTarget);
	return true;
}

//	Leave current team
bool CECHostPlayer::CmdLeaveTeam()
{
	if (IsDead() || !m_pTeam)
		return false;

	CECGameUIMan* pGameUI = g_pGame->GetGameRun()->GetUIManager()->GetInGameUIMan();
	const ACHAR* szMsg = pGameUI->GetStringFromTable(235);
	pGameUI->MessageBox("Game_TeamDisband", szMsg, MB_OKCANCEL, A3DCOLORRGBA(255, 255, 255, 160));
//	g_pGame->GetGameSession()->c2s_CmdTeamLeaveParty();
	return true;
}

//	Kick one member of team
bool CECHostPlayer::CmdKickTeamMember()
{
	return true;
}

//	Search for a team
bool CECHostPlayer::CmdFindTeam()
{
	return true;
}

//	Start trade with other selected player
bool CECHostPlayer::CmdStartTrade()
{
	// first of all see if we need to cancel sitdown work.
	if (m_pWorkMan->GetCurWorkID() == CECHPWork::WORK_SIT)
	{
		g_pGame->GetGameSession()->c2s_CmdStandUp();
		return false;
	}

	if (!CanDo(CANDO_TRADE))
		return false;

	if (!ISPLAYERID(m_idSelTarget) || m_idSelTarget == m_PlayerInfo.cid)
		return false;

	g_pGame->GetGameSession()->trade_Start(m_idSelTarget);
	return true;
}

//	Open booth for selling items
bool CECHostPlayer::CmdSellBooth()
{
	// first of all see if we need to cancel sitdown work.
	if (m_pWorkMan->GetCurWorkID() == CECHPWork::WORK_SIT)
	{
		g_pGame->GetGameSession()->c2s_CmdStandUp();
		return false;
	}

	if (!CanDo(CANDO_BOOTH))
		return false;

	//	Ask server whether we can open booth
	g_pGame->GetGameSession()->c2s_CmdOpenBoothTest(m_idBoothItem, m_iBoothItemSlot);
	return true;
}

//	Open booth for buying items
bool CECHostPlayer::CmdBuyBooth()
{
	return true;
}

//	Play a pose
bool CECHostPlayer::CmdStartPose(int iPose)
{
	// first of all see if we need to cancel sitdown work.
	if (m_pWorkMan->GetCurWorkID() == CECHPWork::WORK_SIT)
	{
		g_pGame->GetGameSession()->c2s_CmdStandUp();
		return false;
	}

	if (!CanDo(CANDO_PLAYPOSE))
		return false;

	CECHPWork* pCurWork = m_pWorkMan->GetCurWork();
	if (!pCurWork || pCurWork->GetWorkID() != CECHPWork::WORK_STAND)
		return false;

	if (iPose == ROLEEXP_SITDOWN)
		g_pGame->GetGameSession()->c2s_CmdSessionEmote(iPose);
	else if (iPose == ROLEEXP_KISS)
	{
		if (ISPLAYERID(m_idSelTarget))
			g_pGame->GetGameSession()->c2s_CmdConEmoteRequest(ROLEEXP_KISS, m_idSelTarget);
	}
	else
		g_pGame->GetGameSession()->c2s_CmdEmoteAction(iPose);

	return true;
}

//	Play a active pose
bool CECHostPlayer::CmdStartActivePose(int iPose)
{
	// first of all see if we need to cancel sitdown work.
	if (m_pWorkMan->GetCurWorkID() == CECHPWork::WORK_SIT)
	{
		g_pGame->GetGameSession()->c2s_CmdStandUp();
		return false;
	}

	if (!CanDo(CANDO_PLAYACTIVEPOSE))
		return false;

	CECHPWork* pCurWork = m_pWorkMan->GetCurWork();
	if (!pCurWork || pCurWork->GetWorkID() != CECHPWork::WORK_STAND)
		return false;

	CECElsePlayer* pPlayer = m_pPlayerMan->GetElsePlayer(m_idSelTarget);
	if (pPlayer == NULL || pPlayer->GetGender() == GetGender())
		return false;

	A3DVECTOR3 vDist = pPlayer->GetServerPos() - GetPos();
	if (vDist.Magnitude() >= 2.8f)
	{
		g_pGame->GetGameRun()->AddFixedMessage(FIXMSG_TARGETISFAR);
		return false;
	}

	g_pGame->GetGameSession()->c2s_CmdInviteActiveEmote(m_idSelTarget, iPose);

	return true;
}

//	Invite selected player to join faction
bool CECHostPlayer::CmdInviteToFaction()
{
	return true;
}

bool CECHostPlayer::CmdPetCall()
{
	int nBattlePetIndex = GetBattlePetIndex();
	if (m_nCurPetId)
		g_pGame->GetGameSession()->c2s_CmdPetRecall(nBattlePetIndex);
	else
	{
		if( nBattlePetIndex == -1 )
		{
			for(int i = 0; i < m_pPetPack->GetSize(); i++)
				if( m_pPetPack->GetItem(i) )
				{
					g_pGame->GetGameSession()->c2s_CmdPetSetStatus(i, m_pPetPack->GetItem(i)->GetTemplateID(), 0, 0);
					g_pGame->GetGameSession()->c2s_CmdPetSummon(i);
					break;
				}
		}
		else
			g_pGame->GetGameSession()->c2s_CmdPetSummon(GetBattlePetIndex());
	}

	return true;
}

bool CECHostPlayer::CmdPetCombine1()
{
	if(m_nPetCombinedType != -1)
		g_pGame->GetGameSession()->c2s_CmdPetUncombine(GetBattlePetIndex());
	else if( GetBattlePetIndex() != -1 )
	{
		if (GetCoolTime(GP_CT_PET_COMBINE1))
			g_pGame->GetGameRun()->AddFixedMessage(FIXMSG_CMD_INCOOLTIME);
		else
			g_pGame->GetGameSession()->c2s_CmdPetCombine(GetBattlePetIndex(), 0);
	}

	return true;
}

bool CECHostPlayer::CmdPetCombine2()
{
	if(m_nPetCombinedType != -1)
		g_pGame->GetGameSession()->c2s_CmdPetUncombine(GetBattlePetIndex());
	else if( GetBattlePetIndex() != -1 )
	{
		if (GetCoolTime(GP_CT_PET_COMBINE2))
			g_pGame->GetGameRun()->AddFixedMessage(FIXMSG_CMD_INCOOLTIME);
		else
			g_pGame->GetGameSession()->c2s_CmdPetCombine(GetBattlePetIndex(), 1);
	}

	return true;
}

bool CECHostPlayer::CmdSetHideEquipMask(int mask)
{
	if (GetCoolTime(GP_CT_SET_FASHION_MASK))
		g_pGame->GetGameRun()->AddFixedMessage(FIXMSG_CMD_INCOOLTIME);
	else
		c2s_SendCmdSetFashionMask(mask);
//	LuaEvent::FirstHideEquip();
	return true;
}

bool CECHostPlayer::CmdGetBattleInfo()
{
	if (GetCoolTime(GP_CT_GET_BATTLEINFO))
		g_pGame->GetGameRun()->AddFixedMessage(FIXMSG_CMD_INCOOLTIME);
	else
		c2s_SendCmdBattleGetInfo();	
	return true;
}