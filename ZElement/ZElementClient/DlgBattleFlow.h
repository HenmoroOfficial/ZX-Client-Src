/********************************************************************
	created:	2013/2/27
	file name:	DlgFlower.h 
	author:		zhuyan
	
	purpose:    鲜花宝贝
*********************************************************************/

#pragma once

#include "DlgBase.h"
#include "EC_Friend.h"
#include "AUI\\AUIImagePicture.h"
#include "AUI\\AUILabel.h"
#include "AUI\\AUIListBox.h"
#include "AUI\\AUITreeView.h"
#include "AUI\\AUICheckBox.h"
#include "AUI\\AUIStillImageButton.h"
#include "hashmap.h"
#include "ExpTypes.h"

struct player_info_in_flow_battle
{
	int id;
	ACString name;
	short level;
	int cls;
	unsigned char battle_faction;
	unsigned short kill;
	unsigned short death;
	unsigned int zone_id;
	unsigned int flow_score;
};

class DlgBattleFlowInfoMax: public CDlgBase 
{
	AUI_DECLARE_COMMAND_MAP();	
	AUI_DECLARE_EVENT_MAP();
public:

	DlgBattleFlowInfoMax();
	virtual ~DlgBattleFlowInfoMax();
	virtual bool OnInitDialog();
	virtual void OnShowDialog();
	virtual void OnHideDialog();
	//1 杀死敌人次数 2 自己死亡次数 3 当前分数 4 最高连杀 5 当前连杀 6 最高多杀
	void UpDateFlowInfo(int kill_cnt, int death_cnt,int score,int m_highkill_cnt,int c_kill_cnt,int m_kill_cnt);
	void OnCommandMin(const char * szCommand);
	void OnCommandExit(const char * szCommand);
	void UpdateFlowBattleInfo(void *pData);
	void addFlowName(int nRoleID,ACString szName);
	void UpdateFlowList();
	player_info_in_flow_battle* GetBattleInfo(int nRoleId);	
private:
	AUILabel *m_pScoreCount;
	AUILabel *m_pCurScore;
	AUILabel *m_pKillCount;
	AUILabel *m_pHighKill;
	AUILabel *m_pCurKill;
	AUILabel *m_pManyKill;
	BATTLE_201304_CONFIG* m_pConfig;
	abase::hash_map<int,player_info_in_flow_battle> m_pIdToName;
};


class DlgBattleFlowInfoMin: public CDlgBase 
{
	AUI_DECLARE_COMMAND_MAP();	
	AUI_DECLARE_EVENT_MAP();
public:

	DlgBattleFlowInfoMin();
	virtual ~DlgBattleFlowInfoMin();
	virtual bool OnInitDialog();
	virtual void OnShowDialog();
	virtual void OnHideDialog();
	// 1 当前分数  2 杀死敌人次数 3 自己死亡次数
	void UpDateFlowInfo(int score,int kill_cnt, int death_cnt);
	void OnCommandMax(const char * szCommand);
private:
	AUILabel *m_pScore;
	AUILabel *m_pDie;
	BATTLE_201304_CONFIG* m_pConfig;
};


class DlgBattleFlowInfoWaitMax: public CDlgBase 
{
	AUI_DECLARE_COMMAND_MAP();	
	AUI_DECLARE_EVENT_MAP();
public:

	DlgBattleFlowInfoWaitMax();
	virtual ~DlgBattleFlowInfoWaitMax();
	virtual bool OnInitDialog();

	// 1 位置
	void UpDateFlowWait(int iPos,int nline, int mapId);
	void OnCommandLeave(const char * szCommand);
	void OnCommandWaitMin(const char * szCommand);
	void UpdateBattleFlowWaitTime(DWORD dwTime);
private:
	AUILabel *m_pTime;
	AUILabel *m_pPos;
	AUILabel *m_pWaitTime;
	int m_nApplyLine;
	int m_nMapId;
	int m_nOldPos;
	DWORD m_dwOldWaitTime;
};




class DlgBattleFlowInfoPrompt: public CDlgBase 
{
	AUI_DECLARE_COMMAND_MAP();	
	AUI_DECLARE_EVENT_MAP();
public:

	DlgBattleFlowInfoPrompt();
	virtual ~DlgBattleFlowInfoPrompt();
	virtual bool OnInitDialog();
	//virtual void OnShowDialog();
	virtual void OnTick();
	void OnCommandEnter(const char * szCommand);
	void OnCommandLeave(const char * szCommand);

	void UpDateTime(int iTime,int nline, int mapId,int iscross);
private:
	DWORD m_nTime;
	bool m_bBegin;
	int m_nApplyLine;
	int m_nMapId;
	int m_niscross;
};

class DlgBattleFlowInfoReachExit: public CDlgBase 
{
	AUI_DECLARE_COMMAND_MAP();	
	AUI_DECLARE_EVENT_MAP();
public:

	DlgBattleFlowInfoReachExit();
	virtual ~DlgBattleFlowInfoReachExit();
	virtual bool OnInitDialog();
	//virtual void OnShowDialog();
	void OnCommandWaitLeave(const char * szCommand);
	void OnCommandLeave(const char * szCommand);

private:

};

class DlgBattleFlowInfoNoReachExit: public CDlgBase 
{
	AUI_DECLARE_COMMAND_MAP();	
	AUI_DECLARE_EVENT_MAP();
public:

	DlgBattleFlowInfoNoReachExit();
	virtual ~DlgBattleFlowInfoNoReachExit();
	virtual bool OnInitDialog();
	virtual void OnShowDialog();
	//virtual void OnShowDialog();
	void OnCommandWaitLeave(const char * szCommand);
	void OnCommandLeave(const char * szCommand);
private:

};


class DlgBattleFlowInfoApply: public CDlgBase 
{
	AUI_DECLARE_COMMAND_MAP();	
	AUI_DECLARE_EVENT_MAP();
public:

	DlgBattleFlowInfoApply();
	virtual ~DlgBattleFlowInfoApply();
	virtual bool OnInitDialog();
	//virtual void OnShowDialog();
	void SendFolwInfoApply();
	
private:
	BATTLE_201304_CONFIG * m_pBattleConfig;
};