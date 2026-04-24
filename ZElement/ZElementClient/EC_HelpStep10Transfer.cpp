/********************************************************************
	created:	2012/8/17
	author:		fuchonggang
	
	purpose:	图形帮助系统-飞天服务
	Copyright (C) 2012 - All Rights Reserved
*********************************************************************/

#include "EC_HelpStep10Transfer.h"
#include "EC_HelpGraphGuide.h"
#include "EC_Global.h"
#include "EC_Game.h"
#include "EC_GameRun.h"
#include "EC_UIManager.h"
#include "EC_GameUIMan.h"
#include "AUI\\AUIDialog.h"


CECHelpStep010Transfer::CECHelpStep010Transfer()
{
	m_iStartIndex	= CECHelpGraphGuide::HELP2012_10_1_TRANSFER;
	m_iEndIndex		= CECHelpGraphGuide::HELP2012_10_2_TRANSFER_DESTPOS;
}

CECHelpStep010Transfer::~CECHelpStep010Transfer()
{
}

void CECHelpStep010Transfer::Help0(bool bStart)
{
	CECGameUIMan *pGameUIMan = g_pGame->GetGameRun()->GetUIManager()->GetInGameUIMan();
	pGameUIMan->GetDialog("Win_MidMap")->GetDlgItem("Shape_1")->Show(bStart);
	pGameUIMan->GetDialog("Win_MidMap")->GetDlgItem("Shape_1")->SetFlash(bStart);
	pGameUIMan->GetDialog("Win_MidMap")->GetDlgItem("Txt_1")->Show(bStart);
	pGameUIMan->GetDialog("Win_MidMap")->GetDlgItem("Txt_1")->SetFlash(bStart);
	pGameUIMan->GetDialog("Win_MidMap")->GetDlgItem("Img_1")->Show(bStart);
	pGameUIMan->GetDialog("Win_MidMap")->GetDlgItem("Img_1")->SetFlash(bStart);
}

void CECHelpStep010Transfer::Help1(bool bStart)
{
	CECGameUIMan *pGameUIMan = g_pGame->GetGameRun()->GetUIManager()->GetInGameUIMan();
	pGameUIMan->GetDialog("Win_MidMap")->GetDlgItem("Shape_1")->Show(bStart);
	pGameUIMan->GetDialog("Win_MidMap")->GetDlgItem("Shape_1")->SetFlash(bStart);
	pGameUIMan->GetDialog("Win_MidMap")->GetDlgItem("Txt_1")->Show(bStart);
	pGameUIMan->GetDialog("Win_MidMap")->GetDlgItem("Txt_1")->SetFlash(bStart);
	pGameUIMan->GetDialog("Win_MidMap")->GetDlgItem("Img_1")->Show(bStart);
	pGameUIMan->GetDialog("Win_MidMap")->GetDlgItem("Img_1")->SetFlash(bStart);
}