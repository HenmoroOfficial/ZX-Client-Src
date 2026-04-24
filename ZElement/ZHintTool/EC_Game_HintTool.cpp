/*
 * FILE: glb_Game.cpp
 *
 * DESCRIPTION: 
 *
 * CREATED BY: Duyuxin, 2003/12/16
 *
 * HISTORY: 
 *
 * Copyright (c) 2003 Archosaur Studio, All Rights Reserved.
 */

#pragma warning (disable: 4284)

#include "EC_Global.h"
#include "EC_Game_HintTool.h"
//#include "EC_Viewport.h"
#include "EC_GameRun_HintTool.h"
//#include "EC_SkinCache.h"
#include "EC_GameSession_HintTool.h"
#include "EC_RTDebug.h"
//#include "EC_ShadowRender.h"
//#include "EC_PortraitRender.h"
//#include "EC_GFXCaster.h"
#include "EC_IvtrTypes.h"
//#include "EC_Resource.h"
//#include "EL_Building.h"
//#include "EC_ImageRes.h"
#include "EC_Configs.h"
//#include "EL_BackMusic.h"
//#include "EC_UIManager.h"
//#include "EC_LoginUIMan.h"
//#include "EC_GameUIMan.h"
//#include "TaskTemplMan.h"
//#include "EC_Model.h"
//#include "EC_ModelMan.h"
//#include "EC_SceneLoader.h"
//#include "EC_SoundCache.h"
//#include "EC_Utility.h"
//#include "EC_SunMoon.h"
#include "EC_Faction_HintTool.h"
//#include "EC_NPC.h"
//#include "EC_World.h"
//#include "EC_Player.h"
//#include "ACSWrapper.h"
//#include "EC_GameRecord.h"
//#include "DlgReplay.h"
#include "EC_HostPlayer_HintTool.h"
#include "EC_FixedMsg.h"
//#include "EC_BossKey.h"
//#include "EC_FunGame.h"
#include "EC_MD5Hash.h"

//#include "privilege.hxx"
#include "elementdataman.h"
#include "itemdataman.h"
#include "hometowntemplate.h"
#include "ElementSkill.h"
#include "DataPathMan.h"

#include "globaldataman.h"

#include <A3DGFXEx.h>
#include <A3DGFXExMan.h>
#include <A3DSkinMan.h>
#include <A3DSkinRender.h>
#include <A3DCursor.h>
#include <A3DFont.h>
#include <A3DFontMan.h>
#include <A3DCamera.h>
#include <A3DLight.h>
#include <A3DLightMan.h>
#include <A3DConfig.h>
#include <A3DFuncs.h>
#include <A3DSkin.h>
#include <A3DViewport.h>
#include <AMEngine.h>
#include <AMConfig.h>
#include <AFileImage.h>
#include <AWScriptFile.h>
#include <AScriptFile.h>
#include <AAssist.h>
#include <A3DLitModelRender.h>
#include "AIniFile.h"

#include "ConfigFromLua.h"
#include "LuaTriggerTask.h"
//#include "EC_Circle.h"
//#include "EC_Cutscene.h"
#include "LuaState.h"
#include "LuaUtil.h"
#include "ScriptValue.h"
#include "LuaFuncCommon.h"

#define new A_DEBUG_NEW

///////////////////////////////////////////////////////////////////////////
//	
//	Define and Macro
//	
///////////////////////////////////////////////////////////////////////////

//extern D3DCAPS8		g_d3dcaps;

// Gameversion
DWORD GAME_VERSION = ((0 << 24) | (2 << 16) | (2 << 8) | 1);
DWORD GAME_BUILD = 1303;

#define ELEMENTSKILL_VERSION	6

///////////////////////////////////////////////////////////////////////////
//	
//	Reference to External variables and functions
//	
///////////////////////////////////////////////////////////////////////////

//bool g_bFocused = false;
//extern game_record gr;
static DWORD l_idMainThread = 0;

///////////////////////////////////////////////////////////////////////////
//	
//	Local Types and Variables and Global variables
//	
///////////////////////////////////////////////////////////////////////////

CECGame* g_pGame = NULL;

extern CECConfigs g_GameCfgs;

///////////////////////////////////////////////////////////////////////////
//	
//	Local functions
//	
///////////////////////////////////////////////////////////////////////////
/*
//	Time callback function
static void __stdcall _TimeCallback(UINT uTimerID, UINT uMsg, DWORD_PTR dwUser, DWORD_PTR dw1, DWORD_PTR dw2)
{
	if (!g_pGame || !g_pGame->GetGameSession())
		return;

	g_pGame->GetGameSession()->NetPulse();
}
*/
///////////////////////////////////////////////////////////////////////////
// for itemdataman we create some dummy functions
///////////////////////////////////////////////////////////////////////////
void set_to_classid(DATA_TYPE type, item_data * data, int major_type)
{
}
void generate_item_guid(item_data*)
{
}

///////////////////////////////////////////////////////////////////////////
//	
//	Implement CECGame
//	
///////////////////////////////////////////////////////////////////////////

CECGame::CECGame() : 
m_SuiteEquipTab(512),
m_TitlePropTab(512),
m_ItemColTab(256)
{
	ASSERT(!g_pGame);

	g_pGame = this;

	m_pA3DEngine		= NULL;
/*	m_pA3DDevice		= NULL;
	m_pBackMusic		= NULL;
	m_pViewport			= NULL;
*/	m_bActive			= false;
/*	m_pDirLight			= NULL;
	m_pDynamicLight		= NULL;
	m_pConfigs			= NULL;
	m_dwTickTime		= 0;
	m_dwRealTickTime	= 0;
*/	m_bDiscardFrame		= false;
	m_pGameRun			= NULL;
	m_pGameSession		= NULL;
//	m_pRTDebug			= NULL;
/*	m_fAverFRate		= 30.0f;
	m_pA3DGFXExMan		= NULL;
	m_pECModelMan		= NULL;
	m_pShadowRender		= NULL;
	m_pPortraitRender	= NULL;
	m_pGFXCaster		= NULL;
	m_pTaskMan			= NULL;
	m_pImageRes			= NULL;
	m_iCurCursor		= -1;
	m_iTimeError		= 0;
	m_iTimeInit			= 0;
	m_dwTimeTickInit	= 0;
	m_iTimeZoneBias		= -480;	// time zone of Beijing
	m_pSkinRender1		= NULL;
	m_pSkinRender2		= NULL;
	m_pBackMusic		= NULL;
	m_pSoundCache		= NULL;
	m_pPrivilege		= NULL;
*/	m_pFactionMan		= NULL;
	m_pDynObjPath		= NULL;
//	m_pLitModelRender1	= NULL;

	memset(&m_GameInit, 0, sizeof (m_GameInit));
//	memset(m_aLights, 0, sizeof (m_aLights));

	m_pItemDataMan		= NULL;
	m_pElementDataMan	= NULL;
	m_pHomeTownDataMan  = NULL;
	m_pDataPathMan		= NULL;

	m_pConfigFromLua = NULL;
	m_iToolType = TT_ZHUTI;
/*	m_pFunGamePlaza	 = NULL;
	m_pLuaTriggerTask = NULL;
	m_pCircleMan	 = NULL;
	
	::InitializeCriticalSection(&m_csMsg);
*/
}

