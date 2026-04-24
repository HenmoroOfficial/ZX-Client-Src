/*
 * FILE: EC_Configs.cpp
 *
 * DESCRIPTION: 
 *
 * CREATED BY: Duyuxin, 2004/8/27
 *
 * HISTORY: 
 *
 * Copyright (c) 2004 Archosaur Studio, All Rights Reserved.
 */

#include "EC_Global.h"
#include "EC_Configs.h"
#include "EC_World.h"
#include "EC_Game.h"
#include "EC_GameRun.h"
#include "EL_Forest.h"
#include "EC_NPC.h"
#include "EC_ShadowRender.h"
#include "EC_FullGlowRender.h"
#include "EC_SimpleBloomRender.h"
#include "EC_ScreenRender.h"
#include "EC_SceneLoader.h"
#include "EC_SceneBlock.h"
#include "EL_CloudManager.h"
#include "EC_ManOrnament.h"
#include "EC_HostPlayer.h"
//#include "EC_GameSession.h"
//#include "EC_Skill.h"
//#include "EC_Secretary.h"
#include "EC_RTDebug.h"
//#include "EC_UIManager.h"
//#include "EC_GameUIMan.h"
//#include "AUI\\AUIDialog.h"
//#include "DlgChat.h"
//#include "DlgSettingVideo.h"

#include "AFI.h"
#include "AIniFile.h"
#include "A3DMacros.h"
#include "A3DTerrain2.h"
#include "AAssist.h"
#include "A3DEngine.h"
#include "A3DDevice.h"
#include "A3DViewport.h"
#include "A3DCameraBase.h"
#include "A3DCamera.h"
#include "A3DGfxExMan.h"
#include "A3DConfig.h"
#include "AMConfig.h"
#include "AMEngine.h"
#include "AMSoundEngine.h"
#include "AMSoundBufferMan.h"

#ifdef BACK_VERSION
#include "EL_GrassLand.h"
#include "EC_Viewport.h"
#include "ELTerrainOutline2.h"
#include "A3DTerrainWater.h"
#else
#include "A3DGrassland.h"
#include "ECViewport.h"
#include "A3DTerrainOutline2.h"
#include "A3DTerrainWater2.h"
#endif

#define new A_DEBUG_NEW

///////////////////////////////////////////////////////////////////////////
//	
//	Define and Macro
//	
///////////////////////////////////////////////////////////////////////////

//	Configs data version
#define EC_CONFIG_VERSION			24
#define EC_SYSTEM_CONFIG_VERSION	1


//  Registry Field
#define REG_COMPANY_NAME _AL("Perfect World Entertainment")

#define REG_INSTALL_PATH _AL("INSTALL_PATH")
#define REG_PATCHER_PATH _AL("PATCHER_PATH")
#define REG_LAUNCHER_PATH _AL("LAUNCHER_PATH")
#define REG_CLIENT_PATH _AL("CLIENT_PATH")
#define REG_UNINSTALL_PATH _AL("UNINSTALL_PATH")

#define PATCHER_PATH _AL("patcher")
#define LAUNCHER_PATH _AL("launcher")
#define CLIENT_PATH _AL("element")
#define PATCHER_EXE _AL("patcher.exe")
#define LAUNCHER_EXE _AL("launcher.exe")
#define UNISTALL_EXE _AL("uninstall.exe")
#define CLIENT_EXE _AL("elementclient.exe")

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

void WriteRegistry(AWString strWorkPath , AWString strKey)
{
	DWORD dwDisposition;
	HKEY hkCompany = NULL;
	HKEY hkGame = NULL;
	wchar_t* strCompany = REG_COMPANY_NAME;
	if(ERROR_SUCCESS == 
		RegCreateKeyEx(HKEY_CLASSES_ROOT, strCompany, 0 , _AL("") , REG_OPTION_NON_VOLATILE , 
		KEY_READ | KEY_WRITE ,	NULL , 	&hkCompany , &dwDisposition ))
	{
		if(ERROR_SUCCESS == 
			RegCreateKeyEx(hkCompany, strKey, 0 ,	_AL("") , REG_OPTION_NON_VOLATILE , 
			KEY_READ | KEY_WRITE ,	NULL , 	&hkGame , &dwDisposition ))
		{
			wchar_t* keys[] = {REG_INSTALL_PATH, REG_PATCHER_PATH ,REG_LAUNCHER_PATH,REG_CLIENT_PATH,REG_UNINSTALL_PATH};
			wchar_t* paths[] = {NULL , PATCHER_PATH , LAUNCHER_PATH , CLIENT_PATH , NULL};
			wchar_t* exes[] = {NULL , PATCHER_EXE , LAUNCHER_EXE , CLIENT_EXE , UNISTALL_EXE};
			const wchar_t* szCurDir = strWorkPath;//[MAX_PATH];
			//GetCurrentDirectoryA(MAX_PATH, szCurDir);
			//szCurDir = WC2AS(strWorkPath);
			wchar_t szParentDir[MAX_PATH];
			const wchar_t* pTemp = wcsrchr(szCurDir , '\\');
			wcsncpy(szParentDir , szCurDir , pTemp - szCurDir);
			szParentDir[pTemp - szCurDir] = '\0';
			for(int i = 0;i < sizeof(paths)/sizeof(wchar_t*) ;i++)
			{
				AWString strTemp = szParentDir;
				strTemp += '\\';
				if(paths[i])
				{
					strTemp += paths[i];
					strTemp += '\\';
				}
				if(exes[i])
				{
					strTemp += exes[i];
					WIN32_FIND_DATA FindFileData;
					HANDLE hFind;
					hFind = FindFirstFile(strTemp , &FindFileData);
					if (hFind == INVALID_HANDLE_VALUE)
					{
						a_LogOutput(1 , "File %s doesn't exist" , strTemp);
						strTemp = _AL("");
					}
				}
				const wchar_t* strValue = strTemp;
				if(ERROR_SUCCESS !=
					RegSetValueEx(hkGame , keys[i] , 0 , REG_SZ , (LPBYTE)strValue , (wcslen(strValue)+1) *sizeof(wchar_t)))
				{
					a_LogOutput(1 , "Write Registry %s failed" , keys[i]);
				}
			}
			RegCloseKey(hkGame);
		}
		RegCloseKey(hkCompany);
	}
}

///////////////////////////////////////////////////////////////////////////
//	
//	Implement CECConfigs
//	
///////////////////////////////////////////////////////////////////////////

