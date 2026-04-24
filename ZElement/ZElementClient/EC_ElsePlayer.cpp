/*
 * FILE: EC_ElsePlayer.cpp
 *
 * DESCRIPTION: 
 *
 * CREATED BY: Duyuxin, 2004/9/10
 *
 * HISTORY: 
 *
 * Copyright (c) 2004 Archosaur Studio, All Rights Reserved.
 */

#include "EC_Global.h"
#include "EC_ElsePlayer.h"
#include "EC_Game.h"
#include "EC_GameRun.h"
#include "EC_World.h"
#include "EC_UIManager.h"
#include "EC_GameUIMan.h"
#include "EC_Model.h"
#include "EC_ShadowRender.h"
#include "EC_NPC.h"
#include "EC_Interactor.h"
#ifdef BACK_VERSION
#include "EC_Viewport.h"
#else
#include "ECViewport.h"
#endif
#include "EC_ManNPC.h"
#include "EC_GFXCaster.h"
#include "EC_TeamMan.h"
#include "EC_Team.h"
#include "EC_ManPlayer.h"
#include "EC_FixedMsg.h"
#include "EC_IvtrItem.h"
#include "EC_IvtrTalismanMain.h"
#include "EC_IvtrTypes.h"
#include "EC_Resource.h"
#include "EC_GameSession.h"
#include "EC_HostPlayer.h"
#include "EC_HostMove.h"
#include "EC_Utility.h"
#include "EC_Skill.h"
#include "EC_PateText.h"
#include "EC_Configs.h"
#include "EC_CDR.h"
#include "EC_GameRecord.h"
#include "EC_Achievement.h"
#include "EC_RTDebug.h"

#include "EC_Game.h"
#include "EC_Configs.h"
#include "EC_Sprite.h"
#include "elementdataman.h"

#include "PlayerBaseInfo_Re.hpp"
#include "PlayerBaseInfo2_Re.hpp"
#include "GetCustomData_Re.hpp"

#include "A3DGFXExMan.h"

#include <A3DFont.h>
#include <A3DCamera.h>
#include <A3DCameraBase.h>
#include <A3DViewport.h>
#include <A3DLight.h>
#ifdef BACK_VERSION
#include <A3DTerrainWater.h>
#else
#include <A3DTerrainWater2.h>
#endif
#include "A3DTerrain2.h"
#include "elementdataman.h"
#include "EC_LoginPlayer.h"
#include "EC_IvtrConsume.h"
#define new A_DEBUG_NEW

///////////////////////////////////////////////////////////////////////////
//	
//	Define and Macro
//	
///////////////////////////////////////////////////////////////////////////

#define MAX_LAGDIST							8.0f	//	Maximum lag distance
#define COLLISION_STATE_TIME_EPSILON		2		//	碰撞状态时间误差(秒)
#define COLLISION_STATE_CLEAR_TIME			3		//	清除碰撞状态时间差

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
//	Class CECEPCacheData
//	
///////////////////////////////////////////////////////////////////////////

CECEPCacheData::CECEPCacheData()
{
	m_dwResFlags	= 0;
	m_cid			= 0;
	m_crc_e			= -1;
	m_crcBooth		= 0;
}


MovingCorrect::MovingCorrect(CECElsePlayer *p) : m_pPlayer(p)
{
	m_vPos = p->GetPos();
	m_vVelocity = p->GetVelocity();
	m_vAcceleration = p->GetAcceleration();
	m_iLastRefresh=0; 
}

void MovingCorrect::Tick(DWORD dwDeltaTime)
{
	m_iLastRefresh += dwDeltaTime;
	if (m_iLastRefresh>2000)
	{
		m_vAcceleration = -m_pPlayer->GetVelocity();
		m_vAcceleration.Normalize();
		m_vAcceleration *= 2;
		m_pPlayer->SetVA(m_pPlayer->GetVelocity(), m_vAcceleration);
		m_iLastRefresh = -10000;
	}
	return;
	if (!m_bStop)
	{
		if (m_iLastRefresh>500)
		{
			m_vAcceleration = -m_pPlayer->GetVelocity();
			m_vAcceleration.Normalize();
			m_vAcceleration *= 2;
			m_pPlayer->SetVA(m_pPlayer->GetVelocity(), m_vAcceleration);
		}
		if (m_iLastRefresh>2000)
		{
			m_pPlayer->GetElsePos();
			m_iLastRefresh = -1000;
		}
	}
	if (m_bStop)
	{
		if (m_pPlayer->GetVelocity().Magnitude()>0.0001)
		{
			m_pPlayer->SetPos(m_vPos);
			m_pPlayer->SetVA(0, 0);
			m_vVelocity = 0;
			m_vAcceleration = 0;
			SetVA(0, 0);
		}
		if (m_iLastRefresh>5000)
		{
			m_pPlayer->GetElsePos();
			m_iLastRefresh = 0;
		}
/*		m_vAcceleration = m_pPlayer->GetPos()-m_vPos;
		if (m_vAcceleration.Normalize()<0.0001)
		{
			m_pPlayer->StopSpecialMove();
			m_vVelocity = 0;
		}
		else
		{
			m_vVelocity = m_vPos-m_pPlayer->GetPos();
			m_pPlayer->SetVA(m_vVelocity, m_vAcceleration);
		}
*/	}
}

void MovingCorrect::CorrectPos(const A3DVECTOR3 &p)
{
	m_iLastRefresh=0; 
	
	m_vPos = p;
}

void MovingCorrect::SetPos(const A3DVECTOR3 &p)
{
	m_iLastRefresh=0; 

	m_vPos = p;
}

void MovingCorrect::SetVA(const A3DVECTOR3 &v, const A3DVECTOR3 &a)
{
	m_iLastRefresh=0; 

	m_vVelocity = v;
	m_vAcceleration = a;
	if (v.Magnitude()>0.0001)
	{
		m_bStop = false;
	}
}

///////////////////////////////////////////////////////////////////////////
//	
//	Implement CECElsePlayer
//	
///////////////////////////////////////////////////////////////////////////

CECElsePlayer::CECElsePlayer(CECPlayerMan* pPlayerMan) : 
CECPlayer(pPlayerMan),
m_vServerPos(0.0f)
{
	m_iCID				= OCID_ELSEPLAYER;
	m_iAttackTarget		= 0;
	m_iCurWork			= 0;
	m_iCurWorkType		= -1;
	m_bStopMove			= false;
	m_fMoveSpeed		= 0.0f;		
	m_iMoveMode			= MOVE_STAND;
	m_bCastShadow		= true;
	m_bEquipReady		= false;
	m_bBaseInfoReady	= false;
	m_pAppearGFX		= NULL;
	m_dwNewEqpMask		= 0;
	m_fDistToHost		= 0.0f;
	m_fDistToHostH		= 0.0f;
	m_iMMIndex			= -1;
	m_bJumping			= false;
	m_iInTeamState		= 0;
	m_fLastSpeed		= 0.0f;
	m_dwLastMoveTime	= 0;
	m_bLoadingModel		= false;
	m_fSkillSpeedRate	= 1.0f;	//Added 2011-06-17.
	m_fYDelta			= 0.0f;
	m_bUseHintModel		= g_pGame->GetConfigs()->GetVideoSettings().bModelLimit;
	m_vVelocity.Clear();
	m_vAcceleration.Clear();
	m_pMovingCorrect = new MovingCorrect(this);
//	m_bServerCollisionState	= false;
	m_iItemWingOrg = 0;

	m_PickupCnt.SetPeriod(500);
	m_FightCnt.SetPeriod(15000);
	m_FightCnt.Reset(true);


	m_pcdr = new OtherPlayer_Move_Info;
	m_pAchievementMan = new CECAchievementMan();
	m_pCDRInfo	= new CDR_INFO;
	
	memset(m_aNewEquips, 0, sizeof (m_aNewEquips));
}

CECElsePlayer::~CECElsePlayer()
{
	delete m_pMovingCorrect;
	delete m_pcdr;
	//
	delete m_pCDRInfo;
}

//	Initlaize object
bool CECElsePlayer::Init(const S2C::info_player_1& Info, int iAppearFlag, bool bReInit)
{
	using namespace S2C;

	if (!bReInit)
	{
		m_PlayerInfo.cid	= Info.cid;
		m_PlayerInfo.crc_e	= -1;
	}

	m_iAppearFlag = iAppearFlag;

	//	Set player info.
	SetPlayerInfo(Info);

	//	Start a work do nothing will avoid some errors
	StartWork(WT_NOTHING, WORK_STAND);


	//	Check equipment data crc
	m_bEquipReady = (m_PlayerInfo.crc_e == Info.crc_e) ? true : false;

	//	Query booth name
	if (m_iBoothState == 2)
		m_pPlayerMan->QueryPlayerBoothName(Info.cid);

	if (!bReInit)
	{
		m_pcdr->fStepHeight	= m_MoveConst.fStepHei;
		m_pcdr->vExts		= m_aabb.Extents;
		m_pcdr->vVelocity.Clear();
	}

	if (!m_pAppearGFX && m_iAppearFlag == APPEAR_ENTERWORLD)
	{
		CECGFXCaster* pGFXCaster = g_pGame->GetGFXCaster();
		m_pAppearGFX = pGFXCaster->LoadGFXEx(res_GFXFile(RES_GFX_PLAYERAPPEAR));
		pGFXCaster->PlayGFXEx(m_pAppearGFX, GetPos(), GetDir(), 1.0f);
	}
	SetFlowBattle(m_iFlowBattle);
	return true;
}

//	Initlaize object
bool CECElsePlayer::Init(const S2C::info_npc& Info, int iAppearFlag, bool bReInit)
{
	using namespace S2C;

	if (!bReInit)
	{
		m_PlayerInfo.cid	= Info.nid;
		m_PlayerInfo.crc_e	= -1;
	}

	m_iAppearFlag = iAppearFlag;

	//	Set player info.
	SetPlayerInfo(Info);

	//	Start a work do nothing will avoid some errors
	StartWork(WT_NOTHING, WORK_STAND);

	//	save equipment data crc
//	m_bEquipReady = (m_PlayerInfo.crc_e == Info.crc_e) ? true : false;
	CECPlayer* pSummoner = m_pPlayerMan->GetPlayer(Info.id_summoner);
	if (pSummoner && (pSummoner->IsHostPlayer() || ((CECElsePlayer*)pSummoner)->IsEquipDataReady()))
	{
		memcpy(m_aEquips, pSummoner->GetEquips(), sizeof(m_aEquips));
		memcpy(m_aHideEquips, pSummoner->GetHideEquips(), sizeof(m_aHideEquips));
		memcpy(m_aNewEquips, pSummoner->GetHideEquips(), sizeof(m_aHideEquips));
		m_PlayerInfo.crc_e = pSummoner->GetPlayerInfo().crc_e;
		m_bEquipReady = true;
	}
	else
		m_bEquipReady = false;

	if (pSummoner)
		SetPlayerBriefInfo(pSummoner->m_iFaceid, pSummoner->m_iHairid, pSummoner->m_iEarid, pSummoner->m_iTailid,
		pSummoner->m_iDefaultSkin, pSummoner->m_iGender, pSummoner->m_strName);

	if (!bReInit)
	{
		m_pcdr->fStepHeight	= m_MoveConst.fStepHei;
		m_pcdr->vExts		= m_aabb.Extents;
		m_pcdr->vVelocity.Clear();
	}

// 	if (!m_pAppearGFX && m_iAppearFlag == APPEAR_ENTERWORLD)
// 	{
// 		CECGFXCaster* pGFXCaster = g_pGame->GetGFXCaster();
// 		m_pAppearGFX = pGFXCaster->LoadGFXEx(res_GFXFile(RES_GFX_PLAYERAPPEAR));
// 		pGFXCaster->PlayGFXEx(m_pAppearGFX, GetPos(), GetDir(), 1.0f);
// 	}

	return true;
}

GNET::Octets CECElsePlayer::gen_info_player_1()
{
	GNET::Octets o;
	o.reserve(sizeof(S2C::info_player_1) + 64);

	add_to_octets<int>				(o, m_PlayerInfo.cid);
	add_to_octets<A3DVECTOR3>		(o, GetPos());
	add_to_octets<unsigned short>	(o, m_PlayerInfo.crc_e);
	add_to_octets<unsigned char>	(o, glb_CompressDirH(GetDir().x, GetDir().z));
	add_to_octets<unsigned char>	(o, m_BasicProps.iProfession);
	add_to_octets<unsigned char>	(o, m_BasicProps.iLevel);
	add_to_octets<unsigned char>	(o, m_nRebornCount);
	add_to_octets<unsigned char>	(o, m_BasicProps.iDeityLevel);
	add_to_octets<char>				(o, m_BasicProps.iPKLevel);
	add_to_octets<char>				(o, m_iCultivation);
	add_to_octets<int>				(o, m_iReputation);
	add_to_octets<int>				(o, m_dwHideEquipMask);

	size_t offset = o.size();
	int state = 0;
	add_to_octets<int>				(o, state);
	int extra_state = 0;
	add_to_octets<int>				(o, extra_state);

	if (m_iChangeShape)
	{
		state |= GP_STATE_SHAPE;
		add_to_octets<int>			(o, m_iChangeShape);
	}
	
	if (m_dwExtStates)
	{
		state |= GP_STATE_EXTEND_PROPERTY;
		add_to_octets<__int64>			(o, m_dwExtStates);
	}

	if (m_idFaction)
	{
		state |= GP_STATE_FACTION;
		add_to_octets<int>			(o, m_idFaction);
		add_to_octets<char>			(o, m_idFRole);
	}

	if (m_iBoothState == 2)
	{
		state |= GP_STATE_BOOTH;
		add_to_octets<unsigned char>(o, m_crcBooth);
	}

	if (m_aCurEffects.GetSize())
	{
		state |= GP_STATE_EFFECT;
		add_to_octets<unsigned char>(o, m_aCurEffects.GetSize());
		
		for (int i = 0; i < m_aCurEffects.GetSize(); i++)
			add_to_octets<short>	(o, m_aCurEffects[i]);
	}

	if (m_RidingPet.id)
	{
		state |= GP_STATE_IN_MOUNT;
		add_to_octets<unsigned char>(o, m_RidingPet.iLevel);
		add_to_octets<int>			(o, m_RidingPet.id);
		add_to_octets<unsigned char>(o, m_RidingPet.bMultiRider? 1:0);
	}

	if (m_iBuddyId)
	{
		state |= GP_STATE_IN_BIND;
		char  bind_type;
		if (m_bMultiRideLeader)
			bind_type = 3;
		else if (m_bMultiRideMember)
			bind_type = 4;
		else
			bind_type = 1;
		add_to_octets<char>			(o, bind_type);
		if (bind_type == 1 || bind_type == 2 || bind_type == 5 || bind_type == 6)
		{
			add_to_octets<int>			(o, m_iBuddyId);
			add_to_octets<char>			(o, 0);		
		}
		else if (bind_type == 3)
		{
			add_to_octets<int>			(o, 0);
			add_to_octets<char>			(o, 0);			
		}
		else if (bind_type == 4)
		{
			add_to_octets<int>			(o, m_iLeaderId);
			add_to_octets<char>			(o, m_iMultiRidePos);
		}
	}

	if (m_iTitle)
	{
		state |= GP_STATE_TITLE;
		add_to_octets<short>		(o, m_iTitle);
	}

	if (m_idSpouse)
	{
		state |= GP_STATE_SPOUSE;
		add_to_octets<int>			(o, m_idSpouse);
	}

	if (m_idFamily)
	{
		state |= GP_STATE_FAMILY;
		add_to_octets<int>			(o, m_idFamily);
	}

	if (m_idMaster)
	{
		state |= GP_STATE_SECT_MASTER_ID;
		add_to_octets<int>			(o, m_idMaster);
	}

	if (m_iMilitaryRank)
	{
		state |= GP_STATE_IN_SJBATTLE;
		add_to_octets<unsigned char>(o, m_iMilitaryRank);
	}
	
	if (m_iVipState)
	{
		state |= GP_STATE_IN_VIPSTATE;
		add_to_octets<unsigned char>(o, m_iVipState);
	}

	if (m_TransfigureInfo.id)
	{
		state |= GP_STATE_TRANSFIGURE;
		add_to_octets<int>			(o, m_TransfigureInfo.id);
	}
	
	if (m_bFashionMode)
		state |= GP_STATE_FASHION;

	if (m_iBattleCamp == GP_BATTLE_CAMP_INVADER)
		state |= GP_STATE_BC_INVADER;
	else if (m_iBattleCamp == GP_BATTLE_CAMP_DEFENDER)
		state |= GP_STATE_BC_DEFENDER;

	if (m_dwGMFlags)
		state |= GP_STATE_GMFLAG;

	if (m_pvp.bEnable)
		state |= GP_STATE_PVP_ENABLE;

	if (m_pvp.bInPVPCombat)
		state |= GP_STATE_INPVPCOMBAT;

	if (m_pvp.iDuelState == DUEL_ST_INDUEL)
		state |= GP_STATE_IN_DUEL;

	if (m_iInTeamState == 2)
		state |= GP_STATE_TEAMLEADER;
	else if (m_iInTeamState == 1)
		state |= GP_STATE_TEAM;
	
	if (IsDead())
		state |= GP_STATE_CORPSE;

	if (IsSitting())
		state |= GP_STATE_SITDOWN;

	if (IsFlying())
		state |= GP_STATE_FLY;
	
	if (m_idCarrier)
	{
		extra_state |= GP_EXTRA_STATE_CARRIER;
		add_to_octets<int>			(o, m_idCarrier);
		add_to_octets<A3DVECTOR3>	(o, GetPos());
		add_to_octets<unsigned char>(o, glb_CompressDirH(GetDir().x, GetDir().z));
	}
	if (m_idCircle)
	{
		extra_state |= GP_EXTRA_STATE_CIRCLE;
		add_to_octets<int>			(o, m_idCircle);
		add_to_octets<unsigned char>(o, m_idCRole);
	}
	if (m_idOrgServer)
	{
		extra_state |= GP_EXTRA_STATE_CROSSZONE;
		add_to_octets<int>			(o, m_idOrgServer);
	}

	if (m_bAnonymous)
		extra_state |= GP_EXTRA_STATE_ANONYMOUS;

	if (m_bInvisible)
		extra_state |= GP_EXTRA_STATE_INVISIBLE;

	if (m_bDim)
		extra_state |= GP_EXTRA_STATE_DIM;

	if (m_iSpecialState & SPECIAL_STATE_IGNITE)
		extra_state |= GP_EXTRA_STATE_IGNITE;
	if (m_iSpecialState & SPECIAL_STATE_FROZEN)
		extra_state |= GP_EXTRA_STATE_FROZEN;
	if (m_iSpecialState & SPECIAL_STATE_FURY)
		extra_state |= GP_EXTRA_STATE_FURY;
	if (m_iSpecialState & SPECIAL_STATE_DARK)
		extra_state |= GP_EXTRA_STATE_DARK;

	//Added 2012-01-16.
	if (m_ucVIPLevel)
	{
		extra_state |= GP_EXTRA_STATE_VIP_AWARD;
		add_to_octets<unsigned char>(o, m_ucVIPLevel);
	}
	//Added end.

	//Added 2012-08-01.
	if ( m_iCollisionSkillState )
	{
		extra_state |= GP_EXTRA_STATE_COLLISION_RAID;
		add_to_octets<int>(o, m_TransfigureInfo.id);
		add_to_octets<int>(o, m_iCollisionSkillState);
	}
	//Added end.

	//Added 2012-08-16.
	if ( m_cXuanYuanDarkLightState )
	{
		extra_state |= GP_EXTRA_STATE_XY_DARK_LIGHT;
		add_to_octets<char>(o, m_cXuanYuanDarkLightState);
	}
	//Added end.

	bool bHasSpirit = false;
	int i;
	for(i=0;i<3;i++)
	{
		if(m_cDarkLightSpirits[i])
		{
			bHasSpirit = true;
			break;
		}
	}

	if(bHasSpirit)
	{
		extra_state |= GP_EXTRA_STATE_XY_SPIRITS;
		for(i=0;i<3;i++)
			add_to_octets<char>(o, m_cDarkLightSpirits[i]);
	}

	if(m_iMirrorImageCnt)
	{
		extra_state |= GP_EXTRA_STATE_MIRROR_IMAGE;
		add_to_octets<char>(o, m_iMirrorImageCnt);
	}

	//Added 2012-08-20.
	if ( m_ucWingChangeColor )
	{
		extra_state |= GP_EXTRA_STATE_CHANGE_WING_COLOR;
		add_to_octets<unsigned char>(o, m_ucWingChangeColor);
	}
	//Added end.
	
	if ( m_iExtraEquipEffect>0 )
	{
		extra_state |= GP_EXTRA_STATE_EXTRA_EQUIP_EFFECT;
		add_to_octets<unsigned char>(o, m_iExtraEquipEffect);
	}

	if(m_iFlowBattle > 0)
	{
		extra_state |= GP_EXTRA_STATE_FLOW_BATTLE;
		add_to_octets<short>(o, m_iFlowBattle);
	}
	
	if(m_iPuppetId > 0)
	{
		extra_state |= GP_EXTRA_STATE_PUPPET;
		add_to_octets<int>(o, m_iPuppetId);
	}

	// set state flag
	memcpy((char*)o.begin() + offset, &state, sizeof(state));
	memcpy((char*)o.begin() + offset +sizeof(int), &extra_state, sizeof(extra_state));
	return o;
}

//	Load resources
bool CECElsePlayer::InitFromCache(const S2C::info_player_1& Info, CECEPCacheData* pCacheData,
								int iAppearFlag)
{
	if (!pCacheData)
	{
		ASSERT(pCacheData);
		return Init(Info, iAppearFlag, false);
	}

	using namespace S2C;

	m_PlayerInfo.cid = pCacheData->m_cid;

	m_iAppearFlag = iAppearFlag;

	//	Set player brief info
	if (!SetPlayerBriefInfo(pCacheData->m_iFaceid, pCacheData->m_iHairid, pCacheData->m_iEarid, pCacheData->m_iTailid,
		pCacheData->m_iDefaultSkin, pCacheData->m_iGender, pCacheData->m_strName))
	{
		a_LogOutput(1, "CECElsePlayer::InitFromCache, Failed to set brief info");
		return false;
	}

	//	Set player info.
	SetPlayerInfo(Info);

	//	Start a work do nothing will avoid some errors
	StartWork(WT_NOTHING, WORK_STAND);

	//	Check equipment data crc
	if ((pCacheData->m_dwResFlags & RESFG_SKIN) && pCacheData->m_crc_e == Info.crc_e)
	{
		m_bEquipReady = true;

		memcpy(m_aNewEquips, pCacheData->m_aEquips, sizeof (m_aNewEquips));
		m_dwNewEqpMask = EQUIP_MASK_ALL;
		m_PlayerInfo.crc_e = Info.crc_e;

	#ifdef _DEBUG
		
		for (int i=0; i < SIZE_EQUIPIVTR; i++)
		{
			if (m_aNewEquips[i] < 0)
			{
				ASSERT(m_aNewEquips[i] >= 0);
			}
		}

	#endif	//	_DEBUG
	}
	else
	{
		m_bEquipReady = false;
	}

	if (m_iBoothState == 2)
	{
		//	Query booth name if necessary
		if (pCacheData->m_crcBooth == m_crcBooth && (pCacheData->m_dwResFlags & CECEPCacheData::RESFG_BOOTHNAME))
			SetBoothName(pCacheData->m_strBoothName);
		else
			m_pPlayerMan->QueryPlayerBoothName(Info.cid);
	}

	m_pcdr->fStepHeight	= m_MoveConst.fStepHei;
	m_pcdr->vExts		= m_aabb.Extents;
	m_pcdr->vVelocity.Clear();

	if (!m_pAppearGFX && m_iAppearFlag == APPEAR_ENTERWORLD)
	{
		CECGFXCaster* pGFXCaster = g_pGame->GetGFXCaster();
		m_pAppearGFX = pGFXCaster->LoadGFXEx(res_GFXFile(RES_GFX_PLAYERAPPEAR));
		pGFXCaster->PlayGFXEx(m_pAppearGFX, GetPos(), GetDir(), 1.0f);
	}

	return true;
}