CECGame::~CECGame()
{
//	::LeaveCriticalSection(&m_csMsg);
//	Release();
}
/*
#define HookGdiFunc(h, f_name, f_entry, f_content, f_content_new) \
{ \
	DWORD dwTemp; \
	f_entry = (PLONGLONG)GetProcAddress(h, #f_name); \
	if (VirtualProtect(f_entry, 16, PAGE_EXECUTE_READWRITE, &dwTemp)) { \
		f_content = f_content_new = *f_entry; \
		unsigned char* p = (unsigned char*)&f_content_new; \
		*p++ = (unsigned char)0xe9; \
		*(DWORD*)p = DWORD((DWORD)&My##f_name - 5 - (DWORD)f_entry); \
		*f_entry = f_content_new; } \
}
*/
/*
static PLONGLONG GetAsyncKeyState_Entry = 0;
static LONGLONG GetAsyncKeyState_Entry_Content = 0;
static LONGLONG GetAsyncKeyState_Entry_Content_New = 0;

SHORT WINAPI MyGetAsyncKeyState(int key)
{
	if (g_pGame->GetGameInit().hWnd && g_pGame->GetGameInit().hWnd != ::GetForegroundWindow())
		return 0;

	*GetAsyncKeyState_Entry = GetAsyncKeyState_Entry_Content;
	SHORT ret = GetAsyncKeyState(key);
	*GetAsyncKeyState_Entry = GetAsyncKeyState_Entry_Content_New;
	return ret;
}

static void setup_hook()
{
	HMODULE h = ::LoadLibraryA("user32.dll");
	HookGdiFunc(h, GetAsyncKeyState, GetAsyncKeyState_Entry, GetAsyncKeyState_Entry_Content, GetAsyncKeyState_Entry_Content_New)
	::FreeLibrary(h);
}

static PLONGLONG TerminateProcess_Entry = 0;
static LONGLONG TerminateProcess_Entry_Content = 0;
static LONGLONG TerminateProcess_Entry_Content_New = 0;

BOOL WINAPI MyTerminateProcess(HANDLE hProcess, UINT uExitCode)
{
	*TerminateProcess_Entry = TerminateProcess_Entry_Content;
	::MessageBoxA(NULL, "警告：程序被异常终止!!!!", "Error", MB_OK);
	char* p = 0;
	memcpy(p, "error", 5);
	return TRUE;
}
*/
static void hook_terminate_process()
{
	/*
	HMODULE h = ::LoadLibraryA("Kernel32.dll");
	HookGdiFunc(h, TerminateProcess, TerminateProcess_Entry, TerminateProcess_Entry_Content, TerminateProcess_Entry_Content_New)
	::FreeLibrary(h);
	*/
}
/*
static PLONGLONG ExitProcess_Entry = 0;
static LONGLONG ExitProcess_Entry_Content = 0;
static LONGLONG ExitProcess_Entry_Content_New = 0;
extern BOOL g_bHookExitProcess;

void WINAPI MyExitProcess(UINT uExitCode)
{
	*ExitProcess_Entry = ExitProcess_Entry_Content;

	if (g_bExceptionOccured || !g_bHookExitProcess)
		::ExitProcess(uExitCode);
	else
	{
		::MessageBoxA(NULL, "警告：程序异常退出!!!!", "Error", MB_OK);
		char* p = 0;
		memcpy(p, "error", 5);
	}
}

static void hook_exit_process()
{
	HMODULE h = ::LoadLibraryA("Kernel32.dll");
	HookGdiFunc(h, ExitProcess, ExitProcess_Entry, ExitProcess_Entry_Content, ExitProcess_Entry_Content_New)
	::FreeLibrary(h);
}
*/
//	Initialize game
bool CECGame::Init(const GAMEINIT& GameInit)
{
	a_LogOutput(1, "Build version %d", GAME_BUILD);

	l_idMainThread = GetCurrentThreadId();

	// setup_hook();
//	hook_terminate_process();
//	hook_exit_process();

/*	// first of all verify the ElementSkill version
	if( ELEMENTSKILL_VERSION != GNET::ElementSkill::GetVersion() )
	{
		MessageBoxA(GameInit.hWnd, "ElementSkill's version is incorrect!", "ERROR", MB_ICONSTOP | MB_OK);
		return false;
	}
*/
	m_GameInit = GameInit;

	//	Initalize random number generator
	a_InitRandom();

	//	Load configs
//	m_pConfigs = &g_GameCfgs;
	m_pConfigFromLua = new CConfigFromLua;
	if (!m_pConfigFromLua->Init())
	{
		glb_ErrorOutput(ECERR_FAILEDTOCALL, "CECGame::Init", __LINE__);
		return false;
	}

/*	m_pLuaTriggerTask = new CLuaTriggerTask;
	if (!m_pLuaTriggerTask->Init())
	{
		glb_ErrorOutput(ECERR_FAILEDTOCALL, "CECGame::Init", __LINE__);
		return false;
	}

	//	Initalize A3D engine
	if (!InitA3DEngine())
	{
		glb_ErrorOutput(ECERR_FAILEDTOCALL, "CECGame::Init", __LINE__);
		return false;
	}

	//	Create fonts
	if (!CreateFonts())
		a_LogOutput(1, "CECGame::Init, failed to create fonts !");

	//	now create gfx manager
	m_pA3DGFXExMan = new A3DGFXExMan();
	if (!m_pA3DGFXExMan->Init(m_pA3DDevice))
	{
		glb_ErrorOutput(ECERR_FAILEDTOCALL, "CECGame::Init", __LINE__);
		return false;
	}

	m_pLitModelRender1 = new A3DLitModelRender;
	if (!m_pLitModelRender1->Init(m_pA3DEngine, 60000, 60000 * 2) )
	{
		a_LogOutput(1, "CECGame::Init, lit model render init fail!");
		return false;
	}
*/
	m_pFactionMan = new CECFactionMan;

/*	m_pCircleMan = new CECCircleMan;
	//	Create ECModel manager
	m_pECModelMan = new CECModelMan();

	//	Create A3D skin renders
	m_pSkinRender1 = m_pA3DEngine->GetA3DSkinMan()->GetDefSkinRender();
	m_pSkinRender2 = new A3DSkinRender;
	m_pSkinRender2->Init(m_pA3DEngine);

	//	Create sound cache
	if (!(m_pSoundCache = new CECSoundCache))
	{
		glb_ErrorOutput(ECERR_NOTENOUGHMEMORY, "CECGame::Init", __LINE__);
		return false;
	}

	m_pSoundCache->SetMaxCacheSize(10 * 1024 * 1024);
*///	m_pSoundCache->SetMaxFileSize(/*256*/400 * 1024);
//	GetAMSoundBufferMan()->SetUserFileCache(m_pSoundCache);

	//	Load cursors
/*	if (!LoadCursors())
	{
		glb_ErrorOutput(ECERR_FAILEDTOCALL, "CECGame::Init", __LINE__);
		return false;
	}

	//	Create shadow renderer object
	if (!(m_pShadowRender = new CECShadowRender()))
	{
		glb_ErrorOutput(ECERR_NOTENOUGHMEMORY, "CECGame::Init", __LINE__);
		return false;
	}

	if (!m_pShadowRender->Init(512))
	{
		a_LogOutput(1, "CECGame::Init, failed to initalize shadow renderer!");
		m_pShadowRender->Release();
	}

	//	Create portrait render
	if (!(m_pPortraitRender = new CECPortraitRender()))
	{
		glb_ErrorOutput(ECERR_NOTENOUGHMEMORY, "CECGame::Init", __LINE__);
		return false;
	}

	if (!m_pPortraitRender->Init(64))
	{
		a_LogOutput(1, "CECGame::Init, failed to initalize portrait renderer!");
		return false;
	}
*/
	//	Run time debug object
/*	if (!(m_pRTDebug = new CECRTDebug))
	{
		glb_ErrorOutput(ECERR_NOTENOUGHMEMORY, "CECGame::Init", __LINE__);
		return false;
	}
*/
/*	if (!m_pRTDebug->Init(13))
	{
		glb_ErrorOutput(ECERR_FAILEDTOCALL, "CECGame::Init", __LINE__);
		return false;
	}
*/
	//	Create GFX caster
/*	if (!(m_pGFXCaster = new CECGFXCaster(m_pA3DGFXExMan)))
	{
		glb_ErrorOutput(ECERR_NOTENOUGHMEMORY, "CECGame::Init", __LINE__);
		return false;
	}
	if (!( m_pFunGamePlaza = new CECFunGamePlaza))
	{
		glb_ErrorOutput(ECERR_NOTENOUGHMEMORY, "CECGame::Init", __LINE__);
		return false;
	}
*/	//	Create game run object
	if (!(m_pGameRun = new CECGameRun))
	{
		glb_ErrorOutput(ECERR_NOTENOUGHMEMORY, "CECGame::Init", __LINE__);
		return false;
	}

	if (!m_pGameRun->Init())
	{
		glb_ErrorOutput(ECERR_FAILEDTOCALL, "CECGame::Init", __LINE__);
		return false;
	}

	//	Create game session
	if (!(m_pGameSession = new CECGameSession))
	{
		glb_ErrorOutput(ECERR_NOTENOUGHMEMORY, "CECGame::Init", __LINE__);
		return false;
	}

	//	Load fixed messages
	if (!m_FixedMsgs.Init("Configs\\fixed_msg.txt", true))
		glb_ErrorOutput(ECERR_FAILEDTOCALL, "CECGame::Init", __LINE__);

	//	Load item description strings
	if (!m_ItemDesc.Init("Configs\\item_desc.txt", true))
		glb_ErrorOutput(ECERR_FAILEDTOCALL, "CECGame::Init", __LINE__);

	//	Load item extend description
	if (!m_ItemExtDesc.Init("Configs\\item_ext_desc.txt", true))
		glb_ErrorOutput(ECERR_FAILEDTOCALL, "CECGame::Init", __LINE__);

	//	Load skill description strings
	if (!m_SkillDesc.Init("Configs\\skillstr.txt", true))
		glb_ErrorOutput(ECERR_FAILEDTOCALL, "CECGame::Init", __LINE__);
	
	//	Load buff description strings
	if (!m_BuffDesc.Init("Configs\\buff_str.txt", true))
		glb_ErrorOutput(ECERR_FAILEDTOCALL, "CECGame::Init", __LINE__);

	//	Load add on description strings
	if (!m_AddOnDesc.Init("Configs\\addon_str.txt", true))
		glb_ErrorOutput(ECERR_FAILEDTOCALL, "CECGame::Init", __LINE__);

	if (!m_HomeTownDesc.Init("Configs\\hometown_str.txt", true))
		glb_ErrorOutput(ECERR_FAILEDTOCALL, "CECGame::Init", __LINE__);

	if (!m_HomeTownIconPath.Init("Configs\\hometown_iconpath.txt", false))
		glb_ErrorOutput(ECERR_FAILEDTOCALL, "CECGame::Init", __LINE__);

	if (!m_PetTalkMsgs.Init("Configs\\pet_talk.txt", true))
		glb_ErrorOutput(ECERR_FAILEDTOCALL, "CECGame::Init", __LINE__);

	//	Load item color definition
	if (!LoadItemColor())
		glb_ErrorOutput(ECERR_FAILEDTOCALL, "CECGame::Init", __LINE__);
	
	//  Load valuable item map
	if (!LoadValuableItemMap())
		glb_ErrorOutput(ECERR_FAILEDTOCALL, "CECGame::Init", __LINE__);

	//	Load pocketable item map
	if(!LoadPocketableItemMap())
		glb_ErrorOutput(ECERR_FAILEDTOCALL, "CECGame::Init", __LINE__);

	if(!LoadPack2PocketItemMap())
		glb_ErrorOutput(ECERR_FAILEDTOCALL, "CECGame::Init", __LINE__);

	if(!LoadAutoPickItemMap())
		glb_ErrorOutput(ECERR_FAILEDTOCALL, "CECGame::Init", __LINE__);
	
	LoadIPMapTab();

	//
	LoadConfigs();

	//deal with the ip and port.
	m_GameInit.iPort = m_ConfigMap["port"].ToInt();
	strcpy( m_GameInit.szIP, (const char*)( m_ConfigMap["address"] ) );

//	LoadDebugCmdShortcut();

//	LoadCutsceneTab();


	g_MD5Hash.init();
	g_MD5Hash.Update("ZPWDATA", 7);
	m_pDataPathMan = new DataPathMan;

	if (m_pDataPathMan->Load("data\\path.data") == 0)
	{
		glb_ErrorOutput(ECERR_FAILEDTOCALL, "CECGame::Init", __LINE__);
		return false;
	}

	BYTE md5[16];
	if( !glb_CalcFileMD5("data\\elements.data", md5) )
		a_LogOutput(1, "data\\elements.data is missing...");
	else
		a_LogOutput(1, "elements data's md5: [%02x%02x%02x%02x%02x%02x%02x%02x%02x%02x%02x%02x%02x%02x%02x%02x]", md5[0], md5[1], md5[2], md5[3],
			md5[4], md5[5], md5[6], md5[7], md5[8], md5[9], md5[10], md5[11], md5[12], md5[13], md5[14], md5[15]);

	m_pItemDataMan = new itemdataman();
	//	now load templates from file
	if (0 != m_pItemDataMan->load_data("data\\elements.data"))
	{
		// linux 不能调用这个
		//		MessageBoxA(NULL, "无法加载游戏数据文件！\n此错误可能是更新不完全所致，请重新进行更新或进行版本验证！", "错误", MB_OK | MB_ICONSTOP);
		a_LogOutput(1, "data\\elements.data load error");
		return false;
	}
	m_pElementDataMan = m_pItemDataMan->GetElementDataMan();

	m_pHomeTownDataMan = HOMETOWN::HometownTemplate::GetInstance();
	if(0 != m_pHomeTownDataMan->load_data("data\\hometowndata"))
	{
		// linux 不能调用这个
//		MessageBoxA(NULL, "无法加载游戏数据文件！\n此错误可能是更新不完全所致，请重新进行更新或进行版本验证！", "错误", MB_OK | MB_ICONSTOP);
		glb_ErrorOutput(ECERR_FAILEDTOCALL, "CECGame::Init", __LINE__);
		return false;
	}

/*	if (!globaldata_load() )
	{
		glb_ErrorOutput(ECERR_FAILEDTOCALL, "CECGame::Init", __LINE__);
		return false;
	}
*/
//	m_pGameRun->InitGfxComposerMan();

/*	if( !glb_CalcFileMD5("data\\tasks.data", md5) )
		a_LogOutput(1, "data\\tasks.data is missing...");
	else
		a_LogOutput(1, "tasks data's md5: [%02x%02x%02x%02x%02x%02x%02x%02x%02x%02x%02x%02x%02x%02x%02x%02x]", md5[0], md5[1], md5[2], md5[3],
			md5[4], md5[5], md5[6], md5[7], md5[8], md5[9], md5[10], md5[11], md5[12], md5[13], md5[14], md5[15]);
	//	Load task templates
	if (!(m_pTaskMan = new ATaskTemplMan))
	{
		glb_ErrorOutput(ECERR_NOTENOUGHMEMORY, "CECGame::Init", __LINE__);
		return false;
	}

	m_pTaskMan->Init(m_pElementDataMan);
	m_pTaskMan->LoadTasksFromPack("data\\tasks.data", "data\\forbidden_task.txt", true);

	//	Load iamge resources
	if (!(m_pImageRes = new CECImageRes))
	{
		glb_ErrorOutput(ECERR_NOTENOUGHMEMORY, "CECGame::Init", __LINE__);
		return false;
	}

	m_pImageRes->LoadAllImages();

	//	Load dynamic object path table
	m_pDynObjPath = new DataPathMan;
	if (!m_pDynObjPath || !m_pDynObjPath->Load("data\\DynamicObjects.data"))
	{
		a_LogOutput(1, "CECGame::Init, Failed to load data\\DynamicObjects.data");
	//	return false;
	}
*/
	//	Build suite equipment table after elementdata.data has been loaded
	BuildSuiteEquipTab();

	//	Build title property table  after elementdata.data has been loaded
	BuildTitlePropTab();

/*	m_pBackMusic = new CELBackMusic();
	if( !m_pBackMusic->Init(g_pGame->GetA3DEngine()->GetAMSoundEngine()) )
	{
		delete m_pBackMusic;
		m_pBackMusic = NULL;

		glb_ErrorOutput(ECERR_FAILEDTOCALL, "CECGame::Init", __LINE__);
		return false;
	}

	// now set sound device volume for the first time
	m_pA3DDevice->GetA3DEngine()->GetAMSoundEngine()->SetVolume(m_pConfigs->GetSystemSettings().nSoundVol);
	m_pBackMusic->SetVolume(m_pConfigs->GetSystemSettings().nMusicVol);
	m_pBackMusic->SetSFXVolume(m_pConfigs->GetSystemSettings().nMusicVol);
	m_pBackMusic->SetCommentatorVolume(m_pConfigs->GetSystemSettings().nMusicVol);

	//	Initialize player's and NPC's static resources
	CECPlayer::InitStaticRes();
	CECNPC::InitStaticRes();
*/	m_pGameRun->LoadSpecialIDConfig();

/*	if (!::timeSetEvent(1000, 100, _TimeCallback, 0, TIME_PERIODIC | TIME_CALLBACK_FUNCTION))
	{
		a_LogOutput(1, "CECGame::Init, failed to call timeSetEvent()");
	}
*/	

/*	extern unsigned long _task_templ_cur_version;
	m_strAllVersion.Format("%x%x%x%x%x", ELEMENTDATA_VERSION, m_pGameRun->GetSpecialIDConfig().version,
			_task_templ_cur_version, m_pTaskMan->GetExportVersion(), globaldata_getgshop_timestamp());
*/	
	return true;
}

