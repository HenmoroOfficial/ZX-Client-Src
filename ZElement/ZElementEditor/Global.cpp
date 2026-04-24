 /*
 * FILE: Global.cpp
 *
 * DESCRIPTION: Global variables and definitions
 *
 * CREATED BY: Duyuxin, 2002/9/1
 *
 * HISTORY:
 *
 * Copyright (c) 2001 Archosaur Studio, All Rights Reserved.
 */

#include "Global.h"
//#include "ElementResMan.h"
#include "A3D.h"
#include "ElementEditor.h"
#include "AMemory.h"
#include "OperationManager.h"
#include "SceneObjectMoveOperation.h"
#include "SceneObjectScaleOperation.h"
#include "SceneObjectRotateOperation.h"
#include "elementdataman.h"
#include "TerrainBlockViewDlg.h"
#include "VssOperation.h"
#include "PathMap\TerrainObstructGenerator.h"
#include "A3DGFXMan.h"
#include "Render.h"
#include "ElementMap.h"
#include "MainFrm.h"
#include "SceneObject.h"
#include "SceneObjectManager.h"
#include "StaticModelObject.h"
#include "AScriptFile.h"
#include "SceneAIGenerator.h"
#include "EL_BackMusic.h"
#include "..\ZElementData\DataPathMan.h"
#include <AF.h>

#ifdef _ANGELICA22
#include "A3DEnvironment.h"
#endif

//#define new A_DEBUG_NEW

///////////////////////////////////////////////////////////////////////////
//
//	Define and Macro
//
///////////////////////////////////////////////////////////////////////////

CString sz_map_file_ex[] = 
{
	"%s.elproj",
	"%s.scene",
	"%s.whmap",
	"%s.water",
	"%s.terrain",
	"%s.scld",
	"%s.record",
	"%s.plt",
	"%s.hmap",
	"%s.gld",
	"%s.cldo",
	"%s.octr",
	"%s.parmap",
	"%s-0.rmap",
	"%s-0.dhmap",
	"%s-0.prmap",
	"%s-0.pdhmap",
	"%s-1.rmap",
	"%s-1.dhmap",
	"%s-1.prmap",
	"%s-1.pdhmap",
	"%s-2.rmap",
	"%s-2.dhmap",
	"%s-2.prmap",
	"%s-2.pdhmap",
	"%s.cmap",
};

///////////////////////////////////////////////////////////////////////////
//
//	Reference to External variables and functions
//
///////////////////////////////////////////////////////////////////////////

ALog		g_Log;
ALog        g_LogVss;
EDITCONFIG	g_Configs;
char		g_szWorkDir[MAX_PATH];
char		g_szIniFile[MAX_PATH];		//	.ini file name
char		g_szEditMapDir[MAX_PATH];	//	Edit maps directory
char		g_szTempMap[MAX_PATH];		//	Temporary map data directory
char		g_szDefTexFile[MAX_PATH];	//	Default texture file name
char        g_szProject[256];      //Project name
const char*	g_szVersion = "1.0.3.1";

const char* g_szCompileTime = "[2012/11/08 17:10]"; // Compile time, to let designer know details of current version.
													// Should be changed before ElememntEditor.exe generates

A3DVECTOR3	g_vOrigin(0.0f);
A3DVECTOR3	g_vAxisX(1.0f, 0.0f, 0.0f);
A3DVECTOR3	g_vAxisY(0.0f, 1.0f, 0.0f);
A3DVECTOR3	g_vAxisZ(0.0f, 0.0f, 1.0f);

A3DVECTOR3  g_vLastPos;
A3DVECTOR3  g_vLastDir;
bool        g_bCameraCapture = false;
float       g_fCameraFov = 56.0f;
float       g_fCameraRadio = 1.333f;
bool        g_bShowCamDir = false;
bool        g_bLockCam = false;
CEdtDataCache	  g_EdtDataCache;

#ifdef _ANGELICA22_FEATURE
A3DPOST_EFFECT_PARAM      g_PostEffectParam;
A3DPOST_EFFECT_PARAM      g_PostEffectParamNight;
#endif
DWORD					  g_dwPostEffectAdditionalFlags = 0;		// 编辑器使用的一些额外的控制标志

HCURSOR    g_hMoveCursor;
HCURSOR    g_hSelectCursor;
HCURSOR    g_hScaleCursor;
HCURSOR    g_hRotateCursor;
HCURSOR    g_hAddObjectCursor;

elementdataman			  g_dataMan;
DataPathMan               g_dataPathMan;
CTerrainObstructGenerator g_TerrainObstuctGenerator;
CBezierMergeDlg           g_BezierMergeDlg;
CLogDlg                   g_LogDlg;
int                       g_nCurrentTerrainBlock = -1;
CTerrainBlockViewDlg      g_BlockViewDlg;
bool                      g_bIsNight = false;
CVssOperation             g_VSS;
CELBackMusic              g_BackMusic;
bool                      g_bReadOnly = true;
bool                      g_bShowPosDot = true;
bool                      g_bBezierPtSnap = false;
bool                      g_bShowModelPlane = false;
bool                      g_bShowSelectedOnly = false;
float                     g_fNightFactor    = 0.0f;
bool                      g_bShowTerrainLight = true;
bool					  g_bOnlyCalMiniMap = false;
DWORD                     g_dwExpCopy = 1;
bool                      g_bShowCollideModelOnly = false;
DWORD                     g_dwWaterColor = 0xa0254f55;
bool                      g_bWTPacket = true;
int						  g_nCurPlayerPassmapLayer = 0;
int						  g_nCurPassmapLayer = 0;
bool					  g_bOnlyLoadNearBuildForLight = false;

class A3DSkillGfxComposerMan;
class A3DSkillGfxMan;
A3DSkillGfxComposerMan*   AfxGetSkillGfxComposerMan(){ return NULL;}
A3DSkillGfxMan * AfxGetSkillGfxEventMan(){ return NULL; }
//void AfxSkillGfxShowCaster(int idCaster, DWORD dwModifier) {}

//	Terrain export method
int	g_iTrnExport = 1;
bool g_bTrnCompressed = true;

///////////////////////////////////////////////////////////////////////////
//
//	Local Types and Variables and Global variables
//
///////////////////////////////////////////////////////////////////////////

static DWORD			l_dwProgressData	= 0;	//	User data of progress
static LPFNLOGOUTPUT	l_lpfnLogOutput		= NULL;
static LPFNPROGRESS		l_lpfnProgress		= NULL;

///////////////////////////////////////////////////////////////////////////
//
//	Local functions
//
///////////////////////////////////////////////////////////////////////////


///////////////////////////////////////////////////////////////////////////
//
//	Implement
//
///////////////////////////////////////////////////////////////////////////

CElementEditorApp* AUX_GetApp()
{
	return (CElementEditorApp*)AfxGetApp();
}

CMainFrame*	AUX_GetMainFrame()
{	
	return (CMainFrame*)AfxGetMainWnd();
}

//	Output message with window's message box
int AUX_MessageBox(int iType, const char* szMsg, ...)
{
	static char szText[1024];
	
	va_list vaList;
	va_start(vaList, szMsg);
	vsprintf(szText, szMsg, vaList);
	va_end(vaList);

	return ((CWnd*)AUX_GetMainFrame())->MessageBox(szText, "提示", iType);
}

//	Restore configs to default value
void AUX_DefaultConfigs()
{
	//	General options
	g_Configs.fAirPassMapDelta  = 10.0f;
	g_Configs.fCameraMove		= 20.0f;
	g_Configs.fCameraRot		= 0.1f;
	g_Configs.bAlongGround		= false;
	g_Configs.bEnableSpeak      = true;
	g_Configs.bShowModelHull    = false;
	g_Configs.bShowBoundBox		= false;
	g_Configs.bHostMode			= false;

	// Pick object options
	g_Configs.bPickStaticModel	= true;   
	g_Configs.bPickLight		= true;
	g_Configs.bPickAudio        = true;
	g_Configs.bPickBoxArea      = true;
	g_Configs.bPickDummy        = true;

	g_Configs.bPickSpecially    = true;
	g_Configs.bPickWater        = true;
	g_Configs.bPickAIArea       = true;
	g_Configs.bPickPath         = true;
	g_Configs.bPickSkinModel    = true;
	g_Configs.bPickCritterGroup = true;
	g_Configs.bPickFixedNpc     = true;
	g_Configs.bPickPrecinct     = true;
	g_Configs.bPickGather       = true;
	g_Configs.bPickRange        = true;
	g_Configs.bPickCloudBox     = true;
	g_Configs.bPickInstanceBox  = true;
	g_Configs.bPickDynamic      = true;
	g_Configs.bPickBuffRegion	= true;
	g_Configs.bPickGuildBuilding	= true;

	//	New map settings
	g_Configs.fTileSize			= 2.0f;
	g_Configs.fMinHeight		= 0.0f;
	g_Configs.fMaxHeight		= 800.0f;

	g_Configs.iMaxTexSize		= 256;

	//	Render options
	g_Configs.fViewRadius		= 200.0f;
	g_Configs.bWireFrame		= false;
	g_Configs.bImmeMaskPaint	= true;
	
	g_Configs.bEnableSky        = true;
	g_Configs.iMaxLayerWeight	= A3DTerrain2::MAX_LAYERWEIGHT;
	g_Configs.bShowObjectName   = true;
	g_Configs.bObjectAddContinue= false;
	g_Configs.bShowVernier      = false;
	g_Configs.bEnableFog        = true;

#ifdef _ANGELICA22_FEATURE
	g_Configs.nWaterEffect      = 0;
	g_Configs.bHDRLitmap		= true;
#else
	g_Configs.bSampleWater      = true;
#endif
	g_Configs.bEnableGlbFog     = true;
	
	g_Configs.bShowStaticModel	= true;
	g_Configs.bShowLight		= true;
	g_Configs.bShowAudio		= true;
	g_Configs.bShowBoxArea		= true;
	g_Configs.bShowDummy		= true;
	g_Configs.bShowSpecially	= true;
	g_Configs.bShowWater		= true;
	g_Configs.bShowAIArea		= true;
	g_Configs.bShowPath			= true;
	g_Configs.bShowCritterGroup = true;
	g_Configs.bShowSkinModel    = true;
	g_Configs.bShowFixedNpc     = true;
	g_Configs.bShowFixedNpcWire	= true;
	g_Configs.bShowTerrainBlock = true;
	g_Configs.bShowSmallBlock   = true;
	g_Configs.bShowRain         = false;
	g_Configs.bShowSnow         = false;
	g_Configs.bShowTree         = true;
	g_Configs.bShowGrass        = true;
	g_Configs.bShowGather       = true;
	g_Configs.bShowTerrain      = true;
	g_Configs.bShowRange        = true;
	g_Configs.bShowCloudBox     = true;
	g_Configs.bShowCloudLine    = false;
	g_Configs.bShowInstanceBox  = false;
	g_Configs.nShowCopyNum      = 0;
	g_Configs.bShowDynamic      = true;
	g_Configs.bShowPostEffect	= false;
	g_Configs.bShowSunMoon		= true;
	
	//	Export options
	g_Configs.iTExpMaskTile		= 128;
	g_Configs.iTExpColNum		= 8;
	g_Configs.iTExpMaxLayer		= 2;
	g_Configs.iTExpBlkGrid		= 32;

	//  Brush options
	g_Configs.iBrushRadius		= 5;
	g_Configs.bCircleBrush      = true;
	g_Configs.fCurveControl1    = 0.0f;
	g_Configs.fCurveControl2    = 0.0f;
	g_Configs.fCurveControl3    = 0.0f;
	g_Configs.fCurveControl4    = 0.0f;
	g_Configs.fCurveControl5    = 0.0f;
	g_Configs.fCurveControl6    = 0.0f;
	g_Configs.fCurveControl7    = 0.0f;

	//	Lighting settings
	g_Configs.iLightMapSize		= 1024;
	g_Configs.iSunDir			= 0;
	g_Configs.iSunHeight		= 50;
	g_Configs.dwSunCol			= 0xffc08020;
	g_Configs.dwAmbient			= 0xff604000;
	g_Configs.fSunPower         = 1.0f;

	g_Configs.iNightLightMapSize	= 1024;
	g_Configs.iNightSunDir			= 0;
	g_Configs.iNightSunHeight		= 50;
	g_Configs.dwNightSunCol			= 0xffc08020;
	g_Configs.dwNightAmbient		= 0xff604000;
	g_Configs.fNightSunPower		= 1.0f;
	g_Configs.fSunFaction           = 1.0f;
	g_Configs.fNightSunFaction      = 1.0f;

	// Visual source safe case
	strcpy(g_Configs.szServerPath,"TestVssData");
	strcpy(g_Configs.szUserName,"hill");
	strcpy(g_Configs.szUserPassword,"3210");

	//
	strcpy(g_Configs.szCurProjectName,"world");
	strcpy(g_Configs.szExpPath,"");

	g_Configs.iSfxVolume = 50;
	g_Configs.iMusicVolume = 50;

	g_Configs.iIsOverseaVersion = 0;
}

