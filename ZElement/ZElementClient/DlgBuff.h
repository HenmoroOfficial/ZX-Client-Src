// Filename	: DlgBuff.h
// Creator	: Xiao Zhou
// Date		: 2006/6/1

#pragma once

#include "DlgBase.h"

class CDlgBuff : public CDlgBase  
{
	AUI_DECLARE_COMMAND_MAP()
	AUI_DECLARE_EVENT_MAP()

public:
	CDlgBuff();
	virtual ~CDlgBuff();

	void OnCommand_CANCEL(const char * szCommand);
	void OnEvent(WPARAM wParam, LPARAM lParam, AUIObject *pObj);
	static ACString GetBuffTime(int iEndTime);
	
protected:
	virtual void OnTick();
	virtual bool OnInitDialog();
};
