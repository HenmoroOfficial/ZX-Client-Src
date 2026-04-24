/*
 * FILE: EC_World.h
 *
 * DESCRIPTION: 
 *
 * CREATED BY: Duyuxin, 2004/8/25
 *
 * HISTORY: 
 *
 * Copyright (c) 2004 Archosaur Studio, All Rights Reserved.
 */

#pragma once

#ifdef BACK_VERSION
#include "EC_Manager.h"
#else
#include "ECManager.h"
#include "ECManagerID.h"
#include "ElementRender.h"
#define CECManager ECManager
#endif
#include "EC_MsgDef.h"
#include "AString.h"
#include <hashtab.h>

///////////////////////////////////////////////////////////////////////////
//	
//	Define and Macro
//	
///////////////////////////////////////////////////////////////////////////


///////////////////////////////////////////////////////////////////////////
//	
//	Types and Global variables
//	
///////////////////////////////////////////////////////////////////////////

class A3DTerrain2;
class A3DSkySphere;
class A3DViewport;
class CECGameRun;
class CECPlayerMan;
class CECNPCMan;
class CECMatterMan;
class CECDecalMan;
class CECSkillGfxMan;
class CECAttacksMan;
class CECHostPlayer;
class CECCDS;
class CECShadowRender;
class CECOrnamentMan;
class CELForest;
class CELTree;
class CELCloudManager;
class CECObject;
class CELPrecinctSet;
class CELPrecinct;
class CELRegionSet;
class CELRegion;
class A3DRain;
class A3DSnow;
class CECSceneLights;
class A3DTerrain2CullHC;
class CECAssureMove;
class A3DGFXEx;
class CECBezier;
class A3DSkyBox;

#ifdef BACK_VERSION
class A3DTerrainWater;
class CECViewport;
class CELGrassLand;
class CELGrassType;
class CELTerrainOutline2;
class CECSunMoon;
#else
class A3DTerrainWater2;
class ECViewport;
#define CECViewport ECViewport
class A3DGrassLand;
class A3DGrassType;
class A3DTerrainOutline2;
class A3DSunMoon;
#endif

// use global var first
// namespace AutoMove
// {
// 	class CMoveMap;
// }

///////////////////////////////////////////////////////////////////////////
//	
//	Declare of Global functions
//	
///////////////////////////////////////////////////////////////////////////


///////////////////////////////////////////////////////////////////////////
//	
//	Class CECWorld
//	
///////////////////////////////////////////////////////////////////////////

class CECWorld
{
public:		//	Types

	friend class CECScene;

	typedef abase::hashtab<CECBezier*, int, abase::_hash_function>	CutsceneBezierTable;

public:		//	Constructor and Destructor

	CECWorld(CECGameRun* pGameRun);
	virtual ~CECWorld();

public:		//	Attributes

public:		//	Operations

	//	Initialize object
	bool Init(int idInst);
	//	Release object
	void Release();

	//	Tick routine
	bool Tick(DWORD dwDeltaTime, CECViewport* pViewport);

#ifdef BACK_VERSION
	//	Render routine
	bool Render(CECViewport* pViewport);
	//	Render shadows routine
	bool RenderShadows(CECViewport* pViewport);
	//	Render grass at last
	bool RenderGrasses(CECViewport* pViewport, bool bRenderAlpha);
	//	Render for reflected
	bool RenderForReflected(A3DViewport* pViewport);
	//	Render for refracted
	bool RenderForRefracted(A3DViewport* pViewport);
	//	Render something on sky
	bool RenderOnSky(A3DViewport* pViewport);
	//	Render something for bloom
	bool RenderForBloom(A3DViewport * pViewport);
	//	Render something for screen
	bool RenderForScreen(A3DViewport * pViewport);
#else
	bool __PreRender(ECViewport* pViewport);
#endif

	//	Tick animation
	bool TickAnimation();

	//	Load world from file
	bool LoadWorld(const char* szFile, const char * szDataPath, const A3DVECTOR3& vInitLoadPos);
	//	Set load center
	void SetLoadCenter(const A3DVECTOR3& vCenter);
	//	Set view radius
	void SetViewRadius(float fRadius);
	//	Set time of the day and adjust several state to make the change take effects
	void SetTimeOfDay(float t);
	//	Checkout instance data
	bool CheckOutInst(int idInst, DWORD dwRegionTimeStamp, DWORD dwPrecinctTimeStamp);

