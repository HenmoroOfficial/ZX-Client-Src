// Filename	: DlgTaskDaily.h
// Creator	: Xiao Zhou
// Date		: 2008/8/26

#pragma once

#include "DlgBase.h"
#include "AUI\\AUILabel.h"
#include "AUI\\AUIListBox.h"
#include "AUI\\AUITextArea.h"
#include "AUI\\AUIRadioButton.h"

#define TASKDAILY_TASKID_MAX 200
#define TASKDAILY_MAX_LINE

struct TaskDailyInfo				// 对应taskdaily.txt
{
	bool			bDays[7];
	
	ACString		strTime;
	tm				tmStartTime;
	tm				tmEncTime;
	BOOL			bNoTimeLimit;
	
	ACString		strName;
	
	unsigned long	ulMinLevel;
	unsigned long	ulMaxLevel;
	ACString		strLevelInfo;
	
	BOOL			bLoginShow;
	BOOL			bSpecial;
	
	ACString		strNPC;
	ACString		strType;
	
	BOOL			bNoCountLimit;
	int				iNumMax;
	
	ACString		strLineInfo;
	bool			bLines[30];
	UINT			uiNumID;
	UINT			uiCommend;
	UINT			uiTaskID[TASKDAILY_TASKID_MAX];
	BOOL			bSuit[TASKDAILY_TASKID_MAX];
	UINT			iCurTaskShow;
	ACString		strDescription;
	int				taskType;
};

class CDlgTaskDaily : public CDlgBase  
{
	AUI_DECLARE_EVENT_MAP();
	AUI_DECLARE_COMMAND_MAP()

public:
	enum daily_task_type
	{
		TASKDAILY_TYPE_MIN,
		TASKDAILY_TYPE_SPECIAL,			// 可完成的特殊任务		金色
		TASKDAILY_TYPE_CANDO,			// 普通加星				白
		TASKDAILY_TYPE_TIME_LIMIT_WAIT,	// 普通					白
		TASKDAILY_TYPE_TIME_LIMIT_MISS, // 超过可完成时间		蓝
		TASKDAILY_TYPE_COND_LIMIT,		// 条件限制				红
		TASKDAILY_TYPE_MAX,
	};
	CDlgTaskDaily();
	virtual ~CDlgTaskDaily();

	void OnCommand_Rdo(const char * szCommand);
	void OnCommand_Btn_Time(const char * szCommand);
	void OnCommand_Btn_TaskName(const char * szCommand);
	void OnCommand_Btn_Level(const char * szCommand);
	void OnCommand_Btn_Type(const char * szCommand);
	void OnCommand_Btn_Number(const char * szCommand);
	void OnCommand_Btn_Suggest(const char * szCommand);
	void OnCommand_Chk_tj1(const char * szCommand);
	void OnCommand_Chk_tj2(const char * szCommand);

	void OnEventLButtonDown_Lst_Task(WPARAM wParam, LPARAM lParam, AUIObject *pObj);
	void OnEventLButtonDown_Txt_NPC(WPARAM wParam, LPARAM lParam, AUIObject *pObj);
	void OnEventMouseMove_Txt_NPC (WPARAM wParam, LPARAM lParam, AUIObject *pObj);
	
	static abase::vector<TaskDailyInfo>* GetTaskDailyInfo(int iWeek) { return &(m_vecTaskDaily[iWeek]); }
	static abase::vector<TaskDailyInfo>* GetAllTaskdaily() { return &m_AllTaskDaily;}

protected:
	virtual void OnTick(void);
	virtual bool OnInitDialog();
	virtual void OnShowDialog();
	
	static abase::vector<TaskDailyInfo>	m_vecTaskDaily[7];
	static abase::vector<TaskDailyInfo> m_AllTaskDaily;
	static	TaskDailyInfo				m_CurTaskDaily;
	static	DWORD						m_dwCurDay;			//当前星期，1-7

	ACString		m_strTaskPopular;

	PAUILISTBOX		m_pLst_Task;
	PAUITEXTAREA	m_pTxt_TaskName;
	PAUITEXTAREA	m_pTxt_NPC;
	PAUITEXTAREA	m_pTxt_Line;
	PAUITEXTAREA	m_pTxt_Desp;
	PAUITEXTAREA	m_pTxt_Explain;
	PAUITEXTAREA	m_pTxt_HotTask;

	BOOL	m_bSortTime;
	BOOL	m_bSortTaskName;
	BOOL	m_bSortLevel;
	BOOL	m_bSortType;
	BOOL	m_bSortNumber;
	BOOL	m_bSortSuggest;
	BOOL	m_bShowRed;
	BOOL	m_bShowBlue;
	BOOL	m_bShowGreen;
	int		m_iCurSel;

	void GetItemLinkItemOn(int x, int y, PAUIOBJECT pObj, AUITEXTAREA_EDITBOX_ITEM * pLink);
};