CECConfigs::CECConfigs()
{
	//	Configs user cannot change ...
	m_strLoginDcf		= "AUI\\LoginUI.dcf";
	m_strInGameDcf		= "AUI\\InGame.dcf";
	m_strLoadingDcf		= "AUI\\LoadingUI.dcf";
	m_strCountryRegion  = "cn";
	m_strWindowTitle	= "诛仙2";
	m_strWindowReplayTitle	= "诛仙录像回放--";
	m_strRegKey			= "";

	m_iCodePage			= 936;
	m_fSceLoadRadius	= 500.0f;
	m_dwDefFogColor		= A3DCOLORRGB(70, 77, 91);
	m_fSevActiveRad		= 170.0f;
	m_fForestDetail		= 1.0f;
	m_fGrassLandDetail	= 1.0f;
	m_fTrnLODDist1		= 180.0f;
	m_fTrnLODDist2		= 320.0f;
	m_dwPlayerText		= 0;
	m_fMinPVRadius		= 5.0f;
	m_fCurPVRadius		= m_fSevActiveRad;
	m_bNormalChat30Words = true;

	//	DEBUG only settings ...
	m_bConsole			= false;
	m_iRTDebug			= 1;
	m_bShowNPCID		= false;
	m_bShowItemId		= false;
	m_fRunSpeed			= 0.0f;
	m_fWalkSpeed		= 0.0f;
	m_bTestDist			= false;
	m_bShowPos			= false;
	m_bShowForest		= true;
	m_bGameStat			= false;
	m_bShowID			= false;
	m_bSkipFrame		= false;
	m_bModelUpt			= true;

	m_bAdjusting		= false;
	m_bSnsPhoto			= true;
	m_bSpread			= true;
	m_bAutoHelperEnable = true;
	m_fCamDis			= 10.0f;
	m_iPlayerDeg		= 180;

	//	Test code ... 
	m_bShowForest		= true;
	m_bShowGrassLand	= true;
	
	m_ConfigsData.DefaultUserSettings(&m_ConfigsData.m_vs, &m_ConfigsData.m_gs);

	BuildTextFlags();
	memset(&m_ls, 0, sizeof (m_ls));
}

CECConfigs::~CECConfigs()
{
}

//	Initialize object
bool CECConfigs::Init(const char* szCfgFile, const char* szSSFile)
{
	//	Load configs ...
	if (!LoadConfigs(szCfgFile))
	{
		glb_ErrorOutput(ECERR_FAILEDTOCALL, "CECConfigs::Init", __LINE__);
		return false;
	}

	//	Load user settings ...
	if (!LoadSystemSettings(szSSFile))
	{
		glb_ErrorOutput(ECERR_FAILEDTOCALL, "CECConfigs::Init", __LINE__);
		return false;
	}

	// ensure the fullscreen resolution can be supported on this device
	if( m_ss.bFullScreen )
	{
#ifdef BACK_VERSION
		IDirect3D8 *	pD3D;
		pD3D = Direct3DCreate8(D3D_SDK_VERSION);
#else
		IDirect3D9 *	pD3D;
		pD3D = Direct3DCreate9(D3D_SDK_VERSION);
#endif
		if( pD3D == NULL )
		{
			glb_ErrorOutput(ECERR_FAILEDTOCALL, "CECConfigs::Init", __LINE__);
			return false;
		}

		A3DDevice::DISPLAYMODEARRAY displayModes;
		A3DDevice::EnumDisplayModes(pD3D, A3DFMT_UNKNOWN, displayModes);

		pD3D->Release(); 

		int idMode;
		for(idMode=0; idMode<displayModes.GetSize(); idMode++)
		{
			if( (int)displayModes[idMode].Width == m_ss.iRndWidth && (int)displayModes[idMode].Height == m_ss.iRndHeight )
				break;
		}

		if( idMode == displayModes.GetSize() )
		{
			// now current device width and height are invalid on this device, engine will start chose best proper display mode
			a_LogOutput(1, "Fullscreen resolution[%dx%d] is not supported. Engine will chose one proper resolution",
				m_ss.iRndWidth, m_ss.iRndHeight);

			int nBestWidth = 0;
			int nBestHeight = 0;
			int nBestDiff = 0x7fffffff;
			for(idMode=0; idMode<displayModes.GetSize(); idMode++)
			{
				int nDiff = abs(int(displayModes[idMode].Width) - m_ss.iRndWidth) + 
					abs(int(displayModes[idMode].Height) - m_ss.iRndHeight);
				if( nDiff < nBestDiff  )
				{
					nBestWidth = displayModes[idMode].Width;
					nBestHeight = displayModes[idMode].Height;
					nBestDiff = nDiff;
				}
			}

			if( nBestWidth == 0 || nBestHeight == 0 )
			{
				a_LogOutput(1, "Can't find proper display mode for this device.");
				return false;
			}
			else
			{
				a_LogOutput(1, "Chose displaymode [%dx%d]", nBestWidth, nBestHeight);
				m_ss.iRndWidth = nBestWidth;
				m_ss.iRndHeight = nBestHeight;
			}
		}
	}
	return true;
}

//	Load configs from file
bool CECConfigs::LoadConfigs(const char* szFile)
{
	AIniFile IniFile;

	if (!IniFile.Open(szFile))
	{
		a_LogOutput(1, "CECConfigs::LoadConfigs, Failed to open file %s", szFile);
		return false;
	}

	//	PathFile section
	AString strSect = "PathFile";

	m_strLoginDcf	= IniFile.GetValueAsString(strSect, "login_ui_dcf", m_strLoginDcf);
	m_strInGameDcf	= IniFile.GetValueAsString(strSect, "ingame_ui_dcf", m_strInGameDcf);
	m_strLoadingDcf	= IniFile.GetValueAsString(strSect, "loading_ui_dcf", m_strLoadingDcf);

	//	Settings section
	strSect = "Settings";

	m_iCodePage				= IniFile.GetValueAsInt(strSect, "code_page", m_iCodePage);
	m_bConsole				= IniFile.GetValueAsInt(strSect, "console", m_bConsole ? 1 : 0) ? true : false;
	m_iRTDebug				= IniFile.GetValueAsInt(strSect, "runtime_debug", m_iRTDebug);
	m_strCountryRegion		= IniFile.GetValueAsString(strSect, "country_region", m_strCountryRegion);
	m_bSnsPhoto				= IniFile.GetValueAsInt(strSect, "sns_photo", m_bSnsPhoto ? 1 : 0) ? true : false;
	m_bSpread				= IniFile.GetValueAsInt(strSect, "spread", m_bSpread ? 1 : 0) ? true : false;
	m_strWindowTitle		= IniFile.GetValueAsString(strSect, "window_title", m_strWindowTitle);
	m_strWindowReplayTitle	= IniFile.GetValueAsString(strSect, "window_replay_title", m_strWindowReplayTitle);
	m_bAutoHelperEnable		= IniFile.GetValueAsInt(strSect, "auto_helper", m_bAutoHelperEnable? 1:0 ) ? true:false;
	m_bNormalChat30Words	= IniFile.GetValueAsInt(strSect, "30_words", m_bNormalChat30Words? 1:0 ) ? true:false;
	m_strCountryRegion.MakeLower();
	m_strRegKey	= IniFile.GetValueAsString(strSect , "reg_key" , m_strRegKey);
	if(m_strRegKey && m_strRegKey != "")		//need to write Registry
	{
		//GetCurrentDirectoryA(MAX_PATH, szCurDir);
		wchar_t szCurDir[MAX_PATH];
		GetCurrentDirectoryW(MAX_PATH , szCurDir);
		WriteRegistry(szCurDir , AS2AC(m_strRegKey));
	}
	
	//	Default environment value
	strSect = "DefEnv";

	m_fDefFogStart		= IniFile.GetValueAsFloat(strSect, "fog_start", 250.0f);
	m_fDefFogEnd		= IniFile.GetValueAsFloat(strSect, "fog_end", 600.0f);
	m_fDefFogDensity	= IniFile.GetValueAsFloat(strSect, "fog_density", 1.0f);
	m_strDefSkyFile0	= IniFile.GetValueAsString(strSect, "sky_image01", "5\\Roof.bmp");
	m_strDefSkyFile1	= IniFile.GetValueAsString(strSect, "sky_image02", "5\\01.bmp");
	m_strDefSkyFile2	= IniFile.GetValueAsString(strSect, "sky_image03", "5\\02.bmp");

	int col[3];
	IniFile.GetValueAsIntArray(strSect, "fog_color", 3, col);
	m_dwDefFogColor = A3DCOLORRGB(col[0], col[1], col[2]);

	IniFile.GetValueAsIntArray(strSect, "ambient", 3, col);
	m_dwDefAmbient = A3DCOLORRGB(col[0], col[1], col[2]);

	IniFile.GetValueAsIntArray(strSect, "light_diffuse", 3, col);
	m_dwDefLightDiff = A3DCOLORRGB(col[0], col[1], col[2]);

	IniFile.GetValueAsIntArray(strSect, "light_specular", 3, col);
	m_dwDefLightSpec = A3DCOLORRGB(col[0], col[1], col[2]);

	IniFile.GetValueAsFloatArray(strSect, "light_dir", 3, m_vDefLightDir.m);
	m_vDefLightDir.Normalize();

	IniFile.Close();

	return true;
}

