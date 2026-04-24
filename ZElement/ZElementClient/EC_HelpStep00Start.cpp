/********************************************************************
	created:	2012/8/17
	author:		fuchonggang
	
	purpose:	图形帮助系统-开始选择
	Copyright (C) 2012 - All Rights Reserved
*********************************************************************/

#include "EC_HelpStep00Start.h"
#include "EC_HelpGraphGuide.h"
#include "EC_Global.h"
#include "EC_Game.h"
#include "EC_GameRun.h"
#include "EC_UIManager.h"
#include "EC_GameUIMan.h"
#include "AUI\\AUIDialog.h"


CECHelpStep00Start::CECHelpStep00Start()
{
	m_iStartIndex	= CECHelpGraphGuide::HELP2012_0_1_OPEN_ORNOT;
	m_iEndIndex		= CECHelpGraphGuide::HELP2012_0_1_OPEN_ORNOT;
}

CECHelpStep00Start::~CECHelpStep00Start()
{
}

void CECHelpStep00Start::Help0(bool bStart)
{
	PAUIDIALOG pDlg = g_pGame->GetGameRun()->GetUIManager()->GetInGameUIMan()->GetDialog("Win_NewerHelp_Hint");
	pDlg->Show(bStart);
	m_rcArea.left = pDlg->GetPos().x;
	m_rcArea.right = pDlg->GetPos().x+pDlg->GetSize().cx;
	m_rcArea.top = pDlg->GetPos().y;
	m_rcArea.bottom = pDlg->GetPos().y+pDlg->GetSize().cy;
	m_bDealKeyMsg = true;
}