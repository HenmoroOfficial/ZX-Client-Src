/*
 * FILE: EC_ManPlayer.cpp
 *
 * DESCRIPTION: 
 *
 * CREATED BY: Duyuxin, 2004/8/30
 *
 * HISTORY: 
 *
 * Copyright (c) 2004 Archosaur Studio, All Rights Reserved.
 */

#pragma warning (disable: 4284)

#include "EC_Global.h"
#include "EC_ManPlayer.h"
#include "EC_HostPlayer.h"
#include "EC_ElsePlayer.h"
#include "EC_LoginPlayer.h"
#ifdef BACK_VERSION
#include "EC_Viewport.h"
#include "A3DTerrainWater.h"
#else
#include "ECViewport.h"
#include "A3DTerrainWater2.h"
#endif
#include "EC_GPDataType.h"
#include "EC_GameRun.h"
#include "EC_Game.h"
#include "EC_UIManager.h"
#include "EC_GameUIMan.h"
#include "EC_GameSession.h"
#include "EC_World.h"
#include "EC_CDS.h"
#include "EC_Utility.h"
#include "EC_Configs.h"
#include "EC_Team.h"
#include "EC_TeamMan.h"
#include "EC_UIManager.h"
#include "EC_GameUIMan.h"
#include "EC_FixedMsg.h"
#include "EC_Resource.h"
#include "EC_SceneLoader.h"
#include "EC_Model.h"
#include "EC_InputCtrl.h"
#include "EC_ManMatter.h"
#include "EC_Faction.h"
#include "EC_GameRecord.h"
#include "EC_Cutscene.h"
#include "EC_CDR.h"
#include "EC_HPWork.h"
#include "EC_HPWorkMove.h"
#include "EC_HPWorkSit.h"
#include "EC_HPWorkFly.h"
#include "EC_CollisionTest.h"
#include "EC_ManNPC.h"

#include "PlayerBaseInfo_Re.hpp"
#include "PlayerBaseInfo2_Re.hpp"
#include "GetCustomData_Re.hpp"

#include "A3DDevice.h"
#include "A3DCamera.h"
#include "A2DSprite.h"
#include "A3DCollision.h"
#include "A3DTerrain2.h"
#include "A2DSpriteBuffer.h"
#include "A3DFont.h"
#include "A3DLight.h"
#include "A3DSkin.h"
#include "A3DFuncs.h"

#include "A3DSkinMan.h"

#include "EC_RTDebug.h"

#define new A_DEBUG_NEW

///////////////////////////////////////////////////////////////////////////
//	
//	Define and Macro
//	
///////////////////////////////////////////////////////////////////////////

#define MAX_CACHEDPLAYER	1024	//	Maximum cached player number

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
//	Implement CECPlayerMan
//	
///////////////////////////////////////////////////////////////////////////

CECPlayerMan::CECPlayerMan(CECGameRun* pGameRun) :
CECManager(pGameRun),
m_PlayerTab(512),
m_UkPlayerTab(32),
m_PlayerCache(MAX_CACHEDPLAYER),
m_aMMPlayers(0, 128)
{
	m_iManagerID	= MAN_PLAYER;
	m_pBoothModel_M	= NULL;
	m_pBoothModel_F	= NULL;
	m_QueryCnt.SetPeriod(3000);
	m_CacheCnt.SetPeriod(300000);
	m_TLTimeCnt.SetPeriod(2000);

	InitializeCriticalSection(&m_csLoad);
}

CECPlayerMan::~CECPlayerMan()
{
	DeleteCriticalSection(&m_csLoad);
}

//	Release manager
void CECPlayerMan::Release()
{
	OnLeaveGameWorld();
}

//	On entering game world
bool CECPlayerMan::OnEnterGameWorld()
{
	if ((m_pBoothModel_M = g_pGame->LoadA3DSkinModel(res_ModelFile(RES_MOD_BOOTH_M))))
		m_pBoothModel_M->EnableSpecular(true);
	
	if ((m_pBoothModel_F = g_pGame->LoadA3DSkinModel(res_ModelFile(RES_MOD_BOOTH_F))))
		m_pBoothModel_F->EnableSpecular(true);

	return true;
}

//	On leaving game world
bool CECPlayerMan::OnLeaveGameWorld()
{
	//	Release all else players
	if (m_PlayerTab.size())
	{
		PlayerTable::iterator it = m_PlayerTab.begin();
		for (; it != m_PlayerTab.end(); ++it)
		{
			CECElsePlayer* pPlayer = *it.value();
			ReleaseElsePlayer(pPlayer);
		}

		m_PlayerTab.clear();
	}

	//	Release all cached player data
	if (m_PlayerCache.size())
	{
		CacheTable::iterator it = m_PlayerCache.begin();
		for (; it != m_PlayerCache.end(); ++it)
		{
			CECEPCacheData* pData = *it.value();
			delete pData;
		}

		m_PlayerCache.clear();
	}
	
	//	Release all loaded models
	ACSWrapper csa(&m_csLoad);

	for (int i=0; i < m_aLoadedModels.GetSize(); i++)
	{
		PLAYERMODEL* pInfo = m_aLoadedModels[i];
		CECPlayer::ReleasePlayerModel(pInfo->Ret);
		delete pInfo;
	}

	m_aLoadedModels.RemoveAll();

	//	Release booth models
	if (m_pBoothModel_M)
	{
		g_pGame->ReleaseA3DSkinModel(m_pBoothModel_M);
		m_pBoothModel_M = NULL;
	}

	if (m_pBoothModel_F)
	{
		g_pGame->ReleaseA3DSkinModel(m_pBoothModel_F);
		m_pBoothModel_F = NULL;
	}

	abase::hash_map<int, CECModel*>::iterator it = m_BoothModelMap.begin();
		
	for(; it != m_BoothModelMap.end(); ++it)
		A3DRELEASE(it->second);
	m_BoothModelMap.clear();	

	m_aTabSels.RemoveAll(false);
	return true;
}

//	Get host player
CECHostPlayer* CECPlayerMan::GetHostPlayer()
{
	return m_pGameRun->GetHostPlayer();
}

//	Process message
bool CECPlayerMan::ProcessMessage(const ECMSG& Msg)
{
	ASSERT(Msg.iManager == MAN_PLAYER);

	if (!GetHostPlayer())
	{
		ASSERT(0);	// 主角不存在，所有玩家相关的协议都无视
		return true;
	}

	if (!Msg.iSubID)
	{
		//	Messages to host player
		GetHostPlayer()->ProcessMessage(Msg);
	}
	else if (Msg.iSubID < 0)
	{
		switch (Msg.dwMsg)
		{
		case MSG_PM_PLAYERINFO:			OnMsgPlayerInfo(Msg);			break;
		case MSG_PM_PLAYERMOVE:			OnMsgPlayerMove(Msg);			break;
		case MSG_PM_PLAYERRUNOUT:		OnMsgPlayerRunOut(Msg);			break;
		case MSG_PM_PLAYEREXIT:			OnMsgPlayerExit(Msg);			break;
		case MSG_PM_PLAYERDIED:			OnMsgPlayerDied(Msg);			break;
		case MSG_PM_PLAYERREVIVE:		OnMsgPlayerRevive(Msg);			break;
		case MSG_PM_PLAYEROUTOFVIEW:	OnMsgPlayerOutOfView(Msg);		break;
		case MSG_PM_PLAYERDISAPPEAR:	OnMsgPlayerDisappear(Msg);		break;
		case MSG_PM_PLAYERSTOPMOVE:		OnMsgPlayerStopMove(Msg);		break;
		case MSG_PM_PLAYERLEVELUP:		OnMsgPlayerLevelUp(Msg);		break;
		case MSG_PM_PLAYEREXTPROP:		OnMsgPlayerExtProp(Msg);		break;
		case MSG_PM_LEAVETEAM:			OnMsgPlayerLeaveTeam(Msg);		break;
		case MSG_PM_DOCONEMOTE:			OnMsgDoConEmote(Msg);			break;
		case MSG_PM_INVALIDOBJECT:		OnMsgInvalidObject(Msg);		break;
		case MSG_PM_PLAYEREXTSTATE:		OnMsgPlayerExtState(Msg);		break;
		case MSG_PM_PICKUPMATTER:		OnMsgPickUpMatter(Msg);			break;
		case MSG_PM_PLAYERBINDSTART:	OnMsgPlayerBindStart(Msg);		break;
		case MSG_PM_PLAYERDUELRLT:		OnMsgPlayerDuelResult(Msg);		break;
		case MSG_PM_TANKLEADER:			OnMsgTankLeader(Msg);			break;
		case MSG_PM_PLAYERSCALESHAPE:	OnMsgPlayerScaleShape(Msg);		break;	//Added 2011-06-28.
		case MSG_PM_PLAYERVIPAWARDUPDATE:	OnMsgPlayerVIPAwardUpdate(Msg);	break;	//Added 2012-01-16.
		case MSG_PM_SPECIAL_MOVE_OPT:	OnMsgPlayerSpecialMoveOpt(Msg);	break;	//Added 2012-06-27.
		case MSG_PM_CHANGENAME:			OnMsgPlayerChangeName(Msg);	break;	//Added 2012-01-16.
		case MSG_PM_COLLISION_SPECIAL_STATE:	OnMsgPlayerCollisionSpecialState(Msg); break;	//Added 2012-08-02.
		case MSG_PM_XY_DARK_LIGHT:		OnMsgPlayerXYDarkLight(Msg);	break;	//Added 2012-08-16.
		case MSG_PM_WING_CHANGE_COLOR:	OnMsgPlayerWingChangeColor(Msg);break;	//Added 2012-08-20.
		case MSG_PM_DIR_VISIBLE_STATE:	OnMsgDirVisibleState(Msg);		break;	//Added 2012-09-05.

		case MSG_PM_XY_DARK_LIGHT_SPIRIT: OnMsgPlayerXYDarkLightSpirit(Msg); break;
		case MSG_PM_MIRROR_IMAGE_CNT:	OnMsgPlayerXYMirrorImageCnt(Msg);	break;
		case MSG_PM_BATH_START:        OnMsgPlayerBathStart(Msg); break;
		case MSG_PM_BATH_GETAWARD:     OnMsgPlayerGetBathAward(Msg);break;
		case MSG_PM_PUPPET_FORM:	   OnMsgPlayerPuppetForm(Msg);		break;
		case MSG_PM_QILIN_BIND_START:  OnMsgPlayerQilinBindStart(Msg);	break;
		case MSG_PM_PLAYER_CHANGE_PUPPET_FORM: OnMsgPlayerPuppetFormChange(Msg); break;
		case MSG_PM_PLAYER_BLOW_OFF:	OnMsgPlayerBlowOff(Msg); break;
		case MSG_PM_BE_CHARGE:			OnMsgPlayerBeCharge(Msg); break;
		case MSG_PM_BLOODPOOL_STATUS:   OnMsgPlayerBloodPool(Msg); break;
		case MSG_PM_TELEPORT_SKILL_INFO: OnMsgPlayerTeleportInfo(Msg); break;

		case MSG_PM_TRAVEL_FREE:
		case MSG_PM_PLAYEREQUIPDATA:
		case MSG_PM_PLAYERBASEINFO:
		case MSG_PM_PLAYERBASEINFO2:
		case MSG_PM_PLAYERCUSTOM:
		case MSG_PM_PLAYERFLY:
		case MSG_PM_PLAYERSITDOWN:
		case MSG_PM_CASTSKILL:
		case MSG_PM_CHANGENAMECOLOR:
		case MSG_PM_PLAYERROOT:
		case MSG_PM_PLAYERATKRESULT:
		case MSG_PM_STARTATTACK:
		case MSG_PM_PLAYERDOEMOTE:
		case MSG_PM_PLAYERUSEITEM:
		case MSG_PM_USEITEMWITHDATA:
		case MSG_PM_ENCHANTRESULT:
		case MSG_PM_PLAYERDOACTION:
		case MSG_PM_PLAYERSKILLRESULT:
		case MSG_PM_PLAYERADVDATA:
		case MSG_PM_PLAYERGATHER:
		case MSG_PM_PLAYERINTEAM:
		case MSG_PM_PLAYERCHGSHAPE:
		case MSG_PM_BOOTHOPT:
		case MSG_PM_PLAYERTRAVEL:
		case MSG_PM_PLAYERPVP:
		case MSG_PM_FASHIONENABLE:	
		case MSG_PM_PLAYEREFFECT:
		case MSG_PM_CHANGEFACE:
		case MSG_PM_PLAYERBINDOPT:
		case MSG_PM_PLAYERMOUNT:
		case MSG_PM_PLAYERDUELOPT:
		case MSG_PM_PLAYERLEVEL2:
		case MSG_PM_MULTIBINDOPT:
		case MSG_PM_CARRIER:
		case MSG_PM_BATH_OPT:
		case MSG_PM_START_INTERACT:
		case MSG_PM_IN_INTERACT:
		case MSG_PM_END_INTERACT:
		case MSG_PM_FINISH_INTERACT:
		case MSG_PM_CANCEL_INTERACT:
		case MSG_PM_QILIN_BINDOPT:
		case MSG_HST_INVITE_ACTIVE_EMOTE:
		case MSG_HST_REPLY_ACTIVE_EMOTE:
		case MSG_PM_PLAY_ACTIVE_EMOTE:
		case MSG_PM_STOP_ACTIVE_EMOTE:

			TransmitMessage(Msg);
			break;

		default:
			ASSERT(0);
			break;
		}
	}
	else
	{
		//	Messages to specified else player
	}

	return true;
}

//	Get else player by id
CECElsePlayer* CECPlayerMan::GetElsePlayer(int cid, DWORD dwBornStamp/* 0 */)
{
	PlayerTable::pair_type Pair = m_PlayerTab.get(cid);
	if (!Pair.second)
		return NULL;

	if (dwBornStamp)
	{
		CECPlayer* pPlayer = *Pair.first;
		if (pPlayer->GetBornStamp() != dwBornStamp)
			return NULL;
	}

	return *Pair.first;
}

//	Get a player (may be host or else player) by id
CECPlayer* CECPlayerMan::GetPlayer(int cid, DWORD dwBornStamp/* 0 */)
{
	CECHostPlayer* pHost = GetHostPlayer();
	if (pHost && pHost->GetCharacterID() == cid)
		return pHost;
	else
		return GetElsePlayer(cid, dwBornStamp);
}

//	Seek out else player with specified id, if he doesn't exist, try to get from server
CECElsePlayer* CECPlayerMan::SeekOutElsePlayer(int cid)
{
	PlayerTable::pair_type Pair = m_PlayerTab.get(cid);
	if (!Pair.second)
	{
		//	Counldn't find this else player, put it into unkonwn player table
		m_UkPlayerTab.put(cid, cid);
		return NULL;
	}

	return *Pair.first;
}

bool CECPlayerMan::OnMsgPlayerInfo(const ECMSG& Msg)
{
	using namespace S2C;

	CECGameSession* pSession = g_pGame->GetGameSession();
	int iHostID = pSession->GetCharacterID();

	BYTE* pDataBuf = (BYTE*)Msg.dwParam1;
	ASSERT(pDataBuf);

	switch (Msg.dwParam2)
	{
	case PLAYER_INFO_1:
	case PLAYER_ENTER_WORLD:
	case PLAYER_ENTER_SLICE:
	{
		info_player_1 cmd;
		if (!cmd.FillData(pDataBuf, Msg.dwParam3))
			return false;

		if (cmd.cid != iHostID)
		{
			CECElsePlayer* pPlayer = ElsePlayerEnter(cmd, Msg.dwParam2);
			if (pPlayer)
			{
				//	Is custom data ready
				if (!pPlayer->IsBaseInfoReady())
					pSession->GetRoleBaseInfo(1, &cmd.cid);

				//	Is equipment data ready
				if (!pPlayer->IsEquipDataReady())
					pSession->c2s_CmdGetOtherEquip(1, &cmd.cid);

				//	Get faction info
				if (pPlayer->GetFactionID())
					g_pGame->GetFactionMan()->GetFaction(pPlayer->GetFactionID(), true, false);
			}
		}

		break;
	}
	case PLAYER_INFO_2:			break;
	case PLAYER_INFO_3:			break;
	case PLAYER_INFO_4:			break;

	case PLAYER_INFO_1_LIST:
	{
		cmd_player_info_1_list cmd;
		if (!cmd.FillData(pDataBuf, Msg.dwParam3))
			return false;

		if (!cmd.count)
			break;

		AArray<int, int> a1(0, cmd.count);
		AArray<int, int> a2(0, cmd.count);
		AArray<int, int> a3(0, cmd.count);

		int i;
		for (i=0; i < cmd.count; i++)
		{
			const info_player_1& Info = cmd.players[i];
			if (Info.cid == iHostID)
				continue;

			CECElsePlayer* pPlayer = ElsePlayerEnter(Info, Msg.dwParam2);
			if (pPlayer)
			{
				//	Is custom data ready
				if (!pPlayer->IsBaseInfoReady())
					a2.Add(Info.cid);

				//	Is equipment data ready
				if (!pPlayer->IsEquipDataReady())
					a1.Add(Info.cid);

				// Get faction info
				if (pPlayer->GetFactionID() && !g_pGame->GetFactionMan()->GetFaction(pPlayer->GetFactionID()))
				{
					int j;
					for (j = 0; j < a3.GetSize(); j++)
						if (a3[j] == pPlayer->GetFactionID())
							break;

					if (j == a3.GetSize())
						a3.Add(pPlayer->GetFactionID());
				}
			}
		}

		//	Get both base info and custom data
		if (a2.GetSize())
			pSession->GetRoleBaseInfo(a2.GetSize(), a2.GetData());

		//	Get new equipment data
		if (a1.GetSize())
			pSession->c2s_CmdGetOtherEquip(a1.GetSize(), a1.GetData());

		//	Get factions info
		if (a3.GetSize())
			pSession->GetFactionInfo(a3.GetSize(), a3.GetData(), 0);

		break;
	}
	case PLAYER_INFO_2_LIST:	break;
	case PLAYER_INFO_3_LIST:	break;
	case PLAYER_INFO_23_LIST:	break;

	case SELF_INFO_1:

		HostPlayerInfo1(*(cmd_self_info_1*)pDataBuf);
		break;

	case PLAYER_INFO_00:
	{
		cmd_player_info_00* pCmd = (cmd_player_info_00*)pDataBuf;
		CECPlayer* pPlayer = GetPlayer(pCmd->idPlayer);
		if (pPlayer)
		{
			ROLEBASICPROP& bp = pPlayer->GetBasicProps();
			ROLEEXTPROP& ep = pPlayer->GetExtendProps();

			bp.iLevel		= pCmd->sLevel;
			bp.iDeityLevel	= pCmd->sDeityLevel;
			bp.iCurHP		= pCmd->iHP;
			bp.iCurMP		= pCmd->iMP;
			bp.iCurDP		= pCmd->iDP;
			bp.irage		= pCmd->iRage;
			ep.max_hp		= pCmd->iMaxHP;
			ep.max_mp		= pCmd->iMaxMP;
			ep.max_dp		= pCmd->iMaxDP;
			if (pPlayer->GetCloneID())
			{
				CECPlayer* pClone = GetPlayer(pPlayer->GetCloneID());
				if (pClone)
				{
					ROLEBASICPROP& bp2 = pClone->GetBasicProps();
					ROLEEXTPROP& ep2 = pClone->GetExtendProps();
					bp2.iLevel		= pCmd->sLevel;
					bp2.iDeityLevel	= pCmd->sDeityLevel;
					bp2.iCurHP		= pCmd->iHP;
					bp2.iCurMP		= pCmd->iMP;
					bp2.iCurDP		= pCmd->iDP;
					ep2.max_hp		= pCmd->iMaxHP;
					ep2.max_mp		= pCmd->iMaxMP;
					ep2.max_dp		= pCmd->iMaxDP;
				}
			}
		//	CuiMing told me don't use this flag, because it isn't
		//	updated in time.
		//	pPlayer->SetFightFlag(pCmd->State ? true : false);
		}
		
		break;
	}
	case PLAYER_EXTRA_EQUIP_EFFECT:
	{
		player_extra_equip_effect* pCmd = (player_extra_equip_effect*)pDataBuf;
		CECPlayer* pPlayer = GetPlayer(pCmd->player_id);
		if (pPlayer)
			pPlayer->SetExtraEquipEffect(pCmd->weapon_effect_level);
		break;
	}
	}

	return true;
}

//	Handler of MSG_PM_PLAYERMOVE
bool CECPlayerMan::OnMsgPlayerMove(const ECMSG& Msg)
{
	using namespace S2C;

	CECGameSession* pSession = g_pGame->GetGameSession();
	int iHostID = pSession->GetCharacterID();

	cmd_object_move* pCmd = (cmd_object_move*)Msg.dwParam1;
	ASSERT(pCmd);

	if (!pCmd->use_time)
		return true;

	if (pCmd->id != iHostID)
	{
		CECElsePlayer* pPlayer = SeekOutElsePlayer(pCmd->id);
		if (pPlayer)
			pPlayer->MoveTo(*pCmd);
		
		g_pGame->RuntimeDebugInfo(RTDCOL_NETWORK, _AL("NET - Elseplayer move"));
	}

	return true;
}

