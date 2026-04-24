/*
 * FILE: EC_Monster.h
 *
 * DESCRIPTION: 
 *
 * CREATED BY: Duyuxin, 2004/9/22
 *
 * HISTORY: 
 *
 * Copyright (c) 2004 Archosaur Studio, All Rights Reserved.
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

struct MONSTER_ESSENCE;

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

class CECMonster : public CECNPC
{
public:		//	Types

public:		//	Constructor and Destructor

	CECMonster(CECNPCMan* pNPCMan);
	virtual ~CECMonster();

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
	//	Is monster in invader camp in battle ?
	virtual bool IsInBattleInvaderCamp();
	//	Is monster in defender camp in battle ?
	virtual bool IsInBattleDefenderCamp();

	virtual bool IsHumanCamp();
	virtual bool IsOrcCamp();
	
	virtual float GetSize();

	const MONSTER_ESSENCE* GetDBEssence() { return m_pDBEssence; }

protected:	//	Attributes

	//	Data in database
	MONSTER_ESSENCE*	m_pDBEssence;
	ACString			m_strCompName;		//	Complete name with summoner's name
protected:	//	Operations
	void BuildCompleteName();
};

///////////////////////////////////////////////////////////////////////////
//	
//	Inline functions
//	
///////////////////////////////////////////////////////////////////////////



