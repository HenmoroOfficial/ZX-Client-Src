#pragma once

#include "DlgBase.h"

class AUIStillImageButton;
class AUITextArea;
class AUICheckBox;
class CECScriptContext;
class CDlgSettingScriptHelp;
class CDlgScriptHelpHistory;

class CDlgScriptHelp : public CDlgBase
{
	AUI_DECLARE_COMMAND_MAP();
	AUI_DECLARE_EVENT_MAP();
protected:
	virtual void OnShowDialog();
	virtual bool OnInitDialog();
	void AdjustHelpHistoryWindowPosition();
public:
	CDlgScriptHelp();
	virtual ~CDlgScriptHelp();
	virtual void DoDataExchange(bool bSave);


	void OnCommandIDCANCEL(const char * szCommand);
	void OnCommandNext(const char * szCommand);
	void OnCommandClose(const char * szCommand);
	void OnCommandDisableHelp(const char * szCommand);
	void OnCommandSetting(const char * szCommand);
	void OnCommandHistory(const char * szCommand);
	void OnMouseMove(WPARAM wParam, LPARAM lParam, AUIObject * pObj);


	void SetScriptContext(CECScriptContext * pContext);
	void SetTipText(const ACHAR * szText);
	void SetHasNextTip(bool bNext);
	bool GetIsDisableHelp();
	void SetIsDisableHelp(bool bDisable);

private:
	CECScriptContext *		m_pContext;
	AUITextArea *			m_pTAreaTip;
	AUIStillImageButton *	m_pBtnNext;
	AUIStillImageButton *	m_pBtnClose;
	AUICheckBox *			m_pChkDisableHelp;
private:
	CDlgSettingScriptHelp *	GetSettingDlg();
	CDlgScriptHelpHistory * GetHistoryDlg();
};