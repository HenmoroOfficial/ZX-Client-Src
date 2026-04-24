// File		: DlgConsole.h
// Creator	: Xiao Zhou
// Date		: 2005/8/16

#pragma once

#include "DlgBase.h"
#include "AUI\\AUIConsole.h"
#include "AUI\\AUIEditBox.h"
#include "vector.h"

class CDlgConsole : public CDlgBase  
{
	AUI_DECLARE_COMMAND_MAP()
	AUI_DECLARE_EVENT_MAP()

	struct CMD_INFO
	{
		int		t;		// —”≥Ÿ÷¥–– ±º‰
		AString cmd;	// √¸¡Ó
	};

	typedef abase::vector<CMD_INFO> CMDVector;

public:

	CDlgConsole();
	virtual ~CDlgConsole();
	
	void OnCommandEdit(const char * szCommand);

	void OnEventKeyDown(WPARAM wParam, LPARAM lParam, AUIObject * pObj);
	void OnEventChar(WPARAM wParam, LPARAM lParam, AUIObject * pObj);

	virtual int CommonMessage(AString strMsg, WPARAM wParam, LPARAM lParam);

protected:
	virtual void DoDataExchange(bool bSave);
	virtual bool OnInitDialog();
	virtual void OnShowDialog();
	virtual void OnTick();
	
	AUIEditBox *	m_pCommand;
	AUIConsole *	m_pCommandEdit;

	int	m_iTickTime;
	int	m_iRunTimes;
	int	m_iCurIndex;

private:
	CMDVector m_vecCmds;
	static const AString SCRIPT_NAME;
};
