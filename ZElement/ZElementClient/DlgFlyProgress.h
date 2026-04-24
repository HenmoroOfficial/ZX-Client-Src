// Filename	: DlgFlyProgress.h
// Creator	: Fu Chonggang
// Date		: 2012-11-27

#pragma once

#include "DlgBase.h"

class CDlgFlyProgress : public CDlgBase  
{
	AUI_DECLARE_COMMAND_MAP()
public:
	CDlgFlyProgress();
	virtual ~CDlgFlyProgress();
	
	void OnCommand_CANCEL(const char * szCommand);
	virtual int CommonMessage(AString strMsg, WPARAM wParam, LPARAM lParam);

private:
	static const int ms_ValueMax;
	static const int ms_ValuePerHour;
};
