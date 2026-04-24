// File		: DlgGoldTrade.h
// Creator	: Xiao Zhou
// Date		: 2006/9/20

#pragma once

#include "DlgBase.h"
#include "AUI\\AUIListBox.h"
#include "AUI\\AUILabel.h"
#include "AUI\\AUIEditBox.h"

class CDlgGoldTrade : public CDlgBase  
{
	AUI_DECLARE_COMMAND_MAP()
	AUI_DECLARE_EVENT_MAP()

public:
	struct StockInfo
	{
		int volume;
		int	price;
		int time;
		int tid;
	};

	enum
	{
		GOLD_ACTION_BILL_RE,
		GOLD_ACTION_ACCOUNT_RE,
		GOLD_ACTION_COMMISSION_RE,
		GOLD_ACTION_TRANSACTION_RE,
		GOLD_ACTION_CANCEL_RE,
	};

	CDlgGoldTrade();
	virtual ~CDlgGoldTrade();

	void OnCommandAccount(const char * szCommand);
	void OnCommandInquire(const char * szCommand);
	void OnCommandChooseMoney(const char * szCommand);
	void OnCommandSell(const char * szCommand);
	void OnCommandBuy(const char * szCommand);
	void OnCommandConfirm(const char * szCommand);
	void OnCommandRefresh(const char * szCommand);
	void OnCommandPwd(const char * szCommand);
	void OnCommandUnlock(const char * szCommand);
	void OnCommandHelp(const char * szCommand);
	void OnCommandCancel(const char * szCommand);
	
	void OnEventLButtonUp(WPARAM wParam, LPARAM lParam, AUIObject * pObj);

	void GoldAction(int idAction, void *pData);
	void SetLocked(bool bLock);
	bool IsLocked() { return m_bLocked; }

	ACString GetMoneyText(int nMoney);

	abase::vector<StockInfo>	m_vecAccountOrders;
	
protected:
	virtual void OnShowDialog();
	virtual bool OnInitDialog();
	virtual void OnTick();
	bool CheckRetcode(int retcode, int defaulErrMsg);

	PAUILISTBOX		m_pLst_Sell;
	PAUILISTBOX		m_pLst_Buy;
	PAUIEDITBOX		m_pTxt_Num;
	PAUILABEL		m_pTxt_Gold;
	PAUILABEL		m_pTxt_Gold1;
	PAUILABEL		m_pTxt_Money11;
	PAUILABEL		m_pTxt_Money12;
//	PAUILABEL		m_pTxt_Money13;
	PAUILABEL		m_pTxt_Money21;
	PAUILABEL		m_pTxt_Money22;
	PAUILABEL		m_pTxt_Money23;
	PAUILABEL		m_pTxt_Money41;
	PAUILABEL		m_pTxt_Money42;
	PAUILABEL		m_pTxt_Money43;
	PAUILABEL		m_pTxt_Money51;
	PAUILABEL		m_pTxt_Money52;
	PAUILABEL		m_pTxt_Money53;
	PAUILABEL		m_pTxt_Poundage;
	int				m_nBuy;
	bool			m_bLocked;
};
