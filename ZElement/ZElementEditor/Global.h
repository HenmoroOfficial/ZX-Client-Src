/*
 * FILE: Global.h
 *
 * DESCRIPTION: Global variables and definitions
 *
 * CREATED BY: Duyuxin, 2003/9/1
 *
 * HISTORY:
 *
 * Copyright (c) 2001 Archosaur Studio, All Rights Reserved.	
 */

#pragma once

#include "stdafx.h"
#include "Resource.h"

#include "ALog.h"
#include "AMemory.h"
#include "AString.h"
#include "A3DTypes.h"
#include "BezierMergeDlg.h"
#include "EdtDataCache.h"
#include "LogDlg.h"
#ifdef _ANGELICA22
#include "ElementRenderParam.h"
#endif
#include "pathMap\PassMapGenerator.h"





class CMainFrame;
class CElementEditorApp;
class A3DTexture;
class elementdataman;
class CTerrainObstructGenerator;
class CTerrainBlockViewDlg;
class CVssOperation;
class CELBackMusic;
class DataPathMan;
class A3DLitModel;
#ifndef _ANGELICA22
class A3DViewport;
#endif



///////////////////////////////////////////////////////////////////////////
//
//	Define and Macro
//
///////////////////////////////////////////////////////////////////////////

#ifdef _ANGELICA22
#define CONFIG_DIR "Configs\\2.2\\"
#define SHADER_DIR "Shaders\\2.2\\"
#else
#define CONFIG_DIR "Configs\\"
#define SHADER_DIR "Shaders\\"
#endif // #ifdef _ANGELICA22


//	Define window message
#define WM_ACTIVATETOOLTABPAGE	WM_APP + 100	//	w: 1 - activate;

//	Viewport index
enum
{
	VIEW_XZ = 0,
	VIEW_XY,
	VIEW_YZ,
	VIEW_PERSPECTIVE,
	NUM_VIEWPORT,
};

///////////////////////////////////////////////////////////////////////////
//
//	Types and Global variables
//
///////////////////////////////////////////////////////////////////////////

struct PLAYER_SAMPLE_POS {
	int u;				// X
	int v;				// Z
	float h;		  // Y
};


//	Editing config
struct EDITCONFIG
{
	//	General options
	float   fAirPassMapDelta;
	float	fCameraMove;		//	Camera move speed (m/s)
	float	fCameraRot;			//	Camera rotation speed (deg/pixel)
	bool	bAlongGround;		//	Along ground
	bool    bEnableSpeak;       //  Speak?
	bool    bShowModelHull; 
	bool	bShowBoundBox;
	bool	bHostMode;
	bool	bNeighborTerrainOnly;
	

	// Pick object options
	bool    bPickStaticModel;   
	bool    bPickLight;
	bool    bPickAudio;
	bool    bPickBoxArea;
	bool    bPickDummy;
	bool    bPickSpecially;
	bool    bPickWater;
	bool    bPickAIArea;
	bool    bPickPath;
	bool    bPickSkinModel;
	bool    bPickCritterGroup;
	bool    bPickFixedNpc;
	bool    bPickPrecinct;
	bool    bPickGather;
	bool    bPickRange;
	bool    bPickCloudBox;
	bool    bPickInstanceBox;
	bool    bPickDynamic;
	bool	bPickBuffRegion;
	bool	bPickGuildBuilding;

	//	New map settings
	float	fTileSize;			//	Terrain tile size
	float	fMinHeight;			//	Minimum height
	float	fMaxHeight;			//	Maximum height

	int		iMaxTexSize;		//	Maximum texture size

	//	Render options
	float	fViewRadius;		//	View (render) radius
	bool	bWireFrame;			//	Wire frame mode
	bool	bImmeMaskPaint;		//	Immediately display mask paint effect
	bool    bEnableSky;         //  Enable sky box
	int		iMaxLayerWeight;	//	Maximum layer weight
	bool    bShowObjectName;    //  Enable object name to show
	bool    bObjectAddContinue; //  Add object continuum;
	bool    bEnableFog;         //  Enable all fog
	bool    bShowVernier;       //  Enable for show vernier
#ifdef _ANGELICA22_FEATURE
	int     nWaterEffect;    
	bool	bHDRLitmap;
#else
	bool    bSampleWater;       
#endif // #ifdef _ANGELICA22_FEATURE
	bool    bEnableGlbFog;      // Enable Volumetric Fog
	bool    bUsePlatformRender; 

	bool    bShowStaticModel;
	bool    bShowLight;
	bool    bShowAudio;
	bool    bShowBoxArea;
	bool    bShowDummy;
	bool    bShowSpecially;
	bool    bShowWater;
	bool    bShowAIArea;
	bool    bShowPath;
	bool    bShowSkinModel;
	bool    bShowCritterGroup;
	bool    bShowFixedNpc;
	bool    bShowFixedNpcWire;
	bool    bShowTerrainBlock;
	bool    bShowSmallBlock;
	bool    bShowRain;
	bool    bShowSnow;
	bool    bShowGrass;
	bool    bShowTree;
	bool    bShowGather;
	bool    bShowTerrain;
	bool    bShowRange;
	bool    bShowCloudBox;
	bool    bShowCloudLine;
	bool    bShowInstanceBox;
	DWORD   dwShowCopys;
	bool    bShowBufRegion;
	bool	bShowMotionPath;
	int     nShowCopyNum;
	bool    bShowDynamic;

