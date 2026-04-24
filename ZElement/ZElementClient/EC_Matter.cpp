/*
 * FILE: EC_Matter.cpp
 *
 * DESCRIPTION: 
 *
 * CREATED BY: Duyuxin, 2004/9/9
 *
 * HISTORY: 
 *
 * Copyright (c) 2004 Archosaur Studio, All Rights Reserved.
 */

#include "EC_Global.h"
#include "EC_Game.h"
#include "EC_Matter.h"
#include "EC_GameRun.h"
#include "EC_GFXCaster.h"
#include "EC_World.h"
#include "EC_HostPlayer.h"
#include "EC_Resource.h"
#include "EC_Model.h"
#include "EC_PateText.h"
#include "EC_Configs.h"
#include "EC_SceneLoader.h"
#include "elementdataman.h"
#include "AUI/AUILuaManager.h"
#include "EC_UIManager.h"
#include "EC_CDR.h"
#include "EC_GameRecord.h"
#include "EC_Utility.h"
#include "EC_GameSession.h"
#include "EC_FixedMsg.h"
#include "EC_NPC.h"

#include "A3DGFXEx.h"
#include "A3DGFXExMan.h"
#include "A3DTerrain2.h"
#include "A3DLight.h"
#include "A3DCamera.h"
#include "A3DViewport.h"
#include "A3DSkinMan.h"
#include "A3DCollision.h"
#include <AFI.h>

#ifdef BACK_VERSION
#include "EC_Viewport.h"
#else
#include "ECViewport.h"
#endif

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

//	Load matter model
CECModel* CECMatter::LoadMatterModel(const char* szFile)
{
	g_pGame->GetA3DEngine()->GetA3DSkinMan()->SetReplaceShaderFile((const char*)glb_ReplaceShader, A3DSkinMan::SHADERREPLACE_USERDEFINE);

	CECModel* pModel = new CECModel;
	if (!pModel)
	{
		g_pGame->GetA3DEngine()->GetA3DSkinMan()->SetReplaceShaderFile("", 0);
		return NULL;
	}

	if (!pModel->Load(szFile))
	{
		a_LogOutput(1, "CECMatter::LoadMatterModel, Failed to load matter model %s", szFile);

		if (!pModel->Load("models\\error\\error.ecm"))
		{
			g_pGame->GetA3DEngine()->GetA3DSkinMan()->SetReplaceShaderFile("", 0);
			delete pModel;
			return NULL;
		}
	}

	g_pGame->GetA3DEngine()->GetA3DSkinMan()->SetReplaceShaderFile("", 0);
	return pModel;
}

//	Release matter model
void CECMatter::ReleaseMatterModel(CECModel* pModel)
{
	if (pModel)
	{
		QueueECModelForRelease(pModel);
	//	pModel->Release();
	//	delete pModel;
	}
}

///////////////////////////////////////////////////////////////////////////
//	
//	Implement CECMatter
//	
///////////////////////////////////////////////////////////////////////////

CECMatter::CECMatter(CECMatterMan* pMatterMan)
{
	m_iCID			= OCID_MATTER;
	m_pMatterMan	= pMatterMan;
	m_pModel		= NULL;
	m_pGfx			= NULL;
	m_szModelFile	= NULL;
	m_szName		= NULL;
	m_fDistToHost	= 0.0f;
	m_fDistToCamera	= 0.0f;
	m_fDistToHostH	= 0.0f;
	m_bRenderName	= false;
	m_bDynModelObj	= false;
	m_bBattleFlag	= false;
	m_pPateName		= NULL;
	m_pPateOwner	= NULL;
	m_dwMatterType	= 0;
	m_iLevelReq		= 0;
	m_nBrushes		= 0;
	m_ppBrushes		= NULL;
	m_bSpecialNameItem = false;
	m_cOwnerType	= CECNPC::OWNER_NONE;
	m_idOwner		= 0;
	m_bPhase		= false;
	m_iPhaseId		= 0;
	m_bStopCollosionDetection = false;
	m_bFirstCDCheck	= false;
	m_iMineStateID	= 0;
	
	if ((m_pPateName = new CECPateText))
		m_pPateName->EnableBorder(true);

	m_aabb.Center	= g_vOrigin;
	m_aabb.Extents.Set(0.3f, 0.3f, 0.3f);
	m_aabb.CompleteMinsMaxs();

	memset(&m_MatterInfo, 0, sizeof (m_MatterInfo));
}

CECMatter::~CECMatter()
{
}

//	Initialize object
bool CECMatter::Init(const S2C::info_matter& Info)
{
	m_MatterInfo.mid	= Info.mid;
	m_MatterInfo.tid	= Info.tid & 0x0000ffff;
	m_MatterInfo.dropper_id = Info.dropper_id;
	m_MatterInfo.dir0	= Info.dir0;
	m_MatterInfo.dir1	= Info.dir1;
	m_MatterInfo.rad	= Info.rad;
	m_iMineStateID		= Info.mine_state;

	switch (Info.state)
	{
	case 1:
		m_bDynModelObj = true;
		break;
	case 2:
		m_bBattleFlag = true;
		m_cBattleFlagState = Info.flag;
		break;
	}
	if (Info.state & GP_STATE_MATTER_OWNER)
	{
		m_cOwnerType = Info.owner_type;
		m_idOwner	 = Info.owner_id;
	}
	//	Read data from database
	if (!m_bDynModelObj)
		ReadDataFromDatabase(m_MatterInfo.tid);

	if (Info.tid & 0x80000000)
		m_dwMatterType |= ITEMFLAG_EXTPROP;

	//	Submit matter model loading request to loading thread
	if (m_bDynModelObj)
	{
		const char* szFile = g_pGame->GetDynObjectPath(Info.tid);

		if (af_CheckFileExt(szFile, ".ecm"))
			QueueECModelForLoad(MTL_ECM_MATTER, Info.mid, 0, Info.pos, szFile, NULL);
		else if (af_CheckFileExt(szFile, ".gfx"))
			LoadGFXFromFile(szFile, 0);
	}
	else
		QueueECModelForLoad(MTL_ECM_MATTER, Info.mid, 0, Info.pos, m_szModelFile, NULL);

	//	Ensure matter won't be dropped underground
	A3DVECTOR3 vPos, vNormal;
	if (VertRayTrace(Info.pos + g_vAxisY * 0.2f, vPos, vNormal, 10.0f))
	{
		if (vPos.y < Info.pos.y)
			vPos.y = Info.pos.y;
	}
	else
		vPos = Info.pos;

	A3DVECTOR3 vAxis = a3d_DecompressDir(Info.dir0, Info.dir1);
	A3DQUATERNION q(vAxis, Info.rad / 255.0f * A3D_2PI);
	A3DMATRIX4 matTran;
	q.ConvertToMatrix(matTran);

	SetPos(vPos);
	SetDirAndUp(matTran.GetRow(2), matTran.GetRow(1));

	//-------------------------------------------------------------------------
	m_bFirstCDCheck	= true;
	//-------------------------------------------------------------------------

	return true;
}

//-----------------------------------------------------------------------------
bool CECMatter::CheckCollosion()
{
	CECHostPlayer* pHost = g_pGame->GetGameRun()->GetHostPlayer();
	if ( pHost && pHost->IsSkeletonReady() )
	{
		A3DAABB aabb = pHost->GetPlayerAABB();
//		return CLS_AABBAABBOverlap(m_aabb.Center, m_aabb.Extents, aabb.Center, aabb.Extents);
		BrushTraceInfo  brushInfo;
		brushInfo.Init( aabb.Center, A3DVECTOR3(0.01f, 0.01f, 0.01f), aabb.Extents );
		return TraceWithBrush( &brushInfo );
	}
	//
	return false;
}

