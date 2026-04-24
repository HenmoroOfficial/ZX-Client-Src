  /*
 * FILE: EC_HostInputFilter.cpp
 *
 * DESCRIPTION: 
 *
 * CREATED BY: Duyuxin, 2004/8/31
 *
 * HISTORY: 
 *
 * Copyright (c) 2004 Archosaur Studio, All Rights Reserved.
 */

//	For WM_MOUSEWHEEL message
#define _WIN32_WINDOWS	0x0500

#include "EC_Global.h"
#include "EC_HostInputFilter.h"
#include "EC_InputCtrl.h"
#include "EC_CameraCtrl.h"
#include "EC_Game.h"
#include "EC_GameRun.h"
#include "EC_Utility.h"
#include "EC_HostPlayer.h"
#include "EC_Configs.h"

#ifdef BACK_VERSION
#include "EC_Manager.h"
#else
#include "ECManager.h"
#include "ECManagerID.h"
#endif
#include "EC_World.h"
#include <windowsx.h>
#include <winuser.h>
#include "EC_GameRecord.h"
#include "EC_BossKey.h"
#include "EC_FixedMsg.h"
#include "EC_Cutscene.h"
#include "EC_UIManager.h"
#include "EC_GameUIMan.h"

#include "AC.h"
#include <AIniFile.h>
#include "A3DDevice.h"

#define new A_DEBUG_NEW

///////////////////////////////////////////////////////////////////////////
//	
//	Define and Macro
//	
///////////////////////////////////////////////////////////////////////////

#define CLICK_DELAY		200
#define LKMAPFILE_VERSION		3

///////////////////////////////////////////////////////////////////////////
//	
//	Reference to External variables and functions
//	
///////////////////////////////////////////////////////////////////////////

static DWORD _JumpTime = 0;

///////////////////////////////////////////////////////////////////////////
//	
//	Local Types and Variables and Global variables
//	
///////////////////////////////////////////////////////////////////////////
char* CECHostInputFilter::m_aLKNames[NUM_LKEY] = 
{
	"none",

	// Old Key
	"ghostmode",
	"jump",
	"pushcamera",
	"pullcamera",
	"autoselect",
	"hideplayer",

	//	Move keys
	"hp_move_forward",		//	LKEY_HP_MOVE_FORWARD
	"hp_move_back",			//	LKEY_HP_MOVE_BACK,	
	"hp_move_left",			//	LKEY_HP_MOVE_LEFT,	
	"hp_move_right",		//	LKEY_HP_MOVE_RIGHT,	

	"chatgetfocus",
	"petattack1",
	"petattack2",
	"petattack3",
	"petattack4",
	"petattack5",
	"petattack6",
	"petattack7",
	"petattack8",
	"petattack9",
	"petattackstop",
	"petattackfollow",
	"petattackoffensive",
	"petattackdefensive",
	"petattackcombat",
	"chatclear",
	"ride1_useitem",
	"ride2_useitem",
	"teammate1shift",
	"teammate2shift",
	"teammate3shift",
	"teammate4shift",
	"teammate5shift",
	"teammate6shift",
	"teammate7shift",
	"teammate8shift",
	"teammate9shift",
	"changefashion1",
	"changefashion2",
	"changefashion3",
	"changefashion4",
	"changefashion5",
	"changefashion6",
	"changefashion7",
	"changefashion8",
	"changefashion9",
	"infofriend",
	"showiteminfo",
	"showmodellimit",
	
	//	UI panel keys
	"ui_show_char",			//	LKEY_UI_SHOWDLG_CHARACTER,
	"ui_show_pet",
	"ui_show_secretary",
	"ui_show_ranklist",
	"ui_show_broadcast",
	"ui_show_produce",
	"ui_show_teacher",
	"ui_show_daily",
	"ui_show_circle",
	"ui_show_zoarium",
	"ui_show_ivtr",			//	LKEY_UI_SHOWDLG_INVENTORY,
	"ui_show_skill",		//	LKEY_UI_SHOWDLG_SKILL,
	"ui_show_task",			//	LKEY_UI_SHOWDLG_TASK,
	"ui_show_friend",		//	LKEY_UI_SHOWDLG_FRIEND,
	"ui_show_faction",		//	LKEY_UI_SHOWDLG_FACTION,
	"ui_show_team",			//	LKEY_UI_SHOWDLG_TEAM,
	"ui_show_mapmid",		//	LKEY_UI_SHOWDLG_MAPMID
	"ui_show_cashshop",		//	LKEY_UI_SHOWDLG_CASHSHOP,
	"ui_show_action",		//	LKEY_UI_SHOWDLG_ACTIONEDIT,	
	"ui_show_help",			//	LKEY_UI_SHOWDLG_HELP,	
	"ui_show_all",			//	LKEY_UI_SHOW_ALLPANELS,

	//	Shortcut keys
	"shortcut_1",			//	LKEY_HP_SHORTCUT_1,
	"shortcut_2",			//	LKEY_HP_SHORTCUT_2,
	"shortcut_3",			//	LKEY_HP_SHORTCUT_3,
	"shortcut_4",			//	LKEY_HP_SHORTCUT_4,
	"shortcut_5",			//	LKEY_HP_SHORTCUT_5,
	"shortcut_6",			//	LKEY_HP_SHORTCUT_6,
	"shortcut_7",			//	LKEY_HP_SHORTCUT_7,
	"shortcut_8",			//	LKEY_HP_SHORTCUT_8,
	"shortcut_9",			//	LKEY_HP_SHORTCUT_9,
	"shortcut_10",			//	LKEY_HP_SHORTCUT_10,
	"shortcut_11",			//	LKEY_HP_SHORTCUT_11,
	"shortcut_12",			//	LKEY_HP_SHORTCUT_12,
	"shortcut_13",			//	LKEY_HP_SHORTCUT_13,
	"shortcut_14",			//	LKEY_HP_SHORTCUT_14,
	"shortcut_15",			//	LKEY_HP_SHORTCUT_15,
	"shortcut_16",			//	LKEY_HP_SHORTCUT_16,
	"shortcut_17",			//	LKEY_HP_SHORTCUT_17,
	"shortcut_18",			//	LKEY_HP_SHORTCUT_18,
	"shortcut_19",			//	LKEY_HP_SHORTCUT_19,
	"shortcut_20",
	"shortcut_21","shortcut_22","shortcut_23","shortcut_24","shortcut_25","shortcut_26","shortcut_27","shortcut_28","shortcut_29","shortcut_30",
	"shortcut_31","shortcut_32","shortcut_33","shortcut_34","shortcut_35","shortcut_36","shortcut_37","shortcut_38","shortcut_39","shortcut_40",
	"shortcut_41","shortcut_42","shortcut_43","shortcut_44","shortcut_45","shortcut_46","shortcut_47","shortcut_48","shortcut_49","shortcut_50",
	"shortcut_51","shortcut_52","shortcut_53","shortcut_54","shortcut_55","shortcut_56","shortcut_57","shortcut_58","shortcut_59","shortcut_60",
	"shortcut_61","shortcut_62","shortcut_63","shortcut_64","shortcut_65","shortcut_66","shortcut_67","shortcut_68","shortcut_69","shortcut_70",
	"shortcut_71","shortcut_72","shortcut_73","shortcut_74","shortcut_75","shortcut_76","shortcut_77","shortcut_78","shortcut_79","shortcut_80",
	"shortcut_81","shortcut_82","shortcut_83","shortcut_84","shortcut_85","shortcut_86","shortcut_87","shortcut_88","shortcut_89","shortcut_90",
	"shortcut_91","shortcut_92",
	//	Hidden keys
	"hp_cancel",			//	LKEY_HP_CANCEL,	
	"ui_show_gm",			//	LKEY_UI_SHOWDLG_GMCONSOLE,
	"ui_show_console",		//	LKEY_UI_SHOWDLG_CONSOLE,
	"ui_ghostmode_front",   //LKEY_UI_GHOSTMODE_FRONT,
	"ui_ghostmode_back",	//LKEY_UI_GHOSTMODE_BACK,
	"ui_ghostmode_up",		//LKEY_UI_GHOSTMODE_UP,
	"ui_ghostmode_down",	//LKEY_UI_GHOSTMODE_DOWN,
	"ui_ghostmode_left",	//LKEY_UI_GHOSTMODE_LEFT,
	"ui_ghostmode_right",//LKEY_UI_GHOSTMODE_RIGHT,
	"ui_ghostmode_forword",//LKEY_UI_GHOSTMODE_FORWORD,
	"ui_ghostmode_backword",//LKEY_UI_GHOSTMODE_BACKWORD,
};
extern game_record gr;

