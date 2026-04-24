/********************************************************************
	created:	2012/8/17
	author:		fuchonggang
	
	purpose:	图形帮助系统
	Copyright (C) 2012 - All Rights Reserved
*********************************************************************/

#pragma  once

#include "wtypes.h"
#include "vector.h"

#define HELP_GRAPHGUIDE_NUM		50

class CECHelpStepBase;

class CECHelpGraphGuide
{
public:		//	Types
	enum
	{
		HELP2012_CLOSE,					// 是否关闭帮助系统
		HELP2012_0_1_OPEN_ORNOT,		// 开始
		HELP2012_1_1_WASD_MOVE,			// 移动
		HELP2012_2_1_PACKAGE,			// 装备
		HELP2012_2_2_TREASURE_CHEST,
		HELP2012_2_3_WEAPON,
		HELP2012_3_1_TRACK,				// 任务
		HELP2012_3_2_FIRSTTASK,
		HELP2012_3_3_TASK_ACCEPT,
		HELP2012_3_4_TASK_UI,
		HELP2012_3_5_TASK_LIST,
		HELP2012_5_1_ATTACK,			// 攻击
		HELP2012_6_1_MEDICINE_INV,		// 药物
		HELP2012_6_2_MEDICINE_HINT,
		HELP2012_7_1_TALISMAN_INV,		// 法宝
		HELP2012_7_2_TALISMAN_HINT,
		HELP2012_7_3_TALISMAN_HPMP,
		HELP2012_7_5_TALISMAN_START,
		HELP2012_8_1_MAP_HOTKEY,		// 地图
		HELP2012_8_2_MAP_NPCMINE_LIST,
		HELP2012_9_1_SKILL_ICON,		// 技能
		HELP2012_9_2_SKILL_ADD,
		HELP2012_9_3_SKILL_SHORTCUT,
		HELP2012_10_1_TRANSFER,			// 传送
		HELP2012_10_2_TRANSFER_DESTPOS,
		HELP2012_11_1_TASK_UI,			// 诛仙任务
		HELP2012_11_2_TASK_ZHUXIAN,
		HELP2012_11_3_TASK_QINGYUN,
		HELP2012_11_4_TASK_QINGYUN_FIRST_TASK,
		HELP2012_11_5_TASK_DELIVER,

		HELP2012_NUM,
	};

	enum
	{
		HELP_STAGE_START,
		HELP_STAGE_MOVE,
		HELP_STAGE_EQUIP,
		HELP_STAGE_TASK,
		HELP_STAGE_ATTACK,
		HELP_STAGE_MEDICINE,
		HELP_STAGE_TALISMAN,
		HELP_STAGE_MAP,
		HELP_STAGE_SKILL,
		HELP_STAGE_TRANSFER,
		HELP_STAGE_TASK_ZHUXIAN,
		HELP_STAGE_NUM,
	};

public:		//	Constructor and Destructor

	CECHelpGraphGuide();
	virtual ~CECHelpGraphGuide();

public:		//	Attributes

public:		//	Operations
	void Tick(unsigned long dwDeltaTime);

	void SetState(int index);
	void ClearState(int index);
	bool GetState(int index);

	void StartHelp(int index);
	void FinishHelp(int index, bool bSave=true);
	void FinishHelpAction(int index);			// 一类帮助结束，播放一个gfx

	unsigned char* GetSaveData();
	void LoadData(void* pDataBuf, int iSize);
	int  GetSize() { return HELP_GRAPHGUIDE_NUM; }

	// 游戏里操作激活哪些对应帮助
	void OpenInv();
	void UseItem(int id);

	// for ESC action
	// click yes, skip current stage
	void SkipCurStage();
	// click cancel, resume help
	void ResumeHelp();

	/** 
	* @brief DealWndProc 
	* 
	* Detailed description.
	* @param[in] message 
	* @param[in] wParam 
	* @param[in] lParam 
	* @return bool  true means action in locked area
	*/
	bool DealWndProc(UINT message, WPARAM wParam, LPARAM lParam);
	
protected:	//	Attributes

	// 默认为0
	// HELP2012_CLOSE 0表示帮助开启
	// 其它 0表示未经过这一步，1表示该步已经完成
	unsigned char m_aHelpState[HELP_GRAPHGUIDE_NUM];
	CECHelpStepBase		*m_pHelpSteps[HELP_STAGE_NUM];

	int		m_iCurHelpStep;			// 当前哪一步帮助
	int		m_iCurHelpStage;		// 当前哪一大类
	
	int		m_iTimer;				// 定时判断帮助触发条件
	bool	m_bLockAction;			// 是否锁定只可操作部分界面
	static const int	ms_iCheckHelpPeriod;

protected:	//	Operations

	int GetIndex(int index);		// 根据m_iCurHelpStep计算m_iCurHelpStage
	void Reset();
	void CreateHelpSteps();
	bool IsHelpOpen();

	void _GenerateCurTasks(abase::vector<int> &vecIds);		// 包括子任务
	void _InsertSubTask(int id, abase::vector<int> &vecIds);
};