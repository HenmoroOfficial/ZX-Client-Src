/*
 * FILE: EC_GameRun.cpp
 *
 * DESCRIPTION: 
 *
 * CREATED BY: Duyuxin, 2004/8/27
 *
 * HISTORY: 
 *
 * Copyright (c) 2004 Archosaur Studio, All Rights Reserved.
 */

//	For WM_MOUSEWHEEL message
#pragma warning (disable: 4284)

#define _WIN32_WINDOWS	0x0500

#include "EC_Global.h"
#include "EC_GameRun_HintTool.h"
#include "EC_UIManager_HintTool.h"
//#include "EC_LoginUIMan.h"
#include "EC_Game_HintTool.h"
#include "EC_GameUIMan_HintTool.h"
//#include "EC_InputCtrl.h"
#include "EC_RTDebug.h"
#include "EC_HostPlayer_HintTool.h"
#include "EC_GameSession_HintTool.h"
/*#include "EC_GFXCaster.h"
#include "EC_Resource.h"
#include "EC_Counter.h"
#include "EC_Viewport.h"
*/#include "EC_World.h"
/*#include "EC_Utility.h"
#include "EC_Configs.h"
#include "EC_ManPlayer.h"
#include "EC_ManNPC.h"
#include "EC_ManMatter.h"
#include "EC_ManOrnament.h"
#include "EC_ManMessage.h"
#include "EC_ManDecal.h"
#include "EC_LoadProgress.h"
#include "EC_FullGlowRender.h"
#include "EC_SimpleBloomRender.h"
#include "EC_ScreenRender.h"
#include "EL_BackMusic.h"
#include "EL_Forest.h"
#include "EL_Grassland.h"
#include "EC_SceneLoader.h"
#include "EC_SceneBlock.h"
*/#include "EC_FixedMsg.h"
/*#include "EC_SoundCache.h"
#include "EL_CloudManager.h"
#include "EC_TeamMan.h"
#include "EC_Instance.h"
#include "EC_ShortcutSet.h"
#include "EC_Shortcut.h"
#include "EC_SunMoon.h"
#include "EC_Clipboard.h"
#include "EC_Faction.h"
#include "EC_ImageRes.h"
#include "EC_HostInputFilter.h"
#include "EC_LoginPlayer.h"
#include "A3DSkillGfxComposer.h"
#include "EC_ManSkillGfx.h"
#include "ElementSkill.h"
*/#include "elementdataman.h"
/*#include "defence/defence.h"
#include "acremotecode.hpp"
#include "acreport.hpp"
*/#include "EC_Inventory.h"
//#include "EC_Team.h"
#include "EC_IvtrItem.h"
/*#include "EC_AutoMove.h"
#include "DlgReplay.h"
#include "EC_CDR.h"
#include "EC_Cutscene.h"

#include "gnetdef.h"
#include "roleinfo"
*/
#include "AFI.h"
#include "A3DEngine.h"
#include "A3DCamera.h"
#include "A3DConfig.h"
#include "A3DLight.h"
#include "A3DSkinRender.h"
#include "A3DSkinMan.h"
#include "A3DWireCollector.h"
#include "A3DFlatCollector.h"
#include "A3DGFXExMan.h"
#include "A3DSkinMeshMan.h"
#include "A3DTrackMan.h"
#include "A3DTerrain2.h"
#include "A3DFont.h"
#include "A3DViewport.h"
#include "AFilePackage.h"
#include "AWString.h"
#include "AWScriptFile.h"
#include "A3DSurface.h"
#include "AM3DSoundDevice.h"
#include "A3DGDI.h"
#include "globaldataman.h"
/*#include "EC_GameRecord.h"
#include "shlwapi.h"
#include "defence\cheaterkiller.h"
#include "defence\imagechecker.h"
#include "EC_Helper.h"
*/#include "LuaState.h"
#include "EC_Secretary.h"
#include "LuaEvent.h"

#define new A_DEBUG_NEW

///////////////////////////////////////////////////////////////////////////
//	
//	Define and Macro
//	
///////////////////////////////////////////////////////////////////////////

#define TIME_TICKANIMATION	30
#define TIME_UPDATEEAR		100

//	User configs data version
#define USERCFG_VERSION		3
#define CHECK_RECORD_DELAY	10000
#define RECORD_FADE_TIME	1000

///////////////////////////////////////////////////////////////////////////
//	
//	Reference to External variables and functions
//	
///////////////////////////////////////////////////////////////////////////

/*extern bool g_bFocused;
extern game_record gr;
extern bool _start_replay(const char* szFile);

int get_record_item_pos()
{
	CECHostPlayer* pHost = g_pGame->GetGameRun()->GetHostPlayer();

	if (!pHost)
		return -1;

	CECInventory* pPack = pHost->GetPack();

	if (!pPack)
		return -1;

	int nCount = pPack->GetSize();

	for (int i = 0; i < pPack->GetSize(); i++)
	{
		CECIvtrItem* pItem = pPack->GetItem(i);

		if (pItem && pItem->GetClassID() == CECIvtrItem::ICID_CAMRECORDER)
			return i;
	}

	return -1;
}

bool begin_save_game_record()
{
	int item_pos = get_record_item_pos();

	if (item_pos < 0)
		return false;

	time_t t = time(0);
	tm cur_tm = *localtime(&t);
	char save_path [MAX_PATH];
	sprintf(save_path, "userdata\\gamesave\\%d-%02d-%02d %02d-%02d-%02d.zgr", cur_tm.tm_year + 1900, cur_tm.tm_mon + 1, cur_tm.tm_mday, cur_tm.tm_hour, cur_tm.tm_min, cur_tm.tm_sec);

	if (gr.begin_save(save_path, g_pGame->GetGameBuild()))
	{
		c2s_SendCmdCheckRecord(item_pos);
		gr.set_check_time(gr.get_total_time());
		g_pGame->GetGameRun()->AddFixedMessage(FIXMSG_RECORD_START, (const ACHAR*)AS2AC(save_path));
		return true;
	}

	gr.release();
	gr.set_state(game_record::enum_gr_disable);
	return false;
}

bool save_edited_record(const wchar_t* title)
{
	time_t t = time(0);
	tm cur_tm = *localtime(&t);
	char save_path [MAX_PATH];
	sprintf(save_path, "userdata\\gamesave\\%d-%02d-%02d %02d-%02d-%02d.zgr", cur_tm.tm_year + 1900, cur_tm.tm_mon + 1, cur_tm.tm_mday, cur_tm.tm_hour, cur_tm.tm_min, cur_tm.tm_sec);

	if (gr.save_edited_record(save_path, title))
	{
		g_pGame->GetGameRun()->AddFixedMessage(FIXMSG_RECORD_START, (const ACHAR*)AS2AC(save_path));
		return true;
	}

	return false;
}

///////////////////////////////////////////////////////////////////////////
//	
//	Local Types and Variables and Global variables
//	
///////////////////////////////////////////////////////////////////////////

//	Game statistic
struct EC_STATISTIC
{
	//	Data size counter
	int		iMeshSize;			//	Skin mesh size
	int		iAllTexSize;		//	Total texture size
	int		iStrmDSize;			//	Dynamic stream size
	int		iStrmSSize;			//	Static stream size
	int		iStrmBSize;			//	Backup stream size
	int		iTrackSize;			//	Track data size
	int		iTrnDataSize;		//	Terrain data size
	int		iSndCacheSize;		//	Current cached sound size

	//	Time counter
	int		iTickTime;			//	Total tick time
	int		iRenderTime;		//	Total render time

	//	Object counter
	int		iNumPlayer;			//	Else player number
	int		iNumNPC;			//	NPC number
	int		iNumMatter;			//	Matter number
	int		iNumScnGFX;			//	Scene GFX number
	int		iNumOnmt;			//	Ornament number
	int		iNumScnModel;		//	Scene ECModel
	float	fSndCacheHit;		//	Sound cache hit percent
	int		iNumTrnBlock;		//	Visible terrain block counter

	//	Memory statistics
	int		iMemPeak;			//	Peak memory size
	DWORD	dwMemAllocCnt;		//	Memory allocate counter
	int		iMemCurSize;		//	Current allocated memory size
	int		iMemRawSize;		//	Current allocated raw memory size
	int		aSMemBlkSize[16];	//	Small memory block size
	int		aSMemSize[16];		//	Small memory allocate size
	int		aSMemBlkCnt[16];	//	Small memory total block counter
	int		aSMemFreeCnt[16];	//	Small memory free block 
	int		iSMemBlkCnt;		//	Small memory block counter
	int		iSMemAllocSize;		//	Allocated small memory size
	int		iLMemBlkCnt;		//	Large memory block counter
	int		iLMemAllocSize;		//	Allocated large memory size
};

//	Frame controller
struct FRAMECTRL
{
	int		iTickCnt;		//	Tick counter
	int		iRenderCnt;		//	Render counter
	int		iTickTime;		//	Accumulate tick time
	int		iAvgRdTime;		//	Average render time
	int		iRenderTime;	//	Accumulate render time
};

static CECCounter		l_SaveCfgCnt;	//	Time counter used to save UI configs
static GNET::RoleInfo	l_SelRoleInfo;	//	Selected character's role info.
static EC_STATISTIC		l_Statistic;	//	Game statistic
static CECCounter		l_StatCnt;		//	Time counter used by statistic
static FRAMECTRL		l_fc;			//	Frame controller
static CECCounter		l_RecordFadeCnt;

///////////////////////////////////////////////////////////////////////////
//	
//	Local functions
//	
///////////////////////////////////////////////////////////////////////////

enum
{
	enumFadeBlack,
	enumFadeWhite,
};

void RecordDrawFade(A3DDevice* pA3DDevice, int alpha, int mode)
{
	if (mode == enumFadeWhite)
	{
		A3DDEVFMT fmt = pA3DDevice->GetDevFormat();
		A3DRECT rc;
		rc.left = 0;
		rc.right = fmt.nWidth;
		rc.top = 0;
		rc.bottom = fmt.nHeight;

		A3DCOLOR cl = A3DCOLORRGBA(alpha, alpha, alpha, alpha);
		pA3DDevice->SetSourceAlpha(A3DBLEND_DESTCOLOR);
		pA3DDevice->SetDestAlpha(A3DBLEND_ONE);
		g_pA3DGDI->Draw2DRectangle(rc, cl);
		g_pA3DGDI->Draw2DRectangle(rc, cl);

		cl = A3DCOLORRGBA(255, 255, 255, alpha);
		pA3DDevice->SetSourceAlpha(A3DBLEND_SRCALPHA);
		pA3DDevice->SetDestAlpha(A3DBLEND_ONE);
		g_pA3DGDI->Draw2DRectangle(rc, cl);

		pA3DDevice->SetDestAlpha(A3DBLEND_INVSRCALPHA);
	}
	else
	{
		A3DCOLOR cl = A3DCOLORRGBA(0, 0, 0, alpha);
		A3DDEVFMT fmt = pA3DDevice->GetDevFormat();
		A3DRECT rc;
		rc.left = 0;
		rc.right = fmt.nWidth;
		rc.top = 0;
		rc.bottom = fmt.nHeight;
		g_pA3DGDI->Draw2DRectangle(rc, cl);
	}
}

//	Draw fade color to the whole client rect
inline void DrawFadeColor(A3DDevice* pA3DDevice, int alpha, A3DCOLOR dwDestColor)
{
	A3DDEVFMT fmt = pA3DDevice->GetDevFormat();
	A3DRECT rc;
	rc.left = 0;
	rc.right = fmt.nWidth;
	rc.top = 0;
	rc.bottom = fmt.nHeight;

	A3DCOLOR cl = A3DCOLORRGBA(A3DCOLOR_GETRED(dwDestColor), A3DCOLOR_GETGREEN(dwDestColor), A3DCOLOR_GETBLUE(dwDestColor), alpha);
	g_pA3DGDI->Draw2DRectangle(rc, cl);
}

//	Blink the screen using dwBlinkColor when HostPlayer is under wicked attack (Current usage)
//	Alpha from 0 -> nMaxAlpha, then from nMaxAlpha -> 0
//	Default half-cycle time is 1000ms
void BlinkScreen(A3DDevice* pA3DDevice, bool bBlink, DWORD dwTick, const int nMaxAlpha, A3DCOLOR dwBlinkColor)
{
	static bool bEnded = true;
	if (bBlink || !bEnded)
	{
		static bool bFadeDir = false;
		static DWORD dwFadeCounter = 0;
		dwFadeCounter += dwTick;
			
		if (dwFadeCounter >= 1000)
		{
			bFadeDir = !bFadeDir;
			dwFadeCounter = 0;
		}

		int a = nMaxAlpha * dwFadeCounter / 1000;
		a_ClampRoof(a, nMaxAlpha);

		if (bFadeDir)
			a = nMaxAlpha - a;

		bEnded = false;
		if (a <= 10 && !bBlink)
			bEnded = true;

		DrawFadeColor(pA3DDevice, a, dwBlinkColor);
	}
}
*/
///////////////////////////////////////////////////////////////////////////
//	
//	Implement CECGameRun
//	
///////////////////////////////////////////////////////////////////////////

CECGameRun::CECGameRun() : 
m_ID2NameTab(1024),
m_Name2IDTab(1024),
m_InstTab(64)
{
	m_pUIManager	= NULL;
	m_pWorld		= NULL;
/*	m_pInputCtrl	= NULL;
	m_pInputFilter	= NULL;
	m_iGameState	= GS_NONE;
	m_dwRenderTime	= 0;
	m_bUIHasCursor	= false;
	m_iCfgDataSize	= 0;
	m_pCfgDataBuf	= NULL;
	m_iLogoutFlag	= -1;
*/	m_pHostPlayer	= NULL;
/*	m_pTeamMan		= NULL;
	m_pNormalSCS	= NULL;
	m_pTeamSCS		= NULL;
	m_pTradeSCS		= NULL;
	m_pPoseSCS		= NULL;
	m_pFactionSCS	= NULL;
	m_pMessageMan	= NULL;
	m_pClipboard	= NULL;
	m_iDblExpMode	= 0;
	m_iDExpEndTime	= 0;
	m_iAvaDExpTime	= 0;
	m_bLockGamePt	= false;
	m_pSkillGfxComposerMan = NULL;
	m_nCurLine		= 0;
	m_nGShopTimeStamp = 0;
	m_bNoPKLine		= false;
	m_bPVPMode		= false;
	m_nCityOwnerFaction = 0;
	m_nCityOwnerMaster = 0;
	m_nCityOccupyTime = 0;
	m_bMasterFlag	= false;
	m_bFlashWindow	= false;
	m_bIsFirstFlash = true;
	m_bHideElsePlayer	= false;
//	m_bIsTray		= false;
	m_nWallowStage = WALLOW_STAGE0;
	m_nWallowTime = 0;
	
	m_pLoadProgress	= NULL;
	m_pFullGlowRender = NULL;
	m_pSBRender		= NULL;
	m_pScreenRender	= NULL;
	m_pECHelper		= NULL;

	//	Set time counters
	l_SaveCfgCnt.SetPeriod(1200000);
	l_StatCnt.SetPeriod(1000);
	l_RecordFadeCnt.SetPeriod(RECORD_FADE_TIME);

	memset(&l_Statistic, 0, sizeof (l_Statistic));
	memset(&l_fc, 0, sizeof (l_fc));
	memset(&m_WallowInfo, 0, sizeof(m_WallowInfo));*/
}

