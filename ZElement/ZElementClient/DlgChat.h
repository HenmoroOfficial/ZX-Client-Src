// Filename	: DlgChat.h
// Creator	: Tom Zhou
// Date		: October 15, 2005

#pragma once

#include "DlgBase.h"
#include "AUI\\AUITextArea.h"
#include "AUI\\AUIScroll.h"
#include "EC_GameUIMan.h"

#define CDLGCHAT_MAX_CHAT_ALPHA	6

class CECIvtrItem;
struct sAchievement;

enum
{
   CHATCHANNEL_COMMON,		//普通
   CHATCHANNEl_TEAM,		//组队
   CHATCHANNEL_PRIVATE,		//密语
   CHATCHANNEL_FACTION,		//帮派
   CHATCHANNEL_FAMILY,		//家族
   CHATCHANNEL_WORLD,		//世界
   CHATCHANNEL_CIRCLE,		//圈子
   CHATCHANNEL_SAMESERVER,	//同服
   CHATCHANNEL_COUNT,
};

class CDlgChat : public CDlgBase  
{
	friend class CECGameUIMan;
	friend class CDlgFriendChat;
	friend class CDlgChannelChat;
	friend class CDlgSettingGame;

	friend class CDlgCrssvrTeamsOptionName;
	AUI_DECLARE_EVENT_MAP();
	AUI_DECLARE_COMMAND_MAP()

public:
	enum
	{
		DLGCHAT_SHOWINFO_BOTH = 0,
		DLGCHAT_SHOWINFO_11,
		DLGCHAT_SHOWINFO_NONE,
		DLGCHAT_SHOWINFO_12,
		DLGCHAT_SHOWINFO_MAX
	};
	typedef struct _CHAT_MSG
	{
		DWORD dwTime;
		char cChannel;
		ACString strMsg;
		CECIvtrItem* pItem;
		sAchievement* pAchievement;
		int nMsgIndex;
		int nPack;
		int nSlot;
		int nAchievement;
	} CHAT_MSG, * P_CHAT_MSG;

	typedef struct _QUICK_TALK
	{
		ACString strName;
		ACString strTarget;
		ACString strNoTarget;
		bool	 bOk;
	} QUICK_TALK, * P_QUICK_TALK;

public:
	CDlgChat();
	virtual ~CDlgChat();

	void OnCommand_close(const char * szCommand);
	void OnCommand_lock(const char * szCommand);
	void OnCommand_speak(const char * szCommand);
	void OnCommand_popface(const char * szCommand);
	void OnCommand_speakmode(const char * szCommand);
	void OnCommand_channel(const char * szCommand);
	void OnCommand_chgalpha(const char * szCommand);
	void OnCommand_clear(const char * szCommand);
	void OnCommand_set(const char * szCommand);
	void OnCommand_send(const char * szCommand);
	void OnCommand_joinchannel(const char * szCommand);
	void OnCommand_CANCEL(const char * szCommand);
	void OnCommandMinimize(const char *szCommand);
	void OnCommand_Action(const char *szCommand);
	void OnCommand_Talk(const char *szCommand);
	void OnCommand_PopFaceManage(const char *szCommand);
	void OnCommand_TalkAction(const char *szCommand);
	void OnCommand_TalkSave(const char *szCommand);
	void OnCommand_TalkNew(const char *szCommand);
	void OnCommand_TalkDelete(const char *szCommand);
	void OnCommand_TalkReset(const char *szCommand);
	void OnCommand_TalkSortAscend(const char *szCommand);
	void OnCommand_TalkSortDescend(const char *szCommand);
	void OnCommand_LocalChatInfo(const char *szCommand);
	void OnCommand_LocalChatInfoSave(const char *szCommand);
	void OnCommand_LocalChatSet(const char *szCommand);
	void OnCommand_SettingGame(const char *szCommand);
	void OnCommand_CustomMode(const char* szCommand);
	void OnCommand_ChatSetting(const char* szCommand);

