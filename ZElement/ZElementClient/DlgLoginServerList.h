// Filename	: DlgLoginServerList.h
// Creator	: Xiao Zhou
// Date		: 2005/11/4
// Desc		: 
// 外服同一ip可能对应多个port，都是一个服务器，
// 内服同一ip多个port必然对应多个服务器

#pragma once

#include "hashmap.h"
#include "vector.h"
#include "AUI\\AUILabel.h"
#include "AUI\\AUIEditBox.h"
#include "AUI\\AUIDialog.h"
#include "AUI\\AUIListBox.h"
#include "AUI\\AUIStillImageButton.h"

typedef abase::hash_map<ACString, int>	ServerIDMap;
class A2DSprite;
class CECPool;

class CDlgLoginServerList : public AUIDialog  
{
	AUI_DECLARE_COMMAND_MAP();
	AUI_DECLARE_EVENT_MAP();

public:
	
	struct SERVERINFO
	{
		ACString strGroup;
		ACString strName;
		abase::vector<AString> vecAddress;
		abase::vector<int>	vecPorts;
		int		iId;
		int		iType;		// -1-无 0-双线 1-电信 2-网通
	};

	struct SERVERENTRY
	{
		SERVERINFO info;
		int		serverID;
		int		pingID;

		in_addr addr;
		bool	addr_valid;
		int		ping_stage;		// -1-未查看 0-查看未ping 1-ping中 2-ping成功
		bool	ping_ok;
		DWORD	num;
		DWORD	lasttime;

		HANDLE	getnameThread;				// 应该加锁，目前没有加锁使用需要小心
		SERVERENTRY() : addr_valid(false), ping_stage(-1) {}
	};

	enum
	{
		SERVER_STATUS_EXP,
		SERVER_STATUS_MONEY,
		SERVER_STATUS_OBJECT,
		SERVER_STATUS_NUM
	};

	enum
	{
		GROUP_TYPE_DOUBLE,
		GROUP_TYPE_DIANXIN,
		GROUP_TYPE_WANGTONG,
	};

public:
	CDlgLoginServerList();
	virtual ~CDlgLoginServerList();

	void OnCommandCancel(const char* szCommand);
	void OnCommandConfirm(const char* szCommand);
	void OnCommandSearch(const char* szCommand);
	void OnCommandSortName(const char* szCommand);
	void OnCommandSortSpeed(const char* szCommand);
	void OnCommandSortStats(const char* szCommand);
	void OnCommandSortOther(const char* szCommand);
	
	// add group strings into listbox
	void OnCommandArea(const char* szCommand);

	// select line type
	void OnCommandLineType(const char* szCommand);
	
	void OnEventLBtnDown_LstServerNew(WPARAM wParam, LPARAM lParam, AUIObject * pObj);
	void OnEventSelect_LstServerNew(WPARAM wParam, LPARAM lParam, AUIObject * pObj);
	void OnEventLBtnDown_LstServer(WPARAM wParam, LPARAM lParam, AUIObject * pObj);
	void OnEventSelect_LstServer(WPARAM wParam, LPARAM lParam, AUIObject * pObj);
	void OnEventKeyDown(WPARAM wParam, LPARAM lParam, AUIObject * pObj);
	
	bool SetServerAddress(ACString strName, ACString strGroup=_AL(""));
	void ChangeGroup(ACString strGroup, ACString strSvr = ACString(_AL("")));

	// 重新随机选择一个端口
	void ResetPort();
	
protected:
	virtual bool OnInitDialog();
	virtual bool Release();
	virtual void OnShowDialog();
	virtual void OnTick();
	virtual void Resize(A3DRECT rcOld, A3DRECT rcNew);
	
	void LoadServerInfo(ACString strCurGroup);

	// Load server\\servergroup_new.txt
	void LoadGroups(ACString strCurGroup);

	bool								m_bSortName;
	bool								m_bSortSpeed;
	bool								m_bSortStats;
	bool								m_bSortOther;
	int									m_iCurSelGroup;		// 当前选择的服务器
	PAUILISTBOX							m_pLst_Group;

	// "time" "index" "load"
	PAUILISTBOX							m_pLst_Server;
	PAUILISTBOX							m_pLst_ServerNew;
	abase::vector<SERVERENTRY>			m_servers;			// 服务器信息
	ServerIDMap							m_mapServerRealID;	// 内服XXOO	29000:10.68.12.11	16 最后一个数
	int									m_nServerSum;
	PAUISTILLIMAGEBUTTON				m_pBtn_Choose;
	CECPool*							m_pPool;

	abase::vector<A2DSprite*>			m_vecImageList;

	abase::vector<ACString>			m_strServersRecommend;
	abase::vector<ACString>			m_vecGroups[3];
};

