// Filename	: DlgShopExchange.h
// Creator	: Xiao Zhou
// Date		: 2008/7/3

#pragma once

#include "DlgBase.h"
#include "AUI\\AUILabel.h"
#include "AUI\\AUIImagePicture.h"
#include "AUI\\AUIStillImageButton.h"
#include "AUI\\AUIEditBox.h"

#define CDLGSHOPEXCHANGE_PAGE_MAX 4
#define CDLGSHOPEXCHANGE_ITEM_MAX 48

class CDlgShopExchange : public CDlgBase  
{
	AUI_DECLARE_EVENT_MAP();
	AUI_DECLARE_COMMAND_MAP()

friend class CDlgPetCombineExchange;

public:
	CDlgShopExchange();
	virtual ~CDlgShopExchange();

	void OnCommand_max(const char * szCommand);
	void OnCommand_minus(const char * szCommand);
	void OnCommand_add(const char * szCommand);
	void OnCommand_set(const char * szCommand);
	void OnCommand_exchange(const char * szCommand);
	void OnCommand_confirm(const char * szCommand);
	void OnCommand_CANCEL(const char * szCommand);

	void OnEventLButtonDown(WPARAM wParam, LPARAM lParam, AUIObject *pObj);
	void OnEventRButtonUp(WPARAM wParam, LPARAM lParam, AUIObject *pObj);

	int GetCurShopSet();
	bool UpdateShop(int nSet = 0);
	void UpdateItemInfo(int nCount = 1);
	static void SetShopIndex(int nIndex) { m_nShopIndex = nIndex; }

protected:
	PAUIIMAGEPICTURE m_pImgU[CDLGSHOPEXCHANGE_ITEM_MAX];
	PAUISTILLIMAGEBUTTON m_pBtnSet[CDLGSHOPEXCHANGE_PAGE_MAX];
	PAUIIMAGEPICTURE m_pImg_Item;
	PAUIIMAGEPICTURE m_pImg_ItemReq[2];
	PAUILABEL m_pTxt_ItemReq[2];
	PAUILABEL m_pTxt_CreditReq[2];
	PAUILABEL m_pTxt_SpecialReq;
	PAUILABEL m_pTxt_Name;
	PAUIEDITBOX m_pTxt_Amount;
	static int m_nShopIndex;
	static int m_nCurSel;
	static int m_nShopSet;

	virtual bool OnInitDialog();
	virtual void OnShowDialog();
	virtual void OnTick();

	void SelectItem(int nSel);

};
