// Filename	: DlgBattleDungeonTwoer.h
// Creator	: Fu Chonggang
// Date		: 2012.4.19
// Desc		: 爬塔副本报名界面

#pragma once

#include "DlgBase.h"

class CDlgBattleDungeonTower : public CDlgBase  
{
	AUI_DECLARE_COMMAND_MAP();

public:
	CDlgBattleDungeonTower();
	virtual ~CDlgBattleDungeonTower();
	
	void OnCommandEnter(const char * szCommand);
	void OnCommandClose(const char * szCommand);
	
	virtual int CommonMessage(AString strMsg, WPARAM wParam, LPARAM lParam);

protected:
	virtual bool OnInitDialog();
	virtual void OnShowDialog();
	virtual void OnTick();

public:
};