//	Handler of MSG_PM_PLAYERRUNOUT
bool CECPlayerMan::OnMsgPlayerRunOut(const ECMSG& Msg)
{
	using namespace S2C;

	CECGameSession* pSession = g_pGame->GetGameSession();

	cmd_leave_slice* pCmd = (cmd_leave_slice*)Msg.dwParam1;
	ASSERT(pCmd && pCmd->id != pSession->GetCharacterID());

	ElsePlayerLeave(pCmd->id, false);

	return true;
}

//	Handler of MSG_PM_PLAYEREXIT
bool CECPlayerMan::OnMsgPlayerExit(const ECMSG& Msg)
{
	using namespace S2C;

	CECGameSession* pSession = g_pGame->GetGameSession();

	cmd_player_leave_world* pCmd = (cmd_player_leave_world*)Msg.dwParam1;
	ASSERT(pCmd);
	ASSERT(pCmd->id != pSession->GetCharacterID());

	ElsePlayerLeave(pCmd->id, true);

	return true;
}

//	Handler of MSG_PM_PLAYERDIED
bool CECPlayerMan::OnMsgPlayerDied(const ECMSG& Msg)
{
	using namespace S2C;

	cmd_player_died* pCmd = (cmd_player_died*)Msg.dwParam1;
	ASSERT(pCmd && ISPLAYERID(pCmd->idPlayer));

	CECElsePlayer* pPlayer = SeekOutElsePlayer(pCmd->idPlayer);
	if (pPlayer && !pPlayer->IsAboutToDie())
		pPlayer->Killed(pCmd->idKiller);
	
	return true;
}

//	Handler of MSG_PM_PLAYERREVIVE
bool CECPlayerMan::OnMsgPlayerRevive(const ECMSG& Msg)
{
	using namespace S2C;

	cmd_player_revive* pCmd = (cmd_player_revive*)Msg.dwParam1;
	ASSERT(pCmd && ISPLAYERID(pCmd->idPlayer));

	CECGameSession* pSession = g_pGame->GetGameSession();

	if (pCmd->idPlayer == pSession->GetCharacterID())
	{
		GetHostPlayer()->Revive(pCmd);
	}
	else
	{
		CECElsePlayer* pPlayer = SeekOutElsePlayer(pCmd->idPlayer);
		if (pPlayer)
			pPlayer->Revive(pCmd);
	}

	return true;
}

bool CECPlayerMan::OnMsgPlayerOutOfView(const ECMSG& Msg)
{
	using namespace S2C;

	int id = (int)Msg.dwParam1;
	CECGameSession* pSession = g_pGame->GetGameSession();
	ASSERT(id != pSession->GetCharacterID());

	ElsePlayerLeave(id, false);

	return true;
}

bool CECPlayerMan::OnMsgPlayerDisappear(const ECMSG& Msg)
{
	using namespace S2C;

	CECGameSession* pSession = g_pGame->GetGameSession();

	cmd_object_disappear* pCmd = (cmd_object_disappear*)Msg.dwParam1;
	ASSERT(pCmd && ISPLAYERID(pCmd->id));
	ASSERT(pCmd->id != pSession->GetCharacterID());

	ElsePlayerLeave(pCmd->id, false);

	return true;
}

bool CECPlayerMan::OnMsgPlayerStopMove(const ECMSG& Msg)
{
	using namespace S2C;

	CECGameSession* pSession = g_pGame->GetGameSession();

	cmd_object_stop_move* pCmd = (cmd_object_stop_move*)Msg.dwParam1;
	ASSERT(pCmd && ISPLAYERID(pCmd->id));
	ASSERT(pCmd->id != pSession->GetCharacterID());

	CECElsePlayer* pPlayer = SeekOutElsePlayer(pCmd->id);
	if (pPlayer)
	{
		pPlayer->StopMoveTo(*pCmd);
		g_pGame->RuntimeDebugInfo(RTDCOL_NETWORK, _AL("NET - Elseplayer stop move"));
	}

	return true;
}

void CECPlayerMan::OnMsgPlayerSpecialMoveOpt(const ECMSG& Msg)
{
	using namespace S2C;

	CECGameSession* pSession = g_pGame->GetGameSession();
	switch ( Msg.dwParam2 )
	{
		case OBJECT_START_SPECIAL_MOVE:
		{
			cmd_object_start_special_move* pCmd = (cmd_object_start_special_move*)Msg.dwParam1;
			ASSERT(pCmd->id != pSession->GetCharacterID());
			CECElsePlayer* pPlayer = SeekOutElsePlayer( pCmd->id );
			if ( pPlayer )
			{
				pPlayer->CheckStartSepcialMove( *pCmd );
			}
			break;
		}
		case OBJECT_SPECIAL_MOVE:
		{
			cmd_object_special_move* pCmd = (cmd_object_special_move*)Msg.dwParam1;
			ASSERT(pCmd->id != pSession->GetCharacterID());
			CECElsePlayer* pPlayer = SeekOutElsePlayer( pCmd->id );
			if ( pPlayer )
			{
				pPlayer->UpdateSpecialMove( *pCmd );
			}
			break;
		}
		case OBJECT_STOP_SPECIAL_MOVE:
		{
			cmd_object_stop_special_move* pCmd = (cmd_object_stop_special_move*)Msg.dwParam1;
			ASSERT(pCmd->id != pSession->GetCharacterID());
			CECElsePlayer* pPlayer = SeekOutElsePlayer( pCmd->id );
			if ( pPlayer )
			{
				pPlayer->StopSpecialMove( *pCmd );
			}
			break;
		}
		case COLLISION_PLAYER_POS:
		{
			collision_player_pos* pCmd = (collision_player_pos*)Msg.dwParam1;
			ASSERT(pCmd->id != pSession->GetCharacterID());
			CECElsePlayer* pPlayer = SeekOutElsePlayer( pCmd->id );
			if ( pPlayer )
			{
				cmd_object_stop_special_move cmdStopMove;
				cmdStopMove.id = pCmd->id;
				cmdStopMove.cur_dir = 0;
				cmdStopMove.cur_pos = pCmd->pos;
				pPlayer->StopSpecialMove(cmdStopMove);
			}
			break;
		}
		default:
		{
			break;
		}
	}
}

bool CECPlayerMan::OnMsgPlayerLevelUp(const ECMSG& Msg)
{
	using namespace S2C;

	cmd_level_up* pCmd = (cmd_level_up*)Msg.dwParam1;
	ASSERT(pCmd && ISPLAYERID(pCmd->idPlayer));

	CECGameSession* pSession = g_pGame->GetGameSession();

	if (pCmd->idPlayer == pSession->GetCharacterID())
	{
		GetHostPlayer()->LevelUp();
	}
	else
	{
		CECElsePlayer* pPlayer = SeekOutElsePlayer(pCmd->idPlayer);
		if (pPlayer)
			pPlayer->LevelUp();
	}

	return true;
}

bool CECPlayerMan::OnMsgPlayerExtProp(const ECMSG& Msg)
{
	using namespace S2C;

	void* pData;
	int idPlayer, iIndex;

	switch (Msg.dwParam2)
	{
	case PLAYER_EXT_PROP_BASE:
	{
		cmd_pep_base* pCmd = (cmd_pep_base*)Msg.dwParam1;
		idPlayer	= pCmd->idPlayer;
		pData		= &pCmd->ep_base;
		iIndex		= EXTPROPIDX_BASE;
		break;
	}
	case PLAYER_EXT_PROP_MOVE:
	{
		cmd_pep_move* pCmd = (cmd_pep_move*)Msg.dwParam1;
		idPlayer	= pCmd->idPlayer;
		pData		= &pCmd->ep_move;
		iIndex		= EXTPROPIDX_MOVE;
		break;
	}
	case PLAYER_EXT_PROP_ATK:
	{
		cmd_pep_attack* pCmd = (cmd_pep_attack*)Msg.dwParam1;
		idPlayer	= pCmd->idPlayer;
		pData		= &pCmd->ep_attack;
		iIndex		= EXTPROPIDX_ATTACK;
		break;
	}
	case PLAYER_EXT_PROP_DEF:
	{
		cmd_pep_def* pCmd = (cmd_pep_def*)Msg.dwParam1;
		idPlayer	= pCmd->idPlayer;
		pData		= &pCmd->ep_def;
		iIndex		= EXTPROPIDX_DEF;
		break;
	}
	default:
		ASSERT(0);
		return false;
	}

	if (!ISPLAYERID(idPlayer))
	{
		ASSERT(ISPLAYERID(idPlayer));
		return false;
	}

	CECGameSession* pSession = g_pGame->GetGameSession();

	if (idPlayer == pSession->GetCharacterID())
	{
		GetHostPlayer()->SetPartExtendProps(iIndex, pData);
	}
	else
	{
		CECElsePlayer* pPlayer = SeekOutElsePlayer(idPlayer);
		if (pPlayer)
			pPlayer->SetPartExtendProps(iIndex, pData);
	}

	return true;
}

bool CECPlayerMan::OnMsgPlayerLeaveTeam(const ECMSG& Msg)
{
	using namespace S2C;

	cmd_team_member_leave* pCmd = (cmd_team_member_leave*)Msg.dwParam1;
	ASSERT(pCmd);

	CECTeamMan* pTeamMan = g_pGame->GetGameRun()->GetTeamMan();
	CECTeam* pTeam = pTeamMan->GetTeam(pCmd->idLeader);
	if (!pTeam)
	{
		ASSERT(pTeam);
		return false;
	}

	CECTeamMember* pMember = pTeam->GetMemberByID(pCmd->idMember);
	if (!pMember)
	{
		ASSERT(pMember);
		return false;
	}

	//	In cmd_team_member_leave, reason won't be GP_LTR_LEADERCANCEL
	if (pCmd->reason != GP_LTR_LEADERCANCEL)
	{
		//	Display the reason
		if (pCmd->reason == GP_LTR_KICKEDOUT)
			g_pGame->GetGameRun()->AddFixedMessage(FIXMSG_TEAMMEMKICKEDOUT, pMember->GetName());
		else
			g_pGame->GetGameRun()->AddFixedMessage(FIXMSG_LEAVETEAM, pMember->GetName());
	}

	pTeam->RemoveMember(pCmd->idMember);

	//	Update team UI
	CECGameUIMan* pGameUI = g_pGame->GetGameRun()->GetUIManager()->GetInGameUIMan();
	pGameUI->UpdateTeam();

	return true;
}

bool CECPlayerMan::OnMsgDoConEmote(const ECMSG& Msg)
{
	using namespace S2C;
	cmd_do_concurrent_emote* pCmd = (cmd_do_concurrent_emote*)Msg.dwParam1;
	ASSERT(pCmd && Msg.dwParam2 == DO_CONCURRENT_EMOTE);

	CECHostPlayer* pHost = GetHostPlayer();

	if (pCmd->id1)
	{
		if (pCmd->id1 == pHost->GetCharacterID())
			pHost->ProcessMessage(Msg);
		else
		{
			CECElsePlayer* pPlayer = SeekOutElsePlayer(pCmd->id1);
			if (pPlayer)
				pPlayer->ProcessMessage(Msg);
		}
	}
	
	if (pCmd->id2)
	{
		if (pCmd->id2 == pHost->GetCharacterID())
			pHost->ProcessMessage(Msg);
		else
		{
			CECElsePlayer* pPlayer = SeekOutElsePlayer(pCmd->id2);
			if (pPlayer)
				pPlayer->ProcessMessage(Msg);
		}
	}
	
	return true;
}

bool CECPlayerMan::OnMsgInvalidObject(const ECMSG& Msg)
{
	using namespace S2C;
	cmd_invalid_object* pCmd = (cmd_invalid_object*)Msg.dwParam1;
	ASSERT(pCmd);

	//	Remove the player if it exists
	CECElsePlayer* pPlayer = GetElsePlayer(pCmd->id);
	if (pPlayer)
	{
		ElsePlayerLeave(pCmd->id, false);
	}
	
	return true;
}

bool CECPlayerMan::OnMsgPlayerExtState(const ECMSG& Msg)
{
	using namespace S2C;

	int cid = 0;

	if (Msg.dwParam2 == UPDATE_EXT_STATE)
		cid = ((cmd_update_ext_state*)Msg.dwParam1)->id;
	else if (Msg.dwParam2 == ICON_STATE_NOTIFY)
		cid = ((cmd_icon_state_notify*)Msg.dwParam1)->id;
	else
	{
		ASSERT(0);
		return false;
	}

	CECHostPlayer* pHost = g_pGame->GetGameRun()->GetHostPlayer();

	if (cid == pHost->GetCharacterID())
	{
		if( Msg.dwParam2 != ICON_STATE_NOTIFY)
			pHost->ProcessMessage(Msg);
	}
	else
	{
		if (Msg.dwParam2 == ICON_STATE_NOTIFY && pHost->GetTeam() && pHost->GetTeam()->GetMemberByID(cid))
		{
			cmd_icon_state_notify cmd;
			
			if (!cmd.FillData((void*)Msg.dwParam1, Msg.dwParam3))
				return false;
			//	Update host's team member's icon state
			CECTeam* pTeam = pHost->GetTeam();
			CECTeamMember* pMember = pTeam->GetMemberByID(cid);
			pMember->ResetIconStates(cmd);
			CECElsePlayer* pPlayer = GetElsePlayer(cid);
			if (pPlayer)
				pPlayer->ProcessMessage(Msg);
		}
		else
		{
			CECElsePlayer* pPlayer = SeekOutElsePlayer(cid);
			if (pPlayer)
				pPlayer->ProcessMessage(Msg);
		}
	}

	return true;
}

void CECPlayerMan::OnMsgPickUpMatter(const ECMSG& Msg)
{
	using namespace S2C;
	cmd_matter_pickup* pCmd = (cmd_matter_pickup*)Msg.dwParam1;
	CECHostPlayer* pHost = g_pGame->GetGameRun()->GetHostPlayer();

	if (pCmd->who == pHost->GetCharacterID())
		pHost->ProcessMessage(Msg);
	else
	{
		CECElsePlayer* pPlayer = SeekOutElsePlayer(pCmd->who);
		if (pPlayer)
			pPlayer->ProcessMessage(Msg);
	}

	//	Remove matter from world
	CECMatterMan* pMatterMan = g_pGame->GetGameRun()->GetWorld()->GetMatterMan();
	pMatterMan->RemoveMatter(pCmd->matter_id);
}

void CECPlayerMan::OnMsgPlayerBindStart(const ECMSG& Msg)
{
	using namespace S2C;
	const cmd_player_bind_start* pCmd = (const cmd_player_bind_start*)Msg.dwParam1;

	CECPlayer* pPlayer = GetPlayer(pCmd->mule);
	if (pPlayer)
		pPlayer->OnStartBinding(pCmd->mule, pCmd->rider);

	pPlayer = GetPlayer(pCmd->rider);
	if (pPlayer)
		pPlayer->OnStartBinding(pCmd->mule, pCmd->rider);
}

void CECPlayerMan::OnMsgPlayerQilinBindStart(const ECMSG& Msg)
{
	using namespace S2C;
	
	player_qilin_start* pCmd  = (player_qilin_start*)Msg.dwParam1;
	if(!pCmd)
		return;

	CECPlayer* pPlayer = GetPlayer(pCmd->invitor);
	if(pPlayer)
		pPlayer->OnStartQiLinBinding(pCmd->invitor, pCmd->invitee);

	pPlayer = GetPlayer(pCmd->invitee);
	if(pPlayer)
		pPlayer->OnStartQiLinBinding(pCmd->invitor, pCmd->invitee);

}

void CECPlayerMan::OnMsgPlayerBathStart(const ECMSG& Msg)
{
   using namespace S2C;
   //由于服务器开始传反了，就反着了用
   const player_bath_start* pCmd = (const player_bath_start*)Msg.dwParam1;

   CECPlayer* pPlayer = GetPlayer(pCmd->invitor);
   if (pPlayer)
   {
	   pPlayer->OnStartBinding(pCmd->invitor,pCmd->invitee,true);
       //pPlayer->DoEmote(ROLEEXP_BATH);
   }
   pPlayer = GetPlayer(pCmd->invitee);
   if (pPlayer)
   {
	   pPlayer->OnStartBinding(pCmd->invitor,pCmd->invitee, true);
       //pPlayer->DoEmote(ROLEEXP_BATH);
   }

}
void CECPlayerMan::OnMsgPlayerGetBathAward(const ECMSG& Msg)
{
	using namespace S2C;
	bath_gain_item* pCmd = (bath_gain_item*)Msg.dwParam1;
	CECPlayer* pPlayer = GetPlayer(pCmd->roleid);
	if(pPlayer == NULL) return;
    pPlayer->BubbleGetSpecialItem(pCmd->item_id,pCmd->item_count);
 
}
void CECPlayerMan::OnMsgPlayerDuelResult(const ECMSG& Msg)
{
	using namespace S2C;
	const cmd_duel_result* pCmd = (const cmd_duel_result*)Msg.dwParam1;
	CECGameRun* pGameRun = g_pGame->GetGameRun();

	const ACHAR* szName1 = pGameRun->GetPlayerName(pCmd->id1, false);
	const ACHAR* szName2 = pGameRun->GetPlayerName(pCmd->id2, false);
	if (!szName1 || !szName2)
		return;

	int r1=0, r2=0;

	if (pCmd->result == 0)
	{
		r1 = r2 = 3;
		pGameRun->AddFixedMessage(FIXMSG_DUEL_DRAW, szName1, szName2);
	}
	else if (pCmd->result == 1)
	{
		r1 = 1;
		r2 = 2;
		pGameRun->AddFixedMessage(FIXMSG_DUEL_WIN, szName1, szName2);
	}

	CECPlayer* pPlayer = GetPlayer(pCmd->id1);
	if (pPlayer)
		pPlayer->SetDuelResult(r1);

	pPlayer = GetPlayer(pCmd->id2);
	if (pPlayer)
		pPlayer->SetDuelResult(r2);
}

void CECPlayerMan::OnMsgTankLeader(const ECMSG& Msg)
{
	using namespace S2C;
	const cmd_tank_leader_notify* pCmd = (const cmd_tank_leader_notify*)Msg.dwParam1;

	if (pCmd->idLeader)
	{
		CECPlayer* pPlayer = GetPlayer(pCmd->idLeader);
		if (pPlayer)
			pPlayer->ChangeTankLeader(pCmd->idTank,pCmd->tidTank, true);
	}
	else
	{
		CECHostPlayer* pHost = GetHostPlayer();
		if (pHost->GetBattleTankNum())
			pHost->ChangeTankLeader(pCmd->idTank, pCmd->tidTank, false);

		//	Handle all else players
		PlayerTable::iterator it = m_PlayerTab.begin();
		for (; it != m_PlayerTab.end(); ++it)
		{
			CECElsePlayer* pPlayer = *it.value();
			if (pPlayer->GetBattleTankNum())
				pPlayer->ChangeTankLeader(pCmd->idTank, pCmd->tidTank, false);
		}
	}
}

/**
*@param:ECMSG, 将协议封装之后的消息体
*@return:void
*@usage:角色管理器初步处理所有的传递给角色的命令消息
**/
void CECPlayerMan::OnMsgPlayerScaleShape(const ECMSG& Msg)
{
	using namespace S2C;

	//获得消息体
	const cmd_player_scale* pCmd = (const cmd_player_scale*)Msg.dwParam1;
	if ( !pCmd ) //消息不存在
	{
		return;
	}

	//获得HostPlayer指针
	CECHostPlayer* pHost = g_pGame->GetGameRun()->GetHostPlayer();

	//判断角色id，决定该协议交给Hostplayer处理还是ElsePlayer处理
	if ( pCmd->player_id == pHost->GetCharacterID() ) //主角
	{
		pHost->ProcessMessage(Msg);
	}
	else //ElsePlayer
	{
		//根据roleid找到该ElsePlayer
		CECElsePlayer* pPlayer = SeekOutElsePlayer(pCmd->player_id);
		if (pPlayer)
		{
			pPlayer->ProcessMessage(Msg);
		}
	}
}

void CECPlayerMan::OnMsgPlayerVIPAwardUpdate(const ECMSG& Msg)
{
	using namespace S2C;

	const player_vip_award_update* pCmd = (const player_vip_award_update*)Msg.dwParam1;
	if ( !pCmd ) //消息不存在
	{
		return;
	}

	//获得HostPlayer指针
	CECHostPlayer* pHost = g_pGame->GetGameRun()->GetHostPlayer();
	//判断角色id，决定该协议交给Hostplayer处理还是ElsePlayer处理
	if ( pCmd->role_id == pHost->GetCharacterID() ) //主角
	{
		pHost->ProcessMessage(Msg);
	}
	else //ElsePlayer
	{
		//根据roleid找到该ElsePlayer
		CECElsePlayer* pPlayer = SeekOutElsePlayer(pCmd->role_id);
		if (pPlayer)
		{
			pPlayer->ProcessMessage(Msg);
		}
	}

}


