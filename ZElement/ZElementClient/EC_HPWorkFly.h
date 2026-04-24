/*
 * FILE: EC_HPWorkFly.h
 *
 * DESCRIPTION: 
 *
 * CREATED BY: Duyuxin, 2004/11/23
 *
 * HISTORY: 
 *
 * Copyright (c) 2004 Archosaur Studio, All Rights Reserved.
 */

#pragma once

#include "EC_HPWork.h"

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


///////////////////////////////////////////////////////////////////////////
//	
//	Class CECHPWorkFly
//	
///////////////////////////////////////////////////////////////////////////

class CECHPWorkFly : public CECHPWork
{
public:		//	Types

public:		//	Constructor and Destructor

	CECHPWorkFly(CECHPWorkMan* pWorkMan);
	virtual ~CECHPWorkFly();

public:		//	Attributes
	
	DWORD m_dwLaunchTime;
	bool m_bContinueFly;
	bool m_bSkillFly; // 玩家释放技能导致的飞行效果

public:		//	Operations

	//	Tick routine
	virtual bool Tick(DWORD dwDeltaTime);
	//	Reset work
	virtual void Reset();
	//	Copy work data
	virtual bool CopyData(CECHPWork* pWork);
	//	This work is do player moving ?
	virtual bool IsMoving() { return true; }
	//	Work is cancel
	virtual void Cancel();

protected:	//	Attributes

	A3DVECTOR3 	m_vDestPos;		//	Destination stop position
	bool		m_bAddSpeed;	//	Add a vertical speed

protected:	//	Operations

	//	On first tick
	virtual void OnFirstTick();
};

///////////////////////////////////////////////////////////////////////////
//	
//	Class CECHPWorkFMove
//	
///////////////////////////////////////////////////////////////////////////

class CECHPWorkFMove : public CECHPWork
{
public:		//	Types

public:		//	Constructor and Destructor

	CECHPWorkFMove(CECHPWorkMan* pWorkMan);
	virtual ~CECHPWorkFMove();

public:		//	Attributes
	
public:		//	Operations

	//	Prepare to move
	void PrepareMove(const A3DVECTOR3& vDestPos, float fMoveTime);

	//	Tick routine
	virtual bool Tick(DWORD dwDeltaTime);
	//	Reset work
	virtual void Reset();
	//	Copy work data
	virtual bool CopyData(CECHPWork* pWork);
	//	This work is do player moving ?
	virtual bool IsMoving() { return true; }

protected:	//	Attributes

	A3DVECTOR3 	m_vDestPos;		//	Destination position
	A3DVECTOR3	m_vMoveDir;		//	Move direction
	float		m_fSpeed;		//	Move speed
	float		m_fDist;		//	Distance
	float		m_fDistCnt;		//	Distance counter
	float		m_fYDelta;		//	

protected:	//	Operations

	//	On first tick
	virtual void OnFirstTick();
	
	//	Finish work
	void Finish();
};

///////////////////////////////////////////////////////////////////////////
//	
//	Inline functions
//	
///////////////////////////////////////////////////////////////////////////



