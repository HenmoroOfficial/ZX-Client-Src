/*
 * FILE: EC_PetCorral.cpp
 *
 * DESCRIPTION: 
 *
 * CREATED BY: Duyuxin, 2005/12/23
 *
 * HISTORY: 
 *
 * Copyright (c) 2005 Archosaur Studio, All Rights Reserved.
 */

#include "EC_Global.h"
#include "EC_PetCorral.h"
#include "EC_GPDataType.h"
#include "EC_Game.h"
#include "elementdataman.h"

#define new A_DEBUG_NEW

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
//	Implement CECPetData
//	
///////////////////////////////////////////////////////////////////////////

CECPetData::CECPetData()
{
	m_iIntimacy	= 0;
	m_iHunger	= 0;
	m_tid		= 0;
	m_tidVis	= 0;
	m_idEgg		= 0;
	m_iClass	= GP_PET_CLASS_INVALID;
	m_fHPFactor	= 0.0f;
	m_iLevel	= 0;
	m_iLevel2	= 0;
	m_iExp		= 0;
	m_iSkillPt	= 0;
	m_dwCol		= 0;

	memset(m_aSkills, 0, sizeof (m_aSkills));
}

CECPetData::~CECPetData()
{
}

//	Initialize object
bool CECPetData::Init(const S2C::info_pet& Info)
{
	m_iIntimacy	= Info.honor_point;
	m_iHunger	= Info.hunger;
	m_tid		= Info.pet_tid;
	m_tidVis	= Info.pet_vis_tid;
	m_idEgg		= Info.pet_egg_tid;
	m_iClass	= Info.pet_class;
	m_fHPFactor	= Info.hp_factor;
	m_iLevel	= Info.level;
	m_iLevel2	= Info.sec_level;
	m_iExp		= Info.exp;
	m_iSkillPt	= Info.skill_point;
	m_dwCol		= Info.color;
	
//	m_strName = ACString((const ACHAR*)Info.name[0], Info.name_len / sizeof (ACHAR));
	//	Temp code: use pet name in database
//	DATA_TYPE DataType;
//	PET_ESSENCE* pe = (PET_ESSENCE*)g_pGame->GetElementDataMan()->get_data_ptr(m_tid, ID_SPACE_ESSENCE, DataType);
//	m_strName = pe->name;

	for (int i=0; i < GP_PET_SKILL_NUM; i++)
	{
		m_aSkills[i].idSkill = Info.skills[i].skill;
		m_aSkills[i].iLevel	 = Info.skills[i].level;
	}

	return true;
}

//	Add experience
int CECPetData::AddExp(int iExp)
{
	m_iExp += iExp;
	return m_iExp;
}

//	Level up
int CECPetData::LevelUp(int iLevel, int iNewExp)
{
	m_iLevel = iLevel;
	m_iExp = iNewExp;
	return m_iLevel;
}

///////////////////////////////////////////////////////////////////////////
//	
//	Implement CECPetCorral
//	
///////////////////////////////////////////////////////////////////////////

CECPetCorral::CECPetCorral()
{
	m_iActivePet	= -1;
	m_iPetSlotNum	= 1;
	
	memset(m_aPetSlots, 0, sizeof (m_aPetSlots));
}

CECPetCorral::~CECPetCorral()
{
	CECPetCorral::RemoveAll();
}

//	Add a pet
bool CECPetCorral::AddPet(int iSlot, const S2C::info_pet& Info)
{
	if (iSlot < 0 || iSlot >= m_iPetSlotNum)
	{
		ASSERT(iSlot >= 0 && iSlot < m_iPetSlotNum);
		return false;
	}

	if (m_aPetSlots[iSlot])
	{
		ASSERT(!m_aPetSlots[iSlot]);
		delete m_aPetSlots[iSlot];
		m_aPetSlots[iSlot] = NULL;
	}

	CECPetData* pPet = new CECPetData;
	if (!pPet)
	{
		glb_ErrorOutput(ECERR_NOTENOUGHMEMORY, "CECPetCorral::AddPet", __LINE__);
		return false;
	}

	pPet->Init(Info);

	m_aPetSlots[iSlot] = pPet;

	return true;
}

//	Free a pet
void CECPetCorral::FreePet(int iSlot, int idPet)
{
	if (iSlot < 0 || iSlot >= m_iPetSlotNum)
	{
		ASSERT(iSlot >= 0 && iSlot < m_iPetSlotNum);
		return;
	}

	CECPetData* pPet = m_aPetSlots[iSlot];
	if (pPet)
	{
		ASSERT(pPet->GetTemplateID() == idPet);
		delete pPet;
		m_aPetSlots[iSlot] = NULL;
	}
}

//	Remove all pets
void CECPetCorral::RemoveAll()
{
	for (int i=0; i < MAX_SLOTNUM; i++)
	{
		if (m_aPetSlots[i])
		{
			delete m_aPetSlots[i];
			m_aPetSlots[i] = NULL;
		}
	}
}

//	Magnify pet slots
void CECPetCorral::MagnifyPetSlots(int iNewNum)
{
	if (m_iPetSlotNum > iNewNum)
	{
		ASSERT(m_iPetSlotNum <= iNewNum);
		return;
	}

	m_iPetSlotNum = iNewNum;
}

//	Check whether corral has empty slots
int CECPetCorral::GetEmptySlotNum()
{
	int iCount = 0;

	for (int i=0; i < m_iPetSlotNum; i++)
	{
		if (!m_aPetSlots[i])
			iCount++;
	}

	return iCount;
}