bool CECMatter::NeedCheckCollosion()
{
	DATA_TYPE DataType;
	const void* pDataPtr = g_pGame->GetElementDataMan()->get_data_ptr(GetTemplateID(), ID_SPACE_ESSENCE, DataType);
	if ( DataType == DT_MINE_ESSENCE )
	{
		const MINE_ESSENCE* pMinEssence = (const MINE_ESSENCE*)pDataPtr;
		if ( pMinEssence->close_collision_detection )
		{
			return true;
		}
	}

	//
	return false;
}
//-----------------------------------------------------------------------------

GNET::Octets CECMatter::gen_matter_info()
{
	GNET::Octets o;
	add_to_octets<int>(o, m_MatterInfo.mid);
	add_to_octets<int>(o, m_MatterInfo.tid);
	add_to_octets<int>(o, m_MatterInfo.dropper_id);
	add_to_octets<A3DVECTOR3>(o, GetPos());
	add_to_octets<unsigned char>(o, m_MatterInfo.dir0);
	add_to_octets<unsigned char>(o, m_MatterInfo.dir1);
	add_to_octets<unsigned char>(o, m_MatterInfo.rad);
	unsigned char state = 0;
	unsigned char flag = 0;

	if (m_bDynModelObj)
		state = 1;
	else if (m_bBattleFlag)
	{
		state = 2;
		flag = m_cBattleFlagState;
	}
	if (m_cOwnerType != CECNPC::OWNER_NONE)
	{
		state |= GP_STATE_MATTER_OWNER;
	}
	add_to_octets<unsigned char>(o, state);
	add_to_octets<unsigned char>(o, flag);
	if (state & GP_STATE_MATTER_OWNER)
	{
		add_to_octets<unsigned char>(o, m_cOwnerType);
		add_to_octets<int>(o, m_idOwner);
	}
	
	if(m_bPhase)
	{
		state |= GP_STATE_MATTER_PHASE;
		add_to_octets<short>(o, m_iPhaseId);
	}

	if(m_iMineStateID != 0)
	{
		state |= GP_STATE_COMBINE_MINE;
		add_to_octets<int>(o, m_iMineStateID);
	}

	if(state)
	return o;
}

//	Release object
void CECMatter::Release()
{
	//	Release model
	if (m_pModel)
	{
		ReleaseMatterModel(m_pModel);
		m_pModel = NULL;
	}

	//	Release GFX
	A3DRELEASE(m_pGfx);

	if (m_pPateName)
	{
		delete m_pPateName;
		m_pPateName = NULL;
	}
	if (m_pPateOwner)
	{
		delete m_pPateOwner;
		m_pPateOwner = NULL;
	}
	if (m_ppBrushes)
	{
		for (int i = 0; i < m_nBrushes; i++)
			delete m_ppBrushes[i];

		delete[] m_ppBrushes;
		m_ppBrushes = NULL;
	}

	m_nBrushes = 0;
}

//	Set loaded model to matter object, this function is used in multithread loading process
bool CECMatter::SetLoadedMatterModel(CECModel* pModel)
{
	if (m_pModel)
	{
		//	This case is seldom happed, but we have to handle it. See
		//	CECNPC::SetNPCLoadedResult() for detail information.
		ReleaseMatterModel(m_pModel);
	}

	m_pModel = pModel;

	// now build ch data if needed;
	m_nBrushes = 0;
	m_ppBrushes = NULL;
	// now construct a CCDBrush from CConvexhullData;
	if( m_pModel && m_pModel->HasCHAABB() )
	{
		ConvexHullDataArray& cdArray = m_pModel->GetConvexHullData();

		A3DMATRIX4 matTM = TransformMatrix(GetDir(), GetUp(), GetPos());
		if( int(cdArray.size()) != m_nBrushes )
		{
			m_nBrushes = cdArray.size();
			m_ppBrushes = new CCDBrush *[cdArray.size()];

			for(int i=0; i<m_nBrushes; i++)
			{
				CConvexHullData chData = *cdArray[i];
				chData.Transform(matTM);

				CQBrush qBrush;
				qBrush.AddBrushBevels(&chData);
				CCDBrush * pCDBrush = new CCDBrush();
				qBrush.Export(pCDBrush);

				m_ppBrushes[i] = pCDBrush;
			}
		}
	}

	if (m_bBattleFlag)
		BattleFlagChange(m_cBattleFlagState, m_cBattleFlagState);
	
	if (!m_bDynModelObj && !m_bBattleFlag && m_pModel->GetA3DSkinModel())
	{
		//	Select a action according to metter's environment
		AString strAct;
		A3DVECTOR3 vPos = GetPos();
		CECWorld* pWorld = g_pGame->GetGameRun()->GetWorld();

		/*
		if (vPos.y < pWorld->GetWaterHeight(vPos) - 0.2f)
			strAct = "掉落保护_水中";
		else if (vPos.y > pWorld->GetTerrainHeight(vPos, NULL) + 0.2f)
			strAct = "掉落保护_空中";
		else
		*/
		strAct = "掉落保护";

		m_pModel->PlayActionByName(strAct, 1.0f);
		//	Force to use mesh aabb
		m_pModel->SetAABBType(CECModel::AABB_INITMESH);
	}

	SetPos(GetPos());
	SetDirAndUp(GetDir(), GetUp());

	m_pModel->SetAutoUpdateFlag(false);
	m_pModel->Tick(1);
	m_pModel->SetAutoUpdateFlag(true);

	if (m_pModel->GetA3DSkinModel())
	{
		//	Set matter's aabb and limit it to a proper extent
		m_aabb.Extents = m_pModel->GetA3DSkinModel()->GetModelAABB().Extents;
		a_Clamp(m_aabb.Extents.x, 0.3f, 1.0f);
		a_Clamp(m_aabb.Extents.y, 0.3f, 1.0f);
		a_Clamp(m_aabb.Extents.z, 0.3f, 1.0f);
		m_aabb.CompleteMinsMaxs();
	}

	return true;
}

//	Process message
bool CECMatter::ProcessMessage(const ECMSG& Msg) 
{ 
	return true; 
}

