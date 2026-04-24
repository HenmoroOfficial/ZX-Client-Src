/*
 * FILE: EC_NPC.cpp
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
#include "EC_NPC.h"
#include "EC_Model.h"
#include "EC_Game.h"
#include "EC_GameRun.h"
#include "EC_World.h"
#ifdef BACK_VERSION
#include "EC_Viewport.h"
#else
#include "ECViewport.h"
#endif
#include "EC_ManPlayer.h"
#include "EC_ManNPC.h"
#include "EC_ShadowRender.h"
#include "EC_Player.h"
#include "EC_Utility.h"
#include "EC_Decal.h"
#include "EC_ManDecal.h"
#include "EC_Resource.h"
//#include "EC_FixedMsg.h"
#include "EC_HostPlayer.h"
//#include "EC_Skill.h"
#include "EC_Monster.h"
//#include "EC_PateText.h"
#include "EC_ImageRes.h"
#include "EC_Configs.h"
#include "EC_SceneLoader.h"
#include "EC_CDR.h"
#include "CDWithCH.h"
//#include "AUI\AUILuaManager.h"
//#include "EC_UIManager.h"
#include "A3DSkillGfxComposer.h"
//#include "EC_GameRecord.h"
//#include "EC_Cutscene.h"
//#include "EC_GameSession.h"

#include <A3DCamera.h>
#include <A3DFont.h>
#include <A3DViewport.h>
#include <A3DFlatCollector.h>
#include <A3DSkinMan.h>
#ifdef BACK_VERSION
#include <A3DTerrainWater.h>
#else
#include <A3DTerrainWater2.h>
#endif
#include <A3DCollision.h>
#include <A3DSkinModelAct.h>
#include <A3DSkeleton.h>
#include <AFI.h>

#include "elementdataman.h"

using namespace CHBasedCD;

#define new A_DEBUG_NEW

///////////////////////////////////////////////////////////////////////////
//	
//	Define and Macro
//	
///////////////////////////////////////////////////////////////////////////

#define MAX_LAGDIST		15.0f	//	Maximum lag distance

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
/*
//	NPC's render for shadow call back function
bool NPCRenderForShadow(A3DViewport * pViewport, void * pArg)
{
	A3DSkinModel * pModel = (A3DSkinModel *) pArg;

	if( !pModel )
		return false;

	g_pGame->GetA3DDevice()->SetZTestEnable(true);
	g_pGame->GetA3DDevice()->SetZWriteEnable(true);
	g_pGame->GetA3DDevice()->SetAlphaTestEnable(true);
	g_pGame->GetA3DDevice()->SetAlphaBlendEnable(false);
	g_pGame->GetA3DDevice()->SetAlphaFunction(A3DCMP_GREATEREQUAL);
	g_pGame->GetA3DDevice()->SetAlphaRef(84);

	pModel->RenderAtOnce(pViewport, A3DSkinModel::RAO_NOMATERIAL, false);

	g_pGame->GetA3DDevice()->SetAlphaBlendEnable(true);
	g_pGame->GetA3DDevice()->SetAlphaTestEnable(false);
	g_pGame->GetA3DDevice()->SetZTestEnable(true);
	g_pGame->GetA3DDevice()->SetZWriteEnable(true);
	return true;
}
*/
//	Distinguish a NPC id. Return CECNPC::NPC_MONSTER if it's monster NPC id. 
//	CECNPC::NPC_SERVER if it's server NPC id. NPC_UNKNOWN if it's not a NPC id
int CECNPC::DistinguishID(int tid)
{
	DATA_TYPE DataType = g_pGame->GetElementDataMan()->get_data_type(tid, ID_SPACE_ESSENCE);
	int iNPCType = -1;

	switch (DataType)
	{
	case DT_NPC_ESSENCE:		iNPCType = NPC_SERVER;		break;
	case DT_MONSTER_ESSENCE:	iNPCType = NPC_MONSTER; 	break;
	case DT_PET_BEDGE_ESSENCE:	iNPCType = NPC_PET;			break;
	default:
		ASSERT(0);
		break;
	}

	return iNPCType;
}

//	Load NPC model
bool CECNPC::LoadNPCModel(int tid, const char* szFile, EC_NPCLOADRESULT& Ret)
{
	Ret.pModel		= NULL;
	Ret.pSkin		= NULL;
	Ret.pLowSkin	= NULL;

	if (!(Ret.pModel = new CECModel))
	{
		glb_ErrorOutput(ECERR_NOTENOUGHMEMORY, "CECNPC::LoadNPCModel", __LINE__);
		return false;
	}

	char szModelFile[MAX_PATH];
	strcpy(szModelFile, szFile);

	int iNPCType = DistinguishID(tid);
	if (iNPCType == NPC_MONSTER || iNPCType == NPC_PET)
	{
		g_pGame->GetA3DEngine()->GetA3DSkinMan()->SetReplaceShaderFile("armor.sdr", A3DSkinMan::SHADERREPLACE_REFLECTPREFIX);
		if (!Ret.pModel->Load(szModelFile, true, A3DSkinModel::LSF_NOSKIN, true))
		{
			//	Now try to load error.ecm
			strcpy(szModelFile, "models\\error\\error.ecm");
			if (!Ret.pModel->Load(szModelFile, true, A3DSkinModel::LSF_NOSKIN, true))
			{
				g_pGame->GetA3DEngine()->GetA3DSkinMan()->SetReplaceShaderFile("", 0);
				a_LogOutput(1, "CECNPC::LoadNPCModel, Failed to load NPC model %s", szFile);
				return false;
			}
		}

		char szSkin[MAX_PATH];
		strncpy(szSkin, szModelFile, MAX_PATH);
		glb_ChangeExtension(szSkin, "ski");

		if (!(Ret.pSkin = g_pGame->LoadA3DSkin(szSkin, true)))
		{
			a_LogOutput(1, "CECNPC::Init(), Failed to load skin %s", szSkin);
			Ret.pSkin = g_pGame->LoadA3DSkin("Models\\error\\error.ski", true);
		}

		glb_ClearExtension(szSkin);
		strcat(szSkin, "二级.ski");

		if( af_IsFileExist(szSkin) )
		{
			if (!(Ret.pLowSkin = g_pGame->LoadA3DSkin(szSkin, true)))
			{
				a_LogOutput(1, "CECNPC::Init(), Falied to load skin %s", szSkin);
				Ret.pLowSkin = g_pGame->LoadA3DSkin("Models\\error\\error.ski", true);
			}
		}

		if (Ret.pModel->GetA3DSkinModel())
		{
			Ret.pModel->GetA3DSkinModel()->AddSkin(NULL, false);
			Ret.pModel->SetAABBType(CECModel::AABB_AUTOSEL);
		}
		g_pGame->GetA3DEngine()->GetA3DSkinMan()->SetReplaceShaderFile("", 0);
	}
	else
	{
		g_pGame->GetA3DEngine()->GetA3DSkinMan()->SetReplaceShaderFile("armor.sdr", A3DSkinMan::SHADERREPLACE_REFLECTPREFIX);
		if (!Ret.pModel->Load(szFile, true, A3DSkinModel::LSF_NOSKIN, true))
		{
			//	Now try to load error.ecm
			strcpy(szModelFile, "models\\error\\error.ecm");
			if (!Ret.pModel->Load(szModelFile))
			{
				g_pGame->GetA3DEngine()->GetA3DSkinMan()->SetReplaceShaderFile("", 0);
				a_LogOutput(1, "CECNPC::LoadNPCModel, Failed to load NPC model %s", szFile);
				return false;
			}
		}

		char szSkin[MAX_PATH];
		strncpy(szSkin, szModelFile, MAX_PATH);
		glb_ChangeExtension(szSkin, "ski");

		if (!(Ret.pSkin = g_pGame->LoadA3DSkin(szSkin, true)))
		{
			a_LogOutput(1, "CECNPC::Init(), Failed to load skin %s", szSkin);
			Ret.pSkin = g_pGame->LoadA3DSkin("Models\\error\\error.ski", true);
		}

		if (Ret.pModel->GetA3DSkinModel())
		{
			Ret.pModel->GetA3DSkinModel()->AddSkin(NULL, false);
			Ret.pModel->SetAABBType(CECModel::AABB_AUTOSEL);
		}
		g_pGame->GetA3DEngine()->GetA3DSkinMan()->SetReplaceShaderFile("", 0);
	}

	// wound channel
	A3DSkinModel* pA3DModel = Ret.pModel->GetA3DSkinModel(); 
	A3DSkeleton* pSkeleton = pA3DModel->GetSkeleton();
	int iNumJoint = pSkeleton->GetJointNum();
	int* aJoints = new int [iNumJoint];

	for (int i = 0; i < iNumJoint; i++)
		aJoints[i] = i;

	A3DSMActionChannel* pChannel = pA3DModel->BuildActionChannel(
		ACT_CHANNEL_WOUND,
		iNumJoint,
		aJoints);

	delete[] aJoints;

	if (!pChannel)
	{
		ReleaseNPCModel(Ret);
		return false;
	}

	pChannel->SetPlayMode(A3DSMActionChannel::PLAY_COMBINE);
	return true;
}

