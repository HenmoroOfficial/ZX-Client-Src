/*
 * FILE: EC_IvtrPetItem.cpp
 *
 * DESCRIPTION: 
 *
 * CREATED BY: Duyuxin, 2005/12/27
 *
 * HISTORY: 
 *
 * Copyright (c) 2005 Archosaur Studio, All Rights Reserved.
 */

#include "EC_Global.h"
#include "EC_IvtrPetItem.h"
#include "EC_Game.h"
#include "EC_FixedMsg.h"
#include "EC_RTDebug.h"
#include "EC_GameRun.h"
#include "EC_HostPlayer.h"
#include "EC_PetCorral.h"
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
//	Implement CECIvtrPetEgg
//	
///////////////////////////////////////////////////////////////////////////

CECIvtrPetEgg::CECIvtrPetEgg(int tid) : CECIvtrItem(tid)
{
	m_iCID	= ICID_PETEGG;

	//	Get database data
	elementdataman* pDB = g_pGame->GetElementDataMan();
	DATA_TYPE DataType;
	m_pDBEssence	= (PET_EGG_ESSENCE*)pDB->get_data_ptr(tid, ID_SPACE_ESSENCE, DataType);
	m_pPetEssence	= (PET_ESSENCE*)pDB->get_data_ptr(m_pDBEssence->id_pet, ID_SPACE_ESSENCE, DataType);
	ASSERT(m_pPetEssence);

	m_iPileLimit	= m_pDBEssence->pile_num_max;
	m_iPrice		= m_pDBEssence->price;
	m_iShopPrice	= m_pDBEssence->shop_price;
	m_iProcType		= m_pDBEssence->proc_type;
	m_iEquipMask	= 0;
//	m_bUseable		= true;
//	m_dwUseFlags	= USE_PERSIST;

	memset(&m_Essence, 0, sizeof(m_Essence));
}

CECIvtrPetEgg::CECIvtrPetEgg(const CECIvtrPetEgg& s) : CECIvtrItem(s)
{
	m_pDBEssence	= s.m_pDBEssence;
	m_Essence		= s.m_Essence;

	int i, iNumSkill = s.m_aSkills.GetSize();
	m_aSkills.SetSize(iNumSkill, 4);

	for (i=0; i < iNumSkill; i++)
		m_aSkills[i] = s.m_aSkills[i];
}

CECIvtrPetEgg::~CECIvtrPetEgg()
{
}

//	Set item detail information
bool CECIvtrPetEgg::SetItemInfo(BYTE* pInfoData, int iDataLen)
{
	CECIvtrItem::SetItemInfo(pInfoData, iDataLen);

	try
	{
		CECDataReader dr(pInfoData, iDataLen);

		m_Essence = *(IVTR_ESSENCE_PETEGG*)dr.Read_Data(sizeof (IVTR_ESSENCE_PETEGG));

		m_aSkills.SetSize(m_Essence.skill_count, 4);
		for (int i=0; i < m_Essence.skill_count; i++)
		{
			PETSKILL& s = m_aSkills[i];
			s.idSkill = dr.Read_int();
			s.iLevel = dr.Read_int();
		}
	}
	catch (CECException& e)
	{
		ASSERT(0);
		a_LogOutput(1, "CECIvtrPetEgg::SetItemInfo, data read error (%d)", e.GetType());
		return false;
	}

	return true;
}

//	Get item icon file name
const char* CECIvtrPetEgg::GetIconFile()
{
	return m_pDBEssence->file_icon;
}

//	Get item name
const wchar_t* CECIvtrPetEgg::GetName()
{
	return m_pDBEssence->name;
}

//	Get item description text
const wchar_t* CECIvtrPetEgg::GetNormalDesc(bool bRepair)
{
	return GetRidingPetDesc(bRepair);
/*	const char wchar_t* szDesc = _AL("");
	switch (m_Essence.pet_class)
	{
	case GP_PET_CLASS_MOUNT:	szDesc = GetRidingPetDesc(bRepair);		break;
	case GP_PET_CLASS_COMBAT:	szDesc = GetCombatPetDesc(bRepair);		break;
	case GP_PET_CLASS_FOLLOW:	szDesc = GetFollowPetDesc(bRepair);		break;
	}

	return szDesc;
*/
}

//	Build riding pet description text
const wchar_t* CECIvtrPetEgg::GetRidingPetDesc(bool bRepair)
{
	if (m_bNeedUpdate)
		return NULL;

	m_strDesc = _AL("");

	//	Try to build item description
	CECStringTab* pDescTab = g_pGame->GetItemDesc();
	CECHostPlayer* pHost = g_pGame->GetGameRun()->GetHostPlayer();
	
	int white = ITEMDESC_COL_WHITE;
	int red = ITEMDESC_COL_RED;

	//	Item name
	if (m_iCount > 1)
		AddDescText(white, true, pDescTab->GetWideString(ITEMDESC_NAMENUMBER), GetName(), m_iCount);
	else
		AddDescText(white, true, pDescTab->GetWideString(ITEMDESC_NAME), GetName());

	//	Food type requirement
	AddFoodTypeDesc();

	if (m_pPetEssence)
	{
		//	Pet level
		AddDescText(-1, true, pDescTab->GetWideString(ITEMDESC_PETLEVEL), m_Essence.level);

		//	Move speed
		float fSpeed = m_pPetEssence->speed_a + (m_Essence.level - 1) * m_pPetEssence->speed_b;
		AddDescText(white, true, pDescTab->GetWideString(ITEMDESC_MOVESPEED), fSpeed);
		
		//	Profession requirement
		AddProfReqDesc(m_pPetEssence->character_combo_id);
	}

	//	Level requirement
	int iLevelReq = a_Max((int)m_Essence.level, m_Essence.req_level);
	if (iLevelReq)
	{
		int col = pHost->GetBasicProps().iLevel >= iLevelReq ? white : red;
		AddDescText(col, true, pDescTab->GetWideString(ITEMDESC_LEVELREQ), iLevelReq);
	}

	//	Price
	AddPriceDesc(white, bRepair);

	//	Extend description
	AddExtDescText();

	return m_strDesc;
}

