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
//#include "EC_PateText.h"
#include "EC_SceneLoader.h"
#include "elementdataman.h"
#include "EC_GPDataType.h"
#include "EC_ManPlayer.h"
#include "EC_World.h"
//#include "EC_FixedMsg.h"

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

	QueueECModelForLoad(MTL_ECM_NPC, Info.nid, m_dwBornStamp, GetServerPos(), g_pGame->GetDataPath(m_pDBEssence->file_to_shown[Info.shape-1]), NULL, tid);

	float fExt = m_fTouchRad * 1.5f;
	m_aabbDefPick.Extents.Set(fExt, fExt, fExt);

	//	If NPC doesn't have specific name, use the name in database
	if (!(Info.state & GP_STATE_NPC_NAME))
	{
		m_strName = m_pDBEssence->name;
	}
	m_strDesc = m_pDBEssence->desc;
	A3DVECTOR3 vPos = Info.pos;
	SetPos(vPos);
	SetDirAndUp(glb_DecompressDirH(Info.dir), g_vAxisY);

	StartWork(WT_NOTHING, WORK_STAND);

	return true;
}

//	Release object
void CECPet::Release()
{
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
