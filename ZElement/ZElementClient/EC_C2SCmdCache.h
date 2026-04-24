/*
 * FILE: EC_C2SCmdCache.h
 *
 * DESCRIPTION: 
 *
 * CREATED BY: Duyuxin, 2005/1/20
 *
 * HISTORY: 
 *
 * Copyright (c) 2005 Archosaur Studio, All Rights Reserved.
 */

#pragma once

#include "hashmap.h"
#include "AList2.h"

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

class CECCounter;

namespace C2S
{
	struct cmd_use_item;
}

///////////////////////////////////////////////////////////////////////////
//	
//	Declare of Global functions
//	
///////////////////////////////////////////////////////////////////////////


///////////////////////////////////////////////////////////////////////////
//	
//	Class CECC2SCmdCache
//	
///////////////////////////////////////////////////////////////////////////

class CECC2SCmdCache
{
public:		//	Types

	typedef abase::hash_map<int, CECCounter*> CounterTable;

public:		//	Constructor and Destructor

	CECC2SCmdCache();
	virtual ~CECC2SCmdCache();

public:		//	Attributes

public:		//	Operations

	//	Remove all un-sent commands in cache
	void RemoveAllCmds();

	//	Tick routine
	bool Tick(DWORD dwDeltaTime);

	//	Send commands ...
	void SendCmdUseItem(BYTE byPackage, BYTE bySlot, int tid, BYTE byCount);
	void SendCmdPickUp(int idItem, int tid);
	void SendCmdSelectTarget(int id);
	void SendCmdExtProps();
	void SendCmdPetProps();
	void SendCmdReviveVillage();
	void SendCmdReviveItem();
	void SendCmdCastSkill(int idSkill, BYTE byPVPMask, int iNumTarget, int* aTargets, A3DVECTOR3* pPos = NULL);	//Modified 2012-05-03.
	void SendCmdCastInstantSkill(int idSkill, BYTE byPVPMask, int iNumTarget, int* aTargets, A3DVECTOR3* pPos = NULL); //Modified 2012-05-03.
	void SendCmdTransfigureSkill(int idSkill, BYTE bySkillType, BYTE byPVPMask, int iNumTarget, int* aTargets,  A3DVECTOR3* pPos = NULL); //Modified 2012-05-05.
	void SendCmdChargeSkill(int idSkill, BYTE byPVPMask, int idTarget, const A3DVECTOR3& vPos, int iNumTarget, int* aTargets);
	void SendCmdEnterSanctuary();
	void SendCmdEnterInstance(int iTransIdx, int idInst);
	void SendCmdActiveRushFly(bool bActive);
	void SendCmdCancelAction();
	void SendCmdNPCSevHello(int nid);
	void SendCmdCastApoiseSkill(int idSkill, BYTE bySkillType, int item_id, short item_index, BYTE force_attack, int iNumTarget, int* aTargets);
	
protected:	//	Attributes

	int		m_idLastPickUpItem;		//	ID of picked up item last time
	int		m_idLastSelTarget;		//	ID of selected item last time
	bool	m_bGetExpProps;
	bool	m_bEnterSanctuary;

	CounterTable	m_CounterMap;

	APtrList<C2S::cmd_use_item*>	m_UseItemCmdList;

protected:	//	Operations

	//	Initialize counters
	bool InitCounters();

	//	Send cached 'use item' command
	void SendCachedCmdUseItem();
	//	Send cached 'get extend properties' command
	void SendCachedCmdGetExtProp();
};

///////////////////////////////////////////////////////////////////////////
//	
//	Inline functions
//	
///////////////////////////////////////////////////////////////////////////



