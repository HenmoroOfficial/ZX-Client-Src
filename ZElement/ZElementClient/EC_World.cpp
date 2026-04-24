/*
 * FILE: EC_World.cpp
 *
 * DESCRIPTION: 
 *
 * CREATED BY: Duyuxin, 2004/8/25
 *
 * HISTORY: 
 *
 * Copyright (c) 2004 Archosaur Studio, All Rights Reserved.
 */

#include "EC_Global.h"
#include "EC_World.h"
#include "EC_ManPlayer.h"
#include "EC_ManNPC.h"
#include "EC_ManMatter.h"
#include "EC_ManOrnament.h"
#include "EC_ManDecal.h"
#include "EC_ManSkillGfx.h"
#include "EC_MsgDef.h"
#include "EC_Game.h"
#include "EC_HostPlayer.h"
#include "EC_CDS.h"
#include "EC_ShadowRender.h"
#include "EC_GameRun.h"
#include "EC_Scene.h"
#include "EC_Configs.h"
#include "EC_Matter.h"
#include "EC_UIManager.h"
#include "EC_Utility.h"
#include "EC_SceneLoader.h"
#include "EC_GameUIMan.h"
#include "EC_GameSession.h"
#include "EL_Tree.h"
#include "EL_Forest.h"
#include "EL_Precinct.h"
#include "EL_Region.h"
#include "EL_BackMusic.h"
#include "EL_CloudManager.h"
#include "EC_Faction.h"
#include "EC_Instance.h"
#include "EC_TriangleMan.h"
#include "EC_BrushMan.h"
#include "EC_InputCtrl.h"
#include "EC_SceneLights.h"
#include "EC_CDR.h"
#include "EC_AutoMove.h"
#include "EC_AssureMove.h"
#include "EC_FixedMsg.h"
#include "EC_Cutscene.h"
#include "AnmSystem.h"
#include "EC_LoginUIMan.h"
#ifdef BACK_VERSION
#include "EC_Viewport.h"
#include "EC_SunMoon.h"
#include "EL_GrassLand.h"
#include "EL_GrassType.h"
#include "ELTerrainOutline2.h"
#else
#include "ECViewport.h"
#include "A3DSunMoon.h"
#include "A3DGrassland.h"
#include "A3DGrassType.h"
#include "A3DTerrainOutline2.h"
#include <elementRender.h>
#endif
#include "A3D.h"
#include <A3DTerrain2CullHC.h>
#include "AFI.h"
#include "A3DGFXExMan.h"
#include "AMVideoClip.h"

#include "terrain.h"
#include "AutoMove\MoveAgent.h"

#include "A3DTerrain2Blk.h"
#include "A3DSkyBox.h"
#include "A3DGrassInteractor.h"
#include "AUI\\AUIDialog.h"
#include "AUI\\AUICheckBox.h"
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
//	Implement CECWorld
//	
///////////////////////////////////////////////////////////////////////////

CECWorld::CECWorld(CECGameRun* pGameRun):m_PlayedCameraIndex(32), m_CtsBezierTab(16)
{
	m_pGameRun			= pGameRun;
	m_pA3DTerrain		= NULL;
	m_pA3DTerrainWater	= NULL;
	m_pA3DTrnCuller		= NULL;
	m_nSkyType			= 0;
	m_pA3DSky			= NULL;
	m_pSkyBox			= NULL;
	m_bWorldLoaded		= false;
	m_pCDS				= NULL;
	m_pForest			= NULL;
	m_pGrassLand		= NULL;
	m_pTerrainOutline	= NULL;
	m_pCloudManager		= NULL;
	m_pScene			= NULL;
	m_pSunMoon			= NULL;
	m_pRain				= NULL;
	m_pSnow				= NULL;
	m_bResetEnv			= true;
	m_pPrecinctSet		= NULL;
	m_pCurPrecinct		= NULL;
	m_pCurPrecinctMusic = NULL;
	m_pRegionSet		= NULL;
	m_pCurRegion		= NULL;
	m_pSceneLights		= NULL;
	m_pAssureMove		= NULL;
	m_pCursorStereoGfx	= NULL;
	m_pTargetStereoGfx	= NULL;
	m_bCenterReset		= false;
	m_dwBornStamp		= 1;
	m_idInst			= 0;

	m_fTrnLoadDelta		= 0.0f;
	m_fTrnLoadCnt		= 0.0f;
	m_fScnLoadDelta		= 0.0f;
	m_fScnLoadCnt		= 0.0f;

	m_dwPostEffectFlag	= 0;

// 	m_pAutoMoveMap      = NULL;
	
	memset(m_aManagers, 0, sizeof (m_aManagers));
	memset(m_aTerrainHoles, 0, sizeof(m_aTerrainHoles));
}

CECWorld::~CECWorld()
{
}

//	Initialize object
bool CECWorld::Init(int idInst)
{
	m_idInst = idInst;

	//	Create managers
	if (!CreateManagers())
	{
		a_LogOutput(1, "CECWorld::Init, Failed to create managers");
		return false;
	}

	//	Create CDS object
	if (!(m_pCDS = new CECCDS))
	{
		glb_ErrorOutput(ECERR_NOTENOUGHMEMORY, "CECWorld::Init", __LINE__);
		return false;
	}

	g_pGame->GetA3DEngine()->SetA3DCDS(m_pCDS);

	// Init nature objects
	if( !InitNatureObjects() )
	{
		a_LogOutput(1, "CECWorld::Init, Failed to init nature objects");
		return false;
	}

	//	Create precinct set
	if (!(m_pPrecinctSet = new CELPrecinctSet))
	{
		glb_ErrorOutput(ECERR_NOTENOUGHMEMORY, "CECWorld::Init", __LINE__);
		return false;
	}

	//	Create region set
	if (!(m_pRegionSet = new CELRegionSet))
	{
		glb_ErrorOutput(ECERR_NOTENOUGHMEMORY, "CECWorld::Init", __LINE__);
		return false;
	}

	//	Create scene light object
	if (!(m_pSceneLights = new CECSceneLights) || !m_pSceneLights->Init(g_pGame->GetA3DDevice()))
	{
		glb_ErrorOutput(ECERR_NOTENOUGHMEMORY, "CECWorld::Init", __LINE__);
		return false;
	}

	//	Create assure move object
	m_pAssureMove = new CECAssureMove();
#ifdef BACK_VERSION
	// set shadow render flags once.
	g_pGame->GetShadowRender()->SetCastShadowFlag(g_pGame->GetConfigs()->GetSystemSettings().bShadow);
#endif
	m_pCursorStereoGfx = g_pGame->GetA3DGFXExMan()->LoadGfx(g_pGame->GetA3DDevice(), "程序联入\\cursor_stereo.gfx", false);

	if (m_pCursorStereoGfx)
	{
		m_pCursorStereoGfx->Start(true);
		m_pCursorStereoGfx->SetVisible(false);
	}

	m_pTargetStereoGfx = g_pGame->GetA3DGFXExMan()->LoadGfx(g_pGame->GetA3DDevice(), "程序联入\\保护盾.gfx", false);

	if (m_pTargetStereoGfx)
	{
		m_pTargetStereoGfx->SetScale(.05f);
		m_pTargetStereoGfx->Start(true);
		m_pTargetStereoGfx->SetVisible(false);
	}

	m_PlayedCameraIndex.clear();
	m_CtsBezierTab.clear();

	return true;
}

//	Initialize terrain water
bool CECWorld::InitTerrainWater()
{
	//	Load terrain
#ifdef BACK_VERSION
	if (!(m_pA3DTerrainWater = new A3DTerrainWater))
#else
	if (!(m_pA3DTerrainWater = new A3DTerrainWater2))
#endif
	{
		glb_ErrorOutput(ECERR_NOTENOUGHMEMORY, "CECWorld::InitTerrainWater", __LINE__);
		return false;
	}
#ifdef BACK_VERSION
	if (!m_pA3DTerrainWater->InitStatically(g_pGame->GetA3DDevice(), g_pGame->GetViewport()->GetA3DViewport(), (A3DCamera*)(g_pGame->GetViewport()->GetA3DCamera())))
#else
	if (!m_pA3DTerrainWater->InitStatically(g_pGame->GetA3DDevice()))//, g_pGame->GetViewport()->GetA3DViewport(), (A3DCamera*)(g_pGame->GetViewport()->GetA3DCamera())))
#endif
	{
		glb_ErrorOutput(ECERR_FAILEDTOCALL, "CECWorld::InitTerrainWater", __LINE__);
		return false;
	}
#ifdef BACK_VERSION
	// set water effect once.
	m_pA3DTerrainWater->SetWaterType(WATER_TYPE_SCROLL);
	m_pA3DTerrainWater->SetCurEdgeHeight(0.2f);
	m_pA3DTerrainWater->SetRenderWavesFlag(false);
	m_pA3DTerrainWater->SetSimpleWaterFlag(!g_pGame->GetConfigs()->GetSystemSettings().nWaterEffect);
	m_pA3DTerrainWater->SetExpensiveWaterFlag(g_pGame->GetConfigs()->GetSystemSettings().bAdvancedWater);
	m_pA3DTerrainWater->SetLightSun(g_pGame->GetDirLight());
	if( g_pGame->GetGameRun()->GetInstance(m_idInst) )
		m_pA3DTerrainWater->SetColorWater(g_pGame->GetGameRun()->GetInstance(m_idInst)->GetColorWater());
#endif
	return true;
}

//	Initialize plants objects
bool CECWorld::InitPlantsObjects()
{
	//	Load Forest here;
	m_pForest = new CELForest();
	if (!m_pForest->Init(g_pGame->GetA3DDevice(), g_pGame->GetDirLight(), &g_Log))
	{
		glb_ErrorOutput(ECERR_FAILEDTOCALL, "CECWorld::InitPlantsObjects", __LINE__);
		return false;
	}
#ifdef BACK_VERSION
	m_pGrassLand = new CELGrassLand();
#else
	m_pGrassLand = new A3DGrassLand();
#endif
	if (!m_pGrassLand->Init(g_pGame->GetA3DDevice(), m_pA3DTerrain, &g_Log))
	{
		glb_ErrorOutput(ECERR_FAILEDTOCALL, "CECWorld::InitPlantsObjects", __LINE__);
		return false;
	}
	
	return true;
}

//	Initialize nature effects
bool CECWorld::InitNatureObjects()
{
	//	now create some nature effects here
#ifdef BACK_VERSION
	m_pSunMoon = new CECSunMoon();
#else
	m_pSunMoon = new A3DSunMoon(g_pGame->GetA3DDevice());
#endif
	if( !m_pSunMoon->Init() )
	{
		glb_ErrorOutput(ECERR_NOTENOUGHMEMORY, "CECWorld::InitNatureObjects", __LINE__);
		return false;
	}

	SYSTEMTIME sysTime;
	GetLocalTime(&sysTime);
	m_pSunMoon->SetTimeOfTheDay((sysTime.wHour * 3600 + sysTime.wMinute * 60 + sysTime.wSecond) / (24.0f * 3600.0f));
	
	m_pRain = new A3DRain();
	if( !m_pRain->Init(g_pGame->GetA3DDevice(), 1.0f) )
	{
		glb_ErrorOutput(ECERR_NOTENOUGHMEMORY, "CECWorld::InitNatureObjects", __LINE__);
		return false;
	}
	m_pRain->SetHostCamera(g_pGame->GetViewport()->GetA3DCamera());
	m_pRain->SetEmitterWidth(20.0f);
	m_pRain->SetEmitterLength(20.0f);
	m_pRain->SetEmitterHeight(8.0f);
	m_pRain->SetUseRateMode(20, true);
	m_pRain->SetSize(0.04f);
	m_pRain->SetSizeVariation(30.0f);
	m_pRain->SetSpeed(0.5f);
	m_pRain->SetSpeedVariation(50.0f);
	m_pRain->SetInheritInfluence(0.0f);
	m_pRain->SetLife(20);
	m_pRain->SetTextureMap("rain.tga");
	m_pRain->SetSrcBlend(A3DBLEND_SRCALPHA);
	m_pRain->SetDestBlend(A3DBLEND_INVSRCALPHA);
	m_pRain->CreateRain();
	m_pRain->SetColorNum(1);
	m_pRain->SetColor(0, A3DCOLORRGBA(255, 255, 255, 160));
	//m_pRain->StartRain();

	m_pSnow = new A3DSnow();
	if( !m_pSnow->Init(g_pGame->GetA3DDevice(), 1.8f) )
	{
		glb_ErrorOutput(ECERR_NOTENOUGHMEMORY, "CECWorld::InitNatureObjects", __LINE__);
		return false;
	}

	m_pSnow->SetHostCamera(g_pGame->GetViewport()->GetA3DCamera());
	m_pSnow->SetEmitterWidth(80.0f);
	m_pSnow->SetEmitterLength(80.0f);
	m_pSnow->SetEmitterHeight(40.0f);
	m_pSnow->SetUseRateMode(20, true);
	m_pSnow->SetSize(0.3f);
	m_pSnow->SetSizeVariation(20.0f);
	m_pSnow->SetSpeed(0.5f);
	m_pSnow->SetSpeedVariation(20.0f);
	m_pSnow->SetLife(100);
	m_pSnow->SetTextureMap("snow.bmp");
	m_pSnow->SetSrcBlend(A3DBLEND_ONE);
	m_pSnow->SetDestBlend(A3DBLEND_INVSRCCOLOR);
	m_pSnow->SetSpinTime(100);
	m_pSnow->SetSpinTimeVariation(50);
	m_pSnow->CreateSnow();
	m_pSnow->SetColor(0, A3DCOLORRGBA(160, 160, 160, 160));
	//m_pSnow->StartSnow();

	return true;
}

void CECWorld::ReleaseNatureObjects()
{
	if( m_pSunMoon )
	{
		m_pSunMoon->Release();
		delete m_pSunMoon;
		m_pSunMoon = NULL;
	}

	if( m_pRain )
	{
		m_pRain->Release();
		delete m_pRain;
		m_pRain = NULL;
	}

	if( m_pSnow )
	{
		m_pSnow->Release();
		delete m_pSnow;
		m_pSnow = NULL;
	}
}

// Render nature objects
bool CECWorld::RenderNatureObjects(CECViewport* pViewport)
{
	A3DVECTOR3 vecCamPos = pViewport->GetA3DViewport()->GetCamera()->GetPos();
	bool bCamBelowWater = m_pA3DTerrainWater->IsUnderWater(vecCamPos);
	// see if camera is under water, if so, just return without rendering these particles.
	if( bCamBelowWater )
		return true;

	if( m_pRain )
		m_pRain->RenderParticles(pViewport->GetA3DViewport());

	if( m_pSnow )
		m_pSnow->RenderParticles(pViewport->GetA3DViewport());
	
	return true;
}

// Tick nature objects
bool CECWorld::TickNatureObjects(DWORD dwDeltaTime)
{
	if( m_pSunMoon )
	{
		if( m_idInst == 0 )
		{
			// login world, must always be at midnoon
			m_pSunMoon->Tick(0);
		}
		else
			m_pSunMoon->Tick(dwDeltaTime);
	}

	static DWORD dwOldTicks = 0;
	dwOldTicks += dwDeltaTime;
	while(dwOldTicks > 33)
	{
		if( m_pRain )
			m_pRain->TickEmitting();

		if( m_pSnow )
			m_pSnow->TickEmitting();

		dwOldTicks -= 33;
	}

	return true;
}

//	Release object
void CECWorld::Release()
{
	//	Release CDS object
	if (m_pCDS)
	{
		g_pGame->GetA3DEngine()->SetA3DCDS(NULL);
		delete m_pCDS;
		m_pCDS = NULL;
	}

	// Release nature objects
	ReleaseNatureObjects();

	//	Release scene before managers
	ReleaseScene();

	//	Release managers
	ReleaseManagers();

	// force to release all loaded resource
	ThreadRemoveAllLoaded();

	if (m_pPrecinctSet)
	{
		delete m_pPrecinctSet;
		m_pPrecinctSet = NULL;
	}

	if (m_pRegionSet)
	{
		delete m_pRegionSet;
		m_pRegionSet = NULL;
	}

	if (m_pSceneLights)
	{
		delete m_pSceneLights;
		m_pSceneLights = NULL;
	}

	if (m_pAssureMove)
	{
		m_pAssureMove->ReleaseMap();
		delete m_pAssureMove;
		m_pAssureMove = NULL;
	}

	if (m_pCursorStereoGfx)
	{
		g_pGame->GetA3DGFXExMan()->CacheReleasedGfx(m_pCursorStereoGfx);
		m_pCursorStereoGfx = NULL;
	}

	if (m_pTargetStereoGfx)
	{
		g_pGame->GetA3DGFXExMan()->CacheReleasedGfx(m_pTargetStereoGfx);
		m_pTargetStereoGfx = NULL;
	}

	m_dwBornStamp = 1;

	m_PlayedCameraIndex.clear();
	

	//	Release all cutscene beziers
	CutsceneBezierTable::iterator it = m_CtsBezierTab.begin();
	for(; it != m_CtsBezierTab.end(); ++it)
	{
		CECBezier* pBezier = *it.value();
		delete pBezier;
	}
	m_CtsBezierTab.clear();

	g_Terrain.Release();
}