void CECPlayerMan::OnMsgPlayerChangeName(const ECMSG& Msg)
{
	using namespace S2C;
	
	const player_change_name* pCmd = (const player_change_name*)Msg.dwParam1;
	if ( !pCmd ) //消息不存在
	{
		return;
	}
	
	ACString strName((ACHAR*)pCmd->name, pCmd->name_len/2);
	//获得HostPlayer指针
	CECHostPlayer* pHost = g_pGame->GetGameRun()->GetHostPlayer();
	//判断角色id，决定该协议交给Hostplayer处理还是ElsePlayer处理
	if ( pCmd->id == pHost->GetCharacterID() ) //主角
	{
		pHost->SetName(strName);
		GNET::RoleInfo ri = g_pGame->GetGameRun()->GetSelectedRoleInfo();
		ri.name.replace(pCmd->name, pCmd->name_len);
		g_pGame->GetGameRun()->SetSelectedRoleInfo(ri);
	}
	else //ElsePlayer
	{
		//根据roleid找到该ElsePlayer
		CECElsePlayer* pPlayer = SeekOutElsePlayer(pCmd->id);
		if (pPlayer)
		{
			pPlayer->SetName(strName);
		}
	}
	
	g_pGame->GetGameRun()->AddPlayerName(pCmd->id, strName, true);
}

void CECPlayerMan::OnMsgPlayerCollisionSpecialState(const ECMSG& Msg)
{
	using namespace S2C;
	
	const cmd_collision_special_state_info* pCmd = (const cmd_collision_special_state_info*)Msg.dwParam1;
	if ( !pCmd )	//该消息不存在
	{
		return;
	}

	//获得HostPlayer指针
	CECHostPlayer* pHost = g_pGame->GetGameRun()->GetHostPlayer();

	CECPlayer* pPlayer = GetPlayer( pCmd->role_id );
	if ( !pPlayer )
	{
		assert(false);
		return;
	}

	int iCollisionSkillState = pPlayer->GetCollisionSkillState();

	switch ( pCmd->skill_type )	//根据不同的技能类型，进行相应处理
	{
		case COLLISION_SKILL_MAXSPEED:		//加速技能
		{
			if ( pHost && pHost->GetCharacterID() == pCmd->role_id )	//Host player
			{
				A3DVECTOR3 vMaxVelocity = pHost->GetVelocity();
				vMaxVelocity.Normalize();
				vMaxVelocity *= (float)COLLISION_MAX_SPEED;
				if ( pHost->GetHPWorkMan() && pHost->GetHPWorkMan()->GetWork_p1() && (pHost->GetHPWorkMan()->GetWork_p1()->GetWorkID() == CECHPWork::WORK_SPECIAL_MOVE) )
				{
					pHost->SetVelocity( vMaxVelocity );
					pHost->SetCDRInfoClipVelocity( vMaxVelocity );
					CECHPWorkSpecialMove* pWork = (CECHPWorkSpecialMove*)pHost->GetHPWorkMan()->GetWork_p1();
					pWork->SetVelocityAndAcceleration( pHost->GetVelocity(), A3DVECTOR3(1.0f) );
				}
			}
			else
			{
				CECElsePlayer* pElsePlayer = SeekOutElsePlayer( pCmd->role_id );
				if ( pElsePlayer )
				{
					A3DVECTOR3 vMaxVelocity = pElsePlayer->GetVelocity();
					vMaxVelocity.Normalize();
					vMaxVelocity *= (float)COLLISION_MAX_SPEED;
					pElsePlayer->SetSvrVA(vMaxVelocity, vMaxVelocity);
				}
			}			
			break;
		}
		case COLLISION_SKILL_HALFSPEED:		//减速技能
		{
			if ( pHost && pHost->GetCharacterID() == pCmd->role_id )	//Host player
			{
				if ( pHost->GetHPWorkMan() && pHost->GetHPWorkMan()->GetWork_p1() && (pHost->GetHPWorkMan()->GetWork_p1()->GetWorkID() == CECHPWork::WORK_SPECIAL_MOVE) )
				{
					pHost->SetVelocity( pHost->GetVelocity() / 2.0f );
					CECHPWorkSpecialMove* pWork = (CECHPWorkSpecialMove*)pHost->GetHPWorkMan()->GetWork_p1();
					pWork->SetVelocityAndAcceleration( pHost->GetVelocity(), A3DVECTOR3(1.0f) );
					pHost->SetCDRInfoClipVelocity( pHost->GetVelocity() );
				}
			}
			else
			{
				CECElsePlayer* pElsePlayer = SeekOutElsePlayer( pCmd->role_id );
				if ( pElsePlayer )
				{
					pElsePlayer->SetVA(pElsePlayer->GetVelocity() / 2.0f, pElsePlayer->GetAcceleration());
					pElsePlayer->SetCDRInfoClipVelocity( pElsePlayer->GetVelocity() );
				}
			}
			break;
		}
		case COLLISION_SKILL_ASHILL:		//不动如山技能
		{
			if ( pCmd->on )
			{
				iCollisionSkillState |= COLLISION_STATE_ASHILL;
				//pPlayer->SetMass( 10000000 );	//设置为最大质量
			}
			else
			{
				iCollisionSkillState &= ~COLLISION_STATE_ASHILL;
			}
			break;
		}
		case COLLISION_SKILL_ROOT:			//定身技能
		{
			if ( pCmd->on )
			{
				iCollisionSkillState |= COLLISION_STATE_ROOT;
			}
			else
			{
				iCollisionSkillState &= ~COLLISION_STATE_ROOT;
			}
			break;
		}
		case COLLISION_SKILL_CHAOS:			//混乱技能
		{
			if ( pCmd->on )
			{
				iCollisionSkillState |= COLLISION_STATE_CHAOS;
			}
			else
			{
				iCollisionSkillState &= ~COLLISION_STATE_CHAOS;
			}
			break;
		}
		case COLLISION_SKILL_VOID:			//虚无技能
		{
			if ( pCmd->on )
			{
				iCollisionSkillState |= COLLISION_STATE_VOID;
			}
			else
			{
				iCollisionSkillState &= ~COLLISION_STATE_VOID;
			}			
			break;
		}
		default:
			break;
	}

	pPlayer->SetCollisionSkillState( iCollisionSkillState );
}

void CECPlayerMan::OnMsgPlayerXYDarkLight(const ECMSG& Msg)
{
	//TODO:
	using namespace S2C;

	switch ( Msg.dwParam2 )
	{
		case PLAYER_DARK_LIGHT_VALUE:
		{
			const cmd_player_dark_light_value* pCmd = (const cmd_player_dark_light_value*)Msg.dwParam1;
			if ( !pCmd )
			{
				assert(false);
				return;
			}

			CECPlayer* pPlayer = GetPlayer( pCmd->id );
			if ( pPlayer )
			{
				pPlayer->SetBlackAndLightValue( pCmd->dark_light_value );
			}
			break;
		}
		case PLAYER_DARK_LIGHT_STATE:
		{
			//TODO:
			const cmd_player_dark_light_state* pCmd = (const cmd_player_dark_light_state*)Msg.dwParam1;
			if ( !pCmd )
			{
				assert(false);
				return;

			}

			CECPlayer* pPlayer = GetPlayer( pCmd->id );
			if ( pPlayer )
			{
				pPlayer->SetDarkLightState( pCmd->dark_light_state );
				//-----------------------------------------------------------------
				//设置强制变换装备掩码
				DWORD dwForceChangeMask = 0;
				dwForceChangeMask |= ( 1 << EQUIPIVTR_HEAD );
				dwForceChangeMask |= ( 1 << EQUIPIVTR_BODY );
				dwForceChangeMask |= ( 1 << EQUIPIVTR_FOOT );
				pPlayer->SetForceChangeMask( dwForceChangeMask );
				//-----------------------------------------------------------------
			}

			//获得HostPlayer指针
			CECHostPlayer* pHost = g_pGame->GetGameRun()->GetHostPlayer();
			assert( pHost != NULL );
			//判断角色id，决定该协议交给Hostplayer处理还是ElsePlayer处理
			if ( pCmd->id == pHost->GetCharacterID() ) //主角
			{
				pHost->UpdateEquipSkins();					//HostPlayer更新装备
			}
			else
			{
				CECElsePlayer* pElsePlayer = SeekOutElsePlayer(pCmd->id);
				if ( pElsePlayer )
				{
					pElsePlayer->LoadPlayerEquipments();	//ElsePlayer更新装备
				}
			}

			//
			break;
		}
		default:
			break;
	}
}

//
void CECPlayerMan::OnMsgDirVisibleState(const ECMSG& Msg)
{
	//TODO:
	using namespace S2C;

	const cmd_dir_visible_state* pCmd = (const cmd_dir_visible_state*)Msg.dwParam1;
	if ( !pCmd )
	{
		assert(false);
		return;
	}
	
	CECPlayer* pPlayer = GetPlayer( pCmd->role_id );
	if ( !pPlayer )
	{
		assert(false);
		return;
	}

	pPlayer->SetDirVisibleStates( pCmd->state_id, pCmd->dir, pCmd->on );
}

void CECPlayerMan::OnMsgPlayerWingChangeColor(const ECMSG& Msg)
{
	using namespace S2C;

	const cmd_player_wing_change_color* pCmd = (const cmd_player_wing_change_color*)Msg.dwParam1;
	if ( !pCmd )
	{
		assert(false);
		return;
	}

	CECPlayer* pPlayer = GetPlayer( pCmd->id );
	if ( !pPlayer )
	{
		assert(false);
		return;
	}
	pPlayer->SetWingChangeColor( pCmd->wing_color );
	DWORD dwForceChangeMask = 0;
	dwForceChangeMask |= ( 1 << EQUIPIVTR_WING );
	pPlayer->SetForceChangeMask( dwForceChangeMask );

	//获得HostPlayer指针
	CECHostPlayer* pHost = g_pGame->GetGameRun()->GetHostPlayer();
	assert( pHost != NULL );
	//判断角色id，决定该协议交给Hostplayer处理还是ElsePlayer处理
	if ( pCmd->id == pHost->GetCharacterID() ) //主角
	{
		pHost->UpdateEquipSkins();	//更新装备
	}
	else
	{
		CECElsePlayer* pElsePlayer = SeekOutElsePlayer(pCmd->id);
		if ( pElsePlayer )
		{
			pElsePlayer->LoadPlayerEquipments();
		}
	}
}

void CECPlayerMan::OnMsgPlayerXYDarkLightSpirit(const ECMSG& Msg)
{
	using namespace S2C;
	cmd_player_dark_light_spirit* pCmd = (cmd_player_dark_light_spirit*)Msg.dwParam1;
	if(!pCmd)
		return;

	CECPlayer* pPlayer = GetPlayer(pCmd->id);
	if (pPlayer)
	{
		for(int i=0;i<3;i++)
			pPlayer->SetDarkLightSpirit(i, pCmd->dark_light_spirit[i]);
	}

	return;
}

void CECPlayerMan::OnMsgPlayerXYMirrorImageCnt(const ECMSG& Msg)
{
	using namespace S2C;
	cmd_mirror_image_cnt* pCmd = (cmd_mirror_image_cnt*)Msg.dwParam1;
	if(!pCmd)
		return;
	
	CECPlayer* pPlayer = GetPlayer(pCmd->id);
	if (pPlayer)
	{
		if(g_pGame->GetConfigs()->GetVideoSettings().nEffect != 1)
			pPlayer->SetMirrorModelCnt(pCmd->mirror_image_cnt);
		else
			pPlayer->SetMirrorModelCnt(0);   //游戏最低配置时，不加载、显示分身模型
	}

	return;
}

void CECPlayerMan::OnMsgPlayerPuppetForm(const ECMSG& Msg)
{
	using namespace S2C;

	puppet_form* pCmd = (puppet_form*)Msg.dwParam1;
	if(!pCmd)
		return;

	CECPlayer* pPlayer = GetPlayer(pCmd->idplayer);
	if(pPlayer)
	{
		if(pCmd->on)
			pPlayer->SummonPupPet(pCmd->idpuppet);
		else
		{
			if(pCmd->idpuppet == pPlayer->GetPupPetId())
				pPlayer->SummonPupPet(0);
		}
	}
}

void CECPlayerMan::OnMsgPlayerPuppetFormChange(const ECMSG& Msg)
{
	using namespace S2C;

	player_change_puppet_form* pCmd = (player_change_puppet_form*)Msg.dwParam1;
	if(!pCmd)
		return;

	CECPlayer* pPlayer = GetPlayer(pCmd->idPlayer);
	if(pPlayer)
		pPlayer->PuppetEnterFightState();
}

void CECPlayerMan::OnMsgPlayerBlowOff(const ECMSG& Msg)
{
	using namespace S2C;

	blow_off* pCmd = (blow_off*)Msg.dwParam1;
	if(!pCmd)
		return;

	CECHostPlayer* pHost = GetHostPlayer();
	if ( pCmd->roleid == pHost->GetCharacterID() ) 
	{
		pHost->UpdateFollowCamera(false, 800);
		pHost->GetHPWorkMan()->FinishAllWork(true);

		CECHPWorkBlewUp* pWork = (CECHPWorkBlewUp*)GetHostPlayer()->GetHPWorkMan()->CreateWork(CECHPWork::WORK_BLEW_UP);
		pWork->PrepareBlew(800, 15.0f);
		pHost->GetHPWorkMan()->StartWork_p1(pWork);
	}
	else 
	{
		CECElsePlayer* pPlayer = SeekOutElsePlayer(pCmd->roleid);
		if (pPlayer)
		{
			pPlayer->PrepareBlewUp(800, 15.0f);
				pPlayer->StartWork(CECElsePlayer::WT_NORMAL, CECElsePlayer::WORK_BLEWUP);
		}
	}
}

void CECPlayerMan::OnMsgPlayerBeCharge(const ECMSG& Msg)
{
	using namespace S2C;

	object_be_charge_to* pCmd = (object_be_charge_to*)Msg.dwParam1;
	if(!pCmd)
		return;


	CECHostPlayer* pHost = GetHostPlayer();
	if(pCmd->idPlayer == pHost->GetCharacterID())
	{
		if(pHost->IsHangerOn() || pHost->IsMultiRideMember())
			return;

		if(pCmd->type == 6)
		{
			CECHPWorkParabolic* pWork = (CECHPWorkParabolic*)GetHostPlayer()->GetHPWorkMan()->CreateWork(CECHPWork::WORK_PARABOLIC);
			pWork->PrepareParabolic(1500, 5, pCmd->destpos);
			pHost->GetHPWorkMan()->StartWork_p2(pWork);
		}
		else if(pCmd->type == 1 || pCmd->type == 3 || pCmd->type == 4 || pCmd->type == 5)
		{

			pHost->SetBeChargeType(pCmd->type);
			
			CDR_INFO*	pCDRInfo = pHost->GetCDRInfo();
			CECHostMove MoveCtrl = pHost->GetMoveControl();
			pCDRInfo->vTPNormal	 = MoveCtrl.m_vFlashTPNormal;
			pCDRInfo->bOnSurface = MoveCtrl.m_bFlashOnGnd;
			pCDRInfo->bCanStay	 = MoveCtrl.m_bFlashCanStay;

			CECHPWorkFMove* pWork = (CECHPWorkFMove*)pHost->GetHPWorkMan()->CreateWork(CECHPWork::WORK_FLASHMOVE);
			pWork->PrepareMove(pCmd->destpos, 1.0f);
			pHost->GetHPWorkMan()->StartWork_p2(pWork);
		}
		else if(pCmd->type == 0)
		{
			pHost->SetPos(pCmd->destpos);

			pHost->UpdateFollowCamera(false, 10);

			CECObject* pObject = g_pGame->GetGameRun()->GetWorld()->GetObject(pCmd->target_id, 0);
			if (!pObject)
				return;
			pHost->SetDirAndUp(pObject->GetDir(), g_vAxisY);
		}
		else if(pCmd->type == 2)
		{
			pHost->SetPos(pCmd->destpos);

			pHost->UpdateFollowCamera(false, 10);
		}
	}
	else 
	{
		CECElsePlayer* pPlayer = SeekOutElsePlayer(pCmd->idPlayer);
		if(pPlayer)
		{
			if(pPlayer->IsHangerOn() || pPlayer->IsMultiRideMember())
				return;

			if(pCmd->type == 6)
			{
				pPlayer->SetServerPos(pCmd->destpos);
				pPlayer->PrepareParabolic(1500, 5, pCmd->destpos);
				OtherPlayer_Move_Info* pCdr = pPlayer->GetOtherPlayerMoveInfo();
				pCdr->bTraceGround = false;
				pPlayer->StartWork(CECElsePlayer::WT_NORMAL, CECElsePlayer::WORK_PARABOLIC);
			}
			else if(pCmd->type == 1 && pCmd->type == 3 || pCmd->type == 4 || pCmd->type == 5)
			{			
				float fSpeed;
				if(pCmd->type == 1)
					fSpeed = 25.0f;
				if(pCmd->type == 4)
					fSpeed = 5.0f;
				else
					fSpeed = 10.0f;
				pPlayer->SetServerPos(pCmd->destpos);
				pPlayer->SetSpeed(fSpeed);
				pPlayer->StartWork(CECElsePlayer::WT_NORMAL, CECElsePlayer::WORK_FLASHMOVE);
			}
			else if(pCmd->type == 0)
			{
				pPlayer->SetPos(pCmd->destpos);
				pPlayer->SetServerPos(pCmd->destpos);

				CECObject* pObject = g_pGame->GetGameRun()->GetWorld()->GetObject(pCmd->target_id, 0);
				if (!pObject)
					return;
				pPlayer->SetDirAndUp(pObject->GetDir(), g_vAxisY);
			}
			else if(pCmd->type == 2)
			{
				pPlayer->SetPos(pCmd->destpos);
				pPlayer->SetServerPos(pCmd->destpos);
			}
			
		}
	}
}


void CECPlayerMan::OnMsgPlayerBloodPool(const ECMSG& Msg)
{
	using namespace S2C;

	notify_bloodpool_status* pCmd = (notify_bloodpool_status*)Msg.dwParam1;
	if(!pCmd)
		return;

	CECPlayer* pPlayer = GetPlayer(pCmd->idPlayer);
	if(pPlayer)
		pPlayer->SetBloodPool(pCmd->on, pCmd->cur_hp, pCmd->max_hp);
}

void CECPlayerMan::OnMsgPlayerTeleportInfo(const ECMSG& Msg)
{
	using namespace S2C;
	
	teleport_skill_info* pCmd = (teleport_skill_info*)Msg.dwParam1;
	if(!pCmd)
		return;
	
	CECHostPlayer* pHost = GetHostPlayer();

	if(pHost && pCmd->idPlayer == pHost->GetCharacterID())
	{
		pHost->SetTeleportInfo(pCmd->idskill, pCmd->teleportnum);
	}

	CECNPCMan* pNPCMan = g_pGame->GetGameRun()->GetWorld()->GetNPCMan();
	if(pNPCMan)
		pNPCMan->SetNPCTeleprot(pCmd->npc_tid, pCmd->teleportnum);
}