//	Save configs to .ini file
void AUX_SaveConfigs()
{
	char szStr[256], szSect[128];

	//	General section
	strcpy(szSect, "General");

	sprintf(szStr, "%f", g_Configs.fCameraMove);
	WritePrivateProfileString(szSect, "Camera_Move", szStr, g_szIniFile);

	sprintf(szStr, "%f", g_Configs.fCameraRot);
	WritePrivateProfileString(szSect, "Camera_Rot", szStr, g_szIniFile);

	sprintf(szStr, "%d", g_Configs.bAlongGround ? 1 : 0);
	WritePrivateProfileString(szSect, "Along_Ground", szStr, g_szIniFile);

	sprintf(szStr, "%d", g_Configs.bEnableSpeak ? 1 : 0);
	WritePrivateProfileString(szSect, "Speak_Enable", szStr, g_szIniFile);
	
	sprintf(szStr, "%d", g_Configs.bShowModelHull ? 1 : 0);
	WritePrivateProfileString(szSect, "Show_Hull", szStr, g_szIniFile);

	sprintf(szStr, "%d", g_Configs.bShowBoundBox ? 1 : 0);
	WritePrivateProfileString(szSect, "Show_BoundBox", szStr, g_szIniFile);

	sprintf(szStr, "%d", g_Configs.bHostMode ? 1 : 0);
	WritePrivateProfileString(szSect, "Host_Mode", szStr, g_szIniFile);

	sprintf(szStr, "%d", g_Configs.bNeighborTerrainOnly ? 1 : 0);
	WritePrivateProfileString(szSect, "Neighbor_Terrain_Only", szStr, g_szIniFile);

	//	New map settings
	sprintf(szStr, "%f", g_Configs.fTileSize);
	WritePrivateProfileString(szSect, "Tile_Size", szStr, g_szIniFile);

	sprintf(szStr, "%f", g_Configs.fMinHeight);
	WritePrivateProfileString(szSect, "Min_Height", szStr, g_szIniFile);

	sprintf(szStr, "%f", g_Configs.fMaxHeight);
	WritePrivateProfileString(szSect, "Max_Height", szStr, g_szIniFile);
	
	// Pick object options
	strcpy(szSect, "Pick");
	
	sprintf(szStr, "%d", g_Configs.bPickStaticModel ? 1 : 0);
	WritePrivateProfileString(szSect, "Pick_StaticModel", szStr, g_szIniFile);
	
	sprintf(szStr, "%d", g_Configs.bPickLight ? 1 : 0);
	WritePrivateProfileString(szSect, "Pick_Light", szStr, g_szIniFile);

	sprintf(szStr, "%d", g_Configs.bPickAudio ? 1 : 0);
	WritePrivateProfileString(szSect, "Pick_Audio", szStr, g_szIniFile);

	sprintf(szStr, "%d", g_Configs.bPickBoxArea ? 1 : 0);
	WritePrivateProfileString(szSect, "Pick_BoxArea", szStr, g_szIniFile);

	sprintf(szStr, "%d", g_Configs.bPickDummy ? 1 : 0);
	WritePrivateProfileString(szSect, "Pick_Dummy", szStr, g_szIniFile);

	sprintf(szStr, "%d", g_Configs.bPickSpecially ? 1 : 0);
	WritePrivateProfileString(szSect, "Pick_Specially", szStr, g_szIniFile);

	sprintf(szStr, "%d", g_Configs.bPickWater ? 1 : 0);
	WritePrivateProfileString(szSect, "Pick_Water", szStr, g_szIniFile);

	sprintf(szStr, "%d", g_Configs.bPickAIArea ? 1 : 0);
	WritePrivateProfileString(szSect, "Pick_AIArea", szStr, g_szIniFile);

	sprintf(szStr, "%d", g_Configs.bPickPath ? 1 : 0);
	WritePrivateProfileString(szSect, "Pick_Path", szStr, g_szIniFile);

	sprintf(szStr, "%d", g_Configs.bPickSkinModel ? 1 : 0);
	WritePrivateProfileString(szSect, "Pick_SkinModel", szStr, g_szIniFile);

	sprintf(szStr, "%d", g_Configs.bPickCritterGroup ? 1 : 0);
	WritePrivateProfileString(szSect, "Pick_CritterGroup", szStr, g_szIniFile);

	sprintf(szStr, "%d", g_Configs.bPickFixedNpc ? 1 : 0);
	WritePrivateProfileString(szSect, "Pick_FixedNpc", szStr, g_szIniFile);
	
	sprintf(szStr, "%d", g_Configs.bPickPrecinct ? 1 : 0);
	WritePrivateProfileString(szSect, "Pick_Precinct", szStr, g_szIniFile);

	sprintf(szStr, "%d", g_Configs.bPickGather ? 1 : 0);
	WritePrivateProfileString(szSect, "Pick_Gather", szStr, g_szIniFile);
	
	sprintf(szStr, "%d", g_Configs.bPickRange ? 1 : 0);
	WritePrivateProfileString(szSect, "Pick_Range", szStr, g_szIniFile);

	sprintf(szStr, "%d", g_Configs.bPickCloudBox ? 1 : 0);
	WritePrivateProfileString(szSect, "Pick_CloudBox", szStr, g_szIniFile);

	sprintf(szStr, "%d", g_Configs.bPickInstanceBox ? 1 : 0);
	WritePrivateProfileString(szSect, "Pick_InstanceBox", szStr, g_szIniFile);
	
	sprintf(szStr, "%d", g_Configs.bPickDynamic ? 1 : 0);
	WritePrivateProfileString(szSect, "Pick_Dynamic", szStr, g_szIniFile);

	sprintf(szStr, "%d", g_Configs.bPickBuffRegion ? 1 : 0);
	WritePrivateProfileString(szSect, "Pick_BuffRegion", szStr, g_szIniFile);
	sprintf(szStr, "%d", g_Configs.bPickGuildBuilding ? 1 : 0);
	WritePrivateProfileString(szSect, "Pick_GuildBuilding", szStr, g_szIniFile);
	//	Render section
	strcpy(szSect, "Render");

	sprintf(szStr, "%f", g_Configs.fViewRadius);
	WritePrivateProfileString(szSect, "Render_Radius", szStr, g_szIniFile);

	sprintf(szStr, "%d", g_Configs.bImmeMaskPaint ? 1 : 0);
	WritePrivateProfileString(szSect, "Imme_Mask_Paint", szStr, g_szIniFile);

	sprintf(szStr, "%d", g_Configs.bEnableSky ? 1 : 0);
	WritePrivateProfileString(szSect, 	"Enable_Sky", szStr, g_szIniFile);
	
	sprintf(szStr, "%d", g_Configs.bShowObjectName ? 1 : 0);
	WritePrivateProfileString(szSect, "Show_ObjectName", szStr, g_szIniFile);

#ifdef _ANGELICA22_FEATURE
	sprintf(szStr, "%d", g_Configs.nWaterEffect);
	WritePrivateProfileString(szSect, "Water_Effect", szStr, g_szIniFile);

	sprintf(szStr, "%d", g_Configs.bHDRLitmap ? 1 : 0);
	WritePrivateProfileString(szSect, "Show_HDRLitmap", szStr, g_szIniFile);
#else
	sprintf(szStr, "%d", g_Configs.bSampleWater ? 1 : 0);
	WritePrivateProfileString(szSect, "Show_SampleWater", szStr, g_szIniFile);
#endif
	//new
	sprintf(szStr, "%d", g_Configs.bShowAIArea ? 1 : 0);
	WritePrivateProfileString(szSect, "Show_AIArea", szStr, g_szIniFile);
	sprintf(szStr, "%d", g_Configs.bShowAudio ? 1 : 0);
	WritePrivateProfileString(szSect, "Show_Audio", szStr, g_szIniFile);
	sprintf(szStr, "%d", g_Configs.bShowBoxArea ? 1 : 0);
	WritePrivateProfileString(szSect, "Show_BoxArea", szStr, g_szIniFile);
	sprintf(szStr, "%d", g_Configs.bShowCritterGroup ? 1 : 0);
	WritePrivateProfileString(szSect, "Show_CritterGroup", szStr, g_szIniFile);
	sprintf(szStr, "%d", g_Configs.bShowDummy ? 1 : 0);
	WritePrivateProfileString(szSect, "Show_Dummy", szStr, g_szIniFile);
	sprintf(szStr, "%d", g_Configs.bShowFixedNpc ? 1 : 0);
	WritePrivateProfileString(szSect, "Show_FixedNpc", szStr, g_szIniFile);
	sprintf(szStr, "%d", g_Configs.bShowFixedNpcWire ? 1 : 0);
	WritePrivateProfileString(szSect, "Show_FixedNpcWire", szStr, g_szIniFile);
	sprintf(szStr, "%d", g_Configs.bShowGather ? 1 : 0);
	WritePrivateProfileString(szSect, "Show_Gather", szStr, g_szIniFile);
	sprintf(szStr, "%d", g_Configs.bShowGrass ? 1 : 0);
	WritePrivateProfileString(szSect, "Show_Grass", szStr, g_szIniFile);
	sprintf(szStr, "%d", g_Configs.bShowLight ? 1 : 0);
	WritePrivateProfileString(szSect, "Show_Light", szStr, g_szIniFile);
	sprintf(szStr, "%d", g_Configs.bShowPath ? 1 : 0);
	WritePrivateProfileString(szSect, "Show_Path", szStr, g_szIniFile);
	sprintf(szStr, "%d", g_Configs.bShowRange ? 1 : 0);
	WritePrivateProfileString(szSect, "Show_Range", szStr, g_szIniFile);
	sprintf(szStr, "%d", g_Configs.bShowSkinModel ? 1 : 0);
	WritePrivateProfileString(szSect, "Show_SkinModel", szStr, g_szIniFile);
	sprintf(szStr, "%d", g_Configs.bShowSpecially ? 1 : 0);
	WritePrivateProfileString(szSect, "Show_Specially", szStr, g_szIniFile);
	sprintf(szStr, "%d", g_Configs.bShowStaticModel ? 1 : 0);
	WritePrivateProfileString(szSect, "Show_StaticModel", szStr, g_szIniFile);
	sprintf(szStr, "%d", g_Configs.bShowTerrain ? 1 : 0);
	WritePrivateProfileString(szSect, "Show_Terrain", szStr, g_szIniFile);
	sprintf(szStr, "%d", g_Configs.bShowTree ? 1 : 0);
	WritePrivateProfileString(szSect, "Show_Tree", szStr, g_szIniFile);
	sprintf(szStr, "%d", g_Configs.bShowWater ? 1 : 0);
	WritePrivateProfileString(szSect, "Show_Water", szStr, g_szIniFile);
	sprintf(szStr, "%d", g_Configs.bShowTerrainBlock ? 1 : 0);
	WritePrivateProfileString(szSect, "Show_TerrainBlock", szStr, g_szIniFile);
	sprintf(szStr, "%d", g_Configs.bShowCloudBox ? 1 : 0);
	WritePrivateProfileString(szSect, "Show_CloudBox", szStr, g_szIniFile);
	sprintf(szStr, "%d", g_Configs.bShowInstanceBox ? 1 : 0);
	WritePrivateProfileString(szSect, "Show_InstanceBox", szStr, g_szIniFile);
	sprintf(szStr, "%d", g_Configs.bShowDynamic ? 1 : 0);
	WritePrivateProfileString(szSect, "Show_Dynamic", szStr, g_szIniFile);

	sprintf(szStr, "%d", g_Configs.bShowBufRegion ? 1 : 0);
	WritePrivateProfileString(szSect, "Show_BufRegion", szStr, g_szIniFile);
	sprintf(szStr, "%d", g_Configs.dwShowCopys);
	WritePrivateProfileString(szSect, "Show_Copy_Num", szStr, g_szIniFile);
	sprintf(szStr, "%d", g_Configs.bShowMotionPath ? 1 : 0);
	WritePrivateProfileString(szSect, "Show_MotionPath", szStr, g_szIniFile);

	sprintf(szStr, "%d", g_Configs.bObjectAddContinue ? 1 : 0);
	WritePrivateProfileString(szSect, "Add_ObjectContinuum", szStr, g_szIniFile);
	
	sprintf(szStr, "%d", g_Configs.bEnableFog ? 1 : 0);
	WritePrivateProfileString(szSect, "Enable_Fog", szStr, g_szIniFile);

	sprintf(szStr, "%d", g_Configs.nShowCopyNum);
	WritePrivateProfileString(szSect, "Show_Copy_Num", szStr, g_szIniFile);

	sprintf(szStr, "%d", g_Configs.bEnableGlbFog ? 1 : 0);
	WritePrivateProfileString(szSect, "Enable_Vol_Fog", szStr, g_szIniFile);

	//	Export section
	strcpy(szSect, "Export");

	sprintf(szStr, "%d", g_Configs.iTExpMaskTile);
	WritePrivateProfileString(szSect, "TExp_Mask_Tile", szStr, g_szIniFile);

	sprintf(szStr, "%d", g_Configs.iTExpColNum);
	WritePrivateProfileString(szSect, "TExp_Col_Num", szStr, g_szIniFile);

	sprintf(szStr, "%d", g_Configs.iTExpMaxLayer);
	WritePrivateProfileString(szSect, "TExp_Max_Layer", szStr, g_szIniFile);

	sprintf(szStr, "%d", g_Configs.iTExpBlkGrid);
	WritePrivateProfileString(szSect, "TExp_Blk_Grid", szStr, g_szIniFile);

	//	Brush options
	strcpy(szSect, "Brush");

	sprintf(szStr, "%d", g_Configs.iBrushRadius);
	WritePrivateProfileString(szSect, "Brush_Radius", szStr, g_szIniFile);

	sprintf(szStr, "%d", g_Configs.bCircleBrush ? 1 : 0);
	WritePrivateProfileString(szSect, "Brush_Circle", szStr, g_szIniFile);

	sprintf(szStr, "%f", g_Configs.fCurveControl1);
	WritePrivateProfileString(szSect, "Curve_Control1", szStr, g_szIniFile);
	sprintf(szStr, "%f", g_Configs.fCurveControl2);
	WritePrivateProfileString(szSect, "Curve_Control2", szStr, g_szIniFile);
	sprintf(szStr, "%f", g_Configs.fCurveControl3);
	WritePrivateProfileString(szSect, "Curve_Control3", szStr, g_szIniFile);
	sprintf(szStr, "%f", g_Configs.fCurveControl4);
	WritePrivateProfileString(szSect, "Curve_Control4", szStr, g_szIniFile);
	sprintf(szStr, "%f", g_Configs.fCurveControl5);
	WritePrivateProfileString(szSect, "Curve_Control5", szStr, g_szIniFile);
	sprintf(szStr, "%f", g_Configs.fCurveControl6);
	WritePrivateProfileString(szSect, "Curve_Control6", szStr, g_szIniFile);
	sprintf(szStr, "%f", g_Configs.fCurveControl7);
	WritePrivateProfileString(szSect, "Curve_Control7", szStr, g_szIniFile);
	 
	//	Lighting settings
	strcpy(szSect, "Lighting");

	sprintf(szStr, "%d", g_Configs.iLightMapSize);
	WritePrivateProfileString(szSect, "LM_Size", szStr, g_szIniFile);

	sprintf(szStr, "%d", g_Configs.iSunDir);
	WritePrivateProfileString(szSect, "Sun_Dir", szStr, g_szIniFile);

	sprintf(szStr, "%d", g_Configs.iSunHeight);
	WritePrivateProfileString(szSect, "Sun_Height", szStr, g_szIniFile);

	sprintf(szStr, "%u", g_Configs.dwSunCol);
	WritePrivateProfileString(szSect, "Sum_Color", szStr, g_szIniFile);

	sprintf(szStr, "%u", g_Configs.dwAmbient);
	WritePrivateProfileString(szSect, "Ambient", szStr, g_szIniFile);

	sprintf(szStr, "%f", g_Configs.fSunPower);
	WritePrivateProfileString(szSect, "Sun_Power", szStr, g_szIniFile);

	// Virtual source safe case
	//strcpy(szSect, "Vss");
	//WritePrivateProfileString(szSect, "Vss_UserName", g_Configs.szUserName, g_szIniFile);
	//WritePrivateProfileString(szSect, "Vss_UserPassword", g_Configs.szUserPassword, g_szIniFile);
	//WritePrivateProfileString(szSect, "Vss_ServerPath", g_Configs.szServerPath, g_szIniFile);

	// Project config
	strcpy(szSect, "Project");
	WritePrivateProfileString(szSect, "Project_Cur", g_Configs.szCurProjectName, g_szIniFile);
	WritePrivateProfileString(szSect, "Project_Exp", g_Configs.szExpPath, g_szIniFile);

	sprintf(szStr, "%d", g_Configs.iMusicVolume);
	WritePrivateProfileString(szSect, "Project_music_volume", szStr, g_szIniFile);
	sprintf(szStr, "%d", g_Configs.iSfxVolume);
	WritePrivateProfileString(szSect, "Project_sfx_volume", szStr, g_szIniFile);

	// Version config
	strcpy(szSect, "Version");
	sprintf(szStr, "%d", g_Configs.iIsOverseaVersion);
	WritePrivateProfileString(szSect, "IsOverseaVersion", szStr, g_szIniFile);
}