// the clone body get the cache data by the summoner id
bool CECElsePlayer::InitFromCache(const S2C::info_npc& Info, CECEPCacheData* pCacheData,
								int iAppearFlag)
{
	if (!pCacheData)
	{
		ASSERT(pCacheData);
		return Init(Info, iAppearFlag, false);
	}

	using namespace S2C;

//	m_PlayerInfo.cid = pCacheData->m_cid;
	m_PlayerInfo.cid = Info.nid;
	m_iAppearFlag = iAppearFlag;

	//	Set player brief info
	if (!SetPlayerBriefInfo(pCacheData->m_iFaceid, pCacheData->m_iHairid, pCacheData->m_iEarid, pCacheData->m_iTailid, 
		pCacheData->m_iDefaultSkin, pCacheData->m_iGender, pCacheData->m_strName))
	{
		a_LogOutput(1, "CECElsePlayer::InitFromCache, Failed to set brief info");
		return false;
	}

	//	Set player info.
	SetPlayerInfo(Info);

	//	Start a work do nothing will avoid some errors
	StartWork(WT_NOTHING, WORK_STAND);

	//	Check equipment data crc
	CECPlayer* pSummoner = m_pPlayerMan->GetPlayer(Info.id_summoner);

	if (pSummoner && (pCacheData->m_dwResFlags & RESFG_SKIN) && pCacheData->m_crc_e == pSummoner->GetPlayerInfo().crc_e)
	{
		m_bEquipReady = true;

		memcpy(m_aNewEquips, pCacheData->m_aEquips, sizeof (m_aNewEquips));
		m_dwNewEqpMask = EQUIP_MASK_ALL;
		m_PlayerInfo.crc_e = pSummoner->GetPlayerInfo().crc_e;

	#ifdef _DEBUG
		
		for (int i=0; i < SIZE_EQUIPIVTR; i++)
		{
			if (m_aNewEquips[i] < 0)
			{
				ASSERT(m_aNewEquips[i] >= 0);
			}
		}

	#endif	//	_DEBUG
	}
	else
	{
		m_bEquipReady = false;
	}

	m_pcdr->fStepHeight	= m_MoveConst.fStepHei;
	m_pcdr->vExts		= m_aabb.Extents;
	m_pcdr->vVelocity.Clear();

// 	if (!m_pAppearGFX && m_iAppearFlag == APPEAR_ENTERWORLD)
// 	{
// 		CECGFXCaster* pGFXCaster = g_pGame->GetGFXCaster();
// 		m_pAppearGFX = pGFXCaster->LoadGFXEx(res_GFXFile(RES_GFX_PLAYERAPPEAR));
// 		pGFXCaster->PlayGFXEx(m_pAppearGFX, GetPos(), GetDir(), 1.0f);
// 	}

	return true;
}

//	Set player info
void CECElsePlayer::SetPlayerInfo(const S2C::info_player_1& Info)
{
	using namespace S2C;

	m_dwStates = Info.state;
	m_BasicProps.iLevel = Info.level;
	m_BasicProps.iPKLevel = Info.pk_level;
	m_BasicProps.iProfession = Info.profession;
	m_BasicProps.iDeityLevel = Info.dt_level;
	m_iReputation = Info.reputation;
	m_nRebornCount = Info.reborn_count;
	m_iCultivation = Info.cultivation;
	m_dwHideEquipMask = Info.hide_equip;
//	m_iRace			= g_pGame->GetGameInit().iRaceTest;
	m_iRace			= CECPlayer::GetRaceByProf(Info.profession);
	m_iPhysique		= CECPlayer::GetPhysiqueByProf(Info.profession);

	//	Update position and direction
	if (Info.extra_state & GP_EXTRA_STATE_CARRIER)
	{
		//	Player is on a carrier NPC
		m_idCarrier = Info.id_carrier;
		m_vRelServerPos = Info.rpos;
		m_vRelStopDir = glb_DecompressDirH(Info.rdir);
		const A3DMATRIX4 matTrans = a3d_ViewMatrix(m_vRelServerPos, m_vRelStopDir, g_vAxisY, 0);
		m_matRelCarrier = a3d_InverseTM(matTrans);
	}
	else
	{
		SetServerPos(Info.pos);
		SetPos(Info.pos);
		SetDirAndUp(glb_DecompressDirH(Info.dir), g_vAxisY);
	}

	//	Set environment
	m_iMoveEnv = MOVEENV_GROUND;
	
	if (Info.state & GP_STATE_FLY)
		m_iMoveEnv = MOVEENV_AIR;
	else
	{
		A3DVECTOR3 vGndPos, vNormal, vTestPos = Info.pos + g_vAxisY;
		VertRayTrace(vTestPos, vGndPos, vNormal);
		float fWaterHei = g_pGame->GetGameRun()->GetWorld()->GetWaterHeight(vTestPos);

		if (Info.pos.y < fWaterHei - m_MoveConst.fShoreDepth)
			m_iMoveEnv = MOVEENV_WATER;
		else if (Info.pos.y < fWaterHei && fWaterHei - vGndPos.y > m_MoveConst.fShoreDepth)
			m_iMoveEnv = MOVEENV_WATER;
	}

	//	Set in team state
	m_iInTeamState = 0;
	if (Info.state & GP_STATE_TEAMLEADER)
		m_iInTeamState = 2;
	else if (Info.state & GP_STATE_TEAM)
		m_iInTeamState = 1;

	//	Parse battle camp
	m_iBattleCamp = GP_BATTLE_CAMP_NONE;
	if (Info.state & GP_STATE_BC_INVADER)
		m_iBattleCamp = GP_BATTLE_CAMP_INVADER;
	else if (Info.state & GP_STATE_BC_DEFENDER)
		m_iBattleCamp = GP_BATTLE_CAMP_DEFENDER;
	else if (Info.state & GP_STATE_IS_OBSERVER)
		m_iBattleCamp = GP_BATTLE_CAMP_OBSERVER;

	//	Parse travel flag
	m_bFashionMode = (Info.state & GP_STATE_FASHION) ? true : false;
	m_iBoothState = (Info.state & GP_STATE_BOOTH) ? 2 : 0;
	if(m_iBoothState == 2)
	{
		m_idBoothItem = Info.booth_item_id;
		if(m_idBoothItem > 0)
		{
			DATA_TYPE DataType;
			BOOTH_FIGURE_ITEM_ESSENCE* pDBEssence = (BOOTH_FIGURE_ITEM_ESSENCE*)g_pGame->GetElementDataMan()->get_data_ptr(m_idBoothItem, ID_SPACE_ESSENCE, DataType);
			if(pDBEssence && DataType==DT_BOOTH_FIGURE_ITEM_ESSENCE)
				m_pSpeBoothModel = m_pPlayerMan->LoadSpeBoothModel(pDBEssence->file_model);
		}
	}
	m_dwGMFlags = (Info.state & GP_STATE_GMFLAG) ? GMF_IAMGM : 0;
	m_pvp.bEnable = (Info.state & GP_STATE_PVP_ENABLE) ? true : false;
	m_pvp.bInPVPCombat = (Info.state & GP_STATE_INPVPCOMBAT) ? true : false;
	m_pvp.iDuelState = (Info.state & GP_STATE_IN_DUEL) ? DUEL_ST_INDUEL : DUEL_ST_NONE;

	if (Info.state & GP_STATE_ADV_MODE)
		DecompAdvData(Info.adv_d1, Info.adv_d2);

	m_iChangeShape		= Info.shape;
	m_dwExtStates		= Info.ext_states;
	m_crcBooth			= Info.crc_booth;
	m_RidingPet.iLevel	= Info.riding_pet_lvl;
	m_RidingPet.id		= Info.id_riding_pet;
	m_RidingPet.bMultiRider = Info.bMultiRide ? true:false;
	if(Info.bind_type == 1 || Info.bind_type ==2)
	{
		m_idCandBuddy		= Info.id_buddy;
	}
	else if(Info.bind_type == 3 || m_RidingPet.bMultiRider)
	{
		m_bMultiRideLeader = true;
	}
	else if(Info.bind_type == 4)
	{
		m_iCandLeaderId = Info.id_buddy;
		m_iCandMultiRidePos = Info.buddy_pos;
	}
	else if(Info.bind_type == 5 || Info.bind_type == 6)
	{
	    m_idCandBathBuddy = Info.id_buddy;
		m_bCandHangerOn   = (Info.bind_type == 5);
	}
	else if(Info.bind_type == 7 || Info.bind_type == 8)
	{
		m_idQilinConnectBuddy = Info.id_buddy;
		m_bQilinConnectHangerOn = (Info.bind_type == 8);
		if(!Info.bDisConnect)
		{
			m_idCandQilinBuddy = Info.id_buddy;
			m_bCandHangerOn = (Info.bind_type == 8);
		}

		if(Info.bind_type == 7 && Info.bDisConnect)
		{
			m_bAddQilinLinkGfx = true;
		}
		
	}
	m_idFRole			= Info.fam_rank;
	SetTitle(Info.title);
	SetSpouse(Info.id_spouse);
	SetFactionID(Info.id_faction);
	SetFamilyID(Info.id_family);
	SetMaster(Info.master_id);
	
	// 和主角在同一个战场中
	m_iBattleType = g_pGame->GetGameRun()->GetHostPlayer()->GetBattleType();
	// 和主角在同一个泡澡地图
	m_iBathType = g_pGame->GetGameRun()->GetHostPlayer()->GetBathType();
	if( m_iBattleType == BATTLE_TYPE_ARENA || m_iBattleType == BATTLE_TYPE_ARENAREBORN || m_iBattleType == BATTLE_TYPE_CRSSVR || m_iBathType != -1)
	{
		m_bFashionModeBackUp = m_bFashionMode;
		m_bFashionMode = false;
		if(m_iBathType != -1)
		   m_bFashionMode = true;
		// 此时模型还没有加载完成，没有办法来更新skin
//		UpdateCurSkins(); 
	//	m_dwHideEquipMask = m_HideEquipMaskInArena;
	}
	if(m_iBattleType == BATTLE_TYPE_FLPW_CRSSVR && (m_iTitle < 12001 || m_iTitle > 12004))
	{
		SetTitle(0);
	}
	if(Info.state & GP_STATE_IN_SJBATTLE)
	{
		SetMilitaryRank(Info.military_rank);
	}
	if(Info.state & GP_STATE_IN_VIPSTATE)
	{
		m_iVipState = Info.vip_state;
	}
	if (Info.state & GP_STATE_TRANSFIGURE)
	{
		Transfigure(Info.id_transfiguredProp);
	}
	if (Info.extra_state & GP_EXTRA_STATE_ANONYMOUS)
	{
		m_bAnonymous = true;
		g_pGame->GetGameRun()->GetHostPlayer()->SetAnonymousState(true);
	}
	else 
		g_pGame->GetGameRun()->GetHostPlayer()->SetAnonymousState(false);
	
	if (Info.extra_state & GP_EXTRA_STATE_INVISIBLE)
	{
		m_bInvisible = true;
	}
	if (Info.extra_state & GP_EXTRA_STATE_DIM)
	{
		m_bDim       = true;
	}
	if (Info.extra_state & GP_EXTRA_STATE_CLONE)
	{
//		m_bHasClone	 = true;
	}
	if (Info.extra_state & GP_EXTRA_STATE_CIRCLE)
	{
		SetCircleID(Info.id_circle);
		SetCRole(Info.circle_role);
	}
	if (Info.extra_state & GP_EXTRA_STATE_CROSSZONE)
	{
		m_idOrgServer = Info.id_orgserver;
	}
	if (Info.extra_state & GP_EXTRA_STATE_IGNITE)
		m_iSpecialState |= SPECIAL_STATE_IGNITE;
	if (Info.extra_state & GP_EXTRA_STATE_FROZEN)
		m_iSpecialState |= SPECIAL_STATE_FROZEN;
	if (Info.extra_state & GP_EXTRA_STATE_FURY)
		m_iSpecialState |= SPECIAL_STATE_FURY;
	if (Info.extra_state & GP_EXTRA_STATE_DARK)
		m_iSpecialState |= SPECIAL_STATE_DARK;

	if (Info.extra_state & GP_EXTRA_STATE_CIRCLEOFDOOM)//如果该玩家是一个阵法的阵眼，那么请求gs查询阵法信息 Added 2011-06-22.
	{
		m_iDelayedOperateState |= GP_EXTRA_STATE_CIRCLEOFDOOM;
		//g_pGame->GetGameSession()->c2s_CmdQueryCodInfo( Info.cid ); //Canceled 2011-08-26.
	}

	if (Info.extra_state & GP_EXTRA_STATE_BE_DRAGGED)//如果该玩家正处于被牵引状态，那么向gs请求查询牵引信息 Added 2011-08-26.
	{
		m_iDelayedOperateState |= GP_EXTRA_STATE_BE_DRAGGED;
		//g_pGame->GetGameSession()->c2s_CmdQueryBeDraggedInfo( Info.cid ); //Canceled 2011-08-26.
	}

	if (Info.extra_state & GP_EXTRA_STATE_BE_PULLED)//如果该玩家正处于被拉扯的状态，那么向gs请求查询拉扯信息 Added 2011-08-26.
	{
		m_iDelayedOperateState |= GP_EXTRA_STATE_BE_PULLED;
		//g_pGame->GetGameSession()->c2s_CmdQueryBePulledInfo( Info.cid ); //Canceled 2011-08-26.
	}

	//Added 2012-01-16.
	if (Info.extra_state & GP_EXTRA_STATE_VIP_AWARD)
	{
		m_ucVIPLevel = Info.vip_award_level;
		m_cIsHideVip = Info.is_hide_vip_level;
	}
	//Added end.

	//Added 2012-08-01.
	if (Info.extra_state & GP_EXTRA_STATE_COLLISION_RAID)
	{
		//新版变身
		NewTransfigure( Info.collision_raid_transfigure_id, true );
		m_iCollisionSkillState = Info.collision_raid_state;
		//-----------------------------------------------------------
		//清理一下ElsePlayer的速度和加速度等信息
		m_bSpecialMoving = false;
		SetCollisionState(false);
//		SetServerCollisionState(false);
		SetCDRInfoClipVelocity(A3DVECTOR3(0.0f, 0.0f, 0.0f));
		//-----------------------------------------------------------
	}
	//Added end.

	//Added 2012-08-16.
	if ( Info.extra_state & GP_EXTRA_STATE_XY_DARK_LIGHT )
	{
		m_cXuanYuanDarkLightState = Info.xuanyuan_dark_light_state;
	}
	//Added end.

	if ( Info.extra_state & GP_EXTRA_STATE_DIR_VISIBLE_STATE )
	{
		SetDirVisibleStates( Info.visible_state_id, Info.visible_state_dir, Info.visible_state_on );
	}
	
	if ( Info.extra_state & GP_EXTRA_STATE_EXTRA_EQUIP_EFFECT )
	{
		m_iExtraEquipEffect = Info.weapon_effect_level;
	}

	if ( Info.extra_state & GP_EXTRA_STATE_FLOW_BATTLE )
	{
		m_iFlowBattle = Info.flow_battle_ckill;
	}

	if(Info.extra_state & GP_EXTRA_STATE_PUPPET)
	{
		m_iPuppetId = Info.puppetid;
		m_iCandPupPetId = Info.puppetid;
	}

	if(Info.extra_state & GP_EXTRA_STATE_INTERACT)
	{
		m_CandInteractorInfo.id = Info.interactor_id;
		m_CandInteractorInfo.pos= Info.interact_pos;
		m_CandInteractorInfo.state = Info.interact_state;
	}

	if (Info.extra_state & GP_EXTRA_STATE_ACTIVE_EMOTE)
	{
		if (Info.active_emote_inviter)
		{
			m_CandEmoteInfo.id = Info.active_emote_id;
			m_CandEmoteInfo.type = Info.active_emote_type;
		}
	}

	//Added 2012-08-20.
	if ( Info.extra_state & GP_EXTRA_STATE_CHANGE_WING_COLOR )
	{
		m_ucWingChangeColor = Info.change_color;
		//!!!调用相应ChangeEquipments接口 ？？？
	}
	//Added end.

	if ( Info.extra_state & GP_EXTRA_STATE_XY_SPIRITS )
	{
		for(int i=0;i<3;i++)
			m_cDarkLightSpirits[i] = Info.dark_light_spirits[i];
	}

	if ( Info.extra_state & GP_EXTRA_STATE_MIRROR_IMAGE)
		m_iMirrorImageCnt = Info.mirror_image_cnt;
	
	if ( Info.extra_state & GP_EXTRA_STATE_TRAVEL)
	{
		m_iTravalVehicleId = Info.traval_vehicle_id;
	}

	if (Info.state & GP_STATE_FLY)
		m_bSkillFly = (Info.start_fly_type==1) ? true : false;

	m_aCurEffects.RemoveAll(false);
	for (int i=0; i < Info.effect_num; i++)
		m_aCurEffects.UniquelyAdd(Info.effects[i]);

	//-------------------------------------------------------------------------
	if ( m_pCDRInfo )	//专用于ElsePlayer的移动时碰撞检测，变量初始化
	{
		m_pCDRInfo->vExtent			= m_aabb.Extents;
#ifdef CDR_HL2_MOVE
		m_pCDRInfo->vClipVel.Clear();
#else
		m_pCDRInfo->fYVel			= 0.0f;
#endif
		m_pCDRInfo->vAbsVelocity.Clear();
		m_pCDRInfo->bOnSurface		= true;
		m_pCDRInfo->bCanStay		= true;
		m_pCDRInfo->fSlopeThresh	= 0.7071f;
		m_pCDRInfo->fStepHeight		= 0.8f;
		m_pCDRInfo->vTPNormal		= g_vAxisY;
	}
	//-------------------------------------------------------------------------
}

void CECElsePlayer::SetPlayerInfo(const S2C::info_npc& Info)
{
	using namespace S2C;
	CECPlayer* pSummoner = m_pPlayerMan->GetPlayer(Info.id_summoner);
	m_idSummoner	= Info.id_summoner;

	{
		SetServerPos(Info.pos);
		SetPos(Info.pos);
		SetDirAndUp(glb_DecompressDirH(Info.dir), g_vAxisY);
	}

	{
		A3DVECTOR3 vGndPos, vNormal, vTestPos = Info.pos + g_vAxisY;
		VertRayTrace(vTestPos, vGndPos, vNormal);
		float fWaterHei = g_pGame->GetGameRun()->GetWorld()->GetWaterHeight(vTestPos);

		if (Info.pos.y < fWaterHei - m_MoveConst.fShoreDepth)
			m_iMoveEnv = MOVEENV_WATER;
		else if (Info.pos.y < fWaterHei && fWaterHei - vGndPos.y > m_MoveConst.fShoreDepth)
			m_iMoveEnv = MOVEENV_WATER;
	}

	m_iMoveEnv = MOVEENV_GROUND;

	if (!pSummoner)
		return;
	m_dwStates =  pSummoner->m_dwStates;
	m_BasicProps = pSummoner->m_BasicProps;
	m_ExtProps.max_hp	= pSummoner->m_ExtProps.max_hp;
	m_ExtProps.max_mp	= pSummoner->m_ExtProps.max_mp;
	m_iReputation = pSummoner->m_iReputation;
	m_nRebornCount = pSummoner->m_nRebornCount;
	m_iCultivation = pSummoner->m_iCultivation;
	m_dwHideEquipMask = pSummoner->m_dwHideEquipMask;
	m_iRace			= CECPlayer::GetRaceByProf(m_BasicProps.iProfession);
	m_iPhysique		= CECPlayer::GetPhysiqueByProf(m_BasicProps.iProfession);
	m_idSummoner	= Info.id_summoner;
	m_ucVIPLevel	= pSummoner->m_ucVIPLevel;
	
	m_iBattleCamp = pSummoner->m_iBattleCamp;

	m_bFashionMode = pSummoner->m_bFashionMode;
	m_pvp = pSummoner->m_pvp;
	
	m_iChangeShape		= pSummoner->m_iChangeShape;
	m_dwExtStates		= pSummoner->m_dwExtStates;

	m_RidingPet.iLevel	= pSummoner->m_RidingPet.iLevel;
	m_RidingPet.id		= pSummoner->m_RidingPet.id;
	
	m_idFRole			= pSummoner->m_idFRole;
	SetTitle(pSummoner->m_iTitle);
	SetSpouse(pSummoner->m_idSpouse);
	SetFactionID(pSummoner->m_idFaction);
	SetFamilyID(pSummoner->m_idFamily);
	SetMaster(pSummoner->m_idMaster);
	SetCircleID(pSummoner->m_idCircle);
	SetCRole(pSummoner->m_idCRole);
	
	// 和主角在同一个战场中
	m_iBattleType = g_pGame->GetGameRun()->GetHostPlayer()->GetBattleType();
	// 和主角在同一个泡澡地图
	m_iBathType = g_pGame->GetGameRun()->GetHostPlayer()->GetBathType();
	if( m_iBattleType == BATTLE_TYPE_ARENA || m_iBattleType == BATTLE_TYPE_ARENAREBORN || m_iBattleType == BATTLE_TYPE_CRSSVR || m_iBathType != -1)
	{
		m_bFashionModeBackUp = m_bFashionMode;
		m_bFashionMode = false;
		if(m_iBathType != -1)
		  m_bFashionMode = true;
		// 此时模型还没有加载完成，没有办法来更新skin
		//		UpdateCurSkins(); 
	}
	if (m_iBattleType == BATTLE_TYPE_SONJIN || m_iBattleType == BATTLE_TYPE_SONJINREBORN)
	{
		SetMilitaryRank(pSummoner->m_iMilitaryRank);
	}
	if(m_iBattleType == BATTLE_TYPE_FLPW_CRSSVR && (m_iTitle < 12001 && m_iTitle > 12004))
	{
		SetTitle(0);
	}
	m_iVipState = pSummoner->m_iVipState;
	
	if (pSummoner->m_TransfigureInfo.id)
	{
		Transfigure(pSummoner->m_TransfigureInfo.id);
	}
	m_bAnonymous = pSummoner->m_bAnonymous;
	m_bDim			= pSummoner->m_bDim;
	m_idOrgServer   = pSummoner->m_idOrgServer;

	if (pSummoner->IsHostPlayer() || ((CECElsePlayer*)pSummoner)->IsEquipDataReady())
	{
		memcpy(m_aEquips, pSummoner->GetEquips(), sizeof(m_aEquips));
		memcpy(m_aHideEquips, pSummoner->GetHideEquips(), sizeof(m_aHideEquips));
		memcpy(m_aNewEquips, pSummoner->GetHideEquips(), sizeof(m_aHideEquips));
		m_PlayerInfo.crc_e = pSummoner->GetPlayerInfo().crc_e;
		m_bEquipReady = true;
	}
	else
		m_bEquipReady = false;
}
//	Release object
void CECElsePlayer::Release()
{
	if (m_pAppearGFX)
	{
		g_pGame->GetGFXCaster()->ReleaseGFXEx(m_pAppearGFX);
		m_pAppearGFX = NULL;
	}

	if (m_pCurSkill)
	{
		//	Release current skill
		delete m_pCurSkill;
		m_pCurSkill = NULL;
	}

	m_idCurSkillTarget = 0;
	m_bLoadingModel = false;

	CECPlayer::Release();
}

//	When all resources are ready, this function will be called
void CECElsePlayer::OnAllResourceReady()
{
	CECPlayer::OnAllResourceReady();

	//	Update extend states. Clear old states before to avoid adding
	//	same state effect twice.
	__int64 dwExtStates = m_dwExtStates;
	SetNewExtendStates(0);
	SetNewExtendStates(dwExtStates);
	ResetPlayLuteAction();

	//	Apply effects
	for (int i=0; i < m_aCurEffects.GetSize(); i++)
		ApplyEffect(m_aCurEffects[i], false);

	//	更新需要延迟操作的玩家状态 Added 2011-08-26.
	if ( m_iDelayedOperateState & GP_EXTRA_STATE_CIRCLEOFDOOM )
	{
		g_pGame->GetGameSession()->c2s_CmdQueryCodInfo( m_PlayerInfo.cid );
	}
	if ( m_iDelayedOperateState & GP_EXTRA_STATE_BE_DRAGGED )
	{
		g_pGame->GetGameSession()->c2s_CmdQueryBeDraggedInfo( m_PlayerInfo.cid );
	}
	if ( m_iDelayedOperateState & GP_EXTRA_STATE_BE_PULLED )
	{
		g_pGame->GetGameSession()->c2s_CmdQueryBePulledInfo( m_PlayerInfo.cid );
	}
	m_iDelayedOperateState = 0;
	//Added end.
}

bool CECElsePlayer::SetPetLoadResult(CECModel* pPetModel)
{
	if (CECPlayer::SetPetLoadResult(pPetModel))
	{
		m_pcdr->vExts = m_aabb.Extents;
		return true;
	}

	return false;
}

