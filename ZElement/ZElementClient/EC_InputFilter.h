/*
 * FILE: EC_InputFilter.h
 *
 * DESCRIPTION: 
 *
 * CREATED BY: Duyuxin, 2004/8/30
 *
 * HISTORY: 
 *
 * Copyright (c) 2004 Archosaur Studio, All Rights Reserved.
 */

#pragma once
#include "EC_InputCtrl.h"
#include "hashmap.h"
///////////////////////////////////////////////////////////////////////////
//	
//	Define and Macro
//	
///////////////////////////////////////////////////////////////////////////
//	Logic keys
//warning:因shortcutsui.lua配置文件依赖于逻辑键的枚举值,所以建议如要添加新的快捷键，不要破坏之前的自定义快捷键的枚举值
enum
{
	LKEY_GHOSTMODE = 1,			//	Trigger camera to ghost mode
	LKEY_JUMP,				//	Test code, Jump
	LKEY_PUSHCAMERA,		//	Push camera
	LKEY_PULLCAMERA,		//	Pull camera
	LKEY_AUTOSELECT,		//	Auto select monster
    LKEY_HIDEELSEPLAYER,
	//	Move keys
	LKEY_HP_MOVE_FORWARD,	    //	Move forward
	LKEY_HP_MOVE_BACK,			//	Move back
	LKEY_HP_MOVE_LEFT,			//	Move left
	LKEY_HP_MOVE_RIGHT,			//	Move right

	LKEY_CHATGETFOCUS,
	LKEY_PETATTACK_1,
	LKEY_PETATTACK_2,
    LKEY_PETATTACK_3,
	LKEY_PETATTACK_4,
	LKEY_PETATTACK_5,
	LKEY_PETATTACK_6,
	LKEY_PETATTACK_7,
	LKEY_PETATTACK_8,
	LKEY_PETATTACK_9,
    LKEY_PETATTACK_STOP,
	LKEY_PETATTACK_FOLLOW,
	LKEY_PETATTACK_OFFENSIVE,
	LKEY_PETATTACK_DEFENSIVE,
	LKEY_PETATTACK_COMBAT,

	LKEY_CHAT_CLEAR,

	LKEY_RIDE1_USEITEM,
	LKEY_RIDE2_USEITEM,

	LKEY_TEAMMATE_1_SHIFT,
	LKEY_TEAMMATE_2_SHIFT,
	LKEY_TEAMMATE_3_SHIFT,
	LKEY_TEAMMATE_4_SHIFT,
	LKEY_TEAMMATE_5_SHIFT,
	LKEY_TEAMMATE_6_SHIFT,
	LKEY_TEAMMATE_7_SHIFT,
	LKEY_TEAMMATE_8_SHIFT,
	LKEY_TEAMMATE_9_SHIFT,

	LKEY_CHANGEFASHION_1,
	LKEY_CHANGEFASHION_2,
	LKEY_CHANGEFASHION_3,
	LKEY_CHANGEFASHION_4,
	LKEY_CHANGEFASHION_5,
	LKEY_CHANGEFASHION_6,
	LKEY_CHANGEFASHION_7,
	LKEY_CHANGEFASHION_8,
	LKEY_CHANGEFASHION_9,

	LKEY_INFOFRIEND,
	LKEY_SHOWITEMINFO,
	LKEY_SHOWMODELLIMIT,

