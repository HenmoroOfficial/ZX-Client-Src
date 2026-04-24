/*
 * FILE: EC_HPWork.cpp
 *
 * DESCRIPTION: 
 *
 * CREATED BY: Duyuxin, 2005/2/6
 *
 * HISTORY: 
 *
 * Copyright (c) 2005 Archosaur Studio, All Rights Reserved.
 */

#include "EC_Global.h"
#include "EC_HPWork.h"
#include "EC_HostPlayer.h"
#include "EC_Game.h"
#include "EC_GameSession.h"

#include "EC_HPWorkUse.h"
#include "EC_HPWorkDead.h"
#include "EC_HPWorkMelee.h"
#include "EC_HPWorkStand.h"
#include "EC_HPWorkTrace.h"
#include "EC_HPWorkFollow.h"
#include "EC_HPWorkAutoMove.h"
#include "EC_HPWorkMove.h"
#include "EC_HPWorkFly.h"
#include "EC_HPWorkFall.h"
#include "EC_HPWorkSit.h"
#include "EC_HPWorkSpell.h"
#include "EC_HPWorkMoveBezier.h"
#include "EC_GameRecord.h"

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
extern game_record gr;

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
//	Implement CECHPWork
//	
///////////////////////////////////////////////////////////////////////////

CECHPWork::CECHPWork(int iWorkID, CECHPWorkMan* pWorkMan) : CECObjectWork(iWorkID)
{
	ASSERT(pWorkMan);
	m_pWorkMan	= pWorkMan;
	m_pHost		= pWorkMan->GetHostPlayer();
}

//	Copy work data
bool CECHPWork::CopyData(CECHPWork* pWork)
{
	ASSERT(pWork && m_iWorkID == pWork->GetWorkID());
	m_bFinished	 = pWork->m_bFinished;
	m_bFirstTick = pWork->m_bFirstTick;
	return true;
}

///////////////////////////////////////////////////////////////////////////
//	
//	Implement CECHPWorkMan
//	
///////////////////////////////////////////////////////////////////////////

CECHPWorkMan::CECHPWorkMan(CECHostPlayer* pHost)
{
	m_pHost		= pHost;
	m_pCurWork	= NULL;
	m_iCurWork	= -1;

	//	Create works
	m_aWorks.SetSize(CECHPWork::NUM_WORK, 10);
	for (int i=0; i < CECHPWork::NUM_WORK; i++)
		m_aWorks[i] = CreateWork(i);

	memset(m_WorkStack, 0, sizeof (m_WorkStack));
	memset(&m_Delayed, 0, sizeof (m_Delayed));
}

CECHPWorkMan::~CECHPWorkMan()
{
	int i;

	for (i=0; i < CECHPWork::NUM_WORK; i++)
		delete m_aWorks[i];
	
	if (m_Delayed.pWork && m_Delayed.bAutoDel)
		delete m_Delayed.pWork;
}

