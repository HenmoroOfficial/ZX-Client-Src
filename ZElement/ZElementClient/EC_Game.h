/*
 * FILE: GL_Game.h
 *
 * DESCRIPTION: 
 *
 * CREATED BY: Duyuxin, 2003/12/16
 *
 * HISTORY: 
 *
 * Copyright (c) 2003 Archosaur Studio, All Rights Reserved.
 */

#pragma once

#include <windows.h>
#include "A3DTypes.h"
#include "AArray.h"
#include "EC_StringTab.h"
#include "hashtab.h"
#include "hashmap.h"
#include "vector.h"

#include "AList2.h"

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

class A3DEngine;
class A3DDevice;
class A3DLight;
class A3DSkinModel;
class A3DSkin;
class A3DMuscleData;
class AMSoundBufferMan;
class AWString;

class CELBuilding;
class CECGameRun;
class CECGameSession;
class CECRTDebug;
class A3DGFXEx;
class A3DGFXExMan;
class CECModelMan;
class A3DCursor;
class A3DFont;
class CECPortraitRender;
class CECGFXCaster;
class CECImageRes;
class CECConfigs;
class ATaskTemplMan;
class CELBackMusic;
class CECSoundCache;
class CECModel;
class CECFactionMan;
class CECFunGamePlaza;
class A3DLitModelRender;

class elementdataman;
class itemdataman;
class DataPathMan;

class CConfigFromLua;
class CLuaTriggerTask;
class CECCircleMan;

#ifdef BACK_VERSION
class CECViewport;
class A3DSkinRender;
class CECShadowRender;
#else
class ECViewport;
#define CECViewport ECViewport
class A3DSkinRenderBase;
#endif

namespace GNET
{
	class Privilege;
}

namespace HOMETOWN
{
	class HometownTemplate;
}
///////////////////////////////////////////////////////////////////////////
//	
//	Declare of Global functions
//	
///////////////////////////////////////////////////////////////////////////


///////////////////////////////////////////////////////////////////////////
//	
//	Class CECGame
//	
///////////////////////////////////////////////////////////////////////////

class CECGame
{
public:	//	Types

	enum
	{
		MAXNUM_LIGHT = 8,	//	Maximum number of A3D Light
	};

	enum
	{
		MAXNUM_CUTSCENE = 128,	//	Maximum number of cutscene
	};


	struct GAMEINIT
	{
		HINSTANCE	hInstance;
		HWND		hWnd;
		bool		bFullscreen;
		int			iRenderWid;
		int			iRenderHei;
		char		szIP[256];
		int			iPort;
		int			iType;// -1-无 0-双线 1-电信 2-网通
		char		szServerName[256];
		int			iServerID;
		bool		bVerCheck;
		int 		iRaceTest;
		char		szArea[256];
		char		szUserName[32];
		char		szToken[32];
		char		szPassword[128];//Modified 2012-08-10.
	};
	
	struct SERVERINFO
	{
		abase::vector<AString> vecAddress;
		abase::vector<int>	vecPorts;
		char	szServerName[256];
		int		iServerID;
		char	szServerGroup[256];	// 2012-8-22 add zy 添加服务器名称
	};

	struct DUNGEONSTEPINFO
	{
		int				iDungeonID;
		AString			szSubWndName;
		int				iStartMsgID;
		int				iStepNum;
		abase::vector<DWORD>*	aStepMonsterIDs;
	};

	struct SIZEMOVEPARAM
	{
		int  iOldWidth;
		int  iOldHeight;
        int  iNewWidth;
		int  iNewHeight;
		bool bSizeMoving;
		SIZEMOVEPARAM(){memset(this,0,sizeof(SIZEMOVEPARAM));}
	};

	typedef abase::hashtab<BYTE, int, abase::_hash_function> ItemExtPropTable;
	typedef abase::hashtab<int, int, abase::_hash_function> SuiteEquipTable;
	typedef abase::hashtab<int, int, abase::_hash_function> TitlePropTable;
	typedef abase::hashtab<BYTE, int, abase::_hash_function> ItemColTable;
	typedef abase::hash_map<AString, AString> IPMap;
	typedef abase::hash_map<int, bool> ValuableItemMap;
	typedef abase::hash_map<int, bool> PocketableItemMap;
	typedef abase::hash_map<int, int>  AutoPickItemMap;
	typedef abase::hash_map<AString, AString> DebugCmdMap;
public:	//	Constructor and Destructor

	CECGame();
	virtual ~CECGame();

public:		//	Attributes

public:		//	Operations

