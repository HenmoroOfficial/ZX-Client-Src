/********************************************************************
	created:	2012/8/17
	author:		fuchonggang
	
	purpose:	图形帮助系统-法宝篇
	Copyright (C) 2012 - All Rights Reserved
*********************************************************************/

#include "EC_HelpStep07Talisman.h"
#include "EC_HelpGraphGuide.h"
#include "EC_Global.h"
#include "EC_Game.h"
#include "EC_GameRun.h"
#include "EC_UIManager.h"
#include "EC_GameUIMan.h"
#include "DlgInventory.h"
#include "AUI\\AUIDialog.h"


CECHelpStep07Talisman::CECHelpStep07Talisman()
{
	m_iStartIndex	= CECHelpGraphGuide::HELP2012_7_1_TALISMAN_INV;
	m_iEndIndex		= CECHelpGraphGuide::HELP2012_7_5_TALISMAN_START;

	m_vecTalismans.push_back(4703);
	m_vecTalismans.push_back(30076);
}

CECHelpStep07Talisman::~CECHelpStep07Talisman()
{
}

void CECHelpStep07Talisman::UseItem(int id, int index)
{
	if (index==CECHelpGraphGuide::HELP2012_7_2_TALISMAN_HINT)
	{
		unsigned int i;
		for (i=0; i<m_vecTalismans.size(); ++i)
		{
			if (id==m_vecTalismans[i])
			{
				g_pGame->GetGameRun()->GetHelpGraphGuide()->FinishHelp(index);	
			}
		}
	}
}

void CECHelpStep07Talisman::Help0(bool bStart)
{
	PAUIDIALOG pDlg = g_pGame->GetGameRun()->GetUIManager()->GetInGameUIMan()->GetDialog("Win_Main");
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

void CECHelpStep07Talisman::Help1(bool bStart)
{
	CDlgInventory *pDlg = (CDlgInventory*)g_pGame->GetGameRun()->GetUIManager()->GetInGameUIMan()->GetDialog("Win_Inventory");
	if (bStart)
	{
		unsigned int i;
		for (i=0; i<m_vecTalismans.size(); ++i)
		{
			if (pDlg->SetHelpItem(m_vecTalismans[i]))
			{
				break;
			}
		}
	}
	else
		pDlg->SetHelpItem(-1);
	pDlg->SetCanMove(!bStart);
	m_rcArea.left = pDlg->GetPos().x;
	m_rcArea.right = pDlg->GetPos().x+pDlg->GetSize().cx;
	m_rcArea.top = pDlg->GetPos().y;
	m_rcArea.bottom = pDlg->GetPos().y+pDlg->GetSize().cy;
	m_bDealKeyMsg = false;
}

void CECHelpStep07Talisman::Help2(bool bStart)
{
	CECGameUIMan *pGameUIMan = g_pGame->GetGameRun()->GetUIManager()->GetInGameUIMan();
	PAUIDIALOG pDlg = pGameUIMan->GetDialog("Win_NewerHelp_ImgPop");
	AUIOBJECT_SETPROPERTY p;
	strcpy(p.fn, "help\\NewerHelp_TalismanDrag.dds");
	pDlg->SetProperty("Frame Image", &p);
	pDlg->Show(bStart);	
	pDlg->SetCanMove(!bStart);
	m_rcArea.left = pDlg->GetPos().x;
	m_rcArea.right = pDlg->GetPos().x+pDlg->GetSize().cx;
	m_rcArea.top = pDlg->GetPos().y;
	m_rcArea.bottom = pDlg->GetPos().y+pDlg->GetSize().cy;
	m_bDealKeyMsg = false;
}

void CECHelpStep07Talisman::Help3(bool bStart)
{
	PAUIDIALOG pDlg = g_pGame->GetGameRun()->GetUIManager()->GetInGameUIMan()->GetDialog("Win_Talisman");
	pDlg->GetDlgItem("Img_1")->Show(bStart);
	pDlg->GetDlgItem("Img_1")->SetFlash(bStart);
	pDlg->GetDlgItem("Txt_1")->Show(bStart);
	pDlg->GetDlgItem("Txt_1")->SetFlash(bStart);
	pDlg->GetDlgItem("Shape_1")->Show(bStart);
	pDlg->GetDlgItem("Shape_1")->SetFlash(bStart);	
	pDlg->SetCanMove(!bStart);
	m_rcArea.left = pDlg->GetPos().x;
	m_rcArea.right = pDlg->GetPos().x+pDlg->GetSize().cx;
	m_rcArea.top = pDlg->GetPos().y;
	m_rcArea.bottom = pDlg->GetPos().y+pDlg->GetSize().cy;
	m_bDealKeyMsg = false;
}