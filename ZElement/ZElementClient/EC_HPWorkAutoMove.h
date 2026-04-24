/********************************************************************
	created:	2007/04/03
	author:		kuiwu
	
	purpose:	host player automove directed by a given path
	Copyright (C) 2007 - All Rights Reserved
*********************************************************************/
#pragma  once

#include "A3DVector.h"
#include "EC_HPWork.h"
#include "EC_Counter.h"
#include "A3DEngine.h"
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
extern ACString g_AutoMoveDestName;

///////////////////////////////////////////////////////////////////////////
//	
//	Declare of Global functions
//	
///////////////////////////////////////////////////////////////////////////

///////////////////////////////////////////////////////////////////////////
//	
//	Class CECHPWorkAutoMove
//	
///////////////////////////////////////////////////////////////////////////

class CECHPWorkAutoMove : public CECHPWork
{
public:		//	Types

public:		//	Constructor and Destructor

	CECHPWorkAutoMove(CECHPWorkMan* pWorkMan);
	virtual ~CECHPWorkAutoMove();

public:		//	Attributes

public:		//	Operations

	//	This work is do player moving ?
	virtual bool IsMoving() { return true; }

	//	Tick routine
	virtual bool Tick(DWORD dwDeltaTime);
	//	Reset work
	virtual void Reset();
	//	Copy work data
	virtual bool CopyData(CECHPWork* pWork);

	virtual  void Cancel();  //finish work 

	//	Stop move
	void StopMove(const A3DVECTOR3& vPos, float fSpeed, int iMoveMode);

	void SetReached() { m_bReach = true; }  // Only for record replay
protected:	//	Attributes

	A3DVECTOR3	m_vDestPos;		//	Destination position
	A3DVECTOR3	m_vCurDirH;		//	Current move direction
	bool		m_bReach;		//	reach the goal
	//bool		m_bMeetSlide;	//	true, meet slide
	int         m_PathIndex;    //  current path index
	
	CECCounter   m_Counter;
protected:	//	Operations

	//	On first tick
	virtual void OnFirstTick();
	
	//	Follow on ground
	bool Follow_Walk(DWORD dwDeltaTime);
	//	Follow in air and water
	bool Follow_FlySwim(DWORD dwDeltaTime);

	//	Calculate vertical speed when fly or swim
	float CalcFlySwimVertSpeed(float fSpeed1, float fPushDir, float fPushAccel, float fDeltaTime);

	bool _CheckBlock(DWORD dwDeltaTime);
	
	int  _GetNextPathIndex(int curNext);

};

///////////////////////////////////////////////////////////////////////////
//	
//	Inline functions
//	
///////////////////////////////////////////////////////////////////////////

