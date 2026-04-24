/*
 * FILE: EC_CameraPath.h
 *
 * DESCRIPTION: 
 *
 * CREATED BY: Xiao Zhou, 2008/12/10
 *
 * HISTORY: 
 *
 * Copyright (c) 2008 Archosaur Studio, All Rights Reserved.
 */

#pragma once

#include "EC_Bezier.h"
#include "EC_Counter.h"
#include "EC_DataExpectation.h"

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

class CLuaState;

///////////////////////////////////////////////////////////////////////////
//	
//	Declare of Global functions
//	
///////////////////////////////////////////////////////////////////////////


///////////////////////////////////////////////////////////////////////////
//	
//	Class CECCameraPath
//	
///////////////////////////////////////////////////////////////////////////

class CECCameraPath
{
public:		//	Constructor and Destructor

	CECCameraPath();
	~CECCameraPath(){};

public:		//	Attributes

public:		//	Operations
	void Tick(DWORD dwDeltaTime);

	bool StartCameraPath(DWORD dwPathIndex, DWORD* pIDBezier=NULL, DWORD dwWaitMilliseconds=0);
	void EndCameraPath();
	bool IsUsingCameraPath();
	bool HasDelayedCameraPath();
	CECBezier* CECCameraPath::GetBezier(DWORD dwPathIndex);
	
	void SetStayTime(DWORD dwTime) { m_counter.SetPeriod(dwTime); m_counter.Reset(false); }


	A3DVECTOR3 GetPos();
	A3DVECTOR3 GetDir();
	float GetRatio();
	float GetFov();

protected:		//	Operations
	void _DelayCameraPath(const unsigned int pathIndex);
	void _ProcessDelayedCamera();

	void _OnStartCamera(DWORD dwPathIndex);

protected:	//	Attributes
	
	CECBezierWalker m_CameraPathWalker;
	bool m_bUsingCameraPath;

	int  m_pathIndex;
	//params about camera.
	int  m_id;

	bool m_bLastShowUI;
	DWORD m_pathIndexDelayed;
	CECCounter m_counter;
	CECDataExpUInt m_dwTickTimeExp;

};

///////////////////////////////////////////////////////////////////////////
//	
//	Inline functions
//	
///////////////////////////////////////////////////////////////////////////