//	Release NPC model
void CECNPC::ReleaseNPCModel(EC_NPCLOADRESULT& Ret)
{
	if (Ret.pModel)
	{
		QueueECModelForRelease(Ret.pModel);
	//	Ret.pModel->Release();
	//	delete Ret.pModel;
		Ret.pModel = NULL;
	}

	if (Ret.pSkin)
	{
		g_pGame->ReleaseA3DSkin(Ret.pSkin, true);
		Ret.pSkin = NULL;
	}

	if (Ret.pLowSkin)
	{
		g_pGame->ReleaseA3DSkin(Ret.pLowSkin, true);
		Ret.pLowSkin = NULL;
	}
}

//	Initialize static resources
bool CECNPC::InitStaticRes()
{
	//	Load action names from file
	if (!m_ActionNames.IsInitialized())
		m_ActionNames.Init("Configs\\actions_npc.txt", false);

	if (!m_SkillActs.IsInitialized())
		m_ActionNames.Init("Configs\\skillacts.txt", false);

	return true;
}

//	Release static resources
void CECNPC::ReleaseStaticRes()
{
	m_ActionNames.Release();
	m_SkillActs.Release();
}

///////////////////////////////////////////////////////////////////////////
//	
//	Implement CECNPC
//	
///////////////////////////////////////////////////////////////////////////

CECStringTab CECNPC::m_ActionNames;	//	Action name table
CECStringTab CECNPC::m_SkillActs;	//	Skill action name table

CECNPC::CECNPC(CECNPCMan* pNPCMan) : 
m_vServerPos(0.0f),
m_strName(_AL("NPC")),
m_strDesc(_AL(""))
{
	m_iCID				= OCID_NPC;
	m_pNPCMan			= pNPCMan;
	m_pNPCModel			= NULL;
	m_pNPCSkin			= NULL;
	m_pNPCSkinLow		= NULL;
	m_pCurSkin			= NULL;
	m_bCastShadow		= false;
	m_iMoveEnv			= MOVEENV_GROUND;
	m_fMoveSpeed		= 1.0f;
	m_bStopMove			= false;
	m_fTouchRad			= 1.0f;
	m_bVisible			= true;
	m_fDistToHost		= 0.0f;
	m_fDistToCamera		= 0.0f;
	m_fDistToHostH		= 0.0f;
	m_iMMIndex			= -1;
	m_dwStates			= 0;
	m_bPushBack			= false;
	m_bAboutToDie		= false;
	m_bRenderName		= false;
	m_nFightTimeLeft	= 0;
//	m_pCurSkill			= NULL;
	m_dwExtStates		= 0;
	m_iRandomProp		= 0;
	m_dwISTimeCnt		= 0;
	m_bRenderBar		= false;
	m_fCurTrans			= 0.0f;
	m_fDstTrans			= 0.0f;
	m_idMaster			= 0;
	m_idSummoner		= 0;
	m_cPetStar			= 0;
	m_bStartFight		= false;
	m_cOwnerType		= OWNER_NONE;
	m_idOwner			= 0;

	m_iCurWork			= 0;
	m_iCurWorkType		= -1;
	m_idAttackTarget	= 0;
	m_iAction			= -1;
	m_bIsMovieNPC		= false;
	m_bDim				= false;
	m_iSpecialState		= 0;

	m_PateContent.iVisible = 0;

// 	if ((m_pPateName = new CECPateText))
// 		m_pPateName->EnableBorder(true);
// 	if((m_pPateDesc = new CECPateText))
// 		m_pPateDesc->EnableBorder(true);
// 	if((m_pPateOwner = new CECPateText))
// 		m_pPateOwner->EnableBorder(true);

	m_pcdr = new OtherPlayer_Move_Info;

// 	m_pPateLastWords1	= NULL;
// 	m_pPateLastWords2	= NULL;
	m_iLastSayCnt		= 0;
	m_pBubbleTexts		= new CECBubbleDecalList;

	m_aabbDefPick.Extents.Set(0.3f, 0.85f, 0.3f);
	m_DisappearCnt.SetPeriod(5000);
	m_LODCnt.SetPeriod(200);
	m_IdleCnt.SetPeriod(25000);
	m_IdleCnt.IncCounter(rand() % 6000);
	
	memset(m_aWorks, 0, sizeof (m_aWorks));
	memset(&m_NPCInfo, 0, sizeof (m_NPCInfo));
	memset(&m_BasicProps, 0, sizeof (m_BasicProps));
	memset(&m_ExtProps, 0, sizeof (m_ExtProps));

	m_nBrushes			= 0;
	m_ppBrushes			= NULL;
	m_pMonEssence		= NULL;
}

CECNPC::~CECNPC()
{
	delete m_pcdr;
}

//	Initlaize object
bool CECNPC::Init(int tid, const S2C::info_npc& Info)
{
	return true;
}

//	Release object
void CECNPC::Release()
{
	//	Release current skill if it exists
// 	if (m_pCurSkill)
// 	{
// 		delete m_pCurSkill;
// 		m_pCurSkill = NULL;
//	}

	//	Clear extend states before model is released
	SetNewExtendStates(0);
	m_aIconStates.RemoveAll();

	ReleaseNPCModel();
// 
// 	if (m_pPateName)
// 	{
// 		delete m_pPateName;
// 		m_pPateName = NULL;
// 	}
// 	
// 	if(m_pPateDesc)
// 	{
// 		delete m_pPateDesc;
// 		m_pPateDesc = NULL;
// 	}
// 	if (m_pPateOwner)
// 	{
// 		delete m_pPateOwner;
// 		m_pPateOwner = NULL;
// 	}
	ReleaseTraceBrush();

// 	if (m_pPateLastWords1)
// 	{
// 		delete m_pPateLastWords1;
// 		m_pPateLastWords1 = NULL;
// 	}
// 
// 	if (m_pPateLastWords2)
// 	{
// 		delete m_pPateLastWords2;
// 		m_pPateLastWords2 = NULL;
// 	}

	if (m_pBubbleTexts)
	{
		delete m_pBubbleTexts;
		m_pBubbleTexts = NULL;
	}
}

//	Load NPC model
bool CECNPC::LoadNPCModel(const char* szFile)
{
	//	Release old model
	ReleaseNPCModel();

	EC_NPCLOADRESULT Ret;
	if (!LoadNPCModel(m_NPCInfo.tid, szFile, Ret))
	{
		a_LogOutput(1, "CECNPC::LoadNPCModel, Failed to load NPC model");
		return false;
	}

	return true;
}

//	Set loaded model to NPC object, this function is used in multithread
//	loading process
bool CECNPC::SetNPCLoadedResult(const EC_NPCLOADRESULT& Ret)
{
	if (m_pNPCModel)
	{
		//	This case may happen like below way:
		//	- first, NPC(n) is created by NPC manager, it send resource loading
		//	  request to loading thread.
		//	- Immediately, for some reason, server tell NPC manager to delete NPC(n),
		//	  but in this time resources hasn't been loaded by loading thread.
		//	- Immediately, for some other reason, server create NPC(n) again, we
		//	  send another resource loading request.
		//	- the first resource loading request is responsed and model loaded
		//	- the second resource loading request is responsed.
		ReleaseNPCModel();
	}

	m_pNPCModel		= Ret.pModel;
	m_pNPCSkin		= Ret.pSkin;
	m_pNPCSkinLow	= Ret.pLowSkin;
	m_pCurSkin		= NULL;

	if (m_pNPCModel && (IsServerNPC() || IsCarrierNPC()))
		m_pNPCModel->GetA3DSkinModel()->ReplaceSkin(0, m_pNPCSkin, false);
	
	Ret.pModel->SetPos(GetPos());
	Ret.pModel->SetDirAndUp(GetDir(), GetUp());

	//	Force to update model once completely to avoid rendering error
	Ret.pModel->SetAutoUpdateFlag(false);
	Ret.pModel->Tick(1);
	Ret.pModel->SetAutoUpdateFlag(true);

	if (IsDead())
	{
		if (!(m_dwStates & GP_STATE_NPC_DELAYDEAD))
			Disappear();

		StartWork(WT_NORMAL, WORK_DEAD);
	}
	else if (m_iAction != -1)
	{
		//	make it stand or play a correct action	
		PlayModelAction(m_iAction, 1.0f);
	}
	else
	{
		if( IsMonsterOrPet() )
			PlayModelAction(ACT_STAND, 1.0f);
		else
			PlayModelAction(ACT_NPC_STAND, 1.0f);
	}

	// if it has a special born action, just play it. if not, this call will not stop current action
	if( m_bBornInSight )
	{
		if( m_pNPCModel && m_pNPCModel->GetComActByName(m_ActionNames.GetANSIString(ACT_COMMON_BORN)) )
			PlayModelAction(ACT_COMMON_BORN, 1.0f);
	}

	RebuildTraceBrush();
	
	//	Reset aabb extents
	m_pcdr->vExts	= GetPickAABB().Extents;

	return true;
}

//	Release NPC model
void CECNPC::ReleaseNPCModel()
{
	EC_NPCLOADRESULT Ret;
	Ret.pModel		= m_pNPCModel;
	Ret.pSkin		= m_pNPCSkin;
	Ret.pLowSkin	= m_pNPCSkinLow;
	ReleaseNPCModel(Ret);

	m_pNPCModel		= NULL;
	m_pNPCSkin		= NULL;
	m_pNPCSkinLow	= NULL;
	m_pCurSkin		= NULL;
}