//	Release game
void CECGame::Release()
{
	A3DRELEASE(m_pGameRun);

	//	Release player's and NPC's static resources
/*	CECPlayer::ReleaseStaticRes();
	CECNPC::ReleaseStaticRes();

	if( m_pBackMusic )
	{
		m_pBackMusic->Release();
		delete m_pBackMusic;
		m_pBackMusic = NULL;
	}
*/
	globaldata_release();

	if( m_pItemDataMan )
	{
		delete m_pItemDataMan;
		m_pItemDataMan = NULL;
	}

	A3DRELEASE(m_pDataPathMan);

	m_FixedMsgs.Release();
	m_PetTalkMsgs.Release();
	m_ItemDesc.Release();
	m_ItemExtDesc.Release();
	m_SkillDesc.Release();
	m_BuffDesc.Release();
	m_AddOnDesc.Release();
	m_HomeTownDesc.Release();
	m_HomeTownIconPath.Release();
	m_SuiteEquipTab.clear();
	m_TitlePropTab.clear();
	m_ItemColTab.clear();
	m_ValuableItemMap.clear();
	m_PocketableItemMap.clear();
	
	//	Close session
	if (m_pGameSession)
	{
		m_pGameSession->Close();
		delete m_pGameSession;
		m_pGameSession = NULL;
		Sleep(500);
	}

	A3DRELEASE(m_pDynObjPath);
//	A3DRELEASE(m_pRTDebug);
/*	A3DRELEASE(m_pShadowRender);
	A3DRELEASE(m_pPortraitRender);
	A3DRELEASE(m_pTaskMan);
	A3DRELEASE(m_pImageRes);

	if (m_pPrivilege)
	{
		delete m_pPrivilege;
		m_pPrivilege = NULL;
	}

	if (m_pGFXCaster)
	{
		delete m_pGFXCaster;
		m_pGFXCaster = NULL;
	}
*/
	if (m_pFactionMan)
	{
		delete m_pFactionMan;
		m_pFactionMan = NULL;
	}
/*
	if (m_pECModelMan)
	{
		delete m_pECModelMan;
		m_pECModelMan = NULL;
	}

	A3DRELEASE(m_pLitModelRender1);

	A3DRELEASE(m_pA3DGFXExMan);
	A3DRELEASE(m_pSkinRender2);
	m_pSkinRender1 = NULL;
	A3DRELEASE(m_pSoundCache);

	ReleaseCursors();
	ReleaseFonts();

	//	Release A3D engine
	CECGame::ReleaseA3DEngine();
*/
//	m_pConfigs = NULL;
	if (m_pConfigFromLua)
	{
		m_pConfigFromLua->Release();
		delete m_pConfigFromLua;
		m_pConfigFromLua = NULL;
	}
/*	if (m_pFunGamePlaza)
	{
		delete m_pFunGamePlaza;
		m_pFunGamePlaza = NULL;
	}
	if (m_pLuaTriggerTask)
	{
		delete m_pLuaTriggerTask;
		m_pLuaTriggerTask = NULL;
	}
	if (m_pCircleMan)
	{
		delete m_pCircleMan;
		m_pCircleMan = NULL;
	}
*/	g_pGame = NULL;
}
/*
//	Load cursors
bool CECGame::LoadCursors()
{
	for (int i=0; i < NUM_RES_CURSOR; i++)
	{
		A3DSysCursor* pCursor = new A3DSysCursor;
		
		if (!pCursor->Load(m_pA3DDevice, res_CursorFile(i)))
		{
			a_LogOutput(1, "CECGame::LoadCursors, failed to cursor %s!", res_CursorFile(i));
			delete pCursor;
			pCursor = NULL;
		}

		m_aCursors.Add(pCursor);
	}

	return true;
}

//	Release cursors
void CECGame::ReleaseCursors()
{
	for (int i=0; i < m_aCursors.GetSize(); i++)
	{
		A3DCursor* pCursor = m_aCursors[i];
		if (pCursor)
			delete pCursor;
	}

	m_aCursors.RemoveAll();

	if (m_pA3DDevice)
		m_pA3DDevice->SetCursor(NULL);
}

//	Create fonts
bool CECGame::CreateFonts()
{
	//	Open font description file
	AWScriptFile ScriptFile;
	if (!ScriptFile.Open("Configs\\fonts.txt"))
	{
		m_aFonts.SetSize(NUM_RES_FONT, 10);
		for (int i=0; i < NUM_RES_FONT; i++)
			m_aFonts[i] = NULL;

		a_LogOutput(1, "CECGame::CreateFonts, Failed to open file Configs\\fonts.txt");
		return false;
	}

	A3DFontMan* pFontMan = m_pA3DEngine->GetA3DFontMan();

	//	Read configs
	while (ScriptFile.GetNextToken(true))
	{
		//	Font name
		wchar_t szName[128];
		a_strcpy(szName, ScriptFile.m_szToken);
		//	Font size
		int iHei = ScriptFile.GetNextTokenAsInt(false);
		//	Bold and Italic flag
		DWORD dwFlags = 0;
		dwFlags |= ScriptFile.GetNextTokenAsInt(false) ? A3DFONT_BOLD : 0;
		dwFlags |= ScriptFile.GetNextTokenAsInt(false) ? A3DFONT_ITALIC : 0;
		//	Sample character
		ScriptFile.GetNextToken(false);
		wchar_t chSample = ScriptFile.m_szToken[0];

		//	Register fonts
		HA3DFONT hFont = pFontMan->RegisterFontType(szName, iHei, dwFlags, chSample);
		if (!hFont)
			m_aFonts.Add(NULL);
		else
		{
			A3DFont* pFont = pFontMan->CreateA3DFont(hFont, 512, 128, 128);
			m_aFonts.Add(pFont);
		}
	}

	ScriptFile.Close();

	return true;
}

//	Release fonts
void CECGame::ReleaseFonts()
{
	if (!m_pA3DEngine)
		return;

	A3DFontMan* pFontMan = m_pA3DEngine->GetA3DFontMan();

	for (int i=0; i < m_aFonts.GetSize(); i++)
	{
		if (m_aFonts[i])
			pFontMan->ReleaseA3DFont(m_aFonts[i]);
	}

	m_aFonts.RemoveAll();
}
*/
//	Get font object
A3DFont* CECGame::GetFont(int iIndex)
{
	if (m_aFonts[iIndex])
		return m_aFonts[iIndex];

	return m_pA3DEngine->GetSystemFont();
}

