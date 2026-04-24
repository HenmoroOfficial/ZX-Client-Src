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
#include "EC_Model.h"
#include "EC_ShadowRender.h"
#include "EC_NPC.h"
#ifdef BACK_VERSION
#include "EC_Viewport.h"
#else
#include "ECViewport.h"
#endif
#include "EC_ManNPC.h"
#include "EC_GFXCaster.h"
//#include "EC_TeamMan.h"
//#include "EC_Team.h"
#include "EC_ManPlayer.h"
#include "EC_FixedMsg.h"
//#include "EC_IvtrItem.h"
//#include "EC_IvtrTypes.h"
#include "EC_Resource.h"
//#include "EC_GameSession.h"
#include "EC_HostPlayer.h"
#include "EC_HostMove.h"
#include "EC_Utility.h"
//#include "EC_Skill.h"
//#include "EC_PateText.h"
#include "EC_Configs.h"
#include "EC_CDR.h"
//#include "EC_GameRecord.h"
//#include "EC_Achievement.h"
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

#define new A_DEBUG_NEW

///////////////////////////////////////////////////////////////////////////
//	
//	Define and Macro
//	
///////////////////////////////////////////////////////////////////////////

#define MAX_LAGDIST		8.0f	//	Maximum lag distance

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

	m_PickupCnt.SetPeriod(500);
	m_FightCnt.SetPeriod(15000);
	m_FightCnt.Reset(true);

	m_pcdr = new OtherPlayer_Move_Info;
//	m_pAchievementMan = new CECAchievementMan();
	
	memset(m_aNewEquips, 0, sizeof (m_aNewEquips));
}

CECElsePlayer::~CECElsePlayer()
{
	delete m_pcdr;
}

//	Initlaize object
bool CECElsePlayer::Init(const S2C::info_player_1& Info, int iAppearFlag, bool bReInit)
{
	return true;
}

//	Initlaize object
bool CECElsePlayer::Init(const S2C::info_npc& Info, int iAppearFlag, bool bReInit)
{
	return true;
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

	return true;
}

//	Set player info
void CECElsePlayer::SetPlayerInfo(const S2C::info_player_1& Info)
{
	
}

void CECElsePlayer::SetPlayerInfo(const S2C::info_npc& Info)
{
	
}
//	Release object
void CECElsePlayer::Release()
{
	if (m_pAppearGFX)
	{
		g_pGame->GetGFXCaster()->ReleaseGFXEx(m_pAppearGFX);
		m_pAppearGFX = NULL;
	}

// 	if (m_pCurSkill)
// 	{
// 		//	Release current skill
// 		delete m_pCurSkill;
// 		m_pCurSkill = NULL;
// 	}

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
// 	if ( m_iDelayedOperateState & GP_EXTRA_STATE_CIRCLEOFDOOM )
// 	{
// 		g_pGame->GetGameSession()->c2s_CmdQueryCodInfo( m_PlayerInfo.cid );
// 	}
// 	if ( m_iDelayedOperateState & GP_EXTRA_STATE_BE_DRAGGED )
// 	{
// 		g_pGame->GetGameSession()->c2s_CmdQueryBeDraggedInfo( m_PlayerInfo.cid );
// 	}
// 	if ( m_iDelayedOperateState & GP_EXTRA_STATE_BE_PULLED )
// 	{
// 		g_pGame->GetGameSession()->c2s_CmdQueryBePulledInfo( m_PlayerInfo.cid );
// 	}
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
	}

	if (m_iBoothState != 2)
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
// 	if (m_pCurSkill && m_pCurSkill->IsChargeable() && m_pCurSkill->IsChanting() && m_IncantCnt.IsFull())
// 	{
// 		m_pCurSkill->StartCharging(2000);
// 		PlayAttackEffect(m_idCurSkillTarget, m_pCurSkill->GetSkillID(), m_pCurSkill->GetSkillLevel(), m_pCurSkill->GetStamp(), -1, 0);
// 	}

	//	Save relative pose if player is on a carrier
	if (m_idCarrier)
		pNPCMan->CalcCarrierRelMat(m_idCarrier, GetAbsoluteTM(), m_matRelCarrier);

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
	else
	{
		CECModel* pModel = m_pPetModel ? m_pPetModel : m_pPlayerModel;

		if (m_pTransfiguredModel)
			pModel = m_pTransfiguredModel;
		else if (m_pChangedModel)
			pModel = m_pChangedModel;

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

	return true;
}

//	Render routine when player use simple models
bool CECElsePlayer::RenderSimpleModels(bool bBooth, CECViewport* pViewport, 
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
		pSimpleModel = m_pPlayerMan->GetBoothModel(m_iGender);

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
			pSimpleModel->Render(pViewport->GetA3DViewport());
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
	}

	return true;
}