void CECConfigs::DefaultUserSettings(EC_SYSTEM_SETTING *pss,
		EC_VIDEO_SETTING *pvs, EC_GAME_SETTING *pgs)
{
	if( pss )
	{
		EC_SYSTEM_SETTING ss;
		ss = *pss;
		ss.nLevel = 0;
		ss.nSight = 0;
		ss.nWaterEffect = 0;
		ss.bAdvancedWater = false;
		ss.bLockCamera = false;
		ss.bSimpleTerrain = true;
		ss.nTreeDetail = 0;
		ss.nGrassDetail = 0;
		ss.nCloudDetail = 0;
		ss.nSceneDetail = 0;
		ss.bShadow = false;
		ss.bSimpleShadow = true;
		ss.bMipMapBias = false;
		ss.bFullGlow = false;
		ss.bSpaceWarp = false;
		ss.bSunFlare = false;
		ss.nSoundVol = 100;
		ss.nMusicVol = 80;
		ss.nBackAnimLevel = 0;
		ss.iRndWidth = 800;
		ss.iRndHeight = 600;
		ss.iTexDetail = 2;
		ss.iSoundQuality = 2;
		ss.bFullScreen = true;
		ss.bWideScreen = false;
		ss.bVSync = false;
		ss.iGamma = 100;
		ss.bCloseShake = false;
		*pss = ss;
	}
	if (pvs || pgs)
	{
		m_ConfigsData.DefaultUserSettings(pvs, pgs);
	}
}

//	Load system settings from local disk file
bool CECConfigs::LoadSystemSettings(const char* szFile)
{
	AIniFile IniFile;

	if (!IniFile.Open(szFile))
	{
		a_LogOutput(1, "CECConfigs::LoadSystemSettings, Failed to open file %s", szFile);
		a_LogOutput(1, "we use default settings to let users go in...");
		return true;
	}

	//	Video section
	AString strSect = "Video";

	m_ss.nLevel			= IniFile.GetValueAsInt(strSect, "Level", 0);
	m_ss.nSight			= IniFile.GetValueAsInt(strSect, "Sight", 0);
	m_ss.nWaterEffect	= IniFile.GetValueAsInt(strSect, "WaterEffect", 0);
	m_ss.bSimpleTerrain = IniFile.GetValueAsInt(strSect, "SimpleTerrain", 0) ? true: false;
	m_ss.nTreeDetail	= IniFile.GetValueAsInt(strSect, "TreeDetail", 0);
	m_ss.nGrassDetail	= IniFile.GetValueAsInt(strSect, "GrassDetail", 0);
	m_ss.nCloudDetail	= IniFile.GetValueAsInt(strSect, "CloudDetail", 0);
	m_ss.nSceneDetail	= IniFile.GetValueAsInt(strSect, "SceneDetail", 1);
	m_ss.bShadow		= IniFile.GetValueAsInt(strSect, "Shadow", 0) ? true : false;
	m_ss.bSimpleShadow	= IniFile.GetValueAsInt(strSect, "SimpleShadow", 1) ? true : false;
	m_ss.bMipMapBias	= IniFile.GetValueAsInt(strSect, "MipMapBias", 0) ? true : false;
	m_ss.bFullGlow		= IniFile.GetValueAsInt(strSect, "FullGlow", 0) ? true : false;
	m_ss.bSpaceWarp		= IniFile.GetValueAsInt(strSect, "SpaceWarp", 0) ? true : false;
	m_ss.bSunFlare		= IniFile.GetValueAsInt(strSect, "SunFlare", 0) ? true : false;
	m_ss.bAdvancedWater	= IniFile.GetValueAsInt(strSect, "AdvancedWaterUD", 0) ? true : false;
	m_ss.bLockCamera	= IniFile.GetValueAsInt(strSect, "LockCamera", 0) ? true : false;

	m_ss.iRndWidth		= IniFile.GetValueAsInt(strSect, "RenderWid", 800);
	m_ss.iRndHeight		= IniFile.GetValueAsInt(strSect, "RenderHei", 600);
	m_ss.fRndScale		= IniFile.GetValueAsFloat(strSect, "RenderScale", 0.0f);
	m_ss.iTexDetail		= IniFile.GetValueAsInt(strSect, "TexDetail", 2);
	m_ss.iSoundQuality  = IniFile.GetValueAsInt(strSect, "SoundQuality", 2);
	m_ss.bFullScreen	= IniFile.GetValueAsInt(strSect, "FullScreen", 1) ? true : false;
	m_ss.bWideScreen	= false;//IniFile.GetValueAsInt(strSect, "WideScreen", 0) ? true : false;
	m_ss.bVSync			= IniFile.GetValueAsInt(strSect, "VerticalSync", 0) ? true : false;
	if( IniFile.GetValueAsInt(strSect, "Version", 0) < 1 )
		m_ss.bVSync = false;
	m_ss.iGamma			= IniFile.GetValueAsInt(strSect, "GammaNew", 100);
	m_ss.bCloseShake	= IniFile.GetValueAsInt(strSect, "CloseShake", 0) ? true : false;
	m_ss.fRndScale		= float(m_ss.iRndHeight)/m_ss.iRndWidth;
	
	//	Audio section
	strSect = "Audio";

	m_ss.nSoundVol = IniFile.GetValueAsInt(strSect, "SFX", 100);
	m_ss.nMusicVol = IniFile.GetValueAsInt(strSect, "MUSIC", 80);
	m_ss.nBackAnimLevel = IniFile.GetValueAsInt(strSect, "BackAnimLevel", 0);
	m_ssRestore = m_ss;
	m_bHighest = false;
		
	IniFile.Close();

	if( !m_ss.bFullScreen )
	{
		// adjust window to fit current screen
		int nMaxX = ::GetSystemMetrics(SM_CXSCREEN);
		int nMaxY = ::GetSystemMetrics(SM_CYSCREEN);
		if( m_ss.iRndWidth > nMaxX )
			m_ss.iRndWidth = nMaxX;
		if( m_ss.iRndHeight > nMaxY )
			m_ss.iRndHeight = nMaxY;
	}

	ConvertSightSetting(m_ss.nSight);

	m_fForestDetail = m_ss.nTreeDetail * 0.2f;
	m_fGrassLandDetail = m_ss.nGrassDetail * 0.25f;

#ifndef BACK_VERSION
	m_ss.bDof = true;				// 景深
	m_ss.bBloom = true;				// 光华渲染
	m_ss.bAA = true;				// 抗锯齿
	//BYTE	nFullGlow;			// 自定义颜色 自定义FullGlow参数
	//BYTE	nUnderWater;		// 水下效果（空间扭曲）0	完全关闭空间扭曲效果 1	只在主摄象机位于水面以下时才开启空间扭曲效果 2	在所有环境下开启空间扭曲效果
	//BYTE	r,g,b;		

#endif

	return true;
}

