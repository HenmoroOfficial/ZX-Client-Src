/*
 * FILE: EC_Pet.h
 *
 * DESCRIPTION: 
 *
 * CREATED BY: Hedi, 2005/12/5
 *
 * HISTORY: 
 *
 * Copyright (c) 2005 Archosaur Studio, All Rights Reserved.
 */

#pragma once

#include "EC_NPC.h"

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

struct PET_BEDGE_ESSENCE;
class A3DGFXEx;

///////////////////////////////////////////////////////////////////////////
//	
//	Declare of Global functions
//	
///////////////////////////////////////////////////////////////////////////


///////////////////////////////////////////////////////////////////////////
//	
//	Class CECMonster
//	
///////////////////////////////////////////////////////////////////////////

class CECPet : public CECNPC
{
public:		//	Types

public:		//	Constructor and Destructor

	CECPet(CECNPCMan* pNPCMan);
	virtual ~CECPet();

public:		//	Attributes

public:		//	Operations

	//	Initlaize object
	virtual bool Init(int tid, const S2C::info_npc& Info);
	//	Release object
	virtual void Release();

	//	Set loaded model to NPC object, this function is used in multithread loading process
	virtual bool SetNPCLoadedResult(const EC_NPCLOADRESULT& Ret);

	//	Tick routine
	virtual bool Tick(DWORD dwDeltaTime);
	//	Render routine
	virtual bool Render(CECViewport* pViewport, int iRenderFlag=0);

	//	Get NPC name color
	virtual DWORD GetNameColor();
	virtual float GetSize();

	const PET_BEDGE_ESSENCE* GetDBEssence() { return m_pDBEssence; }
	int GetShape() { return m_iShape; }
	int GetFace() { return m_iFace; }

	// param: 协议信息
	void SetPetProp(DWORD param);

protected:	//	Attributes

	//	Data in database
	PET_BEDGE_ESSENCE*	m_pDBEssence;
	ACString			m_strCompName;		//	Complete name with master's name
	int					m_iShape;
	int					m_iFace;
	bool				m_bIsFlying;
	A3DGFXEx*			m_pGFXFlyOn;

protected:	//	Operations

	//	Build complete name
	void BuildCompleteName();

};

///////////////////////////////////////////////////////////////////////////
//	
//	Inline functions
//	
///////////////////////////////////////////////////////////////////////////