	void OnEventChar_Txt_Speech(WPARAM wParam, LPARAM lParam, AUIObject *pObj);
	void OnEventKeyDown_Txt_Speech(WPARAM wParam, LPARAM lParam, AUIObject *pObj);
	void OnEventSysChar_Txt_Speech(WPARAM wParam, LPARAM lParam, AUIObject * pObj);
	void OnEventMouseMove_Txt_Content(WPARAM wParam, LPARAM lParam, AUIObject *pObj);
	void OnEventLButtonDown_Txt_Content(WPARAM wParam, LPARAM lParam, AUIObject *pObj);
	void OnEventLButtonUp_Txt_Content(WPARAM wParam, LPARAM lParam, AUIObject *pObj);
	void OnEventRButtonUp_Txt_Content(WPARAM wParam, LPARAM lParam, AUIObject *pObj);
	void OnEventLButtonUp_Face(WPARAM wParam, LPARAM lParam, AUIObject *pObj);
	void OnEventLButtonDown_Img_ZoomIn(WPARAM wParam, LPARAM lParam, AUIObject *pObj);
	void OnEventLButtonUp_Img_ZoomIn(WPARAM wParam, LPARAM lParam, AUIObject *pObj);
	void OnEventMouseMove_Img_ZoomIn(WPARAM wParam, LPARAM lParam, AUIObject *pObj);
	void OnEventLButtonDown_Lst_Word(WPARAM wParam, LPARAM lParam, AUIObject *pObj);
	void OnEventMouseWheel(WPARAM wParam, LPARAM lParam, AUIObject *pObj);
	
	bool IsLocked();
	int GetChatWinSize();
	int GetChatAlpha();
	int GetActiveChannelSet();
	int GetMsgCount(int nWin);
	void IncreaseMsgCount(int nWin);
	abase::vector<CHAT_MSG> & GetAllChatMsgs();
	void RebuildChatContents(int nChannelSet = -1);
	void DetermineChannelByText(const ACHAR *pszText);
	void ReBuildQuickTalkButton();
	void TalkReset();
	void UpdateHint();
	void UpdateChatSetInfo();
	void RearrangeWindow();
	void RefreshChannelTab();

	static void TransformItemColorName(CECIvtrItem *pItem, ACString &strName, A3DCOLOR &color);
	static void TransformAchievementColorName(sAchievement *pAchiv,  ACString &strName, A3DCOLOR &color);

	static bool SaveQuickTalk();
	static bool LoadQuickTalk(bool bReset = false);
	static bool GetPopType() { return m_bPopType;}
	static abase::vector<QUICK_TALK>* GetQuickTalkList() { return &m_vecQuickTalk;}

	static const ACHAR *m_pszColor[CLIENT_CHAT_MAX];
	
	// 改名服务改名后聊天栏名字更新，因为服务器不通知，这个函数暂时没用
	static void ReplaceChatNames(ACString strSrcName, ACString strDstName);

	//
	static int GetSpeekWay(){return m_iSpeakWay;}

protected:
	static int m_nCurChatAlpha;
	static int m_nActiveChannelSet;
	static int m_nChatMsgCount1;
	static int m_nChatMsgCount2;
	static int m_nCurHistory;
	static DWORD m_dwTickFarCry;
	static ACString m_strCurChatText;
	static abase::vector<CHAT_MSG> m_vecHistory;
	static abase::vector<CHAT_MSG> m_vecChatMsg;
	static int m_nMsgIndex;
	static int m_nMouseLastX;
	static int m_nMouseLastY;
	static SIZE m_nOldSize;
	static bool m_bPopType;
	static abase::vector<QUICK_TALK> m_vecQuickTalk;
	static bool m_bLoaded;
	static int	m_iCurrentPage;
	static int	m_iCurrentShow;	//0:1,2  1:1  2: 3:1 4:
	static int	m_iSpeakWay;
	static ACString m_strWhisperName;

	bool m_bLocked;
	PAUIOBJECT m_pObjLock;
	PAUISCROLL m_pScl_TalkView;

	virtual bool OnInitDialog();
	virtual void OnShowDialog();
	virtual	void OnTick();
	virtual bool Render();
	bool GetNameLinkMouseOn(int x, int y, PAUIOBJECT pObj, P_AUITEXTAREA_NAME_LINK pLink);
	bool GetItemLinkItemOn(int x, int y, PAUIOBJECT pObj, AUITEXTAREA_EDITBOX_ITEM * pLink);
	void FilterReturn(ACString &str);
	int GetTextLength(const ACHAR* szText);
};