	//	Initialize game
	bool Init(const GAMEINIT& GameInit);
	//	Release game
	void Release();
	//	Reset game, release all resources
	bool Reset();

	//	Run game
	int RunInRenderThread();
	bool RunInMainThread();
	bool OnceRun();

	//	Window message handler
	bool WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam);

	//	Output a run-time debug string
	void RuntimeDebugInfo(DWORD dwCol, const ACHAR* szMsg, ...);

	//	Dispatch windows message
	bool DispatchWindowsMessage();
	//	Add windows message
	void AddWindowsMessage(MSG& msg);
	
	//	Resources control
	bool LoadLoginRes();
	void ReleaseLoginRes();
	bool LoadInGameRes();
	void ReleaseInGameRes();

	//	Create a A3D Light object
	A3DLight* CreateA3DLight(const A3DLIGHTPARAM* pParams);
	//	Release Light object
	void ReleaseA3DLight(A3DLight* pLight);
	//	Load A3D skin model
	A3DSkinModel* LoadA3DSkinModel(const char* szFile, int iSkinFlag=0);
	//	Release A3D skin model
	void ReleaseA3DSkinModel(A3DSkinModel* pModel);
	//	Load A3D skin
	A3DSkin* LoadA3DSkin(const char* szFile, bool bUnique=true);
	//	Release A3D skin
	void ReleaseA3DSkin(A3DSkin* pSkin, bool bUnique=true);
	//	Load a building model from file
	CELBuilding* LoadBuilding(const char* szFile);
	//	Release a building model
	void ReleaseBuilding(CELBuilding* pBuilding);
	//	Show or hide cursor
	void ShowCursor(bool bShow);
	//	Change current cursor
	int ChangeCursor(int iCursor);
	//	Get current cursor
	int GetCurCursor() { return m_iCurCursor; }
	//	Get font object
	A3DFont* GetFont(int iIndex);
	//	Get item's extend description string
	const wchar_t* GetItemExtDesc(int tid);
	//	Get buff description string
	const wchar_t* GetBuffDesc(int id);
	//	Reset privilege
	bool ResetPrivilege(void* pBuf, int iDataSize);
	//	Render high-light model
#ifdef BACK_VERSION
	bool RenderHighLightModel(CECViewport* pView, CECModel* pModel);
	bool RenderHighLightModel(CECViewport* pView, A3DSkinModel* pModel);
#else
	bool RenderHighLightModel(ECViewport* pView, CECModel* pModel, bool bRenderSkinModel=true);
	bool RenderHighLightModel(ECViewport* pView, A3DSkinModel* pModel);
#endif
	//	Get element data path through path id
	const char* GetDataPath(DWORD id);
	//	Get dynamic object path
	const char* GetDynObjectPath(DWORD dwDynObjID);

	//	Get interfaces
	A3DEngine* GetA3DEngine() { return m_pA3DEngine; }
	A3DDevice* GetA3DDevice() { return m_pA3DDevice; }
	A3DLight* GetDirLight() { return m_pDirLight; }
	A3DLight* GetDynamicLight() { return m_pDynamicLight; }
	CECViewport* GetViewport() { return m_pViewport; }
	CECConfigs* GetConfigs() { return m_pConfigs; }
	CECGameRun* GetGameRun() { return m_pGameRun; }
	CECGameSession* GetGameSession() { return m_pGameSession; }
	CECRTDebug* GetRTDebug() { return m_pRTDebug; }
	A3DGFXExMan* GetA3DGFXExMan() { return m_pA3DGFXExMan; }
	CECModelMan* GetECModelMan() { return m_pECModelMan; }
#ifdef BACK_VERSION
	CECShadowRender* GetShadowRender() { return m_pShadowRender; }
#endif
	CECPortraitRender* GetPortraitRender() { return m_pPortraitRender; }
	CECStringTab* GetFixedMsgTab() { return &m_FixedMsgs; }
	CECStringTab* GetPetTalkMsgs() { return &m_PetTalkMsgs; }
	CECStringTab* GetItemDesc() { return &m_ItemDesc; }
	CECStringTab* GetSkillDesc() { return &m_SkillDesc; }
	CECStringTab* GetBuffDesc() { return &m_BuffDesc; }
	CECStringTab* GetAddOnDesc() { return &m_AddOnDesc; }
	CECStringTab* GetHomeTownDesc() {return &m_HomeTownDesc; }
	CECStringTab* GetHomeTownIconPath() { return &m_HomeTownIconPath; } 
	CECGFXCaster* GetGFXCaster() { return m_pGFXCaster; }
	elementdataman * GetElementDataMan() { return m_pElementDataMan; }
	HOMETOWN::HometownTemplate* GetHomeTownDataMan() { return m_pHomeTownDataMan; }
	DataPathMan* GetDataPathMan() { return m_pDataPathMan; }
	itemdataman * GetItemDataMan() { return m_pItemDataMan; }
	ATaskTemplMan* GetTaskTemplateMan() { return m_pTaskMan; }
	CECImageRes* GetImageRes() { return m_pImageRes; }
