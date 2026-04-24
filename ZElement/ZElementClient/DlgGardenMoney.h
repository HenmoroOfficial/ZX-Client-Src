// Filename	: DlgGardenMoney.h
// Creator	: Fu Chonggang
// Date		: 2009/08/21

#pragma once

#include "DlgBase.h"

class CDlgGardenMoney : public CDlgBase  
{
	AUI_DECLARE_COMMAND_MAP();
	
public:
	CDlgGardenMoney();
	virtual ~CDlgGardenMoney();
	void OnCommand_Gold2System(const char * szCommand);
	void OnCommand_Garden2Money(const char * szCommand);
	void OnCommand_Confirm1(const char * szCommand);
	void OnCommand_Confirm2(const char * szCommand);
	void OnCommand_Confirm3(const char * szCommand);
	void OnCommand_BuyGold(const char * szCommand);
	void OnCommand_BuyMoney(const char * szCommand);
	void OnCommand_Close(const char * szCommand);
	
protected:
	virtual bool OnInitDialog();
	virtual void OnTick();

	int m_iMoneyLocal;
	int m_iMoneyUniversal;
	PAUILABEL				m_pTxt_Money1;
	PAUILABEL				m_pTxt_Money2;
	PAUILABEL				m_pTxt_Money3;
};