//	Create a work object
CECHPWork* CECHPWorkMan::CreateWork(int idWork)
{
	CECHPWork* pWork = NULL;

	switch (idWork)
	{
	case CECHPWork::WORK_STAND:			pWork = new CECHPWorkStand(this);		break;
	case CECHPWork::WORK_MOVETOPOS:		pWork = new CECHPWorkMove(this);		break;
	case CECHPWork::WORK_TRACEOBJECT:	pWork = new CECHPWorkTrace(this);		break;
	case CECHPWork::WORK_HACKOBJECT:	pWork = new CECHPWorkMelee(this);		break;
	case CECHPWork::WORK_SPELLOBJECT:	pWork = new CECHPWorkSpell(this);		break;
	case CECHPWork::WORK_USEITEM:		pWork = new CECHPWorkUse(this);			break;
	case CECHPWork::WORK_DEAD:			pWork = new CECHPWorkDead(this);		break;
	case CECHPWork::WORK_FOLLOW:		pWork = new CECHPWorkFollow(this);		break;
	case CECHPWork::WORK_AUTOMOVE:		pWork = new CECHPWorkAutoMove(this);	break;
	case CECHPWork::WORK_FLYOFF:		pWork = new CECHPWorkFly(this);			break;
	case CECHPWork::WORK_FREEFALL:		pWork = new CECHPWorkFall(this);		break;
	case CECHPWork::WORK_SIT:			pWork = new CECHPWorkSit(this);			break;
	case CECHPWork::WORK_PICKUP:		pWork = new CECHPWorkPick(this);		break;
	case CECHPWork::WORK_GENERAL_OPRT:	pWork = new CECHPWorkGeneralOprt(this);	break;
	case CECHPWork::WORK_CONCENTRATE:	pWork = new CECHPWorkConcentrate(this);	break;
	case CECHPWork::WORK_REVIVE:		pWork = new CECHPWorkRevive(this);		break;
	case CECHPWork::WORK_FLASHMOVE:		pWork = new CECHPWorkFMove(this);		break;
	case CECHPWork::WORK_BEBOUND:		pWork = new CECHPWorkBeBound(this);		break;
	case CECHPWork::WORK_BE_MOVED:		pWork = new CECHPWorkBeMoved(this);		break; // Added 2011-07-28.
	case CECHPWork::WORK_SPECIAL_CHARGE: pWork = new CECHPWorkSpecialCharge(this); break; //Added 2011-08-01.
	case CECHPWork::WORK_SPECIAL_MOVE:	pWork = new CECHPWorkSpecialMove(this);	break; //Added 2012-06-12.
	case CECHPWork::WORK_BLEW_UP:		pWork = new CECHPWorkBlewUp(this);      break;
	case CECHPWork::WORK_PARABOLIC:		pWork = new CECHPWorkParabolic(this);	break;
	case CECHPWork::WORK_MOVENOCOLLISION: pWork = new CECHPWorkMoveNoCollision(this); break;
	case CECHPWork::WORK_MOVEBEZIEL: pWork = new CECHPWorkBezierMove(this); break;
	default:
		ASSERT(0);
		return NULL;
	}

	return pWork;
}

//	Start work
bool CECHPWorkMan::StartWork(int iPriority, CECHPWork* pWork, bool bAutoDel/* true */,
						bool bNoDelay/* false */)
{
	ASSERT(pWork);
	int iWork = pWork->GetWorkID();

	ASSERT(iPriority >= 0 && iPriority < NUM_PRIORITY);
	ASSERT(iWork >= 0 && iWork < m_aWorks.GetSize());

/*	if (!CanStartWork(iPriority, iWork))
	{
		if (bAutoDel)
			delete pWork;

		return false;
	}
*/
	if (!bNoDelay && DelayWork(iPriority, pWork, bAutoDel))
		return true;

	//	Duplicate work data
//	m_aWorks[iWork]->Reset();
	m_aWorks[iWork]->CopyData(pWork);

	if (bAutoDel)
		delete pWork;

	return InternallyStartWork(iPriority, iWork);
}

//	Start specified work
bool CECHPWorkMan::InternallyStartWork(int iPriority, int iWork)
{
	ASSERT(iPriority >= 0 && iPriority < NUM_PRIORITY);
	ASSERT(iWork >= 0 && iWork < m_aWorks.GetSize());

	if (iWork == CECHPWork::WORK_DEAD)
	{
		//	Dead is a special work. Remove all works whose priority >= 1
		for (int i=1; i < NUM_PRIORITY; i++)
		{
			if (m_WorkStack[i])
			{
				m_WorkStack[i]->Cancel();
				m_WorkStack[i] = NULL;
			}
		}

		//	Delete delayed work too
		if (m_Delayed.pWork)
		{
			if (m_Delayed.bAutoDel)
				delete m_Delayed.pWork;

			m_Delayed.pWork = NULL;
		}

		m_WorkStack[PRIORITY_1] = m_aWorks[iWork];
		m_pCurWork = m_WorkStack[PRIORITY_1];
		m_iCurWork = PRIORITY_1;
	}
	else
	{
		CECHPWork* pOldWork = m_WorkStack[iPriority];
		if (m_WorkStack[iPriority])
			m_WorkStack[iPriority]->Cancel();

		m_WorkStack[iPriority] = m_aWorks[iWork];

		if (iPriority >= m_iCurWork)
		{
			if (m_pCurWork && m_pCurWork != pOldWork)
				m_pCurWork->Cancel();

			m_pCurWork = m_WorkStack[iPriority];
			m_iCurWork = iPriority;
		}
	}

	return true;
}

