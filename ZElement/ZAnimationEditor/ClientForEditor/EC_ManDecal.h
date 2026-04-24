/*
 * FILE: EC_ManDecal.h
 *
 * DESCRIPTION: 
 *
 * CREATED BY: Duyuxin, 2005/1/18
 *
 * HISTORY: 
 *
 * Copyright (c) 2005 Archosaur Studio, All Rights Reserved.
 */

#pragma once

#ifdef BACK_VERSION
#include "EC_Manager.h"
#else
#include "ECManager.h"
#include "ECManagerID.h"
#endif
#include "AList2.h"
#include "A3DTypes.h"
#include "AArray.h"

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

class CECDecal;
class CECBubbleDecal;
class CECPateText;
#ifndef BACK_VERSION
#define CECViewport ECViewport
#endif

///////////////////////////////////////////////////////////////////////////
//	
//	Declare of Global functions
//	
///////////////////////////////////////////////////////////////////////////


///////////////////////////////////////////////////////////////////////////
//	
//	Class CECDecalMan
//	
///////////////////////////////////////////////////////////////////////////

#ifdef BACK_VERSION
class CECDecalMan : public CECManager
#else
class CECDecalMan : public ECManager
#endif
{
public:		//	Types

public:		//	Constructor and Destructor

	CECDecalMan(CECGameRun* pGameRun);
	virtual ~CECDecalMan();

public:		//	Attributes

public:		//	Operations

	//	Release manager
	virtual void Release();

	//	Tick routine
	virtual bool Tick(DWORD dwDeltaTime);
	//	Render routine
#ifdef BACK_VERSION
	virtual bool Render(CECViewport* pViewport);
#else
	virtual bool RenderPost(ECViewport* pViewport);
	virtual bool Render(ECViewport* pViewport) { return true; }
#endif

	//	On entering game world
	virtual bool OnEnterGameWorld();
	//	On leaving game world
	virtual bool OnLeaveGameWorld();

	//	Create a bubble decal
	CECBubbleDecal* CreateBubbleDecal(int iDCID);
	//	Register a normal decal prepare to be rendered
	void RegisterDecal(CECDecal* pDecal) { m_aDecals.Add(pDecal); }
	//	Register pate text prepare to be rendered
	void RegisterPateText(CECPateText* pPateText) { m_aPateTexts.Add(pPateText); }
	//	Render registered pate text
	bool RenderPateTexts(CECViewport* pViewport);

protected:	//	Attributes

	APtrList<CECBubbleDecal*>	m_BubbleList;	//	Bubble decal list
	APtrArray<CECDecal*>		m_aDecals;		//	Normal decal array
	APtrArray<CECPateText*>		m_aPateTexts;	//	Registered pate texts

protected:	//	Operations

};

///////////////////////////////////////////////////////////////////////////
//	
//	Inline functions
//	
///////////////////////////////////////////////////////////////////////////