void CECConfigs::MakeSystemSettingsHighest(bool fullGlow)
{
	if(m_bHighest)
		return;

	if(!m_bHighest)
		m_ssRestore = m_ss;

	m_bHighest = true;
	EC_SYSTEM_SETTING	ss = m_ss;
	ss.nSight = 4;
#ifdef BACK_VERSION
	if (g_pGame->GetGameRun()->GetWorld()->GetTerrainWater()->CanRelfect())
#endif
		ss.nWaterEffect = 2;
#ifdef BACK_VERSION
	if (g_pGame->GetShadowRender()->CanDoShadow())
	{
		//				m_SysSetting.bSimpleTerrain = false;
		ss.bShadow = true;
		ss.bSimpleShadow = false;
		ss.nCloudDetail = 4;
	}
#endif
	ss.nTreeDetail = 4;
	ss.nGrassDetail = 4;
	if (g_pGame->GetGameRun()->GetFullGlowRender()->CanDoFullGlow())
	{
		ss.bSunFlare = true;
		ss.bFullGlow = fullGlow;
		ss.bAdvancedWater = true;
	}
	ss.iTexDetail = 0;
	ss.bSimpleTerrain = false;
	ss.bMipMapBias = true;
	SetSystemSettings(ss);
}

void CECConfigs::ResumeSystemSettings()
{
	if (m_bHighest)
	{
		m_bHighest = false;
		// 登陆界面使用，只可能改变尺寸
		m_ssRestore.iRndWidth = m_ss.iRndWidth;
		m_ssRestore.iRndHeight = m_ss.iRndHeight;
		m_ssRestore.fRndScale = m_ss.fRndScale;
		SetSystemSettings(m_ssRestore);
	}
}

float CECConfigs::GetSceneLoadRadius()
{ 
	if (g_pGame->GetGameRun()->GetGameState() == CECGameRun::GS_GAME)
		return m_fSceLoadRadius; 
	else
		return 500.0f;
}

void CECConfigs::SetPlayerVisRadius(float fRadius)
{ 
	m_fCurPVRadius = fRadius; 
	a_Clamp(m_fCurPVRadius, m_fMinPVRadius, m_fSevActiveRad);
}

//	Save system settings to local disk
bool CECConfigs::SaveSystemSettings()
{
	AIniFile IniFile;

	//	Info section
	AString strSect = "Info";

	//	now write the video card information here
	char szInfo[1024];
#ifdef BACK_VERSION
	D3DADAPTER_IDENTIFIER8 idAdapter;
	g_pGame->GetA3DEngine()->GetD3D()->GetAdapterIdentifier(D3DADAPTER_DEFAULT, D3DENUM_NO_WHQL_LEVEL, &idAdapter);
#else
	D3DADAPTER_IDENTIFIER9 idAdapter;
	g_pGame->GetA3DEngine()->GetD3D()->GetAdapterIdentifier(D3DADAPTER_DEFAULT, 0/*D3DENUM_NO_WHQL_LEVEL*/, &idAdapter);
#endif

	IniFile.WriteStringValue(strSect, "card", idAdapter.Description);
	IniFile.WriteStringValue(strSect, "driver", idAdapter.Driver);

	WORD Product = HIWORD(idAdapter.DriverVersion.HighPart);
	WORD Version = LOWORD(idAdapter.DriverVersion.HighPart);
	WORD SubVersion = HIWORD(idAdapter.DriverVersion.LowPart);
	WORD Build = LOWORD(idAdapter.DriverVersion.LowPart);
	sprintf(szInfo, "%d.%d.%04d.%04d", Product, Version, SubVersion, Build);
	IniFile.WriteStringValue(strSect, "version", szInfo);

	sprintf(szInfo, "%08x", idAdapter.VendorId);
	IniFile.WriteStringValue(strSect, "vendor", szInfo);
	sprintf(szInfo, "%08x", idAdapter.DeviceId);
	IniFile.WriteStringValue(strSect, "deviceid", szInfo);
	sprintf(szInfo, "%08x", idAdapter.SubSysId);
	IniFile.WriteStringValue(strSect, "subsysid", szInfo);
	sprintf(szInfo, "{%08X-%04X-%04X-%02X%02X-%02X%02X%02X%02X%02X%02X}", idAdapter.DeviceIdentifier.Data1, idAdapter.DeviceIdentifier.Data2, idAdapter.DeviceIdentifier.Data3, 
		idAdapter.DeviceIdentifier.Data4[0], idAdapter.DeviceIdentifier.Data4[1], idAdapter.DeviceIdentifier.Data4[2], idAdapter.DeviceIdentifier.Data4[3],
		idAdapter.DeviceIdentifier.Data4[4], idAdapter.DeviceIdentifier.Data4[5], idAdapter.DeviceIdentifier.Data4[6], idAdapter.DeviceIdentifier.Data4[7]);
	IniFile.WriteStringValue(strSect, "guid", szInfo);
	
	//	Video section
	strSect = "Video";

	IniFile.WriteIntValue(strSect, "Version", EC_SYSTEM_CONFIG_VERSION);
	IniFile.WriteIntValue(strSect, "Level", m_ss.nLevel);
	IniFile.WriteIntValue(strSect, "Sight", m_ss.nSight);
	IniFile.WriteIntValue(strSect, "WaterEffect", m_ss.nWaterEffect);
	IniFile.WriteIntValue(strSect, "SimpleTerrain", m_ss.bSimpleTerrain);
	IniFile.WriteIntValue(strSect, "TreeDetail", m_ss.nTreeDetail);
	IniFile.WriteIntValue(strSect, "GrassDetail", m_ss.nGrassDetail);
	IniFile.WriteIntValue(strSect, "CloudDetail", m_ss.nCloudDetail);
	IniFile.WriteIntValue(strSect, "SceneDetail", m_ss.nSceneDetail);
	IniFile.WriteIntValue(strSect, "Shadow", m_ss.bShadow);
	IniFile.WriteIntValue(strSect, "SimpleShadow", m_ss.bSimpleShadow);
	IniFile.WriteIntValue(strSect, "MipMapBias", m_ss.bMipMapBias);
	IniFile.WriteIntValue(strSect, "FullGlow", m_ss.bFullGlow);
	IniFile.WriteIntValue(strSect, "SpaceWarp", m_ss.bSpaceWarp);
	IniFile.WriteIntValue(strSect, "SunFlare", m_ss.bSunFlare);
	IniFile.WriteIntValue(strSect, "AdvancedWaterUD", m_ss.bAdvancedWater);
	IniFile.WriteIntValue(strSect, "LockCamera", m_ss.bLockCamera);

	IniFile.WriteIntValue(strSect, "RenderWid", m_ss.iRndWidth);
	IniFile.WriteIntValue(strSect, "RenderHei", m_ss.iRndHeight);
	IniFile.WriteFloatValue(strSect, "RenderScale", m_ss.fRndScale);
	IniFile.WriteIntValue(strSect, "TexDetail", m_ss.iTexDetail);
	IniFile.WriteIntValue(strSect, "SoundQuality", m_ss.iSoundQuality);
	IniFile.WriteIntValue(strSect, "FullScreen", m_ss.bFullScreen);
	IniFile.WriteIntValue(strSect, "WideScreen", m_ss.bWideScreen);
	IniFile.WriteIntValue(strSect, "VerticalSync", m_ss.bVSync);
	IniFile.WriteIntValue(strSect, "GammaNew", m_ss.iGamma);
	IniFile.WriteIntValue(strSect, "CloseShake", m_ss.bCloseShake);

	//	Audio section
	strSect = "Audio";

	IniFile.WriteIntValue(strSect, "SFX", m_ss.nSoundVol);
	IniFile.WriteIntValue(strSect, "MUSIC", m_ss.nMusicVol);
	IniFile.WriteIntValue(strSect, "BackAnimLevel", m_ss.nBackAnimLevel);
	
	IniFile.Save("userdata\\SystemSettings.ini");
	IniFile.Close();

	return true;
}