void CECWorld::ReleaseForAnm()
{
	//	Release CDS object
	if (m_pCDS)
	{
		g_pGame->GetA3DEngine()->SetA3DCDS(NULL);
		delete m_pCDS;
		m_pCDS = NULL;
	}

	// Release nature objects
	ReleaseNatureObjects();

	//	Release scene before managers
	ReleaseSceneForAnm();

	//	Release managers
	ReleaseManagers();

	// force to release all loaded resource
	ThreadRemoveAllLoaded();

	if (m_pPrecinctSet)
	{
		delete m_pPrecinctSet;
		m_pPrecinctSet = NULL;
	}

	if (m_pRegionSet)
	{
		delete m_pRegionSet;
		m_pRegionSet = NULL;
	}

	if (m_pSceneLights)
	{
		delete m_pSceneLights;
		m_pSceneLights = NULL;
	}

	if (m_pAssureMove)
	{
		m_pAssureMove->ReleaseMap();
		delete m_pAssureMove;
		m_pAssureMove = NULL;
	}

	if (m_pCursorStereoGfx)
	{
		g_pGame->GetA3DGFXExMan()->CacheReleasedGfx(m_pCursorStereoGfx);
		m_pCursorStereoGfx = NULL;
	}

	if (m_pTargetStereoGfx)
	{
		g_pGame->GetA3DGFXExMan()->CacheReleasedGfx(m_pTargetStereoGfx);
		m_pTargetStereoGfx = NULL;
	}

	m_dwBornStamp = 1;

	m_PlayedCameraIndex.clear();
	

	//	Release all cutscene beziers
	CutsceneBezierTable::iterator it = m_CtsBezierTab.begin();
	for(; it != m_CtsBezierTab.end(); ++it)
	{
		CECBezier* pBezier = *it.value();
		delete pBezier;
	}
	m_CtsBezierTab.clear();

	g_Terrain.Release();
}

void CECWorld::RestoreGameWorld()
{
	g_pGame->GetA3DEngine()->SetA3DCDS(m_pCDS);
	LoadMoveMaps();
#ifndef BACK_VERSION
	RegisterWorldObjects();
#endif
}

//	Load scene
bool CECWorld::LoadWorld(const char* szFile, const char * szDataPath, const A3DVECTOR3& vInitLoadPos)
{
	if (m_bWorldLoaded)
	{
		//	Scene couldn't be loaded twice
		a_LogOutput(1, "CECWorld::LoadWorld, scene couldn't be loaded twice");
		return false;
	}

	CECGameRun* pGameRun = g_pGame->GetGameRun();

	m_vInitLoadPos = vInitLoadPos;

	af_GetFilePath(szFile, m_strMapPath);

	//	Load terrain data
	const char* pExt = strrchr(szFile, '.');
	AString strResFile(szFile, pExt - szFile);
	strResFile += ".trn2";
	if (!LoadTerrain(strResFile))
	{
		glb_ErrorOutput(ECERR_FILEOPERATION, "CECWorld::LoadWorld", __LINE__);
		return false;
	}

	char szMapName[MAX_PATH];
	af_GetRelativePathNoBase(m_strMapPath, "maps\\", szMapName);

	//	Load assure move map
	float fWidth = m_pA3DTerrain->GetBlockColNum() * m_pA3DTerrain->GetBlockSize();
	float fHeight = m_pA3DTerrain->GetBlockRowNum() * m_pA3DTerrain->GetBlockSize();
	A3DVECTOR3 vecMapOrigin = A3DVECTOR3(-fWidth / 2.0f, 0.0f, fHeight / 2.0f);
	if( m_pAssureMove )
	{
		m_pAssureMove->LoadMap(szFile, vecMapOrigin, fWidth, fHeight);
		m_pAssureMove->StepLoadMap(0);
	}

	pGameRun->StepLoadProgress(2);

	//	Initialize terrain water
	if (!InitTerrainWater())
	{
		a_LogOutput(1, "CECWorld::LoadWorld, failed to initalize terrain water!");
		return false;
	}

	if( m_pAssureMove )
		m_pAssureMove->StepLoadMap(1);

	pGameRun->StepLoadProgress(2);

	//	Initialize plants objects
	if (!InitPlantsObjects())
	{
		a_LogOutput(1, "CECWorld::LoadWorld, failed to initalize plants objects!");
		return false;
	}

	if( m_pAssureMove )
		m_pAssureMove->StepLoadMap(2);

	pGameRun->StepLoadProgress(2);

	//	Load scene objects
	if (!LoadSceneObjects(szFile))
	{
		glb_ErrorOutput(ECERR_FILEOPERATION, "CECWorld::LoadWorld", __LINE__);
		return false;
	}

	if( m_pAssureMove )
		m_pAssureMove->StepLoadMap(3);

	pGameRun->StepLoadProgress(1);

	//	Load sky
	if (!LoadSky())
	{
		glb_ErrorOutput(ECERR_FILEOPERATION, "CECWorld::LoadWorld", __LINE__);
		return false;
	}

	if( m_pAssureMove )
		m_pAssureMove->StepLoadMap(4);

	pGameRun->StepLoadProgress(1);

	//	Load terrain's outline data
	char szTilePath[MAX_PATH];
	sprintf(szTilePath, "loddata\\%s\\olm\\", szMapName);

	// we will always use 1024 size birdviews to avoid seams if DXT1 is supported
	// or else, we use 256 size texture and the outline texture will smooth texture by copy adjacent edges together
	char szTexPath[MAX_PATH];
	HRESULT hval;
	hval = g_pGame->GetA3DDevice()->GetA3DEngine()->GetD3D()->CheckDeviceFormat(D3DADAPTER_DEFAULT, D3DDEVTYPE_HAL, (D3DFORMAT)g_pGame->GetA3DDevice()->GetDevFormat().fmtAdapter, 0, D3DRTYPE_TEXTURE, (D3DFORMAT) A3DFMT_DXT1);
	if( D3D_OK == hval )
		sprintf(szTexPath, "loddata\\%s\\birdviews\\1024\\", szMapName);
	else
		sprintf(szTexPath, "loddata\\%s\\birdviews\\256\\", szMapName);
	
	if (!LoadTerrainOutline(szTilePath, szTexPath))
		glb_ErrorOutput(ECERR_FILEOPERATION, "CECWorld::LoadWorld", __LINE__);
	
	//	Load clouds here
	strResFile = AString(szFile, pExt - szFile);
	strResFile += ".cld";
	strcat(szTexPath, "textures\\cumulus01.tga");
	if (!LoadClouds(strResFile, szTexPath) )
		glb_ErrorOutput(ECERR_FILEOPERATION, "CECWorld::LoadWorld", __LINE__);
	
	if( m_pAssureMove )
		m_pAssureMove->StepLoadMap(5);

	pGameRun->StepLoadProgress(2);
	
	//	Load precincts
	strResFile = szDataPath;
	strResFile += "\\precinct.clt";
	if (glb_FileExist(strResFile) && !m_pPrecinctSet->Load(strResFile))
		glb_ErrorOutput(ECERR_FILEOPERATION, "CECWorld::LoadWorld", __LINE__);

	//	Load regions
	strResFile = szDataPath;
	strResFile += "\\region.clt";
	if (glb_FileExist(strResFile) && !m_pRegionSet->Load(strResFile))
		glb_ErrorOutput(ECERR_FILEOPERATION, "CECWorld::LoadWorld", __LINE__);

	strResFile = m_strMapPath;
	strResFile += "\\scenelights.dat";
	if (glb_FileExist(strResFile) && !m_pSceneLights->Load(strResFile))
		glb_ErrorOutput(ECERR_FILEOPERATION, "CECWorld::LoadWorld", __LINE__);

	//  Load Camera beziers
	strResFile = m_strMapPath;
	strResFile += "\\";
	strResFile += szMapName;
	strResFile += ".cbezier";
	if (glb_FileExist(strResFile) && !LoadCutsceneBeziers(strResFile))
		glb_ErrorOutput(ECERR_FILEOPERATION, "CECWorld::LoadWorld", __LINE__);

	//	Set load center
	SetLoadCenter(vInitLoadPos);

	//	Notify managers
	for (int i=0; i < NUM_MANAGER; i++)
	{
		if (m_aManagers[i])
			m_aManagers[i]->OnEnterGameWorld();
	}

	//	If IsLoadThreadReady() == true, all resources have been loaded in
	//	SetLoadCenter();
	if (!IsLoadThreadReady())
		LoadInMainThread(true);

#ifdef USING_BRUSH_MAN
	GetOrnamentMan()->GetBrushMan()->Build(vInitLoadPos, true);
#endif

	m_bWorldLoaded = true;

	if( m_pAssureMove )
		m_pAssureMove->StepLoadMap(6);
	// now according to shadow render flag, we decide whether use triangle manager
#ifdef BACK_VERSION
	if( g_pGame->GetShadowRender() && g_pGame->GetShadowRender()->GetCastShadowFlag() )
#endif
		GetOrnamentMan()->LoadTriangleMan(vInitLoadPos);
	
	if (pGameRun->GetGameState() == CECGameRun::GS_GAME)
	{
		A3DVECTOR3 vPos = vInitLoadPos;
		vPos.y = GetTerrainHeight(vInitLoadPos);
		g_pGame->GetViewport()->GetA3DCamera()->SetPos(vPos + A3DVECTOR3(0.0f, 1.6f, 0.0f));

		InitLoaderThread();
		StartLoaderThread();
	}
	else
	{
		// login world always in day
		m_pSunMoon->SetTimeOfTheDay(0.5f);
	}


// 	AString strMove = m_strMapPath;
// 	strMove += "\\movemaps\\";
// 	g_AutoMoveAgent.Load(strMove);

 	if (!LoadMoveMaps())
 	{
		glb_ErrorOutput(ECERR_FILEOPERATION, "CECWorld::LoadWorld", __LINE__);
 	}

	//  Load terrain holes
	strResFile = m_strMapPath;
	strResFile += "\\";
	strResFile += szMapName;
	strResFile += ".hole";
	if (glb_FileExist(strResFile) && !LoadTerrainHole(strResFile))
		glb_ErrorOutput(ECERR_FILEOPERATION, "CECWorld::LoadWorld", __LINE__);

#ifndef BACK_VERSION
	RegisterWorldObjects();
#endif
	return true;
}

//	Load scene for animation
bool CECWorld::LoadWorldForAnm(const char* szFile, const char * szDataPath, const A3DVECTOR3& vInitLoadPos)
{
	if (m_bWorldLoaded)
	{
		//	Scene couldn't be loaded twice
		a_LogOutput(1, "CECWorld::LoadWorldForAnm, scene couldn't be loaded twice");
		return false;
	}

	CECGameRun* pGameRun = g_pGame->GetGameRun();

	m_vInitLoadPos = vInitLoadPos;

	af_GetFilePath(szFile, m_strMapPath);

	//	Load terrain data
	const char* pExt = strrchr(szFile, '.');
	AString strResFile(szFile, pExt - szFile);
	strResFile += ".trn2";
	if (!LoadTerrain(strResFile))
	{
		glb_ErrorOutput(ECERR_FILEOPERATION, "CECWorld::LoadWorldForAnm", __LINE__);
		return false;
	}

	char szMapName[MAX_PATH];
	af_GetRelativePathNoBase(m_strMapPath, "maps\\", szMapName);

	//	Load assure move map
	float fWidth = m_pA3DTerrain->GetBlockColNum() * m_pA3DTerrain->GetBlockSize();
	float fHeight = m_pA3DTerrain->GetBlockRowNum() * m_pA3DTerrain->GetBlockSize();
	A3DVECTOR3 vecMapOrigin = A3DVECTOR3(-fWidth / 2.0f, 0.0f, fHeight / 2.0f);
	if( m_pAssureMove )
	{
		m_pAssureMove->LoadMap(szFile, vecMapOrigin, fWidth, fHeight);
		m_pAssureMove->StepLoadMap(0);
	}

	pGameRun->StepLoadProgress(2);

	//	Initialize terrain water
	if (!InitTerrainWater())
	{
		a_LogOutput(1, "CECWorld::LoadWorldForAnm, failed to initalize terrain water!");
		return false;
	}

	if( m_pAssureMove )
		m_pAssureMove->StepLoadMap(1);

	pGameRun->StepLoadProgress(2);

	//	Initialize plants objects
	if (!InitPlantsObjects())
	{
		a_LogOutput(1, "CECWorld::LoadWorldForAnm, failed to initalize plants objects!");
		return false;
	}

	if( m_pAssureMove )
		m_pAssureMove->StepLoadMap(2);

	pGameRun->StepLoadProgress(2);

	//	Load scene objects
	if (!LoadSceneObjects(szFile))
	{
		glb_ErrorOutput(ECERR_FILEOPERATION, "CECWorld::LoadWorldForAnm", __LINE__);
		return false;
	}

	if( m_pAssureMove )
		m_pAssureMove->StepLoadMap(3);

	pGameRun->StepLoadProgress(1);

	//	Load sky
	if (!LoadSky())
	{
		glb_ErrorOutput(ECERR_FILEOPERATION, "CECWorld::LoadWorldForAnm", __LINE__);
		return false;
	}

	if( m_pAssureMove )
		m_pAssureMove->StepLoadMap(4);

	pGameRun->StepLoadProgress(1);

	//	Load terrain's outline data
	char szTilePath[MAX_PATH];
	sprintf(szTilePath, "loddata\\%s\\olm\\", szMapName);

	// we will always use 1024 size birdviews to avoid seams if DXT1 is supported
	// or else, we use 256 size texture and the outline texture will smooth texture by copy adjacent edges together
	char szTexPath[MAX_PATH];
	HRESULT hval;
	hval = g_pGame->GetA3DDevice()->GetA3DEngine()->GetD3D()->CheckDeviceFormat(D3DADAPTER_DEFAULT, D3DDEVTYPE_HAL, (D3DFORMAT)g_pGame->GetA3DDevice()->GetDevFormat().fmtAdapter, 0, D3DRTYPE_TEXTURE, (D3DFORMAT) A3DFMT_DXT1);
	if( D3D_OK == hval )
		sprintf(szTexPath, "loddata\\%s\\birdviews\\1024\\", szMapName);
	else
		sprintf(szTexPath, "loddata\\%s\\birdviews\\256\\", szMapName);
	
	if (!LoadTerrainOutline(szTilePath, szTexPath))
		glb_ErrorOutput(ECERR_FILEOPERATION, "CECWorld::LoadWorldForAnm", __LINE__);
	
	//	Load clouds here
	strResFile = AString(szFile, pExt - szFile);
	strResFile += ".cld";
	strcat(szTexPath, "textures\\cumulus01.tga");
	if (!LoadClouds(strResFile, szTexPath) )
		glb_ErrorOutput(ECERR_FILEOPERATION, "CECWorld::LoadWorldForAnm", __LINE__);
	
	if( m_pAssureMove )
		m_pAssureMove->StepLoadMap(5);

	pGameRun->StepLoadProgress(2);
	
	//	Load precincts
	strResFile = szDataPath;
	strResFile += "\\precinct.clt";
	if (glb_FileExist(strResFile) && !m_pPrecinctSet->Load(strResFile))
		glb_ErrorOutput(ECERR_FILEOPERATION, "CECWorld::LoadWorldForAnm", __LINE__);

	//	Load regions
	strResFile = szDataPath;
	strResFile += "\\region.clt";
	if (glb_FileExist(strResFile) && !m_pRegionSet->Load(strResFile))
		glb_ErrorOutput(ECERR_FILEOPERATION, "CECWorld::LoadWorldForAnm", __LINE__);

	strResFile = m_strMapPath;
	strResFile += "\\scenelights.dat";
	if (glb_FileExist(strResFile) && !m_pSceneLights->Load(strResFile))
		glb_ErrorOutput(ECERR_FILEOPERATION, "CECWorld::LoadWorldForAnm", __LINE__);

	//  Load Camera beziers
	strResFile = m_strMapPath;
	strResFile += "\\";
	strResFile += szMapName;
	strResFile += ".cbezier";
	if (glb_FileExist(strResFile) && !LoadCutsceneBeziers(strResFile))
		glb_ErrorOutput(ECERR_FILEOPERATION, "CECWorld::LoadWorldForAnm", __LINE__);

	//	Set load center
	SetLoadCenter(vInitLoadPos);

	//	Notify managers
	for (int i=0; i < NUM_MANAGER; i++)
	{
		if (m_aManagers[i])
			m_aManagers[i]->OnEnterGameWorld();
	}

	//	If IsLoadThreadReady() == true, all resources have been loaded in
	//	SetLoadCenter();
	if (!IsLoadThreadReady())
		LoadInMainThread(true);

#ifdef USING_BRUSH_MAN
	GetOrnamentMan()->GetBrushMan()->Build(vInitLoadPos, true);
#endif

	m_bWorldLoaded = true;

	if( m_pAssureMove )
		m_pAssureMove->StepLoadMap(6);

	// now according to shadow render flag, we decide whether use triangle manager
#ifdef BACK_VERSION
	if( g_pGame->GetShadowRender() && g_pGame->GetShadowRender()->GetCastShadowFlag() )
#endif
		GetOrnamentMan()->LoadTriangleMan(vInitLoadPos);
	
	if (pGameRun->GetGameState() == CECGameRun::GS_GAME)
	{
		A3DVECTOR3 vPos = vInitLoadPos;
		vPos.y = GetTerrainHeight(vInitLoadPos);
		g_pGame->GetViewport()->GetA3DCamera()->SetPos(vPos + A3DVECTOR3(0.0f, 1.6f, 0.0f));
	}
	else
	{
		// login world always in day
		m_pSunMoon->SetTimeOfTheDay(0.5f);
	}


// 	AString strMove = m_strMapPath;
// 	strMove += "\\movemaps\\";
// 	g_AutoMoveAgent.Load(strMove);

 	if (!LoadMoveMaps())
 	{
		glb_ErrorOutput(ECERR_FILEOPERATION, "CECWorld::LoadWorldForAnm", __LINE__);
 	}

	//  Load terrain holes
	strResFile = m_strMapPath;
	strResFile += "\\";
	strResFile += szMapName;
	strResFile += ".hole";
	if (glb_FileExist(strResFile) && !LoadTerrainHole(strResFile))
		glb_ErrorOutput(ECERR_FILEOPERATION, "CECWorld::LoadWorldForAnm", __LINE__);
#ifndef BACK_VERSION
	RegisterWorldObjects();
#endif
	return true;
}