#ifdef BACK_VERSION
	A3DSkinRender* GetSkinRender1() { return m_pSkinRender1; }
	A3DSkinRender* GetSkinRender2() { return m_pSkinRender2; }
	A3DLitModelRender* GetLitModelRender1() { return m_pLitModelRender1; }
	A3DSkinRender* GetSkinRender1_W() { return m_pSkinRender1_W; }
#else
	A3DSkinRenderBase* GetSkinRender1() { return m_pSkinRender1; }
	A3DSkinRenderBase* GetSkinRender2() { return m_pSkinRender2; }
	A3DSkinRenderBase* GetSkinRenderShadow() { return m_pSkinRenderShadow; }
	A3DSkinRenderBase* GetSkinRender1_W() { return m_pSkinRender1_W; }
	//A3DLitModelRender* GetLitModelRender1() { return m_pLitModelRender1; }
#endif

	CELBackMusic * GetBackMusic() { return m_pBackMusic; }
	CECSoundCache* GetSoundCache() { return m_pSoundCache; }
	GNET::Privilege* GetPrivilege() { return m_pPrivilege; }	
	AMSoundBufferMan* GetAMSoundBufferMan();
	CECFactionMan* GetFactionMan() { return m_pFactionMan; }
	DataPathMan* GetDynObjPathTab() { return m_pDynObjPath; }
	SuiteEquipTable& GetSuiteEquipTable() { return m_SuiteEquipTab; }
	TitlePropTable& GetTitlePropTable() { return m_TitlePropTab; }
	AArray<DUNGEONSTEPINFO>* GetDungeonStepInfo() { return &m_DungeonStepInfoArray; }
	int GetItemNameColorIdx(int tid, int iDefIndex=0);
	DWORD GetItemNameColor(int tid, int iDefIndex=0);
	AString GetIPAddressByName(const char* szName);
	void AddIPAddressByName(const char* szName, const char* szAddr);
	AString GetDebugCmdByName(const char* szName);

	int GetCutsceneTabByIndex(int idCutscene);

	bool OpenURL(const char* szURL);

	//	Get game version string
	const char * GetVersionString() { return m_strAllVersion; }
	//	Get game version
	DWORD GetGameVersion();
	//	Get game build info
	DWORD GetGameBuild();
	//	Get active state
	bool IsActive() { return m_bActive; }
	//	Get initial data
	GAMEINIT& GetGameInit() { return m_GameInit; }
	//	Get logic tick time of current frame
	DWORD GetTickTime() { return m_dwTickTime; }
	//	Get real tick time of current frame
	DWORD GetRealTickTime() { return m_dwRealTickTime; }
	//	Set real tick time of current frame
	void SetRealTickTime(DWORD dwTime) { m_dwRealTickTime = dwTime; }
	//	Get average frame rate
	float GetAverageFrameRate() { return m_fAverFRate; }
	//	Time error with server
	int GetTimeError() { return m_iTimeError; }
	//  Time zone bias between UTC and local time in minutes
	int GetTimeZoneBias() { return m_iTimeZoneBias; }
	// Set the time of the server
	void SetServerTime(int iSevTime, int iTimeZoneBias);
	//	Get server GMT(UTC) time
	int GetServerGMTTime();
	//	Get server local time
	struct tm GetServerLocalTime();
	//	Is time changed by user
	bool IsTimeChangedByUser();
	//	Set LUA config version
	void SetLUAConfigVersion(int version) { m_iLuaVersion = version; }
	//	Get LUA config version
	int GetLUAConfigVersion();
	//	Discard current frame
	void DiscardFrame() { m_bDiscardFrame = true; }
	/**
	 * \brief 
	 * \param[in]
	 * \param[out]
	 * \return
	 * \note
	 * \warning
	 * \todo   
	 * \author kuiwu 
	 * \date 17/8/2006
	 * \see 
	 */
	CConfigFromLua * GetConfigFromLua()
	{
		return m_pConfigFromLua;
	}
	bool IsValuableItem(int tid);
	bool IsPocketableItem(int tid);
	bool IsPack2PocketItem(int tid);
	CECFunGamePlaza* GetFunGamePlaza() { return m_pFunGamePlaza; }
	int  GetAutoPickItemLevel(int idx, int tid);
	CLuaTriggerTask * GetLuaTriggerTask()
	{
		return m_pLuaTriggerTask;
	}
	CECCircleMan* GetCircleMan() { return m_pCircleMan; }
	int GetMappingKeyCode(int key);
	int GetInverseMappingKeyCode(int key);
	//  不适用userdata下面配置是防止玩家修改
	SERVERINFO* GetCrossServerInfo(int id);
	bool LoadSrcServerInfo();
	SERVERINFO* GetSrcServerInfo(int id);

	void SetOldPoint(int nPoint)
	{
		m_nOldPoint = nPoint;
	}
	int GetOldPoint()
	{
		return m_nOldPoint;
	}
	bool GetGuildEnter() const 
	{
		return m_isGuildEnter; 
	}
	void SetGuildEnter(bool val)
	{
		m_isGuildEnter = val; 
	}
	// for test 2012-01-09
	A3DCOLOR GetFashionColor() { return m_colorFashion; }
	void SetFashionColor(A3DCOLOR color) { m_colorFashion = color; }

	DWORD GetPoseOnPet(AString petModelName);
	abase::pair<DWORD, DWORD> GetDungeonStep(DWORD dungeonID);
	
	AString LoadMid();

	bool IsWindowSizeMoving();
	void OnWindowResize(int width,int height);
