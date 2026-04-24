 /*
 * FILE: EC_GameUIMan.cpp
 *
 * DESCRIPTION: 
 *
 * CREATED BY: Duyuxin, 2004/8/27
 *
 * HISTORY: 
 *
 * Copyright (c) 2004 Archosaur Studio, All Rights Reserved.
 */

#pragma warning (disable: 4284)

#include "A3DDevice.h"
#include "A2DSprite.h"
#include "A3DStream.h"
#include "A3DTexture.h"
#include "A3DEngine.h"
#include "A3DCamera.h"
#include "A3DFlatCollector.h"
#include "A3DFuncs.h"
#include "A3DViewport.h"

#include "A3DFont.h"
#include "A3DGDI.h"
#include "AF.h"
#include "AMSoundEngine.h"
#include "AMSoundBufferMan.h"
#include "AUI\\AUICTranslate.h"
#include "AUI\\CSplit.h"
#include "AUI\\AUIDef.h"
#include "AUI\A3DFTFont.h"
#include "AUI\\AUICommon.h"
#include "Task\\TaskTemplMan.h"
#include "Task\\TaskTempl.h"
#include "rpcdata\\roleinfo"
#include "EL_Precinct.h"
#include "gnetdef.h"
#include "globaldataman.h"
#include "elementdataman.h"

#include "EC_GameUIMan2.inl"
#include "EC_Configs.h"
#include "EC_FullGlowRender.h"
#include "EC_Global.h"
#include "EC_Game.h"
#include "EC_GameRun.h"
#include "EC_GameSession.h"
#include "EC_GameUIMan.h"
#include "EC_Helper.h"
#include "EC_AutoHelper.h"
#include "EC_HostInputFilter.h"
#include "EC_InputCtrl.h"
#include "EC_ManPlayer.h"
#include "EC_ManNPC.h"
#include "EC_Monster.h"
#include "EC_NPC.h"
#include "EC_HostPlayer.h"
#include "EC_ElsePlayer.h"
#include "EC_Friend.h"
#include "EC_FixedMsg.h"
#include "EC_DealInventory.h"
#include "EC_IvtrTypes.h"
#include "EC_Inventory.h"
#include "EC_IvtrItem.h"
#include "EC_IvtrEquip.h"
#include "EC_IvtrStone.h"
#include "EC_IvtrConsume.h"
#include "EC_NPCServer.h"
#include "EC_Resource.h"
#include "EC_ShadowRender.h"
#include "EC_Shortcut.h"
#include "EC_ShortcutSet.h"
#include "EC_Skill.h"
#include "EC_Team.h"
#include "EC_TaskInterface.h"
#include "EC_Utility.h"
#include "EC_World.h"

#ifdef BACK_VERSION
#include "EC_Viewport.h"
#include "A3DTerrainWater.h"
#else
#include "ECViewport.h"
#include "A3DTerrainWater2.h"
#endif
#include "EC_Faction.h"
#include "EC_Model.h"
#include "EC_AutoMove.h"
#include "EC_Secretary.h"
#include "LuaEvent.h"
#include "EC_HTC2SRawCmds.h"
#include "EC_Cutscene.h"
#include "EC_GameUIMisc.h"
#include "EC_UIData.h"
#include "EC_BattleDungeon.h"
#include "EC_HelpGraphGuide.h"
#include "EC_NewHelpGuide.h"

#include "PredefinedGMHelpMsg.h"

#include "factioninvite.hrp"
#include "sectinvite.hrp"
#include "crssvrteamsinvite.hrp"
#ifdef LUA_UI
#include "LuaBind.h"
#include "LuaUtil.h"
#include "LuaEvent.h"
#include "chatroomlist_re.hpp"
#include "LuaManager.h"
#include "LuaState.h"
#endif

#ifndef LUA_UI
#include "LuaState.h"
#include "LuaUtil.h"
#endif

#include "ScriptValue.h"
#include "LuaScript.h"
#include "LuaFuncCommon.h"

#define  SHORTCUTS_UI_INFO "ShorcutsUIInfo"
#define  LUA_SHORTCUTSUIINFO_FILE  "Interfaces\\script\\interfaces\\shortcutsui.lua"

#define new A_DEBUG_NEW

// 14-15 CECGAMEUIMAN_MAX_TALISMANSLOT 13->14 reserve 2 slot
#define UPDATE_SHOWVERSION			6
#define LAYOUTDATA_VERSION			15
#define CECGAMEUIMAN_MAX_BROKEN		8
#define CECGAMEUIMAN_LOCALLAYOUT_VERSION		5
#define CECGAMEUIMAN_MAX_TASKHINT	6
#define MSGBOX(sid) MessageBox("", GetStringFromTable(sid), MB_OK, A3DCOLORRGBA(255, 255, 255, 160))

static LPCWSTR _pate_font_face = L"方正隶二简体";
static const float _pate_font_size[3]	= {14.0f, 16.0f, 18.0f};
static const float _hint_font_size = 12.0f;

//extern HWND l_hWebWnd;

enum
{
	DLG_CONSOLE = 0,		//	Console
	DLG_PLAYERCONTEXT,		//	Player context
	DLG_CHAT,				//	Chat window
	NUM_DIALOG,
};
static const char* l_aDlgNames[NUM_DIALOG] = 
{
	"Dlg_Console",
	"Win_QuickAction",
	"Win_Chat"
};
static bool RemoveFilesAndDirectory(char *pszPath);

#define CECGAMEUIMAN_ACCOUNTTIME_TIPMAX 28
static const DWORD l_dwAccountTimeTip[CECGAMEUIMAN_ACCOUNTTIME_TIPMAX] = 
{
	1800, 1500, 1200, 900, 600, 540, 480, 420, 360, 300, 240, 
	180, 120, 60, 50, 40, 30, 20, 10, 9, 8, 7, 6, 5, 4, 3, 2, 1
};

#ifdef LUA_UI
#define LUA_GAMEUIMAN  "GameUIMan"
#endif

CECGameUIMan::CECGameUIMan()
{
	int i;

	m_bClickedChatPart = false;
	m_bClickedUserName = false;
	m_bClickedTaskTrace = false;
	m_nMinSize = 0;
	m_pA2DSpriteMask = NULL;
	for( i = 0; i < ICONS_MAX; i++ )
		m_pA2DSpriteIcons[i] = NULL;
	for( i = 0; i < AUIMANAGER_MAX_EMOTIONGROUPS; i++ )
	{
		m_pA2DSpriteEmotion[i] = NULL;
		m_vecEmotion[i] = NULL;
		m_pA2DSpriteEmotionTask[i] = NULL;
		m_vecEmotionTask[i] = NULL;
	}

	for( i = 0; i < CECGAMEUIMAN_MAX_PROFRESSION; i++ )
	{
		m_pA2DSpriteProfs[i] = NULL;
	}

	for(i=0;i<CECGAMEUIMAN_MAX_MONSTER_PROFESSION;i++)
	{
		m_pA2DSpriteMonsterProfs[i] = NULL;
	}

	m_ptLButtonDown.x = m_ptLButtonDown.y = -1;
	m_vecTargetPos = A3DVECTOR3(0.0f);
	
	m_nCursorState = CURSOR_STATE_NORMAL;
	m_pCurPrecinct = NULL;
	m_idCurFinishTask = -1;
	m_pCurNPCEssence = NULL;

	m_idTrade = -1;
	m_pShowOld = NULL;
	m_bShowAllPanels = true;
	m_bAutoReply = false;
	m_bOnlineNotify = false;
	m_bGTOnLineNotify = false;
	m_bSaveHistory = false;
	m_bUIEnable = false;
	m_bReplayMode = false;
	m_nAccountTimeIndex = 0;

	m_bHideUIPrintMode = false;
	m_pPateFontName = NULL;

	m_bShowTouchmin = true;
	m_cUISize       = 2;
}

CECGameUIMan::~CECGameUIMan()
{
}

bool CECGameUIMan::MessageBox(const char *pszName, const ACHAR *pszText, DWORD dwType, A3DCOLOR color, PAUIDIALOG *ppDlg)
{
	return AUILuaManager::MessageBox(pszName, pszText, dwType, color, ppDlg);
}

//	Initalize manager
bool CECGameUIMan::Init(A3DEngine* pA3DEngine, A3DDevice* pA3DDevice, const char* szDCFile/* NULL */)
{
	int i;
	bool bval;
	char szName[40];
	PAUISTILLIMAGEBUTTON pButton;

#ifdef LUA_UI
	CLuaScript::SetLuaState(g_LuaStateMan.GetUIState());

	//char szScript[100];
	//sprintf(szScript, "Interfaces\\script\\%s.lua", LUA_GAMEUIMAN);
	AString  szScript;
	szScript.Format("Interfaces\\script\\%s.lua", LUA_GAMEUIMAN);
	TLuaFile * pLuaFile = new TLuaFile(szScript, this);
	if (!pLuaFile->Update(true))
	{
		assert(0);
		delete pLuaFile;
		//char msg[200];
		//sprintf(msg, "load %s fail\n", szScript);
		AString msg;
		msg.Format("load %s fail\n", szScript);
		LUA_DEBUG_INFO(msg);
		return false;
	}
	g_LuaFileMan.AddScript(pLuaFile);
#endif

	m_pA3DDevice = pA3DDevice;
	m_vecImageList.clear();
	
	A3DRECT aRects[64];
	LoadIcon("icon\\Transfer.tga", 0, 0, AUI_COLORKEY);	// ICONLIST_TRANSFER
	LoadIcon("icon\\GTonline.png");						// ICONLIST_GTICON
	LoadIcon("icon\\King16.tga");                     // ICONLIST_KINGDOM
	LoadIcon("icon\\GToffline.png");					// ICONLIST_GTOFFLINE
	LoadIcon("icon\\Quest_Recommend.tga");				// ICONLIST_RECOMMEND
	
	
	for (i=0; i < 3; i++)
		aRects[i] = A3DRECT(0, i*18, 36, i*18+18);
	// aRects[i] = A3DRECT(0, i*36, 72, i*36+36);
	LoadIcon("icon\\channelicon.tga", 3, aRects);	// ICONLIST_CHANNEL

//	LoadIcon("icon\\Quest_RecommendBlank.tga");
	bval = LoadIconSet();
	int dumb;	// 将物品悬浮图标首先加入，界面中还会再加入其它
	AddImageListItem(GetIconSet(ICONS_GEM16), dumb);	
	AddImageListItem(GetIconSet(ICONS_RUNE), dumb);	
	SetImageList(&m_vecImageList);

	if( !bval ) return AUI_ReportError(__LINE__, __FILE__);
	
	m_pUIData = new CECUIData();
	bval = m_pUIData->Init();
	if (!bval)
	{
		return false;
	}

	// init manager
	DWORD dwTime = GetTickCount();
	if (!AUILuaManager::Init(pA3DEngine, pA3DDevice, szDCFile))
	{
		a_LogOutput(1, "CECGameUIMan::Init, Failed to call AUILuaManager::Init.");
		return false;
	}

	PAUIDIALOG pShow = GetDialog("Win_Popface");
	pShow->SetData(AUIMANAGER_MAX_EMOTIONGROUPS);

	m_pPateFontName = m_pFontMan->CreatePointFont(_pate_font_size[1], _pate_font_face, STYLE_NORMAL, 0, 0, 0, 0, false, 0, 0, true);
	if( !m_pPateFontName ) return AUI_ReportError(__LINE__, 1, "AUIManager::Init(), failed to call m_pFontMan->CreatePointFont()");
	
	m_FontHint.nShadow = a_atoi(GetStringFromTable(12));
	int nBold = a_atoi(GetStringFromTable(11));
	m_FontHint.pFont = m_pFontMan->CreatePointFont(_hint_font_size, GetStringFromTable(10), nBold>0?STYLE_BOLD:STYLE_NORMAL, 0, 0, 0, 0, false, 0, 0, true);
	if( !m_FontHint.pFont ) return AUI_ReportError(__LINE__, 1, "AUIManager::Init(), failed to call m_pFontMan->CreatePointFont()");
	m_pvecHintOtherFonts = new abase::vector<FONT_WITH_SHADOW>;
	m_pvecHintOtherFonts->push_back(m_FontHint);

	DWORD timeInit = GetTickCount()-dwTime;
	dwTime = GetTickCount();


	InitDialogs();
	DWORD timeinit2 = GetTickCount()-dwTime;

	for (i = 0; i < CECGAMEUIMAN_MAX_QSHOPITEMS; i++)
	{
		AString strDlgName;
		strDlgName.Format("Win_QShopItemDefault%d", i);
		m_pDlgQShopDefault[i] = static_cast<CDlgQShopItem *>(GetDialog(strDlgName));
	}
	m_pDlgSettingCurrent = m_pDlgSettingSystem;
	for (i = 0; i < CECGAMEUIMAN_MAX_PROFRESSION; i++)
	{
		AString strDlgName;
		strDlgName.Format("Win_Skill%02d", i);
		m_pDlgSkillTree[i] = static_cast<CDlgSkillTree *>(GetDialog(strDlgName));

		strDlgName.Format("Win_Talent_%02d", i);
		m_pDlgTalent[i] = static_cast<CDlgSkillTree *>(GetDialog(strDlgName));
	}

	for (i = 0; i < 9; i++)
	{
		AString strDlgName;
		strDlgName.Format("Win_TeamMate%d", i + 1);
		m_pDlgTeamMate[i] = static_cast<CDlgTeamMate *>(GetDialog(strDlgName));
	}

	for (i = 0; i <	CECGAMEUIMAN_MAX_MINIMIZEWINDOWS; i++)
	{
		AString strDlgName;
		strDlgName.Format("Win_MinimizeBar%d", i);
		m_pDlgMinimizeBar[i] = static_cast<CDlgMinimizeBar *>(GetDialog(strDlgName));
	}

	m_pDlgMinimizeBar[0]->AlignTo(m_pDlgSystem,
			AUIDIALOG_ALIGN_INSIDE, AUIDIALOG_ALIGN_RIGHT,
			AUIDIALOG_ALIGN_OUTSIDE, AUIDIALOG_ALIGN_TOP);
	
	GetDialog("Win_Gt")->AlignTo(m_pDlgHost,
		AUIDIALOG_ALIGN_OUTSIDE, AUIDIALOG_ALIGN_RIGHT,
			AUIDIALOG_ALIGN_INSIDE, AUIDIALOG_ALIGN_TOP);

	m_pDlgDoubleExpS->AlignTo(GetDialog("Win_Gt"),
			AUIDIALOG_ALIGN_OUTSIDE, AUIDIALOG_ALIGN_RIGHT,
			AUIDIALOG_ALIGN_INSIDE, AUIDIALOG_ALIGN_TOP);
	m_pDlgDoubleExpS->SetCanMove(false);
		
	m_pDlgChat1->RearrangeWindow();
	m_pDlgChat1->SetCanMove(false);
	m_pDlgChat2->SetCanMove(false);
	GetDialog("Win_ChatSpecInfo")->SetCanMove(false);
	
	GetDialog("Win_Worldmap_Button")->AlignTo(GetDialog("Win_WorldMap"),
		AUIDIALOG_ALIGN_INSIDE, AUIDIALOG_ALIGN_RIGHT,
		AUIDIALOG_ALIGN_INSIDE, AUIDIALOG_ALIGN_BOTTOM);
	GetDialog("Win_Worldmap_Button")->SetPosEx(GetDialog("Win_Worldmap_Button")->GetPos().x-int(42*m_fWindowScale), GetDialog("Win_Worldmap_Button")->GetPos().y-int(34*m_fWindowScale));

	//	Load configs strings
	if (g_pGame->GetConfigs()->HasConsole())
	{
		if (!m_ConsoleCmds.Init("Configs\\console_cmd.txt", false))
			glb_ErrorOutput(ECERR_FAILEDTOCALL, "CECGameUIMan::Init", __LINE__);
	}

	m_pDlgDragDrop->SetCanOutOfWindow(true);

//	PAUIEDITBOX pEdit;

	for( i = 0; ; i++ )
	{
		sprintf(szName, "Btn_Set%d", i + 1);
		pButton = (PAUISTILLIMAGEBUTTON)m_pDlgShop->GetDlgItem(szName);
		if( !pButton ) break;
		pButton->SetPushLike(true);
	}

	PAUIDIALOG pDlg = GetDialog("Win_Broken");
	pDlg->SetPosEx(0, GetDialog("Win_MiniMap")->GetRect().bottom, AUIDialog::alignMax);

	GetDialog("Win_Hint")->GetDlgItem("Txt_Hint")->SetTransparent(true);

	// Bad Words
	AWScriptFile wsf;
	bval = wsf.Open("Configs\\BadWords.txt");
	if( !bval ) return AUI_ReportError(__LINE__, __FILE__);
	while( wsf.GetNextToken(true) )
	{
		ACString strToken((const ACHAR *)wsf.m_szToken);
		strToken.MakeLower();
		m_vecBadWords.push_back(strToken);
	}
	wsf.Close();

	// Item Show Order
	AScriptFile sf;
	bval = sf.Open("Configs\\item_order.txt");
	if( !bval )	return AUI_ReportError(__LINE__, __FILE__);
	i = 0;
	while( !sf.IsEnd() )
	{
		bval = sf.GetNextToken(true);
		if( !bval ) break;		// End of file.
		int id = atoi(sf.m_szToken);
		m_IDOrderMap[id] = i++;
	}
	sf.Close();

	// Initialize Qshop keyword map.
	AWScriptFile sfQshop;
	if (sfQshop.Open("Configs\\Qshop.txt"))
	{
		while( sfQshop.GetNextToken(true) )
		{
			AString keyword = WC2AS( sfQshop.m_szToken );
			sfQshop.GetNextToken(true);
			
			m_QshopKeywordMap[keyword] = sfQshop.m_szToken;
		}
		sfQshop.Close();
	}

	m_pDlgTarget = NULL;
	g_pGame->GetGameRun()->GetHelper()->Reset();
	g_ECAutoMove.ClearPath(true);

	const GNET::RoleInfo& info = g_pGame->GetGameRun()->GetSelectedRoleInfo();
	if (info.lastlogin_time < (info.create_time+10) && g_pGame->GetGameSession()->IsHasReferrer()) // 第一次登陆
	{
		GetDialog("Win_PopFriend")->Show(true);
	}

	m_pDlgChatNormal->OnCommand_set("set1");
	m_pDlgChatPk->OnCommand_set("set1");
	
	pDlg = GetDialog("Win_PKScoreHover2013");
	if (pDlg)
	{
		pDlg->SetCanMove(false);
		pDlg->AlignTo(GetDialog("Win_CharHead"),
			AUIDIALOG_ALIGN_OUTSIDE, AUIDIALOG_ALIGN_RIGHT,
			AUIDIALOG_ALIGN_INSIDE, AUIDIALOG_ALIGN_BOTTOM);
	}
	pDlg = GetDialog("Win_Battle_Dungeon_6v6_Score");
	if (pDlg)
	{
		pDlg->SetCanMove(false);
		pDlg->AlignTo(GetDialog("Win_CharHead"),
			AUIDIALOG_ALIGN_OUTSIDE, AUIDIALOG_ALIGN_RIGHT,
			AUIDIALOG_ALIGN_INSIDE, AUIDIALOG_ALIGN_TOP);
	}
	pDlg = GetDialog("Win_Battle6V6ScoreBoardS");
	if (pDlg)
	{
		pDlg->AlignTo(GetDialog("Win_CharHead"),
			AUIDIALOG_ALIGN_OUTSIDE, AUIDIALOG_ALIGN_RIGHT,
			AUIDIALOG_ALIGN_OUTSIDE, AUIDIALOG_ALIGN_BOTTOM);
	}
	pDlg = GetDialog("Win_Battle6V6ScoreBoardL");
	if (pDlg)
	{
		pDlg->AlignTo(GetDialog("Win_CharHead"),
			AUIDIALOG_ALIGN_OUTSIDE, AUIDIALOG_ALIGN_RIGHT,
			AUIDIALOG_ALIGN_OUTSIDE, AUIDIALOG_ALIGN_BOTTOM);
	}
	pDlg = GetDialog("Win_Help_Hint");
	if (pDlg)
	{
		pDlg->AlignTo(GetDialog("Win_MiniMap"),
			AUIDIALOG_ALIGN_INSIDE, AUIDIALOG_ALIGN_RIGHT,
			AUIDIALOG_ALIGN_OUTSIDE, AUIDIALOG_ALIGN_BOTTOM);
	}
	pDlg = GetDialog("Win_Help_Hint2");
	if (pDlg)
	{
		pDlg->AlignTo(GetDialog("Win_Help_Hint"),
			AUIDIALOG_ALIGN_INSIDE, AUIDIALOG_ALIGN_RIGHT,
			AUIDIALOG_ALIGN_OUTSIDE, AUIDIALOG_ALIGN_BOTTOM);
	}
	pDlg = GetDialog("Win_Equip_Fengshen_Prgs");
	if (pDlg)
	{
		//pDlg->SetCanMove(false);
		pDlg->AlignTo(GetDialog("Win_CharHead"),
			AUIDIALOG_ALIGN_OUTSIDE, AUIDIALOG_ALIGN_RIGHT,
			AUIDIALOG_ALIGN_INSIDE, AUIDIALOG_ALIGN_BOTTOM);
	}
	GetDialog("Win_Battle_Dungeon_SiX_Time")->AlignTo(GetDialog("Win_CharHead"),
		AUIDIALOG_ALIGN_OUTSIDE, AUIDIALOG_ALIGN_RIGHT,
		AUIDIALOG_ALIGN_INSIDE, AUIDIALOG_ALIGN_TOP);

	GetDialog("Win_Battle_Dungeon_Level_Time")->AlignTo(GetDialog("Win_CharHead"),
		AUIDIALOG_ALIGN_OUTSIDE, AUIDIALOG_ALIGN_RIGHT,
		AUIDIALOG_ALIGN_INSIDE, AUIDIALOG_ALIGN_TOP);
	GetDialog("Win_Battle_Dungeon_Level_Time")->Show(false);

	pDlg = GetDialog("Win_Charhead_HpPrgs");
	if(pDlg)
	{
		pDlg->SetCanMove(false);
		pDlg->AlignTo(m_pDlgHost,
			AUIDIALOG_ALIGN_INSIDE, AUIDIALOG_ALIGN_RIGHT,
			AUIDIALOG_ALIGN_INSIDE, AUIDIALOG_ALIGN_BOTTOM);
	}

	pDlg = GetDialog("Win_Target_HpPrgs");
	if(pDlg)
	{
		pDlg->SetCanMove(false);
		pDlg->AlignTo(GetDialog("Win_TargetPlayer"),
			AUIDIALOG_ALIGN_INSIDE, AUIDIALOG_ALIGN_RIGHT,
			AUIDIALOG_ALIGN_INSIDE, AUIDIALOG_ALIGN_BOTTOM);
	}

	memset(&(m_iTalismanSkillDumb[0]), 0, CECGAMEUIMAN_MAX_TALISMANSLOT*sizeof(int));
	m_iCurWallowLevel = 0;
	CECBattleDungeonMan::getSingletonPtr()->SetInTower(false);

	InitShortCutsUIInfo();

	return true;
}

bool CECGameUIMan::LoadIcon(char* szFile, int iNumItem, A3DRECT* aRects, A3DCOLOR colorKey)
{
	A2DSprite* pImage = new A2DSprite;
	if (!pImage)
	{
		a_LogOutput(1, "CECGameUIMan::LoadIcon, Failed to create A2DSprite object");
		return false;
	}
	
	if (!pImage->Init(m_pA3DDevice, szFile, colorKey))
	{
		delete pImage;
		a_LogOutput(1, "CECGameUIMan::LoadIcon, Failed to initialize A2DSprite object from file %s", szFile);
		return false;
	}
	if (iNumItem>1)
		pImage->ResetItems(iNumItem, aRects);
	m_vecImageListControl.push_back(pImage);
	return true;
}

bool CECGameUIMan::LoadIconSet()
{
	bool bval;
	DWORD dwRead;
	AFileImage fi;
	int h, i, j, nIndex;
	A3DRECT *a_rc[ICONS_MAX];
	char szFile[MAX_PATH], szLine[AFILE_LINEMAXLEN];
	int W = 32, H = 32, a_nCountX[ICONS_MAX], a_nCountY[ICONS_MAX];
	GNET::RoleInfo Info = g_pGame->GetGameRun()->GetSelectedRoleInfo();
	char a_szIconFile[ICONS_MAX][40] = { "Action", "Skill", "Ivtr", "State", "SkillGrp", "Guild", "Pet", "Lottery", 
		"Poker", "garden", "Dailypromp24", "Dailypromp32", "Dailypromp48", "Gem16", "Rune16", "xp", "Rune16"/*TODO: 前面这个Rune16是暂替星座图标的,有正式图标的话,需对应修改"*/,
	    "townnpcmonster"};

	for( h = 0; h < ICONS_MAX; h++ )
	{
		sprintf(szFile, "%s\\Surfaces\\IconSet\\IconList_%s.txt",
			af_GetBaseDir(), a_szIconFile[h]);
		bval = fi.Open(szFile, AFILE_OPENEXIST | AFILE_TEXT | AFILE_TEMPMEMORY);
		if( !bval ) return AUI_ReportError(__LINE__, __FILE__);

		fi.ReadLine(szLine, sizeof(szLine), &dwRead);
		W = atoi(szLine);

		fi.ReadLine(szLine, sizeof(szLine), &dwRead);
		H = atoi(szLine);

		fi.ReadLine(szLine, sizeof(szLine), &dwRead);
		a_nCountY[h] = atoi(szLine);

		fi.ReadLine(szLine, sizeof(szLine), &dwRead);
		a_nCountX[h] = atoi(szLine);

		a_rc[h] = new A3DRECT[a_nCountX[h] * a_nCountY[h]];
		if( !a_rc[h] ) return AUI_ReportError(__LINE__, __FILE__);

		for( i = 0; i < a_nCountY[h]; i++ )
		{
			for( j = 0; j < a_nCountX[h]; j++ )
			{
				nIndex = i * a_nCountX[h] + j;
				a_rc[h][nIndex].SetRect(j * W, i * H, j * W + W, i * H + H);

				bval = fi.ReadLine(szLine, sizeof(szLine), &dwRead);
				if( dwRead > 0 && strlen(szLine) > 0 )
					m_IconMap[h][AString(szLine)] = nIndex;
			}
		}

		fi.Close();

		m_pA2DSpriteIcons[h] = new A2DSprite;
		if( !m_pA2DSpriteIcons[h] ) return AUI_ReportError(__LINE__, __FILE__);

		m_pA2DSpriteIcons[h]->SetDynamicTex(true);
		sprintf(szFile, "IconSet\\IconList_%s.dds", a_szIconFile[h]);
		bval = m_pA2DSpriteIcons[h]->Init(m_pA3DDevice, szFile, AUI_COLORKEY);
		if( !bval ) return AUI_ReportError(__LINE__, __FILE__);

		if( h == ICONS_INVENTORY )
			g_pGame->GetGameRun()->StepLoadProgress(5);
		else
			g_pGame->GetGameRun()->StepLoadProgress(1);
	}

	for( h = 0; h < ICONS_MAX; h++ )
	{
		bval = m_pA2DSpriteIcons[h]->ResetItems(
			a_nCountX[h] * a_nCountY[h], a_rc[h]);
		if( !bval ) return AUI_ReportError(__LINE__, __FILE__);

		delete [] a_rc[h];
	}

	m_pA2DSpriteMask = new A2DSprite;
	if( !m_pA2DSpriteMask ) return AUI_ReportError(__LINE__, __FILE__);

	bval = m_pA2DSpriteMask->Init(m_pA3DDevice,
		"InGame\\IconHighlight.dds", AUI_COLORKEY);
	if( !bval ) return AUI_ReportError(__LINE__, __FILE__);

	// Emotions.
	int l;
	for(l = 0; l < AUIMANAGER_MAX_EMOTIONGROUPS; l++ )
	{
		char szFile[MAX_PATH];
		sprintf(szFile, "InGame\\Emotions%d.dds", l);
		m_pA2DSpriteEmotion[l] = new A2DSprite;
		if( !m_pA2DSpriteEmotion[l] ) return AUI_ReportError(__LINE__, __FILE__);
		bval = m_pA2DSpriteEmotion[l]->Init(m_pA3DDevice, szFile, AUI_COLORKEY);
		if( !bval ) break;
		m_vecEmotion[l] = new abase::vector<AUITEXTAREA_EMOTION>;
		if( !m_vecEmotion[l] ) return AUI_ReportError(__LINE__, __FILE__);

		W = H = 32;

		A3DRECT *rc;
		int nNumX = m_pA2DSpriteEmotion[l]->GetWidth() / W;
		int nNumY = m_pA2DSpriteEmotion[l]->GetHeight() / H;

		rc = new A3DRECT[nNumX * nNumY];
		for( i = 0; i < nNumY; i++ )
		{
			for( j = 0; j < nNumX; j++ )
			{
				nIndex = i * nNumX + j;
				rc[nIndex].SetRect(j * W, i * H, j * W + W, i * H + H);
			}
		}
		m_pA2DSpriteEmotion[l]->ResetItems(nNumX * nNumY, rc);
		delete [] rc;

		AWScriptFile s;
		AUITEXTAREA_EMOTION em;
		sprintf(szFile, "Surfaces\\InGame\\Emotions%d.txt", l);
		bval = s.Open(szFile);
		if( !bval ) return AUI_ReportError(__LINE__, __FILE__);

		while( s.GetNextToken(true) )
		{
			em.nStartPos = a_atoi(s.m_szToken);
			s.GetNextToken(true);
			em.nNumFrames = a_atoi(s.m_szToken);

			s.GetNextToken(true);
			em.strHint = s.m_szToken;
			for(i = 0; i < em.nNumFrames; i++ )
			{
				s.GetNextToken(true);
				em.nFrameTick[i] = a_atoi(s.m_szToken);
			}
			m_vecEmotion[l]->push_back(em);
		}
		s.Close();
	}
	// professions
	for( l = 0; l < CECGAMEUIMAN_MAX_PROFRESSION; l++ )
	{
		char szFile[MAX_PATH];
		sprintf(szFile, "Icon\\Pro%d.tga", l);
		m_pA2DSpriteProfs[l] = new A2DSprite;
		if( !m_pA2DSpriteProfs[l] ) return AUI_ReportError(__LINE__, __FILE__);
		bval = m_pA2DSpriteProfs[l]->Init(m_pA3DDevice, szFile, AUI_COLORKEY);
		if( !bval ) // cannot break here because prof is not consistent
		{
			A3DRELEASE(m_pA2DSpriteProfs[l]);
		}
			
	}
	// monster professions
	for(l=0;l<CECGAMEUIMAN_MAX_MONSTER_PROFESSION;l++)
	{
		char szFile[MAX_PATH];
		sprintf(szFile, "Icon\\monsterrace%d.tga", l+1);
		m_pA2DSpriteMonsterProfs[l] = new A2DSprite;
		if( !m_pA2DSpriteMonsterProfs[l] ) return AUI_ReportError(__LINE__, __FILE__);
		bval = m_pA2DSpriteMonsterProfs[l]->Init(m_pA3DDevice, szFile, AUI_COLORKEY);
		if( !bval ) // cannot break here because prof is not consistent
		{
			A3DRELEASE(m_pA2DSpriteMonsterProfs[l]);
		}
	}

	// emotiontask
	for( l = 0; l < AUIMANAGER_MAX_EMOTIONGROUPS; l++ )
	{
		char szFile[MAX_PATH];
		sprintf(szFile, "InGame\\taskEmotions%d.dds", l);
		m_pA2DSpriteEmotionTask[l] = new A2DSprite;
		if( !m_pA2DSpriteEmotionTask[l] ) return AUI_ReportError(__LINE__, __FILE__);
		bval = m_pA2DSpriteEmotionTask[l]->Init(m_pA3DDevice, szFile, AUI_COLORKEY);
		if( !bval ) break;
		m_vecEmotionTask[l] = new abase::vector<AUITEXTAREA_EMOTION>;
		if( !m_vecEmotionTask[l] ) return AUI_ReportError(__LINE__, __FILE__);
		
		W = H = 32;
		
		A3DRECT *rc;
		int nNumX = m_pA2DSpriteEmotionTask[l]->GetWidth() / W;
		int nNumY = m_pA2DSpriteEmotionTask[l]->GetHeight() / H;
		
		rc = new A3DRECT[nNumX * nNumY];
		for( i = 0; i < nNumY; i++ )
		{
			for( j = 0; j < nNumX; j++ )
			{
				nIndex = i * nNumX + j;
				rc[nIndex].SetRect(j * W, i * H, j * W + W, i * H + H);
			}
		}
		m_pA2DSpriteEmotionTask[l]->ResetItems(nNumX * nNumY, rc);
		delete [] rc;
		
		AWScriptFile s;
		AUITEXTAREA_EMOTION em;
		sprintf(szFile, "Surfaces\\InGame\\taskEmotions%d.txt", l);
		bval = s.Open(szFile);
		if( !bval ) return AUI_ReportError(__LINE__, __FILE__);
		
		while( s.GetNextToken(true) )
		{
			em.nStartPos = a_atoi(s.m_szToken);
			s.GetNextToken(true);
			em.nNumFrames = a_atoi(s.m_szToken);
			
			s.GetNextToken(true);
			em.strHint = s.m_szToken;
			for(i = 0; i < em.nNumFrames; i++ )
			{
				s.GetNextToken(true);
				em.nFrameTick[i] = a_atoi(s.m_szToken);
			}
			m_vecEmotionTask[l]->push_back(em);
		}
		s.Close();
	}
	
	AScriptFile sf;
	if( sf.Open("Configs\\Iconsound.txt") )
	{
		int idSubType;
		AString strWave;

		while( sf.GetNextToken(true) )
		{
			idSubType = atoi(sf.m_szToken);
			sf.GetNextToken(true);
			strWave = sf.m_szToken;
			m_IconSound[idSubType] = strWave;
		}

		sf.Close();
	}

	return true;
}

//	Release manager
bool CECGameUIMan::Release(void)
{
#ifdef LUA_UI
	LuaRelease();
#endif
	bool bRet = AUILuaManager::Release();

	int i;
	
	for( i = 0; i < (int)m_vecImageListControl.size(); i++ )
		A3DRELEASE(m_vecImageListControl[i]);
	m_vecImageListControl.clear();

	g_pGame->GetConfigs()->SaveSystemSettings();

	m_ConsoleCmds.Release();

	for( i = 0; i < AUIMANAGER_MAX_EMOTIONGROUPS; i++ )
	{
		if( m_vecEmotion[i] )
		{
			m_vecEmotion[i]->clear();
			delete m_vecEmotion[i];
		}
		A3DRELEASE(m_pA2DSpriteEmotion[i]);
		if( m_vecEmotionTask[i] )
		{
			m_vecEmotionTask[i]->clear();
			delete m_vecEmotionTask[i];
		}
		A3DRELEASE(m_pA2DSpriteEmotionTask[i]);
	}
	for(i=0;i<CECGAMEUIMAN_MAX_PROFRESSION;i++)
	{
		A3DRELEASE(m_pA2DSpriteProfs[i]);
	}
	for(i=0;i<CECGAMEUIMAN_MAX_MONSTER_PROFESSION;i++)
	{
		A3DRELEASE(m_pA2DSpriteMonsterProfs[i]);
	}

	A3DRELEASE(m_pA2DSpriteMask);
	for( i = 0; i < ICONS_MAX; i++ )
	{
		m_IconMap[i].clear();
		A3DRELEASE(m_pA2DSpriteIcons[i]);
	}
	for( i = 0; i < (int)m_mapA2DSpriteGuildIcons.size(); ++i)
		A3DRELEASE(m_mapA2DSpriteGuildIcons[i]);
	m_mapA2DSpriteGuildIcons.clear();

	abase::hash_map<AString, A2DSprite*>::iterator  it = m_mapA2dSpriteTitle.begin();
	for( ; it != m_mapA2dSpriteTitle.end(); ++it)
		A3DRELEASE(it->second);
	m_mapA2dSpriteTitle.clear();

	m_IconSound.clear();
	if (m_pvecHintOtherFonts)
	{
		delete m_pvecHintOtherFonts;
		m_pvecHintOtherFonts = NULL;
	}

	A3DRELEASE(m_pUIData);

	ReleaseShortCutsUIInfo();

	return bRet;
}

//	Handle windows message
bool CECGameUIMan::DealWindowsMessage(UINT uMsg, WPARAM wParam, LPARAM lParam)
{
	if (!m_bUIEnable)
	{
		PAUIDIALOG pDlg = GetDialog("MsgBox_LinkBroken");
		if( pDlg ) pDlg->DealWindowsMessage(uMsg, wParam, lParam, false);
		return false;
	}

	bool bRet = false;

	if( m_pDlgMouseOn && m_nCursorState == CURSOR_STATE_NORMAL &&
		(m_pDlgMouseOn->GetAlpha() > 0 || m_pObjMouseOn) &&
		0 != stricmp(m_pDlgMouseOn->GetName(), "Win_Chat") &&
		0 != stricmp(m_pDlgMouseOn->GetName(), "Win_KfpkChat") &&
		0 != stricmp(m_pDlgMouseOn->GetName(), "Win_QuestMinion") &&
		!(0 == stricmp(m_pDlgMouseOn->GetName(), "Win_Quest") && m_pObjMouseOn && 
		(0 == stricmp(m_pObjMouseOn->GetName(), "Txt_QuestItem") || 0 == stricmp(m_pObjMouseOn->GetName(), "Txt_QuestList"))) &&
		0 != stricmp(m_pDlgMouseOn->GetName(), "Win_ReplayEdit") )
	{
		g_pGame->ChangeCursor(RES_CUR_NORMAL);
	}

	if( m_bShowAllPanels )
	{
		m_bClickedChatPart = false;
		m_bClickedUserName = false;
		m_bClickedTaskTrace = false;

		if( uMsg == WM_INPUTLANGCHANGEREQUEST &&
			ImmIsIME(reinterpret_cast<HKL>(lParam)) &&
			(!m_pDlgActive || !m_pDlgActive->GetFocus() ||
			m_pDlgActive->GetFocus()->GetType() != AUIOBJECT_TYPE_EDITBOX ))
		{
			return true;
		}

		bRet = AUILuaManager::DealWindowsMessage(uMsg, wParam, lParam);
		if( bRet && ((m_bClickedChatPart && !m_bClickedUserName) || m_bClickedTaskTrace))
			bRet = false;
		else if( m_pDlgMouseOn )
		{
			if( WM_RBUTTONUP == uMsg )
			{
				CECHostInputFilter::PRESS press = g_pGame->GetGameRun()->GetHostInputFilter()->GetRBtnPressInfo();
				if( press.bTurnCamera ) bRet = false;
			}
			else if( WM_LBUTTONDBLCLK == uMsg )
				bRet = true;
		}

		if( GetDialog("Win_Popface")->IsShow() &&
			WM_LBUTTONUP == uMsg && (!m_pDlgMouseOn || (m_pDlgMouseOn
			&& !strstr(m_pDlgMouseOn->GetName(), "Win_FriendChat")
			&& 0 != stricmp(m_pDlgMouseOn->GetName(), "Win_Chat")
			&& 0 != stricmp(m_pDlgMouseOn->GetName(), "Win_KfpkChat")
			&& 0 != stricmp(m_pDlgMouseOn->GetName(), "Win_Popface")
			&& 0 != stricmp(m_pDlgMouseOn->GetName(), "Win_PopTalk")
			&& 0 != stricmp(m_pDlgMouseOn->GetName(), "Win_WhisperChat") 
			&& 0 != stricmp(m_pDlgMouseOn->GetName(), "Win_ChannelChat") 
			&& 0 != stricmp(m_pDlgMouseOn->GetName(), "Win_ChatSpec"))) )
		{
			GetDialog("Win_Popface")->Show(false);
		}
// 		if( GetDialog("Win_PopTalk")->IsShow() &&
// 			WM_LBUTTONUP == uMsg && (!m_pDlgMouseOn || (m_pDlgMouseOn
// 			&& !strstr(m_pDlgMouseOn->GetName(), "Win_FriendChat")
// 			&& 0 != stricmp(m_pDlgMouseOn->GetName(), "Win_Chat")
// 			&& 0 != stricmp(m_pDlgMouseOn->GetName(), "Win_PopTalk")
// 			&& 0 != stricmp(m_pDlgMouseOn->GetName(), "Win_Popface")
// 			&& 0 != stricmp(m_pDlgMouseOn->GetName(), "Win_WhisperChat") 
// 			&& 0 != stricmp(m_pDlgMouseOn->GetName(), "Win_ChannelChat"))) )
// 		{
// 			GetDialog("Win_PopTalk")->Show(false);
// 		}

		if( GetDialog("Win_Game_Acupoint")->IsActive() && WM_KEYDOWN == uMsg && wParam == VK_SPACE)
		{
			bRet = true;
		}
	}

	{
		if( !GetDialog("Win_QuickBar_Setting")->IsShow() )
			m_bSettingHotKey = false;
		if( m_bSettingHotKey && (uMsg == WM_KEYDOWN || uMsg == WM_KEYUP || uMsg == WM_SYSKEYDOWN || uMsg == WM_SYSKEYUP) )
		{
			if( wParam != VK_MENU && (uMsg == WM_KEYUP || uMsg == WM_SYSKEYUP) )
			{
				CDlgQuickBarSetting* pSettingHotkey = (CDlgQuickBarSetting*)GetDialog("Win_QuickBar_Setting");
				if( uMsg == WM_KEYUP && wParam == VK_ESCAPE )
				{
					pSettingHotkey->ClearHotKey();
				}
				else
				{
					DWORD dwCtrl = 0;
					if( AUI_PRESS(VK_SHIFT) )
						dwCtrl |= EC_KSF_SHIFT;
					if( AUI_PRESS(VK_MENU) )
						dwCtrl |= EC_KSF_ALT;
					if( AUI_PRESS(VK_CONTROL) )
						dwCtrl |= EC_KSF_CONTROL;
					pSettingHotkey->SetHotKey(wParam, dwCtrl);
				}
			}
			return true;
		}

	}

	int i;
	char szName[40];
	if( bRet )
	{
		if( (uMsg == WM_KEYDOWN || uMsg == WM_KEYUP) && wParam >= VK_F1 && wParam <= VK_F8 )
		{

			DealGameHotkeys(uMsg, wParam, lParam);
			m_pDlgChat->ChangeFocus(NULL);
			m_pDlgChannelChat->ChangeFocus(NULL);

			for( i = 0; i < CECGAMEUIMAN_MAX_CHATS; i++ )
			{
				sprintf(szName, "Win_FriendChat%d", i);
				GetDialog(szName)->ChangeFocus(NULL);
			}
		}
	}
	else
	{
		if( uMsg == WM_KEYUP || uMsg == WM_CHAR || uMsg == WM_KEYDOWN || uMsg == WM_SYSCHAR )
			DealGameHotkeys(uMsg, wParam, lParam);
		else if( uMsg == WM_LBUTTONDOWN || uMsg == WM_LBUTTONDBLCLK )
		{
			m_pDlgChat->ChangeFocus(NULL);
			m_pDlgChannelChat->ChangeFocus(NULL);
			for( i = 0; i < CECGAMEUIMAN_MAX_CHATS; i++ )
			{
				sprintf(szName, "Win_FriendChat%d", i);
				GetDialog(szName)->ChangeFocus(NULL);
			}
		}
	}

	if( m_nCursorState != CURSOR_STATE_NORMAL )
	{
		if( uMsg == WM_MOUSEMOVE )
			bRet = true;
		else if( uMsg == WM_RBUTTONDOWN )
		{
			m_nCursorState = CURSOR_STATE_NORMAL;
			g_pGame->ChangeCursor(RES_CUR_NORMAL);
			bRet = true;
		}
		else
			bRet = bRet;
	}
	else
		bRet = bRet;

	return bRet;
}

bool CECGameUIMan::DealGameHotkeys(UINT uMsg, WPARAM wParam, LPARAM lParam)
{
	CECHostPlayer *pHost = g_pGame->GetGameRun()->GetHostPlayer();
	if( pHost->IsChangingFace() ) return true;
	TCHAR ch = g_pGame->GetMappingKeyCode((TCHAR)wParam);	

	if( m_pDlgActive && m_pDlgActive->GetName() == "Win_Message15" || ch == VK_RETURN)
	{
		GetDialog("Win_Message15")->Show(false);
	}

	/*if( uMsg == WM_KEYDOWN && ch == VK_RETURN && m_bShowAllPanels && 
		(!m_pDlgActive || !m_pDlgActive->GetFocus() || m_pDlgActive->
		GetFocus()->GetType() != AUIOBJECT_TYPE_EDITBOX) )
	{
		PAUIOBJECT pObj = m_pDlgChat->GetDlgItem("DEFAULT_Txt_Speech");

		BringWindowToTop(m_pDlgChat);
		LuaEvent::FirstOpenChat();
		pObj->SetFocus(true);
		m_pDlgChat->ChangeFocus(pObj);
	}
	else if( m_bShowAllPanels && (uMsg == WM_CHAR || uMsg == WM_SYSCHAR) )
	{
		if( pHost->GetCurPetId() )
		{
			if( ch >= '1' && ch <= '9' && AUI_PRESS(VK_MENU) )
			{
				if( pHost->GetCurPetId() )
				{
					if( ch == '1' )
					{
						m_pDlgPetSkill->OnCommandAttack("");
					}
					else
					{
						m_pDlgPetSkill->CastSkill(ch - '2', false);
					}
				}
				return true;
			}
			else if( ch == 's' && AUI_PRESS(VK_MENU) )
			{
				m_pDlgPetSkill->OnCommandStop("");
				return true;
			}
			else if( ch == 'f' && AUI_PRESS(VK_MENU) )
			{
				m_pDlgPetSkill->OnCommandFollow("");
				return true;
			}
			else if( ch == 'a' && AUI_PRESS(VK_MENU) )
			{
				m_pDlgPetSkill->OnCommandOffensive("");
				return true;
			}
			else if( ch == 'd' && AUI_PRESS(VK_MENU) )
			{
				m_pDlgPetSkill->OnCommandDefensive("");
				return true;
			}
			else if( ch == 'v' && AUI_PRESS(VK_MENU) )
			{
				m_pDlgPetSkill->OnCommandCombat("");
				return true;
			}
		}

		*/

		//if( ch == 'm' || ch == 'M' )
		//{
//			if( m_pDlgGuildMap->IsShow() )
//				m_pDlgGuildMap->OnCommandCancel("I
//			{DCANCEL");
//			else if( m_pDlgWorldMap->IsShow() )
//				m_pDlgWorldMap->OnCommand_CANCEL("IDCANCEL");
//			}
//			else
//			{
				//m_pDlgMiniMap->OnCommand_midmap("midmap");
				//LuaEvent::FirstOpenMap();
				//return true;
//			}
		//}
//		if( ch == 'n' || ch == 'N' )
//		{
//			if( !m_pDlgGuildMap->IsShow() )
//			{
//				m_pDlgGuildMap->SetType(CDlgGuildMap::FACTIONMAP_TYPE_NORMAL);
//				m_pDlgGuildMap->Show(true);
//				m_pDlgGMapStatus1->SetPosEx(m_pDlgGuildMap->GetSize().cx / 20, 0, AUIDialog::alignNone, AUIDialog::alignCenter);
//				m_pDlgGMapStatus1->Show(true);
//				m_pDlgGMapStatus1->SetCanMove(false);
//			}
//			else if( m_pDlgGuildMap->GetType() == CDlgGuildMap::FACTIONMAP_TYPE_NORMAL )
//			{
//				m_pDlgGuildMap->Show(false);
//				m_pDlgGMapStatus1->Show(false);
//			}
//		}
		
	//}

	//if( m_pDlgShop->IsShow() ||
	///	m_pDlgTrade->IsShow() ||
	//	m_pDlgWorldMap->IsShow() )
	//{
	//	return true;
	//}

	//int idServiceNPC = pHost->GetCurServiceNPC();
	//if( 0 != idServiceNPC ) return true;

	if( uMsg == WM_CHAR || uMsg == WM_SYSCHAR )
	{
		PAUIDIALOG pShow = NULL;
		
		/*if ( ch == VK_ESCAPE && m_pDlgActive == m_pDlgSystem)
		{
		}
		else if( uMsg == WM_SYSCHAR && (ch == 'h' || ch == 'H') )
		{
			if (g_pGame->GetGameRun()->GetHostPlayer() &&
				g_pGame->GetGameRun()->GetHostPlayer()->GetCutscene() &&
				g_pGame->GetGameRun()->GetHostPlayer()->GetCutscene()->IsPlayingCutscene())
			{
				if(!g_pGame->GetGameRun()->GetHostPlayer()->GetCutscene()->IsOtherKeyDisabled())
					m_bShowAllPanels = !m_bShowAllPanels;
			}
			else
				m_bShowAllPanels = !m_bShowAllPanels;
		}
		*/
		//else if( m_bShowAllPanels )
		{
			//if( ch == 'c' || ch == 'C' )
			//{
			//	m_pDlgSystem->OnCommandCharacter("wcharacter");
			//	return true;
			//}
			//else if( ch == 'h' || ch == 'H' )
			//{
			//	m_pDlgMiniMap->OnCommand_NewHelp("");
			//	return true;
			//}
			//else if( ch == 'b' || ch == 'B' )
			//{
			//	m_pDlgSystem->OnCommandInventory("winventory");
			//	return true;
			//}
			//else if( ch == 'j' || ch == 'J' )
			//{
			//	m_pDlgQuickBar8->OnCommand_QShop("wshop");
			//	return true;
			//}
			//else if( ch == 'r' || ch == 'R' )
			//{
			//	m_pDlgSystem->OnCommandSkill("wskill");
			//	return true;
			//}
			//else if( ch == 'p' || ch == 'P' )
			//{
			//	m_pDlgSystem2->OnCommandPet("wpet");
			//	return true;
			//}
			//else if( ch == 'q' || ch == 'Q' )
			//{
			//	m_pDlgSystem->OnCommandQuest("wquest");
			//	return true;
			//}
			//else if( ch == 'e' || ch == 'E' )
			//{
			//	m_pDlgSystem2->OnCommandAction("waction");
			//	LuaEvent::FirstEmotionWord();
			//	return true;
			//}
			//else if( ch == 't' || ch == 'T' )
			//{
			//	m_pDlgSystem4->OnCommandTeam("wteam");
			//	LuaEvent::FirstTeamSetting();
			//	return true;
			//}
			//else if( ch == 'f' || ch == 'F' )
			//{
			//	m_pDlgSystem4->OnCommandFriend("wfriend");
			//	return true;
			//}
			//else if( ch == 'g' || ch == 'G' )
			//{
			//	m_pDlgSystem4->OnCommandFaction("wfaction");
			//	return true;
			//}
			/*else*/ //if( ch == 'l' || ch == 'L' )
			//{
			//	m_pDlgSystem3->OnCommandHelp("whelp");
			//	return true;
			//}
			//else if( ch == 'i' || ch == 'I' )
			//{
			//	m_pDlgSystem2->OnCommandRankList("wranklist");			
			//	return true;
			//}
			//else if( ch == '~' && glb_IsConsoleEnable() )
			//{
			//	pShow = GetDialog(l_aDlgNames[DLG_CONSOLE]);
			//	pShow->SetCanMove(false);
			//}
			//else if( ch == 'u' || ch == 'U')
			//{
			//	m_pDlgSystem2->OnCommandBroadcast("wbroadcast");
			//	return true;
			//}
			//else if( ch == 4 )		// Ctrl+D
			//{
			//	m_pDlgChat->OnCommand_clear("clear");
			//	return true;
			//}
			//else if (7 == ch)	// Ctrl+G
			//{
			//	if (g_pGame->GetGameRun()->GetHostPlayer()->IsGM())
			//		pShow = m_pDlgGMConsole;
			//}
			//else if( ch == 'o' || ch == 'O' )
			//{
			//	m_pDlgSystem2->OnCommandProduce("wproduce");
			//	return true;
			//}
			//else if( ch == 'y' || ch == 'Y' )
			//{
			//	m_pDlgSystem4->OnCommandTeacher("wteacher");
			//	return true;
			//}
			//else if( ch == 'k' || ch == 'K' )
			//{
			//	m_pDlgMiniMap->OnCommand_daily("");
			//	return true;
			//}
			//else if( ch == 'n' || ch == 'N' )
			//{
			//	m_pDlgFriendList->OnCommandCircle("");
			//	return true;
			//}
			//else if( ch == 'v' || ch == 'V' )
			//{
			//	m_pDlgTalisman->OnCommandZoarium("");
			//	return true;
			//}
			/*else if(ch == 17)
			{
				CDlgInventoryRide* pDlgInvRide = (CDlgInventoryRide*)GetDialog("Win_Inventory_Vechile");
				if(pDlgInvRide)
				{
					if(pDlgInvRide->GetDlgItem("Img_Vechile01")->IsEnabled())
					{
						pDlgInvRide->SendMsgUseItem(1);
					}
				}
				return true;
			}
			else if(ch == 6)
			{
				CDlgInventoryRide* pDlgInvRide = (CDlgInventoryRide*)GetDialog("Win_Inventory_Vechile");
				if(pDlgInvRide)
				{
					if(pDlgInvRide->GetDlgItem("Img_Vechile02")->IsEnabled())
					{
						pDlgInvRide->SendMsgUseItem(0);
					}
				}
				return true;
			}*/

			if( pShow ) pShow->Show(!pShow->IsShow());
		}
	}
	else if( uMsg == WM_KEYDOWN )
	{
		/*if( ch >= '1' && ch <= '9' && AUI_PRESS(VK_SHIFT) )
		{
			CDlgTeamMate *pDlgMate;

			pDlgMate = m_pDlgTeamMate[ch - '1'];
			if( pDlgMate && pDlgMate->IsShow() )
				pDlgMate->OnEventLButtonUp(0, 0, NULL);
		}
		else if( ch >= '1' && ch <= '9' && AUI_PRESS(VK_CONTROL) )
		{
			m_pDlgCharacter->ChangeFashion(ch-'1');
		}
		*/
		/*else if( ch >= '0' && ch <= '9' )
		{
			CECShortcutSet *pSCS = pHost->GetShortcutSet2(0);
			CECShortcut *pSC;
			if( ch == '0' )
				pSC = pSCS->GetShortcut(9);
			else
				pSC = pSCS->GetShortcut(ch - '1');

			if( pSC ) pSC->Execute(ch);

			{
			   if(pSC)
			   {
			       if(ch == '2')
					   NewHelpGuide::FinishGuide(GUIDE_HELP_USESKILL_1);
				   else if(ch == '4')
					   NewHelpGuide::FinishGuide(GUIDE_HELP_USEMEDICINE_1);
			   }
			}
		}
		else if( ch >= VK_F1 && ch <= VK_F8 )
		{
			CECShortcutSet *pSCS = pHost->GetShortcutSet1(0);
			CECShortcut *pSC = pSCS->GetShortcut(ch - VK_F1);
		
			if( pSC ) pSC->Execute(ch);
		}
		*/
		/*else if( AUI_PRESS(VK_CONTROL) )
		{
			if( ch == 'Z' && AUI_PRESS(VK_MENU) )
			{
				PAUIOBJECT pObj = m_pDlgInfo->GetDlgItem("List");
				PAUIOBJECT pInfo = m_pDlgInfoIcon->GetDlgItem("F_3");

				m_pDlgInfo->BuildInfoList(CDlgInfo::INFO_FRIEND);
				m_pDlgInfo->SetData(CDlgInfo::INFO_FRIEND);
				m_pDlgInfo->OnEventLButtonUp_List(0, 0, pObj);
			}
			else if( ch == VK_F11 )
			{
				PAUICHECKBOX pChk = (PAUICHECKBOX)m_pDlgSettingVideo->GetDlgItem("Chk_Item");
				pChk->Check(!pChk->IsChecked());
				m_pDlgSettingVideo->UpdateData();
				m_pDlgSettingVideo->Apply();
			}
			else if( ch == VK_F12 )
			{
				PAUICHECKBOX pChk = (PAUICHECKBOX)m_pDlgSettingSystem->GetDlgItem("Chk_ModelLimit");
				pChk->Check(!pChk->IsChecked());
				m_pDlgSettingSystem->OnCommandModeLimit("");
				m_pDlgSettingSystem->Apply();
			}
		}
		else if( ch == VK_F9 )
		{
			m_pDlgCamera->Show(!m_pDlgCamera->IsShow());
		}
		*/
		//else if( m_bShowAllPanels )
		{
//			if( ch == VK_TAB )
//			{
//				m_pDlgQuickBar8->OnCommand_add("add");
//				return true;
//			}
		}
	}
	else if( uMsg == WM_KEYUP)
	{
		/*if( ch >= '0' && ch <= '9' )
		{
			CECShortcutSet *pSCS = pHost->GetShortcutSet2(0);
			CECShortcut *pSC;
			if( ch == '0' )
				pSC = pSCS->GetShortcut(9);
			else
				pSC = pSCS->GetShortcut(ch - '1');

			if( pSC ) pSC->OnKeyUp();
		}
		else if( ch >= VK_F1 && ch <= VK_F8 )
		{
			CECShortcutSet *pSCS = pHost->GetShortcutSet1(0);
			CECShortcut *pSC = pSCS->GetShortcut(ch - VK_F1);
		
			if( pSC ) pSC->OnKeyUp();
		}*/
	}
	
	return true;
}
void CECGameUIMan::OnLogicKey_UpdateUI(int iLogicKey, bool bKeyDown)
{

	CECHostPlayer *pHost = g_pGame->GetGameRun()->GetHostPlayer();
	if( pHost->IsChangingFace() ) return;


	if(bKeyDown)
	{

       if(iLogicKey == LKEY_CHATGETFOCUS)
	   {
		   if( m_bShowAllPanels && 
			   (!m_pDlgActive || !m_pDlgActive->GetFocus() || m_pDlgActive->
			   GetFocus()->GetType() != AUIOBJECT_TYPE_EDITBOX) )
		   {
			   PAUIOBJECT pObj = m_pDlgChat->GetDlgItem("DEFAULT_Txt_Speech");

			   BringWindowToTop(m_pDlgChat);
			   LuaEvent::FirstOpenChat();
			   pObj->SetFocus(true);
			   m_pDlgChat->ChangeFocus(pObj);
		   }
	   }

	   else if(m_bShowAllPanels)
	   {
		   if( pHost->GetCurPetId() )
		   {
			   if(iLogicKey >= LKEY_PETATTACK_1 && iLogicKey <= LKEY_PETATTACK_9)
			   {
				   if( pHost->GetCurPetId() )
				   {
					   if( iLogicKey == LKEY_PETATTACK_1 )
					   {
						   m_pDlgPetSkill->OnCommandAttack("");
					   }
					   else
					   {
						   m_pDlgPetSkill->CastSkill(iLogicKey - LKEY_PETATTACK_2, false);
					   }
				   }
				   return;
			   }
			   else if( iLogicKey == LKEY_PETATTACK_STOP)
			   {
				   m_pDlgPetSkill->OnCommandStop("");
				   return;
			   }
			   else if( iLogicKey == LKEY_PETATTACK_FOLLOW )
			   {
				   m_pDlgPetSkill->OnCommandFollow("");
				   return;
			   }
			   else if( iLogicKey == LKEY_PETATTACK_OFFENSIVE)
			   {
				   m_pDlgPetSkill->OnCommandOffensive("");
				   return;
			   }
			   else if( iLogicKey == LKEY_PETATTACK_DEFENSIVE)
			   {
				   m_pDlgPetSkill->OnCommandDefensive("");
				   return;
			   }
			   else if( iLogicKey == LKEY_PETATTACK_COMBAT)
			   {
				   m_pDlgPetSkill->OnCommandCombat("");
				   return;
			   }
		   }

	   }


	   if( m_pDlgShop->IsShow() ||
		   m_pDlgTrade->IsShow() ||
		   m_pDlgWorldMap->IsShow() )
	   {
		   return ;
	   }

	   int idServiceNPC = pHost->GetCurServiceNPC();
	   if( 0 != idServiceNPC ) return ;


	   PAUIDIALOG pShow = NULL;

	   //待修改
	   if ( iLogicKey == LKEY_HP_CANCEL && m_pDlgActive == m_pDlgSystem)
	   {

	   }
	   else if( iLogicKey == LKEY_UI_SHOW_ALLPANELS )
	   {
		   if (g_pGame->GetGameRun()->GetHostPlayer() &&
			   g_pGame->GetGameRun()->GetHostPlayer()->GetCutscene() &&
			   g_pGame->GetGameRun()->GetHostPlayer()->GetCutscene()->IsPlayingCutscene())
		   {
			   if(!g_pGame->GetGameRun()->GetHostPlayer()->GetCutscene()->IsOtherKeyDisabled())
				   m_bShowAllPanels = !m_bShowAllPanels;
		   }
		   else
			   m_bShowAllPanels = !m_bShowAllPanels;
	   }
	   else if(m_bShowAllPanels)
	   {
	    
		   if(iLogicKey == LKEY_UI_SHOWDLG_MAPMID)
		   {
			   m_pDlgMiniMap->OnCommand_midmap("midmap");
			   LuaEvent::FirstOpenMap();
			   return;
		   }
		   if(iLogicKey == LKEY_UI_SHOWDLG_CHARACTER)
		   {
			  m_pDlgSystem->OnCommandCharacter("wcharacter");
			  return;
		   }
		   else if(iLogicKey == LKEY_UI_SHOWDLG_HELP)
		   {
			  m_pDlgMiniMap->OnCommand_NewHelp("");
			  return;
		   }
		   else if(iLogicKey == LKEY_UI_SHOWDLG_INVENTORY)
		   {
			  m_pDlgSystem->OnCommandInventory("winventory");
			  return;
		   }
		   else if(iLogicKey == LKEY_UI_SHOWDLG_CASHSHOP)
		   {
			  m_pDlgQuickBar8->OnCommand_QShop("wshop");
			  return;
		   }
		   else if(iLogicKey == LKEY_UI_SHOWDLG_SKILL)
		   {
			  m_pDlgSystem->OnCommandSkill("wskill");
			  return;
		   }
		   else if(iLogicKey == LKEY_UI_SHOWDLG_PET)
		   {
			  m_pDlgSystem2->OnCommandPet("wpet");
			  return;
		   }
		   else if(iLogicKey == LKEY_UI_SHOWDLG_TASK)
		   {
			   m_pDlgSystem->OnCommandQuest("wquest");
			   return;
		   }
		   else if(iLogicKey == LKEY_UI_SHOWDLG_ACTION)
		   {
			   m_pDlgSystem2->OnCommandAction("waction");
			   LuaEvent::FirstEmotionWord();
			   return;
		   }
		   else if(iLogicKey == LKEY_UI_SHOWDLG_TEAM)
		   {
			   m_pDlgSystem4->OnCommandTeam("wteam");
			   LuaEvent::FirstTeamSetting();
			   return;
		   }
		   else if(iLogicKey == LKEY_UI_SHOWDLG_FRIEND)
		   {
			  m_pDlgSystem4->OnCommandFriend("wfriend");
			  return;
		   }
		   else if(iLogicKey == LKEY_UI_SHOWDLG_FACTION)
		   {
			   m_pDlgSystem4->OnCommandFaction("wfaction");
			   return;
		   }
		   else if(iLogicKey == LKEY_UI_SHOWDLG_SECRETARY)
		   {
			  m_pDlgSystem3->OnCommandHelp("whelp");
			  return;
		   }
		   else if(iLogicKey == LKEY_UI_SHOWDLG_RANKLIST)
		   {
			  m_pDlgSystem2->OnCommandRankList("wranklist");
			  return;
		   }
		   else if(iLogicKey == LKEY_UI_SHOWDLG_CONSOLE  && glb_IsConsoleEnable())
		   {
			   pShow = GetDialog(l_aDlgNames[DLG_CONSOLE]);
			   pShow->SetCanMove(false);
		   }
		   else if(iLogicKey == LKEY_UI_SHOWDLG_BROADCAST)
		   {
			   m_pDlgSystem2->OnCommandBroadcast("wbroadcast");
			   return;
		   }
		   else if(iLogicKey == LKEY_CHAT_CLEAR)
		   {
			   m_pDlgChat->OnCommand_clear("clear");
			   return;
		   }
		   else if(iLogicKey == LKEY_UI_SHOWDLG_GMCONSOLE)
		   {
			   if (g_pGame->GetGameRun()->GetHostPlayer()->IsGM())
				   pShow = m_pDlgGMConsole;
		   }
		   else if(iLogicKey == LKEY_UI_SHOWDLG_PRODUCE)
		   {
			   m_pDlgSystem2->OnCommandProduce("wproduce");
			   return;
		   }
		   else if(iLogicKey == LKEY_UI_SHOWDLG_TEACHER)
		   {
			   m_pDlgSystem4->OnCommandTeacher("wteacher");
			   return;
		   }
		   else if(iLogicKey == LKEY_UI_SHOWDLG_DAILY)
		   {
			   m_pDlgMiniMap->OnCommand_daily("");
			   return;
		   }
		   else if(iLogicKey == LKEY_UI_SHOWDLG_CIRCLE)
		   {
			   m_pDlgFriendList->OnCommandCircle("");
			   return;
		   }
		   else if(iLogicKey == LKEY_UI_SHOWDLG_ZOARIUM)
		   {
			   m_pDlgTalisman->OnCommandZoarium("");
			   return;
		   }
		   else if(iLogicKey == LKEY_RIDE1_USEITEM)
		   {
			   CDlgInventoryRide* pDlgInvRide = (CDlgInventoryRide*)GetDialog("Win_Inventory_Vechile");
			   if(pDlgInvRide)
			   {
				   if(pDlgInvRide->GetDlgItem("Img_Vechile01")->IsEnabled())
				   {
					   pDlgInvRide->SendMsgUseItem(1);
				   }
			   }
			   return;
		   }
		   else if(iLogicKey == LKEY_RIDE2_USEITEM)
		   {
			   CDlgInventoryRide* pDlgInvRide = (CDlgInventoryRide*)GetDialog("Win_Inventory_Vechile");
			   if(pDlgInvRide)
			   {
				   if(pDlgInvRide->GetDlgItem("Img_Vechile02")->IsEnabled())
				   {
					   pDlgInvRide->SendMsgUseItem(0);
				   }
			   }
			   return;
		   }

		   if( pShow ) pShow->Show(!pShow->IsShow());
		  
	   }

		if(iLogicKey>=LKEY_TEAMMATE_1_SHIFT && iLogicKey<=LKEY_TEAMMATE_9_SHIFT)
		{
			CDlgTeamMate *pDlgMate;

			int index = iLogicKey-LKEY_TEAMMATE_1_SHIFT;
			pDlgMate = m_pDlgTeamMate[index];
			if( pDlgMate && pDlgMate->IsShow() )
				pDlgMate->OnEventLButtonUp(0, 0, NULL);
		}
		else if(iLogicKey>=LKEY_CHANGEFASHION_1 && iLogicKey<=LKEY_CHANGEFASHION_9)
		{
			int index = iLogicKey-LKEY_CHANGEFASHION_1;
			m_pDlgCharacter->ChangeFashion(index);
		}
		else if(iLogicKey == LKEY_INFOFRIEND)
		{
			PAUIOBJECT pObj = m_pDlgInfo->GetDlgItem("List");
			PAUIOBJECT pInfo = m_pDlgInfoIcon->GetDlgItem("F_3");

			m_pDlgInfo->BuildInfoList(CDlgInfo::INFO_FRIEND);
			m_pDlgInfo->SetData(CDlgInfo::INFO_FRIEND);
			m_pDlgInfo->OnEventLButtonUp_List(0, 0, pObj);
		}
		else if(iLogicKey == LKEY_SHOWITEMINFO)
		{
			PAUICHECKBOX pChk = (PAUICHECKBOX)m_pDlgSettingVideo->GetDlgItem("Chk_Item");
			pChk->Check(!pChk->IsChecked());
			m_pDlgSettingVideo->UpdateData();
			m_pDlgSettingVideo->Apply();
		}
		else if(iLogicKey == LKEY_SHOWMODELLIMIT)
		{
			PAUICHECKBOX pChk = (PAUICHECKBOX)m_pDlgSettingSystem->GetDlgItem("Chk_ModelLimit");
			pChk->Check(!pChk->IsChecked());
			m_pDlgSettingSystem->OnCommandModeLimit("");
			m_pDlgSettingSystem->Apply();
		}
		else if(iLogicKey == LKEY_GHOSTMODE)
		{
			m_pDlgCamera->Show(!m_pDlgCamera->IsShow());
		}
	
	}
}
void CECGameUIMan::OnLogicKey_UpdateShortCut(int iLogicKey, bool bKeyDown)
{
	CECShortcutSet *pSCS = NULL;
	CECShortcut *pSC  = NULL;
	CECHostPlayer *pHost = g_pGame->GetGameRun()->GetHostPlayer();

	if( iLogicKey >= LKEY_HP_SHORTCUT_1 && iLogicKey <= LKEY_HP_SHORTCUT_12 )
	{
		pSCS = pHost->GetShortcutSet2(0);
		if(pSCS)
			pSC = pSCS->GetShortcut(iLogicKey-LKEY_HP_SHORTCUT_1);
		
	}
	else if( iLogicKey >= LKEY_HP_SHORTCUT_13 && iLogicKey <= LKEY_HP_SHORTCUT_24 )
	{
		pSCS = pHost->GetShortcutSet1(0);
		if(pSCS)
		   pSC = pSCS->GetShortcut(iLogicKey - LKEY_HP_SHORTCUT_13);

	}
	else if( iLogicKey >= LKEY_HP_SHORTCUT_25 && iLogicKey <= LKEY_HP_SHORTCUT_36 )
	{
		pSCS = pHost->GetShortcutSet1(1);
		if(pSCS)
			pSC = pSCS->GetShortcut(iLogicKey - LKEY_HP_SHORTCUT_25);

	}
	else if( iLogicKey >= LKEY_HP_SHORTCUT_37 && iLogicKey <= LKEY_HP_SHORTCUT_48 )
	{
		pSCS = pHost->GetShortcutSet2(1);
		if(pSCS)
			pSC = pSCS->GetShortcut(iLogicKey - LKEY_HP_SHORTCUT_37);

	}
	else if( iLogicKey >= LKEY_HP_SHORTCUT_49 && iLogicKey <= LKEY_HP_SHORTCUT_58 )
	{
		pSCS = pHost->GetShortcutSet3(0);
		if(pSCS)
			pSC = pSCS->GetShortcut(iLogicKey - LKEY_HP_SHORTCUT_49);

	}
	else if( iLogicKey >= LKEY_HP_SHORTCUT_59 && iLogicKey <= LKEY_HP_SHORTCUT_68 )
	{
		pSCS = pHost->GetShortcutSet3(1);
		if(pSCS)
			pSC = pSCS->GetShortcut(iLogicKey - LKEY_HP_SHORTCUT_59);

	}
	else if( iLogicKey >= LKEY_HP_SHORTCUT_69 && iLogicKey <= LKEY_HP_SHORTCUT_80 )
	{
		pSCS = pHost->GetShortcutSet3(2);
		if(pSCS)
			pSC = pSCS->GetShortcut(iLogicKey - LKEY_HP_SHORTCUT_69);

	}
	else if( iLogicKey >= LKEY_HP_SHORTCUT_81 && iLogicKey <= LKEY_HP_SHORTCUT_92 )
	{
		pSCS = pHost->GetShortcutSet3(3);
		if(pSCS)
			pSC = pSCS->GetShortcut(iLogicKey - LKEY_HP_SHORTCUT_81);

	}

	if( pSC )
	{
		if(bKeyDown)
		{
			pSC->Execute(iLogicKey);
            if(LKEY_HP_SHORTCUT_1 == iLogicKey)
				NewHelpGuide::FinishGuide(GUIDE_HELP_FIGHT_2);
			else if(LKEY_HP_SHORTCUT_2 == iLogicKey)
				NewHelpGuide::FinishGuide(GUIDE_HELP_USESKILL_1);
			else if(LKEY_HP_SHORTCUT_3 == iLogicKey)
				NewHelpGuide::FinishGuide(GUIDE_HELP_USEMEDICINE_1);
		}
		else
		{
			pSC->OnKeyUp();
		}

	}
	
		
	
}
bool CECGameUIMan::OnCommand(const char* szCommand, AUIDialog* pDlg)
{
	if (AUILuaManager::OnCommand(szCommand, pDlg))
		return true;

	if( 0 == stricmp(pDlg->GetName(), "Win_Popmsg") ||
		0 == stricmp(pDlg->GetName(), "Win_EnterWait") )
	{
		return false;
	}

	//Customize Dlgs
	if( 0 == stricmp(pDlg->GetName(), "Win_ChooseEar") ||
		0 == stricmp(pDlg->GetName(), "Win_ChooseEye") ||
		0 == stricmp(pDlg->GetName(), "Win_ChooseEyebrow") ||
		0 == stricmp(pDlg->GetName(), "Win_ChooseFace") ||
		0 == stricmp(pDlg->GetName(), "Win_ChooseHair") ||
		0 == stricmp(pDlg->GetName(), "Win_ChooseMouth") ||
		0 == stricmp(pDlg->GetName(), "Win_ChooseNose") ||
		0 == stricmp(pDlg->GetName(), "Win_ChooseFacePainting") ||
		0 == stricmp(pDlg->GetName(), "Win_ChoosePreCustomize") ||
		0 == stricmp(pDlg->GetName(), "Win_ChooseStature") ||
		0 == stricmp(pDlg->GetName(), "Win_AdvancedOption") )
	{
		return false;
	}

	//Auto Home Dlgs
	if( 0 == stricmp(pDlg->GetName(), "Dlg_Building") ||
		0 == stricmp(pDlg->GetName(), "Dlg_Light") ||
		0 == stricmp(pDlg->GetName(), "Dlg_LocalTexture") ||
		0 == stricmp(pDlg->GetName(), "Dlg_Mountain") ||
		0 == stricmp(pDlg->GetName(), "Dlg_Plant") ||
		0 == stricmp(pDlg->GetName(), "Dlg_Road") ||
		0 == stricmp(pDlg->GetName(), "Dlg_Terrain") ||
		0 == stricmp(pDlg->GetName(), "Dlg_Water") )
	{
		return false;
	}



	if( 0 == stricmp(szCommand, "IDCANCEL") )
	{
		pDlg->Show(false);
		return true;
	}

	return false;
}

bool CECGameUIMan::OnMessageBox(int iRetVal, AUIDialog* pDlg)
{
	CECGameSession *pSession = g_pGame->GetGameSession();
	CECHostPlayer *pHost = g_pGame->GetGameRun()->GetHostPlayer();
#ifdef LUA_UI
	LuaBind::Push(LuaBind::TypeWrapper<const char *>(), m_pLuaState->GetState(), pDlg->GetName());
	LuaBind::Push(LuaBind::TypeWrapper<int>(), m_pLuaState->GetState(), iRetVal);
	m_pState->Call(m_pLuaState->GetState(), LUA_GAMEUIMAN, "OnMessageBox", 2);
#endif
	if(0 == stricmp(pDlg->GetName(), "FriendCallBack")){
		if(IDYES == iRetVal)
			((AUICheckBox*)GetDialog("Win_MailList")->GetDlgItem("Chk_RefuseToAccept"))->Check(true);
		else if(IDNO == iRetVal)
			((AUICheckBox*)GetDialog("Win_MailList")->GetDlgItem("Chk_RefuseToAccept"))->Check(false);
	}
	if( 0 == stricmp(pDlg->GetName(), "Game_InviteGroup") )
	{
		int idLeader = pDlg->GetData();
		int idTeamSeq = (DWORD)pDlg->GetDataPtr("dword");
		if( IDYES == iRetVal )
		{
			pSession->c2s_CmdTeamAgreeInvite(idLeader, idTeamSeq);
			LuaEvent::FirstTeam();
		}
		else
			pSession->c2s_CmdTeamRejectInvite(idLeader);
	}
	else if( 0 == stricmp(pDlg->GetName(), "Game_AskToJoin") )
	{
		pSession->c2s_CmdTeamReplyJoinAsk(pDlg->GetData(), iRetVal == IDYES);
	}
	else if( 0 == stricmp(pDlg->GetName(), "Game_InviteTrade") )
	{
		if( !m_pDlgTrade->IsShow())
		{
			DWORD dwHandle = (DWORD)pDlg->GetDataPtr("dword");
			pSession->trade_StartResponse(dwHandle, IDYES == iRetVal ? true : false);
		}
	}
	else if( 0 == stricmp(pDlg->GetName(), "Game_Disenchase1") && IDOK == iRetVal )
	{
		CECIvtrItem *pIvtr = (CECIvtrItem *)m_pDlgUninstall1->m_pItema->GetDataPtr("ptr_CECIvtrItem");

		pSession->c2s_CmdNPCSevClearEmbeddedChip(
			m_pDlgUninstall1->m_pItema->GetData(), pIvtr->GetTemplateID(), 1);

		m_pDlgUninstall1->OnEventLButtonDown(0, 0, m_pDlgUninstall1->GetDlgItem("Item_a"));
//		PAUIDIALOG pMsgBox;
//		MessageBox("", GetStringFromTable(228), MB_OK,
//			A3DCOLORRGBA(255, 255, 255, 160), &pMsgBox);
//		pMsgBox->SetLife(3);
	}
	else if( 0 == stricmp(pDlg->GetName(), "Game_Disenchase2") && IDOK == iRetVal )
	{
		CECIvtrItem *pIvtr = (CECIvtrItem *)m_pDlgUninstall2->m_pItema->GetDataPtr("ptr_CECIvtrItem");

		pSession->c2s_CmdNPCSevClearEmbeddedChip(
			m_pDlgUninstall2->m_pItema->GetData(), pIvtr->GetTemplateID(), 0);

		m_pDlgUninstall2->OnEventLButtonDown(0, 0, m_pDlgUninstall2->GetDlgItem("Item_a"));
//		PAUIDIALOG pMsgBox;
//		MessageBox("", GetStringFromTable(228), MB_OK,
//			A3DCOLORRGBA(255, 255, 255, 160), &pMsgBox);
//		pMsgBox->SetLife(3);
	}
	else if( 0 == stricmp(pDlg->GetName(), "Game_TeachSkill") && IDOK == iRetVal )
	{
		CECSkill *pSkill = (CECSkill *)pDlg->GetDataPtr("ptr_CECSkill");
		int nCondition = pHost->CheckSkillLearnCondition(pSkill->GetSkillID(), true);

		if( 0 == nCondition )
			pSession->c2s_CmdNPCSevLearnSkill(pSkill->GetSkillID());
		else if( 1 == nCondition )
			AddChatMessage(GetStringFromTable(270), GP_CHAT_MISC);
		else if( 6 == nCondition )
			AddChatMessage(GetStringFromTable(527), GP_CHAT_MISC);
		else if( 7 == nCondition )
			AddChatMessage(GetStringFromTable(541), GP_CHAT_MISC);
		else if( 8 == nCondition )
			AddChatMessage(GetStringFromTable(271), GP_CHAT_MISC);
		else if( 9 == nCondition )
			AddChatMessage(GetStringFromTable(556), GP_CHAT_MISC);
		else if( 10 == nCondition )
			AddChatMessage(GetStringFromTable(557), GP_CHAT_MISC);
	}
	else if( 0 == stricmp(pDlg->GetName(), "Game_TeamDisband") && IDOK == iRetVal )
	{
		pSession->c2s_CmdTeamLeaveParty();
		m_pDlgArrangeTeam->Show(false);
	}
	else if( 0 == stricmp(pDlg->GetName(), "Game_AbortTask") && IDOK == iRetVal )
	{
		CECTaskInterface *pTask = pHost->GetTaskInterface();
		pTask->GiveUpTask(pDlg->GetData());
	}
	else if( 0 == stricmp(pDlg->GetName(), "Game_RepairAll") && IDOK == iRetVal )
	{
		pSession->c2s_CmdNPCSevRepairAll();
	}
	else if( 0 == stricmp(pDlg->GetName(), "Game_WithdrawMsg") && IDYES == iRetVal )
	{
		pSession->sns_cancel_message(m_pDlgFriendSeek->GetMsgID(),m_pDlgFriendSeek->GetSeekingType());
	}
	else if( 0 == stricmp(pDlg->GetName(), "Game_SendBackMsg") && (IDYES == iRetVal||IDOK == iRetVal) )
	{
		CDlgFriendSeek *pFriendSeekMsg = (CDlgFriendSeek*)GetDialog("Win_FriendSeekingMsg");
		pSession->c2s_CmdResponseSNSMessage(pFriendSeekMsg->GetSeekingType(),pFriendSeekMsg->GetMsgID(),pFriendSeekMsg->GetMsgIDDst(),pFriendSeekMsg->GetMsgSendBack());
	}
	else if( 0 == stricmp(pDlg->GetName(), "Game_PressMsg") && (IDYES == iRetVal||IDOK == iRetVal) )
	{
		CDlgFriendSeek *pFriendSeekMsg = (CDlgFriendSeek*)GetDialog("Win_FriendInfoPub");
		pFriendSeekMsg->PressInfo();
	}
	else if( 0 == stricmp(pDlg->GetName(), "Game_DeleteMsg") && (IDYES == iRetVal||IDOK == iRetVal) )
	{
		CDlgFriendSeek *pFriendSeekMsg = (CDlgFriendSeek*)GetDialog("Win_FriendSeekingMsg");
		pFriendSeekMsg->DeleteLeaveMsg();
	}
	else if( 0 == stricmp(pDlg->GetName(), "Game_VoteSupport") && (IDYES == iRetVal||IDOK == iRetVal) )
	{
		pSession->c2s_CmdVoteSNSMessage(m_pDlgFriendSeek->GetSeekingType(), m_pDlgFriendSeek->GetMsgID(), 1);
	}
	else if( 0 == stricmp(pDlg->GetName(), "Game_VoteOppose") && (IDYES == iRetVal||IDOK == iRetVal) )
	{
		pSession->c2s_CmdVoteSNSMessage(m_pDlgFriendSeek->GetSeekingType(), m_pDlgFriendSeek->GetMsgID(), 0);
	}
	else if( 0 == stricmp(pDlg->GetName(), "Game_MsgAccept") && (IDYES == iRetVal||IDOK == iRetVal) )
	{
		pSession->sns_accept_apply(m_pDlgFriendSeek->GetApplyRoleID(), m_pDlgFriendSeek->GetSeekingType(), m_pDlgFriendSeek->GetMsgID(), 0);
	}
	else if( 0 == stricmp(pDlg->GetName(), "Game_MsgRefuse") && (IDYES == iRetVal||IDOK == iRetVal) )
	{
		pSession->sns_accept_apply(m_pDlgFriendSeek->GetApplyRoleID(), m_pDlgFriendSeek->GetSeekingType(), m_pDlgFriendSeek->GetMsgID(), 1);
	}
	else if( 0 == stricmp(pDlg->GetName(), "Game_UploadPhoto") && (IDYES == iRetVal||IDOK == iRetVal) )
	{
		MessageBox("",GetStringFromTable(7301), MB_OK, A3DCOLORRGBA(255, 255, 255, 160));
	}
	else if( 0 == stricmp(pDlg->GetName(), "Game_WorthyItem") )
	{
		CDlgInventory *pInventory = (CDlgInventory*)GetDialog("Win_Inventory");
		pInventory->SellConfirm(IDYES == iRetVal||IDOK == iRetVal);
	}
	else if( 0 == stricmp(pDlg->GetName(), "Game_WorthyItemDrop") && (IDYES == iRetVal||IDOK == iRetVal) )
	{
		void *ptr;
		AString szType;
		pDlg->ForceGetDataPtr(ptr,szType);
		if (szType == "input")
		{
			InvokeNumberDialog((PAUIOBJECT)ptr, NULL,
								CDlgInputNO::INPUTNO_DROP_ITEM, pDlg->GetData());
		}
		else
			pSession->c2s_CmdDropIvtrItem((int)ptr, pDlg->GetData());
	}
	else if( 0 == stricmp(pDlg->GetName(), "Game_WorthyItemDrop_Equip") && (IDYES == iRetVal||IDOK == iRetVal) )
	{
		pSession->c2s_CmdDropEquipItem(pDlg->GetData());
	}
	else if( 0 == stricmp(pDlg->GetName(), "Game_SellOut") && IDOK == iRetVal )
	{
		pHost->SellItemsToNPC();
		pHost->GetDealPack()->RemoveAllItems();
		UnfreezeAllItems();
	}
	else if( 0 == stricmp(pDlg->GetName(), "Game_PSellOut") && IDOK == iRetVal )
	{
		pHost->SellItemsToBooth();
		pHost->GetDealPack()->RemoveAllItems();
		UnfreezeAllItems();
	}
	else if( 0 == stricmp(pDlg->GetName(), "Game_DelMark") && IDOK == iRetVal )
	{
		PAUILISTBOX pList = (PAUILISTBOX)m_pDlgMiniMapMark->GetDlgItem("List_Choose");
		PAUILISTBOX pList1 = (PAUILISTBOX)m_pDlgMidMap->GetDlgItem("List_Choose");

		int nCurSel = pDlg->GetData();
		m_pDlgMiniMap->m_vecMark.erase(m_pDlgMiniMap->m_vecMark.begin() + nCurSel);
		pList->DeleteString(nCurSel);
		pList1->DeleteString(nCurSel);
	}
	else if( 0 == stricmp(pDlg->GetName(), "Game_DelAllMarks") && IDOK == iRetVal )
	{
		PAUILISTBOX pList = (PAUILISTBOX)m_pDlgMiniMapMark->GetDlgItem("List_Choose");
		PAUILISTBOX pList1 = (PAUILISTBOX)m_pDlgMidMap->GetDlgItem("List_Choose");

		pList->ResetContent();
		pList1->ResetContent();
		m_pDlgMiniMap->m_vecMark.clear();
	}
	else if( 0 == stricmp(pDlg->GetName(), "MsgBox_LinkBroken") && IDYES == iRetVal  )
	{
		g_pGame->GetGameRun()->SetLogoutFlag(2);
	}
	else if( 0 == stricmp(pDlg->GetName(), "Game_DelFriend") && IDOK == iRetVal )
	{
		PAUITREEVIEW pTree = (PAUITREEVIEW)m_pDlgFriendList->GetDlgItem("Tv_FriendList");
		P_AUITREEVIEW_ITEM pItem = pTree->GetSelectedItem();
		if( !pItem ) return true;

		int idFriend = (int)pTree->GetItemData(pItem);
		pSession->friend_Delete(idFriend);
	}
	else if( 0 == stricmp(pDlg->GetName(), "Game_DelGroup") && IDOK == iRetVal )
	{
		PAUITREEVIEW pTree = (PAUITREEVIEW)m_pDlgFriendList->GetDlgItem("Tv_FriendList");
		P_AUITREEVIEW_ITEM pItem = pTree->GetSelectedItem();
		if( !pItem ) return true;

		int idGroup = (int)pTree->GetItemData(pItem);
		pSession->friend_SetGroupName(idGroup, _AL(""));
	}
	else if( 0 == stricmp(pDlg->GetName(), "Game_PVPOpen") && IDOK == iRetVal )
	{
		pSession->c2s_CmdEnalbePVP();
	}
	else if( 0 == stricmp(pDlg->GetName(), "Game_PVPClose") && IDOK == iRetVal )
	{
		pSession->c2s_CmdDisablePVP();
	}
	else if( 0 == stricmp(pDlg->GetName(), "Game_ForgetSkill") && IDOK == iRetVal )
	{
		CECSkill *pSkill = (CECSkill *)pDlg->GetDataPtr("ptr_CECSkill");

		pSession->c2s_CmdNPCSevForgetSkill(pSkill->GetSkillID());
		m_pDlgNPC->Show(false);
		EndNPCService();
	}
	else if( 0 == stricmp(pDlg->GetName(), "Game_DelHistory") && IDOK == iRetVal )
	{
		char szPath[MAX_PATH];
		PAUIDIALOG pMsgBox = NULL;

		sprintf(szPath, "%s\\Userdata\\Msg\\%d", af_GetBaseDir(), pHost->GetCharacterID());
		if( RemoveFilesAndDirectory(szPath) )
		{
			MessageBox("", GetStringFromTable(567), MB_OK,
				A3DCOLORRGBA(255, 255, 255, 160), &pMsgBox);
		}
		else
		{
			MessageBox("", GetStringFromTable(568), MB_OK,
				A3DCOLORRGBA(255, 255, 255, 160), &pMsgBox);
		}
		if( pMsgBox ) pMsgBox->SetLife(3);
	}
	else if( 0 == stricmp(pDlg->GetName(), "Faction_Quit") && IDOK == iRetVal )
	{
		pSession->faction_leave(0);
	}
	else if( 0 == stricmp(pDlg->GetName(), "Family_Quit") && IDOK == iRetVal )
	{
		pSession->faction_leave(1);
	}
	else if( 0 == stricmp(pDlg->GetName(), "Faction_Pass") && IDOK == iRetVal )
	{
		pSession->faction_master_resign(pDlg->GetData(), 0);
	}
	else if( 0 == stricmp(pDlg->GetName(), "Family_Pass") && IDOK == iRetVal )
	{
		pSession->faction_master_resign(pDlg->GetData(), 1);
	}
	else if( 0 == stricmp(pDlg->GetName(), "Faction_Demiss") && IDOK == iRetVal )
	{
		pSession->faction_resign(0);
	}
	else if( 0 == stricmp(pDlg->GetName(), "Family_Demiss") && IDOK == iRetVal )
	{
		pSession->faction_resign(1);
	}
	else if( 0 == stricmp(pDlg->GetName(), "Faction_Kick") && IDOK == iRetVal )
	{
		pSession->faction_expel_member(pDlg->GetData(), 0);
	}
	else if( 0 == stricmp(pDlg->GetName(), "Family_Kick") && IDOK == iRetVal )
	{
		pSession->faction_expel_member(pDlg->GetData(), 1);
	}
	else if( 0 == stricmp(pDlg->GetName(), "Faction_Upgrade") && IDOK == iRetVal )
	{
		pSession->faction_upgrade(0);
	}
	else if( 0 == stricmp(pDlg->GetName(), "Faction_Dismiss") && IDOK == iRetVal )
	{
		pSession->faction_dismiss(0);
	}
	else if( 0 == stricmp(pDlg->GetName(), "Family_Dismiss") && IDOK == iRetVal )
	{
		pSession->faction_dismiss(1);
	}
	else if( 0 == stricmp(pDlg->GetName(), "Faction_Recover") && IDOK == iRetVal )
	{
		pSession->faction_dismiss(0, false);
	}
	else if( 0 == stricmp(pDlg->GetName(), "Family_Recover") && IDOK == iRetVal )
	{
		pSession->faction_dismiss(1, false);
	}
	else if( 0 == stricmp(pDlg->GetName(), "Game_InviteFaction") && pHost->GetFactionID() <= 0 )
	{
		DWORD dwHandle = (DWORD)pDlg->GetDataPtr("dword");
		if( iRetVal == IDYES )
			m_pDlgGuildMan->SetLastRefreshTime(0);
		pSession->faction_invite_response(dwHandle, IDYES == iRetVal ? true : false, 0);
		if( iRetVal == IDYES )
			LuaEvent::FirstJoinFaction();
	}
	else if( 0== stricmp(pDlg->GetName(),"Game_CrssvrTeamsInvite"))
	{
		DWORD dwHandle = (DWORD)pDlg->GetDataPtr("dword");
		pSession->c2s_CmdSendCrssvrTeamsInvite(dwHandle, IDYES == iRetVal ? true : false);
	}
	else if( 0 == stricmp(pDlg->GetName(), "Game_InviteFamily") && pHost->GetFamilyID() <= 0 )
	{
		DWORD dwHandle = (DWORD)pDlg->GetDataPtr("dword");
		if( iRetVal == IDYES )
			m_pDlgGuildMan->SetLastRefreshTime(0);
		pSession->faction_invite_response(dwHandle, IDYES == iRetVal ? true : false, 1);
		if( iRetVal == IDYES )
			LuaEvent::FirstJoinFamily();
	}
	else if( 0 == stricmp(pDlg->GetName(), "Game_InviteSect") )
	{
		DWORD dwHandle = (DWORD)pDlg->GetDataPtr("dword");
		pSession->sect_invite_response(dwHandle, IDYES == iRetVal ? true : false);
	}
	else if( 0 == stricmp(pDlg->GetName(), "Game_ChannelInvite") )
	{
		if( iRetVal == IDYES )
			pSession->chatroom_Join(pDlg->GetData(), pDlg->GetText() );
		else
			pSession->chatroom_RejectInvite(pDlg->GetData(), (DWORD)pDlg->GetDataPtr("dword") );
	}
	else if( 0 == stricmp(pDlg->GetName(), "Game_ChannelJoinAsCreator") && iRetVal == IDYES )
	{
		pSession->chatroom_Join(pDlg->GetText(), _AL("") );
	}
	else if( 0 == stricmp(pDlg->GetName(), "Game_ChannelJoinAsNumber") && iRetVal == IDYES )
	{
		pSession->chatroom_Join(pDlg->GetData(), _AL("") );
	}
	else if( 0 == stricmp(pDlg->GetName(), "Game_ChannelCreate") && iRetVal == IDYES )
	{
		ACString strText = pDlg->GetText();
		int n = strText.Find(_AL("\r"));
		pSession->chatroom_Create(strText.Left(n), strText.Mid(n + 1), 30);
	}
	else if( 0 == stricmp(pDlg->GetName(), "Game_ChannelClose") && iRetVal == IDYES )
	{
		pSession->chatroom_Leave(pDlg->GetData());
	}
	else if( 0 == stricmp(pDlg->GetName(), "Game_MailDelete") && IDOK == iRetVal )
	{
		m_pDlgMailList->SetWaitingResponse(pDlg->GetData());
		pSession->mail_Delete(pDlg->GetData());
	}
	else if( 0 == stricmp(pDlg->GetName(), "Game_AuctionBid") && iRetVal == IDYES )
	{
		pSession->auction_Bid(pDlg->GetData(), (int)pDlg->GetDataPtr(), false);
	}
	else if( 0 == stricmp(pDlg->GetName(), "Game_AuctionBin") && iRetVal == IDYES )
	{
		pSession->auction_Bid(pDlg->GetData(), (int)pDlg->GetDataPtr(), true);
	}
	else if( 0 == stricmp(pDlg->GetName(), "Game_AuctionClose") && iRetVal == IDYES )
	{
		pSession->auction_Close(pDlg->GetData(), 0);
	}
	else if( 0 == stricmp(pDlg->GetName(), "Game_AuctionGiveUp") && iRetVal == IDYES )
	{
		pSession->auction_ExitBid(pDlg->GetData());
	}
	else if( 0 == stricmp(pDlg->GetName(), "Game_DropMoney") && iRetVal == IDOK )
	{
		pSession->c2s_CmdThrowMoney(pDlg->GetData());
	}
	else if( 0 == stricmp(pDlg->GetName(), "Game_QilinInviteBind") )
	{
		pSession->c2s_CmdQilinInviteReply(pDlg->GetData(), iRetVal == IDNO);
	}
	else if( 0 == stricmp(pDlg->GetName(), "Game_InviteBind") )
	{
		pSession->c2s_CmdBindPlayerInviteRe(pDlg->GetData(), iRetVal == IDNO);
	}
	else if( 0 == stricmp(pDlg->GetName(), "Game_RequestBind") )
	{
		pSession->c2s_CmdBindPlayerRequestRe(pDlg->GetData(), iRetVal == IDNO);
	}
	else if( 0 == stricmp(pDlg->GetName(), "Game_InviteRideTogether") )
	{
		pSession->c2s_CmdMultiBindPlayerInviteRe(pDlg->GetData(), iRetVal == IDNO);
	}
	else if( 0 == stricmp(pDlg->GetName(), "Game_InviteDuel") )
	{
		pSession->c2s_CmdDuelReply(pDlg->GetData(), iRetVal == IDNO ? 2 : 0);
	}
	else if( 0 == stricmp(pDlg->GetName(), "Game_PetBanish") && iRetVal == IDOK)
	{
//		pSession->c2s_CmdPetBanish(pDlg->GetData());
	}
	else if( 0 == stricmp(pDlg->GetName(), "Game_PetFree") && iRetVal == IDYES)
	{
		CDlgPetOperate *pDlg = (CDlgPetOperate*)GetDialog("Win_Pet_Release");
		pDlg->PetFree();
	}
	else if( 0 == stricmp(pDlg->GetName(), "Game_BattleLeave") )
	{
		if( iRetVal == IDOK )
			pSession->c2s_CmdNPCSevLeaveBattle();
		EndNPCService();
	}
	else if( 0 == stricmp(pDlg->GetName(), "Game_ChatWorld") && (iRetVal == IDYES || iRetVal == IDOK) )
	{
		pSession->SendChatData(GP_CHAT_FARCRY, pDlg->GetText(), pDlg->GetData(), (int)pDlg->GetDataPtr(), -1, (int) pDlg->GetData64());
		m_pDlgChat->m_dwTickFarCry = GetTickCount();
	}
	else if( 0 == stricmp(pDlg->GetName(), "Game_SellPoint") && iRetVal == IDOK )
	{
		pSession->account_SellPoint(pDlg->GetData(), (int)pDlg->GetDataPtr());
	}
	else if( 0 == stricmp(pDlg->GetName(), "Game_CancelSellPoint") && iRetVal == IDOK )
	{
		pSession->account_CancelSellPoint(pDlg->GetData());
	}
	else if( 0 == stricmp(pDlg->GetName(), "Game_BuyPoint") && iRetVal == IDOK )
	{
		pSession->account_BuyPoint(pDlg->GetData(), (int)pDlg->GetDataPtr());
	}
	else if( 0 == stricmp(pDlg->GetName(), "Game_EquipBind1") && iRetVal == IDYES )
	{
		pSession->c2s_CmdEquipItem((int)pDlg->GetData(), (int)pDlg->GetDataPtr());
		CECIvtrItem *pItem = pHost->GetPack()->GetItem((int)pDlg->GetData());
		if( pItem )
			PlayItemSound(pItem, true);
	}
	else if( 0 == stricmp(pDlg->GetName(), "Game_EquipBind_Wing") && iRetVal == IDYES )
	{
		pHost->UseItemInPack(IVTRTYPE_MOUNTWING, (int)pDlg->GetData());
		CECIvtrItem *pItem = pHost->GetEquipment()->GetItem((int)pDlg->GetData());
		if( pItem )
			PlayItemSound(pItem, true);
	}
	else if(0 == stricmp(pDlg->GetName(), "EnabledMount"))
	{
		CDlgInventoryRide *pDlgManTalk = (CDlgInventoryRide*)GetDialog("Win_Inventory_Vechile");
		if(pDlgManTalk->GetDlgItem("Img_Vechile01"))
		{
			pDlgManTalk->GetDlgItem("Img_Vechile01")->Enable(true);
		}
	}
	else if(0 == stricmp(pDlg->GetName(), "EnabledFly"))
	{
		CDlgInventoryRide *pDlgManTalk = (CDlgInventoryRide*)GetDialog("Win_Inventory_Vechile");
		if(pDlgManTalk->GetDlgItem("Img_Vechile02"))
		{
			pDlgManTalk->GetDlgItem("Img_Vechile02")->Enable(true);
		}
	}
	else if( 0 == stricmp(pDlg->GetName(), "Game_EquipBind_Mount") && iRetVal == IDYES )
	{
		CECIvtrItem *pItem = pHost->GetPack(IVTRTYPE_MOUNTWING)->GetItem((int)pDlg->GetData());
		pSession->c2s_CmdUseItem(IVTRTYPE_MOUNTWING, (BYTE)pDlg->GetData(), pItem->GetTemplateID(), 1);
		pHost->CheckRecordMount(IVTRTYPE_MOUNTWING,(BYTE)pDlg->GetData(), pItem->GetTemplateID()); // 2012-12-25 add by zy
		if( pItem )
			PlayItemSound(pItem, true);
	}
	else if(0 == stricmp(pDlg->GetName(), "Game_Bind_Mount") && iRetVal == IDYES ) // 2013-1-4 add by zy 添加判断
	{
		CECIvtrItem *pItem = pHost->GetPack(int(pDlg->GetDataPtr("Type")))->GetItem((int)pDlg->GetData());
		pSession->c2s_CmdUseItem(int(pDlg->GetDataPtr("Type")), (BYTE)pDlg->GetData(), pItem->GetTemplateID(), 1);
		pHost->CheckRecordMount(int(pDlg->GetDataPtr("Type")),(BYTE)pDlg->GetData(), pItem->GetTemplateID()); // 2012-12-25 add by zy
		if( pItem )
			PlayItemSound(pItem, true);
	}
	else if( 0 == stricmp(pDlg->GetName(), "Game_EquipBind2") && iRetVal == IDYES )
	{
		pHost->UseItemInPack(IVTRTYPE_PACK, (int)pDlg->GetData());
		CECIvtrItem *pItem = pHost->GetPack()->GetItem((int)pDlg->GetData());
		if( pItem )
			PlayItemSound(pItem, true);
		{
		   if(pItem)
		   {
			   if(NewHelpGuide::CheckInventoryItemId(pItem->GetTemplateID(),ESTEP_ITEM_EQUIPTALISMAN_4_2))
			   {
				   if(NewHelpGuide::StartGuide_Step_3(GUIDE_HELP_EQUIPTALISMAN_4))
				   {
					   //此处将“显示法宝界面”的开关强制打开
					   EC_GAME_SETTING setting = g_pGame->GetConfigs()->GetGameSettings();
					   setting.bShowTalismanDlg = true;
					   g_pGame->GetConfigs()->SetGameSettings(setting);
				   }
			   }
		   }
		}
	}
	else if( 0 == stricmp(pDlg->GetName(), "Game_EquipBind4") && iRetVal == IDYES )
	{
		pHost->UseItemInPack(IVTRTYPE_FASHION, (int)pDlg->GetData());
		CECIvtrItem *pItem = pHost->GetFashionPack()->GetItem((int)pDlg->GetData());
		if( pItem )
			PlayItemSound(pItem, true);
	}
	else if( 0 == stricmp(pDlg->GetName(), "Game_EquipBind3") && iRetVal == IDYES )
	{
		m_pDlgCharacter->UseFashionItems((int)pDlg->GetData());
	}
	else if( 0 == stricmp(pDlg->GetName(), "Game_ItemBind") && iRetVal == IDYES )
	{
		pHost->UseItemInPack(IVTRTYPE_PACK, (int)pDlg->GetData());
	}
	else if( 0 == stricmp(pDlg->GetName(), "Game_StopRecord") && iRetVal == IDYES )
	{
		m_pDlgRecordEnd->EndRecord();
	}
	else if( 0 == stricmp(pDlg->GetName(), "Game_CombatInvite") && iRetVal == IDOK )
	{
		pSession->combat_Challenge(pHost->GetCharacterID(), pDlg->GetData());
	}
	else if( 0 == stricmp(pDlg->GetName(), "Game_CombatInviteRe") )
	{
		DWORD dwHandle = pDlg->GetData();
		pSession->combat_invite_response(dwHandle, iRetVal == IDYES);
	}
	else if( 0 == stricmp(pDlg->GetName(), "Game_ReplayEnd") )
	{
		if( iRetVal == IDYES )
			m_pDlgReplay->OnCommandStart("");
		else
			g_pGame->GetGameRun()->SetLogoutFlag(2);
	}
	else if( 0 == stricmp(pDlg->GetName(), "Game_GoldBuy") && iRetVal == IDYES )
	{
		pSession->stock_commission(1, pDlg->GetData(), int(pDlg->GetDataPtr()));
		m_pDlgGoldTrade->GetDlgItem("Txt_Num")->SetText(_AL(""));
		m_pDlgGoldTrade->GetDlgItem("Txt_Num")->SetData(0);
	}
	else if( 0 == stricmp(pDlg->GetName(), "Game_GoldSell") && iRetVal == IDYES )
	{
		pSession->stock_commission(0, pDlg->GetData(), int(pDlg->GetDataPtr()));
		m_pDlgGoldTrade->GetDlgItem("Txt_Num")->SetText(_AL(""));
		m_pDlgGoldTrade->GetDlgItem("Txt_Num")->SetData(0);
	}
	else if( 0 == stricmp(pDlg->GetName(), "Game_GoldGiveUp") && iRetVal == IDYES )
	{
		pSession->stock_cancel(pDlg->GetData(), (int)pDlg->GetDataPtr());
	}
	else if( 0 == stricmp(pDlg->GetName(), "Game_BattleEnter") )
	{
		if( iRetVal == IDYES || iRetVal == IDOK )
		{
			pSession->battle_enter(g_pGame->GetGameRun()->GetCurLine(), pDlg->GetData());
			GetDialog("Win_BattleSongTime")->Show(false);
		}
		
		else
			pSession->battle_leave();
	}
	else if( 0 == stricmp(pDlg->GetName(), "Game_BattleInstancingEnter") )
	{
		EndNPCService();
		if( iRetVal == IDYES || iRetVal == IDOK )
		{
			pSession->instancing_enter(g_pGame->GetGameRun()->GetCurLine(), pDlg->GetData());
			GetDialog("Win_BattleRaidApliList")->Show(false);
		}
		
		else
			pSession->battle_leave();
	}
	else if( 0 == stricmp(pDlg->GetName(), "Game_BattleJoin1") && iRetVal == IDYES )
	{
		pSession->battle_join(g_pGame->GetGameRun()->GetCurLine(), pDlg->GetData(), 1);
	}
	else if( 0 == stricmp(pDlg->GetName(), "Game_BattleJoin2") && iRetVal == IDYES )
	{
		pSession->battle_join(g_pGame->GetGameRun()->GetCurLine(), pDlg->GetData(), 2);
	}
	else if( 0 == stricmp(pDlg->GetName(), "Game_MatterDeleteAll") && iRetVal == IDYES )
	{
		PAUIDIALOG pDlg1 = GetDialog("Win_TalismanSettingAll");
		PAUILISTBOX pListBox = (PAUILISTBOX)pDlg1->GetDlgItem("Lst_Info");
		pListBox->ResetContent();
		if( ((PAUIRADIOBUTTON)pDlg1->GetDlgItem("Rdo_Exclude"))->IsChecked() )
		{
			CMatterSelector::ExcludeList *pList = g_pGame->GetGameRun()->GetHelper()->GetMatterSelector()->GetExcludeList();
			pList->clear();
		}
		else if( ((PAUIRADIOBUTTON)pDlg1->GetDlgItem("Rdo_Include"))->IsChecked() )
		{
			CMatterSelector::IncludeList *pList = g_pGame->GetGameRun()->GetHelper()->GetMatterSelector()->GetIncludeList();
			pList->clear();
		}
	}
	else if( 0 == stricmp(pDlg->GetName(), "Game_MonsterDeleteAll") && iRetVal == IDYES )
	{
		PAUIDIALOG pDlg1 = GetDialog("Win_TalismanSettingAll");
		PAUILISTBOX pListBox = (PAUILISTBOX)pDlg1->GetDlgItem("Lst_ShieldList");
		pListBox->ResetContent();
		CMonsterSelector::ExcludeList *pList = g_pGame->GetGameRun()->GetHelper()->GetMonsterSelector()->GetExcludeList();
		pList->clear();
	}
	else if( 0 == stricmp(pDlg->GetName(), "Game_SetAutoLock") && iRetVal == IDOK )
	{
		pSession->autolock_set(pDlg->GetData());
		LuaEvent::FirstSafeLock();
	}
	else if( 0 == stricmp(pDlg->GetName(), "Game_SectExpel") && iRetVal == IDYES )
	{
		pSession->sect_expel(pDlg->GetData());
	}
	if (0 == stricmp(pDlg->GetName(), "Game_ContestInvite") && iRetVal == IDYES)
	{
		pSession->contest_invite_re(1);
	}
	else if (0 == stricmp(pDlg->GetName(), "Game_ContestExit") && iRetVal == IDOK)
	{
		pSession->contest_exit();
	}
	else if (0 == stricmp(pDlg->GetName(), "Game_ShieldPlayer") && iRetVal == IDOK)
	{
		int idPlayer = pDlg->GetData();
		const ACHAR *pszName = g_pGame->GetGameRun()->GetPlayerName(idPlayer, true);
		m_pDlgFriendList->AddBlackList(pszName);
	}
	else if (0 == stricmp(pDlg->GetName(), "Game_BattleSong") && iRetVal == IDOK)
	{
		CDlgBattleSong *pDlgSong = (CDlgBattleSong*)GetDialog("Win_BattleSong");
		pSession->battle_join(g_pGame->GetGameRun()->GetCurLine(), pDlgSong->GetMapID(), 0);
	}
	else if (0 == stricmp(pDlg->GetName(), "Game_GardenPlow") && iRetVal == IDYES)
	{
		CDlgGardenMan *pDlgGarden = (CDlgGardenMan*)GetDialog("Win_Garden");
		htc2s_SendCmdPlowField(pDlgGarden->GetOwnerId(), pDlg->GetData(), pDlgGarden->GetActionType());
	}
	else if (0 == stricmp(pDlg->GetName(), "game_GardenExchange") && iRetVal == IDYES)
	{
		htc2s_SendCmdBuyItem( pDlg->GetData(), 1);
	}
	else if (0 == stricmp(pDlg->GetName(), "Game_QuickTalkReset") && iRetVal == IDYES)
	{
		CDlgChat *pDlgManTalk = (CDlgChat*)GetDialog("Win_PopfaceManage");
		pDlgManTalk->TalkReset();
	}
	else if (0 == stricmp(pDlg->GetName(), "Game_GardenUseItemWholeFarm") && iRetVal == IDYES)
	{
		CDlgGardenInventory *pDlgGardenInventory = (CDlgGardenInventory*)GetDialog("Win_Garden_Inventory");
		pDlgGardenInventory->UseItem();
	}
	else if (0 == stricmp(pDlg->GetName(), "Game_GardenUseItemField") && iRetVal == IDYES)
	{
		CDlgGardenInventory *pDlgGardenInventory = (CDlgGardenInventory*)GetDialog("Win_Garden_Inventory");
		pDlgGardenInventory->UseItem();
	}
	else if (0 == stricmp(pDlg->GetName(), "Game_QuitDdz") && iRetVal == IDYES)
	{
		CDlgGamePoker *pDlgDdz = (CDlgGamePoker*)GetDialog("Win_GamePoker_Match");
		pDlgDdz->QuitGame();
	}
	else if (0 == stricmp(pDlg->GetName(), "Game_TaskCheck"))
	{
		if ( iRetVal == IDYES || iRetVal == IDOK )
		{
			//接任务
			ATaskTemplMan *pTempMan = g_pGame->GetTaskTemplateMan();
			ATaskTempl *pTemp = pTempMan->GetTaskTemplByID(pDlg->GetData());
			if (pTemp)
			{
				_notify_svr(pHost->GetTaskInterface(), TASK_CLT_NOTIFY_AUTO_DELV, (unsigned short)pDlg->GetData());
				//m_pDlgNPC->PopupCorrespondingServiceDialog(NPC_GIVE_TASK, pDlg->GetData(), NULL);
			}
		}
		else
		{
			CECTaskInterface *pTask = pHost->GetTaskInterface();
			pTask->GiveUpAutoDeliverTask(pDlg->GetData());
		}
	}
	else if( 0 == stricmp(pDlg->GetName(), "Game_Help_WhiteDeal") && iRetVal == IDYES )
	{
		pSession->bot_begin(1, 0);//g_pGame->GetGameRun()->GetHelper()->IsAnswerQuestion());
	}
	else if( 0 == stricmp(pDlg->GetName(), "Game_BattleTerrEnter") && iRetVal == IDYES )
	{
		DWORD info = (DWORD)pDlg->GetDataPtr();
		pSession->c2s_CmdNPCSevTerritoryEnter(pDlg->GetData(), info&0x7fffffff, (info&0x80000000)>0?1:0);
	}
	else if( 0 == stricmp(pDlg->GetName(), "Game_BattleTerrLeave") && iRetVal == IDYES )
	{
		pSession->c2s_CmdTerritoryLeave();
	}
	else if( 0 == stricmp(pDlg->GetName(), "Game_AuctionBuy") && iRetVal == IDYES )
	{
		pSession->c2s_CmdUniqueBid(pDlg->GetData(), pDlg->GetData());
		GetDialog("Win_InputMoney_Auction")->Show(false);
	}
	else if( 0 == stricmp(pDlg->GetName(), "Game_Mark_Transport") && iRetVal == IDYES )
	{
		CommonMessage("Win_MarkAstroList", "transport", 0, 0);
	}
	else if( 0 == stricmp(pDlg->GetName(), "Game_Mark_Change_String") && iRetVal == IDYES )
	{
		CommonMessage("Win_MarkAstroList", "change_string", 0, 0);
	}
	else if( 0 == stricmp(pDlg->GetName(), "Game_Mark_Add") && iRetVal == IDYES )
	{
		CommonMessage("Win_MarkAstroList", "add", 0, 0);
	}
	else if( 0 == stricmp(pDlg->GetName(), "Game_Mark_Del") && iRetVal == IDYES )
	{
		CommonMessage("Win_MarkAstroList", "delete", 0, 0);
	}
	else if( 0 == stricmp(pDlg->GetName(), "Game_OpenCircle") && iRetVal == IDYES )
	{
		GetDialog("Win_ClassmateManage")->Show(true);
	}
	else if( 0 == stricmp(pDlg->GetName(), "Game_EquipLvUp1"))
	{
		if (iRetVal == IDYES)
			MessageBox("Game_EquipLvUp2", GetStringFromTable(3656), MB_YESNO);
		else if (iRetVal == IDOK)
			return true;
	}
	else if( 0 == stricmp(pDlg->GetName(), "Game_EquipLvUpBind") && iRetVal == IDYES )
	{
		((CDlgEquipLvup*)GetDialog("Win_EquipLvup"))->SendLvupCmd();
	}
	else if( 0 == stricmp(pDlg->GetName(), "Game_EquipLvUp2") )
	{
		if (iRetVal == IDYES)
			CommonMessage("Win_EquipLvup", "lv_up", 0, 0);
		else if (iRetVal == IDOK)
			return true;
	}
	else if( 0 == stricmp(pDlg->GetName(), "Game_TaskFly") && iRetVal == IDYES )
	{
		c2s_SendCmdTaskFlyPos(pDlg->GetData(), (int)pDlg->GetDataPtr());
	}
	else if( 0 == stricmp(pDlg->GetName(), "Game_TaskReachSiteFly") && iRetVal == IDYES )
	{
		c2s_SendCmdTaskReachSiteFly(pDlg->GetData());
	}
	else if( 0 == stricmp(pDlg->GetName(), "Game_DailyPrompFly") && iRetVal == IDYES )
	{
		c2s_SendCmdActivityFlyPos(pDlg->GetData());
	}
	else if( 0 == stricmp(pDlg->GetName(), "Game_Consign") && iRetVal == IDYES )
	{
		CommonMessage("Win_ShopConsign", "consign", 0, 0);
	}
	else if( 0 == stricmp(pDlg->GetName(), "Game_ConsignCancel") && iRetVal == IDYES )
	{
		pSession->consign_cancel(pDlg->GetData64());
	}
	else if( 0 == stricmp(pDlg->GetName(), "Game_Accessory_Detach") && iRetVal == IDYES )
	{
		CommonMessage("Win_Accessory_Detach", "detach_confirm", pDlg->GetData(), 0);
	}
	else if( 0 == stricmp(pDlg->GetName(), "Game_Extract_Bijou") && iRetVal == IDYES )
	{
		CommonMessage("Win_Accessory_Spilt", "extract_confirm", 0, 0);
	}
	else if( 0 == stricmp(pDlg->GetName(), "Game_Hole_Drill") && iRetVal == IDYES )
	{
		pSession->c2s_CmdNPCSevIdentifyGemSlots(pDlg->GetData(), (int)pDlg->GetDataPtr());
	}
	else if( 0 == stricmp(pDlg->GetName(), "Game_Accessory_Custom") && iRetVal == IDYES )
	{
		CommonMessage("Win_Accessory_Custom", "confirm", 0, 0);
	}
	else if( 0 == stricmp(pDlg->GetName(), "Game_Accessory_Attach_Confirm") && iRetVal == IDYES )
	{
		CommonMessage("Win_Accessory_Attach", "attach_confirm", pDlg->GetData(), (long)pDlg->GetDataPtr());
	}
	else if( 0 == stricmp(pDlg->GetName(), "Game_DungeonLeave") && iRetVal == IDYES )
	{
		g_pGame->GetGameRun()->SaveConfigsToServer();
		c2s_SendCmdLeaveRaid();
	}
	else if( 0 == stricmp(pDlg->GetName(), "Game_DungeonKick") && iRetVal == IDYES )
	{
		pSession->raid_open_vote(pDlg->GetData(), CECBattleDungeonMan::getSingletonPtr()->m_iRoomId);
	}
	else if( 0 == stricmp(pDlg->GetName(), "Game_RuneDecompose") && iRetVal == IDYES )
	{
		c2s_SendCmdRuneDecompose(pDlg->GetData());
	}
	else if( 0 == stricmp(pDlg->GetName(), "Game_RuneReset") && iRetVal == IDYES )
	{
		CDlgRuneReset *pRuneReset = (CDlgRuneReset*)GetDialog("Win_Equip_Yuanying_Default");
		pRuneReset->DoServerStart();
	}
	else if ( 0 == stricmp(pDlg->GetName(), "Game_RuneRefineChange") && iRetVal == IDYES )
	{
		c2s_SendCmdRefineAction(true, EQUIPIVTR_RUNE);
	}
	else if ( 0 == stricmp(pDlg->GetName(), "Game_FinishInfo") )
	{
		AString *pUrl = (AString*)pDlg->GetDataPtr("url");
		if(iRetVal == IDYES)
			ShellExecuteA(NULL,"open", *pUrl, NULL, NULL, SW_NORMAL);
		delete pUrl;
	}
	else if ( 0 == stricmp(pDlg->GetName(), "Game_ResetSkill") && iRetVal == IDYES )
	{
		c2s_SendCmdResetSkillPropFree(1);
	}
	else if ( 0 == stricmp(pDlg->GetName(), "Game_ResetTalent") && iRetVal == IDYES )
	{
		c2s_SendCmdResetSkillPropFree(2);
	}
	else if ( 0 == stricmp(pDlg->GetName(), "Game_GuessTop3Cancel") && iRetVal == IDYES )
	{
		c2s_SendCmdCancelTop3Guess(pDlg->GetData("area"));
	}
	else if ( 0 == stricmp(pDlg->GetName(), "Game_TalismanComboSkill") && iRetVal == IDYES )
	{
		CommonMessage("Win_TalismanCombineSkill", "comboskill", 0, 0);
	}
	else if ( 0 == stricmp(pDlg->GetName(), "Game_TangYuan_GetAward") && iRetVal == IDYES )
	{
		CommonMessage("Win_Lottery5", "getaward", 0, 0);
	}
	if( 0 == stricmp(pDlg->GetName(), "GameOk_TerriRemind"))
	{
		pSession->territory_get_map();
		GetDialog("Win_Battle_TerriWarArea")->Show(true);
		PAUIOBJECT pObjWar = GetDialog("Win_CharHead")->GetDlgItem("Btn_BattleTerri");
		pObjWar->Show(true);
	}
	else if( 0 == stricmp(pDlg->GetName(), "GameOk_CircleCanGetAward"))
	{
		GetDialog("Win_ClassmateManage")->Show(true);
	}
	else if( 0 == stricmp(pDlg->GetName(), "Game_1000yuanbao") && iRetVal == IDOK )
	{
		int nNum = a_atoi(GetDialog("Win_InputGNO")->GetDlgItem("DEFAULT_Txt_No.")->GetText());
		PAUIOBJECT pObj = GetDialog("Win_InputGNO")->GetDlgItem("Txt_Confirm");
		c2s_SendCmdMallShopping2((int)pObj->GetDataPtr(), (int)pObj->GetData(), 0, nNum);

		GetDialog("Win_InputGNO")->Show(false);
	}
	else if( 0 == stricmp(pDlg->GetName(), "Game_MapTransfer") && iRetVal == IDOK)
	{
		g_pGame->GetGameSession()->c2s_CmdNPCSevUseUiTransfer(pDlg->GetData());
		m_pDlgMapTransfer->Show(false);
	}
	else if ( 0 == stricmp(pDlg->GetName(), "Game_FengshenDrill") && iRetVal == IDOK)
	{
		m_pDlgFengshenDrill->SendCmd();
	}
	else if ( 0 == stricmp(pDlg->GetName(), "Game_FengshenUpgrade") && iRetVal == IDOK)
	{
		m_pDlgFengshenUpgrade->SendCmd();
	}
	else if ( 0 == stricmp(pDlg->GetName(), "Game_AstrologyDestory") && iRetVal == IDOK)
	{
		m_pDlgFengshenConvert->SendCmd();
	}
	else if( 0 == stricmp(pDlg->GetName(),"FactionPkInvite"))
	{
	   CDlgBattleFactionWarApply *pDlgFactionWar = (CDlgBattleFactionWarApply*)GetDialog("Win_Battle_GuildWar_Apply");
	   pDlgFactionWar->ProcessFactionWarInvite(iRetVal == IDOK);
	}
	else if( 0 == stricmp(pDlg->GetName(),"FactionPkBegin"))
	{
	   CDlgBattleFactionWarControl *pDlgFactionWarCrl = (CDlgBattleFactionWarControl*)GetDialog("Win_Battle_GuildWar_Applylist");
	   pDlgFactionWarCrl->ProcessFactionWarBegin(iRetVal == IDOK);
	}
	else if( 0 == stricmp(pDlg->GetName(),"FactionPkWatchApplay"))
	{
	   CDlgBattleFactionWarWatch *pDlgFactionWarWatch = (CDlgBattleFactionWarWatch*)GetDialog("Win_Battle_GuildWarList");
	   pDlgFactionWarWatch->ProcessFactionWarWatchApply(iRetVal == IDOK);
	}
	else if( 0 == stricmp(pDlg->GetName(),"Game_Perfusion") && iRetVal == IDOK) // 2012-10-17 add 添加消息提示 by zy
	{
	  CDlgAgentCombined *pDlgAgentCombined = (CDlgAgentCombined*)GetDialog("Win_DrugEnchanting");	
	  if(pDlgAgentCombined)
	  {
		  pDlgAgentCombined->CheckAgentCombiend();
	  }
	}
	else if( 0 == stricmp(pDlg->GetName(),"Game_AgentItemBind") && iRetVal == IDYES) //2012-10-17 add 绑定提示 by zy
	{
		CDlgAgentCombined* pAgentCombined = (CDlgAgentCombined*)(GetDialog("Win_DrugEnchanting"));
		if(pAgentCombined)
		{
			CECIvtrItem* pItem = g_pGame->GetGameRun()->GetHostPlayer()->GetPack()->GetItem(pDlg->GetData());
			if(pItem)
			{
				pItem->Freeze(true);
				pAgentCombined->SetAgentCombiendType(pItem,pDlg->GetData());
				pAgentCombined->Show(true);
			}
		}
	}
	else if(0 == stricmp(pDlg->GetName(),"KingChangeSlogan"))
	{
	   CDlgKingChangeSlogan* pChangeSlogan = (CDlgKingChangeSlogan*)(GetDialog("Win_KingChat"));
	   pChangeSlogan->ProcessSloganChange(iRetVal == IDOK);
	}
	else if(0 == stricmp(pDlg->GetName(),"KingDismiss"))
	{
	   CDlgKingGrantRightContext* pGrantRightContext = (CDlgKingGrantRightContext*)(GetDialog("Win_KingGrantContext"));
	   pGrantRightContext->ProcessKingDismiss(iRetVal == IDOK,pDlg->GetData());
	}
	else if(0 == stricmp(pDlg->GetName(),"KingGameAttri"))
	{
	  CDlgKingInfo* pKingInfo = (CDlgKingInfo*)(GetDialog("Win_King_Info"));
	  pKingInfo->ProcessGameAttri(iRetVal == IDOK,pDlg->GetData());
	}
	else if(0 == stricmp(pDlg->GetName(),"KingShopBuy"))
	{
	  CDlgKingShop* pKingShop = (CDlgKingShop*)(GetDialog("Win_King_SubRight02"));
	  pKingShop->ProcessKingShopExchange(iRetVal == IDOK,pDlg->GetData());
	}
	else if(0 == stricmp(pDlg->GetName(),"KingWarLeave"))
	{
	  CDlgKingBattleInfo3* pKingBattleInfo = (CDlgKingBattleInfo3*)(GetDialog("Win_King_BattleInfo3"));
	  pKingBattleInfo->ProcessKingWarLeave(iRetVal == IDOK);	
	}
    else if(0 == stricmp(pDlg->GetName(),"TouchExchange"))
	{
	  CDlgTouchExchange* pTouchExchange = (CDlgTouchExchange*)(GetDialog("Win_Game_TouchExchange"));
	  pTouchExchange->ProcessTouchExchange(iRetVal == IDOK ,pDlg->GetData());
	}
	else if(0 == stricmp(pDlg->GetName(),"EquipReName"))
	{
	  CDlgEquipReName* pEquipReName = (CDlgEquipReName*)(GetDialog("Win_Equip_ChangeName"));
	  pEquipReName->ProcessEquipReName(iRetVal == IDOK);
	}
	else if(0 == stricmp(pDlg->GetName(),"Game_KingTransmit"))
	{
		DWORD dwHandle = pDlg->GetData();
		pSession->kingwar_guardinvite_res(dwHandle, iRetVal == IDYES);
	}
    else if(0 == stricmp(pDlg->GetName(),"Game_KingOpenBathInvite"))
	{
	    DWORD dwHandle = pDlg->GetData();
		pSession->kingwar_openbathinvite_res(dwHandle,iRetVal == IDYES);
	}
	else if(0 == stricmp(pDlg->GetName(),"Game_KingBathInvite"))
	{
		pSession->c2s_SendCmdKingBathInviteReply(pDlg->GetData(),iRetVal == IDNO);
	}
	else if(0 == stricmp(pDlg->GetName(),"KingOpenBathroom"))
	{
	    CDlgKingInfo* pKingInfo = (CDlgKingInfo*)(GetDialog("Win_King_Info"));
	    pKingInfo->ProcessOpenBathroom(iRetVal == IDOK);
	}
	else if(0 == stricmp(pDlg->GetName(),"Game_KingOpenBathFlyToNpc") && iRetVal == IDYES)
	{
	    pSession->c2s_SendCmdKingFlyNPCPos();  
	}
	else if (0 == stricmp(pDlg->GetName(), "Game_Rune2013_Install") && iRetVal == IDYES)
	{
		CommonMessage("Win_Equip_Rune", "install", 0, 0);
	}
	else if (0 == stricmp(pDlg->GetName(), "Game_Rune2013_Enchanting") && iRetVal == IDYES)
	{
		CommonMessage("Win_Equip_Rune_Enchanting", "enchanting", 0, 0);
	}
	else if (0 == stricmp(pDlg->GetName(), "Game_RuneLevelUp") && iRetVal == IDOK)
	{
		CDlgRuneLevelUp* pDlgRuneLevelUp = (CDlgRuneLevelUp*)(GetDialog("Win_Equip_Yuanying_Upgrade"));
		pDlgRuneLevelUp->ServerStartWithoutCheck();
	}
	else if (0 == stricmp(pDlg->GetName(), "Game_CalendarPresent_Use_Ingot") && iRetVal == IDYES)
	{
		CommonMessage("Win_CalendarPresent", "getitem", 0, (LPARAM)pDlg);
	}	
	else if(0 == stricmp(pDlg->GetName(), "Inventory_Expand") && iRetVal == IDOK)
	{
		ExpandItem_Info* pItemInfo = (ExpandItem_Info*)pDlg->GetDataPtr("Expand");
		if(pItemInfo)
		{
			if(g_pGame->GetGameRun()->GetHostPlayer()->GetCash() < pItemInfo->iMoney * 100)
			{
				AddChatMessage(GetStringFromTable(8397), GP_CHAT_MISC);
			}
			else
			{
				if(pItemInfo->iCount == 1)
				{
					if(pItemInfo->index == 0)
						c2s_SendCmdMallShopping2(pItemInfo->cost_item[0].itemid, pItemInfo->cost_item[0].itemindex, 0, pItemInfo->cost_item[0].itemcnt);
					else if(pItemInfo->index == 1)
						c2s_SendCmdMallShopping2(pItemInfo->cost_item[1].itemid, pItemInfo->cost_item[1].itemindex, 0, pItemInfo->cost_item[1].itemcnt);
				}
				else if(pItemInfo->iCount == 2)
				{
					c2s_SendCmdMallShopping2(pItemInfo->cost_item[0].itemid, pItemInfo->cost_item[0].itemindex, 0, pItemInfo->cost_item[0].itemcnt);
					c2s_SendCmdMallShopping2(pItemInfo->cost_item[1].itemid, pItemInfo->cost_item[1].itemindex, 0, pItemInfo->cost_item[1].itemcnt);
				}
			}
		}
	}
	else if(0 == stricmp(pDlg->GetName(), "CrssvrChangeCaptain") && iRetVal == IDYES)
	{
		CECGameSession *pSession =  g_pGame->GetGameSession();
		pSession->c2s_CmdSendCrssvrTeamsChgCaptain(pDlg->GetData());
	}
	else if(0 == stricmp(pDlg->GetName(), "CrssvrKickTeamMenmber") && iRetVal == IDYES)
	{
		CECGameSession *pSession =  g_pGame->GetGameSession();
		pSession->c2s_CmdSendCrssvrTeamsKick(pDlg->GetData());
	}
	else if(0 == stricmp(pDlg->GetName(), "CrssvrDismissTeams") && iRetVal == IDYES)
	{
		CECGameSession *pSession =  g_pGame->GetGameSession();
		pSession->c2s_CmdSendCrssvrTeamsDismiss();
	}
	else if(0 == stricmp(pDlg->GetName(), "CrssvrExitTeams") && iRetVal == IDYES)
	{
		CECGameSession *pSession =  g_pGame->GetGameSession();
		pSession->c2s_CmdSendCrssvrTeamsLeave();
	}
	else if(0 == stricmp(pDlg->GetName(), "Calendar_Binded") && iRetVal == IDYES)
	{
		int id = pDlg->GetData();
		int idpos = (int)pDlg->GetDataPtr("pos");
		CDlgCalendarPresent* pDlg = (CDlgCalendarPresent*)GetDialog("Win_CalendarPresent");
		if (pDlg)
			pDlg->OpenSalePromotionItem(idpos, id);
	}
	else if(0 == stricmp(pDlg->GetName(), "Game_InviteActiveEmote"))
	{
		g_pGame->GetGameSession()->c2s_CmdRelpyActiveEmote(pDlg->GetData(), (DWORD)pDlg->GetDataPtr("dword"), iRetVal == IDYES ? 0 : 1);
	}
	else if(0 == stricmp(pDlg->GetName(),"SendCreateBuild") && iRetVal == IDOK)
	{
		DlgGuildCreateLandBuild* pGuildCreateLandBuild = (DlgGuildCreateLandBuild*)(GetDialog("Win_Guild_LandBuild_List"));
		pGuildCreateLandBuild->SendCreateBuild();
	}
	else if(0 == stricmp(pDlg->GetName(),"SendUpDradeBuild") && iRetVal == IDOK)
	{
		DlgGuildCreateLandBuild* pGuildCreateLandBuild = (DlgGuildCreateLandBuild*)(GetDialog("Win_Guild_LandBuild_List"));
		pGuildCreateLandBuild->SendUpDradeBuild();
	}
	else if(0 == stricmp(pDlg->GetName(),"MessageRemoveBuild") && iRetVal == IDOK)
	{
		DlgGuildLandBuild* pGuildLandBuild = (DlgGuildLandBuild*)(GetDialog("Win_Guild_LandBuild"));
		pGuildLandBuild->MessageBoxBuild();
	}
	else if(0 == stricmp(pDlg->GetName(),"RemoveBuild") && iRetVal == IDOK)
	{
		DlgGuildLandBuild* pGuildLandBuild = (DlgGuildLandBuild*)(GetDialog("Win_Guild_LandBuild"));
		pGuildLandBuild->RemoveBuild();
	}
	else if(0 == stricmp(pDlg->GetName(),"TransferBuild") && iRetVal == IDOK)
	{
		DlgGuildLandBuild* pGuildLandBuild = (DlgGuildLandBuild*)(GetDialog("Win_Guild_LandBuild"));
		pGuildLandBuild->SendTransferBuild();
	}
	else if(0 == stricmp(pDlg->GetName(),"GuildReceiveTask") && iRetVal == IDOK)
	{
		g_pGame->GetTaskTemplateMan()->ManualTrigTask(pHost->GetTaskInterface(),  pDlg->GetData());
	}
	return AUILuaManager::OnMessageBox(iRetVal, pDlg);
}

bool CECGameUIMan::Tick(DWORD dwDeltaTime)
{
	bool bval;
	ACString strText;

#ifdef LUA_UI
// 	//test to reload a script
// 	if (GetKeyState('U') & 0x8000)
// 	{
// 		static int count = 0;
// 		//m_pDlgCamera->LuaInit("Interfaces\\script\\win_camera.lua");
// 		//m_pDlgCamera->LuaUpdate();
// 		g_LuaMan.Update();
// 
// 		char msg[200];
// 		sprintf(msg, "reload win_camera.lua %d times\n", count);
// 		++count;
// 		OutputDebugStringA(msg);
// 	}
#endif
	
	CECHostPlayer* pHostPlayer = g_pGame->GetGameRun()->GetHostPlayer();

	if( m_bReplayMode != m_pDlgReplay->IsShow() )
		m_pDlgReplay->Show(m_bReplayMode);
	
	if( m_pDlgFriendList->m_dwBlackListGetNameTime != 0 &&
		GetTickCount() - m_pDlgFriendList->m_dwBlackListGetNameTime > 30000 )
	{
		m_pDlgFriendList->m_dwBlackListGetNameTime = 0;
		EC_BLACKLIST_SETTING bs = g_pGame->GetConfigs()->GetBlackListSettings();
		int nLen = 0;
		while( bs.idPlayer[nLen] > 0 && nLen < EC_BLACKLIST_LEN )
			nLen++;
		int i;
		for (i = 0; i < nLen; i++)
		{
			if( bs.idPlayer[i] > 0 )
			{
				if( g_pGame->GetGameRun()->GetPlayerName(bs.idPlayer[i], false) == NULL )
				{
					memmove(bs.idPlayer + i, bs.idPlayer + i + 1,
						sizeof(int) * (nLen - i - 1));
					i--;
					nLen--;
				}
			}
			else
				break;
		}
		memset(bs.idPlayer + i, 0, sizeof(int) * (EC_BLACKLIST_LEN - i) );
			
		g_pGame->GetConfigs()->SetBlackListSettings(bs);
	}
	
	if (g_pGame->GetGameRun()->GetHelper()->IsRunning() && 
		pHostPlayer->GetCurPetIndex() != -1 &&
		!m_bShowAllPanels )
	{
		m_pDlgPetSkill->Tick();
	}
	

	if( !m_bShowAllPanels || !m_bUIEnable)
	{
		if( !m_bUIEnable )
		{
			DWORD dwTotalTime, dwCnt;
			PAUIDIALOG pDlg = GetDialog("Win_EnterWait");
			PAUILABEL pLabel = (PAUILABEL)pDlg->GetDlgItem("Txt_Message");

			if( m_bReplayMode )
				strText = GetStringFromTable(831);
			else if( g_pGame->GetGameSession()->GetOvertimeCnt(dwTotalTime, dwCnt) )
			{
				dwTotalTime /= 1000;
				dwCnt /= 1000;
				strText.Format(GetStringFromTable(516), dwTotalTime, dwCnt);
			}
			else
				strText = GetStringFromTable(515);

			pLabel->SetText(strText);
			pLabel->SetAlign(AUIFRAME_ALIGN_CENTER);
		}

		return true;
	}
	
	const char *szCountry = g_pGame->GetConfigs()->GetCountryRegion();
	if( stricmp(szCountry, "kr")!=0 && g_pGame->GetGameRun()->GetWallowInfo().anti_wallow_active )	// 韩国不使用这个防沉迷提示
	{
		int stime = g_pGame->GetServerGMTTime();
		int nTime = stime - g_pGame->GetGameRun()->GetWallowInfo().play_time;
		/* 0:00 - 1:00 0 by fuchonggang
		 * 1:00 - 2:00 1
		 * 2:00 - 2:50 2
		 * 2:50 - 2:55 3
		 */
		int curWallowLevel;
		if (nTime < 3600)
			curWallowLevel = 0;
		else if (nTime < 7200)
			curWallowLevel = 1;
		else if (nTime < 7200+3000)
			curWallowLevel = 2;
		else
			curWallowLevel = 3;
		
		ACString strText, strTime;
		strTime.Format(GetStringFromTable(1611), nTime / 3600, ((nTime % 3600) / 60));
		strText.Format(GetStringFromTable(curWallowLevel<2 ? 898 : 897+curWallowLevel), strTime);
		PAUIIMAGEPICTURE pImage = (PAUIIMAGEPICTURE)m_pDlgBuff->GetDlgItem("Img_Captivation");
		pImage->Show(true);
		pImage->SetHint(strText);
		pImage->FixFrame(curWallowLevel<2 ? 0 : curWallowLevel-1);

		if (curWallowLevel != m_iCurWallowLevel)
		{
			m_iCurWallowLevel = curWallowLevel;
			strText = GetStringFromTable(900 + curWallowLevel);
			AddChatMessage(strText, GP_CHAT_BROADCAST);
		}
	}
	else
		m_pDlgBuff->GetDlgItem("Img_Captivation")->Show(false);

	CECInventory *pPack = pHostPlayer->GetPack(IVTRTYPE_EQUIPPACK);
	if( pPack->GetItem(16) && pPack->GetItem(16)->GetClassID() == CECIvtrItem::ICID_TALISMAN_MAINPART && g_pGame->GetConfigs()->GetGameSettings().bShowTalismanDlg)
	{
		if( !GetDialog("Win_TalismanSmall")->IsShow() )
			if (!m_pDlgTalisman->IsShow())
				m_pDlgTalisman->Show(true, false, false);
	}
	else
	{
		m_pDlgTalisman->Show(false);
		GetDialog("Win_TalismanSmall")->Show(false);
	}

	CDlgTargetHP* pDlgTargetHP = (CDlgTargetHP*)GetDialog("Win_Target_HpPrgs");
	int idSelObj = pHostPlayer->GetSelectedTarget();
	if( idSelObj == 0 )
	{
		if( m_pDlgTarget )
		{
			m_pDlgTarget->Show(false);
			m_pDlgTarget = NULL;
		}
		if(pDlgTargetHP)
			pDlgTargetHP->Show(false);
	}
	else
	{
		PAUIDIALOG pDlg = NULL;
		if( ISPLAYERID(idSelObj) )
			pDlg = GetDialog("Win_TargetPlayer");
		else if( ISNPCID(idSelObj) )
		{
			CECNPC *pNPC = g_pGame->GetGameRun()->GetWorld()->GetNPCMan()->GetNPC(idSelObj);
			if( pNPC )
			{
				if(pNPC->IsServerNPC())
				{
					CECNPCServer* pNPCServer = (CECNPCServer*)pNPC;
					const NPC_ESSENCE *pNPCEssence = pNPCServer->GetDBEssence();
					if(pNPCEssence && pNPCEssence->head_icon !=0 )
						pDlg = GetDialog("Win_TargetNPC2");
					else
						pDlg = GetDialog("Win_TargetMonster");
				}
				else if(pNPC->IsPetNPC())
				{
					pDlg = GetDialog("Win_TargetNPC2");					
				}
				else if(pNPC->IsMonsterNPC())
				{
					CECMonster* pMonster = (CECMonster*)pNPC;
					const MONSTER_ESSENCE *pMonsterEssence = pMonster->GetDBEssence();
					if(pMonsterEssence && pMonsterEssence->head_icon !=0 )
						pDlg =  GetDialog("Win_TargetNPC2");
					else
						pDlg = GetDialog("Win_TargetMonster");
				}
			}
		}
		if( pDlg )
		{
			if( pDlg != m_pDlgTarget )
			{
				if( m_pDlgTarget )
					m_pDlgTarget->Show(false);
				if(pDlgTargetHP)
					pDlgTargetHP->Show(false);
				pDlg->Show(true, false, false);
				m_pDlgTarget = (CDlgTarget*)pDlg;
			}
			if(pDlgTargetHP)
			{
				if(ISPLAYERID(idSelObj))
				{
					CECElsePlayer *pPlayer = g_pGame->GetGameRun()->GetWorld()->GetPlayerMan()->GetElsePlayer(idSelObj);
					if(pPlayer && pPlayer->GetBloodPoolState() )
					{
						if(!pDlgTargetHP->IsShow())
							pDlgTargetHP->Show(true);
					}
					else
						pDlgTargetHP->Show(false);
				}
				else
					pDlgTargetHP->Show(false);
			}
		}
	}


//	DWORD dwAccountTime = (DWORD)(
//		(g_pGame->GetGameSession()->GetAccountTime() +
//		g_pGame->GetGameSession()->GetFreeGameTime()) / 1000);
//	bool bTip = false;
//	while( dwAccountTime <= l_dwAccountTimeTip[m_nAccountTimeIndex] &&
//		m_nAccountTimeIndex < CECGAMEUIMAN_ACCOUNTTIME_TIPMAX )
//	{
//		m_nAccountTimeIndex++;
//		bTip = true;
//	}
//	PAUIDIALOG pDlg = GetDialog("Win_AccountTime");
//	if( bTip )
//	{
//		ACString szTip, szTime;
//		int nIndexOld = m_nAccountTimeIndex - 1;
//		if( dwAccountTime > 60 )
//			szTime.Format(GetStringFromTable(1602), l_dwAccountTimeTip[nIndexOld] / 60);
//		else
//			szTime.Format(GetStringFromTable(1601), l_dwAccountTimeTip[nIndexOld]);
//		szTip.Format(GetStringFromTable(696), szTime);
//		if( dwAccountTime > 600 )
//			szTip = _AL("^FFFF00") + szTip;
//		else
//			szTip = _AL("^FF0000") + szTip;
//		pDlg->GetDlgItem("Message")->SetText(szTip);
//		pDlg->Show(true, false, false);
//		pDlg->SetData(GetTickCount());
//	}
//	else
//		if( pDlg->IsShow() && GetTickCount() - pDlg->GetData() > 5000 )
//			pDlg->Show(false);
	RefreshHint();
	RefreshBrokenList();
	m_pDlgTaskTrace->RefreshTaskTrace();
	m_pDlgMinimizeBar[0]->ArrangeMinimizeBar();

	bval = m_pDlgTeamMain->UpdateTeamInfo();
	if( !bval ) return AUI_ReportError(__LINE__, __FILE__);

 	bval = m_pDlgQuickBar8->UpdateShortcuts();
 	if( !bval ) return AUI_ReportError(__LINE__, __FILE__);

	int nCurChatAlpha = m_pDlgChat->GetChatAlpha();
	PAUITEXTAREA pText = (PAUITEXTAREA)m_pDlgChat1->GetDlgItem("Txt_Content");
	if( m_pDlgMouseOn && 
		(0 == stricmp(m_pDlgMouseOn->GetName(), "Win_ChatInfo") ||
		0 == stricmp(m_pDlgMouseOn->GetName(), "Win_KfpkChat") ||
		0 == stricmp(m_pDlgMouseOn->GetName(), "Win_Chat")))
	{
		pText->SetOnlyShowText(false);
		m_pDlgChat1->SetAlpha((nCurChatAlpha + 1) * 42 + 1);
		m_pDlgChat1->GetDlgItem("Img_ZoomIn")->Show(true);
	}
	else if( !m_pDlgActive || !m_pDlgActive->GetCaptureObject() )
	{
		pText->SetOnlyShowText(true);
		m_pDlgChat1->SetAlpha(nCurChatAlpha * 42 + 1);
		m_pDlgChat1->GetDlgItem("Img_ZoomIn")->Show(false);
	}

	pText = (PAUITEXTAREA)m_pDlgChat2->GetDlgItem("Txt_Content");
	if( m_pDlgMouseOn && 0 == stricmp(m_pDlgMouseOn->GetName(), "Win_ChatInfo2"))
	{
		pText->SetOnlyShowText(false);
		m_pDlgChat2->SetAlpha((nCurChatAlpha + 1) * 42 + 1);
		m_pDlgChat2->GetDlgItem("Img_ZoomIn")->Show(true);
	}
	else if( !m_pDlgActive || !m_pDlgActive->GetCaptureObject() )
	{
		pText->SetOnlyShowText(true);
		m_pDlgChat2->SetAlpha(nCurChatAlpha * 42 + 1);
		m_pDlgChat2->GetDlgItem("Img_ZoomIn")->Show(false);
	}

	PAUITREEVIEW pTree = (PAUITREEVIEW)m_pDlgTaskTrace->GetDlgItem("Tv_List");	
	POINT cursorPos;
	if (::GetCursorPos(&cursorPos) == TRUE &&
		::ScreenToClient(m_pA3DDevice->GetDeviceWnd(), &cursorPos) == TRUE)
	{
		A3DRECT rcWindow = GetRect();
		int x = cursorPos.x - rcWindow.left;
		int y = cursorPos.y - rcWindow.top;
		
		if (m_pDlgTaskTrace->GetRect().PtInRect(x, y))
		{
			pTree->SetOnlyShowText(false);
			m_pDlgTaskTrace->GetDlgItem("Img_ZoomIn")->Show(true);
			m_pDlgTaskTrace->SetAlpha(45);
		}
		else if (!m_pDlgActive || !m_pDlgActive->GetCaptureObject())
		{
			pTree->SetOnlyShowText(true);
			m_pDlgTaskTrace->GetDlgItem("Img_ZoomIn")->Show(false);
			m_pDlgTaskTrace->SetAlpha(1);
		}
	}

/*	PAUITREEVIEW pTv_Trace = (PAUITREEVIEW)m_pDlgTaskTrace->GetDlgItem("Tv_List");
	PAUIIMAGEPICTURE pImg_Zoom =  (PAUIIMAGEPICTURE)m_pDlgTaskTrace->GetDlgItem("Img_ZoomIn");
	if (m_pDlgMouseOn == m_pDlgTaskTrace)
	{
		pTv_Trace->SetOnlyShowText(false);
		m_pDlgTaskTrace->SetAlpha(200);
		pImg_Zoom->Show(true);
	}
	else if( !m_pDlgActive || !m_pDlgActive->GetCaptureObject() )
	{
		pTv_Trace->SetOnlyShowText(true);
		m_pDlgTaskTrace->SetAlpha(1);
		pImg_Zoom->Show(false);
	}
*/
	//if( AUIEditBox::IsIMEAlphaNumeric() )
	//	((PAUIIMAGEPICTURE)m_pDlgChat->GetDlgItem("Input"))->FixFrame(0);
	//else
	//	((PAUIIMAGEPICTURE)m_pDlgChat->GetDlgItem("Input"))->FixFrame(1);
	//strText = AUIEditBox::GetCurrentIMEName();
	//m_pDlgChat->GetDlgItem("Txt_1")->SetHint(strText);
	//strText = strText.Left(1);
	//m_pDlgChat->GetDlgItem("Txt_1")->SetText(strText);

	if( m_pDlgDragDrop->IsShow() && m_pDlgActive != m_pDlgDragDrop )
	{
		BringWindowToTop(m_pDlgDragDrop);
		A3DVIEWPORTPARAM *p = m_pA3DEngine->GetActiveViewport()->GetParam();
		POINT ptPos;
		ptPos = m_pDlgDragDrop->GetPos();
		POINT pt = m_pDlgDragDrop->m_ptDragPos;
		m_pDlgDragDrop->SetCapture(true, ptPos.x + pt.x, ptPos.y + pt.y);
	}
	
	int idBuddy = pHostPlayer->GetBuddyID();
	CDlgBuddyState *pBuddyState = (CDlgBuddyState*)GetDialog("Win_RideTogether");
	if( !m_pDlgBuddyState->IsShow() && pBuddyState && !pBuddyState->IsShow() && !pHostPlayer->IsBathing()
		&& pHostPlayer->GetQilinConnectBuddy() == 0 && pHostPlayer->GetAttachMode() != CECPlayer::enumAttachActiveEmote)
	{
		if(idBuddy != 0 )
			m_pDlgBuddyState->SetBuddyID(idBuddy, 0);
		else
		{
			int idMRide = pHostPlayer->GetMultiRideLeaderId();
			if ( idMRide!=0)
				m_pDlgBuddyState->SetBuddyID(idMRide, 1);
			else if (pHostPlayer->IsMultiRideLeader())
			{
				if ( pBuddyState->IsRMinimized() )
				{
					if (!pHostPlayer->IsMultiRideHasMember())
					{
						pBuddyState->Tick();
					}					
				}
				else
				{
					if (pHostPlayer->IsMultiRideHasMember())
					{
						pBuddyState->SetBuddyID(pHostPlayer->GetCharacterID(), 1);
					}
				}
			}
		}
	}
	
	CDlgQilinBuddy* pDlgQilinBuddy = (CDlgQilinBuddy*)GetDialog("Win_InviteRide");
	int idQilinBuddy = pHostPlayer->GetQilinConnectBuddy();
	if(idQilinBuddy && pDlgQilinBuddy && !pDlgQilinBuddy->IsShow())
		pDlgQilinBuddy->Show(true);

	if(idQilinBuddy == 0 && pDlgQilinBuddy && pDlgQilinBuddy->IsShow())
		pDlgQilinBuddy->Show(false);

	if( !m_pDlgDoubleExpS->IsShow() &&
		g_pGame->GetGameRun()->GetDoubleExpMode() >= 2 )
	{
		m_pDlgDoubleExpS->AlignTo(GetDialog("Win_Gt"),
				AUIDIALOG_ALIGN_OUTSIDE, AUIDIALOG_ALIGN_RIGHT,
				AUIDIALOG_ALIGN_INSIDE, AUIDIALOG_ALIGN_TOP);
		m_pDlgDoubleExpS->Show(true);
	}
	
	PAUIDIALOG pDlg = GetDialog("Win_Popface");
	int nCurSet = pHostPlayer->GetCurEmotionSet();
	if( pDlg->GetData() != (DWORD)nCurSet )
	{
		pDlg->SetData(nCurSet);

		PAUIEDITBOX pEmotionEdit;
		pEmotionEdit = (PAUIEDITBOX)m_pDlgChat->GetDlgItem("DEFAULT_Txt_Speech");
		pEmotionEdit->SetCurrentEmotion(nCurSet);
 		pEmotionEdit = (PAUIEDITBOX)m_pDlgChannelChat->GetDlgItem("Txt_Content");
 		pEmotionEdit->SetCurrentEmotion(nCurSet);
		pEmotionEdit = (PAUIEDITBOX)m_pDlgChatWhisper->GetDlgItem("DEFAULT_Txt_Speech");
		pEmotionEdit->SetCurrentEmotion(nCurSet);
		pEmotionEdit = (PAUIEDITBOX)GetDialog("Win_ChatSpec")->GetDlgItem("Txt_Content");
		pEmotionEdit->SetCurrentEmotion(nCurSet);
		char szName[20];
		int i;
		for(i = 0; i < CECGAMEUIMAN_MAX_CHATS; i++ )
		{
			sprintf(szName, "Win_FriendChat%d", i);
			PAUIDIALOG pDlgFriend = GetDialog(szName);
			pEmotionEdit = (PAUIEDITBOX)pDlgFriend->GetDlgItem("Txt_Content");
			pEmotionEdit->SetCurrentEmotion(nCurSet);
		}
		PAUIIMAGEPICTURE pImage;
		for( i = 0; ; i++ )
		{
			sprintf(szName, "Face_%02d", i);
			pImage = (PAUIIMAGEPICTURE)pDlg->GetDlgItem(szName);
			if( !pImage ) break;

			if( i < (int)m_vecEmotion[nCurSet]->size() )
			{
				pImage->SetCover(m_pA2DSpriteEmotion[nCurSet], (*m_vecEmotion[nCurSet])[i].nStartPos);
				pImage->SetHint((*m_vecEmotion[nCurSet])[i].strHint);
			}
			else
			{
				pImage->ClearCover();
				pImage->SetHint(_AL(""));
			}
		}
	}
	
	CDlgInventoryCostume *pDlgInvCos = (CDlgInventoryCostume*)GetDialog("Win_Inventory_Costume");
	pDlgInvCos->UpdateAutoChange(dwDeltaTime);
	
	if (g_pGame->GetGameRun()->GetSecretary()->IsEnable())
	{
		CDlgHelpMemo *pDlgHelpMemo = (CDlgHelpMemo*)GetDialog("Win_Help_Memo");
		pDlgHelpMemo->TickMemo();
	}
	
	CDlgBodyChange *pDlgBCSkill = (CDlgBodyChange*)GetDialog("Win_BodyChangeSkill");
	BOOL bIsTrans = pHostPlayer->IsTransfigured();
	if (!pDlgBCSkill->IsShow() && bIsTrans &&  pHostPlayer->GetTransfigureSkillNum()>0)
		pDlgBCSkill->Show(true);
	else if(pDlgBCSkill->IsShow() && (!bIsTrans || pHostPlayer->GetTransfigureSkillNum()==0))
		pDlgBCSkill->Show(false);
	
	if (pHostPlayer->GetCurPetIndex() != -1)
	{
		m_pDlgHostPet->AutoFeed();
	}
	
	CDlgHost* pDlgHost = (CDlgHost*)GetDialog("Win_Charhead_HpPrgs");
	if(pHostPlayer && pHostPlayer->GetBloodPoolState() && pDlgHost && !pDlgHost->IsShow())
		pDlgHost->Show(true);

	//added 2012-3-31 by Lei Dongyu
	CDlgLevelUp* pTempDlg = (CDlgLevelUp*)GetDialog("Win_Character_LvAttriDetail");
	if(pHostPlayer && pTempDlg && !pTempDlg->IsShow()) 
	{
		if(!pTempDlg->GetLevel())
		{
			pTempDlg->SetLevel(pHostPlayer->GetBasicProps().iLevel);
			pTempDlg->SetClass(pHostPlayer->GetProfession());
			pTempDlg->SetRebornCount(pHostPlayer->GetRebornCount());	
		}
		else if(pTempDlg->GetLevel() != pHostPlayer->GetBasicProps().iLevel
			|| pTempDlg->GetClass() != pHostPlayer->GetProfession())
		{
			if(pTempDlg->GetRebornCount() == pHostPlayer->GetRebornCount())	//不飞升时才显示
			{
				pTempDlg->SetRebornType(true);
				pTempDlg->Show(true);
			}
			else if(pTempDlg->GetLevel() > pHostPlayer->GetBasicProps().iLevel)	//飞升后，新的等级已经从服务器发过来了
			{
				pTempDlg->SetLevel(pHostPlayer->GetBasicProps().iLevel);
				pTempDlg->SetClass(pHostPlayer->GetProfession());
				pTempDlg->SetRebornCount(pHostPlayer->GetRebornCount());
			}
		}
	}

	//UpdateButtonsByTime();

	RefreshShortCutUIInfo();


	/* 星座系统暂不开放，等开放时，把下面这个开关切换移到界面的按钮事件中 wuweixin
	// 显示星座精力值进度条
	if (pHostPlayer && pHostPlayer->IsAstrologyEnergyActive())
	{
		CDlgCommonDlg* pAstrologyEnergyDlg = (CDlgCommonDlg*)GetDialog("Win_Equip_Fengshen_Prgs");
		if (pAstrologyEnergyDlg)
		{
			CECConfigs* pConfigs = g_pGame->GetConfigs();
			if (pAstrologyEnergyDlg->IsShow())
			{
				if (!pConfigs->GetVideoSettings().bPlayerEnergy)
					pAstrologyEnergyDlg->Show(false);
			}
			else if (pConfigs->GetVideoSettings().bPlayerEnergy)
				pAstrologyEnergyDlg->Show(true);
		}
	}
	*/

/*
	CDlgCameraPath* pDlgCameraPath = (CDlgCameraPath*)GetDialog("Win_CameraPath");
	if (GetAsyncKeyState('1')&0x8000)
	{
		pDlgCameraPath->Show(true);
	}
	
	if (GetAsyncKeyState('2')&0x8000)
	{
		pDlgCameraPath->ShowText(_AL("ahwieg\n$2000$jaiwegajweg"));
	}
	
	if (GetAsyncKeyState('3')&0x8000)
	{
		pDlgCameraPath->ShowTextBackGround(true);
	}
	if (GetAsyncKeyState('4')&0x8000)
	{
		pDlgCameraPath->ShowTextBackGround(false);
	}
	if (GetAsyncKeyState('5')&0x8000)
	{
		pDlgCameraPath->Show(false);
	}
	if (GetAsyncKeyState('6')&0x8000)
	{
		pDlgCameraPath->ShowText(_AL(""));
	}
*/

	return AUILuaManager::Tick(dwDeltaTime);
}

bool CECGameUIMan::Render(void)
{
	bool bval;

	if( m_bHideUIPrintMode )
		return true;

	if( !m_bUIEnable || !m_bShowAllPanels )
	{
		bool bRet = true;
		if (GetDialog("Win_CameraPath")->IsShow())
		{
			bRet = GetDialog("Win_CameraPath")->Render();
		}
		if( !m_bUIEnable )
		{
			PAUIDIALOG pDlg;

			pDlg = GetDialog("Win_EnterWait");
			if(!pDlg->IsShow())
				pDlg->Show(true);
			pDlg->Render();

			pDlg = GetDialog("MsgBox_LinkBroken");
			if( pDlg ) pDlg->Render();
		}
		
		GetA3DFTFontMan()->Flush();
		return bRet;
	}
	else
	{
		PAUIDIALOG pDlg = GetDialog("Win_EnterWait");
		if( pDlg->GetData() > 0 )
		{
			ACString strMsg;
			int nTime = g_pGame->GetGameRun()->GetWorld()
				->GetHostPlayer()->GetTimeToExitInstance();

			pDlg->SetData(nTime);
			strMsg.Format(GetStringFromTable(599), nTime / 1000);
			pDlg->GetDlgItem("Txt_Message")->SetText(strMsg);
		}
		else if( pDlg->IsShow() && pDlg->GetData() == 0 )
			pDlg->Show(false);
	}

	if( g_pGame->GetGameRun()->GetWorld()->GetHostPlayer() )
	{
		g_pGame->GetGameRun()->GetWorld()->GetHostPlayer()->RenderForUI(g_pGame->GetViewport());
		if(g_pGame->GetGameRun()->GetWorld()->GetHostPlayer()->IsFreeFlying())
		{
			return true;
		}
	}
	

	bval = UpdateHint();
	if( !bval ) return AUI_ReportError(__LINE__, __FILE__);
	bval = UpdateArrowInfo();
	if( !bval ) return AUI_ReportError(__LINE__, __FILE__);

	int i;
	ALISTPOSITION posMiniMap, posMark, posQShop;
	posMiniMap = m_DlgZOrder.Find(m_pDlgMiniMap);
	if( m_pDlgQShop->IsShow() )
	{
		posQShop = m_DlgZOrder.Find(m_pDlgQShop);
		for(i = 0; i < CECGAMEUIMAN_MAX_QSHOPITEMS; i++)
		{
			if( m_pDlgQShopDefault[i]->IsShow() )
			{
				posMark = m_DlgZOrder.Find(m_pDlgQShopDefault[i]);
				m_DlgZOrder.RemoveAt(posMark);
				m_DlgZOrder.InsertBefore(posQShop, m_pDlgQShopDefault[i]);
			}
		}
	}
	if( GetDialog("Win_QShopPop")->IsShow() )
	{
		posQShop = m_DlgZOrder.Find(GetDialog("Win_QShopPop"));
		for(i = 0; i < CECGAMEUIMAN_MAX_QSHOPITEMS; i++)
		{
			if( m_pDlgQShopDefault[i]->IsShow() )
			{
				posMark = m_DlgZOrder.Find(m_pDlgQShopDefault[i]);
				m_DlgZOrder.RemoveAt(posMark);
				m_DlgZOrder.InsertBefore(posQShop, m_pDlgQShopDefault[i]);
			}
		}
	}
	
	if( GetDialog("WorldMap_Sub")->IsShow() )
	{
		posQShop = m_DlgZOrder.Find(GetDialog("WorldMap_Sub"));
		posMark = m_DlgZOrder.Find(GetDialog("Win_Worldmap_Button"));
		m_DlgZOrder.RemoveAt(posMark);
		m_DlgZOrder.InsertBefore(posQShop, GetDialog("Win_Worldmap_Button"));
	}
	if( GetDialog("WorldMap_TerriSub")->IsShow() )
	{
		posQShop = m_DlgZOrder.Find(GetDialog("WorldMap_TerriSub"));
		posMark = m_DlgZOrder.Find(GetDialog("Win_Worldmap_Button"));
		m_DlgZOrder.RemoveAt(posMark);
		m_DlgZOrder.InsertBefore(posQShop, GetDialog("Win_Worldmap_Button"));
	}
	
	PAUIDIALOG pDlg = GetDialog("Win_Help_Cyclo");
// 	if( pDlg->IsShow() )
// 	{
// 		posQShop = m_DlgZOrder.Find(pDlg);
// 		if( GetDialog("win_explorer")->IsShow() )
// 		{
// 			posMark = m_DlgZOrder.Find(GetDialog("win_explorer"));
// 			m_DlgZOrder.RemoveAt(posMark);
// 			m_DlgZOrder.InsertBefore(posQShop, GetDialog("win_explorer"));
// 		}
// 	}
// 
	pDlg = GetDialog("Win_Help_SearchResult");
	if( pDlg->IsShow() )
	{
		posQShop = m_DlgZOrder.Find(pDlg);
		if( GetDialog("win_explorer")->IsShow() )
		{
			posMark = m_DlgZOrder.Find(GetDialog("win_explorer"));
			m_DlgZOrder.RemoveAt(posMark);
			m_DlgZOrder.InsertBefore(posQShop, GetDialog("win_explorer"));
		}
	}
	
	int nDefaultHintFontSize = AUIManager::m_FontHint.nFontSize;
	if(m_pObjMouseOn && (strncmp(m_pObjMouseOn->GetName(), "Img_Buff", strlen("Img_Buff") ) == 0 || strncmp(m_pObjMouseOn->GetName(), "st_", strlen("st_") ) == 0  || strncmp(m_pObjMouseOn->GetName(), "St_", strlen("St_")) == 0))
		AUIManager::m_FontHint.nFontSize = 10;

	bval = AUILuaManager::Render();
	if( !bval ) return AUI_ReportError(__LINE__, __FILE__);

	AUIManager::m_FontHint.nFontSize = nDefaultHintFontSize;//pUIMan->SetHintFontSize(nDefaultHintFontSize);

	return true;
}

ACString FilterCoordString(const ACHAR* szText)
{
	EditBoxItemsSet ItemsSet;
	ACString strOrgText = UnmarshalEditBoxText(szText, ItemsSet);
	int nCount = ItemsSet.GetItemCount();
	
	if (nCount == 0)
		return ACString(szText);
	
	EditBoxItemMap::iterator it = ItemsSet.GetItemIterator();
	
	for (int i = 0; i < nCount; i++)
	{
		EditBoxItemBase* pItem = it->second;
		
		if (pItem->GetType() == enumEICoord)
		{
			ACString strName = pItem->GetName();
			ACString strInfo = pItem->GetInfo();
			
			BYTE  checknum = 0;
			BYTE  ch, cl;  // 校验位的高位和低位
			for (int i =0; i<strName.GetLength();  ++i)
			{
				checknum =  checknum^strName[i];
			}
			ch = (checknum >> 4) & 0x0F;
			cl = checknum & 0x0F;
			ch += 'w';
			cl += 'm';
			if (strName.IsEmpty())
			{
				ch = 'z';
				ch = 'x';
			}
			int checkcode = (cl << 16) | ch;
			int x,y,z,id, readcode;
			ACString strstrs;
			a_sscanf(strInfo, _AL("%d %d %d %d %d, %s"), &x, &y, &z, &id, &readcode, &strstrs);
			if (readcode!=checkcode)
				pItem->SetName(ACString(_AL(" ")));
		}
		
		++it;
	}
	
	return MarshalEditBoxText(strOrgText, ItemsSet);
}

void CECGameUIMan::AddChatMessage(const ACHAR *pszMsg, char cChannel, int idPlayer, const ACHAR *pszPlayer, 
								  char byFlag, char cEmotion, CECIvtrItem *pItem, sAchievement* pAchievement)
{
	struct _t
	{
		CECIvtrItem* p;
		sAchievement* s;

		_t(CECIvtrItem* _p, sAchievement* _s) : p(_p), s(_s) {}

		~_t()
		{
			delete p;
			delete s;
		}
	};

	_t tmp(pItem, pAchievement);
	ACString strOrg = FilterEmotionSet(pszMsg, cEmotion);
	// 单向好友的时候去掉时间信息
	if( byFlag != CHANNEL_FRIEND && byFlag != CHANNEL_FRIEND_RE )
	{
		ACHAR ch = 0xdbfe;
		int tIndex = strOrg.ReverseFind(ch);
		if (tIndex>=0 && (tIndex+2)<strOrg.GetLength())
		{
			strOrg.CutRight(3);
		}
	}

	if(!ISPLAYERID(idPlayer) || idPlayer == -1 || idPlayer == 0)
		UpdateTextCoord(strOrg);	

	pszMsg = strOrg;
	strOrg = FilterCoordColor(pszMsg, A3DCOLORRGBA(108, 251, 75,  255));
	pszMsg = strOrg;

	if(ISPLAYERID(idPlayer) && idPlayer != -1 && idPlayer != 0)
	{
		strOrg = FilterCoordString(pszMsg);
		pszMsg = strOrg;
	}

	if( PlayerIsBlack(idPlayer) )
		return;
	if( cChannel == GP_CHAT_SYSTEM && a_stricmp(pszMsg,  GetStringFromTable(800)) == 0 )
		return;

	CECHostPlayer *pHostPlayer = g_pGame->GetGameRun()->GetHostPlayer();
	if( byFlag == CHANNEL_FRIEND || byFlag == CHANNEL_FRIEND_RE )
	{
		AddFriendMessage(pszMsg, idPlayer, pszPlayer, byFlag, cEmotion);
		return;
	}
	else if( byFlag == CHANNEL_USERINFO )
	{
		CECFriendMan *pMan = pHostPlayer->GetFriendMan();
		if (pMan)
		{
			if( pszMsg[0] == 'R' )
			{
				if( pMan->GetFriendByID(idPlayer) )
					FriendAction(idPlayer, -1, CDlgFriendList::FRIEND_ACTION_INFO_REFRESH, 0);
			}
			else if( pszMsg[0] == 'L' )
				FriendAction(idPlayer, -1, CDlgFriendList::FRIEND_ACTION_INFO_LEVEL, a_atoi(pszMsg + 1));
			else if( pszMsg[0] == 'P' )
				FriendAction(idPlayer, -1, CDlgFriendList::FRIEND_ACTION_INFO_PROFESSION, a_atoi(pszMsg + 1));
			else if( pszMsg[0] == 'C' )
				FriendAction(idPlayer, -1, CDlgFriendList::FRIEND_ACTION_INFO_CULTIVATION, a_atoi(pszMsg + 1));
			else if( pszMsg[0] == 'A' )
				FriendAction(idPlayer, -1, CDlgFriendList::FRIEND_ACTION_INFO_AREA, a_atoi(pszMsg + 1));
		}
		return;
	}

	ACHAR *pszText;
	CDlgChat::CHAT_MSG msg;
	ACHAR szText[512], szMsg[512];
	CDlgChat *pChat1 = m_pDlgChat1;
	CDlgChat *pChat2 = m_pDlgChat2;
	EC_GAME_SETTING gs = g_pGame->GetConfigs()->GetGameSettings();
	EC_LOCAL_SETTING ls = g_pGame->GetConfigs()->GetLocalSettings();
	PAUITEXTAREA pShow1 = (PAUITEXTAREA)pChat1->GetDlgItem("Txt_Content");
	PAUITEXTAREA pShow2 = (PAUITEXTAREA)pChat2->GetDlgItem("Txt_Content");
	abase::vector<CDlgChat::CHAT_MSG> &vecChatMsg = m_pDlgChat->GetAllChatMsgs();

	if( cChannel == GP_CHAT_FARCRY && !idPlayer )
		msg.strMsg = ACString(_AL("^FF0096")) + pszMsg;
	else if( ISNPCID(idPlayer) )
		msg.strMsg = ACString(_AL("^C8FF64")) + pszMsg;
	else
		msg.strMsg = pszMsg;
	if( cChannel == GP_CHAT_FARCRY && !ISPLAYERID(idPlayer) )
	{
		cChannel = GP_CHAT_SYSTEM;
	}
	msg.cChannel = cChannel;

	int nMsgLen = a_strlen(pszMsg);
	// Booth Message
	if( cChannel == GP_CHAT_WHISPER && 
		pszMsg[nMsgLen - 2] == '!' &&
		pszMsg[nMsgLen - 1] == '#' )
	{
		if( pHostPlayer && pHostPlayer->GetBoothState()==2 )
			m_pDlgBooth1->AddBoothMessage(pszMsg);
		return;
	}

	if( cChannel == GP_CHAT_WHISPER &&
		(int)m_pDlgChatWhisper->GetData() == idPlayer )
	{
		pShow1 = (PAUITEXTAREA)m_pDlgChatWhisper->GetDlgItem("Txt_Chat");
		pszText = (ACHAR *)pShow1->GetText();

		if( a_strlen(pShow1->GetText()) > 0 )
			pShow1->AppendText(_AL("\r"));

		pShow1->AppendText(CDlgChat::m_pszColor[cChannel]);
		pShow1->AppendText(msg.strMsg);
		pShow1->ScrollToTop();
		pShow1->ScrollToBottom();
		if(	!m_pDlgChatWhisper->IsShow() )
			m_pDlgMinimizeBar[0]->FlashDialog(m_pDlgChatWhisper);
	}
	else
	{
		//国王喊话
        {
			if (idPlayer != 0 && idPlayer != -1 && ISPLAYERID(idPlayer) && pHostPlayer->GetKingdomKingId() == idPlayer)
			{
				ACString strKing;
				EditBoxItemBase item(enumEIImage);
				item.SetImageIndex(CECGameUIMan::ICONLIST_KINGDOM);
				item.SetImageFrame(0);
				strKing = (ACHAR)AUICOMMON_ITEM_CODE_START + item.Serialize();
				msg.strMsg = strKing + msg.strMsg;
			}
		}
		msg.pItem = pItem;
		msg.pAchievement = pAchievement;
		msg.nMsgIndex = CDlgChat::m_nMsgIndex++;
		tmp.p = NULL;
		tmp.s = NULL;
		vecChatMsg.push_back(msg);

		if( (int)vecChatMsg.size() > CECGAMEUIMAN_MAX_MSGS )
		{
			delete vecChatMsg.begin()->pItem;
			vecChatMsg.erase(vecChatMsg.begin());
		}

		int nActiveChannelSet = m_pDlgChat->GetActiveChannelSet();
		
		ACString strName;
		A3DCOLOR color;
		if (pItem)
			CDlgChat::TransformItemColorName(pItem, strName, color);
		else if (pAchievement)
			CDlgChat::TransformAchievementColorName(pAchievement, strName, color);

		if (cChannel==GP_CHAT_SPEAKER || cChannel==GP_CHAT_RUMOR)
		{
			PAUITEXTAREA pSpecial = (PAUITEXTAREA)GetDialog("Win_ChatSpecInfo")->GetDlgItem("Txt_Content");
			pSpecial->SetText(CDlgChat::m_pszColor[cChannel]);
			ACString strChannelIcon;
			EditBoxItemBase item(enumEIImage);
			item.SetImageIndex(CECGameUIMan::ICONLIST_CHANNEL);
			item.SetImageFrame(cChannel==GP_CHAT_SPEAKER ? 1:2);
			strChannelIcon = (ACHAR)AUICOMMON_ITEM_CODE_START + item.Serialize();
			pSpecial->AppendText(strChannelIcon);
			pSpecial->AppendText(msg.strMsg, msg.nMsgIndex, strName, color);
			PAUIOBJECT pObj = GetDialog("Win_ChatSpecInfo")->GetDlgItem("ShapeGfx");
			pObj->Show(true);
			pSpecial->SetData(0, "time");

			// Set achievement editbox item color.
			if (pItem == NULL && pAchievement != NULL)
			{
				EditBoxItemsSet* pItemSet = (EditBoxItemsSet*) &pSpecial->GetItemSet();
				EditBoxItemMap::iterator iter = pItemSet->GetItemIterator();

				while(!iter.is_eof())
				{
					EditBoxItemBase* pItem = iter->second;
					if (pItem->GetType() == enumEITask && pItem->GetMsgIndex() == msg.nMsgIndex)
					{
						pItem->SetName(strName);
						pItem->SetColor(color);

						//	Just for calling AUITextArea::BuildRenderCache() to refresh achievement color.
						pSpecial->SetFontAndColor(NULL, -1, pSpecial->GetColor());

						break;
					}

					++iter;
				}
			}
		}
		if (gs.bChannel[nActiveChannelSet][cChannel])
		{
			PAUITEXTAREA pShow	= pShow1;
			CDlgChat	*pChat	= pChat1;
			bool		bLocked = pChat->IsLocked();;
			PAUIOBJECT	pObjLock = m_pDlgChat->GetDlgItem("Btn_Lock");
			if (cChannel == GP_CHAT_DAMAGE		||
				cChannel == GP_CHAT_SYSTEM		||
				cChannel == GP_CHAT_BROADCAST	||
				cChannel == GP_CHAT_MISC		||
				(cChannel>=CLIENT_CHAT_MIN) && (cChannel<CLIENT_CHAT_MAX))
			{
				if (ls.local_chat.bChannel[cChannel])
				{
					pShow = pShow2;
					pChat = pChat2;
					bLocked = pChat2->IsLocked();
					pObjLock = pChat2->GetDlgItem("Btn_Lock");
				}
			}
			pszText = (ACHAR *)pShow->GetText();
			
			if( pShow->GetText()[0] )
				pShow->AppendText(_AL("\r"));
			pShow->AppendText(CDlgChat::m_pszColor[cChannel]);
			
			if (cChannel==GP_CHAT_SYSTEM || cChannel==GP_CHAT_SPEAKER || cChannel==GP_CHAT_RUMOR)
			{
				ACString strChannelIcon;
				EditBoxItemBase item(enumEIImage);
				item.SetImageIndex(CECGameUIMan::ICONLIST_CHANNEL);
				int indexImg;
				if (cChannel==GP_CHAT_SYSTEM)
					indexImg = 0;
				else if (cChannel==GP_CHAT_SPEAKER)
					indexImg = 1;
				else if (cChannel==GP_CHAT_RUMOR)
					indexImg = 2;
				item.SetImageFrame(indexImg);
				strChannelIcon = (ACHAR)AUICOMMON_ITEM_CODE_START + item.Serialize();
				pShow->AppendText(strChannelIcon);
			}
			else
			{
				pShow->AppendText(GetStringFromTable(13100+cChannel));
			}

			if (g_pGame->GetGameRun()->GetHostPlayer()->IsGM() &&
				GP_CHAT_WHISPER == cChannel)
			{
				ACString strMsg = msg.strMsg;
				// get type
				int nSymbolPos = strMsg.Find(GM_HELP_MSG_TYPE_BASE);
				if (nSymbolPos >= 0 && nSymbolPos < strMsg.GetLength())
				{
					strMsg = msg.strMsg.Left(nSymbolPos);
					strMsg += msg.strMsg.Right(msg.strMsg.GetLength()-2-nSymbolPos);
					msg.strMsg = strMsg;
				}
			}

			pShow->AppendText(msg.strMsg, msg.nMsgIndex, strName, color);

			// Set achievement editbox item color.
			if (pItem == NULL && pAchievement != NULL)
			{
				EditBoxItemsSet* pItemSet = (EditBoxItemsSet*) &pShow->GetItemSet();
				EditBoxItemMap::iterator iter = pItemSet->GetItemIterator();
				
				while(!iter.is_eof())
				{
					EditBoxItemBase* pItem = iter->second;
					if (pItem->GetType() == enumEITask && pItem->GetMsgIndex() == msg.nMsgIndex)
					{
						pItem->SetName(strName);
						pItem->SetColor(color);

						//	Just for calling AUITextArea::BuildRenderCache() to refresh achievement color.
						pShow->SetFontAndColor(NULL, -1, pShow->GetColor());

						break;
					}

					++iter;
				}
			}

			if( !bLocked )
			{
				pShow->ScrollToTop();
				pShow->ScrollToBottom();
			}
			if( bLocked )
				pObjLock->SetFlash(true);
			
			pChat->IncreaseMsgCount(1);
		}
	}

	if( cChannel == GP_CHAT_BROADCAST && byFlag == 0 )
		SetMarqueeMsg(pszMsg);
	else if( gs.bAutoReply && a_strlen(gs.szAutoReply) > 0 &&
		cChannel == GP_CHAT_WHISPER && idPlayer > 0 && pszPlayer )
	{
		if(byFlag == CHANNEL_NORMAL)
		{
			CECGameSession *pSession = g_pGame->GetGameSession();
			CECHostPlayer *pHost = g_pGame->GetGameRun()->GetHostPlayer();
			
			AUI_ConvertChatString(gs.szAutoReply, szText, false);
			pSession->SendPrivateChatData(pszPlayer, szText, CHANNEL_NORMALRE, 0, -1, 0, true);
			a_sprintf(szMsg, GetStringFromTable(264), pHost->GetName(), szText);
			AddChatMessage(szMsg, GP_CHAT_WHISPER);
		}
	}
	
	
	if (g_pGame->GetGameRun()->GetHostPlayer()->IsGM() &&
		GP_CHAT_WHISPER == cChannel)
	{
		// get original msg, 
		// in format : "&[playerName]& 悄悄对你说：[startSymbol][type][msg]"
		ACString strMsg = pszMsg;
		// get type
		int nSymbolPos = strMsg.Find(GM_HELP_MSG_TYPE_BASE);
		if (nSymbolPos < 0 || nSymbolPos >= strMsg.GetLength())
			return;
		ACHAR type = strMsg[nSymbolPos + 1];
		if (!TheGMHelpMsgArray::Instance()->FindByType(type, NULL))
			return;
		// get playerid
		int nPlayerID = idPlayer;
		if (nPlayerID <= 0)
		{
			if (!pszPlayer) return;

			CECGameRun *pGameRun = g_pGame->GetGameRun();
			nPlayerID = pGameRun->GetPlayerID(pszPlayer);
			if (nPlayerID <= 0) return;
		}

		// put to gmconsole dialog
		strMsg.CutLeft(nSymbolPos + 2);
		m_pDlgGMConsole->AddUserMessage(strMsg, type, nPlayerID);
	}
	
}

void CECGameUIMan::AddFriendMessage(const ACHAR *pszMsg,
	int idPlayer, const ACHAR *pszPlayer, BYTE byFlag, char cEmotion)
{
	int i;
	char szName[40];
	PAUIDIALOG pDlg;
	ACHAR szText[80];
	ACHAR szMsg[256];
	bool bWindow = false;

	AUI_ConvertChatString(pszPlayer, szText);

	ACString strName;
    if(CDlgFriendList::GetFriendRemark(idPlayer,strName))
	{
	    memset(szText,0,sizeof(szText));
        a_strcpy(szText,strName.GetBuffer(0));
	}
	
	AUI_ConvertChatString(pszMsg, szMsg);
	for( i = 0; i < CECGAMEUIMAN_MAX_CHATS; i++ )
	{
		sprintf(szName, "Win_FriendChat%d", i);
		pDlg = GetDialog(szName);
		if( (int)pDlg->GetData() == idPlayer )
		{
			PAUITEXTAREA pText = (PAUITEXTAREA)pDlg->GetDlgItem("Txt_Chat");

			if( a_strlen(pText->GetText()) > 0 )
				pText->AppendText(_AL("\r"));
			ACString strMsg = szMsg;
			ACString strTemp;
			ACString strTime;
			ACHAR ch = 0xdbfe;
			int tIndex = strMsg.ReverseFind(ch);
			if (tIndex>=0 && (tIndex+2)<strMsg.GetLength())
			{
				time_t t = (strMsg[tIndex+1]<<16) + strMsg[tIndex+2];
				tm tmTime = *localtime(&t);
				tm tmNow = g_pGame->GetServerLocalTime();
				if (tmNow.tm_mday == tmTime.tm_mday && tmNow.tm_mon == tmTime.tm_mon)
					strTime.Format(_AL("  %d:%02d"), tmTime.tm_hour, tmTime.tm_min);
				else 
					strTime.Format(_AL("  %d-%d %d:%02d"), tmTime.tm_mon+1, tmTime.tm_mday, tmTime.tm_hour, tmTime.tm_min);
				strMsg.CutRight(3);
			}
			
			ch = 0xdbff;
			ACString strMark = ACString(ch,2);
			int iQk1 = strMsg.Find(_AL(":")+strMark);
			int iQk2 = strMsg.Find(GetStringFromTable(8493)+strMark);
			int iQk = 0;
			if (iQk1<0)
				if (iQk2<0)
					iQk = -1;
				else
					iQk = iQk2;
			else
				if (iQk2<0)
					iQk = iQk1;
				else
					iQk = iQk1>iQk2 ? iQk2 : iQk1;
			if (iQk>=0 && 0 <= strMsg.Find(_AL("#N")))
			{
				ACString str;
				strMsg = strMsg.Mid(iQk+3);
				int iLeft = strMsg.Find(_AL("#N"));
				
				while(iLeft>=0)
				{
					strMsg[iLeft] = _AL('%');
					strMsg[iLeft+1] = _AL('s');
					strTemp = strMsg;
					strMsg.Format(strTemp, szText);
					iLeft = strMsg.Find(_AL("#N"));
				}

				iLeft = strMsg.Find(_AL("#n"));
				while(iLeft>=0)
				{
					strMsg[iLeft] = _AL('%');
					strMsg[iLeft+1] = _AL('s');
					strTemp = strMsg;
					strMsg.Format(strTemp, g_pGame->GetGameRun()->GetHostPlayer()->GetName());
					iLeft = strMsg.Find(_AL("#n"));
				}
				str = ACString(_AL("^80ffff")) + GetStringFromTable(8494) + strTime + _AL("\r^ffff80") + strMsg;
				pText->AppendText(str);
			} 
			else
			{
				pText->AppendText(_AL("^80ffff"));
				pText->AppendText(szText);
				pText->AppendText(strTime);
				pText->AppendText(_AL("\r^ffff80"));
				pText->AppendText(strMsg);
			}
			if( m_bSaveHistory )
				AddHistory(idPlayer, time(NULL), pszPlayer, strMsg);


			pText->ScrollToTop();
			pText->ScrollToBottom();
			bWindow = true;
			if( !pDlg->IsShow() )
				m_pDlgMinimizeBar[0]->FlashDialog(pDlg);

			break;
		}
	}

	if( !bWindow && byFlag == CHANNEL_FRIEND )
	{
		CECFriendMan *pMan = g_pGame->GetGameRun()->GetHostPlayer()->GetFriendMan();
		if (!pMan)
		{
			return;
		}
		CECFriendMan::FRIEND *pFriend = pMan->GetFriendByID(idPlayer);
		if( pFriend )
		{
			ACString strMsg = szMsg;
			ACString strText;
			ACString strTemp;
			ACString strTime;
			ACHAR ch = 0xdbfe;
			int tIndex = strMsg.ReverseFind(ch);
			if (tIndex>=0 && (tIndex+2)<strMsg.GetLength())
			{
				time_t t = (strMsg[tIndex+1]<<16) + strMsg[tIndex+2];
				tm tmTime = *localtime(&t);
				tm tmNow = g_pGame->GetServerLocalTime();
				if (tmNow.tm_mday == tmTime.tm_mday && tmNow.tm_mon == tmTime.tm_mon)
					strTime.Format(_AL("  %d:%02d"), tmTime.tm_hour, tmTime.tm_min);
				else 
					strTime.Format(_AL("  %d-%d %d:%02d"), tmTime.tm_mon+1, tmTime.tm_mday, tmTime.tm_hour, tmTime.tm_min);
				strMsg.CutRight(3);
			}

			ch = 0xdbff;
			ACString strMark = ACString(ch,2);
			int iQk1 = strMsg.Find(_AL(":")+strMark);
			int iQk2 = strMsg.Find(GetStringFromTable(8493)+strMark);
			int iQk = 0;
			if (iQk1<0)
				if (iQk2<0)
					iQk = -1;
				else
					iQk = iQk2;
			else
				if (iQk2<0)
					iQk = iQk1;
				else
					iQk = iQk1>iQk2 ? iQk2 : iQk1;
			if(iQk>=0 && 0 <= strMsg.Find(_AL("#N")))
			{
				strMsg = strMsg.Mid(iQk+3);
				int iLeft = strMsg.Find(_AL("#N"));
				while(iLeft>=0)
				{
					strMsg[iLeft] = _AL('%');
					strMsg[iLeft+1] = _AL('s');
					strTemp = strMsg;
					strMsg.Format(strTemp, szText);
					iLeft = strMsg.Find(_AL("#N"));
				}
				iLeft = strMsg.Find(_AL("#n"));
				while(iLeft>=0)
				{
					strMsg[iLeft] = _AL('%');
					strMsg[iLeft+1] = _AL('s');
					strTemp = strMsg;
					strMsg.Format(strTemp, g_pGame->GetGameRun()->GetHostPlayer()->GetName());
					iLeft = strMsg.Find(_AL("#n"));
				}
				strText = ACString(_AL("^80ffff")) + GetStringFromTable(8494) + strTime + _AL("\r^ffff80") + strMsg;
			}
			else
				strText = ACString(_AL("^80ffff")) + ACString(szText) + strTime + _AL("\r^ffff80") + ACString(strMsg);
			
			if( m_bSaveHistory )
				AddHistory(idPlayer, time(NULL), pszPlayer, strMsg);
			AddInformation(CDlgInfo::INFO_FRIEND, "Game_FriendMsg", strText, 0xFFFFFFF, idPlayer, 0, 0);
		}
		else
		{
			CECStringTab* pStrTab = g_pGame->GetFixedMsgTab();
			ACString str;
			ACHAR szText[80], szText1[1024];
			AUI_ConvertChatString(pszPlayer, szText);
			AUI_ConvertChatString(pszMsg, szText1, false);
			str.Format(pStrTab->GetWideString(FIXMSG_PRIVATECHAT1), szText, szText1);
		
			AddChatMessage(str, GP_CHAT_WHISPER, idPlayer, pszPlayer, CHANNEL_NORMAL);
			return;
		}
	}

	// Auto reply.
	if( m_bAutoReply && byFlag == CHANNEL_FRIEND )
	{
		ACString strReply = GetStringFromTable(560);
		CECHostPlayer *pHost = g_pGame->GetGameRun()->GetHostPlayer();

		if( m_bSaveHistory )
			AddHistory(pHost->GetCharacterID(), time(NULL), pHost->GetName(), strReply);
		g_pGame->GetGameSession()->SendPrivateChatData(
			pszPlayer, strReply, CHANNEL_FRIEND_RE, idPlayer, -1, 0, true);
		AddFriendMessage(strReply, idPlayer, pHost->GetName(), CHANNEL_FRIEND_RE);
	}
}

void CECGameUIMan::PopupPlayerContextMenu(int idPlayer, int x, int y)
{
	CECHostPlayer *pHost = g_pGame->GetGameRun()->GetHostPlayer();
	int battleType = pHost->GetBattleType();
	if ( battleType!=BATTLE_TYPE_ARENA && battleType!=BATTLE_TYPE_ARENAREBORN && battleType!=BATTLE_TYPE_CRSSVR )
	{
		PAUIDIALOG pDlg = NULL;
		
		idPlayer = idPlayer > 0 ? idPlayer : pHost->GetClickedMan();
		
		if( idPlayer > 0 && ISNPCID(idPlayer) )
		{
			pDlg = m_pDlgNPC;
			if( !pDlg->IsShow() ) pDlg->Show(true);
		}
		else if( pHost->GetCharacterID() != idPlayer )
		{
			pDlg = m_pDlgQuickAction;
			pDlg->SetData(idPlayer);
			pDlg->Show(true);
			BringWindowToTop(pDlg);
		}
		
		// old : if( pDlg ) pDlg->SetPos(x, y);
		if( pDlg ) pDlg->SetPosEx(x, y);
	}
}

//	Add text string to console window
void CECGameUIMan::AddConsoleLine(const ACHAR* szText, A3DCOLOR col)
{
	AUIDialog* pDlg = GetDialog(l_aDlgNames[DLG_CONSOLE]);
	if (!pDlg)
		return;

	AUIConsole* pConsole = (AUIConsole*)pDlg->GetDlgItem("Command_Edit");
	if (!pConsole)
		return;

	pConsole->AddLine(szText, col);
}

bool CECGameUIMan::UpdateTask()
{
	m_pDlgNPC->AutoPopup();
	CDlgTaskList *pDlgTaskList = (CDlgTaskList*)GetDialog("Win_QuestList");
	if (pDlgTaskList->IsShow())
	{
		pDlgTaskList->Show(true);
	}
	CDlgStorageTaskCross *pDlgTaskStorageCross = (CDlgStorageTaskCross*)GetDialog("Win_QuestKf");
	if (pDlgTaskStorageCross->IsShow())
	{
		pDlgTaskStorageCross->OnCommand_ChangeMap("");
		pDlgTaskStorageCross->UpdateTask();
	}
	if (m_pDlgTask->IsShow())
	{
		m_pDlgTask->UpdateTask();	//更新相应任务列表
	}
	return true;
}

void CECGameUIMan::PopUpMonsterInfo(unsigned long ulTask, unsigned long ulMonsterID)
{
	//  若此怪为任务怪，则显示中间提示文字
	
	Task_State_info tsi;
	CECTaskInterface *pTask = g_pGame->GetGameRun()->GetHostPlayer()->GetTaskInterface();
	ATaskTemplMan *pMan = g_pGame->GetTaskTemplateMan();

	bool bDisplayCnt = ((PAUICHECKBOX)GetDialog("Win_Quest")->GetDlgItem("Chk_QuestNum"))->IsChecked();
	if(bDisplayCnt)
	{
		ATaskTempl *pTemp = pMan->GetTaskTemplByID(ulTask);
		if(!pTemp)
			return;

		pTask->GetTaskStateInfo(ulTask, &tsi);

		int j;
		// 杀怪
		for (j = 0; j < MAX_MONSTER_WANTED; j++)
		{
			if(tsi.m_MonsterWanted[j].m_ulMonsterId != ulMonsterID)
				continue;

			// get npc name by id

			DATA_TYPE DataType = g_pGame->GetElementDataMan()->get_data_type(ulMonsterID, ID_SPACE_ESSENCE);
			if (DataType == DT_MONSTER_ESSENCE)
			{
				MONSTER_ESSENCE* pMonster = (MONSTER_ESSENCE *)g_pGame->GetElementDataMan()->get_data_ptr(ulMonsterID, ID_SPACE_ESSENCE, DataType);

				ACString strText;
				strText.Format(_AL("%s: %s(%d/%d)"), pTemp->GetName(), pMonster->name, tsi.m_MonsterWanted[j].m_ulMonstersKilled, tsi.m_MonsterWanted[j].m_ulMonstersToKill);
				
				CDlgPKMsg *pDlgPKMsg = (CDlgPKMsg*)GetDialog("Win_Pkmsg");
				if(pDlgPKMsg)
						pDlgPKMsg->AddMessage(strText);
			}
		}
	}
}

void CECGameUIMan::PopupInviteGroupMenu(int idLeader, const ACHAR* szLeaderName, 
							int iTeamSeq, int nMode, int iFamilyId, int iGuildId)
{
	if( !ISPLAYERID(idLeader) || PlayerIsBlack(idLeader) )
		return;

	CECHostPlayer *pHostPlayer = g_pGame->GetGameRun()->GetHostPlayer();

	ACString strName = szLeaderName;
	if (pHostPlayer->IsAutoTeam(idLeader, iGuildId, iFamilyId, strName.GetBuffer(0)))
	{
		g_pGame->GetGameSession()->c2s_CmdTeamAgreeInvite(idLeader, iTeamSeq);
	}
	else
	{
		ACString strMsg;
		strMsg.Format(GetStringFromTable(204), szLeaderName, GetStringFromTable(208 + nMode));
		AddInformation(CDlgInfo::INFO_SYSTEM, "Game_InviteGroup", strMsg, 20000, idLeader, iTeamSeq, 0);
	}
}

void CECGameUIMan::PopupTradeInviteMenu(int idPlayer, DWORD dwHandle)
{
	if( PlayerIsBlack(idPlayer) )
		return;

	ACHAR szName[40];
	ACString strMsg;
	const ACHAR *pszName = g_pGame->GetGameRun()->GetPlayerName(idPlayer, true);

	AUI_ConvertChatString(pszName, szName);
	strMsg.Format(GetStringFromTable(213), szName);
	AddInformation(CDlgInfo::INFO_SYSTEM, "Game_InviteTrade",
		strMsg, 30000, idPlayer, dwHandle, 0);
}

void CECGameUIMan::PopupTradeResponseMenu(int idPlayer, int idTrade, bool bAgree)
{
	if( bAgree )
	{
		ACHAR szText[256];
		m_pDlgTrade->GetDlgItem("Txt_Name1")->SetAlign(AUIFRAME_ALIGN_CENTER);
		m_pDlgTrade->GetDlgItem("Txt_Name2")->SetAlign(AUIFRAME_ALIGN_CENTER);
		CECPlayerMan *pPlayerMan = g_pGame->GetGameRun()->GetWorld()->GetPlayerMan();
		CECPlayer *pPlayer = (CECPlayer *)pPlayerMan->GetPlayer(idPlayer);
		// GM隐身情况下会pPlayer为空
		if (!pPlayer)
			a_sprintf(szText, GetStringFromTable(769), 150, g_pGame->GetGameRun()->GetPlayerName(idPlayer, true));
		else
			a_sprintf(szText, GetStringFromTable(769), pPlayer->GetBasicProps().iLevel, g_pGame->GetGameRun()->GetPlayerName(idPlayer, true));
		m_pDlgTrade->GetDlgItem("Txt_Name1")->SetText(szText);
		a_sprintf(szText, GetStringFromTable(769), g_pGame->GetGameRun()->GetHostPlayer()->GetBasicProps().iLevel, g_pGame->GetGameRun()->GetHostPlayer()->GetName());
		m_pDlgTrade->GetDlgItem("Txt_Name2")->SetText(szText);

		m_pDlgTrade->Show(true);
		m_pDlgTrade->SetData(idPlayer);
		m_pDlgTrade->SetDataPtr((void *)idTrade,"int");
		m_pDlgTrade->m_pBtnLock->Show(true);
		m_pDlgTrade->m_pBtnLock->Enable(true);
		m_pDlgTrade->m_pBtnLock2->Show(true);
		m_pDlgTrade->m_pBtnLock2->Enable(true);
		m_pDlgTrade->m_pBtnConfirm->Show(false);
		m_pDlgTrade->m_pBtnConfirm2->Show(false);

		m_pDlgInventory->Show(true);

		m_idTrade = idTrade;
	}
	else
	{
		if( idPlayer > 0 )
		{
			ACString strMsg;
			const ACHAR *pszName = g_pGame->GetGameRun()->GetPlayerName(idPlayer, true);

			strMsg.Format(GetStringFromTable(214), pszName);
			AddChatMessage(strMsg, GP_CHAT_MISC);
		}
		else
			AddChatMessage(GetStringFromTable(221), GP_CHAT_MISC);
	}
}

void CECGameUIMan::PopupFriendInviteMenu(
	int idPlayer, const ACHAR *pszName, DWORD dwHandle)
{
	if( PlayerIsBlack(idPlayer) )
		return;

	ACString strMsg;

	strMsg.Format(GetStringFromTable(534), pszName);
	AddInformation(CDlgInfo::INFO_SYSTEM, "Game_InviteFriend", strMsg, 20000, idPlayer, dwHandle, 0);
}

void CECGameUIMan::TradeAction(int idPlayer, int idTrade, int idAction, int nCode)
{
	m_pDlgTrade->TradeAction(idPlayer, idTrade, idAction, nCode);
}

bool CECGameUIMan::UpdateHint()
{
	PAUIDIALOG pDlg = m_pDlgMouseOn;
	PAUIOBJECT pObj = m_pObjMouseOn;
	if( !pDlg || !pObj ) return true;

	CECHostPlayer *pHost = g_pGame->GetGameRun()->GetHostPlayer();

	if( 0 == stricmp(pDlg->GetName(), "Win_Inventory") ||
		0 == stricmp(pDlg->GetName(), "Win_Garden_Inventory") ||
		0 == stricmp(pDlg->GetName(), "Win_InventoryPet") ||
		0 == stricmp(pDlg->GetName(), "Win_Character") ||
		0 == stricmp(pDlg->GetName(), "Win_Shop") ||
		0 == stricmp(pDlg->GetName(), "Win_Trade") ||
		0 == stricmp(pDlg->GetName(), "Win_Storage") ||
		0 == stricmp(pDlg->GetName(), "Win_EPEquip") ||
		0 == stricmp(pDlg->GetName(), "Win_PetEquip")||
		0 == stricmp(pDlg->GetName(), "Win_Inventory_Costume")||
		0 == stricmp(pDlg->GetName(), "Win_Inventory_Vechile")||
		0 == stricmp(pDlg->GetName(), "Win_Shop_Withdraw") ||
		0 == stricmp(pDlg->GetName(), "Win_Equip_Rune") ||
		0 == stricmp(pDlg->GetName(), "Win_ResetSkillPart") ||
		0 == stricmp(pDlg->GetName(), "Win_ResetSkillAll") ||
		0 == stricmp(pDlg->GetName(), "Win_ResetTalent") )
	{
		if( pObj->GetDataPtr("ptr_CECIvtrItem") )
		{
			int iDescType = CECIvtrItem::DESC_NORMAL;
			if (m_nCursorState == CURSOR_STATE_REPAIRING && 0 == stricmp(pDlg->GetName(), "Win_Inventory"))
				iDescType = CECIvtrItem::DESC_REPAIR;

			CECIvtrItem *pItem = (CECIvtrItem *)pObj->GetDataPtr("ptr_CECIvtrItem");
			if( 0 == stricmp(pDlg->GetName(), "Win_Storage") ||
				0 == stricmp(pDlg->GetName(), "Win_Inventory") )
			{
				pItem->SetPriceScale(CECIvtrItem::SCALE_SELL, PLAYER_PRICE_SCALE);
			}

			const wchar_t *pszHint = pItem->GetDesc(iDescType, pHost->GetEquipment());
			if( pszHint )
			{
				AUICTranslate trans;
				AWString strHint = trans.Translate(pszHint);

				if( (pItem->IsEquipment() || pItem->GetClassID() == CECIvtrItem::ICID_PET_ARMOR) &&
					(0 == stricmp(pDlg->GetName(), "Win_Inventory") ||
					0 == stricmp(pDlg->GetName(), "Win_Shop") ||
					0 == stricmp(pDlg->GetName(), "Win_Trade") ||
					0 == stricmp(pDlg->GetName(), "Win_Storage") ||
					0 == stricmp(pDlg->GetName(), "Win_EPEquip") ||
					0 == stricmp(pDlg->GetName(), "Win_PetEquip") && GetDialog("Win_EPEquip")->IsShow() ) ||
					pItem->GetClassID() == CECIvtrItem::ICID_PET_BEDGE && 0 == stricmp(pDlg->GetName(), "Win_EPEquip") )
				{
					strHint += GetCompareEquipDesc(pItem);
				}
				if (g_pGame->GetConfigs()->IsItemIDShown())
				{
					ACString strId;
					strId.Format(_AL("ID:%d\r"), pItem->GetTemplateID());
					strHint = strId + strHint;
				}
				pObj->SetHint(strHint);
			}
			else
			{
				pObj->SetHint(_AL(""));
				if( 0 == stricmp(pDlg->GetName(), "Win_Inventory") ||
					0 == stricmp(pDlg->GetName(), "Win_Character") ||
					0 == stricmp(pDlg->GetName(), "Win_Inventory_Vechile") )
				{
					int nPack, nSlot;
					if( strstr(pObj->GetName(), "Item_") )
					{
						nPack = IVTRTYPE_PACK;
						nSlot = atoi(pObj->GetName() + strlen("Item_"));
					}
					else if( 0 == stricmp(pDlg->GetName(), "Win_Inventory_Vechile") )
					{
						EC_LOCAL_SETTING setting = g_pGame->GetConfigs()->GetLocalSettings();
						if( 0 == stricmp(pObj->GetName(), "Img_Vechile01") )
						{
							nPack = setting.nRecordMountType;
							nSlot = setting.nRecordMountID;
						}
						else if(0 == stricmp(pObj->GetName(), "Img_Vechile02"))
						{
							nPack = IVTRTYPE_EQUIPPACK;
							nSlot = EQUIPIVTR_WING;
						}
						else
						{
							nPack = IVTRTYPE_MOUNTWING;
							nSlot = atoi(pObj->GetName() + strlen("Img_Item"))-1;
						}

					}
					else
					{
						nPack = IVTRTYPE_EQUIPPACK;
						nSlot = atoi(pObj->GetName() + strlen("Equip_"));
					}
					int nSlotInv = 0;
					if( 0 == stricmp(pDlg->GetName(), "Win_Storage") )
						nSlotInv = m_pDlgStorage->GetFirstItem();
					else if( 0 == stricmp(pDlg->GetName(), "Win_Inventory") )
						nSlotInv = m_pDlgInventory->GetFirstItem();
					pHost->GetIvtrItemDetailData(nPack, nSlot + nSlotInv);
					pItem->SetPriceScale(CECIvtrItem::SCALE_SELL, PLAYER_PRICE_SCALE);
				}
				else if( 0 == stricmp(pDlg->GetName(), "Win_Shop")
					|| 0 == stricmp(pDlg->GetName(), "Win_Produce") )
				{
					pItem->GetDetailDataFromLocal();
				}
			}
			if (0 == stricmp(pDlg->GetName(), "Win_Inventory"))
			{
				int nSlot = atoi(pObj->GetName() + strlen("Item_"));
				nSlot += m_pDlgInventory->GetFirstItem();
				m_pDlgInventory->EraseEmphasize(nSlot);
			}
		}
		else
		{
			if( 0 == stricmp(pDlg->GetName(), "Win_Inventory") || 0 == stricmp(pDlg->GetName(), "Win_Storage"))
			{
				if( strstr(pObj->GetName(), "Item_") )
					pObj->SetHint(_AL(""));
			}
			else if( 0 == stricmp(pDlg->GetName(), "Win_Inventory_Costume") ||
				0 == stricmp(pDlg->GetName(), "Win_Inventory_Vechile"))
			{
				if( strstr(pObj->GetName(), "Img_Item") )
				{
					pObj->SetHint(_AL(""));
				}
				if( 0 == stricmp(pDlg->GetName(), "Win_Inventory_Vechile") )
				{
					if( 0 == stricmp(pObj->GetName(), "Img_Vechile01") )
					{
						pObj->SetHint(GetStringFromTable(17604));
					}
					else if(0 == stricmp(pObj->GetName(), "Img_Vechile02"))
					{
						pObj->SetHint(GetStringFromTable(17605));
					}
				}

			}
			else if( 0 == stricmp(pDlg->GetName(), "Win_Character") )
			{
				if( strstr(pObj->GetName(), "Equip_") && stricmp(pObj->GetName(), "Equip_Pet") == 0 )
					pObj->SetHint(GetStringFromTable(360));
			}
			else if( 0 == stricmp(pDlg->GetName(), "Win_EPEquip"))
			{
				if( strstr(pObj->GetName(), "Equip_") )
				{
					if( stricmp(pObj->GetName(), "Equip_Pet") == 0 )
						pObj->SetHint(GetStringFromTable(360));
					else
					{
						int nIndex = atoi(pObj->GetName() + strlen("Equip_"));
						pObj->SetHint(GetStringFromTable(310 + nIndex));
					}
				}
			}
			else if( 0 == stricmp(pDlg->GetName(), "Win_PetEquip") )
			{
				if( strstr(pObj->GetName(), "Img_Equip") )
				{
					int nIndex = atoi(pObj->GetName() + strlen("Img_Equip")) - 1;
					pObj->SetHint(GetStringFromTable(380 + nIndex));
				}
			}
			else if( 0 == stricmp(pDlg->GetName(), "Win_Shop_Withdraw") )
			{
				if( strstr(pObj->GetName(), "Img_Sell") || strstr(pObj->GetName(), "Img_Withdraw") )
				{
					pObj->SetHint(_AL(""));
				}
			}
		}
	}
	else if( (strstr(pDlg->GetName(), "Win_QuickBar") && strstr(pObj->GetName(), "Item_"))
		|| (0 == stricmp(pDlg->GetName(), "Win_Action") && strstr(pObj->GetName(), "Cmd_")) )
	{
		if( pObj->GetDataPtr("ptr_CECShortcut") )
		{
			CECShortcut *pSC = (CECShortcut *)pObj->GetDataPtr("ptr_CECShortcut");
			const wchar_t *pszHint = pSC->GetDesc();
			if( pszHint)
			{
				AUICTranslate trans;
				AWString strHint = trans.Translate(pszHint);

				pObj->SetHint(strHint);
			}
			else
				pObj->SetHint(_AL(""));
		}
		else
			pObj->SetHint(_AL(""));
	}
	else if (strstr(pDlg->GetName(), "Win_Inventory_CostumeQuick") &&
		(strstr(pObj->GetName(), "Img_Hat") || strstr(pObj->GetName(), "Img_cloth") || strstr(pObj->GetName(), "Img_Shoe")) )
	{
		int idTid = pObj->GetData();
		if (idTid>0)
		{
			CECInventory *pInventory = g_pGame->GetGameRun()->GetHostPlayer()->GetFashionPack();
			for( int i = 0; i < 30; i++ )
			{
				CECIvtrItem *pItem;
				if (pItem = pInventory->GetItem(i))
				{
					if (pItem->GetTemplateID() == idTid)
					{
						const wchar_t *pszHint = pItem->GetDesc(CECIvtrItem::DESC_NORMAL, pHost->GetEquipment());
						if( pszHint )
						{
							AUICTranslate trans;
							AWString strHint = trans.Translate(pszHint);
							pObj->SetHint(strHint);
						}
						else
							pObj->SetHint(pszHint);
						break;
					}
				}
			}
		}
	}
	else if (strstr(pDlg->GetName(), "Win_DrugEnchanting") && (strstr(pObj->GetName(), "Item_1")))
	{
		int nIndex = pObj->GetData();
		CECInventory *pInventory = g_pGame->GetGameRun()->GetHostPlayer()->GetPack();
		if(pInventory)
		{
			CECIvtrItem *pItem;
			if (pItem = pInventory->GetItem(nIndex))
			{
				const wchar_t *pszHint = pItem->GetDesc(CECIvtrItem::DESC_NORMAL);
				if( pszHint )
				{
					AUICTranslate trans;
					AWString strHint = trans.Translate(pszHint);
					pObj->SetHint(strHint);
				}
				else
				{
					pObj->SetHint(pszHint);
				}
			}
		}
	}
	else if (strstr(pDlg->GetName(), "Win_Activity_Min") && (strstr(pObj->GetName(), "Btn_Max")))	// 2012-11-6 add 添加移入移出判断 by zy
	{
		AWString strHint;
		if(pObj->IsEnabled())
		{
			 strHint = GetStringFromTable(17201);
		}
		else
		{
			strHint = GetStringFromTable(17202);
		}
		pObj->SetHint(strHint);
	}
	return true;
}

void CECGameUIMan::ShowErrorMsg(const ACHAR *pszMsg, const char *pszName)
{
	MessageBox(pszName, pszMsg, MB_OK, A3DCOLORRGBA(255, 255, 255, 160));
}

bool CECGameUIMan::UIControlCursor()
{
	if( m_nCursorState != CURSOR_STATE_NORMAL )
		return true;
	else
	{
		if( m_pDlgMouseOn && (m_pDlgMouseOn->GetAlpha() > 0 || m_pObjMouseOn) )
			return true;
		else
			return false;
	}
}

CECShortcutSet * CECGameUIMan::GetSCSByDlg(const char *pszDlg)
{
	CECShortcutSet *pSCS = NULL;
	CECHostPlayer *pHost = g_pGame->GetGameRun()->GetHostPlayer();

// 	if( atoi(pszDlg + strlen("Win_QuickBar")) == 10 )
// 	{
// 		if( CDlgQuickBar::GetCurPanel10() != 1 &&
// 			(stricmp(pszDlg, "Win_QuickBar10V_1") == 0 ||
// 			stricmp(pszDlg, "Win_QuickBar10H_1") == 0) )
// 		{
// 			int nCurPanel10 = CDlgQuickBar::GetCurPanel10();
// 			pSCS = pHost->GetShortcutSet1(nCurPanel10 - 1);
// 		}
// 		else
// 		{
// 			int nPanel = atoi(pszDlg + strlen("Win_QuickBar10V_"));
// 			pSCS = pHost->GetShortcutSet1(nPanel - 1);
// 		}
// 	}
// 	else
	{
		if( stricmp(pszDlg, "Win_QuickBar8H_1") == 0 )
		{
			pSCS = pHost->GetShortcutSet2(0);
		}
		else if( stricmp(pszDlg, "Win_QuickBar8H_2") == 0 )
		{
			pSCS = pHost->GetShortcutSet1(0);
		}
		else if( stricmp(pszDlg, "Win_QuickBar8H_3") == 0 )
		{
			pSCS = pHost->GetShortcutSet1(1);
		}
		else if( stricmp(pszDlg, "Win_QuickBar8H_4") == 0 )
		{
			pSCS = pHost->GetShortcutSet2(1);
		}
		else if(stricmp(pszDlg, "Win_QuickBar10V_1") == 0 )
		{
		    pSCS = pHost->GetShortcutSet3(0);
		}
		else if(stricmp(pszDlg, "Win_QuickBar10V_2") == 0 )
		{
			pSCS = pHost->GetShortcutSet3(1);
		}
		else if(stricmp(pszDlg, "Win_QuickBar12C1") == 0 ||
			    stricmp(pszDlg, "Win_QuickBar12C2") == 0 ||
			    stricmp(pszDlg, "Win_QuickBar12C3") == 0 ||
			    stricmp(pszDlg, "Win_QuickBar12C4") == 0 )
		{
			pSCS = pHost->GetShortcutSet3(2);
		}
		else if(stricmp(pszDlg, "Win_QuickBar12C5") == 0 ||
			    stricmp(pszDlg, "Win_QuickBar12C6") == 0 ||
			    stricmp(pszDlg, "Win_QuickBar12C7") == 0 ||
			    stricmp(pszDlg, "Win_QuickBar12C8") == 0 )
		{
			pSCS = pHost->GetShortcutSet3(3);
		}
	}

	return pSCS;
}

bool CECGameUIMan::UpdateTeach(int nPage)
{
//	return m_pDlgTeach->UpdateTeach(nPage);
	return true;
}

int CECGameUIMan::GetItemCount(int idItem, CECInventory *pPack)
{
	int i, nCount = 0;
	CECIvtrItem *pItem;
	CECInventory *pInventory = pPack ? pPack : g_pGame->
		GetGameRun()->GetHostPlayer()->GetPack(IVTRTYPE_PACK);

	for( i = 0; i < pInventory->GetSize(); i++ )
	{
		pItem = pInventory->GetItem(i);
		if( pItem && pItem->GetTemplateID() == idItem )
			nCount += pItem->GetCount();
	}

	return nCount;
}

void CECGameUIMan::ServiceNotify(int idService, void *pData)
{
	CECHostPlayer *pHost = g_pGame->GetGameRun()->GetHostPlayer();

	if( idService == NOTIFY_PRODUCE_START )
	{
		PAUIPROGRESS pProgress = NULL;
		if( m_pDlgProduce->IsShow() )
			pProgress = m_pDlgProduce->m_pPrgs_1;
		else if( m_pDlgPetFactory->IsShow() )
			pProgress = m_pDlgPetFactory->m_pPrgs_1;
		else if( m_pDlgSoulCombine->IsShow() )
			pProgress = m_pDlgSoulCombine->m_pPrgs_1;
		if (pProgress)
		{
			DWORD dwMSecs = ((S2C::cmd_produce_start *)pData)->use_time * 50;
			pProgress->SetData(dwMSecs);
			pProgress->SetAutoProgress(dwMSecs, 0, AUIPROGRESS_MAX);
		}
	}
	else if( idService == NOTIFY_PRODUCE_END_ONE )
	{
		if( m_pDlgProduce->IsShow() )
			m_pDlgProduce->ProduceOne();
		else if (m_pDlgPetFactory->IsShow())
			m_pDlgPetFactory->ProduceOne();
		else if( m_pDlgSoulCombine->IsShow() )
			m_pDlgSoulCombine->ProduceOne();

	}
	else if( idService == NOTIFY_PRODUCE_END )
	{
		if( m_pDlgProduce->IsShow() )
			m_pDlgProduce->ProduceEnd();
		else if( m_pDlgPetFactory->IsShow() )
			m_pDlgPetFactory->ProduceEnd();
		else if( m_pDlgSoulCombine->IsShow() )
			m_pDlgSoulCombine->ProduceEnd();
	}
	else if( idService == NOTIFY_PRODUCE_NULL )
	{
		if( m_pDlgProduce->IsShow() )
			m_pDlgProduce->ProduceOne();
		else if( m_pDlgPetFactory->IsShow() )
			m_pDlgPetFactory->ProduceOne();
		else if( m_pDlgSoulCombine->IsShow() )
			m_pDlgSoulCombine->ProduceOne();
		S2C::cmd_produce_null* pCmd = (S2C::cmd_produce_null*)pData;
		DATA_TYPE datatype;
		const void *dataptr = g_pGame->GetElementDataMan()->get_data_ptr(pCmd->idRecipe, ID_SPACE_RECIPE, datatype);
		if( datatype != DT_INVALID)
		{
			ACString strText;
			strText.Format(GetStringFromTable(616), ((RECIPE_ESSENCE*)dataptr)->name );
			AddChatMessage(strText, GP_CHAT_MISC);
		}
	}
// 	else if( idService == NOTIFY_SPLIT_START )
// 	{
// 		PAUIPROGRESS pProgress = m_pDlgSplit->m_pPrgs_1;
// 		DWORD dwMSecs = ((S2C::cmd_decompose_start *)pData)->use_time * 50;
// 
// 		pProgress->SetAutoProgress(dwMSecs, 0, AUIPROGRESS_MAX);
// 	}
// 	else if( idService == NOTIFY_SPLIT_END )
// 	{
// 		PAUIIMAGEPICTURE pImage;
// 		PAUIPROGRESS pProgress = m_pDlgSplit->m_pPrgs_1;
// 
// 		pImage = m_pDlgSplit->m_pImgItem_a;
// 		pImage->SetCover(NULL, -1);
// 		pImage->SetDataPtr(NULL);
// 
// 		pImage = m_pDlgSplit->m_pImgItem_b;
// 		pImage->SetCover(NULL, -1);
// 		pImage->SetDataPtr(NULL);
// 
// 		pProgress->SetData(0);
// 		pProgress->SetAutoProgress(0, 0, 0);
// 
// 		m_pDlgSplit->m_pTxt_no1->SetText(_AL(""));
// 		m_pDlgSplit->m_pTxt_no2->SetText(_AL(""));
// 		m_pDlgSplit->m_pTxt_Gold->SetText(_AL(""));
// 		m_pDlgSplit->m_pBtn_Cancel->Enable(true);
// 
// 		pHost->GetPack(IVTRTYPE_PACK)->UnfreezeAllItems();
// 	}
// 	else if( idService == NOTIFY_IDENTIFY_END )
// 	{
// 		S2C::cmd_item_identify *pCmd = (S2C::cmd_item_identify *)pData;
// 
// 		if( 0 == pCmd->result )
// 		{
// 			pHost->EndNPCService();
// 			m_pCurNPCEssence = NULL;
// 			m_pDlgIdentify->Show(false);
// 			MessageBox("", GetStringFromTable(505),
// 				MB_OK, A3DCOLORRGBA(255, 255, 255, 160));
// 			pHost->GetPack(IVTRTYPE_PACK)->UnfreezeAllItems();
// 		}
// 		else
// 		{
// 			MessageBox("", GetStringFromTable(506),
// 				MB_OK, A3DCOLORRGBA(255, 255, 255, 160));
// 		}
// 	}
}

void CECGameUIMan::GetUserLayout(void *pvData, DWORD &dwSize)
{
	if( pvData || m_bReplayMode )
	{
		int i;
		USER_LAYOUT ul;
		CECHostPlayer *pHost = g_pGame->GetGameRun()->GetHostPlayer();

		APointF ptPos;
		AIniFile theIni;
		char szIniFile[MAX_PATH], szKey[MAX_PATH];

		theIni.WriteIntValue("Version", "Version", CECGAMEUIMAN_LOCALLAYOUT_VERSION);
		for( i = 0; i < (int)m_aDialog.size() - 1; i++ )
		{
			ptPos = m_aDialog[i]->GetPercentPos();

			sprintf(szKey, "%s_x", m_aDialog[i]->GetName());
			theIni.WriteFloatValue("Windows", szKey, ptPos.x);

			sprintf(szKey, "%s_y", m_aDialog[i]->GetName());
			theIni.WriteFloatValue("Windows", szKey, ptPos.y);
		}

		//ui size
		theIni.WriteIntValue("Windows","uisize",m_cUISize);
		//show touchmin?
		theIni.WriteIntValue("SpecialUIParam","showtouchmin",m_bShowTouchmin ? 1 : 0);

		

		//quikbar layout
		theIni.WriteIntValue("QuickBarLayout","showallgrid",m_QuickBarLayout.bShowAllGrid ? 1: 0);
		theIni.WriteIntValue("QuickBarLayout","showrightbar",m_QuickBarLayout.bShowRightBar ? 1 : 0);
		theIni.WriteIntValue("QuickBarLayout","showcustombar1",m_QuickBarLayout.bShowCustom_Bar1 ? 1 : 0);
		theIni.WriteIntValue("QuickBarLayout","showcustombar2",m_QuickBarLayout.bShowCustom_Bar2 ? 1 : 0);
		theIni.WriteIntValue("QuickBarLayout","custombar1_type",m_QuickBarLayout.nCutomBar1_Type);
		theIni.WriteIntValue("QuickBarLayout","custombar2_type",m_QuickBarLayout.nCutomBar2_Type);

		sprintf(szIniFile, "%s\\Userdata\\Layout", af_GetBaseDir());
		CreateDirectoryA(szIniFile, NULL);

		if( m_bReplayMode )
			sprintf(szIniFile, "%s\\UserData\\Layout\\replay.ini", af_GetBaseDir());
		else
			sprintf(szIniFile, "%s\\UserData\\Layout\\%d.ini", af_GetBaseDir(), pHost->GetCharacterID());
		theIni.Save(szIniFile);
		theIni.Close();

		sprintf(szIniFile, "UserData\\Layout\\%d.key", pHost->GetCharacterID());
		CECHostInputFilter* pFilter = (CECHostInputFilter*)g_pGame->GetGameRun()->GetInputCtrl()->GetKBFilter();
		if (pFilter)
			pFilter->SaveLogicKeyMap(szIniFile);

		AString file_name;
		CreateDirectoryA("userdata\\helper", NULL);
		AString sub_dir;
		sub_dir.Format("%s\\%d", "userdata\\helper", g_pGame->GetGameRun()->GetHostPlayer()->GetCharacterID());
		CreateDirectoryA(sub_dir, NULL);
		file_name.Format("%s\\petautofood.ini", sub_dir);
		AString strValue;
		strValue.Format("%d", CDlgHostPet::GetAutoFoodHp());
		WritePrivateProfileStringA("Settings", "AutoHp", strValue, file_name);
		strValue.Format("%d", CDlgHostPet::GetAutoFoodMp());
		WritePrivateProfileStringA("Settings", "AutoMp", strValue, file_name);
		strValue.Format("%d", CDlgHostPet::GetAutoFoodFull());
		WritePrivateProfileStringA("Settings", "AutoFull", strValue, file_name);

		if( m_bReplayMode )
			return;

		memset(&ul, 0, sizeof(USER_LAYOUT));

		ul.nVersion = LAYOUTDATA_VERSION;
		ul.nMapMode = m_pDlgMiniMap->m_nMode;

		ul.nQuickbar10Mode = 0;
		ul.nQuickbar8Mode = ((PAUICHECKBOX)GetDialog("Win_NewerHelp")->GetDlgItem("Chk_Close"))->IsChecked();

		ul.bGTOnlineNotify = m_bGTOnLineNotify;
		ul.bQuickbarLock = ((PAUICHECKBOX)m_pDlgQuickBar8->GetDlgItem("Chk_Lock"))->IsChecked();
		ul.nChatWinSize = 0; // no use m_pDlgChat->GetChatWinSize();
		ul.nCurChatColor = m_pDlgChat->GetChatAlpha();

		for( i = 0; i < (int)m_pDlgMiniMap->m_vecMark.size(); i++ )
		{
			ul.a_Mark[i].nNPC = m_pDlgMiniMap->m_vecMark[i].nNPC;
			ul.a_Mark[i].vecPos = m_pDlgMiniMap->m_vecMark[i].vecPos;
			ul.a_Mark[i].idInstance = m_pDlgMiniMap->m_vecMark[i].idInstance;
			a_strncpy(ul.a_Mark[i].szName, m_pDlgMiniMap->m_vecMark[i].strName, CECGAMEUIMAN_MARK_NAME_LEN);
		}

		CECFriendMan::GROUP *pGroup;
		CECFriendMan *pMan = g_pGame->GetGameRun()->GetHostPlayer()->GetFriendMan();

		for( i = 0; i < pMan->GetGroupNum(); i++ )
		{
			pGroup = pMan->GetGroupByIndex(i);
			ul.idGroup[i] = pGroup->iGroup;
			ul.clrGroup[i] = pGroup->color;
		}
		ul.bAutoReply = m_bAutoReply;
		ul.bOnlineNotify = m_bOnlineNotify;
		ul.bSaveHistory = m_bSaveHistory;

		CECInventory *pPack = g_pGame->GetGameRun()->GetHostPlayer()->GetPack();
		char szName[20];
		for(i = 0; i < CECGAMEUIMAN_MAX_TALISMANSLOT; i++ )
		{
			CECShortcut *pSC;
			if( i < 3 )
			{
				sprintf(szName, "Item_%d", i + 1);
				pSC = (CECShortcut*)m_pDlgTalisman->GetDlgItem(szName)->GetDataPtr("ptr_CECShortcut");
			}
			else if (i==13)
			{
				sprintf(szName, "Item_4");
				pSC = (CECShortcut*)m_pDlgTalisman->GetDlgItem(szName)->GetDataPtr("ptr_CECShortcut");
			}
			else if( i < 10)
			{
				sprintf(szName, "Img_Skill%02d", i);
				pSC = (CECShortcut*)GetDialog("Win_TalismanSmall")->GetDlgItem(szName)->GetDataPtr("ptr_CECShortcut");
			}
			else
			{
				sprintf(szName, "Img_Item%02d", i-9);
				pSC = (CECShortcut*)GetDialog("Win_TalismanSmall")->GetDlgItem(szName)->GetDataPtr("ptr_CECShortcut");
			}
			if( pSC )
			{
				ul.a_TalismanItem[i].type = pSC->GetType();
				switch(pSC->GetType())
				{
				case CECShortcut::SCT_ITEM:
					ul.a_TalismanItem[i].data = ((CECSCItem*)pSC)->GetIvtrSlot();
					break;
				case CECShortcut::SCT_COMMAND:
					ul.a_TalismanItem[i].data = ((CECSCCommand*)pSC)->GetCommandID();
					break;
				case CECShortcut::SCT_SKILL:
					if( ((CECSCSkill*)pSC)->GetSkill() )
						ul.a_TalismanItem[i].data = ((CECSCSkill*)pSC)->GetSkill()->GetSkillID();
					else
						ul.a_TalismanItem[i].type = -1;
					break;
				case CECShortcut::SCT_SKILLGRP:
					ul.a_TalismanItem[i].data = ((CECSCSkillGrp*)pSC)->GetGroupIndex();
					break;
				}
			}
			else if (m_iTalismanSkillDumb[i]>0)
			{
				ul.a_TalismanItem[i].type = CECShortcut::SCT_SKILL;
				ul.a_TalismanItem[i].data = m_iTalismanSkillDumb[i];
			}
			else
			{
				ul.a_TalismanItem[i].type = -1;
				ul.a_TalismanItem[i].data = 0;
			}
		}
		CECShortcut *pSC = (CECShortcut*)m_pDlgPetPackage->GetDlgItem("Img_AutoFood")->GetDataPtr("ptr_CECShortcut");
		if( pSC )
			ul.nAutoFoodSlot = ((CECSCItem*)pSC)->GetIvtrSlot();
		else
			ul.nAutoFoodSlot = -1;

		memcpy(pvData, &ul, sizeof(USER_LAYOUT));
	}
	else
	{
		dwSize = sizeof(USER_LAYOUT);
	}
}

bool CECGameUIMan::SetUserLayout(void *pvData, DWORD dwSize)
{
	int i;
	char szName[40];
	PAUICHECKBOX pCheck;
	CECFriendMan::MESSAGE *pMsg;

	CECWorld *pWorld = g_pGame->GetGameRun()->GetWorld();
	CECHostPlayer *pHost = g_pGame->GetGameRun()->GetHostPlayer();
	CECFriendMan *pMan = pHost->GetFriendMan();
	
	char szPath[MAX_PATH];
	char cUISize = 2;
	float x, y;
	AIniFile theIni;
	char szKey[MAX_PATH], szIniFile[MAX_PATH];

	sprintf(szIniFile, "UserData\\Layout\\%d.key", pHost->GetCharacterID());

	CECHostInputFilter* pFilter = (CECHostInputFilter*)g_pGame->GetGameRun()->GetInputCtrl()->GetKBFilter();
	if (pFilter && !pFilter->LoadLogicKeyMap(szIniFile))
		pFilter->ResetLogicMap();
	
	if( m_bReplayMode )
		sprintf(szIniFile, "%s\\UserData\\Layout\\replay.ini", af_GetBaseDir());
	else
		sprintf(szIniFile, "%s\\UserData\\Layout\\%d.ini", af_GetBaseDir(), pHost->GetCharacterID());
	if( theIni.Open(szIniFile) )
	{
		int version = theIni.GetValueAsInt("Version", "Version", 0);
		if( version == CECGAMEUIMAN_LOCALLAYOUT_VERSION )
		{
			for( i = 0; i < (int)m_aDialog.size(); i++ )
			{
				if( !m_aDialog[i]->IsCanMove() || m_aDialog[i]->GetAlpha() <= 5 ) continue;
				
				sprintf(szKey, "%s_x", m_aDialog[i]->GetName());
				x = theIni.GetValueAsFloat("Windows", szKey, 4);
				
				sprintf(szKey, "%s_y", m_aDialog[i]->GetName());
				y = theIni.GetValueAsFloat("Windows", szKey, 4);
				
				if( fabs(x) <= 2 && fabs(y) <=2 )
					m_aDialog[i]->SetPercentPosEx(x, y);
			}
			cUISize = theIni.GetValueAsInt("Windows","uisize",2);
			// show touchmin？
			m_bShowTouchmin = theIni.GetValueAsBoolean("SpecialUIParam","showtouchmin",true);

            m_QuickBarLayout.bShowAllGrid      = theIni.GetValueAsBoolean("QuickBarLayout","showallgrid",true);
			m_QuickBarLayout.bShowRightBar     = theIni.GetValueAsBoolean("QuickBarLayout","showrightbar",false);
			m_QuickBarLayout.bShowCustom_Bar1  = theIni.GetValueAsBoolean("QuickBarLayout","showcustombar1",false);
			m_QuickBarLayout.bShowCustom_Bar2  = theIni.GetValueAsBoolean("QuickBarLayout","showcustombar2",false);
			m_QuickBarLayout.nCutomBar1_Type   = theIni.GetValueAsInt("QuickBarLayout","custombar1_type",0);
			m_QuickBarLayout.nCutomBar2_Type   = theIni.GetValueAsInt("QuickBarLayout","custombar2_type",0);

		}
		
		theIni.Close();
	}

	// quikbar layout
	{
	   UpdateQuickBarState();
	}
	// windows size
	{
		SetUIWindowScale(cUISize);
	}
	CommonMessage("Win_Quest", "loadInfo", 0, 0);
	CommonMessage("Win_Quest_Main", "loadInfo", 0, 0);
	CommonMessage("Win_FriendChat0", "loadInfo", 0, 0);
	
	AString file_name;
	AString sub_dir;
	sub_dir.Format("%s\\%d", "userdata\\helper", g_pGame->GetGameRun()->GetHostPlayer()->GetCharacterID());
	file_name.Format("%s\\petautofood.ini", sub_dir);
	CDlgHostPet::SetAutoFoodHp(GetPrivateProfileIntA("Settings", "AutoHp", 50, file_name));
	CDlgHostPet::SetAutoFoodMp(GetPrivateProfileIntA("Settings", "AutoMp", 50, file_name));
	CDlgHostPet::SetAutoFoodFull(GetPrivateProfileIntA("Settings", "AutoFull", 100, file_name));
	
//	if( m_bReplayMode )
//		return true;
	
	sprintf(szPath, "%s\\Userdata\\Msg", af_GetBaseDir());
	CreateDirectoryA(szPath, NULL);
	
	sprintf(szPath, "%s\\Userdata\\Msg\\%d", af_GetBaseDir(), pHost->GetCharacterID());
	CreateDirectoryA(szPath, NULL);
	
	BuildFriendList();
	
	CDlgChat::LoadQuickTalk();
	CDlgHelpMemo *pDlgHelpMemo = (CDlgHelpMemo*)GetDialog("Win_Help_Memo");
	pDlgHelpMemo->LoadRemind();
	
	m_pDlgActive = NULL;
	
	__int8 nVersion = *((__int8*)pvData);
	//only compatible with one back version
	if (nVersion < (LAYOUTDATA_VERSION-1) || nVersion > LAYOUTDATA_VERSION)
		return true;

	typedef struct _USER_LAYOUT14
	{
		__int8 nVersion;
		__int8 nMapMode;
		char nQuickbar10Mode;	//无用
		char nQuickbar8Mode;	//无用
		bool bQuickbarLock;	
		bool bGTOnlineNotify;
		int nChatWinSize;
		int nCurChatColor;
		SAVE_MARK a_Mark[10];
		bool bOnlineNotify;
		bool bAutoReply;
		bool bSaveHistory;
		int idGroup[10];
		A3DCOLOR clrGroup[10];
		TALISMAN_ITEM a_TalismanItem[13];
		int nAutoFoodSlot;
	} USER_LAYOUT14, * P_USER_LAYOUT14;
	

	USER_LAYOUT14 ul14;
	USER_LAYOUT ul;
	memset(&ul, 0, sizeof(USER_LAYOUT));

	if (nVersion == 14)
	{
		memcpy(&ul14, pvData, sizeof(USER_LAYOUT14));
		ul.nMapMode = ul14.nMapMode;
		ul.nQuickbar10Mode = ul14.nQuickbar10Mode;
		ul.nQuickbar8Mode = ul14.nQuickbar8Mode;
		ul.bQuickbarLock = ul14.bQuickbarLock;
		ul.bGTOnlineNotify = ul14.bGTOnlineNotify;
		ul.nChatWinSize = ul14.nChatWinSize;
		ul.nCurChatColor = ul14.nCurChatColor;
		memcpy(&ul.a_Mark, ul14.a_Mark, 5*sizeof(SAVE_MARK));
		ul.bOnlineNotify = ul14.bOnlineNotify;
		ul.bAutoReply = ul14.bAutoReply;
		ul.bSaveHistory = ul14.bSaveHistory;
		memcpy(&ul.idGroup, ul14.idGroup, CECGAMEUIMAN_MAX_GROUPS*sizeof(int));
		memcpy(&ul.clrGroup, ul14.clrGroup, CECGAMEUIMAN_MAX_GROUPS*sizeof(A3DCOLOR));
		memcpy(&ul.a_TalismanItem, ul14.a_TalismanItem, 13*sizeof(TALISMAN_ITEM));
		ul.nAutoFoodSlot = ul14.nAutoFoodSlot;
		ul.a_TalismanItem[13].type = -1;
	}
	else
	{
		memcpy(&ul, pvData, sizeof(USER_LAYOUT));
	}

	m_pDlgMiniMap->m_nMode = ul.nMapMode;
	
	((PAUICHECKBOX)GetDialog("Win_NewerHelp")->GetDlgItem("Chk_Close"))->Check(ul.nQuickbar8Mode>0);
	((PAUICHECKBOX)m_pDlgQuickBar8->GetDlgItem("Chk_Lock"))->Check(ul.bQuickbarLock);
	//	((PAUICHECKBOX)m_pDlgQuickBar10->GetDlgItem("Chk_Lock"))->Check(ul.bQuickbarLock);
	
	m_pDlgChat->m_nCurChatAlpha = ul.nCurChatColor;
	//m_pDlgChat->OnCommand_chgalpha("chgalpha");
	m_pDlgChat->ChangeFocus(NULL);
	
	ACHAR szText[40];
	CDlgMiniMap::MARK Mark;
	PAUILISTBOX pList = (PAUILISTBOX)m_pDlgMiniMapMark->GetDlgItem("List_Choose");
	PAUILISTBOX pList1 = (PAUILISTBOX)m_pDlgMidMap->GetDlgItem("List_Choose");
	
	pList->ResetContent();
	pList1->ResetContent();
	for( i = 0; i < CECGAMEUIMAN_MAX_MARKS; i++ )
	{
		if( a_strlen(ul.a_Mark[i].szName) == 0 ) break;
		
		Mark.nNPC = ul.a_Mark[i].nNPC;
		Mark.vecPos = ul.a_Mark[i].vecPos;
		Mark.strName = ul.a_Mark[i].szName;
		Mark.idInstance = ul.a_Mark[i].idInstance;
		
		m_pDlgMiniMap->m_vecMark.push_back(Mark);
		pList->AddString(Mark.strName);
		pList1->AddString(Mark.strName);
		a_sprintf(szText, _AL("%d,%d"),
			int(Mark.vecPos.x), int(Mark.vecPos.z));
		pList->SetItemHint(pList->GetCount() - 1, szText);
		pList1->SetItemHint(pList->GetCount() - 1, szText);
	}
	ChangePrecinct(m_pCurPrecinct);
	
	m_bAutoReply = ul.bAutoReply;
	m_bOnlineNotify = ul.bOnlineNotify;
	m_bGTOnLineNotify = ul.bGTOnlineNotify;
	m_bSaveHistory = ul.bSaveHistory;
	for( i = 0; i < pMan->GetGroupNum(); i++ )
		pMan->SetGroupColor(ul.idGroup[i], ul.clrGroup[i]);
	m_pDlgFriendList->BuildFriendList();
	for( i = 0; i < CECGAMEUIMAN_MAX_CHATS; i++ )
	{
		sprintf(szName, "Win_FriendChat%d", i);
		pCheck = (PAUICHECKBOX)GetDialog(szName)->GetDlgItem("Chk_Save");
		pCheck->Check(m_bSaveHistory);
	}
	
	if( pMan->GetOfflineMsgNum() > 0 )
	{
		int *idSender = new int[pMan->GetOfflineMsgNum()];
		int num_sender = 0;
		for( i = 0; i < pMan->GetOfflineMsgNum(); i++ )
		{
			pMsg = pMan->GetOfflineMsg(i);

			int j;
			for(j = 0; j < num_sender; j++)
				if( pMsg->idSender == idSender[j] )
					break;

			if( j == num_sender )
				idSender[num_sender++] = pMsg->idSender;

			if( pMsg->idSender )
				AddFriendMessage(pMsg->strMsg, pMsg->idSender, pMsg->strName, pMsg->byFlag);
			else
				AddInformation(CDlgInfo::INFO_SYSTEM, "Game_MsgBox", pMsg->strMsg, 0xFFFFFFF, 0, 0, 0);
		}

		if( num_sender > 0 )
			g_pGame->GetGameSession()->GetPlayerBriefInfo(num_sender, idSender, 2);
		delete []idSender;
	}
	
	memset(m_iTalismanSkillDumb, 0, CECGAMEUIMAN_MAX_TALISMANSLOT*sizeof(int));
	CECInventory *pPack = g_pGame->GetGameRun()->GetHostPlayer()->GetPack();
	for(i = 0; i < CECGAMEUIMAN_MAX_TALISMANSLOT; i++ )
		if( ul.a_TalismanItem[i].type != -1 )
		{
			if( i < 3 )
				sprintf(szName, "Item_%d", i + 1);
			else if (i==13)
				sprintf(szName, "Item_4");
			else if( i < 10 )
				sprintf(szName, "Img_Skill%02d", i);
			else
				sprintf(szName, "Img_Item%02d", i-9);
			CECShortcut *pSC;
			switch(ul.a_TalismanItem[i].type)
			{
			case CECShortcut::SCT_ITEM:
				{
					CECIvtrItem *pItem = pPack->GetItem(ul.a_TalismanItem[i].data);
					if (pItem && i>9 && i!=13) //新加类型，特殊处理
					{
						pSC = new CECSCItem;
						if( pSC )
						{
							if( ((CECSCItem*)pSC)->Init(IVTRTYPE_PACK, ul.a_TalismanItem[i].data, pItem) )
								m_pDlgTalisman->SetTalismanItem(szName, pSC);
						}
					}
					else if( pItem && pItem->GetClassID() == CECIvtrItem::ICID_MEDICINE )
					{
						CECIvtrMedicine *pMedicine = (CECIvtrMedicine*)pItem;
						int iType = pMedicine->GetDBEssence()->type;
						if( (iType == 2 || iType == 5 ||
							i == 0 && (iType == 0 || iType == 3) ||
							i == 1 && (iType == 1 || iType == 4)) ||
							i == 13 && (iType == 6 || iType == 7) )
						{
							pSC = new CECSCItem;
							if( pSC )
							{
								if( ((CECSCItem*)pSC)->Init(IVTRTYPE_PACK, ul.a_TalismanItem[i].data, pItem) )
									m_pDlgTalisman->SetTalismanItem(szName, pSC);
							}
						}
					}
					else if( pItem && pItem->GetClassID() == CECIvtrItem::ICID_TALISMAN_ENERGYFOOD && i == 2 )
					{
						pSC = new CECSCItem;
						if( pSC )
						{
							if( ((CECSCItem*)pSC)->Init(IVTRTYPE_PACK, ul.a_TalismanItem[i].data, pItem) )
								m_pDlgTalisman->SetTalismanItem(szName, pSC);
						}
					}
					else if( pItem && pItem->GetClassID() == CECIvtrItem::ICID_AGENTCOMBINED )
					{
						pSC = new CECSCItem;
						if( pSC )
						{
							if( ((CECSCItem*)pSC)->Init(IVTRTYPE_PACK, ul.a_TalismanItem[i].data, pItem) )
								m_pDlgTalisman->SetTalismanItem(szName, pSC);
						}
					}
					break;
				}
			case CECShortcut::SCT_COMMAND:
				{
					pSC = new CECSCCommand(ul.a_TalismanItem[i].data);
					if( pSC )
						m_pDlgTalisman->SetTalismanItem(szName, pSC);
					break;
				}
			case CECShortcut::SCT_SKILL:
				{
					if(i>=3&&i<10)
					{
						CECSkill *pSkill = g_pGame->GetGameRun()->GetHostPlayer()->GetSkill(ul.a_TalismanItem[i].data);
						if( pSkill )
						{
							pSC = new CECSCSkill;
							if( pSC )
							{
								if( ((CECSCSkill*)pSC)->Init(pSkill) )
									m_pDlgTalisman->SetTalismanItem(szName, pSC);
								else
									delete pSC;
							}
						}
						else
						{
							pSkill = new CECSkill(ul.a_TalismanItem[i].data, 0);
							// 触发技能
							if (pSkill->IsCategory(GNET::CATEGORY_TRIGGERED))
							{
								pSC = new CECSCSkill;
								if( pSC )
								{
									if( ((CECSCSkill*)pSC)->Init(pSkill) )
										m_pDlgTalisman->SetTalismanItem(szName, pSC);
									else
										delete pSC;
								}
							}
							else
								m_iTalismanSkillDumb[i] = ul.a_TalismanItem[i].data;
							delete pSkill;
							pSkill = NULL;
						}
					}
					break;
				}
			case CECShortcut::SCT_SKILLGRP:
				{
					pSC = new CECSCSkillGrp;
					if( pSC )
					{
						if( ((CECSCSkillGrp*)pSC)->Init(ul.a_TalismanItem[i].data) )
							m_pDlgTalisman->SetTalismanItem(szName, pSC);
						else
							delete pSC;
					}
					break;
				}
			}
		}
		
	if( ul.nAutoFoodSlot != -1 )
	{
		CECIvtrItem *pItem = pPack->GetItem(ul.nAutoFoodSlot);
		if( pItem && pItem->GetClassID() == CECIvtrItem::ICID_PET_AUTO_FOOD )
		{
			CECPetAutoFood *pAutoFood = (CECPetAutoFood*)pItem;
			CECSCItem *pSC = new CECSCItem;
			if( pSC )
				{
				if( ((CECSCItem*)pSC)->Init(IVTRTYPE_PACK, ul.nAutoFoodSlot, pItem) )
				{
					m_pDlgPetPackage->SetAutoFood(pSC);
				}
				else
					delete pSC;
			}
		}
	}

	return true;
}
void CECGameUIMan::UpdateQuickBarState()
{
   GetDialog("Win_QuickBar10V_1")->Show(m_QuickBarLayout.bShowRightBar);
   if(!GetDialog("Win_QuickBar10V_1")->IsShow())
     GetDialog("Win_QuickBar10V_2")->Show(false);

   GetDialog("Win_QuickBar12C1")->Show(m_QuickBarLayout.bShowCustom_Bar1 && m_QuickBarLayout.nCutomBar1_Type == 0);
   GetDialog("Win_QuickBar12C2")->Show(m_QuickBarLayout.bShowCustom_Bar1 && m_QuickBarLayout.nCutomBar1_Type == 1);
   GetDialog("Win_QuickBar12C3")->Show(m_QuickBarLayout.bShowCustom_Bar1 && m_QuickBarLayout.nCutomBar1_Type == 2);
   GetDialog("Win_QuickBar12C4")->Show(m_QuickBarLayout.bShowCustom_Bar1 && m_QuickBarLayout.nCutomBar1_Type == 3);

   GetDialog("Win_QuickBar12C5")->Show(m_QuickBarLayout.bShowCustom_Bar2 && m_QuickBarLayout.nCutomBar2_Type == 0);
   GetDialog("Win_QuickBar12C6")->Show(m_QuickBarLayout.bShowCustom_Bar2 && m_QuickBarLayout.nCutomBar2_Type == 1);
   GetDialog("Win_QuickBar12C7")->Show(m_QuickBarLayout.bShowCustom_Bar2 && m_QuickBarLayout.nCutomBar2_Type == 2);
   GetDialog("Win_QuickBar12C8")->Show(m_QuickBarLayout.bShowCustom_Bar2 && m_QuickBarLayout.nCutomBar2_Type == 3);

}
void  CECGameUIMan::_InternalArrangeWindow()
{
	PAUIDIALOG pDlgBase = m_pDlgSystem;
	A3DVIEWPORTPARAM *param = g_pGame->GetViewport()->GetA3DViewport()->GetParam();
	PAUICONSOLE pConsole = (PAUICONSOLE)GetDialog("Dlg_Console")->GetDlgItem("Command_Edit");

	//AUILuaManager::RearrangeWindows(rcOld, rcNew);
	//	m_pDlgBBS->ChangeSize(param->Width, param->Height);
	m_pDlgWorldMap->SetPosEx(0, 0, AUIDialog::alignCenter, AUIDialog::alignCenter);
	pConsole->FitScreen();
	//	m_pDlgSettingCurrent->UpdateView();

	m_pDlgSystem2->AlignTo(pDlgBase,
		AUIDIALOG_ALIGN_OUTSIDE, AUIDIALOG_ALIGN_LEFT,
		AUIDIALOG_ALIGN_INSIDE, AUIDIALOG_ALIGN_BOTTOM);
	m_pDlgSystem3->AlignTo(pDlgBase,
		AUIDIALOG_ALIGN_OUTSIDE, AUIDIALOG_ALIGN_LEFT,
		AUIDIALOG_ALIGN_INSIDE, AUIDIALOG_ALIGN_BOTTOM);
	m_pDlgInfo->AlignTo(m_pDlgInfoIcon,
		AUIDIALOG_ALIGN_OUTSIDE, AUIDIALOG_ALIGN_LEFT,
		AUIDIALOG_ALIGN_INSIDE, AUIDIALOG_ALIGN_TOP);

	{
		/*PAUIDIALOG pDlgSystemClose = GetDialog("Win_MainClose");
		PAUIDIALOG pDlgSystem = GetDialog("Win_Main");

		pDlgSystem->AlignTo(pDlgSystemClose,
			AUIDIALOG_ALIGN_OUTSIDE, AUIDIALOG_ALIGN_LEFT,
			AUIDIALOG_ALIGN_INSIDE, AUIDIALOG_ALIGN_BOTTOM);*/
	}
	m_pDlgChat1->RearrangeWindow();

	GetDialog("Win_Worldmap_Button")->AlignTo(GetDialog("Win_WorldMap"),
		AUIDIALOG_ALIGN_INSIDE, AUIDIALOG_ALIGN_RIGHT,
		AUIDIALOG_ALIGN_INSIDE, AUIDIALOG_ALIGN_BOTTOM);
	GetDialog("Win_Worldmap_Button")->SetPosEx(GetDialog("Win_Worldmap_Button")->GetPos().x-int(42*m_fWindowScale), GetDialog("Win_Worldmap_Button")->GetPos().y-int(34*m_fWindowScale));
	CDlgAchivItemMan *pDlgAchivItemMan = (CDlgAchivItemMan*)GetDialog("Win_Achievement_Sub");
	pDlgAchivItemMan->InitItemDlgs();
	CDlgAchivMain* pDlgAchivMain = (CDlgAchivMain*)GetDialog("Win_Achievement");
	pDlgAchivMain->CalcTargetPos();

	abase::hash_map<int, A2DSprite*>::iterator it = m_mapA2DSpriteGuildIcons.begin();
	for(; it != m_mapA2DSpriteGuildIcons.end(); ++it)
	{
		A2DSprite* pSprite = it->second;
		pSprite->SetScaleX(m_fWindowScale);
		pSprite->SetScaleY(m_fWindowScale);
	}
	m_pA2DSpriteIcons[ICONS_GUILD]->SetScaleX(m_fWindowScale);
	m_pA2DSpriteIcons[ICONS_GUILD]->SetScaleY(m_fWindowScale);
	if( m_FontHint.pFont )
		m_FontHint.pFont->Resize(m_fWindowScale * _hint_font_size);

	abase::vector<A2DSprite*>::iterator itVec;
	for(itVec=m_vecImageList.begin(); itVec!=m_vecImageList.end(); ++itVec)
	{
		(*itVec)->SetScaleX(m_fWindowScale);
		(*itVec)->SetScaleY(m_fWindowScale);
	}
	for(itVec=m_vecImageListControl.begin(); itVec!=m_vecImageListControl.end(); ++itVec)
	{
		(*itVec)->SetScaleX(m_fWindowScale);
		(*itVec)->SetScaleY(m_fWindowScale);
	}

	// 诛仙知道界面只能缩小，不能放大
	if (0&&m_fWindowScale>1)
	{
		GetDialog("Win_ZhuxianKnows")->SetScale(1.0f/m_fWindowScale);
	}

	GetDialog("Win_Gt")->AlignTo(m_pDlgHost,
		AUIDIALOG_ALIGN_OUTSIDE, AUIDIALOG_ALIGN_RIGHT,
		AUIDIALOG_ALIGN_INSIDE, AUIDIALOG_ALIGN_TOP);
	//DisplayMiniButtons();
}
void CECGameUIMan::SetUIWindowScale(char cUISize)
{
	 if(cUISize>=0 && cUISize<=2 && cUISize != m_cUISize)
	 {
         static float windowsize[] = {0.8f,0.9f,1.0f};
		 float fSourceScale = GetWindowScale()/windowsize[m_cUISize];
		 float fScale = fSourceScale*windowsize[cUISize];
		 SetWindowScale(fScale);
         _InternalArrangeWindow();
		 m_cUISize  = cUISize;
	 }

}
float CECGameUIMan::GetCustomUIScale()
{
     static float windowsize[] = {0.8f,0.9f,1.0f};
	 return windowsize[m_cUISize];
}
//	Set UI enable flag
void CECGameUIMan::EnableUI(bool bEnable)
{
	//	Build configs
// 	m_pDlgSettingVideo->UpdateView();
// 	m_pDlgSettingAudio->UpdateView();
// 	m_pDlgSettingUI->UpdateView();
// 	m_pDlgSettingOther->UpdateView();
	
	AUIOBJECT_SETPROPERTY sp;
	PAUIOBJECT pObj = m_pDlgChat1->GetDlgItem("Txt_Content");
	PAUIOBJECT pObj2 = m_pDlgChat2->GetDlgItem("Txt_Content");
	PAUIOBJECT pObj3 = GetDialog("Win_ChatSpecInfo")->GetDlgItem("Txt_Content");
	EC_GAME_SETTING gs = g_pGame->GetConfigs()->GetGameSettings();
	EC_BLACKLIST_SETTING bs = g_pGame->GetConfigs()->GetBlackListSettings();

	pObj->GetProperty("Font", &sp);
	sp.font.nHeight = gs.nFontSize + 10;	// 10 to 14.
	a_Clamp(sp.font.nHeight, 10, 14);
	pObj->SetProperty("Font", &sp);
	pObj2->SetProperty("Font", &sp);
	pObj3->SetProperty("Font", &sp);

	m_pDlgChat->RebuildChatContents();

	//	Retrieve blacklist username.
	for(int i = 0; i < EC_BLACKLIST_LEN; i++ )
		if( bs.idPlayer[i] > 0 )
		{
			g_pGame->GetGameSession()->GetPlayerBriefInfo(1, bs.idPlayer + i, 2);
			m_pDlgFriendList->m_dwBlackListGetNameTime = GetTickCount();
		}

	m_bUIEnable = bEnable;
}

bool CECGameUIMan::UpdateTeam(bool bUpdateNear)
{
	return m_pDlgArrangeTeam->UpdateTeam(bUpdateNear);
}

bool CECGameUIMan::UpdateArrowInfo()
{
	if( m_pDlgWorldMap->IsShow() )
	{
		return true;
	}

	int W = 64, H = 64;
	A3DCamera *pA3DCamera = g_pGame->GetViewport()->GetA3DCamera();
	A3DMATRIX4 mr = RotateY(DEG2RAD(-(int)pA3DCamera->GetDeg() - 90));
	A3DVIEWPORTPARAM *p = m_pA3DEngine->GetActiveViewport()->GetParam();
	CECHostPlayer *pHost = g_pGame->GetGameRun()->GetHostPlayer();
	A3DVECTOR3 vecPos = pHost->GetPos();

	pHost->ClearTargetArrows();
	pHost->SetMoveArrow(false, g_vAxisZ, g_vOrigin);
	pHost->SetTempArrow(false, g_vAxisZ, g_vOrigin);
	pHost->SetTeamArrow(false, g_vAxisZ, g_vOrigin);
	pHost->SetRequestArrow(false, g_vAxisZ, g_vOrigin);

//	if( m_pDlgMiniMap->m_bShowTargetArrow )
//	{
//		for( i = 0; i < (int)m_pDlgMiniMap->m_vecMark.size(); i++ )
//			if( m_pDlgMiniMap->m_vecMark[i].idInstance == g_pGame->GetGameRun()->GetWorld()->GetInstanceID() )
//			{
//				pHost->AddTargetArrow(m_pDlgMiniMap->m_vecMark[i].vecPos - vecPos, m_pDlgMiniMap->m_vecMark[i].vecPos);
//			}
//	}

	DWORD dwTimePassed = GetTickCount() - m_pDlgMiniMap->m_dwTempTargetTime;
	if( m_pDlgMiniMap->m_dwTempTargetTime > 0 && dwTimePassed <= 3000 )
		pHost->SetTempArrow(true, m_pDlgMiniMap->m_vecTempTarget - vecPos, m_pDlgMiniMap->m_vecTempTarget);

	if( m_pDlgTeamMain->IsShowTeamArrow() )
	{
		CECTeam *pTeam = pHost->GetTeam();
		CECWorld *pWorld = g_pGame->GetGameRun()->GetWorld();

		if( pTeam && pTeam->GetLeader() &&
			POSISVALID(pTeam->GetLeader()->GetPos().x) &&
			pTeam->GetLeader()->GetInstanceID() == pWorld->GetInstanceID() &&
			pTeam->GetLeader()->GetCharacterID() != pHost->GetCharacterID() )
		{
			pHost->SetTeamArrow(true, pTeam->GetLeader()->GetPos() - vecPos, pTeam->GetLeader()->GetPos() + A3DVECTOR3(0.0f, 1.8f, 0.0f));
		}
	}

//	PAUICHECKBOX pChk_AwardNPC = (PAUICHECKBOX)GetDialog("Win_Quest")->GetDlgItem("Chk_AwardNPC");
//	if( pChk_AwardNPC && pChk_AwardNPC->IsChecked() && m_pDlgTask->m_AwardNPC != A3DVECTOR3(0, 0, 0) )
//		pHost->SetRequestArrow(true, m_pDlgTask->m_AwardNPC - vecPos, m_pDlgTask->m_AwardNPC);

	if( m_pObjMouseOn == m_pDlgMiniMap->GetDlgItem("Hide_2") ||
//		m_pObjMouseOn == GetDialog("Win_MiniMapB")->GetDlgItem("Hide_2") ||
		m_pObjMouseOn == m_pDlgMidMap->GetDlgItem("Img_MidMap") )
		pHost->SetMoveArrow(true, m_pDlgMiniMap->m_vecNowTarget - vecPos, m_pDlgMiniMap->m_vecNowTarget);
	
	return true;
}
void CECGameUIMan::UpdateKingBathState()
{
    CECHostPlayer* pHostPlayer = g_pGame->GetGameRun()->GetHostPlayer();
	PAUIDIALOG pDlg = GetDialog("Win_King_ShowerInvite");
	if(!pHostPlayer||!pDlg) return;
    //泡澡地图UI更新
	if(pHostPlayer->GetBathType() != -1)
	{
		int idSelObj = pHostPlayer->GetSelectedTarget();
		if( idSelObj != 0 && ISPLAYERID(idSelObj))
		{
		   CECElsePlayer *pPlayer = g_pGame->GetGameRun()->GetWorld()->GetPlayerMan()->GetElsePlayer(idSelObj);
		   if(pPlayer && pPlayer->GetGender() != pHostPlayer->GetGender())
		   {
              pDlg->SetData((DWORD)idSelObj);
			  if(!pDlg->IsShow())
				  pDlg->Show(true);
			  return;
		   }
		 
		}
		
	}
	if(pDlg->IsShow())
	{
		pDlg->SetData(0);
		pDlg->Show(false);
	}
}
void CECGameUIMan::PopupNPCDialog(talk_proc *pTalk)
{
	if( m_bReplayMode )
		return;

	m_pDlgNPC->PopupNPCDialog(pTalk);
}
void CECGameUIMan::PopupNpcMainDialog(talk_proc *pTalk)
{
	if( m_bReplayMode )
		return;

	m_pDlgNpcMain->PopupNPCDialog(pTalk);
}
void CECGameUIMan::PopupNPCDialog(const NPC_ESSENCE *pEssence)
{
	if( m_bReplayMode )
		return;
		
	if (pEssence->id_open_trans != 0 && !g_pGame->GetGameRun()->GetHostPlayer()->IsUiTransIdActive(pEssence->id_open_trans) )
	{
		g_pGame->GetGameSession()->c2s_CmdNPCSevActiveUiTransid(pEssence->id_open_trans);
		EndNPCService();
	}
	else if (pEssence->id_trans_dest_service != 0)
	{
		elementdataman* pDataMan = g_pGame->GetElementDataMan();
		DATA_TYPE dataType;
		const void* pData = pDataMan->get_data_ptr(pEssence->id_trans_dest_service, ID_SPACE_ESSENCE, dataType);
		if (!pData || dataType != DT_NPC_UI_TRANSFER_SERVICE)
		{
			ASSERT(false);
			return;
		}
		m_pDlgMapTransfer->PopupDlgMapTransfer( pEssence->id_open_trans, (NPC_UI_TRANSFER_SERVICE*)pData );
		m_bShowAllPanels = true;
	}
	else
		m_pDlgNPC->PopupNPCDialog(pEssence);
}

void CECGameUIMan::PopupTaskFinishDialog(int idTask, talk_proc *pTalk)
{
	if( m_bReplayMode )
		return;
	//直接将主线任务NPC界面关闭
	if(m_pDlgNpcMain->IsShow())
	{
	   m_pDlgNpcMain->Show(false);
	   EndNPCService();
	}
	m_pDlgNPC->PopupTaskFinishDialog(idTask, pTalk);
}

void CECGameUIMan::PopupTaskCheckInfo(int idTask, bool bRemove)
{
	ATaskTemplMan *pTempMan = g_pGame->GetTaskTemplateMan();
	ATaskTempl *pTemp = pTempMan->GetTaskTemplByID(idTask);
	if (pTemp && pTemp->m_AwardTalk.num_window>0 && pTemp->m_AwardTalk.windows[0].talk_text)
	{
		if (bRemove)
			m_pDlgInfo->DeletePopTask(CDlgInfo::INFO_SYSTEM, idTask);
		else if (m_pDlgInfo->NeedAddPopTask(CDlgInfo::INFO_SYSTEM, idTask))
			AddInformation(CDlgInfo::INFO_SYSTEM, "Game_TaskCheck", pTemp->m_AwardTalk.windows[0].talk_text, 0x0fffffff, idTask, 0, 0);
	}
}

void CECGameUIMan::PopupStorageDialog(bool bClose, bool bFaction)
{
	if( m_bReplayMode )
		return;

	if( bClose )
	{
		m_pDlgStorage->Show(false);
		m_pDlgInventory->Show(false);
		EndNPCService();
	}
	else
	{
		m_pDlgStorage->SetFactionStorage(bFaction);
		m_pDlgStorage->Show(true);
		m_pDlgInventory->Show(true);
	}
}
// 显示中间提示文字
void CECGameUIMan::PopupPKMessage(int idTarget, bool bIsMonster)
{	
// 	Task_State_info tsi;
// 	CECTaskInterface *pTask = g_pGame->GetGameRun()->GetHostPlayer()->GetTaskInterface();
// 	ATaskTemplMan *pMan = g_pGame->GetTaskTemplateMan();
// 
// 	int tid = pNPC->GetTemplateID();
// 	bool bDisplayCnt = ((PAUICHECKBOX)GetDialog("Win_Quest")->GetDlgItem("Chk_QuestNum"))->IsChecked();
// 
// 	ACString strText;
// 	CDlgPKMsg *pDlgPKMsg = (CDlgPKMsg*)GetDialog("Win_Pkmsg");
// 	if(bDisplayCnt)
// 	{
// 		for(int i = 0; i < (int)pTask->GetTaskCount(); i++ )
// 		{
// 			int idTask = pTask->GetTaskId(i);
// 			ATaskTempl *pTemp = pMan->GetTaskTemplByID(idTask);
// 			pTask->GetTaskStateInfo(idTask, &tsi);
// 
// 			int j;
// 			if(bIsMonster)
// 			{
// 				// 杀怪
// 				for (j = 0; j < MAX_MONSTER_WANTED; j++)
// 				{
// 					if(tsi.m_MonsterWanted[j].m_ulMonsterId != tid)
// 						continue;
// 					
// 					strText.Format(_AL("%s: %s(%d/%d)"), pTemp->GetName(), pNPC->GetName(), tsi.m_MonsterWanted[j].m_ulMonstersKilled, tsi.m_MonsterWanted[j].m_ulMonstersToKill);
// 							
// 					if(pDlgPKMsg)
// 						pDlgPKMsg->AddMessage(strText);
// 				}
// 			}
// 			else
// 			{
// 				// 物品
// 				for (j = 0; j < MAX_ITEM_WANTED; j++)
// 				{
// 					if(tsi.m_ItemsWanted[j].m_ulItemId != (unsigned long)idTarget)
// 						continue;
// 
// 					ACString strText;
// 					strText.Format(_AL("%s: %s(%d/%d)"), pTemp->GetName(), pItem->GetName(), iCmdSlotAmount, tsi.m_ItemsWanted[j].m_ulItemsToGet);
// 					
// 					CDlgPKMsg *pDlgPKMsg = (CDlgPKMsg*)pGameUI->GetDialog("Win_Pkmsg");
// 					if(pDlgPKMsg)
// 						pDlgPKMsg->AddMessage(strText);
// 				}
// 			}
// 
// 		}
// 	}

}
void CECGameUIMan::InvokeDragDrop(PAUIDIALOG pDlg, PAUIOBJECT pObj, POINT ptCapture)
{
	m_ptLButtonDown = ptCapture;
	pDlg->SetCapture(false);

	POINT ptPos = pObj->GetPos();

	// old : m_pDlgDragDrop->SetPos(ptPos.x, ptPos.y);
	m_pDlgDragDrop->SetPosEx(ptPos.x, ptPos.y);

	m_pDlgDragDrop->Show(true);
	
	void *ptr;
	AString szType;
	pObj->ForceGetDataPtr(ptr,szType);
	m_pDlgDragDrop->SetDataPtr(ptr,szType);
	m_pDlgDragDrop->SetCapture(true, ptCapture.x, ptCapture.y);

	int nItem;
	A2DSprite *pA2DSprite;
	PAUIIMAGEPICTURE pItem = (PAUIIMAGEPICTURE)pObj;
	
	pItem->GetCover(&pA2DSprite, &nItem);
	pItem = (PAUIIMAGEPICTURE)m_pDlgDragDrop->GetDlgItem("Goods");
	pItem->SetCover(pA2DSprite, nItem);
	pItem->SetDataPtr(pObj,"ptr_AUIObject");
	m_pDlgDragDrop->SetCaptureObject(pItem);
}

int CECGameUIMan::CommonMessage(AString strName, AString strMsg, WPARAM wParam, LPARAM lParam)
{
	CDlgBase* pDlg = (CDlgBase*)GetDialog(strName);
	if (pDlg)
	{
		return pDlg->CommonMessage(strMsg, wParam, lParam);
	}
	return 1;
}

A3DFTFont* CECGameUIMan::GetPateFontName()
{
	int index = g_pGame->GetConfigs()->GetVideoSettings().cNameSize;
	a_Clamp(index, 0, 2);
	if( m_pPateFontName )
		m_pPateFontName->Resize(GetWindowScale() * _pate_font_size[index]);
	return m_pPateFontName;
}

void CECGameUIMan::LoginPromp()
{
	if (!g_pGame->GetGameRun()->GetWorld()->GetCurRegion())
	{
		CommonMessage("Win_Popmsg_SafeArea", "show", 0, 0);
	}
	EC_GAME_CLIENTDATA gcd = g_pGame->GetConfigs()->GetGameClientData();

	/**
	日历功能屏蔽
	*/
	if (gcd.nHideDailyPrompLoginHint==0 && CDlgDailyPrompLoginHint::HasAnyDailyPrompToComplete())
	{
		AddInformation(CDlgInfo::INFO_SYSTEM, "Game_DailyPrompHint", GetStringFromTable(10413), 60000, 0, 0, 0);
	}
	((PAUICHECKBOX)GetDialog("Win_Quest")->GetDlgItem("Chk_FocusAll"))->Check(gcd.nTaskTrace!=0);
	if (gcd.nTaskTrace)
		m_pDlgTask->OnCommand_focusall("");
	
	CECHostPlayer *pHost = g_pGame->GetGameRun()->GetHostPlayer();
	bool bLocalSrv = pHost->GetSrcServerID()==0;
	m_pDlgChatNormal->Show(false);
	m_pDlgChatPk->Show(false);

	GetDialog("Win_Gt")->Show(bLocalSrv);
	if (bLocalSrv)
		m_pDlgChat = m_pDlgChatNormal;
	else
		m_pDlgChat = m_pDlgChatPk;
	m_pDlgChat->Show(true);
	m_pDlgChat->ChangeFocus(NULL);
	m_pDlgSystem4->GetDlgItem("Btn_Guild")->Enable(bLocalSrv);
	m_pDlgSystem4->GetDlgItem("Btn_Teacher")->Enable(bLocalSrv);
    if(NULL != m_pDlgQuickBar8->GetDlgItem("Btn_QShop"))
	   m_pDlgQuickBar8->GetDlgItem("Btn_QShop")->SetHint(GetStringFromTable(bLocalSrv ? 8802:8803));
	m_pDlgSystem3->GetDlgItem("Btn_Repick")->SetHint(GetStringFromTable(bLocalSrv ? 8804:8805));
	m_pDlgAutoLock->GetDlgItem("Btn_Confirm")->Enable(bLocalSrv);
	m_pDlgAutoLock->GetDlgItem("Txt_Hour")->Show(bLocalSrv);
	m_pDlgAutoLock->GetDlgItem("Txt_Minute")->Show(bLocalSrv);
	m_pDlgAutoLock->GetDlgItem("Lab_Reset")->Show(bLocalSrv);
	m_pDlgAutoLock->GetDlgItem("Lab_Hour")->Show(bLocalSrv);
	m_pDlgAutoLock->GetDlgItem("Lab_Minute")->Show(bLocalSrv);
	
	if (g_pGame->GetGameRun()->ShowLoginWallowIpInfo())
	{
		g_pGame->GetGameRun()->SetLastLoginId(g_pGame->GetGameRun()->GetLastLoginId());	// 保证只显示1次
		ACString strInfo;
		const GNET::RoleInfo& info = g_pGame->GetGameRun()->GetSelectedRoleInfo();
		time_t tLastLogin = info.lastlogin_time - g_pGame->GetTimeZoneBias() * 60;
		tm* tmLastLogin = gmtime(&tLastLogin);
		in_addr ipLast; 
		ipLast.s_addr = g_pGame->GetGameRun()->GetLastLoginIp();
		strInfo.Format(GetStringFromTable(11120), tmLastLogin->tm_year+1900, tmLastLogin->tm_mon+1,
			tmLastLogin->tm_mday, tmLastLogin->tm_hour, tmLastLogin->tm_min, ipLast.s_net, ipLast.s_host, ipLast.s_lh, ipLast.s_impno);

		if (info.lastlogin_time < (info.create_time+10)) // 第一次登陆
		{
			if( g_pGame->GetGameRun()->GetWallowInfo().wallow_level!=0xff)
			{
				AddChatMessage(GetStringFromTable(11121+g_pGame->GetGameRun()->GetWallowInfo().msg), GP_CHAT_SYSTEM);
				if (g_pGame->GetGameRun()->GetWallowInfo().msg==1)
				{
					MessageBox("", GetStringFromTable(11125), MB_OK);
				}
			}
		}
		else
		{
			if( g_pGame->GetGameRun()->GetWallowInfo().wallow_level==0xff)
				AddChatMessage(strInfo, GP_CHAT_SYSTEM);
			else
			{
				AddChatMessage(GetStringFromTable(11121+g_pGame->GetGameRun()->GetWallowInfo().msg)+strInfo, GP_CHAT_SYSTEM);
				if (g_pGame->GetGameRun()->GetWallowInfo().msg==1)
				{
					MessageBox("", GetStringFromTable(11125), MB_OK);
				}
			}
		}
	}
	
	// 获取vip信息
	g_pGame->GetGameSession()->c2s_CmdGetVipAwardInfo();
	
	/** minimap下面小按钮根据时间显示 */
	//DisplayMiniButtons();

	const char *szCountry = g_pGame->GetConfigs()->GetCountryRegion();
	//GetDialog("Win_MiniMap")->GetDlgItem("Btn_Gmbbs")->Show(stricmp(szCountry, "cn")==0);
	GetDialog("Win_Minimap_Recommend")->GetDlgItem("Btn_Gmbbs")->Show(stricmp(szCountry, "cn")==0);

	EC_VIDEO_SETTING vs = g_pGame->GetConfigs()->GetVideoSettings();
	if (vs.cUpdateVersion!=UPDATE_SHOWVERSION)
	{
		GetDialog("Win_UpdateDiary")->Show(true);
		vs.cUpdateVersion = UPDATE_SHOWVERSION;
		g_pGame->GetConfigs()->SetVideoSettings(vs);
		g_pGame->GetGameRun()->SaveConfigsToServer();
	}

	GetDialog("Win_CharHead_Prof11")->Show(pHost->GetProfession()>=56 && pHost->GetProfession()<=60);
	GetDialog("Win_CharHead_Prof11")->AlignTo(m_pDlgHost,
		AUIDIALOG_ALIGN_INSIDE, AUIDIALOG_ALIGN_LEFT,
		AUIDIALOG_ALIGN_OUTSIDE, AUIDIALOG_ALIGN_BOTTOM);

//	GetDialog("Win_CharHead_Prof13")->Show(pHost->GetProfession()>=102 && pHost->GetProfession()<=106);
//	GetDialog("Win_CharHead_Prof13")->AlignTo(m_pDlgHost,
//		AUIDIALOG_ALIGN_INSIDE, AUIDIALOG_ALIGN_LEFT,
//		AUIDIALOG_ALIGN_OUTSIDE, AUIDIALOG_ALIGN_BOTTOM);

/*	GetDialog("Win_NewerHelp_Min")->Show(pHost->GetBasicProps().iLevel<=15);
	GetDialog("Win_NewerHelp_Min")->AlignTo( m_pDlgSystem,
		AUIDIALOG_ALIGN_INSIDE, AUIDIALOG_ALIGN_LEFT,
		AUIDIALOG_ALIGN_OUTSIDE, AUIDIALOG_ALIGN_TOP);
*/
	// 获取玩家快捷栏敌对玩家信息
	CECShortcutSet *a_pSCS[] =
	{
		pHost->GetShortcutSet2(0),
		pHost->GetShortcutSet1(0),
		pHost->GetShortcutSet1(1),
		pHost->GetShortcutSet2(1),
		pHost->GetShortcutSet3(0),
		pHost->GetShortcutSet3(1),
		pHost->GetShortcutSet3(2),
		pHost->GetShortcutSet3(3),
	};

	int i, j;
	CECShortcut *pSC;
	for( i = 0; i < sizeof(a_pSCS) / sizeof(CECShortcutSet *); i++ )
	{
		if( !a_pSCS[i] ) continue;

		for( j = 0; j < 12; j++ )
		{
			pSC = a_pSCS[i]->GetShortcut(j);
			
			if(pSC && pSC->GetType() == CECShortcut::SCT_TARGET )
			{
				CECSCTarget* pSCTarget = (CECSCTarget*)pSC;
				int idTarget = pSCTarget->GetTargetID();
				if(pSCTarget->GetTargetType() == CECSCTarget::TT_PLAYER)
				{
					AWString name = pSCTarget->GetTargetName();
					if(name.GetLength() == 0)
					{
						g_pGame->GetGameSession()->GetPlayerBriefInfo(1, &idTarget, 2);
					}
				}
			}
		}
	}

	// xp技能栏
	AUIDialog* pDlg = GetDialog("Win_Skill_Xp");
	if (pDlg)
	{
		CECConfigs* pConfig = g_pGame->GetConfigs();
		if (pConfig->GetLocalSettings().bAutoShowSkillXp)
			pDlg->Show(true);
	}
}

void CECGameUIMan::DisplayMiniButtons()
{	
	CECHostPlayer *pHost = g_pGame->GetGameRun()->GetHostPlayer();
	bool bLocalSrv = pHost->GetSrcServerID()==0;
	//5周年签到 GetDialog("Win_AniversarySign5_Min")->Show(true);
	//诛小仙	GetDialog("Win_AniversaryPet5_Min")->Show(true);
	GetDialog("Win_FoldIcon")->AlignTo(GetDialog("Win_MiniMap"),
		AUIDIALOG_ALIGN_INSIDE, AUIDIALOG_ALIGN_RIGHT,
		AUIDIALOG_ALIGN_OUTSIDE, AUIDIALOG_ALIGN_BOTTOM);
	GetDialog("Win_ZhuxianExplore_Min")->AlignTo(GetDialog("Win_FoldIcon"),
		AUIDIALOG_ALIGN_OUTSIDE, AUIDIALOG_ALIGN_LEFT,
		AUIDIALOG_ALIGN_INSIDE, AUIDIALOG_ALIGN_TOP);
	PAUIDIALOG pDlg = GetDialog("Win_ZhuxianExplore_Min");

	// 爬塔技能界面
	GetDialog("Win_Battle_Dungeon_Tower_Lucky")->AlignTo(GetDialog("Win_FoldIcon"),
		AUIDIALOG_ALIGN_INSIDE, AUIDIALOG_ALIGN_RIGHT,
		AUIDIALOG_ALIGN_OUTSIDE, AUIDIALOG_ALIGN_BOTTOM);

	//上古挑战副本界面
	GetDialog("Win_Dungeon_Ling_Mini")->AlignTo(GetDialog("Win_FoldIcon"),
		AUIDIALOG_ALIGN_INSIDE, AUIDIALOG_ALIGN_RIGHT,
		AUIDIALOG_ALIGN_OUTSIDE, AUIDIALOG_ALIGN_BOTTOM);


	// 6v6碰撞副本
	//GetDialog("Win_Battle_Dungeon_6v6_Min")->Show(false);
	//if (bLocalSrv)
	//{
	//	GetDialog("Win_Battle_Dungeon_6v6_Min")->AlignTo(GetDialog("Win_ZhuxianExplore_Min"),
	//		AUIDIALOG_ALIGN_OUTSIDE, AUIDIALOG_ALIGN_LEFT,
	//		AUIDIALOG_ALIGN_INSIDE, AUIDIALOG_ALIGN_TOP);
	//	pDlg = GetDialog("Win_Battle_Dungeon_6v6_Min");
	//	pDlg->Show(true);
	//	GetDialog("Win_Battle_Dungeon_6v6_Wait")->AlignTo(pDlg,
	//		AUIDIALOG_ALIGN_INSIDE, AUIDIALOG_ALIGN_LEFT,
	//		AUIDIALOG_ALIGN_OUTSIDE, AUIDIALOG_ALIGN_BOTTOM);
	//	GetDialog("Win_Battle_Dungeon_6v6_MapSelect")->GetDlgItem("Txt_Wait")->Show(false);
	//	GetDialog("Win_Battle_Dungeon_6v6_MapSelect")->GetDlgItem("Btn_quit")->Show(false);
	//}
	/// 活跃值 2012-11-5 zy 
	GetDialog("Win_Activity_Min")->AlignTo(GetDialog("Win_ZhuxianExplore_Min"),
		AUIDIALOG_ALIGN_OUTSIDE, AUIDIALOG_ALIGN_LEFT,
		AUIDIALOG_ALIGN_INSIDE, AUIDIALOG_ALIGN_TOP);
	pDlg = GetDialog("Win_Activity_Min");
	


	tm tmNow = g_pGame->GetServerLocalTime();


	// 鲜花 2013-2-27 zy
// 	GetDialog("Win_Rose_Min")->Show(true);
// 	GetDialog("Win_Rose_Min")->AlignTo(pDlg,
// 		AUIDIALOG_ALIGN_OUTSIDE, AUIDIALOG_ALIGN_LEFT,
// 		AUIDIALOG_ALIGN_INSIDE, AUIDIALOG_ALIGN_TOP);
	GetDialog("Win_Rose_Min")->Show(false);
	if ( tmNow.tm_year==113 &&	//2012年
		((tmNow.tm_mon == 2 && ((tmNow.tm_mday == 26 && tmNow.tm_hour >=10) || tmNow.tm_mday > 26))
		|| tmNow.tm_mon == 3
		|| (tmNow.tm_mon == 4 && (tmNow.tm_mday < 7 || (tmNow.tm_mday == 7 && tmNow.tm_hour < 8)))))
	{
		GetDialog("Win_Rose_Min")->Show(true);
		GetDialog("Win_Rose_Min")->AlignTo(pDlg,
			AUIDIALOG_ALIGN_OUTSIDE, AUIDIALOG_ALIGN_LEFT,
			AUIDIALOG_ALIGN_INSIDE, AUIDIALOG_ALIGN_TOP);
		pDlg = GetDialog("Win_Rose_Min");
	}

	// 跨服pk竞猜
	GetDialog("Win_Kf2012Bet_Min")->Show(false);
	if ( tmNow.tm_year==112 &&	//2012年
		(tmNow.tm_mon==7 || tmNow.tm_mon==8	// 8、9月
		|| (tmNow.tm_mon==9 && tmNow.tm_mday <= 10)))	//10.10
	{
		GetDialog("Win_Kf2012Bet_Min")->Show(true);
		GetDialog("Win_Kf2012Bet_Min")->AlignTo(pDlg,
			AUIDIALOG_ALIGN_OUTSIDE, AUIDIALOG_ALIGN_LEFT,
			AUIDIALOG_ALIGN_INSIDE, AUIDIALOG_ALIGN_TOP);
		pDlg = GetDialog("Win_Kf2012Bet_Min");
	}

	// 时光邮局
	GetDialog("Win_AniversaryMail5_Min")->Show(false);
	if ( tmNow.tm_year==112 &&	//2012年
		((tmNow.tm_mon==6 && ((tmNow.tm_mday == 24 && tmNow.tm_hour >=10) || tmNow.tm_mday > 24))		//7.24
		|| tmNow.tm_mon==7																				//8
		|| (tmNow.tm_mon==8 && (tmNow.tm_mday < 28 || (tmNow.tm_mday == 28 && tmNow.tm_hour < 10)))))	//9.28
	{
		GetDialog("Win_AniversaryMail5_Min")->Show(true);
		GetDialog("Win_AniversaryMail5_Min")->AlignTo(pDlg,
			AUIDIALOG_ALIGN_OUTSIDE, AUIDIALOG_ALIGN_LEFT,
			AUIDIALOG_ALIGN_INSIDE, AUIDIALOG_ALIGN_TOP);
		pDlg = GetDialog("Win_AniversaryMail5_Min");
	}
	
	GetDialog("Win_Explorer_Popu_Min")->Show(false);
	if (IsHotWebRunning())
	{
		GetDialog("Win_Explorer_Popu_Min")->Show(true);
		GetDialog("Win_Explorer_Popu_Min")->AlignTo(pDlg,
			AUIDIALOG_ALIGN_OUTSIDE, AUIDIALOG_ALIGN_LEFT,
			AUIDIALOG_ALIGN_INSIDE, AUIDIALOG_ALIGN_TOP);
		pDlg = GetDialog("Win_Explorer_Popu_Min");
	}

	//诛仙知道
	GetDialog("Win_ZhuxianKnows_Min")->AlignTo(pDlg,
		AUIDIALOG_ALIGN_OUTSIDE, AUIDIALOG_ALIGN_LEFT,
		AUIDIALOG_ALIGN_INSIDE, AUIDIALOG_ALIGN_TOP);
	pDlg = GetDialog("Win_ZhuxianKnows_Min");
	pDlg->Show(true);

	//目的引导测试入口
	GetDialog("Win_Help_GameGuide_Min")->AlignTo(pDlg,
		AUIDIALOG_ALIGN_OUTSIDE, AUIDIALOG_ALIGN_LEFT,
		AUIDIALOG_ALIGN_INSIDE, AUIDIALOG_ALIGN_TOP);
	pDlg = GetDialog("Win_Help_GameGuide_Min");
	pDlg->Show(true);

	//国战
	GetDialog("Win_King_Min")->AlignTo(pDlg,
		AUIDIALOG_ALIGN_OUTSIDE, AUIDIALOG_ALIGN_LEFT,
		AUIDIALOG_ALIGN_INSIDE, AUIDIALOG_ALIGN_TOP);
	pDlg = GetDialog("Win_King_Min");
	pDlg->Show(true);

	//if(bLocalSrv)
	{  
		GetDialog("Win_Battle_Dungeon_6v6_Min_Expand")->AlignTo(pDlg,
			AUIDIALOG_ALIGN_OUTSIDE, AUIDIALOG_ALIGN_LEFT,
			AUIDIALOG_ALIGN_INSIDE, AUIDIALOG_ALIGN_TOP);
		pDlg = GetDialog("Win_Battle_Dungeon_6v6_Min_Expand");
		pDlg->Show(true);
	}

	//touch

/*	if(m_bShowTouchmin)
	{
		AIniFile theIni;
		if( theIni.Open("configs\\url.ini") )
		{
			AString szURL = "";
			szURL = theIni.GetValueAsString("ingame", "touch", "");
			theIni.Close();
           if( szURL != "" )
		   {
				GetDialog("Win_Game_Touch_Min")->AlignTo(pDlg,
					AUIDIALOG_ALIGN_OUTSIDE, AUIDIALOG_ALIGN_LEFT,
					AUIDIALOG_ALIGN_INSIDE, AUIDIALOG_ALIGN_TOP);
				pDlg = GetDialog("Win_Game_Touch_Min");
				pDlg->Show(true);
		   }
		}
	}
	*/
	
	GetDialog("Win_Radio_Min")->Show(false);
	if (IsHotWebRunning())
	{
		GetDialog("Win_Radio_Min")->Show(true);
		GetDialog("Win_Radio_Min")->AlignTo(pDlg,
			AUIDIALOG_ALIGN_OUTSIDE, AUIDIALOG_ALIGN_LEFT,
			AUIDIALOG_ALIGN_INSIDE, AUIDIALOG_ALIGN_TOP);
		pDlg = GetDialog("Win_Radio_Min");
	}
}

void CECGameUIMan::UpdateButtonsByTime()
{
	PAUIDIALOG pDlg = GetDialog("Win_Radio_Min");
	tm tmNow = g_pGame->GetServerLocalTime();
	if (tmNow.tm_hour>=18 && tmNow.tm_hour<21)
	{
		if (!pDlg->IsShow())
			pDlg->Show(true);
	}
	else
	{
		if (pDlg->IsShow())
			pDlg->Show(false);
	}
}

bool CECGameUIMan::IsHotWebRunning()
{
	bool bRet = false;
	AWScriptFile s;
	
	bRet = s.Open("Configs\\webhot.txt");
	if( !bRet ) return false;

	bRet = false;
	while( !s.IsEnd() )
	{
		bRet = s.GetNextToken(true);
		if( !bRet ) break;
		int y1, m1, d1, y2, m2, d2;
		a_sscanf(s.m_szToken, _AL("%d.%d.%d-%d.%d.%d"), &y1, &m1, &d1, &y2, &m2, &d2);
		CECCalendar start(y1-1900, m1-1, d1);
		CECCalendar end(y2-1900, m2-1, d2);
		CECCalendar now;
		
		bRet = s.GetNextToken(true);
		if( !bRet ) break;
		
		AUICTranslate tran;
		ACString str(tran.Translate(s.m_szToken));
		
		if (start<=now && now<=end)
		{
			GetDialog("Win_Explorer_Popu_Min")->GetDlgItem("Btn_Max")->SetHint(s.m_szToken);
			bRet = true;
			break;
		}
	}
	
	s.Close();
	return bRet;
}

void CECGameUIMan::AttachEmotion(PAUIOBJECT pObj)
{
	if (pObj->GetType()==AUIOBJECT_TYPE_TEXTAREA)
	{
		PAUITEXTAREA pText = (PAUITEXTAREA)pObj;
		pText->SetEmotionList(m_pA2DSpriteEmotion, m_vecEmotion);
		pText->SetItemMask(0xffffffff);
	}
	else if (pObj->GetType()==AUIOBJECT_TYPE_EDITBOX)
	{
		PAUIEDITBOX pEdit = (PAUIEDITBOX)pObj;
		pEdit->SetEmotionList(m_pA2DSpriteEmotion, m_vecEmotion);
	}
	else if (pObj->GetType()==AUIOBJECT_TYPE_LABEL)
	{
		PAUILABEL pLab = (PAUILABEL)pObj;
		pLab->SetEmotionList(m_pA2DSpriteEmotion, m_vecEmotion);
		pLab->SetItemMask(0xffffffff);
	}
}

void CECGameUIMan::AttachTaskEmotion(PAUIOBJECT pObj)
{
	if (pObj->GetType()==AUIOBJECT_TYPE_TEXTAREA)
	{
		PAUITEXTAREA pText = (PAUITEXTAREA)pObj;
		pText->SetEmotionList(m_pA2DSpriteEmotionTask, m_vecEmotionTask);
		pText->SetItemMask(0xffffffff);
	}
	else if (pObj->GetType()==AUIOBJECT_TYPE_EDITBOX)
	{
		PAUIEDITBOX pEdit = (PAUIEDITBOX)pObj;
		pEdit->SetEmotionList(m_pA2DSpriteEmotionTask, m_vecEmotionTask);
	}
	else if (pObj->GetType()==AUIOBJECT_TYPE_LABEL)
	{
		PAUILABEL pLab = (PAUILABEL)pObj;
		pLab->SetEmotionList(m_pA2DSpriteEmotionTask, m_vecEmotionTask);
		pLab->SetItemMask(0xffffffff);
	}
}

void CECGameUIMan::InvokeNumberDialog(PAUIOBJECT pObjSrc,
	PAUIOBJECT pObjOver, int nMode, int nMax)
{
	ACHAR szText[80];
	PAUIEDITBOX pEdit = (PAUIEDITBOX)m_pDlgInputNO->GetDlgItem("DEFAULT_Txt_No.");

	m_pDlgInputNO->Show(true, true);
	m_pDlgInputNO->SetDataPtr(pObjSrc, "ptr_AUIObject");
	m_pDlgInputNO->SetData((DWORD)pObjOver,"ptr_AUIObject");
	m_pDlgInputNO->SetType(nMode);

	pEdit->SetData(nMax);
	pEdit->SetIsNumberOnly(true);
	a_sprintf(szText, _AL("%d"), 1);
	pEdit->SetText(szText);
}

bool CECGameUIMan::IsImageHelpOpen()
{
	return !((PAUICHECKBOX)GetDialog("Win_NewerHelp")->GetDlgItem("Chk_Close"))->IsChecked();;
}

void CECGameUIMan::GetEmotionList(A2DSprite **ppA2DSprite,
	abase::vector<AUITEXTAREA_EMOTION> **ppEmotion, char cEmotion)
{
	if (ppA2DSprite)
		*ppA2DSprite = m_pA2DSpriteEmotion[cEmotion];

	if (ppEmotion)
		*ppEmotion = m_vecEmotion[cEmotion];
}

bool CECGameUIMan::PlayerIsBlack(int idPlayer)
{
	if( idPlayer > 0 )
	{
		int i;
		EC_BLACKLIST_SETTING bs = g_pGame->GetConfigs()->GetBlackListSettings();

		for( i = 0; i < EC_BLACKLIST_LEN; i++ )
		{
			if( bs.idPlayer[i] == idPlayer )
				return true;
			else if( bs.idPlayer[i] <= 0 )
				break;
		}
	}

	return false;
}

void CECGameUIMan::SetMarqueeMsg(const ACHAR *pszMsg)
{
	PAUIDIALOG pDlg = GetDialog("Win_Popmsg");
	PAUILABEL pLabel = (PAUILABEL)pDlg->GetDlgItem("Message");

	pLabel->SetText(pszMsg);
	pLabel->SetMarquee(true, 3, 40);
	if( !pDlg->IsShow() ) pDlg->Show(true, false, false);
}

void CECGameUIMan::PopupReviveDialog(bool bOpen, int revive_countdown)
{
	if( m_bReplayMode )
		return;

	if( !bOpen )
	{
		m_pDlgRevive->Show(false);
		return;
	}
	CECHostPlayer *pHost = g_pGame->GetGameRun()->GetHostPlayer();

	m_pDlgRevive->m_pBtn_Decide->Enable(true);
	if (pHost->IsInBattle() || pHost->GetKingdomWarType() != 0)
	{
		m_pDlgRevive->m_dwReviveTime = GetTickCount()+revive_countdown*1000;
	}


	if( pHost->GetBasicProps().iLevel < 10 )
		m_pDlgRevive->m_pTxt_D->SetText(GetStringFromTable(274));
	else
		m_pDlgRevive->m_pTxt_D->SetText(GetStringFromTable(a_Random(283, 292)));

/*	if( GetItemCount(3043) > 0 )
		m_pDlgRevive->m_pBtn_Back->Enable(true);
	else
		m_pDlgRevive->m_pBtn_Back->Enable(false);
*/	
	if( !m_pDlgRevive->IsShow() ) m_pDlgRevive->Show(true);
	m_bShowAllPanels = true;
}

void CECGameUIMan::AddTaskHint(int idTask, int idEvent, const ACHAR *pszHint)
{
	TASK_HINT th;
	ACString strHint;
	PAUIDIALOG pDlg = GetDialog("Win_Hint");
	ATaskTemplMan *pMan = g_pGame->GetTaskTemplateMan();
	ATaskTempl *pTemp = pMan->GetTaskTemplByID(idTask);

	if ( pTemp )
	{
		strHint.Format(GetStringFromTable(275 + idEvent), pTemp->GetName());
	}
	th.strHint = strHint;
	th.dwTime = GetTickCount();

	if( m_vecTaskHint.size() >= CECGAMEUIMAN_MAX_TASKHINT )
		m_vecTaskHint.erase(m_vecTaskHint.begin());
	m_vecTaskHint.push_back(th);

	if( !pDlg->IsShow() ) pDlg->Show(true);
	if (idEvent==TASK_MSG_NEW)
	{
		CommonMessage("Win_Battle_Dungeon_Heyang", "show", idTask, 0);
	}
	else
	{
		CommonMessage("Win_Battle_Dungeon_Heyang", "hide", idTask, 0);
	}
}

void CECGameUIMan::ChangePrecinct(CELPrecinct *pPrecinct)
{
	PAUILISTBOX pList = (PAUILISTBOX)m_pDlgMiniMapMark->GetDlgItem("List_Region");
	pList->ResetContent();

	if( !pPrecinct ) return;

	AIniFile inifile;
	if( inifile.Open("configs\\precincthint.ini") )
	{
		char szText[20];
		sprintf(szText, "%d-%d", g_pGame->GetGameRun()->GetWorld()->GetInstanceID(), pPrecinct->GetID());
		int iStringNumber = inifile.GetValueAsInt("Precinct", szText, -1);
		if( iStringNumber != -1 )
		{
			PAUIDIALOG pDlg = GetDialog("Win_PrecinctHint");
			pDlg->GetDlgItem("Txt_Hint")->SetText(GetStringFromTable(iStringNumber));
			pDlg->Show(true);
		}
		inifile.Close();
	}

	PAUILABEL pLabel;
	PAUIDIALOG pDlg = GetDialog("Win_Zone");
	pDlg->Show(true, false, false);

	pLabel = (PAUILABEL)pDlg->GetDlgItem("Txt_Hint");
	int idBattle = g_pGame->GetGameRun()->GetHostPlayer()->GetBattleInfo().idBattle;
	if( idBattle )
	{
		vector<DOMAIN_INFO> *info = globaldata_getdomaininfos();
		DWORD i;
		pLabel->SetText(_AL(""));
		for(i = 0; i < info->size(); i++ )
			if( (*info)[i].id == idBattle )
			{
				ACString szText;
				szText.Format(GetStringFromTable(756), (*info)[i].name);
				pLabel->SetText(szText);
				break;
			}
	}
	else
		pLabel->SetText(pPrecinct->GetName());
	pLabel->SetAlign(AUIFRAME_ALIGN_CENTER);
	pLabel = (PAUILABEL)pDlg->GetDlgItem("Txt_Faction");
	pLabel->SetAlign(AUIFRAME_ALIGN_CENTER);
	int idDomain = pPrecinct->GetDomainID();
	int	idFaction = 0;
// 	if( !m_pDlgGuildMap->GetMapStatus() )
// 		m_pDlgGuildMap->GetMap();
	vector<DOMAIN_INFO> *info = globaldata_getdomaininfos();
	CECFactionMan *pMan = g_pGame->GetFactionMan();
	pLabel->SetData(idDomain);
	DWORD j;
	for(j = 0; j < info->size(); j++ )
		if( (*info)[j].id == idDomain )
		{
			idFaction = (*info)[j].id_owner;
			break;
		}
	
	pLabel->SetText(_AL(""));
	if( idFaction )
	{
		Faction_Info *finfo = pMan->GetFaction(idFaction, true);
		if( finfo )
		{
			ACString szText = GetStringFromTable(710);
			ACHAR szFName[20];
			AUI_ConvertChatString(finfo->GetName(), szFName);
			pLabel->SetText(szText + szFName);
		}
	}
	m_ZoneHint.strHint = pPrecinct->GetName();
	m_ZoneHint.dwTime = GetTickCount();

	int i;
	ACHAR szText[40];
	CELPrecinct::MARK *pMark;
	for( i = 0; i < pPrecinct->GetMarkNum(); i++ )
	{
		pMark = pPrecinct->GetMark(i);
		pList->AddString(pMark->strName);
		a_sprintf(szText, _AL("%d,%d"),
			int(pMark->vPos.x), int(pMark->vPos.z));
		pList->SetItemHint(i, szText);
	}
	m_pCurPrecinct = pPrecinct;
}

void CECGameUIMan::FilterBadWords(ACString &str)
{
	int i, j, nPos;
	ACString strLwr((const ACHAR *)str);
	strLwr.MakeLower();
	
	for( i = 0; i < (int)m_vecBadWords.size(); i++ )
	{
		do
		{
			nPos = strLwr.Find(m_vecBadWords[i]);
			
			if( nPos >= 0 )
			{
				for( j = 0; j < m_vecBadWords[i].GetLength(); j++ )
				{
					strLwr[nPos + j] = '*';
					str[nPos + j] = '*';
				}
			}
		} while( nPos >= 0 );
	}
}

void CECGameUIMan::FilterImages(ACString &str)
{
	EditBoxItemsSet ItemsSet;
	ACString strOrgText = UnmarshalEditBoxText(str, ItemsSet);
	int nCount = ItemsSet.GetItemCount();
	
	if (nCount == 0)
		return ;
	
	EditBoxItemMap::iterator it = ItemsSet.GetItemIterator();
	
	for (int i = 0; i < nCount; i++)
	{
		EditBoxItemBase* pItem = it->second;
		ACHAR c = it->first;
		
		++it;
		if (pItem->GetType() == enumEIImage)
		{
			ItemsSet.DelItemByChar(c);
		}
	}
	
	str = MarshalEditBoxText(strOrgText, ItemsSet);
}

void CECGameUIMan::RefreshBrokenList()
{
	int i, j;
	bool bFound;
	CECIvtrEquip *pItem;
	CECHostPlayer *pHost = g_pGame->GetGameRun()->GetHostPlayer();
	CECInventory *pInventory = pHost->GetEquipment();

	for( i = 0; i < SIZE_EQUIPIVTR; i++ )
	{
		pItem = (CECIvtrEquip *)pInventory->GetItem(i);
		if( pItem && pItem->GetCurEndurance() <= pItem->GetMaxEndurance() / 10 )
		{
			if( m_vecBrokenList.size() >= CECGAMEUIMAN_MAX_BROKEN )
			{
				continue;
			}

			bFound = false;
			for( j = 0; j < (int)m_vecBrokenList.size(); j++ )
			{
				if( i != m_vecBrokenList[j] ) continue;

				bFound = true;
				break;
			}
			if( !bFound ) m_vecBrokenList.push_back(i);
		}
		else
		{
			for( j = 0; j < (int)m_vecBrokenList.size(); j++ )
			{
				if( i != m_vecBrokenList[j] ) continue;

				m_vecBrokenList.erase(m_vecBrokenList.begin() + j);
				break;
			}
		}
	}

	char szName[40];
	AString strFile;
	PAUIIMAGEPICTURE pImage;
	PAUIDIALOG pDlg = GetDialog("Win_Broken");

	for( i = 0; i < CECGAMEUIMAN_MAX_BROKEN; i++ )
	{
		sprintf(szName, "Slot_%d", i + 1);
		pImage = (PAUIIMAGEPICTURE)pDlg->GetDlgItem(szName);

		if( i < (int)m_vecBrokenList.size() )
		{
			pItem = (CECIvtrEquip *)pInventory->GetItem(m_vecBrokenList[i]);
			af_GetFileTitle(pItem->GetIconFile(), strFile);
			strFile.MakeLower();
			pImage->SetCover(m_pA2DSpriteIcons[ICONS_INVENTORY],
				m_IconMap[ICONS_INVENTORY][strFile]);
			pImage->SetTransparent(false);

			if( pItem->GetCurEndurance() > 0 )
			{
				pImage->SetHint(GetStringFromTable(281));
				pImage->SetColor(A3DCOLORRGB(192, 192, 0));
			}
			else
			{
				pImage->SetHint(GetStringFromTable(282));
				pImage->SetColor(A3DCOLORRGB(192, 0, 0));
			}
		}
		else
		{
			pImage->ClearCover();
			pImage->SetHint(_AL(""));
			pImage->SetTransparent(true);
			pImage->SetColor(A3DCOLORRGB(255, 255, 255));
		}
	}
}

void CECGameUIMan::PlayItemSound(CECIvtrItem *pItem, bool bDown)
{
	return;
	if( bDown )
	{
		AString strSound;
		int idSubType = -1;
		DATA_TYPE DataType;
		elementdataman *pDataMan = g_pGame->GetElementDataMan();
		const void *pData = pDataMan->get_data_ptr(
			pItem->GetTemplateID(), ID_SPACE_ESSENCE, DataType);

		if( DataType == DT_EQUIPMENT_ESSENCE )
		{
			const EQUIPMENT_ESSENCE *pEssence = (const EQUIPMENT_ESSENCE *)pData;
			idSubType = pEssence->id_sub_type;
		}

		strSound = m_IconSound[idSubType];
		if( strSound.GetLength() <= 0 )
			strSound = "SFX\\Item\\EquipSound\\_DefaultDown.wav";
		m_pA3DEngine->GetAMSoundEngine()->GetAMSoundBufferMan()->Play2DAutoSound(strSound);
	}
	else
	{
		m_pA3DEngine->GetAMSoundEngine()->GetAMSoundBufferMan()->
			Play2DAutoSound("SFX\\Item\\EquipSound\\_DefaultUp.wav");
	}
}

void CECGameUIMan::UpdateTextCoord(ACString &str)
{
	if (!str)
		return;

	ACString strText;
	ACString strOrg = str;
	while(strOrg.GetLength()>0)
	{
		if ( strOrg.GetLength()<(int)a_strlen(_AL("&location")) )
		{
			strText += strOrg;
			break;
		}
		else if ( strOrg.Left(a_strlen(_AL("&location")))==_AL("&location") )
		{
			int i;
			ACString strInfo;
			for (i=1; i<strOrg.GetLength(); ++i)
			{
				if (strOrg[i]==_AL('&'))
				{
					strInfo = strOrg.Mid(a_strlen(_AL("&location")), i-a_strlen(_AL("&location")));
					break;
				}
			}

			if (i==strOrg.GetLength())
			{
				strText += strOrg;
				break;
			}
			else
			{
				strInfo.CutLeft(1);
				strInfo.CutRight(1);
				CSplit s = strInfo;
				CSPLIT_STRING_VECTOR vec = s.Split(_AL(","));
				if (vec.size()==5)
				{
					EditBoxItemBase item(enumEICoord);
					item.SetName(vec[0]);
					ACString strPos;
					strPos = vec[1]+_AL(" ")+vec[2]+_AL(" ")+vec[3]+_AL(" ")+vec[4];
					item.SetInfo(strPos);
					item.SetColor(A3DCOLORRGB(0, 255, 0));
					strText += (ACHAR)AUICOMMON_ITEM_CODE_START + item.Serialize();
					strOrg.CutLeft(i+1);
				}
				else
				{
					strText += strOrg[0];
					strOrg.CutLeft(1);
					continue;
				}
			}
		}
		else
		{
			strText += strOrg[0];
			strOrg.CutLeft(1);
		}
	}
	
	str = strText;
}

void CECGameUIMan::RefreshHint()
{
	PAUIDIALOG pDlg = GetDialog("Win_Popmsg");
	if( pDlg->IsShow() )
	{
		PAUILABEL pLabel = (PAUILABEL)pDlg->GetDlgItem("Message");
		if( !pLabel->IsMarquee() ) pDlg->Show(false);
	}

	int i;
	ACString strText = _AL("");
	DWORD dwTime = GetTickCount();

	for( i = 0; i < (int)m_vecTaskHint.size(); i++ )
	{
		if( dwTime - m_vecTaskHint[i].dwTime < 5000 )
		{
			strText += m_vecTaskHint[i].strHint;
			strText += _AL("\r");
		}
		else
		{
			m_vecTaskHint.erase(m_vecTaskHint.begin() + i);
			i--;
		}
	}

	pDlg = GetDialog("Win_Hint");
	pDlg->GetDlgItem("Txt_Hint")->SetText(strText);
	if( m_vecTaskHint.size() <= 0 ) pDlg->Show(false);

	if( dwTime - m_ZoneHint.dwTime >= 5000 )
		GetDialog("Win_Zone")->Show(false);
	else
	{
		PAUILABEL pLabel = (PAUILABEL)GetDialog("Win_Zone")->GetDlgItem("Txt_Faction");
		if( pLabel->GetData() )
		{
			DWORD j;
			vector<DOMAIN_INFO> *info = globaldata_getdomaininfos();
			int idFaction = 0;
			for(j = 0; j < info->size(); j++ )
				if( (*info)[j].id == (int)pLabel->GetData() )
				{
					idFaction = (*info)[j].id_owner;
					break;
				}
			if( idFaction )
			{
				CECFactionMan *pMan = g_pGame->GetFactionMan();
				Faction_Info *finfo = pMan->GetFaction(pLabel->GetData());
				if( finfo )
				{
					ACString szText = GetStringFromTable(710);
					ACHAR szFName[20];
					AUI_ConvertChatString(finfo->GetName(), szFName);
					pLabel->SetText(szText + szFName);
					pLabel->SetData(0);
				}
			}
		}
	}
	((CDlgFrdCallBck*)GetDialog("Win_MailtoFriend_Success"))->RefreshHint();
}

void CECGameUIMan::RearrangeWindows(A3DRECT rcOld, A3DRECT rcNew)
{
	AUILuaManager::RearrangeWindows(rcOld, rcNew);
	if(rcOld==rcNew)
		return;
	static float windowsize[] = {0.8f,0.9f,1.0f};
	float fScale = GetWindowScale()*windowsize[m_cUISize];
	SetWindowScale(fScale);
	_InternalArrangeWindow();
}

void CECGameUIMan::AddInformation(int nType, const char *pszType,
	const ACHAR *pszMsg, DWORD dwLife, DWORD dwData1, DWORD dwData2, DWORD dwData3)
{
	if( m_bReplayMode )
		return;

	char szName[40];
	PAUIIMAGEPICTURE pImage;
	CDlgInfo::INFORMATION Info;

	if( !m_pDlgInfoIcon->IsShow() )
		m_pDlgInfoIcon->Show(true);

	Info.dwLifeBegin = GetTickCount();
	Info.dwLife = dwLife;
	Info.strMsg = pszMsg;
	Info.strType = pszType;
	Info.dwData1 = dwData1;
	Info.dwData2 = dwData2;
	Info.dwData3 = dwData3;

	if (!m_pDlgInfo->AddInfo(nType, Info))
		return;

	sprintf(szName, "Back_%d", nType);
	pImage = (PAUIIMAGEPICTURE)m_pDlgInfoIcon->GetDlgItem(szName);
	if( !pImage->IsShow() )
	{
		AMSoundBufferMan *pMan = m_pA3DEngine->GetAMSoundEngine()->GetAMSoundBufferMan();
		
		if( nType == CDlgInfo::INFO_SYSTEM )
			pMan->Play2DAutoSound(AC2AS(GetStringFromTable(570)));
		else if( nType == CDlgInfo::INFO_FRIEND )
			pMan->Play2DAutoSound(AC2AS(GetStringFromTable(571)));
	}
	pImage->Show(true);

	sprintf(szName, "F_%d", nType);
	pImage = (PAUIIMAGEPICTURE)m_pDlgInfoIcon->GetDlgItem(szName);
	pImage->Show(true);
	pImage->FadeInOut(1000);
}

void CECGameUIMan::BuildFriendList(PAUIDIALOG pDlg)
{
	m_pDlgFriendList->BuildFriendList(pDlg);
}

void CECGameUIMan::FriendAction(int idPlayer, int idGroup, int idAction, int nCode)
{
	m_pDlgFriendList->FriendAction(idPlayer, idGroup, idAction, nCode);
	if(idAction == CDlgFriendList::FRIEND_ACTION_FRIEND_UPDATE
		||idAction == CDlgFriendList::FRIEND_ACTION_FRIEND_ADD
		||idAction == CDlgFriendList::FRIEND_ACTION_FRIEND_DELETE)
	{
		m_pDlgFrdCallBck->BuildFriendsLst();
	}
}

void CECGameUIMan::ChannelAction(int idAction, void *pData)
{
	m_pDlgChannelChat->ChannelAction(idAction, pData);
}

void CECGameUIMan::MailAction(int idAction, void *pData)
{
	m_pDlgMailList->MailAction(idAction, pData);
}

void CECGameUIMan::AuctionAction(int idAction, void *pData)
{
//	m_pDlgAuctionList->AuctionAction(idAction, pData);
}

void CECGameUIMan::BattleAction(int idAction, void *pData)
{
//	m_pDlgGuildMap->BattleAction(idAction, pData);
}

void CECGameUIMan::GoldAction(int idAction, void *pData)
{
	m_pDlgGoldTrade->GoldAction(idAction, pData);
}

void CECGameUIMan::RankListAction(void *pData)
{
	m_pDlgRankList->RankListAction(pData);
}

void  CECGameUIMan::RankCrssvrTemas(void * pData)
{
	DlgCrssvrTeamsGroupRank* pDlg = (DlgCrssvrTeamsGroupRank*)GetDialog("Win_Battle_Dungeon_6v6_GroupRank");
	if(pDlg)
	{
		pDlg->UpDateGroupRank(pData);
	}
}

void CECGameUIMan::EnemyListAction(int idAction, void *pData)
{
	m_pDlgFriendList->EnemyListAction(idAction, pData);
}

void CECGameUIMan::AddHistory(int idPlayer, DWORD dwTime,
	const ACHAR *pszPlayer, const ACHAR *pszMsg)
{
	int nLen;
	FILE *file;
	char szFile[MAX_PATH];
	CECHostPlayer *pHost = g_pGame->GetGameRun()->GetHostPlayer();

	sprintf(szFile, "%s\\Userdata\\Msg\\%d\\%d.txt",
		af_GetBaseDir(), pHost->GetCharacterID(), idPlayer);
	file = fopen(szFile, "ab");
	if( !file ) return;

	fseek(file, 0, SEEK_END);

	// Player name.
	nLen = a_strlen(pszPlayer);
	fwrite(&nLen, sizeof(int), 1, file);
	fwrite(pszPlayer, sizeof(ACHAR), nLen, file);

	// Message time.
	fwrite(&dwTime, sizeof(DWORD), 1, file);

	// Message content.
	nLen = a_strlen(pszMsg);
	fwrite(&nLen, sizeof(int), 1, file);
	fwrite(pszMsg, sizeof(ACHAR), nLen, file);

	fclose(file);
}

void CECGameUIMan::PopupBoothDialog(bool bOpen, bool bOwner, bool bPrepare)
{
	if( m_bReplayMode )
		return;

	if( bOpen )
	{
		m_bShowAllPanels = true;
		m_pDlgInventory->Show(true);
		if( bOwner )
		{
			if( bPrepare )
			{
				m_pDlgBooth1->Show(true);
				m_pDlgBooth1->GetDlgItem("Btn_Reset")->Enable(true);
				m_pDlgBooth1->GetDlgItem("DEFAULT_Txt_Name")->Enable(true);
			}
			else
			{
				m_pDlgInventory->Show(false);
				m_pDlgBooth1->GetDlgItem("Btn_Cancel")->Enable(true);
				m_pDlgBooth1->GetDlgItem("Btn_Reset")->Enable(false);
				m_pDlgBooth1->GetDlgItem("Btn_Confirm")->Enable(false);
				m_pDlgBooth1->GetDlgItem("DEFAULT_Txt_Name")->Enable(false);
			}
		}
		else
		{
			ACHAR szText[40];
			ACString strTitle;
			PAUILABEL pLabel = (PAUILABEL)m_pDlgBooth2->GetDlgItem("Title");
			int idTarget = g_pGame->GetGameRun()->GetHostPlayer()->GetSelectedTarget();
			m_pDlgBooth2->SetData(idTarget,"int");
			const ACHAR *pszName = g_pGame->GetGameRun()->GetPlayerName(idTarget, true);

			AUI_ConvertChatString(pszName, szText);
			strTitle.Format(GetStringFromTable(582), szText);
			pLabel->SetAlign(AUIFRAME_ALIGN_CENTER);
			pLabel->SetText(strTitle);
			m_pDlgBooth2->Show(true);
		}
	}
	else
	{
		if( bOwner )
		{
			m_pDlgBooth1->Show(false);
			m_pDlgBooth1->OnCommand_clearmsg("clearmsg");
			PAUITEXTAREA pMsgList = (PAUITEXTAREA)m_pDlgBooth1->GetDlgItem("Txt_MsgList");
			if( pMsgList )
				pMsgList->SetText(_AL(""));
			UnfreezeAllItems();
		}
		else
		{
			m_pDlgBooth2->Show(false);
			m_pDlgInventory->Show(false);
			m_pDlgInputNO->Show(false);
		}
	}
}

void CECGameUIMan::EndNPCService()
{
	m_pCurNPCEssence = NULL;
	g_pGame->GetGameRun()->GetHostPlayer()->EndNPCService();
}

void CECGameUIMan::ChangeWorldInstance(int idInstance)
{
	m_pDlgMiniMap->ChangeWorldInstance(idInstance);
	m_pDlgMidMap->OnCommand_Search("");
}

static bool RemoveFilesAndDirectory(char *pszPath)
{
	BOOL bval;
	HANDLE hFile;
	WIN32_FIND_DATAA wfd;
	AString strFind = AString(pszPath) + AString("\\*.*");

	hFile = FindFirstFileA(strFind, &wfd);
	if( INVALID_HANDLE_VALUE == hFile ) return false;

	while( true )
	{
		if( 0 != stricmp(wfd.cFileName, ".") && 0 != stricmp(wfd.cFileName, "..") )
		{
			bval = DeleteFileA(pszPath + AString("\\") + wfd.cFileName);
			if( !bval )
			{
				FindClose(hFile);
				return false;
			}
		}

		bval = FindNextFileA(hFile, &wfd);
		if( !bval ) break;
	}

	FindClose(hFile);

	return GetLastError() == ERROR_NO_MORE_FILES ? true : false;
}

void CECGameUIMan::PopupFaceLiftDialog()
{
}

void CECGameUIMan::PopupInstanceCountDown(int nTime)
{
	PAUIDIALOG pDlg = GetDialog("Win_EnterWait");

	if( nTime > 0 )
	{
		ACString strMsg;

		pDlg->Show(true);
		pDlg->SetData(nTime);
		strMsg.Format(GetStringFromTable(599), nTime);
		pDlg->GetDlgItem("Txt_Message")->SetText(strMsg);
		if( m_pDlgHost->GetBattleState() == CDlgHost::BATTLE_STATE_INBATTLE )
			m_pDlgHost->OnCommand_battlescore("");
	}
	else
	{
		pDlg->SetData(0);
		pDlg->Show(false);
	}
}

void CECGameUIMan::PopupCrssvrTeamsInviteMenu(int idPlayer, DWORD dwHandle,void* pData)
{
	if( PlayerIsBlack(idPlayer) )
		return;

	ACHAR szName[40];
	ACString strMsg;
	Octets team_name;
	Octets role_name;
	CrssvrTeamsInviteArg* pArg = (CrssvrTeamsInviteArg*)pData;
	ACString strMemName;
	_cp_str(strMemName, pArg->team_name.begin(), pArg->team_name.size());
	ACString str;
	_cp_str(str, pArg->role_name.begin(), pArg->role_name.size());
	
	const ACHAR *pszName = g_pGame->GetGameRun()->GetPlayerName(idPlayer, true);
	AUI_ConvertChatString(pszName, szName);
	strMsg.Format(GetStringFromTable(20010), str, strMemName);
	AddInformation(CDlgInfo::INFO_SYSTEM, "Game_CrssvrTeamsInvite",
		strMsg, 30000, idPlayer, dwHandle, 0);
}

void CECGameUIMan::PopupFactionInviteMenu(int idPlayer,
	const ACHAR *pszFaction, DWORD dwHandle)
{
	if( PlayerIsBlack(idPlayer) )
		return;

	ACHAR szName[40];
	ACString strMsg;

	FactionInvite* p = (FactionInvite*)dwHandle;
	if( static_cast<FactionInviteArg*>(p->GetArgument())->scale == 0 )
	{
		const ACHAR *pszName = g_pGame->GetGameRun()->GetPlayerName(idPlayer, true);
		AUI_ConvertChatString(pszName, szName);
		strMsg.Format(GetStringFromTable(602), szName, pszFaction);
		AddInformation(CDlgInfo::INFO_SYSTEM, "Game_InviteFaction",
			strMsg, 30000, idPlayer, dwHandle, 0);
	}
	else
	{
		const ACHAR *pszName = g_pGame->GetGameRun()->GetPlayerName(idPlayer, true);
		AUI_ConvertChatString(pszName, szName);
		strMsg.Format(GetStringFromTable(861), szName, pszFaction);
		AddInformation(CDlgInfo::INFO_SYSTEM, "Game_InviteFamily",
			strMsg, 30000, idPlayer, dwHandle, 0);
	}
}
void CECGameUIMan::PopupKingGuardTransmit(const ACHAR *pszKing,DWORD dwHandle)
{
	ACString strMsg;
	strMsg.Format(GetStringFromTable(15653),pszKing);
    AddInformation(CDlgInfo::INFO_SYSTEM,"Game_KingTransmit",strMsg,30000,dwHandle,0,0);
}
void CECGameUIMan::PopupKingBathInvite(const ACHAR *pszName,int who)
{
   ACString strMsg;
   strMsg.Format(GetStringFromTable(15663),pszName);
   AddInformation(CDlgInfo::INFO_SYSTEM,"Game_KingBathInvite",strMsg,0xFFFFFFFF,who,0,0);
}
void CECGameUIMan::PopupKingOpenBathInvite(DWORD dwHandle)
{
    AddInformation(CDlgInfo::INFO_SYSTEM,"Game_KingOpenBathInvite",GetStringFromTable(15655),30000,dwHandle,0,0);
}
void CECGameUIMan::PopupKingBathFlyToNpc()
{
    AddInformation(CDlgInfo::INFO_SYSTEM,"Game_KingOpenBathFlyToNpc",GetStringFromTable(15664),60*60*1000,0,0,0);
}
void CECGameUIMan::PopupSectInviteMenu(int idPlayer, DWORD dwHandle)
{
	if( PlayerIsBlack(idPlayer) )
		return;

	ACHAR szName[40];
	ACString strMsg;

	SectInvite* p = (SectInvite*)dwHandle;
	const ACHAR *pszName = g_pGame->GetGameRun()->GetPlayerName(idPlayer, true);
	AUI_ConvertChatString(pszName, szName);
	strMsg.Format(GetStringFromTable(998), szName);
	AddInformation(CDlgInfo::INFO_SYSTEM, "Game_InviteSect",
		strMsg, 30000, idPlayer, dwHandle, 0);
}

void CECGameUIMan::PopupFactionCombatMenu(const ACHAR *pszFaction, DWORD dwHandle)
{
	ACString strMsg;
	strMsg.Format(GetStringFromTable(826), pszFaction);
	AddInformation(CDlgInfo::INFO_SYSTEM, "Game_CombatInviteRe",
		strMsg, 0xFFFFFFFF, dwHandle, 0, 0);
}

void CECGameUIMan::UpdateFactionList(bool bFaction)
{
	if( bFaction || m_pDlgGuildMan->IsShow() )
		m_pDlgGuildMan->BuildFactionList();
	else
		m_pDlgFamilyMan->BuildFactionList();
}

void CECGameUIMan::UpdateChannelList(void* pChannelList)
{
#ifdef LUA_UI
	ChatRoomList_Re *p = (ChatRoomList_Re *)pChannelList;
	LuaBind::Push(LuaBind::TypeWrapper<int>(), m_pLuaState->GetState(), LuaBind::CHATROOM_LIST_COMING);
	LuaBind::Push(LuaBind::TypeWrapper<int>(), m_pLuaState->GetState(), p->sum);
	m_pState->Call(m_pLuaState->GetState(), LUA_GAMEUIMAN, "OnEventMap", 2);
	DWORD i;
	for( i = 0; i < p->rooms.size(); i++ )
	{
		GNET::GChatRoom room = p->rooms[i];
		ACString strSubject((const ACHAR*)room.subject.begin(), room.subject.size() / sizeof (ACHAR));
		ACString strOwner((const ACHAR*)room.owner.begin(), room.owner.size() / sizeof (ACHAR));
		LuaBind::Push(LuaBind::TypeWrapper<int>(), m_pLuaState->GetState(), LuaBind::CHATROOM_INFO);
		LuaBind::Push(LuaBind::TypeWrapper<int>(), m_pLuaState->GetState(), room.roomid); //id
		LuaBind::Push(LuaBind::TypeWrapper<bool>(), m_pLuaState->GetState(), (room.status & 1) == 1); //lock
		LuaBind::Push(LuaBind::TypeWrapper<const char *>(), m_pLuaState->GetState(), AC2AS(strSubject)); //subject
		LuaBind::Push(LuaBind::TypeWrapper<const char *>(), m_pLuaState->GetState(), AC2AS(strOwner)); //owner
		LuaBind::Push(LuaBind::TypeWrapper<int>(), m_pLuaState->GetState(), room.number); //number
		LuaBind::Push(LuaBind::TypeWrapper<int>(), m_pLuaState->GetState(), room.capacity); //capacity
		m_pState->Call(m_pLuaState->GetState(), LUA_GAMEUIMAN, "OnEventMap", 7);
	}
	
#else
	m_pDlgChannelJoin->BuildChannelList(pChannelList);
#endif
}

A2DSprite* CECGameUIMan::GetFactionIcon(int idServer, int idFaction, int *pIndex)
{
	abase::hash_map<int, A2DSprite*>::iterator it = m_mapA2DSpriteGuildIcons.find(idServer);
	if( it == m_mapA2DSpriteGuildIcons.end() )
	{
		AFileImage fi;
		A3DRECT *rc;
		char szFile[200];
		char szLine[200];
		A2DSprite *sprite;
		bool bval;
		int nIndex, i, j;
		DWORD dwRead;

		sprintf(szFile, "%s\\Surfaces\\IconSet\\IconList_Guild%d.txt",
			af_GetBaseDir(), idServer);
		bval = af_IsFileExist(szFile) ? fi.Open(szFile, AFILE_OPENEXIST | AFILE_TEXT | AFILE_TEMPMEMORY) : false;
		if( !bval )
		{
			*pIndex = 0;
			return m_pA2DSpriteIcons[ICONS_GUILD];
		}

		int W, H, CountX, CountY;
		fi.ReadLine(szLine, sizeof(szLine), &dwRead);
		W = atoi(szLine);

		fi.ReadLine(szLine, sizeof(szLine), &dwRead);
		H = atoi(szLine);

		fi.ReadLine(szLine, sizeof(szLine), &dwRead);
		CountY = atoi(szLine);

		fi.ReadLine(szLine, sizeof(szLine), &dwRead);
		CountX = atoi(szLine);

		rc = new A3DRECT[CountX * CountY];
		if( !rc )
		{
			*pIndex = 0;
			return m_pA2DSpriteIcons[ICONS_GUILD];
		}

		for( i = 0; i < CountY; i++ )
		{
			for( j = 0; j < CountX; j++ )
			{
				nIndex = i * CountX + j;
				rc[nIndex].SetRect(j * W, i * H, j * W + W, i * H + H);

				bval = fi.ReadLine(szLine, sizeof(szLine), &dwRead);
				if( dwRead > 0 && strlen(szLine) > 0 )
					m_IconMap[ICONS_GUILD][AString(szLine)] = nIndex;
			}
		}

		fi.Close();

		sprite = new A2DSprite;
		if( !sprite )
		{
			*pIndex = 0;
			return m_pA2DSpriteIcons[ICONS_GUILD];
		}

		sprintf(szFile, "IconSet\\IconList_Guild%d.dds", idServer);
		char szText[200];
		sprintf(szText, "Surfaces\\IconSet\\IconList_Guild%d.dds", idServer);
		bval = af_IsFileExist(szText) ? sprite->Init(m_pA3DDevice, szFile, AUI_COLORKEY) : false;
		if( !bval )
		{
			*pIndex = 0;
			delete sprite;
			return m_pA2DSpriteIcons[ICONS_GUILD];
		}
		sprite->ResetItems(CountX * CountY, rc);
		m_mapA2DSpriteGuildIcons[idServer] = sprite;
		delete []rc;
	}
	AString strIcon;
	strIcon.Format("%d_%d.dds", idServer, idFaction);
	if( m_IconMap[ICONS_GUILD].find(strIcon) != m_IconMap[ICONS_GUILD].end() )
	{
		*pIndex = m_IconMap[ICONS_GUILD][strIcon];
		if( *pIndex < m_mapA2DSpriteGuildIcons[idServer]->GetItemNum() )
			return m_mapA2DSpriteGuildIcons[idServer];
		else
		{
			*pIndex = 0;
			return m_pA2DSpriteIcons[ICONS_GUILD];
		}
	}

	*pIndex = 0;
	return m_pA2DSpriteIcons[ICONS_GUILD];
}

void CECGameUIMan::SetIvtrInfo(PAUIIMAGEPICTURE pImg, int id, int num, bool bClearInfo, bool setHint)
{
	CECIvtrItem* pItem = id==0 ? 0 : CECIvtrItem::CreateItem(id, 0, num);
	if (pItem)
	{
		if (bClearInfo)
			pItem->GetDetailDataFromLocal();
		CECHostPlayer *pHost = g_pGame->GetGameRun()->GetHostPlayer();
		AUICTranslate trans;
		AString strFile;
		af_GetFileTitle(pItem->GetIconFile(), strFile);
		strFile.MakeLower();
		pImg->SetCover(m_pA2DSpriteIcons[CECGameUIMan::ICONS_INVENTORY],
			m_IconMap[CECGameUIMan::ICONS_INVENTORY][strFile]);
		if (setHint)
		{
			const wchar_t* szDesc = pItem->GetDesc();
			if( szDesc )
				pImg->SetHint(trans.Translate(szDesc));
			else
				pImg->SetHint(pItem->GetName());
		}
		
		if (num>1)
		{
			ACString strText;
			strText.Format(_AL("%d"), num);
			pImg->SetText(strText);
		}
		else
			pImg->SetText(_AL(""));
		delete pItem;
	}
	else
	{
		pImg->SetCover(NULL, -1);
		pImg->SetText(_AL(""));
		pImg->SetHint(_AL(""));
	}
}

void CECGameUIMan::SetIvtrInfo(PAUIIMAGEPICTURE pImg, CECIvtrItem* pItem, int num, bool setHint)
{
	if (!pItem)
	{
		pImg->ClearCover();
		pImg->SetText(_AL(""));
		pImg->SetHint(_AL(""));
	}
	AUICTranslate trans;
	AString strFile;
	af_GetFileTitle(pItem->GetIconFile(), strFile);
	strFile.MakeLower();
	pImg->SetCover(m_pA2DSpriteIcons[CECGameUIMan::ICONS_INVENTORY],
		m_IconMap[CECGameUIMan::ICONS_INVENTORY][strFile]);
	if (num>1)
	{
		ACString strText;
		strText.Format(_AL("%d"), num);
		pImg->SetText(strText);
	}
	else
		pImg->SetText(_AL(""));
	if (setHint)
	{
		const wchar_t* szDesc = pItem->GetDesc();
		if( szDesc )
			pImg->SetHint(trans.Translate(szDesc));
		else
			pImg->SetHint(pItem->GetName());
	}
}

void CECGameUIMan::SetSkillInfo(AUIImagePicture* pImg, int id, int level, bool bDelete)
{
	CECSkill* pSkill;
	if (!bDelete && pImg->GetDataPtr())
	{
		CECSkill* pSkillOld = (CECSkill*)pImg->GetDataPtr();
		delete pSkillOld;
		pImg->SetDataPtr(NULL);
	}

	pSkill = new CECSkill(id, level);
	if (pSkill && id>0)
	{
		AString strFile;
		af_GetFileTitle(pSkill->GetIconFile(), strFile);
		strFile.MakeLower();
		int ItemIndex;
		A2DSprite* pSprite = GetSpriteIcon(CECGameUIMan::ICONS_SKILL, strFile.GetBuffer(0), &ItemIndex);
		pImg->SetDataPtr(pSkill);
		pImg->SetCover(pSprite, ItemIndex);
		pImg->SetHint(pSkill->GetDesc());
		if (bDelete)
		{
			delete pSkill;
		}
	}
	else
	{
		pImg->SetDataPtr(NULL);
		pImg->SetCover(NULL, -1);
		pImg->SetHint(_AL(""));
	}
}

bool CECGameUIMan::SetImgIconSet(PAUIIMAGEPICTURE pImg, int type, const char *szName)
{
	if (!pImg || !szName)
		return false;
	
	abase::hash_map<AString, int>::iterator it = m_IconMap[type].find(szName);
	if( it != m_IconMap[type].end() )
		pImg->SetCover(m_pA2DSpriteIcons[type], it->second);
	else
		return false;
	return true;
}

A2DSprite* CECGameUIMan::GetSpriteIcon(int type, const char *szName, int *pIndex)
{
	int nIndex;
	abase::hash_map<AString, int>::iterator it = m_IconMap[type].find(szName);
	if( it != m_IconMap[type].end() )
		nIndex = it->second;
	else
		nIndex = 0;
	if( pIndex )
		*pIndex = nIndex;
	return m_pA2DSpriteIcons[type];
}

void CECGameUIMan::SetTempTargetInMiniMap(const A3DVECTOR3& vecPos)
{
	m_pDlgMiniMap->m_vecTempTarget = vecPos;
	m_pDlgMiniMap->m_dwTempTargetTime = GetTickCount();
	m_pDlgMiniMap->m_idTempTargetInstance = g_pGame->GetGameRun()->GetWorld()->GetInstanceID();
	g_pGame->GetGameRun()->GetHostPlayer()->GetModelTempArrow()->PlayActionByName("动画", 1.0f, true);
}

A3DCOLOR CECGameUIMan::GetItemColor(CECIvtrItem* pItem)
{
	if( pItem->IsFrozen() )
		return A3DCOLORRGB(128, 128, 128);
	else if( pItem->IsEquipment() )
	{
		CECIvtrEquip* pEquip = (CECIvtrEquip *)pItem;
		if( !g_pGame->GetGameRun()->GetHostPlayer()->CanUseEquipment(pEquip) )
			return A3DCOLORRGB(192, 0, 0);
		else
		{
			bool bUseOut = pEquip->GetCurEndurance() ? false : true;
			if (bUseOut)
				return A3DCOLORRGB(128, 128, 128);
			else
				return A3DCOLORRGB(255, 255, 255);
		}
	}
	else if( pItem->IsUseable() && !pItem->CheckUseCondition() )
		return A3DCOLORRGB(192, 0, 0);
	else if( pItem->GetClassID() == CECIvtrItem::ICID_LOTTERY )
	{
		CECIvtrLottery* pLottory = (CECIvtrLottery*)pItem;
		if( pLottory->GetEssence().dice_data[0] != 0 )
			return A3DCOLORRGB(255, 64, 64);
		else
			return A3DCOLORRGB(255, 255, 255);
	}
	else if( pItem->GetClassID() == CECIvtrItem::ICID_MEDICINE )
	{
		CECIvtrMedicine *pMedicine = (CECIvtrMedicine*)pItem;
		if( (pMedicine->GetDBEssence()->type == 0 || pMedicine->GetDBEssence()->type == 2) && pMedicine->GetEssence().hp < pMedicine->GetDBEssence()->hp[0] || 
			(pMedicine->GetDBEssence()->type == 1 || pMedicine->GetDBEssence()->type == 2) && pMedicine->GetEssence().mp < pMedicine->GetDBEssence()->mp[0] || 
			(pMedicine->GetDBEssence()->type == 6) && pMedicine->GetEssence().dp < pMedicine->GetDBEssence()->dp[0] )
			return A3DCOLORRGB(255, 64, 64);
		else
			return A3DCOLORRGB(255, 255, 255);
	}
	else if( pItem->GetClassID() == CECIvtrItem::ICID_TALISMAN_ENERGYFOOD )
	{
		CECIvtrTalismanEnergyFood *pFood = (CECIvtrTalismanEnergyFood*)pItem;
		if( pFood->GetEssence().energy_left < pFood->GetDBEssence()->energy_total )
			return A3DCOLORRGB(255, 64, 64);
		else
			return A3DCOLORRGB(255, 255, 255);
	}
	else if( pItem->GetClassID() == CECIvtrItem::ICID_PET_AUTO_FOOD )
	{
		CECPetAutoFood *pFood = (CECPetAutoFood*)pItem;
		if( pFood->GetCurValue() < pFood->GetDBEssence()->food_value )
			return A3DCOLORRGB(255, 64, 64);
		else
			return A3DCOLORRGB(255, 255, 255);
	}
	return A3DCOLORRGB(255, 255, 255);
}

ACString CECGameUIMan::GetLine(int idLine, bool bNumber)
{
	if (bNumber)
	{
		ACString strLine;
		strLine.Format(GetStringFromTable(357), idLine);
		return strLine;
	}
	if( idLine <= 0 )
		return GetStringFromTable(813);
	else if( idLine >= 100 && idLine < 200 )
		return GetStringFromTable(1312);
	else if( idLine >= 200 && idLine < 300 )
		return GetStringFromTable(1313);
	else if(idLine == 500)
		return GetStringFromTable(17500);
	CECGameRun::LineInfoVec lineinfo = g_pGame->GetGameRun()->GetLinesInfo();
	int j;
	for(j = 0; j < (int)lineinfo.size(); j++ )
		if( lineinfo[j].id == idLine )
			return lineinfo[j].name;
	return GetStringFromTable(574);
}

A2DSprite* CECGameUIMan::LoadTitleImage(AString strName, int iframenum)
{
	abase::hash_map<AString, A2DSprite*>::iterator it = m_mapA2dSpriteTitle.find(strName);
	if(it == m_mapA2dSpriteTitle.end())
	{
		const char* szimagefile = strName;
		A3DRECT rc[20];
		A2DSprite* pSprite = new A2DSprite;
		if(pSprite && pSprite->Init(g_pGame->GetA3DDevice(), const_cast<char*>(szimagefile), 0))
		{
			int W = pSprite->GetWidth();
			int H = pSprite->GetHeight();
			int iWidth = W / iframenum;

			for(int j =0; j < iframenum; j++)
				rc[j] = A3DRECT(j * iWidth, 0, j * iWidth + iWidth, H);							
			pSprite->ResetItems(iframenum, rc);
			m_mapA2dSpriteTitle[strName] = pSprite;
			return pSprite;
		}
		else
			return NULL;
	}
	else
		return it->second;
}

ACString CECGameUIMan::GetCompareEquipDesc(CECIvtrItem *pItem)
{
	EC_GAME_SETTING gs = g_pGame->GetConfigs()->GetGameSettings();
	if( gs.bEquipCompare )
		return _AL("");

	ACString strHint = _AL("");
	AUICTranslate trans;
	if( pItem->IsEquipment() )
	{
		CECIvtrEquip *pEquip = (CECIvtrEquip*)pItem;
		CECInventory *pEquipPack = g_pGame->GetGameRun()->GetHostPlayer()->GetEquipment();
		for(int i = 0; i < SIZE_EQUIPIVTR; i++)
		{
			CECIvtrEquip *pEquip1 = (CECIvtrEquip*)pEquipPack->GetItem(i);
			if( pEquip1 && (pEquip1->GetEquipMask() & pEquip->GetEquipMask()) )
			{
				const wchar_t *pszHint1 = pEquip1->GetDesc();
				if( pszHint1 )
				{
					strHint += GetStringFromTable(997);
					strHint += trans.Translate(pszHint1);
				}
				else
				{
					g_pGame->GetGameRun()->GetHostPlayer()->GetIvtrItemDetailData(IVTRTYPE_EQUIPPACK, i);
					return _AL("");
				}
			}
		}
	}
	else if( pItem->GetClassID() == CECIvtrItem::ICID_PET_ARMOR )
	{
		CECPetArmor *pEquip = (CECPetArmor*)pItem;
		CECInventory *pPetEquipPack = g_pGame->GetGameRun()->GetHostPlayer()->GetPetEquipPack();
		for(int i = 0; i < IVTRSIZE_PETEQUIP; i++)
		{
			CECPetArmor *pEquip1 = (CECPetArmor*)pPetEquipPack->GetItem(i);
			if( pEquip1 && (pEquip1->GetDBEssence()->equip_location == pEquip->GetDBEssence()->equip_location) )
			{
				const wchar_t *pszHint1 = pEquip1->GetDesc();
				if( pszHint1 )
				{
					strHint += GetStringFromTable(997);
					strHint += trans.Translate(pszHint1);
				}
				else
				{
					g_pGame->GetGameRun()->GetHostPlayer()->GetIvtrItemDetailData(IVTRTYPE_PET_EQUIP, i);
					return _AL("");
				}
			}
		}
	}
	else if( pItem->GetClassID() == CECIvtrItem::ICID_PET_BEDGE )
	{
		CECInventory *pPetPack = g_pGame->GetGameRun()->GetHostPlayer()->GetPetPack();
		for(int i = 0; i < pPetPack->GetSize(); i++)
		{
			CECPetBedge *pPet = (CECPetBedge*)pPetPack->GetItem(i);
			if( pPet && pPet->GetEssence().main_status == 0 )
			{
				const wchar_t *pszHint1 = pPet->GetDesc();
				if( pszHint1 )
				{
					strHint += GetStringFromTable(997);
					strHint += trans.Translate(pszHint1);
				}
				else
				{
					g_pGame->GetGameRun()->GetHostPlayer()->GetIvtrItemDetailData(IVTRTYPE_PET, i);
					return _AL("");
				}
			}
		}
	}

	return strHint;
}

void CECGameUIMan::UnfreezeAllItems()
{
	GetDialog("Win_Lottery")->Show(false);
	GetDialog("Win_Lottery2")->Show(false);
	GetDialog("Win_Lottery3")->Show(false);
	CECHostPlayer *pHost = g_pGame->GetGameRun()->GetHostPlayer();
	pHost->GetPack()->UnfreezeAllItems();
}

void CECGameUIMan::GetItemEmphase(int id)
{
	m_pDlgInventory->EmphasizeItem(id);
}

void CECGameUIMan::AddImageListItem(A2DSprite* pSprite, int& index)
{
	index = m_vecImageList.size();
	m_vecImageList.push_back(pSprite);
}

void CECGameUIMan::Int64ToStr(INT64 num, ACString& str)
{
	str.Empty();
	INT64 temp = num;
	bool bNeg = false;;
	if (temp==0)
	{
		str.Format(_AL("%d"),0);
		return;
	}
	else if (temp<0)
	{
		bNeg = true;
		temp = -temp;
	}
	while (temp>0)
	{
		int b = (int)(temp%10);
		ACHAR szText[2];
		a_sprintf(szText,_AL("%d"), b);
		str = szText + str;
		temp/=10;
	}
	if (bNeg)
		str = _AL("-") + str;
}

AString CECGameUIMan::URLEncode(const ACHAR *src, UINT CodePage)
{
	char c[300];
	WideCharToMultiByte(CodePage,0,src,-1,c,300,NULL,NULL);
	AString des="";
	int len=strlen(c);
	for(int i=0;i<len;i++)
	{
		if(c[i]==' ')
			des+='+';
		else if(c[i]>='0'&&c[i]<='9'||
			c[i]>='A'&&c[i]<='Z'||
			c[i]>='a'&&c[i]<='z')
			des+=c[i];
		else
		{
			des+='%';
			char p;
			p=(c[i]>>4)&15;
			if(p<10)
				p+='0';
			else
				
				p+='A'-10;
			des+=p;
			p=c[i]&15;
			if(p<10)
				p+='0';
			else
				p+='A'-10;
			des+=p;
		}
	}
	return des;
}

ACString CECGameUIMan::URLDecode(const char *src, UINT CodePage)
{
	ACString des;
	int len = strlen(src);
	int lend = 0;
	char *dest = new char[len+1];
	char *target = dest;
	for (int i=0; i<len; i++)
	{
		if(src[i]=='+')
		{
			*target++ = ' ';
			lend++;
		}
		else if(src[i]>='0'&&src[i]<='9'||
			src[i]>='A'&&src[i]<='Z'||
			src[i]>='a'&&src[i]<='z')
		{
			*target++ = src[i];
			lend++;
		}
		else if (src[i]=='%')
		{
			if (i+2>=len)
			{
				return ACString(_AL(""));
			}
			int h, l;
			i++;
			if (src[i]>='0'&&src[i]<='9')
				h = src[i]-'0';
			else if (src[i]>='A'&&src[i]<='F')
				h = src[i]-'A'+10;
			else
				return ACString(_AL(""));
			
			i++;
			if (src[i]>='0'&&src[i]<='9')
				l = src[i]-'0';
			else if (src[i]>='A'&&src[i]<='F')
				l = src[i]-'A'+10;
			else
				return ACString(_AL(""));
			*target++ = (h<<4) + l;
			lend++;
		}
	}

	ACHAR szDest[1024];

	int nLength = MultiByteToWideChar(CodePage, 0, dest, lend, szDest, 0);
	MultiByteToWideChar(CodePage, 0, dest, lend, szDest, nLength);
	szDest[nLength] = 0;
	des = szDest;
	return des;
}

void CECGameUIMan::FormatMoney(int money, ACString& str)
{
	str.Empty();
	ACString strTemp;
	if (money/10000 > 0)
	{
		strTemp.Format(GetStringFromTable(1701), money/10000);
	}
	str += strTemp;
	strTemp.Empty();
	if ((money%10000)/100 > 0)
	{
		strTemp.Format(GetStringFromTable(1702), (money%10000)/100);
	}
	str += strTemp;
	strTemp.Empty();
	if (money%100 > 0)
	{
		strTemp.Format(GetStringFromTable(1703), money%100);
	}
	str += strTemp;
}

//弹出好友召回奖励提示
void CECGameUIMan::PopupFriendCallBckSnderAward(const void *str, int len){
	ACString name, msg;
	_cp_str(name, str, len);
	msg.Format(GetStringFromTable(10520),name);
	AddInformation(CDlgInfo::INFO_SYSTEM,
			"Game_OK", msg, 0xFFFFFFF, 0, 0, 0);
}

void CECGameUIMan::PopupFriendCallBckBckerAward(){
	AddInformation(CDlgInfo::INFO_SYSTEM,
			"Game_OK", GetStringFromTable(10521), 0xFFFFFFF, 0, 0, 0);
}

void CECGameUIMan::StartFrdCallBck(int mailSent,char is_scr){
	m_pDlgFrdCallBck->SetMailNum(mailSent);
	((AUICheckBox*)  GetDialog("Win_MailList")->GetDlgItem("Chk_RefuseToAccept"))->Check(!is_scr);
}

void CECGameUIMan::OnFrdCallBckRe(int retcode){
	m_pDlgFrdCallBck->IncreaseMailNum();
	((CDlgFrdCallBck*)GetDialog("Win_MailtoFriend_Success"))->IncreaseMailNum();
	m_pDlgFrdCallBck->BuildFriendsLst();
}

void CECGameUIMan::PlayAwardGfx(unsigned long ulGfxID)
{
	if(ulGfxID >=0 && ulGfxID <=19)
	{
		DATA_TYPE DataType;
		TASK_SPECIAL_AWARD_CONFIG *pData = (TASK_SPECIAL_AWARD_CONFIG *)g_pGame->GetElementDataMan()->get_data_ptr(738, ID_SPACE_CONFIG, DataType);
		if(pData || DataType == DT_TASK_SPECIAL_AWARD_CONFIG)
		{
			const char* szFile;
			szFile = g_pGame->GetDataPath(pData->gfx_path[ulGfxID]);
			szFile += 4 * sizeof(char);     //过滤掉特效路径中的 gfx
			PAUIIMAGEPICTURE pGfx = (PAUIIMAGEPICTURE)GetDialog("Win_WindowGfx")->GetDlgItem("Img_Gfx");
			if(pGfx)
				pGfx->SetGfx(szFile);
			GetDialog("Win_WindowGfx")->Show(true);
		}
	}
}

void CECGameUIMan::PlayTinyGame(unsigned long ulGameID)
{
	if(ulGameID >= 1 && ulGameID <= 1000)
		((CDlgGameSnake*)GetDialog("GameSnake"))->StartStage(ulGameID);
	else if(ulGameID == 4)
		GetDialog("Win_SpringFestival2012")->Show(true);
	else if(ulGameID >= 2001 && ulGameID <= 3000 )
	{
		GetDialog("Win_Game_Acupoint")->Show(true);
		( (CDlgGameAcupoint*) GetDialog("Win_Game_Acupoint") )->StartStage( ulGameID );
	}
	else if (ulGameID == 3001)	// 猜拳
		GetDialog("Win_Game_Guess_Start")->Show(true);
}

bool CECGameUIMan::OpenQShop( const char *pszWinName )
{
	AString keyword = pszWinName;
	AWString searchKey = m_QshopKeywordMap[ keyword ];
	if ( searchKey.IsEmpty( ) )
		return false;

	( (CDlgQShop*) GetDialog( "Win_QShop" ) )->OpenQShop( searchKey );
	
	return true;
}

static DWORD WINAPI OpenUrl(void * ptr)
{
	AString url = *((AString*)ptr);
	delete ptr;
	ptr = NULL;
	ShellExecuteA(NULL,"open", url, NULL, NULL, SW_NORMAL);
	return 0;
}
void CECGameUIMan::OpenUrl(const AString& url)
{
	DWORD dwThreadID;
    AString* pStr = NULL;
	pStr = new AString(url);
	if(!pStr) return;
    CreateThread(NULL, 0, ::OpenUrl, (LPVOID)pStr, 0, &dwThreadID);
}
bool CECGameUIMan::CheckCanTaskFly(int idTask,int idNpc)
{
   CECHostPlayer *pHost = g_pGame->GetGameRun()->GetHostPlayer();
   if(pHost->GetBathType() != -1)
   {
	   MessageBox(15683);
       return false;
   }
   int iNum = pHost->GetPack()->GetItemTotalNum(idFlyItem);
   if (iNum>0 || pHost->GetKingdomTitle() > 0)
   {
	   PAUIDIALOG pMsgBox;
	   int strid = pHost->GetKingdomTitle() > 0 ? 15638 : 6015;
	   MessageBox("Game_TaskFly", GetStringFromTable(strid), 
		   MB_YESNO, A3DCOLORRGBA(255, 255, 255, 160), &pMsgBox);
	   pMsgBox->SetData(idTask);
	   pMsgBox->SetDataPtr((void*)idNpc);
	}
	else
	{
	   MessageBox(9464);
	   return false;
	}
   return true;
}

bool CECGameUIMan::CheckTaskReachSiteFly(int idTask)
{
	CECHostPlayer *pHost = g_pGame->GetGameRun()->GetHostPlayer();
	if(pHost->GetBathType() != -1)
	{
		MessageBox(15683);
		return false;
	}

	int iNum = pHost->GetPack()->GetItemTotalNum(idFlyItem);
	if(iNum > 0 || pHost->GetKingdomTitle() > 0)
	{
		PAUIDIALOG pMsgBox;
		MessageBox("Game_TaskReachSiteFly", GetStringFromTable(6015), 
			MB_YESNO, A3DCOLORRGBA(255, 255, 255, 160), &pMsgBox);
		pMsgBox->SetData(idTask);
	}
	else
	{
		MessageBox(9464);
		return false;
	}

	return true;
}

bool CECGameUIMan::CheckCanDailyPrompFly(int id)
{
	CECHostPlayer *pHost = g_pGame->GetGameRun()->GetHostPlayer();
	if(pHost->GetBathType() != -1)
	{
		MessageBox(15683);
		return false;
	}
    int iNum = pHost->GetPack()->GetItemTotalNum(idFeitian);
	if (iNum>0)
	{
		PAUIDIALOG pMsgBox;
		MessageBox("Game_DailyPrompFly",GetStringFromTable(6015), 
			MB_YESNO, A3DCOLORRGBA(255, 255, 255, 160), &pMsgBox);
		pMsgBox->SetData(id);
	}
	else
	{
		MessageBox(9464);
		return false;
	}
	return true;
}

bool CECGameUIMan::LoadShortCutsUIInfo()
{
	abase::vector<CScriptValue> args;
	abase::vector<CScriptValue> results;

	if (!m_pState->Call(SHORTCUTS_UI_INFO, "GetSelf", args, results))
	{
		return false;
	}

	CScriptValue& ret0 = results[0];

	unsigned int i, j;
	for (i = 0; i < ret0.m_ArrVal.size(); i++)
	{
		abase::vector<CScriptValue>& ret0ik = ret0.m_ArrVal[i].m_ArrKey;
		abase::vector<CScriptValue>& ret0iv = ret0.m_ArrVal[i].m_ArrVal;
		int iLk;
		AString strDlg;
		AString strControl;
		bool bHint;
		bool bText;
		AWString strPreHint;
		AWString strPostHint;
		AWString strPreText;
		AWString strPostText;

		for (j = 0; j < ret0ik.size(); j++)
		{
			CScriptValue& ret0ijk = ret0ik[j];
			if (ret0ijk.m_Type != CScriptValue::SVT_STRING)
			{
				return false;
			}
			AString key; 
			ret0ijk.RetrieveAString(key);
			if (strcmp(key, "lk") == 0)
			{
				iLk = ret0iv[j].GetInt();
			}
			else if (strcmp(key, "dlg") == 0)
			{
				ret0iv[j].RetrieveAString(strDlg);
			}
			else if (strcmp(key, "control") == 0)
			{
				ret0iv[j].RetrieveAString(strControl);
			}
			else if (strcmp(key, "hint") == 0)
			{
				bHint = ret0iv[j].GetBool();
			}
			else if (strcmp(key, "text") == 0)
			{
				bText = ret0iv[j].GetBool();
			}
			else if (strcmp(key, "prehint") == 0)
			{
				ret0iv[j].RetrieveAWString(strPreHint);
			}
			else if (strcmp(key, "posthint") == 0)
			{
				ret0iv[j].RetrieveAWString(strPostHint);
			}
			else if (strcmp(key, "pretext") == 0)
			{
				ret0iv[j].RetrieveAWString(strPreText);
			}
			else if (strcmp(key, "posttext") == 0)
			{
				ret0iv[j].RetrieveAWString(strPostText);
			}
			else
			{
				return false;
			}
		}

		SShortcutsUIInfo   info;
		info.iLk = iLk;
		info.strDlg = strDlg;
		info.strControl = strControl;
		info.bHint = bHint;
		info.bText = bText;
		info.strPreHint = strPreHint;
		info.strPostHint = strPostHint;
		info.strPreText  = strPreText;
		info.strPostText = strPostText;

		m_vecShortCutsUIInfo.push_back(info);
	}


	return true;
}
bool CECGameUIMan::InitShortCutsUIInfo()
{
	m_pState = g_LuaStateMan.GetConfigState();
	assert(m_pState);

	m_pState->Lock();


	CLuaScript * pScript = m_pState->RegisterFile(LUA_SHORTCUTSUIINFO_FILE);

	if (!pScript)
	{
		m_pState->Unlock();
		return false;
	}

	bool rt = LoadShortCutsUIInfo();

	assert(rt);

	m_pState->Unlock();
	return	rt;

}
void CECGameUIMan::ReleaseShortCutsUIInfo()
{
    m_vecShortCutsUIInfo.clear();
	abase::vector<AString> rmTbls;

	rmTbls.push_back(SHORTCUTS_UI_INFO);

	m_pState->Lock();
	m_pState->UnRegister(LUA_SHORTCUTSUIINFO_FILE, &rmTbls);
	m_pState->Unlock();
}
void CECGameUIMan::RefreshShortCutUIInfo()
{
   for (int i=0;i<(int)m_vecShortCutsUIInfo.size();++i)
   {
        const SShortcutsUIInfo& info = m_vecShortCutsUIInfo[i];
		PAUIDIALOG pDlg = GetDialog(info.strDlg);
		if(pDlg && pDlg->IsShow())
		{
		   PAUIOBJECT  pControl = pDlg->GetDlgItem(info.strControl);
		   if(pControl && pControl->IsShow())
		   {
		       if(info.bHint)
			   {
                    ACString strHint = WC2AC(info.strPreHint);
					ACString strShortCut = CDlgQuickBarSettingDetail::GetLKName(info.iLk);
					if(strShortCut != _AL(""))
					{
						strHint += _AL("^ffffff(");
						strHint += _AL("^ffcb4a");
						strHint += strShortCut;
						strHint += _AL("^ffffff)");
					};
					strHint += WC2AC(info.strPostHint);
					pControl->SetHint(strHint);

			   }
			   if(info.bText)
			   {
                    ACString strText = WC2AC(info.strPreText);
					
					ACString strShortCut = CDlgQuickBarSettingDetail::GetLKName(info.iLk);
					if(strShortCut != _AL(""))
					{
						strText += _AL("^ffffff(");
					    strText += _AL("^ffcb4a");
						strText += strShortCut;
						strText += _AL("^ffffff)");
					}
					
					strText += WC2AC(info.strPostText);
					pControl->SetText(strText);

			   }
		   }
		}

   }
}

void CECGameUIMan::SwitchToPrivateChat(ACString strName,bool bNeedForcus,bool bClickName)
{
	if(NULL != m_pDlgChat)
	{
		PAUIEDITBOX pEdit = (PAUIEDITBOX)m_pDlgChat->GetDlgItem("DEFAULT_Txt_Speech");

		if(NULL != pEdit)
		{
			ACString strText = strName + _AL(" ");
			pEdit->SetText(strText);
			strText = _AL("/")+ strText;
			m_pDlgChat->DetermineChannelByText(strText);
			
			if(bNeedForcus)
			{
			   m_pDlgChat->ChangeFocus(pEdit);
			   BringWindowToTop(m_pDlgChat);
			}
			if(bClickName)
			{
			   m_bClickedUserName = true;
			}
		}
	}
}

#ifdef LUA_UI
bool CECGameUIMan::LuaInit(const char *szScript)
{
	if (!CLuaScript::LuaInit(szScript))
	{
		return false;
	}
	LuaBind::SetField(m_pLuaState->GetState(), LUA_GAMEUIMAN, "this", LuaBind::TypeWrapper<void *>(), this);
	return true;
}

bool CECGameUIMan::LuaRelease()
{

	LuaBind::ReleaseTbl(m_pLuaState->GetState(), LUA_GAMEUIMAN);
	CLuaScript::LuaRelease();
	return true;
}

void CECGameUIMan::LuaEvent(int eventID, DWORD arg1, DWORD arg2, const char* arg3, const char* arg4)
{
	int nArgs = 0;
	LuaBind::Push(LuaBind::TypeWrapper<int>(), m_pLuaState->GetState(), eventID);
	++nArgs;
	switch(eventID) 
	{
	case LuaBind::DLG_SHOW_EPEQUIP:
		{
			LuaBind::Push(LuaBind::TypeWrapper<int>(), m_pLuaState->GetState(), arg1);
			++nArgs;
		}
		break;
	default:
		assert(0 && "unknown event");
	}

	m_pState->Call(m_pLuaState->GetState(), LUA_GAMEUIMAN, "OnEventMap", nArgs);
}
#endif


