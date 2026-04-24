/********************************************************************
	created:	2005/08/11
	created:	11:8:2005   15:31
	file base:	DlgAskHelpToGM
	file ext:	h
	author:		yaojun
	
	purpose:	
*********************************************************************/


#pragma once

#include "DlgBase.h"

class AUIEditBox;
class AUIComboBox;

class CDlgAskHelpToGM : public CDlgBase  
{
	AUI_DECLARE_COMMAND_MAP()
protected:
	AUIComboBox * m_pCbxType;
	AUIEditBox * m_pEbxMemo;
	
	DWORD m_dwSelectedType;
	ACString m_strMemo;
protected:
	void DoDataExchange(bool bSave);
	bool OnInitDialog();

	virtual void OnShowDialog();
public:
	void OnCommandConfirm(const char * szCommand);
	void ShowWithHelpMessage(const ACHAR * szMsg, bool bReportPlugin = false);

	CDlgAskHelpToGM();
	virtual ~CDlgAskHelpToGM();
};