//	Load configs from .ini file
void AUX_LoadConfigs()
{
	//	Load configs from .ini file
	char szStr[256], szSect[128];

	//	General section
	strcpy(szSect, "General");

	sprintf(szStr, "%f", g_Configs.fCameraMove);
	GetPrivateProfileString(szSect, "Camera_Move", szStr, szStr, 256, g_szIniFile);
	g_Configs.fCameraMove = (float)atof(szStr);

	sprintf(szStr, "%f", g_Configs.fCameraRot);
	GetPrivateProfileString(szSect, "Camera_Rot", szStr, szStr, 256, g_szIniFile);
	g_Configs.fCameraRot = (float)atof(szStr);

	g_Configs.bAlongGround = GetPrivateProfileInt(szSect, "Along_Ground", g_Configs.bAlongGround ? 1 : 0, g_szIniFile) ? true : false;
	g_Configs.bEnableSpeak = GetPrivateProfileInt(szSect, "Speak_Enable", g_Configs.bEnableSpeak ? 1 : 0, g_szIniFile) ? true : false;	
	g_Configs.bShowModelHull = GetPrivateProfileInt(szSect, "Show_Hull", g_Configs.bShowModelHull ? 1 : 0, g_szIniFile) ? true : false;
	g_Configs.bShowBoundBox = GetPrivateProfileInt(szSect, "Show_BoundBox", g_Configs.bShowBoundBox ? 1 : 0, g_szIniFile) ? true : false;
	g_Configs.bHostMode = GetPrivateProfileInt(szSect, "Host_Mode", g_Configs.bHostMode ? 1 : 0, g_szIniFile) ? true : false;
	g_Configs.bNeighborTerrainOnly = GetPrivateProfileInt(szSect, "Neighbor_Terrain_Only", g_Configs.bNeighborTerrainOnly ? 1 : 0, g_szIniFile) ? true : false;

	//	New map settings
	sprintf(szStr, "%f", g_Configs.fTileSize);
	GetPrivateProfileString(szSect, "Tile_Size", szStr, szStr, 256, g_szIniFile);
	g_Configs.fTileSize = (float)atof(szStr);

	sprintf(szStr, "%f", g_Configs.fMinHeight);
	GetPrivateProfileString(szSect, "Min_Height", szStr, szStr, 256, g_szIniFile);
	g_Configs.fMinHeight = (float)atof(szStr);

	sprintf(szStr, "%f", g_Configs.fMaxHeight);
	GetPrivateProfileString(szSect, "Max_Height", szStr, szStr, 256, g_szIniFile);
	g_Configs.fMaxHeight = (float)atof(szStr);

	//Pick object section
	strcpy(szSect, "Pick");
	g_Configs.bPickStaticModel = GetPrivateProfileInt(szSect, "Pick_StaticModel", g_Configs.bPickStaticModel ? 1 : 0, g_szIniFile) ? true : false;
	g_Configs.bPickLight = GetPrivateProfileInt(szSect, "Pick_Light", g_Configs.bPickLight ? 1 : 0, g_szIniFile) ? true : false;
	g_Configs.bPickAudio = GetPrivateProfileInt(szSect, "Pick_Audio", g_Configs.bPickAudio ? 1 : 0, g_szIniFile) ? true : false;
	g_Configs.bPickBoxArea = GetPrivateProfileInt(szSect, "Pick_BoxArea", g_Configs.bPickBoxArea ? 1 : 0, g_szIniFile) ? true : false;
	g_Configs.bPickDummy = GetPrivateProfileInt(szSect, "Pick_Dummy", g_Configs.bPickDummy ? 1 : 0, g_szIniFile) ? true : false;
	g_Configs.bPickSpecially = GetPrivateProfileInt(szSect, "Pick_Specially", g_Configs.bPickSpecially ? 1 : 0, g_szIniFile) ? true : false;
	g_Configs.bPickWater = GetPrivateProfileInt(szSect, "Pick_Water", g_Configs.bPickWater ? 1 : 0, g_szIniFile) ? true : false;
	g_Configs.bPickAIArea = GetPrivateProfileInt(szSect, "Pick_AIArea", g_Configs.bPickAIArea ? 1 : 0, g_szIniFile) ? true : false;
	g_Configs.bPickPath = GetPrivateProfileInt(szSect, "Pick_Path", g_Configs.bPickPath ? 1 : 0, g_szIniFile) ? true : false;
	g_Configs.bPickSkinModel = GetPrivateProfileInt(szSect, "Pick_SkinModel", g_Configs.bPickSkinModel ? 1 : 0, g_szIniFile) ? true : false;
	g_Configs.bPickCritterGroup = GetPrivateProfileInt(szSect, "Pick_CritterGroup", g_Configs.bPickCritterGroup ? 1 : 0, g_szIniFile) ? true : false;
	g_Configs.bPickFixedNpc = GetPrivateProfileInt(szSect, "Pick_FixedNpc", g_Configs.bPickFixedNpc ? 1 : 0, g_szIniFile) ? true : false;
	g_Configs.bPickPrecinct = GetPrivateProfileInt(szSect, "Pick_Precinct", g_Configs.bPickPrecinct ? 1 : 0, g_szIniFile) ? true : false;
	g_Configs.bPickGather = GetPrivateProfileInt(szSect, "Pick_Gather", g_Configs.bPickGather ? 1 : 0, g_szIniFile) ? true : false;
	g_Configs.bPickRange = GetPrivateProfileInt(szSect, "Pick_Range", g_Configs.bPickRange ? 1 : 0, g_szIniFile) ? true : false;
	g_Configs.bPickCloudBox = GetPrivateProfileInt(szSect, "Pick_CloudBox", g_Configs.bPickCloudBox ? 1 : 0, g_szIniFile) ? true : false;
	g_Configs.bPickInstanceBox = GetPrivateProfileInt(szSect, "Pick_InstanceBox", g_Configs.bPickInstanceBox ? 1 : 0, g_szIniFile) ? true : false;
	g_Configs.bPickDynamic = GetPrivateProfileInt(szSect, "Pick_Dynamic", g_Configs.bPickDynamic ? 1 : 0, g_szIniFile) ? true : false;
	g_Configs.bPickBuffRegion = GetPrivateProfileInt(szSect, "Pick_BuffRegion", g_Configs.bPickBuffRegion ? 1 : 0, g_szIniFile) ? true : false;
	g_Configs.bPickGuildBuilding = GetPrivateProfileInt(szSect, "Pick_GuildBuilding", g_Configs.bPickGuildBuilding ? 1 : 0, g_szIniFile) ? true : false;
	//	Render section
	strcpy(szSect, "Render");
	
	sprintf(szStr, "%f", g_Configs.fViewRadius);
	GetPrivateProfileString(szSect, "Render_Radius", szStr, szStr, 256, g_szIniFile);
	g_Configs.fViewRadius = (float)atof(szStr);
	g_Configs.bCircleBrush = GetPrivateProfileInt(szSect, "Imme_Mask_Paint", g_Configs.bImmeMaskPaint ? 1 : 0, g_szIniFile) ? true : false;
	g_Configs.bEnableSky = GetPrivateProfileInt(szSect, "Enable_Sky", g_Configs.bEnableSky ? 1 : 0, g_szIniFile) ? true : false;
	g_Configs.bShowObjectName = GetPrivateProfileInt(szSect, "Show_ObjectName", g_Configs.bShowObjectName ? 1 : 0, g_szIniFile) ? true : false;
	g_Configs.bObjectAddContinue = GetPrivateProfileInt(szSect, "Add_ObjectContinuum", g_Configs.bObjectAddContinue ? 1 : 0, g_szIniFile) ? true : false;
	g_Configs.bEnableFog = GetPrivateProfileInt(szSect, "Enable_Fog", g_Configs.bEnableFog ? 1 : 0, g_szIniFile) ? true : false;
	g_Configs.bEnableGlbFog = GetPrivateProfileInt(szSect, "Enable_Vol_Fog", g_Configs.bEnableGlbFog ? 1 : 0, g_szIniFile) ? true : false;

#ifdef _ANGELICA22_FEATURE
	g_Configs.nWaterEffect = GetPrivateProfileInt(szSect, "Water_Effect", g_Configs.nWaterEffect, g_szIniFile);
	g_Configs.bHDRLitmap = GetPrivateProfileInt(szSect, "Show_HDRLitmap", g_Configs.bHDRLitmap ? 1 : 0, g_szIniFile) ? true : false;
#else
	g_Configs.bSampleWater = GetPrivateProfileInt(szSect, "Show_SampleWater", g_Configs.bSampleWater ? 1 : 0, g_szIniFile) ? true : false;
#endif
	g_Configs.bShowAIArea = GetPrivateProfileInt(szSect, "Show_AIArea", g_Configs.bShowAIArea ? 1 : 0, g_szIniFile) ? true : false;
	g_Configs.bShowAudio = GetPrivateProfileInt(szSect, "Show_Audio", g_Configs.bShowAudio ? 1 : 0, g_szIniFile) ? true : false;
	g_Configs.bShowBoxArea = GetPrivateProfileInt(szSect, "Show_BoxArea", g_Configs.bShowBoxArea ? 1 : 0, g_szIniFile) ? true : false;
	g_Configs.bShowCritterGroup = GetPrivateProfileInt(szSect, "Show_CritterGroup", g_Configs.bShowCritterGroup ? 1 : 0, g_szIniFile) ? true : false;
	g_Configs.bShowDummy = GetPrivateProfileInt(szSect, "Show_Dummy", g_Configs.bShowDummy ? 1 : 0, g_szIniFile) ? true : false;
	g_Configs.bShowFixedNpc = GetPrivateProfileInt(szSect, "Show_FixedNpc", g_Configs.bShowFixedNpc ? 1 : 0, g_szIniFile) ? true : false;
	g_Configs.bShowFixedNpc = GetPrivateProfileInt(szSect, "Show_FixedNpcWire", g_Configs.bShowFixedNpcWire ? 1 : 0, g_szIniFile) ? true : false;
	g_Configs.bShowGather = GetPrivateProfileInt(szSect, "Show_Gather", g_Configs.bShowGather ? 1 : 0, g_szIniFile) ? true : false;
	g_Configs.bShowGrass = GetPrivateProfileInt(szSect, "Show_Grass", g_Configs.bShowGrass ? 1 : 0, g_szIniFile) ? true : false;
	g_Configs.bShowLight = GetPrivateProfileInt(szSect, "Show_Light", g_Configs.bShowLight ? 1 : 0, g_szIniFile) ? true : false;
	g_Configs.bShowPath = GetPrivateProfileInt(szSect, "Show_Path", g_Configs.bShowPath ? 1 : 0, g_szIniFile) ? true : false;
	g_Configs.bShowRange = GetPrivateProfileInt(szSect, "Show_Range", g_Configs.bShowRange ? 1 : 0, g_szIniFile) ? true : false;
	g_Configs.bShowSkinModel = GetPrivateProfileInt(szSect, "Show_SkinModel", g_Configs.bShowSkinModel ? 1 : 0, g_szIniFile) ? true : false;
	g_Configs.bShowSpecially = GetPrivateProfileInt(szSect, "Show_Specially", g_Configs.bShowSpecially ? 1 : 0, g_szIniFile) ? true : false;
	g_Configs.bShowStaticModel = GetPrivateProfileInt(szSect, "Show_StaticModel", g_Configs.bShowStaticModel ? 1 : 0, g_szIniFile) ? true : false;
	g_Configs.bShowTerrain = GetPrivateProfileInt(szSect, "Show_Terrain", g_Configs.bShowTerrain ? 1 : 0, g_szIniFile) ? true : false;
	g_Configs.bShowTree = GetPrivateProfileInt(szSect, "Show_Tree", g_Configs.bShowTree ? 1 : 0, g_szIniFile) ? true : false;
	g_Configs.bShowWater = GetPrivateProfileInt(szSect, "Show_Water", g_Configs.bShowWater ? 1 : 0, g_szIniFile) ? true : false;
	g_Configs.bShowTerrainBlock = GetPrivateProfileInt(szSect, "Show_TerrainBlock", g_Configs.bShowTerrainBlock ? 1 : 0, g_szIniFile) ? true : false;
	g_Configs.bShowCloudBox = GetPrivateProfileInt(szSect, "Show_CloudBox", g_Configs.bShowCloudBox ? 1 : 0, g_szIniFile) ? true : false;
	g_Configs.bShowInstanceBox = GetPrivateProfileInt(szSect, "Show_InstanceBox", g_Configs.bShowInstanceBox ? 1 : 0, g_szIniFile) ? true : false;
	g_Configs.nShowCopyNum = GetPrivateProfileInt(szSect, "Show_Copy_Num", g_Configs.nShowCopyNum, g_szIniFile);
	g_Configs.bShowDynamic = GetPrivateProfileInt(szSect, "Show_Dynamic", g_Configs.bShowDynamic ? 1 : 0, g_szIniFile) ? true : false;
	g_Configs.bShowPostEffect = GetPrivateProfileInt(szSect, "Show_Post_Effect", g_Configs.bShowPostEffect ? 1 : 0, g_szIniFile) ? true : false;
	g_Configs.bShowSunMoon = GetPrivateProfileInt(szSect, "Show_Sun_Moon", g_Configs.bShowSunMoon ? 1 : 0, g_szIniFile) ? true : false;

	g_Configs.dwShowCopys = GetPrivateProfileInt(szSect, "Show_Copy_Num", g_Configs.dwShowCopys, g_szIniFile);
	g_Configs.bShowBufRegion = GetPrivateProfileInt(szSect, "Show_BufRegion", g_Configs.bShowBufRegion ? 1 : 0, g_szIniFile) ? true : false;
	g_Configs.bShowMotionPath = GetPrivateProfileInt(szSect, "Show_MotionPath", g_Configs.bShowMotionPath ? 1 : 0, g_szIniFile) ? true : false;

	sprintf(szStr, "%d", g_Configs.bShowPostEffect ? 1 : 0);
	WritePrivateProfileString(szSect, "Show_Post_Effect", szStr, g_szIniFile);

	sprintf(szStr, "%d", g_Configs.bShowSunMoon ? 1 : 0);
	WritePrivateProfileString(szSect, "Show_Sun_Moon", szStr, g_szIniFile);

	//	Export section
	strcpy(szSect, "Export");

	g_Configs.iTExpMaskTile = GetPrivateProfileInt(szSect, "TExp_Mask_Tile", g_Configs.iTExpMaskTile, g_szIniFile);
	g_Configs.iTExpColNum = GetPrivateProfileInt(szSect, "TExp_Col_Num", g_Configs.iTExpColNum, g_szIniFile);
	g_Configs.iTExpMaxLayer = GetPrivateProfileInt(szSect, "TExp_Max_Layer", g_Configs.iTExpMaxLayer, g_szIniFile);
	g_Configs.iTExpBlkGrid = GetPrivateProfileInt(szSect, "TExp_Blk_Grid", g_Configs.iTExpBlkGrid, g_szIniFile);

	//	Brush options
	strcpy(szSect, "Brush");

	g_Configs.iBrushRadius = GetPrivateProfileInt(szSect, "Brush_Radius", g_Configs.iBrushRadius, g_szIniFile);
	g_Configs.bCircleBrush = GetPrivateProfileInt(szSect, "Brush_Circle", g_Configs.bCircleBrush ? 1 : 0, g_szIniFile) ? true : false;
	
	GetPrivateProfileString(szSect, "Curve_Control1", szStr, szStr, 256, g_szIniFile);
	g_Configs.fCurveControl1 = (float)atof(szStr);

	GetPrivateProfileString(szSect, "Curve_Control2", szStr, szStr, 256, g_szIniFile);
	g_Configs.fCurveControl2 = (float)atof(szStr);

	GetPrivateProfileString(szSect, "Curve_Control3", szStr, szStr, 256, g_szIniFile);
	g_Configs.fCurveControl3 = (float)atof(szStr);

	GetPrivateProfileString(szSect, "Curve_Control4", szStr, szStr, 256, g_szIniFile);
	g_Configs.fCurveControl4 = (float)atof(szStr);

	GetPrivateProfileString(szSect, "Curve_Control5", szStr, szStr, 256, g_szIniFile);
	g_Configs.fCurveControl5 = (float)atof(szStr);

	GetPrivateProfileString(szSect, "Curve_Control6", szStr, szStr, 256, g_szIniFile);
	g_Configs.fCurveControl6 = (float)atof(szStr);

	GetPrivateProfileString(szSect, "Curve_Control7", szStr, szStr, 256, g_szIniFile);
	g_Configs.fCurveControl7 = (float)atof(szStr);
	
	//	Lighting settings
	strcpy(szSect, "Lighting");

	g_Configs.iLightMapSize = GetPrivateProfileInt(szSect, "LM_Size", g_Configs.iLightMapSize, g_szIniFile);
	g_Configs.iSunDir = GetPrivateProfileInt(szSect, "Sun_Dir", g_Configs.iSunDir, g_szIniFile);
	g_Configs.iSunHeight = GetPrivateProfileInt(szSect, "Sun_Height", g_Configs.iSunHeight, g_szIniFile);
	g_Configs.dwSunCol = (DWORD)GetPrivateProfileInt(szSect, "Sum_Color", (int)g_Configs.dwSunCol, g_szIniFile);
	g_Configs.dwAmbient = (DWORD)GetPrivateProfileInt(szSect, "Ambient", (int)g_Configs.dwAmbient, g_szIniFile);
	GetPrivateProfileString(szSect, "Sun_Power", szStr, szStr, 256, g_szIniFile);
	g_Configs.fSunPower = (float)atof(szStr);

	// Virtual source safe case
	//strcpy(szSect, "Vss");
	
	//GetPrivateProfileString(szSect, "Vss_UserName", g_Configs.szUserName,g_Configs.szUserName,64, g_szIniFile);
	//GetPrivateProfileString(szSect, "Vss_UserPassword", g_Configs.szUserPassword, g_Configs.szUserPassword,64,g_szIniFile);
	//GetPrivateProfileString(szSect, "Vss_ServerPath", g_Configs.szServerPath, g_Configs.szServerPath,128,g_szIniFile);
	
	// Project config
	strcpy(szSect, "Project");
	GetPrivateProfileString(szSect, "Project_Cur", g_Configs.szCurProjectName, g_Configs.szCurProjectName,128,g_szIniFile);
	GetPrivateProfileString(szSect, "Project_Exp", g_Configs.szExpPath, g_Configs.szExpPath,128,g_szIniFile);

	// sound volume
	GetPrivateProfileString(szSect, "Project_music_volume", szStr, szStr, 256, g_szIniFile);
	g_Configs.iMusicVolume = (float)atof(szStr);
	GetPrivateProfileString(szSect, "Project_sfx_volume", szStr, szStr, 256, g_szIniFile);
	g_Configs.iSfxVolume = (float)atof(szStr);

	// Version config
	strcpy(szSect, "Version");
	g_Configs.iIsOverseaVersion = GetPrivateProfileInt(szSect, "IsOverseaVersion", g_Configs.iIsOverseaVersion, g_szIniFile);
}