//	Read matter data from database
bool CECMatter::ReadDataFromDatabase(int tid)
{
	if (!tid)
	{
		ASSERT(0);
		return true;
	}

	//	Set default value
	m_dwMatterType = (tid == 3044) ? MATTER_MONEY : MATTER_ITEM;

	DATA_TYPE DataType;
	const void* pDataPtr = g_pGame->GetElementDataMan()->get_data_ptr(tid, ID_SPACE_ESSENCE, DataType);
	int iFixProps = 0;
	DWORD idPath = 0;

	switch (DataType)
	{
	case DT_EQUIPMENT_ESSENCE:
	{
		const EQUIPMENT_ESSENCE* pData = (const EQUIPMENT_ESSENCE*)pDataPtr;
		m_szName	= pData->name;
		idPath		= pData->file_matter;
		break;
	}
	case DT_TALISMAN_MAINPART_ESSENCE:
	{
		const TALISMAN_MAINPART_ESSENCE* pData = (const TALISMAN_MAINPART_ESSENCE*)pDataPtr;
		m_szName	= pData->name;
		idPath		= pData->file_matter;
		break;
	}
	case DT_MEDICINE_ESSENCE:
	{
		const MEDICINE_ESSENCE* pData = (const MEDICINE_ESSENCE*)pDataPtr;
		m_szName	= pData->name;
		idPath		= pData->file_matter;
		break;
	}
	case DT_MATERIAL_ESSENCE:
	{
		const MATERIAL_ESSENCE* pData = (const MATERIAL_ESSENCE*)pDataPtr;
		m_szName	= pData->name;
		idPath		= pData->file_matter;
		break;
	}
	case DT_SKILLTOME_ESSENCE:
	{
		const SKILLTOME_ESSENCE* pData = (const SKILLTOME_ESSENCE*)pDataPtr;
		m_szName	= pData->name;
		idPath		= pData->file_matter;
		break;
	}
	case DT_TOWNSCROLL_ESSENCE:
	{
		const TOWNSCROLL_ESSENCE* pData = (const TOWNSCROLL_ESSENCE*)pDataPtr;
		m_szName	= pData->name;
		idPath		= pData->file_matter;
		break;
	}
	case DT_REVIVESCROLL_ESSENCE:
	{
		const REVIVESCROLL_ESSENCE* pData = (const REVIVESCROLL_ESSENCE*)pDataPtr;
		m_szName	= pData->name;
		idPath		= pData->file_matter;
		break;
	}
	case DT_TASKDICE_ESSENCE:
	{
		const TASKDICE_ESSENCE* pData = (const TASKDICE_ESSENCE*)pDataPtr;
		m_szName	= pData->name;
		idPath		= pData->file_matter;
		break;
	}
	case DT_TASKNORMALMATTER_ESSENCE:
	{
		const TASKNORMALMATTER_ESSENCE* pData = (const TASKNORMALMATTER_ESSENCE*)pDataPtr;
		m_szName	= pData->name;
		idPath		= pData->file_matter;
		break;
	}
	case DT_MINE_ESSENCE:
	{
		const MINE_ESSENCE* pData = (const MINE_ESSENCE*)pDataPtr;
		m_szName	= pData->name;
		idPath		= pData->file_model;
		m_dwMatterType	= MATTER_MINE;
		m_iLevelReq		= pData->level_required;
		if (m_cOwnerType != CECNPC::OWNER_NONE && m_idOwner)
			m_pPateOwner = new CECPateText;
		break;
	}
	case DT_COMPOUND_MINE_ESSENCE:
	{
		const COMPOUND_MINE_ESSENCE* pData = (const COMPOUND_MINE_ESSENCE*)pDataPtr;

		if (m_iMineStateID != pData->file_mine[0] && m_iMineStateID != pData->file_mine[1]
			&& m_iMineStateID != pData->file_mine[2] && m_iMineStateID != pData->file_mine_spe)
			break;

		DataType = DT_INVALID;
		MINE_ESSENCE* pMineData = (MINE_ESSENCE*) g_pGame->GetElementDataMan()->get_data_ptr(m_iMineStateID, ID_SPACE_ESSENCE, DataType);
		if (DataType != DT_MINE_ESSENCE)
			break;

		m_szName		= pData->name;
		m_dwMatterType	= MATTER_MINE;
		idPath			= pMineData->file_model;
		m_iLevelReq		= pMineData->level_required;
		if (m_cOwnerType != CECNPC::OWNER_NONE && m_idOwner && m_pPateOwner == NULL)
			m_pPateOwner = new CECPateText;

		break;
	}
	case DT_GM_GENERATOR_ESSENCE:
	{
		const GM_GENERATOR_ESSENCE* pData = (const GM_GENERATOR_ESSENCE*)pDataPtr;
		m_szName	= pData->name;
		idPath		= pData->file_matter;
		break;
	}
	case DT_FIREWORKS_ESSENCE:
	{
		const FIREWORKS_ESSENCE* pData = (const FIREWORKS_ESSENCE*)pDataPtr;
		m_szName	= pData->name;
		idPath		= pData->file_matter;
		break;
	}
	case DT_TEXT_FIREWORKS_ESSENCE:
	{
		const TEXT_FIREWORKS_ESSENCE* pData = (const TEXT_FIREWORKS_ESSENCE*)pDataPtr;
		m_szName	= pData->name;
		idPath		= pData->file_matter;
		break;
	}
	case DT_TRANSMITROLL_ESSENCE:
	{
		const TRANSMITROLL_ESSENCE* pData = (const TRANSMITROLL_ESSENCE*)pDataPtr;
		m_szName	= pData->name;
		idPath		= pData->file_matter;
		break;
	}
	case DT_RECIPEROLL_ESSENCE:
	{
		const RECIPEROLL_ESSENCE* pData = (const RECIPEROLL_ESSENCE*)pDataPtr;
		m_szName	= pData->name;
		idPath		= pData->file_matter;
		break;
	}
	case DT_REFINE_TICKET_ESSENCE:
	{
		const REFINE_TICKET_ESSENCE* pData = (const REFINE_TICKET_ESSENCE*)pDataPtr;
		m_szName	= pData->name;
		idPath		= pData->file_matter;
		break;
	}
	case DT_OFFLINE_TRUSTEE_ESSENCE:
	{
		const OFFLINE_TRUSTEE_ESSENCE* pData = (const OFFLINE_TRUSTEE_ESSENCE*)pDataPtr;
		m_szName	= pData->name;
		idPath		= pData->file_matter;
		break;
	}	
	case DT_LUCKYROLL_ESSENCE:
	{
		const LUCKYROLL_ESSENCE* pData = (const LUCKYROLL_ESSENCE*)pDataPtr;
		m_szName	= pData->name;
		idPath		= pData->file_matter;
		break;
	}
	case DT_ESTONE_ESSENCE:
	{
		const ESTONE_ESSENCE* pData = (const ESTONE_ESSENCE*)pDataPtr;
		m_szName	= pData->name;
		idPath		= pData->file_matter;
		break;
	}
	case DT_PSTONE_ESSENCE:
	{
		const PSTONE_ESSENCE* pData = (const PSTONE_ESSENCE*)pDataPtr;
		m_szName	= pData->name;
		idPath		= pData->file_matter;
		break;
	}
	case DT_SSTONE_ESSENCE:
	{
		const SSTONE_ESSENCE* pData = (const SSTONE_ESSENCE*)pDataPtr;
		m_szName	= pData->name;
		idPath		= pData->file_matter;
		break;
	}
	case DT_DOUBLE_EXP_ESSENCE:
	{
		const DOUBLE_EXP_ESSENCE* pData = (const DOUBLE_EXP_ESSENCE*)pDataPtr;
		m_szName	= pData->name;
		idPath		= pData->file_matter;
		break;
	}
	case DT_DESTROYING_ESSENCE:
	{
		const DESTROYING_ESSENCE* pData = (const DESTROYING_ESSENCE*)pDataPtr;
		m_szName		= pData->name;
		idPath			= pData->file_matter;
		break;
	}
	case DT_SKILLMATTER_ESSENCE:
	{
		const SKILLMATTER_ESSENCE* pData = (const SKILLMATTER_ESSENCE*)pDataPtr;
		m_szName		= pData->name;
		idPath			= pData->file_matter;
		break;
	}
	case DT_VEHICLE_ESSENCE:
	{
		const VEHICLE_ESSENCE* pData = (const VEHICLE_ESSENCE*)pDataPtr;
		m_szName		= pData->name;
		idPath			= pData->file_matter;
		break;
	}
	case DT_COUPLE_JUMPTO_ESSENCE:
	{
		const COUPLE_JUMPTO_ESSENCE* pData = (const COUPLE_JUMPTO_ESSENCE*)pDataPtr;
		m_szName		= pData->name;
		idPath			= pData->file_matter;
		break;
	}
	case DT_LOTTERY_ESSENCE:
	{
		const LOTTERY_ESSENCE* pData = (const LOTTERY_ESSENCE*)pDataPtr;
		m_szName		= pData->name;
		idPath			= pData->file_matter;
		break;
	}
	case DT_CAMRECORDER_ESSENCE:
	{
		const CAMRECORDER_ESSENCE* pData = (const CAMRECORDER_ESSENCE*)pDataPtr;
		m_szName		= pData->name;
		idPath			= pData->file_matter;
		break;
	}
	case DT_TALISMAN_EXPFOOD_ESSENCE:
	{
		const TALISMAN_EXPFOOD_ESSENCE* pData = (const TALISMAN_EXPFOOD_ESSENCE*)pDataPtr;
		m_szName		= pData->name;
		idPath			= pData->file_matter;
		break;
	}
	case DT_TALISMAN_MERGEKATALYST_ESSENCE:
	{
		const TALISMAN_MERGEKATALYST_ESSENCE* pData = (const TALISMAN_MERGEKATALYST_ESSENCE*)pDataPtr;
		m_szName		= pData->name;
		idPath			= pData->file_matter;
		break;
	}
	case DT_TALISMAN_ENERGYFOOD_ESSENCE:
	{
		const TALISMAN_ENERGYFOOD_ESSENCE* pData = (const TALISMAN_ENERGYFOOD_ESSENCE*)pDataPtr;
		m_szName		= pData->name;
		idPath			= pData->file_matter;
		break;
	}
	case DT_SPEAKER_ESSENCE:
	{
		const SPEAKER_ESSENCE* pData = (const SPEAKER_ESSENCE*)pDataPtr;
		m_szName		= pData->name;
		idPath			= pData->file_matter;
		break;
	}
	case DT_BOOK_ESSENCE:
	{
		const BOOK_ESSENCE* pData = (const BOOK_ESSENCE*)pDataPtr;
		m_szName		= pData->name;
		idPath			= pData->file_matter;
		break;
	}	
	case DT_SIEGE_ARTILLERY_SCROLL_ESSENCE:
	{
		const SIEGE_ARTILLERY_SCROLL_ESSENCE* pData = (const SIEGE_ARTILLERY_SCROLL_ESSENCE*)pDataPtr;
		m_szName		= pData->name;
		idPath			= pData->file_matter;
		break;
	}
	case DT_PET_BEDGE_ESSENCE:
	{
		const PET_BEDGE_ESSENCE* pData = (const PET_BEDGE_ESSENCE*)pDataPtr;
		m_szName		= pData->name;
		idPath			= pData->file_matter;
		break;
	}
	case DT_PET_FOOD_ESSENCE:
	{
		const PET_FOOD_ESSENCE* pData = (const PET_FOOD_ESSENCE*)pDataPtr;
		m_szName		= pData->name;
		idPath			= pData->file_matter;
		break;
	}
	case DT_PET_SKILL_ESSENCE:
	{
		const PET_SKILL_ESSENCE* pData = (const PET_SKILL_ESSENCE*)pDataPtr;
		m_szName		= pData->name;
		idPath			= pData->file_matter;
		break;
	}
	case DT_PET_ARMOR_ESSENCE:
	{
		const PET_ARMOR_ESSENCE* pData = (const PET_ARMOR_ESSENCE*)pDataPtr;
		m_szName		= pData->name;
		idPath			= pData->file_matter;
		break;
	}
	case DT_PET_AUTO_FOOD_ESSENCE:
	{
		const PET_AUTO_FOOD_ESSENCE* pData = (const PET_AUTO_FOOD_ESSENCE*)pDataPtr;
		m_szName		= pData->name;
		idPath			= pData->file_matter;
		break;
	}
	case DT_PET_REFINE_ESSENCE:
	{
		const PET_REFINE_ESSENCE* pData = (const PET_REFINE_ESSENCE*)pDataPtr;
		m_szName		= pData->name;
		idPath			= pData->file_matter;
		break;
	}
	case DT_PET_ASSIST_REFINE_ESSENCE:
	{
		const PET_ASSIST_REFINE_ESSENCE* pData = (const PET_ASSIST_REFINE_ESSENCE*)pDataPtr;
		m_szName		= pData->name;
		idPath			= pData->file_matter;
		break;
	}
	case DT_EQUIP_SOUL_ESSENCE:
	{
		const EQUIP_SOUL_ESSENCE* pData = (const EQUIP_SOUL_ESSENCE*)pDataPtr;
		m_szName		= pData->name;
		idPath			= pData->file_matter;
		break;
	}
	case DT_SPECIAL_NAME_ITEM_ESSENCE:
	{
		const SPECIAL_NAME_ITEM_ESSENCE * pData = (const SPECIAL_NAME_ITEM_ESSENCE*)pDataPtr;
		m_szName		= pData->name;
		idPath			= pData->file_matter;
		const wchar_t * szDropperName = g_pGame->GetGameRun()->GetPlayerName(m_MatterInfo.dropper_id, false);
		if( !szDropperName && m_MatterInfo.dropper_id >0 )
		{
			g_pGame->GetGameSession()->GetPlayerBriefInfo(1, &(m_MatterInfo.dropper_id), 2);
		}
		m_bSpecialNameItem = true;
		break;
	}
	case DT_GIFT_BAG_ESSENCE:
	{
		const GIFT_BAG_ESSENCE* pData = (const GIFT_BAG_ESSENCE*)pDataPtr;
		m_szName		= pData->name;
		idPath			= pData->file_matter;
		break;
	}
	case DT_VIP_CARD_ESSENCE:
	{
		const VIP_CARD_ESSENCE* pData = (const VIP_CARD_ESSENCE*)pDataPtr;
		m_szName		= pData->name;
		idPath			= pData->file_matter;
		break;
	}
	case DT_CHANGE_SHAPE_CARD_ESSENCE:
	{
		const CHANGE_SHAPE_CARD_ESSENCE* pData = (const CHANGE_SHAPE_CARD_ESSENCE*)pDataPtr;
		m_szName		= pData->name;
		idPath			= pData->file_matter;
		break;
	}
	case DT_CHANGE_SHAPE_STONE_ESSENCE:
	{
		const CHANGE_SHAPE_STONE_ESSENCE* pData = (const CHANGE_SHAPE_STONE_ESSENCE*)pDataPtr;
		m_szName		= pData->name;
		idPath			= pData->file_matter;
		break;
	}
	case DT_MERCENARY_CREDENTIAL_ESSENCE:
	{
		const MERCENARY_CREDENTIAL_ESSENCE* pData = (const MERCENARY_CREDENTIAL_ESSENCE*)pDataPtr;
		m_szName		= pData->name;
		idPath			= pData->file_matter;
		break;
	}
	case DT_TELEPORTATION_ESSENCE:
	{
		const TELEPORTATION_ESSENCE* pData = (const TELEPORTATION_ESSENCE*)pDataPtr;
		m_szName		= pData->name;
		idPath			= pData->file_matter;
		break;
	}
	case DT_TELEPORTATION_STONE_ESSENCE:
	{
		const TELEPORTATION_STONE_ESSENCE* pData = (const TELEPORTATION_STONE_ESSENCE*)pDataPtr;
		m_szName		= pData->name;
		idPath			= pData->file_matter;
		break;
	}
	case DT_UPGRADE_EQUIP_STONE_ESSENCE:
	{
		const UPGRADE_EQUIP_STONE_ESSENCE* pData = (const UPGRADE_EQUIP_STONE_ESSENCE*)pDataPtr;
		m_szName		= pData->name;
		idPath			= pData->file_matter;
		break;
	}
	
	case DT_LOTTERY2_ESSENCE:
	{
		const LOTTERY2_ESSENCE* pData = (const LOTTERY2_ESSENCE*)pDataPtr;
		m_szName		= pData->name;
		idPath			= pData->file_matter;
		break;
	}

	case DT_LOTTERY3_ESSENCE: //Added 2011-07-27.
	{
		const LOTTERY3_ESSENCE* pData = (const LOTTERY3_ESSENCE*)pDataPtr;
		m_szName		= pData->name;
		idPath			= pData->file_matter;
		break;
	}

	case DT_GEM_ESSENCE://Added 2011-03-17.
	{
		const GEM_ESSENCE* pData = (const GEM_ESSENCE*)pDataPtr;
		m_szName		= pData->name;
		idPath			= pData->file_matter;
		break;
	}
	case DT_GENERAL_ARTICLE_ESSENCE://Added 2011-03-17.
	{
		const GENERAL_ARTICLE_ESSENCE* pData = (const GENERAL_ARTICLE_ESSENCE*)pDataPtr;
		m_szName		= pData->name;
		idPath			= pData->file_matter;
		break;
	}
	case DT_GEM_SEAL_ESSENCE: //Added 2011-04-27
	{
		const GEM_SEAL_ESSENCE* pData = (const GEM_SEAL_ESSENCE*)pDataPtr;
		m_szName		= pData->name;
		idPath			= pData->file_matter;
		break;
	}
	case DT_GEM_DUST_ESSENCE: //Added 2011-04-27
	{
		const GEM_DUST_ESSENCE* pData = (const GEM_DUST_ESSENCE*)pDataPtr;
		m_szName		= pData->name;
		idPath			= pData->file_matter;
		break;
	}
	case DT_SMALL_ONLINE_GIFT_BAG_ESSENCE:	//Added 2012-03-23.
	{
		const SMALL_ONLINE_GIFT_BAG_ESSENCE* pData = (const SMALL_ONLINE_GIFT_BAG_ESSENCE*)pDataPtr;
		m_szName		= pData->name;
		idPath			= pData->file_matter;
		break;
	}
	case DT_SCROLL_UNLOCK_ESSENCE:			//Added 2012-03-23.
	{
		const SCROLL_UNLOCK_ESSENCE* pData = (const SCROLL_UNLOCK_ESSENCE*)pDataPtr;
		m_szName		= pData->name;
		idPath			= pData->file_matter;
		break;
	}
	case DT_SCROLL_DIG_COUNT_INC_ESSENCE:			//Added 2012-03-23.
	{
		const SCROLL_DIG_COUNT_INC_ESSENCE* pData = (const SCROLL_DIG_COUNT_INC_ESSENCE*)pDataPtr;
		m_szName		= pData->name;
		idPath			= pData->file_matter;
		break;
	}
	case DT_RUNE_EQUIP_ESSENCE:			//Added 2013-03-09.
	{
		const RUNE_EQUIP_ESSENCE* pData = (const RUNE_EQUIP_ESSENCE*)pDataPtr;
		m_szName		= pData->name;
		idPath			= pData->file_matter;
		break;
	}
	case DT_RUNE_SLOT_ESSENCE:			//Added 2013-03-09.
	{
		const RUNE_SLOT_ESSENCE* pData = (const RUNE_SLOT_ESSENCE*)pDataPtr;
		m_szName		= pData->name;
		idPath			= pData->file_matter;
		break;
	}
	case DT_BOOTH_FIGURE_ITEM_ESSENCE:			//Added 2013-03-09.
	{
		const BOOTH_FIGURE_ITEM_ESSENCE* pData = (const BOOTH_FIGURE_ITEM_ESSENCE*)pDataPtr;
		m_szName		= pData->name;
		idPath			= pData->file_matter;
		break;
	}
	case DT_FLAG_BUFF_ITEM_ESSENCE:			//Added 2013-03-09.
	{
		const FLAG_BUFF_ITEM_ESSENCE* pData = (const FLAG_BUFF_ITEM_ESSENCE*)pDataPtr;
		m_szName		= pData->name;
		idPath			= pData->file_matter;
		break;
	}
	case DT_XINGZUO_ITEM_ESSENCE:			//Added 2013-03-09.
	{
		const XINGZUO_ITEM_ESSENCE* pData = (const XINGZUO_ITEM_ESSENCE*)pDataPtr;
		m_szName		= pData->name;
		idPath			= pData->file_matter;
		break;
	}
	case DT_XINGZUO_ENERGY_ITEM_ESSENCE:			//Added 2013-03-09.
	{
		const XINGZUO_ENERGY_ITEM_ESSENCE* pData = (const XINGZUO_ENERGY_ITEM_ESSENCE*)pDataPtr;
		m_szName		= pData->name;
		idPath			= pData->file_matter;
		break;
	}
	case DT_CASH_MEDIC_MERGE_ITEM_ESSENCE:			//Added 2013-03-09.
	{
		const CASH_MEDIC_MERGE_ITEM_ESSENCE* pData = (const CASH_MEDIC_MERGE_ITEM_ESSENCE*)pDataPtr;
		m_szName		= pData->name;
		idPath			= pData->file_matter;
		break;
	}
	case DT_SALE_PROMOTION_ITEM_ESSENCE:			//Added 2013-03-09.
	{
		const SALE_PROMOTION_ITEM_ESSENCE* pData = (const SALE_PROMOTION_ITEM_ESSENCE*)pDataPtr;
		m_szName		= pData->name;
		idPath			= pData->file_matter;
		break;
	}
	case DT_GIFT_BAG_LOTTERY_DELIVER_ESSENCE:			//Added 2013-03-09.
	{
		const GIFT_BAG_LOTTERY_DELIVER_ESSENCE* pData = (const GIFT_BAG_LOTTERY_DELIVER_ESSENCE*)pDataPtr;
		m_szName		= pData->name;
		idPath			= pData->file_matter;
		break;
	}
	case DT_LOTTERY_TANGYUAN_ITEM_ESSENCE:			//Added 2013-03-09.
	{
		const LOTTERY_TANGYUAN_ITEM_ESSENCE* pData = (const LOTTERY_TANGYUAN_ITEM_ESSENCE*)pDataPtr;
		m_szName		= pData->name;
		idPath			= pData->file_matter;
		break;
	}
	case DT_GIFT_PACK_ITEM_ESSENCE:			//Added 2013-03-09.
	{
		const GIFT_PACK_ITEM_ESSENCE* pData = (const GIFT_PACK_ITEM_ESSENCE*)pDataPtr;
		m_szName		= pData->name;
		idPath			= pData->file_matter;
		break;
	}
	case DT_PROP_ADD_MATERIAL_ITEM_ESSENCE:			//Added 2013-03-09.
	{
		const PROP_ADD_MATERIAL_ITEM_ESSENCE* pData = (const PROP_ADD_MATERIAL_ITEM_ESSENCE*)pDataPtr;
		m_szName		= pData->name;
		idPath			= pData->file_matter;
		break;
	}
	case DT_PROP_ADD_ITEM_ESSENCE:			//Added 2013-03-09.
	{
		const PROP_ADD_ITEM_ESSENCE* pData = (const PROP_ADD_ITEM_ESSENCE*)pDataPtr;
		m_szName		= pData->name;
		idPath			= pData->file_matter;
		break;
	}
	case DT_COLORANT_ITEM_ESSENCE:			//Added 2013-07-12.
		{
			const COLORANT_ITEM_ESSENCE* pData = (const COLORANT_ITEM_ESSENCE*)pDataPtr;
			m_szName		= pData->name;
			idPath			= pData->file_matter;
			break;
		}
	default:

		//ASSERT(0);
		break;
	}

	switch (iFixProps)
	{
	case 1:	m_dwMatterType |= ITEMFLAG_SUIT;		break;
	case 2:	m_dwMatterType |= ITEMFLAG_RARE;		break;
	case 3:	m_dwMatterType |= ITEMFLAG_UNIQUE;		break;
//	case 3:	m_dwMatterType |= ITEMFLAG_MYTHICAL;	break;
	}

	m_szModelFile = g_pGame->GetDataPath(idPath);
	if (!m_szModelFile || !m_szModelFile[0])
		m_szModelFile = res_ModelFile(RES_MOD_TEMP);

	//	Set name
	if (m_pPateName)
		m_pPateName->SetText(m_szName, false);

	//	test code ...
//	m_szModelFile = res_ModelFile(RES_MOD_TEMP);

	return true;
}