//	Get NPC's aabb used to pick
const A3DAABB& CECNPC::GetPickAABB()
{
	if( m_pNPCModel && m_pNPCModel->HasCHAABB() )
	{
		m_aabbDefPick.Center = GetPos() + g_vAxisY * m_pNPCModel->GetCHAABB().Center.y;
		m_aabbDefPick.Extents = m_pNPCModel->GetCHAABB().Extents;
		m_aabbDefPick.CompleteMinsMaxs();
		return m_aabbDefPick;
	}
	
	if (!m_pNPCModel || !m_pNPCModel->GetA3DSkinModel())
	{
		m_aabbDefPick.Center = GetPos() + g_vAxisY * m_aabbDefPick.Extents.y;
		m_aabbDefPick.CompleteMinsMaxs();
		return m_aabbDefPick;
	}

	return m_pNPCModel->GetA3DSkinModel()->GetModelAABB();
}

//	Set last said words
void CECNPC::SetLastSaidWords(const ACHAR* szWords, int nEmotionSet)
{
// 	if (!m_pPateLastWords1)
// 	{
// 		m_pPateLastWords1 = new CECPateText;
// 		m_pPateLastWords1->EnableBackground(true);
// 	}
// 
// 	if (!m_pPateLastWords2)
// 		m_pPateLastWords2 = new CECPateText;
// 
// 	if (!m_pPateLastWords1 || !m_pPateLastWords2)
// 		return;
// 
// 	//-----------------------------------------------------
// 	//增加处理表情逻辑
// 	ACString str = FilterEmotionSet(szWords, nEmotionSet);
// 	szWords = str;
// 	EditBoxItemsSet ItemsSet;
// 	ACString strFilterEmotion = UnmarshalEditBoxText( str, ItemsSet );
// 	//-----------------------------------------------------
// 
// 	int iWholeLen = a_strlen( strFilterEmotion );	//如果szWords中含有表情符号,则不能直接计算字符串长度,而是先转换表情符号Modified from a_strlen(szWords);
// 	int iLen1 = m_pPateLastWords1->SetText(szWords, true, false);
// 
// 	if (iLen1 < iWholeLen)
// 		m_pPateLastWords2->SetText(&szWords[iLen1], true, true);
// 	else
// 		m_pPateLastWords2->Clear();

	m_iLastSayCnt = 20000;
}

//	Tick routine
bool CECNPC::Tick(DWORD dwDeltaTime)
{
	CECObject::Tick(dwDeltaTime);

	DWORD dwRealTime = g_pGame->GetRealTickTime();

	if (m_pNPCModel && IsMonsterOrPet())// && m_LODCnt.IncCounter(dwRealTime))
	{
	//	m_LODCnt.Reset();

		float vScale = 0.08f;
		vScale = ((CECMonster *)this)->GetDBEssence()->size * 0.08f;

		if( m_pNPCSkinLow && m_pNPCSkin )
		{
			if( m_fDistToCamera < g_pGame->GetConfigs()->GetSceneLoadRadius() * vScale )
			{
				if (m_pCurSkin != m_pNPCSkin)
				{
					m_pNPCModel->GetA3DSkinModel()->ReplaceSkin(0, m_pNPCSkin, false);
					m_pCurSkin = m_pNPCSkin;
				}
			}
			else
			{
				if (m_pCurSkin != m_pNPCSkinLow)
				{
					m_pNPCModel->GetA3DSkinModel()->ReplaceSkin(0, m_pNPCSkinLow, false);
					m_pCurSkin = m_pNPCSkinLow;
				}
			}
		}
		else if( m_pNPCSkin )
		{
			if (m_pCurSkin != m_pNPCSkin)
			{
				m_pNPCModel->GetA3DSkinModel()->ReplaceSkin(0, m_pNPCSkin, false);
				m_pCurSkin = m_pNPCSkin;
			}
		}
	}

	//	Update last said words
// 	if (m_pPateLastWords1 && m_pPateLastWords1->GetItemNum() && m_iLastSayCnt)
// 	{
// 		m_iLastSayCnt -= (int)dwDeltaTime;
// 		if (m_iLastSayCnt <= 0)
// 		{
// 			m_iLastSayCnt = 0;
// 
// 			//	Clear string
// 			m_pPateLastWords1->Clear();
// 			m_pPateLastWords2->Clear();
// 		}
// 		else
// 		{
// 			m_pPateLastWords1->Tick(dwDeltaTime);
// 			m_pPateLastWords2->Tick(dwDeltaTime);
// 		}
// 	}
	
	if (m_idOwner && m_cOwnerType != OWNER_NONE)
	{
		BuildOwnerName();
	}

	if ((m_iSpecialState & CECPlayer::SPECIAL_STATE_FROZEN) == 0)
	{
		switch (m_iCurWork)
		{
		case WORK_STAND:	TickWork_Stand(dwDeltaTime);	break;
		case WORK_FIGHT:	TickWork_Fight(dwDeltaTime);	break;
		case WORK_SPELL:	TickWork_Spell(dwDeltaTime);	break;
		case WORK_DEAD:		TickWork_Dead(dwDeltaTime);		break;
		case WORK_MOVE:		TickWork_Move(dwDeltaTime);		break;
		}
	}

	if (m_pNPCModel)
	{
		m_pNPCModel->SetPos(GetPos());
		m_pNPCModel->Tick(dwDeltaTime);
	}

	//	Calculate distance to host player
	CECHostPlayer* pHost = g_pGame->GetGameRun()->GetHostPlayer();
	if (pHost && pHost->IsSkeletonReady())
	{
		m_fDistToHost  = CalcDist(pHost->GetPos(), true);
		m_fDistToHostH = CalcDist(pHost->GetPos(), false);
	}

	//	Sort icon state
	if (m_aIconStates.GetSize() > 1 && (m_dwISTimeCnt += dwRealTime) >= 3000)
	{
		m_dwISTimeCnt = 0;
		//qsort(m_aIconStates.GetData(), m_aIconStates.GetSize(), sizeof (WORD), glb_WordCompare);
	}

	//	Increase disappear counter if it has been triggered
	if (m_DisappearCnt.GetCounter())
	{
		//	When m_DisappearCnt passed half length, start changing model's transparence
		DWORD dwOldCnt = m_DisappearCnt.GetCounter();
		m_DisappearCnt.IncCounter(dwDeltaTime);
		DWORD dwHalf = m_DisappearCnt.GetPeriod() / 2;
		
		if (dwOldCnt < dwHalf && m_DisappearCnt.GetCounter() >= dwHalf)
			SetTransparence(-1.0f, 1.0f, dwHalf);
	}

	if (m_bDim)
	{
		if (m_pNPCModel)
			m_pNPCModel->GetA3DSkinModel()->SetTransparent(0.7f);
	}

	//	Change model's transparence
	if (!m_TransCnt.IsFull() && m_fDstTrans != m_fCurTrans)
	{
		m_fCurTrans += m_fTransDelta * dwDeltaTime;
		if (m_fTransDelta > 0.0f)
			a_ClampRoof(m_fCurTrans, m_fDstTrans);
		else
			a_ClampFloor(m_fCurTrans, m_fDstTrans);

		if (m_pNPCModel)
		{
			if (!m_fCurTrans)
				m_pNPCModel->GetA3DSkinModel()->SetTransparent(-1.0f);
			else
				m_pNPCModel->GetA3DSkinModel()->SetTransparent(m_fCurTrans);
		}
	}

	if (m_pBubbleTexts)
		m_pBubbleTexts->Tick(dwDeltaTime);

	return true;
}

