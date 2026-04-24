// File		: DlgFriendSeek.h
// Creator	: Fu Chonggang
// Date		: 2009/02/11

#pragma once

#include "vector.h"
#include "hashmap.h"
#include "AUI\\AUIEditBox.h"
#include "AUI\\AUIComboBox.h"
#include "AUI\\AUIListBox.h"
#include "AUI\\AUILabel.h"
#include "AUI\\AUITreeView.h"
#include "AUI\\AUIStillImageButton.h"
#include "AUI\\AUITextArea.h"
#include "DlgBase.h"

struct PIE_LOVE_CONFIG;

namespace GNET
{
	class Protocol;
}

class CDlgFriendSeek : public CDlgBase  
{
	friend class CDlgSnsInfo;
	friend class CDlgEPEquip;

	AUI_DECLARE_COMMAND_MAP()
	AUI_DECLARE_EVENT_MAP()
public:
	enum
	{
		SEEKTYPE_MIN = 0,
		SEEKTYPE_MARIAGE = SEEKTYPE_MIN,
		SEEKTYPE_FRIEND,
		SEEKTYPE_TEACHER,
		SEEKTYPE_GUILD,
		SEEKTYPE_MAX
	};

	enum
	{
		SORTTYPE_ORIG = 0,
		SORTTYPE_FAVOR,
		SORTTYPE_GENDER,
		SORTTYPE_FOCUS,
		SORTTYPE_ANSWER,
		SORTTYPE_LEAVETIME
	};

	enum
	{
		ITEMTYPE_TOP,
		ITEMTYPE_OTHER,
		ITEMTYPE_GREEN,
	};
	
	struct ApplyCondiction
	{
		unsigned char uOccupation;
		unsigned char bGender;
		unsigned char nLevel;
		unsigned char nFactionLevel;
	};

	struct FriendSeekTableInfo
	{
		int			nMsgID;
		int			nRoleID;
		ACString	strName;
		int			nFavor;		//人气
		bool		bGender;
		int			nFocus;		//关注度
		int			nAnswer;	//应征人数
		int			charm_player;
		int			charmstar_player;
		int			nPressTime;
		int			nItemType;
		ACString	strMsg;
		ApplyCondiction aCondiction;
		abase::vector<int> acceptList;
	};

	struct ApplyInfo
	{
		int			nRoleid;
		ACString	strName;
		int			nCount;
		int			nLastTime;
		int			nLevel;
		int			nGender;
	};
	
	struct RoleInfo
	{
		int level;
		int sub_level;
		char gender;
		unsigned char occupation;
		int spouse;
		ACString sectname;
		ACString familyname;
		ACString factionname;
		int faction_level;
	};

	struct PlayerInfo
	{
		int roleid;
		int is_private;
		ACString rolename;
		ACString nickname;
		int age;
		int birthday;
		int province;
		int city;
		ACString career;
		ACString address;
		ACString introduce;
//		int charm_personal;
//		int charm_friend;
//		int charm_sect;
//		int charm_faction;
		int charmvec[4];
		int press_time;
	};
	struct MsgInfo
	{
		short index;
		short dst_index;
		char type;
		int roleid;
		ACString rolename;
		int time;
		ACString message;
		bool bInserted;
	};

	struct PlayerInfoCache
	{
		RoleInfo roleinfo;
		PlayerInfo playerinfo;
	};
	typedef abase::vector<FriendSeekTableInfo> FSTableVector;
	typedef abase::vector<ApplyInfo> ApplyInfoVector;
	typedef abase::hash_map<int, PlayerInfoCache> MsgID2PlayerInfoMap;
	typedef abase::vector<MsgInfo> MsgVector;

	struct GetMessageCache
	{
		ApplyInfoVector vecList[3];
		ApplyInfoVector vecApplyList;
		ApplyInfoVector vecAgreelList;
		ApplyInfoVector vecDenyList;
		MsgVector msgVector;

	};
	typedef abase::hash_map<int, GetMessageCache> MsgID2MsgMap;

public:
	CDlgFriendSeek();
	virtual ~CDlgFriendSeek();