	//	UI panel keys
	LKEY_UI_SHOWDLG_CHARACTER,	//	show dialog character
    LKEY_UI_SHOWDLG_PET,
	LKEY_UI_SHOWDLG_SECRETARY,
	LKEY_UI_SHOWDLG_RANKLIST,
	LKEY_UI_SHOWDLG_BROADCAST,
	LKEY_UI_SHOWDLG_PRODUCE,
	LKEY_UI_SHOWDLG_TEACHER,
	LKEY_UI_SHOWDLG_DAILY,
	LKEY_UI_SHOWDLG_CIRCLE,
	LKEY_UI_SHOWDLG_ZOARIUM,
	LKEY_UI_SHOWDLG_INVENTORY,	//	show dialog inventory
	LKEY_UI_SHOWDLG_SKILL,		//	show dialog skill
	LKEY_UI_SHOWDLG_TASK,		//	show dialog task
	LKEY_UI_SHOWDLG_FRIEND,		//	show dialog friend
	LKEY_UI_SHOWDLG_FACTION,	//	show dialog faction
	LKEY_UI_SHOWDLG_TEAM,		//	show dialog team
	LKEY_UI_SHOWDLG_MAPMID,		//	show dialog mapmid
	LKEY_UI_SHOWDLG_CASHSHOP,	//	show dialog cash shop
	LKEY_UI_SHOWDLG_ACTION,		//	show dialog action
	LKEY_UI_SHOWDLG_HELP,		//	show dialog help
	LKEY_UI_SHOW_ALLPANELS,		//	show all panels

	
	//	Shortcut keys
	//快捷栏1
	LKEY_HP_SHORTCUT_1,
	LKEY_HP_SHORTCUT_2,
	LKEY_HP_SHORTCUT_3,
	LKEY_HP_SHORTCUT_4,
	LKEY_HP_SHORTCUT_5,
	LKEY_HP_SHORTCUT_6,
	LKEY_HP_SHORTCUT_7,
	LKEY_HP_SHORTCUT_8,
	LKEY_HP_SHORTCUT_9,
	LKEY_HP_SHORTCUT_10,
	LKEY_HP_SHORTCUT_11,
	LKEY_HP_SHORTCUT_12,
    //快捷栏2
	LKEY_HP_SHORTCUT_13,
	LKEY_HP_SHORTCUT_14,
	LKEY_HP_SHORTCUT_15,
	LKEY_HP_SHORTCUT_16,
	LKEY_HP_SHORTCUT_17,
	LKEY_HP_SHORTCUT_18,
	LKEY_HP_SHORTCUT_19,
	LKEY_HP_SHORTCUT_20,
	LKEY_HP_SHORTCUT_21,
	LKEY_HP_SHORTCUT_22,
	LKEY_HP_SHORTCUT_23,
	LKEY_HP_SHORTCUT_24,
	//快捷栏3
	LKEY_HP_SHORTCUT_25,
	LKEY_HP_SHORTCUT_26,
	LKEY_HP_SHORTCUT_27,
	LKEY_HP_SHORTCUT_28,
	LKEY_HP_SHORTCUT_29,
	LKEY_HP_SHORTCUT_30,
	LKEY_HP_SHORTCUT_31,
	LKEY_HP_SHORTCUT_32,
	LKEY_HP_SHORTCUT_33,
	LKEY_HP_SHORTCUT_34,
	LKEY_HP_SHORTCUT_35,
	LKEY_HP_SHORTCUT_36,
	//快捷栏4
	LKEY_HP_SHORTCUT_37,
	LKEY_HP_SHORTCUT_38,
	LKEY_HP_SHORTCUT_39,
	LKEY_HP_SHORTCUT_40,
	LKEY_HP_SHORTCUT_41,
	LKEY_HP_SHORTCUT_42,
	LKEY_HP_SHORTCUT_43,
	LKEY_HP_SHORTCUT_44,
	LKEY_HP_SHORTCUT_45,
	LKEY_HP_SHORTCUT_46,
	LKEY_HP_SHORTCUT_47,
	LKEY_HP_SHORTCUT_48,
	//纵向快捷栏一
	LKEY_HP_SHORTCUT_49,
	LKEY_HP_SHORTCUT_50,
	LKEY_HP_SHORTCUT_51,
	LKEY_HP_SHORTCUT_52,
	LKEY_HP_SHORTCUT_53,
	LKEY_HP_SHORTCUT_54,
	LKEY_HP_SHORTCUT_55,
	LKEY_HP_SHORTCUT_56,
	LKEY_HP_SHORTCUT_57,
	LKEY_HP_SHORTCUT_58,
	//纵向快捷栏二
	LKEY_HP_SHORTCUT_59,
	LKEY_HP_SHORTCUT_60,
	LKEY_HP_SHORTCUT_61,
	LKEY_HP_SHORTCUT_62,
	LKEY_HP_SHORTCUT_63,
	LKEY_HP_SHORTCUT_64,
	LKEY_HP_SHORTCUT_65,
	LKEY_HP_SHORTCUT_66,
	LKEY_HP_SHORTCUT_67,
	LKEY_HP_SHORTCUT_68,
    //自定义快捷栏一
	LKEY_HP_SHORTCUT_69,
	LKEY_HP_SHORTCUT_70,
	LKEY_HP_SHORTCUT_71,
	LKEY_HP_SHORTCUT_72,
	LKEY_HP_SHORTCUT_73,
	LKEY_HP_SHORTCUT_74,
	LKEY_HP_SHORTCUT_75,
	LKEY_HP_SHORTCUT_76,
	LKEY_HP_SHORTCUT_77,
	LKEY_HP_SHORTCUT_78,
	LKEY_HP_SHORTCUT_79,
	LKEY_HP_SHORTCUT_80,
	//自定义快捷栏二
	LKEY_HP_SHORTCUT_81,
	LKEY_HP_SHORTCUT_82,
	LKEY_HP_SHORTCUT_83,
	LKEY_HP_SHORTCUT_84,
	LKEY_HP_SHORTCUT_85,
	LKEY_HP_SHORTCUT_86,
	LKEY_HP_SHORTCUT_87,
	LKEY_HP_SHORTCUT_88,
	LKEY_HP_SHORTCUT_89,
	LKEY_HP_SHORTCUT_90,
	LKEY_HP_SHORTCUT_91,
	LKEY_HP_SHORTCUT_92,