bool CECWorld::LoadMoveMaps()
{
	AString path;
	path.Format("%s\\movemaps\\PfMap.cfg", m_strMapPath);

	if (!g_AutoMoveAgent.Load(path))
	{
		return false;
	}

	g_Terrain.Release();
	CECInstance * pInstance = g_pGame->GetGameRun()->GetInstance(m_idInst);
	if (!pInstance)
	{
		return false;
	}
	TERRAINCONFIG  terCfg;
	terCfg.nNumAreas = pInstance->GetRowNum() * pInstance->GetColNum();
	terCfg.nNumCols = pInstance->GetColNum();
	terCfg.nNumRows = pInstance->GetRowNum();
	terCfg.vGridSize = m_pA3DTerrain->GetGridSize();
	terCfg.nAreaWidth = m_pA3DTerrain->GetSubTerrainGrid();
	terCfg.nAreaHeight = m_pA3DTerrain->GetSubTerrainGrid();
	terCfg.vHeightMin = 0.0f;
	terCfg.vHeightMax = 800.0f;
	path.Format("%s\\movemaps", m_strMapPath);
	strcpy(terCfg.szMapPath, path);
	if (!g_Terrain.Init(terCfg, -12345.0f, -12345.0f, 12345.0f, 12345.0f))
	{
		g_Terrain.Release();
		return false;
	}
	return true;
}

//	Release current scene
void CECWorld::ReleaseScene()
{
	g_pGame->GetA3DEngine()->SetSky(NULL);

	A3DRELEASE(m_pScene);
	A3DRELEASE(m_pGrassLand);
	A3DRELEASE(m_pForest);
	A3DRELEASE(m_pA3DSky);
	A3DRELEASE(m_pSkyBox);

	// 1. force to exit loader thread
	ExitLoaderThread();

	// 2. release manager
	for (int i=0; i < NUM_MANAGER; i++)
	{
		if (m_aManagers[i])
			m_aManagers[i]->OnLeaveGameWorld();
	}
	
	if (GetHostPlayer())
	{
		GetHostPlayer()->OnLeaveGameWorld();
	}

	// 3. force to release all loaded resource
	ThreadRemoveAllLoaded();

	//	Release terrain after loading thread has been ended
	A3DRELEASE(m_pA3DTrnCuller);
	A3DRELEASE(m_pA3DTerrainWater);
	A3DRELEASE(m_pA3DTerrain);
	A3DRELEASE(m_pTerrainOutline);
	A3DRELEASE(m_pCloudManager);
	
	m_bWorldLoaded = false;
}

//	Release current scene for animation
void CECWorld::ReleaseSceneForAnm()
{
	g_pGame->GetA3DEngine()->SetSky(NULL);

	A3DRELEASE(m_pScene);
	A3DRELEASE(m_pGrassLand);
	A3DRELEASE(m_pForest);
	A3DRELEASE(m_pA3DSky);
	A3DRELEASE(m_pSkyBox);

	// 1. force to exit loader thread
	//ExitLoaderThread();

	// 2. release manager
	for (int i=0; i < NUM_MANAGER; i++)
	{
		if (m_aManagers[i])
			m_aManagers[i]->OnLeaveGameWorld();
	}
	
	//if (GetHostPlayer())
	//{
	//	GetHostPlayer()->OnLeaveGameWorld();
	//}

	// 3. force to release all loaded resource
	if (IsLoadThreadReady())
	{
		SuspendLoadThread();
		ThreadRemoveAllLoaded();
	}

	//	Release terrain after loading thread has been ended
	A3DRELEASE(m_pA3DTrnCuller);
	A3DRELEASE(m_pA3DTerrainWater);
	A3DRELEASE(m_pA3DTerrain);
	A3DRELEASE(m_pTerrainOutline);
	A3DRELEASE(m_pCloudManager);
	
	m_bWorldLoaded = false;
}

//	Load terrain
bool CECWorld::LoadTerrain(const char* szFile)
{
	//	Load terrain
	if (!(m_pA3DTerrain = new A3DTerrain2))
	{
		glb_ErrorOutput(ECERR_NOTENOUGHMEMORY, "CECWorld::LoadTerrain", __LINE__);
		return false;
	}

	float fRadius = g_pGame->GetConfigs()->GetSceneLoadRadius();
	if (!m_pA3DTerrain->Init(g_pGame->GetA3DEngine(), fRadius, fRadius))
	{
		glb_ErrorOutput(ECERR_FAILEDTOCALL, "CECWorld::LoadTerrain", __LINE__);
		return false;
	}

//	bool bMultiThread = g_pGame->GetGameRun()->GetGameState() == CECGameRun::GS_GAME ? true : false;
	bool bMultiThread = true;
	if (!m_pA3DTerrain->Load(szFile, 0.0f, 0.0f, bMultiThread))
	{
		a_LogOutput(1, "CECWorld::LoadTerrain, Failed to load A3DTerrain2.");
		return false;
	}

	CECConfigs* pConfigs = g_pGame->GetConfigs();

	m_pA3DTerrain->SetBlockLoadSpeed(4);
	//m_pA3DTerrain->SetMaxLayerWeight(pConfigs->GetSystemSettings().nGroundDetail);
	m_pA3DTerrain->SetLODType(A3DTerrain2::LOD_NOHEIGHT);
	m_pA3DTerrain->SetLODDist(pConfigs->GetTerrainLODDist1(), pConfigs->GetTerrainLODDist2());

	//	Create terrain culler
	if ((m_pA3DTrnCuller = new A3DTerrain2CullHC))
	{
		//	Try to load horizon culling data
		char szHCFile[MAX_PATH];
		strcpy(szHCFile, szFile);
		af_ChangeFileExt(szHCFile, MAX_PATH, ".t2hc");

		if (m_pA3DTrnCuller->Init(szHCFile))
		{
			m_pA3DTerrain->AttachTerrainCuller(m_pA3DTrnCuller);
		}
		else
		{
			A3DRELEASE(m_pA3DTrnCuller);
		//	a_LogOutput(1, "CECWorld::LoadTerrain, Failed to create horizon cull object");
		}
	}

	return true;
}

//	Load terrain outline
bool CECWorld::LoadTerrainOutline(const char* szTilePath, const char * szTexPath)
{
	const char * szDetailMap = "";

	CECInstance * pInstance = g_pGame->GetGameRun()->GetInstance(m_idInst);

	int w = 1, h = 1;
	if( pInstance )
	{
		w = pInstance->GetColNum();
		h = pInstance->GetRowNum();
		szDetailMap = pInstance->GetDetailTex();
	}
#ifdef BACK_VERSION
	m_pTerrainOutline = new CELTerrainOutline2();
	if( !m_pTerrainOutline->Init(g_pGame->GetA3DDevice(), szTexPath, szTilePath, szDetailMap, m_pA3DTerrain, w, h) )
	{
		delete m_pTerrainOutline;
		m_pTerrainOutline = NULL;
		return false;
	}

	switch( g_pGame->GetConfigs()->GetSystemSettings().nSight + 1 )
	{
	case 1:
		m_pTerrainOutline->SetEndDist(0, 400.0f);
		m_pTerrainOutline->SetEndDist(1, 500.0f);
		m_pTerrainOutline->SetEndDist(2, 600.0f);
		break;

	case 2:
		m_pTerrainOutline->SetEndDist(0, 800.0f);
		m_pTerrainOutline->SetEndDist(1, 900.0f);
		m_pTerrainOutline->SetEndDist(2, 1000.0f);
		break;

	case 3:	
		m_pTerrainOutline->SetEndDist(0, 800.0f);
		m_pTerrainOutline->SetEndDist(1, 900.0f);
		m_pTerrainOutline->SetEndDist(2, 1000.0f);
		break;

	case 4:
		m_pTerrainOutline->SetEndDist(0, 800.0f);
		m_pTerrainOutline->SetEndDist(1, 1200.0f);
		m_pTerrainOutline->SetEndDist(2, 1400.0f);
		break;

	case 5:
		m_pTerrainOutline->SetEndDist(0, 800.0f);
		m_pTerrainOutline->SetEndDist(1, 1200.0f);
		m_pTerrainOutline->SetEndDist(2, 1900.0f);
		break;
	}

	if( g_pGame->GetConfigs()->GetSystemSettings().bSimpleTerrain )
	{
		m_pTerrainOutline->SetReplaceTerrain(true);
	}
	else
	{
		m_pTerrainOutline->SetReplaceTerrain(false);
	}

	m_pTerrainOutline->SetViewScheme(g_pGame->GetConfigs()->GetSystemSettings().nSight);
#else
	//FIXME!!暂时关闭TerrainOutline,因为与现在的2.2不兼容. #define OUTLINE_TILE_GRID_LEN     1  这个值与原2.0引擎不同
	//if(false)
	{
		m_pTerrainOutline = new A3DTerrainOutline2();
		if( !m_pTerrainOutline->Init(g_pGame->GetA3DDevice(), szTexPath, szTilePath, szDetailMap, m_pA3DTerrain, w, h, 2) )
		{
			delete m_pTerrainOutline;
			m_pTerrainOutline = NULL;
			return false;
		}	

		switch( g_pGame->GetConfigs()->GetSystemSettings().nSight + 1 )
		{
		case 1:
			m_pTerrainOutline->SetEndDist(0, 400.0f);
			m_pTerrainOutline->SetEndDist(1, 500.0f);
			m_pTerrainOutline->SetEndDist(2, 600.0f);
			break;

		case 2:
			m_pTerrainOutline->SetEndDist(0, 800.0f);
			m_pTerrainOutline->SetEndDist(1, 900.0f);
			m_pTerrainOutline->SetEndDist(2, 1000.0f);
			break;

		case 3:	
			m_pTerrainOutline->SetEndDist(0, 800.0f);
			m_pTerrainOutline->SetEndDist(1, 900.0f);
			m_pTerrainOutline->SetEndDist(2, 1000.0f);
			break;

		case 4:
			m_pTerrainOutline->SetEndDist(0, 800.0f);
			m_pTerrainOutline->SetEndDist(1, 1200.0f);
			m_pTerrainOutline->SetEndDist(2, 1400.0f);
			break;

		case 5:
			m_pTerrainOutline->SetEndDist(0, 800.0f);
			m_pTerrainOutline->SetEndDist(1, 1200.0f);
			m_pTerrainOutline->SetEndDist(2, 1900.0f);
			break;
		//testmode
		case 51:
			m_pTerrainOutline->SetEndDist(0, 800.0f);
			m_pTerrainOutline->SetEndDist(1, 1200.0f);
			m_pTerrainOutline->SetEndDist(2, 2000.0f);
			break;
		}

		if( g_pGame->GetConfigs()->GetSystemSettings().bSimpleTerrain )
		{
			m_pTerrainOutline->SetReplaceTerrain(true);
		}
		else
		{
			m_pTerrainOutline->SetReplaceTerrain(false);
		}

		m_pTerrainOutline->SetViewScheme(g_pGame->GetConfigs()->GetSystemSettings().nSight);
	}
#endif
	return true;
}

//	Load clouds
bool CECWorld::LoadClouds(const char * szCloudFile, const char * szTexPath)
{
	m_pCloudManager = new CELCloudManager();

	if( !m_pCloudManager->Init(g_pGame->GetA3DDevice(), szCloudFile) )
	{
		m_pCloudManager->Release();
		delete m_pCloudManager;
		m_pCloudManager = NULL;
		return false;
	}

	return true;	
}

//	Load sky
bool CECWorld::LoadSky()
{
	//	Sphere type sky
	A3DSkySphere* pSky = new A3DSkySphere;
	if (!pSky)
		return false;

	//	Sky texture file name shouldn't be set here
	CECConfigs* pConfig = g_pGame->GetConfigs();
	if (!pSky->Init(g_pGame->GetA3DDevice(), NULL, pConfig->m_strDefSkyFile0, 
					pConfig->m_strDefSkyFile1, pConfig->m_strDefSkyFile2))
	{
		a_LogOutput(1, "CECWorld::LoadSky, Failed to create sphere sky !");
		return false;
	}

	pSky->SetFlySpeedU(0.003f);
	pSky->SetFlySpeedV(0.003f);

	m_pA3DSky = pSky;
	
	//	This enable sky can do animation when A3DEngine::TickAnimation is called
	if (m_pA3DSky)
		g_pGame->GetA3DEngine()->SetSky(m_pA3DSky);


	//	Sky box
	A3DSkyBox* pSkyBox = new A3DSkyBox;
	if (!pSkyBox)
		return false;

	//	Sky texture file name shouldn't be set here
	if (!pSkyBox->Init(g_pGame->GetA3DDevice(), NULL, pConfig->m_strDefSkyFile0, pConfig->m_strDefSkyFile0,
		pConfig->m_strDefSkyFile0, pConfig->m_strDefSkyFile0, 
		pConfig->m_strDefSkyFile0, pConfig->m_strDefSkyFile0))
	{
		a_LogOutput(1, "CECWorld::LoadSky, Failed to create sky box!");
		return false;
	}

	m_pSkyBox = pSkyBox;

	return true;
}

bool CECWorld::LoadPlants(const char * szFile)
{
	//	Load Forest here;
	m_pForest = new CELForest();
	if( !m_pForest->Init(g_pGame->GetA3DDevice(), g_pGame->GetDirLight(), &g_Log) )
	{
		glb_ErrorOutput(ECERR_FILEOPERATION, "CECWorld::LoadPlants", __LINE__);
		return false;
	}
	// temp code, we should get tree type's information and call pForest->AddTreeType
	// before we can use it.
	if( !m_pForest->LoadTypesFromConfigFile("trees\\trees.cfg") )
		return false;
#ifdef BACK_VERSION
	m_pGrassLand = new CELGrassLand();
#else
	m_pGrassLand = new A3DGrassLand();
#endif
	if( !m_pGrassLand->Init(g_pGame->GetA3DDevice(), m_pA3DTerrain, &g_Log) )
	{
		glb_ErrorOutput(ECERR_FILEOPERATION, "CECWorld::LoadPlants", __LINE__);
		return false;
	}

	// test code, load from a file.
	m_pGrassLand->Load("maps\\world\\world.gld");
	return true;
}

//	Load scene objects
bool CECWorld::LoadSceneObjects(const char* szFile)
{
	if (!(m_pScene = new CECScene(this)))
	{
		glb_ErrorOutput(ECERR_NOTENOUGHMEMORY, "CECWorld::LoadSceneObjects", __LINE__);
		return false;
	}

	if (!m_pScene->Load(szFile, 0.0f, 0.0f))
	{
		a_LogOutput(1, "CECWorld::LoadSceneObjects, Failed to load scene file %s", szFile);
		return false;
	}

	float fRadius = g_pGame->GetConfigs()->GetSceneLoadRadius();
	m_pScene->SetActRadius(2000.0f);//fRadius);
	m_pScene->SetBlockLoadSpeed(2);

	return true;
}

