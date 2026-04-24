/*
 * FILE: EC_Configs.h
 *
 * DESCRIPTION: 
 *
 * CREATED BY: Duyuxin, 2004/8/27
 *
 * HISTORY: 
 *
 * Copyright (c) 2004 Archosaur Studio, All Rights Reserved.
 */

#pragma once

#include "AString.h"
#include "A3DVector.h"
//#include "EC_GameUIMan.h"
#include "EC_ConfigsData.h"

#ifndef BACK_VERSION
#include "ElementRenderParam.h"
#endif

///////////////////////////////////////////////////////////////////////////
//	
//	Define and Macro
//	
///////////////////////////////////////////////////////////////////////////

#define EC_AUTOREPLY_LEN		CECConfigsCur::EC_AUTOREPLY_LEN
#define EC_BLACKLIST_LEN		CECConfigsCur::EC_BLACKLIST_LEN
#define EC_AUTOTEAM_LEN			CECConfigsCur::EC_AUTOTEAM_LEN
#define EC_USERCHANNEL_NUM		CECConfigsCur::EC_USERCHANNEL_NUM
#define EC_COMBOSKILL_NUM		CECConfigsCur::EC_COMBOSKILL_NUM
#define EC_COMBOSKILL_LEN		CECConfigsCur::EC_COMBOSKILL_LEN
#define EC_COMBOSKILL_ICONNUM	CECConfigsCur::EC_COMBOSKILL_ICONNUM


#define EC_COMBOSKILL			CECConfigsCur::EC_COMBOSKILL
#define EC_VIDEO_SETTING		CECConfigsCur::EC_VIDEO_SETTING
#define EC_GAME_SETTING			CECConfigsCur::EC_GAME_SETTING
#define EC_BLACKLIST_SETTING	CECConfigsCur::EC_BLACKLIST_SETTING
#define EC_AUTOTEAM_SETTING		CECConfigsCur::EC_AUTOTEAM_SETTING
#define EC_GAME_CLIENTDATA		CECConfigsCur::EC_GAME_CLIENTDATA

///////////////////////////////////////////////////////////////////////////
//	
//	Types and Global variables
//	
///////////////////////////////////////////////////////////////////////////

#pragma pack(1)

struct EC_SYSTEM_SETTING
{
	BYTE	nLevel;
	BYTE	nSight;
	BYTE	nWaterEffect;
	bool	bSimpleTerrain;
	BYTE	nTreeDetail;
	BYTE	nGrassDetail;
	BYTE	nCloudDetail;
	BYTE	nSceneDetail;
	bool	bShadow;
	bool	bSimpleShadow;
	bool	bMipMapBias;
	bool	bFullGlow;
	bool	bSpaceWarp;
	bool	bSunFlare;
	bool	bAdvancedWater;
	bool	bLockCamera;
	BYTE	nSoundVol;
	BYTE	nMusicVol;
	BYTE	nBackAnimLevel;  // 0 ~ 4
	int		iGamma;
	
	int		iRndWidth;
	int		iRndHeight;
	float	fRndScale;
	int		iTexDetail;
	int		iSoundQuality;
	bool	bFullScreen;
	bool	bWideScreen;
	
	bool	bVSync;
	bool	bCloseShake;
	
	bool	bEnableMusic;
	bool	bEnableSound;

#ifndef BACK_VERSION
	bool	bDof;				// 景深
	bool	bBloom;				// 光华渲染
	bool	bAA;				// 抗锯齿
	BYTE	nFullGlow;			// 自定义颜色 自定义FullGlow参数
	BYTE	nUnderWater;		// 水下效果（空间扭曲）0	完全关闭空间扭曲效果 1	只在主摄象机位于水面以下时才开启空间扭曲效果 2	在所有环境下开启空间扭曲效果
	BYTE	r,g,b;		

#endif
};

struct EC_LOCAL_SETTING
{
	struct AutoSell
	{
		bool bRarity;
		int  nRarityLevel;
		bool bMaterial;
		int  nMaterialLevel;
		bool bAffuse;
		bool bArrangeAfterSell;
	} auto_sell;

	struct LocalChat
	{
		bool bChannel[32];
		bool bFilterLocalChat;
		int  nLocalChatLevel;
		bool bFilterWhisper;
		int  nWhisperLevel;
	} local_chat;
	
	bool bStopPopDreamHeyangHint;
};