///////////////////////////////////////////////////////////////////////////
//	
//	Local functions
//	
///////////////////////////////////////////////////////////////////////////


///////////////////////////////////////////////////////////////////////////
//	
//	Implement CECHostInputFilter
//	
///////////////////////////////////////////////////////////////////////////

CECHostInputFilter::CECHostInputFilter(CECInputCtrl* pInputCtrl) :
CECInputFilter(pInputCtrl)
{
	m_LBPress.bPressed		= false;
	m_LBPress.bTurnCamera	= false;
	m_RBPress.bPressed		= false;
	m_RBPress.bTurnCamera	= false;

	ResetLogicMap();

}

CECHostInputFilter::~CECHostInputFilter()
{
}
//	Reset logic key map to default settings
void CECHostInputFilter::ResetLogicMap()
{
	//	Clear current map
	ClearAllLogicKeys();
   
	//Old Keys
	//RegisterLogicKey(LKEY_ITFTRIGGER,		    'H',			CKCOMBO_C,	    0,		CKCOMBO_NONE);
	RegisterLogicKey(LKEY_GHOSTMODE,		    VK_F9,			CKCOMBO_NONE,	0,		CKCOMBO_NONE);
	RegisterLogicKey(LKEY_JUMP,		            VK_SPACE,		CKCOMBO_NONE,	0,		CKCOMBO_NONE);
	RegisterLogicKey(LKEY_PUSHCAMERA,		    VK_SUBTRACT,	CKCOMBO_NONE,	0,		CKCOMBO_NONE);
	RegisterLogicKey(LKEY_PULLCAMERA,		    VK_ADD,			CKCOMBO_NONE,	0,		CKCOMBO_NONE);
	RegisterLogicKey(LKEY_AUTOSELECT,		    VK_TAB,			CKCOMBO_NONE,	0,		CKCOMBO_NONE);
    RegisterLogicKey(LKEY_HIDEELSEPLAYER,		VK_F10,			CKCOMBO_NONE,	0,		CKCOMBO_NONE);


	//	Move keys
	RegisterLogicKey(LKEY_HP_MOVE_FORWARD,		'W',			CKCOMBO_NONE,	0,		CKCOMBO_NONE);
	RegisterLogicKey(LKEY_HP_MOVE_BACK,			'S',			CKCOMBO_NONE,	0,	    CKCOMBO_NONE);
	RegisterLogicKey(LKEY_HP_MOVE_LEFT,			'A',			CKCOMBO_NONE,	0,	    CKCOMBO_NONE);
	RegisterLogicKey(LKEY_HP_MOVE_RIGHT,		'D',			CKCOMBO_NONE,	0,	    CKCOMBO_NONE);


    RegisterLogicKey(LKEY_CHATGETFOCUS,		    VK_RETURN,		CKCOMBO_NONE,	0,		CKCOMBO_NONE);
	RegisterLogicKey(LKEY_PETATTACK_1,		    '1',		    CKCOMBO_A,	0,		CKCOMBO_NONE);
	RegisterLogicKey(LKEY_PETATTACK_2,		    '2',		    CKCOMBO_A,	0,		CKCOMBO_NONE);
	RegisterLogicKey(LKEY_PETATTACK_3,		    '3',		    CKCOMBO_A,	0,		CKCOMBO_NONE);
	RegisterLogicKey(LKEY_PETATTACK_4,		    '4',		    CKCOMBO_A,	0,		CKCOMBO_NONE);
	RegisterLogicKey(LKEY_PETATTACK_5,		    '5',		    CKCOMBO_A,	0,		CKCOMBO_NONE);
	RegisterLogicKey(LKEY_PETATTACK_6,		    '6',		    CKCOMBO_A,	0,		CKCOMBO_NONE);
	RegisterLogicKey(LKEY_PETATTACK_7,		    '7',		    CKCOMBO_A,	0,		CKCOMBO_NONE);
	RegisterLogicKey(LKEY_PETATTACK_8,		    '8',		    CKCOMBO_A,	0,		CKCOMBO_NONE);
	RegisterLogicKey(LKEY_PETATTACK_9,		    '9',		    CKCOMBO_A,	0,		CKCOMBO_NONE);

	RegisterLogicKey(LKEY_PETATTACK_STOP,		 's',		    CKCOMBO_A,	0,		CKCOMBO_NONE);
	RegisterLogicKey(LKEY_PETATTACK_FOLLOW,		 'f',		    CKCOMBO_A,	0,		CKCOMBO_NONE);
	RegisterLogicKey(LKEY_PETATTACK_OFFENSIVE,	 'a',		    CKCOMBO_A,	0,		CKCOMBO_NONE);
	RegisterLogicKey(LKEY_PETATTACK_DEFENSIVE,	 'd',		    CKCOMBO_A,	0,		CKCOMBO_NONE);
	RegisterLogicKey(LKEY_PETATTACK_COMBAT,		 'v',		    CKCOMBO_A,	0,		CKCOMBO_NONE);
	
	RegisterLogicKey(LKEY_CHAT_CLEAR,		    'D',		    CKCOMBO_C,	0,		CKCOMBO_NONE);
	RegisterLogicKey(LKEY_RIDE1_USEITEM,		'Q',		    CKCOMBO_C,	0,		CKCOMBO_NONE);
	RegisterLogicKey(LKEY_RIDE2_USEITEM,		'F',		    CKCOMBO_C,	0,		CKCOMBO_NONE);

	RegisterLogicKey(LKEY_TEAMMATE_1_SHIFT,		'1',		    CKCOMBO_S,	0,		CKCOMBO_NONE);
	RegisterLogicKey(LKEY_TEAMMATE_2_SHIFT,		'2',		    CKCOMBO_S,	0,		CKCOMBO_NONE);
	RegisterLogicKey(LKEY_TEAMMATE_3_SHIFT,		'3',		    CKCOMBO_S,	0,		CKCOMBO_NONE);
	RegisterLogicKey(LKEY_TEAMMATE_4_SHIFT,		'4',		    CKCOMBO_S,	0,		CKCOMBO_NONE);
	RegisterLogicKey(LKEY_TEAMMATE_5_SHIFT,		'5',		    CKCOMBO_S,	0,		CKCOMBO_NONE);
	RegisterLogicKey(LKEY_TEAMMATE_6_SHIFT,		'6',		    CKCOMBO_S,	0,		CKCOMBO_NONE);
	RegisterLogicKey(LKEY_TEAMMATE_7_SHIFT,		'7',		    CKCOMBO_S,	0,		CKCOMBO_NONE);
	RegisterLogicKey(LKEY_TEAMMATE_8_SHIFT,		'8',		    CKCOMBO_S,	0,		CKCOMBO_NONE);
	RegisterLogicKey(LKEY_TEAMMATE_9_SHIFT,		'9',		    CKCOMBO_S,	0,		CKCOMBO_NONE);


	RegisterLogicKey(LKEY_CHANGEFASHION_1,		'1',		    CKCOMBO_C,	0,		CKCOMBO_NONE);
	RegisterLogicKey(LKEY_CHANGEFASHION_2,		'2',		    CKCOMBO_C,	0,		CKCOMBO_NONE);
	RegisterLogicKey(LKEY_CHANGEFASHION_3,		'3',		    CKCOMBO_C,	0,		CKCOMBO_NONE);
	RegisterLogicKey(LKEY_CHANGEFASHION_4,		'4',		    CKCOMBO_C,	0,		CKCOMBO_NONE);
	RegisterLogicKey(LKEY_CHANGEFASHION_5,		'5',		    CKCOMBO_C,	0,		CKCOMBO_NONE);
	RegisterLogicKey(LKEY_CHANGEFASHION_6,		'6',		    CKCOMBO_C,	0,		CKCOMBO_NONE);
	RegisterLogicKey(LKEY_CHANGEFASHION_7,		'7',		    CKCOMBO_C,	0,		CKCOMBO_NONE);
	RegisterLogicKey(LKEY_CHANGEFASHION_8,		'8',		    CKCOMBO_C,	0,		CKCOMBO_NONE);
	RegisterLogicKey(LKEY_CHANGEFASHION_9,		'9',		    CKCOMBO_C,	0,		CKCOMBO_NONE);

	RegisterLogicKey(LKEY_INFOFRIEND,		    'Z',		    CKCOMBO_CA,	0,		CKCOMBO_NONE);
	RegisterLogicKey(LKEY_SHOWITEMINFO,		    VK_F11,		    CKCOMBO_C,	0,		CKCOMBO_NONE);
	RegisterLogicKey(LKEY_SHOWMODELLIMIT,		VK_F12,		    CKCOMBO_C,	0,		CKCOMBO_NONE);
	
	
	//	UI panel keys
	RegisterLogicKey(LKEY_UI_SHOWDLG_CHARACTER,	'C',			CKCOMBO_NONE,	0,			CKCOMBO_NONE);
	RegisterLogicKey(LKEY_UI_SHOWDLG_PET,	    'P',			CKCOMBO_NONE,	0,			CKCOMBO_NONE);
	RegisterLogicKey(LKEY_UI_SHOWDLG_SECRETARY,	 0,			    CKCOMBO_NONE,	0,			CKCOMBO_NONE);
	RegisterLogicKey(LKEY_UI_SHOWDLG_RANKLIST,	'I',			CKCOMBO_NONE,	0,			CKCOMBO_NONE);
	RegisterLogicKey(LKEY_UI_SHOWDLG_BROADCAST,	'U',			CKCOMBO_NONE,	0,			CKCOMBO_NONE);
	RegisterLogicKey(LKEY_UI_SHOWDLG_PRODUCE,	'O',			CKCOMBO_NONE,	0,			CKCOMBO_NONE);
	RegisterLogicKey(LKEY_UI_SHOWDLG_TEACHER,	'Y',			CKCOMBO_NONE,	0,			CKCOMBO_NONE);
	RegisterLogicKey(LKEY_UI_SHOWDLG_DAILY,	    'K',			CKCOMBO_NONE,	0,			CKCOMBO_NONE);
	RegisterLogicKey(LKEY_UI_SHOWDLG_CIRCLE,	'N',			CKCOMBO_NONE,	0,			CKCOMBO_NONE);
	RegisterLogicKey(LKEY_UI_SHOWDLG_ZOARIUM,	'V',			CKCOMBO_NONE,	0,			CKCOMBO_NONE);
	RegisterLogicKey(LKEY_UI_SHOWDLG_INVENTORY,	'B',			CKCOMBO_NONE,	0,			CKCOMBO_NONE);
	RegisterLogicKey(LKEY_UI_SHOWDLG_SKILL,		'R',			CKCOMBO_NONE,	0,			CKCOMBO_NONE);
	RegisterLogicKey(LKEY_UI_SHOWDLG_TASK,		'Q',			CKCOMBO_NONE,	0,			CKCOMBO_NONE);
	RegisterLogicKey(LKEY_UI_SHOWDLG_FRIEND,	'F',			CKCOMBO_NONE,	0,			CKCOMBO_NONE);
	RegisterLogicKey(LKEY_UI_SHOWDLG_FACTION,	'G',			CKCOMBO_NONE,	0,			CKCOMBO_NONE);
	RegisterLogicKey(LKEY_UI_SHOWDLG_TEAM,		'T',			CKCOMBO_NONE,	0,			CKCOMBO_NONE);
	RegisterLogicKey(LKEY_UI_SHOWDLG_MAPMID,	'M',			CKCOMBO_NONE,	0,			CKCOMBO_NONE);
	RegisterLogicKey(LKEY_UI_SHOWDLG_CASHSHOP,	'J',			CKCOMBO_NONE,	0,			CKCOMBO_NONE);
	RegisterLogicKey(LKEY_UI_SHOWDLG_ACTION,	'E',			CKCOMBO_NONE,	0,			CKCOMBO_NONE);
	RegisterLogicKey(LKEY_UI_SHOWDLG_HELP,		'H',			CKCOMBO_NONE,	0,			CKCOMBO_NONE);
	RegisterLogicKey(LKEY_UI_SHOW_ALLPANELS,	'H',			CKCOMBO_A,		0,			CKCOMBO_NONE);

	//	Shortcut keys
	RegisterLogicKey(LKEY_HP_SHORTCUT_1,		'1',			CKCOMBO_NONE,	0,			CKCOMBO_NONE);
	RegisterLogicKey(LKEY_HP_SHORTCUT_2,		'2',			CKCOMBO_NONE,	0,			CKCOMBO_NONE);
	RegisterLogicKey(LKEY_HP_SHORTCUT_3,		'3',			CKCOMBO_NONE,	0,			CKCOMBO_NONE);
	RegisterLogicKey(LKEY_HP_SHORTCUT_4,		'4',			CKCOMBO_NONE,	0,			CKCOMBO_NONE);
	RegisterLogicKey(LKEY_HP_SHORTCUT_5,		'5',			CKCOMBO_NONE,	0,			CKCOMBO_NONE);
	RegisterLogicKey(LKEY_HP_SHORTCUT_6,		'6',			CKCOMBO_NONE,	0,			CKCOMBO_NONE);
	RegisterLogicKey(LKEY_HP_SHORTCUT_7,		'7',			CKCOMBO_NONE,	0,			CKCOMBO_NONE);
	RegisterLogicKey(LKEY_HP_SHORTCUT_8,		'8',			CKCOMBO_NONE,	0,			CKCOMBO_NONE);
	RegisterLogicKey(LKEY_HP_SHORTCUT_9,		'9',			CKCOMBO_NONE,	0,			CKCOMBO_NONE);
	RegisterLogicKey(LKEY_HP_SHORTCUT_10,		'0',			CKCOMBO_NONE,	0,			CKCOMBO_NONE);
	RegisterLogicKey(LKEY_HP_SHORTCUT_11,		 0,			    CKCOMBO_NONE,	0,			CKCOMBO_NONE);
	RegisterLogicKey(LKEY_HP_SHORTCUT_12,		 0,			    CKCOMBO_NONE,	0,			CKCOMBO_NONE);
	RegisterLogicKey(LKEY_HP_SHORTCUT_13,		VK_F1,			CKCOMBO_NONE,	0,			CKCOMBO_NONE);
	RegisterLogicKey(LKEY_HP_SHORTCUT_14,		VK_F2,			CKCOMBO_NONE,	0,			CKCOMBO_NONE);
	RegisterLogicKey(LKEY_HP_SHORTCUT_15,		VK_F3,			CKCOMBO_NONE,	0,			CKCOMBO_NONE);
	RegisterLogicKey(LKEY_HP_SHORTCUT_16,		VK_F4,			CKCOMBO_NONE,	0,			CKCOMBO_NONE);
	RegisterLogicKey(LKEY_HP_SHORTCUT_17,		VK_F5,			CKCOMBO_NONE,	0,			CKCOMBO_NONE);
	RegisterLogicKey(LKEY_HP_SHORTCUT_18,		VK_F6,			CKCOMBO_NONE,	0,			CKCOMBO_NONE);
	RegisterLogicKey(LKEY_HP_SHORTCUT_19,		VK_F7,			CKCOMBO_NONE,	0,			CKCOMBO_NONE);
	RegisterLogicKey(LKEY_HP_SHORTCUT_20,		VK_F8,			CKCOMBO_NONE,	0,			CKCOMBO_NONE);

	for (int iKey= LKEY_HP_SHORTCUT_21;iKey<=LKEY_HP_SHORTCUT_92;++iKey)
	{
         RegisterLogicKey(iKey,		0,			CKCOMBO_NONE,	0,			CKCOMBO_NONE);
	}
	//	Hidden keys
	RegisterLogicKey(LKEY_HP_CANCEL,		        VK_ESCAPE,		CKCOMBO_NONE,	0,		CKCOMBO_NONE);
	RegisterLogicKey(LKEY_UI_SHOWDLG_CONSOLE,	    VK_OEM_3,		CKCOMBO_S,		'`',		CKCOMBO_S);
	RegisterLogicKey(LKEY_UI_SHOWDLG_GMCONSOLE,	    'G',			CKCOMBO_C,		0,			CKCOMBO_NONE);

	RegisterLogicKey(LKEY_UI_GHOSTMODE_FRONT,	    VK_HOME,	    CKCOMBO_NONE,	0,			CKCOMBO_NONE);
	RegisterLogicKey(LKEY_UI_GHOSTMODE_BACK,	    VK_END,	        CKCOMBO_NONE,	0,			CKCOMBO_NONE);
	RegisterLogicKey(LKEY_UI_GHOSTMODE_UP,	        VK_INSERT,	    CKCOMBO_NONE,	0,			CKCOMBO_NONE);
	RegisterLogicKey(LKEY_UI_GHOSTMODE_DOWN,	    VK_PRIOR,	    CKCOMBO_NONE,	0,			CKCOMBO_NONE);
	RegisterLogicKey(LKEY_UI_GHOSTMODE_LEFT,	    VK_DELETE,	    CKCOMBO_NONE,	0,			CKCOMBO_NONE);
	RegisterLogicKey(LKEY_UI_GHOSTMODE_RIGHT,	    VK_NEXT,	    CKCOMBO_NONE,	0,			CKCOMBO_NONE);
	RegisterLogicKey(LKEY_UI_GHOSTMODE_FORWORD,	    VK_HOME,	    CKCOMBO_C,		0,			CKCOMBO_NONE);
	RegisterLogicKey(LKEY_UI_GHOSTMODE_BACKWORD,	VK_END,	        CKCOMBO_C,		0,			CKCOMBO_NONE);
	
}
//	Translate input
bool CECHostInputFilter::TranslateInput(int iInput)
{
	if( !g_pGame->IsActive() )
		return true;

	CECHostPlayer* pHost = g_pGame->GetGameRun()->GetHostPlayer();
	if (!pHost || !pHost->HostIsReady())
		return false;

	//if (iInput == INPUT_KEYBOARD)
	//	return TranslateKeyboard(pHost);
	//else if (iInput == INPUT_MOUSE)
	//	return TranslateMouse(pHost);
    if (iInput == INPUT_KEYBOARD)
	{
	   TranslateKeyboard(pHost);
	   TranslateMoveCtrlKeys(pHost);
	}
	else if (iInput == INPUT_MOUSE)
	   TranslateMouse(pHost);
	

	return false;
}
bool CECHostInputFilter::TranslateMoveCtrlKeys(CECHostPlayer* pHost)
{
	if (pHost &&
		pHost->GetCutscene() &&
		pHost->GetCutscene()->IsPlayingCutscene() &&
		pHost->GetCutscene()->IsOtherKeyDisabled())
		return true;

	CECGameRun* pGameRun = g_pGame->GetGameRun();

	bool bForward = IsLogicKeyPressed(LKEY_HP_MOVE_FORWARD);
	bool bBackward = IsLogicKeyPressed(LKEY_HP_MOVE_BACK);
	bool bLeft = IsLogicKeyPressed(LKEY_HP_MOVE_LEFT);
	bool bRight = IsLogicKeyPressed(LKEY_HP_MOVE_RIGHT);

	if(bForward || bBackward || bLeft || bRight)
	{
	   CECGameUIMan* pGameUIMan = pGameRun->GetUIManager()->GetInGameUIMan();
	   if(NULL != pGameUIMan)
		   pGameUIMan->CommonMessage("Win_Help_Wasd","KeyboradMove",0,0);
	}

	int iMove1=-1, iMove2=-1;
	int iYaw = 0;

	if (m_pInputCtrl->KeyIsBeingPressed(VK_UP))
		iMove1 = 8;
	else if (m_pInputCtrl->KeyIsBeingPressed(VK_DOWN))
		iMove1 = 9;

	if (pHost->IsUnderWater() || pHost->IsFlying())
	{
		if (m_pInputCtrl->KeyIsBeingPressed(VK_SPACE))
		{
			if (pHost->IsFlying())
				iMove1 = 8;
			else if (!pHost->CanTakeOffWater())
				iMove1 = 8;
		}
		else
		{
			_JumpTime = 0;

			if (m_pInputCtrl->KeyIsBeingPressed('Z'))
				iMove1 = 9;
		}
	}

	if (!g_pGame->GetConfigs()->GetGameSettings().bTurnaround)
	{
		if (bForward)
		{
			if (bLeft)
				iMove2 = 1;
			else if (bRight)
				iMove2 = 7;
			else
				iMove2 = 0;
		}
		else if (bBackward)
		{
			if (bLeft)
				iMove2 = 3;
			else if (bRight)
				iMove2 = 5;
			else
				iMove2 = 4;
		}
		else if (bLeft)
			iMove2 = 2;
		else if (bRight)
			iMove2 = 6;
	}
	else
	{
		if (bForward)
			iMove2 = 0;
		else if (bBackward)
			iMove2 = 4;

		if (bLeft)
			iYaw = 1;
		else if (bRight)
			iYaw = -1;
	}

	if (gr.get_state() != game_record::enum_gr_replay)
	{
		if (iMove1 >= 0)
			pGameRun->PostMessage(MSG_HST_PUSHMOVE, MAN_PLAYER, 0, iMove1);

		if (iMove2 >= 0)
			pGameRun->PostMessage(MSG_HST_PUSHMOVE, MAN_PLAYER, 0, iMove2);
	}

	if (iYaw)
	{
		int iOffset = (int)(g_pGame->GetRealTickTime() * 0.001f * 400.0f * iYaw);
		pGameRun->PostMessage(MSG_HST_YAW, MAN_PLAYER, 0, iOffset);
	}

	return true;
}
//	Translate keyboard input
bool CECHostInputFilter::TranslateKeyboard(CECHostPlayer* pHost)
{
	CECGameRun* pGameRun = g_pGame->GetGameRun();

	int i, iNumMsg = m_pInputCtrl->GetWinKeyMsgNum();

	for (i=0; i < iNumMsg; i++)
	{
		const CECInputCtrl::WINMSG& Msg = m_pInputCtrl->GetWinKeyMsg(i);
		int lk = ConvertVKToLK(Msg.wParam, Msg.dwCtrlkeys);
		if (lk)
		{
			if (Msg.uMsg == WM_KEYDOWN || Msg.uMsg == WM_SYSKEYDOWN)
				OnKeyDown(pHost, lk, Msg.dwCtrlkeys);
			else
				OnKeyUp(pHost, lk, Msg.dwCtrlkeys);
		}
	}
	return true;
}