	void OnCommand_SortDayLeft(const char * szCommand);
	void OnCommand_SortGender(const char * szCommand);
	void OnCommand_SortFavor(const char * szCommand);
	void OnCommand_SortName(const char * szCommand);
	void OnCommand_SortFocus(const char * szCommand);
	void OnCommand_SortAnswer(const char * szCommand);
	void OnCommand_SortLevel(const char * szCommand);
	
	
	void OnCommand_RdoType(const char * szCommand);
	
	void OnCommand_Tail(const char * szCommand);
	void OnCommand_Head(const char * szCommand);
	void OnCommand_Next(const char * szCommand);
	void OnCommand_Last(const char * szCommand);
	void OnCommand_NextRank(const char * szCommand);
	void OnCommand_LastRank(const char * szCommand);
	
	void OnCommand_MsgRelease(const char * szCommand);		//弹出发布信息界面
	void OnCommand_MsgWithdraw(const char * szCommand);		//撤销消息
	void OnCommand_Refresh(const char * szCommand);			//更新
	void OnCommand_Support(const char * szCommand);			//支持
	void OnCommand_Oppose(const char * szCommand);			//反对
	void OnCommand_Msg(const char * szCommand);				//留言板
	
	void OnCommand_Information(const char * szCommand);		//查看资料
	void OnCommand_Apply(const char * szCommand);			//申请
	void OnCommand_PressInfo(const char * szCommand);		//发布消息
	void PressInfo();		//发布消息
	void OnCommand_CANCEL(const char * szCommand);

	void OnCommand_ConfirmMarriage(const char * szCommand);
	void OnCommand_ConfirmTeacher(const char * szCommand);
	void OnCommand_ConfirmGuild(const char * szCommand);
	void OnCommand_ConfirmFriend(const char * szCommand);
	void OnCommand_SendBack(const char * szCommand);		//回复消息
	void OnCommand_Accept(const char * szCommand);			//接受请求
	void OnCommand_Refuse(const char * szCommand);			//拒绝请求
	void OnCommand_PersCenter(const char * szCommand);
	void OnCommand_Delete(const char * szCommand);
	void OnCommand_CharmRank(const char * szCommand);

	void OnPrtc_SNSPressMessageRe(GNET::Protocol* pProtocol);
	void OnPrtc_SNSCancelMessageRe(GNET::Protocol* pProtocol);
	void OnPrtc_SNSApplyRe(GNET::Protocol* pProtocol);
	void OnPrtc_SNSListMessageRe(GNET::Protocol* pProtocol);
	void OnPrtc_SNSGetMessageRe(GNET::Protocol* pProtocol);
	void OnPrtc_SNSVoteRe(GNET::Protocol* pProtocol);
	void OnPrtc_SNSGetPlayerInfoRe(GNET::Protocol* pData);
	void OnPrtc_SNSAcceptApplyRe(GNET::Protocol* pData);
	void OnPrtc_SNSDeleteLeaveMsgRe(GNET::Protocol* pData);

	void OnEventLButtonDown_Lst_ListInfo(WPARAM wParam, LPARAM lParam, AUIObject *pObj);
	void OnEventLButtonDown_Lst_ListMsg(WPARAM wParam, LPARAM lParam, AUIObject *pObj);
	void OnEventLButtonDown_Lst_ListInformation(WPARAM wParam, LPARAM lParam, AUIObject *pObj);