//	Create player cache data
CECEPCacheData* CECElsePlayer::CreateCacheData()
{
	if (!m_bBaseInfoReady)
		return NULL;

	CECEPCacheData* pData = new CECEPCacheData;

	//	Copy basic info
	pData->m_cid			= m_PlayerInfo.cid;
	pData->m_iFaceid		= m_iFaceid;
	pData->m_iHairid		= m_iHairid;
	pData->m_iEarid			= m_iEarid;
	pData->m_iTailid		= m_iTailid;
	pData->m_iDefaultSkin	= m_iDefaultSkin;
	pData->m_iRace			= m_iRace;
	pData->m_iPhysique      = m_iPhysique;
	pData->m_iGender		= m_iGender;
	pData->m_strName		= GetName();	//	Force to rebuild a string

	if (IsEquipDataReady())
	{
	#ifdef _DEBUG
		
		for (int i=0; i < SIZE_EQUIPIVTR; i++)
		{
			if (m_aNewEquips[i] < 0)
			{
				ASSERT(m_aNewEquips[i] >= 0);
			}
		}

	#endif	//	_DEBUG

		//	Note: IsEquipDataReady() return true only means m_aNewEquips is
		//		ready, but not assure m_aEquips is ready !!
		pData->m_crc_e	= m_PlayerInfo.crc_e;
		memcpy(pData->m_aEquips, m_aNewEquips, sizeof (m_aNewEquips));
		pData->m_dwResFlags |= CECPlayer::RESFG_SKIN;
	}

	if (m_iBoothState == 2 && m_strBoothName.GetLength())
	{
		pData->m_crcBooth		= m_crcBooth;
		pData->m_strBoothName	= GetBoothName();	//	Force to rebuild a string
		pData->m_dwResFlags		|= CECEPCacheData::RESFG_BOOTHNAME;
	}

	return pData;
}

//	Tick routine
bool CECElsePlayer::Tick(DWORD dwDeltaTime)
{
	CECPlayer::Tick(dwDeltaTime);

#ifndef BACK_VERSION
	A3DCameraBase* pCamera = g_pGame->GetViewport()->GetA3DCameraBase();
	m_fDistToCamera	= CalcDist(pCamera->GetPos(), true);
#endif

	if(g_pGame->GetGameRun()->IsIn6v6Battle())
		m_pMovingCorrect->Tick(dwDeltaTime);

	CECHostPlayer* pHost = m_pPlayerMan->GetHostPlayer();
	bool bSelected = pHost->GetSelectedTarget() == GetPlayerInfo().cid;

	if (!m_pPlayerModel && !m_bLoadingModel && IsBaseInfoReady() && IsEquipDataReady())
	{
		if( (!m_bUseHintModel && m_iBoothState != 2) || bSelected)
		{
			LoadPlayerSkeleton(m_aNewEquips, false);
			m_bLoadingModel = true;
		}
	}

	UpdateMirrorAndSpiritModels(dwDeltaTime);

	// 同步分身的装备
	if (m_iCloneID && m_bUpdateCloneEquip && m_bEquipReady)
	{
		CECElsePlayer* pClone = m_pPlayerMan->GetElsePlayer(m_iCloneID);
		if (pClone)
		{
			pClone->UpdateCurEquips(m_PlayerInfo.cid);
		}
		m_bUpdateCloneEquip = false;
	}

	CECNPCMan* pNPCMan = g_pGame->GetGameRun()->GetWorld()->GetNPCMan();
	//	Player is on a carrier
	if (m_idCarrier)
	{
		//	Apply carrier's position and direction change to player
		A3DVECTOR3 vPos, vDir, vSerPos, vStopDir;
		if (pNPCMan->GetPoseOnCarrier(m_idCarrier, m_matRelCarrier, vPos, vDir))
		{
			SetPos(vPos);
			SetDirAndUp(vDir, g_vAxisY);
			
			//	Update server pos	
			vSerPos = m_vRelServerPos * pNPCMan->GetNPC(m_idCarrier)->GetAbsoluteTM();
			vStopDir = a3d_VectorMatrix3x3(m_vRelStopDir, pNPCMan->GetNPC(m_idCarrier)->GetAbsoluteTM());
			SetServerPos(vSerPos);
			vStopDir.y = 0.0f;
			m_vStopDir = Normalize(vStopDir);
		}
	}

	switch (m_iCurWork)
	{
	case WORK_STAND:		TickWork_Stand(dwDeltaTime);		break;
	case WORK_MOVE:			TickWork_Move(dwDeltaTime);			break;
	case WORK_HACKOBJECT:	TickWork_Melee(dwDeltaTime);		break;
	case WORK_SPELL:		TickWork_Spell(dwDeltaTime);		break;
	case WORK_DEAD:			TickWork_Dead(dwDeltaTime);			break;
	case WORK_IDLE:			TickWork_Idle(dwDeltaTime);			break;
	case WORK_FLASHMOVE:	TickWork_FlashMove(dwDeltaTime);	break;
	case WORK_SPECIALMOVE:	TickWork_SpecialMove(dwDeltaTime);	break;
	case WORK_BLEWUP:		TickWork_BlewUp(dwDeltaTime);		break;
	case WORK_PARABOLIC:	TickWork_Parabolic(dwDeltaTime);	break;
	case WORK_MOVENOCOLLISION: TickWork_MoveNoCollision(dwDeltaTime); break;
	}

	if ( m_iBoothState!=2 && !g_pGame->GetGameRun()->IsHideElsePlayer() )
	{	
		//（由于模型缩放后陷入地形下）如果正处于归神/脱灵技能并且不处于坐骑上面时，那么重设模型的位置Added 2011-06-28.
		if ( !IS_EQUAL(m_fScaleShapeRatio,0.0f) && !m_pPetModel/*&& m_GndInfo.bOnGround*/ )
		{
//			float fTerrainHeight = g_pGame->GetGameRun()->GetWorld()->GetTerrain()->GetPosHeight( GetPos() );
//			float fDeltaHeight = fTerrainHeight - m_pPlayerModel->GetModelAABB().Mins.y;//GetA3DSkinModel()->GetModelAABB()
				
//			if ( fabsf(fDeltaHeight) > 0.005f )
			{
//				A3DVECTOR3 vCurPos = GetPos();
//				vCurPos.y = fTerrainHeight + 0.001f;
				if ( m_pPlayerModel ) //模型指针存在，则按照公式设置模型位置，但HostPlayer的位置不变
				{
					m_pPlayerModel->SetPos(GetPos() + A3DVECTOR3(0.0f, (m_fScaleShapeRatio - 1.0f)*(m_aabb.Extents.y+0.14f), 0.0f));//vCurPos
				}
			}
		}
		//Added end.

		//Added 2011-06-17.
		DWORD dwDeltaTimeTmp = dwDeltaTime;
		if ( !IS_EQUAL(m_fSkillSpeedRate,1.0f) ) //效率起见，做此判断
		{
			float fDeltaTime = (float)dwDeltaTime * m_fSkillSpeedRate;
			dwDeltaTime = (DWORD)fDeltaTime;
		}
		//Added end.
		if (m_AttachMode != enumAttachNone && m_bHangerOn)
		{
		}
		else if (m_bAttachedInteractor && !m_bInteractiveSubject)
		{
		}
		else if (m_pTransfiguredModel)
		{
			m_pTransfiguredModel->SetSlowestUpdateFlag(!m_bVisible || m_bUseHintModel);
			m_pTransfiguredModel->Tick(dwDeltaTime);
		}
		else if (m_pChangedModel)
		{
			m_pChangedModel->SetSlowestUpdateFlag(!m_bVisible || m_bUseHintModel);
			m_pChangedModel->Tick(dwDeltaTime);
		}
		else if (m_pPetModel)
		{
			m_pPetModel->SetSlowestUpdateFlag(!m_bVisible || m_bUseHintModel);
			m_pPetModel->Tick(dwDeltaTime);
		}
		else if(m_pPuppetModel)
		{
			m_pPuppetModel->SetSlowestUpdateFlag(!m_bVisible || m_bUseHintModel);
			m_pPuppetModel->Tick(dwDeltaTime);
		}
		else if (m_pBackupModel)
		{
			m_pBackupModel->SetSlowestUpdateFlag(!m_bVisible || m_bUseHintModel);
			m_pBackupModel->Tick(dwDeltaTime);
		}

		if( IsShapeChanged() && m_pDummyModel)
		{
			m_pDummyModel->SetSlowestUpdateFlag(!m_bVisible || m_bUseHintModel);
			m_pDummyModel->Tick(dwDeltaTime);
		}
		
		//Added 2011-06-17.
		dwDeltaTime = dwDeltaTimeTmp;
		//Added end.

	}
	if(m_iBoothState==2)
		UpdateBoothModel();

	if (m_pAppearGFX)
	{
		if (m_pAppearGFX->GetState() == ST_STOP)
		{
			g_pGame->GetGFXCaster()->ReleaseGFXEx(m_pAppearGFX);
			m_pAppearGFX = NULL;
		}
		else
			m_pAppearGFX->SetParentTM(GetAbsoluteTM());
	}

	//	Calculate distance to host player
	if (pHost && pHost->IsSkeletonReady())
	{
		m_fDistToHost  = CalcDist(pHost->GetPos(), true);
		m_fDistToHostH = CalcDist(pHost->GetPos(), false);
	}

	// 如果在竞技场中，更换衣服，换成原始的装备，主角在HostMsg中处理，下面的处理是否放在
	// ElsePlayer de Tick 
	if( (m_iBattleType == BATTLE_TYPE_ARENA || m_iBattleType == BATTLE_TYPE_ARENAREBORN || m_iBattleType == BATTLE_TYPE_CRSSVR) &&
		m_pBackupModel && m_bLoadFlag && !m_bHasChangedEquips)
	{
		int aEquips[SIZE_EQUIPIVTR];
		memset(aEquips, 0, sizeof(aEquips));
//		memcpy(aEquips, m_aEquips, sizeof(aEquips));
		m_dwHideEquipMaskBackUp = m_dwHideEquipMask;
		m_dwHideEquipMask = 0;
		int index = 0;
		CECPlayer::SECT_LEVEL2_INFO* pInfo = CECPlayer::GetSectInfoByProf(GetProfession());
		for (int i= 0; i< NUM_SECT; ++i)
		{
			if(pInfo && (pInfo->mask_sect & (1<<i)) )
			{	
				index = i;
			}
		}
		index = index * 2 + m_iGender;
		ASSERT(index>=0 && index <30);
		memcpy(aEquips, m_EquipsInArena[index], sizeof(m_EquipsInArena[index]));
		ShowEquipments(aEquips,true, true);
		m_bHasChangedEquips = true;
	}
	//在泡澡地图中，更换衣服，换成泡澡的装备，主角在HostMsg中处理
	bool bReady = true;	
	if(m_iBathType != -1 && m_BasicProps.iProfession >= 108 && m_BasicProps.iProfession <= 112)
	{
		DWORD dwMask = (1 << SKIN_FASHION_HEADWEAR_INDEX);
		if(m_pBackupModel != m_pCacheModel && !(dwMask & m_dwTetrapodFashionMask))	
			bReady = true;
		else
			bReady = false;
	}
	if(m_iBathType != -1 && m_pBackupModel && m_bLoadFlag && !m_bHasChangedEquips && bReady)
	{
		int aEquips[SIZE_EQUIPIVTR];
		memset(aEquips, 0, sizeof(aEquips));
		m_dwHideEquipMaskBackUp = m_dwHideEquipMask;
		m_dwHideEquipMask = 0;
		memcpy(aEquips,m_EquipsInBath[GetGender()],sizeof(m_EquipsInBath[GetGender()]));
		ShowEquipments(aEquips,true, true);
		m_bHasChangedEquips = true;
	}
	if (m_pCurSkill && m_pCurSkill->IsChargeable() && m_pCurSkill->IsChanting() && m_IncantCnt.IsFull())
	{
		m_pCurSkill->StartCharging(2000);
		PlayAttackEffect(m_idCurSkillTarget, m_pCurSkill->GetSkillID(), m_pCurSkill->GetSkillLevel(), m_pCurSkill->GetStamp(), -1, 0);
	}

	//	Save relative pose if player is on a carrier
	if (m_idCarrier)
		pNPCMan->CalcCarrierRelMat(m_idCarrier, GetAbsoluteTM(), m_matRelCarrier);

	//-------------------------------------------------------------------------
	if ( m_TransfigureInfo.bCollisionRaid )	//处于碰撞副本！！
	{
		CheckClearCollisionState();
	}
	//-------------------------------------------------------------------------
	
	if(m_pBackupModel && m_iTravalVehicleId!=0)
	{
		ChangeFreeFlyWing();
	}

	return true;
}

//	Internal render routine
#ifdef BACK_VERSION
bool CECElsePlayer::InternalRender(CECViewport* pViewport, int iRenderFlag, bool bHighLight)
#else
bool CECElsePlayer::InternalRender(ECViewport* pViewport, int iRenderFlag, bool bHighLight)
#endif
{
	m_PateContent.iVisible = 0;
	//  观战者，不用渲染
	if(m_iBattleCamp == GP_BATTLE_CAMP_OBSERVER)
		return true;
	//  屏蔽其他玩家，不用渲染
	if(g_pGame->GetGameRun()->IsHideElsePlayer())
		return true;

#ifdef BACK_VERSION
	A3DCameraBase* pCamera = pViewport->GetA3DCamera();
	m_fDistToCamera	= CalcDist(pCamera->GetPos(), true);
#endif

	if( iRenderFlag == RD_NORMAL )
	{
		if (m_fDistToCamera < 30.0f && GetBoothState() != 2)
			m_bCastShadow = true;
		else
			m_bCastShadow = false;
	}
	else
	{
		if (m_fDistToCamera > 30.0f )
			return true;
#ifdef BACK_VERSION
		A3DTerrainWater * pTerrainWater = g_pGame->GetGameRun()->GetWorld()->GetTerrainWater();
#else
		A3DTerrainWater2* pTerrainWater = g_pGame->GetGameRun()->GetWorld()->GetTerrainWater();
#endif
		if( iRenderFlag == RD_REFLECT && pTerrainWater->IsUnderWater(GetPlayerAABB().Maxs) )
			return true;
		else if( iRenderFlag == RD_REFRACT )
		{	
#ifdef BACK_VERSION
			bool bCamUnderWater = pTerrainWater->IsUnderWater(pCamera->GetPos());
#else
			bool bCamUnderWater = pTerrainWater->IsUnderWater(pViewport->GetA3DCameraBase()->GetPos());
#endif
			if( !bCamUnderWater && !pTerrainWater->IsUnderWater(GetPlayerAABB().Mins) )
				return true;
			else if( bCamUnderWater && pTerrainWater->IsUnderWater(GetPlayerAABB().Maxs) )
				return true;
		}

		m_bCastShadow = false;
	}

	if (!IsAllResReady() || InGhostState())
		return true;

	if( !IsClothesOn() )
		return true;

	if (!m_bVisible)
	{
		//	Only render player head text
		if (iRenderFlag == RD_NORMAL && m_bRenderName)
			RenderName(pViewport, g_pGame->GetConfigs()->GetPlayerTextFlags());

		return true;
	}

	if (m_AttachMode != enumAttachNone && m_bHangerOn)
	{
	}
	else if (m_bAttachedInteractor && !m_bInteractiveSubject)
	{
	}
	else
	{
		CECModel* pModel = m_pPlayerModel;
		if (m_pPetModel)
			pModel = m_pPetModel;
		else if (m_pInteractorModel && !m_bInteractiveRenderSubject && m_bAttachedInteractor)
			pModel = m_pInteractorModel;

		if (m_pTransfiguredModel)
			pModel = m_pTransfiguredModel;
		else if (m_pChangedModel)
			pModel = m_pChangedModel;
		else if(m_pPuppetModel)
			pModel = m_pPuppetModel;
		if (pModel)
		{
			if (bHighLight&& !m_bDim && !m_bInvisible)
				g_pGame->RenderHighLightModel(pViewport, pModel);
			else
			{
				if( iRenderFlag != RD_BLOOM )
				{
					pModel->Render(pViewport->GetA3DViewport());
				}
				else
				{
					RenderEquipBloom(pViewport);
					return true;
				}
			}
#ifdef BACK_VERSION
			if (m_bCastShadow && g_pGame->GetShadowRender())
			{
				CECPlayer * pPlayer = this;
				SHADOW_RECEIVER receiver = SHADOW_RECEIVER_TERRAIN; 
				if( !g_pGame->GetConfigs()->GetSystemSettings().bSimpleShadow )
					receiver = (SHADOW_RECEIVER)(SHADOW_RECEIVER_TERRAIN | SHADOW_RECEIVER_LITMODEL);
				//-------------------------------------------------------------
				//由于在人物坐骑过大时，传入ShadowMap的aabb过于小，使得shadow显示不正常，
				//所以在传入aabb时进行相应判断 Added 2012-03-30.
				A3DAABB aabb = m_aabb;
				if ( pModel->GetA3DSkinModel() )
				{
					bool bUpdate = false;
					A3DAABB aabbTmp = pModel->GetA3DSkinModel()->GetModelAABB();
					if ( aabb.Extents.x < aabbTmp.Extents.x )
					{
						aabb.Extents.x = aabbTmp.Extents.x;
						bUpdate = true;
					}
					if ( aabb.Extents.y < aabbTmp.Extents.y )
					{
						aabb.Extents.y = aabbTmp.Extents.y;
						bUpdate = true;
					}
					if ( aabb.Extents.z < aabbTmp.Extents.z )
					{
						aabb.Extents.z = aabbTmp.Extents.z;
					}
					if ( bUpdate )
					{
						aabb.CompleteMinsMaxs();
					}
				}
				//-------------------------------------------------------------
				g_pGame->GetShadowRender()->AddShadower(aabb.Center, aabb, receiver, PlayerRenderForShadow, pPlayer, g_pGame->GetConfigs()->GetSystemSettings().bSimpleShadow);//m_aabb.Center, m_aabb
			}
#endif
		}
	}

	if (iRenderFlag == RD_NORMAL)
	{
		//	Render player head text
		if (m_bRenderBar)
			RenderBars(pViewport);

		if (m_bRenderName)
			RenderName(pViewport, g_pGame->GetConfigs()->GetPlayerTextFlags());

		//	Print player's precise position
		if (g_pGame->GetConfigs()->GetShowPosFlag())
		{
			ACHAR szMsg[100];
			A3DVECTOR3 vPos = GetPos();
			a_sprintf(szMsg, _AL("%.3f, %.3f, %.3f"), vPos.x, vPos.y, vPos.z);
			int y = pViewport->GetA3DViewport()->GetParam()->Height - 20;
			vPos += g_vAxisY * (m_aabb.Extents.y * 3.0f);
			g_pGame->GetA3DEngine()->GetSystemFont()->TextOut3D(szMsg, pViewport->GetA3DViewport(), vPos, 0, 0xffffffff);
		}
	}

	if (iRenderFlag != RD_BLOOM && !m_pTransfiguredModel)
	{
		if (m_pSprite)
		{
			SECT_LEVEL2_INFO* pInfo = GetSectInfoByProf(m_BasicProps.iProfession);
			// 怀光变身情况下不显示法宝
			if (IsShapeChanged() && pInfo && (pInfo->mask_sect & (1<<SECT_13)) )
			{

			}
			else 
				m_pSprite->Render(pViewport);
		}
	}

	// render mirrors & spirits
	if (iRenderFlag != RD_BLOOM)
	{
		int i;
		if(g_pGame->GetConfigs()->GetVideoSettings().nEffect != 1)
		{
			for(i=0;i<m_iMirrorImageCnt;i++)
			{
				CECLoginPlayer* pMirrorPlayer = m_MirrorModels[i];
				if(pMirrorPlayer && pMirrorPlayer->GetVisible())
				{
					CECModel* ppp = pMirrorPlayer->GetPlayerModel();

					float fHeightDiff = fabs(pMirrorPlayer->GetPos().y - GetPos().y);

					if(ppp && fHeightDiff < 2.0f)
						ppp->Render(pViewport->GetA3DViewport());
				}
			}
		}

		for(i=0;i<3;i++)
		{
			CECLoginPlayer* pSpiritPlayer = m_SpiritModels[i];
			if(pSpiritPlayer && pSpiritPlayer->GetVisible())
			{
				CECModel* ppp = pSpiritPlayer->GetPlayerModel();
				if(ppp)
					ppp->Render(pViewport->GetA3DViewport());
			}
		}
	}

	return true;
}

//	Render routine when player use simple models
bool CECElsePlayer::RenderSimpleModels(bool bBooth, ECViewport* pViewport, 
									int iRenderFlag, bool bHighLight)
{
	m_PateContent.iVisible = 0;
#ifdef BACK_VERSION
	A3DCameraBase* pCamera = pViewport->GetA3DCamera();
#else
	A3DCameraBase* pCamera = pViewport->GetA3DCameraBase();
#endif
	m_fDistToCamera	= CalcDist(pCamera->GetPos(), true);

	if (iRenderFlag == RD_REFLECT || iRenderFlag == RD_REFRACT)
		return true;
	
	if (InGhostState())
		return true;
	
	if (g_pGame->GetGameRun()->IsHideElsePlayer())
		return true;

	if (!m_bVisible)
	{
		//	Only render player head text
		if (iRenderFlag == RD_NORMAL && m_bRenderName)
			RenderName(pViewport, g_pGame->GetConfigs()->GetPlayerTextFlags());

		return true;
	}

	A3DSkinModel* pSimpleModel = NULL;
	if (bBooth)
		if(m_idBoothItem == -1)
			pSimpleModel = m_pBoothModel;
		else
			if(m_pSpeBoothModel)
				pSimpleModel = m_pSpeBoothModel->GetA3DSkinModel();

	if (pSimpleModel)
	{
		//	Set light for model
		A3DSkinModel::LIGHTINFO LightInfo;
		memset(&LightInfo, 0, sizeof (LightInfo));
		const A3DLIGHTPARAM& lp = g_pGame->GetDirLight()->GetLightparam();

		LightInfo.colAmbient	= g_pGame->GetA3DDevice()->GetAmbientValue();
		LightInfo.vLightDir		= lp.Direction;
		LightInfo.colDirDiff	= lp.Diffuse;
		LightInfo.colDirSpec	= lp.Specular;
		LightInfo.bPtLight		= false;

		pSimpleModel->SetLightInfo(LightInfo);

		pSimpleModel->SetPos(GetPos());
		pSimpleModel->SetDirAndUp(GetDir(), GetUp());
		pSimpleModel->Update(g_pGame->GetTickTime());

		if (bHighLight && !m_bDim && !m_bInvisible)
			g_pGame->RenderHighLightModel(pViewport, pSimpleModel);
		else
#ifdef BACK_VERSION
			pSimpleModel->RenderAtOnce(pViewport->GetA3DViewport(), 0);
#else
			pSimpleModel->Render(pViewport->GetA3DViewport(), false);
#endif
	}

	//	Render player head text
	if (m_bRenderBar)
		RenderBars(pViewport);

	if (m_bRenderName)
		RenderName(pViewport, g_pGame->GetConfigs()->GetPlayerTextFlags());

	//	Print player's precise position
	if (g_pGame->GetConfigs()->GetShowPosFlag())
	{
		ACHAR szMsg[100];
		A3DVECTOR3 vPos = GetPos();
		a_sprintf(szMsg, _AL("%.3f, %.3f, %.3f"), vPos.x, vPos.y, vPos.z);
		int y = pViewport->GetA3DViewport()->GetParam()->Height - 20;
		vPos += g_vAxisY * (m_aabb.Extents.y * 3.0f);
		g_pGame->GetA3DEngine()->GetSystemFont()->TextOut3D(szMsg, pViewport->GetA3DViewport(), vPos, 0, 0xffffffff);
	}

	return true;
}