	bool	bShowPostEffect;
	bool	bShowSunMoon;
	bool	bShowMirrorPlane;
	bool    bShowIcon;
	
	//	Terrain export options
	int		iTExpMaskTile;		//	Export mask clip tile number
	int		iTExpColNum;		//	Export projection column number
	int		iTExpMaxLayer;		//	Maximum layer number
	int		iTExpBlkGrid;		//	Block grid, each terrain block has (grid x grid) grids

	//  Brush options
	int     iBrushRadius;
	bool    bCircleBrush;
	float   fCurveControl1;     // Brush control point
	float   fCurveControl2;
	float   fCurveControl3;
	float   fCurveControl4;
	float   fCurveControl5;
	float   fCurveControl6;
	float   fCurveControl7;
	//	Lighting settings
	int		iLightMapSize;		//	Light map size
	int		iSunDir;			//	Sun direction
	int		iSunHeight;			//	Sun height
	DWORD	dwSunCol;			//	Sun color
	DWORD	dwAmbient;			//	Ambient color
	float   fSunPower;          //  Light power
	float   fSunFaction;
	int		iNightLightMapSize;		//	Light map size
	int		iNightSunDir;			//	Sun direction
	int		iNightSunHeight;			//	Sun height
	DWORD	dwNightSunCol;			//	Sun color
	DWORD	dwNightAmbient;			//	Ambient color
	float   fNightSunPower;          //  Light power
	float   fNightSunFaction;

	// Virtual source safe case
	char    szUserName[64];
	char    szUserPassword[64];
	char    szServerPath[128];
	char    szCurProjectName[128];
	char    szExpPath[128];
	int     iSfxVolume;
	int     iMusicVolume;
	
	int		iIsOverseaVersion;	//0:local version;   1:Oversea version
};

//	Lighting parameters
struct LIGHTINGPARAMS
{
	A3DVECTOR3	vSunDir;	//	Sun direction
	DWORD		dwAmbient;	//	Ambient color 反向的环境光
    DWORD		dwAmbient2;	//	Ambient color 垂直的环境光
	DWORD		dwSunCol;	//	Sun color
	int			iMapSize;	//	Light map size (width and height in pixels)
	float       fSunPower;  //  Sun Power
    float       fAmbientPower;  //  Ambient Power
};

extern ALog			g_Log;
extern ALog         g_LogVss;
extern EDITCONFIG	g_Configs;
extern char			g_szWorkDir[];
extern char			g_szIniFile[];		//	.ini file name
extern char			g_szEditMapDir[];	//	Edit maps directory
extern char			g_szTempMap[];		//	Temporary map name
extern char			g_szDefTexFile[];	//	Default texture file name
extern char         g_szProject[];      //Project name
extern const char   *	g_szVersion;
extern const char	*	g_szCompileTime;

extern bool         g_bCameraCapture;
extern A3DVECTOR3	g_vLastPos;
extern A3DVECTOR3	g_vLastDir;
extern A3DVECTOR3	g_vOrigin;
extern A3DVECTOR3	g_vAxisX;
extern A3DVECTOR3	g_vAxisY;
extern A3DVECTOR3	g_vAxisZ;
extern bool         g_bReadOnly;
extern bool         g_bShowPosDot;
extern bool         g_bBezierPtSnap;
extern bool         g_bShowModelPlane;
extern bool         g_bShowSelectedOnly;
extern CELBackMusic g_BackMusic;
extern bool         g_bOnlyCalMiniMap;
extern DWORD        g_dwExpCopy;
extern bool         g_bWTPacket;
extern bool			g_bOnlyLoadNearBuildForLight;


//鼠标指针
extern HCURSOR            g_hMoveCursor;
extern HCURSOR            g_hSelectCursor;
extern HCURSOR			  g_hScaleCursor;
extern HCURSOR			  g_hRotateCursor;
extern HCURSOR			  g_hAddObjectCursor;
extern elementdataman     g_dataMan;
extern DataPathMan        g_dataPathMan;
extern CTerrainObstructGenerator g_TerrainObstuctGenerator;
extern CBezierMergeDlg    g_BezierMergeDlg;
extern CLogDlg            g_LogDlg;
extern int                g_nCurrentTerrainBlock;
extern CTerrainBlockViewDlg g_BlockViewDlg;
extern bool               g_bIsNight;
extern CVssOperation      g_VSS;
extern float              g_fNightFactor;
extern bool               g_bShowTerrainLight;
extern bool               g_bShowCollideModelOnly;
extern DWORD              g_dwWaterColor;
extern int				  g_nCurPlayerPassmapLayer;
extern int				  g_nCurPassmapLayer;
extern bool               g_bLockCam;
extern CEdtDataCache	  g_EdtDataCache;

