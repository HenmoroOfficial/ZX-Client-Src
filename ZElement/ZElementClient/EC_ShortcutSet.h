/*
 * FILE: EC_ShortcutSet.h
 *
 * DESCRIPTION: 
 *
 * CREATED BY: Duyuxin, 2005/1/5
 *
 * HISTORY: 
 *
 * Copyright (c) 2005 Archosaur Studio, All Rights Reserved.
 */

#pragma once

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

class CECShortcut;
class CECIvtrItem;
class CECSkill;

///////////////////////////////////////////////////////////////////////////
//	
//	Declare of Global functions
//	
///////////////////////////////////////////////////////////////////////////


///////////////////////////////////////////////////////////////////////////
//	
//	Class CECShortcutSet
//	
///////////////////////////////////////////////////////////////////////////

class CECShortcutSet
{
public:		//	Types

public:		//	Constructor and Destructor

	CECShortcutSet();
	virtual ~CECShortcutSet();

public:		//	Attributes

public:		//	Operations

	//	Initalize object
	bool Init(int iSize);
	//	Release object
	void Release();

	//	Tick routine
	bool Tick(DWORD dwDeltaTime);

	//	Save shortcut configs to specified buffer
	bool SaveConfigData(void* pDataBuf, int* piSize);
	//	Load shortcut configs from specified buffer
	bool LoadConfigData(const void* pDataBuf, int* piSize, DWORD dwVer);

	//	Create a item shortcut at specified position
	bool CreateItemShortcut(int iSlot, int iIvtr, int iIvtrSlot, CECIvtrItem* pItem);
	//	Create a skill shortcut at specified position
	bool CreateSkillShortcut(int iSlot, CECSkill* pSkill);
	//	Create a skill shortcut can't be used currently at specified position
	bool CreateSkillDumbShortcut(int iSlot, int id);
	//	Create a skill group shortcut at specified position
	bool CreateSkillGroupShortcut(int iSlot, int iGroupIdx);
	//	Duplicate a shortcut to specified position
	bool CreateClonedShortcut(int iSlot, CECShortcut* pShortcut);

	//	Create a target shortcut at specified position
	bool CreateTargetShortcut(int iSlot, int nType, int id, int level,int prof, int feishengcnt, int cultivation);
	
	
	//	Put a shortcut into set
	CECShortcut* PutShortcut(int iSlot, CECShortcut* pShortcut);
	//	Get a shortcut from set
	CECShortcut* GetShortcut(int iSlot, bool bRemove=false);
	//	Set a shortcut into set and release old shortcut at this position automatically
	void SetShortcut(int iSlot, CECShortcut* pShortcut);
	//	Exchange shortcut in inventory
	void ExchangeShortcut(int iSlot1, int iSlot2);
	//	Remove all shortcuts
	void RemoveAllShortcuts();

	//	Update item shortcut when item position changed
	void UpdateMovedItemSC(int tidItem, int iSrcIvtr, int iSrcSlot, int iDstIvtr, int iDstSlot);
	//	Update item shortcut when item removed
	void UpdateRemovedItemSC(int tidItem, int iIvtr, int iSlot, int iSameItem);
	//	Update item shortcut when two items exchanged
	void UpdateExchangedItemSC(int tidItem1, int iIvtr1, int iSlot1, int tidItem2, int iIvtr2, int iSlot2);

	//	Replace skill id in skill shortcuts
	void ReplaceSkillID(int idOld, CECSkill* pNewSkill);

	//	Get shortcut number
	int GetShortcutNum() { return m_aShortcuts.GetSize(); }

protected:	//	Attributes

	APtrArray<CECShortcut*>	m_aShortcuts;	//	Shortcut array

protected:	//	Operations

};

///////////////////////////////////////////////////////////////////////////
//	
//	Inline functions
//	
///////////////////////////////////////////////////////////////////////////