//	Process message
bool CECElsePlayer::ProcessMessage(const ECMSG& Msg)
{
	switch (Msg.dwMsg)
	{
	case MSG_PM_PLAYERFLY:			OnMsgPlayerFly(Msg);			break;
	case MSG_PM_PLAYERBASEINFO:		OnMsgPlayerBaseInfo(Msg);		break;
	case MSG_PM_PLAYERBASEINFO2:	OnMsgPlayerBaseInfo2(Msg);		break;
	case MSG_PM_PLAYERCUSTOM:		OnMsgPlayerCustomData(Msg);		break;
	case MSG_PM_PLAYEREQUIPDATA:	OnMsgPlayerEquipData(Msg);		break;
	case MSG_PM_PLAYERSITDOWN:		OnMsgPlayerSitDown(Msg);		break;
	case MSG_PM_CHANGENAMECOLOR:	OnMsgChangeNameColor(Msg);		break;
	case MSG_PM_PLAYERATKRESULT:	OnMsgPlayerAtkResult(Msg);		break;
	case MSG_PM_STARTATTACK:		OnMsgPlayerStartAtk(Msg);		break;
	case MSG_PM_CASTSKILL:			OnMsgPlayerCastSkill(Msg);		break;
	case MSG_PM_PLAYEREXTSTATE:		OnMsgPlayerExtState(Msg);		break;
	case MSG_PM_PLAYERDOEMOTE:		OnMsgPlayerDoEmote(Msg);		break;
	case MSG_PM_PLAYERUSEITEM:		OnMsgPlayerUseItem(Msg);		break;
	case MSG_PM_USEITEMWITHDATA:	OnMsgPlayerUseItemWithData(Msg);break;
	case MSG_PM_ENCHANTRESULT:		OnMsgEnchantResult(Msg);		break;
	case MSG_PM_PLAYERROOT:			OnMsgPlayerRoot(Msg);			break;
	case MSG_PM_PLAYERDOACTION:		OnMsgPlayerDoAction(Msg);		break;
	case MSG_PM_PLAYERSKILLRESULT:	OnMsgPlayerSkillResult(Msg);	break;
	case MSG_PM_PLAYERADVDATA:		OnMsgPlayerAdvData(Msg);		break;
	case MSG_PM_PLAYERINTEAM:		OnMsgPlayerInTeam(Msg);			break;
	case MSG_PM_PICKUPMATTER:		OnMsgPickupMatter(Msg);			break;
	case MSG_PM_PLAYERGATHER:		OnMsgPlayerGather(Msg);			break;
	case MSG_PM_DOCONEMOTE:			OnMsgDoConEmote(Msg);			break;
	case MSG_PM_PLAYERCHGSHAPE:		OnMsgPlayerChangeShape(Msg);	break;
	case MSG_PM_BOOTHOPT:			OnMsgBoothOperation(Msg);		break;
	case MSG_PM_PLAYERTRAVEL:		OnMsgPlayerTravel(Msg);			break;
	case MSG_PM_PLAYERPVP:			OnMsgPlayerPVP(Msg);			break;
	case MSG_PM_FASHIONENABLE:		OnMsgSwitchFashionMode(Msg);	break;
	case MSG_PM_PLAYEREFFECT:		OnMsgPlayerEffect(Msg);			break;
	case MSG_PM_CHANGEFACE:			OnMsgPlayerChangeFace(Msg);		break;
	case MSG_PM_PLAYERBINDOPT:		OnMsgPlayerBindOpt(Msg);		break;
	case MSG_PM_PLAYERMOUNT:		OnMsgPlayerMount(Msg);			break;
	case MSG_PM_PLAYERDUELOPT:		OnMsgPlayerDuelOpt(Msg);		break;
	case MSG_PM_PLAYERLEVEL2:		OnMsgPlayerLevel2(Msg);			break;
	case MSG_PM_MULTIBINDOPT:		OnMsgPlayerMultiBindOpt(Msg);	break;
	case MSG_PM_CARRIER:			OnMsgPlayerCarrierOpt(Msg);		break;	
	case MSG_PM_PLAYERSCALESHAPE:	OnMsgPlayerScaleShape(Msg);		break; //Added 2011-06-28.
	case MSG_PM_PLAYERVIPAWARDUPDATE: OnMsgPlayerVIPAwardUpdate(Msg); break; //Added 2012-01-17.
	case MSG_CSFLOW_BATTLE_PLAYER_INFO: OnMsgHstFlowBattlePlayerInfo(Msg);break;
	case MSG_PM_BATH_OPT:             OnMsgPlayerBathOpt(Msg);      break;
	case MSG_PM_START_INTERACT:		OnMsgPlayerStartInteract(Msg);	break;
	case MSG_PM_IN_INTERACT:		OnMsgPlayerInInteract(Msg);		break;
	case MSG_PM_END_INTERACT:		OnMsgPlayerEndInteract(Msg);	break;
	case MSG_PM_FINISH_INTERACT:	OnMsgPlayerFinishInteract(Msg);	break;
	case MSG_PM_CANCEL_INTERACT:	OnMsgPlayerCancelInteract(Msg);	break;
	case MSG_PM_QILIN_BINDOPT:		OnMsgPlayerQilinBindOpt(Msg);	break;
	case MSG_PM_PLAY_ACTIVE_EMOTE:	OnMsgPlayerPlayActiveEmote(Msg);		break;
	case MSG_PM_STOP_ACTIVE_EMOTE:	OnMsgPlayerStopActiveEmote(Msg);		break;
	case MSG_PM_TRAVEL_FREE:		OnMsgTravelFree(Msg);           break;
	}

	return true;
}

//	Set server position
void CECElsePlayer::SetServerPos(const A3DVECTOR3& vPos)
{
	m_pMovingCorrect->SetPos(vPos);
	if (_isnan(vPos.x) || _isnan(vPos.y) || _isnan(vPos.z))
		return;
	
	m_vServerPos = vPos;

	//	If this player is a mule, change it's rider's server pos too.
	if (m_iBuddyId && !m_bHangerOn)
	{
		CECElsePlayer* pPlayer = m_pPlayerMan->GetElsePlayer(m_iBuddyId);
		if (pPlayer)
			pPlayer->SetServerPos(vPos);
	}

	// If player is interactive subject.
	if (m_bAttachedInteractor && m_bInteractiveSubject)
	{
		CECInteractor* pInteractor = (CECInteractor*) g_pGame->GetGameRun()->GetWorld()->GetNPCMan()->GetNPC(m_InteractorInfo.id);
		ASSERT(pInteractor);
		if (pInteractor)
			pInteractor->SetServerPos(vPos);
	}
}

//	Move to a destination
void CECElsePlayer::MoveTo(const S2C::cmd_object_move& Cmd)
{
	if (!Cmd.use_time)
		return;

	SetServerPos(Cmd.dest);

	m_vMoveDir	= Cmd.dest - GetPos();
	float fDist	= m_vMoveDir.Normalize();

	//	If destination position is too far to us, forcely pull player
	//	to that position.
	if (fDist >= MAX_LAGDIST)
	{
		SetPos(Cmd.dest);
		return;
	}
	
	m_bStopMove	= false;

	int iMoveMode = Cmd.move_mode;
	m_pcdr->bTraceGround = true;

	if (iMoveMode & GP_MOVE_AIR)
	{
		m_iMoveEnv = MOVEENV_AIR;
		m_pcdr->bTraceGround = false;
	}
	else if (iMoveMode & GP_MOVE_WATER)
	{
		m_iMoveEnv = MOVEENV_WATER;
		m_pcdr->bTraceGround = false;
		ShowWing(false);
	}
	else
	{
		m_iMoveEnv = MOVEENV_GROUND;
		
		if (!IsFlying())
			ShowWing(false);
	}

	switch (iMoveMode & GP_MOVE_MASK)
	{
	case GP_MOVE_WALK:	m_iMoveMode = MOVE_MOVE;	m_bWalkRun	= false;	break;
	case GP_MOVE_RUN:	m_iMoveMode = MOVE_MOVE;	m_bWalkRun	= true;		break;
	case GP_MOVE_SLIDE:	m_iMoveMode = MOVE_SLIDE;	break;
	case GP_MOVE_PUSH:	break;
	case GP_MOVE_FALL:	m_iMoveMode = MOVE_FREEFALL;	m_pcdr->bTraceGround = false;	break;
	case GP_MOVE_FLYFALL:	m_pcdr->bTraceGround = false; break;
	case GP_MOVE_JUMP:	m_iMoveMode	= MOVE_JUMP;	m_pcdr->bTraceGround = false;	break;
	case GP_MOVE_QINGGONG:	m_iMoveMode	= MOVE_QINGGONG;	m_pcdr->bTraceGround = false;	break;
	case GP_MOVE_PULLED:	m_iMoveMode	= MOVE_STAND;	m_bStopMove = true;	m_vStopDir = m_vMoveDir;	break; // Added 2011-07-29.
	default:
		ASSERT(0);
		return;
	}

	DWORD dwTimeNow = a_GetTime();
	DWORD dwDeltaTime = dwTimeNow - m_dwLastMoveTime;
	m_dwLastMoveTime = dwTimeNow;
	if( dwDeltaTime < 500 )
		dwDeltaTime = 500;
	if( dwDeltaTime > 1000 )
		dwDeltaTime = 1000;
	
	//	Calcualte speed
	float fSpeed = FIX8TOFLOAT(Cmd.sSpeed);
	m_fMoveSpeed = fDist / (dwDeltaTime * 0.001f);
	a_Clamp(m_fMoveSpeed, 0.0f, fSpeed * 1.2f);

//	test code ...
//	float fTempSpeed = m_fMoveSpeed;
//	if (m_fLastSpeed)	//	Consider speed of last time so that moving appears smoothly
//		m_fMoveSpeed = m_fLastSpeed * 0.4f + m_fMoveSpeed * 0.6f;
//	m_fLastSpeed = fTempSpeed;

	//	test code ...
//	ACHAR szMsg[100];
//	a_sprintf(szMsg, _AL("%f, %f, %d"), fDist, m_fMoveSpeed, Cmd.use_time);
//	a_sprintf(szMsg, _AL("%f, %f, %f"), m_vMoveDir.x, m_vMoveDir.y, m_vMoveDir.z);
//	a_sprintf(szMsg, _AL("%f, %f, %f"), Cmd.dest.x, Cmd.dest.y, Cmd.dest.z);
//	g_pGame->GetRTDebug()->OutputDebugInfo(0xffff0000, szMsg);

	A3DVECTOR3 vDir = m_vMoveDir;
	vDir.y = 0.0f;
	if (!vDir.IsZero())
	{
		vDir.Normalize();
		SetDestDirAndUp(vDir, g_vAxisY, 150);
	}

	if (m_iCurWorkType < WT_NORMAL || m_aWorks[WT_NORMAL] != WORK_MOVE)
		StartWork(WT_NORMAL, WORK_MOVE);

	//	Play action
	if (m_pPlayerModel)
	{
		int iCurAction = (int)m_pPlayerModel->GetCurActionUserData(ACT_CHANNEL_BODY);
		if (IsValidAction(iCurAction))
		{
			if (iCurAction != ACT_TRICK_JUMP && iCurAction != ACT_TRICK_RUN)
			{
				if (m_iMoveMode	== MOVE_JUMP || m_iMoveMode == MOVE_SLIDE)
					PlayAction(ACT_JUMP_LOOP, 1.0f, false);
				else if (m_iMoveMode == MOVE_QINGGONG )
					PlayAction(ACT_QINGGONG_LOOP, 1.0f, false);
				else if ( m_iMoveMode == MOVE_STAND )	//直接站立 Added 2011-07-29.
					PlayAction(ACT_STAND, 1.0f, false);
				else
					PlayAction(GetMoveStandAction(true), 1.0f, false);
			}
		}
		else
			PlayAction(GetMoveStandAction(true), 1.0f, false);
	}
}

//	Stop moving to a destination position
void CECElsePlayer::StopMoveTo(const S2C::cmd_object_stop_move& Cmd)
{
	m_vMoveDir		= Cmd.dest - GetPos();
	m_bStopMove		= true;
	m_fMoveSpeed	= FIX8TOFLOAT(Cmd.sSpeed);
	m_vStopDir		= glb_DecompressDirH(Cmd.dir);

	SetServerPos(Cmd.dest);

	//	test code...Consider speed of last time so that moving appears smoothly
//	if (m_fLastSpeed)
//		m_fMoveSpeed = m_fLastSpeed * 0.4f + m_fMoveSpeed * 0.6f;

	float fDist = m_vMoveDir.Normalize();
	//	If destination position is too far to us, forcely pull player
	//	to that position.
	if (fDist >= MAX_LAGDIST)
	{
		SetPos(Cmd.dest);
		SetDestDirAndUp(m_vStopDir, g_vAxisY, 150);
		WorkFinished(WORK_MOVE);
		return;
	}

	int iMoveMode = Cmd.move_mode;
	m_pcdr->bTraceGround = true;

	if (iMoveMode & GP_MOVE_AIR)
	{
		m_iMoveEnv = MOVEENV_AIR;
		m_pcdr->bTraceGround = false;
	}
	else if (iMoveMode & GP_MOVE_WATER)
	{
		m_iMoveEnv = MOVEENV_WATER;
		m_pcdr->bTraceGround = false;
	}
	else
		m_iMoveEnv = MOVEENV_GROUND;

	switch (iMoveMode & GP_MOVE_MASK)
	{
	case GP_MOVE_WALK:	m_iMoveMode = MOVE_MOVE;	m_bWalkRun	= false;	break;
	case GP_MOVE_RUN:	m_iMoveMode = MOVE_MOVE;	m_bWalkRun	= true;		break;
	case GP_MOVE_PUSH:	break;
	case GP_MOVE_SLIDE:	m_iMoveMode = MOVE_SLIDE;	break;
	case GP_MOVE_FALL:	m_iMoveMode = MOVE_FREEFALL;	m_pcdr->bTraceGround = false;	break;
	case GP_MOVE_FLYFALL:	m_pcdr->bTraceGround = false;	break;
	case GP_MOVE_JUMP:	m_iMoveMode	= MOVE_JUMP;	m_pcdr->bTraceGround = false;	break;
	case GP_MOVE_QINGGONG:	m_iMoveMode	= MOVE_QINGGONG;	m_pcdr->bTraceGround = false;	break;
	case GP_MOVE_PULLED:	break;
	case GP_MOVE_RETURN:	break;
	default:
		ASSERT(0);
		return;
	}

	A3DVECTOR3 vDir = m_vMoveDir;
	vDir.y = 0.0f;
	if (!vDir.IsZero())
	{
		vDir.Normalize();
		SetDestDirAndUp(vDir, g_vAxisY, 150);
	}

	if (m_iCurWorkType < WT_NORMAL || m_aWorks[WT_NORMAL] != WORK_MOVE)
		StartWork(WT_NORMAL, WORK_MOVE);

	//	Play action
	if (m_pPlayerModel)
	{
		int iCurAction = (int)m_pPlayerModel->GetCurActionUserData(ACT_CHANNEL_BODY);
		if (IsValidAction(iCurAction))
		{
			if (iCurAction != ACT_TRICK_JUMP && iCurAction != ACT_TRICK_RUN)
			{
				if (m_iMoveMode	== MOVE_JUMP || m_iMoveMode == MOVE_SLIDE)
					PlayAction(ACT_JUMP_LOOP, 1.0f, false);
				else if (m_iMoveMode == MOVE_QINGGONG )
					PlayAction(ACT_QINGGONG_LOOP, 1.0f, false);
				else
					PlayAction(GetMoveStandAction(true), 1.0f, false);
			}
		}
		else
			PlayAction(GetMoveStandAction(true), 1.0f, false);
	}
}

//	Walk on ground to destination position
bool CECElsePlayer::MovingTo(DWORD dwDeltaTime)
{
	bool bRet = false;

	A3DVECTOR3 vPos, vSetDir, vCurPos = GetPos();
	float fDeltaTime = dwDeltaTime * 0.001f;

	if (m_bStopMove)
	{
		A3DVECTOR3 vDir = m_vServerPos - vCurPos;
		float fDist = vDir.Normalize();
		vPos = MoveStep(vDir, m_fMoveSpeed, fDeltaTime);

		float fMoveDelta = a3d_Magnitude(vPos - vCurPos);
		if (!fMoveDelta || fMoveDelta >= fDist)
		{
			SetPos(m_vServerPos);
			if (GetCarrierID())
				SetDirAndUp(m_vStopDir, g_vAxisY);
			bRet = true;
		}
		else
		{
			SetPos(vPos);
			if (GetCarrierID())
			{
				vSetDir = vDir;
				vSetDir.y = 0.0f;
				if (vSetDir.x != 0.0f)
					SetDirAndUp(Normalize(vSetDir), g_vAxisY);
			}		
		}
	}
	else	//	Just move on
	{
		//	If we have move so far from destination and still don't 
		//	receive new 'move' or 'stop move' command, it's better to
		//	stop moving and goto last destination at once
		float fDist = a3d_Magnitude(m_vServerPos - vCurPos);
		if (fDist >= MAX_LAGDIST)
		{
			SetPos(m_vServerPos);
			m_vStopDir = GetDir();
			return true;
		}
	
		A3DVECTOR3 vDir = m_vMoveDir;
		vDir.Normalize();
		vPos = MoveStep(vDir, m_fMoveSpeed, fDeltaTime);
		SetPos(vPos);
		if (GetCarrierID())
		{
			vSetDir = vDir;
			vSetDir.y = 0.0f;// 在固定的台子上，vSetDir有可能被置位0向量，导致玩家消失
			if (vSetDir.x != 0.0f)
				SetDirAndUp(Normalize(vSetDir), g_vAxisY);
		}	
	}

	return bRet;
}

//	Move step
A3DVECTOR3 CECElsePlayer::MoveStep(const A3DVECTOR3& vDir, float fSpeed, float fTime)
{
	A3DVECTOR3 vRealDir = vDir;

	//	OnAirMove only accept positive speed value
	if (fSpeed < 0.0f)
	{
		vRealDir = -vDir;
		fSpeed = -fSpeed;
	}

	m_pcdr->vCenter			= m_aabb.Center;
	m_pcdr->vVelocity		= vRealDir * fSpeed;
	m_pcdr->t				= fTime;
	m_pcdr->bTestTrnOnly	= false;

	OtherPlayerMove(*m_pcdr);

	if( m_pcdr->bTraceGround)
		SetGroundNormal(m_pcdr->vecGroundNormal);
	else
		SetGroundNormal(g_vAxisY);

	return m_pcdr->vCenter - g_vAxisY * m_pcdr->vExts.y;
}

//	Ground Move step with collosion detection.
A3DVECTOR3 CECElsePlayer::GroundMoveStep( const A3DVECTOR3& vDir, float fSpeed, float fTime, float fSpeedV, float fGravity )
{
	A3DVECTOR3 vRealDirH = vDir;

	//	OnGroundMove only accept positive speed value
	if ( fSpeed < 0.0f )
	{
		vRealDirH = -vDir;
		fSpeed = -fSpeed;
	}

	if ( fabs(vRealDirH.y) > 0.0001 )
	{
		vRealDirH.y = 0.0f;
		vRealDirH.Normalize();
	}

	CDR_INFO& cdr = *m_pCDRInfo;

	cdr.vCenter			= m_aabb.Center;
	cdr.vXOZVelDir		= vRealDirH;
	cdr.fSpeed			= fSpeed;
	cdr.t				= fTime;
	cdr.fGravityAccel	= fGravity;
#ifdef CDR_HL2_MOVE
	cdr.vClipVel.y     += fSpeedV;
#else
	cdr.fYVel		   += fSpeedV;
#endif
	OnGroundMove(cdr);

	if (cdr.bOnSurface && !cdr.vTPNormal.IsZero())
	{
		SetGroundNormal(cdr.vTPNormal);
	}
	else
	{
		SetGroundNormal(g_vAxisY);
	}
	
	A3DVECTOR3 vNewPos = cdr.vCenter - g_vAxisY * m_aabb.Extents.y;

	return vNewPos;
}

void CECElsePlayer::GetElsePos()
{
	static int lastTime = GetTickCount();
	int curTime = GetTickCount();
	if (curTime-lastTime>5000)
	{
		c2s_SendCmdGetCollisionPlayerPos(GetCharacterID());
		lastTime = curTime;
	}
}

//	Player was killed
void CECElsePlayer::Killed(int idKiller)
{
	if( m_pPlayerModel )
		m_pPlayerModel->ClearComActFlag(true);

	m_dwStates |= GP_STATE_CORPSE;
	ShowWing(false);
	StartWork(WT_NORMAL, WORK_DEAD);

	SetUseGroundNormal(true);
}

//	Revive
void CECElsePlayer::Revive(const S2C::cmd_player_revive* pCmd)
{
	//	Revive type
	enum
	{
		REVIVE_RETURN = 0,
		REVIVE_HERE_START,
		REVIVE_HERE_END,
	};

	m_dwStates &= ~GP_STATE_CORPSE;

	if (pCmd->sReviveType != REVIVE_HERE_END)
	{
		m_bAboutToDie = false;
		SetUseGroundNormal(false);

		//	Finish dead work
		WorkFinished(WORK_DEAD);

		if (pCmd->sReviveType == REVIVE_HERE_START)
		{
			StartWork(WT_NORMAL, WORK_IDLE);
			SetIdleWorkInfo(IDLE_REVIVE, 0, 0);
		}
	}
	else
	{
		IdleWorkFinished(IDLE_REVIVE);
	}
}

//	Level up
void CECElsePlayer::LevelUp()
{
//	if (m_pLevelUpGFX)
//		m_pLevelUpGFX->Start(true);
	if (m_pTransfiguredModel)
		m_pTransfiguredModel->PlayGfx(res_GFXFile(RES_GFX_LEVELUP), NULL);
	else if (m_pChangedModel)
		m_pChangedModel->PlayGfx(res_GFXFile(RES_GFX_LEVELUP), NULL);
	else if (m_pPlayerModel)
		m_pPlayerModel->PlayGfx(res_GFXFile(RES_GFX_LEVELUP), NULL);
}

void CECElsePlayer::DeityLevelUp(bool bSuccess, int iLevel)
{
	if (bSuccess)
	{
		if (m_pTransfiguredModel)
			m_pTransfiguredModel->PlayGfx(res_GFXFile(RES_GFX_DEITY_LEVELUP), NULL);
		else if (m_pChangedModel)
			m_pChangedModel->PlayGfx(res_GFXFile(RES_GFX_DEITY_LEVELUP), NULL);
		else if (m_pPlayerModel)
			m_pPlayerModel->PlayGfx(res_GFXFile(RES_GFX_DEITY_LEVELUP), NULL);
	}
}

//	Enter fight state
void CECElsePlayer::EnterFightState()
{
	m_FightCnt.Reset(false);
}

bool CECElsePlayer::IsFighting()
{ 
	return m_bFight || !m_FightCnt.IsFull();
}

//	Get off pet
void CECElsePlayer::GetOffPet()
{
	CECPlayer::GetOffPet();

	//	Restore cdr info
	m_pcdr->vExts = m_aabb.Extents;
}

//	Release work resources of specified type
void CECElsePlayer::ReleaseWork(int iWorkType)
{
	ASSERT(iWorkType >= 0 && iWorkType < NUM_WORKTYPE);

	switch (m_aWorks[iWorkType])
	{
	case WORK_STAND:		break;
	case WORK_MOVE:			break;
	case WORK_HACKOBJECT:	break;
	case WORK_SPELL:		break;
	case WORK_IDLE:			break;
	case WORK_FLASHMOVE:	break;
	}

	m_aWorks[iWorkType] = 0;

	if (m_iCurWorkType == iWorkType)
		m_iCurWork = 0;
}

//	Start a work of specified type
void CECElsePlayer::StartWork(int iWorkType, int iNewWork, DWORD dwParam/* 0 */)
{
	ASSERT(iWorkType >= 0 && iWorkType < NUM_WORKTYPE);

	if (iNewWork == WORK_DEAD)
	{
		//	Dead is a special work
		ReleaseWork(WT_INTERRUPT);
		ReleaseWork(WT_NORMAL);

		m_aWorks[WT_NORMAL]	= iNewWork;
		m_iCurWorkType = iWorkType;
	}
	else if (iWorkType == WT_INTERRUPT)
	{
		//	Release old work
		ReleaseWork(WT_INTERRUPT);
		m_aWorks[WT_INTERRUPT] = iNewWork;

		if (m_iCurWorkType == WT_NORMAL || m_iCurWorkType == WT_NOTHING)
			StopWork(m_iCurWorkType);

		m_aWorks[WT_INTERRUPT] = iNewWork;
		m_iCurWorkType = WT_INTERRUPT;
	}
	else if (iWorkType == WT_NORMAL)
	{
		//	Release old work
		ReleaseWork(WT_NORMAL);
		m_aWorks[WT_NORMAL] = iNewWork;

		if (m_iCurWorkType < 0 || m_iCurWorkType == WT_NORMAL || m_iCurWorkType == WT_NOTHING)
		{
			if (m_iCurWorkType == WT_NOTHING)
				StopWork(WT_NOTHING);

			m_iCurWorkType = WT_NORMAL;
		}
		else
			return;
	}
	else	//	iWorkType == WT_NOTHING
	{
		//	Release old work
		ReleaseWork(WT_NOTHING);
		m_aWorks[WT_NOTHING] = iNewWork;

		if (m_iCurWorkType < 0 || m_iCurWorkType == WT_NOTHING)
			m_iCurWorkType = WT_NOTHING;
		else
			return;
	}

	StartWorkByID(iNewWork, dwParam);
}