//	Save user configs (except system settings) to specified buffer
bool CECConfigs::SaveUserConfigData(void* pDataBuf, int* piSize)
{
	int iTotalSize=0;
	BYTE* pDatas = (BYTE*)pDataBuf;

	//	Version
	iTotalSize += sizeof (DWORD);
	if (pDataBuf)
	{
		*((DWORD*)pDatas) = EC_CONFIG_VERSION;
		pDatas += sizeof (DWORD);
	}

	iTotalSize += sizeof (EC_VIDEO_SETTING);
	if (pDataBuf)
	{
		*((EC_VIDEO_SETTING*)pDatas) = m_ConfigsData.m_vs;
		pDatas += sizeof (EC_VIDEO_SETTING);
	}

	iTotalSize += sizeof (EC_GAME_SETTING);
	if (pDataBuf)
	{
		*((EC_GAME_SETTING*)pDatas) = m_ConfigsData.m_gs;
		pDatas += sizeof (EC_GAME_SETTING);
	}

	iTotalSize += sizeof (EC_BLACKLIST_SETTING);
	if (pDataBuf)
	{
		*((EC_BLACKLIST_SETTING*)pDatas) = m_ConfigsData.m_bs;
		pDatas += sizeof (EC_BLACKLIST_SETTING);
	}

	iTotalSize += sizeof (EC_AUTOTEAM_SETTING);
	if (pDataBuf)
	{
		*((EC_AUTOTEAM_SETTING*)pDatas) = m_ConfigsData.m_ts;
		pDatas += sizeof (EC_AUTOTEAM_SETTING);
	}
	
	iTotalSize += sizeof (EC_GAME_CLIENTDATA);
	if (pDataBuf)
	{
// 		AString strUserName = g_pGame->GetGameSession()->GetUserName();
// 		strUserName.MakeLower();
// 		// 保证新建角色播动画
// 		if (strUserName=="system"||strUserName=="system2")
		{
			m_ConfigsData.m_gcd.nCutsceneData[0] = 0;
			m_ConfigsData.m_gcd.nCutsceneData[1] = 0;
			m_ConfigsData.m_gcd.nCutsceneData[2] = 0;
			m_ConfigsData.m_gcd.nCutsceneData[3] = 0;
		}
		*((EC_GAME_CLIENTDATA*)pDatas) = m_ConfigsData.m_gcd;
		pDatas += sizeof (EC_GAME_CLIENTDATA);
	}

	if (piSize)
		*piSize = iTotalSize;

	if (pDataBuf)
		SaveLocalSetting();

	return true;
}


//	Load shortcut configs (except system settings) from specified buffer
bool CECConfigs::LoadUserConfigData(const void* pDataBuf, int iDataSize)
{
	//CECGameUIMan *pGameUI = g_pGame->GetGameRun()->GetUIManager()->GetInGameUIMan();
	CECConfigsDataLoader loader(&m_ConfigsData);
	try
	{
		loader.Load(pDataBuf, iDataSize);

// 		pGameUI->GetDialog("Win_QuickBar8H_2")->Show(m_ConfigsData.m_vs.nQuickbarNum>=2);
// 		pGameUI->GetDialog("Win_QuickBar8H_3")->Show(m_ConfigsData.m_vs.nQuickbarNum>=3);
// 		pGameUI->GetDialog("Win_QuickBar8H_4")->Show(m_ConfigsData.m_vs.nQuickbarNum>=4);
// 		pGameUI->CommonMessage("Win_BodyChangeSkill", "align_skill_bar", 0, 0);
// 		g_pGame->GetGameRun()->GetSecretary()->SetEnable(!m_ConfigsData.m_gs.bHideSecretary);

		//更新老板键
//		((CDlgSettingVideo*)pGameUI->GetDialog("Win_SettingVideo"))->UpdateView();
	}
	catch (CECException& e)
	{
		ASSERT(0);
		a_LogOutput(1, "CECConfigs::LoadUserConfigData, data read error (%d)", e.GetType());
		loader.Default();
	}

	//	Build player and NPC text flags
	BuildTextFlags();

	m_fCurPVRadius = CalcPlayerVisRadius(m_ConfigsData.m_vs.nDistance);
	g_pGame->GetA3DGFXExMan()->SetPriority(m_ConfigsData.m_vs.nEffect);
	
	//	Verify combo-skill ids
	VerifyComboSkills();
	LoadLocalSetting();
	
// 	CDlgChat *pChat = (CDlgChat *)pGameUI->GetDialog("Win_ChatInfo");
// 	pChat->RebuildChatContents();

	return true;
}

void CECConfigs::SetSceneLoadRadius(float fRadius)
{
	m_fSceLoadRadius = fRadius;
	a_ClampFloor(m_fSceLoadRadius, m_fSevActiveRad);
	g_pGame->GetGameRun()->GetWorld()->SetViewRadius(fRadius);
}

void CECConfigs::SetForestDetail(float fDetail)
{
	m_fForestDetail = fDetail;
	a_Clamp(m_fForestDetail, 0.0f, 1.0f);
	g_pGame->GetGameRun()->GetWorld()->GetForest()->SetLODLevel(m_fForestDetail);
}

void CECConfigs::SetGrassLandDetail(float fDetail)
{
	m_fGrassLandDetail = fDetail;
	a_Clamp(m_fGrassLandDetail, 0.0f, 1.0f);
	g_pGame->GetGameRun()->GetWorld()->GetGrassLand()->SetLODLevel(m_fGrassLandDetail);
}

extern volatile bool g_bMultiThreadRenderMode;
extern void SwitchRenderThreadMode(bool bMultiThread);

