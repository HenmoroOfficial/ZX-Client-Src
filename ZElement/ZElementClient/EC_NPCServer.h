/*
 * FILE: EC_NPCServer.h
 *
 * DESCRIPTION: 
 *
 * CREATED BY: Duyuxin, 2005/1/4
 *
 * HISTORY: 
 *
 * Copyright (c) 2005 Archosaur Studio, All Rights Reserved.
 */

#pragma once

#include "EC_NPC.h"
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

struct NPC_ESSENCE;
struct MONSTER_ESSENCE;

class CECSkill;

///////////////////////////////////////////////////////////////////////////
//	
//	Declare of Global functions
//	
///////////////////////////////////////////////////////////////////////////


///////////////////////////////////////////////////////////////////////////
//	
//	Class CECNPCServer
//	
///////////////////////////////////////////////////////////////////////////

class CECNPCServer : public CECNPC
{
public:		//	Types

	//	Quest icon index
	enum
	{
		QI_NONE = -1,
		QI_OUT = 0,
		QI_OUT_N,
		QI_IN,
		QI_IN_N,
		QI_OUT_K,	
		QI_IN_K,
	};

public:		//	Constructor and Destructor

	CECNPCServer(CECNPCMan* pNPCMan);
	virtual ~CECNPCServer();

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
	virtual bool RenderHighLight(CECViewport* pViewport);

	//	Get NPC name color
	virtual DWORD GetNameColor();
	//	Is monster in invader camp in battle ?
	virtual bool IsInBattleInvaderCamp();
	//	Is monster in defender camp in battle ?
	virtual bool IsInBattleDefenderCamp();
	//  该npc是否可以被攻击，解决了战场中怪物的选中，表面是npc，其实是怪物monster。	
	virtual bool IsAttackable();
	
	//  人神pk，是否是人族
	virtual bool IsHumanCamp();
	//  人神pk，是否是神族
	virtual bool IsOrcCamp();

	//	Get skill number
	int GetSkillNum() { return m_aSkills.GetSize(); }
	//	Get skill by index
	CECSkill* GetSkill(int n) { return m_aSkills[n]; }
	//	Set / Get show quest icon flag
	void SetQuestIconFlag(bool bShow) { m_bQuestIcon = bShow; }
	bool GetQuestIconFlag() { return m_bQuestIcon; }
	//	Build skill list
	bool BuildSkillList();
	//	Get current quest icon
	int GetCurQuestIcon() { return m_iQuestIcon; }
	//	Get way point ID bound with this NPC
	DWORD GetWayPointID();
	//	Restore server dir
	void RestorServerDir();
	void StartGreeting();

	//	Get essence data in database
	const NPC_ESSENCE* GetDBEssence() { return m_pDBEssence; }

protected:	//	Attributes

	//	Data in database
	NPC_ESSENCE*		m_pDBEssence;

	float	m_fTaxRate;		//	Tax rate
	bool	m_bQuestIcon;	//	true show quest icon
	int		m_iQuestIcon;	//	Current quest icon
	bool	m_bCityStatue;
	bool	m_bMaleStatue;
	bool	m_bStatueChanged;
	bool	m_bLoadStatueModelFlag;
	bool	m_bGetStatueNameFlag;
	int		m_nStatueIndex;
	int		m_nCityMasterID;

	A3DVECTOR3				m_vServerDir;
	APtrArray<CECSkill*>	m_aSkills;	//	Skill array

protected:	//	Operations

	//	Render quest icon on header
	bool RenderQuestIcon(CECViewport* pViewport);
	//	Update current qeust icon index
	void UpdateCurQuestIcon();

	void UpdateStatueModel();
};

///////////////////////////////////////////////////////////////////////////
//	
//	Inline functions
//	
///////////////////////////////////////////////////////////////////////////

