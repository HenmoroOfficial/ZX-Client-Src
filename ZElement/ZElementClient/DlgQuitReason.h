// Filename	: DlgQuitReason.h
// Creator	: Wu Weixin
// Date		: 2012/9/5

#pragma once

#include "DlgBase.h"

class AUIStillImageButton;
class AUIRadioButton;

class CDlgQuitReason : public CDlgBase  
{
	AUI_DECLARE_COMMAND_MAP();

private:
	enum Const
	{
		REASON_COUNT = 6,
	};
public:
	CDlgQuitReason();
	virtual ~CDlgQuitReason();
	
	void OnCommandConfirm(const char* szCommand);

protected:
	virtual	bool OnInitDialog();
	virtual void OnShowDialog();
	virtual void OnTick();

	AUIStillImageButton *	m_pBtnConfirm;
	AUIRadioButton *		m_pRdoReasons[REASON_COUNT];
};
