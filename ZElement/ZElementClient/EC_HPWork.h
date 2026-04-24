/*
 * FILE: EC_HPWork.h
 *
 * DESCRIPTION: 
 *
 * CREATED BY: Duyuxin, 2005/2/6
 *
 * HISTORY: 
 *
 * Copyright (c) 2005 Archosaur Studio, All Rights Reserved.
 */

#pragma once

#include "EC_ObjectWork.h"
#include "AArray.h"

///////////////////////////////////////////////////////////////////////////
//	
//	Define and Macro
//	
///////////////////////////////////////////////////////////////////////////


///////////////////////////////////////////////////////////////////////////
//	
//	Types and Global variables
//	
///////////////////////////////////////////////////////////////////////////

class CECHostPlayer;
class CECHPWorkMan;
struct ECMSG;

///////////////////////////////////////////////////////////////////////////
//	
//	Declare of Global functions
//	
///////////////////////////////////////////////////////////////////////////


///////////////////////////////////////////////////////////////////////////
//	
//	Class CECHPWork
//	
///////////////////////////////////////////////////////////////////////////

class CECHPWork : public CECObjectWork
{
public:		//	Types

	//	Host work ID
	enum
	{
		WORK_INVALID = -1,
		WORK_STAND = 0,		//	Stand and do nothing
		WORK_MOVETOPOS,		//	Move to a destination terrain position
		WORK_TRACEOBJECT,	//	Trace specified object (NPC, player etc.)
		WORK_HACKOBJECT,	//	Hack specified object (NPC, player etc.)
		WORK_SPELLOBJECT,	//	Spell magic on specified object (NPC, player etc.)
		WORK_USEITEM,		//	Host use item
		WORK_DEAD,			//	Host is dead
		WORK_FOLLOW,		//	Follow some one else
		WORK_AUTOMOVE,      //  Auto move
		WORK_FLYOFF,		//	Fly off
		WORK_FREEFALL,		//	Free fall
		WORK_SIT,			//	Sit
		WORK_PICKUP,		//	Pickup item
		WORK_CONCENTRATE,	//	All concentrate work
		WORK_REVIVE,		//	Revive
		WORK_FLASHMOVE,		//	Flash move
		WORK_BEBOUND,		//	Be bound by buddy
		WORK_GENERAL_OPRT,	//	General operation
		WORK_BE_MOVED,		//	Host is pulled by other player. Added 2011-07-28.
		WORK_SPECIAL_CHARGE,//	Used in some special considion, at present, it just used for pull. Added 2011-08-01.
		WORK_SPECIAL_MOVE,	//	Host player in special move mode, in which the collision and acceleration are cosidered. Added 2012-06-12.
		WORK_BLEW_UP,
		WORK_PARABOLIC,
		WORK_MOVENOCOLLISION,
		WORK_MOVEBEZIEL,
		NUM_WORK,
	};

	//	Work mask
	enum
	{
		MASK_STAND			= 0x0001,
		MASK_MOVETOPOS		= 0x0002,
		MASK_TRACEOBJECT	= 0x0004,
		MASK_HACKOBJECT		= 0x0008,
		MASK_SPELLOBJECT	= 0x0010,
		MASK_USEITEM		= 0x0020,
		MASK_DEAD			= 0x0040,
		MASK_FOLLOW			= 0x0080,
		MASK_FLYOFF			= 0x0100,
		MASK_FREEFALL		= 0x0200,
		MASK_SIT			= 0x0400,
		MASK_PICKUP			= 0x0800,
		MASK_CONCENTRATE	= 0x1000,
		MASK_REVIVE			= 0x2000,
		MASK_FLASHMOVE		= 0x4000,
		MASK_BEBOUND		= 0x8000,
		MASK_GENERAL_OPRT	= 0x10000,
		MASK_AUTOMOVE       = 0x20000,
		MASK_BE_MOVED		= 0x40000, //Added 2011-07-28.
		MASK_SPECIAL_CHARGE	= 0x80000, //Added 2011-08-01.
		MASK_SPECIAL_MOVE	= 0x100000,//Added 2012-06-12.
		MASK_BLEW_UP		= 0x200000,
		MASK_PARABOLIC		= 0x400000,
		MASK_MOVENOCOLLISION = 0x800000,
		MASK_MOVEBEZIEL		= 0X1000000,
		MASK_ALLWORK		= 0xfffff,
	};

public:		//	Constructor and Destructor

	CECHPWork(int iWorkID, CECHPWorkMan* pWorkMan);
	virtual ~CECHPWork() {}

public:		//	Attributes

public:		//	Operations

