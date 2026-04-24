/*
 * FILE: EC_HPWorkMoveBezier.h
 *
 * DESCRIPTION: 
 *
 * CREATED BY: Fu Chonggang, 2013/09/16
 *
 * HISTORY: 
 *
 * Copyright (c) 2013 Archosaur Studio, All Rights Reserved.
 */

#pragma once

#include "EC_HPWork.h"

///////////////////////////////////////////////////////////////////////////
//	
//	Define and Macro
//	
///////////////////////////////////////////////////////////////////////////

class CECBezier;
class CECBezierWalker;

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
//	Class CECHPWorkMove
//	
///////////////////////////////////////////////////////////////////////////


///////////////////////////////////////////////////////////////////////////
//
//CECHPWorkBezierMove 类声明
//这种移动过程按照贝塞尔曲线走，速度需要预先设定
//
///////////////////////////////////////////////////////////////////////////
class CECHPWorkBezierMove : public CECHPWork
{
public:		//	Constructor and Destructor

	CECHPWorkBezierMove(CECHPWorkMan* pWorkMan);
	virtual ~CECHPWorkBezierMove();

public:		//	Operations

	//	Set Move velocity and acceleration.
	void SetBezierIdAndSpeed(const int iId, const float fSpeed);

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

	void PressCancel() { if(m_iRunState==0) m_iRunState = 1; }
protected:	//	Attributes
	float		m_fSpeed;
	int			m_iIdBezier;
	int			m_iRunState;	// 0:start 1:press cancel 2:ready for landing
	bool		m_bReadyCancel;	//	true, ready to cancel

	CECBezier		*m_pBezier;
	CECBezierWalker *m_pBezierWalker;

protected:	//	Operations

	//	On first tick
	virtual void OnFirstTick();

	//	Tick routine of walking on ground
	bool Tick_Move(DWORD dwDeltaTime);

	//	Finish work
	void Finish();

	void ChangeBezier(const int iId);
};
///////////////////////////////////////////////////////////////////////////
//	
//	Inline functions
//	
///////////////////////////////////////////////////////////////////////////