//	Transmit message
bool CECPlayerMan::TransmitMessage(const ECMSG& Msg)
{
	using namespace S2C;
	using namespace GNET;

	int cid = 0;
	CECHostPlayer* pHost = GetHostPlayer();

	switch (Msg.dwMsg)
	{
	case MSG_PM_PLAYEREQUIPDATA:
		
		if (Msg.dwParam2 == EQUIP_DATA)
			cid = ((cmd_equip_data*)Msg.dwParam1)->idPlayer;
		else if (Msg.dwParam2 == EQUIP_DATA_CHANGED)
			cid = ((cmd_equip_data_changed*)Msg.dwParam1)->idPlayer;
		else if (Msg.dwParam2 == CLONE_PLAYER_EQUIP)
			cid = ((clone_player_equip*)Msg.dwParam1)->id;
		break;

	case MSG_PM_PLAYERBASEINFO:
		
		cid = ((PlayerBaseInfo_Re*)Msg.dwParam1)->player.id;
		break;
	case MSG_PM_PLAYERBASEINFO2:
		cid = ((PlayerBaseInfo2_Re*)Msg.dwParam1)->cloneid;
		break;
	case MSG_PM_PLAYERCUSTOM:

		cid = ((GetCustomData_Re*)Msg.dwParam1)->cus_roleid;
		break;

	case MSG_PM_PLAYERFLY:
		
		if (Msg.dwParam2 == OBJECT_TAKEOFF)
			cid = ((cmd_object_takeoff*)Msg.dwParam1)->object_id;
		else	//	OBJECT_LANDING
			cid = ((cmd_object_landing*)Msg.dwParam1)->object_id;

		break;

	case MSG_PM_PLAYERSITDOWN:
		
		if (Msg.dwParam2 == OBJECT_SIT_DOWN)
			cid = ((cmd_object_sit_down*)Msg.dwParam1)->id;
		else	//	OBJECT_STAND_UP
			cid = ((cmd_object_stand_up*)Msg.dwParam1)->id;

		break;

	case MSG_PM_CASTSKILL:
		
		switch (Msg.dwParam2)
		{
		case OBJECT_CAST_SKILL:			cid = ((cmd_object_cast_skill*)Msg.dwParam1)->caster;			break;
		case OBJECT_CAST_INSTANT_SKILL:	cid = ((cmd_object_cast_instant_skill*)Msg.dwParam1)->caster;	break;
		case OBJECT_CAST_POS_SKILL:		cid = ((cmd_object_cast_pos_skill*)Msg.dwParam1)->caster;		break;
		case SKILL_INTERRUPTED:			cid = ((cmd_skill_interrupted*)Msg.dwParam1)->caster;			break;
		case SKILL_CONTINUE:			cid = ((skill_continue*)Msg.dwParam1)->caster;					break;
		case PLAYER_CHARGE:				cid = ((player_charge*)Msg.dwParam1)->id;						break;
		case PLAYER_STOP_SKILL:			cid = ((player_stop_skill*)Msg.dwParam1)->player_id;			break; //Added 2011-08-30.
		}
		
		break;

	case MSG_PM_CHANGENAMECOLOR:
		
		break;

	case MSG_PM_PLAYERROOT:

		ASSERT(Msg.dwParam2 == OBJECT_ROOT);
		cid = ((cmd_object_root*)Msg.dwParam1)->id;
		break;

	case MSG_PM_PLAYERATKRESULT:

		ASSERT(Msg.dwParam2 == OBJECT_ATTACK_RESULT);
		cid = ((cmd_object_atk_result*)Msg.dwParam1)->attacker_id;
		break;

	case MSG_PM_STARTATTACK:
		ASSERT(Msg.dwParam2 == OBJECT_STARTATTACK);
		cid = ((cmd_object_start_attack*)Msg.dwParam1)->idAttacker;
		break;

	case MSG_PM_PLAYERDOEMOTE:
		
		if (Msg.dwParam2 == OBJECT_DO_EMOTE)
			cid = ((cmd_object_do_emote*)Msg.dwParam1)->id;
		else	//	Msg.dwParam2 == OBJECT_EMOTE_RESTORE
			cid = ((cmd_object_emote_restore*)Msg.dwParam1)->id;

		break;

	case MSG_PM_PLAYERUSEITEM:

		switch (Msg.dwParam2)
		{
		case OBJECT_START_USE:		cid = ((cmd_object_start_use*)Msg.dwParam1)->user;		break;
		case OBJECT_CANCEL_USE:		cid = ((cmd_object_cancel_use*)Msg.dwParam1)->user;		break;
		case OBJECT_USE_ITEM:		cid = ((cmd_object_use_item*)Msg.dwParam1)->user;		break;
		case OBJECT_START_USE_T:	cid = ((cmd_object_start_use_t*)Msg.dwParam1)->user;	break;
		}

		break;

	case MSG_PM_USEITEMWITHDATA:

		cid = ((object_use_item_with_arg*)Msg.dwParam1)->user;
		break;


	case MSG_PM_ENCHANTRESULT:

		cid = ((cmd_enchant_result*)Msg.dwParam1)->caster;
		break;

	case MSG_PM_PLAYERDOACTION:

		cid = ((cmd_obj_do_action*)Msg.dwParam1)->id;
		break;

	case MSG_PM_PLAYERSKILLRESULT:

		cid = ((cmd_object_skill_attack_result*)Msg.dwParam1)->attacker_id;
		break;

	case MSG_PM_PLAYERADVDATA:

		if (Msg.dwParam2 == PLAYER_SET_ADV_DATA)
			cid = ((cmd_player_set_adv_data*)Msg.dwParam1)->id;
		else
		{
			ASSERT(Msg.dwParam2 == PLAYER_CLR_ADV_DATA);
			cid = ((cmd_player_clr_adv_data*)Msg.dwParam1)->id;
		}

		break;

	case MSG_PM_PLAYERINTEAM:
		
		cid = ((cmd_player_in_team*)Msg.dwParam1)->id;
		break;

	case MSG_PM_PLAYERGATHER:

		if (Msg.dwParam2 == PLAYER_GATHER_START)
			cid = ((cmd_player_gather_start*)Msg.dwParam1)->pid;
		else
		{
			ASSERT(Msg.dwParam2 == PLAYER_GATHER_STOP);
			cid = ((cmd_player_gather_stop*)Msg.dwParam1)->pid;
		}

		break;

	case MSG_PM_PLAYERCHGSHAPE:

		cid = ((cmd_player_chgshape*)Msg.dwParam1)->idPlayer;
		break;

	case MSG_PM_BOOTHOPT:
	{
		switch (Msg.dwParam2)
		{
		case SELF_OPEN_BOOTH:
		case BOOTH_TRADE_SUCCESS:
		case OPEN_BOOTH_TEST:
		case PLAYER_BOOTH_INFO:
		case BOOTH_SELL_ITEM:		cid = pHost->GetCharacterID();	break;
		case PLAYER_OPEN_BOOTH:		cid = ((cmd_player_open_booth*)Msg.dwParam1)->pid;	break;
		case PLAYER_CLOSE_BOOTH:	cid = ((cmd_player_close_booth*)Msg.dwParam1)->pid;	break;
		case BOOTH_NAME:			cid = ((cmd_booth_name*)Msg.dwParam1)->pid;	break;
		}

		break;
	}
	case MSG_PM_PLAYERTRAVEL:

		switch (Msg.dwParam2)
		{
		case PLAYER_START_TRAVEL:	cid = ((cmd_player_start_travel*)Msg.dwParam1)->pid; break;
		case HOST_START_TRAVEL:		cid = pHost->GetCharacterID(); break;
		case PLAYER_END_TRAVEL:		cid = ((cmd_player_end_travel*)Msg.dwParam1)->pid; break;
		}

		break;

	case MSG_PM_PLAYERPVP:

		switch (Msg.dwParam2)
		{
		case PLAYER_PVP_NO_PROTECT:	cid = ((cmd_player_enable_pvp*)Msg.dwParam1)->who;	break;
		case PLAYER_DISABLE_PVP:	cid = ((cmd_player_disable_pvp*)Msg.dwParam1)->who;	break;
		case HOST_PVP_COOLDOWN:
		case HOST_ENABLE_FREEPVP:	cid = pHost->GetCharacterID();	break;
		case PVP_COMBAT:			cid = ((cmd_pvp_combat*)Msg.dwParam1)->idPlayer;	break;
		}

		break;

	case MSG_PM_FASHIONENABLE:

		cid = ((cmd_player_enable_fashion*)Msg.dwParam1)->idPlayer;
		break;

	case MSG_PM_PLAYEREFFECT:

		if (Msg.dwParam2 == PLAYER_ENABLE_EFFECT)
			cid = ((cmd_player_enable_effect*)Msg.dwParam1)->id;
		else
		{
			ASSERT(Msg.dwParam2 == PLAYER_DISABLE_EFFECT);
			cid = ((cmd_player_disable_effect*)Msg.dwParam1)->id;
		}

		break;

	case MSG_PM_CHANGEFACE:

		cid = ((cmd_player_chg_face*)Msg.dwParam1)->idPlayer;
		break;

	case MSG_PM_PLAYERBINDOPT:

		switch (Msg.dwParam2)
		{
		case PLAYER_BIND_REQUEST:
		case PLAYER_BIND_INVITE:
		case PLAYER_BIND_REQUEST_REPLY:
		case PLAYER_BIND_INVITE_REPLY:	cid = pHost->GetCharacterID();	break;
		case PLAYER_BIND_STOP:			cid = ((cmd_player_bind_stop*)Msg.dwParam1)->who;	break;
		}

		break;

   case MSG_PM_BATH_OPT:

        switch(Msg.dwParam2)
		{
		 case PLAYER_BATH_INVITE:
		 case PLAYER_BATH_INVITE_REPLAY:
		 case ENTER_BATH_WORLD:
		 case LEAVE_BATH_WORLD: 	cid = pHost->GetCharacterID();	break;
		 case PLAYER_BATH_STOP:    cid = ((player_bath_stop*)Msg.dwParam1)->who; break;
		   
		}
        break;
   case MSG_PM_QILIN_BINDOPT:
	   switch(Msg.dwParam2)
	   {
	   case PLAYER_QILIN_INVITE:
	   case PLAYER_QILIN_INVITE_REPLY:	cid = pHost->GetCharacterID(); break;
	   case PLAYER_QILIN_STOP:			cid = ((player_qilin_stop*)Msg.dwParam1)->who; break;
	   case PLAYER_QILIN_DISCONNECT:	cid = ((player_qilin_disconnet*)Msg.dwParam1)->who; break;
	   case PLAYER_QILIN_RECONNECT:		cid = ((player_qilin_reconnect*)Msg.dwParam1)->who; 
	   }
	   break;
	case MSG_PM_PLAYERMOUNT:

		cid = ((cmd_player_mounting*)Msg.dwParam1)->id;
		break;

	case MSG_PM_PLAYERDUELOPT:

		switch (Msg.dwParam2)
		{
		case DUEL_RECV_REQUEST:
		case DUEL_REJECT_REQUEST:
		case DUEL_PREPARE:
		case DUEL_CANCEL:
		case HOST_DUEL_START:	cid = pHost->GetCharacterID();	break;
		case PLAYER_DUEL_START:	cid = ((cmd_player_duel_start*)Msg.dwParam1)->player_id;	break;
		case DUEL_STOP:			cid = ((cmd_duel_stop*)Msg.dwParam1)->player_id;	break;
		}

		break;

	case MSG_PM_PLAYERLEVEL2:

		cid = ((cmd_task_deliver_level2*)Msg.dwParam1)->id_player;
		break;
	// 多人骑乘
	case MSG_PM_MULTIBINDOPT:
		switch(Msg.dwParam2)
		{
		case PLAYER_MULTI_BIND_INVITE:
		case PLAYER_MULTI_BIND_INVITE_REPLY: cid = pHost->GetCharacterID(); break;
		case PLAYER_MULTI_BIND_JOIN:		 cid = ((cmd_player_multi_bind_join*)Msg.dwParam1)->leader; break;
 		case PLAYER_MULTI_BIND_LEAVE:		 cid = ((cmd_player_multi_bind_leave*)Msg.dwParam1)->leader; break;
		case PLAYER_MULTI_BIND_KICK:		 cid = ((cmd_player_multi_bind_kick*)Msg.dwParam1)->leader;  break;
		}

		break;

	case MSG_PM_CARRIER:
		switch(Msg.dwParam2)
		{
		case PLAYER_ENTER_CARRIER:			cid = ((cmd_player_enter_carrier*)Msg.dwParam1)->player_id;		break;
		case PLAYER_LEAVE_CARRIER:			cid = ((cmd_player_leave_carrier*)Msg.dwParam1)->player_id;		break;
		case PLAYER_MOVE_ON_CARRIER:		cid = ((cmd_player_move_on_carrier*)Msg.dwParam1)->player_id;	break;
 		case PLAYER_STOP_ON_CARRIER:		cid = ((cmd_player_stop_on_carrier*)Msg.dwParam1)->player_id;	break;
		}

		break;

	case MSG_PM_START_INTERACT:
		cid = ((cmd_player_start_interact*) Msg.dwParam1)->player_id;
		break;

	case MSG_PM_IN_INTERACT:
		cid = ((cmd_player_in_interact*) Msg.dwParam1)->player_id;
		break;

	case MSG_PM_END_INTERACT:
		cid = ((cmd_player_end_interact*) Msg.dwParam1)->player_id;
		break;

	case MSG_PM_FINISH_INTERACT:
		cid = ((cmd_player_finish_interact*) Msg.dwParam1)->player_id;
		break;

	case MSG_PM_CANCEL_INTERACT:
		cid = ((cmd_player_cancel_interact*) Msg.dwParam1)->player_id;
		break;

	case MSG_HST_INVITE_ACTIVE_EMOTE:
		cid = ((invite_active_emote*) Msg.dwParam1)->playerid;
		break;
	
	case MSG_HST_REPLY_ACTIVE_EMOTE:
		cid = ((reply_active_emote*) Msg.dwParam1)->playerid;
		break;

	case MSG_PM_PLAY_ACTIVE_EMOTE:
		cid = ((play_active_emote*) Msg.dwParam1)->playerid1;
		break;
	
	case MSG_PM_STOP_ACTIVE_EMOTE:
		cid = ((stop_active_emote*) Msg.dwParam1)->playerid1;
		break;

	case MSG_PM_TRAVEL_FREE:
		
		if (Msg.dwParam2 == PLAYER_START_TRAVEL_AROUND)
			cid = ((player_start_travel_around*)Msg.dwParam1)->player_id;
		else
		{
			ASSERT(Msg.dwParam2 == PLAYER_STOP_TRAVEL_AROUND);
			cid = ((player_stop_travel_around*)Msg.dwParam1)->player_id;
		}
		break;

	default:
		ASSERT(0);
		return false;
	}

	if (!cid)
	{
		ASSERT(cid);
		return false;
	}

	if (cid == pHost->GetCharacterID())
		pHost->ProcessMessage(Msg);
	else
	{
		CECElsePlayer* pPlayer = SeekOutElsePlayer(cid);
		if (pPlayer)
			pPlayer->ProcessMessage(Msg);
	}

	return true;
}

//	Tick host player separately
//	pViewport: viewport used by host player, can be NULL
bool CECPlayerMan::TickHostPlayer(DWORD dwDeltaTime, CECViewport* pViewport)
{
	CECHostPlayer* pHost = GetHostPlayer();
	if (!pHost)
		return true;

	pHost->Tick(dwDeltaTime);

	if (pViewport)
	{
		//	Update camera position and direction
#ifdef BACK_VERSION
		A3DCamera* pCamera = pViewport->GetA3DCamera();
#else
		A3DCameraBase* pCamera = pViewport->GetA3DCameraBase();
#endif
		A3DCoordinate* pCameraCoord = pHost->GetCameraCoord();
		pCamera->SetPos(pCameraCoord->GetPos());
		pCamera->SetDirAndUp(pCameraCoord->GetDir(), pCameraCoord->GetUp());
	}

	return true;
}

//	Tick routine
bool CECPlayerMan::Tick(DWORD dwDeltaTime)
{
	DWORD dwRealTime = g_pGame->GetRealTickTime();

	//	Deliver loaded player models
	DeliverLoadedPlayerModels();

	//	Update all the players collision detection.
//	UpdateAllCollisionStates(dwDeltaTime); //Canceled 2012-07-20.

	//	Tick all players
	PlayerTable::iterator it = m_PlayerTab.begin();
	for (; it != m_PlayerTab.end(); ++it)
	{
		CECElsePlayer* pPlayer = *it.value();
		pPlayer->Tick(dwDeltaTime);
	}

	//-------------------------------------------------------------------------
	//	在每个ElsePlayer做完Tick后（也就做完了他们相应的TickWork），清除碰撞状态
	//	因为：这个碰撞状态时每一帧都更新的 Added 2012-09-01.
	for ( it = m_PlayerTab.begin(); it != m_PlayerTab.end(); ++it )
	{
		CECElsePlayer* pPlayer = *it.value();
		if ( pPlayer )
		{
			pPlayer->SetCollisionState( false );
		}
	}
	//-------------------------------------------------------------------------

	//	Update players in various ranges
	UpdatePlayerInRanges(dwDeltaTime);

	//	Tick Login Players here, if there are
	int nNumLoginPlayers = m_LoginPlayers.size();
	for(int i=0; i < nNumLoginPlayers; i++)
	{
		m_LoginPlayers[i]->Tick(dwDeltaTime);
	}

	//	Clear up player data cache
	if (m_CacheCnt.IncCounter(dwRealTime))
	{
		m_CacheCnt.Reset();
		ClearUpPlayerCache();
	}

	UpdateUnknownElsePlayers();

	//	Update player's tank leader state if host is in battle
	CECHostPlayer* pHost = GetHostPlayer();
	if (pHost && pHost->IsInBattle() && m_TLTimeCnt.IncCounter(dwRealTime))
	{
		m_TLTimeCnt.Reset();

		if (pHost->GetBattleTankNum())
			pHost->UpdateTankLeader();

		it = m_PlayerTab.begin();
		for (; it != m_PlayerTab.end(); ++it)
		{
			CECElsePlayer* pPlayer = *it.value();
			if (pPlayer->GetBattleTankNum())
				pPlayer->UpdateTankLeader();
		}
	}

	// test code
	/*
	if (GetHostPlayer())
	{
		PlayerTable::iterator it = m_PlayerTab.begin();
		for (; it != m_PlayerTab.end(); ++it)
		{
			CECElsePlayer* pPlayer = *it.value();
			if ((pPlayer->GetPos() - GetHostPlayer()->GetPos()).Normalize() < 2.0f)
			{
				if (pPlayer->GetAttachMode() == CECPlayer::enumAttachNone)
				{
					CECPlayer* p1 = GetHostPlayer();
					CECPlayer* p2 = pPlayer;

					if (p1->GetGender() != p2->GetGender())
					{
						if (p1->GetGender() == GENDER_MALE)
							p1->AttachBuddy(p2->GetPlayerInfo().cid);
						else
							p2->AttachBuddy(p1->GetPlayerInfo().cid);
						break;
					}
				}
			}
		}
	}
	*/

	return true; 
}

//	Render routine
bool CECPlayerMan::Render(CECViewport* pViewport)
{
	//	Set light for booth and hint model
	A3DSkinModel::LIGHTINFO LightInfo;
	memset(&LightInfo, 0, sizeof (LightInfo));
	const A3DLIGHTPARAM& lp = g_pGame->GetDirLight()->GetLightparam();

	LightInfo.colAmbient	= g_pGame->GetA3DDevice()->GetAmbientValue();
	LightInfo.vLightDir		= lp.Direction;
	LightInfo.colDirDiff	= lp.Diffuse;
	LightInfo.colDirSpec	= lp.Specular;
	LightInfo.bPtLight		= false;

	if (m_pBoothModel_M)
		m_pBoothModel_M->SetLightInfo(LightInfo);

	if (m_pBoothModel_F)
		m_pBoothModel_F->SetLightInfo(LightInfo);

	abase::hash_map<int, CECModel*>::iterator it = m_BoothModelMap.begin();
		
	for(; it != m_BoothModelMap.end(); ++it)
		if(it->second)
			it->second->GetA3DSkinModel()->SetLightInfo(LightInfo);

	bool bUseHintModel = g_pGame->GetConfigs()->GetVideoSettings().bModelLimit;

	//	Render all else players
	CECHostPlayer* pHost = GetHostPlayer();
	CECElsePlayer* pHHElsePlayer = NULL;	//	The player will be high-light rendered
	int idHoverObject = pHost ? pHost->GetCursorHoverObject() : 0;
	bool bRenderElse = pHost ? !pHost->IsChangingFace() : true;
	bool bRenderSelf = true;

	bool bIsPlayingCutscene = false;

	if( pHost && pHost->GetCutscene())
		bIsPlayingCutscene = pHost->GetCutscene()->IsPlayingCutscene();
		
	if(bIsPlayingCutscene)
	{
		if(!pHost->GetCutscene()->IsElsePlayerShow())
			bRenderElse = false;

		if(!pHost->GetCutscene()->IsSelfShow())
			bRenderSelf = false;
	}
#ifdef BACK_VERSION
	A3DTerrainWater* pWater = g_pGame->GetGameRun()->GetWorld()->GetTerrainWater();
	A3DSkinRender *psrold = g_pGame->GetA3DEngine()->GetA3DSkinMan()->GetCurSkinRender();
#else
	A3DTerrainWater2* pWater = g_pGame->GetGameRun()->GetWorld()->GetTerrainWater();
	A3DSkinRenderBase *psrold = g_pGame->GetA3DEngine()->GetA3DSkinMan()->GetCurSkinRender();
#endif

	if (bRenderElse)
	{
		PlayerTable::iterator it = m_PlayerTab.begin();
		for (; it != m_PlayerTab.end(); ++it)
		{
			CECElsePlayer* pPlayer = *it.value();
			pPlayer->SetRenderFamily(idHoverObject == pPlayer->GetCharacterID());

			if (idHoverObject == pPlayer->GetCharacterID()
			 && pPlayer->GetAttachMode() == CECPlayer::enumAttachNone
			 && !pPlayer->IsRidingOnPet())
				pHHElsePlayer = pPlayer;
			else
			{
				if (pPlayer->GetBoothState() == 2)
					pPlayer->RenderForBooth(pViewport);
				else if (bUseHintModel && (pPlayer->GetUseHintModelFlag() || !pPlayer->IsAllResReady()))
					pPlayer->RenderForHint(pViewport);
				else
					pPlayer->Render(pViewport);
			}
		}

		if (pHHElsePlayer)
		{
			if (pHHElsePlayer->GetBoothState() == 2)
				pHHElsePlayer->RenderForBooth(pViewport, 0, true);
			else if (bUseHintModel && (pHHElsePlayer->GetUseHintModelFlag() || !pHHElsePlayer->IsAllResReady()))
				pHHElsePlayer->RenderForHint(pViewport, 0, true);
			else
				pHHElsePlayer->RenderHighLight(pViewport);
		}
	}

	//	Render host player
	if (pHost && bRenderSelf)
	{
		if (pHost->GetBoothState() != 2)
			pHost->Render(pViewport);
		else
			pHost->RenderForBooth(pViewport);
	}

	//	Render Login Players here, if there are
	int nNumLoginPlayers = m_LoginPlayers.size();
	for(int i=0; i<nNumLoginPlayers; i++)
	{
		m_LoginPlayers[i]->Render(pViewport);
	}

	if (g_pGame->GetGameRun()->GetGameState() == CECGameRun::GS_GAME)
	{
		//	Flush title font
		A3DFont* pFont = g_pGame->GetFont(RES_FONT_TITLE);
		pFont->Flush();

		//	Flush emotion decals
		CECGameUIMan* pGameUI = g_pGame->GetGameRun()->GetUIManager()->GetInGameUIMan();
		A2DSprite* pFaceImg = NULL;
		pGameUI->GetEmotionList(&pFaceImg, NULL);
		if (pFaceImg)
			pFaceImg->GetTextures()->GetSpriteBuffer()->FlushBuffer();
	}

	return true;
}