//	Internal render routine
bool CECNPC::InternalRender(CECViewport* pViewport, int iRenderFlag, bool bHighLight)
{
	if (IsHide())
	{
		return true;
	}
	m_PateContent.iVisible = 0;
#ifdef BACK_VERSION
	A3DCameraBase* pCamera = pViewport->GetA3DCamera();
	m_fDistToCamera	= CalcDist(pCamera->GetPos(), true);
#else
	A3DCameraBase* pCamera = pViewport->GetA3DCameraBase();

	if(iRenderFlag == CECObject::RD_NORMAL)
		m_fDistToCamera	= CalcDist(pCamera->GetPos(), true);
#endif

	if (!m_bVisible)
		return true;

	if( iRenderFlag == RD_NORMAL )
	{
		if (m_fDistToCamera < 30.0f)
			m_bCastShadow = true;
		else
			m_bCastShadow = false;
	}
	else
	{
		// only show nearby npc in reflect and refract and don't show shadow
#ifdef BACK_VERSION
		if (m_fDistToCamera > 30.0f)
			return true;

		A3DTerrainWater * pTerrainWater = g_pGame->GetGameRun()->GetWorld()->GetTerrainWater();
#else
		if (iRenderFlag != RD_SHADOW && m_fDistToCamera > 30.0f)
			return true;

		A3DTerrainWater2* pTerrainWater = g_pGame->GetGameRun()->GetWorld()->GetTerrainWater();
#endif
		if( iRenderFlag == RD_REFLECT && pTerrainWater->IsUnderWater(GetPickAABB().Maxs) )
			return true;
		else if( iRenderFlag == RD_REFRACT )
		{	
			bool bCamUnderWater = pTerrainWater->IsUnderWater(pCamera->GetPos());
			if( !bCamUnderWater && !pTerrainWater->IsUnderWater(GetPickAABB().Mins) )
				return true;
			else if( bCamUnderWater && pTerrainWater->IsUnderWater(GetPickAABB().Maxs) )
				return true;
		}

		m_bCastShadow = false;
	}

	if (m_pNPCModel)
	{
		if (bHighLight)
			g_pGame->RenderHighLightModel(pViewport, m_pNPCModel);
		else
#ifdef BACK_VERSION
			m_pNPCModel->Render(pViewport->GetA3DViewport());

		if (m_bCastShadow && g_pGame->GetShadowRender())
		{
			const A3DAABB& aabb = GetPickAABB();
			SHADOW_RECEIVER receiver = SHADOW_RECEIVER_TERRAIN; 
			if( !g_pGame->GetConfigs()->GetSystemSettings().bSimpleShadow )
				receiver = (SHADOW_RECEIVER)(SHADOW_RECEIVER_TERRAIN | SHADOW_RECEIVER_LITMODEL);
			g_pGame->GetShadowRender()->AddShadower(aabb.Center, aabb, SHADOW_RECEIVER_TERRAIN, NPCRenderForShadow, m_pNPCModel->GetA3DSkinModel(), g_pGame->GetConfigs()->GetSystemSettings().bSimpleShadow);
		}
#else
		{
			bool bRenderGfx = true;
			if(iRenderFlag == RD_SHADOW)
				bRenderGfx = false;
			m_pNPCModel->Render(pViewport->GetA3DViewport(), false, true, 0, NULL);

		}

		// 		if (m_bCastShadow && g_pGame->GetShadowRender())
		// 		{
		// 			const A3DAABB& aabb = GetPickAABB();
		// 			SHADOW_RECEIVER receiver = SHADOW_RECEIVER_TERRAIN; 
		// 			if( !g_pGame->GetConfigs()->GetSystemSettings().bSimpleShadow )
		// 				receiver = (SHADOW_RECEIVER)(SHADOW_RECEIVER_TERRAIN | SHADOW_RECEIVER_LITMODEL);
		// 			g_pGame->GetShadowRender()->AddShadower(aabb.Center, aabb, SHADOW_RECEIVER_TERRAIN, NPCRenderForShadow, m_pNPCModel->GetA3DSkinModel(), g_pGame->GetConfigs()->GetSystemSettings().bSimpleShadow);
		// 		}
#endif
	}

	if (iRenderFlag == RD_NORMAL)
	{
		//	Print NPC's name above header
		if (m_bRenderBar)
			RenderBars(pViewport);

		if (m_bRenderName)
			RenderName(pViewport, g_pGame->GetConfigs()->GetNPCTextFlags());

		//	Print NPC's ID
		if (g_pGame->GetConfigs()->IsNPCIDShown())
		{
			ACHAR szText[20];
			a_sprintf(szText, _AL("NPC: %x"), m_NPCInfo.nid);
			g_pGame->GetA3DEngine()->GetSystemFont()->TextOut3D(szText, pViewport->GetA3DViewport(),
				GetPos() + g_vAxisY * (GetPickAABB().Extents.y * 2.1f), 0, A3DCOLORRGB(255, 255, 0));
		}

		//	Print host's precise position
		if (g_pGame->GetConfigs()->GetShowPosFlag())
		{
			ACHAR szMsg[100];
			A3DVECTOR3 vPos = GetPos();
			a_sprintf(szMsg, _AL("%.3f, %.3f, %.3f"), vPos.x, vPos.y, vPos.z);
			vPos += g_vAxisY * (GetPickAABB().Extents.y * 2.8f);
			g_pGame->GetA3DEngine()->GetSystemFont()->TextOut3D(szMsg, pViewport->GetA3DViewport(), vPos, 0, 0xffffffff);

			a_sprintf(szMsg, _AL("%.3f, %.3f, %.3f"), m_vServerPos.x, m_vServerPos.y, m_vServerPos.z);
			vPos += g_vAxisY * (GetPickAABB().Extents.y * 3.5f);
			g_pGame->GetA3DEngine()->GetSystemFont()->TextOut3D(szMsg, pViewport->GetA3DViewport(), vPos, 0, 0xffffffff);
		}
	}

	return true;
}

//	Process message
bool CECNPC::ProcessMessage(const ECMSG& Msg)
{
	switch (Msg.dwMsg)
	{
	case MSG_NM_NPCATKRESULT:	OnMsgNPCAtkResult(Msg);		break;
	case MSG_NM_STARTATTACK:	OnMsgNPCStartAttack(Msg);	break;
	case MSG_NM_NPCEXTSTATE:	OnMsgNPCExtState(Msg);		break;
	case MSG_NM_NPCCASTSKILL:	OnMsgNPCCastSkill(Msg);		break;
	case MSG_NM_ENCHANTRESULT:	OnMsgNPCEnchantResult(Msg);	break;
	case MSG_NM_NPCROOT:		OnMsgNPCRoot(Msg);			break;
	case MSG_NM_NPCSKILLRESULT:	OnMsgNPCSkillResult(Msg);	break;
	}

	return true;
}

//	Move to a destination
void CECNPC::MoveTo(const S2C::cmd_object_move& Cmd)
{
	if (!Cmd.use_time)
		return;

	m_vServerPos	= Cmd.dest;
	m_vMoveDir		= Cmd.dest - GetPos();
	float fDist		= m_vMoveDir.Normalize();
	
	//	If destination position is too far to us, forcely pull player
	//	to that position.

// 	if (IsCarrierNPC())
// 	{
// 		char szMsg[400];
// 		sprintf(szMsg, "MOVE_TO: Dest: %f, %f, %f\t Dist: %f\t MoveDir: %d\n", Cmd.dest.x, Cmd.dest.y, Cmd.dest.z, fDist, glb_CompressDirH(m_vMoveDir.x, m_vMoveDir.z));
// 		OutputDebugStringA(szMsg);
// 	}

	if (fDist >= MAX_LAGDIST && !m_bIsMovieNPC)
	{
		SetPos(Cmd.dest);
		return;
	}

	int iMoveMode = Cmd.move_mode & GP_MOVE_MASK;

	if (IsCarrierNPC())
	{
		m_bStopMove = true;
		A3DVECTOR3 vTmpDir = m_vMoveDir;
		vTmpDir.y = 0;
		m_vStopDir = Normalize(vTmpDir);
	}
	else
		m_bStopMove = false;

	if (iMoveMode == GP_MOVE_PUSH)
	{
		//	Push back should occur in stop move command
		ASSERT(iMoveMode != GP_MOVE_PUSH);
		return;
	}

	m_pcdr->bTraceGround = true;

	if (Cmd.move_mode & GP_MOVE_AIR)
	{
		m_iMoveEnv = MOVEENV_AIR;
		m_pcdr->bTraceGround = false;
	}
	else if (Cmd.move_mode & GP_MOVE_WATER)
	{
		m_iMoveEnv = MOVEENV_WATER;
		m_pcdr->bTraceGround = false;
	}
	else
	{
		m_iMoveEnv = MOVEENV_GROUND;

		int iTemp = iMoveMode & GP_MOVE_MASK;
		if (iTemp == GP_MOVE_FALL || iTemp == GP_MOVE_FLYFALL)
			m_pcdr->bTraceGround = false;
	}

	if(!m_bIsMovieNPC)
		m_fMoveSpeed = fDist / (Cmd.use_time * 0.001f);

	//	Adjust NPC's direction
	A3DVECTOR3 vDir = m_vMoveDir;
	vDir.y = 0.0f;
	if (!vDir.IsZero())
	{
		vDir.Normalize();
		SetDestDirAndUp(vDir, g_vAxisY, 150);
	}

	bool bPlayAct = false;
	if (m_aWorks[WT_NORMAL] != WORK_MOVE)
	{
		StartWork(WT_NORMAL, WORK_MOVE, m_bPushBack ? 1 : 0);
		bPlayAct = true;
	}

	//	If NPC is told to move just after it was pushed back and
	//	didn't stop yet (in the case, m_aWorks[WT_NORMAL] == WORK_MOVE), 
	//	m_bPushBack will still be true. So move action should be played, 
	//	otherwise NPC will move	without action
	if (m_bPushBack)
	{
		m_bPushBack = false;
		bPlayAct = true;
	}

	if (bPlayAct)
	{
		//	Play run or walk aciton
		PlayMoveAction(iMoveMode);
	}
}

