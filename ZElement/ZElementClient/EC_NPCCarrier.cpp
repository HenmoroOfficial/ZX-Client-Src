/*
 * FILE: EC_NPCCarrier.h
 *
 * DESCRIPTION: 
 *
 * CREATED BY: Wangrui, 2009/8/17
 *
 * HISTORY: 
 *
 * Copyright (c) 2008 Archosaur Studio, All Rights Reserved.
 */

#include "EC_Global.h"
#include "EC_NPCCarrier.h"

///////////////////////////////////////////////////////////////////////////
//	
//	Define and Macro
//	
///////////////////////////////////////////////////////////////////////////


///////////////////////////////////////////////////////////////////////////
//	
//	Reference to External variables and functions
//	
///////////////////////////////////////////////////////////////////////////


///////////////////////////////////////////////////////////////////////////
//	
//	Local Types and Variables and Global variables
//	
///////////////////////////////////////////////////////////////////////////


///////////////////////////////////////////////////////////////////////////
//	
//	Local functions
//	
///////////////////////////////////////////////////////////////////////////


///////////////////////////////////////////////////////////////////////////
//	
//	Implement CECNPCCarrier
//	
///////////////////////////////////////////////////////////////////////////

CECNPCCarrier::CECNPCCarrier(CECNPCMan* pNPCMan) : CECNPCServer(pNPCMan)
{
	m_iCID = OCID_CARRIER;
	m_vCurVel = A3DVECTOR3(0.0f, 0.0f, 0.0f);
}

CECNPCCarrier::~CECNPCCarrier()
{
}

//	Release object
void CECNPCCarrier::Release()
{
	CECNPCServer::Release();
}

//	Tick routine
bool CECNPCCarrier::Tick(DWORD dwDeltaTime)
{
	A3DVECTOR3 vLastPos = GetPos();
	CECNPCServer::Tick(dwDeltaTime);
	A3DVECTOR3 vDelta = GetPos() - vLastPos;
	m_vCurVel = vDelta / ((float)(dwDeltaTime/1000.0));
	//	Rebuild trace brushes.
	//	Note: this is temp code. Call RebuildTraceBrush() here will cause
	//		brushes are built twice in one tick when this NPC is in stand state.
    //	RebuildTraceBrush();

	return true;
}
