/********************************************************************
	created:	2012/8/17
	author:		fuchonggang
	
	purpose:	图形帮助系统-技能篇
	Copyright (C) 2012 - All Rights Reserved
*********************************************************************/

#include "EC_HelpStep09Skill.h"
#include "EC_HelpGraphGuide.h"
#include "EC_Global.h"
#include "EC_Game.h"
#include "EC_GameRun.h"
#include "EC_HostPlayer.h"
#include "EC_UIManager.h"
#include "EC_GameUIMan.h"
#include "AUI\\AUIDialog.h"


CECHelpStep09Skill::CECHelpStep09Skill()
{
	m_iStartIndex	= CECHelpGraphGuide::HELP2012_9_1_SKILL_ICON;
	m_iEndIndex		= CECHelpGraphGuide::HELP2012_9_3_SKILL_SHORTCUT;
}

CECHelpStep09Skill::~CECHelpStep09Skill()
{
}

void CECHelpStep09Skill::Help0(bool bStart)
{
	PAUIDIALOG pDlg = g_pGame->GetGameRun()->GetUIManager()->GetInGameUIMan()->GetDialog("Win_Main");
	pDlg->GetDlgItem("Shape_3")->Show(bStart);
	pDlg->GetDlgItem("Shape_3")->SetFlash(bStart);
	pDlg->GetDlgItem("Txt_3")->Show(bStart);
	pDlg->GetDlgItem("Txt_3")->SetFlash(bStart);
	pDlg->GetDlgItem("Img_3")->Show(bStart);
	pDlg->GetDlgItem("Img_3")->SetFlash(bStart);
	pDlg->SetCanMove(!bStart);
	m_rcArea.left = pDlg->GetPos().x;
	m_rcArea.right = pDlg->GetPos().x+pDlg->GetSize().cx;
	m_rcArea.top = pDlg->GetPos().y;
	m_rcArea.bottom = pDlg->GetPos().y+pDlg->GetSize().cy;
	m_bDealKeyMsg = false;
}

void CECHelpStep09Skill::Help1(bool bStart)
{
	int idProf = g_pGame->GetGameRun()->GetHostPlayer()->GetProfession();
	AString strName;
	strName.Format("Win_Skill%02d", idProf);
	PAUIDIALOG pDlg = g_pGame->GetGameRun()->GetUIManager()->GetInGameUIMan()->GetDialog(strName);
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

void CECHelpStep09Skill::Help2(bool bStart)
{
	CECGameUIMan *pGameUIMan = g_pGame->GetGameRun()->GetUIManager()->GetInGameUIMan();
	PAUIDIALOG pDlg = pGameUIMan->GetDialog("Win_NewerHelp_ImgPop");
	AUIOBJECT_SETPROPERTY p;
	strcpy(p.fn, "help\\NewerHelp_SkillDrag.dds");
	pDlg->SetProperty("Frame Image", &p);
	pDlg->Show(bStart);
	pDlg->SetCanMove(!bStart);
	m_rcArea.left = pDlg->GetPos().x;
	m_rcArea.right = pDlg->GetPos().x+pDlg->GetSize().cx;
	m_rcArea.top = pDlg->GetPos().y;
	m_rcArea.bottom = pDlg->GetPos().y+pDlg->GetSize().cy;
	m_bDealKeyMsg = false;
}