//	RenderForReflected routine
bool CECPlayerMan::RenderForReflect(CECViewport * pViewport)
{
	//	All player should be reflect visible
	//	Render all players
	PlayerTable::iterator it = m_PlayerTab.begin();
	for (; it != m_PlayerTab.end(); ++it)
	{
		CECElsePlayer* pPlayer = *it.value();
		if (pPlayer->GetBoothState() != 2)
			pPlayer->Render(pViewport, CECObject::RD_REFLECT);
		else
			pPlayer->RenderForBooth(pViewport, CECObject::RD_REFLECT);
	}

	//	Render host player
	CECHostPlayer* pHost = GetHostPlayer();
	if (pHost)
	{
		if (pHost->GetBoothState() != 2)
			pHost->Render(pViewport, CECObject::RD_REFLECT);
		else
			pHost->RenderForBooth(pViewport, CECObject::RD_REFLECT);
	}

	//	Render Login Players here, if there are
	int nNumLoginPlayers = m_LoginPlayers.size();
	for(int i=0; i<nNumLoginPlayers; i++)
	{
		m_LoginPlayers[i]->Render(pViewport, CECObject::RD_REFLECT);
	}

	return true;
}

//	RenderForRefract routine
bool CECPlayerMan::RenderForRefract(CECViewport * pViewport)
{
	//	All player should be reflect visible
	//	Render all players
	PlayerTable::iterator it = m_PlayerTab.begin();
	for (; it != m_PlayerTab.end(); ++it)
	{
		CECElsePlayer* pPlayer = *it.value();
		pPlayer->Render(pViewport, CECObject::RD_REFRACT);
	}

	//	Render host player
	CECHostPlayer* pHost = GetHostPlayer();
	if (pHost)
		pHost->Render(pViewport, CECObject::RD_REFRACT);

	//	Render Login Players here, if there are
	int nNumLoginPlayers = m_LoginPlayers.size();
	for(int i=0; i<nNumLoginPlayers; i++)
	{
		m_LoginPlayers[i]->Render(pViewport, CECObject::RD_REFRACT);
	}

	return true;
}

//	RenderForBloom routine
bool CECPlayerMan::RenderForBloom(CECViewport * pViewport)
{
	//	Render all players
	PlayerTable::iterator it = m_PlayerTab.begin();
	for (; it != m_PlayerTab.end(); ++it)
	{
		CECElsePlayer* pPlayer = *it.value();
		pPlayer->Render(pViewport, CECObject::RD_BLOOM);
	}

	//	Render host player
	CECHostPlayer* pHost = GetHostPlayer();
	if (pHost)
		pHost->Render(pViewport, CECObject::RD_BLOOM);

	//	Render Login Players here, if there are
	int nNumLoginPlayers = m_LoginPlayers.size();
	for(int i=0; i<nNumLoginPlayers; i++)
	{
		m_LoginPlayers[i]->Render(pViewport, CECObject::RD_BLOOM);
	}

	return true;
}

#ifndef BACK_VERSION
bool CECPlayerMan::RenderForShadow( ECViewport* pViewport )
{
	PlayerTable::iterator it = m_PlayerTab.begin();
	for (; it != m_PlayerTab.end(); ++it)
	{
		CECElsePlayer* pPlayer = *it.value();
		if(pPlayer->GetBoothState()!=2)
			pPlayer->Render(pViewport, CECObject::RD_SHADOW);
	}

	//	Render host player
	CECHostPlayer* pHost = GetHostPlayer();
	if (pHost&&pHost->GetBoothState()!=2)
		pHost->Render(pViewport, CECObject::RD_SHADOW);

	//	Render Login Players here, if there are
	int nNumLoginPlayers = m_LoginPlayers.size();
	for(int i=0; i<nNumLoginPlayers; i++)
	{
		m_LoginPlayers[i]->Render(pViewport, CECObject::RD_SHADOW);
	}

	return true;
}
#endif

//	Host information arrived
bool CECPlayerMan::HostPlayerInfo1(const S2C::cmd_self_info_1& Info)
{
	//	Set world load center
	ASSERT(m_pGameRun);
	const A3DVECTOR3& vInitLoadPos = m_pGameRun->GetWorld()->GetInitLoadPos();
	if (vInitLoadPos.x != Info.pos.x || vInitLoadPos.z != Info.pos.z)
		m_pGameRun->GetWorld()->SetLoadCenter(Info.pos);

	CECHostPlayer* pHost = GetHostPlayer();

	if (!pHost->Init(Info))
	{
		a_LogOutput(1, "CECPlayerMan::HostPlayerInfo1, Failed to initialize host player");
		return false;
	}

	if (pHost->GetFactionID())
		g_pGame->GetFactionMan()->GetFaction(pHost->GetFactionID(), true, false);

	if (pHost->GetFamilyID())
		g_pGame->GetFactionMan()->GetFaction(pHost->GetFamilyID(), true, true);

	//	Update the camera which is associated with host player
	//	TODO: Directly get viewport here isn't a good way. Maybe we should
	//	find a way to know which camera is associated with host player.
	TickHostPlayer(0, g_pGame->GetViewport());

	return true;
}

//	One else player enter view area
CECElsePlayer* CECPlayerMan::ElsePlayerEnter(const S2C::info_player_1& Info, int iCmd)
{
	//	If this player's id is in unknown table, remove it because this player
	//	won't be unknown anymore
	UkPlayerTable::pair_type Pair = m_UkPlayerTab.get(Info.cid);
	if (Pair.second)
		m_UkPlayerTab.erase(Info.cid);

	int iAppearFlag = (iCmd == S2C::PLAYER_ENTER_WORLD) ? CECElsePlayer::APPEAR_ENTERWORLD : CECElsePlayer::APPEAR_RUNINTOVIEW;

	//	Has player been in active player table ?
	CECElsePlayer* pPlayer = GetElsePlayer(Info.cid);
	if (pPlayer)
	{
		//	This player has existed in player table, call special initial function
		pPlayer->Init(Info, iAppearFlag, true);
		return pPlayer;
	}

	//	Create a new player
	if (!(pPlayer = CreateElsePlayer(Info, iAppearFlag)))
	{
		a_LogOutput(1, "CECPlayerMan::ElsePlayerEnter, Failed to create player (%d)", Info.cid);
		return NULL;
	}

	//	Add player to table
	m_PlayerTab.put(Info.cid, pPlayer);

	return pPlayer;
}

CECElsePlayer* CECPlayerMan::ElsePlayerEnter(const S2C::info_npc& Info)
{
	//	If this player's id is in unknown table, remove it because this player
	//	won't be unknown anymore
	UkPlayerTable::pair_type Pair = m_UkPlayerTab.get(Info.nid);
	if (Pair.second)
		m_UkPlayerTab.erase(Info.nid);

	int iAppearFlag = CECElsePlayer::APPEAR_ENTERWORLD;

	//	Has player been in active player table ?
	CECElsePlayer* pPlayer = GetElsePlayer(Info.nid);
	if (pPlayer)
	{
		//	This player has existed in player table, call special initial function
		pPlayer->Init(Info, iAppearFlag, true);
		return pPlayer;
	}

	//	Create a new player
	if (!(pPlayer = CreateElsePlayer(Info, iAppearFlag)))
	{
		a_LogOutput(1, "CECPlayerMan::ElsePlayerEnter, Failed to create player (%d)", Info.nid);
		return NULL;
	}

	//	Add player to table
	m_PlayerTab.put(Info.nid, pPlayer);
	CECPlayer* pSummoner = GetPlayer(Info.id_summoner);
	if (pSummoner)
		pSummoner->SetCloneID(Info.nid);
	return pPlayer;
}

//	One else player leave view area or exit game
void CECPlayerMan::ElsePlayerLeave(int cid, bool bExit, bool bUpdateMMArray/* true */)
{
	//	Player has been in active player table ?
	CECElsePlayer* pPlayer = GetElsePlayer(cid);
	CECHostPlayer* pHost = GetHostPlayer();
	
	if(pPlayer && (pPlayer->GetAttachMode() == CECPlayer::enumAttachRideOnQilin || pPlayer->GetQilinConnectBuddy()))
	{
		CECPlayer *pBuddy = GetPlayer(pPlayer->GetQilinConnectBuddy());
		if(pBuddy)
		{
			pBuddy->SetQilinConnectBuddy(0);
			pBuddy->SetQilinConnectHangedOn(false);
		}
		pPlayer->SetQilinConnectBuddy(0);
		pPlayer->SetQilinConnectHangedOn(false);
		pPlayer->DetachQilinBuddy();

	}
	
	if(pPlayer && pPlayer->IsMultiRideLeader())
	{
		for(int i=1; i< MULTI_RIDE_CAPACITY; ++i)
		{
			pPlayer->DetachMultiBuddy(i);
		}
	}
	if(pPlayer && pPlayer->IsMultiRideMember())
	{
		CECPlayer* pLeader = GetPlayer(pPlayer->GetMultiRideLeaderId());
		if(pLeader)
		{
			pLeader->RemoveMultiRideMember(cid);
		}
	}
	//	Remove player from m_aMMPlayers array
	if (pPlayer)
	{
		if (bUpdateMMArray)
			RemovePlayerFromMiniMap(pPlayer);

		pPlayer->m_iMMIndex = -1;

		//	If this player is selected by host, cancel the selection
		if (pPlayer->GetCharacterID() == pHost->GetSelectedTarget())
			g_pGame->GetGameSession()->c2s_CmdUnselect();

		RemovePlayerFromTabSels(pPlayer);
	}

	if (bExit)	//	Player exit game
	{
		if (!pPlayer)
		{
			//	Release player data in cache
			CECEPCacheData* pData = PopPlayerFromCache(cid);
			if (pData)
				delete pData;
		}
		else
		{
			//	Remove from active player table
			m_PlayerTab.erase(cid);

			QueuePlayerUndoLoad(cid, true);
		}
	}
	else	//	Player leave view area
	{
		if (!pPlayer)
			return;		//	Counldn't find this player in view area

		//	If this player is a member of host player's team, remain it
		//	in player list !
		//	Note: there are no players in ghost mode anymore !
	/*	if (pHost && pHost->IsTeamMember(cid))
		{
			pPlayer->TurnToGhost();
			return;
		}	*/

		//	This player shouldn't be in cache
		ASSERT(!IsPlayerInCache(cid));

		//	Push player data into cache
		//  if the player is clone, don not cache the data
		if (!ISCLONEPLAYERID(cid))
		{		
			CECEPCacheData* pCacheData = pPlayer->CreateCacheData();
			if (pCacheData)
				PushPlayerToCache(pCacheData);
		}
		//	Remove from active player table
		m_PlayerTab.erase(cid);

		QueuePlayerUndoLoad(cid, true);
	}

	//	Release player object
	ReleaseElsePlayer(pPlayer);
}

//	One else player's more information arrived
bool CECPlayerMan::MoreElsePlayerInfo2(int cid, const S2C::info_player_2& Info)
{
	return true;
}

//	One else player's more information arrived
bool CECPlayerMan::MoreElsePlayerInfo3(int cid, const S2C::info_player_3& Info)
{
	return true;
}

//	One else player's more information arrived
bool CECPlayerMan::MoreElsePlayerInfo4(int cid, const S2C::info_player_4& Info)
{
	return true;
}

//	Create else player
CECElsePlayer* CECPlayerMan::CreateElsePlayer(const S2C::info_player_1& Info, int iAppearFlag)
{
	CECElsePlayer* pPlayer = new CECElsePlayer(this);
	if (!pPlayer)
	{
		glb_ErrorOutput(ECERR_NOTENOUGHMEMORY, "CECPlayerMan::CreateElsePlayer", __LINE__);
		return false;
	}

	pPlayer->SetBornStamp(g_pGame->GetGameRun()->GetWorld()->GetBornStamp());

	//	Try to load player data from cache at first
	CECEPCacheData* pCacheData = PopPlayerFromCache(Info.cid);
	if (pCacheData)
	{
		if (!pPlayer->InitFromCache(Info, pCacheData, iAppearFlag))
		{
			delete pCacheData;
			a_LogOutput(1, "CECPlayerMan::CreateElsePlayer, Failed to init player from cache data.");
			return NULL;
		}

		delete pCacheData;
	}
	else
	{
		if (!pPlayer->Init(Info, iAppearFlag, false))
		{
			glb_ErrorOutput(ECERR_FAILEDTOCALL, "CECPlayerMan::CreateElsePlayer", __LINE__);
			return false;
		}
	}

	return pPlayer;
}

CECElsePlayer* CECPlayerMan::CreateElsePlayer(const S2C::info_npc& Info, int iAppearFlag)
{
	CECElsePlayer* pPlayer = new CECElsePlayer(this);
	if (!pPlayer)
	{
		glb_ErrorOutput(ECERR_NOTENOUGHMEMORY, "CECPlayerMan::CreateElsePlayer", __LINE__);
		return false;
	}

	pPlayer->SetBornStamp(g_pGame->GetGameRun()->GetWorld()->GetBornStamp());

	//	Try to load player data from cache at first
	CECEPCacheData* pCacheData = GetPlayerCacheData(Info.id_summoner);
	if (pCacheData)
	{
		if (!pPlayer->InitFromCache(Info, pCacheData, iAppearFlag))
		{
			delete pCacheData;
			a_LogOutput(1, "CECPlayerMan::CreateElsePlayer, Failed to init player from cache data.");
			return NULL;
		}

//		delete pCacheData;
	}
	else
	{
		if (!pPlayer->Init(Info, iAppearFlag, false))
		{
			glb_ErrorOutput(ECERR_FAILEDTOCALL, "CECPlayerMan::CreateElsePlayer", __LINE__);
			return false;
		}
	}

	return pPlayer;
}


GNET::GamedataSend CECPlayerMan::gen_player_list()
{
	using namespace S2C;

	GNET::GamedataSend gds;
	GNET::Octets& o = gds.data;
	o.reserve(4000);
	cmd_header h;
	h.cmd = PLAYER_INFO_1_LIST;
	add_to_octets<cmd_header>(o, h);
	add_to_octets<unsigned short>(o, m_PlayerTab.size());

	for (PlayerTable::iterator it = m_PlayerTab.begin(); it != m_PlayerTab.end(); ++it)
	{
		CECElsePlayer* pPlayer = *it.value();
		GNET::Octets o1 = pPlayer->gen_info_player_1();
		o.insert(o.end(), o1.begin(), o1.size());
	}

	return gds;
}

inline record_player_cache _gen_record_player_cache(CECEPCacheData* pCache)
{
	record_player_cache c;
	c.m_cid			= pCache->m_cid;
	wcscpy(c.m_strName, pCache->m_strName);
	c.m_Faceid		= (char)pCache->m_iFaceid;
	c.m_Hairid		= (char)pCache->m_iHairid;
	c.m_Earid		= (char)pCache->m_iEarid;
	c.m_Tailid		= (char)pCache->m_iTailid;
	c.m_DefaultSkin	= (char)pCache->m_iDefaultSkin;
	c.m_Gender		= (char)pCache->m_iGender;
	c.m_Race		= (char)pCache->m_iRace;
	c.m_Physique	= (char)pCache->m_iPhysique;
	c.m_dwResFlags	= pCache->m_dwResFlags;
	c.m_crc_e		= pCache->m_crc_e;
	memcpy(c.m_aEquips, pCache->m_aEquips, sizeof(pCache->m_aEquips));
	c.m_crcBooth	= pCache->m_crcBooth;
	wcscpy(c.m_strBoothName, pCache->m_strBoothName);
	return c;
}

GNET::Octets CECPlayerMan::gen_player_cache_list()
{
	GNET::Octets o;
	o.reserve(4000);
	add_to_octets<unsigned short>(o, 0);
	unsigned short count = m_PlayerCache.size();

	for (CacheTable::iterator it = m_PlayerCache.begin(); it != m_PlayerCache.end(); ++it)
	{
		record_player_cache d = _gen_record_player_cache(*it.value());
		o.insert(o.end(), &d, sizeof(d));
	}

	for (PlayerTable::iterator itPlayer = m_PlayerTab.begin(); itPlayer != m_PlayerTab.end(); ++itPlayer)
	{
		CECElsePlayer* pPlayer = *itPlayer.value();
		CECEPCacheData* pCache = pPlayer->CreateCacheData();

		if (pCache)
		{
			record_player_cache d = _gen_record_player_cache(pCache);
			o.insert(o.end(), &d, sizeof(d));
			delete pCache;
			count++;
		}
	}

	memcpy(o.begin(), &count, sizeof(count));
	return o;
}

void CECPlayerMan::create_cache_from_record(GNET::Octets& o)
{
	unsigned short count = *(unsigned short*)o.begin();

	if (o.size() != sizeof(count) + count * sizeof(record_player_cache))
		return;

	record_player_cache* p = (record_player_cache*)((char*)o.begin() + sizeof(count));

	for (unsigned short i = 0; i < count; i++)
	{
		CECEPCacheData* pCache = new CECEPCacheData;
		record_player_cache& c = p[i];

		pCache->m_cid			= c.m_cid;
		pCache->m_strName		= c.m_strName;
		pCache->m_iFaceid		= c.m_Faceid;
		pCache->m_iHairid		= c.m_Hairid;
		pCache->m_iEarid		= c.m_Earid;
		pCache->m_iTailid		= c.m_Tailid;
		pCache->m_iDefaultSkin	= c.m_DefaultSkin;
		pCache->m_iGender		= c.m_Gender;
		pCache->m_iRace			= c.m_Race;
		pCache->m_iPhysique		= c.m_Physique;
		pCache->m_dwResFlags	= c.m_dwResFlags;
		pCache->m_crc_e			= c.m_crc_e;
		memcpy(pCache->m_aEquips, c.m_aEquips, sizeof(pCache->m_aEquips));
		pCache->m_crcBooth		= c.m_crcBooth;
		pCache->m_strBoothName	= c.m_strBoothName;
		PushPlayerToCache(pCache);
	}
}

//	Release else player
void CECPlayerMan::ReleaseElsePlayer(CECElsePlayer* pPlayer)
{
	//	Remove tab-selected array
	RemovePlayerFromTabSels(pPlayer);

	if (pPlayer)
	{
		pPlayer->Release();
		delete pPlayer;
		pPlayer = NULL;
	}
}

//	Load special booth model
CECModel* CECPlayerMan::LoadSpeBoothModel(int tid)
{
	abase::hash_map<int, CECModel*>::iterator it = m_BoothModelMap.find(tid);
	if(it == m_BoothModelMap.end())
	{
		const char* szFile;
		elementdataman* pDB = g_pGame->GetElementDataMan();
		szFile = g_pGame->GetDataPath(tid);
		CECModel* pSpeBoothModel = new CECModel();
		if(pSpeBoothModel)
		{
			pSpeBoothModel->Load(szFile);
			pSpeBoothModel->GetA3DSkinModel()->EnableSpecular(true);
			m_BoothModelMap[tid] = pSpeBoothModel;
			return pSpeBoothModel;
		}		
	}
	else
		return it->second;

	return NULL;
}

//	Get else player cache data
CECEPCacheData* CECPlayerMan::GetPlayerCacheData(int cid)
{
	CacheTable::pair_type Pair = m_PlayerCache.get(cid);
	return Pair.second ? *Pair.first : NULL;
}

//	Pop player data from cache
CECEPCacheData* CECPlayerMan::PopPlayerFromCache(int cid)
{
	CacheTable::pair_type Pair = m_PlayerCache.get(cid);
	if (!Pair.second)
		return NULL;

	CECEPCacheData* pData = *Pair.first;
	ASSERT(pData);

	//	Remove player from cache
	m_PlayerCache.erase(cid);

	return pData;
}