void CECConfigs::SetSystemSettings(const EC_SYSTEM_SETTING& ss, bool bCallFromWndProc)
{
	CECWorld* pWorld = g_pGame->GetGameRun()->GetWorld();
	if (!pWorld)
		return;

	bool bDisplayModeChanged = 
		m_ss.iRndHeight != ss.iRndHeight || 
		m_ss.iRndWidth != ss.iRndWidth || 
		m_ss.bFullScreen != ss.bFullScreen || 
		// m_ss.bWideScreen != ss.bWideScreen ||
		m_ss.bVSync != ss.bVSync;

	m_bAdjusting = true;

#ifdef BACK_VERSION
	//	Apply water effect
	A3DTerrainWater* pWater = pWorld->GetTerrainWater();
	if (pWater)
	{
		pWater->SetSimpleWaterFlag(!ss.nWaterEffect);
		pWater->SetExpensiveWaterFlag(ss.bAdvancedWater);
	}

	if( g_pGame->GetShadowRender() )
	{
		g_pGame->GetShadowRender()->SetCastShadowFlag(ss.bShadow);
		// we decide whether use triangle man according to shadow cast flag.
		if( g_pGame->GetShadowRender()->GetCastShadowFlag() )
			pWorld->GetOrnamentMan()->LoadTriangleMan(g_pGame->GetGameRun()->GetSafeHostPos(g_pGame->GetViewport()));
		else
			pWorld->GetOrnamentMan()->ReleaseTriangleMan(g_pGame->GetGameRun()->GetSafeHostPos(g_pGame->GetViewport()));
	}

	if( g_pGame->GetGameRun()->GetFullGlowRender() )
	{
		g_pGame->GetGameRun()->GetFullGlowRender()->SetGlowOn(ss.bFullGlow);
		g_pGame->GetGameRun()->GetFullGlowRender()->SetWarpOn(ss.bSpaceWarp);
		g_pGame->GetGameRun()->GetFullGlowRender()->SetFlareOn(ss.bSunFlare);
	}
#endif
	
	if (bDisplayModeChanged)
	{
		DWORD dwExStyle = 0;
		DWORD dwStyles = 0;
		int x, y, w, h;
		A3DFORMAT fmtTarget = A3DFMT_UNKNOWN;

		HWND hDevWnd = g_pGame->GetA3DDevice()->GetDeviceWnd();
		const A3DDevice::DISPLAY_MODE& desktopMode = g_pGame->GetA3DDevice()->GetDesktopDisplayMode();

		if( ss.bFullScreen )
		{
			dwStyles = WS_POPUP;
			dwExStyle = WS_EX_TOPMOST;

			x = 0;
			y = 0;
			w = ss.iRndWidth;
			h = ss.iRndHeight;

			if( g_pGame->GetA3DDevice()->GetTarget32() != A3DFMT_UNKNOWN )
				fmtTarget = g_pGame->GetA3DDevice()->GetTarget32();
			else
				fmtTarget = g_pGame->GetA3DDevice()->GetTarget16();
		}
		else
		{
			dwStyles |= WS_POPUP | WS_CAPTION | WS_MINIMIZEBOX | WS_SYSMENU | WS_THICKFRAME;
			
			RECT rcWnd = {0, 0, ss.iRndWidth, ss.iRndHeight};
			AdjustWindowRectEx(&rcWnd, dwStyles, FALSE, dwExStyle);

			w = rcWnd.right - rcWnd.left;
			h = rcWnd.bottom - rcWnd.top;
			x = (desktopMode.nWidth - w) / 2;
			y = (desktopMode.nHeight - h) / 2;

			if( desktopMode.nBitsPerPixel == 32 && g_pGame->GetA3DDevice()->GetTarget32() != A3DFMT_UNKNOWN )
				fmtTarget = g_pGame->GetA3DDevice()->GetTarget32();
			else
				fmtTarget = g_pGame->GetA3DDevice()->GetTarget16();
		}

		SetWindowLong(hDevWnd, GWL_STYLE, dwStyles);
		SetWindowLong(hDevWnd, GWL_EXSTYLE, dwExStyle);

		if( !ss.bFullScreen )
		{
			if( m_ss.bFullScreen )
			{
				// switch from fullscreen to window mode, we need center the window; and redraw all windows
				SetWindowPos(hDevWnd, HWND_NOTOPMOST, x, y, w, h, SWP_SHOWWINDOW);
			}
			else
			{
				SetWindowPos(hDevWnd, HWND_NOTOPMOST, x, y, w, h, SWP_SHOWWINDOW | SWP_NOMOVE);

				if( !bCallFromWndProc )
					InvalidateRect(NULL, NULL, TRUE);
			}
		}
		else
		{
			SetWindowPos(hDevWnd, HWND_TOPMOST, x, y, w, h, SWP_SHOWWINDOW);
		}

		// we must suspend the loader thread before we can do reset of the device, otherwise we may fail when reset the devcie
		// because unmanaged object are still being added into the world.
		SuspendLoadThread();
		g_bMultiThreadRenderMode = false;
		g_pGame->GetA3DEngine()->SetDisplayMode(ss.iRndWidth, ss.iRndHeight, fmtTarget, !ss.bFullScreen, ss.bVSync, SDM_WIDTH | SDM_HEIGHT | SDM_FORMAT | SDM_WINDOW | SDM_VSYNC);
		StartLoaderThread();
		SwitchRenderThreadMode(!ss.bFullScreen);

#ifdef BACK_VERSION
		CECViewport* pViewport = g_pGame->GetViewport();
#else
		ECViewport* pViewport = g_pGame->GetViewport();
#endif
		pViewport->Move(0, 0, ss.iRndWidth, ss.iRndHeight, false/*ss.bWideScreen*/);

#ifdef BACK_VERSION
		// now reload water resources;
		if (pWater)
		{
			pWater->ReloadWaterResources();
		}
#endif

		// now recreate full screen glow render
		CECFullGlowRender * pFullGlowRender = g_pGame->GetGameRun()->GetFullGlowRender();
		if( pFullGlowRender )
			pFullGlowRender->ReloadResource();

		CECSimpleBloomRender * pSBRender = g_pGame->GetGameRun()->GetSBRender();
		if( pSBRender )
			pSBRender->ReloadResource();

		CECScreenRender * pScreenRender = g_pGame->GetGameRun()->GetScreenRender();
		if( pScreenRender )
			pScreenRender->ReloadResource();
	}

	//	Apply view radius
	if (m_ss.nSight != ss.nSight)
	{
		//	Apply sight radius
		ConvertSightSetting(ss.nSight);
		SetSceneLoadRadius(m_fSceLoadRadius);

		A3DTerrain2* pTerrain = pWorld->GetTerrain();
		pTerrain->SetLODDist(m_fTrnLODDist1, m_fTrnLODDist2);

		CECSceneBlock::SetResLoadDists(ss.nSight + 1);
	}

	if( m_ss.nTreeDetail != ss.nTreeDetail )
	{
		//	Apply forest detail
		SetForestDetail(ss.nTreeDetail * 0.25f);
	}

	if( m_ss.nGrassDetail != ss.nGrassDetail )
	{
		//	Apply grass land detail
		SetGrassLandDetail(ss.nGrassDetail * 0.25f);
	}

	if( m_ss.nCloudDetail != ss.nCloudDetail )
	{
		if( g_pGame->GetGameRun()->GetWorld()->GetCloudManager() )
			g_pGame->GetGameRun()->GetWorld()->GetCloudManager()->SetCloudLevel(ss.nCloudDetail * 25 / 10);
	}

	if( m_ss.bMipMapBias != ss.bMipMapBias || bDisplayModeChanged) 
	{
		float v = ss.bMipMapBias ? -1.0f : 0.0f;
		for(int i=0; i<g_pGame->GetA3DDevice()->GetMaxSimultaneousTextures(); i++)
		{
#ifdef BACK_VERSION
			g_pGame->GetA3DDevice()->SetDeviceTextureStageState(i, D3DTSS_MIPMAPLODBIAS, *(DWORD *)&v);
#else
			g_pGame->GetA3DDevice()->SetSamplerState(i, D3DSAMP_MIPMAPLODBIAS, *(DWORD *)&v);
#endif
		}
	}

	if( m_ss.bVSync != ss.bVSync )
	{
		// after changing the present intervals for fullscreen mode, the gamma level will be restored to defaults (on ATI cards)
		// and the same gamme level will not be accepted by system. so here we first change it to a different values and then
		// set it back.
		g_pGame->GetA3DDevice()->SetGammaLevel(100);
	}
	
	g_pGame->GetA3DDevice()->SetGammaLevel(ss.iGamma);

	if( pWorld->GetTerrainOutline() )
	{
		if( ss.bSimpleTerrain )
		{
			// lowest terrain detail, use terrain outline replace
			pWorld->GetTerrainOutline()->SetReplaceTerrain(true);
		}
		else
		{
			pWorld->GetTerrainOutline()->SetReplaceTerrain(false);
		}
	}

	// now we can change the texture quality at runtime.
	if( m_ss.iTexDetail != ss.iTexDetail )
	{
		g_pA3DConfig->SetTextureQuality((A3DTEXTURE_QUALITY)ss.iTexDetail);
		g_pGame->GetA3DEngine()->GetA3DTextureMan()->ReloadAllTextures(true);
	}

	// now we can change the sound quality at runtime.
	if( m_ss.iSoundQuality != ss.iSoundQuality )
	{
		g_pGame->GetA3DEngine()->GetAMEngine()->GetAMConfig()->SetSoundQuality((AMSOUND_QUALITY) ss.iSoundQuality);
		g_pGame->GetA3DEngine()->GetAMEngine()->GetAMSoundEngine()->GetAMSoundBufferMan()->ReloadAll();
	}

	if( m_ss.bLockCamera != ss.bLockCamera )
		g_pGame->GetGameRun()->GetHostPlayer()->GetCameraCtrl()->FixCamPitch_Set(ss.bLockCamera);

	m_ss = ss;
	m_ss.bWideScreen = false;

	m_bAdjusting = false;
}