//  Load cutscene beziers
bool CECWorld::LoadCutsceneBeziers(const char* szFile)
{
	AFile cbezierFile;
	if (!cbezierFile.Open((""), szFile, AFILE_OPENEXIST | AFILE_BINARY | AFILE_TEMPMEMORY))
	{
		a_LogOutput(1, ("CECWorld::LoadCutsceneBeziers, Failed to open file %s"), szFile);
		return false;
	}

	while(cbezierFile.GetPos() < cbezierFile.GetFileLength())
	{
		//	Create a bezier curve object
		CECBezier* pBezier = new CECBezier;
		if (!pBezier)
			return false;

		if (!pBezier->Load(&cbezierFile))
		{
			delete pBezier;
			a_LogOutput(1, ("CECWorld::LoadCutsceneBeziers, failed to load cutscene bezier curve"));
			return false;
		}

	// 	//	Offset bezier
	// 	pBezier->SetOffset(A3DVECTOR3(m_fWorldCenterX, 0.0f, m_fWorldCenterZ));
		
		m_CtsBezierTab.put((int)pBezier->GetGlobalID(), pBezier);
	}

	cbezierFile.Close();
	return true;
}

bool CECWorld::LoadTerrainHole(const char* szFile)
{
	FILE* fp = fopen(szFile, "rb");
	if(fp)
	{
		for(int i=0;i<16;i++)
		{
			for(int j=0;j<16;j++)
			{
				int temp = 0;
				fread(&temp, sizeof(bool), 1, fp);
				m_aTerrainHoles[i*16+j] = (temp != 0);
			}
		}

		fclose(fp);
	}
	else
		return false;

	return true;
}
//	Set load center
void CECWorld::SetLoadCenter(const A3DVECTOR3& vCenter)
{
	if (IsLoadThreadReady())
	{
		SuspendLoadThread();
		ThreadRemoveAllLoaded();
	}

	if (m_pA3DTerrain)
		m_pA3DTerrain->SetLoadCenter(vCenter);

	if (m_pScene)
		m_pScene->SetLoadCenter(vCenter);

	if (IsLoadThreadReady())
	{
		LoadInMainThread(true);

#ifdef USING_BRUSH_MAN
		GetOrnamentMan()->GetBrushMan()->Build(vCenter, true);
#endif

#ifdef USING_TRIANGLE_MAN
		GetOrnamentMan()->GetTriangleMan()->Build(vCenter, true);
#endif

		StartLoaderThread();
	}

	if( m_pGrassLand )
	{
		// we want grassland to build all active grasses when we enter the game 
		m_pGrassLand->SetProgressLoad(false);
		m_pGrassLand->Update(vCenter, 0);
		m_pGrassLand->SetProgressLoad(true);
	}

	g_pGame->GetViewport()->GetA3DCamera()->SetPos(vCenter + A3DVECTOR3(0.0f, 2.0f, 0.0f));

	m_bResetEnv = true;
	m_bCenterReset = true;

	//	Force to update objects in mini-map
	GetPlayerMan()->OnLoadCenterChanged();
	GetNPCMan()->OnLoadCenterChanged();
	GetMatterMan()->OnLoadCenterChanged();

	if(m_nSkyType == 0)
	{
		// wait until sky's texture has been loaded
		while( m_pA3DSky->IsTextureLoading() )
			Sleep(10);
		// stop transition state of sky
		m_pA3DSky->SetTimePassed(0x7fffffff);
	}
// 	else if(m_nSkyType == 1)	// sky box
// 	{
// 		A3DRELEASE(m_pSkyBox);
// 		m_pSkyBox = new A3DSkyBox;
// 
// 		//	Sky box
// 		if (!m_pSkyBox)
// 			return;
// 
// 		CECConfigs* pConfig = g_pGame->GetConfigs();
// 
// 		//	Sky texture file name shouldn't be set here
// 		if (!m_pSkyBox->Init(g_pGame->GetA3DDevice(), NULL, pConfig->m_strDefSkyFile0, pConfig->m_strDefSkyFile0,
// 			pConfig->m_strDefSkyFile0, pConfig->m_strDefSkyFile0, 
// 			pConfig->m_strDefSkyFile0, pConfig->m_strDefSkyFile0))
// 		{
// 			a_LogOutput(1, "CECWorld::SetLoadCenter, Failed to create sky box!");
// 			return;
// 		}
// 	}

	//	Discard current frame
	g_pGame->DiscardFrame();
}

void CECWorld::ForceLoadTerrain()
{
	Tick(1, g_pGame->GetViewport());

	if (IsLoadThreadReady())
	{
		LoadInMainThread(true);

		StartLoaderThread();
	}
}

//	Set view radius
void CECWorld::SetViewRadius(float fRadius)
{
	CECConfigs* pConfigs = g_pGame->GetConfigs();

	a_ClampFloor(fRadius, pConfigs->GetSevActiveRadius());

	CECHostPlayer* pPlayer = GetHostPlayer();
	if (pPlayer && m_pA3DTerrain && m_pScene)
	{
		//	View radius should be equal to active radius, otherwise there
		//	will be block missing at the border between terrain and outline
		m_pA3DTerrain->SetActRadius(fRadius/* + 64.0f */);
		m_pA3DTerrain->SetViewRadius(fRadius);
		m_pScene->SetActRadius(2000.0f);//fRadius);

//		SetLoadCenter(pPlayer->GetPos());
	}
}

//	Create managers
bool CECWorld::CreateManagers()
{
	CECPlayerMan* pPlayerMan = new CECPlayerMan(m_pGameRun);
	m_aManagers[MAN_PLAYER] = pPlayerMan;

	CECNPCMan* pNPCMan = new CECNPCMan(m_pGameRun);
	m_aManagers[MAN_NPC] = pNPCMan;

	CECMatterMan* pMatterMan = new CECMatterMan(m_pGameRun);
	m_aManagers[MAN_MATTER] = pMatterMan;

	CECOrnamentMan* pOnmtMan = new CECOrnamentMan(m_pGameRun);
	m_aManagers[MAN_ORNAMENT] = pOnmtMan;

	CECDecalMan* pDecalMan = new CECDecalMan(m_pGameRun);
	m_aManagers[MAN_DECAL] = pDecalMan;

	CECSkillGfxMan* pSkillGfxMan = new CECSkillGfxMan(m_pGameRun);
	m_aManagers[MAN_SKILLGFX] = pSkillGfxMan;

#ifndef BACK_VERSION
	RegisterManagers();
#endif
	return true;
}

//	Release managers
void CECWorld::ReleaseManagers()
{
#ifndef BACK_VERSION
	UnRegisterManagers();
#endif
	for (int i=0; i < NUM_MANAGER; i++)
	{
		CECManager* pManager = m_aManagers[i];
		if (pManager)
		{
			pManager->Release();
			delete pManager;
		}

		m_aManagers[i] = NULL;
	}
}


bool CECWorld::IsPrecinctMusicChange(CELPrecinct* pPrecinct1, CELPrecinct* pPrecinct2)
{
	if(!pPrecinct1 || !pPrecinct2)
		return true;
	int iNum = pPrecinct1->GetMusicFileNum();
	if(iNum!=pPrecinct2->GetMusicFileNum())
		return true;

	int i;
	for(i=0; i<iNum; ++i)
	{
		AString strName1 = pPrecinct1->GetMusicFile(i);
		AString strName2 = pPrecinct2->GetMusicFile(i);
		if(strName1!=strName2)
			return true;
	}
	return false;
}

//	Calculate terrain and scene loading speed
void CECWorld::CalcSceneLoadSpeed()
{
	CECHostPlayer* pHost = GetHostPlayer();

	//	If world center is just reset, skip calculation
	if (m_bCenterReset || !pHost || !pHost->IsAllResReady())
		return;

	float fAvgRate = g_pGame->GetAverageFrameRate();
	float fSpeedH = pHost->GetMoveControl().GetAverHoriSpeed();
	if (fabs(fSpeedH) > 30.0f)
		return;

	//	Now, we use multi-thread to load terrain data, so needn't
	//	handle it any more
/*	if (m_pA3DTerrain)
	{
		int iWaitBlock = m_pA3DTerrain->GetCandidateBlockNum();
		if (iWaitBlock)
		{
			//	time to pass a block in second
			float fPassTime = 100.0f;
			if (fSpeedH)
			{
				fPassTime = m_pA3DTerrain->GetBlockSize() * 0.9f / fSpeedH;
				a_ClampRoof(fPassTime, 100.0f);
			}

			//	Frame number to pass a block
			float fPassFrame = fPassTime * fAvgRate;
			m_fTrnLoadDelta = iWaitBlock / fPassFrame;
		}

		m_fTrnLoadCnt += m_fTrnLoadDelta;
		m_pA3DTerrain->SetBlockLoadSpeed((int)m_fTrnLoadCnt);
		m_fTrnLoadCnt -= (int)m_fTrnLoadCnt;
	}
*/
	if (m_pScene)
	{
		int iWaitBlock = m_pScene->GetCandidateBlockNum();
		if (iWaitBlock)
		{
			//	time to pass a block in second
			float fPassTime = 100.0f;
			if (fSpeedH)
			{
				fPassTime = m_pScene->GetBlockSize() * 0.8f / fSpeedH;
				a_ClampRoof(fPassTime, 100.0f);
			}

			//	Frame number to pass a block
			float fPassFrame = fPassTime * fAvgRate;
			m_fScnLoadDelta = iWaitBlock / fPassFrame;
		}

		m_fScnLoadCnt += m_fScnLoadDelta;
		m_pScene->SetBlockLoadSpeed((int)m_fScnLoadCnt);
		m_fScnLoadCnt -= (int)m_fScnLoadCnt;
	}
}

//	Tick routine
bool CECWorld::Tick(DWORD dwDeltaTime, CECViewport* pViewport)
{
	for (int i=0; i < NUM_MANAGER; i++)
	{
		if (m_aManagers[i])
			m_aManagers[i]->Tick(dwDeltaTime);
	}

	//	Calculate terrain and block loading speed
	CalcSceneLoadSpeed();

	A3DVECTOR3 vHostPos = m_pGameRun->GetSafeHostPos(pViewport);
#ifdef BACK_VERSION
	A3DCamera* pCamera = pViewport->GetA3DCamera();
#else
	A3DCameraBase* pCamera = pViewport->GetA3DCameraBase();
#endif
	
	// for test 
	if(g_pGame->GetGameRun()->GetHostPlayer() && 
		g_pGame->GetGameRun()->GetHostPlayer()->GetCutscene() &&
		g_pGame->GetGameRun()->GetHostPlayer()->GetCutscene()->IsPlayingCutscene())
		vHostPos = pCamera->GetPos();

	if (m_pA3DTerrain)
	{
		m_pA3DTerrain->Update(dwDeltaTime, pCamera, vHostPos);

		if (m_pTerrainOutline)
			m_pTerrainOutline->Update(pCamera, vHostPos);

		if (m_pGameRun->GetGameState() == CECGameRun::GS_GAME &&
			m_pA3DTerrain->IsMultiThreadLoad())
		{
			int r, c, iBlock;
			while (m_pA3DTerrain->GetNextCandidateBlock(r, c, iBlock))
			{
				QueueTerrainBlock(r, c, iBlock);
			}

			for(int i=0;i<256;i++)
			{
				r = i/16;
				c = i%16;
				if(m_pA3DTerrain->GetActiveBlocks()->rcArea.PtInRect(c, r))
				{
					A3DTerrain2Block* block = m_pA3DTerrain->GetActiveBlocks()->GetBlock(r, c, false);
					if(block)
					{
						if(m_aTerrainHoles[r*16+c])
							block->SetBlockFlags(A3DTRN2LOADERB::T2BKFLAG_NORENDER);
						else
							block->SetBlockFlags(A3DTRN2LOADERB::T2BKFLAG_DEFAULT);
					}
				}
			}

		}
	}
	
	if (m_pCloudManager)
	{
		A3DVECTOR3 vecLightDir = Normalize(A3DVECTOR3(-1.0f, -1.0f, -1.0f));
		if( m_pSunMoon )
			vecLightDir = m_pSunMoon->GetLightDir();

		m_pCloudManager->Tick(dwDeltaTime, pCamera->GetPos() - vecLightDir * 10000.0f);
	}

	if (m_pScene)
	{
		if(g_pGame->GetGameRun()->GetHostPlayer() && 
			g_pGame->GetGameRun()->GetHostPlayer()->GetCutscene() &&
			!g_pGame->GetGameRun()->GetHostPlayer()->GetCutscene()->IsPlayingCutscene())
			m_pScene->Update(dwDeltaTime, pCamera, vHostPos);
	}

	UpdatePostEffect(dwDeltaTime);

	if (m_pSceneLights)
		m_pSceneLights->Update(dwDeltaTime, m_pSunMoon->GetDNFactor(), vHostPos);

	if (m_pA3DTerrainWater)
		m_pA3DTerrainWater->Update(dwDeltaTime);

	if (m_pForest)
		m_pForest->Update(dwDeltaTime);

	if (m_pGrassLand && g_pGame->GetConfigs()->m_bShowGrassLand)
	{
		m_pGrassLand->SetProgressLoad(true);
		m_pGrassLand->Update(vHostPos, dwDeltaTime);
	}

	TickNatureObjects(dwDeltaTime);

	if (m_pCursorStereoGfx)
		m_pCursorStereoGfx->TickAnimation(dwDeltaTime);

	if (m_pTargetStereoGfx)
		m_pTargetStereoGfx->TickAnimation(dwDeltaTime);

	if (m_pGameRun->GetGameState() == CECGameRun::GS_GAME)
	{
		//	Update current precinct
		UpdateCurPrecinct();

		//	Update current region
		UpdateCurRegion();
	}
#ifndef BACK_VERSION

	CECHostPlayer* pHost = GetHostPlayer();
	if( pHost && pHost->IsHangerOn() )
	{
		if (!pHost->IsChangingFace())
			pHost->UpdateFollowCamera(pHost->IsPlayerMoving(), 0);

		// we may readjust the camera in above call, so just reactivate the camera.
		A3DCameraBase* pCamera = pViewport->GetA3DCameraBase();
		A3DCoordinate* pCameraCoord = pHost->GetCameraCoord();
		pCamera->SetPos(pCameraCoord->GetPos());
		pCamera->SetDirAndUp(pCameraCoord->GetDir(), pCameraCoord->GetUp());
		pCamera->Active();
	}

	m_bResetEnv = !m_pScene->AdjustEnvironment(g_pGame->GetTickTime(), pViewport->GetA3DCamera(), m_bResetEnv);

#endif

	m_bCenterReset = false;

	LoadInMainThread(false);

#ifndef BACK_VERSION
	__PreRender(pViewport);
#endif
	
	/*	
	草的踩踏效果开关,建议在PS3.0开启, from sds
	*/
	m_pGrassLand->SetGrassInteractorEnable(true);
	A3DGrassInteractor* pInteractor = m_pGrassLand->GetGrassInteractor();
	if (NULL != pInteractor && NULL != pHost && pHost->IsAllResReady())
	{
		pInteractor->SetInteractorCenterPos(pHost->GetPos());
	}	

	return true;
}