//	Stop moving to a destination position
void CECNPC::StopMoveTo(const S2C::cmd_object_stop_move& Cmd)
{
	if (IsDead())
		return;

	int iMoveMode = Cmd.move_mode & GP_MOVE_MASK;

	if (IsCarrierNPC() && Magnitude(Cmd.dest-GetPos()) < 1.0f)
	{
		// 直接返回，以免后面被强拉造成载体方向抖动而导致卡人的错误
//		OutputDebugStringA("Ignore too near stop command\n");
		return;
	}

	m_vMoveDir		= Cmd.dest - GetPos();
	m_bStopMove		= true;
	m_fMoveSpeed	= FIX8TOFLOAT(Cmd.sSpeed);
	m_vServerPos	= Cmd.dest;
	m_bPushBack		= iMoveMode == GP_MOVE_PUSH;
//	m_vStopDir		= glb_DecompressDirH(Cmd.dir);
	A3DVECTOR3 vTmpDir = m_vMoveDir;
	vTmpDir.y = 0;
	m_vStopDir = Normalize(vTmpDir);

	float fDist = m_vMoveDir.Normalize();

// 	if (IsCarrierNPC())
// 	{
// 		char szMsg[400];
// 		sprintf(szMsg, "STOP_MOVE_TO: Dest: %f, %f, %f\t Dist: %f\t, Speed: %f, MoveDir: %d\n", Cmd.dest.x, Cmd.dest.y, Cmd.dest.z, fDist, m_fMoveSpeed, glb_CompressDirH(m_vMoveDir.x, m_vMoveDir.z));
// 		OutputDebugStringA(szMsg);
// 	}

	//	In two case, we should forcely pull npc to destination position.
	//		1: If destination position is too far to npc.
	//		2: If destination position is very near to npc and npc's move-dir point
	//			to a different direction from npc's stop-dir (m_vStopDir)
	if (iMoveMode != GP_MOVE_RETURN && iMoveMode != GP_MOVE_PUSH)
	{
		bool bPull = false;
		
		if (fDist >= MAX_LAGDIST)
		{
			//	case 1
			bPull = true;
		}
		else if (fDist < 1.0f)
		{
			//	case 2
			A3DVECTOR3 vDirH = m_vMoveDir;
			vDirH.y = 0.0f;
			vDirH.Normalize();
			if (DotProduct(vDirH, m_vStopDir) < 0.7f)
				bPull = true;
		}

		if (bPull)
		{
			SetPos(Cmd.dest);
			SetDestDirAndUp(m_vStopDir, g_vAxisY, 150);
			WorkFinished(WORK_MOVE);
			return;
		}
	}

	m_pcdr->bTraceGround = true;

	if (Cmd.move_mode & GP_MOVE_AIR)
	{
		m_iMoveEnv = MOVEENV_AIR;
		m_pcdr->bTraceGround = false;
	}
	else if (Cmd.move_mode & GP_MOVE_WATER)
	{
		m_iMoveEnv = MOVEENV_WATER;
		m_pcdr->bTraceGround = false;
	}
	else
	{
		m_iMoveEnv = MOVEENV_GROUND;

		if (iMoveMode == GP_MOVE_FALL || iMoveMode == GP_MOVE_FLYFALL)
			m_pcdr->bTraceGround = false;
	}

	if (!m_bPushBack)
	{
		A3DVECTOR3 vDir = m_vMoveDir;
		vDir.y = 0.0f;
		if (!vDir.IsZero())
		{
			vDir.Normalize();
			SetDestDirAndUp(vDir, g_vAxisY, 150);
		}
	}

	if (m_aWorks[WT_NORMAL] != WORK_MOVE)
	{
		StartWork(WT_NORMAL, WORK_MOVE, m_bPushBack ? 1 : 0);

		if (!m_bPushBack)
		{
			//	Play run or walk aciton
			PlayMoveAction(iMoveMode);
		}
	}
}

//	Play move action
void CECNPC::PlayMoveAction(int iMoveMode)
{
	//	Play run or walk aciton
	if (iMoveMode == GP_MOVE_RUN || iMoveMode == GP_MOVE_RETURN)
	{
		if (IsMonsterOrPet())
			PlayModelAction(ACT_RUN, 1.0f, false);
		else
			PlayModelAction(ACT_NPC_RUN, 1.0f, false);
	}
	else
	{
		if (IsMonsterOrPet())
			PlayModelAction(ACT_WALK, 1.0f, false);
		else
			PlayModelAction(ACT_NPC_WALK, 1.0f, false);
	}
}

//	NPC ground move
bool CECNPC::MovingTo(DWORD dwDeltaTime)
{
	bool bRet = false;

	A3DVECTOR3 vPos, vCurPos = GetPos();
	float fDeltaTime = dwDeltaTime * 0.001f;
	CECWorld* pWorld = g_pGame->GetGameRun()->GetWorld();

	if (m_bStopMove)
	{
		A3DVECTOR3 vDir = m_vServerPos - vCurPos;
		float fDist = vDir.Normalize();
		vPos = MoveStep(vDir, m_fMoveSpeed, fDeltaTime);

		float fMoveDelta = a3d_Magnitude(vPos - vCurPos);
		if (fMoveDelta >= fDist)
		{
			SetPos(m_vServerPos);
			bRet = true;
		}
		else
			SetPos(vPos);		
	}
	else	//	Just move on
	{
		//	If we have move so far from destination and still don't 
		//	receive new 'move' or 'stop move' command, it's better to
		//	stop moving and goto last destination at once
		float fDist = a3d_Magnitude(m_vServerPos - vCurPos);
		if ((fDist >= MAX_LAGDIST && !m_bIsMovieNPC) || (m_bIsMovieNPC && fDist < 0.2f))
		{
			SetPos(m_vServerPos);
			m_vStopDir = GetDir();
			return true;
		}
	
		A3DVECTOR3 vDir = m_vMoveDir;
		vDir.Normalize();
		vPos = MoveStep(vDir, m_fMoveSpeed, fDeltaTime);

		if(m_bIsMovieNPC)
		{
			A3DVECTOR3 vGndPos, vTestPos = vPos + g_vAxisY * GetPickAABB().Extents.y;
			A3DVECTOR3 vGndNormal;
			VertRayTrace(vTestPos, vGndPos, vGndNormal);
			vPos.y = vGndPos.y;
		}

		SetPos(vPos);
	}

	//	test code ...
// 	if (IsCarrierNPC())
// 	{
// 		char szMsg[200] = "\0";
// 		sprintf(szMsg, "DIRECTION: %f, %f, %f\t", GetDir().x, GetDir().y, GetDir().z);
// 		sprintf(szMsg, "DEST: %f, %f, %f", Cmd.dest.x, Cmd.dest.y, Cmd.dest.z);
// 		OutputDebugStringA(szMsg);
//	}

	return bRet;
}

//	Move step
A3DVECTOR3 CECNPC::MoveStep(const A3DVECTOR3& vDir, float fSpeed, float fTime)
{
	A3DVECTOR3 vRealDir = vDir;

	//	OnAirMove only accept positive speed value
	if (fSpeed < 0.0f)
	{
		vRealDir = -vDir;
		fSpeed = -fSpeed;
	}

	m_pcdr->vCenter			= GetPos() + g_vAxisY * m_pcdr->vExts.y;
	m_pcdr->vVelocity		= vRealDir * fSpeed;
	m_pcdr->t				= fTime;
	m_pcdr->bTestTrnOnly	= true;

	OtherPlayerMove(*m_pcdr);

	if( m_pcdr->bTraceGround )
		SetGroundNormal(m_pcdr->vecGroundNormal);

	return m_pcdr->vCenter - g_vAxisY * m_pcdr->vExts.y;
}

//	NPC is moving ?
bool CECNPC::IsMoving()
{
	return m_iCurWork == WORK_MOVE ? true : false;
}

const char* CECNPC::GetActionName(int iAct)
{
	return m_ActionNames.GetANSIString(iAct);
}

//	Play model action
bool CECNPC::PlayModelAction(int iAction, float fWeight, bool bRestart/* true */,
							 int iTransTime/* 200 */)
{
	m_iAction = iAction;

	if (m_pNPCModel)
	{
		if (iAction == ACT_WOUNDED)
		{
			m_pNPCModel->GetA3DSkinModel()->PlayActionByName(m_ActionNames.GetANSIString(iAction), ACT_CHANNEL_WOUND);
		}
		else if(iAction == ACT_IDLE)
		{
			m_pNPCModel->PlayActionByName(m_ActionNames.GetANSIString(iAction), fWeight, bRestart, iTransTime);
			m_pNPCModel->QueueAction(m_ActionNames.GetANSIString(ACT_STAND));
		}
		// npc section
		else if(iAction == ACT_NPC_IDLE1 || iAction == ACT_NPC_IDLE2)
		{
			m_pNPCModel->PlayActionByName(m_ActionNames.GetANSIString(iAction), fWeight, bRestart, iTransTime);
			m_pNPCModel->QueueAction(m_ActionNames.GetANSIString(ACT_NPC_STAND));
		}
		else // normal action
			m_pNPCModel->PlayActionByName(m_ActionNames.GetANSIString(iAction), fWeight, bRestart, iTransTime);

		return true;
	}
	else
		return false;
}

//	Set absolute position
void CECNPC::SetPos(const A3DVECTOR3& vPos)
{
	CECObject::SetPos(vPos);

	if (m_pNPCModel)
		m_pNPCModel->SetPos(vPos);
}

//	Set absolute forward and up direction
void CECNPC::SetDirAndUp(const A3DVECTOR3& vDir, const A3DVECTOR3& vUp)
{
	CECObject::SetDirAndUp(vDir, vUp);

	if (m_pNPCModel)
		m_pNPCModel->SetDirAndUp(GetDir(), GetUp());
}

