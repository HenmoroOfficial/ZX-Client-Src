/*
 * FILE: DlgBattleDungeonSsk.h
 *
 * DESCRIPTION: 
 *
 * CREATED BY: libin, 2013/09/02
 *
 * HISTORY: 
 *
 * Copyright (c) 2004 Archosaur Studio, All Rights Reserved.
 */

#pragma once

#include "AUI\\AUILabel.h"
#include "AUI\\AUIProgress.h"
#include "AUI\\AUIImagePicture.h"
#include "AUI\\AUICTranslate.h"

///////////////////////////////////////////////////////////////////////////
//	
//	Class CDlgBattleDungeonSsk
//	
///////////////////////////////////////////////////////////////////////////

class CDlgBattleDungeonSsk : public CDlgBase
{
	AUI_DECLARE_COMMAND_MAP();

public:		//	Constructor and Destructor
	CDlgBattleDungeonSsk();
	virtual ~CDlgBattleDungeonSsk();

public:		//	Operations
	void UpdateInfo(char tower, char monster, char awardindex, DWORD awardid, int score);

protected:	//	Operations

	virtual bool OnInitDialog();
	virtual void OnTick();

protected:
	PAUILABEL			m_pLab_Tower;
	PAUILABEL			m_pLab_Monster;
	PAUILABEL			m_pLab_Time;
	PAUIPROGRESS		m_pProg_Ssk;
	PAUIIMAGEPICTURE	m_pImg_Award;

	DWORD				m_dwAwardId;
};