void CECHostInputFilter::OnKeyDown(CECHostPlayer* pHost, int iLogicKey, DWORD dwFlags)
{
	CECGameRun* pGameRun = g_pGame->GetGameRun();
	CECGameUIMan* pGameUIMan = pGameRun->GetUIManager()->GetInGameUIMan();

	if(!pGameRun || !pGameUIMan)
		return;
	
	bool bFirstPressed = CECInputCtrl::IsFirstPressed(dwFlags);
	
	if(pHost->IsFreeFlying())
	{
		if(bFirstPressed && iLogicKey==LKEY_HP_CANCEL)
			pGameRun->PostMessage(MSG_HST_PRESSCANCEL, MAN_PLAYER, 0);
			
		return;
	}
	if (iLogicKey >= LKEY_START_SC && iLogicKey <= LKEY_END_SC)
	{
		  pGameUIMan->OnLogicKey_UpdateShortCut(iLogicKey,true);
	}
	else if ((iLogicKey >= LKEY_START_PANEL && iLogicKey <= LKEY_END_PANEL) || 
		                             iLogicKey == LKEY_UI_SHOWDLG_GMCONSOLE || 
									 iLogicKey == LKEY_UI_SHOWDLG_CONSOLE )
	{
		if(bFirstPressed)
		   pGameUIMan->OnLogicKey_UpdateUI(iLogicKey,true);
	}
	else
	{
		switch (iLogicKey)
		{
		//case LKEY_ITFTRIGGER:
			//if (m_pInputCtrl->IsCtrlPressed(dwFlags))
		//		pGameRun->PostMessage(MSG_CLOSEALLDIALOG, -1, 0);
		//	break;
		case LKEY_HP_CANCEL:

			if (gr.get_state() == game_record::enum_gr_replay)
				break;
			//	Only the first press time to send message
			if (bFirstPressed)
				pGameRun->PostMessage(MSG_HST_PRESSCANCEL, MAN_PLAYER, 0);
			break;
		case LKEY_GHOSTMODE:
			{
				if(bFirstPressed)
				{
					pGameRun->PostMessage(MSG_HST_CAMERAMODE, MAN_PLAYER, 0);
                    pGameUIMan->OnLogicKey_UpdateUI(iLogicKey,true);
				}
					
			}
			break;

		case LKEY_JUMP:

			if (gr.get_state() == game_record::enum_gr_replay)
				break;

			if (pHost->IsJumpInWater() || pHost->IsFlying())
				break;

			/*	if (m_pHost->IsUnderWater())
			{
			A3DVECTOR3 vPos = m_pHost->GetPos();
			if (vPos.y < pGameRun->GetWorld()->GetWaterHeight(vPos) - 1.7f)
			break;
			}	*/
			if (pHost->IsUnderWater())
			{
				if (!pHost->CanTakeOffWater())
					break;
				else if (_JumpTime == 0)
				{
					_JumpTime = ::GetTickCount();
					break;
				}
				else if (::GetTickCount() - _JumpTime < 1000)
					break;
				else
					_JumpTime = 0;
			}

			pGameRun->PostMessage(MSG_HST_JUMP, MAN_PLAYER, 0);
			break;

		//case LKEY_PLAYTRICK:

		//	if (gr.get_state() == game_record::enum_gr_replay)
		//		break;

		//	pGameRun->PostMessage(MSG_HST_PLAYTRICK, MAN_PLAYER, 0);
		//	break;

		case LKEY_PUSHCAMERA:
		case LKEY_PULLCAMERA:
			{
				int iDelta = (int)(g_pGame->GetRealTickTime() * 0.001f * 120.0f);
				pGameRun->PostMessage(MSG_HST_WHEELCAM, MAN_PLAYER, 0, iLogicKey == LKEY_PUSHCAMERA ? -iDelta : iDelta);
				break;
			}
			break;
		case LKEY_HIDEELSEPLAYER:
			{
			    pGameRun->ToggleHideElsePlayer();
			}
			break;

		}
	}
}

