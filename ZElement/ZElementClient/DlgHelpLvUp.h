// Filename	:	DlgHelpLvUp.h
// Creator	:	Fu Chonggang
// Date		:	Aug 31, 2009

#pragma once


#include "DlgBase.h"
class AUILabel;
class AUIStillImageButton;
class AUIRadioButton;
class AUITextArea;
class AUIEditBox;
class CLuaState;

class CDlgHelpLvUp : public CDlgBase
{
	AUI_DECLARE_COMMAND_MAP()
	AUI_DECLARE_EVENT_MAP()

public:
	CDlgHelpLvUp();
	virtual ~CDlgHelpLvUp();

	virtual int OnChangeUIMan(BYTE bUserChanged, BYTE bRoleChanged, BYTE bLineChanged);

	void OnEventLButtonDown(WPARAM wParam, LPARAM lParam, AUIObject *pObj);
	void OnCommand_CANCEL(const char * szCommand);
	void OnCommand_Secretary(const char * szCommand);
	void OnCommand_RdoType(const char * szCommand);
	void OnCommand_close(const char * szCommand);

protected:
	virtual bool OnInitDialog();
	virtual	bool Release();
	virtual void OnShowDialog();
	virtual void OnTick();
	virtual bool Render();

	void UpdateInfo();

	AUITextArea *m_pTxt_LvUp;

	int m_iLevel;

	CLuaState * m_pState;
};
