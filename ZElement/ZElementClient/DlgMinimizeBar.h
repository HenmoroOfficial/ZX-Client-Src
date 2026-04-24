// Filename	: DlgMinimizeBar.h
// Creator	: Xiao Zhou
// Date		: 2005/11/14

#pragma once

#include "DlgBase.h"
#include "AUI\\AUILabel.h"
#include "AUI\\AUIDialog.h"

class CDlgMinimizeBar : public CDlgBase  
{
	AUI_DECLARE_COMMAND_MAP();
	AUI_DECLARE_EVENT_MAP();

public:
	CDlgMinimizeBar();
	virtual ~CDlgMinimizeBar();

	void OnCommandCancel(const char * szCommand);

	void OnEventLButtonDBCLK(WPARAM wParam, LPARAM lParam, AUIObject *pObj);

	void MinimizeDialog(PAUIDIALOG pDlg, const ACHAR * szSubject, int nType);
	void RestoreDialog(PAUIDIALOG pDlg);
	void FlashDialog(PAUIDIALOG pDlg);
	void ArrangeMinimizeBar();

	enum
	{
		MINIMIZEBAR_TYPE_FRIEND,
		MINIMIZEBAR_TYPE_WHISPER,
		MINIMIZEBAR_TYPE_CHATROOM,
		MINIMIZEBAR_TYPE_QUESTION,
		MINIMIZEBAR_TYPE_RIDEKICK,
		MINIMIZEBAR_TYPE_SECRETARY,
		MINIMIZEBAR_TYPE_GARDEN,
		MINIMIZEBAR_TYPE_GAMEHALL,
		MINIMIZEBAR_TYPE_GAMEDDZ,
		MINIMIZEBAR_TYPE_BROADCAST,
		MINIMIZEBAR_TYPE_BOOTH,
		MINIMIZEBAR_TYPE_COLLISION6V6,
		MINIMIZEBAR_TYPE_FlowBattle,
	};

protected:
	virtual bool OnInitDialog();
	virtual void OnTick();

	PAUILABEL	m_pTxt_Subject;
	static int	m_nTotalBars;
	static bool	m_bSystemMenuExpand;
	bool		m_bFlash;
};
