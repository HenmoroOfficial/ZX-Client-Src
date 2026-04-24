// Filename	: DlgCalendar.h
// Creator	: Fu Chonggang
// Date		: 2009/02/10

#pragma once

#include "DlgBase.h"
#include "aui/AUIComboBox.h"

class AUIEditBox;
class CECCalendar;

class CDlgCalendar : public CDlgBase  
{
	AUI_DECLARE_EVENT_MAP();
	AUI_DECLARE_COMMAND_MAP()

public:

	CDlgCalendar();
	virtual ~CDlgCalendar();
	
	void OnCommandDay(const char* szCommand);
	void OnCommandYearUp(const char* szCommand);
	void OnCommandYearDown(const char* szCommand);
	void OnCommandConfirm(const char* szCommand);
	void OnCommandCancel(const char* szCommand);

	void OnEventLButtonDay(WPARAM wParam, LPARAM lParam, AUIObject * pObj);
	void OnEventLButtonDblClkDay(WPARAM wParam, LPARAM lParam, AUIObject * pObj);

protected:
	virtual bool OnInitDialog();
	virtual bool Release();
	virtual void OnShowDialog();
	virtual void OnTick();
	virtual void DoDataExchange(bool bSave);

	PAUICOMBOBOX	m_pYear;
	PAUICOMBOBOX	m_pMonth;
	CECCalendar*	m_pCalendar;
};