#pragma pack()

///////////////////////////////////////////////////////////////////////////
//	
//	Declare of Global functions
//	
///////////////////////////////////////////////////////////////////////////


///////////////////////////////////////////////////////////////////////////
//	
//	Class CECConfigs
//	
///////////////////////////////////////////////////////////////////////////

class CECConfigs
{
public:		//	Types

public:		//	Constructor and Destructor

	CECConfigs();
	virtual ~CECConfigs();
	

public:		//	Attributes

	//	Default environments
	float		m_fDefFogStart;
	float		m_fDefFogEnd;
	float		m_fDefFogDensity;
	DWORD		m_dwDefFogColor;
	AString		m_strDefSkyFile0;
	AString		m_strDefSkyFile1;
	AString		m_strDefSkyFile2;
	DWORD		m_dwDefLightDiff;
	DWORD		m_dwDefLightSpec;
	A3DVECTOR3	m_vDefLightDir;
	DWORD		m_dwDefAmbient;

	//	Test code, temporary parameters
	bool		m_bShowForest;		//	true, render forest, false, no render
	bool		m_bShowGrassLand;	//	true, render grass land, false, no render

public:		//	Operations

	//	Initialize object
	bool Init(const char* szCfgFile, const char* szSSFile);

	//	Save system settings to local disk
	bool SaveSystemSettings();
	//	Save user configs (except system settings) to specified buffer
	bool SaveUserConfigData(void* pDataBuf, int* piSize);
	//	Load shortcut configs (except system settings) from specified buffer
	bool LoadUserConfigData(const void* pDataBuf, int iDataSize);
	//	Restore user settings to default value
	
	//	Configs user cannot change ...
	const char* GetLoginUIDcf() { return m_strLoginDcf; }
	const char* GetInGameUIDcf() { return m_strInGameDcf; }
	const char* GetLoadingUIDcf() { return m_strLoadingDcf; }
	const char* GetCountryRegion() {return m_strCountryRegion;}
	const char* GetWindowTitle() {return m_strWindowTitle;}
	const char* GetWindowReplayTitle() {return m_strWindowReplayTitle;}

	int GetLanCodePage() { return m_iCodePage; }
	float GetDefFogStart() { return m_fDefFogStart; }
	float GetDefFogEnd() { return m_fDefFogEnd; }
	float GetDefFogDensity() { return m_fDefFogDensity; }
	DWORD GetDefFogColor() { return m_dwDefFogColor; }
	DWORD GetDefAmbient() { return m_dwDefAmbient; }
	DWORD GetDefLightDiff() { return m_dwDefLightDiff; }
	A3DVECTOR3 GetDefLightDir() { return m_vDefLightDir; }
	DWORD GetDefLightSpec() { return m_dwDefLightSpec; }
	float GetSevActiveRadius() { return m_fSevActiveRad; }
	void SetSceneLoadRadius(float fRadius);
	float GetSceneLoadRadius();
	void SetPlayerVisRadius(float fRadius);
	float GetPlayerVisRadius() { return m_fCurPVRadius; } 
	float CalcPlayerVisRadius(int iPercent) { return m_fMinPVRadius + (m_fSevActiveRad - m_fMinPVRadius) * iPercent / 100.0f; }
	void SetForestDetail(float fDetail);
	void SetGrassLandDetail(float fDetail);
	float GetForestDetail() { return m_fForestDetail; }
	float GetGrassLandDetail() { return m_fGrassLandDetail; }
	float GetTerrainLODDist1() { return m_fTrnLODDist1; } 
	float GetTerrainLODDist2() { return m_fTrnLODDist2; } 
	DWORD GetPlayerTextFlags() { return m_dwPlayerText; }
	DWORD GetNPCTextFlags() { return m_dwNPCText; }

