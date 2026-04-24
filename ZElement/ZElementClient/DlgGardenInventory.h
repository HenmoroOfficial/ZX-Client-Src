// Filename	: DlgGardenInventory.h
// Creator	: Fu Chonggang
// Date		: 2009/08/21

#pragma once

#include "DlgBase.h"
#include "AUI\\AUILabel.h"
#include "AUI\\AUIImagePicture.h"
#include "AUI\\AUIStillImageButton.h"
#include "AUI\\AUIScroll.h"
#include "AUI\\AUICheckBox.h"
#include "EC_HTPDataType.h"

#define CECDLGSHOP_PACKMAX 48
#define CECDLGSHOP_PACKLINEMAX 6

class CDlgGardenInventory : public CDlgBase  
{
	AUI_DECLARE_COMMAND_MAP()
	AUI_DECLARE_EVENT_MAP()
	
public:
	CDlgGardenInventory();
	virtual ~CDlgGardenInventory();
	void OnCommand_Close(const char * szCommand);
	void OnEventLButtonDown(WPARAM wParam, LPARAM lParam, AUIObject *pObj);
	void OnEventLButtonDblclk(WPARAM wParam, LPARAM lParam, AUIObject *pObj);
	
	void OnPrtc_UseItem(void*);
	void OnPrtc_GetPackage(void*);
	
	int GetFirstItem() { return m_iFirstLine * CECDLGSHOP_PACKLINEMAX; }
	void SetFieldId( int fieldid) {  m_fieldid = fieldid;}
	void UseItem();
	
protected:
	virtual void OnTick();
	virtual bool OnInitDialog();
	void UpdateInventory();
	void UpdateHint();
	
	PAUILABEL m_pTxt_BagNum;
	PAUILABEL m_pTxt_Money;
	PAUIIMAGEPICTURE m_pImgItem[CECDLGSHOP_PACKMAX];
	PAUISCROLL m_pScl_Item;

	int m_iFirstLine;
	int m_iCapacity;

	int m_pos;
	int m_itemid;
	int m_targettype;
	int m_fieldid;
};