//	NPC was killed
void CECNPC::Killed(bool bDelay)
{
	if( m_pNPCModel )
		m_pNPCModel->ClearComActFlag(true);

	m_dwStates |= STATE_CORPSE;

	//	No delay die, enter disappear process immediately
	if (!bDelay || m_idSummoner)
		Disappear();

	StartWork(WT_NORMAL, WORK_DEAD, m_dwStates);

	SetUseGroundNormal(true);
}

void CECNPC::Disappear(bool bAtOnce)
{
	ReleaseTraceBrush();
	if(!bAtOnce)
	{
		//	Trigger disappear counting to start
		m_DisappearCnt.SetCounter(1);
		PlayModelAction(ACT_NPC_DISAPPEAR, 1.0f);
	}
}

//	NPC is damaged
void CECNPC::Damaged(int iDamage, int nDivisions, DWORD dwModifier/* 0 */)
{

	if (dwModifier & MOD_DEITY_DAMAGE )
	{
		if (iDamage > 0)
		{	
			int index = BUBBLE_DEITY_DAMAGE;
			bool bDeadlyStrike = (dwModifier & MOD_DT_CRITICAL_STRIKE) ? true : false;
			bool bRetort = (dwModifier & MOD_RETORT) ? true : false;
			PlayModelAction(ACT_WOUNDED, 1.0f);	

			int p1 = 0;
			if (bDeadlyStrike)
				p1 |= 0x0004;
			else if (bRetort)
				p1 |= 0x0002;
			
			if (nDivisions < 2)
			{
				BubbleText(index, (DWORD)iDamage, p1);
			}
			else
			{
				int* p = new int[nDivisions];
				_get_rand_damage(iDamage, nDivisions, p);

				for (int i = 0; i < nDivisions; i++)
					BubbleText(index, p[i], p1, i + 1 == nDivisions, true);

				delete[] p;
			}
		}
		return;
	}


	if (iDamage == -2)
	{
		// this message is caused by a help skill, so don't use a wounded action here
		if (dwModifier & MOD_IMMUNE)
			BubbleText(BUBBLE_IMMUNE, 0);
		else if (dwModifier & MOD_RESIST)
			BubbleText(BUBBLE_RESIST, 0);
		else if (dwModifier & MOD_NULLITY)
			BubbleText(BUBBLE_INVALIDHIT, 0);
	}
	else if (iDamage == -1)
	{
		// when else player hit this npc iDamage is -1, 
		// so if iDamage is -1 we will shoud the wounded animation
		PlayModelAction(ACT_WOUNDED, 1.0f);

		if (dwModifier & MOD_IMMUNE)
			BubbleText(BUBBLE_IMMUNE, 0);
		else if (dwModifier & MOD_RESIST)
			BubbleText(BUBBLE_RESIST, 0);
		else if (dwModifier & MOD_NULLITY)
			BubbleText(BUBBLE_INVALIDHIT, 0);
	}
	else
	{
		//	this message is related to the host, so we should show a pop up message
		//	Popup a damage decal
		bool bDeadlyStrike = (dwModifier & MOD_CRITICAL_STRIKE) ? true : false;
		bool bRetort = (dwModifier & MOD_RETORT) ? true : false;

		if (iDamage > 0)
		{
			PlayModelAction(ACT_WOUNDED, 1.0f);

			int p1 = 0;
			if (bDeadlyStrike)
				p1 |= 0x0001;
			else if (bRetort)
				p1 |= 0x0002;
			
			int index = dwModifier & MOD_SKILL ? BUBBLE_SKILL_DAMAGE : BUBBLE_DAMAGE;

			if (nDivisions < 2)
			{
				BubbleText(index, (DWORD)iDamage, p1);
			}
			else
			{
				int* p = new int[nDivisions];
				_get_rand_damage(iDamage, nDivisions, p);

				for (int i = 0; i < nDivisions; i++)
					BubbleText(index, p[i], p1, i + 1 == nDivisions, true);

				delete[] p;
			}
			
		}
		else if (dwModifier & MOD_IMMUNE)
			BubbleText(BUBBLE_IMMUNE, 0);
		else if (dwModifier & MOD_RESIST)
			BubbleText(BUBBLE_RESIST, 0);
		else if (dwModifier & MOD_NULLITY)
			BubbleText(BUBBLE_INVALIDHIT, 0);
		else
			BubbleText(BUBBLE_HITMISSED, 0);
	}
}

void CECNPC::PlayAttackEffect(int idTarget, int idSkill, int skillLevel, unsigned char cSerialId, int nDamage, DWORD dwModifier)
{
	if (!m_pNPCModel)
		return;

	m_bStartFight = true;

	// see if a melee attack
	if( !idSkill )
	{
		const char* szAct = ((rand() % 2) == 0 ? m_ActionNames.GetANSIString(ACT_ATTACK1) : m_ActionNames.GetANSIString(ACT_ATTACK2));
		m_pNPCModel->PlayAttackAction(szAct, cSerialId, m_NPCInfo.nid, idTarget, NULL);
		m_pNPCModel->QueueAction(m_ActionNames.GetANSIString(ACT_GUARD), 300);
	}
	else
	{
// 		if( skillLevel == 0 )
// 		{
// 			if( m_pCurSkill )
// 				skillLevel = m_pCurSkill->GetSkillLevel();
// 			else
// 				skillLevel = 1;
// 		}

//		char szAct[64];
//		sprintf(szAct, "%s", GNET::ElementSkill::GetAction(idSkill, 26));
//		m_pNPCModel->PlayAttackAction(szAct, cSerialId, m_NPCInfo.nid, idTarget, NULL);
//		m_pNPCModel->QueueAction(m_ActionNames.GetANSIString(ACT_GUARD), 300);
	}
}

//	Stop attack
void CECNPC::StopAttack()
{
	if (m_aWorks[WT_NORMAL] == WORK_FIGHT)
	{
		ReleaseWork(WT_NORMAL);

		if (m_iCurWorkType == WT_NORMAL)
		{
			m_iCurWorkType = WT_NOTHING;
			StartWorkByID(m_aWorks[WT_NOTHING], 0);
		}

		// signal early attack event
		if( m_pNPCModel )
			m_pNPCModel->ClearComActFlag(true);
	}
}

//	Release work resources of specified type
void CECNPC::ReleaseWork(int iWorkType)
{
	ASSERT(iWorkType >= 0 && iWorkType < NUM_WORKTYPE);

	switch (m_aWorks[iWorkType])
	{
	case WORK_STAND:	break;
	case WORK_FIGHT:	break;
	case WORK_SPELL:	break;
	case WORK_DEAD:		break;
	case WORK_MOVE:		break;
	}

	m_aWorks[iWorkType] = 0;

	if (m_iCurWorkType == iWorkType)
		m_iCurWork = 0;
}