	//	DEBUG only settings ...
	bool HasConsole() { return m_bConsole; }
	void SetRTDebugLevel(int iLevel) { m_iRTDebug = iLevel; }
	int GetRTDebugLevel() { return m_iRTDebug; }
	void ShowNPCID(bool bShow) { m_bShowNPCID = bShow; }
	bool IsItemIDShown() { return m_bShowItemId; }
	void ShowItemID(bool bShow) { m_bShowItemId = bShow; }
	bool IsNPCIDShown() { return m_bShowNPCID; }
	void SetHostRunSpeed(float fSpeed) { m_fRunSpeed = fSpeed; }
	float GetHostRunSpeed() { return m_fRunSpeed; }
	float GetHostWalkSpeed() { return m_fWalkSpeed; }
	bool GetTestDistFlag() { return m_bTestDist; }
	void SetTestDistFlag(bool bFlag) { m_bTestDist = bFlag; }
	bool GetShowPosFlag() { return m_bShowPos; }
	void SetShowPosFlag(bool bFlag) { m_bShowPos = bFlag; }
	bool GetShowForestFlag() { return m_bShowForest; }
	void SetShowForestFlag(bool bFlag) { m_bShowForest = bFlag; }
	bool GetShowGameStatFlag() { return m_bGameStat; }
	void SetShowGameStatFlag(bool bFlag) { m_bGameStat = bFlag; }
	bool GetShowIDFlag() { return m_bShowID; }
	void SetShowIDFlag(bool bFlag) { m_bShowID = bFlag; }
	bool GetSkipFrameFlag() { return m_bSkipFrame; }
	void SetSkipFrameFlag(bool bTrue) { m_bSkipFrame = bTrue; }
	bool GetModelUpdateFlag() { return m_bModelUpt; }
	void SetModelUpdateFlag(bool bTrue) { m_bModelUpt = bTrue; }
	bool Limit30Words() { return m_bNormalChat30Words; }

	bool IsAdjusting() { return m_bAdjusting; }
	bool IsSnsPhotoOpen() { return m_bSnsPhoto; }
	bool IsSpreadOpen() { return m_bSpread; }
	bool IsAutoHelperEnable() { return m_bAutoHelperEnable; }
	
	void SetCamDis(float fCamDis) { m_fCamDis = fCamDis; }
	float GetCamDis() { return m_fCamDis; }
	void SetPlayerDeg(int deg)	{m_iPlayerDeg = deg; }
	int  GetPlayerDeg()			{return m_iPlayerDeg; }
	
	void DefaultUserSettings(EC_SYSTEM_SETTING *pss,
		EC_VIDEO_SETTING *pvs, EC_GAME_SETTING *pgs);
	//	User settings ...
	const EC_SYSTEM_SETTING& GetSystemSettings() { return m_ss; }
	EC_SYSTEM_SETTING * GetSystemSettingsPtr() { return &m_ss; }
	void SetSystemSettings(const EC_SYSTEM_SETTING& ss, bool bCallFromWndProc=false);
	const EC_VIDEO_SETTING& GetVideoSettings() { return m_ConfigsData.m_vs; }
	const EC_GAME_SETTING& GetGameSettings() { return m_ConfigsData.m_gs; }
	const EC_BLACKLIST_SETTING& GetBlackListSettings() { return m_ConfigsData.m_bs; }
	const EC_GAME_CLIENTDATA& GetGameClientData() { return m_ConfigsData.m_gcd; }
	const EC_AUTOTEAM_SETTING& GetAutoTeamSettings() { return m_ConfigsData.m_ts; }
	const EC_LOCAL_SETTING& GetLocalSettings() { return m_ls; }
	void SetVideoSettings(const EC_VIDEO_SETTING& vs);
	void SetGameSettings(const EC_GAME_SETTING& gs) { m_ConfigsData.m_gs = gs; }
	void SetBlackListSettings(const EC_BLACKLIST_SETTING& bs) { m_ConfigsData.m_bs = bs;}
	void SetAutoTeamSettings(const EC_AUTOTEAM_SETTING& ts) { m_ConfigsData.m_ts = ts; }
	void SetGameClientData(const EC_GAME_CLIENTDATA& gcd) { m_ConfigsData.m_gcd = gcd; }
	void SetLocalSettings(const EC_LOCAL_SETTING& ls) { m_ls = ls;}

#ifndef BACK_VERSION
	A3DPOST_EFFECT_PARAM* GetPostEffectParamPtr() { return &m_effect; }
	const A3DPOST_EFFECT_PARAM& GetPostEffectParam() const { return m_effect; }
	void AdjustPostEffectBySystemSetting(A3DPOST_EFFECT_PARAM& effect);// 不影响动画
	void AdjustPostEffectGlobal(A3DPOST_EFFECT_PARAM& effect);// 影响动画等
	bool IsRenderPostProcess()						{ return m_bRenderPostProcess;}
	void SetRenderPostProcess(bool bRender)			{ m_bRenderPostProcess = bRender;}

