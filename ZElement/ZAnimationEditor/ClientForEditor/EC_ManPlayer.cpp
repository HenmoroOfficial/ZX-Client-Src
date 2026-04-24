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
//#include "EC_LoginPlayer.h"
#ifdef BACK_VERSION
#include "EC_Viewport.h"
#else
#include "ECViewport.h"
#endif
#include "EC_GPDataType.h"
#include "EC_GameRun.h"
#include "EC_Game.h"
//#include "EC_UIManager.h"
//#include "EC_GameUIMan.h"
//#include "EC_GameSession.h"
#include "EC_World.h"
#include "EC_CDS.h"
#include "EC_Utility.h"
#include "EC_Configs.h"
//#include "EC_Team.h"
//#include "EC_TeamMan.h"
//#include "EC_UIManager.h"
//#include "EC_GameUIMan.h"
//#include "EC_FixedMsg.h"
#include "EC_Resource.h"
#include "EC_SceneLoader.h"
#include "EC_Model.h"
//#include "EC_InputCtrl.h"
#include "EC_ManMatter.h"
//#include "EC_Faction.h"
//#include "EC_GameRecord.h"
//#include "EC_Cutscene.h"

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
	return true;
}

//	Handler of MSG_PM_PLAYERMOVE
bool CECPlayerMan::OnMsgPlayerMove(const ECMSG& Msg)
{
	return true;
}

//	Handler of MSG_PM_PLAYERRUNOUT
bool CECPlayerMan::OnMsgPlayerRunOut(const ECMSG& Msg)
{
	return true;
}

//	Handler of MSG_PM_PLAYEREXIT
bool CECPlayerMan::OnMsgPlayerExit(const ECMSG& Msg)
{
	return true;
}

//	Handler of MSG_PM_PLAYERDIED
bool CECPlayerMan::OnMsgPlayerDied(const ECMSG& Msg)
{
	return true;
}

//	Handler of MSG_PM_PLAYERREVIVE
bool CECPlayerMan::OnMsgPlayerRevive(const ECMSG& Msg)
{
	return true;
}

bool CECPlayerMan::OnMsgPlayerOutOfView(const ECMSG& Msg)
{
	return true;
}

bool CECPlayerMan::OnMsgPlayerDisappear(const ECMSG& Msg)
{
	return true;
}

bool CECPlayerMan::OnMsgPlayerStopMove(const ECMSG& Msg)
{
	return true;
}

bool CECPlayerMan::OnMsgPlayerLevelUp(const ECMSG& Msg)
{
	return true;
}

bool CECPlayerMan::OnMsgPlayerExtProp(const ECMSG& Msg)
{
	return true;
}

bool CECPlayerMan::OnMsgPlayerLeaveTeam(const ECMSG& Msg)
{
	return true;
}

bool CECPlayerMan::OnMsgDoConEmote(const ECMSG& Msg)
{
	return true;
}

bool CECPlayerMan::OnMsgInvalidObject(const ECMSG& Msg)
{
	return true;
}

bool CECPlayerMan::OnMsgPlayerExtState(const ECMSG& Msg)
{
	return true;
}

void CECPlayerMan::OnMsgPickUpMatter(const ECMSG& Msg)
{
}

void CECPlayerMan::OnMsgPlayerBindStart(const ECMSG& Msg)
{
}

void CECPlayerMan::OnMsgPlayerDuelResult(const ECMSG& Msg)
{
	
}

void CECPlayerMan::OnMsgTankLeader(const ECMSG& Msg)
{
	
}

/**
*@param:ECMSG, 将协议封装之后的消息体
*@return:void
*@usage:角色管理器初步处理所有的传递给角色的命令消息
**/
void CECPlayerMan::OnMsgPlayerScaleShape(const ECMSG& Msg)
{

}

void CECPlayerMan::OnMsgPlayerVIPAwardUpdate(const ECMSG& Msg)
{

}