//	Start work
void CECElsePlayer::StartWorkByID(int iWorkID, DWORD dwParam)
{
	switch (iWorkID)
	{
	case WORK_STAND:		StartWork_Stand(dwParam);		break;
	case WORK_MOVE:			StartWork_Move(dwParam);		break;
	case WORK_HACKOBJECT:	StartWork_Melee(dwParam);		break;
	case WORK_SPELL:		StartWork_Spell(dwParam);		break;
	case WORK_PICKUP:		StartWork_Pickup(dwParam);		break;
	case WORK_GENERAL_OPRT:	StartWork_GeneralOprt(dwParam);	break;
	case WORK_DEAD:			StartWork_Dead(dwParam);		break;
	case WORK_USEITEM:		StartWork_UseItem(dwParam);		break;
	case WORK_IDLE:			break;
	case WORK_FLASHMOVE:	StartWork_FlashMove(dwParam);	break;
	case WORK_SPECIALMOVE:	StartWork_SpecialMove(dwParam);	break;
	case WORK_BLEWUP:		StartWork_BlewUp(dwParam);		break;
	case WORK_PARABOLIC:	StartWork_Parabolic(dwParam);	break;
	case WORK_MOVENOCOLLISION: StartWork_MoveNoCollision(dwParam); break;
	default:
		ASSERT(0);
		return;
	}

	m_iCurWork = iWorkID;
}

//	Stop / pause work of specified type
void CECElsePlayer::StopWork(int iWorkType)
{
}

//	Current work finished
void CECElsePlayer::WorkFinished(int iWorkID)
{
	//	Note: below judge can prevent much problems when we attempt to
	//		finish a work but don't assure we are doing this work
	if (m_iCurWork != iWorkID)
		return;

	ASSERT(m_iCurWork > 0 && m_iCurWorkType >= 0);

	//-------------------------------------------------------------------------
	//	Reset the special moving flag to false;
	if ( iWorkID == WORK_SPECIALMOVE )
	{
		m_bSpecialMoving = false;
		m_vVelocity.Clear();
		SetCollisionState(false);
//		SetServerCollisionState(false);
#ifdef CDR_HL2_MOVE
			m_pCDRInfo->vClipVel.x = 0;
			m_pCDRInfo->vClipVel.z = 0;
#endif

		GetElsePos();
	}
	//-------------------------------------------------------------------------

	//	Release current work
	ReleaseWork(m_iCurWorkType);

	for (int i=m_iCurWorkType-1; i >= 0; i--)
	{
		if (m_aWorks[i])
		{
			m_iCurWorkType = i;
			StartWorkByID(m_aWorks[i], 0);
			break;
		}
	}
}

//	Finish idle work
void CECElsePlayer::IdleWorkFinished(int iType)
{
	if (m_iCurWork != WORK_IDLE || m_IdleWork.iType != iType)
		return;
	
	WorkFinished(WORK_IDLE);
}

void CECElsePlayer::StartWork_Stand(DWORD dwParam)
{
	m_iMoveMode = MOVE_STAND;
	PlayAction(GetMoveStandAction(false, IsFighting()), 1.0f);
}

void CECElsePlayer::StartWork_Move(DWORD dwParam)
{
//	m_iMoveMode	= MOVE_MOVE;
	PlayAction(GetMoveStandAction(true), 1.0f);
}

void CECElsePlayer::StartWork_Melee(DWORD dwParam)
{
	m_iMoveMode	= MOVE_STAND;
	PlayAction(ACT_ATTACK_1 + a_Random(0, 3), 1.0f);
	EnterFightState();
}

void CECElsePlayer::StartWork_Spell(DWORD dwParam)
{
	m_iMoveMode	= MOVE_STAND;
	//傀儡姿态放技能进入战斗状态
	if(m_pPuppetModel && m_pCurSkill && m_pCurSkill->GetSkillID() != 4783 && m_pCurSkill->GetSkillID() != 4784 &&
		m_pCurSkill->GetSkillID() != 4785 && m_pCurSkill->GetSkillID() != 4786 && m_pCurSkill->GetSkillID() != 4787)
		EnterFightState();
//	PlayAction();
}

void CECElsePlayer::StartWork_Dead(DWORD dwParam)
{
	m_iMoveMode = MOVE_STAND;
	
	A3DVECTOR3 vCurPos = GetPos();
	A3DVECTOR3 vGndPos, vNormal, vTestPos = vCurPos + g_vAxisY;
	VertRayTrace(vTestPos, vGndPos, vNormal, 10.0f);
	float fWaterHei = g_pGame->GetGameRun()->GetWorld()->GetWaterHeight(vTestPos);

	int iFlag = 0;
//	if (vCurPos.y < fWaterHei - m_MoveConst.fShoreDepth || 
//		(vCurPos.y < fWaterHei && fWaterHei - vGndPos.y > m_MoveConst.fShoreDepth))
	/*
	if (CheckWaterMoveEnv(vCurPos, fWaterHei, vGndPos.y))
		iFlag = 1;
	else if (vCurPos.y > vGndPos.y + 0.5f)
		iFlag = 2;
		*/

	if (dwParam)
	{
		if (iFlag == 1)
			PlayAction(ACT_WATERDIE_LOOP, 1.0f);
		else if (iFlag == 2)
			PlayAction(ACT_AIRDIE, 1.0f);
		else
			PlayAction(ACT_GROUNDDIE_LOOP, 1.0f);
	}
	else
	{
		if (iFlag == 1)
		{
			PlayAction(ACT_WATERDIE, 1.0f);
			PlayAction(ACT_WATERDIE_LOOP, 1.0f, true, 200, true);
		}
		else if (iFlag == 2)
		{
			PlayAction(ACT_AIRDIE_ST, 1.0f);
			PlayAction(ACT_AIRDIE, 1.0f, true, 200, true);
		}
		else
		{
			PlayAction(ACT_GROUNDDIE, 1.0f);
			PlayAction(ACT_GROUNDDIE_LOOP, 1.0f, true, 200, true);
		}
	}
}

void CECElsePlayer::StartWork_Pickup(DWORD dwParam)
{
	m_iMoveMode = MOVE_STAND;
	m_PickupCnt.Reset();

	if (!dwParam)
	{
		PlayAction(ACT_PICKUP_MATTER, 1.0f);
		PlayAction(ACT_STAND, 1.0f, true, 200, true);
	}
	else
	{
		PlayAction(ACT_PICKUP, 1.0f);
		PlayAction(ACT_PICKUP_LOOP, 1.0f, false, 200, true);
	}
}

void CECElsePlayer::StartWork_GeneralOprt(DWORD dwParam)
{
	m_iMoveMode = MOVE_STAND;

	if( GetAttachMode() == enumAttachNone && !m_pPetModel )
		PlayAction(ACT_GENERAL_OPRT, 1.0f);
}

void CECElsePlayer::StartWork_UseItem(DWORD dwParam)
{
	m_iMoveMode = MOVE_STAND;
	PlayStartUseItemAction(dwParam);
}

void CECElsePlayer::StartWork_FlashMove(DWORD dwParam)
{
	m_iMoveMode = MOVE_MOVE;
	
	//Canceled 2011-09-05.
//	PlayAction(ACT_JUMP_START, 1.0f, true, 0);
//	PlayAction(ACT_JUMP_LOOP, 1.0f, true, 0, true);
	//Canceled end.
	if(m_bQilinFlashMove)
		PlayAction(CECPlayer::ACT_QINGGONG_LOOP, 1.0f, true, 0, false);
	else
		PlayAction(CECPlayer::ACT_FIGHT_CHARGE, 1.0f, true, 0, false); // Added 2011-09-05.
//	float fCurTerrainHeight = g_pGame->GetGameRun()->GetWorld()->GetTerrainHeight(GetPos());
//	m_fYDelta = GetPos().y - fCurTerrainHeight + 0.1f;
}

void CECElsePlayer::StartWork_SpecialMove(DWORD dwParam)
{
	PlayAction(ACT_RUN, 1.0f, true, 200, true);
}

void CECElsePlayer::StartWork_BlewUp(DWORD dwParam)
{
	PlayAction(ACT_WOUNDED, 1.0);
}

void CECElsePlayer::StartWork_Parabolic(DWORD dwParam)
{
	if(m_bQilinFlashMove)
		PlayAction(CECPlayer::ACT_QINGGONG_LOOP, 1.0f, true, 0, false);
}

void CECElsePlayer::StartWork_MoveNoCollision(DWORD dwParam)
{
	if(m_bQilinFlashMove)
		PlayAction(CECPlayer::ACT_QINGGONG_LOOP, 1.0f, true, 0, false);
}

void CECElsePlayer::SetIdleWorkInfo(int iType, int iOTTime, DWORD dwParam)
{
	m_IdleWork.iType	= iType;
	m_IdleWork.bOTCheck	= iOTTime > 0 ? true : false;

	if (m_IdleWork.bOTCheck)
	{
		m_IdleWork.OTCnt.SetPeriod(iOTTime);
		m_IdleWork.OTCnt.Reset();
	}

	switch (iType)
	{
	case IDLE_SITDOWN:

		m_iMoveMode = MOVE_STAND;

		if (dwParam)
			PlayAction(ACT_SITDOWN_LOOP, 1.0f);
		else
		{
			PlayAction(ACT_SITDOWN, 1.0f);
			PlayAction(ACT_SITDOWN_LOOP, 1.0f, true, 200, true);
		}

		break;

	case IDLE_REVIVE:
		
		PlayAction(ACT_REVIVE, 1.0f);
		break;

	case IDLE_BOOTH:
		
		m_iMoveMode = MOVE_STAND;
		break;
	}
}

void CECElsePlayer::TickWork_Stand(DWORD dwDeltaTime)
{
	if (m_pPlayerModel)
	{
		bool bFight = IsFighting();
		m_FightCnt.IncCounter(dwDeltaTime);

		int iCurAction = (int)m_pPlayerModel->GetCurActionUserData(ACT_CHANNEL_BODY);
		if (iCurAction == ACT_JUMP_LOOP)
			PlayAction(GetMoveStandAction(false, IsFighting()), 1.0f, false);
		else
		{
			//	Change state from fighting to normal standing
			if (bFight != IsFighting())
				PlayAction(GetMoveStandAction(false, IsFighting()), 1.0f, false);
		}
	}
}

void CECElsePlayer::TickWork_Move(DWORD dwDeltaTime)
{
	//	Move player
	if (MovingTo(dwDeltaTime))
	{
		m_fLastSpeed = 0.0f;
		SetDestDirAndUp(m_vStopDir, g_vAxisY, 150);
		WorkFinished(WORK_MOVE);
	}
}

void CECElsePlayer::TickWork_Melee(DWORD dwDeltaTime)
{
/*	A3DVECTOR3 vTarget;

	if (ISNPCID(m_iAttackTarget))
	{
		CECNPC* pNPC = g_pGame->GetGameRun()->GetWorld()->GetNPCMan()->GetNPC(m_iAttackTarget);
		if (!pNPC || pNPC->IsDead())
		{
			//	Taget has missed ?
			WorkFinished(WORK_HACKOBJECT);
			return;
		}

		vTarget = pNPC->GetPos();
	}
	else if (ISPLAYERID(m_iAttackTarget))
	{
		CECPlayer* pPlayer = m_pPlayerMan->GetPlayer(m_iAttackTarget);
		if (!pPlayer)
		{
			//	Taget has missed ?
			WorkFinished(WORK_HACKOBJECT);
			return;
		}

		vTarget = pPlayer->GetPos();
	}
	else
	{
		ASSERT(0);
		return;
	}

	//	Check whether player still face to it's attack target.
	A3DVECTOR3 vDir = vTarget - GetPos();
	vDir.y = 0.0f;
	if (!vDir.Normalize())
		vDir = GetDir();

	SetDestDirAndUp(vDir, g_vAxisY, 100);
*/
	TurnFaceTo(m_iAttackTarget);

	//	Slide to server position
	A3DVECTOR3 vDir = m_vServerPos - GetPos();
	float fDist = vDir.Normalize();
	if (fDist > 0.0001f)
	{
		float fMoveDist = 10.0f * dwDeltaTime * 0.001f;
		if (fMoveDist > fDist)
			fMoveDist = fDist;

		SetPos(GetPos() + vDir * fMoveDist);
	}

	if (m_FightCnt.IncCounter(dwDeltaTime))
	{
		//	Finish fight state
		WorkFinished(WORK_HACKOBJECT);
	}
}

void CECElsePlayer::TickWork_Spell(DWORD dwDeltaTime)
{
	m_FightCnt.IncCounter(dwDeltaTime);

	int iRealTime = g_pGame->GetRealTickTime();
	if (m_SkillCnt.IncCounter(iRealTime))
	{
		if (IsInDomainState())
		{
			// 如果是领域技能，把技能时间延长
			m_SkillCnt.SetPeriod(300000);
			m_SkillCnt.Reset();
			return;
		}
		WorkFinished(WORK_SPELL);

		if (m_pCurSkill)
		{
			//	Release current skill
			delete m_pCurSkill;
			m_pCurSkill = NULL;

		}
		m_idCurSkillTarget = 0;
	}
	else
	{
		if (m_idCurSkillTarget && m_idCurSkillTarget != m_PlayerInfo.cid)
			TurnFaceTo(m_idCurSkillTarget);
	}
}

void CECElsePlayer::TickWork_Pickup(DWORD dwDeltaTime)
{
	DWORD dwRealTime = g_pGame->GetRealTickTime();
	if (m_PickupCnt.IncCounter(dwRealTime))
	{
		WorkFinished(WORK_PICKUP);
	}
}

void CECElsePlayer::TickWork_Dead(DWORD dwDeltaTime)
{
}

void CECElsePlayer::TickWork_Idle(DWORD dwDeltaTime)
{
	bool bFinish = false;

	DWORD dwRealTime = g_pGame->GetRealTickTime();
	if (m_IdleWork.bOTCheck && m_IdleWork.OTCnt.IncCounter(dwRealTime))
		bFinish = true;

	switch (m_IdleWork.iType)
	{
	case IDLE_SITDOWN:

		break;

	case IDLE_REVIVE:
		
		break;

	case IDLE_BOOTH:
		
		break;
	}

	if (bFinish)
	{
		WorkFinished(WORK_IDLE);
	}
}

void CECElsePlayer::TickWork_FlashMove(DWORD dwDeltaTime)
{
	A3DVECTOR3 vCurPos = GetPos();
	A3DVECTOR3 vDir = m_vServerPos - vCurPos;
	vDir.y = 0;	//Added 2011-07-22.
	float fDist = vDir.Normalize();
/*	float fMoveDelta = m_fMoveSpeed * dwDeltaTime * 0.001f;

	if (fMoveDelta >= fDist)
	{
		SetPos(m_vServerPos);
		WorkFinished(WORK_FLASHMOVE);
	}
	else
	{
		vCurPos = vCurPos + vDir * fMoveDelta;
		float fTerrHeight = g_pGame->GetGameRun()->GetWorld()->GetTerrainHeight(vCurPos);
		vCurPos.y = m_fYDelta + fTerrHeight;
		SetPos(vCurPos);
		SetDirAndUp(vDir, g_vAxisY);
	}
*/  //Canceled 2011-09-05.

	//-------------------------------------------------------------------------
	//Added 2011-09-05.
	float fDeltaTime = dwDeltaTime * 0.001f;
	vCurPos = MoveStep(vDir, m_fMoveSpeed, fDeltaTime);
	float fMoveDelta = a3d_MagnitudeH(vCurPos - GetPos());
	if ( !fMoveDelta || fMoveDelta >= fDist )
	{
		SetPos(m_vServerPos);
		WorkFinished(WORK_FLASHMOVE);
		if(m_bQilinFlashMove)
		{
			ReAttachQilin();
			m_bQilinFlashMove = false;
		}
	}
	else
	{
		if(m_bQilinFlashMove)
			vCurPos.y += 0.15f;
		SetPos(vCurPos);
		SetDirAndUp(vDir, g_vAxisY);
	}
	//-------------------------------------------------------------------------

}

void CECElsePlayer::TickWork_SpecialMove(DWORD dwDeltaTime)
{
	float fDeltaTime = dwDeltaTime * 0.001f;

	//Deal with the stop special move command.
	if ( m_bStopMove )
	{
		A3DVECTOR3 vCurPos = GetPos();
		A3DVECTOR3 vDir = m_vServerPos - vCurPos;
		float fDist = vDir.Normalize();
		A3DVECTOR3 vPos = GroundMoveStep(vDir, m_fMoveSpeed, fDeltaTime);//MoveStep
		float fMoveDelta = a3d_Magnitude(vPos - vCurPos);
		if ( !fMoveDelta || fMoveDelta >= fDist || fDist < 0.002f )	// if the fDist( the distance between the server pos and current pos ) is too little, then set the pos directly.
		{
			SetPos(m_vServerPos);
			SetDestDirAndUp(m_vStopDir, g_vAxisY, 150);
			WorkFinished(WORK_SPECIALMOVE);
			m_vVelocity.Clear();
		}
		else
		{
			SetPos(vPos);
		}
		//
		return;
	}

	//-------------------------------------------------------------------------
	if ( IsCollisionRooting() )	//如果处于不可移动状态，则停止移动
	{
		WorkFinished(WORK_SPECIALMOVE);
		m_vVelocity.Clear();
		return;
	}
	//========
	m_bSpecialMoving	=	true;	//Set the special moving flag to true.
/*	static unsigned int uFrameCount = 0;
	static unsigned int uCollideFrameCount = 0;
	uFrameCount++;*/
	//-------------------------------------------------------------------------
	//Store the current velocity as previous velocity.
	A3DVECTOR3 vPrevVelocity = m_vVelocity;
	//vt = v0 + at.
	m_vVelocity += m_vAcceleration * fDeltaTime;

	//-------------------------------------------------------------------------
	//如果当前速度大于设定阈值，则设置到最大速度
	if ( m_vVelocity.Magnitude() > (float)COLLISION_MAX_SPEED )
	{
		m_vVelocity.Normalize();
		m_vVelocity *= (float)COLLISION_MAX_SPEED;
	}
	//-------------------------------------------------------------------------
	float fCurrentSpeed = m_vVelocity.Magnitude();
	// if the current velocity is in the opposite direction, then it means invalide.
	if ( DotProduct( vPrevVelocity, m_vVelocity ) < 0.0f || fCurrentSpeed <= 0.01f )	//0.01f
	{
		A3DVECTOR3 vDir = vPrevVelocity;
		float iMag = vDir.Normalize();
		A3DVECTOR3 vCurPos = GroundMoveStep(vDir, fCurrentSpeed/2, fCurrentSpeed/m_vAcceleration.Normalize());
		SetPos(vCurPos);
		if(iMag>0.5f)
			SetDirAndUp(vDir, g_vAxisY);
		WorkFinished(WORK_SPECIALMOVE);
		m_vVelocity.Clear();
		return;
	}

	A3DVECTOR3 vDir = m_vVelocity;
	vDir.Normalize();
//	A3DVECTOR3 vCurPos = GroundMoveStep(vDir, fCurrentSpeed, fDeltaTime);	//MoveStep	//////////Canceled
	//-------------------------------------------------------------------------
	bool bCollide = g_pGame->GetGameRun()->GetWorld()->GetPlayerMan()->UpdateCollisionStates( this, fDeltaTime );
	if ( bCollide )	//|| GetCollisionState()
	{
		//---------------------------------------------------------------------
/*		if ( uFrameCount == (uCollideFrameCount+1) )
		{
			WorkFinished(WORK_SPECIALMOVE);
			uCollideFrameCount = uFrameCount;
			a_LogOutput( 1, "----------ElsePlayerID=%d, 连续两次碰撞了！uCollideFrameCount = %d, uFrameCount = %d", GetCharacterID(), uCollideFrameCount, uFrameCount );
			return;
		}
		else
		{
			uCollideFrameCount = uFrameCount;
		}*/
		//---------------------------------------------------------------------
#ifdef CDR_HL2_MOVE
			A3DVECTOR3 vNewDir = GetVelocity();
//			vNewDir.Normalize();
			m_pCDRInfo->vClipVel.x = vNewDir.x;
			m_pCDRInfo->vClipVel.z = vNewDir.z;
#endif
		return;
	}

	//如果没有发生碰撞，则移动相应的距离
	A3DVECTOR3 vCurPos = GroundMoveStep(vDir, fCurrentSpeed, fDeltaTime);
	//-------------------------------------------------------------------------
	float fMoveDelta = a3d_MagnitudeH(vCurPos - GetPos());
	if ( fMoveDelta < 0.0001f )// speed is so slow? or be blocked??
	{
		WorkFinished(WORK_SPECIALMOVE);
		m_vVelocity.Clear();
		return;
	}
	else
	{
		SetPos(vCurPos);
		SetDirAndUp(vDir, g_vAxisY);
	}
	//-------------------------------------------------------------------------
}

void CECElsePlayer::SetCDRInfoClipVelocity(const A3DVECTOR3& vClipVelocity)
{
#ifdef CDR_HL2_MOVE
	if ( m_pCDRInfo )
	{
		m_pCDRInfo->vClipVel.x		= vClipVelocity.x;
		m_pCDRInfo->vClipVel.z		= vClipVelocity.z;
	}
#endif
}

void CECElsePlayer::TickWork_BlewUp(DWORD dwDeltaTime)
{
	A3DVECTOR3 vecPos = GetPos();
	
	m_dwPastTime += 50;
	if(m_dwPastTime > m_dwDuration)
	{
		WorkFinished(WORK_BLEWUP);
		vecPos.y = m_fHeight;
		SetPos(vecPos);
		return;
	}
	else
	{
		if(m_dwPastTime < m_dwDuration / 2)   //up
		{
			m_fSpeed = m_fSpeed - m_fAccelerate * 0.05f;
			if(m_fSpeed > 0)
			{
				vecPos.y += m_fSpeed * 0.05f;
				SetPos(vecPos);
			}
		}
		else						//down
		{
			m_fSpeed = m_fSpeed + m_fAccelerate * 0.05f;
			if(m_fSpeed > 0)
			{
				vecPos.y -= m_fSpeed * 0.05f;
				if(vecPos.y > m_fHeight)
					SetPos(vecPos);
				else
				{
					WorkFinished(WORK_BLEWUP);
					vecPos.y = m_fHeight;
					SetPos(vecPos);
					return;
				}
			}
		}
	}
}

void CECElsePlayer::TickWork_Parabolic(DWORD dwDeltaTime)
{
	A3DVECTOR3 vecPos = GetPos();
	DWORD dwPastTime = GetTickCount() - m_dwStartTime;
	float fDeltaTime = dwDeltaTime * 0.001f;
	float dist = a3d_MagnitudeH(m_vecDest - vecPos);
	if(dwPastTime > m_dwDuration || dist < 1)
	{
		WorkFinished(WORK_PARABOLIC);
		SetPos(m_vecDest);
		return;
	}
	else
	{
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

		A3DVECTOR3 vCurPos = GetPos();
		A3DVECTOR3 vMoveDir = m_vecDest - vCurPos;
		vMoveDir.y = 0.0f;
		vMoveDir.Normalize();
		if (!vMoveDir.IsZero())
		{
			SetDirAndUp(vMoveDir, g_vAxisY);
		}

		A3DVECTOR3 vVel = vMoveDir * m_fSpeed + g_vAxisY * m_fSpeedV * dirV;

		float fSpeed = vVel.Normalize();
		vCurPos = MoveStep(vVel, fSpeed, fDeltaTime);
		SetPos(vCurPos);
	}
}

void CECElsePlayer::TickWork_MoveNoCollision(DWORD dwDeltaTime)
{	
	if(m_bQilinFlashMove)
	{
		CECPlayer * pPlayer = g_pGame->GetGameRun()->GetWorld()->GetPlayerMan()->GetPlayer(m_idMoveToTarget);
		if(pPlayer)
		{
			m_vecDest = pPlayer->GetPos() + g_vAxisY * 5.0f;
		}
	}

	float fDeltaTime = dwDeltaTime * 0.001f;
	A3DVECTOR3 vCurPos = GetPos();
	A3DVECTOR3 vMoveDir = m_vecDest - vCurPos;
	A3DVECTOR3 vMoveDirH = A3DVECTOR3(vMoveDir.x, 0.0f, vMoveDir.z);
	vMoveDirH.y = 0.0f;


	float fDist = vMoveDir.Normalize();

	vCurPos = vCurPos + vMoveDir * m_fSpeed * fDeltaTime;
	A3DVECTOR3 vMoveDelta = vCurPos - GetPos();
	float fMovDelta = vMoveDelta.Normalize();
	if (fMovDelta >= fDist)
	{
		WorkFinished(WORK_MOVENOCOLLISION);
		if(m_bQilinFlashMove)
		{
			ReAttachQilin();
			m_bQilinFlashMove = false;
		}
		return;
	}
	else
	{
		SetPos(vCurPos);
		if(!vMoveDir.IsZero())
			SetDirAndUp(vMoveDirH, g_vAxisY);
	}
}

