// File		: DlgLoginRefId.h
// Creator	: Fu Chonggang
// Date		: 2009/04/27

#pragma once

#include "AUI\\AUIDialog.h"

class AUIEditBox;

class CDlgLoginCommon : public AUIDialog  
{
	AUI_DECLARE_COMMAND_MAP()

	void OnCommandConfirm(const char * szCommand);
	void OnCommandCancel(const char * szCommand);

public:
	CDlgLoginCommon();
	virtual ~CDlgLoginCommon();
	
protected:
	virtual void OnShowDialog();
	virtual bool OnInitDialog();
	ACString m_strId;

	AUIEditBox *m_pEdtId;
};