//	Change a 3D edit box to flat style
void AUX_FlatEditBox(CEdit* pEdit)
{
	//	Remove client edge at first
	DWORD dwStyles = ::GetWindowLong(pEdit->m_hWnd, GWL_EXSTYLE);
	dwStyles &= ~WS_EX_CLIENTEDGE;
	::SetWindowLong(pEdit->m_hWnd, GWL_EXSTYLE, dwStyles);

	//	Add border style
	dwStyles = ::GetWindowLong(pEdit->m_hWnd, GWL_STYLE);
	dwStyles |= WS_BORDER;
	::SetWindowLong(pEdit->m_hWnd, GWL_STYLE, dwStyles);
}

//	Chose a proper texture size
int AUX_ChoseTextureSize(float fApproSize)
{
	int iTexSize = 1;
	int iMax = (int)(ceil(fApproSize) + 0.5f);

	while (iTexSize < iMax)
	{
		iTexSize <<= 1;
	}

	return iTexSize;
}

//	Check whether specified exists
bool AUX_FileExist(const char* szFile)
{
	if (!szFile || !szFile[0])
		return false;

	FILE* fp = fopen(szFile, "rb");
	if (!fp)
		return false;

	fclose(fp);
	return true;
}

//	Change file's extension
void AUX_ChangeExtension(char* szFile, char* szNewExt)
{
	char* pTemp = strrchr(szFile, '.');
	if (!pTemp)
	{
		strcat(szFile, ".");
		strcat(szFile, szNewExt);
	}
	else
	{
		strcpy(pTemp+1, szNewExt);
	}
}

