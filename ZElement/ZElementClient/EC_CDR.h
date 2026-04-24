/*
 * FILE: EC_CDR.h
 *
 * DESCRIPTION:  A set of interfaces define the moving strategy of the avatar.
 *							Which focus on collision detection and give an appropriate response.
 *
 * CREATED BY: He wenfeng, 2005/3/8
 *
 * HISTORY: 
 *
 * Copyright (c) 2004 Archosaur Studio, All Rights Reserved.
 */

#pragma once
#include "A3DVector.h"

//inspired  by half life 2
#define  CDR_HL2_MOVE


// for other player's movement
struct OtherPlayer_Move_Info
{
	// Bounding sphere of avator
	A3DVECTOR3	vCenter;
	A3DVECTOR3	vExts;
	float fStepHeight;

	// Velocity 
	A3DVECTOR3 vVelocity;

	// time span ( sec )
	float t;
	
	bool bTraceGround;	//	Whether trace the ground
	bool bTestTrnOnly;	//	Trace terrain only
	
	A3DVECTOR3 vecGroundNormal;	// if bTraceGround is true, this will contain the ground normal when returned
};

// for on-air move case
//@note  : change to AABB.  By Kuiwu[22/9/2005]
struct ON_AIR_CDR_INFO
{
	A3DVECTOR3	vCenter;
	A3DVECTOR3  vExtent;  
	
	// Hold a height from the surface of terrain or building
	float	fHeightThresh;
	
	// Velocity Info
	A3DVECTOR3 vVelDir;
	float fSpeed;

	// time span ( sec )
	float t;					

	//@note : SlopeThresh seems useless on air or under water. By Kuiwu[22/9/2005]
	// Slope Thresh
	//float fSlopeThresh;

	// Distance Thresh under the water surface
	float fUnderWaterDistThresh;

	A3DVECTOR3 vTPNormal;

	// On air or water, true if on air, false for on water case.
	bool bOnAir;				
	
	// After the move action is done, If the fHeightThresh 
	// still be satisfied, bMeetHeightThresh is set to true.
	bool bMeetHeightThresh;

	ON_AIR_CDR_INFO()
	{
		vTPNormal.Clear();
	}
};

// for on-ground move case
//@note  : change to AABB.  By Kuiwu[22/9/2005]
struct CDR_INFO
{
	//the aabb
	A3DVECTOR3	vCenter;
	//@note : the caller should make sure ext(.x, .y, .z) > 0.  By Kuiwu[22/9/2005]
	A3DVECTOR3  vExtent; 

	float	fStepHeight;
	
	// Velocity Info
	A3DVECTOR3 vXOZVelDir;
#ifdef CDR_HL2_MOVE
	A3DVECTOR3 vClipVel;  //velolcity after clipping
#else
	float fYVel;	
#endif
	
	float fSpeed;

	// time span ( sec )
	float t;					

	// Gravity acceleration
	float fGravityAccel;

	// the Climb Slope Thresh
	float fSlopeThresh;

	// Tangent plane Info
	A3DVECTOR3 vTPNormal;

	// Absolute Velocity: output for forcast!  
	A3DVECTOR3 vAbsVelocity;

	//the moving dist
	float     fMoveDist;  
	//block by obstacle
	bool      bBlocked; 
	//on the surface
	bool      bOnSurface;
	//can stay
	bool      bCanStay;

};

//	VertAABBTrace output value
struct CDR_TRACE_OUTPUT
{
	A3DVECTOR3	vHitPos;
	A3DVECTOR3	vHitNormal;
	float		fFaction;		//	< 0.0f: invalid value
	int			iUserData1;
	int			iUserData2;

	CDR_TRACE_OUTPUT::CDR_TRACE_OUTPUT() : vHitPos(0.0f), vHitNormal(0.0f)
	{
		fFaction	= -1.0f;
		iUserData1	= 0;
		iUserData2	= 0;
	}
};

void OtherPlayerMove(OtherPlayer_Move_Info& OPMoveInfo);

void OnGroundMove(CDR_INFO& CDRInfo);

void OnAirMove(ON_AIR_CDR_INFO& OnAirCDRInfo);

// Trace the building and the ground
bool VertRayTrace(const A3DVECTOR3& vPos, A3DVECTOR3& vHitPos, A3DVECTOR3& vHitNormal, float DeltaY=1000.0f);
bool RayTrace(const A3DVECTOR3& vStart, const A3DVECTOR3& vDelta, A3DVECTOR3& vHitPos, A3DVECTOR3& vNormal, float& fraction);
bool VertAABBTrace(const A3DVECTOR3& vCenter, const A3DVECTOR3& vExt, CDR_TRACE_OUTPUT& Out, float DeltaY=1000.0f);
bool AABBTrace(const A3DVECTOR3& vCenter, const A3DVECTOR3& vExt, const A3DVECTOR3& vDelta, A3DVECTOR3& vEndPos, float& fFaction);

// only trace the building
bool RayTraceEnv(const A3DVECTOR3& vStart, const A3DVECTOR3& vDelta, A3DVECTOR3& vHitPos, A3DVECTOR3& vNormal, float& fraction);
bool VertRayTraceEnv(const A3DVECTOR3& vPos, A3DVECTOR3& vHitPos, A3DVECTOR3& vHitNormal, float DeltaY=1000.0f);

// Trace carrier NPCs
bool RayTraceCarrier(const A3DVECTOR3& vStart, const A3DVECTOR3& vDelta, A3DVECTOR3& vHitPos, A3DVECTOR3& vNormal, float& fraction);

// test if the position lies in the valid submaps
bool IsPosInAvailableMap(const A3DVECTOR3& vPos);

void SetMaxFlyHeight(float f);

namespace CHBasedCD
{
	class BrushTraceInfo;
};

bool AABBCollideWithBrush(CHBasedCD::BrushTraceInfo * pInfo);

void UpdateVelocityAfterCD( const A3DVECTOR3& vPos1, const A3DVECTOR3& vPos2, int nMass1, int nMass2, A3DVECTOR3& vVelocity1, A3DVECTOR3& vVelocity2 );

void CalculateDeltaDist( const A3DVECTOR3& vVelocity, float fAcceleration, const A3DVECTOR3& vClipVelocity, const A3DVECTOR3& vTPNormal, float fDaltaTime, A3DVECTOR3& vDeltaDist );