//	Set player's brief info
bool CECElsePlayer::SetPlayerBriefInfo(int iFaceid, int iHairid, int iEarid, int iTailid, 
									   int iDefaultSkin, int iGender, const ACHAR* szName)
{
//	m_iRace				= iRace;
	m_iFaceid			= iFaceid;
	m_iHairid			= iHairid;
	m_iEarid			= iEarid;
	m_iTailid			= iTailid;
	m_iDefaultSkin		= iDefaultSkin;
	m_iGender			= iGender;
	m_bBaseInfoReady	= true;

	//	Get player name and save into name cache. Because name got this time
	//	must be right, so we force to overwrite old name
	m_strName = szName;
	if (ISCLONEPLAYERID(m_PlayerInfo.cid))
		g_pGame->GetGameRun()->AddPlayerName(m_idSummoner, szName, true);
	else
		g_pGame->GetGameRun()->AddPlayerName(m_PlayerInfo.cid, szName, true);

	if (m_pPateName)
		m_pPateName->SetText(szName, false);

	//	Calculate player's AABB
	CalcPlayerAABB();

	//	Update upper body radius using new AABB
	m_pcdr->fStepHeight	= m_MoveConst.fStepHei;
	m_pcdr->vExts		= m_aabb.Extents;
	return true;
}

//	Change equipment
bool CECElsePlayer::ChangeEquipments(bool bReset, int crc, int iAddMask, int iDelMask, 
									 int* aAddedEquip)
{
	m_PlayerInfo.crc_e = crc;

	if (bReset)
	{
		m_dwNewEqpMask	= EQUIP_MASK_ALL;
		m_bEquipReady	= true;

		memset(m_aEquips, 0xff, sizeof (m_aEquips));
		memset(m_aNewEquips, 0, sizeof (m_aNewEquips));
	}

	//	Remove equipment at first
	if (iDelMask)
	{
		for (int i=0; i < SIZE_EQUIPIVTR; i++)
		{
			if (iDelMask & (1 << i))
			{
				m_aNewEquips[i] = 0;
				m_dwNewEqpMask |= (1 << i);
			}
		}
	}

	if (iAddMask && aAddedEquip)
	{
		int iCount = 0;
		for (int i=0; i < SIZE_EQUIPIVTR; i++)
		{
			if (iAddMask & (1 << i))
			{
				if( i >= EQUIPIVTR_FASHION_EYE && i <= EQUIPIVTR_FASHION_BACK 
					|| i == EQUIPIVTR_FASHION_WEAPON)
				{
					unsigned short sTemp =  ( aAddedEquip[iCount] & 0xffff);
					unsigned short sTemp2 =  ( aAddedEquip[iCount++] & 0xffff0000) >> 16;
					m_aNewEquips[i] = sTemp;
					int nItemIdex = sTemp2;
					if(nItemIdex)
					{
						CECIvtrIndividualityItem* pItem = 	(CECIvtrIndividualityItem*)CECIvtrItem::CreateItem(nItemIdex, 0, 1);
						if(pItem)
						{
							if ( pItem->GetClassID() == CECIvtrItem::ICID_INDIVIDUALITY_ITEM )
							{
								WORD nColorId = pItem->GetDBEssence()->index;
								m_aNewEquips[i] |= (nColorId << 16) & 0x7fffffff;
							}
							delete pItem;
						}
					}
				}
				else
				{
					m_aNewEquips[i] = aAddedEquip[iCount++];
				}
				m_dwNewEqpMask |= (1 << i);
			}
		}
	}

	if (IsSkeletonReady())
	{
		//  竞技场中不能换装
		if(m_iBattleType != BATTLE_TYPE_ARENA && m_iBattleType != BATTLE_TYPE_ARENAREBORN && m_iBattleType != BATTLE_TYPE_CRSSVR && m_iBathType == -1)
			LoadPlayerEquipments();
	}
	return true;
}

//	Load player equipments
bool CECElsePlayer::LoadPlayerEquipments()
{
#ifdef _DEBUG
	
	for (int i=0; i < SIZE_EQUIPIVTR; i++)
	{
		if (m_aNewEquips[i] < 0)
		{
			ASSERT(m_aNewEquips[i] >= 0);
		}
	}

#endif	//	_DEBUG

	ShowEquipments(m_aNewEquips);
	SetResReadyFlag(RESFG_SKIN, true);

/*	if (m_pPlayerModel)
	{
		//	If weapon changed, action index may be changed, so update here
		int iCurAction = (int)m_pPlayerModel->GetCurActionUserData(ACT_CHANNEL_BODY);
		if (IsValidAction(iCurAction))
			PlayAction(iCurAction, 1.0f, false);
	}
*/
	return true;
}

//	Set loaded model to player object, this function is used in multithread loading process
bool CECElsePlayer::SetPlayerLoadedResult(EC_PLAYERLOADRESULT& Ret)
{
	if (!CECPlayer::SetPlayerLoadedResult(const_cast<EC_PLAYERLOADRESULT&>(Ret)))
		return false;

	if(IsSkillFlying())
		ShowWing(false);
	else if(IsFlying())
		ShowWing(true);

	StartWork(WT_NOTHING, WORK_STAND);

	if (IsDead())
	{
		ShowWing(false);
		StartWork(WT_NORMAL, WORK_DEAD, 1);
	}
	else if (IsSitting())
	{
		StartWork(WT_NORMAL, WORK_IDLE);
		SetIdleWorkInfo(IDLE_SITDOWN, 0, 1);
	}
	else if (m_iBoothState == 2)
	{
		StartWork(WT_NORMAL, WORK_IDLE);
		SetIdleWorkInfo(IDLE_BOOTH, 0, 0);
	}

	return true;
}

//	Is spelling magic
bool CECElsePlayer::IsSpellingMagic()
{
	return m_aWorks[m_iCurWorkType] == WORK_SPELL;
}

void CECElsePlayer::OnMsgPlayerFly(const ECMSG& Msg)
{
	using namespace S2C;

	if(Msg.dwParam3 == 1)	// 玩家释放技能导致的飞行效果
		m_bSkillFly = true;
	else
		m_bSkillFly = false;

	if (IsAllResReady())
	{
		if (Msg.dwParam2 == OBJECT_LANDING)
			ShowWing(false);
		else	//	OBJECT_TAKEOFF
		{
			if(m_bSkillFly)	// 技能导致的飞行效果，不显示飞剑
				ShowWing(false);
			else 
				ShowWing(true);
		}
	}

	if (Msg.dwParam2 == OBJECT_LANDING)
		m_dwStates &= ~GP_STATE_FLY;
	else
		m_dwStates |= GP_STATE_FLY;
}

void CECElsePlayer::OnMsgPlayerBaseInfo(const ECMSG& Msg)
{
	using namespace GNET;
	
	PlayerBaseInfo_Re* p = (PlayerBaseInfo_Re*)Msg.dwParam1;
	const GRoleBase& base = p->player;

	ASSERT((int)base.id == m_PlayerInfo.cid);
	
	if (!base.name.size())
		return;

	ACString strName = ACString((const ACHAR*)base.name.begin(), base.name.size() / sizeof (ACHAR));
	SetPlayerBriefInfo(base.faceid, base.hairid, base.earid, base.tailid, base.fashionid, base.gender, strName);
}


void CECElsePlayer::OnMsgPlayerBaseInfo2(const ECMSG& Msg)
{
	using namespace GNET;
	
	PlayerBaseInfo2_Re* p = (PlayerBaseInfo2_Re*)Msg.dwParam1;
	const GRoleBase& base = p->player;

	ASSERT(p->cloneid == m_PlayerInfo.cid);
	
	if (!base.name.size())
		return;

	ACString strName = ACString((const ACHAR*)base.name.begin(), base.name.size() / sizeof (ACHAR));
	SetPlayerBriefInfo(base.faceid, base.hairid, base.earid, base.tailid, base.fashionid, base.gender, strName);
}


void CECElsePlayer::OnMsgPlayerCustomData(const ECMSG& Msg)
{
}

void CECElsePlayer::OnMsgPlayerEquipData(const ECMSG& Msg)
{
	using namespace S2C;

	int crc, iAddMask, iDelMask, *aAdded=NULL;
	bool bReset;

	if (Msg.dwParam2 == EQUIP_DATA)
	{
		cmd_equip_data* pCmd = (cmd_equip_data*)Msg.dwParam1;
		ASSERT(pCmd);

		bReset		= true;
		crc			= pCmd->crc;
		iAddMask	= pCmd->mask;
		iDelMask	= 0;
		aAdded		= pCmd->data;
	}
	else if( Msg.dwParam2 == EQUIP_DATA_CHANGED)
	{
		cmd_equip_data_changed* pCmd = (cmd_equip_data_changed*)Msg.dwParam1;
		ASSERT(pCmd);

		bReset		= false;
		crc			= pCmd->crc;
		iAddMask	= pCmd->mask_add;
		iDelMask	= pCmd->mask_del;
		aAdded		= iAddMask ? pCmd->data_add : NULL;
	}
	else if(Msg.dwParam2 == CLONE_PLAYER_EQUIP)
	{
		clone_player_equip cmd;
		if (!cmd.FillData((const void*)Msg.dwParam1, Msg.dwParam3))
		{
			ASSERT(0);
			return;
		}
		// save the pos and reset the pos
		A3DVECTOR3 vPos = GetPos();
		SetPlayerInfo(cmd.info);
		SetPos(vPos);
		SetServerPos(vPos);
		// 清除掉可能从主人复制过来的隐身信息
		m_bInvisible = false;

		bReset	= true;
		crc		= cmd.crc;
		iAddMask= cmd.mask;
		iDelMask= 0;
		aAdded	= cmd.data;
	}

	//	Change equipment
	ChangeEquipments(bReset, crc, iAddMask, iDelMask, aAdded);
}

void CECElsePlayer::OnMsgPlayerSitDown(const ECMSG& Msg)
{
	using namespace S2C;

	if (Msg.dwParam2 == OBJECT_SIT_DOWN)
	{
		m_dwStates |= GP_STATE_SITDOWN;
		cmd_object_sit_down* pCmd = (cmd_object_sit_down*)Msg.dwParam1;

		StartWork(WT_NORMAL, WORK_IDLE);
		SetIdleWorkInfo(IDLE_SITDOWN, 0, 0);
	}
	else	//	Msg.dwParams2 == OBJECT_STAND_UP
	{
		m_dwStates &= ~GP_STATE_SITDOWN;
		cmd_object_stand_up* pCmd = (cmd_object_stand_up*)Msg.dwParam1;
		if (m_aWorks[WT_NORMAL] == WORK_IDLE && m_IdleWork.iType == IDLE_SITDOWN)
		{
			PlayAction(ACT_STANDUP, 1.0f);
			IdleWorkFinished(IDLE_SITDOWN);
		}
	}
}

void CECElsePlayer::OnMsgPlayerAtkResult(const ECMSG& Msg)
{
	using namespace S2C;
	
	cmd_object_atk_result* pCmd = (cmd_object_atk_result*)Msg.dwParam1;
	ASSERT(pCmd && pCmd->attacker_id == m_PlayerInfo.cid);
	// int nDamage = (pCmd->target_id == g_pGame->GetGameRun()->GetHostPlayer()->GetCharacterID() ? pCmd->damage : -1);
	int nDamage = pCmd->damage;

	if (pCmd->attack_flag & MOD_RETORT)
	{
		AfxSkillGfxShowDamage(GetPlayerInfo().cid, pCmd->target_id, nDamage, 1, pCmd->attack_flag);
		AfxSkillGfxShowCaster(GetPlayerInfo().cid, pCmd->attack_flag);
	}
	else if (m_pPlayerModel)
		m_pPlayerModel->AddOneAttackDamageData(ACT_CHANNEL_BODY, GetPlayerInfo().cid, pCmd->target_id, pCmd->attack_stamp, pCmd->attack_flag, nDamage);
	
	// PK Alert when ElsePlayer Attacks HostPlayer, Set Flag and Reset a TimeCounter (automatically)
	// (The Flag will be cleared by the TimeCounter in the HostPlayer when Enough time elapsed that No new attack comes form an ElsePlayer)
	CECHostPlayer * pHost = m_pPlayerMan->GetHostPlayer();
	// Other Player Attack Host Player
	if (pCmd->target_id == pHost->GetCharacterID())
	{
		pHost->SetUnderAtk();
	}
}

void CECElsePlayer::OnMsgPlayerStartAtk(const ECMSG& Msg)
{
	using namespace S2C;

	cmd_object_start_attack* pCmd = (cmd_object_start_attack*)Msg.dwParam1;
	ASSERT(pCmd && pCmd->idAttacker == m_PlayerInfo.cid);

	m_iAttackTarget = pCmd->idTarget;

	//	Face to target
	TurnFaceTo(pCmd->idTarget);
	PlayAttackEffect(pCmd->idTarget, 0, 0, pCmd->attack_stamp, -1,	0);

	if (m_aWorks[WT_NORMAL] != WORK_HACKOBJECT)
		StartWork(WT_NORMAL, WORK_HACKOBJECT);

	//	Enter fight state
	EnterFightState();
}

void CECElsePlayer::OnMsgPlayerSkillResult(const ECMSG& Msg)
{
	using namespace S2C;

	cmd_object_skill_attack_result* pCmd = (cmd_object_skill_attack_result*)Msg.dwParam1;
	ASSERT(pCmd && pCmd->attacker_id == m_PlayerInfo.cid);
	// int nDamage = (pCmd->target_id == g_pGame->GetGameRun()->GetHostPlayer()->GetCharacterID() ? pCmd->damage : -1);
	int nDamage = pCmd->damage;

	if (pCmd->attack_flag & MOD_RETORT)
	{
		AfxSkillGfxShowDamage(GetPlayerInfo().cid, pCmd->target_id, nDamage, 1, pCmd->attack_flag | MOD_SKILL);
		AfxSkillGfxShowDamage(GetPlayerInfo().cid, pCmd->target_id, pCmd->dt_damage, 1, pCmd->attack_flag | MOD_DEITY_DAMAGE);
		AfxSkillGfxShowCaster(GetPlayerInfo().cid, pCmd->attack_flag | MOD_SKILL);
	}
	else if (m_pPlayerModel)
	{
		m_pPlayerModel->AddOneAttackDamageData(ACT_CHANNEL_BODY, GetPlayerInfo().cid, pCmd->target_id, pCmd->attack_stamp, pCmd->attack_flag | MOD_SKILL, nDamage);
		m_pPlayerModel->AddOneAttackDamageData(ACT_CHANNEL_BODY, GetPlayerInfo().cid, pCmd->target_id, pCmd->attack_stamp, pCmd->attack_flag | MOD_DEITY_DAMAGE, pCmd->dt_damage);
	}
// 	if (m_iSpecialState & SPECIAL_STATE_FROZEN)
// 		UpdateSpecialState(1, false);
// 	else
// 		UpdateSpecialState(1, true);
}

void CECElsePlayer::OnMsgPlayerCastSkill(const ECMSG& Msg)
{
	using namespace S2C;
	
	if (Msg.dwParam2 == OBJECT_CAST_SKILL)
	{
		cmd_object_cast_skill* pCmd = (cmd_object_cast_skill*)Msg.dwParam1;
		
		if(pCmd->state == 1)
		{
			return;
		}

		if (m_pCurSkill)
		{
			//	Release current skill
			delete m_pCurSkill;
		}

		if (!(m_pCurSkill = new CECSkill(pCmd->skill, pCmd->level)))
			return;

		// Added 2011-06-20. 吟唱阶段, 无论如何都设置技能发放速度值，专为太昊的麻痹和迅疾技能使用
		if ( pCmd->state == 0 )
		{
			if ( pCmd->cast_speed_rate >= 100 )
			{
				pCmd->cast_speed_rate = 90;
			}
			m_fSkillSpeedRate = 1.0f / ((100 - pCmd->cast_speed_rate)*0.01f);

			//Modified 2011-08-02.--------------------------------------
			//阵法技能使用
			const char* szCircleGfxName = m_pCurSkill->GetCircleGfxName();
			if ( szCircleGfxName && szCircleGfxName[0] != '\0' ) //如果当前技能中有阵法特效？是否应该要判断该技能为阵法技能
			{
				char szCODSponsorGfxName[512], szCODGfxName[512];
				sprintf( szCODSponsorGfxName, "%s%s%s.gfx", "程序联入\\", szCircleGfxName, "阵眼");
				sprintf( szCODGfxName, "%s%s%s.gfx", "程序联入\\", szCircleGfxName, "选区" );
				if(m_pPlayerModel)
				{
					m_pPlayerModel->PlayGfx(szCODSponsorGfxName, NULL);//"程序联入\\阵眼测试.gfx"
					m_pPlayerModel->PlayGfx(szCODGfxName, NULL);//"程序联入\\法阵选区测试.gfx"
				}
				//将该阵法的gfx名字添加到gfx名字列表中
				CECHostPlayer* pHost = m_pPlayerMan->GetHostPlayer();
				if ( pHost )
				{
					pHost->AddCODGfxName( pCmd->caster, szCircleGfxName );
				}
			}
			//Modified end.---------------------------------------------
		}
		//Added end.

		m_SkillCnt.SetPeriod((pCmd->time + m_pCurSkill->GetExecuteTime()) * 10);
		m_SkillCnt.Reset();

		if(pCmd->spirit_index >= 0)		// 由灵来释放技能
		{

		}
		else
		{
			StartWork(WT_NORMAL, WORK_SPELL);
		}

		m_idCurSkillTarget = pCmd->targets[0];
		m_pCurSkill->SetStamp(pCmd->attack_stamp);
		if (m_pCurSkill->IsChargeable())
		{
			int iTime = pCmd->time;
			a_ClampFloor(iTime, 10);
			m_IncantCnt.SetPeriod(iTime);
			m_IncantCnt.Reset();
			
			m_pCurSkill->StartChanting();		
		}

		// 判断是否需要灵模型来释放
		if(pCmd->spirit_index >= 0)	
		{
			CECLoginPlayer* pSpiritPlayer = CreateSpiritModel(pCmd->spirit_index);
			if(pSpiritPlayer)
			{
				pSpiritPlayer->SetPos(GetPos());
				pSpiritPlayer->SetVisible(true);
				pSpiritPlayer->SetCurSkill(m_pCurSkill);
				pSpiritPlayer->TurnFaceTo(pCmd->targets[0]);
				pSpiritPlayer->PlayAttackEffect(pCmd->targets[0], pCmd->skill, pCmd->level, pCmd->attack_stamp, -1, 0, NULL);
			}
		}
		else
		{
			TurnFaceTo(pCmd->targets[0]);
			PlayAttackEffect(pCmd->targets[0], pCmd->skill, pCmd->level, pCmd->attack_stamp, -1, 0, NULL, -1, &(pCmd->cast_skill_pos));
		}
		
		// add chain gfx
		if(m_pCurSkill->GetRangeType() == GNET::TYPE_CHAIN)	// 链式攻击技能
		{
			CECHostPlayer* pHost = m_pPlayerMan->GetHostPlayer();
			if(pHost)
			{
				pHost->RemoveChainGfxInfo(pCmd->caster);

				abase::vector<int> vecChainTargets;

				for(int i=0;i<pCmd->target_cnt;i++)
					vecChainTargets.push_back(pCmd->targets[i]);

				pHost->AddChainGfxInfo(pCmd->caster, vecChainTargets);
			}
		}

		// 多条线形攻击技能
		if(m_pCurSkill->GetRangeType() == GNET::TYPE_MULTILINE)
		{
			//-----------------------------------------------------------------
			//更新多线性攻击技能特效 Added 2012-09-11.
			UpdateMultilineSkillGfx( pCmd->targets, pCmd->target_cnt );
			//-----------------------------------------------------------------
		}
	}
	else if (Msg.dwParam2 == SKILL_INTERRUPTED)
	{
		if (m_pPlayerModel)
			m_pPlayerModel->ClearComActFlag(false);

		WorkFinished(WORK_SPELL);

		if (m_pCurSkill)
		{
			//	Release current skill
			delete m_pCurSkill;
			m_pCurSkill = NULL;
		}

		m_idCurSkillTarget = 0;
		StartWork_Stand(0);	
	}
	else if (Msg.dwParam2 == OBJECT_CAST_INSTANT_SKILL)
	{
		cmd_object_cast_instant_skill* pCmd = (cmd_object_cast_instant_skill*)Msg.dwParam1;

		if (m_pCurSkill)
		{
			//	Release current skill
			delete m_pCurSkill;
			m_pCurSkill = NULL;
			m_idCurSkillTarget = 0;
		}
		//	m_idCurSkillTarget = pCmd->target;
	}
	else if (Msg.dwParam2 == OBJECT_CAST_POS_SKILL)
	{
		cmd_object_cast_pos_skill* pCmd = (cmd_object_cast_pos_skill*)Msg.dwParam1;
		SetServerPos(pCmd->pos);

		float fDist = a3d_Magnitude(pCmd->pos - GetPos());
		m_fMoveSpeed = 1.3f * fDist * 1000.0f / GNET::ElementSkill::GetExecuteTime(pCmd->skill, pCmd->level);
		StartWork(WT_NORMAL, WORK_FLASHMOVE);
	}
	else if (Msg.dwParam2 == SKILL_CONTINUE)
	{
		skill_continue* pCmd = (skill_continue*)Msg.dwParam1;
		if(m_pCurSkill && m_pCurSkill->IsChargeable() && m_pCurSkill->GetSkillID() == pCmd->skill_id)
		{
			m_pCurSkill->EndCharging();
			PlayAttackEffect(m_idCurSkillTarget, pCmd->skill_id, m_pCurSkill->GetSkillLevel(), m_pCurSkill->GetStamp(), -1, 0);
		}
	}
	else if (Msg.dwParam2 == PLAYER_CHARGE)
	{
		player_charge* pCmd = (player_charge*)Msg.dwParam1;

		//在之前瞬移技能的基础上加上真正的冲锋技能效果（共用同一个协议）Modified 2011-07-22.
		if ( !pCmd->type )	// 之前的瞬移技能
		{
			SetPos(pCmd->target_pos);
			SetServerPos(pCmd->target_pos);

			CECObject* pObject = g_pGame->GetGameRun()->GetWorld()->GetObject(pCmd->target_id, 0);
			if (!pObject)
				return;
			SetDirAndUp(pObject->GetDir(), g_vAxisY);
		}
		else if ( 1 == pCmd->type || 3 == pCmd->type || 4 == pCmd->type || 5 == pCmd->type )	// 新增加的冲锋技能
		{
			SetServerPos(pCmd->target_pos);

			float fDist = a3d_Magnitude(pCmd->target_pos - GetPos());
			m_fMoveSpeed = 25.0f;
			StartWork(WT_NORMAL, WORK_FLASHMOVE);			
		}
		else if( 2 == pCmd->type )	// 闪现技能
		{
			SetPos(pCmd->target_pos);
			SetServerPos(pCmd->target_pos);
			if(m_pPlayerModel)
				m_pPlayerModel->PlayGfx("程序联入\\闪现.gfx", NULL);
		}
		else if( 6 == pCmd->type)
		{
			PrepareParabolic(1500, 5, pCmd->target_pos);
			m_pcdr->bTraceGround = false;
			StartWork(WT_NORMAL, WORK_PARABOLIC);
		}

	}
	else if (Msg.dwParam2 == PLAYER_STOP_SKILL) //Added 2011-06-17.Modified HOST_STOP_SKILL 2011-08-30.
	{
		m_fSkillSpeedRate = 1.0f;

		player_stop_skill* pCmd = (player_stop_skill*)Msg.dwParam1;
		if(!pCmd)
			return;
		
		if(pCmd->spirit_index >=0)
		{
			CECLoginPlayer* pSpiritPlayer = m_SpiritModels[pCmd->spirit_index];
			if(pSpiritPlayer)
			{
				pSpiritPlayer->SetVisible(false);
			}
		}

		CECHostPlayer* pHost = m_pPlayerMan->GetHostPlayer();
		if(pHost)
			pHost->RemoveChainGfxInfo(pCmd->player_id);
	
	}
}

void CECElsePlayer::OnMsgPlayerDoEmote(const ECMSG& Msg)
{
	using namespace S2C;

	if (Msg.dwParam2 == OBJECT_DO_EMOTE)
	{
		cmd_object_do_emote* pCmd = (cmd_object_do_emote*)Msg.dwParam1;

		DoEmote(pCmd->emotion);		

		if( m_iBuddyId )
		{
			CECPlayer* pBuddy = m_pPlayerMan->GetPlayer(m_iBuddyId);
			pBuddy->DoEmote(pCmd->emotion);
		}
	}
	else if (Msg.dwParam2 == OBJECT_EMOTE_RESTORE)
	{
		PlayAction(ACT_STAND, 1.0f, false);
	}
}

