/*
 * FILE: EC_HPWorkSpell.h
 *
 * DESCRIPTION: 
 *
 * CREATED BY: Duyuxin, 2005/3/8
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

class CECSkill;

///////////////////////////////////////////////////////////////////////////
//	
//	Declare of Global functions
//	
///////////////////////////////////////////////////////////////////////////


///////////////////////////////////////////////////////////////////////////
//	
//	Class CECHPWorkSpell
//	
///////////////////////////////////////////////////////////////////////////

class CECHPWorkSpell : public CECHPWork
{
public:		//	Types

	//	Spell magic state
	enum
	{
		ST_INCANT = 0,
		ST_SPELL,
	};

public:		//	Constructor and Destructor

	CECHPWorkSpell(CECHPWorkMan* pWorkMan);
	virtual ~CECHPWorkSpell();

public:		//	Attributes

public:		//	Operations

	//	Prepare cast
	void PrepareCast(int idTarget, CECSkill* pSkill, int iIncantTime);
	//  设置蓄力技能的蓄力最大时间
	void SetChargeTime(int iChargeTime) { m_iChargeTime = iChargeTime; }
	//	Change state
	void ChangeState(int iState);
	//	Get state
	int GetState() { return m_iState; }

	//	Tick routine
	virtual bool Tick(DWORD dwDeltaTime);
	//	Reset work
	virtual void Reset();
	//	Copy work data
	virtual bool CopyData(CECHPWork* pWork);

	CECSkill * GetSkill() const {return m_pSkill;}
protected:	//	Attributes

	CECSkill*	m_pSkill;		//	Skill object
	CECCounter	m_OverTimeCnt;	//	Over time counter
	int			m_iState;
	int			m_idTarget;		//	Target id
	int			m_iChargeTime;	//  蓄力技能的最大蓄力时间
protected:	//	Operations

	//	On first tick
	virtual void OnFirstTick();
};


///////////////////////////////////////////////////////////////////////////
//
//CECHPWorkSpecialCharge 类的声明 Added 2011-08-01.
//
///////////////////////////////////////////////////////////////////////////
class CECHPWorkSpecialCharge : public CECHPWork
{
public:		//	Constructor and Destructor

	CECHPWorkSpecialCharge(CECHPWorkMan* pWorkMan);
	virtual ~CECHPWorkSpecialCharge();

public:		//	Operations

	//	set current work finished.
	void SetFinished() { m_bFinished = true; }
	//	Tick routine
	virtual bool Tick(DWORD dwDeltaTime);
	//	Reset work
	virtual void Reset();
	//	Copy work data
	virtual bool CopyData(CECHPWork* pWork);
	//	继承的接口，需要注意的是，WorkMan在Finish当前work的时候会调用Cancel接口，而不会调用Finish接口
	virtual void Cancel() { m_bFinished = true; }

protected:	//	Attributes

protected:	//	Operations

	//	On first tick
	virtual void OnFirstTick();
};

///////////////////////////////////////////////////////////////////////////
//	
//	Inline functions
//	
///////////////////////////////////////////////////////////////////////////