//	Get dynamic object path
const char* CECGame::GetDynObjectPath(DWORD dwDynObjID)
{
	return m_pDynObjPath ? m_pDynObjPath->GetPathByID(dwDynObjID) : NULL;
}

/*	Window message handler

	Return true if message was processed, otherwise return false.
*/
/*
bool CECGame::WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
	if (CECBossKey::GetInstance()->DealWindowsMessage(hWnd,message,wParam,lParam))
	{
		return true;
	}
	switch (message)
	{
	case WM_KILLFOCUS:
		if( m_bActive )
		{
			if (m_pA3DDevice && !m_pA3DDevice->GetDevFormat().bWindowed)
			{
				if(IsWindow((HWND)wParam))
				{
					DWORD pid;
					GetWindowThreadProcessId((HWND)wParam,&pid);
					if(pid==GetCurrentProcessId())
					{
						break;
					}
				}
				ChangeDisplaySettings(NULL, CDS_RESET);
				ShowWindow(hWnd, SW_MINIMIZE);
				InvalidateRect(NULL, NULL, TRUE);
				m_pA3DDevice->SetNeedResetFlag(true);
			}
		}
		g_bFocused = false;
		//OutputDebugStringA("WM_KILLFOCUS\n");
		break;

	case WM_SETFOCUS:
		g_bFocused = true;
		//OutputDebugStringA("WM_SETFOCUS\n");
		break;

	case WM_ACTIVATE:
		if( wParam == WA_INACTIVE && m_bActive )
		{
			if (m_pA3DDevice && !m_pA3DDevice->GetDevFormat().bWindowed)
			{
				if(IsWindow((HWND)lParam))
				{
					DWORD pid;
					GetWindowThreadProcessId((HWND)lParam,&pid);
					if(pid==GetCurrentProcessId())
					{
						break;
					}
				}
				ChangeDisplaySettings(NULL, CDS_RESET);
				ShowWindow(hWnd, SW_MINIMIZE);
				InvalidateRect(NULL, NULL, TRUE);
				m_pA3DDevice->SetNeedResetFlag(true);
			}
		}
		//OutputDebugStringA("WM_ACTIVATE\n");
		break;

	case WM_ACTIVATEAPP:
		m_bActive = wParam ? true : false; 
		if(!m_bActive)
		{
			//SetForegroundWindow(hWnd);
			GUITHREADINFO guithreadinfo;
			guithreadinfo.cbSize = sizeof(GUITHREADINFO);
			if(GetGUIThreadInfo((DWORD)lParam,&guithreadinfo))
			{
				if(IsWindow(guithreadinfo.hwndActive))
				{
					DWORD pid;
					GetWindowThreadProcessId(guithreadinfo.hwndActive,&pid);
					if(pid==GetCurrentProcessId())
					{
						m_bActive = true;
					}
				}
			}
		}
		if( m_bActive )
		{
			if( m_pBackMusic )
			{
				m_pBackMusic->RestartMusic();
				m_pBackMusic->SetVolume(m_pConfigs->GetSystemSettings().nMusicVol);
			}
			if( m_pA3DEngine )
			{
				if( m_pA3DEngine->GetAMSoundEngine() )
					m_pA3DEngine->GetAMSoundEngine()->SetVolume(m_pConfigs->GetSystemSettings().nSoundVol);
			}
		}
		else
		{
			if (m_pA3DDevice && !m_pA3DDevice->GetDevFormat().bWindowed)
			{
				ChangeDisplaySettings(NULL, CDS_RESET);
				ShowWindow(hWnd, SW_MINIMIZE);
				InvalidateRect(NULL, NULL, TRUE);
				m_pA3DDevice->SetNeedResetFlag(true);
			}
		}
		//OutputDebugStringA("WM_ACTIVATEAPP\n");
		break;

	case WM_PAINT:
		PAINTSTRUCT	ps;
		HDC hdc;
		hdc = BeginPaint(hWnd, &ps);
		if( m_pConfigs )
			BitBlt(hdc, 0, 0, m_pConfigs->GetSystemSettings().iRndWidth, m_pConfigs->GetSystemSettings().iRndHeight, NULL, 0, 0, BLACKNESS);
		EndPaint(hWnd, &ps);
		break;

	case WM_SETCURSOR:

		if (m_pA3DDevice && m_pA3DDevice->GetShowCursor())
		{
			if( LOWORD(lParam) != HTCLIENT )
			{
				return false;
			}
			else
			{
				A3DCursor* pCursor = m_pA3DDevice->GetCursor();
				if (pCursor)
					pCursor->OnSysSetCursor();
			}
		}
		else
			SetCursor(NULL);
		
		break;

	case WM_QUIT:
	case WM_CLOSE:
	case WM_DESTROY:

		//	Do nothing to mask ALT-F4
		break;

	case WM_GETMINMAXINFO:
		{
			MINMAXINFO * pInfo = (MINMAXINFO *) lParam;

			if( (GetConfigs() && GetConfigs()->GetSystemSettings().bFullScreen) || GetWindowLong(hWnd, GWL_STYLE) == WS_POPUP )
			{
				// fullscreen mode, we don't limit the window size
				pInfo->ptMaxSize.x = 4000;
				pInfo->ptMaxSize.y = 3000;
				pInfo->ptMinTrackSize.x = 320;
				pInfo->ptMinTrackSize.y = 240;
				pInfo->ptMaxTrackSize.x = 4000;
				pInfo->ptMaxTrackSize.y = 3000;
			}
			else
			{
				// window mode, we must limit the window to fit on current screen.
				RECT rectWnd, rectClient;
				GetWindowRect(hWnd, &rectWnd);
				GetClientRect(hWnd, &rectClient);
				int dx = rectWnd.right - rectWnd.left - rectClient.right + rectClient.left;
				int dy = rectWnd.bottom - rectWnd.top - rectClient.bottom + rectClient.top;
				pInfo->ptMaxSize.x = ::GetSystemMetrics(SM_CXSCREEN) + dx;
				pInfo->ptMaxSize.y = ::GetSystemMetrics(SM_CYSCREEN) + dy;
				pInfo->ptMinTrackSize.x = 320 + dx;
				pInfo->ptMinTrackSize.y = 240 + dy;
				pInfo->ptMaxTrackSize.x = ::GetSystemMetrics(SM_CXSCREEN) + dx;
				pInfo->ptMaxTrackSize.y = ::GetSystemMetrics(SM_CYSCREEN) + dy;
			}
		}
		break;

	case WM_SIZING:
		{
			EC_SYSTEM_SETTING ss = GetConfigs()->GetSystemSettings();
			float r1 = 1.0f / ss.fRndScale;	//(float)ss.iRndWidth / ss.iRndHeight;
			float r2 = ss.fRndScale;	//(float)ss.iRndHeight / ss.iRndWidth;
// 			if( ss.fRndScale > 0.5f && ss.fRndScale < 2.0f )
// 			{
// 				r1 = 1.0f / ss.fRndScale;
// 				r2 = ss.fRndScale;
// 			}
// 
			RECT rectWnd, rectClient;
			GetWindowRect(hWnd, &rectWnd);
			GetClientRect(hWnd, &rectClient);
			int nFrameWidth = rectWnd.right - rectWnd.left - (rectClient.right - rectClient.left);
			int nFrameHeight = rectWnd.bottom - rectWnd.top - (rectClient.bottom - rectClient.top);
			RECT * pDrag = (RECT *) lParam;
			int nWidth = pDrag->right - pDrag->left - nFrameWidth;
			int nHeight = pDrag->bottom - pDrag->top - nFrameHeight;

			switch(wParam)
			{
			case WMSZ_TOP:
				pDrag->right = (int)(pDrag->left + nHeight * r1) + nFrameWidth;
				break;

			case WMSZ_BOTTOM:
				pDrag->right = (int)(pDrag->left + nHeight * r1) + nFrameWidth;
				break;

			case WMSZ_BOTTOMLEFT:
				if( nWidth > nHeight * r1 )
					pDrag->bottom = (int)(pDrag->top + nWidth * r2) + nFrameHeight;
				else
					pDrag->left = (int)(pDrag->right - nHeight * r1) - nFrameWidth;
				break;

			case WMSZ_BOTTOMRIGHT:
				if( nWidth > nHeight * r1 )
					pDrag->bottom = (int)(pDrag->top + nWidth * r2) + nFrameHeight;
				else
					pDrag->right = (int)(pDrag->left + nHeight * r1) + nFrameWidth;
				break;

			case WMSZ_TOPLEFT:
				if( nWidth > nHeight * r1 )
					pDrag->top = (int)(pDrag->bottom - nWidth * r2) - nFrameHeight;
				else
					pDrag->left = (int)(pDrag->right - nHeight * r1) - nFrameWidth;
				break;

			case WMSZ_TOPRIGHT:
				if( nWidth > nHeight * r1 )
					pDrag->top = (int)(pDrag->bottom - nWidth * r2) - nFrameHeight;
				else
					pDrag->right = (int)(pDrag->left + nHeight * r1) + nFrameWidth;
				break;

			case WMSZ_LEFT:
				pDrag->bottom = (int)(pDrag->top + nWidth * r2) + nFrameHeight;
				break;

			case WMSZ_RIGHT:
				pDrag->bottom = (int)(pDrag->top + nWidth * r2) + nFrameHeight;
				break;
			}
		}
		break;

	case WM_SIZE:
		if( GetConfigs() && !GetConfigs()->IsAdjusting() && m_pA3DDevice )
		{
			int w, h;
			w = LOWORD(lParam);
			h = HIWORD(lParam);

			if( w == 0 || h == 0 )
				break;

			EC_SYSTEM_SETTING ss = GetConfigs()->GetSystemSettings();
			ss.iRndWidth = w;
			ss.iRndHeight = h;

			A3DViewport * pViewport = g_pGame->GetViewport()->GetA3DViewport();
			A3DVIEWPORTPARAM param1 = *pViewport->GetParam();
			
			GetConfigs()->SetSystemSettings(ss, true);

			A3DVIEWPORTPARAM param2 = *pViewport->GetParam();

			if (m_pGameRun)
			{
				A3DRECT rect1, rect2;
				rect1.SetRect(0, 0, param1.Width, param1.Height);
				rect2.SetRect(0, 0, param2.Width, param2.Height);
				int nCurUIMan = m_pGameRun->GetUIManager()->GetCurrentUIMan();
				if( nCurUIMan == CECUIManager::UIMAN_LOGIN )
				{
					m_pGameRun->GetUIManager()->GetLoginUIMan()->RearrangeWindows(rect1, rect2);
				}
				else if( nCurUIMan == CECUIManager::UIMAN_INGAME )
				{
					m_pGameRun->GetUIManager()->GetInGameUIMan()->RearrangeWindows(rect1, rect2);
				}

				AfxGetGFXExMan()->Resize2DViewport();
			}
		}
		else
		{
			return false;
		}

		break;

	case WM_KEYDOWN:
		if( wParam != VK_PROCESSKEY && !GetAsyncKeyState(wParam) )
			return true;

	default:
		if (m_pGameRun)
			return m_pGameRun->DealWindowsMessage(message, wParam, lParam);

		return false;
	}

	return true;
}

extern HANDLE g_hExitGame;
extern HANDLE g_hMainThread;
extern HANDLE g_hRenderEvent;
extern HANDLE g_hRenderThread;
extern HANDLE g_hWaittoHandleACP;
extern volatile bool g_bMainSuspened;
extern volatile bool g_bMultiThreadRenderMode;
extern CRITICAL_SECTION g_csRenderThread;

int CECGame::RunInRenderThread()
{
	int iRet = -1;

	while (1)
	{
		if (g_bExceptionOccured || ::WaitForSingleObject(g_hExitGame, 0) == WAIT_OBJECT_0)
		{
			iRet = 0;
			break;
		}

		if (g_bMainSuspened)
			::SetEvent(g_hWaittoHandleACP); //In renderthreadfunc
		//	::ResumeThread(g_hMainThread);

		WaitForSingleObject(g_hRenderEvent, 20);

		if (!OnceRun())
			break;
	}

	return iRet;
}

bool CECGame::RunInMainThread()
{
	if (!DispatchWindowsMessage())
	{
		a_LogOutput(1, "CECGame::Run(), break because DispatchWindowsMessage return false!");
		return false;
	}

	if (g_bMultiThreadRenderMode)
		return true;

	if (!GetA3DDevice()->GetAutoResetFlag() && GetA3DDevice()->GetNeedResetFlag() && GetA3DDevice()->GetCanResetFlag())
	{
		SuspendLoadThread();

		if (!g_pGame->GetA3DDevice()->Reset())
			a_LogOutput(1, "CECGame::Run(), need reset the device, but we fail to call it!");

		// now restore some once set render states.
		if( m_pConfigs->GetSystemSettings().bMipMapBias )
		{
			float v = -1.0f;
			for(int i=0; i<m_pA3DDevice->GetMaxSimultaneousTextures(); i++)
			{
				m_pA3DDevice->SetDeviceTextureStageState(i, D3DTSS_MIPMAPLODBIAS, *(DWORD *)&v);
			}
		}

		StartLoaderThread();
	}

	return OnceRun();
}

bool CECGame::OnceRun()
{
	static DWORD dwLastFrame = 0;
	static int iTickCnt = 0;
	static DWORD dwAccuTime = 0;
	static DWORD dwMGCTime = 0;	//	Memory garbage collect time

	ACSWrapper csa(&g_csRenderThread);

	if (dwLastFrame == 0)
	{
		dwLastFrame = a_GetTime();
		return true;
	}
	else
	{
		DWORD dwCurTime = a_GetTime();
		m_dwRealTickTime = dwCurTime - dwLastFrame;
		dwLastFrame = dwCurTime;
	}

	if (m_bDiscardFrame || !m_dwRealTickTime)
	{
		//	Discard this frame
		m_bDiscardFrame = false;
		return true;
	}

	//	Calculate average frame rate
	dwAccuTime += m_dwRealTickTime;
	if (++iTickCnt >= 15)
	{
		m_fAverFRate = 1000.0f / (dwAccuTime / 15.0f);

		iTickCnt	= 0;
		dwAccuTime	= 0;
	}

	//	Update network timer
	m_pGameSession->Update(m_dwRealTickTime);


	//	Clamp logic tick time
	m_dwTickTime = m_dwRealTickTime;
	if (m_dwTickTime > 250)
		m_dwTickTime = 250;

	//	Game tick routine
	if (!m_pGameRun->Tick(m_dwTickTime))
	{
		a_LogOutput(1, "CECGame::Run(), break because CECGameRun::Tick return false");
		return false;
	}

	if( (m_bActive && !IsMinimized(m_GameInit.hWnd)) || (m_pA3DDevice && m_pA3DDevice->GetDevFormat().bWindowed && IsWindowVisible(m_GameInit.hWnd) && !IsMinimized(m_GameInit.hWnd)) )
	{
		bool bNeedRender = true;
		if( !m_bActive )
		{
			int nSkipLevel = 4 - m_pConfigs->GetSystemSettings().nBackAnimLevel;
			a_Clamp(nSkipLevel, 0, 4);
			if( nSkipLevel >= 4 )
				bNeedRender = false;
			else if( nSkipLevel == 0 )
				bNeedRender = true;
			else
			{
				if( iTickCnt % (int)(1 << (nSkipLevel + 1)) )
					bNeedRender = false;
			}
		}

		if( bNeedRender )
		{
			if (!m_pGameRun->Render())
			{
				a_LogOutput(1, "CECGame::Run(), break because CECGameRun::Render() return false!");
				return false;
			}
		}
		else
			Sleep(20);
	}
	else
		Sleep(20);
	
	if( m_bActive && g_bFocused )
	{
		m_pBackMusic->Update(m_dwTickTime);
		m_pBackMusic->UpdateSFX(m_dwTickTime);
		m_pBackMusic->UpdateCommentator(m_dwTickTime);
	}
	
	//	Do memory garbage collect
	if ((dwMGCTime += m_dwRealTickTime) >= 4000)
	{
		dwMGCTime = 0;
	//	a_MemGarbageCollect();
	}

	if( IsTimeChangedByUser() )
	{
		if( m_pGameSession->IsConnected() )
		{
			a_LogOutput(1, "want to terminte by will 3");
			g_bHookExitProcess = FALSE;
			::ExitProcess(0);
		}
	}

	return true;
}

//	Add windows message
void CECGame::AddWindowsMessage(MSG& msg)
{
	::EnterCriticalSection(&m_csMsg);
	m_MsgList.AddTail(msg);
	::LeaveCriticalSection(&m_csMsg);
}

//	Dispatch windows message
bool CECGame::DispatchWindowsMessage()
{
	MSG msg;
	while (::PeekMessage(&msg, NULL, 0, 0, PM_REMOVE))
	{
		if (msg.message == WM_QUIT)
			return false;

		::TranslateMessage(&msg);
		::DispatchMessage(&msg);
	}

	return true;
}

//	Initialize A3D engine
bool CECGame::InitA3DEngine()
{
	//	Init Engine
	if (!(m_pA3DEngine = new A3DEngine))
	{
		glb_ErrorOutput(ECERR_NOTENOUGHMEMORY, "CECGame::InitA3DEngine", __LINE__);
		return false;
	}
	
	A3DDEVFMT devFmt;
	devFmt.bWindowed	= !m_GameInit.bFullscreen;
	devFmt.nWidth		= m_GameInit.iRenderWid;
	devFmt.nHeight		= m_GameInit.iRenderHei;
	devFmt.fmtTarget	= A3DFMT_UNKNOWN;
	devFmt.fmtDepth		= A3DFMT_UNKNOWN;
//	devFmt.bVSync		= m_pConfigs->GetSystemSettings().bVSync;
	devFmt.bVSync		= false;
						
	// A3DDEV_ALPHATARGETFIRST: if we use alpha target first and the desktop display mode is just what we use 
	// as the full-screen mode for the game, the game may not minimized when switched off. To solve this, we should
	// add a manual call to ChangeDisplaySetting when we receive WM_ACTIVATEAPP
	DWORD devFlags = A3DDEV_ALLOWMULTITHREAD;
	if( g_d3dcaps.PixelShaderVersion >= D3DPS_VERSION(1, 4) )
		devFlags |= A3DDEV_ALPHATARGETFIRST;

	if (gr.get_state() == game_record::enum_gr_record
	 || gr.get_state() == game_record::enum_gr_replay)
		devFlags |= A3DDEV_FPU_PRESERVE;

	if (!m_pA3DEngine->Init(m_GameInit.hInstance, m_GameInit.hWnd, &devFmt, devFlags))
	{
		glb_ErrorOutput(ECERR_FAILEDTOCALL, "CECGame::InitA3DEngine", __LINE__);
		return false;
	}

	g_pA3DConfig->RT_SetShowFPSFlag(false);
	g_pA3DConfig->SetFlagLinearSprite(true);

	g_pA3DConfig->SetTextureQuality((A3DTEXTURE_QUALITY)m_pConfigs->GetSystemSettings().iTexDetail);
	m_pA3DEngine->GetAMEngine()->GetAMConfig()->SetSoundQuality((AMSOUND_QUALITY)m_pConfigs->GetSystemSettings().iSoundQuality);

	m_pA3DDevice = m_pA3DEngine->GetA3DDevice();

	// In element client we use multithread to load terrain meshes etc.
	// so the reset operation can not be done by the device automatically, or we may not release all unmanaged
	// device objects before we reset, so reset will fail.
	m_pA3DDevice->SetAutoResetFlag(false);

	// show a black screen without loading text
	m_pA3DDevice->BeginRender();
	m_pA3DDevice->Clear(D3DCLEAR_TARGET, 0x00000000, 1.0f, 0);
	m_pA3DDevice->EndRender();
	m_pA3DDevice->Present();

	//	Create viewport
	if (!(m_pViewport = new CECViewport))
	{
		glb_ErrorOutput(ECERR_NOTENOUGHMEMORY, "CECGame::InitA3DEngine", __LINE__);
		return false;
	}

	CECViewport::INITPARAMS ViewParams;
	memset(&ViewParams, 0, sizeof (ViewParams));

	ViewParams.rcView.left		= 0;
	ViewParams.rcView.top		= 0;
	ViewParams.rcView.right		= m_GameInit.iRenderWid;
	ViewParams.rcView.bottom	= m_GameInit.iRenderHei;
	ViewParams.fMinZ			= 0.0f;
	ViewParams.fMaxZ			= 1.0f;
	ViewParams.bClearColor		= false;
	ViewParams.bClearZ			= true;
	ViewParams.colClear			= 0x00000000;
	ViewParams.fFOV				= DEG2RAD(DEFCAMERA_FOV);
	ViewParams.fNearPlane		= 0.2f;
	ViewParams.fFarPlane		= 2000.0f;

	if( m_pConfigs->GetSystemSettings().bWideScreen && m_GameInit.iRenderHei * 16 > m_GameInit.iRenderWid * 9 )
	{
		int nNewHeight = m_GameInit.iRenderWid * 9 / 16;
		ViewParams.rcView.top = (m_GameInit.iRenderHei - nNewHeight) >> 1;
		ViewParams.rcView.bottom = ViewParams.rcView.top + nNewHeight;
	}

	float w = (float)m_GameInit.iRenderWid;
	float h = (float)m_GameInit.iRenderHei;
	ViewParams.bOrthoCamera	= true;
	ViewParams.fOCLeft		= -w;
	ViewParams.fOCRight		= w;
	ViewParams.fOCBottom	= -h;
	ViewParams.fOCTop		= h;
	ViewParams.fOCZNear		= -1500.0f;
	ViewParams.fOCZFar		= 1500.0f;

	if (!m_pViewport->Init(ViewParams))
	{
		glb_ErrorOutput(ECERR_FAILEDTOCALL, "CECGame::InitA3DEngine", __LINE__);
		return false;
	}
	m_pViewport->Activate(false);

	//	Create directional light
	A3DLIGHTPARAM LightParams;
	memset(&LightParams, 0, sizeof (LightParams));

	//	Set default light parameters
	LightParams.Type = A3DLIGHT_DIRECTIONAL;
	LightParams.Direction = m_pConfigs->m_vDefLightDir;
	LightParams.Diffuse = m_pConfigs->m_dwDefLightDiff;
	LightParams.Specular = m_pConfigs->m_dwDefLightSpec;
	LightParams.Attenuation0 = 1.0f;
	if (!(m_pDirLight = CreateA3DLight(&LightParams)))
	{
		glb_ErrorOutput(ECERR_FAILEDTOCALL, "CECGame::InitA3DEngine", __LINE__);
		return false;
	}
	m_pDirLight->TurnOn();

	//	Create dynamic point light
	LightParams.Type = A3DLIGHT_POINT;
	LightParams.Specular = A3DCOLORVALUE(0.0f, 0.0f, 0.0f, 1.0f);
	if (!(m_pDynamicLight = CreateA3DLight(&LightParams)))
	{
		glb_ErrorOutput(ECERR_FAILEDTOCALL, "CECGame::InitA3DEngine", __LINE__);
		return false;
	}

	m_pDynamicLight->TurnOff();

	//	Set default ambient value and fog parameters
	m_pA3DDevice->SetAmbient(m_pConfigs->m_dwDefAmbient);
	m_pA3DDevice->SetFogEnable(true);
	m_pA3DDevice->SetFogStart(m_pConfigs->m_fDefFogStart);
	m_pA3DDevice->SetFogEnd(m_pConfigs->m_fDefFogEnd);
	m_pA3DDevice->SetFogDensity(m_pConfigs->m_fDefFogDensity);
	m_pA3DDevice->SetFogColor(m_pConfigs->m_dwDefFogColor);

	m_pA3DDevice->SetGammaLevel(m_pConfigs->GetSystemSettings().iGamma);

	//	Load skin model shaders
	A3DSkinMan* pSkinMan = m_pA3DEngine->GetA3DSkinMan();
	if (!pSkinMan->LoadSkinModelVertexShaders("Configs\\skinmodelvs.cfg"))
	{
		a_LogOutput(1, "CECGame::InitA3DEngine, Failed to load skinmodelvs.cfg");
		return false;
	}

	pSkinMan->SetDirLight(m_pDirLight);
	pSkinMan->SetDPointLight(m_pDynamicLight);
	
	if( m_pConfigs->GetSystemSettings().bMipMapBias )
	{
		float v = -1.0f;
		for(int i=0; i<m_pA3DDevice->GetMaxSimultaneousTextures(); i++)
		{
			m_pA3DDevice->SetDeviceTextureStageState(i, D3DTSS_MIPMAPLODBIAS, *(DWORD *)&v);
		}
	}
	
	return true;
}

//	Release A3D engine
void CECGame::ReleaseA3DEngine()
{
	//	Release lights
	ReleaseA3DLight(m_pDirLight);
	ReleaseA3DLight(m_pDynamicLight);

	//	Release viewport
	if (m_pViewport)
	{
		delete m_pViewport;
		m_pViewport = NULL;
	}

	A3DRELEASE(m_pA3DEngine);
	m_pA3DDevice = NULL;
}
*/
/*	Create a A3D Light object

	pParams: light's parameters, if this parameter is NULL, light's parameter must
			 be set through A3DLight::SetLightParam later.
*/
/*
A3DLight* CECGame::CreateA3DLight(const A3DLIGHTPARAM* pParams)
{
	ASSERT(m_pA3DEngine);

	int i;

	for (i=0; i < MAXNUM_LIGHT; i++)
	{
		if (!m_aLights[i])
			break;
	}

	if (i >= MAXNUM_LIGHT)
		return NULL;

	A3DLight* pLight = NULL;

	//	Create a light
	m_pA3DEngine->GetA3DLightMan()->CreateLight(i, &pLight);

	if (!pLight)
	{
		a_LogOutput(1, "Failed to create A3D light");
		return NULL;
	}

	if (pParams)
		pLight->SetLightParam(*pParams);

	m_aLights[i] = pLight;

	return pLight;
}

//	Release A3D Light object
void CECGame::ReleaseA3DLight(A3DLight* pLight)
{
	if (!pLight)
		return;

	for (int i=0; i < MAXNUM_LIGHT; i++)
	{
		if (m_aLights[i] == pLight)
		{
			m_aLights[i] = NULL;
			break;
		}
	}
	// 使用A3DLightMan创建的灯光也要对应的由A3DLightMan来释放
	m_pA3DEngine->GetA3DLightMan()->ReleaseLight(pLight);
//	A3DRELEASE(pLight);
}

//	Reset game, release all resources
bool CECGame::Reset()
{
	if (m_pSkinRender1)
	{
		m_pSkinRender1->ResetRenderInfo(true);
		m_pSkinRender1->ResetRenderInfo(false);
	}

	if (m_pSkinRender2)
	{
		m_pSkinRender2->ResetRenderInfo(true);
		m_pSkinRender2->ResetRenderInfo(false);
	}

	if (m_pGFXCaster)
		m_pGFXCaster->Reset();

	if (m_pImageRes)
		m_pImageRes->Release();

	if (m_pA3DGFXExMan)
	{
		m_pA3DGFXExMan->Release();
		m_pA3DGFXExMan->Init(m_pA3DDevice);
	}

	if (m_pA3DEngine)
	{
		if (!m_pA3DEngine->ResetResource())
		{
			a_LogOutput(1, "Failed to reset A3DEngine resources.");
			return false;
		}
	}

	if (m_pImageRes)
		m_pImageRes->LoadAllImages();
	
	//	Discard current frame
	m_bDiscardFrame = true;

	return true;
}
*/

