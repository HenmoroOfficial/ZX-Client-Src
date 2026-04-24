/********************************************************************
	created:	2012/8/17
	author:		fuchonggang
	
	purpose:	图形帮助系统-移动篇
	Copyright (C) 2012 - All Rights Reserved
*********************************************************************/

#include "EC_HelpStep01Move.h"
#include "EC_HelpGraphGuide.h"
#include "EC_Global.h"
#include "EC_Game.h"
#include "EC_GameRun.h"
#include "EC_UIManager.h"
#include "EC_GameUIMan.h"
#include "AUI\\AUIDialog.h"


CECHelpStep01Move::CECHelpStep01Move()
{
	m_iStartIndex	= CECHelpGraphGuide::HELP2012_1_1_WASD_MOVE;
	m_iEndIndex		= CECHelpGraphGuide::HELP2012_1_1_WASD_MOVE;
}

CECHelpStep01Move::~CECHelpStep01Move()
{
}

void CECHelpStep01Move::Help0(bool bStart)
{
	PAUIDIALOG pDlg = g_pGame->GetGameRun()->GetUIManager()->GetInGameUIMan()->GetDialog("Win_NewerHelp_WASD");
	pDlg->Show(bStart);
	pDlg->SetCanMove(!bStart);
	m_rcArea.left = pDlg->GetPos().x;
	m_rcArea.right = pDlg->GetPos().x+pDlg->GetSize().cx;
	m_rcArea.top = pDlg->GetPos().y;
	m_rcArea.bottom = pDlg->GetPos().y+pDlg->GetSize().cy;
	m_bDealKeyMsg = true;
}