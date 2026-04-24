// File		: DlgGoldAccount.h
// Creator	: Xiao Zhou
// Date		: 2006/9/20

#pragma once

#include "DlgBase.h"
#include "AUI\\AUIListBox.h"
#include "AUI\\AUILabel.h"
#include "AUI\\AUIEditBox.h"


class CDlgGoldAccount : public CDlgBase  
{
	friend class CDlgGoldTrade;

	AUI_DECLARE_COMMAND_MAP()
	AUI_DECLARE_EVENT_MAP()

public:
	CDlgGoldAccount();
	virtual ~CDlgGoldAccount();

	void OnCommandConfirm(const char * szCommand);
	void OnCommandChooseMoney(const char * szCommand);
	void OnCommandGoldMax(const char * szCommand);
	void OnCommandMoneyMax(const char * szCommand);

	void OnEventLButtonUp(WPARAM wParam, LPARAM lParam, AUIObject * pObj);

protected:
	virtual void OnShowDialog();
	virtual bool OnInitDialog();
	virtual void OnTick();

	PAUIEDITBOX		m_pTxt_Gold;
	PAUIEDITBOX		m_pTxt_Silver;
	PAUILABEL		m_pTxt_Money1;
	PAUILABEL		m_pTxt_Money2;
	PAUILABEL		m_pTxt_Money3;
};