	//	Get terrain height of specified position
	float GetTerrainHeight(const A3DVECTOR3& vPos, A3DVECTOR3* pvNormal=NULL);
	//	Get water height of specified position
	float GetWaterHeight(const A3DVECTOR3& vPos);
	//	Get ground height of specified poistion
	float GetGroundHeight(const A3DVECTOR3& vPos, A3DVECTOR3* pvNormal=NULL);
	//	Get terrain object
	A3DTerrain2* GetTerrain() { return m_pA3DTerrain; }
	//	Get terrain water object
#ifdef BACK_VERSION
	A3DTerrainWater * GetTerrainWater() { return m_pA3DTerrainWater; }
#else
	A3DTerrainWater2 * GetTerrainWater() { return m_pA3DTerrainWater; }
#endif
	//	Get terrain culler object
	A3DTerrain2CullHC* GetTerrainCuller() { return m_pA3DTrnCuller; }
	//	Get A3DSky object
	A3DSkySphere * GetSkySphere();
	A3DSkyBox * GetSkyBox();
	//	Get terrain outline object
#ifdef BACK_VERSION
	CELTerrainOutline2 * GetTerrainOutline() { return m_pTerrainOutline; }
#else
	A3DTerrainOutline2 * GetTerrainOutline() { return m_pTerrainOutline; }
#endif
	//	Get cloud manager
	CELCloudManager * GetCloudManager() { return m_pCloudManager; }
	//	Get scene object
	CECScene* GetScene() { return m_pScene; }
	//	Get sunmoon object
#ifdef BACK_VERSION
	CECSunMoon* GetSunMoon() { return m_pSunMoon; }
#else
	A3DSunMoon* GetSunMoon() { return m_pSunMoon; }
#endif

	//	Get manager interface
	CECPlayerMan* GetPlayerMan() { return (CECPlayerMan*)m_aManagers[MAN_PLAYER]; }
	CECNPCMan* GetNPCMan() { return (CECNPCMan*)m_aManagers[MAN_NPC]; }
	CECMatterMan* GetMatterMan() { return (CECMatterMan*)m_aManagers[MAN_MATTER]; }
	CECOrnamentMan* GetOrnamentMan() { return (CECOrnamentMan*)m_aManagers[MAN_ORNAMENT]; }
	CECDecalMan* GetDecalMan() { return (CECDecalMan*)m_aManagers[MAN_DECAL]; }
	CECSkillGfxMan* GetSkillGfxMan() { return (CECSkillGfxMan*)m_aManagers[MAN_SKILLGFX]; }

	//	Get manager by index
#ifdef BACK_VERSION
	CECManager* GetManager(int n) { return m_aManagers[n]; }
#else
	ECManager* GetManager(int n) { return m_aManagers[n]; }
#endif
	CECHostPlayer* GetHostPlayer();
	//	Get object by specified ID
	CECObject* GetObject(int idObject, int iAliveFlag);
	//	Get CDS interface
	CECCDS* GetCDS() { return m_pCDS; }
	//	Get forest interface
	CELForest* GetForest() { return m_pForest; }
	//	Get Grassland interface
#ifdef BACK_VERSION
	CELGrassLand * GetGrassLand() { return m_pGrassLand; }
#else
	A3DGrassLand * GetGrassLand() { return m_pGrassLand; }
#endif
	//	Get world loaded flag
	bool IsWorldLoaded() { return m_bWorldLoaded; }
	//	Get map path
	const char* GetMapPath() { return m_strMapPath; }
	//	Get born stamp
	DWORD GetBornStamp() { return m_dwBornStamp++; }
	//	Get initial load position
	const A3DVECTOR3& GetInitLoadPos() { return m_vInitLoadPos; }
	//	Get princinct set
	CELPrecinctSet* GetPrecinctSet() { return m_pPrecinctSet; }
	//	Get current precinct which host player is in
	CELPrecinct* GetCurPrecinct() { return m_pCurPrecinct; }
	//	Get sanctuary region set
	CELRegionSet* GetRegionSet() { return m_pRegionSet; }
	//	Get scene light object
	CECSceneLights* GetSceneLights() { return m_pSceneLights; }	  
	//	Get assure move object
	CECAssureMove* GetAssureMove() { return m_pAssureMove; }
	//	Get cursor stereo gfx
	A3DGFXEx* GetCursorStereoGfx() { return m_pCursorStereoGfx; }
	//	Get target stereo gfx
	A3DGFXEx* GetTargetStereoGfx() { return m_pTargetStereoGfx; }

