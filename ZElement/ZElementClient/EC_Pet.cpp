/*
 * FILE: EC_Pet.cpp
 *
 * DESCRIPTION: 
 *
 * CREATED BY: Hedi, 2005/12/5
 *
 * HISTORY: 
 *
 * Copyright (c) 2004 Archosaur Studio, All Rights Reserved.
 */

#include "EC_Global.h"
#include "EC_Pet.h"
#include "EC_Game.h"
#include "EC_GameRun.h"
#include "EC_Model.h"
#include "EC_Utility.h"
#include "EC_PateText.h"
#include "EC_SceneLoader.h"
#include "elementdataman.h"
#include "EC_GPDataType.h"
#include "EC_ManPlayer.h"
#include "EC_World.h"
#include "EC_FixedMsg.h"
#include "A3DGFXEx.h"
#include "A3DGFXElement.h"
#include "EC_GFXCaster.h"
#include "EC_IvtrConsume.h"
#include "EC_HostPlayer.h"


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
//	Implement CECMonster
//	
///////////////////////////////////////////////////////////////////////////

CECPet::CECPet(CECNPCMan* pNPCMan) : CECNPC(pNPCMan)
{
	m_iCID				= OCID_PET;
	m_pDBEssence		= NULL;
	m_fTouchRad			= 1.0f;
	m_iShape			= 0;
	m_iFace				= 0;
	m_bIsFlying			= true;
	m_pGFXFlyOn			= 0;
}

CECPet::~CECPet()
{
}

//	Initlaize object
bool CECPet::Init(int tid, const S2C::info_npc& Info)
{
	if (!CECNPC::Init(tid, Info))
		return false;

	if (!(Info.state & GP_STATE_NPC_PET))
	{
		// must has pet flag set.
		return false;
	}

	//	Get database data
	elementdataman* pDB = g_pGame->GetElementDataMan();
	DATA_TYPE DataType;
	m_pDBEssence = (PET_BEDGE_ESSENCE*)pDB->get_data_ptr(tid, ID_SPACE_ESSENCE, DataType);

	if( m_pDBEssence )
	{
		SetUseGroundNormal(m_pDBEssence->stand_mode == 0 ? true : false);
		m_fTouchRad	= m_pDBEssence->size;
		m_BasicProps.iLevel = 1;
	}

	//	Submit EC model loading request to loading thread
	assert(m_dwBornStamp);

	m_iShape = Info.shape;
	m_iFace = Info.face;
	QueueECModelForLoad(MTL_ECM_NPC, Info.nid, m_dwBornStamp, GetServerPos(), g_pGame->GetDataPath(m_pDBEssence->file_to_shown[Info.shape-1]), NULL, tid);

	float fExt = m_fTouchRad * 1.5f;
	m_aabbDefPick.Extents.Set(fExt, fExt, fExt);

	//	If NPC doesn't have specific name, use the name in database
	if (!(Info.state & GP_STATE_NPC_NAME))
	{
		m_strName = m_pDBEssence->name;
		if (m_pPateName)
			m_pPateName->SetText(m_strName, false);
	}
	m_strDesc = m_pDBEssence->desc;
	if (m_pPateDesc && m_strDesc != _AL(""))
		m_pPateDesc->SetText(m_strDesc, false);
	A3DVECTOR3 vPos = Info.pos;
	SetPos(vPos);
	SetDirAndUp(glb_DecompressDirH(Info.dir), g_vAxisY);

	StartWork(WT_NOTHING, WORK_STAND);

	return true;
}

//	Release object
void CECPet::Release()
{
	if(m_pGFXFlyOn)
	{
		g_pGame->GetGFXCaster()->ReleaseGFXEx(m_pGFXFlyOn);
		m_pGFXFlyOn = NULL;
	}
	
	CECNPC::Release();
}

//	Set loaded model to NPC object, this function is used in multithread loading process
bool CECPet::SetNPCLoadedResult(const EC_NPCLOADRESULT& Ret)
{
	CECNPC::SetNPCLoadedResult(Ret);
	return true;
}

//	Build complete name
void CECPet::BuildCompleteName()
{
	const ACHAR* szMasterName = g_pGame->GetGameRun()->GetPlayerName(m_idMaster, false);
	if (szMasterName && m_pPateName)
	{
		if( m_strDesc != _AL(""))
			m_strCompName.Format(g_pGame->GetFixedMsgTab()->GetWideString(FIXMSG_WHOSETHING), szMasterName, m_strDesc);
		else 
			m_strCompName.Format(g_pGame->GetFixedMsgTab()->GetWideString(FIXMSG_WHOSETHING), szMasterName, m_strName);
		//m_pPateName->SetText(m_strCompName, false);
		m_pPateDesc->SetText(m_strCompName, false);
	}	
}