//	Write string to file
bool AUX_WriteString(FILE* fp, const char* str)
{
	ASSERT(str);

	//	Write length of string
	int iLen = strlen(str);
	fwrite(&iLen, 1, sizeof (int), fp);

	//	Write string data
	if (iLen)
		fwrite(str, 1, iLen, fp);

	return true;
}

#include <winuser.h>

//	Create project directory of specified name
bool AUX_CreateProjectDir(HWND hWnd, const CString& strProjName)
{
	if (strProjName == "")
	{
		MessageBox(hWnd, "项目名称中不能为空", "错误", MB_OK);
		return false;
	}

	char szChars[] = {'\\', '/', ':', '*', '?', '"', '<', '>', '|', '\0'};
	if (strProjName.FindOneOf(szChars) >= 0)
	{
		MessageBox(hWnd, "项目名称中不能包含字符: \\ / : * ? \" < > |", "错误", MB_OK);
		return false;
	}

	//	Check whether there is a same name project exists
	char szDir[MAX_PATH];
	sprintf(szDir, "%s%s\\%s", g_szWorkDir, g_szEditMapDir, strProjName);
	
	char szCurDir[MAX_PATH];
	GetCurrentDirectory(MAX_PATH, szCurDir);
	if (SetCurrentDirectory(szDir))
	{
		SetCurrentDirectory(szCurDir);
		MessageBox(hWnd, "同名项目已经存在", "错误", MB_OK);
		return false;
	}

	SetCurrentDirectory(szCurDir);

	//	Create project directory
	sprintf(szDir, "%s%s\\%s", g_szWorkDir, g_szEditMapDir, strProjName);
	AUX_CreateDirectory(szDir);

	return true;
}

//	Create brush
HBRUSH AUX_CreateBrush(UINT lbStyle, COLORREF lbColor, LONG lbHatch)
{
	LOGBRUSH logBrush = {lbStyle, lbColor, lbHatch};
	return ::CreateBrushIndirect(&logBrush);
}

//	Create a directory
bool AUX_CreateDirectory(const char* szDir)
{
	CString strDir = szDir;
	int iLen = strDir.GetLength();

	if (iLen <= 3)
		return true;

	//	Clear the last '\\'
	if (strDir.GetAt(iLen-1) == '\\')
		strDir.SetAt(iLen-1, '\0');

	//	Save current directory
	char szCurDir[MAX_PATH];
	GetCurrentDirectory(MAX_PATH, szCurDir);

	CString strTemp;

	int iPos = strDir.Find('\\', 0);
	while (iPos > 0)
	{
		strTemp = strDir.Mid(0, iPos);
		CreateDirectory(strTemp, NULL);
		iPos = strDir.Find('\\', iPos+1);
	}

	CreateDirectory(szDir, NULL);

	//	Restore current directory
	SetCurrentDirectory(szCurDir);

	return true;
}

