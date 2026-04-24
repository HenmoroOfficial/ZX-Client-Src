/********************************************************************
	created:	2012/11/5
	file name:	DlgDynamicSystem.h 
	author:		zhuyan
	
	purpose:		
*********************************************************************/

#pragma once

#include "DlgBase.h"
#include "AUI\\AUIImagePicture.h"
#include "AUI\\AUILabel.h"
#include "AUI\\AUIListBox.h"
#include "aui\\AUIProgress.h"
#include "hashmap.h"
#include "EC_Menology.h"
class CECIvtrItem;

class CDlgDynamicMin : public CDlgBase 
{
	AUI_DECLARE_COMMAND_MAP();	
public:
	CDlgDynamicMin() {}
	virtual ~CDlgDynamicMin() {}
	virtual bool OnInitDialog();
	void OnCommandOpen(const char * szCommand);
	virtual void OnTick();
};


class DlgDynamicContent : public CDlgBase 
{
	AUI_DECLARE_COMMAND_MAP();	
	AUI_DECLARE_EVENT_MAP();
public:
	enum
	{
		AWARD_MAX = 6,
	};
	DlgDynamicContent() {}
	virtual ~DlgDynamicContent() {}
	virtual bool OnInitDialog();
	void UpDataUIInterface(int nType);
 	void OnEventLButtonList(WPARAM wParam, LPARAM lParam, AUIObject * pObj);
	void OnEventLButtonDownNpc(WPARAM wParam, LPARAM lParam, AUIObject *pObj);
	void OnEventMouseMoveNpc(WPARAM wParam, LPARAM lParam, AUIObject *pObj);
	bool CheckLevel(const MenologyInfo* info);
	int GetSuitTask(const unsigned int* tasks, const int num);
private:
	AUIListBox *m_pLst_Task;				// list
	AUILabel *m_pTxt_TaskLine;				// line
	AUILabel *m_pTxt_TaskLv;				// level
	AUILabel *m_pTxt_TaskTime;				// 活动时间
	AUITextArea *m_pTxt_TaskNpc;			// npc名字
	AUITextArea *m_pTxt_TaskNpcMap;			// npc所在地图
	AUILabel *m_pTxt_BossName;				// boss name
	AUILabel *m_pTxt_BossLv;				// boss level
	AUITextArea *m_pTxt_Desc;				// desc
	AUIImagePicture *m_pImg_Award[AWARD_MAX];
	AUIImagePicture *m_pImg_Task;			// Win_DailyPrompBoss： boss形象 其它：任务/战场图片
	bool		m_bLoaded;					// is boss model loaded
	int			m_nCurSel;
	ACString	m_strHintNpc;

	abase::vector<A2DSprite*> m_vecImageList;			// 动态生成需要在任务描述中显示的图片
};


class DlgDynamicSystem : public CDlgBase  
{
	AUI_DECLARE_EVENT_MAP();
	AUI_DECLARE_COMMAND_MAP();
public:
	struct DYNAMIC_CHILD_INFO
	{
		int nIndex;				// 编号
		int nTypeid;			//  类型
		ACString name;			// 活动名称
		int	nRequire_level;			// 修真等级限制
		int	nRenascence_count;		// 转生次数要求
	};
	struct DYNAMIC_INFO
	{	
		int id;			//  类型
		int nGain;		// 奖励积分
		ACString szTypeName;	// 活动名称
	};

	DlgDynamicSystem();
	virtual ~DlgDynamicSystem();
	virtual bool OnInitDialog();
	virtual void OnShowDialog();
	virtual void OnHideDialog();
/*	void OnEventLButtonDownItem(WPARAM wParam, LPARAM lParam, AUIObject * pObj);*/
	void OnCommandType(const char* szCommand);
	void OnEventLButtonList(WPARAM wParam, LPARAM lParam, AUIObject * pObj);
	void UpdataDysnamic();
	void UpdataDysnamicItem();
	void GetGroupItem(int nID,const char *pszItemName);
private:
	PAUILISTBOX			m_pTvTaskList;
	AUIImagePicture *   m_pImgSelected[4];
	abase::hash_map<int, DYNAMIC_INFO> m_mapDyanmicList;
	abase::hash_map<int, DYNAMIC_CHILD_INFO> m_mapDyanmicChild;
	PAUIPROGRESS		m_pPrg_Point;
};