//	Tick routine
bool CECPet::Tick(DWORD dwDeltaTime)
{
	CECNPC::Tick(dwDeltaTime);

	if (!m_strCompName.GetLength())
	{
		//	Try to build complete name
		BuildCompleteName();
	}

	CECPlayerMan* pPlayerMan = g_pGame->GetGameRun()->GetWorld()->GetPlayerMan();
	CECPlayer* pPlayer = pPlayerMan->GetPlayer(m_idMaster);
	m_bIsFlying = pPlayer && pPlayer->IsFlying();
	if (m_bIsFlying)
	{
		// 模型加载完成再加载gfx
		if(!m_pGFXFlyOn && m_pNPCModel)
		{
			m_pGFXFlyOn		= g_pGame->GetGFXCaster()->LoadGFXEx("怪物\\通用\\圣诞鹿脚下.gfx");	// 暂时先这样写，问策划是否需要加到模板配置里面
			A3DAABB gfxAABB = m_pGFXFlyOn->GetAABBOrg();
			A3DAABB modelAABB = m_pNPCModel->GetModelAABB();
			if (gfxAABB.Extents.x<modelAABB.Extents.x || gfxAABB.Extents.z<modelAABB.Extents.z)
			{
				float fScaleX = modelAABB.Extents.x/gfxAABB.Extents.x;
				float fScaleZ = modelAABB.Extents.z/gfxAABB.Extents.z;
				m_pGFXFlyOn->SetScale(fScaleX>fScaleZ ? fScaleX : fScaleZ);
			}
		}

		if (m_pGFXFlyOn)
		{
			if (m_pGFXFlyOn->GetState()==ST_STOP)
			{
				m_pGFXFlyOn->Start(true);
			}
			m_pGFXFlyOn->SetPos(GetPos());
		}
	}
	else
	{
		if (m_pGFXFlyOn && m_pGFXFlyOn->GetState()!=ST_STOP)
		{
			m_pGFXFlyOn->Stop();
		}
	}

	return true;
}

//	Render routine
bool CECPet::Render(CECViewport* pViewport, int iRenderFlag/* 0 */)
{
	if(g_pGame->GetGameRun()->IsHideElsePlayer())
		return true;
	CECNPC::Render(pViewport, iRenderFlag);
	return true;
}

//	Get NPC name color
DWORD CECPet::GetNameColor()
{
	CECPlayerMan* pPlayerMan = g_pGame->GetGameRun()->GetWorld()->GetPlayerMan();
	CECPlayer* pPlayer = pPlayerMan->GetPlayer(m_idMaster);
	if (!pPlayer)
		return CECNPC::GetNameColor();

	return pPlayer->GetNameColor();
}

float CECPet::GetSize()
{
	return m_pDBEssence->size;
}

void CECPet::SetPetProp(DWORD param)
{
	using namespace S2C;
	player_get_summon_petprop* pCmd = (player_get_summon_petprop*)param;
	CECPetBedge *pPet = (CECPetBedge*)g_pGame->GetGameRun()->GetHostPlayer()->GetPetPack()->GetItem(pCmd->pet_index);
	if(!pPet)
		return;
	pPet->GetEssence().attr[itemdataman::_pet_bedge_essence::MAX_HP].cur_value = pCmd->prop.max_hp;
	pPet->GetEssence().attr[itemdataman::_pet_bedge_essence::MAX_VIGOR].cur_value  = pCmd->prop.max_mp;
	pPet->GetEssence().crit_rate = pCmd->prop.crit_rate/1000.0f;
	pPet->GetEssence().crit_damage = pCmd->prop.crit_damage;
	pPet->GetEssence().attr[itemdataman::_pet_bedge_essence::MIN_ATTACK].cur_value  = pCmd->prop.damage_low;
	pPet->GetEssence().attr[itemdataman::_pet_bedge_essence::MAX_ATTACK].cur_value  = pCmd->prop.damage_high;
	pPet->GetEssence().attr[itemdataman::_pet_bedge_essence::HIT].cur_value  = pCmd->prop.attack;
	pPet->GetEssence().attr[itemdataman::_pet_bedge_essence::DEFENCE].cur_value  = pCmd->prop.defense;
	pPet->GetEssence().attr[itemdataman::_pet_bedge_essence::JOUK].cur_value  = pCmd->prop.armor;
	pPet->GetEssence().anti_crit_rate = pCmd->prop.anti_crit/1000.0f;
	pPet->GetEssence().anti_crit_damage = pCmd->prop.anti_crit_damage;
	pPet->GetEssence().skill_attack_rate = pCmd->prop.skill_attack_rate/1000.0f;
	pPet->GetEssence().skill_armor_rate = pCmd->prop.skill_armor_rate/1000.0f;
	pPet->GetEssence().attr[itemdataman::_pet_bedge_essence::RESISTANCE1].cur_value  = pCmd->prop.resistance[0];
	pPet->GetEssence().attr[itemdataman::_pet_bedge_essence::RESISTANCE2].cur_value  = pCmd->prop.resistance[1];
	pPet->GetEssence().attr[itemdataman::_pet_bedge_essence::RESISTANCE3].cur_value  = pCmd->prop.resistance[2];
	pPet->GetEssence().attr[itemdataman::_pet_bedge_essence::RESISTANCE4].cur_value  = pCmd->prop.resistance[3];
	pPet->GetEssence().attr[itemdataman::_pet_bedge_essence::RESISTANCE5].cur_value  = pCmd->prop.resistance[4];
	pPet->GetEssence().attr[itemdataman::_pet_bedge_essence::RESISTANCE6].cur_value  = pCmd->prop.resistance[5];
}