CECGameRun::~CECGameRun()
{

}

//	Initialize object
bool CECGameRun::Init()
{
	//	Create full glow render
/*	m_pFullGlowRender = new CECFullGlowRender();
	if( !m_pFullGlowRender->Init(g_pGame->GetA3DDevice()) )
	{
		glb_ErrorOutput(ECERR_NOTENOUGHMEMORY, "CECGameRun::Init", __LINE__);
		m_pFullGlowRender->Release();
		return false;
	}

	if( !m_pFullGlowRender->CanDoFullGlow() )
	{
		// we have to switch to simple bloom render
		m_pSBRender = new CECSimpleBloomRender();
		if( !m_pSBRender->Init(g_pGame->GetA3DDevice()) )
		{
			glb_ErrorOutput(ECERR_NOTENOUGHMEMORY, "CECGameRun::Init", __LINE__);
			m_pSBRender->Release();
			return false;
		}
	}
*/
	/*
	m_pScreenRender = new CECScreenRender();
	if( !m_pScreenRender->Init(g_pGame->GetA3DDevice()) )
	{
		glb_ErrorOutput(ECERR_NOTENOUGHMEMORY, "CECGameRun::Init", __LINE__);
		m_pScreenRender->Release();
		return false;
	}*/

/*	if( m_pFullGlowRender->CanDoFullGlow() )
	{
		m_pFullGlowRender->SetGlowOn(g_pGame->GetConfigs()->GetSystemSettings().bFullGlow);
		m_pFullGlowRender->SetBloomOn(true, NULL, NULL);
		m_pFullGlowRender->SetWarpOn(g_pGame->GetConfigs()->GetSystemSettings().bSpaceWarp);
		m_pFullGlowRender->SetFlareOn(g_pGame->GetConfigs()->GetSystemSettings().bSunFlare);
	}
	
	//	Load instance information
	if (!LoadInstanceInfo("Configs\\instance.txt"))
	{
		glb_ErrorOutput(ECERR_FAILEDTOCALL, "CECGameRun::Init", __LINE__);
		return false;
	}

	//	Create UI manager
*/	if (!(m_pUIManager = new CECUIManager))
	{
		glb_ErrorOutput(ECERR_NOTENOUGHMEMORY, "CECGameRun::Init", __LINE__);
		return false;
	}

	if (!m_pUIManager->Init(g_pGame->GetA3DEngine(), -1))
	{
		a_LogOutput(1, "CECGameRun::Init, Failed to initialize UI manager.");
		return false;
	}

	//	Create input controller
/*	if (!(m_pInputCtrl = new CECInputCtrl))
	{
		glb_ErrorOutput(ECERR_NOTENOUGHMEMORY, "CECGameRun::Init", __LINE__);
		return false;
	}

	//	Create host input filter
	if (!(m_pInputFilter = new CECHostInputFilter(m_pInputCtrl)))
	{
		glb_ErrorOutput(ECERR_NOTENOUGHMEMORY, "CECGameRun::Init", __LINE__);
		return false;
	}

	//	Clipboard
	if (!(m_pClipboard = new CECClipboard))
	{
		glb_ErrorOutput(ECERR_NOTENOUGHMEMORY, "CECGameRun::Init", __LINE__);
		return false;
	}

	// helper
	if (!(m_pECHelper = new CECHelper))
	{
		glb_ErrorOutput(ECERR_NOTENOUGHMEMORY, "CECGameRun::Init", __LINE__);
		return false;
	}
	if (!m_pECHelper->Init())
	{
		a_LogOutput(1, "CECGameRun::Init, Failed to initialize ECHelper.");
		//return false;  
		//let the game go on
		delete m_pECHelper;
		m_pECHelper = NULL;
	}

	if(!(m_pSecretary = new CECSecretary))
	{
		glb_ErrorOutput(ECERR_NOTENOUGHMEMORY, "CECGameRun::Init", __LINE__);
		return false;
	}
	if (!m_pSecretary->Init())
	{
		a_LogOutput(1, "CECGameRun::Init, Failed to initialize ECSecretary.");
		//return false;  
		//let the game go on
		delete m_pSecretary;
		m_pSecretary = NULL;
	}

	//load automove map transfer info
	g_ECAutoMove.LoadTransMap("configs\\transfer_map.cfg");

	
	g_pGame->ChangeCursor(RES_CUR_NORMAL);
	g_pGame->ShowCursor(true);
*/
	//New Added by Ford.W 2010-07-22. Just for compile, no other usage.
	m_pHostPlayer = new CECHostPlayer;
	if( !m_pHostPlayer )
	{
		return false;
	}
	S2C::cmd_self_info_1 Info;
	m_pHostPlayer->Init( Info );
	//Added end.
	return true;
}

