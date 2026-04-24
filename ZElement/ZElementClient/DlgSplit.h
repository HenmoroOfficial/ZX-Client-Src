// Filename	: DlgSplit.h
// Creator	: Tom Zhou
// Date		: October 12, 2005

#pragma once

#include "DlgBase.h"
#include "AUI\\AUILabel.h"
#include "AUI\\AUIProgress.h"
#include "AUI\\AUIImagePicture.h"
#include "AUI\\AUIStillImageButton.h"

class CDlgSplit : public CDlgBase  
{
	friend class CECGameUIMan;

	AUI_DECLARE_EVENT_MAP();
	AUI_DECLARE_COMMAND_MAP()

public:
	CDlgSplit();
	virtual ~CDlgSplit();

	void OnCommand_begin(const char * szCommand);
	void OnCommand_CANCEL(const char * szCommand);

	void OnEventLButtonDown_Item_a(WPARAM wParam, LPARAM lParam, AUIObject *pObj);

protected:
	PAUILABEL m_pTxt_no1;
	PAUILABEL m_pTxt_no2;
	PAUILABEL m_pTxt_Gold;
	PAUIPROGRESS m_pPrgs_1;
	PAUIIMAGEPICTURE m_pImgItem_a;
	PAUIIMAGEPICTURE m_pImgItem_b;
	PAUISTILLIMAGEBUTTON m_pBtn_Start;
	PAUISTILLIMAGEBUTTON m_pBtn_Cancel;

	virtual bool OnInitDialog();
};