//	Load A3D skin model
//A3DSkinModel* CECGame::LoadA3DSkinModel(const char* szFile, int iSkinFlag/* 0 */)
/*{
	A3DSkinModel* pModel = new A3DSkinModel;
	if (!pModel)
	{
		glb_ErrorOutput(ECERR_NOTENOUGHMEMORY, "CECGame::LoadA3DSkinModel", __LINE__);
		return false;
	}

	if (!pModel->Init(m_pA3DEngine))
	{
		delete pModel;
		glb_ErrorOutput(ECERR_FAILEDTOCALL, "CECGame::LoadA3DSkinModel", __LINE__);
		return false;
	}

	if (!pModel->Load(szFile, iSkinFlag))
	{
		delete pModel;
		a_LogOutput(1, "CECGame::LoadA3DSkinModel, Failed to load %s", szFile);
		return false;
	}

	return pModel;
}

//	Release A3D skin model
void CECGame::ReleaseA3DSkinModel(A3DSkinModel* pModel)
{
	if (!pModel)
		return;

	pModel->Release();
	delete pModel;
}
*/
//	Load A3D skin
//A3DSkin* CECGame::LoadA3DSkin(const char* szFile, bool bUnique/* true */)
/*{
	A3DSkinMan* pSkinMan = m_pA3DEngine->GetA3DSkinMan();
	return pSkinMan->LoadSkinFile(szFile);
}
*/
//	Release A3D skin
//void CECGame::ReleaseA3DSkin(A3DSkin* pSkin, bool bUnique/* true */)
/*{
	if (pSkin)
	{
		A3DSkinMan* pSkinMan = m_pA3DEngine->GetA3DSkinMan();
		pSkinMan->ReleaseSkin(&pSkin);
	}	
}

//	Load a building model from file
CELBuilding* CECGame::LoadBuilding(const char* szFile)
{
	CELBuilding* pBuilding = new CELBuilding;
	if (!pBuilding)
	{
		glb_ErrorOutput(ECERR_NOTENOUGHMEMORY, "CECGame::LoadBuilding", __LINE__);
		return NULL;
	}

	AFileImage File;
	if (!File.Open("", szFile, AFILE_OPENEXIST | AFILE_BINARY))
	{
		delete pBuilding;
		a_LogOutput(1, "CECGame::LoadBuilding, failed to open file %s", szFile);
		return NULL;
	}

	if (!pBuilding->Load(m_pA3DDevice, &File))
	{
		delete pBuilding;
		a_LogOutput(1, "CECGame::LoadBuilding, failed to load lit model %s", szFile);
		return NULL;
	}

	File.Close();

	return pBuilding;
}

//	Release building model
void CECGame::ReleaseBuilding(CELBuilding* pBuilding)
{
	A3DRELEASE(pBuilding);
}
*/
//	Output a run-time debug string
void CECGame::RuntimeDebugInfo(DWORD dwCol, const ACHAR* szMsg, ...)
{
/*	if (!m_pRTDebug)
		return;

	if (!szMsg || !szMsg[0])
		return;

	ACHAR szBuf[512];

	va_list vaList;
	va_start(vaList, szMsg);
	a_vsprintf(szBuf, szMsg, vaList);
	va_end(vaList);

	m_pRTDebug->OutputDebugInfo(dwCol, szBuf);
*/
}
/*
//	Show or hide cursor
void CECGame::ShowCursor(bool bShow)
{
	m_pA3DDevice->ShowCursor(bShow);
}

//	Change current cursor
int CECGame::ChangeCursor(int iCursor)
{
	if (iCursor == m_iCurCursor)
		return iCursor;

	if (m_aCursors[iCursor])
		m_pA3DDevice->SetCursor(m_aCursors[iCursor]);

	// force show this cursor
	ShowCursor(g_pGame->GetA3DDevice()->GetShowCursor());

	if( l_idMainThread != GetCurrentThreadId() )
	{
		// ::SetCursor must be called from main thread to take effects, so here we should post a WM_SETCURSOR message
		// to ensure the main thread receive WM_SETCURSOR and update the cursor again
		PostMessage(m_GameInit.hWnd, WM_SETCURSOR, (WPARAM)m_GameInit.hWnd, MAKELPARAM(HTCLIENT, WM_MOUSEMOVE));
	}

	int iOldCursor = m_iCurCursor;
	m_iCurCursor = iCursor;
	return iOldCursor;
}

//	Set time error
void CECGame::SetServerTime(int iSevTime, int iTimeZoneBias)
{
	int iOldTimeError = m_iTimeError;
	m_iTimeError = iSevTime - time(NULL);
	m_iTimeInit = iSevTime;
	m_dwTimeTickInit = GetTickCount();
	m_iTimeZoneBias = iTimeZoneBias;

	struct tm serverLocal = GetServerLocalTime();
	int nTimeInDay = serverLocal.tm_hour * 3600 + serverLocal.tm_min * 60 + serverLocal.tm_sec;
	GetGameRun()->GetWorld()->GetSunMoon()->SetTimeOfTheDay(nTimeInDay / (4.0f * 3600.0f));
	
	S2C::player_wallow_info wallowinfo = GetGameRun()->GetWallowInfo();
	wallowinfo.play_time += m_iTimeError - iOldTimeError;
	GetGameRun()->SetWallowInfo(wallowinfo);
}
*/
//	Get server GMT(UTC) time
int CECGame::GetServerGMTTime()
{ 
	return time(NULL) + m_iTimeError; 
}
/*
struct tm CECGame::GetServerLocalTime()
{
	time_t serverTime = GetServerGMTTime();
	serverTime -= m_iTimeZoneBias * 60; // UTC = localtime + bias => localtime = UTC - bias
	return *gmtime(&serverTime);
}

bool CECGame::IsTimeChangedByUser()
{
	if( m_iTimeInit == 0 )
		return false;

	DWORD dwTickPassed = GetTickCount() - m_dwTimeTickInit;
	int time1 = m_iTimeInit + dwTickPassed / 1000;
	int time2 = GetServerGMTTime();
	if( abs(time1 - time2) < 600 )
		return false;
	else
		return true;
}
*/
bool CECGame::LoadLoginRes()
{
	return true;
}

