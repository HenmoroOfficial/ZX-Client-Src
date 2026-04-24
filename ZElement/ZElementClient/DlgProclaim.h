// Filename	: DlgProclaim.h
// Creator	: Tom Zhou
// Date		: September 8, 2005

#pragma once

#include "DlgBase.h"
#include "AUI\\AUIEditBox.h"

class CDlgProclaim : public CDlgBase  
{
	friend class CDlgGuildMan;
	friend class CDlgGuildInfo;
	friend class CDlgGuildBuild;
	AUI_DECLARE_COMMAND_MAP()

public:
	CDlgProclaim();
	virtual ~CDlgProclaim();
	bool OnInitDialog();

	void OnCommand_confirm(const char * szCommand);
	void OnCommand_CANCEL(const char * szCommand);

protected:
	PAUIEDITBOX m_pDEFAULT_Txt_Input;
};