//	Tick routine
bool CECWorld::TickForAnmSystem(DWORD dwDeltaTime, CECViewport* pViewport)
{
	for (int i=0; i < NUM_MANAGER; i++)
	{
		if (m_aManagers[i])
			m_aManagers[i]->Tick(dwDeltaTime);
	}

	//	Calculate terrain and block loading speed
	CalcSceneLoadSpeed();
#ifdef BACK_VERSION
	A3DCamera* pCamera = pViewport->GetA3DCamera();
#else
	A3DCameraBase* pCamera = pViewport->GetA3DCameraBase();
#endif
 	A3DVECTOR3 vHostPos = pCamera->GetPos();

	if (m_pA3DTerrain)
	{
		m_pA3DTerrain->Update(dwDeltaTime, pCamera, vHostPos);

		if (m_pTerrainOutline)
			m_pTerrainOutline->Update(pCamera, vHostPos);

		if (m_pGameRun->GetGameState() == CECGameRun::GS_GAME &&
			m_pA3DTerrain->IsMultiThreadLoad())
		{
			int r, c, iBlock;
			while (m_pA3DTerrain->GetNextCandidateBlock(r, c, iBlock))
			{
				QueueTerrainBlock(r, c, iBlock);
			}

			for(int i=0;i<256;i++)
			{
				r = i/16;
				c = i%16;
				if(m_pA3DTerrain->GetActiveBlocks()->rcArea.PtInRect(c, r))
				{
					A3DTerrain2Block* block = m_pA3DTerrain->GetActiveBlocks()->GetBlock(r, c, false);
					if(block)
					{
						if(m_aTerrainHoles[r*16+c])
							block->SetBlockFlags(A3DTRN2LOADERB::T2BKFLAG_NORENDER);
						else
							block->SetBlockFlags(A3DTRN2LOADERB::T2BKFLAG_DEFAULT);
					}
				}
			}

		}
	}
	
	if (m_pCloudManager)
	{
		A3DVECTOR3 vecLightDir = Normalize(A3DVECTOR3(-1.0f, -1.0f, -1.0f));
		if( m_pSunMoon )
			vecLightDir = m_pSunMoon->GetLightDir();

		m_pCloudManager->Tick(dwDeltaTime, pCamera->GetPos() - vecLightDir * 10000.0f);
	}

	if (m_pScene)
	{
//		if(g_pGame->GetGameRun()->GetHostPlayer() && 
//			g_pGame->GetGameRun()->GetHostPlayer()->GetCutscene())
			m_pScene->Update(dwDeltaTime, pCamera, vHostPos);
	}

	if (m_pSceneLights)
		m_pSceneLights->Update(dwDeltaTime, m_pSunMoon->GetDNFactor(), vHostPos);

	if (m_pA3DTerrainWater)
		m_pA3DTerrainWater->Update(dwDeltaTime);

	if (m_pForest)
		m_pForest->Update(dwDeltaTime);

	if (m_pGrassLand && g_pGame->GetConfigs()->m_bShowGrassLand)
	{
		m_pGrassLand->SetProgressLoad(true);
		m_pGrassLand->Update(vHostPos, dwDeltaTime);
	}

	TickNatureObjects(dwDeltaTime);

	if (m_pCursorStereoGfx)
		m_pCursorStereoGfx->TickAnimation(dwDeltaTime);

	if (m_pTargetStereoGfx)
		m_pTargetStereoGfx->TickAnimation(dwDeltaTime);

	if (m_pGameRun->GetGameState() == CECGameRun::GS_GAME)
	{
		//	Update current precinct
		UpdateCurPrecinct();

		//	Update current region
		UpdateCurRegion();
	}


#ifndef BACK_VERSION

	CECHostPlayer* pHost = GetHostPlayer();
	if( pHost && pHost->IsHangerOn() )
	{
		if (!pHost->IsChangingFace())
			pHost->UpdateFollowCamera(pHost->IsPlayerMoving(), 0);

		// we may readjust the camera in above call, so just reactivate the camera.
		A3DCameraBase* pCamera = pViewport->GetA3DCameraBase();
		A3DCoordinate* pCameraCoord = pHost->GetCameraCoord();
		pCamera->SetPos(pCameraCoord->GetPos());
		pCamera->SetDirAndUp(pCameraCoord->GetDir(), pCameraCoord->GetUp());
		pCamera->Active();
	}

	m_bResetEnv = !m_pScene->AdjustEnvironment(g_pGame->GetTickTime(), pViewport->GetA3DCamera(), m_bResetEnv);

#endif

	m_bCenterReset = false;

	LoadInMainThread(false);

#ifndef BACK_VERSION
	__PreRender(pViewport);
#endif

	/*	
	草的踩踏效果开关,建议在PS3.0开启, from sds
	*/
	m_pGrassLand->SetGrassInteractorEnable(true);
	A3DGrassInteractor* pInteractor = m_pGrassLand->GetGrassInteractor();
	if (NULL != pInteractor && NULL != pHost && pHost->IsAllResReady())
	{
		pInteractor->SetInteractorCenterPos(pHost->GetPos());
	}	


	return true;
}


//	Tick animation
bool CECWorld::TickAnimation()
{
	for (int i=0; i < NUM_MANAGER; i++)
	{
		if (m_aManagers[i])
			m_aManagers[i]->TickAnimation();
	}

	return true;
}

#ifndef BACK_VERSION
bool CECWorld::__PreRender( ECViewport* pViewport )
{

	CECHostPlayer* pHost = GetHostPlayer();
	//PLATFORM_RENDERINFO* pRenderInfo = g_pGame->GetGameRun()->GetPlatformRenderInfo();
	ELEMENT_RENDER_PARAM* pRenderParam = g_pGame->GetGameRun()->GetElementRenderParam();
	CECConfigs* pConfigs = g_pGame->GetConfigs();//glb_GetConfigs();

	const CECScene::ENVIRONMENT& env = m_pScene->m_Env;
	pRenderParam->bUnderWater = env.bUnderWater;

	float nf = m_pSunMoon->GetDNFactor();
	float df = 1.0f - nf;


	A3DCOLOR fogColor = 0xff000000;
	if( env.bUnderWater )
		fogColor = (env.UWFogCol.cur * df + env.UWFogCol_n.cur * nf).ToRGBAColor();
	else
		fogColor = (env.FogCol.cur * df + env.FogCol_n.cur * nf).ToRGBAColor();

	if (pHost && pHost->HostIsReady() == false)
	{
		pRenderParam->crClear = 0x00000000;
		pRenderParam->bRenderScene = false;
	}
	else
	{
		//g_pGame->GetA3DDevice()->Clear(D3DCLEAR_TARGET, fogColor & 0x00ffffff, 1.0f, 0);
		pRenderParam->crClear = fogColor & 0x00ffffff;
		pRenderParam->bRenderScene = true;
	}

	ElementRender* pElementRender = g_pGame->GetGameRun()->GetElementRender();

	pRenderParam->paramLight       = g_pGame->GetDirLight()->GetLightparam();
	pRenderParam->fDNFactor        = m_pSunMoon->GetDNFactor();
	pRenderParam->nWaterEffect     = pConfigs->GetSystemSettings().nWaterEffect;
	pRenderParam->pSelf            = pElementRender;

	//Switch
	CECGameUIMan* pGameUI = g_pGame->GetGameRun()->GetUIManager()->GetInGameUIMan();
	pRenderParam->bModelLightMap   = true; //pConfigs->IsModelLightMap();

	pRenderParam->bRenderScene = true;//pConfigs->IsRenderScene();
	pRenderParam->bRenderSky = true;//pConfigs->IsRenderSky();
	pRenderParam->bRenderTerrain = true;//pConfigs->IsRenderTerrain();
	pRenderParam->bRenderSceneObject = true;//pConfigs->IsRenderSceneObject();
	pRenderParam->bRenderLitModel = true;//pConfigs->IsRenderLitModel();
	pRenderParam->bRenderSkinModel = true;//pConfigs->IsRenderSkinModel();
	pRenderParam->bRenderGrass = true; //pConfigs->IsRenderGrass();
	pRenderParam->bRenderShadow = pConfigs->GetSystemSettings().bShadow;
	pRenderParam->bRenderWater = true;//pConfigs->IsRenderWater();
	pRenderParam->bRenderGFX = true; //pConfigs->IsRenderGFX();
	pRenderParam->bRenderZPressPass = false;
	pRenderParam->bRenderZToRenderTarget = false;
	pRenderParam->bRenderPostProcess = true;//pConfigs->IsRenderPostProcess();
	pRenderParam->bRenderUI = true;;//pConfigs->IsRenderUI();

	if (pGameUI && pGameUI->GetDialog("Panel_SetEffect")->IsShow())
	{
		pRenderParam->bRenderScene = ((PAUICHECKBOX)(pGameUI->GetDialog("Panel_SetEffect")->GetDlgItem("Check_RenderScene")))->IsChecked();//pConfigs->IsRenderScene();
		pRenderParam->bRenderSky = ((PAUICHECKBOX)(pGameUI->GetDialog("Panel_SetEffect")->GetDlgItem("Check_RenderSky")))->IsChecked();//pConfigs->IsRenderSky();
		pRenderParam->bRenderTerrain = ((PAUICHECKBOX)(pGameUI->GetDialog("Panel_SetEffect")->GetDlgItem("Check_RenderTerrain")))->IsChecked();//pConfigs->IsRenderTerrain();
		pRenderParam->bRenderSceneObject = ((PAUICHECKBOX)(pGameUI->GetDialog("Panel_SetEffect")->GetDlgItem("Check_RenderSceneObiect")))->IsChecked();//pConfigs->IsRenderSceneObject();
		pRenderParam->bRenderLitModel = ((PAUICHECKBOX)(pGameUI->GetDialog("Panel_SetEffect")->GetDlgItem("Check_RenderLitModel")))->IsChecked();//pConfigs->IsRenderLitModel();
		pRenderParam->bRenderSkinModel = ((PAUICHECKBOX)(pGameUI->GetDialog("Panel_SetEffect")->GetDlgItem("Check_RenderSkinModel")))->IsChecked();//pConfigs->IsRenderSkinModel();
		pRenderParam->bRenderGrass = ((PAUICHECKBOX)(pGameUI->GetDialog("Panel_SetEffect")->GetDlgItem("Check_RenderGrass")))->IsChecked(); //pConfigs->IsRenderGrass();
		pRenderParam->bRenderShadow = pConfigs->GetSystemSettings().bShadow;
		pRenderParam->bRenderWater = ((PAUICHECKBOX)(pGameUI->GetDialog("Panel_SetEffect")->GetDlgItem("Check_RenderWater")))->IsChecked();//pConfigs->IsRenderWater();
		pRenderParam->bRenderGFX = ((PAUICHECKBOX)(pGameUI->GetDialog("Panel_SetEffect")->GetDlgItem("Check_RenderGFX")))->IsChecked(); //pConfigs->IsRenderGFX();
		pRenderParam->bRenderZPressPass = false;
		pRenderParam->bRenderPostProcess = ((PAUICHECKBOX)(pGameUI->GetDialog("Panel_SetEffect")->GetDlgItem("Check_RenderPostProcess")))->IsChecked();//pConfigs->IsRenderPostProcess();
		pRenderParam->bRenderUI = true;;//pConfigs->IsRenderUI();
	}

	pRenderParam->bUseCustomShadowCenter = false;

	if (pHost && pHost->HostIsReady())
	{
		pRenderParam->bUseCustomShadowCenter = true;
		pRenderParam->vCustomShadowCenter = pHost->GetPos();
	}
    
	if (g_pGame->GetGameRun()->GetAnmSystem()->IsPlaying())
	{
		pRenderParam->vCustomShadowCenter = g_pGame->GetGameRun()->GetAnmSystem()->GetCenterPos();

	}

	// 	if(IsAnimationWorld() || g_pGame->GetGameRun()->IsPlayingCameraAnimation())
	// 	{
	// 		pRenderParam->bAnimationWorld = true;
	// 	}
	// 	else
	//{
	pRenderParam->bAnimationWorld = false;
	//}

/*
	bool bRealTime = false;
	m_pA3DTerrain->EnableLight(true);
	if (GetAsyncKeyState('9')&0x8000)
	{
		bRealTime = true;
	}
	if (GetAsyncKeyState('6')&0x8000)
	{
		m_pA3DTerrain->EnableLight(false);
	}

	g_pGame->GetA3DEngine()->GetA3DLitModelRender()->SetRealTimeLight(bRealTime);
	A3D::g_pA3DTerrain2Env->SetRealTimeLight(bRealTime);

	
	//	Get specified pixel shader
	A3DPixelShader* m_pps3LayersLM = A3D::g_pA3DTerrain2Env->GetPS3LayersLM();
	A3DPixelShader* m_pps3LayersLMAlphaShadow =A3D::g_pA3DTerrain2Env->GetPS3LayersLMAlphaShadow();
	A3DPixelShader* m_pps3LayersVL = A3D::g_pA3DTerrain2Env->GetPS3LayersVL();
	A3DPixelShader* m_ppsTexMerge = A3D::g_pA3DTerrain2Env->GetPSTexMerge();
	A3DPixelShader* m_ppsTMLM = A3D::g_pA3DTerrain2Env->GetPSTMLM();

	//	A3D::g_pA3DTerrain2Env->Get specified hlsl
	A3DHLSL* pHlsl3 =		A3D::g_pA3DTerrain2Env->GetHLSL3LayersLM(false);//(bool bShadowMap) const;
	A3DHLSL* pHlsl6 =		A3D::g_pA3DTerrain2Env->GetHLSL6LayersLM(false);//(bool bShadowMap) const;
	A3DVertexDecl* m_pvsDecl =	A3D::g_pA3DTerrain2Env->GetVertexDecl();

	//Render Terrain with HDR LightMap
	A3DPixelShader* m_pps3LayersNoLM = A3D::g_pA3DTerrain2Env->GetPS3LayersNoLM();
	A3DPixelShader* m_ppsLightMapHDR = A3D::g_pA3DTerrain2Env->GetPSLightMapHDR();
	A3DVertexShader* m_pvsLightMapHDR = A3D::g_pA3DTerrain2Env->GetVSLightMapHDR();
	A3DTexture*	m_pLightMapLookup =	A3D::g_pA3DTerrain2Env->GetHDRLookup();

	//Render Terrain one pass hdr lightmap
	A3DPixelShader* m_pps3LayersOnePassHDRLM = A3D::g_pA3DTerrain2Env->GetGetPS3LayersOnePassHDRLM();

	//	A3D::g_pA3DTerrain2Env->Get vertex shader
	A3DVertexShader* m_pvsTerrain = A3D::g_pA3DTerrain2Env->GetVertexShader();
	//	A3D::g_pA3DTerrain2Env->Get vertex shader for water refract rendering
	A3DVertexShader* m_pvsRefract = A3D::g_pA3DTerrain2Env->GetRefractVertexShader();
	//	A3D::g_pA3DTerrain2Env->Get mask texture format
	A3DFORMAT m_MaskMapFmt = A3D::g_pA3DTerrain2Env->GetMaskMapFormat();
	//	A3D::g_pA3DTerrain2Env->Get light map texture format
	A3DFORMAT m_LightMapFmt = A3D::g_pA3DTerrain2Env->GetLightMapFormat() ;
	//	Is light map 32 bit
	bool m_bLightMap32Bit = A3D::g_pA3DTerrain2Env->IsLightMap32Bit();
	//	A3D::g_pA3DTerrain2Env->Get support pixel shader flag
	bool m_bSupportPS = A3D::g_pA3DTerrain2Env->GetSupportPSFlag();
	//	A3D::g_pA3DTerrain2Env->Get pure white texture
	A3DTexture* m_pWhiteTex = A3D::g_pA3DTerrain2Env->GetWhiteTexture();
	//	A3D::g_pA3DTerrain2Env->Get unit white texture for color map (used in mask area)
	A3DTexture* m_pUnitWhiteTex = A3D::g_pA3DTerrain2Env->GetUnitWhiteTexture();

	//A3D::g_pA3DTerrain2Env->Get shadow shaders
	A3DVertexShader* m_pShadowMapVS = A3D::g_pA3DTerrain2Env->GetShadowMapVS();
	A3DPixelShader*	m_pShadowMapPS = A3D::g_pA3DTerrain2Env->GetShadowMapPS();
	A3DTexture* m_pLookupMap = A3D::g_pA3DTerrain2Env->GetLookupMap();

	A3DVertexShader* m_pTerrainShadowVS = A3D::g_pA3DTerrain2Env->GetShadowVS();
	A3DPixelShader*	m_pTerrainShadowPS = A3D::g_pA3DTerrain2Env->GetShadowPS();

	A3DVertexShader* m_pShadowMapL8VS = A3D::g_pA3DTerrain2Env->GetShadowMapL8VS();
	A3DPixelShader*	m_pShadowMapL8PS = A3D::g_pA3DTerrain2Env->GetShadowMapL8PS();
	A3DVertexShader* m_pShadowL8VS = A3D::g_pA3DTerrain2Env->GetShadowL8VS();
	A3DPixelShader*	m_pShadowL8PS=  A3D::g_pA3DTerrain2Env->GetShadowL8PS();
*/
	return true;
}

bool CECWorld::RenderLogin(ECViewport* pViewport)
{
	CECLoginUIMan* pLoginUI = m_pGameRun->GetUIManager()->GetLoginUIMan();
	if( pLoginUI )
		pLoginUI->RenderBackground();

	//	Delay CECDecalMan's rendering
	//for (int i=0; i < NUM_MANAGER-1; i++)
	//{
		//if (m_aManagers[i])
		//	m_aManagers[i]->Render(pViewport);
	//}

	g_pGame->GetA3DDevice()->SetAlphaBlendEnable(false);
	g_pGame->GetA3DDevice()->SetAlphaTestEnable(true);
	g_pGame->GetA3DDevice()->SetAlphaFunction(A3DCMP_GREATER);
	g_pGame->GetA3DDevice()->SetAlphaRef(84);

	
	A3DSkinRender* psr1 = (A3DSkinRender*)(g_pGame->GetSkinRender1());
	A3DSkinRender* psr1_W = (A3DSkinRender*)(g_pGame->GetSkinRender1_W());
	g_pGame->GetA3DEngine()->GetA3DSkinMan()->SetCurSkinRender(psr1);
	//g_pGame->GetA3DEngine()->GetA3DLitModelRender()->SetAlphaMeshesArray(0);
	if(m_aManagers[MAN_PLAYER])
		m_aManagers[MAN_PLAYER]->Render(pViewport);
	//	Render non alpha skin models
	psr1->Render(pViewport->GetA3DViewport(), false);
	psr1->ResetRenderInfo(false);
	psr1_W->Render(pViewport->GetA3DViewport(), false);
	psr1_W->ResetRenderInfo(false);

	//RenderShadows(pViewport);

	// turn on alpha test, and set ref value to a small value to mask off pure transparent area.
	g_pGame->GetA3DDevice()->SetAlphaTestEnable(true);
	g_pGame->GetA3DDevice()->SetAlphaRef(0x1);

	g_pGame->GetA3DDevice()->SetAlphaBlendEnable(true);

	psr1->RenderAlpha(pViewport->GetA3DViewport());
	psr1->ResetRenderInfo(true);
	psr1_W->RenderAlpha(pViewport->GetA3DViewport());
	psr1_W->ResetRenderInfo(true);

	// turn off alpha test again
	g_pGame->GetA3DDevice()->SetAlphaTestEnable(false);
	g_pGame->GetA3DDevice()->SetAlphaRef(84);
	return true;
}
#else