//	Set absolute position
void CECMatter::SetPos(const A3DVECTOR3& vPos)
{
	CECObject::SetPos(vPos);

	m_aabb.Center = vPos + A3DVECTOR3(0.0f, m_aabb.Extents.y, 0.0f);
	m_aabb.CompleteMinsMaxs();

	if (m_pModel)
		m_pModel->SetPos(vPos);

	if (m_pGfx)
		m_pGfx->SetPos(vPos);
}

//	Set absolute forward and up direction
void CECMatter::SetDirAndUp(const A3DVECTOR3& vDir, const A3DVECTOR3& vUp)
{
	CECObject::SetDirAndUp(vDir, vUp);

	if (m_pModel)
		m_pModel->SetDirAndUp(vDir, vUp);

	if (m_pGfx)
		m_pGfx->SetDirAndUp(vDir, vUp);
}

//	Tick routine
bool CECMatter::Tick(DWORD dwDeltaTime) 
{
	if (m_pModel)
		m_pModel->Tick(dwDeltaTime);

	if (m_pGfx)
		m_pGfx->TickAnimation(dwDeltaTime);

	//	Calculate distance to host player
	CECHostPlayer* pHost = g_pGame->GetGameRun()->GetHostPlayer();
	if (pHost && pHost->IsSkeletonReady())
	{
		m_fDistToHost  = CalcDist(pHost->GetPos(), true);
		m_fDistToHostH = CalcDist(pHost->GetPos(), false);
	}

	//-------------------------------------------------------------------------
	// Deal with the problem that when the mine generated dynamically, some times
	// the host player may be traped by the collision detection.
	if ( m_pModel && m_bFirstCDCheck )
	{
		if ( NeedCheckCollosion() && CheckCollosion() )
		{
			m_bStopCollosionDetection = true;
		}
		m_bFirstCDCheck = false;
	}

	//	if the stop cd flag is true, and there is no collosion, then reset the flag to false;
	if ( m_bStopCollosionDetection && !CheckCollosion() )
	{
		m_bStopCollosionDetection = false;
	}
	//-------------------------------------------------------------------------

	return true; 
}

