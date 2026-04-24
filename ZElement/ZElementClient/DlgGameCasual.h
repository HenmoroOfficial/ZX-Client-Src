// Filename	: DlgGameCasual.h
// Creator	: Fu Chonggang
// Date		: 2009/03/04

#pragma once

#include "DlgBase.h"

class CDlgGameCasual : public CDlgBase  
{
	AUI_DECLARE_COMMAND_MAP();

public:
	CDlgGameCasual();
	virtual ~CDlgGameCasual();
	void OnCommand_TdGame(const char * szCommand);
	void OnCommand_LlkGame(const char * szCommand);
	void OnCommand_DdzGame(const char * szCommand);
	void OnCommand_TouchGame(const char* szCommand);

protected:
	virtual void OnTick();
};
