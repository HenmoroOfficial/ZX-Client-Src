// Filename	: DlgGuildMapStatus.h
// Creator	: Xiao Zhou
// Date		: 2005/1/5

#pragma once

#include "DlgBase.h"
#include "AUI\\AUILabel.h"
#include "AUI\\AUIEditBox.h"

class CDlgGuildMapStatus : public CDlgBase  
{
	AUI_DECLARE_COMMAND_MAP()

public:
	CDlgGuildMapStatus();
	virtual ~CDlgGuildMapStatus();

	void OnCommandCancel(const char * szCommand);
	void OnCommandConfirm(const char * szCommand);
	void OnCommandAllBattleTime(const char * szCommand);

	void SetIndex(int n);

protected:
	virtual void OnShowDialog();
	virtual bool OnInitDialog();
	virtual void OnTick();

	PAUILABEL	m_pTxt_Name;
	PAUILABEL	m_pTxt_Attach;
	PAUILABEL	m_pTxt_Attack1;
	PAUILABEL	m_pTxt_Attack2;
	PAUILABEL	m_pTxt_Time;
	PAUILABEL	m_pTxt_Time1;
	PAUILABEL	m_pTxt_Time2;
	int			m_nIndex;

};