	NUM_CUSTOMIZE_LKEY,

	//	Below logic keys shouldn't be shown and customized
	LKEY_HP_CANCEL = NUM_CUSTOMIZE_LKEY,	//	Cancel action etc.
	LKEY_UI_SHOWDLG_GMCONSOLE,	//	show dialog gm console
	LKEY_UI_SHOWDLG_CONSOLE,	//	show dialog console
	LKEY_UI_GHOSTMODE_FRONT,
	LKEY_UI_GHOSTMODE_BACK,
	LKEY_UI_GHOSTMODE_UP,
	LKEY_UI_GHOSTMODE_DOWN,
	LKEY_UI_GHOSTMODE_LEFT,
	LKEY_UI_GHOSTMODE_RIGHT,
	LKEY_UI_GHOSTMODE_FORWORD,
	LKEY_UI_GHOSTMODE_BACKWORD,

	NUM_LKEY,

	//	Labels for easy using, not real logic keys
	//LKEY_START_MOVE		= LKEY_HP_MOVE_FORWARD,
	//	LKEY_END_MOVE		= LKEY_HP_PULLCAMERA,
	//LKEY_START_CHAT		= LKEY_UI_CHAT_ENTER,
	//LKEY_END_CHAT		= LKEY_UI_CHAT_USER,
	LKEY_START_SC		= LKEY_HP_SHORTCUT_1,
	LKEY_END_SC			= LKEY_HP_SHORTCUT_92,
	//LKEY_START_SELECT	= LKEY_HP_AUTOSELECT,
	//LKEY_END_SELECT		= LKEY_HP_SEL_TM4,
	LKEY_START_PANEL	= LKEY_CHATGETFOCUS,
	LKEY_END_PANEL		= LKEY_UI_SHOW_ALLPANELS,
	//LKEY_START_OTHER	= LKEY_SYS_HIDEPLAYER,
	//LKEY_END_OTHER		= LKEY_SYS_CAMERAMODE,
	//LKEY_START_HIDDEN	= LKEY_HP_CANCEL,
	//LKEY_END_HIDDEN		= LKEY_UI_SHOWDLG_CONSOLE,
};

