// Filename	:	DlgHelpMemo.h
// Creator	:	Fu Chonggang
// Date		:	Aug 17, 2009

#pragma once


#include "DlgBase.h"
#include "hashmap.h"
class AUILabel;
class AUIRadioButton;
class AUIStillImageButton;
class AUITextArea;
class AUIScroll;
class AUICheckBox;

#define DLGHELPMEMO_NUMPERPAGE	6
#define DLGHELPMEMO_NUMCUSTOMIZE 5
class CDlgHelpMemo : public CDlgBase
{
	AUI_DECLARE_COMMAND_MAP()
	AUI_DECLARE_EVENT_MAP()
	enum
	{
		HELPMEMOTYPE_TODAY = 1,
		HELPMEMOTYPE_SYSTEM,
		HELPMEMOTYPE_CUSTOM,
	};
	struct TaskRemind
	{
		ACString strName;
		ACString strTime;
		tm		tmStartTime;
		tm		tmEncTime;
		int		iRemind;	//0：不提醒 1：一次 2：每次进游戏 3：始终
	};
	typedef abase::vector<TaskRemind> TaskRemindVector;
	typedef abase::hash_map<ACString, TaskRemind> TaskRemindMap;

public:
	CDlgHelpMemo();
	virtual ~CDlgHelpMemo();

	virtual bool Release();

	void OnEventLButtonDown(WPARAM wParam, LPARAM lParam, AUIObject *pObj);

	void OnCommand_CANCEL(const char * szCommand);
	void OnCommand_apply(const char * szCommand);
	void OnCommand_cancel(const char * szCommand);
	void OnCommand_close(const char * szCommand);
	void OnCommand_confirm(const char * szCommand);
	void OnCommand_confirmType(const char * szCommand);
	void OnCommand_ChkRemind(const char * szCommand);
	void OnCommand_RdoWeek(const char * szCommand);
	void OnCommand_RdoType(const char * szCommand);
	
	void OnEventMouseWheel(WPARAM wParam, LPARAM lParam, AUIObject *pObj);
	bool LoadRemind();
	bool SaveRemind();
	void TickMemo();
	void CalRecentMemo();
	bool CloseOneReminding();
	void ClearRemindings();

protected:
	virtual bool OnInitDialog();
	virtual void OnShowDialog();
	virtual void OnTick();
	virtual bool Render();

	void SetInfo(bool bInit = false);

	AUILabel *m_pTitle;
	AUIStillImageButton *m_pBtn_Apply;
	AUIStillImageButton *m_pBtn_Cancel;
	AUIStillImageButton *m_pBtn_Close;
	AUIStillImageButton *m_pBtn_Confirm;
	AUIRadioButton *m_pRdo_Today;
	AUIRadioButton *m_pRdo_System;
	AUIRadioButton *m_pRdo_Custom;
	AUITextArea *m_pTxt_LvUp;
	AUIScroll *m_pScl_Memo;
	AUICheckBox *m_pChk_Index[DLGHELPMEMO_NUMPERPAGE];
	static AUILabel *m_pLab_Time[DLGHELPMEMO_NUMPERPAGE];
	static AUILabel *m_pLab_Task[DLGHELPMEMO_NUMPERPAGE];
	int m_nFirstLine;
	static int m_iType;
	static int m_iWeek;
	static int m_iCurIndex;

	static TaskRemindMap m_mapRemind;
	static TaskRemind m_vecCustome[DLGHELPMEMO_NUMCUSTOMIZE];
	static DWORD m_dwNextMemoTime;						// 10分钟以后
	static TaskRemind m_nextRemind;						// 10分钟以后
	static abase::vector<TaskRemind*> m_vecReminding;	// 10分钟之内
};
