// Filename	: DlgTeamContext.h
// Creator	: Tom Zhou
// Date		: September 1, 2005

#pragma once

#include "DlgBase.h"

class CDlgHostAction : public CDlgBase  
{
	AUI_DECLARE_COMMAND_MAP()

public:
	CDlgHostAction() {}
	virtual ~CDlgHostAction() {}

	void OnCommand_CopyName(const char * szCommand);
};
