/*
 * FILE: EC_Manager.h
 *
 * DESCRIPTION: 
 *
 * CREATED BY: Duyuxin, 2004/8/30
 *
 * HISTORY: 
 *
 * Copyright (c) 2004 Archosaur Studio, All Rights Reserved.
 */

#pragma once

///////////////////////////////////////////////////////////////////////////
//	
//	Types and Global variables
//	
///////////////////////////////////////////////////////////////////////////

class ECViewport;
class CECGameRun;
struct ECRAYTRACE;
struct ECMSG;
class A3DViewport;

///////////////////////////////////////////////////////////////////////////
//	
//	Declare of Global functions
//	
///////////////////////////////////////////////////////////////////////////


///////////////////////////////////////////////////////////////////////////
//	
//	Class ECManager
//	
///////////////////////////////////////////////////////////////////////////

class ECManager
{
public:		//	Types

public:		//	Constructor and Destructor

	ECManager(CECGameRun* pGameRun);
	virtual ~ECManager() {}

public:		//	Attributes

public:		//	Operations

	//	Release manager
	virtual void Release() {}

	//	Tick routine
	virtual bool Tick(DWORD dwDeltaTime) { return true; }
	//	Render routine, for registering objects
	virtual bool Render(ECViewport* pViewport) = 0;
    //  Render objects after scene rendered
    virtual bool RenderPost(ECViewport* pViewport) = 0;
	//	RenderForReflected routine
	virtual bool RenderForReflect(ECViewport* pViewport) { return true; }
	//	RenderForRefract routine
	virtual bool RenderForRefract(ECViewport* pViewport) { return true; }
	//	RenderForShadow routine
	virtual bool RenderForShadow(ECViewport* pViewport) { return true; }
	//	RenderForBloom routine
	virtual bool RenderForBloom(ECViewport * pViewport) { return true; }
	
	virtual bool RenderForSilhouette(ECViewport* pViewport) { return true; }


	virtual bool ZPrePass(A3DViewport* pViewport) { return true; }

	//	Tick animation
	virtual bool TickAnimation() { return true; }

	//	Process message
	virtual bool ProcessMessage(const ECMSG& Msg) { return true; }

	//	On entering game world
	virtual bool OnEnterGameWorld() { return true; }
	//	On leaving game world
	virtual bool OnLeaveGameWorld() { return true; }

	//	Ray trace
	virtual bool RayTrace(ECRAYTRACE* pTraceInfo) { return false; }

	//	Get tick time (ms) of this frame
	DWORD GetTickTime() { return m_dwCurTickTime; }
	//	Get manager ID
	int GetManagerID() { return m_iManagerID; }
    
protected:	//	Attributes

	int			m_iManagerID;		//	Manager's ID
	CECGameRun* m_pGameRun;			//	Game run object
	DWORD		m_dwCurTickTime;	//	Logic tick time

protected:	//	Operations

	//	Begin recroding tick time
	void BeginTickTime();
	//	End recroding tick time
	void EndTickTime();
};

///////////////////////////////////////////////////////////////////////////
//	
//	Inline functions
//	
///////////////////////////////////////////////////////////////////////////



