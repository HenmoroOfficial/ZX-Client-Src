// Filename	: DlgFaceLift.h
// Creator	: Tom Zhou
// Date		: October 30, 2005

#pragma once

#include "DlgBase.h"
#include "AUI\\AUIImagePicture.h"

class CDlgFaceLift : public CDlgBase  
{
	friend class CECGameUIMan;

	AUI_DECLARE_COMMAND_MAP()

public:
	CDlgFaceLift();
	virtual ~CDlgFaceLift();

	void OnCommand_start(const char * szCommand);
	void OnCommand_CANCEL(const char * szCommand);

protected:
	PAUIIMAGEPICTURE m_pImgItem;

	virtual bool OnInitDialog();
};
