// File		: DlgResetProp.h
// Creator	: Xiao Zhou
// Date		: 2006/1/12

#pragma once

#include "DlgBase.h"
#include "AUI\\AUIImagePicture.h"
#include "AUI\\AUIRadioButton.h"
#include "AUI\\AUILabel.h"
#include "AUI\\AUIStillImageButton.h"

class CECIvtrItem;

class CDlgResetProp : public CDlgBase  
{
	AUI_DECLARE_COMMAND_MAP()
	AUI_DECLARE_EVENT_MAP()

public:
	CDlgResetProp();
	virtual ~CDlgResetProp();

	void OnCommandConfirm(const char * szCommand);
	void OnCommandCancel(const char * szCommand);

	void OnEventLButtonDown(WPARAM wParam, LPARAM lParam, AUIObject * pObj);

	void SetScroll(CECIvtrItem *pItem);
	
protected:
	virtual void OnShowDialog();
	virtual bool OnInitDialog();
	virtual void OnTick();

	PAUIIMAGEPICTURE		m_pImg_Item;
	PAUISTILLIMAGEBUTTON	m_pBtn_Confirm;
	int						m_nPropEntryIndex;
	
};
