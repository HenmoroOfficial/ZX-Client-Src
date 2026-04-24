/*
 * FILE: EC_Monster.cpp
 *
 * DESCRIPTION: 
 *
 * CREATED BY: Duyuxin, 2004/9/22
 *
 * HISTORY: 
 *
 * Copyright (c) 2004 Archosaur Studio, All Rights Reserved.
 */

#include "EC_Global.h"
#include "EC_Monster.h"
#include "EC_Game.h"
#include "EC_Model.h"
#include "EC_Utility.h"
#include "EC_PateText.h"
#include "EC_SceneLoader.h"
#include "EC_HostPlayer.h"
#include "EC_GameRun.h"
#include "EC_ManPlayer.h"
#include "EC_World.h"
#include "EC_FixedMsg.h"
#include "EC_GPDataType.h"
#include "EC_CDR.h"
#include "elementdataman.h"

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

CECMonster::CECMonster(CECNPCMan* pNPCMan) : CECNPC(pNPCMan)
{
	m_iCID			= OCID_MONSTER;
	m_pDBEssence	= NULL;
}

CECMonster::~CECMonster()
{
}

//	Initlaize object
bool CECMonster::Init(int tid, const S2C::info_npc& Info)
{
	if (!CECNPC::Init(tid, Info))
		return false;

	//	Get database data
	elementdataman* pDB = g_pGame->GetElementDataMan();
	DATA_TYPE DataType;
	m_pDBEssence = (MONSTER_ESSENCE*)pDB->get_data_ptr(tid, ID_SPACE_ESSENCE, DataType);
	m_pMonEssence = m_pDBEssence;

	SetUseGroundNormal(m_pDBEssence->stand_mode == 0 ? true : false);
	m_fTouchRad	= m_pDBEssence->size;
	m_BasicProps.iLevel = m_pDBEssence->level;

	//	Submit EC model loading request to loading thread
	QueueECModelForLoad(MTL_ECM_NPC, Info.nid, GetBornStamp(), GetServerPos(), 
		g_pGame->GetDataPath(m_pDBEssence->file_model), NULL, tid);

	float fExt = m_fTouchRad * 1.5f;
	m_aabbDefPick.Extents.Set(fExt, fExt, fExt);
	m_pcdr->vExts = m_aabbDefPick.Extents;

	//	If NPC doesn't have specific name, use the name in database
	if (!(Info.state & GP_STATE_NPC_NAME))
	{
		m_strName = m_pDBEssence->name;
		if (m_pPateName)
			m_pPateName->SetText(m_strName, false);
	}
	m_strDesc = m_pDBEssence->monster_desc;
	if (m_pPateDesc && m_strDesc != _AL(""))
		m_pPateDesc->SetText(m_strDesc, false);

	A3DVECTOR3 vPos = Info.pos;
	SetPos(vPos);
	SetDirAndUp(glb_DecompressDirH(Info.dir), g_vAxisY);

	StartWork(WT_NOTHING, WORK_STAND);

	return true;
}

//	Release object
void CECMonster::Release()
{
	CECNPC::Release();
}

//	Set loaded model to NPC object, this function is used in multithread loading process
bool CECMonster::SetNPCLoadedResult(const EC_NPCLOADRESULT& Ret)
{
	CECNPC::SetNPCLoadedResult(Ret);
	return true;
}

//	Tick routine
bool CECMonster::Tick(DWORD dwDeltaTime)
{
	CECNPC::Tick(dwDeltaTime);
	if(m_idSummoner && m_strCompName.GetLength() == 0)
	{
		BuildCompleteName();
	}
	return true;
}

//	Render routine
bool CECMonster::Render(CECViewport* pViewport, int iRenderFlag/* 0 */)
{
	CECNPC::Render(pViewport, iRenderFlag);
	return true;
}

//	Build complete name
void CECMonster::BuildCompleteName()
{
	const ACHAR* szMasterName = g_pGame->GetGameRun()->GetPlayerName(m_idSummoner, false);
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
//	Get NPC name color
DWORD CECMonster::GetNameColor()
{
	DWORD dwNameCol = A3DCOLORRGB(255, 192, 0);
	if (IsInBattleInvaderCamp())
	{
		//	In invader camp in battle
		dwNameCol = NAMECOL_BC_RED;
	}
	else if (IsInBattleDefenderCamp())
	{
		//	In defender camp in battle
		dwNameCol = NAMECOL_BC_BLUE;
	}
	else if (m_idSummoner)
	{
		CECPlayerMan* pPlayerMan = g_pGame->GetGameRun()->GetWorld()->GetPlayerMan();
		CECPlayer* pPlayer = pPlayerMan->GetPlayer(m_idSummoner);
		if (!pPlayer)
			dwNameCol = CECNPC::GetNameColor();
		else
			dwNameCol =  pPlayer->GetNameColor();	
	}
	else
	{
		switch (m_pDBEssence->name_color)
		{
		case 0: {
			CECHostPlayer* pHost = g_pGame->GetGameRun()->GetHostPlayer();
			dwNameCol = pHost->DecideTargetNameCol(m_BasicProps.iLevel);
			break; }
		case 1:
			dwNameCol = 0xffec870e;
			break;
		case 2:
			dwNameCol = 0xffd9d919;
			break;
		case 3:
			dwNameCol = 0xff945305;
			break;
		case 4:
			dwNameCol = 0xff6ec3c9;
			break;
		case 5:
			dwNameCol = 0xff9370db;
			break;
		case 6:
			dwNameCol = 0xffff1cae;
			break;
		case 7:
			dwNameCol = 0xff3232cd;
			break;
		}
	}

	return dwNameCol;
}

//	Is monster in invader camp in battle ?
bool CECMonster::IsInBattleInvaderCamp()
{ 
	return (m_pDBEssence && (m_pDBEssence->faction & ( (1<<4) | (1<<6) | (1 << 10) )) ) ? true : false; 
}

//	Is monster in defender camp in battle ?
bool CECMonster::IsInBattleDefenderCamp()
{ 
	return (m_pDBEssence && (m_pDBEssence->faction & ( (1 << 5) | (1<<7) | (1<<11) )) ) ? true : false; 
}

bool CECMonster::IsHumanCamp()
{
	return (m_pDBEssence && (m_pDBEssence->faction & (1<<16) )) ? true : false;
}

bool CECMonster::IsOrcCamp()
{
	return (m_pDBEssence && (m_pDBEssence->faction & (1<<17) )) ? true : false;
}

float CECMonster::GetSize()
{
	return m_pDBEssence->size;
}