//	Push player to cache
bool CECPlayerMan::PushPlayerToCache(CECEPCacheData* pData)
{
	CacheTable::pair_type Pair = m_PlayerCache.get(pData->m_cid);
	if (Pair.second)
	{
		//	This case shouldn't happen, but we still handle it.
		CECEPCacheData* pTemp = *Pair.first;
		delete pTemp;
		m_PlayerCache.erase(pData->m_cid);
	}

	pData->m_dwCacheTime = (DWORD)(a_GetTime() * 0.001f);
	m_PlayerCache.put(pData->m_cid, pData);
	return true;
}

//	Clear up player data cache
void CECPlayerMan::ClearUpPlayerCache()
{
	if (m_PlayerCache.size() < MAX_CACHEDPLAYER)
		return;

	DWORD dwCurTime = a_GetTime() / 1000;

	//	Release some old enough cached data
	int aIDs[50], iCount=0;
	
	CacheTable::iterator it = m_PlayerCache.begin();
	for (; it != m_PlayerCache.end(); ++it)
	{
		CECEPCacheData* pData = *it.value();
		if (pData->m_dwCacheTime + 240 < dwCurTime)
		{
			aIDs[iCount++] = pData->m_cid;
			if (iCount >= 50)
				break;

			delete pData;
		}
	}

	for (int i=0; i < iCount; i++)
		m_PlayerCache.erase(aIDs[i]);
}

//	Update players in various ranges (Active, visible, mini-map etc.)
void CECPlayerMan::UpdatePlayerInRanges(DWORD dwDeltaTime)
{
	CECHostPlayer* pHost = GetHostPlayer();
	if (!pHost || !pHost->IsAllResReady())
		return;

	CECConfigs* pConfigs = g_pGame->GetConfigs();
	int idHoverObject = pHost->GetCursorHoverObject();
	int idSelected = pHost->GetSelectedTarget();
	bool bRenderNameByCamera = true;
	if (GetHostPlayer() && 
		GetHostPlayer()->GetCutscene() && 
		GetHostPlayer()->GetCutscene()->IsPlayingCutscene() && 
		!GetHostPlayer()->GetCutscene()->IsNameShow())
	{
		bRenderNameByCamera = false;
	}

	//	Get current player visible radius
	float fPVRadius = pConfigs->GetPlayerVisRadius();
	if (!pConfigs->GetVideoSettings().bModelLimit)
		fPVRadius = pConfigs->GetSevActiveRadius();

	m_aMMPlayers.RemoveAll(false);

	//	Check all else players
	PlayerTable::iterator it = m_PlayerTab.begin();
	while(it != m_PlayerTab.end())
	{
		CECElsePlayer* pPlayer = *it.value();
		float fDistToHostH = pPlayer->GetDistToHostH();
		float fDistToHost = pPlayer->GetDistToHost();
		float fDistToCamera = pPlayer->GetDistToCamera();

		//	Check player's visiblity
		if( pConfigs->GetVideoSettings().bModelLimit )
			pPlayer->SetUseHintModelFlag(fDistToHost <= fPVRadius ? false : true);
		else
			pPlayer->SetUseHintModelFlag(false);

		//	Check whether name is visible
		if (bRenderNameByCamera && (fDistToHost <= 20.0f || pPlayer->GetCharacterID() == idHoverObject || pPlayer->GetCharacterID() == idSelected) )
			pPlayer->SetRenderNameFlag(true);
		else
			pPlayer->SetRenderNameFlag(false);

		//	Set bars visible flags
		pPlayer->SetRenderBarFlag(bRenderNameByCamera && pPlayer->GetCharacterID() == idSelected);

		//	Check whether player is in mini-map
		if (fDistToHostH > pConfigs->GetSevActiveRadius())
		{		 
			// backup next item before delete it.
			++it;

			ElsePlayerLeave(pPlayer->GetCharacterID(), false, false);
			continue;
		}
		else if (!pPlayer->InGhostState() && fDistToHostH <= pConfigs->GetSevActiveRadius())
			pPlayer->m_iMMIndex = m_aMMPlayers.Add(pPlayer);

		 ++it;
	}
}

//	Remove else player from m_aMMPlayers array
void CECPlayerMan::RemovePlayerFromMiniMap(CECElsePlayer* pPlayer)
{
	ASSERT(pPlayer);
	int iIndex = pPlayer->m_iMMIndex;
	if (iIndex < 0 || iIndex >= m_aMMPlayers.GetSize())
		return;

	CECElsePlayer* pTempPlayer = m_aMMPlayers[iIndex];
	if (pTempPlayer != pPlayer)
	{
		ASSERT(pTempPlayer == pPlayer);
		return;
	}

	//	Exchange with the last layer in array
	int iLastIdx = m_aMMPlayers.GetSize() - 1;
	if (m_aMMPlayers.GetSize() > 1 && iIndex != iLastIdx)
	{
		pTempPlayer = m_aMMPlayers[iLastIdx];
		m_aMMPlayers[iIndex] = pTempPlayer;
		pTempPlayer->m_iMMIndex = iIndex;
		m_aMMPlayers.RemoveAt(iLastIdx);
	}
	else
		m_aMMPlayers.RemoveAtQuickly(iIndex);
}

//	When world's loading center changed
void CECPlayerMan::OnLoadCenterChanged()
{
	m_aMMPlayers.RemoveAll(false);
}

//	Ray trace
bool CECPlayerMan::RayTrace(ECRAYTRACE* pTraceInfo)
{
	//	If shift is pressed, ignore all players
	if (pTraceInfo->dwKeyStates & EC_KSF_SHIFT)
		return true;
	//  If F10 is pressed before, ignore all players
	if (g_pGame->GetGameRun()->IsHideElsePlayer())
		return true;

	A3DVECTOR3 vHitPos, vNormal;
	float fFracion;
	bool bRet = false;

/*	Trace host player heself

	if (m_pHostPlayer &&
		(pTraceInfo->iTraceObj == TRACEOBJ_LBTNCLICK || 
		pTraceInfo->iTraceObj == TRACEOBJ_TESTRAY))
	{
		//	Trace host player
		const A3DAABB* paabb = &m_pHostPlayer->GetPlayerAABB();
		if (CLS_RayToAABB3(pTraceInfo->vStart, pTraceInfo->vDelta, paabb->Mins, paabb->Maxs,
					vHitPos, &fFracion, vNormal))
		{
			if (fFracion < pTraceInfo->pTraceRt->fFraction)
			{
				pTraceInfo->pTraceRt->fFraction		= fFracion;
				pTraceInfo->pTraceRt->vHitPos		= vHitPos;
				pTraceInfo->pTraceRt->vPoint		= vHitPos;
				pTraceInfo->pTraceRt->vNormal		= vNormal;

				pTraceInfo->pECTraceRt->fFraction	= fFracion;
				pTraceInfo->pECTraceRt->iEntity		= ECENT_PLAYER;
				pTraceInfo->pECTraceRt->iObjectID	= m_pHostPlayer->GetCharacterID();
				pTraceInfo->pECTraceRt->vNormal		= vNormal;

				bRet = true;
			}
		}
	}
*/
	//	Trace all else player
	PlayerTable::iterator it = m_PlayerTab.begin();
	for (; it != m_PlayerTab.end(); ++it)
	{
		CECElsePlayer* pPlayer = *it.value();
		if (pPlayer->InGhostState())
			continue;

		if (pPlayer->GetBattleCamp() == GP_BATTLE_CAMP_OBSERVER)
			continue;

		//	TRACEOBJ_LBTNCLICK is caused by player pressed left button
	//	if (pTraceInfo->iTraceObj == TRACEOBJ_LBTNCLICK && pPlayer->IsDead())
	//		continue;

		const A3DAABB* paabb;
		if ((pTraceInfo->iTraceObj == TRACEOBJ_LBTNCLICK ||
			pTraceInfo->iTraceObj == TRACEOBJ_RBTNCLICK) &&
			!pPlayer->IsDead())		//	When player died, use GetPlayerPickAABB()
			paabb = &pPlayer->GetPlayerAABB();
		else
			paabb = &pPlayer->GetPlayerPickAABB();//////---------------------NEED MODIFY??
			//paabb = &pPlayer->GetPlayerAABB();
		
		if (CLS_RayToAABB3(pTraceInfo->vStart, pTraceInfo->vDelta, paabb->Mins, paabb->Maxs,
					vHitPos, &fFracion, vNormal))
		{
			if (fFracion < pTraceInfo->pTraceRt->fFraction)
			{
				pTraceInfo->pTraceRt->fFraction		= fFracion;
				pTraceInfo->pTraceRt->vHitPos		= vHitPos;
				pTraceInfo->pTraceRt->vPoint		= vHitPos;
				pTraceInfo->pTraceRt->vNormal		= vNormal;

				pTraceInfo->pECTraceRt->fFraction	= fFracion;
				pTraceInfo->pECTraceRt->iEntity		= ECENT_PLAYER;
				pTraceInfo->pECTraceRt->iObjectID	= pPlayer->GetCharacterID();
				pTraceInfo->pECTraceRt->vNormal		= vNormal;

				bRet = true;
			}
		}
	}

	return bRet;
}

// Trace for CD
bool CECPlayerMan::TraceWithBrush(BrushTraceInfo * pInfo, APtrArray<CECElsePlayer*>& playerArray )
{
	if ( !pInfo )
	{
		return false;
	}

	bool bCollide = false;
	//save original result
	bool		bStartSolid = pInfo->bStartSolid;	//	Collide something at start point
	bool		bAllSolid = pInfo->bAllSolid;		//	All in something
	int			iClipPlane = pInfo->iClipPlane;		//	Clip plane's index
	float		fFraction = 100.0f;		//	Fraction
	A3DVECTOR3  vNormal = pInfo->ClipPlane.GetNormal(); //clip plane normal
	float       fDist = pInfo->ClipPlane.GetDist();	//clip plane dist

	PlayerTable::iterator iter = m_PlayerTab.begin();
   	A3DVECTOR3 resDelta(pInfo->vDelta);
    A3DVECTOR3 resStart(pInfo->vStart);
	playerArray.RemoveAll(false);
//	m_aTracedPlayers.RemoveAll(false);
	for ( ; iter != m_PlayerTab.end(); ++iter )
	{
		CECElsePlayer* pElsePlayer = *iter.value();
		if ( !pElsePlayer )
		{
			continue;
		}

		if ( pElsePlayer->TraceWithBrush( pInfo ) )
		{
			if ( pInfo->fFraction < fFraction )
			{
				fFraction = pInfo->fFraction;
				bAllSolid = pInfo->bAllSolid;
				bStartSolid = pInfo->bStartSolid;
				iClipPlane = pInfo->iClipPlane;
//				bCollide = true;
				vNormal = pInfo->ClipPlane.GetNormal();
				fDist = pInfo->ClipPlane.GetDist();
			}
			bCollide = true;
			playerArray.Add( pElsePlayer );
//			m_aTracedPlayers.Add( pElsePlayer );
		}
	}
	
	//set back
	pInfo->fFraction = fFraction;
	pInfo->bStartSolid = bStartSolid;
	pInfo->bAllSolid = bAllSolid;
	pInfo->iClipPlane = iClipPlane;
	pInfo->ClipPlane.SetNormal(vNormal);
	pInfo->ClipPlane.SetD(fDist);

	//
	return bCollide;
}

/*
return
0 完全不可能碰撞
1 碰撞发生
2 可能碰撞，需要再用别的方式检测，这儿是CLS_AABBAABBOverlap
*/

#define COLLISION_EPSILON 0.0001f

int MayCollision(const A3DVECTOR3& p1, const A3DVECTOR3& v1, const A3DVECTOR3& p2, const A3DVECTOR3& v2, float extTotal, float fTime)
{
	A3DVECTOR3 deltaPos = p2-p1;
	float dist = deltaPos.Normalize();
	//A3DMATRIX4 mat = a3d_RotateY(DEG2RAD(90));
	//A3DVECTOR3 deltaNormal = deltaPos * mat;
	float v = DotProduct(v1, deltaPos);
	A3DVECTOR3 vTangent1 = v * deltaPos;
	v = DotProduct(v2, deltaPos);
	A3DVECTOR3 vTangent2 = v * deltaPos;

	a_LogOutput( 1, "testColl, p1=(%f, %f), p2=(%f, %f), v1=(%f, %f), v1=(%f, %f)", 
		p1.x, p1.z, p2.x, p2.z, v1.x, v1.z, v2.x, v2.z);
					
	// 粘到一起
/*	if (dist<extTotal)
	{
		return 0;
	}
	else 
*/	if (DotProduct(vTangent1, vTangent2)<COLLISION_EPSILON)		// p1->p2<-  or p1<-p2->
	{
		// 一定碰撞
		if (DotProduct(vTangent1, deltaPos)>0 || DotProduct(vTangent2, deltaPos)<0)
		{
			return 2;
		}
		else
			return 0;
	}
	else if (DotProduct(vTangent1, deltaPos)>0)	// p1->p2->
	{
		if (vTangent2.Magnitude()>vTangent1.Magnitude())
			return 0;
		else
		{
			A3DVECTOR3 vTest = (p1 + vTangent1*fTime + deltaPos*extTotal) - (p2 + vTangent2*fTime);
			if ( DotProduct(vTest, deltaPos)>0 )
				return 0;
			else
				return 1;
		}
	}
	else if (DotProduct(vTangent1, deltaPos)<0)	// p1<-p2<-
	{
		if (vTangent1.Magnitude()>vTangent2.Magnitude())
			return 0;
		else
		{
			A3DVECTOR3 vTest = (p2 + vTangent2*fTime + deltaPos*extTotal) - (p1 + vTangent1*fTime);
			if ( DotProduct(vTest, deltaPos)>0 )
				return 0;
			else
				return 1;
		}
	}
	else		// invalid
	{

	}

	return 0;
}

// Update the Host player's collision detection states with all other else players. Currently just used for special move.
bool CECPlayerMan::UpdateHostCollisionStates( float fDeltaTime )
{
	bool bRet = false;

	PlayerTable::iterator iter = m_PlayerTab.begin();
	CECHostPlayer* pHost = GetHostPlayer();
	if ( pHost )
	{
		A3DAABB aabb1 = pHost->GetPlayerAABB(); //GetPlayerPickAABB()
		//-------------------------------------------------------------
		//Calculate the new AABB for move states.
		A3DVECTOR3 vDist1; /*= pHost->GetVelocity() * fDeltaTime; pHost->GetVelocity() * fDeltaTime + pHost->GetAcceleration() * fDeltaTime * fDeltaTime * 0.5f;*/
		//NEW ADDED
//		CalculateDeltaDist( pHost->GetVelocity(), 10.f, pHost->GetCDRInfo()->vClipVel, pHost->GetCDRInfo()->vTPNormal, fDeltaTime, vDist1 );
#ifdef CDR_HL2_MOVE
		A3DVECTOR3 vPrevClipVelocity = pHost->GetCDRInfo()->vClipVel;	//先保存住在没执行GroundMove之前的clip 速度，因为：这个函数可能要同时调用两次
		A3DVECTOR3 vCurDir = pHost->GetVelocity();
		vCurDir.Normalize();
		vDist1 = pHost->GetMoveControl().GroundMove(vCurDir, pHost->GetVelocity().Magnitude(), fDeltaTime, pHost->GetVertSpeed());
		vDist1 -= pHost->GetPos();
		//加大一点预测距离值？
		A3DVECTOR3 vDistDelta = vDist1;
		vDistDelta.Normalize();
		vDist1 += (vDistDelta * 0.05f);
		//
		pHost->GetCDRInfo()->vClipVel = vPrevClipVelocity;				//恢复原来的clip 速度!!!
#endif
		//NEW ADDED END
		BrushTraceInfo  brushInfo1;
		brushInfo1.Init( aabb1.Center, vDist1, aabb1.Extents + A3DVECTOR3( 0.3f, 0.0f, 0.3f ) ); //Modified, extent the x-z-axis length.
//		a_LogOutput( 1, "----------PridictValue--->PlayerID=%d: vDist1=(%f, %f, %f)----------", pHost->GetCharacterID(), vDist1.x, vDist1.y, vDist1.z );
		//-------------------------------------------------------------
		for ( ; iter != m_PlayerTab.end(); ++iter )
		{
			CECElsePlayer* pElsePlayer = *iter.value();
			if ( !pElsePlayer || (pElsePlayer->GetCollisionSkillState() & COLLISION_STATE_VOID) )	//玩家不存在，或者玩家处于虚无技能状态，则不进行碰撞检测
			{
				continue;
			}
			A3DAABB aabb2 = pElsePlayer->GetPlayerAABB(); //GetPlayerPickAABB()
			//-------------------------------------------------------------
			//Calculate the new AABB for move states.
			A3DVECTOR3 vDist2(0.0f); /*= pElsePlayer->GetVelocity() * fDeltaTime;pElsePlayer->GetVelocity() * fDeltaTime + pElsePlayer->GetAcceleration() * fDeltaTime * fDeltaTime * 0.5f;*/
			//NEW ADDED
//			CalculateDeltaDist( pElsePlayer->GetVelocity(), 10.f, pElsePlayer->GetCDRInfo()->vClipVel, pElsePlayer->GetCDRInfo()->vTPNormal, fDeltaTime, vDist2 );
			if ( !pElsePlayer->GetVelocity().IsZero() )
			{
#ifdef CDR_HL2_MOVE
				A3DVECTOR3 vPrevClipVel = pElsePlayer->GetCDRInfo()->vClipVel;	//!!!先保存好else player在执行GroundMoveStep之前的clip速度，因为这个函数要被同时调用两次
				A3DVECTOR3 vCurDir = pElsePlayer->GetVelocity();
				vCurDir.Normalize();
				vDist2 = pElsePlayer->GroundMoveStep( vCurDir, pElsePlayer->GetVelocity().Magnitude(), fDeltaTime );
				vDist2 -= pElsePlayer->GetPos();
				//加大一点预测距离值？
				A3DVECTOR3 vDistDelta2 = vDist2;
				vDistDelta2.Normalize();
				vDist2 += (vDistDelta2 * 0.05f);
				//
				pElsePlayer->GetCDRInfo()->vClipVel = vPrevClipVel;				//恢复之前的clip速度
#endif
				//				a_LogOutput( 1, "----------Else PridictValue--->PlayerID=%d: vDist2=(%f, %f, %f)----------", pElsePlayer->GetCharacterID(), vDist2.x, vDist2.y, vDist2.z );
			}
			//NEW ADDED END
			BrushTraceInfo  brushInfo2;
			brushInfo2.Init( aabb2.Center, vDist2, aabb2.Extents + A3DVECTOR3( 0.3f, 0.0f, 0.3f ) ); // 
			//-------------------------------------------------------------
			// 粗判断
			if ( CLS_AABBAABBOverlap(brushInfo1.BoundAABB.Center, brushInfo1.BoundAABB.Extents, brushInfo2.BoundAABB.Center, brushInfo2.BoundAABB.Extents) )
			{
				if ( MayCollision(pHost->GetPos(), pHost->GetVelocity(), pElsePlayer->GetPos(), pElsePlayer->GetVelocity(), aabb1.Extents.MagnitudeH()+aabb2.Extents.MagnitudeH(), fDeltaTime)>0 )
				{
					A3DVECTOR3 vVelocityHost = pHost->GetVelocity();
					A3DVECTOR3 vVelocityElsePlayer2 = pElsePlayer->GetVelocity();
					//				int	iMass1 = ( pHost->GetCollisionSkillState() & COLLISION_STATE_ASHILL ) ? COLLISION_PLAYER_MAX_MASS : pHost->GetMass();
					UpdateVelocityAfterCD( pHost->GetPos(), pElsePlayer->GetPos(), pHost->GetMass(), pElsePlayer->GetMass(), vVelocityHost, vVelocityElsePlayer2 );
					g_DispColliInfo.Add(pHost->GetPos(), pElsePlayer->GetPos(), pHost->GetVelocity(), pElsePlayer->GetVelocity(), vVelocityHost, vVelocityElsePlayer2);
					
					//-------------------------------------------------------------
					//TEST!!
					//				A3DVECTOR3 vDistance1 = vVelocityHost * fDeltaTime * 1.5f;
					//				A3DVECTOR3 vDistance2 = vVelocityElsePlayer2 * fDeltaTime * 1.5f;
					//				pHost->SetPos( pHost->GetPos() + vDistance1 );
					//				pElsePlayer->SetPos( pElsePlayer->GetPos() + vDistance2 );
					//TEST END
					a_LogOutput( 1, "H->E AfterColl, v1=(%f, %f), v1=(%f, %f)", 
						vVelocityHost.x, vVelocityHost.z, vVelocityElsePlayer2.x, vVelocityElsePlayer2.z);

//					a_LogOutput( 1, "----------ElsePlayer--->PlayerID=%d, PrevCDVelocity=(%f, %f, %f), AfterCDVelocity=(%f, %f, %f)----------", pElsePlayer->GetCharacterID(), pElsePlayer->GetVelocity().x, pElsePlayer->GetVelocity().y, pElsePlayer->GetVelocity().z, vVelocityElsePlayer2.x, vVelocityElsePlayer2.y, vVelocityElsePlayer2.z );
					//-------------------------------------------------------------
					pHost->SetVelocity( vVelocityHost );
					if ( pElsePlayer->GetVelocity().IsZero() && !vVelocityElsePlayer2.IsZero() )
					{
						pElsePlayer->StartSpecialMove();
					}
					A3DVECTOR3 vAcceleration = -vVelocityElsePlayer2;
					vAcceleration.Normalize();
					vAcceleration *= 2.0f;
					pElsePlayer->SetVA(vVelocityElsePlayer2, vAcceleration);
					pElsePlayer->SetCollisionState( true );
					pElsePlayer->UpdateCollisionState( g_pGame->GetServerGMTTime(), 0 );
#ifdef CDR_HL2_MOVE
					//				m_pCDRInfo->vClipVel.x = 0;
					//				m_pCDRInfo->vClipVel.z = 0;
					pElsePlayer->SetCDRInfoClipVelocity( vVelocityElsePlayer2 );
#endif
					bRet = true;
				}
			}			
		}
	}

	//
	return bRet;
}

