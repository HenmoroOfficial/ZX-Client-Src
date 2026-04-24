/*
 * FILE: EC_HostMove.h
 *
 * DESCRIPTION: 
 *
 * CREATED BY: Duyuxin, 2004/12/3
 *
 * HISTORY: 
 *
 * Copyright (c) 2004 Archosaur Studio, All Rights Reserved.
 */

#pragma once

#include "A3DVector.h"
#include "EC_Counter.h"

///////////////////////////////////////////////////////////////////////////
//	
//	Define and Macro
//	
///////////////////////////////////////////////////////////////////////////

//	Negative accelerate in water and air
#define EC_NACCE_AIR		-5.0f
#define EC_NACCE_WATER		-5.0f

//	Push accelerate
#define EC_PUSH_ACCE		8.0f	//	Normal push accelerate in air and water
#define EC_FLYOFF_ACCE		10.0f	//	Fly off accelerate

#define EC_SLOPE_Y			0.7071f
#define EC_GRAVITY			9.8f

///////////////////////////////////////////////////////////////////////////
//	
//	Types and Global variables
//	
///////////////////////////////////////////////////////////////////////////

class CECHostPlayer;

///////////////////////////////////////////////////////////////////////////
//	
//	Declare of Global functions
//	
///////////////////////////////////////////////////////////////////////////


///////////////////////////////////////////////////////////////////////////
//	
//	Class CECHostMove
//	
///////////////////////////////////////////////////////////////////////////

class CECHostMove
{
public:		//	Types

	//	Delayed stop move data
	struct STOPMOVE
	{
		bool		bValid;		//	Valid flag
		A3DVECTOR3	vPos;
		float		fSpeed;
		int			iMoveMode;
		BYTE		byDir;
		DWORD		dwTimeStamp;
		float		fTime;
	};

public:		//	Constructor and Destructor

	CECHostMove(CECHostPlayer* pHost);
	virtual ~CECHostMove();

public:		//	Attributes

	A3DVECTOR3	m_vFlashTPNormal;	//	TP normal after last flash move
	bool		m_bFlashOnGnd;		//	On ground flag after last flash move
	bool		m_bFlashCanStay;	//	Can stay flag after last flash move

public:		//	Operations

	//	Tick routine
	void Tick(DWORD dwDeltaTime);

	//	Forcase moving and send move command
	void SendMoveCmd(const A3DVECTOR3& vCurPos, int iDestType, const A3DVECTOR3& vDest, const A3DVECTOR3& vMoveDir, float fSpeed, int iMoveMode);
	//	Forcase moving and send move command
	void SendMoveCmd(const A3DVECTOR3& vCurPos, int iDestType, const A3DVECTOR3& vDest, const A3DVECTOR3& vVel, int iMoveMode)
	{
		A3DVECTOR3 vMoveDir(vVel);
		float fSpeed = vMoveDir.Normalize();
		SendMoveCmd(vCurPos, iDestType, vDest, vMoveDir, fSpeed, iMoveMode);
	}
	//	Send stop move command
	void SendStopMoveCmd(const A3DVECTOR3& vPos, float fSpeed, int iMoveMode);
	//	Send stop move command using current position and default speed and move mode
	void SendStopMoveCmd();

	//	Check whether host meet a slope
	bool MeetSlope(const A3DVECTOR3& vMoveDirH, float& fMaxSpeedV);
	//	Is stoping ?
	bool IsStop() { return m_bStop; }

	//	Ground move
	A3DVECTOR3 GroundMove(const A3DVECTOR3& vDirH, float fSpeedH, float fTime, float fSpeedV=0.0f, float fGravity=EC_GRAVITY);
	//	Air/Water move
	A3DVECTOR3 AirWaterMove(const A3DVECTOR3& vSpeed, float fTime, bool bInAir, bool bTrace=false)
	{
		A3DVECTOR3 vDir = vSpeed;
		float fSpeed = vDir.Normalize();
		return AirWaterMove(vDir, fSpeed, fTime, bInAir, bTrace);
	}
	A3DVECTOR3 AirWaterMove(const A3DVECTOR3& vDir, float fSpeed, float fTime, bool bInAir, bool bTrace=false);
	//	Flash move
	A3DVECTOR3 FlashMove(const A3DVECTOR3& vDir, float fSpeed, float fDist);

	//  try to go a specified pos
	A3DVECTOR3 TryToGo(const A3DVECTOR3& vPos);

	//	Get move blocked counter
	int MoveBlocked() { return m_iBlockedCnt; }
	//	Get host average horizonal moving speed (m/s)
	float GetAverHoriSpeed() { return m_fAverSpeedH; }
	//	Force to set host's last position
	void SetHostLastPos(const A3DVECTOR3& vPos) { m_vLastPos = vPos; }
	//	Set host's last position sent to server
	void SetLastSevPos(const A3DVECTOR3& vPos) { m_vLastSevPos = vPos; }
	//	Get host's last position sent to server
	const A3DVECTOR3& GetLastSevPos() { return m_vLastSevPos; }
	//	Set / Get move stamp
	void SetMoveStamp(WORD wStamp) { m_wMoveStamp = wStamp; }
	WORD GetMoveStamp() { return m_wMoveStamp; }
	//	Set / Get slide block flag
	void SetSlideLock(bool bLock) { m_bSlideLock = bLock; }
	bool GetSlideLock() { return m_bSlideLock; }

	//	Reset move control to stop state
	void Reset();

	//	Reset the move time for special use.
	void ResetMoveTime() { m_fMoveTime = 0.0f; }

	//-------------------------------------------------------------------------
	//	Send start Special-Move cmd to server
	void SendStartSpecialMoveCmd( const A3DVECTOR3& velocity, const A3DVECTOR3& acceleration, const A3DVECTOR3& vCurPos, char collisionState = 0 );

	//	Send Special-Move cmd to server
	void SendSpecialMoveCmd( const A3DVECTOR3& velocity, const A3DVECTOR3& acceleration, const A3DVECTOR3& curPos, bool bNoDelay = false, char collisionState = 0 );

	//	Send Stop-Special-Move cmd to server
	void SendStopSpecialMoveCmd();
	//-------------------------------------------------------------------------

protected:	//	Attributes

	CECHostPlayer*		m_pHost;	//	Host player

	CECCounter	m_CmdTimeCnt;		//	Move command sending time counter
	CECCounter	m_StopCmdCnt;
	CECCounter	m_SpecialMoveCmdCnt;//	Special move command sending time counter
	DWORD		m_dwMoveTimeCnt;	//	Move command sending time counter
	DWORD		m_dwMoveCmdInter;	//	Interval of sending move command to server
	bool		m_bStop;			//	Stop flag
	int			m_iBlockedCnt;		//	Move blocked counter
	bool		m_bSlideLock;		//	Locked when slide
	float		m_fMoveTime;		//	Move time counter
	float		m_fBlockTime;		//	Block time counter
	A3DVECTOR3	m_vBlockMove;		//	Block move counter

	float		m_fAverSpeedH;		//	Host average horizonal moving speed (m/s)
	A3DVECTOR3	m_vLastPos;			//	Host's position in last tick
	DWORD		m_dwLastTime;		//	Last tick time
	WORD		m_wMoveStamp;		//	Move stamp
	A3DVECTOR3	m_vLastSevPos;		//	Host's last position sent to server
	bool		m_bLastJump;		//	Last jump flag

	STOPMOVE	m_DelayedStop;

protected:	//	Operations

};

///////////////////////////////////////////////////////////////////////////
//	
//	Inline functions
//	
///////////////////////////////////////////////////////////////////////////



