/*
 * FILE: EC_PetCorral.h
 *
 * DESCRIPTION: 
 *
 * CREATED BY: Duyuxin, 2005/12/23
 *
 * HISTORY: 
 *
 * Copyright (c) 2005 Archosaur Studio, All Rights Reserved.
 */

#pragma once

#include "AAssist.h"

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

namespace S2C
{
	struct info_pet;
}

///////////////////////////////////////////////////////////////////////////
//	
//	Declare of Global functions
//	
///////////////////////////////////////////////////////////////////////////


///////////////////////////////////////////////////////////////////////////
//	
//	Class CECPetData
//	
///////////////////////////////////////////////////////////////////////////

class CECPetData
{
public:		//	Types

	//	Hunger level
	enum
	{
		HUNGER_LEVEL_0,			//	饱食
		HUNGER_LEVEL_1,			//	正常
		HUNGER_LEVEL_2,			//	饿程度一级 
		HUNGER_LEVEL_3,		
		HUNGER_LEVEL_4,			//	饿程度二级
		HUNGER_LEVEL_5,		
		HUNGER_LEVEL_6,		
		HUNGER_LEVEL_7,			//	饿程度三级
		HUNGER_LEVEL_8,		
		HUNGER_LEVEL_9,		
		HUNGER_LEVEL_10,		
		HUNGER_LEVEL_11,		//	饿程度四级
		HUNGER_LEVEL_COUNT,
	};

	//	Intimacy level
	enum
	{
		INTIMACY_LEVEL_0,		//	野性难驯, 0-50
		INTIMACY_LEVEL_1,		//	反复无偿, 51-150
		INTIMACY_LEVEL_2,		//	乖巧听话, 151-500
		INTIMACY_LEVEL_3,		//	忠心耿耿, 501-999
		INTIMACY_LEVEL_COUNT,
		INTIMACY_POINT_MAX = 999,
	};

	enum
	{
		MAX_SKILLNUM = 16,	
	};

	struct PETSKILL
	{
		int idSkill;
		int iLevel;
	};

public:		//	Constructor and Destructor

	CECPetData();
	virtual ~CECPetData();

public:		//	Attributes

public:		//	Operations

	//	Initialize object
	bool Init(const S2C::info_pet& Info);

	//	Add experience
	int AddExp(int iExp);
	//	Level up
	int LevelUp(int iLevel, int iNewExp);

	//	Get properties
	int GetIntimacy() const { return m_iIntimacy; }
	void SetIntimacy(int iValue) { m_iIntimacy = iValue; }
	int GetHunger() const { return m_iHunger; }
	void SetHunger(int iValue) { m_iHunger = iValue; }
	int	GetTemplateID() const { return m_tid; }
	int GetVisibleID() const { return m_tidVis; }
	int GetEggID() const { return m_idEgg; }
	int GetClass() const { return m_iClass; }
	float GetHPFactor() const { return m_fHPFactor; }
	int GetLevel() const { return m_iLevel; }
	int GetLevel2() const { return m_iLevel2; }
	int GetExp() const { return m_iExp; }
	int GetSkillPt() const { return m_iSkillPt; }
	DWORD GetColor() const { return m_dwCol; }
	const ACHAR* GetName() const { return m_strName; }

	//	Get skill
	const PETSKILL* GetSkill(int n)
	{
		if (n >= 0 && n < MAX_SKILLNUM)
			return &m_aSkills[n];
		else
		{
			ASSERT(0);
			return NULL;
		}
	}

protected:	//	Attributes

	int			m_iIntimacy; 	//	好感度
	int			m_iHunger;		//	饥饿度
	int			m_tid;       	//	宠物的模板ID
	int			m_tidVis;     	//	宠物形象的模板ID（如果为0，则表示无特殊可见ID）
	int			m_idEgg;     	//	宠物蛋的ID
	int			m_iClass;    	//	宠物类型 战宠，骑宠，观赏宠
	float		m_fHPFactor; 	//	血量比例（复活和收回时使用） 0则为死亡
	int			m_iLevel;    	//	宠物级别
	int			m_iLevel2;   	//	第二级别
	int			m_iExp;      	//	宠物当前经验
	int			m_iSkillPt;  	//	剩余技能点
	DWORD		m_dwCol;		//	颜色, 目前此项无效
	ACString	m_strName;		

	PETSKILL	m_aSkills[MAX_SKILLNUM];

protected:	//	Operations

};

///////////////////////////////////////////////////////////////////////////
//	
//	Class CECPetCorral
//	
///////////////////////////////////////////////////////////////////////////

class CECPetCorral
{
public:		//	Types

	enum
	{
		MAX_SLOTNUM	= 8,
	};

public:		//	Constructor and Destructor

	CECPetCorral();
	virtual ~CECPetCorral();

public:		//	Attributes

public:		//	Operations

	//	Add a pet
	bool AddPet(int iSlot, const S2C::info_pet& Info);
	//	Free a pet
	void FreePet(int iSlot, int idPet);
	//	Remove all pets
	void RemoveAll();

	//	Magnify pet slots
	void MagnifyPetSlots(int iNewNum);
	//	Check whether corral has empty slots
	int GetEmptySlotNum();
	//	Get number of current active pet slots
	int GetPetSlotNum() { return m_iPetSlotNum; }
	//	Get pet data of specified slot
	CECPetData* GetPetData(int iSlot)
	{
		if (iSlot < 0 || iSlot >= MAX_SLOTNUM)
		{
			ASSERT(iSlot >= 0 && iSlot < MAX_SLOTNUM);
			return NULL;
		}

		return m_aPetSlots[iSlot];
	}

	//	Get current active pet's index
	int GetActivePetIndex() { return m_iActivePet; }
	//	Set current active pet's index
	void SetActivePetIndex(int iIndex) { m_iActivePet = iIndex; }
	//	Get current active pet's data
	CECPetData* GetActivePet()
	{
		if (m_iActivePet >= 0 && m_iActivePet < MAX_SLOTNUM)
			return m_aPetSlots[m_iActivePet];
		else
			return NULL;
	}

protected:	//	Attributes

	int			m_iActivePet;				//	Index of current active pet
	int			m_iPetSlotNum;				//	Number of current active pet slots
	CECPetData*	m_aPetSlots[MAX_SLOTNUM];	//	Pet slots
		
protected:	//	Operations

};

///////////////////////////////////////////////////////////////////////////
//	
//	Inline functions
//	
///////////////////////////////////////////////////////////////////////////