void CECConfigs::SetVideoSettings(const EC_VIDEO_SETTING& vs)
{
	m_ConfigsData.m_vs = vs;

	//	Build player and NPC text flags
	BuildTextFlags();

	m_fCurPVRadius = CalcPlayerVisRadius(m_ConfigsData.m_vs.nDistance);

	g_pGame->GetA3DGFXExMan()->SetPriority(m_ConfigsData.m_vs.nEffect);
}

//	Convert sight radius setting
void CECConfigs::ConvertSightSetting(int iSight)
{
	//	Apply sight radius
	switch (iSight)
	{
	case 0:	
	case 1:	
	case 2:	
	case 3:	

		m_fSceLoadRadius = m_fSevActiveRad;
		m_fTrnLODDist1 = 40.0f;
		m_fTrnLODDist2 = 80.0f;
		break;

	case 4:	
		
		m_fSceLoadRadius = 250.0f;
		m_fTrnLODDist1 = 120.0f;
		m_fTrnLODDist2 = 240.0f;
		break;

		/*
	case 2:
	case 3:
	case 4:
		
		m_fSceLoadRadius = 420.0f;
		m_fTrnLODDist1 = 180.0f;
		m_fTrnLODDist2 = 320.0f;
		break;
		*/

	default:		
		ASSERT(0);
		return;
	}
}

//	Build player and NPC text flags
void CECConfigs::BuildTextFlags()
{
	//	Player name flags
	m_dwPlayerText = 0;

	if (m_ConfigsData.m_vs.bPlayerHeadText)
	{
		m_dwPlayerText |= m_ConfigsData.m_vs.bPlayerName ? CECPlayer::RNF_NAME : 0;
		m_dwPlayerText |= m_ConfigsData.m_vs.bPlayerTitle ? CECPlayer::RNF_TITLE : 0;
		m_dwPlayerText |= m_ConfigsData.m_vs.bPlayerFaction ? CECPlayer::RNF_FACTION : 0;
		m_dwPlayerText |= m_ConfigsData.m_vs.bPlayerFamily ? CECPlayer::RNF_FAMILY : 0;
		m_dwPlayerText |= m_ConfigsData.m_vs.bPlayerTalk ? CECPlayer::RNF_WORDS : 0;
		m_dwPlayerText |= m_ConfigsData.m_vs.bPlayerCircle ? CECPlayer::RNF_CIRCLE : 0;
		m_dwPlayerText |= m_ConfigsData.m_vs.bPlayerServer ? CECPlayer::RNF_SERVER : 0;
	}

	if (m_ConfigsData.m_vs.bPlayerShop)
	{
		m_dwPlayerText |= m_ConfigsData.m_vs.bPlayerShop ? CECPlayer::RNF_BUY : 0;
		m_dwPlayerText |= m_ConfigsData.m_vs.bPlayerShop ? CECPlayer::RNF_SELL : 0;
	}

	//	NPC name flags
	m_dwNPCText	= 0;

	m_dwNPCText |= m_ConfigsData.m_vs.bNPCName ? CECNPC::RNF_NPCNAME : 0;
	m_dwNPCText |= m_ConfigsData.m_vs.bMonsterName ? CECNPC::RNF_MONSTERNAME : 0;
	m_dwNPCText |= CECNPC::RNF_WORDS;
}

//	Verify combo-skill ids, this function will replace junior skills with
//	senior skills if condition met.
void CECConfigs::VerifyComboSkills()
{
	CECHostPlayer* pHost = g_pGame->GetGameRun()->GetHostPlayer();

// 	for (int j=0; j < EC_COMBOSKILL_NUM; j++)
// 	{
// 		if (!m_ConfigsData.m_vs.comboSkill[j].nIcon)
// 			continue;
// 
// 		for (int n=0; n < EC_COMBOSKILL_LEN; n++)
// 		{
// 			int idSkill = m_ConfigsData.m_vs.comboSkill[j].idSkill[n];
// 			if (idSkill > 0)
// 			{
// 				CECSkill* pSkill = pHost->GetSkill(idSkill, true);
// 				m_ConfigsData.m_vs.comboSkill[j].idSkill[n] = pSkill ? pSkill->GetSkillID() : 0;
// 			}
// 			else if( idSkill != -1 && idSkill != -2 )
// 				m_ConfigsData.m_vs.comboSkill[j].idSkill[n] = 0;
// 		}
// 	}
}

