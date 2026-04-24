// Filename	: DlgInventory.h
// Creator	: Tom Zhou
// Date		: October 10, 2005

#pragma once

#include "DlgBase.h"
#include "EC_IvtrTypes.h"
#include "AUI\\AUILabel.h"
#include "AUI\\AUIImagePicture.h"
#include "AUI\\AUIStillImageButton.h"
#include "AUI\\AUIScroll.h"
#include "AUI\\AUICheckBox.h"
#include "AUI\\AUIRadioButton.h"
#define CECDLGSHOP_PACKMAX 48
#define CECDLGSHOP_PACKLINEMAX 6
#define CECINVENTORY_SIZEMAX 252

class CECIvtrItem;

class CDlgInventory : public CDlgBase  
{
	AUI_DECLARE_EVENT_MAP();
	AUI_DECLARE_COMMAND_MAP()

public:
	CDlgInventory();
	virtual ~CDlgInventory();
	virtual bool Tick(void);

	void OnCommand_choosemoney(const char * szCommand);
	void OnCommand_Pack(const char * szCommand);
	void OnCommand_showinfo(const char * szCommand);
	void OnCommand_booth(const char * szCommand);
	void OnCommand_split(const char * szCommand);
	void OnCommand_AutoArrangeItems(const char * szCommand);
	void OnCommand_MoveAllToPacket(const char * szCommand);
	void OnCommand_PetPocketHelp(const char * szCommand);
	void OnCommand_SetAutoIn(const char * szCommand);
	void OnCommand_CANCEL(const char * szCommand);
	void OnCommand_Convert(const char * szCommand);
	void OnCommand_Page(const char * szCommand);
	void OnCommand_Expand(const char * szCommand);
	void OnCommand_ModeSwitch(const char * szCommand);

	void OnEventLButtonDown(WPARAM wParam, LPARAM lParam, AUIObject *pObj);
	void OnEventLButtonDblclk(WPARAM wParam, LPARAM lParam, AUIObject *pObj);
	void OnEventLButtonDownMoney(WPARAM wParam, LPARAM lParam, AUIObject *pObj);
	void OnEventMouseWheel(WPARAM wParam, LPARAM lParam, AUIObject *pObj);

	void OnItemExchanged(int iSlot1, int iSlot2);

	int GetShowItem();
	void SetShowItem(int nShowItem);

	int GetFirstItem() { return m_nFirstLine * CECDLGSHOP_PACKLINEMAX; }
	void SellConfirm(bool bYes=false);
	void EmphasizeItem(int id);
	void EraseEmphasize(int index);
	bool EmphasizeButton() { return m_aItemEmphasize.size()>0;}
	bool GetIsPageMode() {return m_bPageMode;}
	void SetPage(int iPage);
	static bool GetIsArranging() { return m_bArranging; }

	bool SetHelpItem(int id);

	enum
	{
		INVENTORY_ITEM_NORMAL = 0,
		INVENTORY_ITEM_TASK,
		INVENTORY_ITEM_ALONG
	};

protected:
	static bool m_bArranging;
	static int m_nShowIvtrItem;
	int m_nFirstLine;
	bool m_bPageArrange;
	bool m_bPageMode;

	PAUILABEL m_pLab_Expand;
	PAUILABEL m_pLab_Expand1;
	PAUILABEL m_pLab_ExpandLevel15;
	PAUILABEL m_pLab_ExpandLevel45;
	PAUILABEL m_pLab_ExpandLevel75;
	PAUILABEL m_pLab_ExpandLevel105;
	PAUILABEL m_pTxt_BagNum;
	PAUILABEL m_pTxt_Money1;
	PAUILABEL m_pTxt_Money2;
	PAUILABEL m_pTxt_Money3;
	PAUIIMAGEPICTURE m_pImgItem[CECDLGSHOP_PACKMAX];
	PAUISCROLL m_pScl_Item;
	AUIObject *m_pObjSell;
	PAUIRADIOBUTTON	m_pRdo_Page[6];

	struct ExchangeItem {
		int iSrc;
		int iDst;
	};
	int CalcArrangeExchanges();
	void SendArrangeExchanges();

	static DWORD ARRANGE_ITEM_INTERVEL_SEC;
	
	abase::vector<BYTE> m_aPackItemFrozen;
	abase::vector<int> m_aItemEmphasize;

	DWORD m_nLastSendTime;
	DWORD m_nSendInterval;
	int m_nCurSendNum;
	abase::vector<ExchangeItem> m_aExchanges2Send;
	void UpdateArrangeSend();

	PAUISTILLIMAGEBUTTON m_pBtn_Switch;
	PAUISTILLIMAGEBUTTON m_pBtn_Expand;
	PAUISTILLIMAGEBUTTON m_pBtn_Arrange;
	PAUISTILLIMAGEBUTTON m_pBtn_ArrangeAll;
	PAUISTILLIMAGEBUTTON m_pBtn_Pocket;
	PAUISTILLIMAGEBUTTON m_pBtn_FashionPocket;
	PAUISTILLIMAGEBUTTON m_pBtn_WingPocket;
	PAUICHECKBOX		 m_pChk_AutoIn;

	int		m_iHelpItemId;

	bool UpdateInventory();
	virtual bool OnInitDialog();
	virtual void OnShowDialog();
	
	void ItemForConsign(PAUIOBJECT pObj, CECIvtrItem* pItem);
	bool CanConsign(CECIvtrItem* pItem);
	void ItemForBijouUpgrade(PAUIOBJECT pObj, CECIvtrItem* pItem);
};
