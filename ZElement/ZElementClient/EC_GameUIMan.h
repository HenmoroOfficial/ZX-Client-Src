/*
 * FILE: EC_GameUIMan.h
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

#include <windowsx.h>

#include "A3DEngine.h"
#include "A3DViewport.h"
#include "EC_StringTab.h"
#include "EC_NetDef.h"
#include "AUI\\AUILuaManager.h"
#include "AUI\\AUITextArea.h"
#include "vector.h"

#ifdef LUA_UI
#include "LuaScript.h"
#endif


#ifndef LUA_UI
class  CLuaState;
#endif
struct talk_proc;
struct NPC_ESSENCE;
struct sAchievement;

class CLuaDlg;
class CECUIData;

///////////////////////////////////////////////////////////////////////////
//	
//	Define and Macro
//	
///////////////////////////////////////////////////////////////////////////
#define CECGAMEUIMAN_MAX_MARKS				10
#define CECGAMEUIMAN_MARK_NAME_LEN			8
#define CECGAMEUIMAN_MAX_GROUPS				10
#define CECGAMEUIMAN_MAX_CHATS				10
#define CECGAMEUIMAN_MAX_MINIMIZEWINDOWS	13
#define CECGAMEUIMAN_MAX_QSHOPITEMS			9
#define CECGAMEUIMAN_MAX_PROFRESSION		128
#define	CECGAMEUIMAN_MAX_TALISMANSLOT		14
#define CECGAMEUIMAN_MAX_MONSTER_PROFESSION	16

#define TALKPROC_IS_TERMINAL(id)		(((id) & 0x80000000) && ((id) & 0x40000000))
#define TALKPROC_IS_FUNCTION(id)		((id) & 0x80000000)
#define TALKPROC_IS_WINDOW(id)			(!TALKPROC_IS_FUNCTION(id))
#define TALKPROC_GET_FUNCTION_ID(id)	((id) & 0x7FFFFFFF)

#define CECGAMEUIMAN_DISCOUNT(m)		int(0.3f * float(m))

#define FVF_MAPTLVERTEX  (D3DFVF_XYZRHW | D3DFVF_DIFFUSE | D3DFVF_SPECULAR | D3DFVF_TEX2)

enum
{
	CLIENT_CHAT_MIN		= GP_CHAT_MAX,
	CLIENT_CHAT_MONEY	= CLIENT_CHAT_MIN,
	CLIENT_CHAT_EXPERIENCE,
	CLIENT_CHAT_PICKUP,
	CLIENT_CHAT_KILL,
	CLIENT_CHAT_RESERVED3,
	CLIENT_CHAT_MAX,
};

///////////////////////////////////////////////////////////////////////////
//	
//	Types and Global variables
//	
///////////////////////////////////////////////////////////////////////////
class A3DStream;
class A3DPixelShader;
class CECShortcutSet;
class CECInventory;
class CECIvtrItem;
class CELPrecinct;
class CECCustomizeMgr;
class AUIImagePicture;

///////////////////////////////////////////////////////////////////////////
//	
//	Declare of Global functions
//	
///////////////////////////////////////////////////////////////////////////

///////////////////////////////////////////////////////////////////////////
//	
//	Class CECGameUIMan
//	
///////////////////////////////////////////////////////////////////////////
#ifdef LUA_UI
class CECGameUIMan : public AUILuaManager, public CLuaScript
#else
class CECGameUIMan : public AUILuaManager
#endif
{
#include "EC_GameUIMan1.inl"

public:		//	Constructor and Destructor
	CECGameUIMan();
	virtual ~CECGameUIMan();

public:		//	Attributes
	enum
	{
		NOTIFY_PRODUCE_START = 0,
		NOTIFY_PRODUCE_END_ONE,
		NOTIFY_PRODUCE_END,
		NOTIFY_PRODUCE_NULL,
		NOTIFY_SPLIT_START,
		NOTIFY_SPLIT_END,
		NOTIFY_IDENTIFY_END
	};
	enum
	{
		ICONS_ACTION = 0,
		ICONS_SKILL,
		ICONS_INVENTORY,
		ICONS_STATE,
		ICONS_SKILLGRP,
		ICONS_GUILD,
		ICONS_PET,
		ICONS_LOTTERY,
		ICONS_POKER,
		ICONS_GARDEN,	// 庄园小图标
		ICONS_DAILYPROMP24,	// 每日任务界面24*24
		ICONS_DAILYPROMP32,	// 每日任务界面32*32
		ICONS_DAILYPROMP48,	// 每日任务界面48*48
		ICONS_GEM16,		// 宝石16x16
		ICONS_RUNE,			// 符语16x16
		ICONS_XPSKILL,		// xp技能圆图标
		ICONS_ASTROLOGY,	// 星座16x16
		ICONS_TOWNNPCMONSTER,
		ICONS_MAX
	};
	enum
	{
		ICON_HINT_GEM	=	0,
		ICON_HINT_RUNE,
	};
	enum
	{
		CURSOR_STATE_NORMAL,
		CURSOR_STATE_REPAIRING,
		CURSOR_STATE_SEPERATING,
		CURSOR_STATE_GARDENOPERATE,
		CURSOR_STATE_ZOOM,
	};
	enum
	{
		ICONLIST_TRANSFER,
		ICONLIST_GTICON,
		ICONLIST_KINGDOM,
		ICONLIST_GTOFFLINE,
		ICONLIST_RECOMMEND,
		ICONLIST_CHANNEL,
//		ICONLIST_RECOMMEND_BLANK,
		ICONLIST_NUM,
	};

public:		//	Operations
	virtual bool Init(A3DEngine* pA3DEngine, A3DDevice* pA3DDevice, const char* szDCFile=NULL);
	virtual bool Release(void);
	virtual bool Tick(DWORD dwDeltaTime);
	virtual bool Render(void);
	virtual bool OnCommand(const char* szCommand, AUIDialog* pDlg);
	virtual bool OnMessageBox(int iRetVal, AUIDialog* pDlg);
	virtual bool DealWindowsMessage(UINT uMsg, WPARAM wParam, LPARAM lParam);
	virtual void RearrangeWindows(A3DRECT rcOld, A3DRECT rcNew);
	virtual bool MessageBox(const char *pszName, const ACHAR *pszText,
		DWORD dwType = MB_OK, A3DCOLOR color = A3DCOLORRGBA(255, 255, 255, 160), PAUIDIALOG *ppDlg = NULL);

	bool MessageBox(int id) { return MessageBox("",GetStringFromTable(id));}

	void AddConsoleLine(const ACHAR* szText, A3DCOLOR col);
	void ChangePrecinct(CELPrecinct *pPrecinct);
	// 这个函数包含一些逻辑处理，目前主要是接到任务弹出相应梦回河阳城副本界面
	// TASK_MSG_NEW = 1
	void AddTaskHint(int idTask, int idEvent, const ACHAR *pszHint = NULL);
	void AddChatMessage(const ACHAR *pszMsg, char cChannel,	int idPlayer = -1, const ACHAR *pszPlayer = NULL, 
		char byFlag = 0, char cEmotion = 0, CECIvtrItem *pItem = NULL, sAchievement* pAchievement = NULL);
	void AddFriendMessage(const ACHAR *pszMsg, int idPlayer, const ACHAR *pszPlayer, BYTE byFlag, char cEmotion = 0);
	void AddInformation(int nType, const char *pszType, const ACHAR *pszMsg,
		DWORD dwLife, DWORD dwData1, DWORD dwData2, DWORD dwData3);

	void FilterBadWords(ACString &str);
	void FilterImages(ACString &str);
	A3DCOLOR GetItemColor(CECIvtrItem* pItem);
	ACString GetLine(int idLine, bool bNumber=false);

	bool UpdateTask();
	void PopUpMonsterInfo(unsigned long ulTask, unsigned long ulMonsterID);
	void UpdateFactionList(bool bFaction = true);
	void UpdateChannelList(void* pChannelList);
	bool UpdateTeach(int nPage = -1);
	bool UpdateTeam(bool bUpdateNear = false);

	void PopupReviveDialog(bool bOpen = true, int revive_countdown = 0);
	void PopupStorageDialog(bool bClose = false, bool bFaction = false);
	void PopupInviteGroupMenu(int idLeader, const ACHAR* szLeaderName, int iTeamSeq, int nMode = 0, int iFamilyId = 0, int iGuildId = 0);
	void PopupTradeInviteMenu(int idPlayer, DWORD dwHandle);
	void PopupTradeResponseMenu(int idPlayer, int idTrade, bool bAgree);
	void PopupFactionInviteMenu(int idPlayer, const ACHAR *pszFaction, DWORD dwHandle);
	void PopupSectInviteMenu(int idPlayer, DWORD dwHandle);
	void PopupPlayerContextMenu(int idPlayer = -1, int x = -1, int y = -1);
	void PopupFriendInviteMenu(int idPlayer, const ACHAR *pszName, DWORD dwHandle);
	void PopupBoothDialog(bool bOpen = true, bool bOwner = true, bool bPrepare = false);
	void PopupNPCDialog(talk_proc *pTalk);
	void PopupNpcMainDialog(talk_proc *pTalk);
	void PopupNPCDialog(const NPC_ESSENCE *pEssence);
	void PopupTaskFinishDialog(int idTask, talk_proc *pTalk);
	void PopupTaskCheckInfo(int idTask, bool bRemove = false);
	void PopupFaceLiftDialog();
	void PopupInstanceCountDown(int nTime);
	void PopupFactionCombatMenu(const ACHAR *pszFaction, DWORD dwHandle);
	void PopupPKMessage(int idTarget, bool bIsMonster);
	void PopupKingGuardTransmit(const ACHAR *pszKing,DWORD dwHandle);
	void PopupKingOpenBathInvite(DWORD dwHandle);
	void PopupKingBathInvite(const ACHAR *pszName,int who);
	void PopupKingBathFlyToNpc();
	//弹出快雪时晴奖励
	void PopupFriendCallBckSnderAward(const void *str, int len);
	void PopupFriendCallBckBckerAward();
	void PopupCrssvrTeamsInviteMenu(int idPlayer, DWORD dwHandle,void* pData);

	void StartFrdCallBck(int mailSent,char is_scr);
	void OnFrdCallBckRe(int retcode);
	
	void PlayAwardGfx(unsigned long ulGfxID);     //播放任务奖励gfx特效
	void PlayTinyGame(unsigned long ulGameID);    //弹出任务小游戏
	
	bool OpenQShop( const char *pszWinName );	//快捷销售功能

	bool UIControlCursor();
	void ChangeWorldInstance(int idInstance);
	void ServiceNotify(int idService, void *pData);
	void TradeAction(int idPlayer, int idTrade, int idAction, int nCode);
	void FriendAction(int idPlayer, int idGroup, int idAction, int nCode);
	void ChannelAction(int idAction, void *pData);
	void MailAction(int idAction, void *pData);
	void AuctionAction(int idAction, void *pData);
	void BattleAction(int idAction, void *pData);
	void GoldAction(int idAction, void *pData);
	void EnemyListAction(int idAction, void *pData);
	void RankListAction(void *pData);
	void RankCrssvrTemas(void * pData);
	void ShowErrorMsg(const ACHAR *pszMsg, const char *pszName = "");
	void SetMarqueeMsg(const ACHAR *pszMsg);
	void GetUserLayout(void *pvData, DWORD &dwSize);
	bool SetUserLayout(void *pvData, DWORD dwSize);
	void BuildFriendList(PAUIDIALOG pDlg = NULL);
	void EndNPCService();
	void GetEmotionList(A2DSprite **ppA2DSprite,
		abase::vector<AUITEXTAREA_EMOTION> **ppEmotion, char cEmotion = 0);
	void SetTempTargetInMiniMap(const A3DVECTOR3& vecPos);
	
	A2DSprite* GetFactionIcon(int idServer, int idFaction, int *pIndex);
	bool SetImgIconSet(AUIImagePicture *pImg, int type, const char *szName);
	A2DSprite* GetSpriteIcon(int type, const char *szName, int *pIndex);
	A2DSprite* GetIconSet(int type) { return m_pA2DSpriteIcons[type]; }
	int		   GetIconId(int type, const AString strName) { if(m_IconMap[type].find(strName)!=m_IconMap[type].end()) return m_IconMap[type][strName]; else {return 0;}}
	void SetIvtrInfo(AUIImagePicture* pImg, int id, int num = 1, bool bClearInfo = false, bool setHint=true);
	void SetIvtrInfo(AUIImagePicture* pImg, CECIvtrItem* pItem, int num = 1, bool setHint=true);
	void SetSkillInfo(AUIImagePicture* pImg, int id, int level = 1, bool bDelete = false);

	A2DSprite* LoadTitleImage(AString strName, int iframenum);
	
	ACString GetCompareEquipDesc(CECIvtrItem *pItem);
	void GetItemEmphase(int id);
	void UnfreezeAllItems();

	//	Set UI enable flag
	void EnableUI(bool bEnable);
	bool IsUIEnabled() { return m_bUIEnable; }

	bool IsShowAllPanels() { return m_bShowAllPanels; }
	
	void SetShowAllPanels(bool bShow) { m_bShowAllPanels = bShow; }
	
	void SetReplayMode(bool bReplayMode) { m_bReplayMode = bReplayMode; }

	void SetHideUIPrintMode(bool bMode) { m_bHideUIPrintMode = bMode; }

	inline int GetPixelsByPos(float x)
	{ return int((m_nMinSize * (x)) / (2.0f * 11.0f * 512.0f)); }
	inline float GetPosByPixels(int x)
	{ return 2.0f * 11.0f * 512.0f * float(x) / float(m_nMinSize); }
	
	typedef abase::hash_map<int, int> IDOrderMap;
	const IDOrderMap& GetIDOrderMap() { return m_IDOrderMap; }
	void AddImageListItem(A2DSprite* pSprite, int& index);
	
	void Int64ToStr(INT64 num, ACString& str);
	AString URLEncode(const ACHAR *src, UINT CodePage = CP_UTF8);
	ACString URLDecode(const char *src, UINT CodePage = CP_UTF8);
	void FormatMoney(int money, ACString& str);
	void InvokeDragDrop(PAUIDIALOG pDlg, PAUIOBJECT pObj, POINT ptCapture);
	int CommonMessage(AString strName, AString strMsg, WPARAM wParam, LPARAM lParam);
	A3DFTFont* GetPateFontName();
	void LoginPromp();
	void AttachEmotion(PAUIOBJECT pObj);
	void AttachTaskEmotion(PAUIOBJECT pObj);
	abase::vector<A2DSprite*>* GetControlImgLst() { return &m_vecImageListControl; }
	void InvokeNumberDialog(PAUIOBJECT pObjSrc, PAUIOBJECT pObjOver, int nMode, int nMax);

	CECUIData* GetUIData() { return m_pUIData; }
	bool IsImageHelpOpen();
	
	NPC_ESSENCE*	GetNpcEssence() { return m_pCurNPCEssence; }
	void PlayItemSound(CECIvtrItem *pItem, bool bDown);
	
	void UpdateTextCoord(ACString &str);

	void SetShowTouchMin(bool isshow){m_bShowTouchmin = isshow;}

	bool CheckCanTaskFly(int idTask,int idNpc);
	bool CheckTaskReachSiteFly(int idTask);
	bool CheckCanDailyPrompFly(int id);

	static void OpenUrl(const AString& url);

	void OnLogicKey_UpdateUI(int iLogicKey, bool bKeyDown);
	void OnLogicKey_UpdateShortCut(int iLogicKey, bool bKeyDown);

	void UpdateQuickBarState();

	void SwitchToPrivateChat(ACString strName,bool bNeedForcus = true,bool bClickName = false);

protected:	//	Attributes
	typedef struct _SAVE_MARK
	{
		int nNPC;
		A3DVECTOR3 vecPos;
		ACHAR szName[CECGAMEUIMAN_MARK_NAME_LEN + 1];
		int	idInstance;
	} SAVE_MARK, * P_SAVE_MARK;
	typedef struct _TALISMAN_ITEM
	{
		int type;
		int	data;
	} TALISMAN_ITEM, * P_TALISMAN_ITEM;
	typedef struct _USER_LAYOUT
	{
		__int8 nVersion;
		__int8 nMapMode;
		char nQuickbar10Mode;	//无用
		char nQuickbar8Mode;	//用作图形帮助系统是否开启
		bool bQuickbarLock;	
		bool bGTOnlineNotify;
		int nChatWinSize;
		int nCurChatColor;
		SAVE_MARK a_Mark[CECGAMEUIMAN_MAX_MARKS];
		bool bOnlineNotify;
		bool bAutoReply;
		bool bSaveHistory;
		int idGroup[CECGAMEUIMAN_MAX_GROUPS];
		A3DCOLOR clrGroup[CECGAMEUIMAN_MAX_GROUPS];
		TALISMAN_ITEM a_TalismanItem[CECGAMEUIMAN_MAX_TALISMANSLOT]; // 0-2:item	3-9:skill	10-12:itemup	13:item
		int nAutoFoodSlot;
	} USER_LAYOUT, * P_USER_LAYOUT;
	typedef struct _TASK_HINT
	{
		DWORD dwTime;
		ACString strHint;
	} TASK_HINT, *P_TASK_HINT;


public:
	//技能栏界面相关布局
	typedef struct _SQuickBarLayOut
	{
	   bool bShowAllGrid;
	   bool bShowRightBar;
	   bool bShowCustom_Bar1;
	   bool bShowCustom_Bar2;
	   BYTE nCutomBar1_Type;
	   BYTE nCutomBar2_Type;
	   _SQuickBarLayOut()
	   {
	     memset(this,0,sizeof(_SQuickBarLayOut));
		 bShowAllGrid = true;
	   }

	}SQuickBarLayOut; 

	//快捷键界面配置
	typedef struct _SShortcutsUIInfo
	{
	    int  iLk;
		//界面名称
		AString strDlg;
		//控件名称
		AString strControl;
		//是否跟踪悬浮
		bool bHint;
		//是否跟踪文本
		bool bText;
		//前置悬浮
		AWString strPreHint;
		//后置悬浮
		AWString strPostHint;
		//前置文本
		AWString strPreText;
		//后置文本
		AWString strPostText;
	}SShortcutsUIInfo;


	typedef abase::vector<SShortcutsUIInfo>   ShortcutsUIInfoList;

protected:
	
	int	m_nAccountTimeIndex;
	CECStringTab m_ConsoleCmds;		//	Console command strings

	int m_nMinSize;

	POINT m_ptLButtonDown;

	A2DSprite *m_pA2DSpriteMask;
	A2DSprite *m_pA2DSpriteIcons[ICONS_MAX];
	abase::hash_map<int, A2DSprite*> m_mapA2DSpriteGuildIcons;
	abase::hash_map<int, AString> m_IconSound;
	abase::hash_map<AString, int> m_IconMap[ICONS_MAX];
	abase::vector<A2DSprite*> m_vecImageList;
	abase::vector<A2DSprite*> m_vecImageListControl;
	
	abase::hash_map<AString, A2DSprite*> m_mapA2dSpriteTitle;

	bool m_bClickedChatPart;
	bool m_bClickedUserName;
	A2DSprite *m_pA2DSpriteEmotion[AUIMANAGER_MAX_EMOTIONGROUPS];

	A2DSprite *m_pA2DSpriteProfs[CECGAMEUIMAN_MAX_PROFRESSION];
	A2DSprite *m_pA2DSpriteMonsterProfs[CECGAMEUIMAN_MAX_MONSTER_PROFESSION];

	abase::vector<AUITEXTAREA_EMOTION> *m_vecEmotion[AUIMANAGER_MAX_EMOTIONGROUPS];
	A2DSprite *m_pA2DSpriteEmotionTask[AUIMANAGER_MAX_EMOTIONGROUPS];
	abase::vector<AUITEXTAREA_EMOTION> *m_vecEmotionTask[AUIMANAGER_MAX_EMOTIONGROUPS];
	abase::vector<ACString> m_vecBadWords;

	IDOrderMap m_IDOrderMap; // Show Order of Ivtr Item. Order: 0, 1, ...

	abase::hash_map<AString, ACString> m_QshopKeywordMap;

	bool m_bClickedTaskTrace;
	int m_nCursorState;
	int m_idCurFinishTask;
	NPC_ESSENCE *m_pCurNPCEssence;

	A3DVECTOR3 m_vecTargetPos;
	CELPrecinct *m_pCurPrecinct;
	A2DSprite *m_pA2DSpriteTarget;
	A2DSprite *m_pA2DSpriteMoveArrow;

	int m_idTrade;
	bool m_bShowAllPanels;
	bool m_bHideUIPrintMode;
	PAUIDIALOG m_pShowOld;
	TASK_HINT m_ZoneHint;
	abase::vector<int> m_vecBrokenList;
	abase::vector<TASK_HINT> m_vecTaskHint;
	bool m_bAutoReply, m_bOnlineNotify, m_bGTOnLineNotify, m_bSaveHistory;
	int		m_iCurWallowLevel;
	
	bool m_bUIEnable;		//	UI enable flag
	bool m_bReplayMode;
	int  m_iTalismanSkillDumb[CECGAMEUIMAN_MAX_TALISMANSLOT];	// 保存法宝栏天界没有的技能

	CECCustomizeMgr *m_CustomizeMgr;
	A3DFTFont *m_pPateFontName;
	FONT_WITH_SHADOW	m_FontHint;
	
	CECUIData*	m_pUIData;

	//is touchmin first
	bool  m_bShowTouchmin;
	//quikbar layout
	SQuickBarLayOut  m_QuickBarLayout;

    char  m_cUISize;
	bool m_bSettingHotKey;	// 自定义快捷键

	ShortcutsUIInfoList   m_vecShortCutsUIInfo;
	CLuaState             *m_pState;

protected:

	bool InitShortCutsUIInfo();
	bool LoadShortCutsUIInfo();
	void ReleaseShortCutsUIInfo();
	void RefreshShortCutUIInfo();

public:

	void SettingHotKey(bool b) { m_bSettingHotKey=b; }
	bool IsSettingHotKey() { return m_bSettingHotKey; }
	SQuickBarLayOut* GetQuickBarLayout(){return &m_QuickBarLayout;}

	char  GetUIWindowScale(){return m_cUISize;}
	void  SetUIWindowScale(char cUISize);
	float GetCustomUIScale();
	void  _InternalArrangeWindow();

protected:	//	Operations
	bool LoadIcon(char* szFile, int iNumItem=1, A3DRECT* aRects=NULL, A3DCOLOR colorKey=0);
	bool LoadIconSet();
	void InitDialogs();

	void ParseUserConsoleCommand();
	void ParseDebugConsoleCommand();
	void ParseGMConsoleCommand();
	bool CompConsoleCommand(int iCmd);
	bool OnDlgCmd_Console(const ACHAR* szCommand);

	void RefreshHint();
	void RefreshBrokenList();
	bool UpdateHint();
	bool UpdateArrowInfo();
	void UpdateKingBathState();

	bool PlayerIsBlack(int idPlayer);
	bool DealGameHotkeys(UINT uMsg, WPARAM wParam, LPARAM lParam);
	void AddHistory(int idPlayer, DWORD dwTime, const ACHAR *pszPlayer, const ACHAR *pszMsg);
	void DisplayMiniButtons();
	void UpdateButtonsByTime();
	bool IsHotWebRunning();

	static CECShortcutSet * GetSCSByDlg(const char *pszDlg);
	static int GetItemCount(int idItem, CECInventory *pPack = NULL);

	virtual PAUIDIALOG CreateDlgInstance(const AString strTemplName);

public:
#ifdef LUA_UI
	bool LuaInit(const char * szScript);
	bool LuaRelease();
	void LuaEvent(int eventID, DWORD arg1 = 0, DWORD arg2 = 0, const char* arg3 = NULL, const char* arg4 = NULL);
#endif
};

ACString FilterCoordString(const ACHAR* szText);