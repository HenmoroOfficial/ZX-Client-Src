/********************************************************************
	created:	2005/08/15
	created:	15:8:2005   19:38
	file name:	DlgStorage.h
	author:		yaojun
	
	purpose:	
*********************************************************************/


#pragma once

#include "DlgBase.h"
#include "AUI\\AUIImagePicture.h"
#include "AUI\\AUIStillImageButton.h"
#include "AUI\\AUILabel.h"
#include "AUI\\AUIScroll.h"
#include "AUI\\AUIRadioButton.h"

#define CECDLGSTORAGE_PACKMAX 48
#define CECDLGSTORAGE_PACKLINEMAX 6
#define CECDLGSTORAGE_SIZEMAX 252

class CDlgStorage : public CDlgBase  
{
	AUI_DECLARE_COMMAND_MAP()
	AUI_DECLARE_EVENT_MAP()

public:
	CDlgStorage();
	virtual ~CDlgStorage();
	virtual bool Tick(void);

	void OnLButtonDownItem(WPARAM, LPARAM, AUIObject *pObj);
	void OnLButtonDbClkItem(WPARAM, LPARAM, AUIObject *pObj);
	void OnLButtonDownItem_Money(WPARAM, LPARAM, AUIObject * pObj);
	void OnEventMouseWheel(WPARAM wParam, LPARAM lParam, AUIObject *pObj);

	void OnCommandChooseMoney(const char * szCommand);
	void OnCommand_AutoArrangeItems(const char * szCommand);
	void OnCommandIDCANCEL(const char * szCommand);
	void OnCommandPage(const char * szCommand);
	void OnCommandExpand(const char * szCommand);
	void OnCommandModeSwitch(const char * szCommand);

	int GetFirstItem() { return m_nFirstLine * CECDLGSTORAGE_PACKLINEMAX; }
	void SetFactionStorage(bool bFaction) { m_bFaction = bFaction; }
	bool IsFactionStorage() { return m_bFaction; }
	bool IsPageMode() { return m_bPageMode;}
	void SetPage(int iPage);
	void OnItemExchanged(int iSlot1, int iSlot2);

protected:
	PAUIIMAGEPICTURE m_pImgItem[CECDLGSTORAGE_PACKMAX];
	int m_nFirstLine;
	bool m_bFaction;
	bool m_bPageArrange;
	bool m_bPageMode;

	PAUILABEL m_pLab_Expand;
	PAUILABEL m_pTxt_BagNum;
	PAUILABEL m_pTxt_Money1;
	PAUILABEL m_pTxt_Money2;
	PAUILABEL m_pTxt_Money3;
	PAUILABEL m_pTxt_Title1;
	PAUILABEL m_pTxt_Title2;
	PAUILABEL m_pLab_Money;
	PAUIIMAGEPICTURE m_pImg_Money;
	PAUISCROLL m_pScl_Item;
	PAUIRADIOBUTTON m_pRdo_Page[6];

	struct ExchangeItem {
		int iSrc;
		int iDst;
	};
	int CalcArrangeExchanges();
	void SendArrangeExchanges();

	static DWORD ARRANGE_ITEM_INTERVEL_SEC;

	DWORD m_nLastSendTime;
	DWORD m_nSendInterval;
	int m_nCurSendNum;
	abase::vector<ExchangeItem> m_aExchanges2Send;
	void UpdateArrangeSend();

	PAUISTILLIMAGEBUTTON m_pBtn_Switch;
	PAUISTILLIMAGEBUTTON m_pBtn_Arrange;
	PAUISTILLIMAGEBUTTON m_pBtn_ArrangeAll;
	PAUISTILLIMAGEBUTTON m_pBtn_Expand;

	bool UpdateStorage();
	virtual bool OnInitDialog();
	virtual void OnShowDialog();
};

