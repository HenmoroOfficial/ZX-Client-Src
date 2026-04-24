// Filename	: DlgMailOption.h
// Creator	: Xiao Zhou
// Date		: 2005/10/31

#pragma once

#include "DlgBase.h"

class CDlgMailOption : public CDlgBase  
{
	AUI_DECLARE_COMMAND_MAP();

public:
	CDlgMailOption();
	virtual ~CDlgMailOption();

	void OnCommandCancel(const char* szCommand);
	void OnCommandReply(const char* szCommand);
	void OnCommandPreserve(const char* szCommand);
	void OnCommandNoPreserve(const char* szCommand);
	void OnCommandDelete(const char* szCommand);
};
