// File		: DlgTrade.h
// Creator	: Xiao Zhou
// Date		: 2005/8/16

#pragma once

#include "DlgBase.h"
#include "EC_IvtrTypes.h"
#include "AUI\\AUIEditBox.h"
#include "AUI\\AUIImagePicture.h"
#include "AUI\\AUIStillImageButton.h"
#include "AUI\\AUILabel.h"

class CDlgTrade : public CDlgBase  
{
	friend class CDlgInputNO;
	friend class CECGameUIMan;

	AUI_DECLARE_COMMAND_MAP()
	AUI_DECLARE_EVENT_MAP()

public:
	CDlgTrade();
	virtual ~CDlgTrade();
	virtual bool Tick(void);

	void OnCommandCANCEL(const char * szCommand);
	void OnCommandLock(const char * szCommand);
	void OnCommandComplete(const char * szCommand);
	void OnCommandChoosemoney(const char * szCommand);

	void OnEventLButtonDown(WPARAM wParam, LPARAM lParam, AUIObject * pObj);
	void OnEventLButtonDBLCLK(WPARAM wParam, LPARAM lParam, AUIObject * pObj);
	void OnEventLButtonDown_Money(WPARAM wParam, LPARAM lParam, AUIObject * pObj);

	void TradeAction(int idPlayer, int idTrade, int idAction, int nCode);

	enum
	{
		TRADE_ACTION_NONE = 0,
		TRADE_ACTION_ALTER,
		TRADE_ACTION_LOCK,
		TRADE_ACTION_UNLOCK,
		TRADE_ACTION_CANCEL,
		TRADE_ACTION_DEAL,
		TRADE_ACTION_END
	};

protected:
	bool UpdateTradeInfo();
	virtual bool OnInitDialog();
	virtual void OnShowDialog();
	
	int						m_idTradeAction;
	AUIStillImageButton *	m_pBtnLock;
	AUIStillImageButton *	m_pBtnLock2;
	AUIStillImageButton *	m_pBtnConfirm;
	AUIStillImageButton *	m_pBtnConfirm2;
	AUIImagePicture *		m_pMyItem[IVTRSIZE_DEALPACK];
	AUIImagePicture *		m_pYourItem[IVTRSIZE_DEALPACK];
	AUIEditBox *			m_pTxtGold;
	AUIEditBox *			m_pTxtGold2;
	PAUILABEL m_pTxt_Money1U;
	PAUILABEL m_pTxt_Money2U;
	PAUILABEL m_pTxt_Money3U;
	PAUILABEL m_pTxt_Money1I;
	PAUILABEL m_pTxt_Money2I;
	PAUILABEL m_pTxt_Money3I;
};