	//	Get id of instance
	int GetInstanceID() { return m_idInst; }
	
	bool IsCameraPlayed(int indexCamera);
	void SetCameraPlayed(int indexCamera, bool bPlayed = true);
	void ClearCameraPlayed(){ m_PlayedCameraIndex.clear();}
	
	// Get cutscene bezier curve
	CECBezier* GetCutsceneBezier(DWORD dwBezierIndex);

	// Stop current background music
	void StopCurrMusic();
	// Restart background music
	void RestartMusic();

	void SetSkyType(int nType)	{ ASSERT(nType ==0 || nType == 1); m_nSkyType = nType; }
	int	 GetSkyType()	{ return m_nSkyType; }

	void ForceLoadTerrain();
	CELRegion* GetCurRegion() { return m_pCurRegion; }

	bool GetTerrainHoleState(int index) { return m_aTerrainHoles[index]; }
#ifndef BACK_VERSION
	// For ElementRender
	void RegisterManagers();
	void UnRegisterManagers();
	void RegisterWorldObjects();

	//  Reset managers
	bool ResetManagers();

	const A3DPOST_EFFECT_DOF_PARAM& GetPostEffectDOFParam() const { return m_EffectDOFParamLast; }
	const A3DPOST_EFFECT_BLOOM_PARAM& GetPostEffectBloomParam() const { return m_EffectBloomParamLast; }
	const A3DPOST_EFFECT_FULLGLOW_PARAM& GetPostEffectFullGlowParam() const { return m_EffectFullGlowParamLast; }
	void SetEffectDOFParamDest(const A3DPOST_EFFECT_DOF_PARAM &param) { m_EffectDOFParamDest = param; }
	void SetEffectBloomParamDest(const A3DPOST_EFFECT_BLOOM_PARAM &param) { m_EffectBloomParamDest = param; }
	void SetEffectFullGlowParamDest(const A3DPOST_EFFECT_FULLGLOW_PARAM &param) { m_EffectFullGlowParamDest = param; }
	void UpdatePostEffect(DWORD dwDeltaTime);
#endif
	
// 	AutoMove::CMoveMap * GetAutoMoveMap()
// 	{
// 		return m_pAutoMoveMap;
// 	}
protected:	//	Attributes

	CECGameRun*			m_pGameRun;					//	Game run object
	A3DTerrain2*		m_pA3DTerrain;				//	Terrain object
#ifdef BACK_VERSION
	A3DTerrainWater*	m_pA3DTerrainWater;			//	Terrain water object
#else
	A3DTerrainWater2*	m_pA3DTerrainWater;			//	Terrain water object
#endif
	A3DTerrain2CullHC*	m_pA3DTrnCuller;			//	Terrain culler
	int					m_nSkyType;					//	Sky Type: 0:SkySphere 1:SkyBox
	A3DSkySphere*		m_pA3DSky;					//	Sky object
	A3DSkyBox*			m_pSkyBox;					//  Sky object 2
	CECCDS*				m_pCDS;						//	CDS object
	CECManager*			m_aManagers[NUM_MANAGER];	//	Manager array
	CELForest*			m_pForest;					//	Forest Object for the world
#ifdef BACK_VERSION
	CELGrassLand *		m_pGrassLand;				//	Grass Land Object for the world
	CELTerrainOutline2* m_pTerrainOutline;			//	Terrain outline object
#else
	A3DGrassLand *		m_pGrassLand;				//	Grass Land Object for the world
	A3DTerrainOutline2* m_pTerrainOutline;			//	Terrain outline object
#endif
	CELCloudManager *	m_pCloudManager;			//	Clouds object
	CECScene*			m_pScene;					//	Scene object
#ifdef BACK_VERSION
	CECSunMoon*			m_pSunMoon;					//	Sun and moon in the world
#else
	A3DSunMoon*			m_pSunMoon;					//	Sun and moon in the world
#endif
	A3DRain*			m_pRain;					//	rain in the world
	A3DSnow*			m_pSnow;					//	snow in the world
	CELPrecinctSet*		m_pPrecinctSet;				//	Precinct set
	CELPrecinct*		m_pCurPrecinct;				//	Current precinct host player is in
	CELRegionSet*		m_pRegionSet;				//	Region set
	CELRegion*			m_pCurRegion;				//	Current region host player is in
	CECSceneLights*		m_pSceneLights;				//	all lights in the scene
	CECAssureMove*		m_pAssureMove;				//	object used to assure move
	A3DGFXEx*			m_pCursorStereoGfx;
	A3DGFXEx*			m_pTargetStereoGfx;


// 	AutoMove::CMoveMap   * m_pAutoMoveMap;          //  auto move map

