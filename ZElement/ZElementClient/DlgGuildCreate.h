// Filename	: DlgGuildCreate.h
// Creator	: Tom Zhou
// Date		: October 25, 2005

#pragma once

#include "DlgBase.h"
#include "AUI\\AUIEditBox.h"

class CDlgGuildCreate : public CDlgBase  
{
	AUI_DECLARE_COMMAND_MAP()

public:
	CDlgGuildCreate();
	virtual ~CDlgGuildCreate();

	void OnCommand_confirm(const char * szCommand);
	void OnCommand_CANCEL(const char * szCommand);

protected:
	PAUIEDITBOX m_pDEFAULT_Txt_Input;

	virtual bool OnInitDialog();
};