void CECHostInputFilter::OnKeyUp(CECHostPlayer* pHost, int iLogicKey, DWORD dwFlags)
{
	CECGameRun* pGameRun = g_pGame->GetGameRun();
	CECGameUIMan* pGameUIMan = pGameRun->GetUIManager()->GetInGameUIMan();

	if(pHost->IsFreeFlying())
	{
		if(iLogicKey==LKEY_HP_CANCEL)
			pGameRun->PostMessage(MSG_HST_PRESSCANCEL, MAN_PLAYER, 0);
			
		return;
	}

	if (iLogicKey >= LKEY_START_SC && iLogicKey <= LKEY_END_SC)
	{
		if(pGameUIMan)
		   pGameUIMan->OnLogicKey_UpdateShortCut(iLogicKey,false);
		
	}
	else if ((iLogicKey >= LKEY_START_PANEL && iLogicKey <= LKEY_END_PANEL) )
	{
		if(pGameUIMan)
			pGameUIMan->OnLogicKey_UpdateUI(iLogicKey,false);
	}
	else
	{
		switch (iLogicKey)
		{
		case LKEY_HP_CANCEL:
			break;
		case LKEY_AUTOSELECT:
			if (gr.get_state() == game_record::enum_gr_replay)
				break;
			//玩家手动的Tab自动选怪，优先选人
			pGameRun->PostMessage(MSG_HST_AUTOSELECT, MAN_PLAYER, 0, 0);
			if(pGameUIMan)
				pGameUIMan->CommonMessage("Win_Help_Tab","KeyboradTab",0,0);
			break;
		//case LKEY_BOSSKEY:

		//	if(gr.get_state() == game_record::enum_gr_replay)
			//	break;

			//break;
		}
	}

}

