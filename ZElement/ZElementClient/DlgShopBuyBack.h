// Filename	: DlgShopBuyBack.h
// Creator	: Fu Chonggang
// Date		: 2012.7.31

#pragma once

#include "DlgBase.h"
#include "EC_IvtrTypes.h"

class AUILabel;
class AUIImagePicture;
class CECInventory;

class CDlgShopBuyBack : public CDlgBase  
{
	AUI_DECLARE_COMMAND_MAP();
	AUI_DECLARE_EVENT_MAP();

public:
	CDlgShopBuyBack();
	virtual ~CDlgShopBuyBack();

	void OnCommandBuyBack(const char * szCommand);
	
	void OnEventLButtonDownSell(WPARAM wParam, LPARAM lParam, AUIObject *pObj);
	void OnEventLButtonDownBuy(WPARAM wParam, LPARAM lParam, AUIObject *pObj);

	void UpdateInventory();
	void Reset();
protected:
	virtual bool OnInitDialog();
	virtual void OnShowDialog();
	virtual bool Release();
	virtual void OnTick();

	CECInventory *m_pInvBuyBack;
	
	AUILabel* m_pTxt_Money1;
	AUILabel* m_pTxt_Money2;
	AUILabel* m_pTxt_Money3;
	AUILabel* m_pTxt_BuyMoney1;
	AUILabel* m_pTxt_BuyMoney2;
	AUILabel* m_pTxt_BuyMoney3;

	AUIImagePicture* m_pImgInv[IVTRSIZE_SELLBACKPACK];
	AUIImagePicture* m_pImgBuy[IVTRSIZE_SELLBACKPACK];
};