//	Set global log output and progress bar function
void AUX_SetProgressFuncs(LPFNLOGOUTPUT lpfnLog, LPFNPROGRESS lpfnProgress, DWORD dwUserData)
{
	l_dwProgressData	= dwUserData;
	l_lpfnLogOutput		= lpfnLog;
	l_lpfnProgress		= lpfnProgress;
}

//	Output progress log
void AUX_ProgressLog(const char* szMsg, ...)
{
	if (!l_lpfnLogOutput)
		return;

	char szBuf[1024];

	va_list vaList;
	va_start(vaList, szMsg);
	vsprintf(szBuf, szMsg, vaList);
	va_end(vaList);

	l_lpfnLogOutput(l_dwProgressData, szBuf);
}

//	Reset progress bar
void AUX_ResetProgressBar(int iTotal)
{
	if (l_lpfnProgress)
		l_lpfnProgress(l_dwProgressData, -iTotal, 0);
}

//	Step progress bar
void AUX_StepProgressBar(int iStep)
{
	if (l_lpfnProgress)
		l_lpfnProgress(l_dwProgressData, 0, iStep);
}

void AUX_LoadCursor()
{
	g_hSelectCursor = LoadCursor(AfxGetInstanceHandle(),MAKEINTRESOURCE(IDC_CURSOR_CROSS));
	g_hMoveCursor = LoadCursor(AfxGetInstanceHandle(),MAKEINTRESOURCE(IDC_CURSOR_OBJECT_MOVE));
	g_hScaleCursor = LoadCursor(AfxGetInstanceHandle(),MAKEINTRESOURCE(IDC_CURSOR_SCALE));
    g_hRotateCursor = LoadCursor(AfxGetInstanceHandle(),MAKEINTRESOURCE(IDC_CURSOR_ROTATE));;
    g_hAddObjectCursor = LoadCursor(AfxGetInstanceHandle(),MAKEINTRESOURCE(IDC_CURSOR_OBJECTADD));
	
	CSceneObjectMoveOperation*pMove =  (CSceneObjectMoveOperation*)g_OperationContainer.GetOperation(OPERATION_SCENE_OBJECT_MOVE);
	pMove->SetOperationCursor(g_hMoveCursor);
	CSceneObjectRotateOperation*pRotate =  (CSceneObjectRotateOperation*)g_OperationContainer.GetOperation(OPERATION_SCENE_OBJECT_ROTATE);
	pRotate->SetOperationCursor(g_hRotateCursor);
	SceneObjectScaleOperation*pScale =  (SceneObjectScaleOperation*)g_OperationContainer.GetOperation(OPERATION_SCENE_OBJECT_SCALE);
	pScale->SetOperationCursor(g_hScaleCursor);	
}

void AUX_LoadTemplateData()
{
	if(-1==g_dataMan.load_data("Data\\elements.data"))
	{
		AfxMessageBox("AUX_LoadTemplateData(),Load Data\\elements.data failed!");
		g_Log.Log("AUX_LoadTemplateData(),Load Data\\elements.data failed!");
	}
}

void AUX_LoadDataPath()
{
	if(0==g_dataPathMan.Load("Data\\path.data"))
	{
		AfxMessageBox("AUX_LoadDataPath(),Load Data\\path.data failed!");
		g_Log.Log("AUX_LoadDataPath(),Load Data\\path.data failed!");
	}
}

bool AUX_FileIsReadOnly(CString szFilePathName)
{
	DWORD flag = GetFileAttributes(szFilePathName);
	if(flag==-1)
	{
		return true;
	}
	if (!(flag & FILE_ATTRIBUTE_READONLY)) 
	{
		return false;
	}
	return true;
}

bool AUX_FileIsExist(CString szFilePathName)
{
	FILE *file;
	file = fopen(szFilePathName,"r");
	if(file) 
	{
		fclose(file);
		return true;
	}
	else return false;
}

bool AUX_CheckOutWorkspace()
{
	g_VSS.SetProjectPath("EditMaps");
	g_VSS.CheckOutFile("EditMaps\\workspace.ws");
	return !AUX_FileIsReadOnly("EditMaps\\workspace.ws");
}

bool AUX_CheckInWorkspace()
{
	g_VSS.SetProjectPath("EditMaps");
	g_VSS.CheckInFile("workspace.ws");
	return AUX_FileIsReadOnly("EditMaps\\workspace.ws");
}

bool AUX_CheckOutProjList()
{
	CString str;
	str.Format("%s",g_szEditMapDir);
	g_VSS.SetProjectPath(str);
	str.Format("%s\\projlist.dat",g_szEditMapDir);
	g_VSS.CheckOutFile(str);
	return !AUX_FileIsReadOnly(str);
}

bool AUX_CheckInProjList()
{
	CString str;
	str.Format("%s",g_szEditMapDir);
	g_VSS.SetProjectPath(str);
	g_VSS.CheckInFile("projlist.dat");
	str.Format("%s\\projlist.dat",g_szEditMapDir);
	return AUX_FileIsReadOnly(str);
}

bool AUX_CheckOutResTable()
{
	g_VSS.SetProjectPath("ResTable");
	g_VSS.CheckOutFile("ResTable\\ElementRes.table");
	return !AUX_FileIsReadOnly("ResTable\\ElementRes.table");
}

bool AUX_UndoCheckOutResTable()
{
	g_VSS.SetProjectPath("ResTable");
	g_VSS.UndoCheckOut("ResTable\\ElementRes.table");
	return AUX_FileIsReadOnly("ResTable\\ElementRes.table");
}

bool AUX_UndoCheckOutWorkspace()
{
	g_VSS.SetProjectPath("EditMaps");
	g_VSS.UndoCheckOut("EditMaps\\workspace.ws");
	return AUX_FileIsReadOnly("EditMaps\\workspace.ws");
}

bool AUX_CheckInResTable()
{
	g_VSS.SetProjectPath("ResTable");
	g_VSS.CheckInFile("ElementRes.table");
	return AUX_FileIsReadOnly("ResTable\\ElementRes.table");
}

bool Aux_IsAllFilesCheckOut(char *szMap)
{
	CString str,str1;
	for( int i = 0; i < sizeof(sz_map_file_ex)/sizeof(sz_map_file_ex[0]); ++i)
	{
		str1.Format("%s\\%s",g_szEditMapDir,szMap);
		str.Format(sz_map_file_ex[i],szMap);
		str = str1 + "\\" + str;
		if(!g_VSS.IsCheckOutStatus(str)) return false;
	}
	str.Format("%s\\%s\\MaskMaps.data",g_szEditMapDir,szMap);
	if(!g_VSS.IsCheckOutStatus(str)) return false;
	
	return true;
}

bool Aux_IsAllFilesCheckIn(char *szMap)
{
	CString str,str1;
	for( int i = 0; i < sizeof(sz_map_file_ex)/sizeof(sz_map_file_ex[0]); ++i)
	{
		str1.Format("%s\\%s",g_szEditMapDir,szMap);
		str.Format(sz_map_file_ex[i],szMap);
		str = str1 + "\\" + str;
		if(g_VSS.IsCheckOutStatus(str)==1) return false;
	}
	str.Format("%s\\%s\\MaskMaps.data",g_szEditMapDir,szMap);
	if(g_VSS.IsCheckOutStatus(str)) return false;
	
	return true;
}

bool AUX_CheckOutMap(char *szMap)
{
	CString str,str1;
	str.Format("%s\\%s",g_szEditMapDir,szMap);
	g_VSS.SetProjectPath(str);
	g_VSS.CheckOutAllFile(str);
	CString rc = g_VSS.GetLastCommandResult();
	if(!Aux_IsAllFilesCheckOut(szMap)) 
	{
		AfxMessageBox(rc);
		return false;
	}
	return true;
}

void AUX_UndoCheckOutMap(char *szMap)
{
	CString str1;
	str1.Format("%s\\%s",g_szEditMapDir,szMap);
	g_VSS.SetProjectPath(str1);
	g_VSS.UndoCheckOutFiles();
}

bool AUX_CheckInMap(char *szMap)
{
	CString str,str1;
	if(!Aux_IsAllFilesCheckOut(szMap)) return false;

	str.Format("%s\\%s",g_szEditMapDir,szMap);
	g_VSS.SetProjectPath(str);
	
	for(int i = 0; i < sizeof(sz_map_file_ex)/sizeof(sz_map_file_ex[0]); ++i)
	{
		str.Format(sz_map_file_ex[i],szMap);
		g_VSS.CheckInFile(str);
		str1.Format("%s\\%s",g_szEditMapDir,szMap);
		str = str1 + "\\" + str;	
		if(g_VSS.IsCheckOutStatus(str)==1) return false;
	}
	
	//MaskMaps.data
	str = "MaskMaps.data";
	g_VSS.CheckInFile(str);
	str.Format("%s\\%s\\MaskMaps.data",g_szEditMapDir,szMap);
	if(g_VSS.IsCheckOutStatus(str)) return false;
	
	return true;
}

bool AUX_CopyFolder(const char *szDst,const char *szSrc,bool bRender)
{
	CFileFind finder;
	CString fileName,ext;
	CString strWildcard;
	strWildcard += szSrc;
	strWildcard += "\\*.*";
	
	BOOL bWorking = finder.FindFile(strWildcard);
	int n = 0;
	while (bWorking)
	{
		bWorking = finder.FindNextFile();
		
		//是"."或是".." 跳过.
		if (finder.IsDots())
			continue;
		
		// 是文件夹，继续搜寻.
		if (finder.IsDirectory())
		{
			continue;
		}
		// 是文件.
		fileName = finder.GetFilePath();
		ext = finder.GetFileName();
		ext.MakeLower();
		if(bRender)
		{
			if(-1 == ext.Find("render")) continue;
		}
		CreateDirectory(szDst, NULL);
		if(!AUX_CopyFile(szDst,fileName))
		{
			CString msg;
			msg.Format("AUX_CopyFolder(), 拷贝文件失败!%s",fileName);
			AfxMessageBox(msg);
			g_Log.Log(msg);
			return false;
		}
	}
	finder.Close();
	return true;
}

