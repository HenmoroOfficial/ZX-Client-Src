/*
 * FILE: EC_HPWorkSit.h
 *
 * DESCRIPTION: 
 *
 * CREATED BY: Duyuxin, 2004/12/27
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


///////////////////////////////////////////////////////////////////////////
//	
//	Class CECHPWorkSit
//	
///////////////////////////////////////////////////////////////////////////

class CECHPWorkSit : public CECHPWork
{
public:		//	Types

public:		//	Constructor and Destructor

	CECHPWorkSit(CECHPWorkMan* pWorkMan);
	virtual ~CECHPWorkSit();

public:		//	Attributes

public:		//	Operations

	//	Set m_bBeSitting flag
	void SetBeSittingFlag(bool bTrue) { m_bBeSitting = bTrue; }

	//	Tick routine
	virtual bool Tick(DWORD dwDeltaTime);
	//	Reset work
	virtual void Reset();
	//	Copy work data
	virtual bool CopyData(CECHPWork* pWork);

protected:	//	Attributes

	bool	m_bBeSitting;
	
protected:	//	Operations

	//	On first tick
	virtual void OnFirstTick();
};

///////////////////////////////////////////////////////////////////////////
//	
//	Class CECHPWorkPick
//	
///////////////////////////////////////////////////////////////////////////

class CECHPWorkPick : public CECHPWork
{
public:		//	Types

public:		//	Constructor and Destructor

	CECHPWorkPick(CECHPWorkMan* pWorkMan);
	virtual ~CECHPWorkPick();

public:		//	Attributes

public:		//	Operations

	bool IsGather() const { return m_bIsGather; }
	void SetGather(bool bGather) { m_bIsGather = bGather; }
	void SetOprt(int nOprt) { m_nOprt = nOprt; }

	//	Tick routine
	virtual bool Tick(DWORD dwDeltaTime);
	//	Reset work
	virtual void Reset();
	//	Copy work data
	virtual bool CopyData(CECHPWork* pWork);

protected:	//	Attributes

	CECCounter	m_TimeCnt;		//	Pick time counter
	bool		m_bIsGather;
	int			m_nOprt;

protected:	//	Operations

	//	On first tick
	virtual void OnFirstTick();
};

///////////////////////////////////////////////////////////////////////////
//	
//	Class CECHPWorkGeneralOprt
//	
///////////////////////////////////////////////////////////////////////////

class CECHPWorkGeneralOprt : public CECHPWork
{
public:		//	Types

public:		//	Constructor and Destructor

	CECHPWorkGeneralOprt(CECHPWorkMan* pWorkMan);
	virtual ~CECHPWorkGeneralOprt();

public:		//	Attributes

public:		//	Operations

	//	Tick routine
	virtual bool Tick(DWORD dwDeltaTime);
	//	Reset work
	virtual void Reset();
	//	Copy work data
	virtual bool CopyData(CECHPWork* pWork);

protected:	//	Attributes

protected:	//	Operations

	//	On first tick
	virtual void OnFirstTick();
};

///////////////////////////////////////////////////////////////////////////
//	
//	Class CECHPWorkConcentrate
//	
///////////////////////////////////////////////////////////////////////////

class CECHPWorkConcentrate : public CECHPWork
{
public:		//	Types

	enum
	{
		DO_SUMMONPET = 0,	//	Summon pet
		DO_RECALLPET,
		DO_COMBINE1,
		DO_UNCOMBINE1,
		DO_COMBINE2,
		DO_UNCOMBINE2,
		DO_FREE,
		DO_ADOPT,
		DO_REFINE,
		DO_UNKNOWN,
	};

public:		//	Constructor and Destructor

	CECHPWorkConcentrate(CECHPWorkMan* pWorkMan);
	virtual ~CECHPWorkConcentrate();

public:		//	Attributes
	
public:		//	Operations

	//	Set / Get do what flag
	void SetDoWhat(int iDoWhat) { m_iDoWhat = iDoWhat; }
	int GetDoWhat() { return m_iDoWhat; }

	//	Tick routine
	virtual bool Tick(DWORD dwDeltaTime);
	//	Reset work
	virtual void Reset();
	//	Copy work data
	virtual bool CopyData(CECHPWork* pWork);

protected:	//	Attributes

	int		m_iDoWhat;

protected:	//	Operations

	//	On first tick
	virtual void OnFirstTick();
};

///////////////////////////////////////////////////////////////////////////
//	
//	Class CECHPWorkBeBound
//	
///////////////////////////////////////////////////////////////////////////

class CECHPWorkBeBound : public CECHPWork
{
public:		//	Types

public:		//	Constructor and Destructor

	CECHPWorkBeBound(CECHPWorkMan* pWorkMan);
	virtual ~CECHPWorkBeBound();

public:		//	Attributes
	
public:		//	Operations

	//	Tick routine
	virtual bool Tick(DWORD dwDeltaTime);
	//	Reset work
	virtual void Reset();
	//	Copy work data
	virtual bool CopyData(CECHPWork* pWork);

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

///////////////////////////////////////////////////////////////////////////
//	
//	Class CECHPWorkBlewUp
//	
///////////////////////////////////////////////////////////////////////////

class CECHPWorkBlewUp : public CECHPWork
{
public:		//	Types

public:		//	Constructor and Destructor

	CECHPWorkBlewUp(CECHPWorkMan* pWorkMan);
	virtual ~CECHPWorkBlewUp();

public:		//	Attributes

public:		//	Operations

	//	Tick routine
	virtual bool Tick(DWORD dwDeltaTime);
	//	Reset work
	virtual void Reset();
	//	Copy work data
	virtual bool CopyData(CECHPWork* pWork);

	void PrepareBlew(DWORD DWDuration, float fSpeed);

protected:	//	Attributes
	DWORD		m_dwDuration;
	DWORD		m_dwPastTime;
	A3DVECTOR3  m_vecPostion;
	float		m_fSpeed;
	float		m_fHeight;
	float		m_fAccelerate;
	protected:	//	Operations

	//	On first tick
	virtual void OnFirstTick();
};

///////////////////////////////////////////////////////////////////////////
//	
//	Class CECHPWorkBlewUp
//	
///////////////////////////////////////////////////////////////////////////

class CECHPWorkParabolic : public CECHPWork
{
public:		//	Types

public:		//	Constructor and Destructor

	CECHPWorkParabolic(CECHPWorkMan* pWorkMan);
	virtual ~CECHPWorkParabolic();

public:		//	Attributes

public:		//	Operations

	//	Tick routine
	virtual bool Tick(DWORD dwDeltaTime);
	//	Reset work
	virtual void Reset();
	//	Copy work data
	virtual bool CopyData(CECHPWork* pWork);

	void PrepareParabolic(DWORD dwDuration, float height, A3DVECTOR3 vecDest);

protected:	//	Attributes
	DWORD		m_dwStartTime;
	DWORD		m_dwDuration;
	float		m_fSpeedH;
	float		m_fSpeedV;
	float		m_fAccelerate;
	A3DVECTOR3  m_vecPostion;
	A3DVECTOR3	m_vecDest;
	float		m_fHeight;
	float		m_fDistance;
	float		m_fRunSpeedBackup;
	float		m_fMinAirHeiBackup;
protected:	//	Operations

	//	On first tick
	virtual void OnFirstTick();

	void Finish();
};

///////////////////////////////////////////////////////////////////////////
//	
//	Class CECHPWorkMoveNoCollision 
//	用于特殊目的、没有碰撞的移动
///////////////////////////////////////////////////////////////////////////

class CECHPWorkMoveNoCollision : public CECHPWork
{
public:		//	Types

public:		//	Constructor and Destructor

	CECHPWorkMoveNoCollision(CECHPWorkMan* pWorkMan);
	virtual ~CECHPWorkMoveNoCollision();

public:		//	Attributes

public:		//	Operations
	
	//	Tick routine
	virtual bool Tick(DWORD dwDeltaTime);
	//	Reset work
	virtual void Reset();
	//	Copy work data
	virtual bool CopyData(CECHPWork* pWork);

	void SetMoveInfo(DWORD dwStartTime, DWORD dwDuration, float fSpeed, A3DVECTOR3 vecDest, int idTarget);

protected:	//	Attributes
	DWORD       m_dwStartTime;
	DWORD		m_dwDuration;
	float		m_fSpeed;
	A3DVECTOR3  m_vecDest;
	int			m_idMoveToTarget;
protected:	//	Operations

	//	On first tick
	virtual void OnFirstTick();
	void Finish();
};