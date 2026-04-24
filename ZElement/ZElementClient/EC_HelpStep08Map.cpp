/********************************************************************
	created:	2012/8/17
	author:		fuchonggang
	
	purpose:	图形帮助系统-地图篇
	Copyright (C) 2012 - All Rights Reserved
*********************************************************************/

#include "EC_HelpStep08Map.h"
#include "EC_HelpGraphGuide.h"
#include "EC_Global.h"
#include "EC_Game.h"
#include "EC_GameRun.h"
#include "EC_UIManager.h"
#include "EC_GameUIMan.h"
#include "AUI\\AUIDialog.h"


CECHelpStep08Map::CECHelpStep08Map()
{
	m_iStartIndex	= CECHelpGraphGuide::HELP2012_8_1_MAP_HOTKEY;
	m_iEndIndex		= CECHelpGraphGuide::HELP2012_8_2_MAP_NPCMINE_LIST;
}

CECHelpStep08Map::~CECHelpStep08Map()
{
}

void CECHelpStep08Map::Help0(bool bStart)
{
	PAUIDIALOG pDlg = g_pGame->GetGameRun()->GetUIManager()->GetInGameUIMan()->GetDialog("Win_MiniMap");
	if(NULL != pDlg)
	{
		if(NULL != pDlg->GetDlgItem("Shape_1"))
		{
		   pDlg->GetDlgItem("Shape_1")->Show(bStart);
		   pDlg->GetDlgItem("Shape_1")->SetFlash(bStart);
		}
		if(NULL != pDlg->GetDlgItem("Txt_1"))
		{
		   pDlg->GetDlgItem("Txt_1")->Show(bStart);
		   pDlg->GetDlgItem("Txt_1")->SetFlash(bStart);
		}
		if(NULL != pDlg->GetDlgItem("Img_1"))
		{
		   pDlg->GetDlgItem("Img_1")->Show(bStart);
		   pDlg->GetDlgItem("Img_1")->SetFlash(bStart);
		}
		pDlg->SetCanMove(!bStart);
		m_rcArea.left = pDlg->GetPos().x;
		m_rcArea.right = pDlg->GetPos().x+pDlg->GetSize().cx;
		m_rcArea.top = pDlg->GetPos().y;
		m_rcArea.bottom = pDlg->GetPos().y+pDlg->GetSize().cy;

	}
	m_bDealKeyMsg = false;
}

void CECHelpStep08Map::Help1(bool bStart)
{
	PAUIDIALOG pDlg = g_pGame->GetGameRun()->GetUIManager()->GetInGameUIMan()->GetDialog("Win_MidMap");
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