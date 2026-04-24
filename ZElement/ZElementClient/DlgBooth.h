// Filename	: DlgBooth.h
// Creator	: Tom Zhou
// Date		: October 11, 2005

#pragma once

#include "DlgBase.h"
#include "AUI\\AUITextArea.h"
#include "AUI\\AUILabel.h"

class CDlgBooth : public CDlgBase  
{
	AUI_DECLARE_EVENT_MAP();
	AUI_DECLARE_COMMAND_MAP()

public:
	CDlgBooth();
	virtual ~CDlgBooth();
	virtual bool Tick(void);

	void OnCommand_max(const char * szCommand);
	void OnCommand_add(const char * szCommand);
	void OnCommand_minus(const char * szCommand);
	void OnCommand_buy(const char * szCommand);
	void OnCommand_sell(const char * szCommand);
	void OnCommand_reset(const char * szCommand);
	void OnCommand_confirm(const char * szCommand);
	void OnCommand_sendmsg(const char * szCommand);
	void OnCommand_clearmsg(const char * szCommand);
	void OnCommand_Minimize(const char * szCommand);
	void OnCommand_CANCEL(const char * szCommand);

	void OnEventLButtonUp_SellItem(WPARAM wParam, LPARAM lParam, AUIObject *pObj);
	void OnEventLButtonUp_BuyItem(WPARAM wParam, LPARAM lParam, AUIObject *pObj);
	void OnEventLButtonDown(WPARAM wParam, LPARAM lParam, AUIObject *pObj);
	void OnEventLButtonDblclk_SellItem(WPARAM wParam, LPARAM lParam, AUIObject *pObj);
	void OnEventLButtonDblclk_Sell(WPARAM wParam, LPARAM lParam, AUIObject *pObj);
	void OnEventLButtonDblclk_Buy(WPARAM wParam, LPARAM lParam, AUIObject *pObj);
	void OnEventMouseMove_MsgList(WPARAM wParam, LPARAM lParam, AUIObject *pObj);
	void OnEventLButtonDown_MsgList(WPARAM wParam, LPARAM lParam, AUIObject *pObj);
	void OnEventLButtonUp_MsgList(WPARAM wParam, LPARAM lParam, AUIObject *pObj);
	void OnEventRButtonDown_MsgList(WPARAM wParam, LPARAM lParam, AUIObject *pObj);
	void AddBoothMessage(const ACHAR *pszMsg);

	void TickBoothMinimize();

protected:
	PAUILABEL m_pTxt_Money1;
	PAUILABEL m_pTxt_Money2;
	PAUILABEL m_pTxt_Money3;
	PAUILABEL m_pTxt_BuyMoney1;
	PAUILABEL m_pTxt_BuyMoney2;
	PAUILABEL m_pTxt_BuyMoney3;
	PAUILABEL m_pTxt_SellMoney1;
	PAUILABEL m_pTxt_SellMoney2;
	PAUILABEL m_pTxt_SellMoney3;
	__int64 m_nMoney1, m_nMoney2;
	
	virtual bool OnInitDialog();
	virtual void OnShowDialog();

	bool UpdateBooth();
	bool UpdateBooth1();
	bool UpdateBooth2();
	bool GetNameLinkMouseOn(int x, int y,PAUIOBJECT pObj, P_AUITEXTAREA_NAME_LINK pLink);
};