void CECElsePlayer::OnMsgPlayerUseItem(const ECMSG& Msg)
{
	using namespace S2C;

	switch (Msg.dwParam2)
	{
	case OBJECT_START_USE:
	{
		cmd_object_start_use * pCmd = (cmd_object_start_use*)Msg.dwParam1;
		StartWork(WT_NORMAL, WORK_USEITEM, pCmd->item);
		break;
	}

	case OBJECT_CANCEL_USE:	
	{
		cmd_object_cancel_use * pCmd = (cmd_object_cancel_use*)Msg.dwParam1;
		WorkFinished(WORK_USEITEM);
		break;
	}

	case OBJECT_USE_ITEM:
	{
		cmd_object_use_item* pCmd = (cmd_object_use_item*)Msg.dwParam1;
		WorkFinished(WORK_USEITEM);
		PlayUseItemEffect(pCmd->item);
		break;
	}

	case OBJECT_START_USE_T:
	{
		cmd_object_start_use_t * pCmd = (cmd_object_start_use_t*)Msg.dwParam1;
		StartWork(WT_NORMAL, WORK_USEITEM, pCmd->item);
		TurnFaceTo(pCmd->target);
		break;
	}

	default:
		ASSERT(0);
		return;
	}
}

void CECElsePlayer::OnMsgPlayerUseItemWithData(const ECMSG& Msg)
{
	using namespace S2C;
	object_use_item_with_arg* pCmd = (object_use_item_with_arg*)Msg.dwParam1;
	WorkFinished(WORK_USEITEM);
	PlayUseItemEffect(pCmd->item, pCmd->arg, pCmd->size);
}

void CECElsePlayer::OnMsgPlayerRoot(const ECMSG& Msg)
{
	using namespace S2C;
	cmd_object_root* pCmd = (cmd_object_root*)Msg.dwParam1;
	ASSERT(pCmd && pCmd->id == m_PlayerInfo.cid);

	SetServerPos(pCmd->pos);

	WorkFinished(WORK_MOVE);
}

void CECElsePlayer::OnMsgPlayerDoAction(const ECMSG& Msg)
{
	using namespace S2C;
	cmd_obj_do_action* pCmd = (cmd_obj_do_action*)Msg.dwParam1;
	ASSERT(pCmd && pCmd->id == m_PlayerInfo.cid);

	if (m_pPlayerModel)
	{
		int iCurAction = (int)m_pPlayerModel->GetCurActionUserData(ACT_CHANNEL_BODY);
		if (IsValidAction(iCurAction))
		{
			PlayAction(pCmd->action, 1.0f, false);
			PlayAction(iCurAction, 1.0f, true, 200, true);
		}
	}
}

void CECElsePlayer::OnMsgPlayerInTeam(const ECMSG& Msg)
{
	using namespace S2C;
	cmd_player_in_team* pCmd = (cmd_player_in_team*)Msg.dwParam1;
	ASSERT(pCmd && pCmd->id == m_PlayerInfo.cid);

	m_iInTeamState = pCmd->state;
}

void CECElsePlayer::OnMsgPickupMatter(const ECMSG& Msg)
{
	using namespace S2C;
	cmd_matter_pickup* pCmd = (cmd_matter_pickup*)Msg.dwParam1;
	ASSERT(pCmd && pCmd->who == m_PlayerInfo.cid);

	StartWork(WT_NORMAL, WORK_PICKUP, 0);
}

void CECElsePlayer::OnMsgPlayerGather(const ECMSG& Msg)
{
	using namespace S2C;

	if (Msg.dwParam2 == PLAYER_GATHER_START)
	{
		cmd_player_gather_start* pCmd = (cmd_player_gather_start*)Msg.dwParam1;
		ASSERT(pCmd && pCmd->pid == m_PlayerInfo.cid);
		StartWork(WT_NORMAL, WORK_PICKUP, 1);
	}
	else if (Msg.dwParam2 == PLAYER_GATHER_STOP)
	{
		cmd_player_gather_stop* pCmd = (cmd_player_gather_stop*)Msg.dwParam1;
		ASSERT(pCmd && pCmd->pid == m_PlayerInfo.cid);

		WorkFinished(WORK_PICKUP);
	}
}

void CECElsePlayer::OnMsgDoConEmote(const ECMSG& Msg)
{
	using namespace S2C;
	cmd_do_concurrent_emote* pCmd = (cmd_do_concurrent_emote*)Msg.dwParam1;
	ASSERT(pCmd && Msg.dwParam2 == DO_CONCURRENT_EMOTE);

	PlayAction(pCmd->emotion, 1.0f, true);
}

void CECElsePlayer::OnMsgPlayerChangeShape(const ECMSG& Msg)
{
	using namespace S2C;
	cmd_player_chgshape* pCmd = (cmd_player_chgshape*)Msg.dwParam1;
	ASSERT(pCmd && pCmd->idPlayer == m_PlayerInfo.cid);
	TransformShape(pCmd->shape, true);
}

void CECElsePlayer::OnMsgBoothOperation(const ECMSG& Msg)
{
	using namespace S2C;

	switch (Msg.dwParam2)
	{
	case PLAYER_OPEN_BOOTH:
	{
		cmd_player_open_booth* pCmd = (cmd_player_open_booth*)Msg.dwParam1;
		ASSERT(pCmd->pid == m_PlayerInfo.cid);

		m_iBoothState	= 2;
		m_crcBooth		= pCmd->booth_crc;

		m_idBoothItem	= pCmd->item_id;
		DATA_TYPE DataType;
		BOOTH_FIGURE_ITEM_ESSENCE* pDBEssence = (BOOTH_FIGURE_ITEM_ESSENCE*)g_pGame->GetElementDataMan()->get_data_ptr(pCmd->item_id, ID_SPACE_ESSENCE, DataType);
		if(pCmd->item_id != -1)
			m_pSpeBoothModel = m_pPlayerMan->LoadSpeBoothModel(pDBEssence->file_model);
		
		//	Set booth name
		if (pCmd->name_len && pCmd->name)
		{
			ACString strBoothName((const ACHAR*)pCmd->name, pCmd->name_len / sizeof (ACHAR));
			SetBoothName(strBoothName);
		}
		else
			SetBoothName(_AL("Booth"));

		StartWork(WT_NORMAL, WORK_IDLE);
		SetIdleWorkInfo(IDLE_BOOTH, 0, 0);
		break;
	}
	case PLAYER_CLOSE_BOOTH:
	{
		cmd_player_close_booth* pCmd = (cmd_player_close_booth*)Msg.dwParam1;
		ASSERT(pCmd->pid == m_PlayerInfo.cid);

		m_iBoothState	= 0;
		
		//	If host is visiting this player's booth, notify him
		CECHostPlayer* pHost = m_pPlayerMan->GetHostPlayer();
		if (pHost->GetBoothState() == 3 && pHost->GetCurServiceNPC() == m_PlayerInfo.cid)
			pHost->OnOtherBoothClosed();

		IdleWorkFinished(IDLE_BOOTH);
		break;
	}
	case BOOTH_NAME:
	{
		cmd_booth_name* pCmd = (cmd_booth_name*)Msg.dwParam1;
		ASSERT(pCmd->pid == m_PlayerInfo.cid);

		if (pCmd->name && pCmd->name_len)
		{
			m_crcBooth = pCmd->crc_name;
			ACString strBoothName((const ACHAR*)pCmd->name, pCmd->name_len / sizeof (ACHAR));
			SetBoothName(strBoothName);
		}

		break;
	}
	}
}

void CECElsePlayer::OnMsgPlayerTravel(const ECMSG& Msg)
{
	using namespace S2C;
}

void CECElsePlayer::OnMsgPlayerChangeFace(const ECMSG& Msg)
{
	using namespace S2C;
	cmd_player_chg_face* pCmd = (cmd_player_chg_face*)Msg.dwParam1;
//	m_PlayerInfo.crc_c = pCmd->crc_c;
	g_pGame->GetGameSession()->GetRoleCustomizeData(1, &m_PlayerInfo.cid);
}

void CECElsePlayer::OnMsgPlayerBindOpt(const ECMSG& Msg)
{
	using namespace S2C;

	if (Msg.dwParam2 == PLAYER_BIND_STOP)
	{
		m_idCandBuddy = 0;
		m_bCandHangerOn = false;

		if (!m_bHangerOn)
			DetachBuddy(NULL);
	}
}
void CECElsePlayer::OnMsgPlayerBathOpt(const ECMSG& Msg)
{
    using namespace S2C;
	if(Msg.dwParam2 == PLAYER_BATH_STOP)
	{
		//player_bath_stop* pCmd = (player_bath_stop*)Msg.dwParam1;
		m_idCandBathBuddy = 0;
		m_bCandHangerOn = false;

		if(!m_bHangerOn)
			DetachBathBuddy(NULL);
		PlayAction(ACT_STAND, 1.0f, false);

	}
       
}

void CECElsePlayer::OnMsgPlayerQilinBindOpt(const ECMSG& Msg)
{
	using namespace S2C;

	CECHostPlayer* pHost = g_pGame->GetGameRun()->GetHostPlayer();

	switch(Msg.dwParam2)
	{
	case PLAYER_QILIN_STOP:
	{
		CECPlayer *pBuddy = m_pPlayerMan->GetPlayer(m_idQilinConnectBuddy);
		if(pBuddy)
		{
			pBuddy->m_idQilinConnectBuddy = 0;
			pBuddy->m_bQilinConnectHangerOn = false;
		}
		m_idQilinConnectBuddy = 0;
		m_bQilinConnectHangerOn = false;
		m_idCandQilinBuddy = 0;
		m_bCandHangerOn = false;

		//	Detach buddy
		pHost->RemovePullGfxInfo(m_PlayerInfo.cid);
		DetachQilinBuddy(NULL);	
		PlayAction(ACT_STAND, 1.0f, false);
		break;
	}
	case PLAYER_QILIN_DISCONNECT:
	{
		pHost->AddPullGfxInfo(m_idQilinConnectBuddy, m_PlayerInfo.cid, 2);
		DetachQilinBuddy(NULL);
		break;
	}
	case PLAYER_QILIN_RECONNECT:
	{
		pHost->RemovePullGfxInfo(m_idQilinConnectBuddy);
		A3DVECTOR3 target_pos;
		CECPlayer* pBuddy = m_pPlayerMan->GetPlayer(m_idQilinConnectBuddy);
		if(pBuddy)
			target_pos = pBuddy->GetPos();

//		m_bQilinFlashMove = true;
//		SetServerPos(target_pos);
//		m_fMoveSpeed = 20.0f;
//		StartWork(WT_NORMAL, WORK_FLASHMOVE);

		m_bQilinFlashMove = true;
		SetMoveInfo(2000, 2000, 20.0f, target_pos, m_idQilinConnectBuddy);
		StartWork(WT_NORMAL, WORK_MOVENOCOLLISION);

		break;
	}
	}
}


// 多人骑乘
void CECElsePlayer::OnMsgPlayerMultiBindOpt(const ECMSG& Msg)
{
	using namespace S2C;

	if (Msg.dwParam2 == PLAYER_MULTI_BIND_LEAVE)
	{
		cmd_player_multi_bind_leave* pCmd = (cmd_player_multi_bind_leave*)Msg.dwParam1;
		CECPlayer * pMember = m_pPlayerMan->GetPlayer(pCmd->member);
		if (pMember)
			DetachMultiBuddy(pCmd->pos, pMember);
	}
	else if(Msg.dwParam2 == PLAYER_MULTI_BIND_JOIN)
	{
		cmd_player_multi_bind_join* pCmd = (cmd_player_multi_bind_join*)Msg.dwParam1;

		m_iMultiCandBuddyId[pCmd->pos] = pCmd->member;
		//AttachMultiBuddy(pCmd->pos, pCmd->member);
	}
	else if(Msg.dwParam2 == PLAYER_MULTI_BIND_KICK)
	{
		cmd_player_multi_bind_kick* pCmd = (cmd_player_multi_bind_kick*)Msg.dwParam1;
		DetachMultiBuddy(pCmd->pos);
 	}
}

void CECElsePlayer::OnMsgPlayerCarrierOpt(const ECMSG& Msg)
{
	using namespace S2C;

	switch(Msg.dwParam2)
	{
	case PLAYER_ENTER_CARRIER:
	{
		cmd_player_enter_carrier* pCmd = (cmd_player_enter_carrier*)Msg.dwParam1;

		m_idCarrier = pCmd->carrier_id;	
		A3DVECTOR3 vRelPos(pCmd->rpos);
		A3DVECTOR3 vRelDir(glb_DecompressDirH(pCmd->rdir));
 		const A3DMATRIX4 matTrans = a3d_ViewMatrix(vRelPos, vRelDir, g_vAxisY, 0.0f);
		m_matRelCarrier = a3d_InverseTM(matTrans);

		g_pGame->RuntimeDebugInfo(RTDCOL_NETWORK, _AL("NET - Elseplayer enter carrier"));

		break;
	}
	case PLAYER_LEAVE_CARRIER:
	{
		cmd_player_leave_carrier* pCmd = (cmd_player_leave_carrier*)Msg.dwParam1;
		
		m_idCarrier = 0;
		m_matRelCarrier.Identity();

		g_pGame->RuntimeDebugInfo(RTDCOL_NETWORK, _AL("NET - Elseplayer leave carrier"));	
		
		break;
	}
	case PLAYER_MOVE_ON_CARRIER:
	{
		cmd_player_move_on_carrier* pCmd = (cmd_player_move_on_carrier*)Msg.dwParam1;
		
		CECNPC* pNPCCarrier = g_pGame->GetGameRun()->GetWorld()->GetNPCMan()->GetNPC(m_idCarrier);
 		if (!pNPCCarrier)
		{
			//	The carrier has not been loaded now. 
			//	We update the relative matrix using move_on_carrier information. 
			//  m_matRelCarrier.SetRow(3, pCmd->vRelDest);
			return;
		}

		if (!pCmd->use_time)
			return;
		
		m_vRelServerPos = pCmd->vRelDest;
		A3DVECTOR3 vDstPos = pCmd->vRelDest * pNPCCarrier->GetAbsoluteTM();

		cmd_object_move FakeCmd;
		FakeCmd.dest = vDstPos;
		FakeCmd.id = pCmd->player_id;
		FakeCmd.move_mode = pCmd->move_mode;
		FakeCmd.sSpeed = pCmd->sSpeed;
		FakeCmd.use_time = pCmd->use_time;

		MoveTo(FakeCmd);

		g_pGame->RuntimeDebugInfo(RTDCOL_NETWORK, _AL("NET - Elseplayer move on carrier"));	

		break;
	}
	case PLAYER_STOP_ON_CARRIER:
	{
		cmd_player_stop_on_carrier* pCmd = (cmd_player_stop_on_carrier*)Msg.dwParam1;

		CECNPC* pNPCCarrier = g_pGame->GetGameRun()->GetWorld()->GetNPCMan()->GetNPC(m_idCarrier);
 		if (!pNPCCarrier)
			return;

		m_vRelServerPos = pCmd->vRelDest;
		m_vRelStopDir = glb_DecompressDirH(pCmd->rdir);
		A3DVECTOR3 vDstPos = pCmd->vRelDest * pNPCCarrier->GetAbsoluteTM();
		A3DVECTOR3 vDir = a3d_VectorMatrix3x3(glb_DecompressDirH(pCmd->rdir), pNPCCarrier->GetAbsoluteTM());

		cmd_object_stop_move FakeCmd;
		FakeCmd.dest = vDstPos;
		FakeCmd.dir = glb_CompressDirH(vDir.x, vDir.z);
		FakeCmd.id = pCmd->player_id;
		FakeCmd.move_mode = pCmd->move_mode;
		FakeCmd.sSpeed = pCmd->sSpeed;

		StopMoveTo(FakeCmd);

		g_pGame->RuntimeDebugInfo(RTDCOL_NETWORK, _AL("NET - Elseplayer stop move on carrier"));	

		break;
	}
	default:
		ASSERT(0);
	    break;
	}
}

void CECElsePlayer::OnMsgPlayerDuelOpt(const ECMSG& Msg)
{
	using namespace S2C;

	if (Msg.dwParam2 == PLAYER_DUEL_START)
	{
		m_pvp.iDuelState = DUEL_ST_INDUEL;
		m_pvp.iDuelRlt = 0;
	}
	else if (Msg.dwParam2 == DUEL_STOP)
		m_pvp.iDuelState = DUEL_ST_NONE;
	else
	{
		ASSERT(0);
	}
}

void CECElsePlayer::OnMsgPlayerStartInteract(const ECMSG& Msg)
{
	using namespace S2C;

	cmd_player_start_interact* pCmd = (cmd_player_start_interact*)Msg.dwParam1;
	ASSERT(pCmd && pCmd->player_id == m_PlayerInfo.cid);

	CECInteractor* pInteractor = (CECInteractor*) g_pGame->GetGameRun()->GetWorld()->GetObject(pCmd->obj_id, 0);
	if (!pInteractor)
		return;

	pInteractor->StartInteract(m_PlayerInfo.cid, pCmd->pos);
	StartInteract(pCmd->obj_id, pCmd->pos);
}

void CECElsePlayer::OnMsgPlayerInInteract(const ECMSG& Msg)
{
	using namespace S2C;

	cmd_player_in_interact* pCmd = (cmd_player_in_interact*)Msg.dwParam1;
	ASSERT(pCmd && pCmd->player_id == m_PlayerInfo.cid);

	CECInteractor* pInteractor = (CECInteractor*) g_pGame->GetGameRun()->GetWorld()->GetObject(pCmd->obj_id, 0);
	if (!pInteractor)
		return;

	pInteractor->InInteract(m_PlayerInfo.cid, pCmd->pos);
	InInteract(pCmd->obj_id, pCmd->pos);
}

void CECElsePlayer::OnMsgPlayerEndInteract(const ECMSG& Msg)
{
	using namespace S2C;

	cmd_player_end_interact* pCmd = (cmd_player_end_interact*)Msg.dwParam1;
	ASSERT(pCmd && pCmd->player_id == m_PlayerInfo.cid);

	CECInteractor* pInteractor = (CECInteractor*) g_pGame->GetGameRun()->GetWorld()->GetObject(pCmd->obj_id, 0);
	if (!pInteractor)
		return;

	pInteractor->EndInteract(m_PlayerInfo.cid, pCmd->pos);
	EndInteract(pCmd->obj_id, pCmd->pos);
}

void CECElsePlayer::OnMsgPlayerFinishInteract(const ECMSG& Msg)
{
	using namespace S2C;

	cmd_player_finish_interact* pCmd = (cmd_player_finish_interact*)Msg.dwParam1;
	ASSERT(pCmd && pCmd->player_id == m_PlayerInfo.cid);

	CECInteractor* pInteractor = (CECInteractor*) g_pGame->GetGameRun()->GetWorld()->GetObject(pCmd->obj_id, 0);
	if (!pInteractor)
		return;

	//	Finish interactions.
	pInteractor->FinishInteract(m_PlayerInfo.cid, pCmd->pos);
	FinishInteract(pCmd->obj_id, pCmd->pos);

	//	Sync positions.
	pInteractor->SetPos(pCmd->obj_pos);
	pInteractor->SetServerPos(pCmd->obj_pos);
	SetPos(pCmd->player_pos);
	SetServerPos(pCmd->player_pos);
}

void CECElsePlayer::OnMsgPlayerCancelInteract(const ECMSG& Msg)
{
	using namespace S2C;

	cmd_player_cancel_interact* pCmd = (cmd_player_cancel_interact*)Msg.dwParam1;
	ASSERT(pCmd && pCmd->player_id == m_PlayerInfo.cid);

	CECInteractor* pInteractor = (CECInteractor*) g_pGame->GetGameRun()->GetWorld()->GetObject(pCmd->obj_id, 0);
	if (!pInteractor)
		return;

	//	Finish interactions.
	pInteractor->CancelInteract(m_PlayerInfo.cid);
	CancelInteract(pCmd->obj_id);

	//	Sync positions.
	pInteractor->SetPos(pCmd->obj_pos);
	pInteractor->SetServerPos(pCmd->obj_pos);
	SetPos(pCmd->player_pos);
	SetServerPos(pCmd->player_pos);
}

void CECElsePlayer::OnMsgPlayerPlayActiveEmote(const ECMSG& Msg)
{
	using namespace S2C;

	play_active_emote* pCmd = (play_active_emote*)Msg.dwParam1;
	ASSERT(pCmd);

	m_CandEmoteInfo.id = pCmd->playerid2;
	m_CandEmoteInfo.type = pCmd->type;
}

void CECElsePlayer::OnMsgPlayerStopActiveEmote(const ECMSG& Msg)
{
	using namespace S2C;

	stop_active_emote* pCmd = (stop_active_emote*)Msg.dwParam1;
	ASSERT(pCmd);

	if (pCmd->playerid2 != m_iBuddyId || pCmd->type != m_iEmoteType)
		return;

	StopActiveEmote();
}

void CECElsePlayer::OnMsgTravelFree(const ECMSG& Msg)
{	
	using namespace S2C;
	CECNPCMan* pNPCMan = g_pGame->GetGameRun()->GetWorld()->GetNPCMan();
	CECGameUIMan *pGameUI = g_pGame->GetGameRun()->GetUIManager()->GetInGameUIMan();

	switch(Msg.dwParam2)
	{
	case PLAYER_START_TRAVEL_AROUND:
		{
			player_start_travel_around* pCmd = (player_start_travel_around*)Msg.dwParam1;
			m_iTravalVehicleId = pCmd->travel_id;
			ChangeFreeFlyWing();
			//ShowWing(false);
		}
		break;
	case PLAYER_STOP_TRAVEL_AROUND:
		{
			m_iTravalVehicleId = 0;
			m_dwStates &= ~GP_STATE_FLY;
			ShowWing(false);
			if(m_iItemWingOrg>0)
			{
				m_aNewEquips[EQUIPIVTR_WING] = m_iItemWingOrg;
				LoadPlayerEquipments();
			}
		}
		break;
	}

}

void CECElsePlayer::ChangeFreeFlyWing()
{
	if(m_iTravalVehicleId==0)
		return;

	m_iItemWingOrg = m_aNewEquips[EQUIPIVTR_WING];
	m_aNewEquips[EQUIPIVTR_WING] = m_iTravalVehicleId;
	m_iTravalVehicleId = 0;

	LoadPlayerEquipments();

	m_dwStates |= GP_STATE_FLY;
	/*CECIvtrTalismanMain *pEquip = (CECIvtrTalismanMain*)CECIvtrItem::CreateItem(m_iTravalVehicleId, 0, 1);
	if(!pEquip)
		return;

	TALISMAN_MAINPART_ESSENCE* pTalisman = (TALISMAN_MAINPART_ESSENCE*)pEquip->GetDBEssence();

	if (!pTalisman->is_aircraft)
		return;

	const char* szModelFile = "";
	szModelFile = g_pGame->GetDataPath(pTalisman->file_model[0]);
	AString strNewModelFile;
	EquipsLoadResult loadResult;
	memset(&loadResult, 0, sizeof(EquipsLoadResult));
	loadResult.nFlyMode = pTalisman->fly_mode;
	ChangeWing(&loadResult, szModelFile, m_BasicProps.iProfession, m_iChangeShape);

	SetEquipsLoadedResult(loadResult, true);
	delete pEquip;
	m_iTravalVehicleId = 0;*/
}

