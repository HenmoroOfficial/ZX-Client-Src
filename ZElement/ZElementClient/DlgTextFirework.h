// File		: DlgTextFirework.h
// Creator	: Xiao Zhou
// Date		: 2006/12/18

#pragma once

#include "DlgBase.h"
#include "AUI\\AUILabel.h"
#include "AUI\\AUIEditBox.h"
#include "AUI\\AUIImagePicture.h"
#include "AUI\\AUIStillImageButton.h"

class CECIvtrItem;

class CDlgTextFirework : public CDlgBase  
{
	AUI_DECLARE_COMMAND_MAP()
	AUI_DECLARE_EVENT_MAP()

public:
	CDlgTextFirework();
	virtual ~CDlgTextFirework();

	void OnCommandConfirm(const char * szCommand);
	void OnCommandCancel(const char * szCommand);
	void OnCommandReset(const char * szCommand);

	void OnEventLButtonDown(WPARAM wParam, LPARAM lParam, AUIObject * pObj);

	void SetFirework(int nSlot);

protected:
	virtual bool OnInitDialog();
	virtual bool Render();

	PAUISTILLIMAGEBUTTON	m_pBtn_Confirm;
	PAUIIMAGEPICTURE		m_pImg_Item;
	PAUIEDITBOX				m_pTxt_Name;
	PAUILABEL				m_pTxt_Sel;
	PAUIEDITBOX				m_pTxt_Firework;
	int						m_nSolt;
};
