// File		: DlgEquipUndestroy.h
// Creator	: Xiao Zhou
// Date		: 2006/7/19

#pragma once

#include "DlgBase.h"
#include "AUI\\AUIEditBox.h"
#include "AUI\\AUIImagePicture.h"
#include "AUI\\AUIStillImageButton.h"

class CECIvtrItem;

class CDlgEquipUndestroy : public CDlgBase  
{
	AUI_DECLARE_COMMAND_MAP()
	AUI_DECLARE_EVENT_MAP()

public:
	CDlgEquipUndestroy();
	virtual ~CDlgEquipUndestroy();

	void OnCommandConfirm(const char * szCommand);
	void OnCommandCancel(const char * szCommand);

	void OnEventLButtonDown(WPARAM wParam, LPARAM lParam, AUIObject * pObj);

	void SetEquip(CECIvtrItem *pItem, int nSlot);

protected:
	virtual bool OnInitDialog();
	virtual void OnShowDialog();

	PAUISTILLIMAGEBUTTON	m_pBtn_Confirm;
	PAUIIMAGEPICTURE		m_pImg_Item;
	PAUIEDITBOX				m_pTxt_Name;
	int						m_nSolt;
};