//	Translate mouse input
bool CECHostInputFilter::TranslateMouse(CECHostPlayer* pHost)
{
	if (pHost &&
		pHost->GetCutscene() &&
		pHost->GetCutscene()->IsPlayingCutscene() &&
		pHost->GetCutscene()->IsOtherKeyDisabled())
		return true;

	CECGameRun* pGameRun = g_pGame->GetGameRun();
	int i, iNumMsg = m_pInputCtrl->GetWinMouseMsgNum();

	for (i=0; i < iNumMsg; i++)
	{
		const CECInputCtrl::WINMSG& Msg = m_pInputCtrl->GetWinMouseMsg(i);
		int x = GET_X_LPARAM(Msg.lParam);
		int y = GET_Y_LPARAM(Msg.lParam);
		DWORD dwFlags = Msg.dwCtrlkeys;

		switch (Msg.uMsg)
		{
		case WM_LBUTTONDOWN:	OnLButtonDown(pHost, x, y, dwFlags);	break;
		case WM_LBUTTONDBLCLK:	OnLButtonDblClk(pHost, x, y, dwFlags);	break;
		case WM_LBUTTONUP:		OnLButtonUp(pHost, x, y, dwFlags);		break;
		case WM_RBUTTONDOWN:	OnRButtonDown(pHost, x, y, dwFlags);	break;
		case WM_RBUTTONDBLCLK:	OnRButtonDblClk(pHost, x, y, dwFlags);	break;
		case WM_RBUTTONUP:		OnRButtonUp(pHost, x, y, dwFlags);		break;
		case WM_MOUSEMOVE:		OnMouseMove(pHost, x, y, dwFlags);		break;

		case WM_MOUSEWHEEL:
		{
			int iDelta = ((short)HIWORD(Msg.wParam));
			dwFlags = LOWORD(Msg.wParam);
			OnMouseWheel(pHost, x, y, iDelta, dwFlags);
			break;
		}
		}
	}

	if (m_LBPress.bTurnCamera || m_RBPress.bTurnCamera)
	{
		int iTurn = m_LBPress.bTurnCamera ? 1 : 0;

		if (!pHost->IsChangingFace())
		{
			if (m_TurnOffset.x) pGameRun->PostMessage(MSG_HST_YAW, MAN_PLAYER, 0, -m_TurnOffset.x, 0, iTurn);
			if (m_TurnOffset.y) pGameRun->PostMessage(MSG_HST_PITCH, MAN_PLAYER, 0, -m_TurnOffset.y, 0, iTurn);
		}

		m_TurnOffset.Set(0, 0);

		if( g_pGame->GetA3DDevice()->GetShowCursor() )
			g_pGame->ShowCursor(false);
	}
	else
	{
		if( !g_pGame->GetA3DDevice()->GetShowCursor() )
			g_pGame->ShowCursor(true);
	}

//	if (m_LBPress.bTurnCamera && m_RBPress.bTurnCamera)
//		pGameRun->PostMessage(MSG_HST_PUSHMOVE, MAN_PLAYER, 0, 0);

	return true;
}