	//	This work is do player moving ?
	virtual bool IsMoving() { return false; }
	//	Copy work data
	virtual bool CopyData(CECHPWork* pWork);
	//	Generate record msg
	virtual bool gen_record_msg(ECMSG& msg) { return false; }

protected:	//	Attributes

	CECHPWorkMan*	m_pWorkMan;	//	Work manager
	CECHostPlayer*	m_pHost;	//	Host player object

protected:	//	Operations

};

///////////////////////////////////////////////////////////////////////////
//	
//	Class CECHPWorkMan
//	
///////////////////////////////////////////////////////////////////////////

class CECHPWorkMan
{
public:		//	Types

	friend class CECHostPlayer;

	//	Work priority
	enum
	{
		PRIORITY_0 = 0,
		PRIORITY_1,
		PRIORITY_2,
		PRIORITY_3,
		NUM_PRIORITY,
	};

	//	Delayed work info.
	struct DELAYWORK
	{
		int			iPriority;
		CECHPWork*	pWork;
		bool		bAutoDel;
	};

public:		//	Constructor and Destructor

	CECHPWorkMan(CECHostPlayer* pHost);
	virtual ~CECHPWorkMan();

public:		//	Attributes

public:		//	Operations

	//	Start work
	bool StartWork(int iPriority, CECHPWork* pWork, bool bAutoDel=true, bool bNoDelay=false);
	bool StartWork_p0(CECHPWork* pWork, bool bAutoDel=true, bool bNoDelay=false) { return StartWork(PRIORITY_0, pWork, bAutoDel, bNoDelay); }
	bool StartWork_p1(CECHPWork* pWork, bool bAutoDel=true, bool bNoDelay=false) { return StartWork(PRIORITY_1, pWork, bAutoDel, bNoDelay); }
	bool StartWork_p2(CECHPWork* pWork, bool bAutoDel=true, bool bNoDelay=false) { return StartWork(PRIORITY_2, pWork, bAutoDel, bNoDelay); }
	bool StartWork_p3(CECHPWork* pWork, bool bAutoDel=true, bool bNoDelay=false) { return StartWork(PRIORITY_3, pWork, bAutoDel, bNoDelay); }
	//	Finish current work
	void FinishCurWork(int idWork, bool bCancel);
	//	Finish all work
	void FinishAllWork(bool bGotoStand);
	//	Get current work
	CECHPWork* GetCurWork() { return m_pCurWork; }
	int GetCurWorkID() { return m_pCurWork ? m_pCurWork->GetWorkID() : CECHPWork::WORK_INVALID; }
	//	Get work of specified priority
	CECHPWork* GetWork(int iPriority) { ASSERT(iPriority >= PRIORITY_0 && iPriority < NUM_PRIORITY); return m_WorkStack[iPriority]; }
	CECHPWork* GetWork_p0() { return m_WorkStack[PRIORITY_0]; }
	CECHPWork* GetWork_p1() { return m_WorkStack[PRIORITY_1]; }
	CECHPWork* GetWork_p2() { return m_WorkStack[PRIORITY_2]; }
	CECHPWork* GetWork_p3() { return m_WorkStack[PRIORITY_3]; }
	//	Get work by ID
	CECHPWork* GetWorkByID(int id, bool bReset);
	//	Create a work object
	CECHPWork* CreateWork(int idWork);
	//	Get delayed work
	CECHPWork* GetDelayedWork() { return m_Delayed.pWork; }

	//	Get host player object
	CECHostPlayer* GetHostPlayer() { return m_pHost; }

 	//	Adjust Host Player work,  Only for record replay(as seeking progress bar, host player out of work).
	void AdjustHPWork(); 

	//	Only for record replay ( when replay record, auto moving don't stop correctly sometimes, force to stop it)
	void StopAutoMove();
protected:	//	Attributes

	CECHostPlayer*	m_pHost;	//	Host player object

	CECHPWork*	m_WorkStack[NUM_PRIORITY];	//	Current work stack
	CECHPWork*	m_pCurWork;					//	Current work
	int			m_iCurWork;					//	Current work's priority
	DELAYWORK	m_Delayed;					//	Delayed work

	APtrArray<CECHPWork*>	m_aWorks;	//	Work array

protected:	//	Operations

	//	Start specified work
	bool InternallyStartWork(int iPriority, int iWork);
	//	Decrease current work to a lower priority work
	void DecCurWorkPriority();
	//	Check whether we can start a work
	bool CanStartWork(int iPriority, int iWork);
	//	Delay specified work ?
	bool DelayWork(int iPriority, CECHPWork* pWork, bool bAutoDel);
	//	Start the delayed work
	void StartDelayedWork();
};

///////////////////////////////////////////////////////////////////////////
//	
//	Inline functions
//	
///////////////////////////////////////////////////////////////////////////

