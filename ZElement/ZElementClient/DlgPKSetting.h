// Filename	: DlgPKSetting.h
// Creator	: Xiao Zhou
// Date		: 2005/11/18

#pragma once

#include "DlgBase.h"
#include "AUI\\AUICheckBox.h"

class CDlgPKSetting : public CDlgBase  
{
	AUI_DECLARE_COMMAND_MAP()


public:
	CDlgPKSetting();
	virtual ~CDlgPKSetting();

	void OnCommand_Confirm(const char * szCommand);

protected:
	virtual bool OnInitDialog();
	virtual void OnShowDialog();
	
	PAUICHECKBOX		m_pChk_Gprotect;
	PAUICHECKBOX		m_pChk_Wprotect;
	PAUICHECKBOX		m_pChk_Bprotect;
	PAUICHECKBOX		m_pChk_Rprotect;
	PAUICHECKBOX		m_pChk_Zprotect;
};