// Update the collision detection states, called by each elseplayer when they do special move.
bool CECPlayerMan::UpdateCollisionStates( CECElsePlayer* pPlayer, float fDeltaTime )
{
	bool bRet = false;
	//
	if ( !pPlayer )
	{
		return false;
	}

	PlayerTable::iterator iter = m_PlayerTab.begin();
	CECHostPlayer* pHost = GetHostPlayer();

	//-------------------------------------------------------------------------
	A3DAABB aabb2 = pPlayer->GetPlayerAABB();
	A3DVECTOR3 vDist;
#ifdef CDR_HL2_MOVE
	A3DVECTOR3 vPrevClipVel2 = pPlayer->GetCDRInfo()->vClipVel;	//!!!先保存好else player在执行GroundMoveStep之前的clip速度，因为这个函数要被同时调用两次
	A3DVECTOR3 vCurDir = pPlayer->GetVelocity();
	vCurDir.Normalize();
	vDist = pPlayer->GroundMoveStep( vCurDir, pPlayer->GetVelocity().Magnitude(), fDeltaTime );
	vDist -= pPlayer->GetPos();
	//加大一点预测距离值？
	A3DVECTOR3 vDistDelta = vDist;
	vDistDelta.Normalize();
	vDist += (vDistDelta * 0.05f);
	//
	pPlayer->GetCDRInfo()->vClipVel = vPrevClipVel2;			//恢复之前的clip 速度
#endif
	BrushTraceInfo  brushInfo;
	brushInfo.Init( aabb2.Center, vDist, aabb2.Extents + A3DVECTOR3( 0.3f, 0.0f, 0.3f ) ); // 
	//-------------------------------------------------------------------------
	
	// First calculate the collision detection between the host player and the else player.
	if ( pHost && !pHost->IsSpecialMoving() && !(pHost->GetCollisionSkillState() & COLLISION_STATE_VOID) )	// The Host is not in the special moving. And  the Host is NOT in the VOID state.
	{
		A3DAABB aabb1 = pHost->GetPlayerAABB(); //GetPlayerPickAABB()
//		A3DAABB aabb2 = pPlayer->GetPlayerAABB(); //GetPlayerPickAABB()
		//-------------------------------------------------------------
		//Calculate the new AABB for move states.
		A3DVECTOR3 vDist1(0.0f); /*= pHost->GetVelocity() * fDeltaTime; pHost->GetVelocity() * fDeltaTime + pHost->GetAcceleration() * fDeltaTime * fDeltaTime * 0.5f;*/
		//NEW ADDED
//		CalculateDeltaDist( pHost->GetVelocity(), 10.f, pHost->GetCDRInfo()->vClipVel, pHost->GetCDRInfo()->vTPNormal, fDeltaTime, vDist1 );
/*		A3DVECTOR3 vPrevClipVel1 = pHost->GetCDRInfo()->vClipVel;	//先保存住在没执行GroundMove之前的clip 速度，因为：这个函数可能要同时调用两次
		A3DVECTOR3 vCurDir = pHost->GetVelocity();
		vCurDir.Normalize();
		vDist1 = pHost->GetMoveControl().GroundMove(vCurDir, pHost->GetVelocity().Magnitude(), fDeltaTime, pHost->GetVertSpeed());
		vDist1 -= pHost->GetPos();
		pHost->GetCDRInfo()->vClipVel = vPrevClipVel1;				//恢复之前的clip 速度
*///////////////////没有移动，哪里来的移动速度和距离啊！！
		//NEW ADDED END
		BrushTraceInfo  brushInfo1;
		brushInfo1.Init( aabb1.Center, vDist1, aabb1.Extents + A3DVECTOR3( 0.3f, 0.0f, 0.3f ) ); //Modified, extent the x-z-axis length.	 	

//		A3DVECTOR3 vDist; /*= pPlayer->GetVelocity() * fDeltaTime;pPlayer->GetVelocity() * fDeltaTime + pPlayer->GetAcceleration() * fDeltaTime * fDeltaTime * 0.5f;*/
		//NEW ADDED
//		CalculateDeltaDist( pPlayer->GetVelocity(), 10.f, pPlayer->GetCDRInfo()->vClipVel, pPlayer->GetCDRInfo()->vTPNormal, fDeltaTime, vDist );
//		pPlayer->SetPrevCDRClipVelocity( pPlayer->GetCDRInfo()->vClipVel );	//!!!先保存好else player在执行GroundMoveStep之前的clip速度，因为这个函数要被同时调用两次
//		A3DVECTOR3 vCurDir = pPlayer->GetVelocity();
//		vCurDir.Normalize();
//		vDist = pPlayer->GroundMoveStep( vCurDir, pPlayer->GetVelocity().Magnitude(), fDeltaTime );
//		vDist -= pPlayer->GetPos();
		//NEW ADDED END
//		BrushTraceInfo  brushInfo;
//		brushInfo.Init( aabb2.Center, vDist, aabb2.Extents + A3DVECTOR3( 0.3f, 0.0f, 0.3f ) );
		//-------------------------------------------------------------
		if ( CLS_AABBAABBOverlap(brushInfo1.BoundAABB.Center, brushInfo1.BoundAABB.Extents, brushInfo.BoundAABB.Center, brushInfo.BoundAABB.Extents) )	//the collision has been detected.
		{
			if ( MayCollision(pHost->GetPos(), pHost->GetVelocity(), pPlayer->GetPos(), pPlayer->GetVelocity(), aabb1.Extents.MagnitudeH()+aabb2.Extents.MagnitudeH(), fDeltaTime)>0 )
			{
				//compute the new velocity
				A3DVECTOR3 vVelocityElsePlayer = pPlayer->GetVelocity();
				A3DVECTOR3 vVelocityHostPlayer = pHost->GetVelocity();
				UpdateVelocityAfterCD( pHost->GetPos(), pPlayer->GetPos(), pHost->GetMass(), pPlayer->GetMass(), vVelocityHostPlayer, vVelocityElsePlayer );
				g_DispColliInfo.Add(pHost->GetPos(), pPlayer->GetPos(), pHost->GetVelocity(), pPlayer->GetVelocity(), vVelocityHostPlayer, vVelocityElsePlayer);

				a_LogOutput( 1, "E->H AfterColl, v1=(%f, %f), v1=(%f, %f)", 
					vVelocityHostPlayer.x, vVelocityHostPlayer.z, vVelocityElsePlayer.x, vVelocityElsePlayer.z);
				//-----------------------------------------------------------------
				//TEST!!
				//			A3DVECTOR3 vDist1 = vVelocityHostPlayer * fDeltaTime * 1.5f;
				//			A3DVECTOR3 vDist2 = vVelocityElsePlayer * fDeltaTime * 1.5f;
				//			pHost->SetPos( pHost->GetPos() + vDist1 );
				//			pPlayer->SetPos( pPlayer->GetPos() + vDist2 );
				//TEST END
				//-----------------------------------------------------------------
				A3DVECTOR3 vAcceleration = -vVelocityElsePlayer;
				vAcceleration.Normalize();
				vAcceleration *= 2.0f;
				pPlayer->SetVA( vVelocityElsePlayer, vAcceleration);
				pPlayer->SetCollisionState( true );
				pPlayer->UpdateCollisionState( g_pGame->GetServerGMTTime(), 0 );
				pHost->SetVelocity( vVelocityHostPlayer );
#ifdef CDR_HL2_MOVE
				//			m_pCDRInfo->vClipVel.x = 0;
				//			m_pCDRInfo->vClipVel.z = 0;
				pPlayer->SetCDRInfoClipVelocity( vVelocityElsePlayer );
#endif
				bRet = true;
			}
		}
		//-------------------------------------------------------------
		if ( !pHost->GetVelocity().IsZero() && pHost->GetHPWorkMan() &&
			 (!pHost->GetHPWorkMan()->GetWork_p1() || (pHost->GetHPWorkMan()->GetWork_p1() && (pHost->GetHPWorkMan()->GetWork_p1()->GetWorkID() != CECHPWork::WORK_SPECIAL_MOVE)) ) )
		{
			CECHPWorkSpecialMove* pWork = (CECHPWorkSpecialMove*)pHost->GetHPWorkMan()->CreateWork(CECHPWork::WORK_SPECIAL_MOVE);
			if ( pWork )
			{
				pHost->SetCollisionState( true );//=============需要特别小心此处逻辑!!!!!!!!!!
				a_LogOutput( 1, "PlayerID=%d----------HostPlayer被撞了！----------", pHost->GetCharacterID() );
				pWork->SetVelocityAndAcceleration( pHost->GetVelocity(), A3DVECTOR3(1.0f) );
				pHost->GetHPWorkMan()->StartWork_p1(pWork);
			}
		}
		//-------------------------------------------------------------
	}

	//	Second, calculate the collision detection between the pPlayer and all other else players if Needed!
	bool bQueueAfter = false;	//Just a flag, which means that the player in the player table all need operate.
//	A3DAABB aabb1 = pPlayer->GetPlayerAABB(); //GetPlayerPickAABB()
	//-------------------------------------------------------------
	//Calculate the new AABB for move states.
//	A3DVECTOR3 vDist1; /*= pPlayer->GetVelocity() * fDeltaTime;pPlayer->GetVelocity() * fDeltaTime + pPlayer->GetAcceleration() * fDeltaTime * fDeltaTime * 0.5f;*/
	//NEW ADDED
//	CalculateDeltaDist( pPlayer->GetVelocity(), 10.f, pPlayer->GetCDRInfo()->vClipVel, pPlayer->GetCDRInfo()->vTPNormal, fDeltaTime, vDist1 );
	//NEW ADDED END
//	BrushTraceInfo  brushInfo1;
//	brushInfo1.Init( aabb1.Center, vDist1, aabb1.Extents + A3DVECTOR3( 0.3f, 0.0f, 0.3f ) );
	//-------------------------------------------------------------
	
	A3DAABB aabb1 = pPlayer->GetPlayerPickAABB();
	for ( ; iter != m_PlayerTab.end(); ++iter )
	{
		CECElsePlayer* pElsePlayer = *iter.value();
		if ( !pElsePlayer || (pElsePlayer->GetCollisionSkillState() & COLLISION_STATE_VOID) )	//玩家不存在或者玩家处于碰撞技能虚无状态，则不进行碰撞检测
		{
			continue;
		}

		if ( pPlayer == pElsePlayer )	//	the pPlayer self.
		{
			bQueueAfter = true;
			continue;
		}

		if ( !bQueueAfter && pElsePlayer->IsSpecialMoving() )
		{
			continue;
		}

		A3DAABB aabb2 = pElsePlayer->GetPlayerAABB(); //GetPlayerPickAABB()
		//-------------------------------------------------------------
		//Calculate the new AABB for move states.
		A3DVECTOR3 vDist2(0.0f); /*= pElsePlayer->GetVelocity() * fDeltaTime;pElsePlayer->GetVelocity() * fDeltaTime + pElsePlayer->GetAcceleration() * fDeltaTime * fDeltaTime * 0.5f;*/
		//NEW ADDED
//		CalculateDeltaDist( pElsePlayer->GetVelocity(), 10.f, pElsePlayer->GetCDRInfo()->vClipVel, pElsePlayer->GetCDRInfo()->vTPNormal, fDeltaTime, vDist2 );
		if ( !pElsePlayer->GetVelocity().IsZero() )
		{
#ifdef CDR_HL2_MOVE
			A3DVECTOR3 vPrevClipVel2 = pElsePlayer->GetCDRInfo()->vClipVel;	//!!!先保存好else player在执行GroundMoveStep之前的clip速度，因为这个函数要被同时调用两次
			A3DVECTOR3 vCurDir = pElsePlayer->GetVelocity();
			vCurDir.Normalize();
			vDist2 = pElsePlayer->GroundMoveStep( vCurDir, pElsePlayer->GetVelocity().Magnitude(), fDeltaTime );
			vDist2 -= pElsePlayer->GetPos();
			//加大一点预测距离值？
			A3DVECTOR3 vDistDelta2 = vDist2;
			vDistDelta2.Normalize();
			vDist2 += (vDistDelta2 * 0.05f);
			//
			pElsePlayer->GetCDRInfo()->vClipVel = vPrevClipVel2;			//恢复之前的clip速度
#endif
		}
		//NEW ADDED END
		BrushTraceInfo  brushInfo2;
		brushInfo2.Init( aabb2.Center, vDist2, aabb2.Extents + A3DVECTOR3( 0.3f, 0.0f, 0.3f ) ); // 
		//-------------------------------------------------------------
		if ( CLS_AABBAABBOverlap(brushInfo.BoundAABB.Center, brushInfo.BoundAABB.Extents, brushInfo2.BoundAABB.Center, brushInfo2.BoundAABB.Extents) )
		{
			if ( MayCollision(pElsePlayer->GetPos(), pElsePlayer->GetVelocity(), pPlayer->GetPos(), pPlayer->GetVelocity(), aabb1.Extents.MagnitudeH()+aabb2.Extents.MagnitudeH(), fDeltaTime)>0 )
			{
				A3DVECTOR3 vVelocityElsePlayer1 = pPlayer->GetVelocity();
				A3DVECTOR3 vVelocityElsePlayer2 = pElsePlayer->GetVelocity();
				UpdateVelocityAfterCD( pPlayer->GetPos(), pElsePlayer->GetPos(), pPlayer->GetMass(), pElsePlayer->GetMass(), vVelocityElsePlayer1, vVelocityElsePlayer2 );
				g_DispColliInfo.Add(pPlayer->GetPos(), pElsePlayer->GetPos(), pPlayer->GetVelocity(), pElsePlayer->GetVelocity(), vVelocityElsePlayer1, vVelocityElsePlayer2);

				a_LogOutput( 1, "E->E AfterColl, v1=(%f, %f), v1=(%f, %f)", 
					vVelocityElsePlayer1.x, vVelocityElsePlayer1.z, vVelocityElsePlayer2.x, vVelocityElsePlayer2.z);
				
				A3DVECTOR3 vAcceleration = -vVelocityElsePlayer1;
				vAcceleration.Normalize();
				vAcceleration *= 2.0f;
				pPlayer->SetVA(vVelocityElsePlayer1, vAcceleration);
				if ( pElsePlayer->GetVelocity().IsZero() && !vVelocityElsePlayer2.IsZero() )
				{
					pElsePlayer->StartSpecialMove();
				}
				vAcceleration = -vVelocityElsePlayer2;
				vAcceleration.Normalize();
				vAcceleration *= 2.0f;
				pElsePlayer->SetVA(vVelocityElsePlayer2, vAcceleration);
				pElsePlayer->SetCollisionState( true );
				pElsePlayer->UpdateCollisionState( g_pGame->GetServerGMTTime(), 0 );
#ifdef CDR_HL2_MOVE
				pPlayer->SetCDRInfoClipVelocity( vVelocityElsePlayer1 );
				pElsePlayer->SetCDRInfoClipVelocity( vVelocityElsePlayer2 );
#endif
				bRet = true;
			}
		}
	}

	//
	return bRet;
}
/*
void CECPlayerMan::UpdateAllCollisionStates(DWORD dwDeltaTime)
{
	//
	//
	PlayerTable::iterator iter = m_PlayerTab.begin();
	CECHostPlayer* pHost = GetHostPlayer();
	float fDeltaTime = dwDeltaTime * 0.001f;

	//Collision detection for the host player to all the else players.
	if ( pHost && pHost->GetHPWorkMan() )
	{
		CECHPWork* pP1Work = pHost->GetHPWorkMan()->GetWork_p1();
		if ( !pP1Work || (pP1Work && pP1Work->GetWorkID() != CECHPWork::WORK_SPECIAL_MOVE) )
		{
			A3DAABB aabb1 = pHost->GetPlayerPickAABB();
			for ( ; iter != m_PlayerTab.end(); ++iter )
			{
				CECElsePlayer* pElsePlayer = *iter.value();
				if ( !pElsePlayer )
				{
					continue;
				}
				A3DAABB aabb2 = pElsePlayer->GetPlayerPickAABB();
				//-------------------------------------------------------------
				//Calculate the new AABB for move states.
				A3DVECTOR3 vDist = pElsePlayer->GetVelocity() * fDeltaTime + pElsePlayer->GetAcceleration() * fDeltaTime * fDeltaTime * 0.5f;
				BrushTraceInfo  brushInfo;
				brushInfo.Init( aabb2.Center, vDist, aabb2.Extents );
				//-------------------------------------------------------------
				if ( CLS_AABBAABBOverlap(aabb1.Center, aabb1.Extents, brushInfo.BoundAABB.Center, brushInfo.BoundAABB.Extents) )	//the collision has been detected.
				{
					if ( MayCollision(pElsePlayer->GetPos(), pElsePlayer->GetVelocity(), pHost->GetPos(), pHost->GetVelocity(), aabb1.Extents.MagnitudeH()+aabb2.Extents.MagnitudeH(), fDeltaTime)>0 )
					{
						//compute the new velocity
						A3DVECTOR3 vVelocityElsePlayer = pElsePlayer->GetVelocity();
						A3DVECTOR3 vVelocityHostPlayer = pHost->GetVelocity();
						UpdateVelocityAfterCD( pHost->GetPos(), pElsePlayer->GetPos(), 1, 1, vVelocityHostPlayer, vVelocityElsePlayer );
						a_LogOutput( 1, "AfterColl, v1=(%f, %f), v1=(%f, %f)", 
							vVelocityHostPlayer.x, vVelocityHostPlayer.z, vVelocityElsePlayer.x, vVelocityElsePlayer.z);
						if ( pElsePlayer->GetVelocity().IsZero() )
						{
							pElsePlayer->StartSpecialMove();
						}
						A3DVECTOR3 vAcceleration = -vVelocityElsePlayer;
						vAcceleration.Normalize();
						vAcceleration *= 2.0f;
						pElsePlayer->SetAcceleration( vAcceleration );
						pElsePlayer->SetVelocity( vVelocityElsePlayer );
						pHost->SetVelocity( vVelocityHostPlayer );
					}
				}
			}
			//-------------------------------------------------------------
			if ( !pHost->GetVelocity().IsZero() )
			{
				CECHPWorkSpecialMove* pWork = (CECHPWorkSpecialMove*)pHost->GetHPWorkMan()->CreateWork(CECHPWork::WORK_SPECIAL_MOVE);
				if ( pWork )
				{
					pWork->SetVelocityAndAcceleration( pHost->GetVelocity(), A3DVECTOR3(1.0f) );
					pHost->GetHPWorkMan()->StartWork_p1(pWork);
				}
			}
			//-------------------------------------------------------------
		}
	}

	//Collision detection for all the else players.
	for ( iter = m_PlayerTab.begin(); iter != m_PlayerTab.end(); ++iter )
	{
		CECElsePlayer* pElsePlayer = *iter.value();
		if ( !pElsePlayer )
		{
			continue;
		}
		A3DAABB aabb1 = pElsePlayer->GetPlayerPickAABB();
		//-------------------------------------------------------------
		//Calculate the new AABB for move states.
		A3DVECTOR3 vDist = pElsePlayer->GetVelocity() * fDeltaTime + pElsePlayer->GetAcceleration() * fDeltaTime * fDeltaTime * 0.5f;
		BrushTraceInfo  brushInfo1;
		brushInfo1.Init( aabb1.Center, vDist, aabb1.Extents );
		//-------------------------------------------------------------

		PlayerTable::iterator iter2 = iter;
		for (  ++iter2; iter2 != m_PlayerTab.end(); ++iter2 )
		{
			CECElsePlayer* pElsePlayer2 = *iter2.value();
			if ( !pElsePlayer2 )
			{
				continue;
			}
			A3DAABB aabb2 = pElsePlayer2->GetPlayerPickAABB();
			//-------------------------------------------------------------
			//Calculate the new AABB for move states.
			A3DVECTOR3 vDist2 = pElsePlayer2->GetVelocity() * fDeltaTime + pElsePlayer2->GetAcceleration() * fDeltaTime * fDeltaTime * 0.5f;
			BrushTraceInfo  brushInfo2;
			brushInfo2.Init( aabb2.Center, vDist2, aabb2.Extents );
			//-------------------------------------------------------------
			if ( CLS_AABBAABBOverlap(brushInfo1.BoundAABB.Center, brushInfo1.BoundAABB.Extents, brushInfo2.BoundAABB.Center, brushInfo2.BoundAABB.Extents) )
			{
				if ( MayCollision(pElsePlayer->GetPos(), pElsePlayer->GetVelocity(), pElsePlayer2->GetPos(), pElsePlayer2->GetVelocity(), aabb1.Extents.MagnitudeH()+aabb2.Extents.MagnitudeH(), fDeltaTime)>0 )
				{
					A3DVECTOR3 vVelocityElsePlayer1 = pElsePlayer->GetVelocity();
					A3DVECTOR3 vVelocityElsePlayer2 = pElsePlayer2->GetVelocity();
					UpdateVelocityAfterCD( pElsePlayer->GetPos(), pElsePlayer2->GetPos(), 1, 1, vVelocityElsePlayer1, vVelocityElsePlayer2 );
					a_LogOutput( 1, "AfterColl, v1=(%f, %f), v1=(%f, %f)", 
						vVelocityElsePlayer1.x, vVelocityElsePlayer1.z, vVelocityElsePlayer2.x, vVelocityElsePlayer2.z);
					if ( pElsePlayer->GetVelocity().IsZero() && !vVelocityElsePlayer1.IsZero() )
					{
						pElsePlayer->StartSpecialMove();
					}
					pElsePlayer->SetVelocity( vVelocityElsePlayer1 );
					if ( pElsePlayer2->GetVelocity().IsZero() && !vVelocityElsePlayer2.IsZero() )
					{
						pElsePlayer2->StartSpecialMove();
					}
					pElsePlayer2->SetVelocity( vVelocityElsePlayer2 );
				}
			}
		}
	}
}
*/
//	Update unknown else player table
void CECPlayerMan::UpdateUnknownElsePlayers()
{
	DWORD dwRealTime = g_pGame->GetRealTickTime();
	if (!m_QueryCnt.IncCounter(dwRealTime))
		return;

	m_QueryCnt.Reset();

	//	Query unknown player's information
	if (m_UkPlayerTab.size())
	{
		AArray<int, int> aIDs;

		UkPlayerTable::iterator it = m_UkPlayerTab.begin();
		for (; it != m_UkPlayerTab.end(); ++it)
			aIDs.Add(*it.value());

		g_pGame->GetGameSession()->c2s_CmdQueryPlayerInfo1(aIDs.GetSize(), aIDs.GetData());

		m_UkPlayerTab.clear();
	}

	//	Query booth name
	if (m_aBoothQuery.GetSize())
	{
		g_pGame->GetGameSession()->c2s_CmdQueryBoothName(m_aBoothQuery.GetSize(), m_aBoothQuery.GetData());
		m_aBoothQuery.RemoveAll(false);
	}
}

