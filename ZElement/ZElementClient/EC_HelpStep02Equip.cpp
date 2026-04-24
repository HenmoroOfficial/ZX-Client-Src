/********************************************************************
	created:	2012/8/17
	author:		fuchonggang
	
	purpose:	图形帮助系统-装备使用篇
	Copyright (C) 2012 - All Rights Reserved
*********************************************************************/

#include "EC_HelpStep02Equip.h"
#include "EC_HelpGraphGuide.h"
#include "EC_Global.h"
#include "EC_Game.h"
#include "EC_GameRun.h"
#include "EC_UIManager.h"
#include "EC_GameUIMan.h"
#include "DlgInventory.h"
#include "AUI\\AUIDialog.h"


CECHelpStep02Equip::CECHelpStep02Equip()
{
	m_iStartIndex	= CECHelpGraphGuide::HELP2012_2_1_PACKAGE;
	m_iEndIndex		= CECHelpGraphGuide::HELP2012_2_3_WEAPON;

	m_vecTreasureChests.push_back(24346);
	m_vecTreasureChests.push_back(37881);
	m_vecTreasureChests.push_back(49423);
	m_vecTreasureChests.push_back(31884);
	m_vecTreasureChests.push_back(37882);
	m_vecTreasureChests.push_back(31885);

	m_vecWeapons.push_back(23864);
	m_vecWeapons.push_back(37870);
	m_vecWeapons.push_back(49417);
	m_vecWeapons.push_back(31882);
	m_vecWeapons.push_back(37871);
	m_vecWeapons.push_back(31883);

	m_iIdIndex = 0;
}

CECHelpStep02Equip::~CECHelpStep02Equip()
{
}

void CECHelpStep02Equip::UseItem(int id, int index)
{
	if (index==CECHelpGraphGuide::HELP2012_2_2_TREASURE_CHEST)
	{
		unsigned int i;
		for (i=0; i<m_vecTreasureChests.size(); ++i)
		{
			if (id==m_vecTreasureChests[i])
			{
				g_pGame->GetGameRun()->GetHelpGraphGuide()->FinishHelp(index);
				break;
			}
		}
	}
	else if (index==CECHelpGraphGuide::HELP2012_2_3_WEAPON)
	{
		unsigned int i;
		for (i=0; i<m_vecWeapons.size(); ++i)
		{
			if (id==m_vecWeapons[i])
			{
				g_pGame->GetGameRun()->GetHelpGraphGuide()->FinishHelp(index);
				break;
			}
		}
	}
}

void CECHelpStep02Equip::Help0(bool bStart)
{
	PAUIDIALOG pDlg =g_pGame->GetGameRun()->GetUIManager()->GetInGameUIMan()->GetDialog("Win_Main");
	if(!pDlg->IsShow())
		pDlg->Show(true);
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

void CECHelpStep02Equip::Help1(bool bStart)
{
	CDlgInventory *pDlg = (CDlgInventory*)g_pGame->GetGameRun()->GetUIManager()->GetInGameUIMan()->GetDialog("Win_Inventory");
	if (bStart)
	{
		if(!pDlg->IsShow())
			pDlg->Show(true);
		unsigned int i;
		for (i=0; i<m_vecTreasureChests.size(); ++i)
		{
			if (pDlg->SetHelpItem(m_vecTreasureChests[i]))
			{
				m_iIdIndex = i;
				break;
			}
		}
	}
//	else
	//		pDlg->SetHelpItem(-1);
	pDlg->SetCanMove(!bStart);
	m_rcArea.left = pDlg->GetPos().x;
	m_rcArea.right = pDlg->GetPos().x+pDlg->GetSize().cx;
	m_rcArea.top = pDlg->GetPos().y;
	m_rcArea.bottom = pDlg->GetPos().y+pDlg->GetSize().cy;
	m_bDealKeyMsg = false;
}

void CECHelpStep02Equip::Help2(bool bStart)
{
	CDlgInventory *pDlg = (CDlgInventory*)g_pGame->GetGameRun()->GetUIManager()->GetInGameUIMan()->GetDialog("Win_Inventory");
	if (bStart)
	{
		if(!pDlg->IsShow())
			pDlg->Show(true);
		pDlg->SetHelpItem(m_vecWeapons[m_iIdIndex]);
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