bool AUX_CopyFolderRecur(const char *szDst,const char *szSrc)
{
	CFileFind finder;
	CString fileName,ext;
	CString strWildcard;
	strWildcard += szSrc;
	strWildcard += "\\*.*";
	
	CreateDirectory(szDst, NULL);
	BOOL bWorking = finder.FindFile(strWildcard);
	int n = 0;
	while (bWorking)
	{
		bWorking = finder.FindNextFile();
		
		//是"."或是".." 跳过.
		if (finder.IsDots())
			continue;
		
		// 是文件夹，继续搜寻.
		if (finder.IsDirectory())
		{
			CString name = finder.GetFilePath();
			name.Replace(szSrc,"");
			name.Delete(0,1);
			CString dstName = szDst;
			dstName += "\\";
			dstName += name;
			AUX_CopyFolderRecur(dstName, finder.GetFilePath());
			continue;
		}
		// 是文件.
		fileName = finder.GetFilePath();
		ext = finder.GetFileName();
		ext.MakeLower();
		if(!AUX_CopyFile(szDst,fileName))
		{
			CString msg;
			msg.Format("AUX_CopyFolder(), 拷贝文件失败!%s",fileName);
			AfxMessageBox(msg);
			g_Log.Log(msg);
			return false;
		}
	}
	finder.Close();
	return true;
}

bool AUX_CopyFile(const char* dst,const char* src)
{
	CFile sFile,dFile;
	CString strPath,strName;
	AString temp = src;
	
	int pos = temp.ReverseFind('\\');
	if(pos == -1)
	{
		pos = 0;
		strName = temp;
	}else
	{
		strPath = temp.Mid(0, pos);
		strName = temp.Mid(pos+1,temp.GetLength());
	}
	strPath = dst;
	strPath +="\\";
	strPath += strName;

	
	
	DWORD flag = GetFileAttributes(strPath);
	if (flag & FILE_ATTRIBUTE_READONLY) 
	{
		flag ^= FILE_ATTRIBUTE_READONLY;
		SetFileAttributes(strPath,flag);
	}

	
	if(!CopyFile(src,strPath,false))
	{
		g_Log.Log("AUX_CopyFile(),打开文件%s失败!",src);
		return false;
	}
	
	return true;
	/*
	if(!sFile.Open(src,CFile::modeRead | CFile::typeBinary | CFile::shareDenyWrite))
	{
		g_Log.Log("AUX_CopyFile(),打开文件%s失败!",src);
		return false;
	}
	if(!dFile.Open(strPath,CFile::modeCreate | CFile::modeWrite | CFile::typeBinary | CFile::shareDenyWrite))
	{
		sFile.Close();
		g_Log.Log("AUX_CopyFile(),创建文件%s失败!",strPath);
		return false;
	}
	DWORD dwFileLen = sFile.GetLength();
	DWORD dwLoaded = 0;
	if(dwFileLen > 0)
	{
		BYTE* pBuffer = new BYTE[dwFileLen];
		ASSERT(pBuffer);
		if(0==sFile.ReadHuge(pBuffer,dwFileLen))
		{
			delete []pBuffer;
			goto failed;
		}
		dFile.WriteHuge(pBuffer,dwFileLen);
		delete []pBuffer;
	}
	sFile.Close();
	dFile.Close();
	return true;
	*/
}

bool AUX_CheckOutAllMap()
{
	return true;
}

bool AUX_CheckInAllMap()
{
	return true;
}

bool AUX_ModifiedResTable()
{
	/*
	CMainFrame *pFrame = AUX_GetMainFrame();
	if(pFrame)
	{
		CElementMap *pMap = pFrame->GetMap();
		if(pMap)
		{
			ElementResMan mResMan;
			if(!AUX_CheckOutResTable())
			{
				AfxMessageBox("AUX_ModifiedResTable(),不能从服务器上CheckOut文件ElementRes.table!");
				return false;
			}
			if(!mResMan.Load())
			{
				if(!AUX_CheckInResTable())
				{
					AfxMessageBox("AUX_ModifiedResTable(),不能把文件ElementRes.table CheckIn到服务器!");
					return false;
				}
				return false;
			}
			
			CSceneObjectManager *pMan = pMap->GetSceneObjectMan();
			APtrList<CSceneObject*>* pListObj = pMan->GetSortObjectList(CSceneObject::SO_TYPE_STATIC_MODEL);
			ALISTPOSITION pos = pListObj->GetTailPosition();
			while( pos )
			{
				CSceneObject* ptemp = pListObj->GetPrev(pos);
				if(!ptemp->IsDeleted())
				{
					((CStaticModelObject*)ptemp)->QueryRef(&mResMan);
				}
			}

			//删除多余的对象引用.因为该地图中可能有已经被删掉了的对象，这样就必须去除它的引用
			mResMan.DeleteRedundanceReference(ElementResMan::RES_TYPE_BUILDING,pMap->GetMapName(),pMan);
			
			if(!mResMan.Save())
			{
				AUX_UndoCheckOutResTable();
				return false;
			};
			
			if(!AUX_CheckInResTable())
			{
				AfxMessageBox("AUX_ModifiedResTable(),不能把文件ElementRes.table CheckIn到服务器!");
				return false;
			}
		}
	}
	*/
	return true;
}



//下面的两个函数是修改2005.4.28前策划和美术地图AI数据不一致问题
//以后不会存在一个地图被两个人同时使用的问题，所以本修复只做一次
void AUX_Repair_ExpAiData()
{
	AFile dFile;
	if(!dFile.Open("Repair.temp",AFILE_CREATENEW))
	{
		AfxMessageBox("不能创建文件Repair.temp!");
		return;
	}
	CELFileArchive ar(&dFile,false);
	DWORD dwVersion = FS_VERSION;
	DWORD r;
	
	// Load project file list
	AScriptFile File;
	DWORD dwCol;
	char szProj[256];
	sprintf(szProj,"%s\\ProjList.dat",g_szEditMapDir);
	if (!File.Open(szProj))
	{
		g_Log.Log("Open file %s failed!",szProj);
		return;
	}
	
	if(File.GetNextToken(true))
	{
		CString strCol(File.m_szToken);
		if(strCol.Find("col=")!=-1)
		{
			strCol.Replace("col=","");
			dwCol = (int)atof(strCol);
		}else
		{
			File.ResetScriptFile();
			dwCol = 1;
		}
		
	}
	
	CElementMap emap;
	CString mapName;
	DWORD mapNum = 0;
	dFile.Write(&mapNum,sizeof(DWORD),&r);
	
	while (File.GetNextToken(true))
	{
	    g_Log.Log("正处理(输出)：");
		g_Log.Log(File.m_szToken);
		mapName.Format("%s%s\\%s\\%s.elproj",g_szWorkDir,g_szEditMapDir,File.m_szToken,File.m_szToken);
		if(!emap.Load(mapName))
		{
			AfxMessageBox("加载地图失败!");
			File.Close();
			dFile.Close();
			return;
		}
		mapNum ++;
		dFile.WriteString(emap.GetMapName());

		CSceneObjectManager *pMan = emap.GetSceneObjectMan();
		int num = pMan->GetSortObjectNum(CSceneObject::SO_TYPE_FIXED_NPC);
		dFile.Write(&num,sizeof(int),&r);

		ALISTPOSITION pos = pMan->m_listSceneObject.GetTailPosition();
		while( pos )
		{
			CSceneObject* ptemp = pMan->m_listSceneObject.GetPrev(pos);
			int type = ptemp->GetObjectType();
			if(type == CSceneObject::SO_TYPE_FIXED_NPC)
			{
				CSceneFixedNpcGenerator *pFNpc = (CSceneFixedNpcGenerator*)ptemp;
				if(!pFNpc->Save(ar,dwVersion)) goto failed;
			}
		}

		num = pMan->GetSortObjectNum(CSceneObject::SO_TYPE_AIAREA);
		dFile.Write(&num,sizeof(int),&r);
		pos = pMan->m_listSceneObject.GetTailPosition();
		while( pos )
		{
			CSceneObject* ptemp = pMan->m_listSceneObject.GetPrev(pos);
			int type = ptemp->GetObjectType();
			if( type == CSceneObject::SO_TYPE_AIAREA)
			{
				CSceneAIGenerator *pAi = (CSceneAIGenerator*)ptemp;
				if(!pAi->Save(ar,dwVersion)) goto failed;
			}
			
		}
		emap.Release();
	}
	dFile.Seek(0,AFILE_SEEK_SET);
	dFile.Write(&mapNum,sizeof(DWORD),&r);
	File.Close();
	dFile.Close();
	return;

failed:
	emap.Release();
	dFile.Close();
	AfxMessageBox("输出AI数据文件失败!");
}

void AUX_Repair_LoadAiData()
{
	AFile dFile;
	if(!dFile.Open("Repair.temp",AFILE_OPENEXIST))
	{
		AfxMessageBox("不能打开文件Repair.temp!");
		return;
	}
	CELFileArchive ar(&dFile,true);
	DWORD dwVersion = FS_VERSION;
	DWORD r;
	AString mapName;
	DWORD mapNum;
	DWORD num;
	CElementMap emap;
	dFile.Read(&mapNum,sizeof(DWORD),&r);
	dFile.ReadString(mapName);
	for(int i = 0; i < mapNum; i++ )
	{
		dFile.ReadString(mapName);
		CString temp1 = mapName;
		mapName.Format("%s%s\\%s\\%s.elproj",g_szWorkDir,g_szEditMapDir,temp1,temp1);
		g_Log.Log("正处理(加)：");
		g_Log.Log(mapName);
		if(!emap.Load(mapName)) goto failed;
		CSceneObjectManager *pMan = emap.GetSceneObjectMan();
		dFile.Read(&num,sizeof(int),&r);
		int j;
		for(j = 0; j < num; j++)
		{
			CSceneFixedNpcGenerator *pFNpc = new CSceneFixedNpcGenerator;
			ASSERT(pFNpc);
			if(!pFNpc->Load(ar,dwVersion,LOAD_NORMAL)) 
			{
				delete pFNpc;
				goto failed;
			}
			pMan->AddSceneObject(pFNpc);
		}

		dFile.Read(&num,sizeof(int),&r);
		for(j = 0; j < num; j++)
		{
			CSceneAIGenerator *pAI = new CSceneAIGenerator;
			ASSERT(pAI);
			if(!pAI->Load(ar,dwVersion,LOAD_NORMAL)) 
			{
				delete pAI;
				goto failed;
			}
			pMan->AddSceneObject(pAI);
		}

		if(!emap.Save(mapName)) goto failed;
		emap.Release();
	}
	return;
failed:
	emap.Release();
	dFile.Close();
	AfxMessageBox("把AI数据连入地图失败!");
}