protected:	//	Attributes

	A3DEngine*			m_pA3DEngine;
	A3DDevice*			m_pA3DDevice;
	A3DLight*			m_pDirLight;		//	Directional light
	A3DLight*			m_pDynamicLight;	//	Dynamic point light
	CECViewport*		m_pViewport;		//	Main viewport
	CECConfigs*			m_pConfigs;			//	Configs
	CECGameRun*			m_pGameRun;			//	Game run object
	CECGameSession*		m_pGameSession;		//	Game session
	CECRTDebug*			m_pRTDebug;			//	Run-Time debug object
	A3DGFXExMan*		m_pA3DGFXExMan;		//	GFX object manager
	CECModelMan*		m_pECModelMan;		//	ECModel manager
#ifdef BACK_VERSION
	CECShadowRender*	m_pShadowRender;	//	Shadow renderer object
#endif
	CECPortraitRender*	m_pPortraitRender;	//	Portrait renderer object
	CECGFXCaster*		m_pGFXCaster;		//	GFX caster
	CECStringTab		m_FixedMsgs;		//	Fixed message table
	CECStringTab		m_PetTalkMsgs;
	CECStringTab		m_ItemDesc;			//	Item desciption string table
	CECStringTab		m_SkillDesc;		//	Skill description string table
	CECStringTab		m_ItemExtDesc;		//	Item extend description string table
	CECStringTab		m_BuffDesc;			//	Buff description string table
	CECStringTab		m_AddOnDesc;		//	Addon description string table
	CECStringTab		m_HomeTownDesc;		//  Addon description string table for home town
	CECStringTab		m_HomeTownIconPath; //  home town icon path map id->path
	ItemColTable		m_ItemColTab;		//	Item color table
	itemdataman *		m_pItemDataMan;		//	global templates manager
	elementdataman *	m_pElementDataMan;	//	global element templates manager
	HOMETOWN::HometownTemplate *  m_pHomeTownDataMan; //  home town templates manager
	DataPathMan*		m_pDataPathMan;		//	global path manager
	DataPathMan*		m_pDynObjPath;		//	Dynamic object path table
	ATaskTemplMan*		m_pTaskMan;			//	Task template manager
	CECImageRes*		m_pImageRes;		//	Image resources
#ifdef BACK_VERSION
	A3DSkinRender*		m_pSkinRender1;		//	Skin render 1
	A3DSkinRender*		m_pSkinRender2;		//	Skin render 2
	A3DLitModelRender*	m_pLitModelRender1;	//	Backup litmodel render
	A3DSkinRender*		m_pSkinRender1_W;	//	Skin render 1 in water 
#else
	A3DSkinRenderBase*		m_pSkinRender1;		//	Skin render 1
	A3DSkinRenderBase*		m_pSkinRender2;		//	Skin render 2
	A3DSkinRenderBase*		m_pSkinRenderShadow;//	Skin render Shadow
	A3DSkinRenderBase*		m_pSkinRender1_W;	//	Skin render 1 in water 
