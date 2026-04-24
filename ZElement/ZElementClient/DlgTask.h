// Filename	: DlgTask.h
// Creator	: Tom Zhou
// Date		: October 12, 2005

#pragma once

#include "DlgTaskBase.h"
#include "hashmap.h"
#include "AUI\\AUILabel.h"
#include "AUI\\AUIImagePicture.h"
#include "AUI\\AUITreeView.h"
#include "AUI\\AUITextArea.h"
#include "AUI\\AUIStillImageButton.h"
#include "AUI\\AUICheckBox.h"

class ATaskTempl;

// 模板里面的type+1
enum TaskType
{
	TT_MAIN			= 0,		//诛仙任务	-1	
	TT_TOURISM		= 1,		//游历任务	0
	TT_CHALLENGE	= 2,		//挑战任务	1
	TT_MONSTER		= 3,		//平妖任务	2
	TT_EXAM			= 4,		//试炼任务	3
	TT_EXPLORE		= 5,		//探险任务	4
	TT_CHUANGGONG	= 6,		//传功任务	5
	TT_SUTRA		= 7,		//经典任务	6
	TT_XIUZHEN		= 8,		//修真任务	7
	TT_ERRANTRY		= 9,		//侠义任务	8
	TT_TEACHER		= 10,		//师门任务	9
	TT_GUILD		= 11,		//帮会任务	10
	TT_TREASURE		= 12,		//寻宝任务	11
	TT_ADVENTURE	= 13,		//奇遇任务	12
	TT_LOVE			= 14,		//情缘任务	13
	TT_CROSSSERVER	= 15,		//跨服任务	14
	TT_CROSS_MONSTER	= 16,	//跨服平妖任务	15
	TT_CROSS_CHALLENGE	= 17,	//跨服挑战任务	16
	TT_CROSS_EXAM		= 18,	//跨服试炼任务	17
	TT_NUM,
};
const int TaskColor[] =
{
	0xffffdc8a,		//诛仙任务	-1
	0xff8eb9ff,		//游历任务	0
	0xffff765c,		//挑战任务	1
	0xff56a782,		//平妖任务	2
	0xffe3e3e3,		//试炼任务	3
	0xffe12500,		//探险任务	4
	0xff61f2ff,		//传功任务	5
	0xfffffd44,		//经典任务	6
	0xffd181ff,		//修真任务	7
	0xff12ff00,		//侠义任务	8
	0xff0065fe,		//师门任务	9
	0xff9fb1b7,		//帮会任务	10
	0xffff9c00,		//寻宝任务	11
	0xffa6fcd5,		//奇遇任务	12
	0xfffa9ae0,		//情缘任务	13
	0xffedb92d,		//跨服任务	14
	0xff56a782,		//跨服平妖任务	15
	0xffff765c,		//跨服挑战任务	16
	0xffe3e3e3,		//跨服试炼任务	17
};

const int TaskNewColor[] = 
{
	0xffffffff,	// Default
	0xfffa9ae0, // 日常任务 1
	0xffa6fcd5, // 活动任务 2
	0xffffdc8a, // 诛仙任务 3
	0xff8eb9ff, // 支线任务 4
	0xff32cd32, // 帮派任务 5
};

const int DisableColor = 0xff808080;

typedef struct _SORT_TASK
{
	int nSortType;	// 类别
	__int64 nSortValue;	// 权重 
	unsigned long ulTemplID;	// 任务id
} SORT_TASK, * P_SORT_TASK;

class CDlgTask : public CDlgTaskBase  
{
	AUI_DECLARE_EVENT_MAP()
	AUI_DECLARE_COMMAND_MAP()

	enum TaskState
	{
		TS_FINISH,
		TS_DOING,
		TS_CANDELIVER,
		TS_TODO,
	};



	typedef abase::vector<SORT_TASK*> SortTaskVector;
public:
	CDlgTask();
	virtual ~CDlgTask();
	virtual bool Tick(void);

	void OnCommand_focusall(const char * szCommand);
	void OnCommand_focus(const char * szCommand);
	void OnCommand_abandon(const char * szCommand);
	void OnCommand_TaskDaily(const char * szCommand);
	void OnCommand_ComboType(const char * szCommand);
	void OnCommand_RdoType(const char * szCommand);
	void OnCommand_Return(const char * szCommand);
	void OnCommand_Accept(const char * szCommand);
	void OnCommand_Ancient(const char * szCommand);
	void OnCommand_CANCEL(const char * szCommand);
	void OnCommand_Prev(const char* szCommand);
	void OnCommand_Next(const char* szCommand);
	void OnCommand_Prev2(const char* szCommand);
	void OnCommand_Next2(const char* szCommand);

	void OnCommand_Minimize(const char* szCommand);

