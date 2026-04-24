/********************************************************************
	created:	2012/8/17
	author:		fuchonggang
	
	purpose:	图形帮助系统-诛仙任务
	Copyright (C) 2012 - All Rights Reserved
*********************************************************************/

#include "EC_HelpStep11TaskZhuxian.h"
#include "EC_HelpGraphGuide.h"
#include "EC_Global.h"
#include "EC_Game.h"
#include "EC_GameRun.h"
#include "EC_UIManager.h"
#include "EC_GameUIMan.h"
#include "AUI\\AUIDialog.h"


CECHelpStep11TaskZhuxian::CECHelpStep11TaskZhuxian()
{
	m_iStartIndex	= CECHelpGraphGuide::HELP2012_11_1_TASK_UI;
	m_iEndIndex		= CECHelpGraphGuide::HELP2012_11_5_TASK_DELIVER;
}

CECHelpStep11TaskZhuxian::~CECHelpStep11TaskZhuxian()
{
}

void CECHelpStep11TaskZhuxian::Help0(bool bStart)
{
	PAUIDIALOG pDlg = g_pGame->GetGameRun()->GetUIManager()->GetInGameUIMan()->GetDialog("Win_Main");
	pDlg->GetDlgItem("Shape_2")->Show(bStart);
	pDlg->GetDlgItem("Shape_2")->SetFlash(bStart);
	pDlg->GetDlgItem("Txt_2")->Show(bStart);
	pDlg->GetDlgItem("Txt_2")->SetFlash(bStart);
	pDlg->GetDlgItem("Img_2")->Show(bStart);
	pDlg->GetDlgItem("Img_2")->SetFlash(bStart);
	pDlg->SetCanMove(!bStart);
	m_rcArea.left = pDlg->GetPos().x;
	m_rcArea.right = pDlg->GetPos().x+pDlg->GetSize().cx;
	m_rcArea.top = pDlg->GetPos().y;
	m_rcArea.bottom = pDlg->GetPos().y+pDlg->GetSize().cy;
	m_bDealKeyMsg = false;
}

void CECHelpStep11TaskZhuxian::Help1(bool bStart)
{
	PAUIDIALOG pDlg = g_pGame->GetGameRun()->GetUIManager()->GetInGameUIMan()->GetDialog("Win_Quest");
	pDlg->GetDlgItem("Rdo_4")->SetFlash(bStart);
	pDlg->SetCanMove(!bStart);
	m_rcArea.left = pDlg->GetPos().x;
	m_rcArea.right = pDlg->GetPos().x+pDlg->GetSize().cx;
	m_rcArea.top = pDlg->GetPos().y;
	m_rcArea.bottom = pDlg->GetPos().y+pDlg->GetSize().cy;
	m_bDealKeyMsg = false;
}

void CECHelpStep11TaskZhuxian::Help2(bool bStart)
{
	PAUIDIALOG pDlg = g_pGame->GetGameRun()->GetUIManager()->GetInGameUIMan()->GetDialog("Win_Quest_Main");
	pDlg->GetDlgItem("Rdo_Book1")->SetFlash(bStart);
	pDlg->SetCanMove(!bStart);
	m_rcArea.left = pDlg->GetPos().x;
	m_rcArea.right = pDlg->GetPos().x+pDlg->GetSize().cx;
	m_rcArea.top = pDlg->GetPos().y;
	m_rcArea.bottom = pDlg->GetPos().y+pDlg->GetSize().cy;
	m_bDealKeyMsg = false;
}

void CECHelpStep11TaskZhuxian::Help3(bool bStart)
{
	PAUIDIALOG pDlg = g_pGame->GetGameRun()->GetUIManager()->GetInGameUIMan()->GetDialog("Win_Quest");
	pDlg->GetDlgItem("Shape_1")->Show(bStart);
	pDlg->GetDlgItem("Shape_1")->SetFlash(bStart);
	pDlg->GetDlgItem("Txt_1")->Show(bStart);
	pDlg->GetDlgItem("Txt_1")->SetFlash(bStart);
	pDlg->GetDlgItem("Img_1")->Show(bStart);
	pDlg->GetDlgItem("Img_1")->SetFlash(bStart);
	pDlg->SetCanMove(!bStart);
	m_rcArea.left = pDlg->GetPos().x;
	m_rcArea.right = pDlg->GetPos().x+pDlg->GetSize().cx;
	m_rcArea.top = pDlg->GetPos().y;
	m_rcArea.bottom = pDlg->GetPos().y+pDlg->GetSize().cy;
	m_bDealKeyMsg = false;
}

void CECHelpStep11TaskZhuxian::Help4(bool bStart)
{
	PAUIDIALOG pDlg = g_pGame->GetGameRun()->GetUIManager()->GetInGameUIMan()->GetDialog("Win_Quest");
	pDlg->GetDlgItem("Btn_Accept")->Show(bStart);
	pDlg->SetCanMove(!bStart);
	m_rcArea.left = pDlg->GetPos().x;
	m_rcArea.right = pDlg->GetPos().x+pDlg->GetSize().cx;
	m_rcArea.top = pDlg->GetPos().y;
	m_rcArea.bottom = pDlg->GetPos().y+pDlg->GetSize().cy;
	m_bDealKeyMsg = false;
}