// Filename	:	DlgDailyPromp.h
// Creator	:	Fu Chonggang
// Date		:	Sep 2, 2010

#pragma once


#include "DlgBase.h"
#include "hashtab.h"
#include "vector.h"
class AUIEditBox;
class AUIStillImageButton;
class AUILabel;
class AUIListBox;
class AUIRadioButton;
class AUISubDialog;
class AUITextArea;
class AUIImagePicture;
class CECModel;
struct MenologyInfo;

class CDlgDailyPromp : public CDlgBase
{
	AUI_DECLARE_COMMAND_MAP()
	AUI_DECLARE_EVENT_MAP()

	enum
	{
		TYPE_RECOMMAND,
		TYPE_WEEKLY,
		TYPE_BATTLEFIELD,
		TYPE_MONSTER,
		TYPE_NUM,
	};

	enum
	{
		ICONTYPE_TASK24,
		ICONTYPE_TASK32,
		ICONTYPE_TASK48,
	};

	enum
	{
		AWARD_MAX = 6,
	};

public:
	CDlgDailyPromp();
	virtual ~CDlgDailyPromp();

	virtual int CommonMessage(AString strMsg, WPARAM wParam, LPARAM lParam);
	virtual bool Release();

	void OnEventLButtonDownList(WPARAM wParam, LPARAM lParam, AUIObject *pObj);
	void OnEventLButtonDownListMonster(WPARAM wParam, LPARAM lParam, AUIObject *pObj);
	void OnEventLButtonDownNpc(WPARAM wParam, LPARAM lParam, AUIObject *pObj);
	void OnEventMouseMoveNpc(WPARAM wParam, LPARAM lParam, AUIObject *pObj);

	void OnCommand_CANCEL(const char * szCommand);
	void OnCommand_RdoTypeToday(const char * szCommand);
	void OnCommand_RdoTypeWeek(const char * szCommand);
	void OnCommand_RdoTypeBattle(const char * szCommand);
	void OnCommand_RdoTypeBoss(const char * szCommand);
	void OnCommand_Boss15Only(const char * szCommand);
	void OnCommand_LoginShowState(const char * szCommand);
	
	void OnCommand_DailySortTime(const char * szCommand);
	void OnCommand_DailySortLevel(const char * szCommand);
	void OnCommand_DailySortStatus(const char * szCommand);
	void OnCommand_DailySortLevelBoss(const char * szCommand);
	void OnCommand_DailySortLvRecommandBoss(const char * szCommand);
	void OnCommand_DailySortLineBoss(const char * szCommand);
	
	void OnCommand_DailySortReward(const char * szCommand);
	void OnCommand_DailySortName(const char * szCommand);
	void OnCommand_DailySortCount(const char * szCommand);
	void OnCommand_DailySortCond(const char * szCommand);
	void OnCommand_DailySortPeriod(const char * szCommand);
	void OnCommand_DailySortBossRefresh(const char * szCommand);

	static bool DailyShowToday(const MenologyInfo* info);


protected:
	virtual bool OnInitDialog();
	virtual void OnShowDialog();
	virtual void OnTick();
	virtual bool Render();
	void UpdateDaily();
	void UpdateWeekly();
	void UpdateBattleInfo();
	void UpdateMonster();
	bool CheckLevel(const MenologyInfo* info);

	AUILabel *m_pTxt_Time;					// time
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
	CECModel*	m_pModel;					// boss model
	bool		m_bLoaded;					// is boss model loaded
	int			m_nCurSel;
	ACString	m_strHintNpc;

	static bool						ms_bLoaded;
	static DWORD					ms_dwActionTime;
	static int						m_nType;
	static A2DSprite*				m_pFlyIcon;
	static abase::vector<A2DSprite*> m_vecImageList;
	bool GetExpAward(const int reward, ACString& strAward, ACString& strHint);
	bool GetMoneyAward(const int reward, ACString& strAward, ACString& strHint);
	bool GetItemAward(const int reward, ACString& strAward, ACString& strHint);
public:
	static int GetSuitTask(const unsigned int* tasks, const int num);
};