//	Render routine
bool CECWorld::Render(CECViewport* pViewport)
{
	if (0 && m_pGameRun->GetGameState() == CECGameRun::GS_LOGIN)
	{
		g_pGame->GetA3DDevice()->SetAlphaBlendEnable(false);
		g_pGame->GetA3DDevice()->SetAlphaTestEnable(true);
		g_pGame->GetA3DDevice()->SetAlphaFunction(A3DCMP_GREATER);
		g_pGame->GetA3DDevice()->SetAlphaRef(84);

		A3DSkinRender* psr1 = g_pGame->GetSkinRender1();
		g_pGame->GetA3DEngine()->GetA3DSkinMan()->SetCurSkinRender(psr1);
		g_pGame->GetA3DEngine()->GetA3DLitModelRender()->SetAlphaMeshesArray(0);

		if (m_aManagers[MAN_PLAYER])
			m_aManagers[MAN_PLAYER]->Render(pViewport);

		//	Render non alpha skin models
		psr1->Render(pViewport->GetA3DViewport(), false);
		psr1->ResetRenderInfo(false);

		RenderShadows(pViewport);

		// turn on alpha test, and set ref value to a small value to mask off pure transparent area.
		g_pGame->GetA3DDevice()->SetAlphaTestEnable(true);
		g_pGame->GetA3DDevice()->SetAlphaRef(0x1);
		
		g_pGame->GetA3DDevice()->SetAlphaBlendEnable(true);

		//	Render alpha skin models
		// alpha skin mesh dont set z
		psr1->RenderAlpha(pViewport->GetA3DViewport());
		psr1->ResetRenderInfo(true);

		// turn off alpha test again
		g_pGame->GetA3DDevice()->SetAlphaTestEnable(false);
		g_pGame->GetA3DDevice()->SetAlphaRef(84);
		return true;
	}

	A3DEngine* pA3DEngine = g_pGame->GetA3DEngine();
	A3DViewport* pA3DViewport = pViewport->GetA3DViewport();
	CECHostPlayer* pHost = GetHostPlayer();
	CECConfigs* pConfigs = g_pGame->GetConfigs();

	//	test code...
	if (m_pGameRun->GetGameState() == CECGameRun::GS_GAME && (!pHost || !pHost->HostIsReady()))
		return true;

	if( pHost && pHost->IsHangerOn() )
	{
		if (!pHost->IsChangingFace())
			pHost->UpdateFollowCamera(pHost->IsPlayerMoving(), 0);

		// we may readjust the camera in above call, so just reactivate the camera.
		A3DCameraBase* pCamera = pA3DViewport->GetCamera();
		A3DCoordinate* pCameraCoord = pHost->GetCameraCoord();
		pCamera->SetPos(pCameraCoord->GetPos());
		pCamera->SetDirAndUp(pCameraCoord->GetDir(), pCameraCoord->GetUp());
		pCamera->Active();
	}

	m_bResetEnv = !m_pScene->AdjustEnvironment(g_pGame->GetTickTime(), pViewport->GetA3DCamera(), m_bResetEnv);

	float nf = m_pSunMoon->GetDNFactor();
	float df = 1.0f - nf;
	A3DCOLOR fogColor = 0xff000000;
	if( m_pScene->m_Env.bUnderWater )
		fogColor = (m_pScene->m_Env.UWFogCol.cur * df + m_pScene->m_Env.UWFogCol_n.cur * nf).ToRGBAColor();
	else
		fogColor = (m_pScene->m_Env.FogCol.cur * df + m_pScene->m_Env.FogCol_n.cur * nf).ToRGBAColor();
	g_pGame->GetA3DDevice()->Clear(D3DCLEAR_TARGET, fogColor & 0x00ffffff, 1.0f, 0);

	A3DSkinRender* psr1 = g_pGame->GetSkinRender1();
	A3DSkinRender* psr1_W = g_pGame->GetSkinRender1_W();
	pA3DEngine->GetA3DSkinMan()->SetCurSkinRender(psr1);
	g_pGame->GetA3DEngine()->GetA3DLitModelRender()->SetAlphaMeshesArray(0);

	if(m_nSkyType == 1)	// sky box
	{
		if(m_pSkyBox)
		{
			m_pSkyBox->SetCamera(pViewport->GetA3DCamera());

			if(!m_pScene->m_Env.bUnderWater)
				m_pSkyBox->Render();
		}
	}
	else // m_nSkyType == 0
	{
		//	Render sky first
		if (m_pA3DSky)
		{
			m_pA3DSky->SetCamera(pViewport->GetA3DCamera());

			if( !m_pScene->m_Env.bUnderWater )
				m_pA3DSky->Render();
		}

		if( !RenderOnSky(pA3DViewport) )
		return false;
	}
	
	bool bRenderTerrain = true;
	if( m_pTerrainOutline && m_pTerrainOutline->GetReplaceTerrain() )
		bRenderTerrain = false;

	if (m_pA3DTerrain && bRenderTerrain)
	{
		m_pA3DTerrain->SetDNFactor(m_pSunMoon->GetDNFactor());

		if (!m_pScene->m_Env.bUnderWater)
			m_pA3DTerrain->Render(pA3DViewport);
		else
			m_pA3DTerrain->RenderUnderWater(pA3DViewport, m_pA3DTerrainWater->GetCurCaustTexture(),
								m_pA3DTerrainWater->GetCaustDU(), m_pA3DTerrainWater->GetCaustDV());
	}

	m_pScene->Render(pViewport);

	if (m_pForest && !m_pScene->m_Env.bUnderWater && pConfigs->m_bShowForest)
	{
		m_pForest->Render(pA3DViewport);
	}
	
	g_pGame->GetA3DDevice()->SetAlphaBlendEnable(false);
	g_pGame->GetA3DDevice()->SetAlphaTestEnable(true);
	g_pGame->GetA3DDevice()->SetAlphaFunction(A3DCMP_GREATER);
	g_pGame->GetA3DDevice()->SetAlphaRef(84);

	//	Delay CECDecalMan's rendering
	for (int i=0; i < NUM_MANAGER-1; i++)
	{
		if (m_aManagers[i])
			m_aManagers[i]->Render(pViewport);
	}

	if (m_pCursorStereoGfx)
		g_pGame->GetA3DGFXExMan()->RegisterGfx(m_pCursorStereoGfx);

	if (m_pTargetStereoGfx)
		g_pGame->GetA3DGFXExMan()->RegisterGfx(m_pTargetStereoGfx);

	g_pGame->GetA3DDevice()->SetAlphaBlendEnable(false);
	g_pGame->GetA3DDevice()->SetAlphaTestEnable(true);

	g_pGame->GetA3DEngine()->GetA3DLitModelRender()->SetDNFactor(m_pSunMoon->GetDNFactor());
	g_pGame->GetA3DEngine()->GetA3DLitModelRender()->Render(pA3DViewport, false);
	g_pGame->GetA3DEngine()->GetA3DLitModelRender()->ResetRenderInfo(false);
	
	//	Render non alpha skin models
	psr1->Render(pA3DViewport, false);
	psr1->ResetRenderInfo(false);
	psr1_W->Render(pA3DViewport, false);
	psr1_W->ResetRenderInfo(false);

	RenderGrasses(pViewport, false);

	pA3DEngine->FlushCachedAlphaMesh(pA3DViewport);
	g_pGame->GetA3DDevice()->SetAlphaTestEnable(false);
	g_pGame->GetA3DDevice()->SetAlphaBlendEnable(true);

	g_pGame->GetA3DDevice()->SetTextureColorOP(1, A3DTOP_DISABLE);
	g_pGame->GetA3DDevice()->SetTextureAlphaOP(1, A3DTOP_DISABLE);
	g_pGame->GetA3DDevice()->ClearTexture(1);

	RenderShadows(pViewport);

	// turn on alpha test, and set ref value to a small value to mask off pure transparent area.
	g_pGame->GetA3DDevice()->SetAlphaTestEnable(true);
	g_pGame->GetA3DDevice()->SetAlphaRef(0x1);
	
	//	Render alpha skin models in water
	// alpha skin mesh dont set z
	psr1_W->RenderAlpha(pA3DViewport);
	psr1_W->ResetRenderInfo(true);
	
	// turn off alpha test again
	g_pGame->GetA3DDevice()->SetAlphaTestEnable(false);
	g_pGame->GetA3DDevice()->SetAlphaRef(84);

	A3DLIGHTPARAM lightParam = g_pGame->GetDirLight()->GetLightparam();
	if (m_pA3DTerrainWater)
	{
		m_pA3DTerrainWater->SetDNFactor(m_pSunMoon->GetDNFactor());
		A3DTerrain2 * pTerrain = m_pA3DTerrain;
		if( !bRenderTerrain )
			pTerrain = NULL;
		if (!m_pScene->m_Env.bUnderWater)
		{							   
			m_pA3DTerrainWater->RenderReflect(lightParam.Direction, pViewport->GetA3DViewport(), pTerrain, m_pA3DSky, NULL, WorldRenderForReflected, WorldRenderForRefracted, this, WorldRenderOnSky, this, true, m_pA3DTerrain);
		}
		else
			m_pA3DTerrainWater->RenderRefract(lightParam.Direction, pViewport->GetA3DViewport(), pTerrain, m_pA3DSky, NULL, WorldRenderForRefracted, this, WorldRenderOnSky, this, true);
	}

	// turn on alpha test, and set ref value to a small value to mask off pure transparent area.
	g_pGame->GetA3DDevice()->SetAlphaTestEnable(true);
	g_pGame->GetA3DDevice()->SetAlphaRef(0x1);

	g_pGame->GetA3DEngine()->GetA3DLitModelRender()->RenderAlpha(pA3DViewport);
	g_pGame->GetA3DEngine()->GetA3DLitModelRender()->ResetRenderInfo(true);

	//	Render alpha skin models
	// alpha skin mesh dont set z
	psr1->RenderAlpha(pA3DViewport);
	psr1->ResetRenderInfo(true);
	
	// turn off alpha test again
	g_pGame->GetA3DDevice()->SetAlphaTestEnable(false);
	g_pGame->GetA3DDevice()->SetAlphaRef(84);

	// now we render nature objects
	RenderNatureObjects(pViewport);

	// Render grasses
	RenderGrasses(pViewport, true);

	if( m_pSunMoon )
		m_pSunMoon->RenderSolarFlare(pViewport);

	// Render Clouds
	if( m_pCloudManager )
	{
		CECConfigs* pConfigs = g_pGame->GetConfigs();
		if( pConfigs->GetSystemSettings().nCloudDetail > 0 )
			m_pCloudManager->Render(pViewport->GetA3DViewport());
	}

	if (m_aManagers[MAN_DECAL])
		m_aManagers[MAN_DECAL]->Render(pViewport);

	return true;
}

//	Render shadows routine
bool CECWorld::RenderShadows(CECViewport* pViewport)
{
	if( !g_pGame->GetShadowRender() )
		return true;

	// first setup terrain object pointer for shadows
	A3DLIGHTPARAM param = g_pGame->GetDirLight()->GetLightparam();
	// now render the shadows
	if (g_pGame->GetShadowRender())
	{
		g_pGame->GetShadowRender()->SetA3DTerrain(m_pA3DTerrain);
		g_pGame->GetShadowRender()->Render(pViewport->GetA3DViewport(), param.Direction);
	}

	return true;
}

//	Render grasses
bool CECWorld::RenderGrasses(CECViewport* pViewport, bool bRenderAlpha)
{
	if( !g_pGame->GetConfigs()->m_bShowGrassLand )
		return true;

	if (m_pGrassLand)
	{
		m_pGrassLand->SetDNFactor(m_pSunMoon->GetDNFactor());
		m_pGrassLand->Render(pViewport->GetA3DViewport(), bRenderAlpha);
	}

	return true;
}

bool CECWorld::RenderForReflected(A3DViewport* pViewport)
{
	CECViewport ecViewport;
	ecViewport.InitFromA3D(pViewport); 

	CECConfigs* pConfigs = g_pGame->GetConfigs();

	//	Use a temporary skin render so that we don't break world's main rendering
	A3DSkinRender* psr = g_pGame->GetSkinRender2();
	A3DSkinRender* poldsr = g_pGame->GetA3DEngine()->GetA3DSkinMan()->GetCurSkinRender();
	g_pGame->GetA3DEngine()->GetA3DSkinMan()->SetCurSkinRender(psr);

	A3DLitModelRender* plmr = g_pGame->GetLitModelRender1();
	g_pGame->GetA3DEngine()->SetA3DLitModelRender(plmr);

	GfxRenderContainer rc;
	g_pGame->GetA3DGFXExMan()->SetCustomGfxRenderContainer(&rc);

	if (pConfigs->GetSystemSettings().nWaterEffect == 2 && m_pForest)
		m_pForest->Render(pViewport);
		
	g_pGame->GetA3DDevice()->SetAlphaBlendEnable(false);
	g_pGame->GetA3DDevice()->SetAlphaTestEnable(true);
	g_pGame->GetA3DDevice()->SetAlphaFunction(A3DCMP_GREATER);
	g_pGame->GetA3DDevice()->SetAlphaRef(84);

	for (int i=0; i < NUM_MANAGER; i++)
	{
		if (m_aManagers[i])
		{
			if( m_aManagers[i]->GetManagerID() == MAN_NPC )
			{
				if (pConfigs->GetSystemSettings().nWaterEffect == 2)
					m_aManagers[i]->RenderForReflect(&ecViewport);
			}
			else
				m_aManagers[i]->RenderForReflect(&ecViewport);
		}
	}

	g_pGame->GetA3DEngine()->GetA3DLitModelRender()->Render(pViewport, false);
	g_pGame->GetA3DEngine()->GetA3DLitModelRender()->ResetRenderInfo(false);
	
	psr->Render(pViewport, false);
	psr->ResetRenderInfo(false);

	if (pConfigs->GetSystemSettings().nWaterEffect == 2)
		RenderGrasses(&ecViewport, false);

	g_pGame->GetA3DDevice()->SetAlphaTestEnable(false);
	g_pGame->GetA3DDevice()->SetAlphaBlendEnable(true);

	RenderShadows(&ecViewport);

	g_pGame->GetA3DEngine()->GetA3DLitModelRender()->RenderAlpha(pViewport);
	g_pGame->GetA3DEngine()->GetA3DLitModelRender()->ResetRenderInfo(true);

	psr->RenderAlpha(pViewport);
	psr->ResetRenderInfo(true);

	// last flush gfx
	g_pGame->GetA3DGFXExMan()->RenderAllSkinModels(pViewport);
	//	Render skin models from gfx
	psr->Render(pViewport, false);
	psr->ResetRenderInfo(false);
	psr->RenderAlpha(pViewport);
	psr->ResetRenderInfo(true);
	g_pGame->GetA3DGFXExMan()->RenderAllGfx(pViewport);

	//	Restore skin render
	g_pGame->GetA3DEngine()->GetA3DSkinMan()->SetCurSkinRender(poldsr);
	g_pGame->GetA3DEngine()->SetA3DLitModelRender(NULL);
	g_pGame->GetA3DGFXExMan()->SetCustomGfxRenderContainer(NULL);

	return true;
}

