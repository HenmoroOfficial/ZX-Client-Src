// Filename	: DlgMessageBox.h
// Creator	: Fu Chonggang
// Date		: 2012.11.26

#pragma once

#include "DlgBase.h"

class AUILabel;

class CDlgMessageBoxYesTime : public CDlgBase  
{
	AUI_DECLARE_COMMAND_MAP();

public:
	CDlgMessageBoxYesTime();
	virtual ~CDlgMessageBoxYesTime();

	void OnCommandConfirm(const char * szCommand);
	void OnCommandCancel(const char * szCommand);
	
	virtual int CommonMessage(AString strMsg, WPARAM wParam, LPARAM lParam);

protected:
	virtual bool OnInitDialog();
	virtual void OnShowDialog();
	virtual void OnTick();
	
	DWORD		m_dwTime;
	DWORD		m_dwTotalTime;
	AUILabel	*m_pLabTime;
};