//	Start a work of specified type
void CECNPC::StartWork(int iWorkType, int iNewWork, DWORD dwParam/* 0 */)
{
	ASSERT(iWorkType >= 0 && iWorkType < NUM_WORKTYPE);

	if (iNewWork == WORK_DEAD)
	{
		//	Dead is a special work
		ReleaseWork(WT_INTERRUPT);
		ReleaseWork(WT_NORMAL);

		m_aWorks[WT_NORMAL]	= iNewWork;
		m_iCurWorkType		= WT_NORMAL;
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

//	Start works 
void CECNPC::StartWorkByID(int iWorkID, DWORD dwParam)
{
	//	Ignore all message if this NPC is dead.
//	if (IsDead())
//		return;

	switch (iWorkID)
	{
	case WORK_STAND:	StartWork_Stand(dwParam);	break;
	case WORK_FIGHT:	StartWork_Fight(dwParam);	break;
	case WORK_SPELL:	StartWork_Spell(dwParam);	break;
	case WORK_DEAD:		StartWork_Dead(dwParam);	break;
	case WORK_MOVE:		StartWork_Move(dwParam);	break;
	}

	m_iCurWork = iWorkID;
}

//	Stop / pause work of specified type
void CECNPC::StopWork(int iWorkType)
{
}

//	Current work finished
void CECNPC::WorkFinished(int iWorkID)
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

//	Start work WORK_STAND
void CECNPC::StartWork_Stand(DWORD dwParam)
{
	if (!m_bStartFight)
	{
		if( IsMonsterOrPet() )
			PlayModelAction(ACT_STAND, 1.0f);
		else
			PlayModelAction(ACT_NPC_STAND, 1.0f);
	}
}

//	Start work WORK_FIGHT
void CECNPC::StartWork_Fight(DWORD dwParam)
{
	m_nFightTimeLeft = dwParam;
	// we should not play attack animation here, because all animation are controlled
	// through attack result messages.
}

//	Start work WORK_SPELL
void CECNPC::StartWork_Spell(DWORD dwParam)
{
}

//	Start work WORK_DEAD
void CECNPC::StartWork_Dead(DWORD dwParam)
{
	if (IsMonsterOrPet())
		PlayModelAction(ACT_DIE, 1.0f);
	else
		PlayModelAction(ACT_NPC_DIE, 1.0f);
}

//	Start work WORK_MOVE
void CECNPC::StartWork_Move(DWORD dwParam)
{
	m_bStartFight = false;

	if( m_pNPCModel )
	{
		m_pNPCModel->ClearComActFlag(true);

		// when non-carrier NPC is moving, no collision detection needed.
		if (!IsCarrierNPC())
			ReleaseTraceBrush();
	}
}

void CECNPC::TickWork_Stand(DWORD dwDeltaTime)
{
	if( m_IdleCnt.IncCounter(dwDeltaTime) )
	{
		m_IdleCnt.Reset(0);

		if( IsMonsterOrPet() )
			PlayModelAction(ACT_IDLE, 1.0f);	
		else
			PlayModelAction(ACT_IDLE, 1.0f); //PlayModelAction(ACT_NPC_IDLE1 + (rand() % 2), 1.0f);
	}
}

void CECNPC::TickWork_Fight(DWORD dwDeltaTime)
{
	m_nFightTimeLeft -= dwDeltaTime;

	if (m_nFightTimeLeft < 0)
	{
		m_nFightTimeLeft = 0;
		WorkFinished(WORK_FIGHT);
		return;
	}

	//	Face to destination
	NPCTurnFaceTo(m_idAttackTarget, 100);
	
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
}

void CECNPC::TickWork_Spell(DWORD dwDeltaTime)
{
	int iRealTime = g_pGame->GetRealTickTime();
	if (m_SkillCnt.IncCounter(iRealTime))
	{
		WorkFinished(WORK_SPELL);

// 		if (m_pCurSkill)
// 		{
// 			//	Release current skill
// 			delete m_pCurSkill;
// 			m_pCurSkill = NULL;
// 		}

		m_idCurSkillTarget = 0;
	}
	else
	{
		if (m_idCurSkillTarget && m_idCurSkillTarget != m_NPCInfo.nid)
			NPCTurnFaceTo(m_idCurSkillTarget);
	}
}

void CECNPC::TickWork_Dead(DWORD dwDeltaTime)
{
}

void CECNPC::TickWork_Move(DWORD dwDeltaTime)
{
	//	Move NPC
	if (m_bAboutToDie)
	{
		WorkFinished(WORK_MOVE);
	}
	else if (MovingTo(dwDeltaTime))
	{
		SetDestDirAndUp(m_vStopDir, g_vAxisY, 150);
		WorkFinished(WORK_MOVE);

		// when stopped, we should rebuild the convex brushes for collision detection.
		RebuildTraceBrush();
	}
}

void CECNPC::OnMsgNPCAtkResult(const ECMSG& Msg)
{

}

void CECNPC::OnMsgNPCStartAttack(const ECMSG& Msg)
{

}

void CECNPC::OnMsgAttackHostResult(int idHost, int nDamage, int nFlag, int nSpeed)
{
	ASSERT(FALSE);
	/*
	if( !IsDead() )
	{
		//	Face to target
		NPCTurnFaceTo(idHost);
		m_idAttackTarget = idHost;

		//	now start a fight work
		if (IsMonsterNPC())
		{
			CECMonster * pMonster = (CECMonster *) this;
			const MONSTER_ESSENCE * pMonsterEssence = pMonster->GetDBEssence();
			StartWork(WT_NORMAL, WORK_FIGHT, DWORD(pMonsterEssence->attack_speed * 1000));
		}
	}
	PlayAttackEffect(idHost, 0, 0, nDamage, nFlag, nSpeed);
	*/
}

void CECNPC::OnMsgNPCSkillResult(const ECMSG& Msg)
{

}

void CECNPC::OnMsgNPCExtState(const ECMSG& Msg)
{

}

void CECNPC::OnMsgNPCCastSkill(const ECMSG& Msg)
{
	
}

void CECNPC::OnMsgNPCEnchantResult(const ECMSG& Msg)
{
	
}

void CECNPC::OnMsgNPCRoot(const ECMSG& Msg)
{

}

//	Render titles / names / talks above player's header
bool CECNPC::RenderName(CECViewport* pViewport, DWORD dwFlags)
{
	if (!FillPateContent(pViewport))
		return true;
	return true;
}

//	Render bar above player's header
bool CECNPC::RenderBars(CECViewport* pViewport)
{
	if (!FillPateContent(pViewport))
		return true;

	int x, y, cx=100, cy=3;
	A3DFlatCollector* pFlat = g_pGame->GetA3DEngine()->GetA3DFlatCollector();
	CECConfigs* pConfigs = g_pGame->GetConfigs();
	DWORD colEmpty = A3DCOLORRGB(234, 159, 90);

	x = m_PateContent.iBaseX - (cx >> 1);
	y = m_PateContent.iCurY - 3;

	//	Draw HP bar
	if (pConfigs->GetVideoSettings().bPlayerHP)
	{
		if (m_ExtProps.max_hp && m_BasicProps.iCurHP)
		{
			int iLen = (int)ceil(cx * ((float)m_BasicProps.iCurHP / m_ExtProps.max_hp));
			a_Clamp(iLen, 1, cx);
			pFlat->AddRect_2D(x, y, x+cx-1, y+cy-1, colEmpty);
			pFlat->AddRect_2D(x, y, x+iLen-1, y+cy-1, A3DCOLORRGB(255, 57, 62));
		}

		y -= cy + 1;
	}

	m_PateContent.iCurY	= y;

	return true;
}

//	Set new visible extend states
void CECNPC::SetNewExtendStates(__int64 dwNewStates)
{
	static const char* szBasePath = "策划联入\\状态效果\\";
	AString strGFXFile;
	
	m_dwExtStates = dwNewStates;
}

//	Fill pate content
bool CECNPC::FillPateContent(CECViewport* pViewport)
{
	//	If m_PateContent.iVisible != 0, pate content has been filled
	//	for this frame
	if (m_PateContent.iVisible)
		return m_PateContent.iVisible == 2 ? true : false;

	A3DVIEWPORTPARAM* pViewPara = pViewport->GetA3DViewport()->GetParam();

	//	Calculate basic center position on screen
	const A3DAABB& aabb = GetPickAABB();
	A3DVECTOR3 vPos = GetPos();
	vPos.y = aabb.Center.y + aabb.Extents.y * 1.15f;
	
	A3DMATRIX4 matVPS = pViewport->GetA3DViewport()->GetCamera()->GetVPTM() * pViewport->GetA3DViewport()->GetViewScale();
	A3DVECTOR4 vScrPos = TransformToScreen(vPos, matVPS);

	if (vScrPos.z < pViewPara->MinZ || vScrPos.z > pViewPara->MaxZ)
	{
		m_PateContent.iVisible = 1;		//	In-visible
		return false;
	}

	m_PateContent.iVisible	= 2;
	m_PateContent.iBaseX	= (int)vScrPos.x;
	m_PateContent.iBaseY	= (int)vScrPos.y;
	m_PateContent.iCurY		= m_PateContent.iBaseY;
	m_PateContent.z			= vScrPos.z;
	m_PateContent.rhw		= vScrPos.w;

	return true;
}

//	Set NPC's transparence
void CECNPC::SetTransparence(float fCur, float fDest, DWORD dwTime)
{
	if (fCur >= 0.0f)
	{
		m_fCurTrans = fCur;

		if (m_pNPCModel)
			m_pNPCModel->GetA3DSkinModel()->SetTransparent(fCur);
	}

	m_fDstTrans		= fDest;
	m_fTransDelta	= (fDest - m_fCurTrans) / dwTime;

	m_TransCnt.SetPeriod(dwTime);
	m_TransCnt.Reset();
}

bool CECNPC::RayTrace(ECRAYTRACE* pTraceInfo)
{
	A3DVECTOR3 vHitPos, vNormal;
	float fFraction;

	bool bRet = false;

	if( m_pNPCModel && m_pNPCModel->HasCHAABB() )
	{
		A3DMATRIX4 matToNPC = a3d_ViewMatrix(GetPos(), GetDir(), GetUp(), 0.0f);
		A3DVECTOR3 vecStart = pTraceInfo->vStart * matToNPC;
		A3DVECTOR3 vecDelta = a3d_VectorMatrix3x3(pTraceInfo->vDelta, matToNPC);

		const A3DAABB& aabb = m_pNPCModel->GetCHAABB();
		if (CLS_RayToAABB3(vecStart, vecDelta, aabb.Mins, aabb.Maxs,
					vHitPos, &fFraction, vNormal))
		{
			CHBasedCD::ConvexHullDataArray & chArray = m_pNPCModel->GetConvexHullData();

			for(int i=0; i<int(chArray.size()); i++)
			{
				CConvexHullData * pCH = chArray[i];
				const CFace * pFace;
				if( RayIntersectWithCH(vecStart, vecDelta, *pCH, &pFace, vHitPos, fFraction) )
				{
					if (fFraction < pTraceInfo->pTraceRt->fFraction)
					{
						vNormal = pFace->GetNormal();

						A3DMATRIX4 matNPC = GetAbsoluteTM();
						vHitPos = vHitPos * matNPC;
						vNormal = a3d_VectorMatrix3x3(vNormal, matNPC);

						pTraceInfo->pTraceRt->fFraction		= fFraction;
						pTraceInfo->pTraceRt->vHitPos		= vHitPos;
						pTraceInfo->pTraceRt->vPoint		= vHitPos;
						pTraceInfo->pTraceRt->vNormal		= vNormal;

						pTraceInfo->pECTraceRt->fFraction	= fFraction;
						pTraceInfo->pECTraceRt->iEntity		= ECENT_NPC;
						pTraceInfo->pECTraceRt->iObjectID	= GetNPCID();
						pTraceInfo->pECTraceRt->vNormal		= vNormal;

						bRet = true;
					}
				}
			}
		}
	}
	else
	{
		const A3DAABB& aabb = GetPickAABB();
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
				pTraceInfo->pECTraceRt->iEntity		= ECENT_NPC;
				pTraceInfo->pECTraceRt->iObjectID	= GetNPCID();
				pTraceInfo->pECTraceRt->vNormal		= vNormal;

				bRet = true;
			}
		}
	}

	if (bRet && IsCarrierNPC())
		pTraceInfo->pECTraceRt->iEntity = ECENT_CARRIER;

	return bRet;
}

