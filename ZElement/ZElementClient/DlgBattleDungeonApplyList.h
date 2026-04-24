/**
* @File DlgBattleDungeonApplyList.h
* @Brief 副本报名完毕等待进入界面
* @Author Fu Chonggang
* @Date 2011-7-25 10:39:30
* @Copyright (c) 2011 ZhuXian Studio, All Rights Reserved. 
*/
#pragma once

#include "DlgBase.h"

class AUILabel;
class AUIListBox;

class CDlgBattleDungeonApplyList : public CDlgBase  
{
	AUI_DECLARE_COMMAND_MAP()
	AUI_DECLARE_EVENT_MAP()

	// 服务器给的
	enum RaidKickReason
	{       
		RAID_ROOMCLOSE =0, //副本房间关闭
		RAID_INSTANCE_START_ERR = 1,//开启副本失败
		RAID_INSTANCE_ENTER_ERR = 2,//进入副本失败
		RAID_INSTANCE_CLOSE = 3,//副本关闭
		RAID_KICKOUT = 4,//被房主踢出
	};

public:
	CDlgBattleDungeonApplyList();
	virtual ~CDlgBattleDungeonApplyList();
	
	void OnCommandKick(const char * szCommand);
	void OnCommandChange(const char * szCommand);
	void OnCommandOpen(const char * szCommand);
	void OnCommandAccept(const char * szCommand);
	void OnCommandRefuse(const char * szCommand);
	void OnCommandLeave(const char * szCommand);
	void OnCommandCancel(const char * szCommand);
	
	void OnEventRButtonUpLstIn(WPARAM wParam, LPARAM lParam, AUIObject * pObj);
	void OnEventRButtonUpLstOut(WPARAM wParam, LPARAM lParam, AUIObject * pObj);
	
	virtual int CommonMessage(AString strMsg, WPARAM wParam, LPARAM lParam);
	
	typedef void (CDlgBattleDungeonApplyList::*PFUNC)(void*);
	typedef abase::hash_map<AString, PFUNC>	FuncMap;

protected:
	virtual void OnTick();
	virtual bool OnInitDialog();
	virtual void OnShowDialog();

	void OnJoinReq(void* pData);		// deliver发给副本房主关于申请人申请加入副本房间的请求
	void OnOpenRe(void* pData);			// 开启副本房间
	void OnJoinRe(void* pData);			// 报名加入副本房间
	void OnJoinApply(void* pData);		// 条件不满足，申请加入副本房间失败时
	void OnAcceptRe(void* pData);		// 房主接受申请者的请求后，delivery返回的协议	无处理
	void OnEnterRe(void* pData);		// 进入副本
	void OnStartRe(void* pData);		// 通知房主开启副本
	void OnAppointRe(void* pData);		// 通知之前的老团长：副本团长已转移		不成功提示
	void OnAppointNotify(void* pData);	// 通知其他人：副本团长已转移
	void OnStartNotify(void* pData);	// 通知所有成员：副本开启
	void OnKickRe(void* pData);			// 通知团长踢人情况
	void OnKickNotify(void* pData);		// 通知被踢
	void OnQuitRe(void* pData);			// 玩家不同意进入副本
	void OnGetRoomRe(void* pData);		// 返回关于某副本房间的相关信息（申请人列表，成员列表）

private:
	AUIListBox*		m_pLstIn;
	AUIListBox*		m_pLstOut;
	AUILabel*		m_pLabTimeLeftStart;
	AUILabel*		m_pLabOwnerName;
	AUILabel*		m_pLabOwnerProf;
	AUILabel*		m_pLabOwnerLv;
	AUILabel*		m_pLabPlayerNum;

	AUIObject*		m_pBtnKick;
	AUIObject*		m_pBtnChange;
	AUIObject*		m_pBtnAccept;
	AUIObject*		m_pBtnRefuse;

	FuncMap		m_mapFuncs;

	int			m_iApplyEndTime;
	
	void OnBattleQuitClear();
};