bool CECWorld::RenderForRefracted(A3DViewport* pViewport)
{
	CECViewport ecViewport;
	ecViewport.InitFromA3D(pViewport); 

	CECConfigs* pConfigs = g_pGame->GetConfigs();

	//	Use a temporary skin render so that we don't break world's main rendering
	A3DSkinRender* psr = g_pGame->GetSkinRender2();
	A3DSkinRender* poldsr = g_pGame->GetA3DEngine()->GetA3DSkinMan()->GetCurSkinRender();
	g_pGame->GetA3DEngine()->GetA3DSkinMan()->SetCurSkinRender(psr);

	A3DLitModelRender* plmr = g_pGame->GetLitModelRender1();
	g_pGame->GetA3DEngine()->SetA3DLitModelRender(plmr);

	GfxRenderContainer rc;
	g_pGame->GetA3DGFXExMan()->SetCustomGfxRenderContainer(&rc);

	A3DVECTOR3 vecCamPos = pViewport->GetCamera()->GetPos();
	bool bCamBelowWater = m_pA3DTerrainWater->IsUnderWater(vecCamPos);

	if( bCamBelowWater )
	{
		if (m_pForest)
			m_pForest->Render(pViewport);
	}

	g_pGame->GetA3DDevice()->SetAlphaBlendEnable(false);
	g_pGame->GetA3DDevice()->SetAlphaTestEnable(true);
	g_pGame->GetA3DDevice()->SetAlphaFunction(A3DCMP_GREATER);
	g_pGame->GetA3DDevice()->SetAlphaRef(84);

	for (int i=0; i < NUM_MANAGER; i++)
	{
		if (m_aManagers[i])
			m_aManagers[i]->RenderForRefract(&ecViewport);
	}

	// render critters.
	m_pScene->Render(&ecViewport);

	if( bCamBelowWater )
	{
		g_pGame->GetA3DEngine()->GetA3DLitModelRender()->Render(pViewport, false);
		g_pGame->GetA3DEngine()->GetA3DLitModelRender()->ResetRenderInfo(false);
	}
	else
	{
		g_pGame->GetA3DEngine()->GetA3DLitModelRender()->RenderForRefract(pViewport, m_pA3DTerrainWater->GetCurWaterHeight());
		g_pGame->GetA3DEngine()->GetA3DLitModelRender()->ResetRenderInfo(false);
	}

	psr->Render(pViewport, false);
	psr->ResetRenderInfo(false);

	if (pConfigs->GetSystemSettings().nWaterEffect == 2)
	{
		if( bCamBelowWater )
			RenderGrasses(&ecViewport, false);
		else
		{
			if (m_pGrassLand)
			{
				m_pGrassLand->SetDNFactor(m_pSunMoon->GetDNFactor());
				m_pGrassLand->RenderForRefract(pViewport, m_pA3DTerrainWater->GetCurWaterHeight());
			}
		}
	}

	g_pGame->GetA3DDevice()->SetAlphaBlendEnable(true);
	g_pGame->GetA3DDevice()->SetAlphaTestEnable(false);

	RenderShadows(&ecViewport);

	g_pGame->GetA3DEngine()->GetA3DLitModelRender()->RenderAlpha(pViewport);
	g_pGame->GetA3DEngine()->GetA3DLitModelRender()->ResetRenderInfo(true);

	psr->RenderAlpha(pViewport);
	psr->ResetRenderInfo(true);

	if (pConfigs->GetSystemSettings().nWaterEffect == 2)
		RenderGrasses(&ecViewport, true);

	// last flush gfx
	g_pGame->GetA3DGFXExMan()->RenderAllSkinModels(pViewport);
	//	Render skin models from gfx
	psr->Render(pViewport, false);
	psr->ResetRenderInfo(false);
	psr->RenderAlpha(pViewport);
	psr->ResetRenderInfo(true);
	g_pGame->GetA3DGFXExMan()->RenderAllGfx(pViewport);

	if (m_pCloudManager && bCamBelowWater)
		m_pCloudManager->Render(pViewport);

	//	Restore skin render
	g_pGame->GetA3DEngine()->GetA3DSkinMan()->SetCurSkinRender(poldsr);
	g_pGame->GetA3DEngine()->SetA3DLitModelRender(NULL);
	g_pGame->GetA3DGFXExMan()->SetCustomGfxRenderContainer(NULL);

	return true;
}

bool CECWorld::RenderForBloom(A3DViewport* pViewport)
{
	CECViewport ecViewport;
	ecViewport.InitFromA3D(pViewport); 

	g_pGame->GetA3DDevice()->SetAlphaBlendEnable(false);
	g_pGame->GetA3DDevice()->SetAlphaTestEnable(true);
	g_pGame->GetA3DDevice()->SetAlphaFunction(A3DCMP_GREATER);
	g_pGame->GetA3DDevice()->SetAlphaRef(84);

	for (int i=0; i < NUM_MANAGER; i++)
	{
		if (m_aManagers[i])
			m_aManagers[i]->RenderForBloom(&ecViewport);
	}

	A3DSkinRender* psr = g_pGame->GetSkinRender1();
	psr->Render(pViewport, false);
	psr->ResetRenderInfo(false);

	
	g_pGame->GetA3DDevice()->SetAlphaBlendEnable(true);
	g_pGame->GetA3DDevice()->SetAlphaTestEnable(false);

	psr->RenderAlpha(pViewport);
	psr->ResetRenderInfo(true);

	// last flush gfx
	g_pGame->GetA3DGFXExMan()->RenderAllSkinModels(pViewport);
	//	Render skin models from gfx
	psr->Render(pViewport, false);
	psr->ResetRenderInfo(false);
	psr->RenderAlpha(pViewport);
	psr->ResetRenderInfo(true);
	g_pGame->GetA3DGFXExMan()->RenderAllGfx(pViewport, true);

	return true;
}

bool CECWorld::RenderForScreen(A3DViewport* pViewport)
{
	/*
	static bool bInit = false;
	static AMVideoClip clip;

	if( !bInit )
	{			   
    	 CoInitialize ( NULL ); 

		bInit = true;
		clip.Init(g_pGame->GetA3DEngine()->GetA3DDevice(), g_pGame->GetA3DEngine()->GetAMVideoEngine(), "video\\dance.avi");	
		clip.SetVidPos(A3DPOINT2(0, 0));
		//clip.SetBlendMode(A3DBLEND_SRCCOLOR, A3DBLEND_INVSRCCOLOR);
		//clip.Start(true);
	}

	CECViewport ecViewport;
	ecViewport.InitFromA3D(pViewport); 

	A3DEngine* pA3DEngine = g_pGame->GetA3DEngine();
	A3DViewport* pA3DViewport = pViewport;
	CECHostPlayer* pHost = GetHostPlayer();
	CECConfigs* pConfigs = g_pGame->GetConfigs();

	//	test code...
	if (m_pGameRun->GetGameState() == CECGameRun::GS_GAME && (!pHost || !pHost->HostIsReady()))
		return true;

	if( pHost && pHost->IsHangerOn() )
	{
		if (!pHost->IsChangingFace())
			pHost->UpdateFollowCamera(pHost->IsPlayerMoving(), 0);

		// we may readjust the camera in above call, so just reactivate the camera.
		A3DCameraBase* pCamera = pA3DViewport->GetCamera();
		A3DCoordinate* pCameraCoord = pHost->GetCameraCoord();
		pCamera->SetPos(pCameraCoord->GetPos());
		pCamera->SetDirAndUp(pCameraCoord->GetDir(), pCameraCoord->GetUp());
		pCamera->Active();
	}

	m_bResetEnv = !m_pScene->AdjustEnvironment(g_pGame->GetTickTime(), pA3DViewport->GetCamera(), m_bResetEnv);

	float nf = m_pSunMoon->GetDNFactor();
	float df = 1.0f - nf;
	A3DCOLOR fogColor = 0xff000000;
	if( m_pScene->m_Env.bUnderWater )
		fogColor = (m_pScene->m_Env.UWFogCol.cur * df + m_pScene->m_Env.UWFogCol_n.cur * nf).ToRGBAColor();
	else
		fogColor = (m_pScene->m_Env.FogCol.cur * df + m_pScene->m_Env.FogCol_n.cur * nf).ToRGBAColor();
	g_pGame->GetA3DDevice()->Clear(D3DCLEAR_TARGET, fogColor & 0x00ffffff, 1.0f, 0);

	A3DSkinRender* psr1 = g_pGame->GetSkinRender1();
	pA3DEngine->GetA3DSkinMan()->SetCurSkinRender(psr1);
	g_pGame->GetA3DEngine()->GetA3DLitModelRender()->SetAlphaMeshesArray(0);

	//	Render sky first
	if (m_pA3DSky)
	{
		if( !m_pScene->m_Env.bUnderWater )
			m_pA3DSky->Render();
	}

	if (m_pA3DTerrain)
	{
		m_pA3DTerrain->SetDNFactor(m_pSunMoon->GetDNFactor());

		if (!m_pScene->m_Env.bUnderWater)
			m_pA3DTerrain->Render(pA3DViewport);
		else
			m_pA3DTerrain->RenderUnderWater(pA3DViewport, m_pA3DTerrainWater->GetCurCaustTexture(),
								m_pA3DTerrainWater->GetCaustDU(), m_pA3DTerrainWater->GetCaustDV());
	}

	m_pScene->Render(&ecViewport);

	if (m_pForest && !m_pScene->m_Env.bUnderWater && pConfigs->m_bShowForest)
	{
		m_pForest->Render(pA3DViewport);
	}
	
	g_pGame->GetA3DDevice()->SetAlphaBlendEnable(false);
	g_pGame->GetA3DDevice()->SetAlphaTestEnable(true);
	g_pGame->GetA3DDevice()->SetAlphaFunction(A3DCMP_GREATER);
	g_pGame->GetA3DDevice()->SetAlphaRef(84);

	if (m_aManagers[MAN_ORNAMENT])
		m_aManagers[MAN_ORNAMENT]->Render(&ecViewport);
	
	g_pGame->GetA3DDevice()->SetAlphaBlendEnable(false);
	g_pGame->GetA3DDevice()->SetAlphaTestEnable(true);

	g_pGame->GetA3DEngine()->GetA3DLitModelRender()->SetDNFactor(m_pSunMoon->GetDNFactor());
	g_pGame->GetA3DEngine()->GetA3DLitModelRender()->Render(pA3DViewport, false);
	g_pGame->GetA3DEngine()->GetA3DLitModelRender()->ResetRenderInfo(false);
	
	//	Render non alpha skin models
	psr1->Render(pA3DViewport, false);
	psr1->ResetRenderInfo(false);

	RenderGrasses(&ecViewport, false);

	pA3DEngine->FlushCachedAlphaMesh(pA3DViewport);
	g_pGame->GetA3DDevice()->SetAlphaTestEnable(false);
	g_pGame->GetA3DDevice()->SetAlphaBlendEnable(true);

	g_pGame->GetA3DDevice()->SetTextureColorOP(1, A3DTOP_DISABLE);
	g_pGame->GetA3DDevice()->SetTextureAlphaOP(1, A3DTOP_DISABLE);
	g_pGame->GetA3DDevice()->ClearTexture(1);

	A3DLIGHTPARAM lightParam = g_pGame->GetDirLight()->GetLightparam();
	if (m_pA3DTerrainWater)
	{
		m_pA3DTerrainWater->SetDNFactor(m_pSunMoon->GetDNFactor());
		A3DTerrain2 * pTerrain = m_pA3DTerrain;
		pTerrain = NULL;
		if (!m_pScene->m_Env.bUnderWater)
		{							   
			m_pA3DTerrainWater->RenderReflect(lightParam.Direction, pA3DViewport, pTerrain, m_pA3DSky, NULL, WorldRenderForReflected, WorldRenderForRefracted, this, WorldRenderOnSky, this, true, m_pA3DTerrain);
		}
		else
			m_pA3DTerrainWater->RenderRefract(lightParam.Direction, pA3DViewport, pTerrain, m_pA3DSky, NULL, WorldRenderForRefracted, this, WorldRenderOnSky, this, true);
	}

	// turn on alpha test, and set ref value to a small value to mask off pure transparent area.
	g_pGame->GetA3DDevice()->SetAlphaTestEnable(true);
	g_pGame->GetA3DDevice()->SetAlphaRef(0x1);

	g_pGame->GetA3DEngine()->GetA3DLitModelRender()->RenderAlpha(pA3DViewport);
	g_pGame->GetA3DEngine()->GetA3DLitModelRender()->ResetRenderInfo(true);

	//	Render alpha skin models
	// alpha skin mesh dont set z
	psr1->RenderAlpha(pA3DViewport);
	psr1->ResetRenderInfo(true);
	
	// turn off alpha test again
	g_pGame->GetA3DDevice()->SetAlphaTestEnable(false);
	g_pGame->GetA3DDevice()->SetAlphaRef(84);

	// Render grasses
	RenderGrasses(&ecViewport, true);

	clip.DrawToBack();
	*/
	return true;
}

bool CECWorld::RenderOnSky(A3DViewport* pViewport)
{
	CECViewport ecViewport;

	ecViewport.InitFromA3D(pViewport);

	if( m_pSunMoon )
		m_pSunMoon->Render(&ecViewport);

	CECConfigs* pConfigs = g_pGame->GetConfigs();

	if (m_pTerrainOutline)
	{
		m_pTerrainOutline->SetDNFactor(m_pSunMoon->GetDNFactor());
		m_pTerrainOutline->Render2(pViewport);
	}

	return true;
}
#endif


//	Get host player object
CECHostPlayer* CECWorld::GetHostPlayer()
{
	CECPlayerMan* pPlayerMan = GetPlayerMan();
	return pPlayerMan ? pPlayerMan->GetHostPlayer() : NULL;
}

//	Get terrain height of specified position
float CECWorld::GetTerrainHeight(const A3DVECTOR3& vPos, A3DVECTOR3* pvNormal)
{
	A3DTerrain2* pTerrain = GetTerrain();
	ASSERT(pTerrain);
	return pTerrain->GetPosHeight(vPos, pvNormal);
}

//	Get ground height of specified poistion
float CECWorld::GetGroundHeight(const A3DVECTOR3& vPos, A3DVECTOR3* pvNormal/* NULL */)
{
	A3DVECTOR3 vGndPos, vNormal;
	VertRayTrace(vPos, vGndPos, vNormal);
	if (pvNormal) *pvNormal = vNormal;
	return vGndPos.y;
}

//	Get water height of specified position
float CECWorld::GetWaterHeight(const A3DVECTOR3& vPos)
{
#ifdef BACK_VERSION
	A3DTerrainWater* pWater = GetTerrainWater();
#else
	A3DTerrainWater2* pWater = GetTerrainWater();
#endif
	ASSERT(pWater);
	return pWater->GetWaterHeight(vPos);
}

//	Get A3DSky object
A3DSkySphere * CECWorld::GetSkySphere()
{
	return m_pA3DSky;
}

//	Get A3DSky object
A3DSkyBox * CECWorld::GetSkyBox()
{
	return m_pSkyBox;
}


//	Get object by specified ID
//	iAliveFlag: 0, both alive and dead; 1, must be alive; 2, must be dead
CECObject* CECWorld::GetObject(int idObject, int iAliveFlag)
{
	CECObject* pObject = NULL;

	if (ISNPCID(idObject))
	{
		if (!(pObject = GetNPCMan()->GetNPC(idObject)))
			return NULL;

		if ((iAliveFlag == 1 && ((CECNPC*)pObject)->IsDead()) ||
			(iAliveFlag == 2 && !((CECNPC*)pObject)->IsDead()))
			return NULL;
	}
	else if (ISPLAYERID(idObject))
	{
		if (!(pObject = GetPlayerMan()->GetPlayer(idObject)))
			return NULL;

		if ((iAliveFlag == 1 && ((CECPlayer*)pObject)->IsDead()) ||
			(iAliveFlag == 2 && !((CECPlayer*)pObject)->IsDead()))
			return NULL;
	}
	else if (ISMATTERID(idObject))
		pObject = GetMatterMan()->GetMatter(idObject);

	return pObject;
}

//	Update current precinct which the host player is in
void CECWorld::UpdateCurPrecinct()
{
	if (!m_pPrecinctSet)
		return;

	CELBackMusic* pBackMusic = g_pGame->GetBackMusic();
	bool bNight = m_pSunMoon->GetDNFactor() > 0.5f ? true : false;

	CECHostPlayer* pHost = GetHostPlayer();
	if (!pHost || !pHost->IsSkeletonReady())
		return;

	if(pHost->GetCutscene() &&
		pHost->GetCutscene()->IsPlayingCutscene())
	{		
		return;
	}

	A3DVECTOR3 vPos = pHost->GetPos();
	CELPrecinct* pPrecinct = m_pPrecinctSet->IsPointIn(vPos.x, vPos.z, GetInstanceID());
	if (!pPrecinct)
	{
		pBackMusic->StopMusic(true, true);
		pBackMusic->StopBackSFX(true);
		CECGameUIMan* pGameUI = g_pGame->GetGameRun()->GetUIManager()->GetInGameUIMan();
		pGameUI->ChangePrecinct(pPrecinct);
		return;
	}

	//	Change precinct ...
	if (!m_pCurPrecinct || m_pCurPrecinct != pPrecinct)
	{
		if( !m_pCurPrecinct || m_pCurPrecinct->GetID() != pPrecinct->GetID() )
		{
			if (pPrecinct->GetID() && g_pGame->GetGameRun()->GetGameState() == CECGameRun::GS_GAME)
			{
				//	Notify UI module
				CECGameUIMan* pGameUI = g_pGame->GetGameRun()->GetUIManager()->GetInGameUIMan();
				pGameUI->ChangePrecinct(pPrecinct);
			}
		}
	
		// ChangeMusic
		CELPrecinct *pPrecinctMusic = m_pPrecinctSet->IsPointInWithMusic(vPos.x, vPos.z, GetInstanceID());
		if(pPrecinctMusic && IsPrecinctMusicChange(pPrecinctMusic, m_pCurPrecinctMusic))
		{
			//	Change background music
			pBackMusic->SetLoopType((CELBackMusic::LOOPTYPE)pPrecinctMusic->GetMusicLoopType());
			pBackMusic->SetSilenceTime(0);//pPrecinctMusic->GetMusicInterval() * 1000);

			int i, iNumMusic = pPrecinctMusic->GetMusicFileNum();
			for (i=0; i < iNumMusic; i++)
				pBackMusic->PlayMusic(pPrecinctMusic->GetMusicFile(i), false/*true*/, i == 0 ? true : false);

			//	Change background sfx
			AString str;
			if( bNight )
				str = pPrecinctMusic->GetSoundFile_n();
			else
				str = pPrecinctMusic->GetSoundFile();
			
			pPrecinctMusic->SetNightSFX(bNight);
			if (str.GetLength() && af_CheckFileExt(str, ".wav"))
				pBackMusic->PlayBackSFX(str);
			else
				pBackMusic->StopBackSFX(true);
				
			m_pCurPrecinctMusic = pPrecinctMusic;
		}

		m_pCurPrecinct = pPrecinct;

	}
	else
	{
		if( m_pCurPrecinct->GetNightSFX() != bNight )
		{
			AString str;
			if( bNight )
				str = m_pCurPrecinct->GetSoundFile_n();
			else
				str = m_pCurPrecinct->GetSoundFile();

			m_pCurPrecinct->SetNightSFX(bNight);
			if (str.GetLength() && af_CheckFileExt(str, ".wav"))
				pBackMusic->PlayBackSFX(str);
			else
				pBackMusic->StopBackSFX(true);
		}
	}
}