//	Set server position
void CECElsePlayer::SetServerPos(const A3DVECTOR3& vPos)
{
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
	PlayAction(CECPlayer::ACT_FIGHT_CHARGE, 1.0f, true, 0, false); // Added 2011-09-05.
//	float fCurTerrainHeight = g_pGame->GetGameRun()->GetWorld()->GetTerrainHeight(GetPos());
//	m_fYDelta = GetPos().y - fCurTerrainHeight + 0.1f;
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

// 		if (m_pCurSkill)
// 		{
// 			//	Release current skill
// 			delete m_pCurSkill;
// 			m_pCurSkill = NULL;
// 
// 		}
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
	}
	else
	{
		SetPos(vCurPos);
		SetDirAndUp(vDir, g_vAxisY);
	}
	//-------------------------------------------------------------------------

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

// 	if (m_pPateName)
// 		m_pPateName->SetText(szName, false);

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
				m_aNewEquips[i] = aAddedEquip[iCount++];
				m_dwNewEqpMask |= (1 << i);
			}
		}
	}

	if (IsSkeletonReady())
	{
		//  竞技场中不能换装
		if(m_iBattleType != BATTLE_TYPE_ARENA && m_iBattleType != BATTLE_TYPE_ARENAREBORN && m_iBattleType != BATTLE_TYPE_CRSSVR )
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

	if (m_pPlayerModel)
	{
		//	If weapon changed, action index may be changed, so update here
		int iCurAction = (int)m_pPlayerModel->GetCurActionUserData(ACT_CHANNEL_BODY);
		if (IsValidAction(iCurAction))
			PlayAction(iCurAction, 1.0f, false);
	}

	return true;
}

//	Set loaded model to player object, this function is used in multithread loading process
bool CECElsePlayer::SetPlayerLoadedResult(EC_PLAYERLOADRESULT& Ret)
{
	if (!CECPlayer::SetPlayerLoadedResult(const_cast<EC_PLAYERLOADRESULT&>(Ret)))
		return false;

	if (IsFlying())
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

}

void CECElsePlayer::OnMsgPlayerBaseInfo(const ECMSG& Msg)
{
;
}


void CECElsePlayer::OnMsgPlayerBaseInfo2(const ECMSG& Msg)
{

}


void CECElsePlayer::OnMsgPlayerCustomData(const ECMSG& Msg)
{
}

void CECElsePlayer::OnMsgPlayerEquipData(const ECMSG& Msg)
{

}

void CECElsePlayer::OnMsgPlayerSitDown(const ECMSG& Msg)
{
	
}

void CECElsePlayer::OnMsgPlayerAtkResult(const ECMSG& Msg)
{
	
}

void CECElsePlayer::OnMsgPlayerStartAtk(const ECMSG& Msg)
{

}

void CECElsePlayer::OnMsgPlayerSkillResult(const ECMSG& Msg)
{

}

void CECElsePlayer::OnMsgPlayerCastSkill(const ECMSG& Msg)
{

}

void CECElsePlayer::OnMsgPlayerDoEmote(const ECMSG& Msg)
{
	
}

void CECElsePlayer::OnMsgPlayerUseItem(const ECMSG& Msg)
{

}

void CECElsePlayer::OnMsgPlayerUseItemWithData(const ECMSG& Msg)
{

}

void CECElsePlayer::OnMsgPlayerRoot(const ECMSG& Msg)
{

}

void CECElsePlayer::OnMsgPlayerDoAction(const ECMSG& Msg)
{

}

void CECElsePlayer::OnMsgPlayerInTeam(const ECMSG& Msg)
{

}

void CECElsePlayer::OnMsgPickupMatter(const ECMSG& Msg)
{

}

void CECElsePlayer::OnMsgPlayerGather(const ECMSG& Msg)
{

}

void CECElsePlayer::OnMsgDoConEmote(const ECMSG& Msg)
{

}

void CECElsePlayer::OnMsgPlayerChangeShape(const ECMSG& Msg)
{

}

void CECElsePlayer::OnMsgBoothOperation(const ECMSG& Msg)
{

}

void CECElsePlayer::OnMsgPlayerTravel(const ECMSG& Msg)
{
	
}

void CECElsePlayer::OnMsgPlayerChangeFace(const ECMSG& Msg)
{

}

void CECElsePlayer::OnMsgPlayerBindOpt(const ECMSG& Msg)
{

}

// 多人骑乘
void CECElsePlayer::OnMsgPlayerMultiBindOpt(const ECMSG& Msg)
{

}

void CECElsePlayer::OnMsgPlayerCarrierOpt(const ECMSG& Msg)
{
	
}

void CECElsePlayer::OnMsgPlayerDuelOpt(const ECMSG& Msg)
{

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
	default:
		ASSERT(0);
		break;
	}

	PlayAction(iAction, 1.0f);

	if (!bSession)
		PlayAction(GetMoveStandAction(false, false), 1.0f, true, 300, true);

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
			else if (m_pPlayerModel)
				m_pPlayerModel->StopChannelAction(ACT_CHANNEL_BODY, true);
		}
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
	}
}