	int GetSeekingType() { return m_iSeekingType;}
	int GetMsgID() { return m_iMsgID;}
	int GetMsgIDDst() { return m_iMsgIDDst;}
	ACString GetMsgSendBack() { return m_strMsg; }
	void SetInfoType(int iType) { m_iSeekingType = iType; }
	int GetApplyRoleID() { return m_iApplyRoleID;}
	void InsertMessage(int id, int dstId);
	void SetPlayerInfo(int is_private, const ACHAR* szNickname, int birthday, int province, int city, const ACHAR* szCareer, const ACHAR* szAddress, const ACHAR* szIntroduce);
	void AddCharmRank();
	int Charm2Star(int charm);
	void DeleteLeaveMsg();
	void ChangeTitle();
protected:
	virtual bool OnInitDialog();
	virtual void OnShowDialog();
	virtual void OnTick();
	void SortList();
	void AddToList(int iPage = -1);
	void AddVoteList(int iPage = -1);
	void AddTreeList();
	bool CheckProfression(int cont);
	P_AUITREEVIEW_ITEM InsertParentMsg(MsgVector& msgVer, int index);
	P_AUITREEVIEW_ITEM GetSuitItem(P_AUITREEVIEW_ITEM item, int data);
	void FilterReturn(ACString &str);
	void AddStringToList(const ACHAR* szText);
	bool CheckCondition();
protected:
	static int m_iIntervel[SEEKTYPE_MAX];
	static int m_iMsgID;
	static int m_iSeekingType;
	static int m_iLastReleaseTime;
	static int m_iMsgType;
	static int m_iApplyRoleID;
	static int m_iMsgRoleID;
	static bool m_bShowInfo;
	static MsgID2PlayerInfoMap m_id2PlayerInfoCache;	// 应征人物个人信息缓存
	static int m_iPlayerInfoType;
	static ApplyCondiction m_acSelf[SEEKTYPE_MAX];					// 个人各种征求条件
	static ACString m_strMsgLeave[SEEKTYPE_MAX];					// 个人各种征求条件
	static MsgID2MsgMap		m_mapMsgCache[SEEKTYPE_MAX];		// 留言缓存
	static int m_iCurPage;		//0-3
	static int m_iCurIndex;
	static FriendSeekTableInfo m_curInfo;
	bool m_bCanSend;
	ACString m_strMsg;
	int m_iMsgIDDst;
	int m_iCurCharmPage;
	int m_iCurMsgPage;		//0-3
	int m_iCurMsgIndex;
	int m_iSortType;
	int m_iLastRefreshTime[SEEKTYPE_MAX];
	int m_iLastRefreshMsgTime;
	int m_idelLeaveID;
	PIE_LOVE_CONFIG*	m_pPlConfig;

	FSTableVector m_vecFSTable[SEEKTYPE_MAX];				// 各种信息缓存
	
	PAUILISTBOX		m_pLst_All;
	PAUILISTBOX		m_pLst_Charm;
	PAUIEDITBOX		m_pEdtMessage;
	PAUIEDITBOX		m_pEdtAnswer;
	PAUITREEVIEW	m_pTvMessage;
	PAUISTILLIMAGEBUTTON	m_Btn_VoteSupport;
	PAUISTILLIMAGEBUTTON	m_Btn_VoteOppose;
	PAUISTILLIMAGEBUTTON	m_Btn_Refresh;
	PAUISTILLIMAGEBUTTON	m_Btn_Head;
	PAUISTILLIMAGEBUTTON	m_Btn_Tail;
	PAUISTILLIMAGEBUTTON	m_Btn_Last;
	PAUISTILLIMAGEBUTTON	m_Btn_Next;
	PAUISTILLIMAGEBUTTON	m_Btn_Accept;
	PAUISTILLIMAGEBUTTON	m_Btn_Refuse;
	PAUISTILLIMAGEBUTTON	m_Btn_Information;
	PAUISTILLIMAGEBUTTON	m_Btn_Send;
	PAUISTILLIMAGEBUTTON	m_Btn_Apply;
	PAUISTILLIMAGEBUTTON	m_Btn_WithDraw;
	PAUISTILLIMAGEBUTTON	m_Btn_Msg;
	PAUISTILLIMAGEBUTTON	m_Btn_Delete;
	PAUILABEL			m_pLabPage;
	PAUITEXTAREA		m_pTxtMsgHead;
	PAUILABEL			m_pLabTitle;

};