//	Release object
void CECGameRun::Release()
{
	//	Release resources
//	EndGameState(false);

/*	if (m_pCfgDataBuf)
	{
		a_free(m_pCfgDataBuf);
		m_pCfgDataBuf = NULL;
	}

*/	if (m_pUIManager)
	{
		m_pUIManager->Release();
		delete m_pUIManager;
		m_pUIManager = NULL;
	}

/*	delete m_pInputFilter;
	m_pInputFilter = NULL;

	if (m_pInputCtrl)
	{
		m_pInputCtrl->Release();
		delete m_pInputCtrl;
		m_pInputCtrl = NULL;
	}

	if (m_pClipboard)
	{
		delete m_pClipboard;
		m_pClipboard = NULL;
	}

	if (m_pSBRender)
	{
		m_pSBRender->Release();
		delete m_pSBRender;
		m_pSBRender = NULL;
	}

	if (m_pScreenRender)
	{
		m_pScreenRender->Release();
		delete m_pScreenRender;
		m_pScreenRender = NULL;
	}

	if (m_pFullGlowRender)
	{
		m_pFullGlowRender->Release();
		delete m_pFullGlowRender;
		m_pFullGlowRender = NULL;
	}
	
	if (m_pECHelper)
	{
		delete m_pECHelper;
		m_pECHelper = NULL;
	}
	
	if (m_pSecretary)
	{
		delete m_pSecretary;
		m_pSecretary = NULL;
	}

*/	//	Release all instance information
	InstTable::iterator it = m_InstTab.begin();
	for (; it != m_InstTab.end(); ++it)
	{
		CECInstance* pInst = *it.value();
		delete pInst;
	}

	m_InstTab.clear();

	// Release composer man
/*	ReleaseGfxComposerMan();*/

	//Added by Ford.W 2010-07-22.
	if( m_pHostPlayer )
	{
		m_pHostPlayer->Release();
		delete m_pHostPlayer;
		m_pHostPlayer = NULL;
	}
	//Added end.

	//
	ClearNameIDPairs();
	//
}
/*
//	Init gfx composer man
void CECGameRun::InitGfxComposerMan()
{
	m_pSkillGfxComposerMan = new _SGC::A3DSkillGfxComposerMan();
}

//	Release Gfx composer man
void CECGameRun::ReleaseGfxComposerMan()
{
	if( m_pSkillGfxComposerMan )
	{
		m_pSkillGfxComposerMan->Release();
		delete m_pSkillGfxComposerMan;
		m_pSkillGfxComposerMan = NULL;
	}
}
*/
void CECGameRun::LoadSpecialIDConfig()
{
	DATA_TYPE dt;
	int idEssence = g_pGame->GetElementDataMan()->get_first_data_id(ID_SPACE_CONFIG, dt);

	while (idEssence)
	{
		if (dt == DT_SPECIAL_ID_CONFIG)
		{
			m_SpecialIDConfig = *(SPECIAL_ID_CONFIG*)g_pGame->GetElementDataMan()->get_data_ptr(idEssence, ID_SPACE_CONFIG, dt);
			break;
		}

		idEssence = g_pGame->GetElementDataMan()->get_next_data_id(ID_SPACE_CONFIG, dt);
	}
}
/*
//	Start game
bool CECGameRun::StartGame(int idInst, const A3DVECTOR3& vHostPos)
{
	//	End current game state
	EndGameState();

	memset(&m_WallowInfo, 0, sizeof(m_WallowInfo));
	m_nCityOwnerFaction = 0;
	m_nCityOwnerMaster = 0;
	m_nCityOccupyTime = 0;
	m_bMasterFlag = false;
	m_iGameState = GS_GAME;
	m_bHideElsePlayer = false;

	if (gr.get_state() == game_record::enum_gr_replay)
		g_pGame->ResetPrivilege(NULL, 0);

	if (!g_pGame->LoadInGameRes())
	{
		a_LogOutput(1, "CECGameRun::StartGame, Failed to call LoadInGameRes().");
		return false;
	}

	//	Create message manager
	if (!(m_pMessageMan = new CECMessageMan(this)))
	{
		glb_ErrorOutput(ECERR_NOTENOUGHMEMORY, "CECGameRun::StartGame", __LINE__);
		return false;
	}
	
	//	Change UI manager
	if (!m_pUIManager->ChangeCurUIManager(CECUIManager::UIMAN_LOADING))
	{
		a_LogOutput(1, "CECGameRun::StartGame, Failed to change UI manager.");
		return false;
	}

	//	Create default game world
	if (!JumpToInstance(idInst, vHostPos))
	{
		a_LogOutput(1, "CECGameRun::StartGame, Failed to create game world.");
		return false;
	}

	//	Create host player
	if (!CreateHostPlayer())
	{
		a_LogOutput(1, "CECGameRun::StartGame, Failed to create host player.");
		return false;
	}

	//	Create team manager
	if (!(m_pTeamMan = new CECTeamMan))
	{
		glb_ErrorOutput(ECERR_NOTENOUGHMEMORY, "CECGameRun::StartGame", __LINE__);
		return false;
	}

	// Reset faction manager
	g_pGame->GetFactionMan()->Release(false);

	//	Create shortcuts
	if (!CreateShortcuts())
	{
		a_LogOutput(1, "CECGameRun::StartGame, Failed to create shortcuts");
		return false;
	}

	//	Change UI manager
	if (!m_pUIManager->ChangeCurUIManager(CECUIManager::UIMAN_INGAME))
	{
		a_LogOutput(1, "CECGameRun::StartGame, Failed to change UI manager.");
		return false;
	}

	CECGameUIMan* pGameUIMan = m_pUIManager->GetInGameUIMan();
	if (pGameUIMan)
	{
		pGameUIMan->ChangeWorldInstance(idInst);

		if (gr.get_state() == game_record::enum_gr_replay)
			pGameUIMan->SetReplayMode(true);
	}

	l_SaveCfgCnt.Reset();

	//	Change cursor to default icon
	g_pGame->ChangeCursor(RES_CUR_NORMAL);
	//	Discard current frame
	g_pGame->DiscardFrame();

	//	Clear frame controller
	memset(&l_fc, 0, sizeof (l_fc));

	return true;
}

//	End game state
void CECGameRun::OnEndGameState()
{
	//	Release UI module
	m_pUIManager->ChangeCurUIManager(-1);

	//	Release shortcuts
	ReleaseShortcuts();

	//	Release team manager
	A3DRELEASE(m_pTeamMan);

	//	Release host player before world released
	ReleaseHostPlayer();

	//	Release world
	ReleaseWorld();

	//	Release message manager
	A3DRELEASE(m_pMessageMan);

	g_pGame->ReleaseInGameRes();

	//	Release all player name and ID cache
	ClearNameIDPairs();

	if (gr.get_state() == game_record::enum_gr_record)
	{
		gr.end_save();
		gr.release();
		gr.set_state(game_record::enum_gr_disable);
	}
}

//	Start login interface
bool CECGameRun::StartLogin(bool bFirst)
{
	//	End current game state
	EndGameState();

	m_iGameState = GS_LOGIN;

	//	Change UI manager
	if (!m_pUIManager->ChangeCurUIManager(CECUIManager::UIMAN_LOADING))
	{
		a_LogOutput(1, "CECGameRun::StartLogin, Failed to change UI manager.");
		return false;
	}

	if( !CreateLoginWorld(A3DVECTOR3(485.181f, 237.421f, -208.612f)) )
	{
		a_LogOutput(1, "CECGameRun::StartLogin, Failed to create login world.");
		return false;
	}

	//	Change UI manager
	if (!m_pUIManager->ChangeCurUIManager(CECUIManager::UIMAN_LOGIN))
	{
		a_LogOutput(1, "CECGameRun::StartLogin, Failed to change UI manager.");
		return false;
	}

	if (bFirst)
		m_pUIManager->GetLoginUIMan()->LaunchPreface();

	//	Change cursor to default icon
	g_pGame->ChangeCursor(RES_CUR_NORMAL);
	//	Discard current frame
	g_pGame->DiscardFrame();

	return true;
}

//	End login state
void CECGameRun::OnEndLoginState()
{
	//	Release UI module
	m_pUIManager->ChangeCurUIManager(-1);

	//  Release World
	ReleaseLoginWorld();
}
*/
//	End current game state
//void CECGameRun::EndGameState(bool bReset/* true */)
/*{
	if (m_iGameState == GS_NONE)
		return;

	int iCurState = m_iGameState;
	m_iGameState = GS_NONE;

	if (iCurState == GS_LOGIN)
		OnEndLoginState();
	else if (iCurState == GS_GAME)
		OnEndGameState();

	//	Stop background sound and music
	CELBackMusic* pBackMusic = g_pGame->GetBackMusic();
	if (pBackMusic)
	{
		pBackMusic->StopMusic(true, true);
		pBackMusic->StopBackSFX();
	}

	if (bReset)
		g_pGame->Reset();
}

//	Create world
bool CECGameRun::CreateWorld(int id, const A3DVECTOR3& vHostPos)
{
	CECInstance* pInst = GetInstance(id);
	if (!pInst)
	{
		a_LogOutput(1, "CECGameRun::CreateWorld, wrong instance id: %d", id);
		return false;
	}

	if (!(m_pWorld = new CECWorld(this)))
	{
		glb_ErrorOutput(ECERR_NOTENOUGHMEMORY, "CECGameRun::CreateWorld", __LINE__);
		return false;
	}

	if (!m_pWorld->Init(id))
	{
		a_LogOutput(1, "CECGameRun::CreateWorld, Failed to initialize world.");
		return false;
	}

	//	Set scene resource loading distance
	CECSceneBlock::SetResLoadDists(g_pGame->GetConfigs()->GetSystemSettings().nSight + 1);

	char szBackPic[MAX_PATH];
	
	AString strPath = pInst->GetPath();
	if (pInst->GetID() == 12 && vHostPos.x > 355 && vHostPos.x < 512 && vHostPos.z > 353 && vHostPos.z < 512 )
		strPath += "-1";
	sprintf(szBackPic, "MapLoading\\%s.dds", strPath);

	//	Begin load progress
	if (!gr.is_seeking())
		BeginLoadProgress(CECLoadProgress::LOAD_ENTERGAME, 0, 100, szBackPic);

	//	Load scene from file
	char szFile[MAX_PATH];
	sprintf(szFile, "maps\\%s\\%s.ecwld", pInst->GetPath(), pInst->GetPath());
	char szDataPath[MAX_PATH];
	sprintf(szDataPath, "maps\\%s", pInst->GetDataPath());

	if (!m_pWorld->LoadWorld(szFile, szDataPath, vHostPos))
	{
		a_LogOutput(1, "CECGameRun::CreateWorld, Failed to load world.");
		return false;
	}

	//	End load progress
	if (!gr.is_seeking())
		EndLoadProgress();

	//	Set default fog parameters
	A3DDevice* pA3DDevice = g_pGame->GetA3DDevice();
	CECConfigs* pConfigs = g_pGame->GetConfigs();
	pA3DDevice->SetFogStart(pConfigs->GetDefFogStart());
	pA3DDevice->SetFogEnd(pConfigs->GetDefFogEnd());
	pA3DDevice->SetFogDensity(pConfigs->GetDefFogDensity());
	pA3DDevice->SetFogColor(pConfigs->GetDefFogColor());
	//	Set default light parameters
	pA3DDevice->SetAmbient(pConfigs->GetDefAmbient());
	A3DLIGHTPARAM param = g_pGame->GetDirLight()->GetLightparam();
	param.Ambient = A3DCOLORVALUE(0.0f);
	param.Diffuse = a3d_ColorRGBAToColorValue(pConfigs->GetDefLightDiff());
	param.Direction = pConfigs->GetDefLightDir();
	g_pGame->GetDirLight()->SetLightParam(param);
	SetMaxFlyHeight(pInst->GetMaxFlyHeight());

	if( m_pWorld->GetForest() )
		m_pWorld->GetForest()->SetLODLevel(pConfigs->GetSystemSettings().nTreeDetail * 0.25f);
	if( m_pWorld->GetGrassLand() )
		m_pWorld->GetGrassLand()->SetLODLevel(pConfigs->GetSystemSettings().nGrassDetail * 0.25f);
	if( m_pWorld->GetCloudManager() )
		m_pWorld->GetCloudManager()->SetCloudLevel(pConfigs->GetSystemSettings().nCloudDetail * 25 / 10);

	// adjust time of the day;
	struct tm serverLocal = g_pGame->GetServerLocalTime();
	int nTimeInDay = serverLocal.tm_hour * 3600 + serverLocal.tm_min * 60 + serverLocal.tm_sec;
	m_pWorld->GetSunMoon()->SetTimeOfTheDay(nTimeInDay / (4.0f * 3600.0f));

	if( m_pFullGlowRender->CanDoFullGlow() )
	{
		m_pFullGlowRender->SetBloomOn(true, WorldRenderForBloom, GetWorld());
	}
	return true;
}

//	Release world
void CECGameRun::ReleaseWorld()
{
	m_pInputCtrl->ClearKBFilterStack();
	m_pInputCtrl->ClearMouFilterStack();

	g_pGame->GetViewport()->SwitchCamera(false);

	if (m_pWorld)
	{
		m_pWorld->Release();
		delete m_pWorld;
		m_pWorld = NULL;
	}
}

//	Create Login World
bool CECGameRun::CreateLoginWorld(const A3DVECTOR3& vCameraPos)
{
	if (!g_pGame->LoadLoginRes())
	{
		a_LogOutput(1, "CECGameRun::CreateLoginWorld, Failed to call LoadLoginRes().");
		return false;
	}

	if (!(m_pWorld = new CECWorld(this)))
	{
		glb_ErrorOutput(ECERR_NOTENOUGHMEMORY, "CECGameRun::CreateLoginWorld", __LINE__);
		return false;
	}

	if (!m_pWorld->Init(0))
	{
		a_LogOutput(1, "CECGameRun::CreateLoginWorld, Failed to initialize world.");
		return false;
	}

	//	Set scene resource loading distance
	CECSceneBlock::SetResLoadDists(0);

	//	Begin load progress
	BeginLoadProgress(CECLoadProgress::LOAD_LOGIN, 0, 100, NULL);

	//	Load login scene from file
	if (!m_pWorld->LoadWorld("Maps\\Login\\Login.ecwld", "Maps\\Login", vCameraPos))
	{
		a_LogOutput(1, "CECGameRun::CreateLoginWorld, Failed to load world.");
		return false;
	}

	//	End load progress
	EndLoadProgress();

	g_pGame->GetViewport()->GetA3DCamera()->SetPos(vCameraPos);

	//	Set default fog parameters
	A3DDevice* pA3DDevice = g_pGame->GetA3DDevice();
	CECConfigs* pConfigs = g_pGame->GetConfigs();
	pA3DDevice->SetFogStart(pConfigs->GetDefFogStart());
	pA3DDevice->SetFogEnd(pConfigs->GetDefFogEnd());
	pA3DDevice->SetFogDensity(pConfigs->GetDefFogDensity());
	pA3DDevice->SetFogColor(pConfigs->GetDefFogColor());

	//	Set default light parameters
	pA3DDevice->SetAmbient(pConfigs->GetDefAmbient());
	A3DLIGHTPARAM param = g_pGame->GetDirLight()->GetLightparam();
	param.Ambient = A3DCOLORVALUE(0.0f);
	param.Diffuse = a3d_ColorRGBAToColorValue(pConfigs->GetDefLightDiff());
	param.Direction = pConfigs->GetDefLightDir();
	g_pGame->GetDirLight()->SetLightParam(param);

	m_pFullGlowRender->SetGlowType(FULLGLOW_TYPE_LOGIN, 0xffffffff);

	char szTheme[MAX_PATH];
	sprintf(szTheme, "music\\theme_%c.mp3", 'a');// + (GetTickCount() / 100) % 3);
	g_pGame->GetBackMusic()->PlayMusic(szTheme, false, true);
	g_pGame->GetBackMusic()->PauseMusic(true);
	
	if( g_pGame->GetGameRun()->GetWorld()->GetForest() )
		g_pGame->GetGameRun()->GetWorld()->GetForest()->SetLODLevel(1.0f);
	if( g_pGame->GetGameRun()->GetWorld()->GetGrassLand() )
		g_pGame->GetGameRun()->GetWorld()->GetGrassLand()->SetLODLevel(1.0f);
	if( g_pGame->GetGameRun()->GetWorld()->GetCloudManager() )
		g_pGame->GetGameRun()->GetWorld()->GetCloudManager()->SetCloudLevel(10);
	if( m_pFullGlowRender->CanDoFullGlow() )
	{
		m_pFullGlowRender->SetBloomOn(true, WorldRenderForBloom, GetWorld());
	}
	return true;
}

//	Release Login World
void CECGameRun::ReleaseLoginWorld()
{
	if (m_pWorld)
	{
		m_pWorld->Release();
		delete m_pWorld;
		m_pWorld = NULL;
	}

	g_pGame->ReleaseLoginRes();

	return;
}

//	Create host player
bool CECGameRun::CreateHostPlayer()
{
	ASSERT(m_pWorld);
	CECPlayerMan* pPlayerMan = m_pWorld->GetPlayerMan();

	//	Create host player
	if (!(m_pHostPlayer = new CECHostPlayer(pPlayerMan)))
	{
		glb_ErrorOutput(ECERR_NOTENOUGHMEMORY, "CECGameRun::CreateHostPlayer", __LINE__);
		return false;
	}

	return true;
}

//	Release host player
void CECGameRun::ReleaseHostPlayer()
{
	//	Release host player
	if (m_pHostPlayer)
	{
		m_pHostPlayer->Release();
		delete m_pHostPlayer;
		m_pHostPlayer = NULL;
	}
}

//	Create shortcuts
bool CECGameRun::CreateShortcuts()
{
	//	Normal command shortcut set
	m_pNormalSCS = new CECShortcutSet;
	m_pNormalSCS->Init(8);

	CECSCCommand* pSC = new CECSCCommand(CECSCCommand::CMD_NORMALATTACK);
	m_pNormalSCS->SetShortcut(0, pSC);
	pSC = new CECSCCommand(CECSCCommand::CMD_WALKRUN);
	m_pNormalSCS->SetShortcut(1, pSC);
	pSC = new CECSCCommand(CECSCCommand::CMD_SITDOWN);
	m_pNormalSCS->SetShortcut(2, pSC);
	pSC = new CECSCCommand(CECSCCommand::CMD_ASSISTATTACK);
	m_pNormalSCS->SetShortcut(3, pSC);
	pSC = new CECSCCommand(CECSCCommand::CMD_PICKUP);
	m_pNormalSCS->SetShortcut(4, pSC);
	pSC = new CECSCCommand(CECSCCommand::CMD_PET_CALL);
	m_pNormalSCS->SetShortcut(5, pSC);
	pSC = new CECSCCommand(CECSCCommand::CMD_PET_COMBINE1);
	m_pNormalSCS->SetShortcut(6, pSC);
	pSC = new CECSCCommand(CECSCCommand::CMD_PET_COMBINE2);
	m_pNormalSCS->SetShortcut(7, pSC);
		
	//	Team command shortcut set
	m_pTeamSCS = new CECShortcutSet;
	m_pTeamSCS->Init(2);

	pSC = new CECSCCommand(CECSCCommand::CMD_INVITETOTEAM);
	m_pTeamSCS->SetShortcut(0, pSC);
	pSC = new CECSCCommand(CECSCCommand::CMD_LEAVETEAM);
	m_pTeamSCS->SetShortcut(1, pSC);
//	pSC = new CECSCCommand(CECSCCommand::CMD_KICKTEAMMEM);
//	m_pTeamSCS->SetShortcut(2, pSC);
//	pSC = new CECSCCommand(CECSCCommand::CMD_FINDTEAM);
//	m_pTeamSCS->SetShortcut(2, pSC);

	//	Trade command shortcut set
	m_pTradeSCS = new CECShortcutSet;
	m_pTradeSCS->Init(2);

	pSC = new CECSCCommand(CECSCCommand::CMD_STARTTRADE);
	m_pTradeSCS->SetShortcut(0, pSC);
	pSC = new CECSCCommand(CECSCCommand::CMD_SELLBOOTH);
	m_pTradeSCS->SetShortcut(1, pSC);
	
	//	Pose command shortcut set
	m_pPoseSCS = new CECShortcutSet;
	m_pPoseSCS->Init(NUM_ROLEEXP);

	for (int i=0; i < NUM_ROLEEXP; i++)
	{
		pSC = new CECSCCommand(CECSCCommand::CMD_PLAYPOSE);
		pSC->SetParam(i);
		m_pPoseSCS->SetShortcut(i, pSC);
	}

	//	Faction command shortcut set
	m_pFactionSCS = new CECShortcutSet;
	m_pFactionSCS->Init(1);

	pSC = new CECSCCommand(CECSCCommand::CMD_INVITETOFACTION);
	m_pFactionSCS->SetShortcut(0, pSC);

	return true;
}

//	Release shortcuts
void CECGameRun::ReleaseShortcuts()
{
	A3DRELEASE(m_pNormalSCS);
	A3DRELEASE(m_pTeamSCS);
	A3DRELEASE(m_pTradeSCS);
	A3DRELEASE(m_pPoseSCS);
	A3DRELEASE(m_pFactionSCS);
}
*/
//	Release name/id pairs cache
void CECGameRun::ClearNameIDPairs()
{
	if (m_ID2NameTab.size())
	{
		ID2NameTable::iterator it = m_ID2NameTab.begin();
		for (; it != m_ID2NameTab.end(); ++it)
		{
			ACString* pstrName = *it.value();
			delete pstrName;
		}

		m_ID2NameTab.clear();
	}

	m_Name2IDTab.clear();
}
/*
//	Get command shortcut (except pose command) by command ID
CECSCCommand* CECGameRun::GetCmdShortcut(int iCommand)
{
	int i;

	ASSERT(m_pNormalSCS);
	for (i=0; i < m_pNormalSCS->GetShortcutNum(); i++)
	{
		CECSCCommand* pCmdSC = (CECSCCommand*)m_pNormalSCS->GetShortcut(i);
		if (pCmdSC && pCmdSC->GetCommandID() == iCommand)
			return pCmdSC;
	}

	ASSERT(m_pTeamSCS);
	for (i=0; i < m_pTeamSCS->GetShortcutNum(); i++)
	{
		CECSCCommand* pCmdSC = (CECSCCommand*)m_pTeamSCS->GetShortcut(i);
		if (pCmdSC && pCmdSC->GetCommandID() == iCommand)
			return pCmdSC;
	}

	ASSERT(m_pTradeSCS);
	for (i=0; i < m_pTradeSCS->GetShortcutNum(); i++)
	{
		CECSCCommand* pCmdSC = (CECSCCommand*)m_pTradeSCS->GetShortcut(i);
		if (pCmdSC && pCmdSC->GetCommandID() == iCommand)
			return pCmdSC;
	}

	ASSERT(m_pFactionSCS);
	for (i=0; i < m_pFactionSCS->GetShortcutNum(); i++)
	{
		CECSCCommand* pCmdSC = (CECSCCommand*)m_pFactionSCS->GetShortcut(i);
		if (pCmdSC && pCmdSC->GetCommandID() == iCommand)
			return pCmdSC;
	}

	return NULL;
}

//	Get pose command shortcut by pose
CECSCCommand* CECGameRun::GetPoseCmdShortcut(int iPose)
{
	ASSERT(m_pPoseSCS);
	for (int i=0; i < m_pPoseSCS->GetShortcutNum(); i++)
	{
		CECSCCommand* pCmdSC = (CECSCCommand*)m_pPoseSCS->GetShortcut(i);
		if (pCmdSC && pCmdSC->GetParam() == (DWORD)iPose)
			return pCmdSC;
	}

	return NULL;
}

//	Deal windows message
bool CECGameRun::DealWindowsMessage(UINT message, WPARAM wParam, LPARAM lParam)
{
	if (!m_pUIManager || !m_pInputCtrl)
		return false;
	
	bool  bDisableOtherKey = false;
	if (GetWorld() && 
		GetWorld()->GetHostPlayer() &&
		GetWorld()->GetHostPlayer()->GetCutscene() &&
		GetWorld()->GetHostPlayer()->GetCutscene()->IsPlayingCutscene() &&
		GetWorld()->GetHostPlayer()->GetCutscene()->IsOtherKeyDisabled())
 	{
		bDisableOtherKey = true;
	}

	if ( !bDisableOtherKey && message == WM_SYSKEYUP && wParam == VK_F10 )
	{
		if (!m_pWorld)
			return true;
		if (!m_pHostPlayer)
			return true;
		m_bHideElsePlayer = !m_bHideElsePlayer;
		LuaEvent::FirstHideElsePlayer();
		return true;
	}

	if (m_pUIManager->DealWindowsMessage(message, wParam, lParam))
	{
	//	m_bUIHasCursor = m_pUIManager->UIControlCursor();
		return true;
	}
	else
	{
//		if (message == WM_MOUSEMOVE)
//			m_bUIHasCursor = false;

		switch (message)
		{
		case WM_KEYDOWN:
		case WM_KEYUP:
			if (bDisableOtherKey && VK_ESCAPE!=wParam)
 			{
				//disable other key except Esc.
 			}
			else
			{
				if( message == WM_KEYUP && wParam == VK_SNAPSHOT )
				{
					if (!m_pWorld)
						break;

					CaptureScreen(1);
					break;
				}

				if (m_pInputCtrl->DealKeyMessage(message, wParam, lParam))
					return true;
			}

			break;

		case WM_LBUTTONDOWN:
		case WM_LBUTTONDBLCLK:
		case WM_LBUTTONUP:
		case WM_RBUTTONDOWN:
		case WM_RBUTTONDBLCLK:
		case WM_RBUTTONUP:
	//	case WM_MBUTTONDOWN:
	//	case WM_MBUTTONUP:
		case WM_MOUSEMOVE:
		case WM_MOUSEWHEEL:
			if (bDisableOtherKey)
				return true;

			if (m_pInputCtrl->DealMouseMessage(message, wParam, lParam))
				return true;

			break;
		}
	}

	return false;
}

GNET::GamedataSend gen_server_time()
{
	using namespace S2C;

	GNET::GamedataSend svrtime;
	GNET::Octets& o = svrtime.data;
	cmd_header h;
	h.cmd = SERVER_TIME;
	add_to_octets<cmd_header>(o, h);
	add_to_octets<int>(o, g_pGame->GetServerGMTTime());
	add_to_octets<int>(o, g_pGame->GetLUAConfigVersion());
	return svrtime;
}

#define RECORD_EXTRA_BOOTH_NAME		1

struct record_extra_data
{
	unsigned int	flag;
	wchar_t			host_booth_name[15];
};

GNET::Octets gen_record_extra_data()
{
	CECGameRun* pGameRun = g_pGame->GetGameRun();
	CECHostPlayer* pHost = pGameRun->GetHostPlayer();

	GNET::Marshal::OctetsStream o, o1;
	o.reserve(128);
	o1.reserve(128);
	unsigned int flag = 0;

	if (pHost->GetBoothState() == 2)
	{
		flag |= RECORD_EXTRA_BOOTH_NAME;
		wchar_t	host_booth_name[15];
		wcscpy(host_booth_name, pHost->GetBoothName());
		o1 << GNET::Octets(host_booth_name, sizeof(host_booth_name));
	}

	o << flag;
	o << o1;

	return o;
}

void apply_record_extra_data(GNET::Octets& o)
{
	using namespace GNET;

	CECGameRun* pGameRun = g_pGame->GetGameRun();
	CECHostPlayer* pHost = pGameRun->GetHostPlayer();

	try
	{
		Marshal::OctetsStream os(o), o1;
		unsigned int state;
		os >> state;
		os >> o1;

		if (state & RECORD_EXTRA_BOOTH_NAME)
		{
			GNET::Octets oBooth;
			o1 >> oBooth;
			pHost->SetBoothName((const wchar_t*)oBooth.begin());
		}
	}
	catch (Marshal::Exception &)
	{
	}
}

void record_add_chat_msg(CECGameRun* p, GNET::Octets& o)
{
	try
	{
		char cChannel;
		GNET::Octets msg;
		GNET::Marshal::OctetsStream os(o);
		os >> cChannel;
		os >> msg;
		p->AddChatMessage(ACString((ACHAR*)msg.begin(), msg.size() / sizeof(ACHAR)), cChannel);
	}
	catch (Marshal::Exception &)
	{
	}
}

void CECGameRun::StartRecordGame()
{
	if (gr.get_state() != game_record::enum_gr_disable)
		return;

	if (!m_pWorld || !m_pHostPlayer || !m_pHostPlayer->HostIsReady())
		return;

	gr.set_host_info(
		g_pGame->GetGameSession()->GetCharacterID(),
		m_pHostPlayer->GetGender(),
		m_pHostPlayer->GetFaceid(),
		m_pHostPlayer->GetHairid(),
		m_pHostPlayer->GetName(),
		a_strlen(m_pHostPlayer->GetName()) * sizeof(ACHAR));

	gr.set_state(game_record::enum_gr_record);

	if (!begin_save_game_record())
		return;

	RecordSaveKeyPoint();
}

void CECGameRun::RecordSaveKeyPoint()
{
	CECPlayerMan* pPlayerMan = m_pWorld->GetPlayerMan();
	CECNPCMan* pNPCMan = m_pWorld->GetNPCMan();
	CECMatterMan* pMatterMan = m_pWorld->GetMatterMan();

	frame_record* fr = gr.inc_frame();
	fr->set_break();
	fr->set_key_point(m_pWorld->GetInstanceID(), m_pHostPlayer->GetPos(), gr.get_record_stamp());
	gr.push_cur_frame();

	fr = gr.inc_frame();
	fr->set_break();
	fr->set_key_point2();
	fr->push_event(new event_record_protocol(&m_pHostPlayer->gen_self_info()));
	fr->push_event(new event_record_protocol(&m_pHostPlayer->gen_self_info_00()));
	fr->push_event(new event_record_protocol(&m_pHostPlayer->gen_cmd_own_ext_prop()));
	fr->push_event(new event_record_protocol(&m_pHostPlayer->gen_cmd_get_own_money()));
	fr->push_event(new event_record_protocol(&m_pHostPlayer->gen_player_cash()));
	fr->push_event(new event_record_protocol(&m_pHostPlayer->gen_pk_value()));
	fr->push_event(new event_record_protocol(&m_pHostPlayer->gen_mafia_contribution()));
	fr->push_event(new event_record_protocol(&m_pHostPlayer->gen_region_reputation()));
	fr->push_event(new event_record_protocol(&m_pHostPlayer->gen_skill_data()));
	fr->push_event(new event_record_protocol(&m_pHostPlayer->GetPack()->gen_item_detail_data(IVTRTYPE_PACK)));
	fr->push_event(new event_record_protocol(&m_pHostPlayer->GetTaskPack()->gen_item_detail_data(IVTRTYPE_TASKPACK)));
	fr->push_event(new event_record_protocol(&m_pHostPlayer->GetTrashBox()->gen_item_detail_data(IVTRTYPE_TRASHBOX)));
	fr->push_event(new event_record_protocol(&m_pHostPlayer->GetFactionTrashBox()->gen_item_detail_data(IVTRTYPE_FACTIONTRASHBOX)));
	fr->push_event(new event_record_protocol(&m_pHostPlayer->GetEquipment()->gen_item_detail_data(IVTRTYPE_EQUIPPACK)));
	fr->push_event(new event_record_protocol(&m_pHostPlayer->gen_title_list()));
	fr->push_event(new event_record_data(RECORD_DATA_PLAYER_CACHE, pPlayerMan->gen_player_cache_list()));
	fr->push_event(new event_record_protocol(&pPlayerMan->gen_player_list()));
	fr->push_event(new event_record_protocol(&pNPCMan->gen_npc_list()));
	fr->push_event(new event_record_protocol(&pMatterMan->gen_matter_list()));
	// the last event in first frame must be task data
	fr->push_event(new event_record_protocol(&m_pHostPlayer->gen_task_data()));
	gr.push_cur_frame();

	fr = gr.inc_frame();
	fr->set_break();
	fr->set_key_point2();
	SaveConfigsToServer(true);

	if (m_pHostPlayer->GetTeam())
	{
		GNET::Marshal::OctetsStream os;

		if (m_pHostPlayer->GetTeam()->marshal(os))
			fr->push_event(new event_record_data(RECORD_DATA_TEAM, os));
	}

	fr->push_event(new event_record_data(RECORD_DATA_SKILL_ADDON, m_pHostPlayer->gen_skill_addon_data()));
	GNET::Marshal::OctetsStream osFaction;

	if (g_pGame->GetFactionMan()->marshal(osFaction))
		fr->push_event(new event_record_data(RECORD_DATA_FACTION, osFaction));

	fr->push_event(new event_record_protocol(&gen_server_time()));
	fr->push_event(new event_record_data(RECORD_DATA_EXTRA_DATA, gen_record_extra_data()));

	if (m_pHostPlayer->GetSelectedTarget())
		fr->push_event(new event_record_protocol(&m_pHostPlayer->gen_sel_target()));

	gr.push_cur_frame();

	fr = gr.inc_frame();
	ECMSG msg;

	if (m_pHostPlayer->gen_record_msg(msg))
	{
		fr->set_break();
		fr->set_key_point2();
		fr->push_event(new event_record_msg(msg));
		gr.push_cur_frame();
		gr.inc_frame();
	}
}

//	Game tick routine
bool CECGameRun::Tick(DWORD dwDeltaTime)
{
	const char *szCountry = g_pGame->GetConfigs()->GetCountryRegion();
	if (GetGameState() == CECGameRun::GS_GAME && 
		0 == stricmp(szCountry, "kr") &&
		m_pUIManager->GetCurrentUIMan() == CECUIManager::UIMAN_INGAME && m_pUIManager->GetInGameUIMan())
	{
		CECGameUIMan* pGameUI = (CECGameUIMan*)m_pUIManager->GetInGameUIMan();
		m_nWallowTime += dwDeltaTime;
		if (m_nWallowTime > 60*60*1000 && m_nWallowStage == WALLOW_STAGE0)
		{
			pGameUI->AddChatMessage(pGameUI->GetStringFromTable(9311), GP_CHAT_BROADCAST);
			m_nWallowStage = WALLOW_STAGE1;
		}
		else if (m_nWallowTime > 2*60*60*1000 && m_nWallowStage == WALLOW_STAGE1)
		{
			pGameUI->AddChatMessage(pGameUI->GetStringFromTable(9312), GP_CHAT_BROADCAST);
			m_nWallowStage = WALLOW_STAGE2;
		}
		else if (m_nWallowTime > 3*60*60*1000 && m_nWallowStage == WALLOW_STAGE2)
		{
			pGameUI->AddChatMessage(pGameUI->GetStringFromTable(9313), GP_CHAT_BROADCAST);
			m_nWallowStage = WALLOW_STAGE3;
		}
	}
	DWORD dwTickTime = a_GetTime();
	CECGameSession* pSession = g_pGame->GetGameSession();

	pSession->ProcessNewProtocols();
	DWORD dwRealTime = g_pGame->GetRealTickTime();

	if (m_iLogoutFlag >= 0)
	{
		Logout(m_iLogoutFlag);
		m_iLogoutFlag = -1;
	}

	if (m_pUIManager)
		m_bUIHasCursor = m_pUIManager->UIControlCursor();
	else
		m_bUIHasCursor = false;

	//	Deal input first
	if (m_pInputCtrl)
		m_pInputCtrl->Tick();

	//	Tick world
	if (!TickGameWorld(dwDeltaTime, g_pGame->GetViewport()))
		return false;

	//	Tick UI
	if (m_pUIManager)
		m_pUIManager->Tick(dwDeltaTime);

	//	Tick GFX caster
	g_pGame->GetGFXCaster()->Tick(dwDeltaTime);

	//	Tick GFX Manager
	g_pGame->GetA3DGFXExMan()->Tick(dwDeltaTime);

	//	A3DEngine::TickAnimation trigger animation of many objects.
	//	For example: A3DSky objects, GFX objects etc.
	static DWORD dwAnimTime = 0;
	dwAnimTime += dwDeltaTime;
	while (dwAnimTime >= TIME_TICKANIMATION)
	{
		dwAnimTime -= TIME_TICKANIMATION;
		g_pGame->GetA3DEngine()->TickAnimation();
	}

	//	Update ear position so that all 3D sounds' positions are correct
	static DWORD dwEarTime = 0;

	DWORD dwEarUpdateTime = TIME_UPDATEEAR;
	if (gr.get_state() == game_record::enum_gr_replay )
	{
		dwEarUpdateTime = (DWORD)(dwEarUpdateTime * gr.get_play_speed());
		if( dwEarUpdateTime < 1 ) dwEarUpdateTime = 1;
	}
	if ((dwEarTime += dwDeltaTime) >= dwEarUpdateTime)
	{
		dwEarTime -= dwEarUpdateTime;

		CECHostPlayer* pHostPlayer = NULL;
		if (m_pWorld)
			pHostPlayer = m_pWorld->GetHostPlayer();

		A3DCamera * pCamera = g_pGame->GetViewport()->GetA3DCamera();

		if (GetGameState() == CECGameRun::GS_GAME && pHostPlayer && pHostPlayer->HostIsReady())
		{
			AM3DSoundDevice * pAM3DSoundDevice = g_pGame->GetA3DEngine()->GetAMSoundEngine()->GetAM3DSoundDevice();
			A3DVECTOR3 vecDir = pCamera->GetDirH();
			A3DVECTOR3 vecUp = A3DVECTOR3(0.0f, 1.0f, 0.0f);

			// Now we should adjust the 3d sound device's pos and orientation;
			if (pAM3DSoundDevice)
			{
				pAM3DSoundDevice->SetPosition(pHostPlayer->GetPos() + A3DVECTOR3(0.0f, 0.8f, 0.0f));
				pAM3DSoundDevice->SetOrientation(vecDir, vecUp);
				pAM3DSoundDevice->UpdateChanges();
			}
		}
		else
			g_pGame->GetViewport()->GetA3DCamera()->UpdateEar();
	}

	//	Tick Run-Time debug information
	g_pGame->GetRTDebug()->Tick(dwDeltaTime);

	//	Save UI configs when time reached
	if (m_iGameState == GS_GAME && l_SaveCfgCnt.IncCounter(dwRealTime))
	{
		ForceSaveConfigsToServer();
	}
	
	l_StatCnt.IncCounter(dwDeltaTime);

	pSession->ClearOldProtocols();

	dwTickTime = a_GetTime() - dwTickTime;
	l_Statistic.iTickTime = (int)dwTickTime;

	if (GetGameState() == CECGameRun::GS_GAME && l_fc.iAvgRdTime)
	{
		//	Accumulate tick time
		l_fc.iTickCnt++;
		l_fc.iTickTime += (int)dwTickTime;
	}

	g_LuaStateMan.Tick(dwDeltaTime);

	if (gr.get_state() == game_record::enum_gr_record && GetGameState() == CECGameRun::GS_GAME)
		gr.push_cur_frame();

	// For flash window state Update
	if (!g_bFocused) // the main window is currently not focused on
	{												
		if (m_pHostPlayer && m_pHostPlayer->IsUnderWickedAtk())							// Player under wicked attack
		{
			// When under wicked attack, directly set the m_bFlashWindow to true
			m_bFlashWindow = true;
		}
		else if ((m_pHostPlayer && m_pHostPlayer->IsDead()) ||							// Player dead
			(GetGameState() == CECGameRun::GS_GAME && !pSession->IsConnected())			// Disconnect in game state
			)
		{
			// When play is dead or disconnected
			// Only flash window at the first time
			// After the first SetFocus, then minimize the window will not lead to a new flash
			if (m_bIsFirstFlash)
				m_bFlashWindow = true;
			else
				m_bFlashWindow = false;
		}
		else																		// Everything OK
		{
			// Nothing happened, then no need to notify
			m_bFlashWindow = false;
		}
	}
	else	// Now focused on
	{
		// When the window is focused on
		// If now flashing window, and it is the first flash, just set the m_bIsFirstFlash to false
		if (m_bFlashWindow && m_bIsFirstFlash)
			m_bIsFirstFlash = false;

		// Focused on, no need to flash
		m_bFlashWindow = false;													// Do not need flash window

		//	When window is focused on
		//	Test If the the notify state is still
		//	If everything is OK, then reset m_bIsFirstFlash to true ---- which means the initial state
		if (!( 
			(m_pHostPlayer && m_pHostPlayer->IsUnderWickedAtk()) ||					// Player under wicked attack
			(m_pHostPlayer && m_pHostPlayer->IsDead()) ||							// Player dead
			(GetGameState() == CECGameRun::GS_GAME && !pSession->IsConnected())		// Disconnect in game state
			))
		{
			m_bIsFirstFlash = true;
		}
	}
//	if(m_bIsTray)
//	{
//		if(m_pHostPlayer&& m_pHostPlayer->IsDead())
//			m_bTrayFlash = true;
//	}
//	else
// 		m_bTrayFlash = false;
	return true;
}

#define DEFENCE_INTERV_MIN 30000
#define DEFENCE_INTERV_MAX 60000

static DWORD dwDefenceCount = 0;
static DWORD dwDefenceInterval = DEFENCE_INTERV_MIN + (DEFENCE_INTERV_MAX - DEFENCE_INTERV_MIN) * rand() / RAND_MAX;
static DWORD dwLastTime = 0;

int defence_max_check_interval = DEFENCE_INTERV_MAX * 2 / 1000;
abase::vector<ACRemoteCode*> g_DefenceCodeVec;

extern HANDLE g_hExitGame;
extern CRITICAL_SECTION g_csDefence;

volatile time_t g_DenfenceTimeMark = 0;
volatile bool g_bHostInGame = false;

// #define DEFENCE_TIME_LOG

void CECGameRun::CheckDefenceCode()
{
	while (true)
	{
		if (g_bExceptionOccured || ::WaitForSingleObject(g_hExitGame, 0) == WAIT_OBJECT_0)
			break;

		g_bHostInGame = (m_pHostPlayer && m_pHostPlayer->HostIsReady());

		if (!g_bHostInGame)
		{
			dwDefenceCount = 0;
			dwLastTime = 0;
			g_DenfenceTimeMark = 0;
			::Sleep(1000);
			continue;
		}

		CheckDefenceCodeImpl();
		Sleep(2000);
	}
}
*/
//void CECGameRun::CheckDefenceCodeImpl()
//{
	/*
	 *	anti hacker code here
	 */