//	Transmit message
bool CECPlayerMan::TransmitMessage(const ECMSG& Msg)
{

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

	//	Tick all players
	PlayerTable::iterator it = m_PlayerTab.begin();
	for (; it != m_PlayerTab.end(); ++it)
	{
		CECElsePlayer* pPlayer = *it.value();
		pPlayer->Tick(dwDeltaTime);
	}

	//	Update players in various ranges
	UpdatePlayerInRanges(dwDeltaTime);

	//	Tick Login Players here, if there are
// 	int nNumLoginPlayers = m_LoginPlayers.size();
// 	for(int i=0; i < nNumLoginPlayers; i++)
// 	{
// 		m_LoginPlayers[i]->Tick(dwDeltaTime);
// 	}

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

	bool bUseHintModel = g_pGame->GetConfigs()->GetVideoSettings().bModelLimit;

	//	Render all else players
	CECHostPlayer* pHost = GetHostPlayer();
	CECElsePlayer* pHHElsePlayer = NULL;	//	The player will be high-light rendered
	int idHoverObject = pHost ? pHost->GetCursorHoverObject() : 0;
	bool bRenderElse = pHost ? !pHost->IsChangingFace() : true;
	bool bRenderSelf = true;

	bool bIsPlayingCutscene = false;

// 	if( pHost && pHost->GetCutscene())
// 		bIsPlayingCutscene = pHost->GetCutscene()->IsPlayingCutscene();
// 		
// 	if(bIsPlayingCutscene)
// 	{
// 		if(!pHost->GetCutscene()->IsElsePlayerShow())
// 			bRenderElse = false;
// 
// 		if(!pHost->GetCutscene()->IsSelfShow())
// 			bRenderSelf = false;
// 	}

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
// 	int nNumLoginPlayers = m_LoginPlayers.size();
// 	for(int i=0; i<nNumLoginPlayers; i++)
// 	{
// 		m_LoginPlayers[i]->Render(pViewport);
// 	}

// 	if (g_pGame->GetGameRun()->GetGameState() == CECGameRun::GS_GAME)
// 	{
// 		//	Flush title font
// 		A3DFont* pFont = g_pGame->GetFont(RES_FONT_TITLE);
// 		pFont->Flush();
// 
// 		//	Flush emotion decals
// 		CECGameUIMan* pGameUI = g_pGame->GetGameRun()->GetUIManager()->GetInGameUIMan();
// 		A2DSprite* pFaceImg = NULL;
// 		pGameUI->GetEmotionList(&pFaceImg, NULL);
// 		if (pFaceImg)
// 			pFaceImg->GetTextures()->GetSpriteBuffer()->FlushBuffer();
// 	}

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
// 	int nNumLoginPlayers = m_LoginPlayers.size();
// 	for(int i=0; i<nNumLoginPlayers; i++)
// 	{
// 		m_LoginPlayers[i]->Render(pViewport, CECObject::RD_REFLECT);
// 	}

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
// 	int nNumLoginPlayers = m_LoginPlayers.size();
// 	for(int i=0; i<nNumLoginPlayers; i++)
// 	{
// 		m_LoginPlayers[i]->Render(pViewport, CECObject::RD_REFRACT);
// 	}

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
// 	int nNumLoginPlayers = m_LoginPlayers.size();
// 	for(int i=0; i<nNumLoginPlayers; i++)
// 	{
// 		m_LoginPlayers[i]->Render(pViewport, CECObject::RD_BLOOM);
// 	}

	return true;
}

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

// 	if (pHost->GetFactionID())
// 		g_pGame->GetFactionMan()->GetFaction(pHost->GetFactionID(), true, false);
// 
// 	if (pHost->GetFamilyID())
// 		g_pGame->GetFactionMan()->GetFaction(pHost->GetFamilyID(), true, true);

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
// 		if (pPlayer->GetCharacterID() == pHost->GetSelectedTarget())
// 			g_pGame->GetGameSession()->c2s_CmdUnselect();

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
// 	if (GetHostPlayer() && 
// 		GetHostPlayer()->GetCutscene() && 
// 		GetHostPlayer()->GetCutscene()->IsPlayingCutscene() && 
// 		!GetHostPlayer()->GetCutscene()->IsNameShow())
// 	{
// 		bRenderNameByCamera = false;
// 	}

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
// 	if (pTraceInfo->dwKeyStates & EC_KSF_SHIFT)
// 		return true;
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
			paabb = &pPlayer->GetPlayerPickAABB();
		
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

//		g_pGame->GetGameSession()->c2s_CmdQueryPlayerInfo1(aIDs.GetSize(), aIDs.GetData());

		m_UkPlayerTab.clear();
	}

	//	Query booth name
	if (m_aBoothQuery.GetSize())
	{
//		g_pGame->GetGameSession()->c2s_CmdQueryBoothName(m_aBoothQuery.GetSize(), m_aBoothQuery.GetData());
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

	if (!CECPlayer::LoadPlayerModel(iPhysique, iProfession, iLevel, iEarid, iTailid, iFaceid, iHairid, 
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

	if (!CECPlayer::LoadPlayerEquips(iPhysique, iProfession, iDefaultSkin, iGender, dwEquipMask, pEquips, pInfo->Ret.EquipResult))
	{
		delete pInfo;
		return false;
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
				if (!pPlayer->SetEquipsLoadedResult(pInfo->Ret.EquipResult, true))
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

#ifndef BACK_VERSION
bool CECPlayerMan::RenderForShadow( ECViewport* pViewport )
{
	PlayerTable::iterator it = m_PlayerTab.begin();
	for (; it != m_PlayerTab.end(); ++it)
	{
		CECElsePlayer* pPlayer = *it.value();
		pPlayer->Render(pViewport, CECObject::RD_SHADOW);
	}

	//	Render host player
	CECHostPlayer* pHost = GetHostPlayer();
	if (pHost)
		pHost->Render(pViewport, CECObject::RD_SHADOW);

	//	Render Login Players here, if there are
/*	int nNumLoginPlayers = m_LoginPlayers.size();
	for(int i=0; i<nNumLoginPlayers; i++)
	{
		m_LoginPlayers[i]->Render(pViewport, CECObject::RD_SHADOW);
	}
*/
	return true;
}
#endif