void CECGame::ReleaseLoginRes()
{
}

bool CECGame::LoadInGameRes()
{
	return true;
}

void CECGame::ReleaseInGameRes()
{
}

//	Get game version
DWORD CECGame::GetGameVersion()
{
	return GAME_VERSION;
}

//	Get game build
DWORD CECGame::GetGameBuild()
{
	return GAME_BUILD;
}
/*
AMSoundBufferMan* CECGame::GetAMSoundBufferMan()
{
	return m_pA3DEngine->GetAMEngine()->GetAMSoundEngine()->GetAMSoundBufferMan();
}
*/
//	Get item's extend description string
const wchar_t* CECGame::GetItemExtDesc(int tid)
{
	return m_ItemExtDesc.GetWideString(tid);
}

//	Get buff description string
const wchar_t* CECGame::GetBuffDesc(int id)
{
	return m_BuffDesc.GetWideString(id);
}

//	Build suite equipment table
void CECGame::BuildSuiteEquipTab()
{
	if (!m_pElementDataMan)
		return;

	DATA_TYPE DataType = DT_INVALID;
	
	int tid = m_pElementDataMan->get_first_data_id(ID_SPACE_ESSENCE, DataType);

	while (tid)
	{
		const void* pData = m_pElementDataMan->get_data_ptr(tid, ID_SPACE_ESSENCE, DataType);

		if (DataType == DT_SUITE_ESSENCE)
		{
			const SUITE_ESSENCE* pSuiteEss = (const SUITE_ESSENCE*)pData;

			for (int i=0; i < pSuiteEss->max_equips; i++)
			{
				bool bVal = m_SuiteEquipTab.put(pSuiteEss->equipments[i].id, tid);
				ASSERT(bVal);
			}
		}

		//	Get next item
		tid = m_pElementDataMan->get_next_data_id(ID_SPACE_ESSENCE, DataType);
	}
}

