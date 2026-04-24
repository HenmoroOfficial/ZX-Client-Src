// File		: DlgSoftKB.h
// Creator	: Fu Chonggang
// Date		: 2009/02/03

#pragma once

#include "DlgBase.h"

class CDlgSoftKB : public CDlgBase  
{
	AUI_DECLARE_COMMAND_MAP()
	AUI_DECLARE_EVENT_MAP()

public:
	CDlgSoftKB();
	virtual ~CDlgSoftKB();

	void OnCommandChar(const char * szCommand);
	void OnCommandSwitch(const char * szCommand);

protected:
	virtual void OnShowDialog();
	virtual void OnTick();
	PAUIDIALOG m_pDlgActive;
	PAUIOBJECT m_pObjActive;
};