	void SetGlobalFog(bool b)	{ m_bGlobalFog=b; }
	bool GetGlobalFog() { return m_bGlobalFog; }
#endif

	void SaveLocalSetting();
	void LoadLocalSetting();	
	
	//	Load system settings from local disk file
	bool LoadSystemSettings(const char* szFile);
	void MakeSystemSettingsHighest(bool fullGlow=true);
	void ResumeSystemSettings();

protected:	//	Attributes

	//	Configs user cannot change ...
	AString		m_strLoginDcf;		//	Login UI dcf file
	AString		m_strInGameDcf;		//	In game UI dcf file
	AString		m_strLoadingDcf;	//	Loading UI dcf file
	AString     m_strCountryRegion;	// country or region
	AString     m_strWindowTitle;	// window title
	AString     m_strWindowReplayTitle;  // window replay title
	AString		m_strRegKey;		//	Registry Key

	int			m_iCodePage;		//	Language code page
	float		m_fSevActiveRad;	//	Server active radius
	float		m_fSceLoadRadius;	//	Current scene loading radius
	float		m_fMinPVRadius;		//	Minimum player visible radius
	float		m_fCurPVRadius;		//	Current player visible radius
	float		m_fForestDetail;	//	Forest detail (0-1)
	float		m_fGrassLandDetail;	//	Grass land detail (0-1)
	float		m_fTrnLODDist1;		//	Terrain LOD distance 1
	float		m_fTrnLODDist2;		//	Terrain LOD distance 2
	DWORD		m_dwPlayerText;		//	Player text flags
	DWORD		m_dwNPCText;		//	NPC text flags
	int			m_iRTDebug;			//	Run-time debug message level
	bool		m_bNormalChat30Words;

	//	DEBUG only settings ...
	bool		m_bConsole;			//	true, using console
	bool		m_bShowNPCID;		//	Show NPC's ID on it's head
	bool		m_bShowItemId;		//	Show item id in package
	float		m_fRunSpeed;		//	Host run speed.
	float		m_fWalkSpeed;		//	Host walk speed
	bool		m_bTestDist;		//	Test distance
	bool		m_bShowPos;			//	Show position
	bool		m_bGameStat;		//	Show game statistic
	bool		m_bShowID;			//	Show player ID
	bool		m_bSkipFrame;		//	Skip frame flag
	bool		m_bModelUpt;		//	Model update optimization flag

	bool		m_bAdjusting;		//	flag indicates the system setting is being adjusted
	bool		m_bSnsPhoto;
	bool		m_bSpread;
	bool		m_bAutoHelperEnable;//  auto helper is enabled?
	float		m_fCamDis;			//  跨服动画里面的摄像机距离
	int			m_iPlayerDeg;		//  跨服动画里面的模型朝向的角度

	//	User settings ...
	EC_SYSTEM_SETTING		m_ss;		//	System settings
	EC_SYSTEM_SETTING		m_ssRestore;	//	System settings to restore
	bool					m_bHighest;		// 标志当前是否临时将系统设置调为最高
	CECConfigsCur			m_ConfigsData;

	//EC_SYSTEM_SETTING		m_ss;		//	System settings
	//EC_VIDEO_SETTING		m_vs;		//	Video settings
	//EC_GAME_SETTING		m_gs;		//	Game settings
	//EC_GAME_CLIENTDATA	m_gcd;		//	Blacklist settings
	//EC_BLACKLIST_SETTING	m_bs;		//	Blacklist settings
	//EC_AUTOTEAM_SETTING	m_ts;		//	Auto Team Arrange setting
	EC_LOCAL_SETTING		m_ls;

	bool			m_bRenderPostProcess;
#ifndef BACK_VERSION
	A3DPOST_EFFECT_PARAM	m_effect;
	bool			m_bGlobalFog;
#endif

protected:	//	Operations

	//	Load configs from file
	bool LoadConfigs(const char* szFile);
	
	//	Convert sight radius setting
	void ConvertSightSetting(int iSight);
	//	Build player and NPC text flags
	void BuildTextFlags();
	//	Verify combo-skill ids
	void VerifyComboSkills();
};

///////////////////////////////////////////////////////////////////////////
//	
//	Inline functions
//	
///////////////////////////////////////////////////////////////////////////


