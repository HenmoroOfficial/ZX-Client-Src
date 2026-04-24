/********************************************************************
	created:	2012/8/17
	author:		fuchonggang
	
	purpose:	图形帮助系统-任务篇
	Copyright (C) 2012 - All Rights Reserved
*********************************************************************/

#include "EC_HelpStep03Task.h"
#include "EC_HelpGraphGuide.h"
#include "EC_Global.h"
#include "EC_Game.h"
#include "EC_GameRun.h"
#include "EC_UIManager.h"
#include "EC_GameUIMan.h"
#include "AUI\\AUIDialog.h"


CECHelpStep03Task::CECHelpStep03Task()
{
	m_iStartIndex	= CECHelpGraphGuide::HELP2012_3_1_TRACK;
	m_iEndIndex		= CECHelpGraphGuide::HELP2012_3_5_TASK_LIST;
}

CECHelpStep03Task::~CECHelpStep03Task()
{
}

void CECHelpStep03Task::Help0(bool bStart)
{
	PAUIDIALOG pDlg = g_pGame->GetGameRun()->GetUIManager()->GetInGameUIMan()->GetDialog("Win_QuestMinion");
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

void CECHelpStep03Task::Help1(bool bStart)
{
	PAUIDIALOG pDlg = g_pGame->GetGameRun()->GetUIManager()->GetInGameUIMan()->GetDialog("Win_NPC");
	pDlg->GetDlgItem("Shape_1")->Show(bStart);
	pDlg->GetDlgItem("Shape_1")->SetFlash(bStart);
	pDlg->GetDlgItem("Txt_2")->Show(bStart);
	pDlg->GetDlgItem("Txt_2")->SetFlash(bStart);
	pDlg->GetDlgItem("Img_1")->Show(bStart);
	pDlg->GetDlgItem("Img_1")->SetFlash(bStart);
	pDlg->SetCanMove(!bStart);
	m_rcArea.left = pDlg->GetPos().x;
	m_rcArea.right = pDlg->GetPos().x+pDlg->GetSize().cx;
	m_rcArea.top = pDlg->GetPos().y;
	m_rcArea.bottom = pDlg->GetPos().y+pDlg->GetSize().cy;
	m_bDealKeyMsg = false;

}

void CECHelpStep03Task::Help2(bool bStart)
{
	PAUIDIALOG pDlg = g_pGame->GetGameRun()->GetUIManager()->GetInGameUIMan()->GetDialog("Win_NPC");
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

void CECHelpStep03Task::Help3(bool bStart)
{
	PAUIDIALOG pDlg = g_pGame->GetGameRun()->GetUIManager()->GetInGameUIMan()->GetDialog("Win_Main");
	if (!pDlg->IsShow())
	{
		pDlg->Show(true);
	}
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

void CECHelpStep03Task::Help4(bool bStart)
{
	PAUIDIALOG pDlg = g_pGame->GetGameRun()->GetUIManager()->GetInGameUIMan()->GetDialog("Win_Quest");
	if (!pDlg->IsShow())
	{
		pDlg->Show(true);
	}
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