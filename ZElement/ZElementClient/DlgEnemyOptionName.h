// File		: DlgEnemyOptionName.h
// Creator	: Fu Chonggang
// Date		: 2009/6/17

#pragma once

#include "DlgBase.h"

class CDlgEnemyOptionName : public CDlgBase  
{
	AUI_DECLARE_COMMAND_MAP()

public:
	CDlgEnemyOptionName();
	virtual ~CDlgEnemyOptionName();
	virtual void OnTick();

	void OnCommandCANCAL(const char *szCommand);
	void OnCommandWhisper(const char *szCommand);
	void OnCommandUpdate(const char *szCommand);
	void OnCommandDeleteEnemy(const char *szCommand);
	void OnCommandCopyName(const char *szCommand);

};
