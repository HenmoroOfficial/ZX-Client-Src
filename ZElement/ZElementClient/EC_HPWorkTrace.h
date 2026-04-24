/*
 * FILE: EC_HPWorkTrace.h
 *
 * DESCRIPTION: 
 *
 * CREATED BY: Duyuxin, 2004/10/11
 *
 * HISTORY: 
 *
 * Copyright (c) 2004 Archosaur Studio, All Rights Reserved.
 */

#pragma once

#include "EC_HPWork.h"
#include "EC_Counter.h"

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


///////////////////////////////////////////////////////////////////////////
//	
//	Declare of Global functions
//	
///////////////////////////////////////////////////////////////////////////

class CECObject;
class CECSkill;

///////////////////////////////////////////////////////////////////////////
//	
//	Class CECHPWorkTrace
//	
///////////////////////////////////////////////////////////////////////////

class CECHPWorkTrace : public CECHPWork
{
public:		//	Types

	//	Trace reason
	enum
	{
		TRACE_NONE = -1,
		TRACE_ATTACK = 0,	//	Normal attack
		TRACE_PICKUP,		//	Pickup object
		TRACE_TALK,			//	Go to talk
		TRACE_SPELL,		//	Cast magic
		TRACE_GATHER,		//	Gather object
		TRACE_TELEPORT,		//  Monster teleport
		TRACE_INTERACT,		//	Interact with object
	};

public:		//	Constructor and Destructor

	CECHPWorkTrace(CECHPWorkMan* pWorkMan);
	virtual ~CECHPWorkTrace();

public:		//	Attributes

public:		//	Operations

	//	Change trace target
	void ChangeTarget(int idTarget, int iReason);

	//	Tick routine
	virtual bool Tick(DWORD dwDeltaTime);
	//	Reset work
	virtual void Reset();
	//	Work is cancel
	virtual void Cancel();

	//	This work is do player moving ?
	virtual bool IsMoving() { return true; }
	//	Copy work data
	virtual bool CopyData(CECHPWork* pWork);

	//	User press cancel button
	void PressCancel();
	//	Set move close flag
	void SetMoveCloseFlag(bool bMoveClose) { m_bMoreClose = bMoveClose; }
	//	Get trace reason
	int GetTraceReason() { return m_iReason; }
	//	Set / Get force attack flag
	void SetForceAttack(bool bTrue) { m_bForceAttack = bTrue; }
	bool GetForceAttack() { return m_bForceAttack; }
	//	Set / Get prepared skill
	void SetPrepSkill(CECSkill* pSkill) { m_pPrepSkill = pSkill; }
	CECSkill* GetPrepSkill() { return m_pPrepSkill; }
	//	Get target ID
	int GetTarget() { return m_idTarget; }
	//	Generate record msg
	virtual bool gen_record_msg(ECMSG& msg);

protected:	//	Attributes

	int			m_idTarget;		//	Target object's id
	bool		m_bHaveMoved;	//	Have moved flag
	A3DVECTOR3	m_vTargetPos;	//	Target position
	bool		m_bMeetSlide;	//	true, meet slide
	int			m_iReason;		//	Trace reason
	bool		m_bCheckTouch;	//	Check whether touch target in this frame
	A3DVECTOR3	m_vCurDirH;		//	Current move direction
	bool		m_bReadyCancel;	//	true, ready to cancel
	bool		m_bMoreClose;	//	Move close flag
	bool		m_bForceAttack;	//	Force attack flag
	CECSkill*	m_pPrepSkill;	//	Skill prepared to be casted

protected:	//	Operations

	//	On first tick
	virtual void OnFirstTick();

	//	Trace on ground
	bool Trace_Walk(float fDeltaTime);
	//	Trace in air and water
	bool Trace_FlySwim(float fDeltaTime);

	//	Stop move when touch target
	void StopMove(bool bFinish);
	//	Touched target
	void TouchedTarget(CECObject* pObject);
	//	Handle the case that target died when host is tracing it
	bool OnTargetDied(CECObject* pTarget);
	//	Is valid time to touch target ?
	bool IsGoodTimeToTouch();
	//	Check prepare skill
	void CheckPrepSkill();
};

///////////////////////////////////////////////////////////////////////////
//	
//	Inline functions
//	
///////////////////////////////////////////////////////////////////////////

