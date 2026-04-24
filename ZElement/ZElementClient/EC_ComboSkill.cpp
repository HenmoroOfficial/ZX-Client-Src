/*
 * FILE: EC_ComboSkill.cpp
 *
 * DESCRIPTION: 
 *
 * CREATED BY: Duyuxin, 2005/12/26
 *
 * HISTORY: 
 *
 * Copyright (c) 2005 Archosaur Studio, All Rights Reserved.
 */

#include "EC_Global.h"
#include "EC_ComboSkill.h"
#include "EC_Game.h"
#include "EC_HostPlayer.h"

#define new A_DEBUG_NEW

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
//	Implement CECComboSkill
//	
///////////////////////////////////////////////////////////////////////////

CECComboSkill::CECComboSkill()
{
	m_pHost			= NULL;
	m_iGroup		= -1;
	m_iCursor		= 0;
	m_bStop			= false;
	m_iLoopStart	= -1;
	m_idTarget		= 0;
	m_bForceAtk		= false;

	memset(&m_cs, 0, sizeof (m_cs));
}

//	Initialize object
bool CECComboSkill::Init(CECHostPlayer* pHost, int iGroup, int idTarget, 
					bool bForceAttack)
{
	if (iGroup < 0 || iGroup >= EC_COMBOSKILL_NUM)
	{
		ASSERT(0);
		return false;
	}

	m_pHost		= pHost;
	m_iGroup	= iGroup;
	m_iCursor	= 0;
	m_bStop		= false;
	m_idTarget	= idTarget;
	m_bForceAtk	= bForceAttack;

	CECConfigs* pCfg = g_pGame->GetConfigs();
	m_cs = pCfg->GetVideoSettings().comboSkill[iGroup];

	//	Find the last loop start flag
	int i;
	m_iLoopStart = -1;
	for (i=0; i < EC_COMBOSKILL_LEN; i++)
	{
		if (m_cs.idSkill[i] == SID_LOOPSTART)
			m_iLoopStart = i;
	}

	//	Ensure there is valid skill id after loop start flag
	if (m_iLoopStart >= 0)
	{
		for (i=m_iLoopStart+1; i < EC_COMBOSKILL_LEN; i++)
		{
			int id = m_cs.idSkill[i];
			if (id > 0 || id == SID_ATTACK)
				break;
		}

		//	If no valid skill after loop start flag is found, 
		//	disable this loop start flag
		if (i >= EC_COMBOSKILL_LEN)
			m_iLoopStart = -1;
	}

	//	Move cursor to proper position
	StepCursor(true);

	return true;
}

//	Continue combo skill
//	Return true if combo skill will continues, false if combo skill stops
//	bMeleeing: true, player is being meleeing
bool CECComboSkill::Continue(bool bMeleeing)
{
	if (m_bStop || !m_cs.nIcon || !m_pHost ||
		m_iCursor < 0 || m_iCursor >= EC_COMBOSKILL_LEN)
		return false;

	int idSkill = GetNextSkill();
	if (idSkill > 0)
	{
		if (!m_pHost->ApplySkillShortcut(idSkill, true, m_idTarget, m_bForceAtk ? 1 : 0))
			return false;
	}
	else if (idSkill == SID_ATTACK)
	{
		if (!bMeleeing)
			m_pHost->CmdNormalAttack(false, true, m_idTarget, m_bForceAtk ? 1 : 0);
	}
	else
	{
		ASSERT(0);
		return false;
	}

	//	Forward cursor
	StepCursor(false);

	return true;
}

//	Step cursor
void CECComboSkill::StepCursor(bool bFirst)
{
	if (bFirst)
		m_iCursor = -1;

	while (1)
	{
		m_iCursor++;

		if (m_iCursor >= EC_COMBOSKILL_LEN)
		{
			//	If we has loop flag, jump to it
			if (m_iLoopStart >= 0)
				m_iCursor = m_iLoopStart;
			else
			{
				m_bStop = true;
				return;
			}
		}
		else
		{
			int id = m_cs.idSkill[m_iCursor];
			if (id > 0 || id == SID_ATTACK)
				break;
		}
	}
}

//	Get id of skill next Continue() will trigger
int CECComboSkill::GetNextSkill()
{
	if (m_bStop || m_iCursor < 0 || m_iCursor >= EC_COMBOSKILL_LEN)
		return 0;

	return m_cs.idSkill[m_iCursor];
}

//	Replace skill id
void CECComboSkill::ReplaceSkillID(int idOld, int idNew)
{
	for (int i=0; i < EC_COMBOSKILL_LEN; i++)
	{
		if (m_cs.idSkill[i] == (short)idOld)
			m_cs.idSkill[i] = (short)idNew;
	}
}
