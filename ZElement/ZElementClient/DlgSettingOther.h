// Filename	:	DlgSettingOther.h
// Creator	:	Fu Chonggang
// Date		:	Sep 11, 2009

#pragma once

#include "Dlgbase.h"

class AUISubDialog;
class CDlgSetting;

class CDlgSettingOther : public CDlgBase  
{
	AUI_DECLARE_COMMAND_MAP()
	AUI_DECLARE_EVENT_MAP()
public:
	CDlgSettingOther();
	virtual ~CDlgSettingOther();
	void OnCommandApply(const char * szCommand);
	void OnCommandConfirm(const char * szCommand);
	void OnCommandCancel(const char * szCommand);
	void OnCommandBack(const char * szCommand);
	void OnCommandDefault(const char * szCommand);

protected:
	virtual bool OnInitDialog();
	virtual void OnShowDialog();
	
private:
	CDlgSetting *m_pDlgCurSetting;
};