//	Update current region which the host player is in
void CECWorld::UpdateCurRegion()
{
	if (!m_pRegionSet)
		return;

	CECHostPlayer* pHost = GetHostPlayer();
	if (!pHost || !pHost->IsSkeletonReady())
		return;

	//	Note: we use the host position that client sent to server last time
	//		rather than current host's position in client.
	A3DVECTOR3 vPos = pHost->GetMoveControl().GetLastSevPos();
	CELRegion* pRegion = m_pRegionSet->IsPointInRegion(vPos.x, vPos.z);
	if (pRegion != m_pCurRegion)
	{
		//	Change region ...
		if (pRegion)
		{
			//	Enter new region
			g_pGame->GetGameSession()->c2s_CmdEnterSanctuary();
		}
		else
		{
			//	Leave current region and enter no-region area
		}

		m_pCurRegion = pRegion;
	}

	if( pHost->IsClothesOn() )
	{
		//  we wait until the host player has loaded its all equipments.
		//	Check transport box
		CELTransportBox* pTrans = m_pRegionSet->IsPointInTransport(vPos.x, vPos.y, vPos.z, GetInstanceID());
		if (pTrans)
		{
			if (pHost->GetBasicProps().iLevel >= pTrans->GetLevelLimit())
				g_pGame->GetGameSession()->c2s_CmdEnterInstance(pTrans->GetIndex(), pTrans->GetInstanceID());
			else
			{
				CECUIManager* pUIMan = g_pGame->GetGameRun()->GetUIManager();
				if (pUIMan)
				{
					static DWORD timeLast = 0;
					if( timeLast + 10000 < timeGetTime() )
					{
						ACString str;
						str.Format(g_pGame->GetFixedMsgTab()->GetWideString(FISMSG_TRANSPORT_LIMIT), pTrans->GetLevelLimit());
						pUIMan->ShowErrorMsg(str, "errmsg_transport");
						timeLast = timeGetTime();
					}
				}
			}	
		}
	}
}

void CECWorld::SetTimeOfDay(float t)
{
	if(m_nSkyType == 0)
	{
		while( m_pA3DSky->IsTextureLoading() )
			Sleep(10);

		m_pA3DSky->SetTimePassed(0x7fffffff);
		m_bResetEnv = true;
		m_pSunMoon->SetTimeOfTheDay(t);
	}
}

//	Checkout instance data
bool CECWorld::CheckOutInst(int idInst, DWORD dwRegionTimeStamp, DWORD dwPrecinctTimeStamp)
{
	if (m_idInst != idInst)
		return false;

	if (!m_pRegionSet || m_pRegionSet->GetTimeStamp() != dwRegionTimeStamp)
		return false;

	if (!m_pPrecinctSet || m_pPrecinctSet->GetTimeStamp() != dwPrecinctTimeStamp)
		return false;

	return true;
}

bool CECWorld::IsCameraPlayed(int indexCamera)
{
	abase::hashtab<bool, int, abase::_hash_function>::pair_type pair = m_PlayedCameraIndex.get(indexCamera);
	return pair.second;
}

// Get cutscene bezier curve
CECBezier* CECWorld::GetCutsceneBezier(DWORD dwBezierIndex)
{
	CutsceneBezierTable::pair_type Pair = m_CtsBezierTab.get(dwBezierIndex);
	return Pair.second ? *Pair.first : NULL;
}

void CECWorld::SetCameraPlayed(int indexCamera, bool bPlayed)
{
	abase::hashtab<bool, int, abase::_hash_function>::pair_type pair = m_PlayedCameraIndex.get(indexCamera);
	if (bPlayed)
	{
		if (!pair.second)
		{
			m_PlayedCameraIndex.put(indexCamera,true);
		}
	}else
	{
		if (pair.second)
		{
			m_PlayedCameraIndex.erase(indexCamera);
		}
	}
}

// Stop current background music
void CECWorld::StopCurrMusic()
{
	CELBackMusic* pBackMusic = g_pGame->GetBackMusic();
	pBackMusic->StopMusic(true, true);
	pBackMusic->StopBackSFX(true);
}

// Restart background music
void CECWorld::RestartMusic()
{
	CELBackMusic* pBackMusic = g_pGame->GetBackMusic();
	bool bNight = m_pSunMoon->GetDNFactor() > 0.5f ? true : false;

	CECHostPlayer* pHost = GetHostPlayer();
	if (!pHost || !pHost->IsSkeletonReady())
		return;

	A3DVECTOR3 vPos = pHost->GetPos();
	CELPrecinct* pPrecinct = m_pPrecinctSet->IsPointIn(vPos.x, vPos.z, GetInstanceID());
	if (!pPrecinct)
	{
		pBackMusic->StopMusic(true, true);
		pBackMusic->StopBackSFX(true);
		CECGameUIMan* pGameUI = g_pGame->GetGameRun()->GetUIManager()->GetInGameUIMan();
		pGameUI->ChangePrecinct(pPrecinct);
		return;
	}

	//	Change background music
	pBackMusic->SetLoopType((CELBackMusic::LOOPTYPE)pPrecinct->GetMusicLoopType());
	pBackMusic->SetSilenceTime(0);//pPrecinct->GetMusicInterval() * 1000);

	int i, iNumMusic = pPrecinct->GetMusicFileNum();
	for (i=0; i < iNumMusic; i++)
		pBackMusic->PlayMusic(pPrecinct->GetMusicFile(i), false/*true*/, i == 0 ? true : false);

	//	Change background sfx
	AString str;
	if( bNight )
		str = pPrecinct->GetSoundFile_n();
	else
		str = pPrecinct->GetSoundFile();
	
	pPrecinct->SetNightSFX(bNight);
	if (str.GetLength() && af_CheckFileExt(str, ".wav"))
		pBackMusic->PlayBackSFX(str);
	else
		pBackMusic->StopBackSFX(true);
}

#ifndef BACK_VERSION

void CECWorld::RegisterManagers()
{
	ElementRender* pElementRender = g_pGame->GetGameRun()->GetElementRender();
	for(int i = 0; i < NUM_MANAGER; i++)
	{
		pElementRender->RegisterManager(m_aManagers[i]);
	}
}
void CECWorld::UnRegisterManagers()
{
	ElementRender* pElementRender = g_pGame->GetGameRun()->GetElementRender();
	for(int i = 0; i < NUM_MANAGER; i++)
	{
		pElementRender->UnRegisterManager(m_aManagers[i]);
	}
}

void CECWorld::RegisterWorldObjects()
{
	ElementRender* pElementRender = g_pGame->GetGameRun()->GetElementRender();
	if (pElementRender)
	{
		ElementRender::WorldObjects worldObjs = {0};

		worldObjs.pSunMoon         = m_pSunMoon;
		worldObjs.pTerrainOutline  = m_pTerrainOutline;
		worldObjs.pA3DTerrain      = m_pA3DTerrain;
		worldObjs.pA3DTerrainWater2= m_pA3DTerrainWater;
#ifdef _WATER_FFT_
		//worldObjs.pA3DTerrainWaterFFT = m_pA3DTerrainWaterFFT;
#endif
		worldObjs.pScene           = m_pScene;
		worldObjs.pGrassLand       = m_pGrassLand;
		worldObjs.pRain            = m_pRain;
		worldObjs.pSnow            = m_pSnow;
		worldObjs.pA3DSky          = m_pA3DSky;
		worldObjs.pCloudRender	   = m_pCloudManager;
		//worldObjs.pAnimationWorld  = &g_Animator;
		pElementRender->RegisterWorldObjects(worldObjs);

		pElementRender->SetWaterReflectMap("");
		// 		const CECInstance::sInstanceInfo* pInstance = CECInstance::GetInstance().GetInstanceInfo(m_idInst);
		// 		if (NULL != pInstance && pInstance->strCubeMap.GetLength() > 0)
		// 		{
		// 			pElementRender->SetWaterReflectMap(pInstance->strCubeMap);
		// 		}
	}
}

bool CECWorld::ResetManagers()
{
	for (int i=0; i < NUM_MANAGER; i++)
	{
		if (m_aManagers[i] &&
			i != MAN_ORNAMENT &&
			i != MAN_DEMOPLAYER)
		{
			m_aManagers[i]->OnLeaveGameWorld();
		}
	}
	for (int i=0; i < NUM_MANAGER; i++)
	{
		if (m_aManagers[i] &&
			i != MAN_ORNAMENT &&
			i != MAN_DEMOPLAYER)
		{
			m_aManagers[i]->OnEnterGameWorld();
		}
	}
	return true;
}

void CECWorld::UpdatePostEffect(DWORD dwDeltaTime)
{
	if (m_pScene)
	{
		m_dwPostEffectFlag = m_pScene->GetPostEffectFlag();
		SetEffectDOFParamDest(m_pScene->GetPostEffectDOFParam());

		if (m_pSunMoon)
		{
			float nf = m_pSunMoon->GetDNFactor();
			float df = 1.0f - nf;

			const A3DPOST_EFFECT_BLOOM_PARAM &bloomParamDay = m_pScene->GetPostEffectBloomParamDay();
			const A3DPOST_EFFECT_BLOOM_PARAM &bloomParamNight = m_pScene->GetPostEffectBloomParamNight();
			A3DPOST_EFFECT_BLOOM_PARAM bloomParam;
			bloomParam.fBlurSize = df * bloomParamDay.fBlurSize + nf * bloomParamNight.fBlurSize;
			bloomParam.fBrightThreshold = df * bloomParamDay.fBrightThreshold + nf * bloomParamNight.fBrightThreshold;
			bloomParam.fBrightScale = df * bloomParamDay.fBrightScale + nf * bloomParamNight.fBrightScale;
			SetEffectBloomParamDest(bloomParam);

			const A3DPOST_EFFECT_FULLGLOW_PARAM &fullGlowParamDay = m_pScene->GetPostEffectFullGlowParamDay();
			const A3DPOST_EFFECT_FULLGLOW_PARAM &fullGlowParamNight = m_pScene->GetPostEffectFullGlowParamNight();
			A3DPOST_EFFECT_FULLGLOW_PARAM fullGlowParam;
			fullGlowParam.fBlurSize = df * fullGlowParamDay.fBlurSize + nf * fullGlowParamNight.fBlurSize;
			fullGlowParam.fGlowLevel = df * fullGlowParamDay.fGlowLevel + nf * fullGlowParamNight.fGlowLevel;
			fullGlowParam.fGlowPower = df * fullGlowParamDay.fGlowPower + nf * fullGlowParamNight.fGlowPower;
			fullGlowParam.vGlowColor.x = df * fullGlowParamDay.vGlowColor.x + nf * fullGlowParamNight.vGlowColor.x;
			fullGlowParam.vGlowColor.y = df * fullGlowParamDay.vGlowColor.y + nf * fullGlowParamNight.vGlowColor.y;
			fullGlowParam.vGlowColor.z = df * fullGlowParamDay.vGlowColor.z + nf * fullGlowParamNight.vGlowColor.z;
			fullGlowParam.vGlowColor.w = df * fullGlowParamDay.vGlowColor.w + nf * fullGlowParamNight.vGlowColor.w;
			SetEffectFullGlowParamDest(fullGlowParam);
		}
	}

	m_EffectDOFParamLast.fNearBlur = (m_EffectDOFParamLast.fNearBlur + m_EffectDOFParamDest.fNearBlur) * 0.5f;
	m_EffectDOFParamLast.fNearFocus = (m_EffectDOFParamLast.fNearFocus + m_EffectDOFParamDest.fNearFocus) * 0.5f;
	m_EffectDOFParamLast.fFarFocus = (m_EffectDOFParamLast.fFarFocus + m_EffectDOFParamDest.fFarFocus) * 0.5f;
	m_EffectDOFParamLast.fFarBlur = (m_EffectDOFParamLast.fFarBlur + m_EffectDOFParamDest.fFarBlur) * 0.5f;
	m_EffectDOFParamLast.fClampBlurFar = (m_EffectDOFParamLast.fClampBlurFar + m_EffectDOFParamDest.fClampBlurFar) * 0.5f;

	m_EffectBloomParamLast.fBlurSize = (m_EffectBloomParamLast.fBlurSize + m_EffectBloomParamDest.fBlurSize) * 0.5f;
	m_EffectBloomParamLast.fBrightThreshold = (m_EffectBloomParamLast.fBrightThreshold + m_EffectBloomParamDest.fBrightThreshold) * 0.5f;
	m_EffectBloomParamLast.fBrightScale = (m_EffectBloomParamLast.fBrightScale + m_EffectBloomParamDest.fBrightScale) * 0.5f;

	m_EffectFullGlowParamLast.fBlurSize = (m_EffectFullGlowParamLast.fBlurSize + m_EffectFullGlowParamDest.fBlurSize) * 0.5f;
	m_EffectFullGlowParamLast.fGlowLevel = (m_EffectFullGlowParamLast.fGlowLevel + m_EffectFullGlowParamDest.fGlowLevel) * 0.5f;
	m_EffectFullGlowParamLast.fGlowPower = (m_EffectFullGlowParamLast.fGlowPower + m_EffectFullGlowParamDest.fGlowPower) * 0.5f;
	m_EffectFullGlowParamLast.vGlowColor.x = (m_EffectFullGlowParamLast.vGlowColor.x + m_EffectFullGlowParamDest.vGlowColor.x) * 0.5f;
	m_EffectFullGlowParamLast.vGlowColor.y = (m_EffectFullGlowParamLast.vGlowColor.y + m_EffectFullGlowParamDest.vGlowColor.y) * 0.5f;
	m_EffectFullGlowParamLast.vGlowColor.z = (m_EffectFullGlowParamLast.vGlowColor.z + m_EffectFullGlowParamDest.vGlowColor.z) * 0.5f;
	m_EffectFullGlowParamLast.vGlowColor.w = (m_EffectFullGlowParamLast.vGlowColor.w + m_EffectFullGlowParamDest.vGlowColor.w) * 0.5f;

	m_EffectCC = m_pScene->GetPostEffectCCParamDay();
	m_EffectCC.fFactor = m_pSunMoon->GetDNFactor();
	m_EffectCC.szTex2 = m_pScene->GetPostEffectCCParamNight().szTex1;
}
#else

void WorldRenderForReflected(A3DViewport * pViewport, void * pArg)
{
	CECWorld * pWorld = (CECWorld *) pArg;
	pWorld->RenderForReflected(pViewport);
}

void WorldRenderForRefracted(A3DViewport * pViewport, void * pArg)
{
	CECWorld * pWorld = (CECWorld *) pArg;
	pWorld->RenderForRefracted(pViewport);
}

void WorldRenderOnSky(A3DViewport * pViewport, void * pArg)
{
	CECWorld * pWorld = (CECWorld *) pArg;
	pWorld->RenderOnSky(pViewport);
}

void WorldRenderForBloom(A3DViewport * pViewport, void * pArg)
{
	CECWorld * pWorld = (CECWorld *) pArg;
	pWorld->RenderForBloom(pViewport);
}

void WorldRenderForScreen(A3DViewport * pViewport, void * pArg)
{
	CECWorld * pWorld = (CECWorld *) pArg;
	pWorld->RenderForScreen(pViewport);
}

#endif