/*	int nCharId = g_pGame->GetGameSession()->GetCharacterID();

	DWORD dwDeltaTime;

	if (dwLastTime == 0)
	{
		dwLastTime = GetTickCount();
		dwDeltaTime = 0;
	}
	else
	{
		DWORD dwNow = GetTickCount();
		dwDeltaTime = dwNow - dwLastTime;
		dwLastTime = dwNow;
	}

	dwDefenceCount += dwDeltaTime;

	if (dwDefenceCount >= dwDefenceInterval)
	{
#ifdef DEFENCE_TIME_LOG
		LARGE_INTEGER l, _start, _end, _interv;
		QueryPerformanceFrequency(&l);
		QueryPerformanceCounter(&_start);
#endif

		g_DenfenceTimeMark = time(NULL);

		PBYTE buf = NULL;
		int len = Collector::GetInfo(Collector::GetNextInfoType(), &buf, NULL, NULL);
		Octets oct;

		if (buf && len > 0)
		{
			oct.replace(buf, len);
			ACReport report(nCharId, oct);
			g_pGame->GetGameSession()->SendNetData(report, true);
			delete[] buf;
		}

		dwDefenceCount = 0;
		dwDefenceInterval = DEFENCE_INTERV_MIN + (DEFENCE_INTERV_MAX - DEFENCE_INTERV_MIN) * rand() / RAND_MAX;

#ifdef DEFENCE_TIME_LOG
		QueryPerformanceCounter(&_end);
		_interv.QuadPart = _end.QuadPart - _start.QuadPart;
		double _f = (double)_interv.QuadPart * 1000.0 / l.QuadPart;
		char s[100];
		sprintf(s, "1. defence report time = %.2f\n", _f);
		a_LogOutput(1, s);
#endif
	}

	CheaterKiller::TryKillCheaters();
	
	::EnterCriticalSection(&g_csDefence);
	abase::vector<ACRemoteCode*> vec(g_DefenceCodeVec);
	g_DefenceCodeVec.clear();
	::LeaveCriticalSection(&g_csDefence);

	for (size_t i = 0; i < vec.size(); i++)
	{
#ifdef DEFENCE_TIME_LOG
		LARGE_INTEGER l, _start, _end, _interv;
		QueryPerformanceFrequency(&l);
		QueryPerformanceCounter(&_start);
#endif
		ACRemoteCode* pCode = vec[i];
		if (pCode->dstroleid != nCharId)
		{
			pCode->Destroy();
			continue;
		}

		for (size_t i = 0; i < pCode->content.size(); i++)
			NetDLL::Parse(pCode->content[i].begin(), pCode->content[i].size());

		PBYTE buf = NULL;
		int len = Collector::GetInfoNetDLL(&buf);
		Octets oct;

		if (buf && len > 0)
		{
			oct.replace( buf, len );
			ACReport report(nCharId, oct);
			g_pGame->GetGameSession()->SendNetData(report, true);
			delete[] buf;
		}

		pCode->Destroy();
		pCode = NULL;

#ifdef DEFENCE_TIME_LOG
		QueryPerformanceCounter(&_end);
		_interv.QuadPart = _end.QuadPart - _start.QuadPart;
		double _f = (double)_interv.QuadPart * 1000.0 / l.QuadPart;
		char s[100];
		sprintf(s, "2. defence check time = %.2f\n", _f);
		a_LogOutput(1, s);
#endif

	}

	for (i = 0; i < 2; i++)
	{
		PBYTE buf = NULL;
		int len = Collector::GetInfoNetDLLEx(&buf);
		Octets oct;

		if (buf && len > 0)
		{
			oct.replace(buf, len);
			ACReport report(nCharId, oct);
			g_pGame->GetGameSession()->SendNetData(report, true);
			delete[] buf;
		}
	}
	
	ImageChecker::CheckImage();
}

//	Tick game world
bool CECGameRun::TickGameWorld(DWORD dwDeltaTime, CECViewport* pViewport)
{
	if (!m_pWorld)
		return true;

	//	Offset world center for skin model manager, this will fix skin model
	//	flash rendering problem caused by float calculation precision error !
	g_pGame->GetA3DEngine()->GetA3DSkinMan()->SetWorldCenter(pViewport->GetA3DCamera()->GetPos());

	bool bHostReady = m_pHostPlayer && m_pHostPlayer->HostIsReady();

	//	Tick all carriers
	m_pWorld->GetNPCMan()->TickCarriers(dwDeltaTime);

	//	Tick host player at first
	if (bHostReady)
		m_pHostPlayer->Tick(dwDeltaTime);

	//	Dispatch game messages
	if (m_pMessageMan)
		m_pMessageMan->Tick(dwDeltaTime);

	//	Some messages also effect host's position and direction, so update
	//	camera position and direction after messages were dispatched
	if (bHostReady && !m_pHostPlayer->IsChangingFace())
	{
		A3DCamera* pCamera = pViewport->GetA3DCamera();
		A3DCoordinate* pCameraCoord = m_pHostPlayer->GetCameraCoord();
		pCamera->SetPos(pCameraCoord->GetPos());
		pCamera->SetDirAndUp(pCameraCoord->GetDir(), pCameraCoord->GetUp());
	}

	m_pWorld->Tick(dwDeltaTime, g_pGame->GetViewport());
	bHostReady = m_pHostPlayer && m_pHostPlayer->HostIsReady();

	if (bHostReady)
	{
		g_ECAutoMove.SetStartPos(m_pHostPlayer->GetPos());
		g_ECAutoMove.Tick(dwDeltaTime);

		if (m_pECHelper)
		{
			m_pECHelper->Tick(dwDeltaTime);
		}
	}

	if(bHostReady && m_pSecretary)
	{
		m_pSecretary->Tick(dwDeltaTime);
	}

	return true;
}

//	Game render routine
bool CECGameRun::Render(bool bPresent)
{
	if (!m_pWorld)
		return true;

	CECHostPlayer* pHostPlayer = m_pWorld->GetHostPlayer();
	A3DEngine* pA3DEngine = g_pGame->GetA3DEngine();
	A3DDevice* pA3DDevice = g_pGame->GetA3DDevice();
	CECViewport* pViewport = g_pGame->GetViewport();

	if (gr.is_seeking() || (GetGameState() == CECGameRun::GS_GAME && pHostPlayer && !pHostPlayer->HostIsReady()))
	{
		if (!BeginRender())
		{
			a_LogOutput(1, "CECGameRun::Render(), Failed to begin render!");
			return false;
		}

		A3DDEVFMT devFmt = pA3DDevice->GetDevFormat();
		A3DVIEWPORTPARAM viewParam;
		viewParam.X = 0;
		viewParam.Y = 0;
		viewParam.Width  = devFmt.nWidth;
		viewParam.Height = devFmt.nHeight;
		viewParam.MinZ = 0.0f;
		viewParam.MaxZ = 1.0f;
		pA3DDevice->SetViewport(&viewParam);
		
		A3DCOLOR bgColor = 0x00000000;
		pA3DDevice->Clear(D3DCLEAR_TARGET, bgColor, 1.0f, 0);

		if( m_pUIManager->GetCurrentUIManPtr() )
			m_pUIManager->GetCurrentUIManPtr()->GetA3DFTFontMan()->Flush();
		if (m_pUIManager)
			m_pUIManager->Render();

		if (gr.get_fade_state() != game_record::enum_fade_none)
			RecordDrawFade(pA3DDevice, 255, enumFadeBlack);
	
		EndRender(bPresent);
		return true;
	}

	if (g_pGame->GetConfigs()->GetSkipFrameFlag() &&
		GetGameState() == CECGameRun::GS_GAME && l_fc.iAvgRdTime)
	{
		if (l_fc.iTickCnt < 10 &&
			l_fc.iTickTime + l_fc.iAvgRdTime > l_fc.iTickCnt * 150)
			return true;

		l_fc.iTickTime	= 0;
		l_fc.iTickCnt	= 0;
	}

	DWORD dwRenderTime = a_GetTime();

	if (!BeginRender())
	{
		a_LogOutput(1, "CECGameRun::Render(), Failed to begin render!");
		return false;
	}

	if( g_pGame->GetConfigs()->GetSystemSettings().bWideScreen )
	{
		A3DDEVFMT devFmt = pA3DDevice->GetDevFormat();
		A3DVIEWPORTPARAM * pMainView = g_pGame->GetViewport()->GetA3DViewport()->GetParam();

		if( pMainView->Y > 0 )
		{
			// we must clear top and bottom bar to be pure black, otherwise, there maybe some random images
			A3DVIEWPORTPARAM viewParam;
			viewParam.X = 0;
			viewParam.Y = 0;
			viewParam.Width  = devFmt.nWidth;
			viewParam.Height = pMainView->Y;
			viewParam.MinZ = 0.0f;
			viewParam.MaxZ = 1.0f;
			pA3DDevice->SetViewport(&viewParam);
			pA3DDevice->Clear(D3DCLEAR_TARGET, 0x00000000, 1.0f, 0);

			viewParam.Y = devFmt.nHeight - pMainView->Y;
			viewParam.Height = pMainView->Y;
			pA3DDevice->SetViewport(&viewParam);
			pA3DDevice->Clear(D3DCLEAR_TARGET, 0x00000000, 1.0f, 0);
		}
	}

	//	First of all, render current frame's screen content
	if( m_pScreenRender )
	{
		m_pScreenRender->GetShootingCam()->SetPos(pViewport->GetA3DCamera()->GetPos());
		m_pScreenRender->GetShootingCam()->SetDirAndUp(pViewport->GetA3DCamera()->GetDir(), pViewport->GetA3DCamera()->GetUp());
		m_pScreenRender->RenderScreen(pViewport->GetA3DViewport(), WorldRenderForScreen, GetWorld());
	}

	if( m_pFullGlowRender )
	{
		m_pFullGlowRender->BeginGlow();
		m_pFullGlowRender->Update(g_pGame->GetTickTime());
	}

	if( m_pSBRender )
		m_pSBRender->Update(g_pGame->GetTickTime());
	
	if( m_pScreenRender )
		m_pScreenRender->Update(g_pGame->GetTickTime());
	
	// don't clear color buffer because we may do motion blur effects.
	pViewport->Activate(false);
	g_pGame->GetA3DDevice()->Clear(D3DCLEAR_ZBUFFER, 0xff000000, 1.0f, 0);

	//	Render world
	if (m_pWorld)
	{
		if( GetGameState() == CECGameRun::GS_LOGIN )
		{
			CECLoginUIMan* pLoginUI = m_pUIManager->GetLoginUIMan();
			if( pLoginUI )
				pLoginUI->RenderBackground();
		}

		m_pWorld->Render(pViewport);
	}

	g_ECAutoMove.Render(pViewport);

	//	Render GFX
	g_pGame->GetGFXCaster()->Render(pViewport);
	pA3DEngine->RenderGFX(pViewport->GetA3DViewport(), 0xffffffff);

	if (g_pA3DConfig->RT_GetShowBoundBoxFlag())
		g_pGame->GetA3DGFXExMan()->RenderAllGfxAABB();

	g_pGame->GetA3DGFXExMan()->RenderAllSkinModels(pViewport->GetA3DViewport());
	A3DSkinRender* psr1 = g_pGame->GetSkinRender1();

	//	Render non alpha skin models
	psr1->Render(pViewport->GetA3DViewport(), false);
	psr1->ResetRenderInfo(false);
	psr1->RenderAlpha(pViewport->GetA3DViewport());
	psr1->ResetRenderInfo(true);

	g_pGame->GetA3DGFXExMan()->RenderAllGfx(pViewport->GetA3DViewport());

	//	Flush wire and flat collectors
	pA3DEngine->GetA3DWireCollector()->Flush();
	pA3DEngine->GetA3DFlatCollector()->Flush();

	if( m_pFullGlowRender )
	{
		m_pFullGlowRender->EndGlow(pViewport->GetA3DViewport());
	}
	if( m_pSBRender )
	{
		m_pSBRender->DoSimpleBloom(pViewport->GetA3DViewport(), WorldRenderForBloom, GetWorld());
	}

	//	Render all registered pate text
	if (m_pWorld && m_pWorld->GetDecalMan())
	{
		pA3DDevice->SetAlphaBlendEnable(false);
		pA3DDevice->SetAlphaTestEnable(true);
		m_pWorld->GetDecalMan()->RenderPateTexts(pViewport);
		pA3DDevice->SetAlphaBlendEnable(true);
		pA3DDevice->SetAlphaTestEnable(false);
	}

	//	Flush images
	g_pGame->GetImageRes()->PresentDraws();
	//  make sure 3D sprites be flushed, clear buffer for 2D sprites.
	g_pGame->GetA3DEngine()->FlushInternalSpriteBuffer();
	//	Render 2D gfx
	if( m_pUIManager->GetCurrentUIMan() == CECUIManager::UIMAN_LOGIN ||
		m_pUIManager->GetCurrentUIMan() == CECUIManager::UIMAN_INGAME && m_pUIManager->GetInGameUIMan() &&
		m_pUIManager->GetInGameUIMan()->IsShowAllPanels() )
		AfxGetGFXExMan()->Register2DGfx();
	AfxGetGFXExMan()->Render2DGfx(true);

	//	Render UI
	if( m_pUIManager->GetCurrentUIManPtr() )
		m_pUIManager->GetCurrentUIManPtr()->GetA3DFTFontMan()->Flush();
	if (m_pUIManager)
		m_pUIManager->Render();

	AfxGetGFXExMan()->Render2DGfx(false);

	//	Render Run-Time debug information
	g_pGame->GetRTDebug()->Render(pViewport);

	//	Render statistic
	if (g_pGame->GetConfigs()->GetShowGameStatFlag())
		DisplayGameStatistic();

	if (gr.get_fade_state() != game_record::enum_fade_none)
	{
		int a = 255 * l_RecordFadeCnt.GetCounter() / l_RecordFadeCnt.GetPeriod();
		a_ClampRoof(a, 255);

		if (gr.get_fade_state() == game_record::enum_fade_in)
			a = 255 - a;

		RecordDrawFade(pA3DDevice, a, enumFadeBlack);
	}
	
	//	Host Player is ready , Host Player is under attack and
	//	Host Player is not in battle or in duel or in war (under wicked attack) AND host player is not Invader (Yellow name state)
	if (m_pHostPlayer)
		BlinkScreen(pA3DDevice, m_pHostPlayer->IsUnderWickedAtk(), g_pGame->GetTickTime(), 160, A3DCOLORRGB(255, 65, 65));


	EndRender(bPresent);

	dwRenderTime = a_GetTime() - dwRenderTime;

	if (GetGameState() == CECGameRun::GS_GAME)
	{
		//	Calculate average render time
		l_fc.iRenderTime += (int)dwRenderTime;
		l_fc.iRenderCnt++;

		int iTotal = l_fc.iAvgRdTime ? 30 : 10;
		if (l_fc.iRenderCnt >= iTotal)
		{
			l_fc.iAvgRdTime	 = (int)((float)l_fc.iRenderTime / l_fc.iRenderCnt + 0.5f);
			l_fc.iRenderCnt  = 0;
			l_fc.iRenderTime = 0;
		}
	}

	// clear dynamic light list.
	GfxLightParamList& lightList = g_pGame->GetA3DGFXExMan()->GetLightParamList();
	g_pGame->GetA3DGFXExMan()->LockLightParamList();
	lightList.clear();
	g_pGame->GetA3DGFXExMan()->UnlockLightParamList();
	return true;
}

//	Begin render
bool CECGameRun::BeginRender()
{
	A3DEngine* pA3DEngine = g_pGame->GetA3DEngine();

	if (!pA3DEngine->BeginRender())
		return false;

	m_dwRenderTime = a_GetTime();

	return true;
}

//	End render
bool CECGameRun::EndRender(bool bPresent)
{
	A3DEngine* pA3DEngine = g_pGame->GetA3DEngine();

	A3DSkinRender* psr = g_pGame->GetSkinRender1();
	if (psr)
	{
		psr->ResetRenderInfo(false);
		psr->ResetRenderInfo(true);
	}

	if ((psr = g_pGame->GetSkinRender2()))
	{
		psr->ResetRenderInfo(false);
		psr->ResetRenderInfo(true);
	}

	if (!pA3DEngine->EndRender())
		return false;

	if( bPresent )
		pA3DEngine->Present();

	m_dwRenderTime = a_GetTime() - m_dwRenderTime;
	l_Statistic.iRenderTime = (int)m_dwRenderTime;

	return true;
}

//	Set logout flag
void CECGameRun::SetLogoutFlag(int iFlag)
{
	m_iLogoutFlag = iFlag;
}
*/
/*
//	Logout
void CECGameRun::Logout(int iFlag)
{
	ASSERT(m_iGameState == GS_GAME);

	bool bExitApp = false;
	bool bExitFromReplay = false;

	if (gr.get_state() == game_record::enum_gr_replay)
	{
		gr.release();
		gr.set_state(game_record::enum_gr_disable);
		bExitFromReplay = true;

		CECGameUIMan* pGameUI = (CECGameUIMan*)m_pUIManager->GetInGameUIMan();
		DWORD dw;
		pGameUI->GetUserLayout(NULL, dw);

		if (iFlag != 0)
			iFlag = 2;
	}

	if (iFlag == 0)		//	Exit application directly
	{
		bExitApp = true;
	}
	else if (iFlag == 1)	//	Logout game and re-select role
	{
		StartLogin(false);
		
		//	Goto select role interface directly
		CECLoginUIMan* pLoginUI = m_pUIManager->GetLoginUIMan();
		if (pLoginUI)
		{
			pLoginUI->LaunchCharacter(true);
			g_pGame->GetGameSession()->ReLogin(true);
		}
		else
		{
			ASSERT(pLoginUI);
			bExitApp = true;
		}
	}
	else if (iFlag == 2)	//	Logout game and goto login state
	{
		StartLogin(false);

		CECLoginUIMan* pLoginUI = m_pUIManager->GetLoginUIMan();
		if (pLoginUI)
		{
			g_pGame->GetGameSession()->ReLogin(false);

			if (bExitFromReplay)
				pLoginUI->OnCommand("replay", pLoginUI->GetDialog("Win_LoginReplay"));
		}
		else
		{
			ASSERT(pLoginUI);
			bExitApp = true;
		}
	}

	if (bExitApp)
	{
		//	Exit game application
		EndGameState(false);
		::PostMessage(g_pGame->GetGameInit().hWnd, WM_QUIT, 0, 0);
	}
}

//	Save necessary user configs (UI, shortcut, accelerate keys, etc.) to server
bool CECGameRun::SaveConfigsToServer(bool bRecord)
{
	if (!m_pWorld || 
		!m_pWorld->GetHostPlayer() || !m_pWorld->GetHostPlayer()->HostIsReady() || 
		!m_pUIManager->GetInGameUIMan())
		return false;

	int iTotalSize = 0;

	//	Version
	iTotalSize += sizeof (DWORD);

	//	Calculate data buffer size at first
	iTotalSize += sizeof (int);
	int iHostSize = 0;
	CECHostPlayer* pHost = m_pWorld->GetHostPlayer();
	pHost->SaveConfigData(NULL, &iHostSize);
	iTotalSize += iHostSize;

	iTotalSize += sizeof (int);
	DWORD dwUISize = 0;
	CECGameUIMan* pGameUI = (CECGameUIMan*)m_pUIManager->GetInGameUIMan();
	pGameUI->GetUserLayout(NULL, dwUISize);
	iTotalSize += (int)dwUISize;

	iTotalSize += sizeof (int);
	int iSettingSize = 0;
	g_pGame->GetConfigs()->SaveUserConfigData(NULL, &iSettingSize);
	iTotalSize += iSettingSize;

	//	Allocate data buffer
	void* pDataBuf = a_malloc(iTotalSize);
	if (!pDataBuf)
	{
		glb_ErrorOutput(ECERR_NOTENOUGHMEMORY, "CECGameRun::SaveConfigsToServer", __LINE__);
		return false;
	}

	BYTE* pData = (BYTE*)pDataBuf;

	//	Save version
	*((DWORD*)pData) = USERCFG_VERSION;
	pData += sizeof (DWORD);

	//	Save host config data
	*((int*)pData) = iHostSize;
	pData += sizeof (int);
	pHost->SaveConfigData(pData, &iHostSize);
	pData += iHostSize;

	//	Save UI config data
	*((int*)pData) = (int)dwUISize;
	pData += sizeof (int);
	pGameUI->GetUserLayout(pData, dwUISize);
	pData += dwUISize;

	//	Save user setting data
	*((int*)pData) = iSettingSize;
	pData += sizeof (int);
	g_pGame->GetConfigs()->SaveUserConfigData(pData, &iSettingSize);
	pData += iSettingSize;

	//	Here we make a cache. If the data will be sent is same as last
	//	time, we can ignore it.
	if (!bRecord)
	{
		if (m_pCfgDataBuf)
		{
			if (m_iCfgDataSize == iTotalSize && !memcmp(m_pCfgDataBuf, pDataBuf, iTotalSize))
			{
				//	Data of these 2 times is identical
				a_free(pDataBuf);
				return true;
			}

			//	Data isn't identical
			a_free(m_pCfgDataBuf);
			m_pCfgDataBuf	= NULL;
			m_iCfgDataSize	= 0;
		}

		//	Copy data of this time in order to compare next time
		if ((m_pCfgDataBuf = (BYTE*)a_malloc(iTotalSize)))
		{
			memcpy(m_pCfgDataBuf, pDataBuf, iTotalSize);
			m_iCfgDataSize = iTotalSize;
		}
	}

	//	Compress config data
	DWORD dwCompLen = iTotalSize * 2;
	void* pCompBuf = a_malloc(dwCompLen);
	if (!pCompBuf)
	{
		glb_ErrorOutput(ECERR_NOTENOUGHMEMORY, "CECGameRun::SaveConfigsToServer", __LINE__);
		return false;
	}

	//	Don't compress version info, directly copy it.
	int iVerLen = sizeof (DWORD);
	memcpy(pCompBuf, pDataBuf, iVerLen);
	BYTE* pSrc = (BYTE*)pDataBuf + iVerLen;
	BYTE* pDst = (BYTE*)pCompBuf + iVerLen;
	dwCompLen -= iVerLen;
	AFilePackage::Compress(pSrc, iTotalSize - iVerLen, pDst, &dwCompLen);

	//	Send data to server
	if (!bRecord)
		g_pGame->GetGameSession()->SaveConfigData(pCompBuf, dwCompLen+iVerLen);
	else
	{
		gr.get_cur_frame()->push_event(
			new event_record_protocol(
				&GNET::GetUIConfig_Re(
					ERR_SUCCESS,
					pHost->GetCharacterID(),
					0,
					GNET::Octets(pCompBuf, dwCompLen+iVerLen))));
	}

	a_free(pDataBuf);
	a_free(pCompBuf);

	return true;
}

//	Force save necessary user configs (UI, shortcut, accelerate keys) to server
bool CECGameRun::ForceSaveConfigsToServer(bool bRecord)
{
	l_SaveCfgCnt.Reset();
	SaveConfigsToServer();
	m_pSecretary->SaveStatesToServer();

	return true;
}

//	Load necessary user configs (UI, shortcut, accelerate keys) from server
bool CECGameRun::LoadConfigsFromServer(const void* pDataBuf, int iDataSize)
{
	if (!pDataBuf || !iDataSize)
		return true;

	ASSERT(m_pWorld && m_pWorld->GetHostPlayer() && m_pUIManager->GetInGameUIMan());

	BYTE* pData = (BYTE*)pDataBuf;

	//	Read version
	DWORD dwVer = *(DWORD*)pData;
	pData += sizeof (DWORD);
	if (dwVer > USERCFG_VERSION)
	{
		ASSERT(dwVer > USERCFG_VERSION);
		return false;
	}

	void* pUncompBuf = NULL;
	DWORD dwRealLen = iDataSize - sizeof (DWORD);

	if (dwVer >= 3)
	{
		//	Uncompress config data
		dwRealLen = 4096;
		if (!(pUncompBuf = a_malloc(dwRealLen)))
		{
			glb_ErrorOutput(ECERR_NOTENOUGHMEMORY, "CECGameRun::SaveConfigsToServer", __LINE__);
			return false;
		}

		AFilePackage::Uncompress(pData, iDataSize-sizeof (DWORD), (BYTE*)pUncompBuf, &dwRealLen);
		pData = (BYTE*)pUncompBuf;
	}

	try
	{
		CECDataReader dr(pData, (int)dwRealLen);

		//	Load host configs ...
		CECHostPlayer* pHost = m_pWorld->GetHostPlayer();
		int iSize = dr.Read_int();
		if (!pHost->LoadConfigData(dr.Read_Data(iSize)))
		{
			if (pUncompBuf)
				a_free(pUncompBuf);

			a_LogOutput(1, "CECGameRun::LoadConfigsFromServer, Failed to load host configs ...");
			return false;
		}

		//	Load UI configs ...
		CECGameUIMan* pGameUI = m_pUIManager->GetInGameUIMan();
		iSize = dr.Read_int();
		if (!pGameUI->SetUserLayout(dr.Read_Data(iSize), iSize))
		{
			if (pUncompBuf)
				a_free(pUncompBuf);

			a_LogOutput(1, "CECGameRun::LoadConfigsFromServer, Failed to set user layout ...");
			return false;
		}

		//	Load user settings ...
		if (dwVer >= 2)
		{
			iSize = dr.Read_int();
			g_pGame->GetConfigs()->LoadUserConfigData(dr.Read_Data(iSize), iSize);
		}

		if (pUncompBuf)
			a_free(pUncompBuf);
	}
	catch (CECException& e)
	{
		ASSERT(0);
		a_LogOutput(1, "CECGameRun::LoadConfigsFromServer, data read error (%d)", e.GetType());

		if (pUncompBuf)
			a_free(pUncompBuf);
	}
	catch(...)
	{
		ASSERT(0);
		a_LogOutput(1, "CECGameRun::LoadConfigsFromServer, data read error UNKNOWN");
	}
	return true;
}
*/
//	Add fixed format message to chat window
void CECGameRun::AddFixedMessage(int iMsg, ...)
{
	CECStringTab* pStrTab = g_pGame->GetFixedMsgTab();
	const wchar_t* szFixMsg = pStrTab->GetWideString(iMsg);
	if (!szFixMsg)
		return;

	ACHAR szBuf[1024];

	va_list vaList;
	va_start(vaList, iMsg);
	a_vsprintf(szBuf, szFixMsg, vaList);
	va_end(vaList);

/*	CECGameUIMan* pGameUI = m_pUIManager->GetInGameUIMan();
	if (pGameUI)
		pGameUI->AddChatMessage(szBuf, GP_CHAT_SYSTEM);
	else
		g_pGame->RuntimeDebugInfo(RTDCOL_CHATLOCAL, szBuf);*/
}
/*
void CECGameRun::AddFixedChannelMsg(int iMsg, char cChannel, ...)
{
	CECStringTab* pStrTab = g_pGame->GetFixedMsgTab();
	const wchar_t* szFixMsg = pStrTab->GetWideString(iMsg);
	if (!szFixMsg)
		return;

	ACHAR szBuf[1024];

	va_list vaList;
	va_start(vaList, cChannel);
	a_vsprintf(szBuf, szFixMsg, vaList);
	va_end(vaList);

	CECGameUIMan* pGameUI = m_pUIManager->GetInGameUIMan();
	if (pGameUI)
		pGameUI->AddChatMessage(szBuf, cChannel);
	else
		g_pGame->RuntimeDebugInfo(RTDCOL_CHATLOCAL, szBuf);
}
*/
//	Add message to chat window
//void CECGameRun::AddChatMessage(const ACHAR *pszMsg, char cChannel, int idPlayer/* -1 */, 
//								const ACHAR* szName/* NULL */, BYTE byFlag, char cEmotion, CECIvtrItem* pItem)
/*{
	CECGameUIMan* pGameUI = m_pUIManager->GetInGameUIMan();
	if (pGameUI)
		pGameUI->AddChatMessage(pszMsg, cChannel, idPlayer, szName, byFlag, cEmotion, pItem);
	else
	{
		g_pGame->RuntimeDebugInfo(RTDCOL_CHATLOCAL, pszMsg);
		delete pItem;
	}
}

//	Set / Get selected character's role info.
const GNET::RoleInfo& CECGameRun::GetSelectedRoleInfo()
{
	return l_SelRoleInfo;
}

void CECGameRun::SetSelectedRoleInfo(const GNET::RoleInfo& Info)
{
	l_SelRoleInfo = Info;
}

void CECGameRun::UpdateSelectedRoleRace(int new_cls)
{
	l_SelRoleInfo.occupation = new_cls;
}
//	Display game statistic
void CECGameRun::DisplayGameStatistic()
{
	if (!m_pWorld)
		return;

	if (l_StatCnt.IsFull())
	{
		//	Reset statistic...
		l_StatCnt.Reset();

		l_Statistic.iMeshSize		= g_pGame->GetA3DEngine()->GetA3DSkinMan()->GetSkinMeshMan()->CalcDataSize();
		l_Statistic.iAllTexSize		= g_pGame->GetA3DEngine()->GetA3DTextureMan()->GetTextureDataSize();
		l_Statistic.iStrmDSize		= A3DStream::m_iDynDataSize;
		l_Statistic.iStrmSSize		= A3DStream::m_iStaticDataSize;
		l_Statistic.iStrmBSize		= A3DStream::m_iBackupDataSize;
		l_Statistic.iTrackSize		= A3D::g_A3DTrackMan.GetTrackDataSize();
		l_Statistic.iTrnDataSize	= m_pWorld->GetTerrain() ? m_pWorld->GetTerrain()->GetBlockDataSize() : 0;
		l_Statistic.iSndCacheSize	= g_pGame->GetSoundCache()->GetCacheSize();

		l_Statistic.iNumPlayer		= m_pWorld->GetPlayerMan()->GetElsePlayerNum();
		l_Statistic.iNumNPC			= m_pWorld->GetNPCMan()->GetNPCNum();
		l_Statistic.iNumMatter		= m_pWorld->GetMatterMan()->GetMatterNum();
		l_Statistic.iNumScnGFX		= m_pWorld->GetOrnamentMan()->GetGFXNum();
		l_Statistic.iNumOnmt		= m_pWorld->GetOrnamentMan()->GetOrnamentNum();
		l_Statistic.iNumScnModel	= m_pWorld->GetOrnamentMan()->GetECModelNum();
		l_Statistic.iNumTrnBlock	= m_pWorld->GetTerrain() ? m_pWorld->GetTerrain()->GetRenderedBlockNum() : 0;

		if (g_pGame->GetSoundCache()->GetLoadFileCnt())
			l_Statistic.fSndCacheHit = g_pGame->GetSoundCache()->GetFileCacheHitCnt() * 100.0f / g_pGame->GetSoundCache()->GetLoadFileCnt();
		else
			l_Statistic.fSndCacheHit = 0.0f;

		l_Statistic.iMemPeak		= a_GetMemPeakSize();
		l_Statistic.dwMemAllocCnt	= a_GetMemAllocCounter();
		l_Statistic.iMemCurSize		= a_GetMemCurSize();
		l_Statistic.iMemRawSize		= a_GetMemRawSize();

		l_Statistic.iSMemAllocSize	= 0;
		l_Statistic.iSMemBlkCnt		= 0;
		for (int i=0; i < 16; i++)
		{
			a_GetSmallMemInfo(i, &l_Statistic.aSMemBlkSize[i], &l_Statistic.aSMemBlkCnt[i], &l_Statistic.aSMemFreeCnt[i]);
			l_Statistic.aSMemSize[i] = l_Statistic.aSMemBlkSize[i] * l_Statistic.aSMemBlkCnt[i];
			l_Statistic.iSMemAllocSize += l_Statistic.aSMemSize[i];
			l_Statistic.iSMemBlkCnt += l_Statistic.aSMemBlkCnt[i];
		}

		a_GetLargeMemInfo(&l_Statistic.iLMemBlkCnt, &l_Statistic.iLMemAllocSize);
	}

	A3DFont* pA3DFont = g_pGame->GetA3DEngine()->GetSystemFont();
	ACHAR szMsg[256];
	int x, y, i;
	DWORD dwCol;

	//	Data size counter ...
	dwCol = 0xffff8040;
	x = 50;
	y = 100;
	a_sprintf(szMsg, _AL("Skin Mesh Size: %.2f K"), l_Statistic.iMeshSize / 1024.0f);
	pA3DFont->TextOut(x, y, szMsg, dwCol);
	y += 16;

	a_sprintf(szMsg, _AL("All Texture Size: %.2f K"), l_Statistic.iAllTexSize / 1024.0f);
	pA3DFont->TextOut(x, y, szMsg, dwCol);
	y += 16;

	a_sprintf(szMsg, _AL("Stream (D) Size: %.2f K"), l_Statistic.iStrmDSize / 1024.0f);
	pA3DFont->TextOut(x, y, szMsg, dwCol);
	y += 16;

	a_sprintf(szMsg, _AL("Stream (S) Size: %.2f K"), l_Statistic.iStrmSSize / 1024.0f);
	pA3DFont->TextOut(x, y, szMsg, dwCol);
	y += 16;

	a_sprintf(szMsg, _AL("Stream (B) Size: %.2f K"), l_Statistic.iStrmBSize / 1024.0f);
	pA3DFont->TextOut(x, y, szMsg, dwCol);
	y += 16;

	a_sprintf(szMsg, _AL("Track Size: %.2f K"), l_Statistic.iTrackSize / 1024.0f);
	pA3DFont->TextOut(x, y, szMsg, dwCol);
	y += 16;

	a_sprintf(szMsg, _AL("Terrain Size: %.2f K"), l_Statistic.iTrnDataSize / 1024.0f);
	pA3DFont->TextOut(x, y, szMsg, dwCol);
	y += 16;

	a_sprintf(szMsg, _AL("Sound Cache: %.2f K"), l_Statistic.iSndCacheSize / 1024.0f);
	pA3DFont->TextOut(x, y, szMsg, dwCol);
	y += 16;

	//	Object counter ...
	dwCol = 0xff00ffff;
	x = 270;
	y = 100;
	a_sprintf(szMsg, _AL("player num: %d"), l_Statistic.iNumPlayer);
	pA3DFont->TextOut(x, y, szMsg, dwCol);
	y += 16;

	a_sprintf(szMsg, _AL("NPC num: %d"), l_Statistic.iNumNPC);
	pA3DFont->TextOut(x, y, szMsg, dwCol);
	y += 16;

	a_sprintf(szMsg, _AL("matter num: %d"), l_Statistic.iNumMatter);
	pA3DFont->TextOut(x, y, szMsg, dwCol);
	y += 16;

	a_sprintf(szMsg, _AL("scene GFX num: %d"), l_Statistic.iNumScnGFX);
	pA3DFont->TextOut(x, y, szMsg, dwCol);
	y += 16;

	a_sprintf(szMsg, _AL("building num: %d"), l_Statistic.iNumOnmt);
	pA3DFont->TextOut(x, y, szMsg, dwCol);
	y += 16;

	a_sprintf(szMsg, _AL("scene model num: %d"), l_Statistic.iNumScnModel);
	pA3DFont->TextOut(x, y, szMsg, dwCol);
	y += 16;

	a_sprintf(szMsg, _AL("snd cache hit: %.1f%%"), l_Statistic.fSndCacheHit);
	pA3DFont->TextOut(x, y, szMsg, dwCol);
	y += 16;

	a_sprintf(szMsg, _AL("vis trn block: %d"), l_Statistic.iNumTrnBlock);
	pA3DFont->TextOut(x, y, szMsg, dwCol);
	y += 16;

	//	Time counter ...
	dwCol = 0xffff00ff;
	y += 100;
	a_sprintf(szMsg, _AL("tick time: %d"), l_Statistic.iTickTime);
	pA3DFont->TextOut(x, y, szMsg, dwCol);
	y += 16;

	a_sprintf(szMsg, _AL("render time: %d"), l_Statistic.iRenderTime);
	pA3DFont->TextOut(x, y, szMsg, dwCol);
	y += 16;

	//	Gfx Stat
	x = 50;
	y = 300;
	a_sprintf(szMsg, _AL("gfx tick time: %.2f"), AfxGetGFXExMan()->GetTotalTickTime());
	pA3DFont->TextOut(x, y, szMsg, dwCol);
	y += 16;

	a_sprintf(szMsg, _AL("gfx render time: %.2f"), AfxGetGFXExMan()->GetTotalRenderTime());
	pA3DFont->TextOut(x, y, szMsg, dwCol);
	y += 16;

	a_sprintf(szMsg, _AL("gfx total count: %d"), AfxGetGFXExMan()->GetTotalGfxCount());
	pA3DFont->TextOut(x, y, szMsg, dwCol);
	y += 16;

	a_sprintf(szMsg, _AL("gfx render count: %d"), AfxGetGFXExMan()->GetTotalGfxRenderCount());
	pA3DFont->TextOut(x, y, szMsg, dwCol);
	y += 16;

	a_sprintf(szMsg, _AL("gfx draw count: %d"), AfxGetGFXExMan()->GetTotalDrawCount());
	pA3DFont->TextOut(x, y, szMsg, dwCol);
	y += 16;
	
	a_sprintf(szMsg, _AL("gfx prim count: %d"), AfxGetGFXExMan()->GetTotalPrimCount());
	pA3DFont->TextOut(x, y, szMsg, dwCol);
	y += 16;

	a_sprintf(szMsg, _AL("par total count: %d"), AfxGetGFXExMan()->GetParticleTotalCount());
	pA3DFont->TextOut(x, y, szMsg, dwCol);
	y += 16;

	a_sprintf(szMsg, _AL("par render count: %d"), AfxGetGFXExMan()->GetParticleRenderCount());
	pA3DFont->TextOut(x, y, szMsg, dwCol);
	y += 16;

	//	Memory statistics ...
	dwCol = 0xffffff00;
	x = 440;
	y = 100;
	a_sprintf(szMsg, _AL("mem peak: %.2f K"), l_Statistic.iMemPeak / 1024.0f);
	pA3DFont->TextOut(x, y, szMsg, dwCol);
	y += 16;

	a_sprintf(szMsg, _AL("mem alloc cnt: %u"), l_Statistic.dwMemAllocCnt);
	pA3DFont->TextOut(x, y, szMsg, dwCol);
	y += 16;

	a_sprintf(szMsg, _AL("mem size: %.2f K"), l_Statistic.iMemCurSize / 1024.0f);
	pA3DFont->TextOut(x, y, szMsg, dwCol);
	y += 16;

	a_sprintf(szMsg, _AL("mem raw size: %.2f K"), l_Statistic.iMemRawSize / 1024.0f);
	pA3DFont->TextOut(x, y, szMsg, dwCol);
	y += 16;

	a_sprintf(szMsg, _AL("mem(S) blk cnt: %d"), l_Statistic.iSMemBlkCnt);
	pA3DFont->TextOut(x, y, szMsg, dwCol);
	y += 16;

	a_sprintf(szMsg, _AL("mem(S) size: %.2f K"), l_Statistic.iSMemAllocSize / 1024.0f);
	pA3DFont->TextOut(x, y, szMsg, dwCol);
	y += 16;

	a_sprintf(szMsg, _AL("mem(L) blk cnt: %d"), l_Statistic.iLMemBlkCnt);
	pA3DFont->TextOut(x, y, szMsg, dwCol);
	y += 16;

	a_sprintf(szMsg, _AL("mem(L) size: %.2f K"), l_Statistic.iLMemAllocSize / 1024.0f);
	pA3DFont->TextOut(x, y, szMsg, dwCol);
	y += 16;

	y += 16;
	for (i=0; i < 16; i++)
	{
		a_sprintf(szMsg, _AL("mem(S) [%d]: %d, %d, %d, %.2f K"), i, l_Statistic.aSMemBlkSize[i], 
			l_Statistic.aSMemBlkCnt[i], l_Statistic.aSMemFreeCnt[i], l_Statistic.aSMemSize[i] / 1024.0f);
		pA3DFont->TextOut(x, y, szMsg, dwCol);
		y += 16;
	}

	// sound load count
	dwCol = 0xff00ff00;
	x = 640;
	y = 100;
	a_sprintf(szMsg, _AL("snd always: %d"), g_pGame->GetSoundCache()->GetLoadCount(enumSoundLoadAlways));
	pA3DFont->TextOut(x, y, szMsg, dwCol);
	y += 16;

	a_sprintf(szMsg, _AL("snd noloop: %d"), g_pGame->GetSoundCache()->GetLoadCount(enumSoundLoadNonLoop));
	pA3DFont->TextOut(x, y, szMsg, dwCol);
	y += 16;

	a_sprintf(szMsg, _AL("snd loop  : %d"), g_pGame->GetSoundCache()->GetLoadCount(enumSoundLoadLoop));
	pA3DFont->TextOut(x, y, szMsg, dwCol);
	y += 16;
}

//	Begin load progress
bool CECGameRun::BeginLoadProgress(int iLoadID, int iMin, int iMax, const char* szBackPic)
{
	A3DRELEASE(m_pLoadProgress);

	if (!(m_pLoadProgress = new CECLoadProgress))
		return false;

	if( !m_pLoadProgress->Init(iLoadID, szBackPic) )
		return false;

	m_pLoadProgress->SetProgressRange(iMin, iMax);
	m_pLoadProgress->SetProgressPos(0);

	return true;
}

//	End load progress
void CECGameRun::EndLoadProgress()
{
	A3DRELEASE(m_pLoadProgress);
}

//	Step load progress
void CECGameRun::StepLoadProgress(int iStep)
{
	if (m_pLoadProgress)
	{
		m_pLoadProgress->OffsetProgressPos(iStep);
		m_pLoadProgress->Render();

		g_pGame->DispatchWindowsMessage();
	}
}

//	Get host player's position or a candidate position
A3DVECTOR3 CECGameRun::GetSafeHostPos(CECViewport* pViewport)
{
	if (m_pHostPlayer && m_pHostPlayer->HostIsReady())
		return m_pHostPlayer->GetPos();
	else
	{
		//	Use camera's position to imitate host's position
		if (!pViewport)
			pViewport = g_pGame->GetViewport();

		return pViewport->GetA3DCamera()->GetPos();
	}
}
*/
//	Get player name
const ACHAR* CECGameRun::GetPlayerName(int cid, bool bAutoGen)
{
	ID2NameTable::pair_type Pair = m_ID2NameTab.get(cid);
	if (Pair.second)
		return **Pair.first;

	if (bAutoGen)
	{
		m_strAutoName.Format(_AL("P-%d"), cid);
		return m_strAutoName;
	}

	return NULL;
}
/*
//	Get player ID by name
int CECGameRun::GetPlayerID(const ACHAR* szName)
{
	Name2IDTable::pair_type Pair = m_Name2IDTab.get(szName);
	return Pair.second ? *Pair.first : 0;
}
*/
//	Add player name to table
//void CECGameRun::AddPlayerName(int cid, const ACHAR* szName, bool bOverwrite/* false */)
/*{
	ID2NameTable::pair_type Pair = m_ID2NameTab.get(cid);
	if (Pair.second)
	{
		if (bOverwrite)
		{
			//	Name exists, replace it
			ACString* pstrName = *Pair.first;
			ACString strOldName = *pstrName;
			*pstrName = szName;

			//	Overwrite name2id table too
			Name2IDTable::pair_type Pair2 = m_Name2IDTab.get(strOldName);
			if (Pair2.second)
			{
				m_Name2IDTab.erase(strOldName);
				m_Name2IDTab.put(*pstrName, cid);
			}
		}

		return;
	}

	//	Create new name
	ACString* pstrName = new ACString(szName);
	if (pstrName)
	{
		m_ID2NameTab.put(cid, pstrName);
		m_Name2IDTab.put(*pstrName, cid);
	}
}
*/
//	Get instance by ID
CECInstance* CECGameRun::GetInstance(int id)
{
	InstTable::pair_type Pair = m_InstTab.get(id);
	return Pair.second ? *Pair.first : NULL;
}
/*
//	Load instance information from file
bool CECGameRun::LoadInstanceInfo(const char* szFile)
{
	AWScriptFile sf;
	if (!sf.Open(szFile))
	{
		a_LogOutput(1, "CECGameRun::LoadInstanceInfo, Failed to load %s", szFile);
		return false;
	}

	while (sf.PeekNextToken(true))
	{
		CECInstance* pInst = new CECInstance;
		if (!pInst || !pInst->Load(&sf))
		{
			a_LogOutput(1, "CECGameRun::LoadInstanceInfo, Failed to read instance data");
			return false;
		}

		if (!m_InstTab.put(pInst->GetID(), pInst))
		{
			//	ID collsion ?
			ASSERT(0);
			delete pInst;
		}
	}

	sf.Close();

	return true;
}

//	Jump to specified position in a instance
bool CECGameRun::JumpToInstance(int idInst, const A3DVECTOR3& vPos)
{
	CECInstance* pInst = GetInstance(idInst);
	if (!pInst)
	{
		a_LogOutput(1, "CECGameRun::JumpToInstance, wrong instance id: %d", idInst);
		return false;
	}

	if (m_pWorld)
	{
		if (m_pWorld->GetInstanceID() == pInst->GetID())
		{
			m_pWorld->SetLoadCenter(vPos);
			return true;
		}
		else
		{
			//	Release current world
			ReleaseWorld();
		}
	}

	//	Create new world
	if (!CreateWorld(idInst, vPos))
	{
		a_LogOutput(1, "CECGameRun::JumpToInstance, failed to create world %d", idInst);
		return false;
	}

	if (m_pHostPlayer)
		m_pHostPlayer->SetPlayerMan(m_pWorld->GetPlayerMan());

	CECGameUIMan* pGameUIMan = m_pUIManager->GetInGameUIMan();
	if (pGameUIMan)
		pGameUIMan->ChangeWorldInstance(idInst);

	m_pInputCtrl->ClearKBFilterStack();
	m_pInputCtrl->ClearMouFilterStack();
	m_pInputCtrl->ActivateKBFilter(m_pInputFilter);
	m_pInputCtrl->ActivateMouseFilter(m_pInputFilter);

	return true;
}

// test code
A3DVECTOR3 _org_pos(0);

//	Process message
bool CECGameRun::ProcessMessage(const ECMSG& Msg)
{
	ASSERT(Msg.iManager < 0);

	switch (Msg.dwMsg)
	{
	case MSG_EXITGAME:
	
		//	Exit game
		::PostMessage(g_pGame->GetGameInit().hWnd, WM_QUIT, 0, 0);
		break;

	case MSG_CLOSEALLDIALOG:
		
		break;

	case MSG_INSTCHECKOUT:
	{
		const S2C::cmd_inst_data_checkout* pCmd = (const S2C::cmd_inst_data_checkout*)Msg.dwParam1;
		m_nGShopTimeStamp = pCmd->gshop_time_stamp;
		
		if (m_nGShopTimeStamp != globaldata_getgshop_timestamp())
		{
			AddFixedMessage(FIXMSG_ERR_GSHOPDATA);
		}

		if (!m_pWorld || !m_pWorld->CheckOutInst(pCmd->idInst, pCmd->region_time_stamp, pCmd->precinct_time_stamp))
		{
			AddFixedMessage(FIXMSG_ERR_INSTDATA);
		}

		break;
	}
	case MSG_DOUBLETIME:
	{
		if (Msg.dwParam2 == S2C::DOUBLE_EXP_TIME)
		{
			const S2C::cmd_double_exp_time* pCmd = (const S2C::cmd_double_exp_time*)Msg.dwParam1;
			if (pCmd->mode)
			{
				m_iDblExpMode |= DBEXP_OWN;
				m_iDExpEndTime = pCmd->end_time;
			}
			else
				m_iDblExpMode &= ~DBEXP_OWN;
		}
		else if (Msg.dwParam2 == S2C::AVAILABLE_DOUBLE_EXP_TIME)
		{
			const S2C::cmd_available_double_exp_time* pCmd = (const S2C::cmd_available_double_exp_time*)Msg.dwParam1;
			m_iAvaDExpTime = pCmd->available_time;
		}

		break;
	}
	}

	return true; 
}
*/
//	Post message
//bool CECGameRun::PostMessage(DWORD dwMsg, int iManager, int iSubID, DWORD p1/* 0 */, 
//						DWORD p2/* 0 */, DWORD p3/* 0 */, DWORD p4/* 0 */)
/*{
	ECMSG Msg;

	Msg.dwMsg		= dwMsg;
	Msg.iManager	= iManager;
	Msg.iSubID		= iSubID;
	Msg.dwParam1	= p1;
	Msg.dwParam2	= p2;
	Msg.dwParam3	= p3;
	Msg.dwParam4	= p4;

	return PostMessage(Msg);
}

//	Post message
bool CECGameRun::PostMessage(const ECMSG& Msg)
{
	ASSERT(m_pMessageMan);
	if (m_pMessageMan)
		m_pMessageMan->AddMessage(Msg);
	return true;
}

//	Get remain time of current double experence time
int CECGameRun::GetRemainDblExpTime()
{
	int iRemainTime = m_iDExpEndTime - g_pGame->GetServerGMTTime();
	a_ClampFloor(iRemainTime, 0);
	return iRemainTime;
}

bool CECGameRun::CaptureScreen(int N)
{
	if( g_pGame->GetA3DDevice()->GetDevFormat().fmtTarget != A3DFMT_A8R8G8B8 &&
		g_pGame->GetA3DDevice()->GetDevFormat().fmtTarget != A3DFMT_X8R8G8B8 )
		N = 1;

	SYSTEMTIME st;
	GetLocalTime(&st);

	char szTitle[MAX_PATH], szFile[MAX_PATH];
	sprintf(szTitle, "%04d-%02d-%02d %02d-%02d-%02d.bmp",
		st.wYear, st.wMonth, st.wDay, st.wHour, st.wMinute, st.wSecond);
	sprintf(szFile, "%s\\Screenshots\\", af_GetBaseDir());
	glb_CreateDirectory(szFile);
	strcat(szFile, szTitle);

	// we turn off the fps shown and take a snap shot
	bool bShowFPS = g_pA3DConfig->RT_GetShowFPSFlag();
	int bRTDebug = g_pGame->GetConfigs()->GetRTDebugLevel();
	g_pA3DConfig->RT_SetShowFPSFlag(false);
	g_pGame->GetConfigs()->SetRTDebugLevel(0);
	
	if( N == 1 )
	{
		Render(false);
		Sleep(100);// wait a while to let the entire scene rendered to back.
		g_pGame->GetA3DDevice()->ExportColorToFile(szFile);
		int nType = g_pGame->GetConfigs()->GetGameSettings().nPrintScreenType;

		if (nType != 0)
		{
			char szJPGFile[MAX_PATH];
			strcpy(szJPGFile, szFile);
			strcpy(::PathFindExtensionA(szJPGFile), ".jpg");
			int nQuality;

			if (nType == 1)
				nQuality = 95;
			else if (nType == 2)
				nQuality = 80;
			else
				nQuality = 60;

			if (glb_ConvertBmpToJpeg(szFile, szJPGFile, nQuality))
				::DeleteFileA(szFile);
		}
	}
	else
	{
		int nWidth = g_pGame->GetA3DDevice()->GetDevFormat().nWidth;
		int nHeight = g_pGame->GetA3DDevice()->GetDevFormat().nHeight;

		A3DSurface * pTempSurface = NULL;
		pTempSurface = new A3DSurface();
		if( !pTempSurface->Create(g_pGame->GetA3DDevice(), nWidth, nHeight, g_pGame->GetA3DDevice()->GetDevFormat().fmtTarget) )
		{
			delete pTempSurface;
			pTempSurface = NULL;
			N = 1;
		}
		
		FILE * fpFile = fopen(szFile, "wb");
		if( !fpFile )
		{
			a_LogOutput(0, "Can not create screen shot file %s", szFile);
			return false;
		}

		int pitch = N * nWidth * 3;
		pitch += (4 - (pitch & 0x3)) & 0x3;

		BITMAPFILEHEADER bmfh;
		BITMAPINFOHEADER bmih;
		memset(&bmfh, 0, sizeof(bmfh));
		memset(&bmih, 0, sizeof(bmih));
		bmfh.bfType = 0x4D42;
		bmfh.bfSize = sizeof(bmfh) + sizeof(bmih) + pitch * nHeight;
		bmfh.bfOffBits = sizeof(bmfh) + sizeof(bmih);
		
		bmih.biSize = sizeof(bmih);
		bmih.biWidth = N * nWidth;
		bmih.biHeight = N * nHeight;
		bmih.biPlanes = 1;
		bmih.biBitCount = 24;
		bmih.biCompression = BI_RGB;
		
		fwrite(&bmfh, sizeof(bmfh), 1, fpFile);
		fwrite(&bmih, sizeof(bmih), 1, fpFile);

		BYTE * pTempLine = new BYTE[pitch * nHeight];

		A3DCameraBase * pCamera = g_pGame->GetViewport()->GetA3DCamera();
		for(int h=0; h<N; h++)
		{
			for(int w=0; w<N; w++)
			{
				A3DMATRIX4 matPostOffset;
				matPostOffset = Scaling((float)N, (float)N, 1.0f) * Translate(N - 1.0f - w * 2, h * 2 - (N - 1.0f), 0.0f);
				pCamera->SetPostProjectTM(matPostOffset);

				m_pFullGlowRender->SetBloomMotionBlur(false);
				Render(false);
				Sleep(100);// wait a while to let the entire scene rendered to back.
				m_pFullGlowRender->SetBloomMotionBlur(true);

				HRESULT hval;
				hval = g_pGame->GetA3DDevice()->GetD3DDevice()->CopyRects(g_pGame->GetA3DDevice()->GetBackBuffer(), NULL, 0, pTempSurface->GetDXSurface(), NULL);
				if( hval != D3D_OK )
					continue;
				
				D3DLOCKED_RECT rectLocked;
				hval = pTempSurface->GetDXSurface()->LockRect(&rectLocked, NULL, D3DLOCK_READONLY);
				if( hval != D3D_OK )
					continue;
				
				int x, y;
				DWORD * pLines = NULL;
				for(y=0; y<nHeight; y++)
				{
					pLines = (DWORD *)((BYTE*)rectLocked.pBits + y * rectLocked.Pitch);
					for(x=0; x<nWidth; x++)
					{
						A3DCOLOR color = pLines[x];
						pTempLine[(nHeight - 1 - y) * pitch + (w * nWidth + x) * 3] = A3DCOLOR_GETBLUE(color);
						pTempLine[(nHeight - 1 - y) * pitch + (w * nWidth + x) * 3 + 1] = A3DCOLOR_GETGREEN(color);
						pTempLine[(nHeight - 1 - y) * pitch + (w * nWidth + x) * 3 + 2] = A3DCOLOR_GETRED(color);
					}
				}

				pTempSurface->GetDXSurface()->UnlockRect();
			}

			fseek(fpFile, bmfh.bfOffBits + (N * nHeight - h * nHeight - nHeight) * pitch, SEEK_SET);
			fwrite(pTempLine, pitch * nHeight, 1, fpFile);
		}

		fclose(fpFile);
		pCamera->SetPostProjectTM(IdentityMatrix());
		if( pTempSurface )
		{
			pTempSurface->Release();
			delete pTempSurface;
			pTempSurface = NULL;
		}

		delete [] pTempLine;
	}

	g_pA3DConfig->RT_SetShowFPSFlag(bShowFPS);
	g_pGame->GetConfigs()->SetRTDebugLevel(bRTDebug);

	const ACHAR* szMsg = g_pGame->GetFixedMsgTab()->GetWideString(FIXMSG_SCREENSHOT);
	g_pGame->GetRTDebug()->OutputNotifyMessage(0xff0000ff, szMsg);
	return true;
}
*/
