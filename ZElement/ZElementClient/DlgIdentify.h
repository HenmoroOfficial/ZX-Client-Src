// Filename	: DlgIdentify.h
// Creator	: Tom Zhou
// Date		: October 12, 2005

#pragma once

#include "DlgBase.h"
#include "AUI\\AUIImagePicture.h"
#include "AUI\\AUIStillImageButton.h"

class CDlgIdentify : public CDlgBase  
{
	AUI_DECLARE_EVENT_MAP();
	AUI_DECLARE_COMMAND_MAP()

public:
	CDlgIdentify();
	virtual ~CDlgIdentify();

	void OnCommand_start(const char * szCommand);
	void OnCommand_CANCEL(const char * szCommand);

	void OnEventLButtonDown_Item(WPARAM wParam, LPARAM lParam, AUIObject *pObj);

protected:
	PAUIIMAGEPICTURE m_pImgItem;
	PAUISTILLIMAGEBUTTON m_pBtn_Confirm;

	virtual bool OnInitDialog();
};