//	Interval render routine
#ifdef BACK_VERSION
bool CECMatter::InternalRender(CECViewport* pViewport, bool bHighLight)
{
	A3DCameraBase* pCamera = pViewport->GetA3DCamera();
	m_fDistToCamera	= CalcDist(pCamera->GetPos(), true);
	
	if (!m_bVisible)
		return true;
	
	if (m_pModel)
	{
		if (bHighLight)
			g_pGame->RenderHighLightModel(pViewport, m_pModel);
		else
			m_pModel->Render(pViewport->GetA3DViewport());
	}
	
	if (m_pGfx)
		g_pGame->GetGFXCaster()->GetGFXExMan()->RegisterGfx(m_pGfx);
	
	if (m_bRenderName)
		RenderName(pViewport);
	
	return true;
}
#else
bool CECMatter::InternalRender(ECViewport* pViewport, bool bHighLight, int nRenderFlag)
{
	// 	A3DCameraBase* pCamera = pViewport->GetA3DCamera();
	// 	m_fDistToCamera	= CalcDist(pCamera->GetPos(), true);
	
	if (!m_bVisible)
		return true;
	
	if (m_pModel)
	{
		if (bHighLight)
			g_pGame->RenderHighLightModel(pViewport, m_pModel);
		else
		{
			bool bRenderGfx = true;
			if(nRenderFlag == RD_SHADOW)
				bRenderGfx = false;
			m_pModel->Render(pViewport->GetA3DViewport(), false, true, 0, NULL);
		}
	}
	
	if (m_pGfx && nRenderFlag != CECObject::RD_SHADOW)
		g_pGame->GetGFXCaster()->GetGFXExMan()->RegisterGfx(m_pGfx);
	
	if (m_bRenderName && nRenderFlag == CECObject::RD_NORMAL)
		RenderName(pViewport);
	
	return true;
}
#endif

