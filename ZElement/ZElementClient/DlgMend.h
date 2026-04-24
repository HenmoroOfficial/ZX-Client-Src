// Filename	: DlgMend.h
// Creator	: Fu Chonggang
// Date		: 2008/12/03

#pragma once

#include "DlgBase.h"

class CECIvtrItem;
class AUIImagePicture;

class CDlgMend : public CDlgBase  
{
	AUI_DECLARE_COMMAND_MAP()
	AUI_DECLARE_EVENT_MAP()

public:
	CDlgMend();
	virtual ~CDlgMend();

	void OnCommand_Mend(const char* szCommand);
	void OnCommand_MendMoney(const char* szCommand);
	void OnCommand_Cancel(const char* szCommand);

	void SetBrokeItem(CECIvtrItem *pItem, int nSlot);
	void SetRepairItem(CECIvtrItem *pItem, int nSlot);
	void OnEventLButtonDown(WPARAM wParam, LPARAM lParam, AUIObject * pObj);

protected:
	virtual bool OnInitDialog();
	virtual void OnShowDialog();
	virtual void OnTick();

	AUIImagePicture *m_pImg_ItemBroke;
	AUIImagePicture *m_pImg_ItemRepair;

	CECIvtrItem *m_pItemBroke;
	CECIvtrItem *m_pItemRepair;
	int	m_iIndexBroke;
	int	m_iIndexRepair;	
};
