// Filename	: DlgExit.h
// Creator	: Tom Zhou
// Date		: October 27, 2005

#pragma once

#include "DlgBase.h"
#include "AUI\\AUICheckBox.h"

class CDlgExit : public CDlgBase  
{
	AUI_DECLARE_COMMAND_MAP()

public:
	CDlgExit();
	virtual ~CDlgExit();

	void OnCommand_confirm(const char * szCommand);
	void ShowAgentInfo( int bonus_time, int left_time, double bonus_exp );

protected:
	virtual bool OnInitDialog();
	virtual void OnShowDialog();

	PAUICHECKBOX m_pChk_Forcequit;
	PAUICHECKBOX m_pChk_ExitOffExp;
	PAUICHECKBOX m_pChk_GameQuitOffExp;
};