//	Check whether we can start a work
bool CECHPWorkMan::CanStartWork(int iPriority, int iWork)
{
	if (m_pHost->IsMeleeing())
	{
		switch (iWork)
		{
		case CECHPWork::WORK_MOVETOPOS:
		case CECHPWork::WORK_FOLLOW:
		case CECHPWork::WORK_TRACEOBJECT:

			return false;
		}
	}

	return true;
}

//	Delay specified work ?
bool CECHPWorkMan::DelayWork(int iPriority, CECHPWork* pWork, bool bAutoDel)
{
	ASSERT(pWork);

	//	If there isn't work running, needn't delaying
	if (!m_pCurWork)
		return false;

	bool bDelay = false;

	if (m_pHost->IsMeleeing())
	{
		//	Send cancel fighting request
		g_pGame->GetGameSession()->c2s_CmdCancelAction();
		bDelay = true;
	}
	else
	{
		int idWork = m_pCurWork->GetWorkID();
		switch (idWork)
		{
		case CECHPWork::WORK_PICKUP:
		case CECHPWork::WORK_SPELLOBJECT:
		case CECHPWork::WORK_GENERAL_OPRT:
			
			bDelay = true;
			break;
		}
	}

	if (bDelay)
	{
		//	Release currently delayed work
		if (m_Delayed.pWork && m_Delayed.bAutoDel)
			delete m_Delayed.pWork;

		m_Delayed.iPriority	= iPriority;
		m_Delayed.pWork		= pWork;
		m_Delayed.bAutoDel	= bAutoDel;

		return true;
	}

	return false;
}

//	Start the delayed work
void CECHPWorkMan::StartDelayedWork()
{
	if (m_Delayed.pWork)
	{
		int idWork = m_Delayed.pWork->GetWorkID();
		m_aWorks[idWork]->CopyData(m_Delayed.pWork);

		if (m_Delayed.bAutoDel)
			delete m_Delayed.pWork;
	
		m_Delayed.pWork = NULL;

		InternallyStartWork(m_Delayed.iPriority, idWork);
	}
}

//	Finish current work
void CECHPWorkMan::FinishCurWork(int idWork, bool bCancel)
{
	if (!m_pCurWork)
		return;

	if (idWork >= 0)
	{
		//	Check work ID
		if (!m_pCurWork || m_pCurWork->GetWorkID() != idWork)
		{
		//	ASSERT(0);
			return;
		}
	}

	ASSERT(m_iCurWork >= 0 && m_iCurWork < NUM_PRIORITY);

//	if (bCancel)
		m_pCurWork->Cancel();

	//	Finish current work
	m_WorkStack[m_iCurWork] = NULL;

	DecCurWorkPriority();

	//	If there is a work delayed, start it
	StartDelayedWork();		
}

//	Decrease current work to a lower priority work
void CECHPWorkMan::DecCurWorkPriority()
{
	if (!m_iCurWork)
	{
		m_iCurWork = -1;
		m_pCurWork = NULL;
	}
	else
	{
		for (int i=m_iCurWork-1; i >= 0; i--)
		{
			if (m_WorkStack[i])
			{
				m_iCurWork = i;
				m_pCurWork = m_WorkStack[i];
				m_pCurWork->OnWorkShift();
				break;
			}
		}
	}
}

