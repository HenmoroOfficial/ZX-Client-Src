/*
 * FILE: DlgDungeonStepDetail.cpp
 *
 * DESCRIPTION: 
 *
 * CREATED BY: libin, 2013/10/08
 *
 * HISTORY: 
 *
 * Copyright (c) 2004 Archosaur Studio, All Rights Reserved.
 */

#include "AUI//AUISubDialog.h"
#include "EC_Game.h"
#include "EC_Global.h"
#include "EC_GameUIMan.h"
#include "EC_BattleDungeon.h"
#include "DlgDungeonStepDetail.h"

///////////////////////////////////////////////////////////////////////////
//	
//	Define and Macro
//	
///////////////////////////////////////////////////////////////////////////

///////////////////////////////////////////////////////////////////////////
//	
//	Reference to External variables and functions
//	
///////////////////////////////////////////////////////////////////////////

///////////////////////////////////////////////////////////////////////////
//	
//	Local Types and Variables and Global variables
//	
///////////////////////////////////////////////////////////////////////////

///////////////////////////////////////////////////////////////////////////
//	
//	Local functions
//	
///////////////////////////////////////////////////////////////////////////

///////////////////////////////////////////////////////////////////////////
//	
//	Implement CDlgDungeonStepDetail
//	
///////////////////////////////////////////////////////////////////////////

AUI_BEGIN_EVENT_MAP(CDlgDungeonStepDetail, CDlgBase)
AUI_ON_EVENT("",	WM_MOUSEWHEEL,		OnEventMouseWheel)
AUI_ON_EVENT("*",	WM_MOUSEWHEEL,		OnEventMouseWheel)
AUI_END_EVENT_MAP();

CDlgDungeonStepDetail::CDlgDungeonStepDetail()
{
}

CDlgDungeonStepDetail::~CDlgDungeonStepDetail()
{
}

void CDlgDungeonStepDetail::OnEventMouseWheel(WPARAM wParam, LPARAM lParam, AUIObject *pObj)
{
	const AArray<CECGame::DUNGEONSTEPINFO>* stepInfo = g_pGame->GetDungeonStepInfo();
	PAUIDIALOG pDlgDetail = GetGameUIMan()->GetDialog("Win_Battle_Dungeon_Step_Detail");
	for (int i = 0; i < stepInfo->GetSize(); ++ i )
	{
		PAUISUBDIALOG subDlg = (PAUISUBDIALOG) pDlgDetail->GetDlgItem((*stepInfo)[i].szSubWndName);
		if (subDlg == NULL || subDlg->GetSubDialog() != this)
			continue;
		
		subDlg->OnDlgItemMessage(WM_MOUSEWHEEL, wParam, lParam);
	}
}