bool AUX_ToEulerAnglesXYZ (float * pMatrix, float& rfXAngle, float& rfYAngle,  float& rfZAngle) 
{
		//convert the input left hand to the right hand 3*3 rotation matrix
		float m_afEntry[9];
		
		m_afEntry[0] = pMatrix[0];  m_afEntry[1] = pMatrix[4];  m_afEntry[2] = pMatrix[8];  
		m_afEntry[3] = pMatrix[1];  m_afEntry[4] = pMatrix[5];  m_afEntry[5] = pMatrix[9];  
		m_afEntry[6] = pMatrix[2];  m_afEntry[7] = pMatrix[6];  m_afEntry[8] = pMatrix[10];  
	

    // rot =  cy*cz          -cy*sz           sy
    //        cz*sx*sy+cx*sz  cx*cz-sx*sy*sz -cy*sx
    //       -cx*cz*sy+sx*sz  cz*sx+cx*sy*sz  cx*cy
    if ( m_afEntry[2] < 1.0f )
    {
        if ( m_afEntry[2] > -1.0f )
        {
            rfXAngle = atan2(-m_afEntry[5],m_afEntry[8]);
            rfYAngle = asin((double)m_afEntry[2]);
            rfZAngle = atan2(-m_afEntry[1],m_afEntry[0]);
            return true;
        }
        else
        {
            // WARNING.  Not unique.  XA - ZA = -atan2(r10,r11)
            rfXAngle = -atan2(m_afEntry[3],m_afEntry[4]);
            rfYAngle = -A3D_PI/2.0;
            rfZAngle = 0.0f;
            return false;
        }
    }
    else
    {
        // WARNING.  Not unique.  XAngle + ZAngle = atan2(r10,r11)
        rfXAngle = atan2(m_afEntry[3],m_afEntry[4]);
        rfYAngle = A3D_PI/2.0;
        rfZAngle = 0.0;
        return false;
    }
}

bool AUX_IncreaseVersion(const char *szMap)
{
	char szFullName[MAX_PATH];
	sprintf(szFullName, "%s%s\\%s\\%s.scene", g_szWorkDir,g_szEditMapDir, szMap,szMap);
	if(!AUX_FileIsReadOnly(szFullName))
	{
		FILE *pFile;
		pFile = fopen(szFullName,"r+b");
		if (pFile==NULL)
		{
			g_Log.Log("AUX_IncreaseVersion, Failed to open scene object file %s", szFullName);
			return false;
		}
		//skip head
		if(0!=fseek(pFile,sizeof(DWORD),SEEK_SET)) 
		{
			fclose(pFile);
			return false;
		}
		DWORD dwVersion,dwVssVersion;
		dwVssVersion = 0;
		if(!fread(&dwVersion,sizeof(DWORD),1,pFile)) 
		{
			fclose(pFile);
			return false;
		}
		if(dwVersion>0x2c) 
		{
			if(!fread(&dwVssVersion,sizeof(DWORD),1,pFile))
			{
				fclose(pFile);
				return false;
			}
			if(0!=fseek(pFile,sizeof(DWORD)*2,SEEK_SET)) 
			{
				fclose(pFile);
				return false;
			}
			if(dwVssVersion>50000)
			{
				AfxMessageBox("AUX_IncreaseVersion,程序跟踪VSS版本数据可能已经发生错误! 请马上与程序部门联系!");
				return false;
			}
			dwVssVersion++;
			if(!fwrite(&dwVssVersion,sizeof(DWORD),1,pFile))
			{
				fclose(pFile);
				return false;
			}
		}
		fclose(pFile);
	}else return false;
	return true;
}

void CALLBACK AUX_VssLog(const char* szLog)
{
	g_LogVss.Log(szLog);
	if(AUX_GetMainFrame())
		if(AUX_GetMainFrame()->GetVssBar())
			AUX_GetMainFrame()->GetVssBar()->AddString(szLog);	
}

const char* _format_global_id = "GlobalID=%d";
int GenerateGlobalID()
{
	g_VSS.SetProjectPath("ResTable");
	g_VSS.CheckOutFile("ResTable\\GlobalID.dat");
	if(!g_VSS.IsCheckOutStatus("ResTable\\GlobalID.dat")) return -1;

	AFile File;
	char szBuf[AFILE_LINEMAXLEN];
	int nId = -1;
	DWORD dwRead;
	memset(szBuf,0,AFILE_LINEMAXLEN);
	if (File.Open("ResTable\\GlobalID.dat", AFILE_OPENEXIST | AFILE_TEXT)) 	
	{
		while(true)
		{
			if (!File.ReadLine(szBuf, AFILE_LINEMAXLEN, &dwRead)
		||	strlen(szBuf) == 0) break;
			CString str = szBuf;
			if(str.Find("GlobalID")!=-1)
			{
				sscanf(szBuf, _format_global_id,&nId);
				break;
			}
		}
	}
	File.Close();
	
	if(nId!=-1)
	{
		AFile sfile;
		char	szLine[AFILE_LINEMAXLEN];
		if (!sfile.Open("ResTable\\GlobalID.dat", AFILE_CREATENEW | AFILE_TEXT))
			goto fail;
		sprintf(szLine, _format_global_id, nId + 1);
		sfile.WriteLine(szLine);
		sfile.Close();

		g_VSS.SetProjectPath("ResTable");
		g_VSS.CheckInFile("GlobalID.dat");
		if(g_VSS.IsCheckOutStatus("ResTable\\GlobalID.dat")) return -1;
		return nId + 1;
	}

fail:	
	g_VSS.SetProjectPath("ResTable");
	g_VSS.UndoCheckOut("ResTable\\GlobalID.dat");
	return -1;
}

class APhysXScene;

APhysXScene *gGetAPhysXScene(void)
{
	return 0;
}

void Aux_CaptureCamera()
{
	if(g_Render.GetA3DEngine()==NULL) return;
	float parm[4];
	A3DCamera *pca = (A3DCamera*) g_Render.GetA3DEngine()->GetActiveCamera();
	if(pca && !g_bCameraCapture)
	{
		g_vLastPos = pca->GetPos();
		g_vLastDir = pca->GetDir();
		pca->GetProjectionParam(&parm[0],&parm[1],&parm[2],&parm[3]);
		g_fCameraFov = parm[0];
		g_fCameraRadio = parm[3];
		g_bCameraCapture = true;
	}
	AUX_GetMainFrame()->HideBezierPointPro();
}

void Aux_RestoreCamera()
{
	if(g_Render.GetA3DEngine()==NULL) return;
	A3DCamera *pca = (A3DCamera*) g_Render.GetA3DEngine()->GetActiveCamera();
	if(pca && g_bCameraCapture)
	{
		pca->SetDirAndUp(g_vLastDir,A3DVECTOR3(0,1.0f,0));
		pca->SetPos(g_vLastPos);
		float parm[4];
		pca->GetProjectionParam(&parm[0],&parm[1],&parm[2],&parm[3]);
		pca->SetProjectionParam(g_fCameraFov,parm[1],parm[2],g_fCameraRadio);
		g_bCameraCapture = false;
	}
	
	AUX_GetMainFrame()->HideBezierPointPro();
}

__GLOBALDATA AUX_GetGlobalData()
{
	__GLOBALDATA dat;
	dat.fBrightness = 2.0f;
	dat.fBrightness_n = 2.0f;

	CString str;
	str.Format("%s",g_szEditMapDir);
	g_VSS.SetProjectPath(str);
	str.Format("%s\\global.dat",g_szEditMapDir);
	g_VSS.GetFile(str);

	FILE *pf = fopen(str,"r");
	if(pf == NULL) return dat;

	fscanf(pf,"Brightness: %f",&dat.fBrightness);
	fscanf(pf,"Brightness_n: %f",&dat.fBrightness_n);


	fclose(pf);
	return dat;
}

__GLOBALDATA g_GlobalData;

bool AUX_ModifyGlobalData(__GLOBALDATA& dat)
{
	CString str;
	str.Format("%s",g_szEditMapDir);
	g_VSS.SetProjectPath(str);
	str.Format("%s\\global.dat",g_szEditMapDir);
	g_VSS.CheckOutFile(str);
	int r = g_VSS.IsCheckOutStatus(str);
	if(0==r) return false;
	else 
	{
		FILE *pf = fopen(str,"w");
		if(pf == NULL)
		{
			g_VSS.UndoCheckOut(str);
			return false;
		}

		fprintf(pf,"Brightness: %f",dat.fBrightness);
		fprintf(pf,"Brightness_n: %f",dat.fBrightness_n);

		fclose(pf);

		if(r==2)
		{
			g_VSS.AddFile(str);
			return true;
		}
	}

	str.Format("%s",g_szEditMapDir);
	g_VSS.SetProjectPath(str);
	g_VSS.CheckInFile("global.dat");
	str.Format("%s\\global.dat",g_szEditMapDir);
	if(0==g_VSS.IsCheckOutStatus(str)) return true;
	return false;
}

void LoadVertLitDataFromFile(A3DLitModel* pLitModel, const char* szVLitDataFormat, int nDayNight)
{
	const int	nMesh = pLitModel->GetNumMeshes();
	AString		strLitDataFile;
	char		buffer[1024];
	A3DCOLORVALUE crVert;

	crVert.a = 1.0f;

	for(int i = 0; i < nMesh; i++)
	{
		strLitDataFile.Format(szVLitDataFormat, i);
		FILE* fp = fopen(strLitDataFile, "rt");
		if(fp != NULL)
		{	
			A3DLitMesh* pMesh = pLitModel->GetMesh(i);
			const int nVert = pMesh->GetNumVerts();

			fgets(buffer, 1024, fp);
			if(nVert == atoi(buffer))
			{
				A3DCOLOR* pColor = nDayNight == 0 ? pMesh->GetDayColors() : pMesh->GetNightColors();

				for(int n = 0; n < nVert; n++)
				{
					fgets(buffer, 1024, fp);
					sscanf(buffer, "VD %f,%f,%f", &crVert.r, &crVert.g, &crVert.b);

					pColor[n] = crVert.ToRGBAColor();
				}
				
#ifdef _ANGELICA22_FEATURE
				g_Render.GetA3DEngine()->GetA3DEnvironment()->SetDNFactor(-1);
				pMesh->UpdateColorByDayNight();
				g_Render.GetA3DEngine()->GetA3DEnvironment()->SetDNFactor(g_fNightFactor);
#endif
				//pMesh->UpdateColors();
			}

			fclose(fp);
		}
	}
}

float AUX_GetAxisGizmoProjLenByViewport(A3DViewport* pViewport, const A3DVECTOR3& vDir)
{
	float fAxisGizmoProjLen = 0.f;

	if (!pViewport)
		return fAxisGizmoProjLen;

	A3DCameraBase* pCamera = pViewport->GetCamera();
	if (!pCamera)
		return fAxisGizmoProjLen;

	A3DVIEWPORTPARAM* param = pViewport->GetParam();
	if (!param)
		return fAxisGizmoProjLen;

	if (pCamera->GetZFront() < 0)
		fAxisGizmoProjLen = (600.f / param->Width) * pViewport->GetCamera()->GetZBack() / 240.0f;
	else
		fAxisGizmoProjLen= (10.0f / (float)param->Height) * vDir.Magnitude();

	return fAxisGizmoProjLen;
}
