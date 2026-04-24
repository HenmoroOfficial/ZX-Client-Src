/********************************************************************
	created:	2012/8/17
	author:		fuchonggang
	
	purpose:	图形帮助系统-战斗篇
	Copyright (C) 2012 - All Rights Reserved
*********************************************************************/

#include "EC_HelpStep05Attack.h"
#include "EC_HelpGraphGuide.h"
#include "EC_Global.h"
#include "EC_Game.h"
#include "EC_GameRun.h"
#include "EC_UIManager.h"
#include "EC_GameUIMan.h"
#include "AUI\\AUIDialog.h"


CECHelpStep05Attack::CECHelpStep05Attack()
{
	m_iStartIndex	= CECHelpGraphGuide::HELP2012_5_1_ATTACK;
	m_iEndIndex		= CECHelpGraphGuide::HELP2012_5_1_ATTACK;
}

CECHelpStep05Attack::~CECHelpStep05Attack()
{
}

void CECHelpStep05Attack::Help0(bool bStart)
{
	CECGameUIMan *pGameUIMan = g_pGame->GetGameRun()->GetUIManager()->GetInGameUIMan();
	PAUIDIALOG pDlg = pGameUIMan->GetDialog("Win_NewerHelp_ImgPop");
	AUIOBJECT_SETPROPERTY p;
	strcpy(p.fn, "help\\NewerHelp_Combat_1.dds");
	pDlg->SetProperty("Frame Image", &p);
	pDlg->Show(true);
	pDlg->SetCanMove(!bStart);
	m_rcArea.left = pDlg->GetPos().x;
	m_rcArea.right = pDlg->GetPos().x+pDlg->GetSize().cx;
	m_rcArea.top = pDlg->GetPos().y;
	m_rcArea.bottom = pDlg->GetPos().y+pDlg->GetSize().cy;
	m_bDealKeyMsg = false;
}