//	Build combat pet description text
const wchar_t* CECIvtrPetEgg::GetCombatPetDesc(bool bRepair)
{
	return m_strDesc;
}

//	Build follow pet description text
const wchar_t* CECIvtrPetEgg::GetFollowPetDesc(bool bRepair)
{
	return m_strDesc;
}

//	Add food type requirment description
void CECIvtrPetEgg::AddFoodTypeDesc()
{
	elementdataman* pDB = g_pGame->GetElementDataMan();
	DATA_TYPE DataType;
	PET_ESSENCE* pPetEssence = (PET_ESSENCE*)pDB->get_data_ptr(m_Essence.pet_tid, ID_SPACE_ESSENCE, DataType);
	if (!pPetEssence)
		return;

	CECStringTab* pDescTab = g_pGame->GetItemDesc();
	AddDescText(ITEMDESC_COL_WHITE, false, pDescTab->GetWideString(ITEMDESC_FOODTYPE));

	for (int i=0; i < MAX_PET_FOOD; i++)
	{
		if (pPetEssence->food_mask & (1 << i))
		{
			m_strDesc += _AL(" ");
			AddDescText(-1, false, pDescTab->GetWideString(ITEMDESC_FOOD_GRASS+i));
		}
	}

	m_strDesc += _AL("\\r");
}

///////////////////////////////////////////////////////////////////////////
//	
//	Implement CECIvtrPetFood
//	
///////////////////////////////////////////////////////////////////////////

CECIvtrPetFood::CECIvtrPetFood(int tid) : CECIvtrItem(tid)
{
	m_iCID	= ICID_PETFOOD;

	//	Get database data
	elementdataman* pDB = g_pGame->GetElementDataMan();
	DATA_TYPE DataType;
	m_pDBEssence	= (PET_FOOD_ESSENCE*)pDB->get_data_ptr(tid, ID_SPACE_ESSENCE, DataType);

	m_iPileLimit	= m_pDBEssence->pile_num_max;
	m_iPrice		= m_pDBEssence->price;
	m_iShopPrice	= m_pDBEssence->shop_price;
	m_iProcType		= m_pDBEssence->proc_type;
	m_iEquipMask	= 0;
	m_bUseable		= true;

	m_bNeedUpdate	= false;
}

CECIvtrPetFood::CECIvtrPetFood(const CECIvtrPetFood& s) : CECIvtrItem(s)
{
	m_pDBEssence	= s.m_pDBEssence;
}

CECIvtrPetFood::~CECIvtrPetFood()
{
}

//	Set item detail information
bool CECIvtrPetFood::SetItemInfo(BYTE* pInfoData, int iDataLen)
{
	CECIvtrItem::SetItemInfo(pInfoData, iDataLen);
	return true;
}

//	Get item icon file name
const char* CECIvtrPetFood::GetIconFile()
{
	return m_pDBEssence->file_icon;
}

//	Get item name
const wchar_t* CECIvtrPetFood::GetName()
{
	return m_pDBEssence->name;
}

//	Get item cool time
int CECIvtrPetFood::GetCoolTime(int* piMax/* NULL */)
{
	CECHostPlayer* pHost = g_pGame->GetGameRun()->GetHostPlayer();
	if (!pHost)
		return 0;

	int iTime = pHost->GetCoolTime(GP_CT_FEED_PET, piMax);
	return iTime;
}

//	Check item use condition
bool CECIvtrPetFood::CheckUseCondition()
{
	CECPetData* pPet = g_pGame->GetGameRun()->GetHostPlayer()->GetPetCorral()->GetActivePet();
	if (!pPet)
		return false;

	DATA_TYPE DataType;
	const PET_ESSENCE* pPetEssence = (const PET_ESSENCE*)g_pGame->GetElementDataMan()->get_data_ptr(pPet->GetTemplateID(), ID_SPACE_ESSENCE, DataType);
	if (!pPetEssence)
		return false;
	if (!CheckCrossUseCondition())
		return false; 
	return (m_pDBEssence->food_type & pPetEssence->food_mask) ? true : false;
}

//	Get item description text
const wchar_t* CECIvtrPetFood::GetNormalDesc(bool bRepair)
{
	m_strDesc = _AL("");

	//	Try to build item description
	CECStringTab* pDescTab = g_pGame->GetItemDesc();
	int white = ITEMDESC_COL_WHITE;

	//	Item name
	if (m_iCount > 1)
		AddDescText(white, true, pDescTab->GetWideString(ITEMDESC_NAMENUMBER), GetName(), m_iCount);
	else
		AddDescText(white, true, pDescTab->GetWideString(ITEMDESC_NAME), GetName());

	//	Food type
	for (int i=0; i < MAX_PET_FOOD; i++)
	{
		if (m_pDBEssence->food_type & (1 << i))
		{
			AddDescText(white, false, pDescTab->GetWideString(ITEMDESC_FOODTYPE));
			m_strDesc += _AL(" ");
			AddDescText(-1, true, pDescTab->GetWideString(ITEMDESC_FOOD_GRASS+i));
			break;
		}
	}

	//	Price
	AddPriceDesc(white, bRepair);

	//	Extend description
	AddExtDescText();

	return m_strDesc;
}