//	Build suite equipment table
void CECGame::BuildTitlePropTab()
{
	if (!m_pElementDataMan)
		return;

	DATA_TYPE DataType = DT_INVALID;
	
	int tid = m_pElementDataMan->get_first_data_id(ID_SPACE_CONFIG, DataType);

	while (tid)
	{
		const void* pData = m_pElementDataMan->get_data_ptr(tid, ID_SPACE_CONFIG, DataType);

		if (DataType == DT_TITLE_PROP_CONFIG)
		{
			const TITLE_PROP_CONFIG* pTitleProp = (const TITLE_PROP_CONFIG*)pData;

			m_TitlePropTab.put(pTitleProp->id_title, tid);
		}

		//	Get next item
		tid = m_pElementDataMan->get_next_data_id(ID_SPACE_CONFIG, DataType);
	}
}

//	Load item color
bool CECGame::LoadItemColor()
{
	AScriptFile sf;

	//	Load item color table
	if (sf.Open("Configs\\item_color.txt"))
	{
		while (sf.PeekNextToken(true))
		{
			//	Get item id
			int idItem = sf.GetNextTokenAsInt(true);
			//	Get color index
			BYTE byCol = (BYTE)sf.GetNextTokenAsInt(false);

			if (!m_ItemColTab.put(idItem, byCol))
			{
				//	Property id collision, shouldn't happen
	//			ASSERT(0);
			}
		}

		sf.Close();
		return true;
	}
	else
	{
		a_LogOutput(1, "CECGame::LoadItemColor, failed to open file item_color.txt");
		return false;
	}
}


bool CECGame::LoadConfigs( void )
{
	//open ini file.
	AIniFile iniFile;
	if( !iniFile.Open("Configs\\ip_map.cfg") )
	{
		a_LogOutput(1, "Parse ini file, Can not open the file!");
		return false;
	}
	//get the config data.
	m_ConfigMap["type"] = iniFile.GetValueAsString( "config", "type" );
	m_ConfigMap["port"] = iniFile.GetValueAsString( "config", "port" );
	m_ConfigMap["address"] = iniFile.GetValueAsString( "config", "address" );
	m_ConfigMap["so_sndbuf"] = iniFile.GetValueAsString( "config", "so_sndbuf" );
	m_ConfigMap["so_rcvbuf"] = iniFile.GetValueAsString( "config", "so_rcvbuf" );
	m_ConfigMap["tcp_nodelay"] = iniFile.GetValueAsString( "config", "tcp_nodelay" );
	m_ConfigMap["accumulate"] = iniFile.GetValueAsString( "config", "accumulate" );
	m_ConfigMap["debug"] = iniFile.GetValueAsString( "config", "debug" );
	m_ConfigMap["tooltype"] = iniFile.GetValueAsString( "config", "tooltype" );
	if(m_ConfigMap["tooltype"]=="xunbao")
		m_iToolType = TT_XUNBAO;
	else if(m_ConfigMap["tooltype"]=="zhuti")
		m_iToolType = TT_ZHUTI;
	
	//
	//close file.
	iniFile.Close();

	return true;
}

bool CECGame::LoadIPMapTab()
{
	AScriptFile sf;

	if (sf.Open("Configs\\ip_map.cfg"))
	{
		while (sf.PeekNextToken(true))
		{
			AString strDN, strIP;

			//	Get domain name
			sf.GetNextToken(true);
			strDN = sf.m_szToken;

			//	Get ip address
			sf.GetNextToken(false);
			strIP = sf.m_szToken;

			m_IPMap[strDN] = strIP;
		}

		sf.Close();
	}

	AScriptFile sf1;

	if (sf1.Open("userdata\\ip_map.txt"))
	{
		while (sf1.PeekNextToken(true))
		{
			AString strDN, strIP;

			//	Get domain name
			sf1.GetNextToken(true);
			strDN = sf1.m_szToken;

			//	Get ip address
			sf1.GetNextToken(false);
			strIP = sf1.m_szToken;

			m_IPMap[strDN] = strIP;
		}

		sf1.Close();
	}

	SaveIPMapTab();
	return true;
}

void CECGame::SaveIPMapTab()
{
	FILE* fp = fopen("userdata\\ip_map.txt", "wb");

	if (fp)
	{
		IPMap::iterator it = m_IPMap.begin();

		for (; it != m_IPMap.end(); ++it)
		{
			AString str = it->first;
			str += " ";
			str += it->second;
			str += "\r\n";
			fwrite((const char*)str, 1, str.GetLength(), fp);
		}

		fclose(fp);
	}
}
/*
bool CECGame::LoadCutsceneTab()
{
	abase::vector<CScriptValue> args;
	abase::vector<CScriptValue> results;
	CScriptValue val;

	CLuaState *pState = g_LuaStateMan.GetAIState();
	if (!pState)
	{
		ASSERT(0);
		return false;
	}

	int i = 0;
	while(i<MAXNUM_CUTSCENE)		
	{
		val.SetVal((double)(i+1));
		args.clear();
		args.push_back(val);
		pState->Call(ONLYSHOWONCE_TBL_NAME, ("GetOnlyShowOnceCutscenes"), args, results);
		if( results.size()<=0 || CScriptValue::SVT_NUMBER!=results[0].m_Type)
		{
			a_LogOutput(1, ("CECGame::LoadCutsceneTab(), error cutscene index %d\n"), i);
			return false;
		}

		m_iCutsceneTab[i] = results[0].GetInt();
		if(m_iCutsceneTab[i] == 0)
			break;

		i++;		
	}

	return true;
}

void CECGame::SaveCutsceneTab()
{

}
*/
AString CECGame::GetIPAddressByName(const char* szName)
{
	IPMap::iterator it = m_IPMap.find(szName);

	if (it == m_IPMap.end())
		return AString();
	else
		return it->second;
}

