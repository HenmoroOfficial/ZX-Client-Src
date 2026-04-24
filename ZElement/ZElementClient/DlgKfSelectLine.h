// Filename	:	DlgKfSelectLine.h
// Creator	:	Fu Chonggang
// Date		:	Sep 30, 2010

#pragma once


#include "DlgBase.h"
class AUIListBox;
class AUIStillImageButton;
class AUILabel;

class CDlgKfSelectLine : public CDlgBase
{
	AUI_DECLARE_COMMAND_MAP()
	AUI_DECLARE_EVENT_MAP()

public:
	CDlgKfSelectLine();
	virtual ~CDlgKfSelectLine();

	virtual int CommonMessage(AString strMsg, WPARAM wParam, LPARAM lParam);
	virtual bool Release();

	void OnEventLButtonDown(WPARAM wParam, LPARAM lParam, AUIObject *pObj);

	void OnCommand_CANCEL(const char * szCommand);
	void OnCommand_sortname(const char * szCommand);
	void OnCommand_sortstats(const char * szCommand);
	void OnCommand_selectline(const char * szCommand);
	void OnCommand_return(const char * szCommand);

protected:
	virtual bool OnInitDialog();
	virtual void OnShowDialog();
	virtual void OnTick();
	virtual bool Render();

	AUIListBox *m_pLst_Line;
	AUIStillImageButton *m_pBtn_SortName1;
	AUIStillImageButton *m_pBtn_SortStats1;
	AUIStillImageButton *m_pBtn_Confirm;
	AUIStillImageButton *m_pBtn_Cancel;
	AUILabel *m_pTitle;
};
