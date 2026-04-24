// Filename	: DlgRecord.h
// Creator	: Xiao Zhou
// Date		: 2006/10/23

#pragma once

#include "DlgBase.h"
#include "AUI\\AUILabel.h"

class CDlgRecord : public CDlgBase  
{
	AUI_DECLARE_COMMAND_MAP();

public:
	CDlgRecord();
	virtual ~CDlgRecord();

	void OnCommandStart(const char * szCommand);
	void OnCommandEnd(const char * szCommand);
	void OnCommandCancel(const char * szCommand);

	void EndRecord();

protected:
	virtual void OnTick();
	virtual void OnShowDialog();
};