void CECGame::AddIPAddressByName(const char* szName, const char* szAddr)
{
	m_IPMap[szName] = szAddr;
	SaveIPMapTab();
}

bool CECGame::LoadDebugCmdShortcut()
{
	AScriptFile sfile;
	if (!sfile.Open("cmds.txt"))
		return false;

	while (sfile.PeekNextToken(true))
	{
		AString strKey, strValue;
		
		sfile.GetNextToken(true);
		strKey = sfile.m_szToken;

		//	Get ip address
		sfile.GetNextToken(false);
		strValue = sfile.m_szToken;

		m_DebugCmd[strKey] = strValue;
	}

	sfile.Close();
	return true;
}

void CECGame::SaveDebugCmdShortcut()
{

}
/*
AString CECGame::GetDebugCmdByName(const char* szName)
{
	DebugCmdMap::iterator it = m_DebugCmd.find(szName);

	if (it == m_DebugCmd.end())
		return AString();
	else
		return it->second;
}

int CECGame::GetCutsceneTabByIndex(int idCutscene)
{
	for(int i=0;i<MAXNUM_CUTSCENE; i++)
	{
		if(m_iCutsceneTab[i] == idCutscene)
			return i;
	}

	return -1;
}
*/
//	Get item name color
int CECGame::GetItemNameColorIdx(int tid, int iDefIndex/* 0 */)
{
	int iIndex = iDefIndex;

	ItemColTable::pair_type Pair = m_ItemColTab.get(tid);
	if (Pair.second)
		iIndex = *Pair.first;

	if (iIndex < 0 || iIndex >= 10)
	{
		ASSERT(iIndex >= 0 && iIndex < 10);
		iIndex = 0;
	}

	return iIndex;
}

DWORD CECGame::GetItemNameColor(int tid, int iDefIndex/* 0 */)
{
	int iIndex = GetItemNameColorIdx(tid, iDefIndex);
	const ACHAR* pDesc = GetItemDesc()->GetWideString(iIndex + ITEMDESC_COL_WHITE);
	DWORD color = 0xffffffff;

	if (pDesc)
	{
		a_sscanf(pDesc, _AL("^%x"), &color);
	}

	return color | 0xff000000;
}
/*
//	Reset privilege
bool CECGame::ResetPrivilege(void* pBuf, int iDataSize)
{
	using namespace GNET;

	if (!m_pPrivilege)
	{
		if (!(m_pPrivilege = new Privilege))
			return false;
	}

	if (pBuf && iDataSize)
		m_pPrivilege->Init(pBuf, iDataSize);
	else
		m_pPrivilege->Reset();

	return true;
}

//	Render high-light model
bool CECGame::RenderHighLightModel(CECViewport* pView, CECModel* pModel)
{
	if (!pModel || !pModel->GetA3DSkinModel())
		return false;

	A3DSkinModel* pSkinModel = pModel->GetA3DSkinModel();

	A3DCOLORVALUE col = pSkinModel->GetExtraEmissive();
	pSkinModel->SetExtraEmissive(A3DCOLORVALUE(1.0f));
	pModel->Render(pView->GetA3DViewport(), true);
	pSkinModel->SetExtraEmissive(col);

	//	A3DSkinRender* psr = m_pA3DEngine->GetA3DSkinMan()->GetCurSkinRender();
	//	A3DTEXTUREOP colOP = psr->GetColorOP();
	//	psr->SetColorOP(A3DTOP_MODULATE2X);
	//	pModel->Render(pView, true);
	//	psr->SetColorOP(colOP);
	return true;
}

bool CECGame::RenderHighLightModel(CECViewport* pView, A3DSkinModel* pModel)
{
	if (!pModel)
		return false;

	A3DCOLORVALUE col = pModel->GetExtraEmissive();
	pModel->SetExtraEmissive(A3DCOLORVALUE(1.0f));
	pModel->RenderAtOnce(pView->GetA3DViewport(), 0);
	pModel->SetExtraEmissive(col);
	

	//	A3DSkinRender* psr = m_pA3DEngine->GetA3DSkinMan()->GetCurSkinRender();
	//	A3DTEXTUREOP colOP = psr->GetColorOP();
	//	psr->SetColorOP(A3DTOP_MODULATE2X);
	//	pModel->RenderAtOnce(pView);
	//	psr->SetColorOP(colOP);
	return true;
}
*/
//	Get element data path through path id
const char* CECGame::GetDataPath(DWORD id)
{
	return m_pDataPathMan->GetPathByID(id);
}
/*
int CECGame::GetLUAConfigVersion()
{
	return m_iLuaVersion;
}
*/
bool CECGame::LoadValuableItemMap()
{
	AScriptFile sf;
	//	Load valuable item table
	if (sf.Open("Configs\\item_valuable.txt"))
	{
		while (!sf.IsEnd())
		{
			//	Get item id
			int idItem = sf.GetNextTokenAsInt(true);
			m_ValuableItemMap[idItem] = true;
		}
		sf.Close();
		return true;
	}
	else
	{
		a_LogOutput(1, "CECGame::LoadValuableItem, failed to open file item_valuable.txt");
		return false;
	}
}

bool CECGame::LoadPocketableItemMap()
{
	AScriptFile sf;

	if(sf.Open("Configs\\item_pocketable.txt"))
	{
		while(!sf.IsEnd())
		{
			int idItem = sf.GetNextTokenAsInt(true);
			m_PocketableItemMap[idItem] = true;
		}
		sf.Close();
		return true;
	}
	else
	{
		a_LogOutput(1, "CECGame::LoadPocketableItemMap, failed to open file item_pocketable.txt");
		return false;
	}
}

bool CECGame::LoadPack2PocketItemMap()
{
	AScriptFile sf;

	if(sf.Open("Configs\\item_pack2pocket.txt"))
	{
		while(!sf.IsEnd())
		{
			int idItem = sf.GetNextTokenAsInt(true);
			m_Pack2PocketItemMap[idItem] = true;
		}
		sf.Close();
		return true;
	}
	else
	{
		a_LogOutput(1, "CECGame::LoadPack2PocketItemMap, failed to open file item_pack2pocket.txt");
		return false;
	}
}
bool CECGame::LoadAutoPickItemMap()
{
	AIniFile iniFile;
	if(iniFile.Open("configs\\AutoPick.ini"))
	{
		int nNum = 0;
		int i = 0;
		nNum = iniFile.GetValueAsInt("Qizhen", "Number", nNum);
		
		for(i = 0; i < nNum; i++)
		{
			AString strChannelNameKey;
			AString strChannelURLKey;
			int tid =0, level = 0;
			strChannelNameKey.Format("id%d", i);
			strChannelURLKey.Format("level%d", i);

			tid = iniFile.GetValueAsInt( "Qizhen", strChannelNameKey, tid);
			level = iniFile.GetValueAsInt( "Qizhen", strChannelURLKey, level);
			m_AutoPickItemMap[0][tid] = level;
		}
		nNum = iniFile.GetValueAsInt("Yuanliao", "Number", nNum);
		for( i = 0; i < nNum; i++)
		{
			AString strChannelNameKey;
			AString strChannelURLKey;
			int tid = 0, level =0;
			strChannelNameKey.Format("id%d", i);
			strChannelURLKey.Format("level%d", i);

			tid = iniFile.GetValueAsInt("Yuanliao", strChannelNameKey, tid);
			level = iniFile.GetValueAsInt("Yuanliao", strChannelURLKey, level);
			m_AutoPickItemMap[1][tid] = level;
		}
		nNum = iniFile.GetValueAsInt("Qianghuafu", "Number", nNum);
		for( i = 0; i < nNum; i++)
		{
			AString strChannelNameKey;
			AString strChannelURLKey;
			int tid = 0, level =0;
			strChannelNameKey.Format("id%d", i);
			strChannelURLKey.Format("level%d", i);

			tid = iniFile.GetValueAsInt("Qianghuafu", strChannelNameKey, tid);
			level = iniFile.GetValueAsInt("Qianghuafu", strChannelURLKey, level);
			m_AutoPickItemMap[2][tid] = level;
		}
		nNum = iniFile.GetValueAsInt("Guanzhufu", "Number", nNum);
		for( i = 0; i < nNum; i++)
		{
			AString strChannelNameKey;
			AString strChannelURLKey;
			int tid = 0, level =0;
			strChannelNameKey.Format("id%d", i);
			strChannelURLKey.Format("level%d", i);

			tid = iniFile.GetValueAsInt("Guanzhufu", strChannelNameKey, tid);
			level = iniFile.GetValueAsInt("Guanzhufu", strChannelURLKey, level);
			m_AutoPickItemMap[3][tid] = level;
		}
	}
	return true;
}
/*
// 物品是否是高价值物品
bool CECGame::IsValuableItem(int tid)
{
	ValuableItemMap::iterator it = m_ValuableItemMap.find(tid);
	if(it != m_ValuableItemMap.end())
		return true;
	return false;
}

// 物品是否可以放入随身包裹中
bool CECGame::IsPocketableItem(int tid)
{
	PocketableItemMap::iterator it = m_PocketableItemMap.find(tid);
	if( it != m_PocketableItemMap.end())
		return true;
	return false;
}

bool CECGame::IsPack2PocketItem(int tid)
{
	PocketableItemMap::iterator it = m_Pack2PocketItemMap.find(tid);
	if( it != m_Pack2PocketItemMap.end())
		return true;
	return false;
}

int CECGame::GetAutoPickItemLevel(int idx, int tid)
{
	if( idx >=4 )
		return 0;
	AutoPickItemMap::iterator it = m_AutoPickItemMap[idx].find(tid);
	if( it!= m_AutoPickItemMap[idx].end())
		return it->second;
	else
		return 0;
}
*/