void CECHostInputFilter::OnLButtonDown(CECHostPlayer* pHost, int x, int y, DWORD dwFlags)
{
	m_LBPress.bPressed		= true;
	m_LBPress.dwPressTime	= a_GetTime();
	m_LBPress.bTurnCamera	= false;
	m_LBPress.ptPress.Set(x, y);

	//	TODO: If we enable mouse mode 2, below lines should be moved
	//	to OnLButtonUp.
	if (!pHost->IsChangingFace())
		g_pGame->GetGameRun()->PostMessage(MSG_HST_LBTNCLICK, MAN_PLAYER, 0, x, y, dwFlags, 0);
}

void CECHostInputFilter::OnLButtonDblClk(CECHostPlayer* pHost, int x, int y, DWORD dwFlags)
{
	m_LBPress.bPressed		= true;
	m_LBPress.dwPressTime	= a_GetTime();
	m_LBPress.bTurnCamera	= false;
	m_LBPress.ptPress.Set(x, y);

	//	TODO: If we enable mouse mode 2, below lines should be moved
	//	to OnLButtonUp.
	if (!pHost->IsChangingFace())
		g_pGame->GetGameRun()->PostMessage(MSG_HST_LBTNCLICK, MAN_PLAYER, 0, x, y, dwFlags, 1);
}