bool CECNPC::TraceWithBrush(BrushTraceInfo * pInfo)
{
	if( !m_pNPCModel || !m_pNPCModel->HasCHAABB() || m_nBrushes == 0 )
		return false;

	BrushTraceInfo info = *pInfo;

	A3DAABB aabb = m_pNPCModel->GetCHAABB();
	aabb.Center += GetPos();
	aabb.CompleteMinsMaxs();   // it is necessary
	if (m_pNPCModel->GetA3DSkinModel())
	{
		A3DAABB aabb2 = m_pNPCModel->GetA3DSkinModel()->GetModelAABB();
		aabb.Merge(aabb2);
	}
	aabb.Extents+=A3DVECTOR3(1.0f, 1.0f, 1.0f);
//	aabb.Center += GetPos();
	aabb.CompleteMinsMaxs();

	A3DVECTOR3	vPoint, vNormal;
	float		fFraction;		//	Fraction
	if (!IsCarrierNPC())
	{
		if (pInfo->bIsPoint && !CLS_RayToAABB3(pInfo->vStart, pInfo->vDelta, aabb.Mins, aabb.Maxs, vPoint, &fFraction, vNormal ) ) 
			return false;
		if (!pInfo->bIsPoint && !CLS_AABBAABBOverlap(aabb.Center, aabb.Extents, pInfo->BoundAABB.Center, pInfo->BoundAABB.Extents) )
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
		if (m_ppBrushes[i]->Trace(&info) && (info.fFraction < fFraction)) {
				//update the saving info
				bStartSolid = info.bStartSolid;
				bAllSolid = info.bAllSolid;
				iClipPlane = info.iClipPlane;
				fFraction = info.fFraction;
				vNormal = info.ClipPlane.GetNormal();
				fDist = info.ClipPlane.GetDist();

				bCollide = true;
		}
	}	

	if (bCollide && !bStartSolid) 
	{
		*pInfo = info;
		pInfo->bStartSolid = bStartSolid;
		pInfo->bAllSolid = bAllSolid;
		pInfo->iClipPlane = iClipPlane;
		pInfo->fFraction = fFraction;
		pInfo->ClipPlane.SetNormal(vNormal);
		pInfo->ClipPlane.SetD(fDist);
	}
	return bCollide;
}

bool CECNPC::RebuildTraceBrush()
{
	// monsters and pets need not to do trace with brush, so don't build the brushes here.
	if (IsMonsterNPC())
	{
		return true;
	}
	else if (IsPetNPC())
	{
		return true;
	}
	
	// first release old ones.
	if( m_ppBrushes )
	{
		for(int i=0; i<m_nBrushes; i++)
		{
			if( m_ppBrushes[i] )
			{
				delete m_ppBrushes[i];
				m_ppBrushes[i] = NULL;
			}
		}

		delete [] m_ppBrushes;
		m_ppBrushes = NULL;
		m_nBrushes = 0;
	}

	if( m_pNPCModel && m_pNPCModel->HasCHAABB() )
	{
		ConvexHullDataArray& cdArray = m_pNPCModel->GetConvexHullData();
		A3DMATRIX4 matTM = TransformMatrix(GetDir(), GetUp(), GetPos());

		m_nBrushes = cdArray.size();
		m_ppBrushes = new CCDBrush *[m_nBrushes];

		for(int i=0; i<m_nBrushes; i++)
		{
			CConvexHullData chData = *cdArray[i];

			if (!IsCarrierNPC())
				chData.Transform(matTM);

			CQBrush qBrush;
			qBrush.AddBrushBevels(&chData);
			CCDBrush * pCDBrush = new CCDBrush();
			qBrush.Export(pCDBrush);

			m_ppBrushes[i] = pCDBrush;
		}
	}

	return true;
}

bool CECNPC::ReleaseTraceBrush()
{
	if( m_ppBrushes )
	{
		for(int i=0; i<m_nBrushes; i++)
		{
			if( m_ppBrushes[i] )
			{
				delete m_ppBrushes[i];
				m_ppBrushes[i] = NULL;
			}
		}

		delete [] m_ppBrushes;
		m_ppBrushes = NULL;
		m_nBrushes = 0;
	}

	return true;
}

void CECNPC::NPCTurnFaceTo(int idTarget, DWORD dwTime)
{
	if (m_pMonEssence&& m_pMonEssence->id_strategy == 4)
		return;

	TurnFaceTo(idTarget, dwTime);
}

//	Print bubble text
void CECNPC::BubbleText(int iIndex, DWORD dwNum, int p1/* 0 */, bool bLastOne/* true */, bool bRandPos/* false*/)
{
}

void CECNPC::BuildOwnerName()
{
	const ACHAR* szOwnerName = g_pGame->GetGameRun()->GetPlayerName(m_idOwner,false);
	if (!szOwnerName)
	{
		static DWORD timeLast = 0;
		if( timeLast + 1000 < timeGetTime() )
		{
//			g_pGame->GetGameSession()->GetPlayerBriefInfo(1, &m_idOwner, 2);
			timeLast = timeGetTime();
		}
	}
	else
	{	
// 		if (m_cOwnerType == OWNER_APPRENTICE)
// 			m_strOwnerName.Format(g_pGame->GetFixedMsgTab()->GetWideString(FIXMSG_NPC_OWNER_NAME), szOwnerName);
// 		else
// 			m_strOwnerName.Format(_AL("<%s>"), szOwnerName);
//		m_pPateOwner->SetText(m_strOwnerName, false);
	}	
}

int CECNPC::GetSelectStrategy()
{
	if (m_pMonEssence)
		return m_pMonEssence->select_strategy;
	return 0;
}

void CECNPC::EnterDimState(unsigned char state)
{
	m_bDim = state? true:false;
	if(m_pNPCModel)
	{
		if (m_bDim)
			m_pNPCModel->GetA3DSkinModel()->SetTransparent(0.7f);
		else
			m_pNPCModel->GetA3DSkinModel()->SetTransparent(-1.0f);
	}
}

bool CECNPC::IsHide()
{
	if(m_bIsMovieNPC)
		return false;

	CECHostPlayer* pHostPlayer = g_pGame->GetGameRun()->GetHostPlayer();

	return false;
}

void CECNPC::UpdateSpecialState(int state, bool on)
{
	if (on)
		m_iSpecialState |= (1 << state);
	else 
		m_iSpecialState &= ~(1<< state);
	
	if (m_iSpecialState & CECPlayer::SPECIAL_STATE_FROZEN)
	{
		WorkFinished(WORK_MOVE);
		if ( m_pNPCModel )	//无论如何，先判断一下指针是否存在
		{
			m_pNPCModel->StopAllActions();
		}
	}
}

/**
*@param: fScale:缩放比例
*@return: bool:缩放操作是否成功
*@usage: 缩放整个NPC以及挂在NPC身上的gfx光效
**/
bool CECNPC::ScaleNPCAndGFX( float fScale )
{
	if ( m_pNPCModel )
	{
		m_pNPCModel->ScaleAllRootBonesAndGfx(fScale);
		int iChildModelCount = m_pNPCModel->GetChildCount();
		for ( int k = 0; k < iChildModelCount; ++k )
		{
			CECModel* pModel = m_pNPCModel->GetChildModel(k);
			if ( pModel )
			{
				pModel->ScaleAllRootBonesAndGfx(fScale);
			}
		}
	}
	else
	{
		return false;
	}

	//
	return true;
}

#ifdef BACK_VERSION
//	NPC's render for shadow call back function
bool NPCRenderForShadow(A3DViewport * pViewport, void * pArg)
{
	A3DSkinModel * pModel = (A3DSkinModel *) pArg;

	if( !pModel )
		return false;

	g_pGame->GetA3DDevice()->SetZTestEnable(true);
	g_pGame->GetA3DDevice()->SetZWriteEnable(true);
	g_pGame->GetA3DDevice()->SetAlphaTestEnable(true);
	g_pGame->GetA3DDevice()->SetAlphaBlendEnable(false);
	g_pGame->GetA3DDevice()->SetAlphaFunction(A3DCMP_GREATEREQUAL);
	g_pGame->GetA3DDevice()->SetAlphaRef(84);

	pModel->RenderAtOnce(pViewport, A3DSkinModel::RAO_NOMATERIAL, false);

	g_pGame->GetA3DDevice()->SetAlphaBlendEnable(true);
	g_pGame->GetA3DDevice()->SetAlphaTestEnable(false);
	g_pGame->GetA3DDevice()->SetZTestEnable(true);
	g_pGame->GetA3DDevice()->SetZWriteEnable(true);
	return true;
}
#endif