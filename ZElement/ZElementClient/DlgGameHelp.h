// Filename	: DlgGameHelp.h
// Creator	: Fu Chonggang
// Date		: 2009/08/18

#pragma once

#include "DlgBase.h"

class CDlgGameHelp : public CDlgBase  
{
	AUI_DECLARE_COMMAND_MAP();

public:
	CDlgGameHelp();
	virtual ~CDlgGameHelp();
	void OnCommand_Close(const char * szCommand);

protected:
	virtual bool OnInitDialog();
	virtual void OnTick();
};
