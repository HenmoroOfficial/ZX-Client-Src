/*
 * FILE: EC_Interactor.h
 *
 * DESCRIPTION: 
 *
 * CREATED BY: Li Bin, 2013.07.22
 *
 * HISTORY: 
 *
 * Copyright (c) 2004 Archosaur Studio, All Rights Reserved.
 */

#pragma once

#include "EC_NPC.h"
#include "ExpTypes.h"

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

struct INTERACTION_OBJECT_ESSENCE;

///////////////////////////////////////////////////////////////////////////
//	
//	Declare of Global functions
//	
///////////////////////////////////////////////////////////////////////////


///////////////////////////////////////////////////////////////////////////
//	
//	Class CECInteractor
//	
///////////////////////////////////////////////////////////////////////////

class CECInteractor : public CECNPC
{
public:		//	Types
	enum
	{
		MAX_INTERACTIVED_PLAYER_NUM = 6,
	};

	enum INTERACTIVE_STATE
	{
		NO_INTERACTIVE = 0,
		START_INTERACT,
		IN_INTERACT,
		END_INTERACT,
	};

	struct InteractiveData
	{
		int					m_iPlayerID;
		int					m_iAttachTime;
		int					m_iDetachTime;
		INTERACTIVE_STATE	m_sState;
		CECModel *			m_pPlayerModel;
		AString				m_sPlayerAction;
		AString				m_sPlayerMoveAction;
		AString				m_sObjAction;
		AString				m_sObjMoveAction;
	};

public:		//	Constructor and Destructor

	CECInteractor(CECNPCMan* pNPCMan);
	virtual ~CECInteractor();

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

	virtual bool IsInteractive() { return m_bAttached; }//{ return m_iInteractiveNum != 0; }

	//	Get NPC name color
	virtual DWORD GetNameColor();

	virtual float GetSize();

	const INTERACTION_OBJECT_ESSENCE*	GetDBEssence()					{ return m_pDBEssence; }
	const MONSTER_ESSENCE*				GetMonstDBEssence()				{ return m_pMonEssence; }
	CECModel *							GetNPCModel()					{ return m_pNPCModel; }	
	const InteractiveData&				GetInteractiveData(int pos)		{ return m_itInteractiveData[pos]; }

	bool StartInteract(int playerid, int pos = 0);
	bool InInteract(int playerid, int pos = 0);
	bool EndInteract(int playerid, int pos = 0);
	bool FinishInteract(int playerid, int pos = 0);
	bool CancelInteract(int playerid);

protected:	//	Attributes

	//	Data in database
	INTERACTION_OBJECT_ESSENCE*	m_pDBEssence;
	ACString			m_strCompName;		//	Complete name with summoner's name

	//	Interactive players' ids.
	InteractiveData		m_itInteractiveData[MAX_INTERACTIVED_PLAYER_NUM];

	int					m_iInteractiveNum;
	bool				m_bShowUI;
	bool				m_bAttached;

protected:	//	Operations
	void BuildCompleteName();

	inline bool	IsSubject()			{ return m_pDBEssence->main_body == 0; }
	inline bool	IsRenderSubject()	{ return m_pDBEssence->client_main_body == 0; }

	void AttachPlayerModel(int pos);
	void DetachPlayerModel(int pos);
};

///////////////////////////////////////////////////////////////////////////
//	
//	Inline functions
//	
///////////////////////////////////////////////////////////////////////////