void CECHostInputFilter::OnLButtonUp(CECHostPlayer* pHost, int x, int y, DWORD dwFlags)
{
	m_LBPress.bPressed = false;

	if (m_LBPress.bTurnCamera)
	{
		m_LBPress.bTurnCamera = false;
		m_pInputCtrl->CaptureMouse(false);
	}

	if (!pHost->IsChangingFace())
		g_pGame->GetGameRun()->PostMessage(MSG_HST_TURNCAM, MAN_PLAYER, 0, 0, 1);
}

void CECHostInputFilter::OnRButtonDown(CECHostPlayer* pHost, int x, int y, DWORD dwFlags)
{
	if (m_pInputCtrl->IsAltPressed(dwFlags))
	{
		if (!pHost->IsChangingFace())
			g_pGame->GetGameRun()->PostMessage(MSG_HST_RBTNCLICK, MAN_PLAYER, 0, x, y, dwFlags);
	}
	else
	{
		m_ptTurnStart.Set(x, y);
		m_TurnOffset.Set(0, 0);
		m_pInputCtrl->CaptureMouse(true);

		m_RBPress.bTurnCamera = true;

		if (!pHost->IsChangingFace())
			g_pGame->GetGameRun()->PostMessage(MSG_HST_TURNCAM, MAN_PLAYER, 0, 1);
	}

	//For special usage for Host	Added 2012-05-07.
	if ( !pHost->IsChangingFace() )
	{
		g_pGame->GetGameRun()->PostMessage(MSG_HST_RBTNCLICK_OTHER, MAN_PLAYER, 0, x, y, dwFlags);
	}
}