bool CECElsePlayer::DoEmote(int idEmote)
{
	int iAction = ACT_STAND;
	bool bSession = false;

	//	Select action according to pose
	switch (idEmote)
	{
	case ROLEEXP_WAVE:		iAction = ACT_EXP_WAVE;			break;
	case ROLEEXP_NOD:		iAction = ACT_EXP_NOD;			break;
	case ROLEEXP_SHAKEHEAD:	iAction = ACT_EXP_SHAKEHEAD;	break;
	case ROLEEXP_SHRUG:		iAction = ACT_EXP_SHRUG;		break;
	case ROLEEXP_LAUGH:		iAction = ACT_EXP_LAUGH;		break;
	case ROLEEXP_ANGRY:		iAction = ACT_EXP_ANGRY;		break;
	case ROLEEXP_STUN:		iAction = ACT_EXP_STUN;			break;
	case ROLEEXP_DEPRESSED:	iAction = ACT_EXP_DEPRESSED;	break;
	case ROLEEXP_KISSHAND:	iAction = ACT_EXP_KISSHAND;		break;
	case ROLEEXP_SHY:		iAction = ACT_EXP_SHY;			break;
	case ROLEEXP_SALUTE:	iAction = ACT_EXP_SALUTE;		break;

	case ROLEEXP_SITDOWN:
		
		iAction = ACT_EXP_SITDOWN;
		bSession = true;
		break;

	case ROLEEXP_ASSAULT:	iAction = ACT_EXP_ASSAULT;		break;
	case ROLEEXP_THINK:		iAction = ACT_EXP_THINK;		break;
	case ROLEEXP_DEFIANCE:	iAction = ACT_EXP_DEFIANCE;		break;
	case ROLEEXP_VICTORY:	iAction = ACT_EXP_VICTORY;		break;
	case ROLEEXP_GAPE:		iAction = ACT_GAPE;				break;
	case ROLEEXP_KISS:		iAction = ACT_EXP_KISS;			break;
	case ROLEEXP_FIGHT:		iAction = ACT_EXP_FIGHT;		break;
	case ROLEEXP_ATTACK1:	iAction = ACT_EXP_ATTACK1;		break;
	case ROLEEXP_ATTACK2:	iAction = ACT_EXP_ATTACK2;		break;
	case ROLEEXP_ATTACK3:	iAction = ACT_EXP_ATTACK3;		break;
	case ROLEEXP_ATTACK4:	iAction = ACT_EXP_ATTACK4;		break;
	case ROLEEXP_DEFENCE:	iAction = ACT_EXP_DEFENCE;		break;	
	case ROLEEXP_FALL:		iAction = ACT_EXP_FALL;			break;
	case ROLEEXP_FALLONGROUND:	iAction = ACT_EXP_FALLONGROUND;	break;
	case ROLEEXP_LOOKAROUND:	iAction = ACT_EXP_LOOKAROUND;	break;	
	case ROLEEXP_DANCE:		iAction = ACT_EXP_DANCE;		break;
	case ROLEEXP_TWO_KISS:	iAction = ACT_TWO_KISS;			break;
	case ROLEEXP_FIREWORK:	iAction = ACT_ATTACK_TOSS;	break;
	case ROLEEXP_BATH:      iAction = ACT_BATH;  break;
//	case ROLEEXP_PUPPETSHOW: PupPetShow(); break;
	default:
		ASSERT(0);
		break;
	}

//	if(idEmote == ROLEEXP_PUPPETSHOW)
//		return true;

	PlayAction(iAction, 1.0f);

	if (!bSession)
		PlayAction(GetMoveStandAction(false, false), 1.0f, true, 300, true);

	return true;
}

bool CECElsePlayer::PlayActiveEmote(ACTIVEEMOTEINFO info)
{
	if (! CECPlayer::PlayActiveEmote(info))
		return false;

	int iNextAction = ACT_STAND;
	switch(m_iEmoteActionType)
	{
		case CECPlayer::ACT_PROPOSE:
			iNextAction = CECPlayer::ACT_PROPOSE_LOOP;
		break;

		default:
			iNextAction  = ACT_STAND;
	}

	PlayAction(iNextAction, 1.0f, true, 300, true);
	return true;
}

void CECElsePlayer::StartGeneralOprt(int op_id, int duration)
{
	StartWork(WT_NORMAL, WORK_GENERAL_OPRT, op_id);
}

void CECElsePlayer::StopGeneralOprt(int op_id)
{
	WorkFinished(WORK_GENERAL_OPRT);
}

//	Get number of equipped items of specified suite
int CECElsePlayer::GetEquippedSuiteItem(int idSuite, int* aItems/* NULL */)
{
	CECGame::SuiteEquipTable& SuiteTab = g_pGame->GetSuiteEquipTable();

	int i, iItemCnt = 0;

	for (i=0; i < SIZE_EQUIPIVTR; i++)
	{
		int idEquip = m_aEquips[i] & 0xffff;
		if (!idEquip)
			continue;

		CECGame::SuiteEquipTable::pair_type pair = SuiteTab.get(idEquip);
		if (!pair.second || idSuite != *pair.first)
			continue;

		if (aItems)
			aItems[iItemCnt] = idEquip;
		
		iItemCnt++;
	}

	return iItemCnt;
}

//	Search the full suite
int CECElsePlayer::SearchFullSuite()
{
	DATA_TYPE DataType = DT_INVALID;
	elementdataman* pDataMan = g_pGame->GetElementDataMan();
	CECGame::SuiteEquipTable& SuiteMapTab = g_pGame->GetSuiteEquipTable();
	
	int i;
	typedef abase::hashtab<int, int, abase::_hash_function> SuiteTable;
	SuiteTable SuiteTab(32);

	//	Check how many suits of equipment host have dressed
	for (i=0; i < SIZE_EQUIPIVTR; i++)
	{
		int idEquip = m_aEquips[i] & 0xffff;
		if (!idEquip)
			continue;
		
		CECGame::SuiteEquipTable::pair_type pair = SuiteMapTab.get(idEquip);
		if (!pair.second)
			continue;

		int idSuite = *pair.first;

		const void* pData = pDataMan->get_data_ptr(idSuite, ID_SPACE_ESSENCE, DataType);
		if (DataType != DT_SUITE_ESSENCE)
			continue;

		const SUITE_ESSENCE* pSuiteEss = (const SUITE_ESSENCE*)pData;
		if (pSuiteEss->file_gfx[0])
			SuiteTab.put(idSuite, pSuiteEss->max_equips);
	}

	if (!SuiteTab.size())
		return 0;	//	No suite was found

	//	Check whether suite is full
	SuiteTable::iterator it = SuiteTab.begin();
	for (; it != SuiteTab.end(); ++it)
	{
		int idSuite = *it.key();
		int iMaxNum = *it.value();

		if (iMaxNum == GetEquippedSuiteItem(idSuite))
			return idSuite;
	}

	return 0;
}

//	When weapon has been changed, this function will be called
void CECElsePlayer::OnWeaponChanged()
{
	// replay current action to yield weapon change result
	if(m_pPlayerModel)
		PlayAction(m_pPlayerModel->GetCurActionUserData(ACT_CHANNEL_BODY), 1.0f, true);
}

void CECElsePlayer::UpdateCurEquips(int id)
{
	CECPlayer* pPlayer = m_pPlayerMan->GetPlayer(id);
	if (pPlayer)
	{
		memcpy(m_aNewEquips, pPlayer->GetHideEquips(), sizeof(m_aNewEquips));
		m_bEquipReady = true;
	}
}

void CECElsePlayer::UpdateSpecialState(int state, bool on)
{
	int oldstate = m_iSpecialState;
	if (on)
		m_iSpecialState |= (1 << state);
	else 
		m_iSpecialState &= ~(1<< state);

	if (on)
	{
		if (m_iSpecialState & SPECIAL_STATE_FROZEN)
		{
			WorkFinished(WORK_MOVE);

			if (m_pTransfiguredModel)
				m_pTransfiguredModel->StopChannelAction(ACT_CHANNEL_BODY, true);
			else if (m_pChangedModel)
				m_pChangedModel->StopChannelAction(ACT_CHANNEL_BODY, true);
			else if (m_pPetModel && m_pPlayerModel)
			{	
				m_pPetModel->StopChannelAction(ACT_CHANNEL_BODY, true);
				m_pPlayerModel->StopChannelAction(ACT_CHANNEL_BODY, true);
			}
			else if(m_pPuppetModel && m_pPlayerModel)
			{
				m_pPuppetModel->StopChannelAction(ACT_CHANNEL_BODY, true);
				m_pPlayerModel->StopChannelAction(ACT_CHANNEL_BODY, true);
			}
			else if (m_pPlayerModel)
				m_pPlayerModel->StopChannelAction(ACT_CHANNEL_BODY, true);
		}

		if(m_iSpecialState & SPECIAL_STATE_XUANFENG)
			PlayAction(GetMoveStandAction(CurActionIsMoving(), IsFighting()), 1.0f, false);
	}

	if (!on)
	{
		if((oldstate & SPECIAL_STATE_FROZEN) && (m_iSpecialState & SPECIAL_STATE_FROZEN) == 0)
		{
			PlayAction(ACT_STAND, 1.0f, false);
// 			if (m_pTransfiguredModel)
// 				m_pTransfiguredModel->StopChannelAction(ACT_CHANNEL_BODY, false);
// 			else if (m_pChangedModel)
// 				m_pChangedModel->StopChannelAction(ACT_CHANNEL_BODY, false);
// 			else if (m_pPetModel && m_pPlayerModel)
// 			{	
// 				m_pPetModel->StopChannelAction(ACT_CHANNEL_BODY, false);
// 				m_pPlayerModel->StopChannelAction(ACT_CHANNEL_BODY, false);
// 			}
// 			else if (m_pPlayerModel)
// 				m_pPlayerModel->StopChannelAction(ACT_CHANNEL_BODY, false);
		}
		if((oldstate & SPECIAL_STATE_XUANFENG) && (m_iSpecialState & SPECIAL_STATE_XUANFENG) == 0)
		{
			PlayAction(GetMoveStandAction(CurActionIsMoving(), IsFighting()), 1.0f, false);
		}
	}
}

// Receive the start-special-move message from server, and then start the special move work.
void CECElsePlayer::CheckStartSepcialMove( const S2C::cmd_object_start_special_move& specialMoveCmd )
{
	g_pGame->RuntimeDebugInfo(RTDCOL_WARNING, ACString(GetName()) + _AL("start move"));

	SetPos( specialMoveCmd.cur_pos );

	if ( specialMoveCmd.collision_state )
	{
		UpdateCollisionState( specialMoveCmd.time_stamp, 1 );
	}
	
	SetVA(specialMoveCmd.velocity, specialMoveCmd.acceleration);
	StartSpecialMove();	//???
	m_pMovingCorrect->m_iLastRefresh = 0;
// 
// 	//Currently in the special-move state. It means the else player is being colloded by other players in this client.
// 	if ( (m_iCurWorkType == WT_NORMAL && m_aWorks[WT_NORMAL] == WORK_SPECIALMOVE) || (CheckMoveCmd(specialMoveCmd.time_stamp)) )//!GetCollisionState() && specialMoveCmd.collision_state
// 	{
// 		//What's the next????
// //		SetVelocity( specialMoveCmd.velocity );
// //		SetAcceleration( specialMoveCmd.acceleration );
// 	}
// 	else if ( m_iCurWorkType < WT_NORMAL || m_aWorks[WT_NORMAL] != WORK_SPECIALMOVE )	//Can start the special move.
// 	{
// 		SetPos(specialMoveCmd.cur_pos);
// 	}
// 	//-------------------------------------------------------------------------
/*	if ( GetCollisionState() )	//客户端已经碰撞了
	{
		if ( specialMoveCmd.collision_state )	//服务器也碰撞了 清除碰撞状态
		{
//			SetServerCollisionState( false );
			SetCollisionState( false );
			a_LogOutput( 1, "PlayerID=%d----------两边同时处于碰撞啊哦！----------", GetCharacterID() );
		}
		else
		{
			a_LogOutput( 1, "PlayerID=%d----------怎么可能出现客户端ElsePlayer碰撞了，服务器HostPlayer没有碰撞呢？----------", GetCharacterID() );
		}
	}
	else						//客户端不处于碰撞状态
	{
		if ( specialMoveCmd.collision_state )	//服务器单边处于碰撞状态
		{
//			SetServerCollisionState( true );
			a_LogOutput( 1, "PlayerID=%d----------ElsePlayer 没处于碰撞，HostPlayer处于碰撞----------", GetCharacterID() );
		}
	}*/
	//-------------------------------------------------------------------------
}

void CECElsePlayer::SetVA( const A3DVECTOR3& vVelocity, const A3DVECTOR3& vAcceleration )
{
	m_vVelocity = vVelocity;
	m_vAcceleration = vAcceleration;
}

void CECElsePlayer::SetSvrVA( const A3DVECTOR3& vVelocity, const A3DVECTOR3& vAcceleration )
{
	m_pMovingCorrect->SetVA(vVelocity, vAcceleration);
	m_vAcceleration = vAcceleration;
}

// Receive the special move message, and update the move info.
void CECElsePlayer::UpdateSpecialMove( const S2C::cmd_object_special_move& specialMoveCmd )
{
	g_pGame->RuntimeDebugInfo(0xffffffff, ACString(GetName()) + _AL("start move"));
#ifdef _DEBUG
/*	a_LogOutput(1, "From SERVER===The Move speed=(%f,%f,%f), The acceleration=(%f, %f, %f), The Current pos=(%f, %f, %f)", 
					specialMoveCmd.velocity.x, specialMoveCmd.velocity.y, specialMoveCmd.velocity.z, specialMoveCmd.acceleration.x, specialMoveCmd.acceleration.y, specialMoveCmd.acceleration.z,
					specialMoveCmd.cur_pos.x, specialMoveCmd.cur_pos.y, specialMoveCmd.cur_pos.z);
	a_LogOutput(1, "From CLIENT===The Move speed=(%f,%f,%f), The acceleration=(%f, %f, %f), The Current pos=(%f, %f, %f)", 
					m_vVelocity.x, m_vVelocity.y, m_vVelocity.z, m_vAcceleration.x, m_vAcceleration.y, m_vAcceleration.z,
					GetPos().x, GetPos().y, GetPos().z);
*/
#endif
	//-------------------------------------------------------------------------
/*	float fSpeedCur		= a3d_Magnitude( specialMoveCmd.velocity );
	float fSpeedPrev	= a3d_Magnitude( m_vVelocity );
	if ( fabs(fSpeedCur - fSpeedPrev) > 1.5 )
	{
		a_LogOutput( 1, "---The SERVER Speed=%f, The CLIENT Speed=%f---", fSpeedCur, fSpeedPrev );
		return;
	}
*/
/*	if ( GetCollisionState() )					//如果客户端这边玩家处于碰撞状态
	{
		if ( specialMoveCmd.collision_state )	//如果服务器那边该玩家也处于碰撞状态，则说明完全一致，清除碰撞状态
		{
			a_LogOutput( 1, "PlayerID=%d----------两边同时处于碰撞状态----------", GetCharacterID() );
//			SetServerCollisionState(false);	//?????服务器的标志需要及时清除么，这样就解决不了可能的服务器多次碰撞状态?????
			SetCollisionState( false );
			return;
		}
		else									//如果服务器那边该玩家没有处于碰撞状态，说明两边不同步，则暂时抛弃服务器发过来状态，等待服务器那边同步
		{
			if ( GetServerCollisionState() )	//服务器等到了客户端同步
			{
//				SetServerCollisionState(false);
				SetCollisionState( false );
				a_LogOutput( 1, "PlayerID=%d----------两边已经同步了，可以接受位置和速度同步----------", GetCharacterID() );
				return;
			}
			else								//客户端碰撞了，服务器那边还没碰撞
			{
				a_LogOutput( 1, "PlayerID=%d----------ElsePlayer处于碰撞状态,HostPlayer没处于碰撞状态----------", GetCharacterID() );
				return;
			}
		}
	}
	else										//客户端这边没处于碰撞状态
	{
		if ( specialMoveCmd.collision_state )	//如果服务器那边该玩家处于碰撞状态，说明两边不同步，则保存服务器已经碰撞的状态,等待客户端这边同步
		{
			a_LogOutput( 1, "PlayerID=%d----------HostPlayer处于碰撞状态,ElsePlayer没处于碰撞状态----------", GetCharacterID() );
//			SetServerCollisionState(true);
			return;
		}
	}
*/
	//---------------------------------------------------
	if ( specialMoveCmd.collision_state )	//发生了碰撞 更新碰撞状态
	{
		UpdateCollisionState( specialMoveCmd.time_stamp, 1 );
	}
	
	m_pMovingCorrect->m_iLastRefresh = 0;
	if ( CheckMoveCmd( specialMoveCmd.time_stamp ) )	//如果没有通过状态检测，则不更新server发过来的速度和加速度信息
	{
		return;
	}
	//---------------------------------------------------
	A3DVECTOR3 vDist = specialMoveCmd.cur_pos - GetPos();
	const float fDeltaTime = 0.025f; //experience value.
	A3DVECTOR3 vDeltaVelocity = vDist * fDeltaTime;
	a_LogOutput( 1, "----------Delta Velocity=(%f, %f, %f)---------", vDeltaVelocity.x, vDeltaVelocity.y, vDeltaVelocity.z );
	vDeltaVelocity += specialMoveCmd.velocity;
	//-------------------------------------------------------------------------
	SetSvrVA(vDeltaVelocity, specialMoveCmd.acceleration);
	//SetVelocity( vDeltaVelocity ); //specialMov^eCmd.velocity
	//SetAcceleration( specialMoveCmd.acceleration );
	if ( m_iCurWorkType < WT_NORMAL || m_aWorks[WT_NORMAL] != WORK_SPECIALMOVE )
	{
		StartWork( WT_NORMAL, WORK_SPECIALMOVE );
		m_bStopMove = false;	//???`
	}
}

void CECElsePlayer::CorrectPos(CECPlayer *pPlayer)
{
	A3DVECTOR3 deltTest = pPlayer->GetPos() - GetServerPos();
	float aabbLen = 1.2f; //暂时写死 pPlayer->GetPlayerAABB().Extents.MagnitudeH() + GetPlayerAABB().Extents.MagnitudeH();
	if (deltTest.Magnitude()<aabbLen)
	{
		A3DVECTOR3 delt = pPlayer->GetPos() - GetPos();
		delt.Normalize();
		pPlayer->SetPos(GetServerPos() + delt*aabbLen);
	}
}

//
void CECElsePlayer::StopSpecialMove( const S2C::cmd_object_stop_special_move& specialMoveCmd )
{
	g_pGame->RuntimeDebugInfo(RTDCOL_WARNING, ACString(GetName()) + _AL("stop move"));
	m_vStopDir				= glb_DecompressDirH( specialMoveCmd.cur_dir );
	m_bStopMove				= false;//true
	m_vMoveDir				= specialMoveCmd.cur_pos - GetPos();
	m_fMoveSpeed			= 2.5f;

	SetServerPos(specialMoveCmd.cur_pos);

	CorrectPos(m_pPlayerMan->GetHostPlayer());
	CECPlayerMan::PlayerTable::iterator iter = m_pPlayerMan->GetPlayerTable().begin();
	for ( ; iter != m_pPlayerMan->GetPlayerTable().end(); ++iter )
	{
		CECElsePlayer* pElsePlayer = *iter.value();
		if ( !pElsePlayer || pElsePlayer==this )	//玩家不存在或者玩家处于碰撞技能虚无状态，则不进行碰撞检测
		{
			continue;
		}
		CorrectPos(pElsePlayer);
	}
	m_pMovingCorrect->m_iLastRefresh = 10000;

	float fDist				= m_vMoveDir.Normalize();
	m_pMovingCorrect->m_bStop = true;
	if ( 1/*fDist >= MAX_LAGDIST || fDist < 0.25f*/ )	//0.15 is a thrshold.
	{
		SetPos(specialMoveCmd.cur_pos);
		SetDestDirAndUp(m_vStopDir, g_vAxisY, 150);
		WorkFinished(WORK_SPECIALMOVE);
		return;
	}

	A3DVECTOR3 vDir = m_vMoveDir;
	vDir.y = 0.0f;
	if ( !vDir.IsZero() )
	{
		vDir.Normalize();
		SetDestDirAndUp(vDir, g_vAxisY, 150);
	}

	if ( m_iCurWorkType < WT_NORMAL || m_aWorks[WT_NORMAL] != WORK_SPECIALMOVE )
	{
		StartWork(WT_NORMAL, WORK_SPECIALMOVE);
	}
}


//-----------------------------------------------------------------------------
void CECElsePlayer::UpdateCollisionState( int iCollisionTime, unsigned char ucType )
{
	int		i = 0;
	bool	bOk = false;
	if ( !ucType )	//client有新的碰撞状态
	{
		//检查整个server collision state info list，有相匹配的状态，则清除，否则，存入client state info list
		for ( ; i < m_CollisionStateInfoList.arServerCDStateList.GetSize(); ++i )
		{
			if ( abs(m_CollisionStateInfoList.arServerCDStateList[i].iCollisionTime - iCollisionTime) <= COLLISION_STATE_TIME_EPSILON )	//同一时间戳
			{
				bOk = true;
				break;
			}
		}

		if ( bOk )	//清除匹配状态
		{
			a_LogOutput(1, "client同步状态匹配，client时间戳：%d, server时间戳：%d", iCollisionTime, m_CollisionStateInfoList.arServerCDStateList[i].iCollisionTime);
			m_CollisionStateInfoList.arServerCDStateList.RemoveAt(i);
		}
		else		//存储client状态信息到client state info list
		{
			COLLISION_STATE_INFO	collisionStateInfo;
			collisionStateInfo.iCollisionTime	= iCollisionTime;
			m_CollisionStateInfoList.arClientCDStateList.Add( collisionStateInfo );
		}
	}
	else if ( ucType == 1 )	//服务器传过来的碰撞状态
	{
		//检查整个client collision state info list，如果有匹配的状态，则清除，否则，存入server collision state info list
		for ( ; i < m_CollisionStateInfoList.arClientCDStateList.GetSize(); ++i )
		{
			if ( abs(m_CollisionStateInfoList.arClientCDStateList[i].iCollisionTime - iCollisionTime) <= COLLISION_STATE_TIME_EPSILON )	//同一时间戳
			{
				bOk = true;
				break;
			}
		}

		if ( bOk )	//清除匹配状态
		{
			a_LogOutput(1, "server同步状态匹配，client时间戳：%d, server时间戳：%d", m_CollisionStateInfoList.arClientCDStateList[i].iCollisionTime, iCollisionTime);
			m_CollisionStateInfoList.arClientCDStateList.RemoveAt(i);
		}
		else		//存储client状态信息到server state info list
		{
			COLLISION_STATE_INFO	collisionStateInfo;
			collisionStateInfo.iCollisionTime	= iCollisionTime;
			m_CollisionStateInfoList.arServerCDStateList.Add( collisionStateInfo );			
		}
	}
}

int	CECElsePlayer::CheckMoveCmd( int iCmdTime )
{
	//消息发过来的时间和server collisionstate info list 和 client collision state info list进行比较
	//如果时间匹配，表明当前消息暂时不能立即用于更新
	int iRet = 0;
	if ( m_CollisionStateInfoList.arClientCDStateList.GetSize()  || m_CollisionStateInfoList.arServerCDStateList.GetSize() )
	{
		a_LogOutput(1, "----------CheckMoveCmd 不同步状态----------");
		iRet = 1;
	}

	return iRet;
}

void CECElsePlayer::CheckClearCollisionState()
{
	//-------------------------------------------------------------------------
	if ( m_CollisionStateInfoList.arClientCDStateList.GetSize() > 5 )
	{
		a_LogOutput(1, "---------- 客户端很多碰撞状态----------");
	}
	//-------------------------------------------------------------------------
	int i = 0;
	for ( ; i < m_CollisionStateInfoList.arClientCDStateList.GetSize(); ++i )
	{
		if ( abs(m_CollisionStateInfoList.arClientCDStateList[i].iCollisionTime - g_pGame->GetServerGMTTime()) > COLLISION_STATE_CLEAR_TIME )
		{
			m_CollisionStateInfoList.arClientCDStateList.RemoveAt(i);
			--i;
		}
	}

	for ( i = 0; i < m_CollisionStateInfoList.arServerCDStateList.GetSize(); ++i )
	{
		if ( abs(m_CollisionStateInfoList.arServerCDStateList[i].iCollisionTime - g_pGame->GetServerGMTTime()) > COLLISION_STATE_CLEAR_TIME )
		{
			m_CollisionStateInfoList.arServerCDStateList.RemoveAt(i);
			--i;
		}
	}

	// 保证能够显示
 	if (GetPos().x>47 || GetPos().x<-37 || GetPos().z>-208 || GetPos().z<-292 || GetPos().y<-210 || GetPos().y>230)
 	{
 		GetElsePos();
 	}
}

void CECElsePlayer::PrepareBlewUp(DWORD DWDuration, float fSpeed)
{
	m_dwPastTime = 0;
	m_dwDuration = DWDuration;
	m_fSpeed = fSpeed;
	m_fAccelerate = m_fSpeed / (float)(m_dwDuration * 0.0005f);
	m_fHeight = GetPos().y;
}

void CECElsePlayer::PrepareParabolic(DWORD dwDuration, float height, A3DVECTOR3 vecDest)
{
	float distance = a3d_MagnitudeH(vecDest - GetPos());
	
	m_dwStartTime = GetTickCount();
	m_dwDuration  = dwDuration;
	m_fSpeed	  = distance / (float) (dwDuration * 0.001f);
	m_fSpeedV	  = height / (float) (dwDuration * 0.0005f) * 2.0f;
	m_fAccelerate = m_fSpeedV / (float)(m_dwDuration * 0.0005f); 
	m_vecDest	  = vecDest;
	m_fHeight	  = height;
	m_fDistance	  = distance;
}

void CECElsePlayer::SetMoveInfo(DWORD dwStartTime, DWORD dwDuration, float fSpeed, A3DVECTOR3 vecDest, int idTarget)
{
	m_dwStartTime    = dwStartTime;
	m_dwDuration     = dwDuration;
	m_fSpeed	     = fSpeed;
	m_vecDest        = vecDest;
	m_idMoveToTarget = idTarget;
}