	AString		m_strMapPath;		//	Map path
	bool		m_bResetEnv;		//	true, reset environment
	bool		m_bWorldLoaded;		//	true, world has been loaded
	A3DVECTOR3	m_vInitLoadPos;		//	Initial load position
	bool		m_bCenterReset;		//	true, this tick center is reset
	DWORD		m_dwBornStamp;		//	Born stamp
	int			m_idInst;			//	id of instance

	float		m_fTrnLoadDelta;	//	Terrain loading speed control
	float		m_fTrnLoadCnt;
	float		m_fScnLoadDelta;	//	Scene loading speed control
	float		m_fScnLoadCnt;

#ifndef BACK_VERSION
	// Post Effect
	A3DPOST_EFFECT_DOF_PARAM      m_EffectDOFParamLast;
	A3DPOST_EFFECT_DOF_PARAM      m_EffectDOFParamDest;
	A3DPOST_EFFECT_BLOOM_PARAM    m_EffectBloomParamLast;
	A3DPOST_EFFECT_BLOOM_PARAM    m_EffectBloomParamDest;
	A3DPOST_EFFECT_FULLGLOW_PARAM m_EffectFullGlowParamLast;
	A3DPOST_EFFECT_FULLGLOW_PARAM m_EffectFullGlowParamDest;
#endif

	abase::hashtab<bool, int, abase::_hash_function> m_PlayedCameraIndex;//camera's index played in precinct(unique with precinct).
	CutsceneBezierTable	m_CtsBezierTab;	// cutscene bezier table
	bool		m_aTerrainHoles[256];

protected:	//	Operations

	//	Initialize terrain water
	bool InitTerrainWater();
	//	Initialize plants objects
	bool InitPlantsObjects();

	// Initialize nature effects
	bool InitNatureObjects();
	// Release nature effects
	void ReleaseNatureObjects();
	// Render nature objects
	bool RenderNatureObjects(CECViewport* pViewport);
	// Tick nature objects
	bool TickNatureObjects(DWORD dwDeltaTime);
	//	Update current precinct which the host player is in
	void UpdateCurPrecinct();
	//	Update current region which the host player is in
	void UpdateCurRegion();

	//	Load terrain
	bool LoadTerrain(const char* szFile);
	//	Load terrain outline
	bool LoadTerrainOutline(const char * szTilePath, const char * szTexPath);
	//	Load clouds
	bool LoadClouds(const char * szFile, const char * szTexPath);
	//	Load scene objects
	bool LoadSceneObjects(const char* szFile);
	//	Load sky
	bool LoadSky();
	//	Load plants
	bool LoadPlants(const char * szFile);	
	//  Load MoveMaps
	bool LoadMoveMaps();
	//	Release current scene
	void ReleaseScene();
	//	Calculate terrain and scene loading speed
	void CalcSceneLoadSpeed();

	//  Load cutscene beziers
	bool LoadCutsceneBeziers(const char* szFile);
	//  Load terrain holes
	bool LoadTerrainHole(const char* szFile);

	//	Create managers
	bool CreateManagers();
	//	Release managers
	void ReleaseManagers();
};

#ifdef BACK_VERSION
void WorldRenderForReflected(A3DViewport * pViewport, void * pArg);
void WorldRenderForRefracted(A3DViewport * pViewport, void * pArg);
void WorldRenderOnSky(A3DViewport * pViewport, void * pArg);
void WorldRenderForBloom(A3DViewport * pViewport, void * pArg);
void WorldRenderForScreen(A3DViewport * pViewport, void * pArg);
#endif

///////////////////////////////////////////////////////////////////////////
//	
//	Inline functions
//	
///////////////////////////////////////////////////////////////////////////

