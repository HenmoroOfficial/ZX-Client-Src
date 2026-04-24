// Filename	:	DlgDailyPrompLoginHint.h
// Creator	:	Fu Chonggang
// Date		:	Sep 14, 2010

#pragma once

#include "DlgBase.h"

class AUIListBox;
class AUILabel;
class AUIStillImageButton;
class AUICheckBox;
class A2DSprite;

class CDlgDailyPrompLoginHint : public CDlgBase
{
	AUI_DECLARE_COMMAND_MAP()
	AUI_DECLARE_EVENT_MAP()

public:
	CDlgDailyPrompLoginHint();
	virtual ~CDlgDailyPrompLoginHint();

	void OnEventLButtonDown(WPARAM wParam, LPARAM lParam, AUIObject *pObj);
	void OnEventMouseMove(WPARAM wParam, LPARAM lParam, AUIObject *pObj);

	virtual int CommonMessage(AString strMsg, WPARAM wParam, LPARAM lParam);
	virtual bool Release();

	void OnCommand_CANCEL(const char * szCommand);
	void OnCommand_taskdaily(const char * szCommand);
	void OnCommand_LoginShowState(const char * szCommand);
	void OnCommand_Exit(const char * szCommand);
	void OnCommand_close(const char * szCommand);

	static bool HasAnyDailyPrompToComplete();

protected:
	virtual bool OnInitDialog();
	virtual void OnShowDialog();
	virtual void OnTick();
	virtual bool Render();

	AUIListBox *m_pLst_TaskDailyList;
	AUILabel *m_pLab_List;
	AUIStillImageButton *m_pBtn_TaskDaily;
	AUIStillImageButton *m_pBtn_Close;
	AUILabel *m_pTitle;
	AUICheckBox *m_pChk_NeverHint;
	AUICheckBox *m_pChk_ExitOffExp;
	
	abase::vector<A2DSprite*> m_vecImageList;
    static	int GetSuitTask(const unsigned int* tasks, const int num);
};