//	Render titles / names / talks above player's header
bool CECMatter::RenderName(CECViewport* pViewport)
{
	if (!g_pGame->GetConfigs()->GetVideoSettings().bGoodsName)
		return true;

	A3DFont* pFont = g_pGame->GetFont(RES_FONT_TITLE);
	if (!pFont || !m_pPateName)
	{
		ASSERT(0);
		return false;
	}

	A3DVIEWPORTPARAM* pViewPara = pViewport->GetA3DViewport()->GetParam();
	A3DFontMan* pFontMan = g_pGame->GetA3DEngine()->GetA3DFontMan();

	//	Calculate basic center position on screen
	A3DVECTOR3 vPos;
	if( m_pModel && m_pModel->HasCHAABB() )
	{
		const A3DAABB& aabb = m_pModel->GetCHAABB();
		vPos = m_pModel->GetPos() + g_vAxisY * (aabb.Extents.y * 2.3f);
	}
	else if (m_pModel && m_pModel->GetA3DSkinModel())
	{
		const A3DAABB& aabb = m_pModel->GetA3DSkinModel()->GetModelAABB();
		vPos = aabb.Center + g_vAxisY * (aabb.Extents.y * 1.3f);
	}
	else
		vPos = m_aabb.Center + g_vAxisY * (m_aabb.Extents.y * 1.3f);

	A3DVECTOR3 vTemp;
	A3DMATRIX4 matVPS = pViewport->GetA3DViewport()->GetCamera()->GetVPTM() * pViewport->GetA3DViewport()->GetViewScale();
	A3DVECTOR4 vScrPos = TransformToScreen(vPos, matVPS);

	if (vScrPos.z < pViewPara->MinZ || vScrPos.z > pViewPara->MaxZ)
		return true;

	//	Decide matter name color
	DWORD dwNameCol = A3DCOLORRGB(255, 255, 255);
	int iColIdx = g_pGame->GetItemNameColorIdx(m_MatterInfo.tid);
	if (iColIdx > 0)
	{
		dwNameCol = g_pGame->GetItemNameColor(m_MatterInfo.tid);
	}
	else
	{
		if (m_dwMatterType & ~MATTER_TYPEMASK)
		{
			if (m_dwMatterType & ITEMFLAG_MYTHICAL)
				dwNameCol = A3DCOLORRGB(170, 50, 255);
			else if (m_dwMatterType & ITEMFLAG_UNIQUE)
			//	dwNameCol = A3DCOLORRGB(206, 188, 134);
				dwNameCol = A3DCOLORRGB(255, 96, 0);
			else if (m_dwMatterType & ITEMFLAG_SUIT)
				dwNameCol = A3DCOLORRGB(108, 251, 75);
			else if (m_dwMatterType & ITEMFLAG_RARE)
				dwNameCol = A3DCOLORRGB(255, 220, 80);
			else if (m_dwMatterType & ITEMFLAG_EXTPROP)
				dwNameCol = A3DCOLORRGB(128, 128, 255);
		}
	}
	if(m_bSpecialNameItem && m_MatterInfo.dropper_id > 0 && m_strCompName == _AL(""))
	{
		const ACHAR* szDropperName = g_pGame->GetGameRun()->GetPlayerName(m_MatterInfo.dropper_id, false);
		if(!szDropperName)
		{
			g_pGame->GetGameSession()->GetPlayerBriefInfo(1, &(m_MatterInfo.dropper_id), 2);
		}
		else
			m_strCompName.Format(g_pGame->GetFixedMsgTab()->GetWideString(FIXMSG_NAME_COMBINE), szDropperName, m_szName);

		m_pPateName->SetText(m_strCompName, false);	
 	}
	
	if (m_cOwnerType != CECNPC::OWNER_NONE && m_idOwner && m_strOwnerName.GetLength() == 0)
	{
		BuildOwnerName();
	}
	AUIManager* pUIMan = g_pGame->GetGameRun()->GetUIManager()->GetCurrentUIManPtr();
	assert(pUIMan);
	float fScale = pUIMan->GetWindowScale();

	int x, y=(int)vScrPos.y, cx, cy;

	if (m_pPateOwner && m_cOwnerType != CECNPC::OWNER_NONE && m_idOwner)
	{
		m_pPateOwner->GetExtent(&cx, &cy);
		cx = int(cx * fScale);
		cy = int(cy * fScale);
		x = (int)(vScrPos.x - (cx >> 1));
		y -= cy + 2;
	//	m_pPateName->Render(pViewport, x, y, dwNameCol, vScrPos.z);
		m_pPateOwner->RegisterRender(x, y, dwNameCol, vScrPos.z, vScrPos.w);
	}
	//	Draw name string
	m_pPateName->GetExtent(&cx, &cy);
	cx = int(cx * fScale);
	cy = int(cy * fScale);
	x = (int)(vScrPos.x - (cx >> 1));
//	y -= (cy >> 1);
	y -= cy + 2;
//	m_pPateName->Render(pViewport, x, y, dwNameCol, vScrPos.z);
	m_pPateName->RegisterRender(x, y, dwNameCol, vScrPos.z, vScrPos.w);

	return true;
}