///////////////////////////////////////////////////////////////////////////
//	
//	Types and Global variables
//	
///////////////////////////////////////////////////////////////////////////

//class CECInputCtrl;

///////////////////////////////////////////////////////////////////////////
//	
//	Declare of Global functions
//	
///////////////////////////////////////////////////////////////////////////


///////////////////////////////////////////////////////////////////////////
//	
//	Class CECInputFilter
//	
///////////////////////////////////////////////////////////////////////////

#undef INPUT_KEYBOARD
#undef INPUT_MOUSE

class CECInputFilter
{
public:		//	Types

	//	Input type
	enum
	{
		INPUT_KEYBOARD = 0,
		INPUT_MOUSE,
	};

	//	Clear logic key flag
	enum
	{
		CLEAR_MAP1	= 0x01,
		CLEAR_MAP2	= 0x02,
		CLEAR_ALL	= 0x03,
	};

	//	Control key combo-flag
	enum
	{
		CKCOMBO_SCA = 0,
		CKCOMBO_CA,
		CKCOMBO_SA,
		CKCOMBO_SC,
		CKCOMBO_S,
		CKCOMBO_C,
		CKCOMBO_A,
		CKCOMBO_NONE,
		NUM_CKCOMBO,
	};

	struct VK_GROUP
	{
		DWORD	dwVKey1;
		DWORD	dwVKey2;
	};

	typedef abase::hash_map<int, VK_GROUP> LK2VKMap;

public:		//	Constructor and Destructor

	static DWORD	m_dwPressMask[NUM_CKCOMBO];

	CECInputFilter(CECInputCtrl* pInputCtrl);
	virtual ~CECInputFilter() {}

public:		//	Attributes

public:		//	Operations

	//	Translate input
	virtual bool TranslateInput(int iInput) { return true; }

	//	Check whether specified logic key is pressed
	bool IsLogicKeyPressed(int iLogicKey);
	//	Check whether specified logic key is toggled
	bool IsLogicKeyToggled(int iLogicKey);

	//	Get logic key that is coresponding to specified vk
	int FindLogicKey(int vk, DWORD dwCtrlKeys);
	//	Remap logic key
	bool RemapLogicKey(int lk, int vk1, DWORD dwCtrlKeys1, int vk2, DWORD dwCtrlKeys2);
	//	Get logic key info
	bool GetLogicKeyInfo(int lk, int& vk1, DWORD& dwCtrlKeys1, int& vk2, DWORD& dwCtrlKeys2);

protected:	//	Attributes

	CECInputCtrl*	m_pInputCtrl;	//	Input controller

	int			m_VK2LKMap[NUM_VIRTUALKEY][NUM_CKCOMBO];		//	Virtual key to logic key map
	LK2VKMap	m_LK2VKMap;		//	Logic key map to virtual key map

protected:	//	Operations

	//	Translate virtual key to logic key
	int ConvertVKToLK(int vk, DWORD dwCtrlKeys);
	//	Compress virtual key and control key states
	DWORD CompressVK(int vk, DWORD dwCtrlKeys);
	//	Decompress virtual key and control key states
	void DecompressVK(DWORD dwSrc, int& vk, DWORD& dwCtrlKeys);
	//	Register logic key
	void RegisterLogicKey(int lk, int vk1, int iCKCombo1, int vk2, int iCKCombo2);
	//	Clear specified lk-vk map if it exists
	void ClearLogicKeyMap(int lk, int iClearFlag);
	//	Comvert control key states to combo index
	int GetCKComboIndex(DWORD dwCtrlKeys);
	//	Clear all logic keys
	void ClearAllLogicKeys();
	

};

///////////////////////////////////////////////////////////////////////////
//	
//	Inline functions
//	
///////////////////////////////////////////////////////////////////////////