//	Finish all work
void CECHPWorkMan::FinishAllWork(bool bGotoStand)
{
	for (int i=0; i < NUM_PRIORITY; i++)
		m_WorkStack[i] = NULL;

	m_iCurWork = -1;
	m_pCurWork = NULL;

	//	Delete delayed work too
	if (m_Delayed.pWork && m_Delayed.bAutoDel)
		delete m_Delayed.pWork;

	m_Delayed.pWork = NULL;

	//	Restore to stand state
	if (bGotoStand)
	{
		StartWork_p0(CreateWork(CECHPWork::WORK_STAND));
	}
}

//	Get work by ID
CECHPWork* CECHPWorkMan::GetWorkByID(int id, bool bReset)
{
	CECHPWork* pWork = m_aWorks[id];
	if (bReset)
		pWork->Reset();

	return pWork;
}

//	Adjust host player work status as replying record
void CECHPWorkMan::AdjustHPWork()
{
	A3DVECTOR3 vDelta = gr.get_cur_frame_host_pos() - gr.get_pre_frame_host_pos();
	if(gr.get_seek_adjust() == game_record::enum_seek_start || gr.get_seek_adjust() == game_record::enum_seek_second)
	{
		if(!GetCurWork())   // current work is null
		{
			if(vDelta.IsZero())
			{
				StartWork_p0(CreateWork(CECHPWork::WORK_STAND));
			}
			else
			{
				vDelta.y = 0.0f;
				vDelta.Normalize();
				CECHPWorkMove* pHPWork = (CECHPWorkMove*)CreateWork(CECHPWork::WORK_MOVETOPOS);
				pHPWork->SetDestination(CECHPWorkMove::DEST_DIR, vDelta);
				StartWork_p1(pHPWork);
			}
			gr.set_seek_adjust(game_record::enum_seek_end);
		}
		else if(GetCurWorkID() == CECHPWork::WORK_STAND)
		{
			if(!vDelta.IsZero())
			{
				gr.set_seek_adjust(game_record::enum_seek_end);
				vDelta.y = 0.0f;
				vDelta.Normalize();
				CECHPWorkMove* pHPWork = (CECHPWorkMove*)CreateWork(CECHPWork::WORK_MOVETOPOS);
				pHPWork->SetDestination(CECHPWorkMove::DEST_DIR, vDelta);
				StartWork_p1(pHPWork);
			}
			else
			{
				if(gr.get_seek_adjust()== game_record::enum_seek_start)
					gr.set_seek_adjust(game_record::enum_seek_second);          // try again next frame
				else if(gr.get_seek_adjust() == game_record::enum_seek_second)
					gr.set_seek_adjust(game_record::enum_seek_none);            // host player is not auto moving
			}
		}
	}
	else if(gr.get_seek_adjust() == game_record::enum_seek_end)
	{
		if(vDelta.IsZero() && GetCurWorkID() != CECHPWork::WORK_STAND)
		{
			FinishCurWork(GetCurWorkID(),true);
			StartWork_p0(CreateWork(CECHPWork::WORK_STAND));
			gr.set_seek_adjust(game_record::enum_seek_none);
		}
	}
}

//	force to stop auto move, Only for record replay.
void CECHPWorkMan::StopAutoMove()
{
	A3DVECTOR3 vDelta  = gr.get_cur_frame_host_pos() - gr.get_pre_frame_host_pos();
	if(vDelta.IsZero())
	{
		CECHPWorkAutoMove* pHPWork = (CECHPWorkAutoMove *)GetCurWork();
		pHPWork->SetReached();
		FinishCurWork(GetCurWorkID(), true);
		StartWork_p0(CreateWork(CECHPWork::WORK_STAND));
	}	
}