bool CECMatter::RayTrace(ECRAYTRACE* pTraceInfo)
{
	bool bRet = false;

	if( IsBattleFlag() )
	{
		if( g_pGame->GetGameRun()->GetHostPlayer()->IsInBattle() )
		{
			if( g_pGame->GetGameRun()->GetHostPlayer()->GetBattleCamp() == GP_BATTLE_CAMP_INVADER )
			{
				if( GetBattleFlagState() == 1 )
					return bRet;
			}
			else if( g_pGame->GetGameRun()->GetHostPlayer()->GetBattleCamp() == GP_BATTLE_CAMP_DEFENDER )
			{
				if( GetBattleFlagState() == 2 )
					return bRet;
			}
		}
	}

	A3DVECTOR3 vHitPos, vNormal;
	float fFraction;

	if( m_pModel && m_pModel->HasCHAABB() )
	{
		A3DMATRIX4 matTo = a3d_ViewMatrix(GetPos(), GetDir(), GetUp(), 0.0f);
		A3DVECTOR3 vecStart = pTraceInfo->vStart * matTo;
		A3DVECTOR3 vecDelta = a3d_VectorMatrix3x3(pTraceInfo->vDelta, matTo);

		const A3DAABB& aabb = m_pModel->GetCHAABB();
		if (CLS_RayToAABB3(vecStart, vecDelta, aabb.Mins, aabb.Maxs,
					vHitPos, &fFraction, vNormal))
		{
			CHBasedCD::ConvexHullDataArray & chArray = m_pModel->GetConvexHullData();

			for(int i=0; i<int(chArray.size()); i++)
			{
				CConvexHullData * pCH = chArray[i];
				const CFace * pFace;
				if( RayIntersectWithCH(vecStart, vecDelta, *pCH, &pFace, vHitPos, fFraction) )
				{
					if (fFraction < pTraceInfo->pTraceRt->fFraction)
					{
						vNormal = pFace->GetNormal();

						A3DMATRIX4 mat = GetAbsoluteTM();
						vHitPos = vHitPos * mat;
						vNormal = a3d_VectorMatrix3x3(vNormal, mat);

						pTraceInfo->pTraceRt->fFraction		= fFraction;
						pTraceInfo->pTraceRt->vHitPos		= vHitPos;
						pTraceInfo->pTraceRt->vPoint		= vHitPos;
						pTraceInfo->pTraceRt->vNormal		= vNormal;

						pTraceInfo->pECTraceRt->fFraction	= fFraction;
						pTraceInfo->pECTraceRt->iEntity		= IsDynModel() ? ECENT_DYN_OBJ : ECENT_MATTER;
						pTraceInfo->pECTraceRt->iObjectID	= GetMatterID();
						pTraceInfo->pECTraceRt->vNormal		= vNormal;

						bRet = true;
					}
				}
			}
		}
	}
	else
	{
		const A3DAABB& aabb = GetMatterAABB();
		if (CLS_RayToAABB3(pTraceInfo->vStart, pTraceInfo->vDelta, aabb.Mins, aabb.Maxs,
					vHitPos, &fFraction, vNormal))
		{
			if (fFraction < pTraceInfo->pTraceRt->fFraction)
			{
				pTraceInfo->pTraceRt->fFraction		= fFraction;
				pTraceInfo->pTraceRt->vHitPos		= vHitPos;
				pTraceInfo->pTraceRt->vPoint		= vHitPos;
				pTraceInfo->pTraceRt->vNormal		= vNormal;

				pTraceInfo->pECTraceRt->fFraction	= fFraction;
				pTraceInfo->pECTraceRt->iEntity		= IsDynModel() ? ECENT_DYN_OBJ : ECENT_MATTER;
				pTraceInfo->pECTraceRt->iObjectID	= GetMatterID();
				pTraceInfo->pECTraceRt->vNormal		= vNormal;

				bRet = true;
			}
		}
	}

	return bRet;
}

