// File		: DlgFriendList.h
// Creator	: Xiao Zhou
// Date		: 2005/8/18

#pragma once

#include "DlgBase.h"
#include "EC_Friend.h"
#include <vector.h>
#include <hashmap.h>
#include "AUI\\AUITreeView.h"
#include "AUI\\AUIListBox.h"
#include "AUI\\AUIStillImageButton.h"

class CDlgFriendList : public CDlgBase  
{
	friend class CDlgFriendColor;
	friend class CDlgFriendOptionGroup;
	friend class CDlgFriendOptionName;
	friend class CDlgFriendMoveGroup;
	friend class CDlgEnemyOptionName;
	
	AUI_DECLARE_COMMAND_MAP()
	AUI_DECLARE_EVENT_MAP()

public:
	enum
	{
		ENEMYLIST_ACTION_UPDATELIST,
		ENEMYLIST_ACTION_UPDATE
	};

	struct ENEMY_INFO
	{
		int id;
		ACString name;
		int pktime;
		char lineid;
		char status;
	};
	
	virtual int CommonMessage(AString strMsg, WPARAM wParam, LPARAM lParam);
	void OnCommandCANCAL(const char *szCommand);
	void OnCommandSetPage(const char *szCommand);
	void OnCommandAdd(const char *szCommand);
	void OnCommandAddFriend(const char *szCommand);
	void OnCommandRemove(const char *szCommand);
	void OnCommandChat(const char *szCommand);
	void OnCommandInvite(const char *szCommand);
	void OnCommandPreserve(const char *szCommand);
	void OnCommandUpdate(const char *szCommand);
	void OnCommandSetting(const char *szCommand);
	void OnCommandCircle(const char *szCommand);

	void OnEventButtonDBCLK(WPARAM wParam, LPARAM lParam, AUIObject * pObj);
	void OnEventRButtonUp(WPARAM wParam, LPARAM lParam, AUIObject * pObj);
	void OnEventRButtonUpEnemy(WPARAM wParam, LPARAM lParam, AUIObject * pObj);

	CDlgFriendList();
	virtual ~CDlgFriendList();

	void BuildFriendList(PAUIDIALOG pDlg = NULL, int curSel = 0);
	void FriendAction(int idPlayer, int idGroup, int idAction, int nCode);
	void AddBlackList(const ACHAR* szName);
	void EnemyListAction(int idAction, void *pData);
	bool IsEnemy(int id);

public:

	static bool  GetFriendRemark(int iPlayerId,ACString& strName);
	static bool  AddFriendRemark(int iPlayerId,ACString& strName);
	static bool  DeleteFriendRemark(int iPlayerId);

	enum
	{
		FRIEND_ACTION_FRIEND_ADD = 0,
		FRIEND_ACTION_FRIEND_DELETE,
		FRIEND_ACTION_FRIEND_UPDATE,
		FRIEND_ACTION_GROUP_ADD,
		FRIEND_ACTION_GROUP_DELETE,
		FRIEND_ACTION_GROUP_RENAME,
		FRIEND_ACTION_INFO_REFRESH,
		FRIEND_ACTION_INFO_LEVEL,
		FRIEND_ACTION_INFO_PROFESSION,
		FRIEND_ACTION_INFO_CULTIVATION,
		FRIEND_ACTION_INFO_AREA,
		FRIEND_ACTION_INFO_GT,
	};

	typedef struct _BLACK_CACHE
	{
		DWORD dwTime;
		ACString strName;
	} BLACK_CACHE, * P_BLACK_CACHE;
	
	int m_dwBlackListGetNameTime;
	
protected:
	virtual bool OnInitDialog();
	virtual void OnShowDialog();
	virtual void OnTick();
	P_AUITREEVIEW_ITEM SearchItem(int id, bool bSearchPlayer);
	P_AUITREEVIEW_ITEM InsertFriend(CECFriendMan::FRIEND* fd, PAUITREEVIEW pTree, P_AUITREEVIEW_ITEM pRoot, bool bSel = false);
	void EnableObject(PAUIDIALOG pDlg, const char *pszCommand, bool bEnable = true);
	
	abase::vector<BLACK_CACHE> m_vecBlackCache;
	abase::hash_map<int, ENEMY_INFO> m_mapEnemyList;
	PAUITREEVIEW			m_pTvFriendList;
	PAUILISTBOX				m_pLstEnemyList;
	PAUISTILLIMAGEBUTTON	m_pBtn_Add;
	PAUISTILLIMAGEBUTTON	m_pBtn_AddFriend;
	PAUISTILLIMAGEBUTTON	m_pBtn_Preserve;
	PAUISTILLIMAGEBUTTON	m_pBtn_Invite;
	PAUISTILLIMAGEBUTTON	m_pBtn_Update;
	PAUISTILLIMAGEBUTTON	m_pBtn_Chat;
	PAUISTILLIMAGEBUTTON	m_pBtn_Setting;
	PAUISTILLIMAGEBUTTON	m_pBtn_Circle;
	
	AString m_szCurPage;
	bool	m_bGetEnemyList;
};
