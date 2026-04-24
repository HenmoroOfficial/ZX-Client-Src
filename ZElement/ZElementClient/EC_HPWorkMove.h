/*
 * FILE: EC_HPWorkMove.h
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
//	Class CECHPWorkMove
//	
///////////////////////////////////////////////////////////////////////////

class CECHPWorkMove : public CECHPWork
{
public:		//	Types

	enum
	{
		DEST_2D = 0,
		DEST_3D,
		DEST_DIR,
		DEST_PUSH,
		DEST_STANDJUMP,
	};

public:		//	Constructor and Destructor

	CECHPWorkMove(CECHPWorkMan* pWorkMan);
	virtual ~CECHPWorkMove();

public:		//	Attributes

public:		//	Operations

	//	Set destination position or direction
	void SetDestination(int iDestType, const A3DVECTOR3& vMoveDest);

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

	//	Play move target effect
	void PlayMoveTargetGFX(const A3DVECTOR3& vPos, const A3DVECTOR3& vNormal);

	//	User press cancel button
	void PressCancel() { m_bReadyCancel = true; }
	//	Generate record msg
	virtual bool gen_record_msg(ECMSG& msg);

protected:	//	Attributes

	A3DVECTOR3	m_vMoveDest;	//	Move destination position or direction
	int			m_iDestType;	//	Destination type
	bool		m_bHaveDest;	//	true, have destination
	bool		m_bMeetSlide;	//	true, meet slide
	A3DVECTOR3	m_vCurDir;		//	Current move direction
	bool		m_bReadyCancel;	//	true, ready to cancel
	bool		m_bGliding;		//	glide
	float		m_fGlideTime;
	float		m_fGlideSpan;
	float		m_fGlideAng;
	float		m_fGlideVel;	//	glide angular vel
	float		m_fGlidePitch;	//	glide pitch angle
	float		m_fCurPitch;
	float		m_fPushPitch;
	float		m_fPushLean;

protected:	//	Operations

	//	On first tick
	virtual void OnFirstTick();

	//	Tick routine of walking on ground
	bool Tick_Walk(float fDeltaTime);
	//	Tick routine of flying or swimming
	bool Tick_FlySwim(float fDeltaTime);
	//	Start gliding
	void Glide(float fMoveTime, const A3DVECTOR3& vMoveDirH, float fDeltaTime, bool bFly);

	//	Calculate vertical speed when fly or swim
	float CalcFlySwimVertSpeed(float fSpeed1, float fPushDir, float fPushAccel, float fDeltaTime);
	//	Finish work
	void Finish();
};


///////////////////////////////////////////////////////////////////////////
//
//CECHPWorkBeMoved 类声明
//
///////////////////////////////////////////////////////////////////////////
class CECHPWorkBeMoved : public CECHPWork
{
public:		//	Constructor and Destructor

	CECHPWorkBeMoved(CECHPWorkMan* pWorkMan);
	virtual ~CECHPWorkBeMoved();

public:		//	Operations

	//	Set destination position or direction
	void SetDestination(const A3DVECTOR3& vDestPos, float fSpeed);

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

protected:	//	Attributes

	A3DVECTOR3	m_vDestPos;		//	Move destination position
	A3DVECTOR3	m_vCurDir;		//	Current move direction
	bool		m_bReadyCancel;	//	true, ready to cancel
	float		m_fSpeed;		//	current move speed

protected:	//	Operations

	//	On first tick
	virtual void OnFirstTick();

	//	Tick routine of walking on ground
	bool Tick_Move(float fDeltaTime);

	//	Finish work
	void Finish();
};


///////////////////////////////////////////////////////////////////////////
//
//CECHPWorkSpecialMove 类声明
//
///////////////////////////////////////////////////////////////////////////
class CECHPWorkSpecialMove : public CECHPWork
{
public:		//	Constructor and Destructor

	CECHPWorkSpecialMove(CECHPWorkMan* pWorkMan);
	virtual ~CECHPWorkSpecialMove();

public:		//	Operations

	//	Set Move velocity and acceleration.
	void SetVelocityAndAcceleration(const A3DVECTOR3& vVelocity, const A3DVECTOR3& vAcceleration);

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

protected:	//	Attributes
	bool		m_bReadyCancel;	//	true, ready to cancel
	A3DVECTOR3	m_vCurrentVelocity;	// current velocity
	A3DVECTOR3	m_vAcceleration;//	current acceleration

protected:	//	Operations

	//	On first tick
	virtual void OnFirstTick();

	//	Tick routine of walking on ground
	bool Tick_Move(float fDeltaTime);

	//	Finish work
	void Finish();

	//	Update the current acceleration
	void UpdateCurrentAcceleration( float fDeltaTime );
};

///////////////////////////////////////////////////////////////////////////
//	
//	Inline functions
//	
///////////////////////////////////////////////////////////////////////////

