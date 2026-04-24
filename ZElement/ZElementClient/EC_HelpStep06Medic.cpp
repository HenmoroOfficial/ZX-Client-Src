/********************************************************************
	created:	2012/8/17
	author:		fuchonggang
	
	purpose:	图形帮助系统-药品篇
	Copyright (C) 2012 - All Rights Reserved
*********************************************************************/

#include "EC_HelpStep06Medic.h"
#include "EC_HelpGraphGuide.h"
#include "EC_Global.h"
#include "EC_Game.h"
#include "EC_GameRun.h"
#include "EC_UIManager.h"
#include "EC_GameUIMan.h"
#include "DlgInventory.h"
#include "AUI\\AUIDialog.h"


CECHelpStep06Medic::CECHelpStep06Medic()
{
	m_iStartIndex	= CECHelpGraphGuide::HELP2012_6_1_MEDICINE_INV;
	m_iEndIndex		= CECHelpGraphGuide::HELP2012_6_2_MEDICINE_HINT;

	m_vecMedics.push_back(3999);
}

CECHelpStep06Medic::~CECHelpStep06Medic()
{
}

void CECHelpStep06Medic::UseItem(int id, int index)
{
	if (index==CECHelpGraphGuide::HELP2012_6_2_MEDICINE_HINT)
	{
		unsigned int i;
		for (i=0; i<m_vecMedics.size(); ++i)
		{
			if (id==m_vecMedics[i])
			{
				g_pGame->GetGameRun()->GetHelpGraphGuide()->FinishHelp(index);	
			}
		}
	}
}

void CECHelpStep06Medic::Help0(bool bStart)
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

void CECHelpStep06Medic::Help1(bool bStart)
{
	CDlgInventory *pDlg = (CDlgInventory*)g_pGame->GetGameRun()->GetUIManager()->GetInGameUIMan()->GetDialog("Win_Inventory");
	if (bStart)
	{
		unsigned int i;
		for (i=0; i<m_vecMedics.size(); ++i)
		{
			if (pDlg->SetHelpItem(m_vecMedics[i]))
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