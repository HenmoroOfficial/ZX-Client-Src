// Filename	: DlgShop.h
// Creator	: Tom Zhou
// Date		: October 11, 2005

#pragma once

#include "DlgBase.h"
#include "EC_IvtrTypes.h"
#include "AUI\\AUILabel.h"
#include "AUI\\AUIImagePicture.h"
#include "AUI\\AUIStillImageButton.h"

class CDlgShop : public CDlgBase  
{
	AUI_DECLARE_EVENT_MAP();
	AUI_DECLARE_COMMAND_MAP()

public:
	CDlgShop();
	virtual ~CDlgShop();
	virtual bool Tick(void);

	void OnCommand_set(const char * szCommand);
	void OnCommand_buy(const char * szCommand);
	void OnCommand_sell(const char * szCommand);
	void OnCommand_repair(const char * szCommand);
	void OnCommand_repairall(const char * szCommand);
	void OnCommand_left(const char * szCommand);
	void OnCommand_right(const char * szCommand);
	void OnCommand_AutoSellSetting(const char * szCommand);
	void OnCommand_AutoSell(const char * szCommand);
	void OnCommand_Confirm(const char * szCommand);
	void OnCommand_BuyBack(const char * szCommand);
	void OnCommand_CANCEL(const char * szCommand);

	void OnEventLButtonDown(WPARAM wParam, LPARAM lParam, AUIObject *pObj);
	void OnEventRButtonUp(WPARAM wParam, LPARAM lParam, AUIObject *pObj);
	void OnEventLButtonDblclk(WPARAM wParam, LPARAM lParam, AUIObject *pObj);

	int GetCurShopSet();
	bool UpdateShop(int nSet = 0);

protected:
	int m_nCurShopSet;
	int	m_nFirstSet;

	PAUILABEL m_pTxt_Money1;
	PAUILABEL m_pTxt_Money2;
	PAUILABEL m_pTxt_Money3;
	PAUILABEL m_pTxt_BuyMoney1;
	PAUILABEL m_pTxt_BuyMoney2;
	PAUILABEL m_pTxt_BuyMoney3;
	PAUILABEL m_pTxt_SellMoney1;
	PAUILABEL m_pTxt_SellMoney2;
	PAUILABEL m_pTxt_SellMoney3;
	PAUIIMAGEPICTURE m_pImgU[IVTRSIZE_NPCPACK];
	PAUIIMAGEPICTURE m_pImgBuy[IVTRSIZE_BUYPACK];
	PAUIIMAGEPICTURE m_pImgSell[IVTRSIZE_SELLPACK];
	PAUISTILLIMAGEBUTTON m_pBtnSet[NUM_NPCIVTR];

	virtual bool OnInitDialog();
	virtual void OnShowDialog();
	void UpdateSetButton();
	bool SellItems();
	bool m_bIsAutoSelling;
};