//	Load player model in loading thread
bool CECPlayerMan::ThreadLoadPlayerModel(
	int cid,
	DWORD dwBornStamp,
	int iPhysique,
	int iProfession,
	int iLevel,
	int iEarid,
	int iTailid,
	int iFaceid,
	int iHairid,
	int iDefaultSkin,
	int iGender,
	const int* pEquips,
	const char* szPetPath,
	const char* szChangedPath)
{
	PLAYERMODEL* pInfo = new PLAYERMODEL;
	memset(pInfo, 0, sizeof(*pInfo));
	pInfo->cid = cid;
	pInfo->dwBornStamp = dwBornStamp;

	if (!CECPlayer::LoadPlayerModel(cid, iPhysique, iProfession, iLevel, iEarid, iTailid, iFaceid, iHairid, 
		iDefaultSkin, iGender, pEquips, szPetPath, szChangedPath, pInfo->Ret))
	{
		delete pInfo;
		return false;
	}

	ACSWrapper csa(&m_csLoad);
	m_aLoadedModels.Add(pInfo);

	return true;
}

//	Load player mirror model in loading thread
bool CECPlayerMan::ThreadLoadMirrorModel(
	int cid,
	DWORD dwBornStamp,
	int iPhysique,
	int iProfession,
	int iLevel,
	int iEarid,
	int iTailid,
	int iFaceid,
	int iHairid,
	int iDefaultSkin,
	int iGender,
	const int* pEquips,
	const char* szPetPath,
	const char* szChangedPath)
{
	PLAYERMODEL* pInfo = new PLAYERMODEL;
	memset(pInfo, 0, sizeof(*pInfo));
	pInfo->cid = cid;
	pInfo->dwBornStamp = dwBornStamp;
	pInfo->bOnlyMirrorModel = true;

	if (!CECPlayer::LoadMirrorModel(cid, iPhysique, iProfession, iLevel, iEarid, iTailid, iFaceid, iHairid, 
		iDefaultSkin, iGender, pEquips, szPetPath, szChangedPath, pInfo->Ret))
	{
		delete pInfo;
		return false;
	}

	ACSWrapper csa(&m_csLoad);
	m_aLoadedModels.Add(pInfo);

	return true;
}

bool CECPlayerMan::ThreadLoadPlayerEquips(int cid, DWORD dwBornStamp, int iPhysique, int iProfession, int iDefaultSkin, int iGender, const int* pEquips, DWORD dwEquipMask)
{
	PLAYERMODEL* pInfo = new PLAYERMODEL;
	memset(pInfo, 0, sizeof(*pInfo));
	pInfo->cid = cid;
	pInfo->dwBornStamp = dwBornStamp;
	pInfo->bOnlyEquip = true;

	if (!CECPlayer::LoadPlayerEquips(cid, iPhysique, iProfession, iDefaultSkin, iGender, dwEquipMask, pEquips, pInfo->Ret.EquipResult[0]))
	{
		delete pInfo;
		return false;
	}

	ACSWrapper csa(&m_csLoad);
	m_aLoadedModels.Add(pInfo);	

	return true;
}

bool CECPlayerMan::ThreadLoadMirrorPlayerEquips(int cid, DWORD dwBornStamp, int iPhysique, int iProfession, int iDefaultSkin, int iGender, const int* pEquips, DWORD dwEquipMask)
{
	PLAYERMODEL* pInfo = new PLAYERMODEL;
	memset(pInfo, 0, sizeof(*pInfo));
	pInfo->cid = cid;
	pInfo->dwBornStamp = dwBornStamp;
	pInfo->bOnlyMirrorEquip = true;

	for(int i=0;i<6;i++)
	{
		if (!CECPlayer::LoadPlayerEquips(cid, iPhysique, iProfession, iDefaultSkin, iGender, dwEquipMask, pEquips, pInfo->Ret.EquipResult[i]))
		{
			delete pInfo;
			return false;
		}
	}

	ACSWrapper csa(&m_csLoad);
	m_aLoadedModels.Add(pInfo);	

	return true;
}

bool CECPlayerMan::ThreadLoadPetModel(int cid, DWORD dwBornStamp, const char* szPetPath)
{
	PLAYERMODEL* pInfo = new PLAYERMODEL;
	memset(pInfo, 0, sizeof(*pInfo));
	pInfo->cid = cid;
	pInfo->dwBornStamp = dwBornStamp;
	pInfo->bOnlyPet = true;

	if (!CECPlayer::LoadPetModel(szPetPath, pInfo->Ret))
	{
		delete pInfo;
		return false;
	}

	ACSWrapper csa(&m_csLoad);
	m_aLoadedModels.Add(pInfo);

	return true;
}

bool CECPlayerMan::ThreadLoadChangedModel(int cid, DWORD dwBornStamp, const char* szPath)
{
	PLAYERMODEL* pInfo = new PLAYERMODEL;
	memset(pInfo, 0, sizeof(*pInfo));
	pInfo->cid = cid;
	pInfo->dwBornStamp = dwBornStamp;
	pInfo->bOnlyChangedModel = true;

	if (!CECPlayer::LoadChangedModel(szPath, pInfo->Ret))
	{
		delete pInfo;
		return false;
	}

	ACSWrapper csa(&m_csLoad);
	m_aLoadedModels.Add(pInfo);

	return true;
}

bool CECPlayerMan::ThreadLoadTransfiguredModel(int cid, DWORD dwBornStamp, const char* szPath)
{
	PLAYERMODEL* pInfo = new PLAYERMODEL;
	memset(pInfo, 0, sizeof(*pInfo));
	pInfo->cid = cid;
	pInfo->dwBornStamp = dwBornStamp;
	pInfo->bOnlyTransfiguredModel = true;

	if (!CECPlayer::LoadTransfiguredModel(szPath, pInfo->Ret))
	{
		delete pInfo;
		return false;
	}

	ACSWrapper csa(&m_csLoad);
	m_aLoadedModels.Add(pInfo);

	return true;
}

bool CECPlayerMan::ThreadLoadAircraftModel(int cid, DWORD dwBornStamp, const char* szPath)
{
	PLAYERMODEL* pInfo = new PLAYERMODEL;
	memset(pInfo, 0, sizeof(*pInfo));
	pInfo->cid = cid;
	pInfo->dwBornStamp = dwBornStamp;
	pInfo->bOnlyAircraft = true;

	if (!CECPlayer::LoadAircraftModel(szPath, pInfo->Ret))
	{
		delete pInfo;
		return false;
	}

	ACSWrapper csa(&m_csLoad);
	m_aLoadedModels.Add(pInfo);

	return true;
}

bool CECPlayerMan::ThreadLoadPuppetModel(int cid, DWORD dwBornStamp, const char* szPuppetPath)
{
	PLAYERMODEL* pInfo = new PLAYERMODEL;
	memset(pInfo, 0, sizeof(*pInfo));
	pInfo->cid = cid;
	pInfo->dwBornStamp = dwBornStamp;
	pInfo->bOnlyPuppetModel = true;

	if(!CECPlayer::LoadPuppetModel(szPuppetPath, pInfo->Ret))
	{
		delete pInfo;
		return false;
	}

	ACSWrapper csa(&m_csLoad);
	m_aLoadedModels.Add(pInfo);

	return true;
}

//	Deliver loaded player models
void CECPlayerMan::DeliverLoadedPlayerModels()
{
	ACSWrapper csa(&m_csLoad);

	for (int i=0; i < m_aLoadedModels.GetSize(); i++)
	{
		PLAYERMODEL* pInfo = m_aLoadedModels[i];
		CECPlayer* pPlayer = GetPlayer(pInfo->cid, pInfo->dwBornStamp);

		if (!pPlayer)
			CECPlayer::ReleasePlayerModel(pInfo->Ret);
		else
		{
			if (pInfo->bOnlyEquip)
			{
				if (!pPlayer->SetEquipsLoadedResult(pInfo->Ret.EquipResult[0], true))
					CECPlayer::ReleasePlayerModel(pInfo->Ret);
			}
			else if(pInfo->bOnlyMirrorEquip)
			{
				if (!pPlayer->SetMirrorEquipsLoadedResult(pInfo->Ret, true))
					CECPlayer::ReleasePlayerModel(pInfo->Ret);
			}
			else if (pInfo->bOnlyPet)
			{
				if (!pPlayer->SetPetLoadResult(pInfo->Ret.pPetModel))
					CECPlayer::ReleasePlayerModel(pInfo->Ret);
			}
			else if (pInfo->bOnlyChangedModel)
			{
				if (!pPlayer->SetChangedModelLoadResult(pInfo->Ret.pChangedModel))
					CECPlayer::ReleasePlayerModel(pInfo->Ret);
			}
			else if(pInfo->bOnlyTransfiguredModel)
			{
				if (!pPlayer->SetTransfigureModelLoadResult(pInfo->Ret.pTransfiguredModel))
					CECPlayer::ReleasePlayerModel(pInfo->Ret);
			}
			else if (pInfo->bOnlyAircraft)
			{
				if (!pPlayer->SetAircraftModelLoadResult(pInfo->Ret.pAircraftModel))
					CECPlayer::ReleasePlayerModel(pInfo->Ret);
			}
			else if(pInfo->bOnlyPuppetModel)
			{
				if(!pPlayer->SetPuppetModelLoadResult(pInfo->Ret.pPuppetModel))
					CECPlayer::ReleasePlayerModel(pInfo->Ret);
			}
			else if (pInfo->bOnlyMirrorModel)
			{
				if (!pPlayer->SetMirrorModelLoadResult(pInfo->Ret))
					CECPlayer::ReleasePlayerModel(pInfo->Ret);
			}
			else if (!pPlayer->SetPlayerLoadedResult(pInfo->Ret))
				CECPlayer::ReleasePlayerModel(pInfo->Ret);
		}

		delete pInfo;
	}

	m_aLoadedModels.RemoveAll(false);
}

int CECPlayerMan::TabSelectEnemy(int idCurSel)
{
	const float fSelDist = 20.0f;
	int i;

	if (!idCurSel)
	{
		//	Rebuild selected table
		m_aTabSels.RemoveAll(false);
	}
	else
	{
		for (i=0; i < m_aTabSels.GetSize(); i++)
		{
			if (m_aTabSels[i]->GetDistToHost() > fSelDist)
			{
				m_aTabSels.RemoveAtQuickly(i);
				i--;
			}
		}
	}

	float fMinDist = 10000.0f;
	CECElsePlayer* pCand = NULL;
	CECHostPlayer* pHost = GetHostPlayer();

	//	Trace all Else Players
	PlayerTable::iterator it = m_PlayerTab.begin();
	for (; it != m_PlayerTab.end(); ++it)
	{
		CECElsePlayer* pPlayer = *it.value();
		if (pPlayer->IsDead() || pPlayer->GetCharacterID() == idCurSel)
			continue;

		if (pHost->IsInBattle())
		{
			if (pPlayer->GetBattleCamp() == GP_BATTLE_CAMP_NONE || pPlayer->GetBattleCamp() == pHost->GetBattleCamp() || pPlayer->GetBattleCamp() == GP_BATTLE_CAMP_OBSERVER)
				continue;
		}
//		将帮战的判断放在attackable里面处理，可以打对方帮派的人，但是如果是可以攻击的闲杂人等，也是可以手动tab到的
// 		else if (pHost->GetCombatFaction())
// 		{
// 			if (pHost->GetCombatFaction() != pPlayer->GetFactionID())
// 				continue;
// 		}
		else if (pHost->GetPVPInfo().ucFreePVPType == 2)
		{
			if (pHost->GetCultivation() == pPlayer->GetCultivation())
				continue;
		}
		else if(pHost->AttackableJudge(pPlayer->GetCharacterID(), false) != 1)	// 不能被攻击，则不是敌人，则不能被tab选到
		{
			continue;
		}

		float fDist = pPlayer->GetDistToHost();
		if (fDist > fSelDist)
			continue;

		//	Check whether this npc is in selected array
		for (i=0; i < m_aTabSels.GetSize(); i++)
		{
			if (pPlayer == m_aTabSels[i])
				break;
		}

		if (i < m_aTabSels.GetSize())
			continue;	//	This player has been in selected array

		//	Record the nearest one as candidate
		if (fDist < fMinDist)
		{
			fMinDist = fDist;
			pCand	 = pPlayer;
		}
	}

	const int iMaxCand = 4;

	if (pCand)
	{
		if (m_aTabSels.GetSize() >= iMaxCand)
		{
			m_aTabSels.RemoveAt(m_aTabSels.GetSize()-1);
			m_aTabSels.InsertAt(0, pCand);
		}
		else
			m_aTabSels.Add(pCand);

		return pCand->GetCharacterID();
	}
	else	//	No proper candidate was found
	{
		if (!m_aTabSels.GetSize())
			return idCurSel;	//	Return without change

		//	Try to select one which has been in selected array
		int iIndex = -1;
		for (i=0; i < m_aTabSels.GetSize(); i++)
		{
			if (m_aTabSels[i]->GetCharacterID() == idCurSel)
			{
				iIndex = i;
				break;
			}
		}

		iIndex = (iIndex + 1) % m_aTabSels.GetSize();
		return m_aTabSels[iIndex]->GetCharacterID();
	}
}

// Get nearest player of specified position
int CECPlayerMan::GetNearestEnemyPlayer(const A3DVECTOR3& vTargetPos, float fRange, abase::vector<int> pVecTargets, float* fDistToTarget)
{
	size_t i;
	float fMinDist = 10000.0f;
	int idEnemyPlayer = 0;

	CECHostPlayer* pHost = GetHostPlayer();

	//	Trace all Else Players
	PlayerTable::iterator it = m_PlayerTab.begin();
	for (; it != m_PlayerTab.end(); ++it)
	{
		CECElsePlayer* pPlayer = *it.value();
		if (pPlayer->IsDead())
			continue;

		if (pHost->IsInBattle())
		{
			if (pPlayer->GetBattleCamp() == GP_BATTLE_CAMP_NONE || pPlayer->GetBattleCamp() == pHost->GetBattleCamp() || pPlayer->GetBattleCamp() == GP_BATTLE_CAMP_OBSERVER)
				continue;
		}
		else if (pHost->GetPVPInfo().ucFreePVPType == 2)
		{
			if (pHost->GetCultivation() == pPlayer->GetCultivation())
				continue;
		}
		else if(pHost->AttackableJudge(pPlayer->GetCharacterID(), false) != 1)	// 不能被攻击，则不是敌人，则不能被tab选到
		{
			continue;
		}

		float fDist = Magnitude(vTargetPos - pPlayer->GetPos());//pPlayer->GetDistToHost();
		if (fDist > fMinDist || fDist > fRange)
			continue;

		//	Check whether this npc is in target array
		for (i=0; i < pVecTargets.size(); i++)
		{
			if (pVecTargets[i] == pPlayer->GetCharacterID())
				break;
		}

		if (i < pVecTargets.size())
			continue;	//	This npc has been in selected array

		//	Record the nearest one as candidate
		if (fDist < fMinDist)
		{
			fMinDist = fDist;
			idEnemyPlayer = pPlayer->GetCharacterID();
		}
	}

	*fDistToTarget = fMinDist;
	return idEnemyPlayer;
}

//	Remove player from tab-selected array
void CECPlayerMan::RemovePlayerFromTabSels(CECElsePlayer* pPlayer)
{
	for (int i=0; i < m_aTabSels.GetSize(); i++)
	{
		if (m_aTabSels[i] == pPlayer)
		{
			m_aTabSels.RemoveAtQuickly(i);
			break;
		}
	}
}

void CECPlayerMan::ProcessNPC2PlayerMsg(const S2C::info_npc& Info)
{
	CECGameSession* pSession = g_pGame->GetGameSession();
	CECElsePlayer* pPlayer = ElsePlayerEnter(Info);
	if (pPlayer)
	{
		//	Is custom data ready
 		if (!pPlayer->IsBaseInfoReady())
 			pSession->GetRoleBaseInfo2(Info.id_summoner, Info.nid);

		//	Is equipment data ready
		//  在这里取还是等着召唤者出来之后再同步？
		//  如果在这里取，那么对于返回的结果需要遍历所有的玩家来更新对应的分身的状态
		int id = Info.id_summoner;
		CECPlayer* pSummoner = GetPlayer(id);
		if (!pPlayer->IsEquipDataReady())
		{
			if (pSummoner)
			{
				pSummoner->SetCloneID(Info.nid);
				if (pSummoner->IsHostPlayer())
				{
					ASSERT(0);
				}
				else
				{
					if ( ((CECElsePlayer*)pSummoner)->IsEquipDataReady() )
					{
						pPlayer->UpdateCurEquips(Info.id_summoner);
					}
					else
					{
						pSummoner->SetUpdateCloneEquip();
					}
				}
			}
			else
				pSession->c2s_CmdGetCloneEquip(id, Info.nid);
		}
		//	Get faction info
		if (pPlayer->GetFactionID())
			g_pGame->GetFactionMan()->GetFaction(pPlayer->GetFactionID(), true, false);
	}
}

void CECPlayerMan::ProcessNPC2PlayerMsg(int nid, int killerid)
{
	CECPlayer* pPlayer = GetPlayer(nid);
	if(pPlayer)
	{
		pPlayer->Killed(killerid);
		ElsePlayerLeave(nid, true);
	}	
}

void CECPlayerMan::ProcessNPC2PlayerMsg(S2C::cmd_npc_info_00& Info)
{
	CECPlayer* pPlayer = GetPlayer(Info.idNPC);
	if(pPlayer)
	{
		ROLEBASICPROP& bp = pPlayer->GetBasicProps();
		ROLEEXTPROP& ep = pPlayer->GetExtendProps();

		bp.iCurHP		= Info.iHP;
		ep.max_hp		= Info.iMaxHP;
	}	
}