bool CECMatter::TraceWithBrush(BrushTraceInfo * pInfo)
{
	if( !m_pModel || !m_pModel->HasCHAABB() )
	{
		return false;
	}

	// Expand the AABB of the model to handle the case that
	// convex hulls of the model protrude the AABB!
	A3DAABB aabb = m_pModel->GetCHAABB();
	aabb.Extents+=A3DVECTOR3(8.0f, 8.0f, 8.0f);
	aabb.Center += GetPos();
	aabb.CompleteMinsMaxs();

	A3DVECTOR3 vPoint, vNormal;
	float		fFraction;		//	Fraction
	
	if (pInfo->bIsPoint && !CLS_RayToAABB3(pInfo->vStart, pInfo->vDelta, aabb.Mins, aabb.Maxs, vPoint, &fFraction, vNormal ) ) {
		return false;
	}
	
	if (!pInfo->bIsPoint && !CLS_AABBAABBOverlap(aabb.Center, aabb.Extents, pInfo->BoundAABB.Center, pInfo->BoundAABB.Extents) ) {
		return false;
	}
	
	//save original result
	bool		bStartSolid = pInfo->bStartSolid;	//	Collide something at start point
	bool		bAllSolid = pInfo->bAllSolid;		//	All in something
	int			iClipPlane = pInfo->iClipPlane;		//	Clip plane's index
	
	fFraction = 100.0f;
	vNormal = pInfo->ClipPlane.GetNormal(); //clip plane normal
	float       fDist = pInfo->ClipPlane.GetDist();	//clip plane dist

	// Detect the collision with Convex Hull
	bool bCollide = false;
	for(int i=0;i<m_nBrushes;i++)
	{
		if (m_ppBrushes[i]->Trace(pInfo) && (pInfo->fFraction < fFraction)) {
				//update the saving info
				bStartSolid = pInfo->bStartSolid;
				bAllSolid = pInfo->bAllSolid;
				iClipPlane = pInfo->iClipPlane;
				fFraction = pInfo->fFraction;
				vNormal = pInfo->ClipPlane.GetNormal();
				fDist = pInfo->ClipPlane.GetDist();

				bCollide = true;
		}
	}	
	if (bCollide) {
		pInfo->bStartSolid = bStartSolid;
		pInfo->bAllSolid = bAllSolid;
		pInfo->iClipPlane = iClipPlane;
		pInfo->fFraction = fFraction;
		pInfo->ClipPlane.SetNormal(vNormal);
		pInfo->ClipPlane.SetD(fDist);
	}
	return bCollide;

}

void CECMatter::BattleFlagChange(int nOrgState, int nNewState)
{
	static const char* _flag_act[3][3] =
	{
		{ "灰白",	"灰白到红", "灰白到蓝"	},
		{ "",		"红",		"红到蓝"	},
		{ "",		"蓝到红",	"蓝"		},
	};

	m_cBattleFlagState = (char)nNewState;

	if (!m_pModel)
		return;

	if (nOrgState < 0 || nNewState < 0 || nOrgState > 2 || nNewState > 2)
		return;

	m_pModel->PlayActionByName(_flag_act[nOrgState][nNewState], 1.0f);
}

//	Load gfx from file
bool CECMatter::LoadGFXFromFile(const char* szFile, BYTE byScale)
{
	m_pGfx = g_pGame->GetGFXCaster()->GetGFXExMan()->LoadGfx(g_pGame->GetA3DDevice(), szFile, false);
	if (!m_pGfx)
		return false;

	//float fScale = byScale * 16.0f / 256.0f;
	//m_pGfx->SetScale(fScale);
	m_pGfx->Start();

	return true;
}

void CECMatter::BuildOwnerName()
{
	const ACHAR* szName = g_pGame->GetGameRun()->GetPlayerName(m_idOwner, false);
	if (!szName)
	{
		static DWORD timeLast = 0;
		if( timeLast + 1000 < timeGetTime() )
		{
			g_pGame->GetGameSession()->GetPlayerBriefInfo(1, &m_idOwner, 2);
			timeLast = timeGetTime();
		}		
	}
	else
	{
		if (m_cOwnerType == CECNPC::OWNER_APPRENTICE)
			m_strOwnerName.Format(g_pGame->GetFixedMsgTab()->GetWideString(FIXMSG_NPC_OWNER_NAME), szName);
		else
			m_strOwnerName.Format(_AL("<%s>"), szName);
		m_pPateOwner->SetText(m_strOwnerName, false);
	}
}

#ifndef BACK_VERSION
bool CECMatter::Render( ECViewport* pViewport, int nRenderFlag )
{
	if(!IsPhaseShow())
		return true;

	if(nRenderFlag == CECObject::RD_NORMAL)
	{
		A3DCameraBase* pCamera = pViewport->GetA3DCameraBase();
		m_fDistToCamera	= CalcDist(pCamera->GetPos(), true);
	}
	return InternalRender(pViewport, false, nRenderFlag);
}

bool CECMatter::RenderHighLight( ECViewport* pViewport, int nRenderFlag )
{
	if(nRenderFlag == CECObject::RD_NORMAL)
	{
		A3DCameraBase* pCamera = pViewport->GetA3DCameraBase();
		m_fDistToCamera	= CalcDist(pCamera->GetPos(), true);
	}
	return InternalRender(pViewport, true, nRenderFlag);
}
#endif

bool CECMatter::IsPhaseShow()
{
	CECHostPlayer* pHost = g_pGame->GetGameRun()->GetHostPlayer();
	if(m_bPhase && m_iPhaseId != 0)
	{
		if(m_iPhaseId > 0 && m_iPhaseId <= CECHostPlayer::PHASE_NUM)
			if(pHost->GetPhaseByIndex(m_iPhaseId - 1) == -1)   //根据默认相位判读
			{
				if(pHost->GetDefaultPhaseByIndex(m_iPhaseId - 1) == 1)
					return true;
				else 
					return false;
			}
			else					//根据改变的相位判断
			{
				if(pHost->GetPhaseByIndex(m_iPhaseId - 1) == 1)
					return true;
				else 
					return false;
			}
	}

	return true;
}

bool CECMatter::ChangeMineState(int stateid)
{
	DATA_TYPE DataType = DT_INVALID;
	COMPOUND_MINE_ESSENCE* pData = (COMPOUND_MINE_ESSENCE*) g_pGame->GetElementDataMan()->get_data_ptr(GetTemplateID(), ID_SPACE_ESSENCE, DataType);
	if (DataType != DT_COMPOUND_MINE_ESSENCE)
		return false;

	if (stateid != pData->file_mine[0] && stateid != pData->file_mine[1]
		&& stateid != pData->file_mine[2] && stateid != pData->file_mine_spe)
		return false;

	DataType = DT_INVALID;
	MINE_ESSENCE* pMineData = (MINE_ESSENCE*) g_pGame->GetElementDataMan()->get_data_ptr(stateid, ID_SPACE_ESSENCE, DataType);
	if (DataType != DT_MINE_ESSENCE)
		return false;

	int idPath = pMineData->file_model;
	m_szModelFile = g_pGame->GetDataPath(idPath);
	if (!m_szModelFile || !m_szModelFile[0])
		m_szModelFile = res_ModelFile(RES_MOD_TEMP);

	// Load model.
	QueueECModelForLoad(MTL_ECM_MATTER, GetMatterID(), 0, GetPos(), m_szModelFile, NULL);

	m_dwMatterType	= MATTER_MINE;
	m_iLevelReq		= pMineData->level_required;
	m_szName		= pMineData->name;
	m_iMineStateID	= stateid;

	//	Set name
	if (m_pPateName)
		m_pPateName->SetText(m_szName, false);

	return true;
}