void CECHostInputFilter::OnRButtonDblClk(CECHostPlayer* pHost, int x, int y, DWORD dwFlags)
{
	OnRButtonDown(pHost, x, y, dwFlags);
}

void CECHostInputFilter::OnRButtonUp(CECHostPlayer* pHost, int x, int y, DWORD dwFlags)
{
	CECGameRun* pGameRun = g_pGame->GetGameRun();

	if (m_RBPress.bTurnCamera)
	{
		if (!m_LBPress.bTurnCamera)
		{
			if (!pHost->IsChangingFace())
			{
				if (m_TurnOffset.x) pGameRun->PostMessage(MSG_HST_YAW, MAN_PLAYER, 0, -m_TurnOffset.x, 0);
				if (m_TurnOffset.y) pGameRun->PostMessage(MSG_HST_PITCH, MAN_PLAYER, 0, -m_TurnOffset.y, 0);
			}

			m_TurnOffset.Set(0, 0);
		}

		m_pInputCtrl->CaptureMouse(false);

		m_RBPress.bTurnCamera = false;

		if (!pHost->IsChangingFace())
			pGameRun->PostMessage(MSG_HST_TURNCAM, MAN_PLAYER, 0, 0);
	}
}

void CECHostInputFilter::OnMouseMove(CECHostPlayer* pHost, int x, int y, DWORD dwFlags)
{
	if (m_LBPress.bTurnCamera || m_RBPress.bTurnCamera)
	{
		m_TurnOffset.x += x - m_ptTurnStart.x;
		m_TurnOffset.y += y - m_ptTurnStart.y;

		//	Restore cursor position to start position
		POINT pt = {m_ptTurnStart.x, m_ptTurnStart.y};
		::ClientToScreen(g_pGame->GetGameInit().hWnd, &pt);
		 ::SetCursorPos(pt.x, pt.y);
	}
}

void CECHostInputFilter::OnMouseWheel(CECHostPlayer* pHost, int x, int y, int iDelta, DWORD dwFlags)
{
	if (g_pGame->GetConfigs()->GetGameSettings().bReverseWheel)
		iDelta = -iDelta;

	if (!pHost->IsChangingFace())
		g_pGame->GetGameRun()->PostMessage(MSG_HST_WHEELCAM, MAN_PLAYER, 0, iDelta);
}

void CECHostInputFilter::RegisterBossKeys(unsigned int key, unsigned int dwCtrlKey )
{
	CECBossKey* pBossKey = CECBossKey::GetInstance();
	if (!pBossKey)
	{
		return;
	}
	pBossKey->RemoveAllBossKeys();

	if (key)
	{
		unsigned int fsModifiers = 0;
		if (dwCtrlKey&EC_KSF_SHIFT)	 { fsModifiers|=MOD_SHIFT; }
		if (dwCtrlKey&EC_KSF_CONTROL){ fsModifiers|=MOD_CONTROL; }
		if (dwCtrlKey&EC_KSF_ALT)	 { fsModifiers|=MOD_ALT; }
		HWND hDevWnd = g_pGame->GetA3DDevice()->GetDeviceWnd();
		if (hDevWnd)
		{
			bool bResult = pBossKey->AddBossKey(hDevWnd,key,fsModifiers);
			if (!bResult)
			{
				CECStringTab* pStrTab = g_pGame->GetFixedMsgTab();
				if(CECBossKey::ERR_REG_BY_OTHER == pBossKey->GetLastError())
				{
					g_pGame->GetGameRun()->AddChatMessage(pStrTab->GetWideString(FIXMSG_HOTKEY_REG_BYOTHER),GP_CHAT_SYSTEM);
//					g_pGame->GetGameRun()->AddFixedChannelMsg(FIXMSG_HOT_KEY_REG_BY_OTHER,GP_CHAT_SYSTEM);	
				}
			}
		}
	}
}

//	Load logic key map from file
bool CECHostInputFilter::LoadLogicKeyMap(const char* szFile)
{
	AIniFile file;
	if (!file.Open(szFile))
	{
		a_LogOutput(1, "CECHostInputFilter::LoadLogicKeyMap, failed to load key map from file [%s]", szFile);
		return false;
	}

	//	Read infos
	AString strSect = "Info";
	int iVersion = file.GetValueAsInt(strSect, "version", LKMAPFILE_VERSION);

	//	Read key map
	strSect = "Map";
	int aVals[4];

	//	Remap all logic key maps
	for (int i=1; i < NUM_LKEY; i++)
	{
		if (!file.GetValueAsIntArray(strSect, m_aLKNames[i], 4, aVals))
			continue;

		int vk1 = aVals[0];
		int vk2 = aVals[2];
		if (vk1 < 0 || vk1 >= NUM_VIRTUALKEY || vk2 < 0 || vk2 >= NUM_VIRTUALKEY)
			return false;

		RemapLogicKey(i, aVals[0], (DWORD)aVals[1], aVals[2], (DWORD)aVals[3]);
	}

	file.Close();
	return true;
}

//	Save logic key map to file
bool CECHostInputFilter::SaveLogicKeyMap(const char* szFile)
{
	AIniFile file;

	//	Write info section
	AString strSect = "Info";
	file.WriteIntValue(strSect, "version", LKMAPFILE_VERSION);

	//	Write key map section
	strSect = "Map";
	char szText[256];

	for (int i=1; i < NUM_LKEY; i++)
	{
		int vk1, vk2;
		DWORD dwCtrlKeys1, dwCtrlKeys2;
		if (GetLogicKeyInfo(i, vk1, dwCtrlKeys1, vk2, dwCtrlKeys2))
		{
			sprintf(szText, "%d, %d, %d, %d", vk1, dwCtrlKeys1, vk2, dwCtrlKeys2);
			file.WriteStringValue(strSect, m_aLKNames[i], szText);
		}
	}

	if (!file.Save(szFile))
	{
		file.Close();
		a_LogOutput(1, "CECHostInputFilter::SaveLogicKeyMap, failed to save key map to file [%s]", szFile);
		return false;
	}

	file.Close();
	return true;
}