#ifdef _ANGELICA22
extern A3DPOST_EFFECT_PARAM g_PostEffectParam;
extern A3DPOST_EFFECT_PARAM g_PostEffectParamNight;
#endif 
extern DWORD				g_dwPostEffectAdditionalFlags;		// 编辑器使用的一些额外的控制标志

#define						PostEffectAdditionalFlag_UnitedWarp 1

//	Global output message functino
typedef void (*LPFNLOGOUTPUT)(DWORD dwUserData, const char* szMsg);

/*	Global progress bar function

	iTotal: < 0. reset progress bar, -iTotal is maximum progress step number
			>= 0. iComplete is current step.
*/
typedef void (*LPFNPROGRESS)(DWORD dwUserData, int iTotal, int iComplete);

///////////////////////////////////////////////////////////////////////////
//
//	Declare of Global functions
//
///////////////////////////////////////////////////////////////////////////

//	Get application object
CElementEditorApp* AUX_GetApp();
//	Get main frame window object
CMainFrame*	AUX_GetMainFrame();
//	Popup a application message box
int AUX_MessageBox(int iType, const char* szMsg, ...);
//	Change a 3D edit box to flat style
void AUX_FlatEditBox(CEdit* pEdit);
//	Create brush
HBRUSH AUX_CreateBrush(UINT lbStyle, COLORREF lbColor, LONG lbHatch);
//	Create a directory
bool AUX_CreateDirectory(const char* szDir);

//	Save all configs to .ini file
void AUX_SaveConfigs();
//	Restore configs to default value
void AUX_DefaultConfigs();
//	Load configs from .ini file
void AUX_LoadConfigs();

//	Create project directory of specified name
bool AUX_CreateProjectDir(HWND hWnd, const CString& strProjName);
//	Chose a proper texture size
int AUX_ChoseTextureSize(float fApproSize);

//	Check whether specified exists
bool AUX_FileExist(const char* szFile);
//	Change file's extension
void AUX_ChangeExtension(char* szFile, char* szNewExt);
//	Write string to file
bool AUX_WriteString(FILE* fp, const char* str);

//	Set global log output and progress bar function
void AUX_SetProgressFuncs(LPFNLOGOUTPUT lpfnLog, LPFNPROGRESS lpfnProgress, DWORD dwUserData);
//	Output progress log
void AUX_ProgressLog(const char* szMsg, ...);
//	Reset progress bar
void AUX_ResetProgressBar(int iTotal);
//	Step progress bar
void AUX_StepProgressBar(int iStep);

//  Load corsor
void AUX_LoadCursor();

//  Load Template data
void AUX_LoadTemplateData();

//  Load data path
void AUX_LoadDataPath();

//  Fuction for vss
bool AUX_FileIsReadOnly(CString szFilePathName);

bool AUX_FileIsExist(CString szFilePathName);

bool AUX_CheckOutResTable();

bool AUX_UndoCheckOutResTable();

bool AUX_CheckInResTable();

bool AUX_CheckOutWorkspace();

bool AUX_CheckInWorkspace();

bool AUX_CheckOutProjList();

bool AUX_CheckInProjList();

bool AUX_UndoCheckOutWorkspace();

bool AUX_CheckOutMap(char *szMap);

bool AUX_CheckInMap(char *szMap);

bool AUX_CheckOutAllMap();

bool AUX_CheckInAllMap();

void AUX_UndoCheckOutMap(char *szMap);

bool AUX_UndoCheckOutResTable();

bool AUX_ModifiedResTable();

bool Aux_IsAllFilesCheckIn(char *szMap);

bool AUX_CopyFile(const char* dst, const char* src);

bool AUX_CopyFolder(const char *szDst,const char *szSrc,bool bRender = true);

bool AUX_CopyFolderRecur(const char *szDst,const char *szSrc);

//下面的两个函数是修改2005.4.28前策划和美术地图AI数据不一致问题
//以后不会存在一个地图被两个人同时使用的问题，所以本修复只做一次
void AUX_Repair_ExpAiData();

void AUX_Repair_LoadAiData();

bool AUX_ToEulerAnglesXYZ (float * pMatrix, float& rfXAngle, float& rfYAngle,  float& rfZAngle);

bool AUX_IncreaseVersion(const char* szMap);

void CALLBACK AUX_VssLog(const char* szLog);

int GenerateGlobalID();

void Aux_CaptureCamera();

void Aux_RestoreCamera();

void LoadVertLitDataFromFile(A3DLitModel* pLitModel, const char* szVLitDataFormat, int nDayNight);

float AUX_GetAxisGizmoProjLenByViewport(A3DViewport* pViewport, const A3DVECTOR3& vDir);
///////////////////////////////////////////////////////////////////////////
//
//	Inline functions
//
///////////////////////////////////////////////////////////////////////////

struct __GLOBALDATA 
{
	float fBrightness;
	float fBrightness_n;

	__GLOBALDATA() : fBrightness(1.0f),fBrightness_n(1.0f) {}
};

__GLOBALDATA AUX_GetGlobalData();
bool AUX_ModifyGlobalData(__GLOBALDATA& dat);

extern __GLOBALDATA g_GlobalData;
