// File		: DlgGoldInquire.h
// Creator	: Xiao Zhou
// Date		: 2006/9/20

#pragma once

#include "DlgBase.h"
#include "AUI\\AUIListBox.h"
#include "AUI\\AUIStillImageButton.h"

class CDlgGoldInquire : public CDlgBase  
{
	friend class CDlgGoldTrade;

	AUI_DECLARE_COMMAND_MAP()

public:
	CDlgGoldInquire();
	virtual ~CDlgGoldInquire();

	void OnCommandGiveUp(const char * szCommand);
	void OnCommandTrade(const char * szCommand);
	void OnCommandUnlock(const char * szCommand);
	void OnCommandHelp(const char * szCommand);
	void OnCommandCancel(const char * szCommand);

	void CardAction(int idAction, void *pData);

	
protected:
	virtual void OnShowDialog();
	virtual bool OnInitDialog();
	virtual void OnTick();

	PAUILISTBOX				m_pLst_Sell;
	PAUILISTBOX				m_pLst_Buy;
	PAUILISTBOX				m_pLst_Inquire;
	PAUISTILLIMAGEBUTTON	m_pBtn_GiveUp;
};
