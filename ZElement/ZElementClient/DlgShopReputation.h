#pragma once

#include "DlgBase.h"
#include "EC_IvtrTypes.h"
#include "AUI\\AUILabel.h"
#include "AUI\\AUIImagePicture.h"
#include "AUI\\AUIStillImageButton.h"
#include "AUI\\AUIEditBox.h"

#define CDLGSHOPREPUTATION_PAGE_MAX 4
#define	CDLGSHOPREPUTATION_ITEM_MAX 48

class CDlgShopReputation : public CDlgBase  
{
	AUI_DECLARE_EVENT_MAP();
	AUI_DECLARE_COMMAND_MAP()
public:

	enum
	{
		TIANZONG_CONSUME = 22,
		DIZONG_CONSUME,
		RENZONG_CONSUME,		
	};

	CDlgShopReputation();
	virtual ~CDlgShopReputation();
	virtual bool Tick(void);

	void OnCommand_set(const char * szCommand);
	void OnCommand_buy(const char * szCommand);
	void OnCommand_CANCEL(const char * szCommand);

	void OnEventLButtonDown(WPARAM wParam, LPARAM lParam, AUIObject *pObj);
	void OnEventRButtonUp(WPARAM wParam, LPARAM lParam, AUIObject *pObj);
	void OnEventLButtonDblclk(WPARAM wParam, LPARAM lParam, AUIObject *pObj);
	bool UpdateShop();
	int  GetCurShopSet();

protected:
	int m_nCurShopSet;
	int m_iRepu_Consume_Value[3];
	bool m_iFlag;
	PAUILABEL m_pTxt_Reputation23;
	PAUILABEL m_pTxt_Reputation24;
	PAUILABEL m_pTxt_Reputation25;
	PAUILABEL m_pTxt_Reputation26;
	PAUILABEL m_pTxt_Reputation27;
	PAUILABEL m_pTxt_Reputation28;
	PAUILABEL m_pLab_Hint;
	PAUILABEL m_pLab_Hint2;
	PAUIIMAGEPICTURE m_pImgU[IVTRSIZE_NPCPACK];
	PAUIIMAGEPICTURE m_pImgBuy[IVTRSIZE_BUYPACK];
	PAUISTILLIMAGEBUTTON m_pBtnSet[NUM_NPCIVTR];	
	virtual bool OnInitDialog();
	virtual void OnShowDialog();
};
