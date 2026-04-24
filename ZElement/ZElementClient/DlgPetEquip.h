// Filename	: DlgPetEquip.h
// Creator	: Xiao Zhou
// Date		: 2008/4/11

#pragma once

#include "DlgBase.h"
#include "EC_IvtrTypes.h"
#include "AUI\\AUIImagePicture.h"

class CDlgPetEquip : public CDlgBase  
{
	AUI_DECLARE_COMMAND_MAP()
	AUI_DECLARE_EVENT_MAP()
		
public:
	CDlgPetEquip();
	virtual ~CDlgPetEquip();

	void OnCommand_CANCEL(const char * szCommand);
	void OnEventLButtonDown(WPARAM wParam, LPARAM lParam, AUIObject *pObj);
	void OnEventLButtonDblclk(WPARAM wParam, LPARAM lParam, AUIObject *pObj);

protected:
	PAUIIMAGEPICTURE m_pImgEquip[IVTRSIZE_PETEQUIP];

	virtual bool OnInitDialog();
	virtual void OnTick();
};