#endif
	
	CELBackMusic *		m_pBackMusic;		//	Back ground music
	CECSoundCache*		m_pSoundCache;		//	Sound cache
	GNET::Privilege*	m_pPrivilege;		//	Client privilege
	CECFactionMan*		m_pFactionMan;		//	Faction manager
	SuiteEquipTable		m_SuiteEquipTab;	//	Suite equipment table
	TitlePropTable		m_TitlePropTab;		//	Title property table
	IPMap				m_IPMap;			//	Domain name ip map
	ValuableItemMap		m_ValuableItemMap;  //  Valuable item map
	PocketableItemMap	m_PocketableItemMap;//  the item list that can put into pocket
	PocketableItemMap	m_Pack2PocketItemMap; //  the item list that move from pack to pocket per half an hour when gua ji
	AutoPickItemMap		m_AutoPickItemMap[4];	// auto pick and auto sell item config
	char				m_KeyCodeMap[256];	// key code mapping for foreign game client version, because the keyboard isn't American-style keyboard. 
	char				m_KeyCodeInverseMap[256]; // key code inverse mapping 


	DebugCmdMap			m_DebugCmd;			//  Debug cmd map
	int					m_iCutsceneTab[MAXNUM_CUTSCENE]; // Cutscene table

	CRITICAL_SECTION	m_csMsg;
	AList2<MSG, MSG&>	m_MsgList;

	GAMEINIT		m_GameInit;			//	Game init parameters
	bool			m_bActive;			//	true, Run game
	DWORD			m_dwTickTime;		//	Logic time of current tick
	DWORD			m_dwRealTickTime;	//	Real tick time
	bool			m_bDiscardFrame;	//	true, discard this frame
	float			m_fAverFRate;		//	Average frame rate
	int				m_iCurCursor;		//	Current cursor
	int				m_iTimeError;		//	Time error with server
	int				m_iTimeInit;		//	Server initial time 
	DWORD			m_dwTimeTickInit;	//	Synchronous tick with server initial time
	int				m_iTimeZoneBias;	//	Server time zone bias, in minutes
	int				m_iLuaVersion;		//	LUA config version for check md5

	A3DLight*		m_aLights[MAXNUM_LIGHT];

	APtrArray<A3DCursor*>	m_aCursors;		//	Cursor objects
	APtrArray<A3DFont*>		m_aFonts;		//	Fonts array
	AString					m_strAllVersion;//  All version's string

	CConfigFromLua      * m_pConfigFromLua;
	CECFunGamePlaza*	m_pFunGamePlaza;	//	the fun game plaza
	CLuaTriggerTask		* m_pLuaTriggerTask;	
	CECCircleMan*		m_pCircleMan;
	abase::vector<SERVERINFO>	m_CrossServerInfo;
	abase::vector<SERVERINFO>	m_SrcServerInfo;
	
	unsigned int	m_iTaskbarRestart;

	int m_nOldPoint;				// 旧活跃度积分
	
	// for test, 2012-01-09
	A3DCOLOR m_colorFashion;
	bool m_isGuildEnter;			// 2013-8-15 add by zy 添加是否进入帮派基地标记

	abase::hashtab<DWORD, AString, abase::_hash_function> m_PoseOnPetTab;

	AArray<DUNGEONSTEPINFO> m_DungeonStepInfoArray;

	SIZEMOVEPARAM  m_SizeMoveParam;

protected:	//	Operations

	//	Initialize A3D engine
	bool InitA3DEngine();
	//	Release A3D engine
	void ReleaseA3DEngine();
	//	Load cursors
	bool LoadCursors();
	//	Release cursors
	void ReleaseCursors();
	//	Create fonts
	bool CreateFonts();
	//	Release fonts
	void ReleaseFonts();
	//	Build suite equipment table
	void BuildSuiteEquipTab();
	//	Build title property table
	void BuildTitlePropTab();
	//	Load item color
	bool LoadItemColor();
	//	Load ip map table
	bool LoadIPMapTab();
	void SaveIPMapTab();
	
	//  Load cutscene config table
	bool LoadCutsceneTab();
	void SaveCutsceneTab();

	//  Load debug cmd shortcut
	bool LoadDebugCmdShortcut();
	void SaveDebugCmdShortcut();
	//  load valuable item map
	bool LoadValuableItemMap();

	bool LoadPocketableItemMap();
	//  天人合一状态下，定时从背包转移到随身包裹的特定物品列表
	bool LoadPack2PocketItemMap();
	
	bool LoadAutoPickItemMap();

	bool LoadKeyCodeMapping();

	bool LoadCrossServerInfo();

	bool LoadPoseOnPetTab();

	bool LoadDungeonStepTab();

};

///////////////////////////////////////////////////////////////////////////
//	
//	Inline functions
//	
///////////////////////////////////////////////////////////////////////////

