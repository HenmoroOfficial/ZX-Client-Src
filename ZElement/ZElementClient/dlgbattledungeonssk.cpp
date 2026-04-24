/*
 * FILE: DlgBattleDungeonSsk.cpp
 *
 * DESCRIPTION: 
 *
 * CREATED BY: libin, 2013/09/02
 *
 * HISTORY: 
 *
 * Copyright (c) 2004 Archosaur Studio, All Rights Reserved.
 */

#include "AFI.h"
#include "EC_Game.h"
#include "EC_GameUIMan.h"
#include "EC_IvtrConsume.h"
#include "EC_GameUIMisc.h"
#include "EC_BattleDungeon.h"
#include "DlgBase.h"
#include "DlgBattleDungeonSsk.h"

///////////////////////////////////////////////////////////////////////////
//	
//	Define and Macro
//	
///////////////////////////////////////////////////////////////////////////

#define new A_DEBUG_NEW

AUI_BEGIN_COMMAND_MAP(CDlgBattleDungeonSsk, CDlgBase)
AUI_END_COMMAND_MAP()

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
//	Implement DlgBattleDungeonSsk
//	
///////////////////////////////////////////////////////////////////////////

CDlgBattleDungeonSsk::CDlgBattleDungeonSsk()
{
	m_pLab_Tower = NULL;
	m_pLab_Monster = NULL;
	m_pLab_Time = NULL;
	m_pProg_Ssk = NULL;
	m_pImg_Award = NULL;

	m_dwAwardId = -1;
}

CDlgBattleDungeonSsk::~CDlgBattleDungeonSsk()
{	
}

void CDlgBattleDungeonSsk::UpdateInfo(char tower, char monster, char awardindex, DWORD awardid, int score)
{
	ACString labelTxt;
	labelTxt.Format(GetGameUIMan()->GetStringFromTable(20191), monster);
	m_pLab_Monster->SetText(labelTxt);

	labelTxt.Format(GetGameUIMan()->GetStringFromTable(20192), tower);
	m_pLab_Tower->SetText(labelTxt);

	m_pImg_Award->FixFrame(awardindex + 1);

	const float totalScore = 400.0f;
	m_pProg_Ssk->SetProgress(int (score / totalScore * 100));

	if (m_dwAwardId != awardid)
	{
		m_dwAwardId = awardid;
		CECIvtrItem *pItem = CECIvtrItem::CreateItem(m_dwAwardId, 0, 1);
		if (pItem)
		{
			pItem->GetDetailDataFromLocal();
			AUICTranslate trans;
			AString strFile;
			m_pImg_Award->SetHint(trans.Translate(pItem->GetDesc()));
			delete pItem;
		}
	}	
}

bool CDlgBattleDungeonSsk::OnInitDialog()
{
	DDX_Control("Txt_02", m_pLab_Tower);
	DDX_Control("Txt_01", m_pLab_Monster);
	DDX_Control("Txt_Time", m_pLab_Time);
	DDX_Control("Prgs_01", m_pProg_Ssk);
	DDX_Control("Img_Award", m_pImg_Award);

	return true;
}

void CDlgBattleDungeonSsk::OnTick()
{
	ACString strTime = GetTimeText(CECBattleDungeonMan::getSingletonPtr()->m_iEndTime-GetGame()->GetServerGMTTime());
	m_pLab_Time->SetText(strTime);
}