	void OnEventLButtonDown_Tv_Quest(WPARAM wParam, LPARAM lParam, AUIObject *pObj);		// 任务列表
	void OnEventLButtonDown_Txt_QuestItem(WPARAM wParam, LPARAM lParam, AUIObject *pObj);	// 任务描述、所需内容
	void OnEventMouseMove_Txt_QuestItem(WPARAM wParam, LPARAM lParam, AUIObject *pObj);		// 任务描述、所需内容

	void OnEventLButtonDown_TvTaskTrace(WPARAM wParam, LPARAM lParam, AUIObject *pObj);		// 快捷追踪
	void OnEventMouseMove_TvTaskTrace(WPARAM wParam, LPARAM lParam, AUIObject *pObj);		// 快捷追踪
	void OnEventLButtonDown_Img_ZoomIn(WPARAM wParam, LPARAM lParam, AUIObject *pObj);		// 快捷追踪缩放
	void OnEventLButtonUp_Img_ZoomIn(WPARAM wParam, LPARAM lParam, AUIObject *pObj);		// 快捷追踪缩放
	void OnEventMouseMove_Img_ZoomIn(WPARAM wParam, LPARAM lParam, AUIObject *pObj);		// 快捷追踪缩放
	
	virtual int CommonMessage(AString strMsg, WPARAM wParam, LPARAM lParam);

	void UpdateTaskMonster(int idTask);	// 更新怪物信息，小地图显示
	void RefreshTaskTrace();			// 任务追踪
	bool UpdateTaskDoing(int idTask = -1);
	bool UpdateTaskCanDeliver(int idTask = -1);
	bool UpdateTaskFinish(int idTask = -1);
	bool UpdateTaskToDo(int idTask = -1);
	
	void UpdateTask();

	void UpdateTaskListCur();
	void UpdateTaskListCanDeliver();
	void UpdateTaskListZhuxian();
	void UpdateFocus();
	
	void InsertTaskChildren(P_AUITREEVIEW_ITEM pRoot,
		unsigned long idTask, bool bExpand, bool bKey);
	static void GetItemLinkItemOn(int x, int y, PAUIOBJECT pObj, AUITEXTAREA_EDITBOX_ITEM * pLink);

	A3DVECTOR3 m_AwardNPC;		// 没用，本来打算提示完成任务npc
	static abase::hash_map<int, bool> m_mapTaskMonster;

protected:
	int m_idLastTask;
	int m_idSelTask;
	int m_iFirstItem;
	static abase::vector<int> m_vecTraceTaskID;
	static int	m_idFocus;

	PAUILABEL m_pText;
	PAUILABEL m_pTxt_QuestNO;
	PAUITREEVIEW m_pTv_Quest;
	PAUITEXTAREA m_pTxt_Content;
//	PAUITEXTAREA m_pTxt_QuestItem;
	PAUITREEVIEW m_pTv_TraceContent;
	PAUISTILLIMAGEBUTTON m_pBtn_Focus;
	PAUISTILLIMAGEBUTTON m_pBtn_Abandon;
	PAUISTILLIMAGEBUTTON m_pBtn_MainQuest;
	PAUISTILLIMAGEBUTTON m_pBtn_NormalQuest;
	AUIStillImageButton *m_pBtn_Accept;

	PAUISTILLIMAGEBUTTON m_pBtn_Prev;
	PAUISTILLIMAGEBUTTON m_pBtn_Next;
	PAUISTILLIMAGEBUTTON m_pBtn_Prev2;
	PAUISTILLIMAGEBUTTON m_pBtn_Next2;
	PAUILABEL m_pTxt_BaseAward;
	PAUILABEL m_pTxt_ItemAward;
	PAUIIMAGEPICTURE m_pImg_Item[CDLGTASK_AWARDITEM_MAX];
	PAUIIMAGEPICTURE m_pImg_Item2[CDLGTASK_AWARDITEM_MAX];	// 随机奖励
	PAUICHECKBOX m_pChk_FocusAll;
	PAUIOBJECT	m_pRdo3;
	PAUISTILLIMAGEBUTTON  m_pAncient;	

	AUITextArea*		m_pTxt_Intro;
	AUIImagePicture*	m_pImg_IntroPic;
	int			m_iCurPage;

	int m_nMouseLastX;
	int m_nMouseLastY;
	SIZE m_nOldSize;

	int m_nAwardItemShowIndex;
	int m_nAwardItemShowIndex2;

	bool ms_bTaskCanDeliverTreeExpand[TT_NUM];

	bool m_bMinimized;

	virtual bool OnInitDialog();
	virtual bool Release();
	virtual void OnShowDialog();
	void FocusChildren(int idTask);	
	ACString FormatTaskHint(const ACHAR* szText);
};
