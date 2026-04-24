/********************************************************************
	created:	2005/08/15
	created:	15:8:2005   17:59
	file name:	DlgRevive.h
	author:		yaojun
	
	purpose:	
*********************************************************************/


#pragma once

#include "DlgBase.h"
#include "AUI\\AUILabel.h"
#include "AUI\\AUIStillImageButton.h"

class CDlgRevive : public CDlgBase  
{
	friend class CECGameUIMan;

	AUI_DECLARE_COMMAND_MAP()

public:
	CDlgRevive();
	virtual ~CDlgRevive();
	virtual void OnTick();

	void OnCommandBack(const char * szCommand);
	void OnCommandConfirm(const char * szCommand);
	void OnCommandAccept(const char * szCommand);
	void OnCommandCancel(const char * szCommand);
	void BattleOut();

protected:
	PAUILABEL m_pTxt_D;
	PAUISTILLIMAGEBUTTON m_pBtn_Back;
	PAUISTILLIMAGEBUTTON m_pBtn_Decide;
	PAUISTILLIMAGEBUTTON m_pBtn_Accept;

	DWORD m_dwReviveTime;

	virtual bool OnInitDialog();
};