void CECConfigs::SaveLocalSetting()
{
	char szPath[MAX_PATH];
	sprintf(szPath, "%s\\UserData\\LocalSetting", af_GetBaseDir());
	CreateDirectoryA(szPath, NULL);
	sprintf(szPath, "%s\\UserData\\LocalSetting\\%d.txt", af_GetBaseDir(), g_pGame->GetGameRun()->GetHostPlayer()->GetCharacterID());
	AFile afile;
	if (afile.Open(szPath, AFILE_OPENEXIST))
	{
		afile.Close();
		DeleteFileA(szPath);
	}
	
	AIniFile IniFile;
	AString strSect = "AutoSell";
	IniFile.WriteIntValue(strSect, "bRarity",			m_ls.auto_sell.bRarity);
	IniFile.WriteIntValue(strSect, "nRarityLevel",		m_ls.auto_sell.nRarityLevel);
	IniFile.WriteIntValue(strSect, "bMaterial",			m_ls.auto_sell.bMaterial);
	IniFile.WriteIntValue(strSect, "nMaterialLevel",	m_ls.auto_sell.nMaterialLevel);
	IniFile.WriteIntValue(strSect, "bAffuse",			m_ls.auto_sell.bAffuse);
	IniFile.WriteIntValue(strSect, "bArrangeAfterSell", m_ls.auto_sell.bArrangeAfterSell);
	strSect = "LocalChannel";
	int iChannel = 0;
	for (int i=0; i<32; i++)
	{
		if (m_ls.local_chat.bChannel[i])
			iChannel |= (1<<i);
	}
	IniFile.WriteIntValue(strSect, "iChannel",			iChannel);
	IniFile.WriteIntValue(strSect, "bFilterLocalChat", m_ls.local_chat.bFilterLocalChat);
	IniFile.WriteIntValue(strSect, "nLocalChatLevel", m_ls.local_chat.nLocalChatLevel);
	IniFile.WriteIntValue(strSect, "bFilterWhisper", m_ls.local_chat.bFilterWhisper);
	IniFile.WriteIntValue(strSect, "nWhisperLevel", m_ls.local_chat.nWhisperLevel);
	sprintf(szPath, "%s\\UserData\\LocalSetting\\%d.ini", af_GetBaseDir(), g_pGame->GetGameRun()->GetHostPlayer()->GetCharacterID());
	
	strSect = "Others";
	IniFile.WriteIntValue(strSect, "bPopDreamHeyangHint", m_ls.bStopPopDreamHeyangHint ? 1 : 0);

	IniFile.Save(szPath);
	IniFile.Close();
}

void CECConfigs::LoadLocalSetting()
{
	char szPath[MAX_PATH];
	sprintf(szPath, "%s\\UserData\\LocalSetting\\%d.txt", af_GetBaseDir(), g_pGame->GetGameRun()->GetHostPlayer()->GetCharacterID());
	AFile afile;
	DWORD dwRead;
	memset(&m_ls, 0, sizeof (m_ls));
	if (afile.Open(szPath, AFILE_OPENEXIST|AFILE_BINARY|AFILE_NOHEAD))
	{
		afile.Read(&m_ls, sizeof(EC_LOCAL_SETTING), &dwRead);
		afile.Close();
		return;
	}
	sprintf(szPath, "%s\\UserData\\LocalSetting\\%d.ini", af_GetBaseDir(), g_pGame->GetGameRun()->GetHostPlayer()->GetCharacterID());
	AIniFile IniFile;
	
	if (!IniFile.Open(szPath))
	{
		a_LogOutput(1, "CECConfigs::LoadLocalSetting, Failed to open file %s", szPath);
	}
	else
	{
		AString strSect = "AutoSell";
		m_ls.auto_sell.bRarity				= IniFile.GetValueAsInt(strSect, "bRarity", 0)>0;
		m_ls.auto_sell.nRarityLevel			= IniFile.GetValueAsInt(strSect, "nRarityLevel", 0);
		m_ls.auto_sell.bMaterial			= IniFile.GetValueAsInt(strSect, "bMaterial", 0)>0;
		m_ls.auto_sell.nMaterialLevel		= IniFile.GetValueAsInt(strSect, "nMaterialLevel", 0);
		m_ls.auto_sell.bAffuse				= IniFile.GetValueAsInt(strSect, "bAffuse", 0)>0;
		m_ls.auto_sell.bArrangeAfterSell	= IniFile.GetValueAsInt(strSect, "bArrangeAfterSell", 0)>0;
		strSect = "LocalChannel";
		int iChannel						= IniFile.GetValueAsInt(strSect, "iChannel", 0);
		for (int i=0; i<32; i++)
		{
			if (iChannel & (1<<i))
				m_ls.local_chat.bChannel[i] = true;
			else
				m_ls.local_chat.bChannel[i] = false;
		}
		m_ls.local_chat.bFilterLocalChat	= IniFile.GetValueAsInt(strSect, "bFilterLocalChat", 0)>0;
		m_ls.local_chat.nLocalChatLevel		= IniFile.GetValueAsInt(strSect, "nLocalChatLevel", 0);
		m_ls.local_chat.bFilterWhisper		= IniFile.GetValueAsInt(strSect, "bFilterWhisper", 0)>0;
		m_ls.local_chat.nWhisperLevel		= IniFile.GetValueAsInt(strSect, "nWhisperLevel", 0);
		
		strSect = "Others";
		m_ls.bStopPopDreamHeyangHint		= IniFile.GetValueAsInt(strSect, "bPopDreamHeyangHint", 0) > 0;
		IniFile.Close();
	}
}

#ifndef BACK_VERSION
void CECConfigs::AdjustPostEffectGlobal(A3DPOST_EFFECT_PARAM& effect)
{
	effect.dwPostEffectMask |= A3DPOST_EFFECT_SunMoon;

	if (m_ss.bSunFlare)
		effect.dwPostEffectMask |=A3DPOST_EFFECT_Flare;
	else
		effect.dwPostEffectMask  &= ~A3DPOST_EFFECT_Flare;
}

void CECConfigs::AdjustPostEffectBySystemSetting(A3DPOST_EFFECT_PARAM& effect)
{
	if (m_ss.bDof)
		effect.dwPostEffectMask |=A3DPOST_EFFECT_Dof;
	else
		effect.dwPostEffectMask  &= ~A3DPOST_EFFECT_Dof;

	if (m_ss.bBloom)
		effect.dwPostEffectMask |=A3DPOST_EFFECT_Bloom;
	else
		effect.dwPostEffectMask  &= ~A3DPOST_EFFECT_Bloom;

	if (m_ss.nFullGlow)
	{
		effect.dwPostEffectMask |=A3DPOST_EFFECT_FullGlow;
		if (m_ss.nFullGlow==2)
		{
			effect.fullGlowParam.vGlowColor = A3DVECTOR4(m_ss.r/255.f, m_ss.g/255.f,  m_ss.b/255.f,0.45f);
		}
	}
	else
		effect.dwPostEffectMask  &= ~A3DPOST_EFFECT_FullGlow;

	if (m_ss.bAA)
		effect.dwPostEffectMask |=A3DPOST_EFFECT_AA;
	else
		effect.dwPostEffectMask  &= ~A3DPOST_EFFECT_AA;

	if (m_ss.nUnderWater==0)
	{
		effect.dwPostEffectMask  &= ~A3DPOST_EFFECT_Warp;
	}
	else
	{
		//effect.dwPostEffectMask |=A3DPOST_EFFECT_Warp;// 由外层逻辑控制
	}
}
#endif