/*
 * FILE: EC_IvtrConsume.cpp
 *
 * DESCRIPTION: 
 *
 * CREATED BY: Duyuxin, 2005/1/4
 *
 * HISTORY: 
 *
 * Copyright (c) 2005 Archosaur Studio, All Rights Reserved.
 */

#include "EC_Global.h"
#include "EC_IvtrConsume.h"
#include "EC_IvtrRune.h"
#include "EC_FixedMsg.h"
#include "elementdataman.h"
#include "EC_GameRecord.h"
#include "EC_UIManager.h"
#include "EC_GameUIMan.h"
#include "EC_Inventory.h"
#include "globaldataman.h"
#ifndef _INVENTORY_DESC_MAN //Added by Ford.W 2010-07-16.//Modified 2010-07-22.
#include "EC_Game.h"
#include "EC_GameRun.h"
#include "EC_HostPlayer.h"
#include "EC_GameSession.h"
#include "EC_Faction.h"
#else
#include "EC_Game_HintTool.h"
#include "EC_GameRun_HintTool.h"
#include "EC_HostPlayer_HintTool.h"
#include "EC_GameSession_HintTool.h"
#include "EC_Faction_HintTool.h"
#include "AUICommon_HintTool.h"
#endif

#include <AUI\\AUIDialog.h>
#include "AUI\\AUICTranslate.h"
#include "EC_Skill.h"
#include "AFilePackage.h"
#include "luastate.h"

#define new A_DEBUG_NEW

///////////////////////////////////////////////////////////////////////////
//	
//	Define and Macro
//	
///////////////////////////////////////////////////////////////////////////

#define  LOTTERY_TBL_NAME    "Lottery_Desc"

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
//	Implement CECIvtrMedicine
//	
///////////////////////////////////////////////////////////////////////////

CECIvtrMedicine::CECIvtrMedicine(int tid, int expire_date) : CECIvtrItem(tid, expire_date)
{
	m_iCID	= ICID_MEDICINE;

	//	Get database data
	elementdataman* pDB = g_pGame->GetElementDataMan();
	DATA_TYPE DataType;
	m_pDBEssence	= (MEDICINE_ESSENCE*)pDB->get_data_ptr(tid, ID_SPACE_ESSENCE, DataType);
	m_pDBMajorType	= (MEDICINE_MAJOR_TYPE*)pDB->get_data_ptr(m_pDBEssence->id_major_type, ID_SPACE_ESSENCE, DataType);
	m_pDBSubType	= (MEDICINE_SUB_TYPE*)pDB->get_data_ptr(m_pDBEssence->id_sub_type, ID_SPACE_ESSENCE, DataType);
	m_iPileLimit	= m_pDBEssence->pile_num_max;
	m_iPrice		= m_pDBEssence->price;
	m_iShopPrice	= m_pDBEssence->shop_price;
	m_iEquipMask	= 0;
	m_iLevelReq		= m_pDBEssence->require_level;
	m_iProcType		= m_pDBEssence->proc_type;
	m_bUseable		= true;

	if( ( m_pDBEssence->type >= 3 && m_pDBEssence->type <= 5 ) || m_pDBEssence->type == 7 )
		m_bNeedUpdate = false;
}

CECIvtrMedicine::CECIvtrMedicine(const CECIvtrMedicine& s) : CECIvtrItem(s)
{
	m_pDBEssence	= s.m_pDBEssence;
	m_pDBMajorType	= s.m_pDBMajorType;
	m_pDBSubType	= s.m_pDBSubType;
	m_iLevelReq		= s.m_iLevelReq;
	m_Essence		= s.m_Essence;
}

CECIvtrMedicine::~CECIvtrMedicine()
{
}

//	Set item detail information
bool CECIvtrMedicine::SetItemInfo(BYTE* pInfoData, int iDataLen)
{
	switch(m_pDBEssence->type)
	{
	case 0:
		if( iDataLen != sizeof(int) )
			return false;
		m_Essence.hp = *(int *)pInfoData;
		break;

	case 1:
		if( iDataLen != sizeof(int) )
			return false;
		m_Essence.mp = *(int *)pInfoData;
		break;

	case 2:
		if( iDataLen != sizeof(int) * 2 )
			return false;
		m_Essence.hp = ((int *)pInfoData)[0];
		m_Essence.mp = ((int *)pInfoData)[1];
		break;

	case 3:
		break;

	case 4:
		break;

	case 5:
		break;
	case 6:
		if( iDataLen != sizeof(int) )
			return false;
		m_Essence.dp = *(int*)pInfoData;
		break;
	case 7:
		break;
	}

	CECIvtrItem::SetItemInfo(pInfoData, iDataLen);
	return true;
}

GNET::Octets CECIvtrMedicine::gen_item_info()
{
	GNET::Octets o;

	switch (m_pDBEssence->type)
	{
	case 0:
		add_to_octets<int>(o, m_Essence.hp);
		break;

	case 1:
		add_to_octets<int>(o, m_Essence.mp);
		break;

	case 2:
		add_to_octets<int>(o, m_Essence.hp);
		add_to_octets<int>(o, m_Essence.mp);
		break;
	}

	return o;
}

//	Get item icon file name
const char* CECIvtrMedicine::GetIconFile()
{
	return g_pGame->GetDataPath(m_pDBEssence->file_icon);
}

//	Get item name
const wchar_t* CECIvtrMedicine::GetName()
{
	return m_pDBEssence->name;
}

//	Get item cool time
int CECIvtrMedicine::GetCoolTime(int* piMax/* NULL */)
{
	CECHostPlayer* pHost = g_pGame->GetGameRun()->GetHostPlayer();
	if (!pHost)
		return 0;
	if (m_pDBEssence->type >= 0 && m_pDBEssence->type <= 5)
	{
		int ret = pHost->GetCoolTime(GP_CT_HP + m_pDBEssence->type, piMax);
		return (NULL==piMax || *piMax>200) ? ret : 0;	// hp/mp药品200ms以下冷却不显示，防外挂用
	}
	else if (m_pDBEssence->type > 5)
		return pHost->GetCoolTime(GP_CT_DP + m_pDBEssence->type - 6, piMax);
	return 0;
}

//	Get item description text
const wchar_t* CECIvtrMedicine::GetNormalDesc(bool bRepair)
{
	if (m_bNeedUpdate)
		return NULL;

	m_strDesc = _AL("");

	//	Try to build item description
	CECStringTab* pDescTab = g_pGame->GetItemDesc();
	int red = ITEMDESC_COL_RED;
	int white = ITEMDESC_COL_WHITE;
	int name_color = DecideNameCol();
	CECHostPlayer* pHost = g_pGame->GetGameRun()->GetHostPlayer();

	if (m_iCount > 1)
		AddDescText(name_color, true, pDescTab->GetWideString(ITEMDESC_NAMENUMBER), GetName(), m_iCount);
	else
		AddDescText(name_color, true, pDescTab->GetWideString(ITEMDESC_NAME), GetName());

	AddStateDesc(pDescTab);
	AddExpireTimeDesc();

	if (m_pDBEssence->renascence_count > 0)
	{
		int col = (pHost->GetRebornCount() >= m_pDBEssence->renascence_count) ? ITEMDESC_COL_WHITE : ITEMDESC_COL_RED;
		AddDescText(col, true, pDescTab->GetWideString(ITEMDESC_RENASCENCE_REQUIRED), pDescTab->GetWideString(ITEMDESC_RENASCENCE_1+m_pDBEssence->renascence_count-1));
	}
	
	if (m_pDBEssence->cool_time > 0)
	{
		AddDescText(white, true, pDescTab->GetWideString(ITEMDESC_COOLTIME), m_pDBEssence->cool_time / 1000);
	}

	//	Level requirement
	if (m_iLevelReq)
	{
		int col = g_pGame->GetGameRun()->GetHostPlayer()->GetBasicProps().iLevel >= m_iLevelReq ? white : red;
		AddDescText(col, true, pDescTab->GetWideString(ITEMDESC_LEVELREQ), m_iLevelReq);
	}

	//	Build effect description
	BuildEffectDesc();

	//	Price
	AddPriceDesc(white, bRepair);

	//	Extend description
	AddExtDescText();

	return m_strDesc;
}

//	Get item description for booth buying
const wchar_t* CECIvtrMedicine::GetBoothBuyDesc()
{
	if (!m_bNeedUpdate)
		return GetNormalDesc(false);

	m_strDesc = _AL("");

	CECStringTab* pDescTab = g_pGame->GetItemDesc();
	int white = ITEMDESC_COL_WHITE;
	int name_color = DecideNameCol();

	if (m_iCount > 1)
		AddDescText(name_color, true, pDescTab->GetWideString(ITEMDESC_NAMENUMBER), GetName(), m_iCount);
	else
		AddDescText(name_color, true, pDescTab->GetWideString(ITEMDESC_NAME), GetName());

	//	Price
	AddPriceDesc(white, false);

	return m_strDesc;
}

//	Build medicine effect description
void CECIvtrMedicine::BuildEffectDesc()
{
	CECStringTab* pDescTab = g_pGame->GetItemDesc();
	int white = ITEMDESC_COL_WHITE;

	AddDescText(white, false, pDescTab->GetWideString(ITEMDESC_USEEFFECT));

	switch (m_pDBEssence->type)
	{
	case 0:	//	瞬回 HP
		AddDescText(white, true, pDescTab->GetWideString(ITEMDESC_RECRUITHP), m_Essence.hp, m_pDBEssence->hp[0]);
		break;

	case 1:	//	瞬回 MP
		AddDescText(white, true, pDescTab->GetWideString(ITEMDESC_RECRUITMP), m_Essence.mp, m_pDBEssence->mp[0]);
		break;

	case 2:	//	瞬回 HP MP
		AddDescText(white, true, pDescTab->GetWideString(ITEMDESC_RECRUITHPMP), m_Essence.hp, m_pDBEssence->hp[0], m_Essence.mp, m_pDBEssence->mp[0]);
		break;

	case 3:	//	6秒回HP
		AddDescText(white, true, pDescTab->GetWideString(ITEMDESC_ADDHPINTIME), m_pDBEssence->hp[0], (m_pDBEssence->hp[1] + m_pDBEssence->hp[2] + m_pDBEssence->hp[3]) / 3);
		break;

	case 4:	//	6秒回MP
		AddDescText(white, true, pDescTab->GetWideString(ITEMDESC_ADDMPINTIME), m_pDBEssence->mp[0], (m_pDBEssence->mp[1] + m_pDBEssence->mp[2] + m_pDBEssence->mp[3]) / 3);
		break;

	case 5:	//	6秒回HP MP
		AddDescText(white, true, pDescTab->GetWideString(ITEMDESC_ADDHPMPINTIME), m_pDBEssence->hp[0], (m_pDBEssence->hp[1] + m_pDBEssence->hp[2] + m_pDBEssence->hp[3])/3, m_pDBEssence->mp[0], (m_pDBEssence->mp[1] + m_pDBEssence->mp[2] + m_pDBEssence->mp[3])/3);
		break;
	
	case 6://   瞬回DP
		AddDescText(white, true, pDescTab->GetWideString(ITEMDESC_RECRUITDP), m_Essence.dp, m_pDBEssence->dp[0]);
		break;

	case 7://   持续回DP
		AddDescText(white, true, pDescTab->GetWideString(ITEMDESC_ADDDPINTIME), m_pDBEssence->dp[0], (m_pDBEssence->dp[1] + m_pDBEssence->dp[2] + m_pDBEssence->dp[3]) / 3);
		break;
		
	default:
		ASSERT(0);
		return;
	}
}

//	Check item use condition
bool CECIvtrMedicine::CheckUseCondition()
{
	CECHostPlayer* pHost = g_pGame->GetGameRun()->GetHostPlayer();

	if (pHost->GetBasicProps().iLevel < GetLevelRequirement())
		return false;

	if (pHost->GetRebornCount() < m_pDBEssence->renascence_count)	//增加对转生次数的判断(飞升) Added 2012-03-29.
	{
		return false;
	}

	if (!pHost->IsInBattle() && m_pDBEssence->only_in_war)
		return false;
	
	if (!CheckCrossUseCondition())
		return false;

	return true;
}

ACString CECIvtrMedicine::_Serialize() const
{
	return m_Essence.MakeString();
}

bool CECIvtrMedicine::_Unserialize(const ACHAR* szText)
{
	m_Essence.FromString(szText);
	m_bNeedUpdate = false;
	return true;
}

//	Get drop model for shown
const char * CECIvtrMedicine::GetDropModel()
{
	return g_pGame->GetDataPath(m_pDBEssence->file_matter);
}

///////////////////////////////////////////////////////////////////////////
//	
//	Implement CECIvtrFirework
//	
///////////////////////////////////////////////////////////////////////////

CECIvtrFirework::CECIvtrFirework(int tid, int expire_date) : CECIvtrItem(tid, expire_date)
{
	m_iCID	= ICID_FIREWORK;

	//	Get database data
	elementdataman* pDB = g_pGame->GetElementDataMan();
	DATA_TYPE DataType;
	m_pDBEssence = (FIREWORKS_ESSENCE*)pDB->get_data_ptr(tid, ID_SPACE_ESSENCE, DataType);

	m_iPileLimit	= m_pDBEssence->pile_num_max;
	m_iPrice		= m_pDBEssence->price;
	m_iShopPrice	= m_pDBEssence->shop_price;
	m_iProcType		= m_pDBEssence->proc_type;
	m_iEquipMask	= 0;
	m_bUseable		= true;
	m_dwUseFlags	= USE_PERSIST;

	m_bNeedUpdate	= false;
}

CECIvtrFirework::CECIvtrFirework(const CECIvtrFirework& s) : CECIvtrItem(s)
{
	m_pDBEssence	= s.m_pDBEssence;
}

CECIvtrFirework::~CECIvtrFirework()
{
}

//	Set item detail information
bool CECIvtrFirework::SetItemInfo(BYTE* pInfoData, int iDataLen)
{
	CECIvtrItem::SetItemInfo(pInfoData, iDataLen);
	return true;
}

//	Get item icon file name
const char* CECIvtrFirework::GetIconFile()
{
	return g_pGame->GetDataPath(m_pDBEssence->file_icon);
}

//	Get item name
const wchar_t* CECIvtrFirework::GetName()
{
	return m_pDBEssence->name;
}

//	Get item cool time
int CECIvtrFirework::GetCoolTime(int* piMax/* NULL */)
{
	CECHostPlayer* pHost = g_pGame->GetGameRun()->GetHostPlayer();
	return pHost ? pHost->GetCoolTime(CP_CT_FIREWORKS, piMax) : 0;
}

//	Get item description text
const wchar_t* CECIvtrFirework::GetNormalDesc(bool bRepair)
{
	m_strDesc = _AL("");

	//	Try to build item description
	CECStringTab* pDescTab = g_pGame->GetItemDesc();
	int white = ITEMDESC_COL_WHITE;
	int name_color = DecideNameCol();

	if (m_iCount > 1)
		AddDescText(name_color, true, pDescTab->GetWideString(ITEMDESC_NAMENUMBER), GetName(), m_iCount);
	else
		AddDescText(name_color, true, pDescTab->GetWideString(ITEMDESC_NAME), GetName());

	AddStateDesc(pDescTab);
	AddExpireTimeDesc();

	//	Firework prepare time
//	AddDescText(white, true, pDescTab->GetWideString(ITEMDESC_FIREWORKTIME), m_pDBEssence->time_to_fire);

	//	Price
	AddPriceDesc(white, bRepair);

	//	Firework level
	if (m_pDBEssence->level)
	{
		m_strDesc += _AL("\\r");
		m_strDesc += pDescTab->GetWideString(ITEMDESC_COL_WHITE);
		for (int i=0; i < m_pDBEssence->level; i++)
			m_strDesc += pDescTab->GetWideString(ITEMDESC_PENTAGON);
	}

	//	Extend description
	AddExtDescText();

	return m_strDesc;
}

//	Get drop model for shown
const char * CECIvtrFirework::GetDropModel()
{
	return g_pGame->GetDataPath(m_pDBEssence->file_matter);
}

///////////////////////////////////////////////////////////////////////////
//	
//	Implement CECIvtrTextFirework
//	
///////////////////////////////////////////////////////////////////////////

CECIvtrTextFirework::CECIvtrTextFirework(int tid, int expire_date) : CECIvtrItem(tid, expire_date)
{
	m_iCID	= ICID_TEXT_FIREWORK;

	//	Get database data
	elementdataman* pDB = g_pGame->GetElementDataMan();
	DATA_TYPE DataType;
	m_pDBEssence = (TEXT_FIREWORKS_ESSENCE*)pDB->get_data_ptr(tid, ID_SPACE_ESSENCE, DataType);

	m_iPileLimit	= m_pDBEssence->pile_num_max;
	m_iPrice		= m_pDBEssence->price;
	m_iShopPrice	= m_pDBEssence->shop_price;
	m_iProcType		= m_pDBEssence->proc_type;
	m_iEquipMask	= 0;
	m_bUseable		= true;
	m_dwUseFlags	= USE_PERSIST;

	m_bNeedUpdate	= false;
}

CECIvtrTextFirework::CECIvtrTextFirework(const CECIvtrTextFirework& s) : CECIvtrItem(s)
{
	m_pDBEssence	= s.m_pDBEssence;
}

CECIvtrTextFirework::~CECIvtrTextFirework()
{
}

//	Set item detail information
bool CECIvtrTextFirework::SetItemInfo(BYTE* pInfoData, int iDataLen)
{
	CECIvtrItem::SetItemInfo(pInfoData, iDataLen);
	return true;
}

//	Get item icon file name
const char* CECIvtrTextFirework::GetIconFile()
{
	return g_pGame->GetDataPath(m_pDBEssence->file_icon);
}

//	Get item name
const wchar_t* CECIvtrTextFirework::GetName()
{
	return m_pDBEssence->name;
}

//	Get item cool time
int CECIvtrTextFirework::GetCoolTime(int* piMax/* NULL */)
{
	CECHostPlayer* pHost = g_pGame->GetGameRun()->GetHostPlayer();
	return pHost ? pHost->GetCoolTime(CP_CT_FIREWORKS, piMax) : 0;
}

//	Get item description text
const wchar_t* CECIvtrTextFirework::GetNormalDesc(bool bRepair)
{
	m_strDesc = _AL("");

	//	Try to build item description
	CECStringTab* pDescTab = g_pGame->GetItemDesc();
	int white = ITEMDESC_COL_WHITE;
	int name_color = DecideNameCol();

	if (m_iCount > 1)
		AddDescText(name_color, true, pDescTab->GetWideString(ITEMDESC_NAMENUMBER), GetName(), m_iCount);
	else
		AddDescText(name_color, true, pDescTab->GetWideString(ITEMDESC_NAME), GetName());

	AddStateDesc(pDescTab);
	AddExpireTimeDesc();

	//	Firework prepare time
//	AddDescText(white, true, pDescTab->GetWideString(ITEMDESC_FIREWORKTIME), m_pDBEssence->time_to_fire);

	//	Price
	AddPriceDesc(white, bRepair);

	//	Firework level
	if (m_pDBEssence->level)
	{
		m_strDesc += _AL("\\r");
		m_strDesc += pDescTab->GetWideString(ITEMDESC_COL_WHITE);
		for (int i=0; i < m_pDBEssence->level; i++)
			m_strDesc += pDescTab->GetWideString(ITEMDESC_PENTAGON);
	}

	//	Extend description
	AddExtDescText();

	return m_strDesc;
}

//	Get drop model for shown
const char * CECIvtrTextFirework::GetDropModel()
{
	return g_pGame->GetDataPath(m_pDBEssence->file_matter);
}

///////////////////////////////////////////////////////////////////////////
//	
//	Implement CECIvtrRecipeRoll
//	
///////////////////////////////////////////////////////////////////////////

CECIvtrRecipeRoll::CECIvtrRecipeRoll(int tid, int expire_date) : CECIvtrItem(tid, expire_date)
{
	m_iCID	= ICID_RECIPEROLL;

	//	Get database data
	elementdataman* pDB = g_pGame->GetElementDataMan();
	DATA_TYPE DataType;
	m_pDBEssence = (RECIPEROLL_ESSENCE*)pDB->get_data_ptr(tid, ID_SPACE_ESSENCE, DataType);
	DATA_TYPE dt;
	m_pRelatedRecipe = (RECIPE_ESSENCE *) g_pGame->GetElementDataMan()->get_data_ptr(m_pDBEssence->id_recipe, ID_SPACE_RECIPE, dt);
	if( dt != DT_RECIPE_ESSENCE )
		m_pRelatedRecipe = NULL;

	m_iPileLimit	= m_pDBEssence->pile_num_max;
	m_iPrice		= m_pDBEssence->price;
	m_iShopPrice	= m_pDBEssence->shop_price;
	m_iProcType		= m_pDBEssence->proc_type;
	m_iEquipMask	= 0;
	m_bUseable		= true;
	m_dwUseFlags	= USE_PERSIST;

	m_bNeedUpdate	= false;
}

CECIvtrRecipeRoll::CECIvtrRecipeRoll(const CECIvtrRecipeRoll& s) : CECIvtrItem(s)
{
	m_pDBEssence	= s.m_pDBEssence;
	m_pRelatedRecipe= s.m_pRelatedRecipe;
}

CECIvtrRecipeRoll::~CECIvtrRecipeRoll()
{
}

//	Set item detail information
bool CECIvtrRecipeRoll::SetItemInfo(BYTE* pInfoData, int iDataLen)
{
	CECIvtrItem::SetItemInfo(pInfoData, iDataLen);
	return true;
}

//	Get item icon file name
const char* CECIvtrRecipeRoll::GetIconFile()
{
	return g_pGame->GetDataPath(m_pDBEssence->file_icon);
}

//	Get item name
const wchar_t* CECIvtrRecipeRoll::GetName()
{
	return m_pDBEssence->name;
}

//	Get item cool time
int CECIvtrRecipeRoll::GetCoolTime(int* piMax/* NULL */)
{
	CECHostPlayer* pHost = g_pGame->GetGameRun()->GetHostPlayer();
	//return pHost ? pHost->GetCoolTime(CP_CT_FIREWORKS, piMax) : 0;
	return 0;
}

bool CECIvtrRecipeRoll::CheckUseCondition()
{
	if( !m_pRelatedRecipe )
		return false;
	if (!CheckCrossUseCondition())
		return false;

	CECHostPlayer* pHost = g_pGame->GetGameRun()->GetHostPlayer();
	return IsUseable() && pHost && !pHost->HasRecipe(m_pDBEssence->id_recipe) && pHost->GetProduceSkillLevel() >= m_pRelatedRecipe->level;
}

//	Get item description text
const wchar_t* CECIvtrRecipeRoll::GetNormalDesc(bool bRepair)
{
	m_strDesc = _AL("");

	//	Try to build item description
	CECStringTab* pDescTab = g_pGame->GetItemDesc();
	int white = ITEMDESC_COL_WHITE;
	int red = ITEMDESC_COL_RED;
	int name_color = DecideNameCol();
	
	if (m_iCount > 1)
		AddDescText(name_color, true, pDescTab->GetWideString(ITEMDESC_NAMENUMBER), GetName(), m_iCount);
	else
		AddDescText(name_color, true, pDescTab->GetWideString(ITEMDESC_NAME), GetName());

	AddStateDesc(pDescTab);
	AddExpireTimeDesc();

	CECHostPlayer* pHost = g_pGame->GetGameRun()->GetHostPlayer();
	if( m_pRelatedRecipe )
	{
		AddDescText(white, true, L"%s", pDescTab->GetWideString(ITEMDESC_RECIPE_CLASS1 + m_pRelatedRecipe->recipe_class));

		if (pHost && m_pRelatedRecipe && pHost->GetProduceSkillLevel() >= m_pRelatedRecipe->level)
			AddDescText(white, true, pDescTab->GetWideString(ITEMDESC_PRODUCE_LEVEL), m_pRelatedRecipe->level);
		else
			AddDescText(red, true, pDescTab->GetWideString(ITEMDESC_PRODUCE_LEVEL), m_pRelatedRecipe->level);

		if (m_pRelatedRecipe->recipe_class == 1)
		{
			AddDescText(white, true, pDescTab->GetWideString(ITEMDESC_RECIPE_TECH),
				m_pRelatedRecipe->required[0],
				m_pRelatedRecipe->required[1],
				m_pRelatedRecipe->required[2],
				m_pRelatedRecipe->required[3],
				m_pRelatedRecipe->required[4],
				m_pRelatedRecipe->required[5]);

			AddDescText(white, true, pDescTab->GetWideString(ITEMDESC_RECIPE_CONSTRUCTION), m_pRelatedRecipe->required[6]);

			/*
			if (m_pRelatedRecipe->products[0].id_to_make)
			{
				CECIvtrItem* pItem = CECIvtrItem::CreateItem(m_pRelatedRecipe->products[0].id_to_make, 0, 1);

				if (pItem)
				{
					if (m_pRelatedRecipe->products[0].min_num_make == 1 && m_pRelatedRecipe->products[0].max_num_make == 1)
						AddDescText(white, true, L"%s", pItem->GetName());
					else
					{
						AddDescText(white, true, pDescTab->GetWideString(ITEMDESC_RECIPE_PRODUCT),
							pItem->GetName(),
							m_pRelatedRecipe->products[0].min_num_make,
							m_pRelatedRecipe->products[0].max_num_make);
					}

					delete pItem;
				}
			}
			*/

			for (int i = 0; i < 6; i++)
			{
				int id = m_pRelatedRecipe->materials[i].id;

				if (id)
				{
					CECIvtrItem* pItem = CECIvtrItem::CreateItem(id, 0, 1);

					if (pItem)
					{
						AddDescText(white, true, pDescTab->GetWideString(ITEMDESC_RECIPE_MATERIAL), pItem->GetName(), m_pRelatedRecipe->materials[i].num);
						delete pItem;
					}
				}
			}
		}

		if (pHost && pHost->HasRecipe(m_pDBEssence->id_recipe))
			AddDescText(red, true, pDescTab->GetWideString(ITEMDESC_ALREADYLEARNED));
	}

	//	Price
	AddPriceDesc(white, bRepair);

	//	Extend description
	AddExtDescText();

	return m_strDesc;
}

//	Get drop model for shown
const char * CECIvtrRecipeRoll::GetDropModel()
{
	return g_pGame->GetDataPath(m_pDBEssence->file_matter);
}



///////////////////////////////////////////////////////////////////////////
//	
//	Implement CECIvtrDoubleExp
//	
///////////////////////////////////////////////////////////////////////////

CECIvtrDoubleExp::CECIvtrDoubleExp(int tid, int expire_date) : CECIvtrItem(tid, expire_date)
{
	m_iCID	= ICID_DOUBLEEXP;

	//	Get database data
	elementdataman* pDB = g_pGame->GetElementDataMan();
	DATA_TYPE DataType;
	m_pDBEssence = (DOUBLE_EXP_ESSENCE*)pDB->get_data_ptr(tid, ID_SPACE_ESSENCE, DataType);
	
	m_iPileLimit	= m_pDBEssence->pile_num_max;
	m_iPrice		= m_pDBEssence->price;
	m_iShopPrice	= m_pDBEssence->shop_price;
	m_iProcType		= m_pDBEssence->proc_type;
	m_iEquipMask	= 0;
	m_bUseable		= true;
	m_dwUseFlags	= USE_PERSIST;

	m_bNeedUpdate	= false;
}

CECIvtrDoubleExp::CECIvtrDoubleExp(const CECIvtrDoubleExp& s) : CECIvtrItem(s)
{
	m_pDBEssence	= s.m_pDBEssence;
}

CECIvtrDoubleExp::~CECIvtrDoubleExp()
{
}

//	Set item detail information
bool CECIvtrDoubleExp::SetItemInfo(BYTE* pInfoData, int iDataLen)
{
	CECIvtrItem::SetItemInfo(pInfoData, iDataLen);
	return true;
}

//	Get item icon file name
const char* CECIvtrDoubleExp::GetIconFile()
{
	return g_pGame->GetDataPath(m_pDBEssence->file_icon);
}

//	Get item name
const wchar_t* CECIvtrDoubleExp::GetName()
{
	return m_pDBEssence->name;
}

//	Get item cool time
int CECIvtrDoubleExp::GetCoolTime(int* piMax/* NULL */)
{
	CECHostPlayer* pHost = g_pGame->GetGameRun()->GetHostPlayer();
	return pHost ? pHost->GetCoolTime(CP_CT_DOUBLEEXPMATTER, piMax) : 0;
}

bool CECIvtrDoubleExp::CheckUseCondition()
{
	if(!CheckCrossUseCondition())
		return false;
	return true;	
}

//	Get item description text
const wchar_t* CECIvtrDoubleExp::GetNormalDesc(bool bRepair)
{
	m_strDesc = _AL("");

	//	Try to build item description
	CECStringTab* pDescTab = g_pGame->GetItemDesc();
	int white = ITEMDESC_COL_WHITE;
	int red = ITEMDESC_COL_RED;
	int name_color = DecideNameCol();
	
	if (m_iCount > 1)
		AddDescText(name_color, true, pDescTab->GetWideString(ITEMDESC_NAMENUMBER), GetName(), m_iCount);
	else
		AddDescText(name_color, true, pDescTab->GetWideString(ITEMDESC_NAME), GetName());

	AddStateDesc(pDescTab);
	AddExpireTimeDesc();

	//	Price
	AddPriceDesc(white, bRepair);

	//	Extend description
	AddExtDescText();

	return m_strDesc;
}

//	Get drop model for shown
const char * CECIvtrDoubleExp::GetDropModel()
{
	return g_pGame->GetDataPath(m_pDBEssence->file_matter);
}

///////////////////////////////////////////////////////////////////////////
//	
//	Implement CECIvtrSkillMat
//	
///////////////////////////////////////////////////////////////////////////

CECIvtrSkillMat::CECIvtrSkillMat(int tid, int expire_date) : CECIvtrItem(tid, expire_date)
{
	m_iCID	= ICID_SKILLMATTER;

	//	Get database data
	elementdataman* pDB = g_pGame->GetElementDataMan();
	DATA_TYPE DataType;
	m_pDBEssence = (SKILLMATTER_ESSENCE*)pDB->get_data_ptr(tid, ID_SPACE_ESSENCE, DataType);

	m_iPileLimit	= m_pDBEssence->pile_num_max;
	m_iPrice		= m_pDBEssence->price;
	m_iShopPrice	= m_pDBEssence->shop_price;
	m_iProcType		= m_pDBEssence->proc_type;
	m_iEquipMask	= 0;
	m_bUseable		= true;

	m_bNeedUpdate	= false;
}

CECIvtrSkillMat::CECIvtrSkillMat(const CECIvtrSkillMat& s) : CECIvtrItem(s)
{
	m_pDBEssence	= s.m_pDBEssence;
}

CECIvtrSkillMat::~CECIvtrSkillMat()
{
}

//	Set item detail information
bool CECIvtrSkillMat::SetItemInfo(BYTE* pInfoData, int iDataLen)
{
	CECIvtrItem::SetItemInfo(pInfoData, iDataLen);
	return true;
}

//	Get item icon file name
const char* CECIvtrSkillMat::GetIconFile()
{
	return g_pGame->GetDataPath(m_pDBEssence->file_icon);
}

//	Get item name
const wchar_t* CECIvtrSkillMat::GetName()
{
	return m_pDBEssence->name;
}

//	Get item cool time
int CECIvtrSkillMat::GetCoolTime(int* piMax/* NULL */)
{
	CECHostPlayer* pHost = g_pGame->GetGameRun()->GetHostPlayer();
	// 对其他目标使用的物品技能
	if (m_pDBEssence->cast_skill)
	{
		if (pHost)
		{
			CECSkill* pSkill = pHost->GetItemSkillByID(m_pDBEssence->id_skill);
			if (pSkill)
			{		
				if (piMax)
					*piMax = pSkill->GetCoolingTime();
				return pSkill->GetCoolingCnt();
			}
			else
				return 0;
		}
		else
			return 0;
	}
	else
	{
		int iType;
		if (m_pDBEssence->cool_type<3)
			iType = GP_CT_SKILLMATTER1 + m_pDBEssence->cool_type;
		else
			iType = GP_CT_SKILLMATTER4 + m_pDBEssence->cool_type-3;
		return pHost ? pHost->GetCoolTime(iType, piMax) : 0;
	}
}

//	Get item description text
const wchar_t* CECIvtrSkillMat::GetNormalDesc(bool bRepair)
{
	m_strDesc = _AL("");

	//	Try to build item description
	CECStringTab* pDescTab = g_pGame->GetItemDesc();
	int red = ITEMDESC_COL_RED;
	int white = ITEMDESC_COL_WHITE;
	int name_color = DecideNameCol();
	CECHostPlayer* pHost = g_pGame->GetGameRun()->GetHostPlayer();

	if (m_iCount > 1)
		AddDescText(name_color, true, pDescTab->GetWideString(ITEMDESC_NAMENUMBER), GetName(), m_iCount);
	else
		AddDescText(name_color, true, pDescTab->GetWideString(ITEMDESC_NAME), GetName());

	AddStateDesc(pDescTab);
	AddExpireTimeDesc();

	if (m_pDBEssence->renascence_count > 0)
	{
		int col = (pHost->GetRebornCount() >= m_pDBEssence->renascence_count) ? ITEMDESC_COL_WHITE : ITEMDESC_COL_RED;
		AddDescText(col, false, pDescTab->GetWideString(ITEMDESC_RENASCENCE_REQUIRED), pDescTab->GetWideString(ITEMDESC_RENASCENCE_1+m_pDBEssence->renascence_count-1));
	}

	//	Level requirement
	if (m_pDBEssence->level_required)
	{
		int col = g_pGame->GetGameRun()->GetHostPlayer()->GetBasicProps().iLevel >= m_pDBEssence->level_required ? white : red;
		AddDescText(col, true, pDescTab->GetWideString(ITEMDESC_LEVELREQ), m_pDBEssence->level_required);
	}

	//	Price
	AddPriceDesc(white, bRepair);

	//	Extend description
	AddExtDescText();

	return m_strDesc;
}

//	Check item use condition
bool CECIvtrSkillMat::CheckUseCondition()
{
	if (!m_pDBEssence)
		return false;

	//	Check level requirement
	if (g_pGame->GetGameRun()->GetHostPlayer()->GetBasicProps().iLevel < m_pDBEssence->level_required)
		return false;

	if (!g_pGame->GetGameRun()->GetHostPlayer()->IsInBattle() && m_pDBEssence->only_in_war)
		return false;
	if (!CheckCrossUseCondition())
		return false;

	return true;
}

//	Get drop model for shown
const char * CECIvtrSkillMat::GetDropModel()
{
	return g_pGame->GetDataPath(m_pDBEssence->file_matter);
}



///////////////////////////////////////////////////////////////////////////
//	
//	Implement CECIvtrVehicle
//	
///////////////////////////////////////////////////////////////////////////

CECIvtrVehicle::CECIvtrVehicle(int tid, int expire_date) : CECIvtrItem(tid, expire_date)
{
	m_iCID	= ICID_VEHICLE;

	//	Get database data
	elementdataman* pDB = g_pGame->GetElementDataMan();
	DATA_TYPE DataType;
	m_pDBEssence = (VEHICLE_ESSENCE*)pDB->get_data_ptr(tid, ID_SPACE_ESSENCE, DataType);

	m_iPileLimit	= m_pDBEssence->pile_num_max;
	m_iPrice		= m_pDBEssence->price;
	m_iShopPrice	= m_pDBEssence->shop_price;
	m_iProcType		= m_pDBEssence->proc_type;
	m_iEquipMask	= 0;
	m_bUseable		= true;

	m_bNeedUpdate	= true;
}

CECIvtrVehicle::CECIvtrVehicle(const CECIvtrVehicle& s) : CECIvtrItem(s), m_addons(s.m_addons)
{
	m_pDBEssence	= s.m_pDBEssence;
}

CECIvtrVehicle::~CECIvtrVehicle()
{
}

//	Set item detail information
bool CECIvtrVehicle::SetItemInfo(BYTE* pInfoData, int iDataLen)
{
	CECIvtrItem::SetItemInfo(pInfoData, iDataLen);
	m_addons.clear();

	if (iDataLen >= sizeof(int))
	{
		int count = *(int*)pInfoData;

		if (iDataLen == int((count + 1) * sizeof(int)))
		{
			int* p = (int*)(pInfoData + sizeof(int));

			for (int i = 0; i < count; i++)
				m_addons.push_back(p[i]);
		}
		else
		{
			assert(false);
		}
	}

	return true;
}

//	Get item icon file name
const char* CECIvtrVehicle::GetIconFile()
{
	return g_pGame->GetDataPath(m_pDBEssence->file_icon);
}

//	Get item name
const wchar_t* CECIvtrVehicle::GetName()
{
	return m_pDBEssence->name;
}

//	Get item cool time
int CECIvtrVehicle::GetCoolTime(int* piMax/* NULL */)
{
	CECHostPlayer* pHost = g_pGame->GetGameRun()->GetHostPlayer();
	return pHost ? pHost->GetCoolTime(GP_CT_VEHICLE, piMax) : 0;
}

//	Get item description text
const wchar_t* CECIvtrVehicle::GetNormalDesc(bool bRepair)
{
	if (m_bNeedUpdate)
		return NULL;

	m_strDesc = _AL("");

	//	Try to build item description
	CECStringTab* pDescTab = g_pGame->GetItemDesc();
	int red = ITEMDESC_COL_RED;
	int white = ITEMDESC_COL_WHITE;
	int name_color = DecideNameCol();
	CECHostPlayer* pHost = g_pGame->GetGameRun()->GetHostPlayer();

	if (m_iCount > 1)
		AddDescText(name_color, true, pDescTab->GetWideString(ITEMDESC_NAMENUMBER), GetName(), m_iCount);
	else
		AddDescText(name_color, true, pDescTab->GetWideString(ITEMDESC_NAME), GetName());

	AddStateDesc(pDescTab);
	AddExpireTimeDesc();
	
	if (m_pDBEssence->renascence_count > 0)
	{
		int col = (pHost->GetRebornCount() >= m_pDBEssence->renascence_count) ? ITEMDESC_COL_WHITE : ITEMDESC_COL_RED;
		AddDescText(col, true, pDescTab->GetWideString(ITEMDESC_RENASCENCE_REQUIRED), pDescTab->GetWideString(ITEMDESC_RENASCENCE_1+m_pDBEssence->renascence_count-1));
	}	

	//	Level requirement
	if (m_pDBEssence->level_required)
	{
		int col = g_pGame->GetGameRun()->GetHostPlayer()->GetBasicProps().iLevel >= m_pDBEssence->level_required ? white : red;
		AddDescText(col, true, pDescTab->GetWideString(ITEMDESC_LEVELREQ), m_pDBEssence->level_required);
	}
	
	for (size_t i = 0; i < m_addons.size(); i++)
	{
		DATA_TYPE dt;
		EQUIPMENT_ADDON* pAddOn = (EQUIPMENT_ADDON*) g_pGame->GetElementDataMan()->get_data_ptr(m_addons[i], ID_SPACE_ADDON, dt);

		if (dt != DT_EQUIPMENT_ADDON)
			continue;

		AddDescText(ITEMDESC_COL_LIGHTBLUE, true, _AL("%s"), CECIvtrEquip::GetEquipAddonDesc(pAddOn->id, pAddOn->type, pAddOn->param1, pAddOn->param2, pAddOn->param3, 1));
	}

	AddDescText(name_color, true, pDescTab->GetWideString(ITEMDESC_MOVESPEED), m_pDBEssence->speed);

	//	Price
	AddPriceDesc(white, bRepair);

	//	Extend description
	AddExtDescText();

	return m_strDesc;
}

//	Check item use condition
bool CECIvtrVehicle::CheckUseCondition()
{
	if (!m_pDBEssence)
		return false;

	//	Check level requirement
	if (g_pGame->GetGameRun()->GetHostPlayer()->GetBasicProps().iLevel < m_pDBEssence->level_required)
		return false;
	
	if (!g_pGame->GetGameRun()->GetHostPlayer()->IsInBattle() && m_pDBEssence->only_in_war)
		return false;	

	if(!(g_pGame->GetGameRun()->GetHostPlayer()->GetProfession() >= 102 && g_pGame->GetGameRun()->GetHostPlayer()->GetProfession() <= 106) && m_pDBEssence->only_qianji)
		return false;
	
#ifndef _INVENTORY_DESC_MAN
	if(m_pDBEssence->nation_position_mask != 0)
	{
		char nKingTitle = g_pGame->GetGameRun()->GetHostPlayer()->GetKingdomTitle();
		unsigned int mask = nKingTitle == 0 ? 0 : 1<<(nKingTitle-1);
		if(!(mask & m_pDBEssence->nation_position_mask))
			return false;
	}
#endif
	
	if (!CheckCrossUseCondition())
		return false;

	return true;
}

//	Get drop model for shown
const char * CECIvtrVehicle::GetDropModel()
{
	return g_pGame->GetDataPath(m_pDBEssence->file_matter);
}

ACString CECIvtrVehicle::_Serialize() const
{
	size_t sz = 1+m_addons.size();
	int* p = new int[sz];
	*p = m_addons.size();

	if (*p)
		memcpy(p+1, m_addons.begin(), m_addons.size() * sizeof(int));

	ACString str = BufferMakeString(p, sz * sizeof(int));
	delete[] p;
	return str;
}

bool CECIvtrVehicle::_Unserialize(const ACHAR* szText)
{
	size_t sz;
	unsigned char* p = BufferFromString(szText, sz);

	if (sz >= 4)
	{
		size_t count = *(int*)p;

		if (count && (count+1) * sizeof(int) == sz)
		{
			int* n = (int*)(p+4);

			for (size_t i = 0; i < count; i++)
				m_addons.push_back(n[i]);
		}
	}

	delete[] p;
	m_bNeedUpdate = false;
	return true;
}

///////////////////////////////////////////////////////////////////////////
//	
//	Implement CECIvtrCoupleJumpTo
//	
///////////////////////////////////////////////////////////////////////////

CECIvtrCoupleJumpTo::CECIvtrCoupleJumpTo(int tid, int expire_date) : CECIvtrItem(tid, expire_date)
{
	m_iCID	= ICID_COUPLE_JUMPTO;

	//	Get database data
	elementdataman* pDB = g_pGame->GetElementDataMan();
	DATA_TYPE DataType;
	m_pDBEssence = (COUPLE_JUMPTO_ESSENCE*)pDB->get_data_ptr(tid, ID_SPACE_ESSENCE, DataType);

	m_iPileLimit	= m_pDBEssence->pile_num_max;
	m_iPrice		= m_pDBEssence->price;
	m_iShopPrice	= m_pDBEssence->shop_price;
	m_iProcType		= m_pDBEssence->proc_type;
	m_iEquipMask	= 0;
	m_bUseable		= true;

	m_bNeedUpdate	= false;
}

CECIvtrCoupleJumpTo::CECIvtrCoupleJumpTo(const CECIvtrCoupleJumpTo& s) : CECIvtrItem(s)
{
	m_pDBEssence	= s.m_pDBEssence;
}

CECIvtrCoupleJumpTo::~CECIvtrCoupleJumpTo()
{
}

//	Set item detail information
bool CECIvtrCoupleJumpTo::SetItemInfo(BYTE* pInfoData, int iDataLen)
{
	CECIvtrItem::SetItemInfo(pInfoData, iDataLen);
	return true;
}

//	Get item icon file name
const char* CECIvtrCoupleJumpTo::GetIconFile()
{
	return g_pGame->GetDataPath(m_pDBEssence->file_icon);
}

//	Get item name
const wchar_t* CECIvtrCoupleJumpTo::GetName()
{
	return m_pDBEssence->name;
}

//	Get item cool time
int CECIvtrCoupleJumpTo::GetCoolTime(int* piMax/* NULL */)
{
	CECHostPlayer* pHost = g_pGame->GetGameRun()->GetHostPlayer();
	return pHost ? pHost->GetCoolTime(GP_CT_COUPLE_JUMPTO, piMax) : 0;
}

//	Get item description text
const wchar_t* CECIvtrCoupleJumpTo::GetNormalDesc(bool bRepair)
{
	m_strDesc = _AL("");

	//	Try to build item description
	CECStringTab* pDescTab = g_pGame->GetItemDesc();
	int red = ITEMDESC_COL_RED;
	int white = ITEMDESC_COL_WHITE;
	int name_color = DecideNameCol();

	if (m_iCount > 1)
		AddDescText(name_color, true, pDescTab->GetWideString(ITEMDESC_NAMENUMBER), GetName(), m_iCount);
	else
		AddDescText(name_color, true, pDescTab->GetWideString(ITEMDESC_NAME), GetName());

	AddStateDesc(pDescTab);
	AddExpireTimeDesc();

	//	Price
	AddPriceDesc(white, bRepair);

	//	Extend description
	AddExtDescText();

	return m_strDesc;
}

//	Check item use condition
bool CECIvtrCoupleJumpTo::CheckUseCondition()
{
	if (!m_pDBEssence)
		return false;

	//	Check level requirement
	if (g_pGame->GetGameRun()->GetHostPlayer()->GetSpouse() == 0)
		return false;
	
	if (!CheckCrossUseCondition())
		return false;

	return true;
}

//	Get drop model for shown
const char * CECIvtrCoupleJumpTo::GetDropModel()
{
	return g_pGame->GetDataPath(m_pDBEssence->file_matter);
}

///////////////////////////////////////////////////////////////////////////
//	
//	Implement CECIvtrLottery
//	
///////////////////////////////////////////////////////////////////////////

CECIvtrLottery::CECIvtrLottery(int tid, int expire_date) : CECIvtrItem(tid, expire_date)
{
	m_iCID	= ICID_LOTTERY;

	//	Get database data
	elementdataman* pDB = g_pGame->GetElementDataMan();
	DATA_TYPE DataType;
	m_pDBEssence = (LOTTERY_ESSENCE*)pDB->get_data_ptr(tid, ID_SPACE_ESSENCE, DataType);
	memset(&m_Essence, 0, sizeof(m_Essence));
	
	m_iPileLimit	= m_pDBEssence->pile_num_max;
	m_iPrice		= m_pDBEssence->price;
	m_iShopPrice	= m_pDBEssence->shop_price;
	m_iProcType		= m_pDBEssence->proc_type;
	m_iEquipMask	= 0;
	m_bUseable		= true;

	m_bNeedUpdate	= true;
	m_strRank		= _AL("");
}

CECIvtrLottery::CECIvtrLottery(const CECIvtrLottery& s) : CECIvtrItem(s)
{
	m_pDBEssence	= s.m_pDBEssence;
	m_Essence		= s.m_Essence;
	m_strRank		= s.m_strRank;
}

CECIvtrLottery::~CECIvtrLottery()
{
}

//	Set item detail information
bool CECIvtrLottery::SetItemInfo(BYTE* pInfoData, int iDataLen)
{
	if( iDataLen == 0 )
		memset(&m_Essence.dice_data, 0, sizeof(m_Essence.dice_data));
	else
		memcpy(m_Essence.dice_data, pInfoData, min(6, min(iDataLen, m_pDBEssence->dice_count)));

	CECIvtrItem::SetItemInfo(pInfoData, iDataLen);
	return true;
}

GNET::Octets CECIvtrLottery::gen_item_info()
{
	return GNET::Octets(&m_Essence, sizeof(m_Essence));
}

//	Get item icon file name
const char* CECIvtrLottery::GetIconFile()
{
	return g_pGame->GetDataPath(m_pDBEssence->file_icon);
}

//	Get item name
const wchar_t* CECIvtrLottery::GetName()
{
	return m_pDBEssence->name;
}

//	Get item cool time
int CECIvtrLottery::GetCoolTime(int* piMax/* NULL */)
{
	CECHostPlayer* pHost = g_pGame->GetGameRun()->GetHostPlayer();
	return pHost ? pHost->GetCoolTime(GP_CT_LOTTERY, piMax) : 0;
}

//	Get item description text
const wchar_t* CECIvtrLottery::GetNormalDesc(bool bRepair)
{
	if (m_bNeedUpdate)
		return NULL;

	m_strDesc = _AL("");

	//	Try to build item description
	CECStringTab* pDescTab = g_pGame->GetItemDesc();
	int red = ITEMDESC_COL_RED;
	int white = ITEMDESC_COL_WHITE;
	int name_color = DecideNameCol();

	if (m_iCount > 1)
		AddDescText(name_color, true, pDescTab->GetWideString(ITEMDESC_NAMENUMBER), GetName(), m_iCount);
	else
		AddDescText(name_color, true, pDescTab->GetWideString(ITEMDESC_NAME), GetName());

//  将“彩票”去掉
//	AddDescText(white, true, pDescTab->GetWideString(ITEMDESC_LOTTERY));

	if (m_pDBEssence->type == 0)
	{
		ACString strDiced;
		int nDiced = 0;
		for(int i=0; i<6; i++)
		{
			if( m_Essence.dice_data[i] > 0 && m_Essence.dice_data[i] <= 32 )
			{
				strDiced = strDiced + _AL("  ") + m_pDBEssence->candidates[m_Essence.dice_data[i] - 1].desc;
				nDiced ++;
			}
			else
				break;
		}
//		AddDescText(ITEMDESC_COL_GREEN, true, pDescTab->GetWideString(ITEMDESC_DICED), nDiced, m_pDBEssence->dice_count, strDiced);
		if(nDiced)
		{
			if(m_strRank == _AL(""))
				GetAwardRank();
			AddDescText(ITEMDESC_COL_GREEN, true, pDescTab->GetWideString(ITEMDESC_NEW_DICED), m_strRank);
		}
		else
			AddDescText(ITEMDESC_COL_GREEN, true, pDescTab->GetWideString(ITEMDESC_UNDRAW_LOTTERY));
	}
	else if(m_pDBEssence->type == 1)
	{
		int index = -1;
		if(m_Essence.dice_data[0] != 0)
		{	
			if(m_pDBEssence->dice_count ==2)
				index = (m_Essence.dice_data[0] - 1) * 4 + m_Essence.dice_data[1] - 1;
			else if(m_pDBEssence->dice_count ==1)
				index = m_Essence.dice_data[0] -1;
		}
		if (index >= 0 && index < sizeof(m_pDBEssence->candidates) / sizeof(m_pDBEssence->candidates[0]))
			AddDescText(ITEMDESC_COL_GREEN, true, pDescTab->GetWideString(ITEMDESC_NEW_DICED), m_pDBEssence->candidates[index].desc);
		else
			AddDescText(ITEMDESC_COL_GREEN, true, pDescTab->GetWideString(ITEMDESC_UNDRAW_LOTTERY));
	}

	AddStateDesc(pDescTab);
	AddExpireTimeDesc();

	//	Price
	AddPriceDesc(white, bRepair);

	//	Extend description
	AddExtDescText();

	return m_strDesc;
}

//	Check item use condition
bool CECIvtrLottery::CheckUseCondition()
{
	if (!m_pDBEssence)
		return false;
	
	if (!CheckCrossUseCondition())
		return false;

	return true;
}

//	Get drop model for shown
const char * CECIvtrLottery::GetDropModel()
{
	return g_pGame->GetDataPath(m_pDBEssence->file_matter);
}

ACString CECIvtrLottery::_Serialize() const
{
	return ItemMakeString(&m_Essence);
}

bool CECIvtrLottery::_Unserialize(const ACHAR* szText)
{
	ItemFromString(szText, &m_Essence);
	m_bNeedUpdate = false;
	return true;
}

bool CECIvtrLottery::SetEmptyInfo()
{
	SetItemInfo(NULL, 0);
	return true;
}

void CECIvtrLottery::GetAwardRank()
{
	abase::vector<CScriptValue> args, dicedatas;
	abase::vector<CScriptValue> results;
	CScriptValue dicedata;

	for(int i = 0; i < m_pDBEssence->dice_count; i++)
		dicedatas.push_back(CScriptValue((double)m_Essence.dice_data[i]));
	dicedata.SetArray(dicedatas);
	args.push_back((double)GetTemplateID());
	args.push_back(dicedata);

	g_LuaStateMan.GetAIState()->Call(LOTTERY_TBL_NAME, "GetLotteryDesc", args, results);

// 					if (results.size() == 1 && results[0].m_Type == CScriptValue::SVT_STRING)
// 						strDesc = results[0].GetAWString();
	if (results.size() == 1 && results[0].m_Type == CScriptValue::SVT_STRING)
	{
		results[0].RetrieveAWString(m_strRank);
 	}
}


///////////////////////////////////////////////////////////////////////////
//	
//	Implement CECIvtrLottery2
//	
///////////////////////////////////////////////////////////////////////////

CECIvtrLottery2::CECIvtrLottery2(int tid, int expire_date) : CECIvtrItem(tid, expire_date)
{
	m_iCID	= ICID_LOTTERY2;

	//	Get database data
	elementdataman* pDB = g_pGame->GetElementDataMan();
	DATA_TYPE DataType;
	m_pDBEssence = (LOTTERY2_ESSENCE*)pDB->get_data_ptr(tid, ID_SPACE_ESSENCE, DataType);
	
	m_iPileLimit	= m_pDBEssence->pile_num_max;
	m_iPrice		= m_pDBEssence->price;
	m_iShopPrice	= m_pDBEssence->shop_price;
	m_iProcType		= m_pDBEssence->proc_type;
	m_iEquipMask	= 0;
	m_bUseable		= true;

	m_bNeedUpdate	= true;
}

CECIvtrLottery2::CECIvtrLottery2(const CECIvtrLottery2& s) : CECIvtrItem(s)
{
	m_pDBEssence	= s.m_pDBEssence;
}

CECIvtrLottery2::~CECIvtrLottery2()
{
}

//	Set item detail information
bool CECIvtrLottery2::SetItemInfo(BYTE* pInfoData, int iDataLen)
{
// 	if( iDataLen == 0 )
// 		memset(&m_Essence.dice_data, 0, sizeof(m_Essence.dice_data));
// 	else
// 		memcpy(m_Essence.dice_data, pInfoData, min(6, min(iDataLen, m_pDBEssence->dice_count)));

	CECIvtrItem::SetItemInfo(pInfoData, iDataLen);
	return true;
}


//	Get item icon file name
const char* CECIvtrLottery2::GetIconFile()
{
	return g_pGame->GetDataPath(m_pDBEssence->file_icon);
}

//	Get item name
const wchar_t* CECIvtrLottery2::GetName()
{
	return m_pDBEssence->name;
}

//	Get item cool time
int CECIvtrLottery2::GetCoolTime(int* piMax/* NULL */)
{
	CECHostPlayer* pHost = g_pGame->GetGameRun()->GetHostPlayer();
	return pHost ? pHost->GetCoolTime(GP_CT_LOTTERY, piMax) : 0;
}

//	Get item description text
const wchar_t* CECIvtrLottery2::GetNormalDesc(bool bRepair)
{
	if (m_bNeedUpdate)
		return NULL;

	m_strDesc = _AL("");

	//	Try to build item description
	CECStringTab* pDescTab = g_pGame->GetItemDesc();
	int red = ITEMDESC_COL_RED;
	int white = ITEMDESC_COL_WHITE;
	int name_color = DecideNameCol();

	if (m_iCount > 1)
		AddDescText(name_color, true, pDescTab->GetWideString(ITEMDESC_NAMENUMBER), GetName(), m_iCount);
	else
		AddDescText(name_color, true, pDescTab->GetWideString(ITEMDESC_NAME), GetName());

//  将“彩票”去掉
//	AddDescText(white, true, pDescTab->GetWideString(ITEMDESC_LOTTERY));

// 	if (m_pDBEssence->type == 0)
// 	{
// 		ACString strDiced;
// 		int nDiced = 0;
// 		for(int i=0; i<6; i++)
// 		{
// 			if( m_Essence.dice_data[i] > 0 && m_Essence.dice_data[i] <= 32 )
// 			{
// 				strDiced = strDiced + _AL("  ") + m_pDBEssence->candidates[m_Essence.dice_data[i] - 1].desc;
// 				nDiced ++;
// 			}
// 			else
// 				break;
// 		}
// //		AddDescText(ITEMDESC_COL_GREEN, true, pDescTab->GetWideString(ITEMDESC_DICED), nDiced, m_pDBEssence->dice_count, strDiced);
// 		if(nDiced)
// 		{
// 			if(m_strRank == _AL(""))
// 				GetAwardRank();
// 			AddDescText(ITEMDESC_COL_GREEN, true, pDescTab->GetWideString(ITEMDESC_NEW_DICED), m_strRank);
// 		}
// 		else
// 			AddDescText(ITEMDESC_COL_GREEN, true, pDescTab->GetWideString(ITEMDESC_UNDRAW_LOTTERY));
// 	}
// 	else if(m_pDBEssence->type == 1)
// 	{
// 		int index = -1;
// 		if(m_Essence.dice_data[0] != 0)
// 		{	
// 			if(m_pDBEssence->dice_count ==2)
// 				index = (m_Essence.dice_data[0] - 1) * 4 + m_Essence.dice_data[1] - 1;
// 			else if(m_pDBEssence->dice_count ==1)
// 				index = m_Essence.dice_data[0] -1;
// 		}
// 		if (index >= 0 && index < sizeof(m_pDBEssence->candidates) / sizeof(m_pDBEssence->candidates[0]))
// 			AddDescText(ITEMDESC_COL_GREEN, true, pDescTab->GetWideString(ITEMDESC_NEW_DICED), m_pDBEssence->candidates[index].desc);
// 		else
// 			AddDescText(ITEMDESC_COL_GREEN, true, pDescTab->GetWideString(ITEMDESC_UNDRAW_LOTTERY));
// 	}

	AddStateDesc(pDescTab);
	AddExpireTimeDesc();

	//	Price
	AddPriceDesc(white, bRepair);

	//	Extend description
	AddExtDescText();

	return m_strDesc;
}

//	Check item use condition
bool CECIvtrLottery2::CheckUseCondition()
{
	if (!m_pDBEssence)
		return false;
	
	if (!CheckCrossUseCondition())
		return false;

	return true;
}

//	Get drop model for shown
const char * CECIvtrLottery2::GetDropModel()
{
	return g_pGame->GetDataPath(m_pDBEssence->file_matter);
}

// ACString CECIvtrLottery::_Serialize() const
// {
// 	return ItemMakeString(&m_Essence);
// }
// 
// bool CECIvtrLottery::_Unserialize(const ACHAR* szText)
// {
// 	ItemFromString(szText, &m_Essence);
// 	m_bNeedUpdate = false;
// 	return true;
// }


///////////////////////////////////////////////////////////////////////////
//
//CECIvtrLottery3类的实现
//
///////////////////////////////////////////////////////////////////////////
CECIvtrLottery3::CECIvtrLottery3(int tid, int expire_date) : CECIvtrItem(tid, expire_date)
{
	m_iCID	= ICID_LOTTERY3;

	//	Get database data
	elementdataman* pDB = g_pGame->GetElementDataMan();
	DATA_TYPE DataType;
	m_pDBEssence = (LOTTERY3_ESSENCE*)pDB->get_data_ptr(tid, ID_SPACE_ESSENCE, DataType);
	
	m_iPileLimit	= m_pDBEssence->pile_num_max;
	m_iPrice		= m_pDBEssence->price;
	m_iShopPrice	= m_pDBEssence->shop_price;
	m_iProcType		= m_pDBEssence->proc_type;
	m_iEquipMask	= 0;
	m_bUseable		= true;

	m_bNeedUpdate	= true;
}

CECIvtrLottery3::CECIvtrLottery3(const CECIvtrLottery3& lottery) : CECIvtrItem(lottery)
{
	m_pDBEssence	= lottery.m_pDBEssence;
}

CECIvtrLottery3::~CECIvtrLottery3()
{
}

//	Set item detail information
bool CECIvtrLottery3::SetItemInfo(BYTE* pInfoData, int iDataLen)
{
	CECIvtrItem::SetItemInfo(pInfoData, iDataLen);
	return true;
}

//	Get item icon file name
const char* CECIvtrLottery3::GetIconFile()
{
	return g_pGame->GetDataPath(m_pDBEssence->file_icon);
}

//	Get item name
const wchar_t* CECIvtrLottery3::GetName()
{
	return m_pDBEssence->name;
}

//	Get item cool time
int CECIvtrLottery3::GetCoolTime(int* piMax/* NULL */)
{
	CECHostPlayer* pHost = g_pGame->GetGameRun()->GetHostPlayer();
	return pHost ? pHost->GetCoolTime(GP_CT_LOTTERY, piMax) : 0;
}

//	Get item description text
const wchar_t* CECIvtrLottery3::GetNormalDesc(bool bRepair)
{
	if (m_bNeedUpdate)
		return NULL;

	m_strDesc = _AL("");

	//	Try to build item description
	CECStringTab* pDescTab = g_pGame->GetItemDesc();
	int red = ITEMDESC_COL_RED;
	int white = ITEMDESC_COL_WHITE;
	int name_color = DecideNameCol();

	if (m_iCount > 1)
	{
		AddDescText(name_color, true, pDescTab->GetWideString(ITEMDESC_NAMENUMBER), GetName(), m_iCount);
	}
	else
	{
		AddDescText(name_color, true, pDescTab->GetWideString(ITEMDESC_NAME), GetName());
	}

	// Bind info.
	AddStateDesc(pDescTab);

	//
	AddExpireTimeDesc();

	//	Price
	AddPriceDesc(white, bRepair);

	//	Extend description
	AddExtDescText();

	return m_strDesc;
}

//	Check item use condition
bool CECIvtrLottery3::CheckUseCondition()
{
	if (!m_pDBEssence)
		return false;
	
	if (!CheckCrossUseCondition())
		return false;

	return true;
}

//	Get drop model for shown
const char * CECIvtrLottery3::GetDropModel()
{
	return g_pGame->GetDataPath(m_pDBEssence->file_matter);
}

///////////////////////////////////////////////////////////////////////////
//	
//	Implement CECCamRecorder
//	
///////////////////////////////////////////////////////////////////////////

CECCamRecorder::CECCamRecorder(int tid, int expire_date) : CECIvtrItem(tid, expire_date)
{
	m_iCID	= ICID_CAMRECORDER;

	//	Get database data
	elementdataman* pDB = g_pGame->GetElementDataMan();
	DATA_TYPE DataType;
	m_pDBEssence = (CAMRECORDER_ESSENCE*)pDB->get_data_ptr(tid, ID_SPACE_ESSENCE, DataType);
	
	m_iPileLimit	= m_pDBEssence->pile_num_max;
	m_iPrice		= m_pDBEssence->price;
	m_iShopPrice	= m_pDBEssence->shop_price;
	m_iProcType		= m_pDBEssence->proc_type;
	m_iEquipMask	= 0;
	m_bUseable		= true;

	m_bNeedUpdate	= false;
}

CECCamRecorder::CECCamRecorder(const CECCamRecorder& s) : CECIvtrItem(s)
{
	m_pDBEssence	= s.m_pDBEssence;
}

CECCamRecorder::~CECCamRecorder()
{
}

//	Set item detail information
bool CECCamRecorder::SetItemInfo(BYTE* pInfoData, int iDataLen)
{
	CECIvtrItem::SetItemInfo(pInfoData, iDataLen);
	return true;
}

//	Get item icon file name
const char* CECCamRecorder::GetIconFile()
{
	return g_pGame->GetDataPath(m_pDBEssence->file_icon);
}

//	Get item name
const wchar_t* CECCamRecorder::GetName()
{
	return m_pDBEssence->name;
}

//	Get item cool time
int CECCamRecorder::GetCoolTime(int* piMax/* NULL */)
{
	CECHostPlayer* pHost = g_pGame->GetGameRun()->GetHostPlayer();
	return pHost ? pHost->GetCoolTime(GP_CT_CAMRECORDER, piMax) : 0;
	return 0;
}

//	Get item description text
const wchar_t* CECCamRecorder::GetNormalDesc(bool bRepair)
{
	if (m_bNeedUpdate)
		return NULL;

	m_strDesc = _AL("");

	//	Try to build item description
	CECStringTab* pDescTab = g_pGame->GetItemDesc();
	int red = ITEMDESC_COL_RED;
	int white = ITEMDESC_COL_WHITE;
	int name_color = DecideNameCol();

	if (m_iCount > 1)
		AddDescText(name_color, true, pDescTab->GetWideString(ITEMDESC_NAMENUMBER), GetName(), m_iCount);
	else
		AddDescText(name_color, true, pDescTab->GetWideString(ITEMDESC_NAME), GetName());

	AddStateDesc(pDescTab);
	AddExpireTimeDesc();

	//	Price
	AddPriceDesc(white, bRepair);

	//	Extend description
	AddExtDescText();

	return m_strDesc;
}

//	Check item use condition
bool CECCamRecorder::CheckUseCondition()
{
	if (!m_pDBEssence)
		return false;

#ifndef _INVENTORY_DESC_MAN //Added by Ford.W 2010-07-16.	
	// if already show record window, just ignore it.
	PAUIDIALOG pDlg =  g_pGame->GetGameRun()->GetUIManager()->GetInGameUIMan()->GetDialog("Win_Record");
	if( pDlg && pDlg->IsShow() )
		return false;
	pDlg = g_pGame->GetGameRun()->GetUIManager()->GetInGameUIMan()->GetDialog("Win_RecordEnd");
	if( pDlg && pDlg->IsShow() )
		return false;
#endif
	if (!CheckCrossUseCondition())
		return false;

	return true;
}

//	Get drop model for shown
const char * CECCamRecorder::GetDropModel()
{
	return g_pGame->GetDataPath(m_pDBEssence->file_matter);
}

///////////////////////////////////////////////////////////////////////////
//	
//	Implement CECIvtrTalismanExpFood
//	
///////////////////////////////////////////////////////////////////////////

CECIvtrTalismanExpFood::CECIvtrTalismanExpFood(int tid, int expire_date) : CECIvtrItem(tid, expire_date)
{
	m_iCID	= ICID_TALISMAN_EXPFOOD;

	//	Get database data
	elementdataman* pDB = g_pGame->GetElementDataMan();
	DATA_TYPE DataType;
	m_pDBEssence = (TALISMAN_EXPFOOD_ESSENCE*)pDB->get_data_ptr(tid, ID_SPACE_ESSENCE, DataType);
	
	m_iPileLimit	= m_pDBEssence->pile_num_max;
	m_iPrice		= m_pDBEssence->price;
	m_iShopPrice	= m_pDBEssence->shop_price;
	m_iProcType		= m_pDBEssence->proc_type;
	m_iEquipMask	= 0;
	m_bNeedUpdate	= false;
}

CECIvtrTalismanExpFood::CECIvtrTalismanExpFood(const CECIvtrTalismanExpFood& s) : CECIvtrItem(s)
{
	m_pDBEssence	= s.m_pDBEssence;
}

CECIvtrTalismanExpFood::~CECIvtrTalismanExpFood()
{
}

//	Set item detail information
bool CECIvtrTalismanExpFood::SetItemInfo(BYTE* pInfoData, int iDataLen)
{
	CECIvtrItem::SetItemInfo(pInfoData, iDataLen);
	return true;
}

//	Get item icon file name
const char* CECIvtrTalismanExpFood::GetIconFile()
{
	return g_pGame->GetDataPath(m_pDBEssence->file_icon);
}

//	Get item name
const wchar_t* CECIvtrTalismanExpFood::GetName()
{
	return m_pDBEssence->name;
}

//	Get item description text
const wchar_t* CECIvtrTalismanExpFood::GetNormalDesc(bool bRepair)
{
	if (m_bNeedUpdate)
		return NULL;

	m_strDesc = _AL("");

	//	Try to build item description
	CECStringTab* pDescTab = g_pGame->GetItemDesc();
	int red = ITEMDESC_COL_RED;
	int white = ITEMDESC_COL_WHITE;
	int name_color = DecideNameCol();

	if (m_iCount > 1)
		AddDescText(name_color, true, pDescTab->GetWideString(ITEMDESC_NAMENUMBER), GetName(), m_iCount);
	else
		AddDescText(name_color, true, pDescTab->GetWideString(ITEMDESC_NAME), GetName());

	AddDescText(white, true, pDescTab->GetWideString(ITEMDESC_TALISMAN_EXPFOOD), m_pDBEssence->exp_added);

	AddStateDesc(pDescTab);

	AddExpireTimeDesc();

	//	Price
	AddPriceDesc(white, bRepair);

	//	Extend description
	AddExtDescText();

	return m_strDesc;
}

//	Get drop model for shown
const char * CECIvtrTalismanExpFood::GetDropModel()
{
	return g_pGame->GetDataPath(m_pDBEssence->file_matter);
}

///////////////////////////////////////////////////////////////////////////
//	
//	Implement CECIvtrTalismanMergeKatalyst
//	
///////////////////////////////////////////////////////////////////////////

CECIvtrTalismanMergeKatalyst::CECIvtrTalismanMergeKatalyst(int tid, int expire_date) : CECIvtrItem(tid, expire_date)
{
	m_iCID	= ICID_TALISMAN_MERGEKATALYST;

	//	Get database data
	elementdataman* pDB = g_pGame->GetElementDataMan();
	DATA_TYPE DataType;
	m_pDBEssence = (TALISMAN_MERGEKATALYST_ESSENCE*)pDB->get_data_ptr(tid, ID_SPACE_ESSENCE, DataType);
	
	m_iPileLimit	= m_pDBEssence->pile_num_max;
	m_iPrice		= m_pDBEssence->price;
	m_iShopPrice	= m_pDBEssence->shop_price;
	m_iProcType		= m_pDBEssence->proc_type;
	m_iEquipMask	= 0;
	m_bNeedUpdate	= false;
}

CECIvtrTalismanMergeKatalyst::CECIvtrTalismanMergeKatalyst(const CECIvtrTalismanMergeKatalyst& s) : CECIvtrItem(s)
{
	m_pDBEssence	= s.m_pDBEssence;
}

CECIvtrTalismanMergeKatalyst::~CECIvtrTalismanMergeKatalyst()
{
}

//	Set item detail information
bool CECIvtrTalismanMergeKatalyst::SetItemInfo(BYTE* pInfoData, int iDataLen)
{
	CECIvtrItem::SetItemInfo(pInfoData, iDataLen);
	return true;
}

//	Get item icon file name
const char* CECIvtrTalismanMergeKatalyst::GetIconFile()
{
	return g_pGame->GetDataPath(m_pDBEssence->file_icon);
}

//	Get item name
const wchar_t* CECIvtrTalismanMergeKatalyst::GetName()
{
	return m_pDBEssence->name;
}

//	Get item description text
const wchar_t* CECIvtrTalismanMergeKatalyst::GetNormalDesc(bool bRepair)
{
	if (m_bNeedUpdate)
		return NULL;

	m_strDesc = _AL("");

	//	Try to build item description
	CECStringTab* pDescTab = g_pGame->GetItemDesc();
	int red = ITEMDESC_COL_RED;
	int white = ITEMDESC_COL_WHITE;
	int name_color = DecideNameCol();

	if (m_iCount > 1)
		AddDescText(name_color, true, pDescTab->GetWideString(ITEMDESC_NAMENUMBER), GetName(), m_iCount);
	else
		AddDescText(name_color, true, pDescTab->GetWideString(ITEMDESC_NAME), GetName());

	AddStateDesc(pDescTab);

	AddExpireTimeDesc();

	//	Price
	AddPriceDesc(white, bRepair);

	//	Extend description
	AddExtDescText();

	return m_strDesc;
}

//	Get drop model for shown
const char * CECIvtrTalismanMergeKatalyst::GetDropModel()
{
	return g_pGame->GetDataPath(m_pDBEssence->file_matter);
}


///////////////////////////////////////////////////////////////////////////
//	
//	Implement CECIvtrTalismanEnergyFood
//	
///////////////////////////////////////////////////////////////////////////

CECIvtrTalismanEnergyFood::CECIvtrTalismanEnergyFood(int tid, int expire_date) : CECIvtrItem(tid, expire_date)
{
	m_iCID	= ICID_TALISMAN_ENERGYFOOD;

	//	Get database data
	elementdataman* pDB = g_pGame->GetElementDataMan();
	DATA_TYPE DataType;
	m_pDBEssence = (TALISMAN_ENERGYFOOD_ESSENCE*)pDB->get_data_ptr(tid, ID_SPACE_ESSENCE, DataType);
	
	m_iPileLimit	= m_pDBEssence->pile_num_max;
	m_iPrice		= m_pDBEssence->price;
	m_iShopPrice	= m_pDBEssence->shop_price;
	m_iProcType		= m_pDBEssence->proc_type;
	m_iEquipMask	= 0;
}

CECIvtrTalismanEnergyFood::CECIvtrTalismanEnergyFood(const CECIvtrTalismanEnergyFood& s) : CECIvtrItem(s)
{
	m_Essence		= s.m_Essence;
	m_pDBEssence	= s.m_pDBEssence;
}

CECIvtrTalismanEnergyFood::~CECIvtrTalismanEnergyFood()
{
}

//	Set item detail information
bool CECIvtrTalismanEnergyFood::SetItemInfo(BYTE* pInfoData, int iDataLen)
{
	if( iDataLen != sizeof(int) )
		return false;
	m_Essence.energy_left = *(int *)pInfoData;

	CECIvtrItem::SetItemInfo(pInfoData, iDataLen);
	return true;
}

//	Get item icon file name
const char* CECIvtrTalismanEnergyFood::GetIconFile()
{
	return g_pGame->GetDataPath(m_pDBEssence->file_icon);
}

//	Get item name
const wchar_t* CECIvtrTalismanEnergyFood::GetName()
{
	return m_pDBEssence->name;
}

//	Get item description text
const wchar_t* CECIvtrTalismanEnergyFood::GetNormalDesc(bool bRepair)
{
	if (m_bNeedUpdate)
		return NULL;

	m_strDesc = _AL("");

	//	Try to build item description
	CECStringTab* pDescTab = g_pGame->GetItemDesc();
	int red = ITEMDESC_COL_RED;
	int white = ITEMDESC_COL_WHITE;
	int name_color = DecideNameCol();

	if (m_iCount > 1)
		AddDescText(name_color, true, pDescTab->GetWideString(ITEMDESC_NAMENUMBER), GetName(), m_iCount);
	else
		AddDescText(name_color, true, pDescTab->GetWideString(ITEMDESC_NAME), GetName());

	AddDescText(white, true, pDescTab->GetWideString(ITEMDESC_TALISMAN_ENERGYFOOD), m_Essence.energy_left, m_pDBEssence->energy_total);

	AddStateDesc(pDescTab);

	AddExpireTimeDesc();

	//	Price
	AddPriceDesc(white, bRepair);

	//	Extend description
	AddExtDescText();

	return m_strDesc;
}

//	Get drop model for shown
const char * CECIvtrTalismanEnergyFood::GetDropModel()
{
	return g_pGame->GetDataPath(m_pDBEssence->file_matter);
}

//	Check item use condition
bool CECIvtrTalismanEnergyFood::CheckUseCondition()
{
	if (!CheckCrossUseCondition())
		return false;

	CECGameUIMan *pUIMan = g_pGame->GetGameRun()->GetUIManager()->GetInGameUIMan();
	CECInventory *pPack = g_pGame->GetGameRun()->GetHostPlayer()->GetPack(IVTRTYPE_EQUIPPACK);
	if( pPack->GetItem(16) && pPack->GetItem(16)->GetClassID() == CECIvtrItem::ICID_TALISMAN_MAINPART  )
		return true;

	return false;
}

ACString CECIvtrTalismanEnergyFood::_Serialize() const
{
	return m_Essence.MakeString();
}

bool CECIvtrTalismanEnergyFood::_Unserialize(const ACHAR* szText)
{
	m_Essence.FromString(szText);
	m_bNeedUpdate = false;
	return true;
}

///////////////////////////////////////////////////////////////////////////
//	
//	Implement CECPetBedge
//	
///////////////////////////////////////////////////////////////////////////

CECPetBedge::CECPetBedge(int tid, int expire_date) : CECIvtrItem(tid, expire_date)
{
	m_iCID	= ICID_PET_BEDGE;

	memset(&m_Essence, 0, sizeof (m_Essence));

	//	Get database data
	elementdataman* pDB = g_pGame->GetElementDataMan();
	DATA_TYPE DataType;
	m_pDBEssence = (PET_BEDGE_ESSENCE*)pDB->get_data_ptr(tid, ID_SPACE_ESSENCE, DataType);
	memset(&m_CoolDown, 0, sizeof(m_CoolDown));

	m_iPileLimit	= m_pDBEssence->pile_num_max;
	m_iPrice		= m_pDBEssence->price;
	m_iShopPrice	= m_pDBEssence->shop_price;
	m_iProcType		= m_pDBEssence->proc_type;
	m_iEquipMask	= 0;
	m_bUseable		= true;
}

CECPetBedge::CECPetBedge(const CECPetBedge& s) : CECIvtrItem(s)
{
	m_pDBEssence	= s.m_pDBEssence;
	m_Essence		= s.m_Essence;
	memcpy(m_CoolDown, s.m_CoolDown, sizeof(m_CoolDown));
}

CECPetBedge::~CECPetBedge()
{
}

//	Set item detail information
bool CECPetBedge::SetItemInfo(BYTE* pInfoData, int iDataLen)
{
	if (iDataLen >= sizeof(itemdataman::_pet_bedge_essence))
		memcpy(&m_Essence, pInfoData, sizeof(m_Essence));
	else
		memset(&m_Essence, 0, sizeof(m_Essence));

	CECIvtrItem::SetItemInfo(pInfoData, iDataLen);
	return true;
}

GNET::Octets CECPetBedge::gen_item_info()
{
	return GNET::Octets(&m_Essence, sizeof(m_Essence));
}

//	Get item icon file name
const char* CECPetBedge::GetIconFile()
{
	return g_pGame->GetDataPath(m_pDBEssence->file_icon);
}

//	Get item name
const wchar_t* CECPetBedge::GetName()
{
	int len = a_strlen((ACHAR*)m_Essence.name);

	if (len == 0)
		return m_pDBEssence->name;
	else
		return (wchar_t*)m_Essence.name;
}

//	Get item cool time
int CECPetBedge::GetCoolTime(int* piMax/* NULL */)
{
	return 0;
}

//	Get item description text
const wchar_t* CECPetBedge::GetNormalDesc(bool bRepair)
{
	if (m_bNeedUpdate)
		return NULL;

	m_strDesc = _AL("");

	//	Try to build item description
	CECStringTab* pDescTab = g_pGame->GetItemDesc();
	CECStringTab* pFixMsg = g_pGame->GetFixedMsgTab();

	int red = ITEMDESC_COL_RED;
	int white = ITEMDESC_COL_WHITE;
	int name_color = ITEMDESC_COL_YELLOW;
	ACString strText;

	ACHAR szText[512];
	AUI_ConvertChatString(GetName(), szText, false);
	AUICTranslate trans;
	AWString strName = trans.ReverseTranslate(szText);		// 名字中有'\\'，转为'\\'+'\\'，因为updatehint的时候会调用translate
	
	if (m_iCount > 1)
		AddDescText(name_color, true, pDescTab->GetWideString(ITEMDESC_NAMENUMBER), strName, m_iCount);
	else
		AddDescText(name_color, true, pDescTab->GetWideString(ITEMDESC_NAME), strName);

	if ( m_Essence.reborn_cnt>0 )
	{
		CECHostPlayer* pHost = g_pGame->GetGameRun()->GetHostPlayer();
		int col = (pHost->GetRebornCount() > 0) ? ITEMDESC_COL_PURPLE : ITEMDESC_COL_RED;
		AddDescText(col, true, _AL("%s"), pDescTab->GetWideString(ITEMDESC_RENASCENCE_1));
	}

	AddStateDesc(pDescTab);

	AddExpireTimeDesc();

	if (m_pDBEssence->proc_type & (1 << 4))
		AddDescText(white, true, _AL("%s"), pDescTab->GetWideString(ITEMDESC_CANT_TRADE));

	if (m_pDBEssence->proc_type & (1 << 2))
		AddDescText(white, true, _AL("%s"), pDescTab->GetWideString(ITEMDESC_CANT_SELL));

	AddDescText(white, true, pDescTab->GetWideString(ITEMDESC_PET_RACE), m_pDBEssence->name);
	AddDescText(white, true, pDescTab->GetWideString(ITEMDESC_PET_LEVEL), m_Essence.level);
	//AddDescText(white, true, pDescTab->GetWideString(ITEMDESC_PET_ORG), pDescTab->GetWideString(ITEMDESC_PET_ORG0+m_Essence.orgin));
	
	AUI_ConvertChatString(GetOwnerName(), szText, false);
	AWString ownerName = trans.ReverseTranslate(szText);		// 名字中有'\\'，转为'\\'+'\\'，因为updatehint的时候会调用translate
	AddDescText(ITEMDESC_COL_YELLOW, true, pDescTab->GetWideString(ITEMDESC_PET_MASTER), ownerName);	// 名字中有&, 名字中不允许^

	if (m_Essence.identify == 0)
	{
		AddDescText(white, true, pDescTab->GetWideString(ITEMDESC_PET_BASIC_HP), m_Essence.attr[itemdataman::_pet_bedge_essence::MAX_HP].cur_value);
		AddDescText(white, true, pDescTab->GetWideString(ITEMDESC_PET_BASIC_VIGOR), m_Essence.attr[itemdataman::_pet_bedge_essence::MAX_VIGOR].cur_value);
		AddDescText(white, true, pDescTab->GetWideString(ITEMDESC_PET_BASIC_ATTACK), m_Essence.attr[itemdataman::_pet_bedge_essence::MIN_ATTACK].cur_value, m_Essence.attr[itemdataman::_pet_bedge_essence::MAX_ATTACK].cur_value);
		AddDescText(white, true, pDescTab->GetWideString(ITEMDESC_PET_BASIC_DEFENCE), m_Essence.attr[itemdataman::_pet_bedge_essence::DEFENCE].cur_value);
		AddDescText(white, true, pDescTab->GetWideString(ITEMDESC_PET_BASIC_HIT), m_Essence.attr[itemdataman::_pet_bedge_essence::HIT].cur_value);
		AddDescText(white, true, pDescTab->GetWideString(ITEMDESC_PET_BASIC_JOUK), m_Essence.attr[itemdataman::_pet_bedge_essence::JOUK].cur_value);
	}
	else
	{
		AddDescText(ITEMDESC_COL_RED, true, pDescTab->GetWideString(ITEMDESC_PET_MAINTYPE), pDescTab->GetWideString(ITEMDESC_PET_MAINTYPE0+m_Essence.main_type));
		int t_c = 0;
		AWString s;

		int i;
		for (i = 0; i < 13; i++)
		{
			if (m_Essence.talents[i] == 10)
			{
				CECSkill* pSkill = new CECSkill(1210 + i, 1);

				if (t_c == 0)		
					s = g_pGame->GetSkillDesc()->GetWideString(pSkill->GetSkillID() * 10);
				//	s = pSkill->GetName();
				else
				{
					s += _AL("/");
					s += g_pGame->GetSkillDesc()->GetWideString(pSkill->GetSkillID() * 10);
				}

				delete pSkill;

				if (++t_c == 4)
					break;
			}
		}

		if (t_c < 4)
		{
			for (int i = 0; i < 13; i++)
			{
				if (m_Essence.talents[i] == 5)
				{
					CECSkill* pSkill = new CECSkill(1210 + i, 1);

					if (t_c == 0)
					{
						//s = pSkill->GetName();
						s = g_pGame->GetSkillDesc()->GetWideString(pSkill->GetSkillID() * 10);
					}
					else
					{
						s += _AL("/");
						//s += pSkill->GetName();
						s += g_pGame->GetSkillDesc()->GetWideString(pSkill->GetSkillID() * 10);
					}

					delete pSkill;

					if (++t_c == 4)
						break;
				}
			}
		}

		AddDescText(white, true, pDescTab->GetWideString(ITEMDESC_PET_TALENT), s);

		AddDescText(ITEMDESC_COL_YELLOW, true, pDescTab->GetWideString(ITEMDESC_PET_HOROSCOPE), pDescTab->GetWideString(ITEMDESC_PET_HOROSCOPE0+m_Essence.horoscope));
		ACString strStar;
		if (m_Essence.reborn_star==0)
			strStar.Format(_AL("%d"), m_Essence.star);
		else
			strStar.Format(_AL("%d +%d"), m_Essence.star, m_Essence.reborn_star);
		AddDescText(ITEMDESC_COL_YELLOW, true, pDescTab->GetWideString(ITEMDESC_PET_STAR), strStar);

		if (m_Essence.skills[8].id)
		{
			CECSkill* pSkill = new CECSkill(m_Essence.skills[8].id, m_Essence.skills[8].level);
			AddDescText(ITEMDESC_COL_YELLOW, true, pDescTab->GetWideString(ITEMDESC_PET_SKILL_LEVEL), g_pGame->GetSkillDesc()->GetWideString(pSkill->GetSkillID() * 10), m_Essence.skills[8].level);
			delete pSkill;
		}

		if (m_Essence.skills[11].id)
		{
			CECSkill* pSkill = new CECSkill(m_Essence.skills[11].id, m_Essence.skills[11].level);
			AddDescText(ITEMDESC_COL_YELLOW, true, pDescTab->GetWideString(ITEMDESC_PET_SKILL_LEVEL), g_pGame->GetSkillDesc()->GetWideString(pSkill->GetSkillID() * 10), m_Essence.skills[11].level);
			delete pSkill;
		}

		AddDescText(white, true, pDescTab->GetWideString(ITEMDESC_PET_HP), m_Essence.attr[itemdataman::_pet_bedge_essence::MAX_HP].cur_value, (int)m_Essence.attr[itemdataman::_pet_bedge_essence::MAX_HP].potential);
		AddDescText(white, true, pDescTab->GetWideString(ITEMDESC_PET_VIGOR), m_Essence.attr[itemdataman::_pet_bedge_essence::MAX_VIGOR].cur_value, (int)m_Essence.attr[itemdataman::_pet_bedge_essence::MAX_VIGOR].potential);
		AddDescText(white, true, pDescTab->GetWideString(ITEMDESC_PET_ATTACK), m_Essence.attr[itemdataman::_pet_bedge_essence::MIN_ATTACK].cur_value, m_Essence.attr[itemdataman::_pet_bedge_essence::MAX_ATTACK].cur_value, (int)m_Essence.attr[itemdataman::_pet_bedge_essence::MIN_ATTACK].potential, (int)m_Essence.attr[itemdataman::_pet_bedge_essence::MAX_ATTACK].potential);
		AddDescText(white, true, pDescTab->GetWideString(ITEMDESC_PET_DEFENCE), m_Essence.attr[itemdataman::_pet_bedge_essence::DEFENCE].cur_value, (int)m_Essence.attr[itemdataman::_pet_bedge_essence::DEFENCE].potential);
		AddDescText(white, true, pDescTab->GetWideString(ITEMDESC_PET_HIT), m_Essence.attr[itemdataman::_pet_bedge_essence::HIT].cur_value, (int)m_Essence.attr[itemdataman::_pet_bedge_essence::HIT].potential);
		AddDescText(white, true, pDescTab->GetWideString(ITEMDESC_PET_JOUK), m_Essence.attr[itemdataman::_pet_bedge_essence::JOUK].cur_value, (int)m_Essence.attr[itemdataman::_pet_bedge_essence::JOUK].potential);

		AddDescText(white, true, pDescTab->GetWideString(ITEMDESC_PET_RESISTANCE0), m_Essence.attr[itemdataman::_pet_bedge_essence::RESISTANCE1].cur_value, (int)m_Essence.attr[itemdataman::_pet_bedge_essence::RESISTANCE1].potential);
		AddDescText(white, true, pDescTab->GetWideString(ITEMDESC_PET_RESISTANCE1), m_Essence.attr[itemdataman::_pet_bedge_essence::RESISTANCE2].cur_value, (int)m_Essence.attr[itemdataman::_pet_bedge_essence::RESISTANCE2].potential);
		AddDescText(white, true, pDescTab->GetWideString(ITEMDESC_PET_RESISTANCE2), m_Essence.attr[itemdataman::_pet_bedge_essence::RESISTANCE3].cur_value, (int)m_Essence.attr[itemdataman::_pet_bedge_essence::RESISTANCE3].potential);
		AddDescText(white, true, pDescTab->GetWideString(ITEMDESC_PET_RESISTANCE3), m_Essence.attr[itemdataman::_pet_bedge_essence::RESISTANCE4].cur_value, (int)m_Essence.attr[itemdataman::_pet_bedge_essence::RESISTANCE4].potential);
		AddDescText(white, true, pDescTab->GetWideString(ITEMDESC_PET_RESISTANCE4), m_Essence.attr[itemdataman::_pet_bedge_essence::RESISTANCE5].cur_value, (int)m_Essence.attr[itemdataman::_pet_bedge_essence::RESISTANCE5].potential);
		// AddDescText(white, true, pDescTab->GetWideString(ITEMDESC_PET_RESISTANCE5), m_Essence.attr[itemdataman::_pet_bedge_essence::RESISTANCE6].cur_value, (int)m_Essence.attr[itemdataman::_pet_bedge_essence::RESISTANCE6].potential);

		if (m_Essence.reborn_cnt>0)
		{
			AddDescText(white, true, _AL("%s"), pDescTab->GetWideString(ITEMDESC_PET_LIFE_PERFECT));
			AddDescText(white, true, _AL("%s"), pDescTab->GetWideString(ITEMDESC_PET_HUNGER_NEVER));
			AddDescText(white, true, _AL("%s"), pDescTab->GetWideString(ITEMDESC_PET_HONOR_PERFECT));
		}
		else
		{
			AddDescText(white, true, pDescTab->GetWideString(ITEMDESC_PET_LIFE), m_Essence.age, m_Essence.life);
			AddDescText(white, true, pDescTab->GetWideString(ITEMDESC_PET_HUNGER), m_Essence.cur_hunger_point, m_Essence.max_hunger_point);
			AddDescText(white, true, pDescTab->GetWideString(ITEMDESC_PET_HONOR), m_Essence.cur_honor_point, m_Essence.max_honor_point);
		}
		AddDescText(white, true, pDescTab->GetWideString(ITEMDESC_PET_LIVING_SKILL), 
			m_Essence.make_attr[itemdataman::_pet_bedge_essence::ACUITY], 
			m_Essence.make_attr[itemdataman::_pet_bedge_essence::STAMINA], 
			m_Essence.make_attr[itemdataman::_pet_bedge_essence::SAWY], 
			m_Essence.make_attr[itemdataman::_pet_bedge_essence::DOWRY], 
			m_Essence.make_attr[itemdataman::_pet_bedge_essence::WISDOM], 
			m_Essence.make_attr[itemdataman::_pet_bedge_essence::CNR]);

		int count = 0;
		for (i = 0; i < 8; i++)
		{
			itemdataman::_pet_bedge_essence::skill_t& s = m_Essence.skills[i];

			if (s.id)
			{
				count++;
				CECSkill* pSkill = new CECSkill(s.id, s.level);
				AddDescText(ITEMDESC_COL_LIGHTBLUE, true, pDescTab->GetWideString(ITEMDESC_PET_SKILL), count, g_pGame->GetSkillDesc()->GetWideString(pSkill->GetSkillID() * 10), s.level);
				delete pSkill;
			}
		}
		for (i = 12; i < 16; i++)
		{
			itemdataman::_pet_bedge_essence::skill_t& s = m_Essence.skills[i];

			if (s.id)
			{
				count++;
				CECSkill* pSkill = new CECSkill(s.id, s.level);
				AddDescText(ITEMDESC_COL_LIGHTBLUE, true, pDescTab->GetWideString(ITEMDESC_PET_SKILL), count, g_pGame->GetSkillDesc()->GetWideString(pSkill->GetSkillID() * 10), s.level);
				delete pSkill;
			}
		}
	}

	//	Price
	AddPriceDesc(white, bRepair);

	//	Extend description
	AddExtDescText();

	if (m_Essence.owner_id || m_pDBEssence->level == 999)
	{
		m_strDesc += _AL("\\r");
		AddDescText(white, false, pDescTab->GetWideString(ITEMDESC_PET_ADOPT));
	}
	else
	{
		m_strDesc += _AL("\\r");
		AddDescText(white, false, pDescTab->GetWideString(ITEMDESC_PET_UN_ADOPT));
	}

	return m_strDesc;
}

ACString CECPetBedge::_Serialize() const
{
	const size_t sz = sizeof(m_Essence);
	assert(sz == 450);

	DWORD c_sz = 1024;
	unsigned char out[1024];
	AFilePackage::Compress((unsigned char*)&m_Essence, sz, out, &c_sz);
	ACString str = BufferMakeString(out, c_sz);
	return str;
}

bool CECPetBedge::_Unserialize(const ACHAR* szText)
{
	size_t sz;
	DWORD sz_u = 1024;
	unsigned char buf[1024];
	unsigned char* p = BufferFromString(szText, sz);

	if (AFilePackage::Uncompress(p, sz, buf, &sz_u) != 0)
	{
		delete[] p;
		return false;
	}

	if (sz_u != sizeof(m_Essence))
	{
		delete[] p;
		return false;
	}

	memcpy(&m_Essence, buf, sizeof(m_Essence));
	delete[] p;
	m_bNeedUpdate = false;
	return true;
}

//	Check item use condition
bool CECPetBedge::CheckUseCondition()
{
	if (!m_pDBEssence)
		return false;
	if (!CheckCrossUseCondition())
		return false;

	return true;
}

//	Get drop model for shown
const char * CECPetBedge::GetDropModel()
{
	return g_pGame->GetDataPath(m_pDBEssence->file_matter);
}

void CECPetBedge::RefreshCoolDown(DWORD dwTick)
{
	//	Update cool time
	for (int i = 0; i < 8; i++)
	{
		PET_SKILL_COOLDOWN& s = m_CoolDown[i];

		if (m_Essence.skills[i].id && s.iCoolMax && s.iCoolCnt)
		{
			s.iCoolCnt -= dwTick;
			a_ClampFloor(s.iCoolCnt, 0);
		}
	}
}

//	Set skill cool time
void CECPetBedge::SetSkillCoolTime(int iCoolIdx, int iTime)
{
	if (iCoolIdx <= GP_CT_SKILL_START)
		return;

	int idSkill = iCoolIdx - GP_CT_SKILL_START;

	for (int i = 0; i < 8; i++)
	{
		if (m_Essence.skills[i].id == idSkill)
		{
			m_CoolDown[i].iCoolMax	= iTime;
			m_CoolDown[i].iCoolCnt	= iTime;
			m_CoolDown[i].iCoolFinishTime = iTime + GetTickCount();
			return;
		}
	}
}

//	Get skill cool time
int CECPetBedge::GetSkillCoolTime(int iSkillIdx, int* piMax/* NULL */)
{
	if (iSkillIdx >= 0 && iSkillIdx < 8)
	{
		if (piMax)
			*piMax = m_CoolDown[iSkillIdx].iCoolMax;
		m_CoolDown[iSkillIdx].iCoolCnt = m_CoolDown[iSkillIdx].iCoolFinishTime - GetTickCount();
		a_ClampFloor(m_CoolDown[iSkillIdx].iCoolCnt, 0);
		return m_CoolDown[iSkillIdx].iCoolCnt;
	}
	else
	{
		if (piMax)
			*piMax = 0;

		return 0;
	}
}

ACString CECPetBedge::GetOwnerName()
{
	int iLen = a_strlen((ACHAR*)GetEssence().owner_name);
	if (iLen>10)
		iLen = 10;
	return ACString((ACHAR*)GetEssence().owner_name, iLen);
}

///////////////////////////////////////////////////////////////////////////
//	
//	Implement CECPetFood
//	
///////////////////////////////////////////////////////////////////////////

CECPetFood::CECPetFood(int tid, int expire_date) : CECIvtrItem(tid, expire_date)
{
	m_iCID	= ICID_PET_FOOD;

	//	Get database data
	elementdataman* pDB = g_pGame->GetElementDataMan();
	DATA_TYPE DataType;
	m_pDBEssence = (PET_FOOD_ESSENCE*)pDB->get_data_ptr(tid, ID_SPACE_ESSENCE, DataType);
	
	m_iPileLimit	= m_pDBEssence->pile_num_max;
	m_iPrice		= m_pDBEssence->price;
	m_iShopPrice	= m_pDBEssence->shop_price;
	m_iProcType		= m_pDBEssence->proc_type;
	m_iEquipMask	= 0;
	m_bUseable		= true;
}

CECPetFood::CECPetFood(const CECPetFood& s) : CECIvtrItem(s)
{
	m_pDBEssence	= s.m_pDBEssence;
}

CECPetFood::~CECPetFood()
{
}

//	Set item detail information
bool CECPetFood::SetItemInfo(BYTE* pInfoData, int iDataLen)
{
	CECIvtrItem::SetItemInfo(pInfoData, iDataLen);
	return true;
}

//	Get item icon file name
const char* CECPetFood::GetIconFile()
{
	return g_pGame->GetDataPath(m_pDBEssence->file_icon);
}

//	Get item name
const wchar_t* CECPetFood::GetName()
{
	return m_pDBEssence->name;
}

//	Get item cool time
int CECPetFood::GetCoolTime(int* piMax/* NULL */)
{
	/*
	CECHostPlayer* pHost = g_pGame->GetGameRun()->GetHostPlayer();
	return pHost ? pHost->GetCoolTime(GP_CT_PETFOOD, piMax) : 0;
	*/
	
	return 0;
}

//	Get item description text
const wchar_t* CECPetFood::GetNormalDesc(bool bRepair)
{
	if (m_bNeedUpdate)
		return NULL;

	m_strDesc = _AL("");

	//	Try to build item description
	CECStringTab* pDescTab = g_pGame->GetItemDesc();
	CECStringTab* pFixMsg = g_pGame->GetFixedMsgTab();
	int red = ITEMDESC_COL_RED;
	int white = ITEMDESC_COL_WHITE;
	int name_color = DecideNameCol();

	if (m_iCount > 1)
		AddDescText(name_color, true, pDescTab->GetWideString(ITEMDESC_NAMENUMBER), GetName(), m_iCount);
	else
		AddDescText(name_color, true, pDescTab->GetWideString(ITEMDESC_NAME), GetName());

	AddStateDesc(pDescTab);
	AddExpireTimeDesc();

	//	Price
	AddPriceDesc(white, bRepair);

	//	Extend description
	AddExtDescText();

	return m_strDesc;
}

//	Check item use condition
bool CECPetFood::CheckUseCondition()
{
	if (!m_pDBEssence)
		return false;
	if (!CheckCrossUseCondition())
		return false;
	return true;
}

//	Get drop model for shown
const char * CECPetFood::GetDropModel()
{
	return g_pGame->GetDataPath(m_pDBEssence->file_matter);
}


///////////////////////////////////////////////////////////////////////////
//	
//	Implement CECPetAutoFood
//	
///////////////////////////////////////////////////////////////////////////

CECPetAutoFood::CECPetAutoFood(int tid, int expire_date) : CECIvtrItem(tid, expire_date)
{
	m_iCID	= ICID_PET_AUTO_FOOD;

	//	Get database data
	elementdataman* pDB = g_pGame->GetElementDataMan();
	DATA_TYPE DataType;
	m_pDBEssence = (PET_AUTO_FOOD_ESSENCE*)pDB->get_data_ptr(tid, ID_SPACE_ESSENCE, DataType);

	m_iPileLimit	= m_pDBEssence->pile_num_max;
	m_iPrice		= m_pDBEssence->price;
	m_iShopPrice	= m_pDBEssence->shop_price;
	m_iProcType		= m_pDBEssence->proc_type;
	m_iEquipMask	= 0;
	cur_value		= m_pDBEssence->food_value;
	m_bUseable		= true;
}

CECPetAutoFood::CECPetAutoFood(const CECPetAutoFood& s) : CECIvtrItem(s)
{
	m_pDBEssence	= s.m_pDBEssence;
	cur_value		= s.cur_value;
}

CECPetAutoFood::~CECPetAutoFood()
{
}

//	Set item detail information
bool CECPetAutoFood::SetItemInfo(BYTE* pInfoData, int iDataLen)
{
	CECIvtrItem::SetItemInfo(pInfoData, iDataLen);

	if (iDataLen == sizeof(int))
		cur_value = *(int*)pInfoData;

	return true;
}

//	Get item icon file name
const char* CECPetAutoFood::GetIconFile()
{
	return g_pGame->GetDataPath(m_pDBEssence->file_icon);
}

//	Get item name
const wchar_t* CECPetAutoFood::GetName()
{
	return m_pDBEssence->name;
}

//	Get item cool time
int CECPetAutoFood::GetCoolTime(int* piMax/* NULL */)
{

	return 0;
}

//	Get item description text
const wchar_t* CECPetAutoFood::GetNormalDesc(bool bRepair)
{
	if (m_bNeedUpdate)
		return NULL;

	m_strDesc = _AL("");

	//	Try to build item description
	CECStringTab* pDescTab = g_pGame->GetItemDesc();
	CECStringTab* pFixMsg = g_pGame->GetFixedMsgTab();
	int red = ITEMDESC_COL_RED;
	int white = ITEMDESC_COL_WHITE;
	int name_color = DecideNameCol();

	if (m_iCount > 1)
		AddDescText(name_color, true, pDescTab->GetWideString(ITEMDESC_NAMENUMBER), GetName(), m_iCount);
	else
		AddDescText(name_color, true, pDescTab->GetWideString(ITEMDESC_NAME), GetName());

	AddStateDesc(pDescTab);
	AddExpireTimeDesc();

	AddDescText(white, true, pDescTab->GetWideString(ITEMDESC_PET_AUTOFOOD_VALUE), cur_value, m_pDBEssence->food_value);
	AddDescText(white, true, pDescTab->GetWideString(ITEMDESC_PET_AUTOFOOD_GEN1), (int)m_pDBEssence->hp_gen);
	AddDescText(white, true, pDescTab->GetWideString(ITEMDESC_PET_AUTOFOOD_GEN2), (int)m_pDBEssence->mp_gen);
	AddDescText(white, true, pDescTab->GetWideString(ITEMDESC_PET_AUTOFOOD_GEN3), (int)m_pDBEssence->hunger_gen);

	//	Price
	AddPriceDesc(white, bRepair);

	//	Extend description
	AddExtDescText();

	return m_strDesc;
}

//	Check item use condition
bool CECPetAutoFood::CheckUseCondition()
{
	if (!m_pDBEssence)
		return false;
	if (!CheckCrossUseCondition())
		return false;
	return true;
}

//	Get drop model for shown
const char * CECPetAutoFood::GetDropModel()
{
	return g_pGame->GetDataPath(m_pDBEssence->file_matter);
}

ACString CECPetAutoFood::_Serialize() const
{
	return ItemMakeString(&cur_value);
}

bool CECPetAutoFood::_Unserialize(const ACHAR* szText)
{
	ItemFromString(szText, &cur_value);
	m_bNeedUpdate = false;
	return true;
}

///////////////////////////////////////////////////////////////////////////
//	
//	Implement CECPetRefine
//	
///////////////////////////////////////////////////////////////////////////

CECPetRefine::CECPetRefine(int tid, int expire_date) : CECIvtrItem(tid, expire_date)
{
	m_iCID	= ICID_PET_REFINE;

	//	Get database data
	elementdataman* pDB = g_pGame->GetElementDataMan();
	DATA_TYPE DataType;
	m_pDBEssence = (PET_REFINE_ESSENCE*)pDB->get_data_ptr(tid, ID_SPACE_ESSENCE, DataType);
	
	m_iPileLimit	= m_pDBEssence->pile_num_max;
	m_iPrice		= m_pDBEssence->price;
	m_iShopPrice	= m_pDBEssence->shop_price;
	m_iProcType		= m_pDBEssence->proc_type;
	m_iEquipMask	= 0;
	m_bUseable		= true;
}

CECPetRefine::CECPetRefine(const CECPetRefine& s) : CECIvtrItem(s)
{
	m_pDBEssence	= s.m_pDBEssence;
}

CECPetRefine::~CECPetRefine()
{
}

//	Set item detail information
bool CECPetRefine::SetItemInfo(BYTE* pInfoData, int iDataLen)
{
	CECIvtrItem::SetItemInfo(pInfoData, iDataLen);
	return true;
}

//	Get item icon file name
const char* CECPetRefine::GetIconFile()
{
	return g_pGame->GetDataPath(m_pDBEssence->file_icon);
}

//	Get item name
const wchar_t* CECPetRefine::GetName()
{
	return m_pDBEssence->name;
}

//	Get item cool time
int CECPetRefine::GetCoolTime(int* piMax/* NULL */)
{

	return 0;
}

//	Get item description text
const wchar_t* CECPetRefine::GetNormalDesc(bool bRepair)
{
	if (m_bNeedUpdate)
		return NULL;

	m_strDesc = _AL("");

	//	Try to build item description
	CECStringTab* pDescTab = g_pGame->GetItemDesc();
	CECStringTab* pFixMsg = g_pGame->GetFixedMsgTab();
	int red = ITEMDESC_COL_RED;
	int white = ITEMDESC_COL_WHITE;
	int name_color = DecideNameCol();

	if (m_iCount > 1)
		AddDescText(name_color, true, pDescTab->GetWideString(ITEMDESC_NAMENUMBER), GetName(), m_iCount);
	else
		AddDescText(name_color, true, pDescTab->GetWideString(ITEMDESC_NAME), GetName());

	AddStateDesc(pDescTab);
	AddExpireTimeDesc();

	//	Price
	AddPriceDesc(white, bRepair);

	//	Extend description
	AddExtDescText();

	return m_strDesc;
}

//	Check item use condition
bool CECPetRefine::CheckUseCondition()
{
	if (!m_pDBEssence)
		return false;
	if (!CheckCrossUseCondition())
		return false;
	return true;
}

//	Get drop model for shown
const char * CECPetRefine::GetDropModel()
{
	return g_pGame->GetDataPath(m_pDBEssence->file_matter);
}

///////////////////////////////////////////////////////////////////////////
//	
//	Implement CECPetAssistRefine
//	
///////////////////////////////////////////////////////////////////////////

CECPetAssistRefine::CECPetAssistRefine(int tid, int expire_date) : CECIvtrItem(tid, expire_date)
{
	m_iCID	= ICID_PET_ASSIST_REFINE;

	//	Get database data
	elementdataman* pDB = g_pGame->GetElementDataMan();
	DATA_TYPE DataType;
	m_pDBEssence = (PET_ASSIST_REFINE_ESSENCE*)pDB->get_data_ptr(tid, ID_SPACE_ESSENCE, DataType);
	
	m_iPileLimit	= m_pDBEssence->pile_num_max;
	m_iPrice		= m_pDBEssence->price;
	m_iShopPrice	= m_pDBEssence->shop_price;
	m_iProcType		= m_pDBEssence->proc_type;
	m_iEquipMask	= 0;
	m_bUseable		= true;
}

CECPetAssistRefine::CECPetAssistRefine(const CECPetAssistRefine& s) : CECIvtrItem(s)
{
	m_pDBEssence	= s.m_pDBEssence;
}

CECPetAssistRefine::~CECPetAssistRefine()
{
}

//	Set item detail information
bool CECPetAssistRefine::SetItemInfo(BYTE* pInfoData, int iDataLen)
{
	CECIvtrItem::SetItemInfo(pInfoData, iDataLen);
	return true;
}

//	Get item icon file name
const char* CECPetAssistRefine::GetIconFile()
{
	return g_pGame->GetDataPath(m_pDBEssence->file_icon);
}

//	Get item name
const wchar_t* CECPetAssistRefine::GetName()
{
	return m_pDBEssence->name;
}

//	Get item cool time
int CECPetAssistRefine::GetCoolTime(int* piMax/* NULL */)
{

	return 0;
}

//	Get item description text
const wchar_t* CECPetAssistRefine::GetNormalDesc(bool bRepair)
{
	if (m_bNeedUpdate)
		return NULL;

	m_strDesc = _AL("");

	//	Try to build item description
	CECStringTab* pDescTab = g_pGame->GetItemDesc();
	CECStringTab* pFixMsg = g_pGame->GetFixedMsgTab();
	int red = ITEMDESC_COL_RED;
	int white = ITEMDESC_COL_WHITE;
	int name_color = DecideNameCol();

	if (m_iCount > 1)
		AddDescText(name_color, true, pDescTab->GetWideString(ITEMDESC_NAMENUMBER), GetName(), m_iCount);
	else
		AddDescText(name_color, true, pDescTab->GetWideString(ITEMDESC_NAME), GetName());

	AddStateDesc(pDescTab);
	AddExpireTimeDesc();

	//	Price
	AddPriceDesc(white, bRepair);

	//	Extend description
	AddExtDescText();

	return m_strDesc;
}

//	Check item use condition
bool CECPetAssistRefine::CheckUseCondition()
{
	if (!m_pDBEssence)
		return false;
	if (!CheckCrossUseCondition())
		return false;
	return true;
}

//	Get drop model for shown
const char * CECPetAssistRefine::GetDropModel()
{
	return g_pGame->GetDataPath(m_pDBEssence->file_matter);
}

///////////////////////////////////////////////////////////////////////////
//	
//	Implement CECPetSkill
//	
///////////////////////////////////////////////////////////////////////////

CECPetSkill::CECPetSkill(int tid, int expire_date) : CECIvtrItem(tid, expire_date)
{
	m_iCID	= ICID_PET_SKILL;

	//	Get database data
	elementdataman* pDB = g_pGame->GetElementDataMan();
	DATA_TYPE DataType;
	m_pDBEssence = (PET_SKILL_ESSENCE*)pDB->get_data_ptr(tid, ID_SPACE_ESSENCE, DataType);
	
	m_iPileLimit	= m_pDBEssence->pile_num_max;
	m_iPrice		= m_pDBEssence->price;
	m_iShopPrice	= m_pDBEssence->shop_price;
	m_iProcType		= m_pDBEssence->proc_type;
	m_iEquipMask	= 0;
	m_bUseable		= true;
}

CECPetSkill::CECPetSkill(const CECPetSkill& s) : CECIvtrItem(s)
{
	m_pDBEssence	= s.m_pDBEssence;
}

CECPetSkill::~CECPetSkill()
{
}

//	Set item detail information
bool CECPetSkill::SetItemInfo(BYTE* pInfoData, int iDataLen)
{
	CECIvtrItem::SetItemInfo(pInfoData, iDataLen);
	return true;
}

//	Get item icon file name
const char* CECPetSkill::GetIconFile()
{
	return g_pGame->GetDataPath(m_pDBEssence->file_icon);
}

//	Get item name
const wchar_t* CECPetSkill::GetName()
{
	return m_pDBEssence->name;
}

//	Get item cool time
int CECPetSkill::GetCoolTime(int* piMax/* NULL */)
{
	/*
	CECHostPlayer* pHost = g_pGame->GetGameRun()->GetHostPlayer();
	return pHost ? pHost->GetCoolTime(GP_CT_AIHELPER, piMax) : 0;
	*/
	
	return 0;
}

//	Get item description text
const wchar_t* CECPetSkill::GetNormalDesc(bool bRepair)
{
	if (m_bNeedUpdate)
		return NULL;

	m_strDesc = _AL("");

	//	Try to build item description
	CECStringTab* pDescTab = g_pGame->GetItemDesc();
	CECStringTab* pFixMsg = g_pGame->GetFixedMsgTab();
	int red = ITEMDESC_COL_RED;
	int white = ITEMDESC_COL_WHITE;
	int name_color = DecideNameCol();

	if (m_iCount > 1)
		AddDescText(name_color, true, pDescTab->GetWideString(ITEMDESC_NAMENUMBER), GetName(), m_iCount);
	else
		AddDescText(name_color, true, pDescTab->GetWideString(ITEMDESC_NAME), GetName());

	AddStateDesc(pDescTab);
	AddExpireTimeDesc();

	//	Price
	AddPriceDesc(white, bRepair);

	//	Extend description
	AddExtDescText();

	return m_strDesc;
}

//	Check item use condition
bool CECPetSkill::CheckUseCondition()
{
	if (!m_pDBEssence)
		return false;
	if (!CheckCrossUseCondition())
		return false;
	return true;
}

//	Get drop model for shown
const char * CECPetSkill::GetDropModel()
{
	return g_pGame->GetDataPath(m_pDBEssence->file_matter);
}

///////////////////////////////////////////////////////////////////////////
//	
//	Implement CECPetArmor
//	
///////////////////////////////////////////////////////////////////////////

CECPetArmor::CECPetArmor(int tid, int expire_date) : CECIvtrItem(tid, expire_date)
{
	m_iCID	= ICID_PET_ARMOR;

	memset(&m_Essence, 0, sizeof (m_Essence));

	//	Get database data
	elementdataman* pDB = g_pGame->GetElementDataMan();
	DATA_TYPE DataType;
	m_pDBEssence = (PET_ARMOR_ESSENCE*)pDB->get_data_ptr(tid, ID_SPACE_ESSENCE, DataType);
	
	m_iPileLimit	= m_pDBEssence->pile_num_max;
	m_iPrice		= m_pDBEssence->price;
	m_iShopPrice	= m_pDBEssence->shop_price;
	m_iProcType		= m_pDBEssence->proc_type;
	m_iEquipMask	= 0;
	m_bUseable		= false;
}

CECPetArmor::CECPetArmor(const CECPetArmor& s) : CECIvtrItem(s)
{
	m_pDBEssence	= s.m_pDBEssence;
	m_Essence		= s.m_Essence;
}

CECPetArmor::~CECPetArmor()
{
}

//	Set item detail information
bool CECPetArmor::SetItemInfo(BYTE* pInfoData, int iDataLen)
{
	if (iDataLen != sizeof(itemdataman::_pet_armor_essence))
		memset(&m_Essence, 0, sizeof(m_Essence));
	else
		memcpy(&m_Essence, pInfoData, sizeof(m_Essence));
	
	CECIvtrItem::SetItemInfo(pInfoData, iDataLen);
	return true;
}

//	Get item icon file name
const char* CECPetArmor::GetIconFile()
{
	return g_pGame->GetDataPath(m_pDBEssence->file_icon);
}

//	Get item name
const wchar_t* CECPetArmor::GetName()
{
	return m_pDBEssence->name;
}

//	Get item cool time
int CECPetArmor::GetCoolTime(int* piMax/* NULL */)
{
	/*
	CECHostPlayer* pHost = g_pGame->GetGameRun()->GetHostPlayer();
	return pHost ? pHost->GetCoolTime(GP_CT_CAMRECORDER, piMax) : 0;
	*/
	return 0;
}

//	Get item description text
const wchar_t* CECPetArmor::GetNormalDesc(bool bRepair)
{
	if (m_bNeedUpdate)
		return NULL;

	m_strDesc = _AL("");

	//	Try to build item description
	CECStringTab* pDescTab = g_pGame->GetItemDesc();
	CECStringTab* pFixMsg = g_pGame->GetFixedMsgTab();
	int red = ITEMDESC_COL_RED;
	int white = ITEMDESC_COL_WHITE;
	int name_color = DecideNameCol();

	if (m_iCount > 1)
		AddDescText(name_color, true, pDescTab->GetWideString(ITEMDESC_NAMENUMBER), GetName(), m_iCount);
	else
		AddDescText(name_color, true, pDescTab->GetWideString(ITEMDESC_NAME), GetName());

	AddDescText(white, true, _AL("%s"), pDescTab->GetWideString(ITEMDESC_PET_ARMOR));
	
	AddStateDesc(pDescTab);

	if( m_pDBEssence->desc[0] )
		AddDescText(white, true, _AL("%s"), m_pDBEssence->desc);

	AddExpireTimeDesc();

	if (m_pDBEssence->pet_type_mask)
	{
		static const unsigned int uAllSect = (1 << 16) - 1;

		if (m_pDBEssence->pet_type_mask != uAllSect)
		{
			AddDescText(ITEMDESC_COL_WHITE, false, pDescTab->GetWideString(ITEMDESC_PET_RACE_REQUIRE));

			for (int i = 0; i < 16; i++)
			{
				if (m_pDBEssence->pet_type_mask & (1 << i))
				{
					m_strDesc += _AL(" ");
					AddDescText(ITEMDESC_COL_WHITE, false, pFixMsg->GetWideString(FIXMSG_PET_RACE1 + i));
				}
			}

			m_strDesc += _AL("\\r");
		}
	}

	AddDescText(ITEMDESC_COL_WHITE, true, pDescTab->GetWideString(ITEMDESC_LEVELREQ), m_pDBEssence->lev_required);

	if (m_pDBEssence->astro_type)
	{
		static const unsigned int uAllSect = (1 << 9) - 1;

		if (m_pDBEssence->astro_type != uAllSect)
		{
			AddDescText(ITEMDESC_COL_WHITE, false, pDescTab->GetWideString(ITEMDESC_PET_ASTRO_REQUIRE));

			for (int i = 0; i < 9; i++)
			{
				if (m_pDBEssence->astro_type & (1 << i))
				{
					m_strDesc += _AL(" ");
					AddDescText(ITEMDESC_COL_WHITE, false, pFixMsg->GetWideString(FIXMSG_PET_ASTRO1 + i));
				}
			}

			m_strDesc += _AL("\\r");
		}
	}

	AddDescText(ITEMDESC_COL_WHITE, true, pDescTab->GetWideString(ITEMDESC_PET_ASTRO_LEV_REQUIRE), m_pDBEssence->astro_level);
	AddDescText(ITEMDESC_COL_WHITE, true, pDescTab->GetWideString(ITEMDESC_PET_EQUIP_LOC), pDescTab->GetWideString(ITEMDESC_PET_EQUIP_LOC_1 + m_pDBEssence->equip_location));
	int blue = ITEMDESC_COL_LIGHTBLUE;

	if( m_Essence.maxhp )
		AddDescText(blue, true, pDescTab->GetWideString(ITEMDESC_EQUIPADDON_1), m_Essence.maxhp);
	if( m_Essence.maxmp )
		AddDescText(blue, true, pDescTab->GetWideString(ITEMDESC_EQUIPADDON_2), m_Essence.maxmp);
	if( m_Essence.attack)
		AddDescText(blue, true, pDescTab->GetWideString(ITEMDESC_EQUIPADDON_3), m_Essence.attack);
	if( m_Essence.defence)
		AddDescText(blue, true, pDescTab->GetWideString(ITEMDESC_EQUIPADDON_4), m_Essence.defence);
	if( m_Essence.hit)
		AddDescText(blue, true, pDescTab->GetWideString(ITEMDESC_EQUIPADDON_5), m_Essence.hit);
	if( m_Essence.jouk)
		AddDescText(blue, true, pDescTab->GetWideString(ITEMDESC_EQUIPADDON_6), m_Essence.jouk);
	if( m_Essence.resistance[0] )
		AddDescText(blue, true, pDescTab->GetWideString(ITEMDESC_EQUIPADDON_9), m_Essence.resistance[0]);
	if( m_Essence.resistance[1] )
		AddDescText(blue, true, pDescTab->GetWideString(ITEMDESC_EQUIPADDON_10), m_Essence.resistance[1]);
	if( m_Essence.resistance[2] )
		AddDescText(blue, true, pDescTab->GetWideString(ITEMDESC_EQUIPADDON_11), m_Essence.resistance[2]);
	if( m_Essence.resistance[3] )
		AddDescText(blue, true, pDescTab->GetWideString(ITEMDESC_EQUIPADDON_12), m_Essence.resistance[3]);
	if( m_Essence.resistance[4] )
		AddDescText(blue, true, pDescTab->GetWideString(ITEMDESC_EQUIPADDON_13), m_Essence.resistance[4]);
	if( m_Essence.resistance[5] )
		AddDescText(blue, true, pDescTab->GetWideString(ITEMDESC_EQUIPADDON_14), m_Essence.resistance[5]);
	if( m_Essence.crit_rate > 0.0001)
		AddDescText(blue, true, pDescTab->GetWideString(ITEMDESC_EQUIPADDON_7), m_Essence.crit_rate * 100.0f);
	if( m_Essence.crit_damage > 0.0001 )
		AddDescText(blue, true, pDescTab->GetWideString(ITEMDESC_EQUIPADDON_8), m_Essence.crit_damage * 100.0);

	//	Price
	AddPriceDesc(white, bRepair);

	//	Extend description
	AddExtDescText();

	return m_strDesc;
}

//	Check item use condition
bool CECPetArmor::CheckUseCondition()
{
	if (!m_pDBEssence)
		return false;
	if (!CheckCrossUseCondition())
		return false;
	return true;
}

//	Get drop model for shown
const char * CECPetArmor::GetDropModel()
{
	return g_pGame->GetDataPath(m_pDBEssence->file_matter);
}

ACString CECPetArmor::_Serialize() const
{
	return ItemMakeString(&m_Essence);
}

bool CECPetArmor::_Unserialize(const ACHAR* szText)
{
	ItemFromString(szText, &m_Essence);
	m_bNeedUpdate = false;
	return true;
}

///////////////////////////////////////////////////////////////////////////
//	
//	Implement CECIvtrBook
//	
///////////////////////////////////////////////////////////////////////////

CECIvtrBook::CECIvtrBook(int tid, int expire_date) : CECIvtrItem(tid, expire_date)
{
	m_iCID	= ICID_BOOK;

	//	Get database data
	elementdataman* pDB = g_pGame->GetElementDataMan();
	DATA_TYPE DataType;
	m_pDBEssence = (BOOK_ESSENCE*)pDB->get_data_ptr(tid, ID_SPACE_ESSENCE, DataType);
	
	m_iPileLimit	= m_pDBEssence->pile_num_max;
	m_iPrice		= m_pDBEssence->price;
	m_iShopPrice	= m_pDBEssence->shop_price;
	m_iProcType		= m_pDBEssence->proc_type;
	m_iEquipMask	= 0;
	m_bUseable		= false;
}

CECIvtrBook::CECIvtrBook(const CECIvtrBook& s) : CECIvtrItem(s)
{
	m_pDBEssence	= s.m_pDBEssence;
}

CECIvtrBook::~CECIvtrBook()
{
}

//	Set item detail information
bool CECIvtrBook::SetItemInfo(BYTE* pInfoData, int iDataLen)
{
	return CECIvtrItem::SetItemInfo(pInfoData, iDataLen);
}

//	Get item icon file name
const char* CECIvtrBook::GetIconFile()
{
	return g_pGame->GetDataPath(m_pDBEssence->file_icon);
}

//	Get item name
const wchar_t* CECIvtrBook::GetName()
{
	return m_pDBEssence->name;
}

//	Get item cool time
int CECIvtrBook::GetCoolTime(int* piMax/* NULL */)
{
	return 0;
}

//	Get item description text
const wchar_t* CECIvtrBook::GetNormalDesc(bool bRepair)
{
	if (m_bNeedUpdate)
		return NULL;

	m_strDesc = _AL("");

	//	Try to build item description
	CECStringTab* pDescTab = g_pGame->GetItemDesc();
	CECStringTab* pFixMsg = g_pGame->GetFixedMsgTab();
	int red = ITEMDESC_COL_RED;
	int white = ITEMDESC_COL_WHITE;
	int name_color = DecideNameCol();

	if (m_iCount > 1)
		AddDescText(name_color, true, pDescTab->GetWideString(ITEMDESC_NAMENUMBER), GetName(), m_iCount);
	else
		AddDescText(name_color, true, pDescTab->GetWideString(ITEMDESC_NAME), GetName());

	AddStateDesc(pDescTab);
	AddExpireTimeDesc();

	//	Price
	AddPriceDesc(white, bRepair);

	//	Extend description
	AddExtDescText();

	return m_strDesc;
}

//	Check item use condition
bool CECIvtrBook::CheckUseCondition()
{
	if (!m_pDBEssence)
		return false;
	if (!CheckCrossUseCondition())
		return false;
	return true;
}

//	Get drop model for shown
const char * CECIvtrBook::GetDropModel()
{
	return g_pGame->GetDataPath(m_pDBEssence->file_matter);
}

///////////////////////////////////////////////////////////////////////////
//	
//	Implement CECIvtrEquipSoul
//	
///////////////////////////////////////////////////////////////////////////

CECIvtrEquipSoul::CECIvtrEquipSoul(int tid, int expire_date) : CECIvtrItem(tid, expire_date)
{
	m_iCID	= ICID_EQUIP_SOUL;

	//	Get database data
	elementdataman* pDB = g_pGame->GetElementDataMan();
	DATA_TYPE DataType;
	m_pDBEssence = (EQUIP_SOUL_ESSENCE*)pDB->get_data_ptr(tid, ID_SPACE_ESSENCE, DataType);
	
	m_iPileLimit	= m_pDBEssence->pile_num_max;
	m_iPrice		= m_pDBEssence->price;
	m_iShopPrice	= m_pDBEssence->shop_price;
	m_iProcType		= m_pDBEssence->proc_type;
	m_iEquipMask	= 0;
	m_bUseable		= false;
}

CECIvtrEquipSoul::CECIvtrEquipSoul(const CECIvtrEquipSoul& s) : CECIvtrItem(s)
{
	m_pDBEssence	= s.m_pDBEssence;
}

CECIvtrEquipSoul::~CECIvtrEquipSoul()
{
}

//	Set item detail information
bool CECIvtrEquipSoul::SetItemInfo(BYTE* pInfoData, int iDataLen)
{
	return CECIvtrItem::SetItemInfo(pInfoData, iDataLen);
}

//	Get item icon file name
const char* CECIvtrEquipSoul::GetIconFile()
{
	return g_pGame->GetDataPath(m_pDBEssence->file_icon);
}

//	Get item name
const wchar_t* CECIvtrEquipSoul::GetName()
{
	return m_pDBEssence->name;
}

//	Get item cool time
int CECIvtrEquipSoul::GetCoolTime(int* piMax/* NULL */)
{
	return 0;
}

//	Get item description text
const wchar_t* CECIvtrEquipSoul::GetNormalDesc(bool bRepair)
{
	if (m_bNeedUpdate)
		return NULL;

	m_strDesc = _AL("");

	//	Try to build item description
	CECStringTab* pDescTab = g_pGame->GetItemDesc();
	CECStringTab* pFixMsg = g_pGame->GetFixedMsgTab();
	int red = ITEMDESC_COL_RED;
	int white = ITEMDESC_COL_WHITE;
	int name_color = DecideNameCol();

	AddDescText(name_color, false, pDescTab->GetWideString(ITEMDESC_EQUIP_SOUL));

	if (m_iCount > 1)
		AddDescText(name_color, true, pDescTab->GetWideString(ITEMDESC_NAMENUMBER), GetName(), m_iCount);
	else
		AddDescText(name_color, true, pDescTab->GetWideString(ITEMDESC_NAME), GetName());

	AddStateDesc(pDescTab);

	DATA_TYPE dt;
	EQUIPMENT_ADDON* pAddOn = (EQUIPMENT_ADDON*) g_pGame->GetElementDataMan()->get_data_ptr(m_pDBEssence->id_addon1, ID_SPACE_ADDON, dt);

	if (dt == DT_EQUIPMENT_ADDON)
	{
		AddDescText(ITEMDESC_COL_LIGHTBLUE, true, _AL("%s%s"), pDescTab->GetWideString(ITEMDESC_EQUIP_SOUL_EFFECT), CECIvtrEquip::GetEquipAddonDesc(pAddOn->id, pAddOn->type, pAddOn->param1, pAddOn->param2, pAddOn->param3, 1));
	}

	AddDescText(ITEMDESC_COL_LIGHTBLUE, true, pDescTab->GetWideString(ITEMDESC_EQUIP_SOUL_COST), m_pDBEssence->cost_soul);

	BuildRaceReqDesc();

	AddDescText(white, false, pDescTab->GetWideString(ITEMDESC_EQUIP_SOUL_FACTION_LIMIT));

	static const unsigned int uAllSect = (1<<NUM_SECT) -1;
	static const unsigned int uHumanSect = (1 << NUM_HUMAN_SECT) - 1;
	static const unsigned int uOrcSect = uAllSect - uHumanSect;
	if (m_pDBEssence->sect_mask == uAllSect || m_pDBEssence->sect_mask == uHumanSect || m_pDBEssence->sect_mask == uOrcSect)
	{
		AddDescText(white, true, pDescTab->GetWideString(ITEMDESC_ALL));
	}
	else
	{
		for(int i = 0; i< NUM_SECT; ++i)
		{
			if(m_pDBEssence->sect_mask & (1<<i))
			{
				m_strDesc += _AL(" ");
				AddDescText(white, false, pFixMsg->GetWideString(FIXMSG_SECT_1+i));
			}
		}
		m_strDesc += _AL("\\r");
	}

	AddDescText(white, true, pDescTab->GetWideString(ITEMDESC_EQUIP_SOUL_LEVEL_LIMIT), m_pDBEssence->level_required);

	AddDescText(white, false, pDescTab->GetWideString(ITEMDESC_EQUIP_SOUL_LOCATION_LIMIT));

	//可镶嵌部位，目前限定在武器、帽子、衣服、鞋子上面，如果扩展，这里也需要修改，并且需要将部位名字抽取出来放到itemdesc.txt中
	
	if (m_pDBEssence->equip_mask & EQUIP_MASK_WEAPON)
		AddDescText(white, false, pDescTab->GetWideString(ITEMDESC_WEAPON));
	if (m_pDBEssence->equip_mask & EQUIP_MASK_HEAD)
		AddDescText(white, false, pDescTab->GetWideString(ITEMDESC_HAT));
	if (m_pDBEssence->equip_mask & EQUIP_MASK_BODY)
		AddDescText(white, false, pDescTab->GetWideString(ITEMDESC_CLOTHES));
	if (m_pDBEssence->equip_mask & EQUIP_MASK_FOOT)
		AddDescText(white, false, pDescTab->GetWideString(ITEMDESC_SHOE));

	m_strDesc += _AL("\\r");

	AddDescText(white, false, pDescTab->GetWideString(ITEMDESC_EQUIP_SOUL_HOLE_LIMIT));

	switch(m_pDBEssence->hole_type)
	{
	case 0:
		AddDescText(white, true, pDescTab->GetWideString(ITEMDESC_ALL));
		break;
	case 1:
		AddDescText(ITEMDESC_COL_DARKGOLD, true, pDescTab->GetWideString(ITEMDESC_GOLD));
		break;
	case 2:
		AddDescText(ITEMDESC_COL_GREEN, true, pDescTab->GetWideString(ITEMDESC_WOOD));
		break;	
	case 3:
		AddDescText(ITEMDESC_COL_LIGHTBLUE, true, pDescTab->GetWideString(ITEMDESC_WATER));
		break;
	case 4:
		AddDescText(ITEMDESC_COL_RED, true, pDescTab->GetWideString(ITEMDESC_FIRE));
		break;
	case 5:
		AddDescText(ITEMDESC_COL_YELLOW, true, pDescTab->GetWideString(ITEMDESC_EARTH));
		break;
	}
	AddExpireTimeDesc();
	//	Price
	AddPriceDesc(white, bRepair);

	AddExtDescText();
	return m_strDesc;
}

void CECIvtrEquipSoul::BuildRaceReqDesc()
{
	if(m_pDBEssence->require_race == 2)
		return;

	CECStringTab* pDescTab = g_pGame->GetItemDesc();
	CECHostPlayer* pHost = g_pGame->GetGameRun()->GetHostPlayer();

	int col;

	CECStringTab* pFixMsg = g_pGame->GetFixedMsgTab();
	if( (pHost->GetRace() == RACE_HUMAN && m_pDBEssence->require_race == 0) ||
		( pHost->GetRace() == RACE_ORC && m_pDBEssence->require_race ==1) )
		col = ITEMDESC_COL_WHITE;
	else
		col = ITEMDESC_COL_RED;

	AddDescText(col, false, pDescTab->GetWideString(ITEMDESC_RACEREQ));
	m_strDesc += _AL(" ");
	if(m_pDBEssence->require_race ==0)
		AddDescText(col, true, pFixMsg->GetWideString(FIXMSG_RACE_HUMAN));
	else if(m_pDBEssence->require_race == 1)
		AddDescText(col, true, pFixMsg->GetWideString(FIXMSG_RACE_ORC));
	else if(m_pDBEssence->require_race == 3)
		AddDescText(col, true, pFixMsg->GetWideString(FIXMSG_RACE_MONSTER));
}

//	Check item use condition
bool CECIvtrEquipSoul::CheckUseCondition()
{
	if (!m_pDBEssence)
		return false;
	if (!CheckCrossUseCondition())
		return false;
	return true;
}

//	Get drop model for shown
const char * CECIvtrEquipSoul::GetDropModel()
{
	return g_pGame->GetDataPath(m_pDBEssence->file_matter);
}

///////////////////////////////////////////////////////////////////////////
//	
//	Implement CECIvtrSpecialNameItem
//	
///////////////////////////////////////////////////////////////////////////

CECIvtrSpecialNameItem::CECIvtrSpecialNameItem(int tid, int expire_date) : CECIvtrItem(tid, expire_date)
{
	m_iCID	= ICID_SPECIAL_NAME_ITEM;

	//	Get database data
	elementdataman* pDB = g_pGame->GetElementDataMan();
	DATA_TYPE DataType;
	m_pDBEssence = (SPECIAL_NAME_ITEM_ESSENCE*)pDB->get_data_ptr(tid, ID_SPACE_ESSENCE, DataType);
	
	m_iPileLimit	= m_pDBEssence->pile_num_max;
	m_iPrice		= m_pDBEssence->price;
	m_iShopPrice	= m_pDBEssence->shop_price;
	m_iProcType		= m_pDBEssence->proc_type;
	m_iEquipMask	= 0;
	m_bUseable		= false;
}

CECIvtrSpecialNameItem::CECIvtrSpecialNameItem(const CECIvtrSpecialNameItem& s) : CECIvtrItem(s)
{
	m_pDBEssence	= s.m_pDBEssence;
	m_Essence		= s.m_Essence;
}

CECIvtrSpecialNameItem::~CECIvtrSpecialNameItem()
{
}

//	Set item detail information
bool CECIvtrSpecialNameItem::SetItemInfo(BYTE* pInfoData, int iDataLen)
{
	if( iDataLen != sizeof(int) )
		return false;
	m_Essence.owner_id = *(int *)pInfoData;
	return CECIvtrItem::SetItemInfo(pInfoData, iDataLen);
}

//	Get item icon file name
const char* CECIvtrSpecialNameItem::GetIconFile()
{
	return g_pGame->GetDataPath(m_pDBEssence->file_icon);
}

//	Get item name
const wchar_t* CECIvtrSpecialNameItem::GetName()
{
	const wchar_t * szOwnerName = g_pGame->GetGameRun()->GetPlayerName(m_Essence.owner_id, false);
	if( m_Essence.owner_id >0 && !szOwnerName )
	{
		static DWORD timeLast = 0;
		if( timeLast + 1000 < timeGetTime() )
		{
			g_pGame->GetGameSession()->GetPlayerBriefInfo(1, &(m_Essence.owner_id), 2);
			timeLast = timeGetTime();
		}
	}
	else if(szOwnerName)
	{
		CECStringTab* pDescTab = g_pGame->GetItemDesc();
		a_sprintf(m_strSpecialName, pDescTab->GetWideString(ITEMDESC_STRCAT), szOwnerName, m_pDBEssence->name);
		return m_strSpecialName;
	}
	return m_pDBEssence->name;
}

//	Get item cool time
int CECIvtrSpecialNameItem::GetCoolTime(int* piMax/* NULL */)
{
	return 0;
}

//	Get item description text
const wchar_t* CECIvtrSpecialNameItem::GetNormalDesc(bool bRepair)
{
	if (m_bNeedUpdate)
		return NULL;

	m_strDesc = _AL("");

	//	Try to build item description
	CECStringTab* pDescTab = g_pGame->GetItemDesc();
	CECStringTab* pFixMsg = g_pGame->GetFixedMsgTab();
	int red = ITEMDESC_COL_RED;
	int white = ITEMDESC_COL_WHITE;
	int name_color = DecideNameCol();

	if (m_iCount > 1)
		AddDescText(name_color, true, pDescTab->GetWideString(ITEMDESC_NAMENUMBER), GetName(), m_iCount);
	else
		AddDescText(name_color, true, pDescTab->GetWideString(ITEMDESC_NAME), GetName());

	AddStateDesc(pDescTab);

	//	Price
	AddPriceDesc(white, bRepair);

	AddExtDescText();
	return m_strDesc;
}

//	Check item use condition
bool CECIvtrSpecialNameItem::CheckUseCondition()
{
	if (!m_pDBEssence)
		return false;
	
	if (!CheckCrossUseCondition())
		return false;

	return true;
}

//	Get drop model for shown
const char * CECIvtrSpecialNameItem::GetDropModel()
{
	return g_pGame->GetDataPath(m_pDBEssence->file_matter);
}


///////////////////////////////////////////////////////////////////////////
//	
//	Implement CECIvtrGiftBag
//	
///////////////////////////////////////////////////////////////////////////

CECIvtrGiftBag::CECIvtrGiftBag(int tid, int expire_date) : CECIvtrItem(tid, expire_date)
{
	m_iCID	= ICID_GIFT_BAG;

	//	Get database data
	elementdataman* pDB = g_pGame->GetElementDataMan();
	DATA_TYPE DataType;
	m_pDBEssence = (GIFT_BAG_ESSENCE*)pDB->get_data_ptr(tid, ID_SPACE_ESSENCE, DataType);
	
	m_iPileLimit	= m_pDBEssence->pile_num_max;
	m_iPrice		= m_pDBEssence->price;
	m_iShopPrice	= m_pDBEssence->shop_price;
//	m_iLevelReq		= m_pDBEssence->require_level;
//	m_iProfReq		= m_pDBEssence->character_combo_id;
	m_iGenderReq	= m_pDBEssence->require_gender;
	m_iProcType		= m_pDBEssence->proc_type;
	m_iEquipMask	= 0;
	m_bUseable		= false;	// why false? 因为如果不是false，打开包裹如果不能使用会有不断的提示
	m_bNeedUpdate	= false;
}

CECIvtrGiftBag::CECIvtrGiftBag(const CECIvtrGiftBag& s) : CECIvtrItem(s)
{
	m_pDBEssence	= s.m_pDBEssence;
	m_iGenderReq	= s.m_iGenderReq;
}

CECIvtrGiftBag::~CECIvtrGiftBag()
{
}

//	Set item detail information
bool CECIvtrGiftBag::SetItemInfo(BYTE* pInfoData, int iDataLen)
{
	return CECIvtrItem::SetItemInfo(pInfoData, iDataLen);
}

//	Get item icon file name
const char* CECIvtrGiftBag::GetIconFile()
{
	return g_pGame->GetDataPath(m_pDBEssence->file_icon);
}

//	Get item name
const wchar_t* CECIvtrGiftBag::GetName()
{
	return m_pDBEssence->name;
}

//	Get item cool time
int CECIvtrGiftBag::GetCoolTime(int* piMax/* NULL */)
{
	return 0;
}

//	Get item description text
const wchar_t* CECIvtrGiftBag::GetNormalDesc(bool bRepair)
{
	m_strDesc = _AL("");

	//	Try to build item description
	CECStringTab* pDescTab = g_pGame->GetItemDesc();
	CECStringTab* pFixMsg = g_pGame->GetFixedMsgTab();
	int red = ITEMDESC_COL_RED;
	int white = ITEMDESC_COL_WHITE;
	int name_color = DecideNameCol();
	CECHostPlayer* pHost = g_pGame->GetGameRun()->GetHostPlayer();
	if (m_iCount > 1)
		AddDescText(name_color, true, pDescTab->GetWideString(ITEMDESC_NAMENUMBER), GetName(), m_iCount);
	else
		AddDescText(name_color, true, pDescTab->GetWideString(ITEMDESC_NAME), GetName());
	
	AddStateDesc(pDescTab);

	//	Level requirement
	int iHostLevel = pHost->GetBasicProps().iLevel;	
	int iEverMaxLevel = iHostLevel;

	for (int i = 0; i< pHost->GetRebornCount(); ++i)
	{
		if (iEverMaxLevel < pHost->GetRebornInfo()[i].level)
			iEverMaxLevel = pHost->GetRebornInfo()[i].level;
	}
	if (m_pDBEssence->level_everLimit)
	{
		int col = red;
		if (iEverMaxLevel >= m_pDBEssence->level_everLimit)
			col = white;
		AddDescText(col,true, pDescTab->GetWideString(ITEMDESC_EVER_LEVELREQ), m_pDBEssence->level_everLimit);
	}

	if(m_pDBEssence->level_required > 0 && m_pDBEssence->level_upperLimit > 0)
	{
		int col = red;
		if(iHostLevel >= m_pDBEssence->level_required && iHostLevel <= m_pDBEssence->level_upperLimit)
			col = white;
		AddDescText(col, true, pDescTab->GetWideString(ITEMDESC_LEVELUPPERLIMIT), m_pDBEssence->level_required, m_pDBEssence->level_upperLimit);
	}
	else if(m_pDBEssence->level_required == 0 && m_pDBEssence->level_upperLimit > 0 )
	{
		int col = red;
		if(iHostLevel >= m_pDBEssence->level_required && iHostLevel <= m_pDBEssence->level_upperLimit)
			col = white;
		AddDescText(col, true, pDescTab->GetWideString(ITEMDESC_LEVELUPPERLIMIT), 1, m_pDBEssence->level_upperLimit);
	}
	else if(m_pDBEssence->level_required >0 && m_pDBEssence->level_upperLimit == 0 )
	{
		int col = red;
		if(iHostLevel >= m_pDBEssence->level_required)
			col = white;
		AddDescText(col, true, pDescTab->GetWideString(ITEMDESC_LEVELREQ), m_pDBEssence->level_required);
 	}
	
	// gender requirment
	BuildGenderReqDesc();

	if (m_pDBEssence->renascence_count > 0)
	{
		int col = (pHost->GetRebornCount() >= m_pDBEssence->renascence_count) ? ITEMDESC_COL_WHITE : ITEMDESC_COL_RED;
		AddDescText(col, true, pDescTab->GetWideString(ITEMDESC_RENASCENCE_REQUIRED), pDescTab->GetWideString(ITEMDESC_RENASCENCE_1+m_pDBEssence->renascence_count-1));
	}
	
	if (m_pDBEssence->money_need)
	{
		ACHAR szPrice[256];

		GetPriceText(szPrice, m_pDBEssence->money_need);
		AddDescText(white, true, pDescTab->GetWideString(ITEMDESC_GIFT_BAG_NEED_MONEY), szPrice);
	}

	AddDescText(white, true, pDescTab->GetWideString(ITEMDESC_GIFT_BAG_NEED_SPACE), m_pDBEssence->num_object);

	AddExpireTimeDesc();	
	//	Price
	AddPriceDesc(white, bRepair);

	AddExtDescText();
	return m_strDesc;
}

//	Check item use condition
bool CECIvtrGiftBag::CheckUseCondition()
{
	if (!m_pDBEssence)
		return false;
	CECHostPlayer* pHost = g_pGame->GetGameRun()->GetHostPlayer();
	if(!pHost)
		return false;
	CECStringTab* pDescTab = g_pGame->GetItemDesc();
	//	Check level requirement
	int iHostLevel = pHost->GetBasicProps().iLevel;
	int iEverMaxLevel = iHostLevel;

	for (int i = 0; i< pHost->GetRebornCount(); ++i)
	{
		if (iEverMaxLevel < pHost->GetRebornInfo()[i].level)
			iEverMaxLevel = pHost->GetRebornInfo()[i].level;
	}
	if (m_pDBEssence->level_everLimit && m_pDBEssence->level_everLimit > iEverMaxLevel)
	{
		g_pGame->GetGameRun()->AddFixedMessage(FIXMSG_LEVELTOOLOW);
		return false;		
	}
	if (m_pDBEssence->level_required && pHost->GetBasicProps().iLevel < m_pDBEssence->level_required)
	{
		g_pGame->GetGameRun()->AddFixedMessage(FIXMSG_LEVELTOOLOW);
		return false;
	}
	if (m_pDBEssence->level_upperLimit && pHost->GetBasicProps().iLevel > m_pDBEssence->level_upperLimit)
	{
		g_pGame->GetGameRun()->AddFixedMessage(FIXMSG_LEVELTOOHIGH);
		return false;
 	}
	if (m_iGenderReq != 2 && pHost->GetGender() != m_iGenderReq)
	{
		g_pGame->GetGameRun()->AddFixedMessage(FIXMSG_GIFT_BAG_GENDER_LIMIT, m_iGenderReq==0? pDescTab->GetWideString(ITEMDESC_MALE):pDescTab->GetWideString(ITEMDESC_FEMALE));
		return false;
	}
	if (pHost->GetRebornCount() < m_pDBEssence->renascence_count)
	{
		g_pGame->GetGameRun()->AddFixedMessage(FIXMSG_GIFT_BAG_REBORN_LIMIT);
		return false;
	}
	if (((pHost->GetProfession() < 64)&&((m_pDBEssence->character_combo_id & (UINT64(1)<< pHost->GetProfession())) == 0))  ||
		((pHost->GetProfession() >= 64)&&((m_pDBEssence->character_combo_id2 & (UINT64(1)<< (pHost->GetProfession() - 64))) == 0)) ) // Modified 2011-07-18.
	{
		g_pGame->GetGameRun()->AddFixedMessage(FIXMSG_GIFT_BAG_PREF_LIMIT);
		return false;
	}
	if ( m_pDBEssence->god_devil_mask !=0 && (pHost->GetCultivation() & m_pDBEssence->god_devil_mask) == 0)
	{
		g_pGame->GetGameRun()->AddFixedMessage(FIXMSG_GIFT_BAG_CULTI_LIMIT);
		return false;
	}
	if ( m_pDBEssence->id1_object_need )
	{
		if(pHost->GetPack()->GetItemTotalNum(m_pDBEssence->id1_object_need ) < (int)(m_pDBEssence->id1_object_num) )
		{
			g_pGame->GetGameRun()->AddFixedMessage(FIXMSG_GIFT_BAG_LACK_ITEM);
			return false;
		}
	}
	if (m_pDBEssence->id2_object_need )
	{
		if(pHost->GetPack()->GetItemTotalNum(m_pDBEssence->id2_object_need) < (int)(m_pDBEssence->id2_object_num) )
		{	
			g_pGame->GetGameRun()->AddFixedMessage(FIXMSG_GIFT_BAG_LACK_ITEM);
			return false;
		}
	}
	if (pHost->GetMoneyAmount() < m_pDBEssence->money_need)
	{
		g_pGame->GetGameRun()->AddFixedMessage(FIXMSG_GIFT_BAG_LACK_MONEY);
		return false;
	}
	if( (int)(m_pDBEssence->num_object) > pHost->GetPack()->GetEmptySlotNum())
	{
		g_pGame->GetGameRun()->AddFixedMessage(FIXMSG_GIFT_BAG_LACK_SPACE);
		return false;
	}

	if (!CheckCrossUseCondition())
		return false;

	return true;
}

//	Get drop model for shown
const char * CECIvtrGiftBag::GetDropModel()
{
	return g_pGame->GetDataPath(m_pDBEssence->file_matter);
}

//	Build gender requirement description
void CECIvtrGiftBag::BuildGenderReqDesc()
{
	if (m_iGenderReq == 2)
		return;		//	Both male and female can equip

	CECStringTab* pDescTab = g_pGame->GetItemDesc();
	CECHostPlayer* pHost = g_pGame->GetGameRun()->GetHostPlayer();

	int col;

	//	Gender requirement
	CECStringTab* pFixMsg = g_pGame->GetFixedMsgTab();
	if (pHost->GetGender() != m_iGenderReq)
		col = ITEMDESC_COL_RED;
	else
		col = ITEMDESC_COL_WHITE;

	AddDescText(col, false, pDescTab->GetWideString(ITEMDESC_GENDERREQ));
	m_strDesc += _AL(" ");
	if (m_iGenderReq == 0)
		AddDescText(col, true, pFixMsg->GetWideString(FIXMSG_GENDER_MALE));
	else	//	if (m_iGenderReq == 1)
		AddDescText(col, true, pFixMsg->GetWideString(FIXMSG_GENDER_FEMALE));
}


///////////////////////////////////////////////////////////////////////////
//	
//	Implement CECIvtrGiftBagLotteryDeliver
//	
///////////////////////////////////////////////////////////////////////////

CECIvtrGiftBagLotteryDeliver::CECIvtrGiftBagLotteryDeliver(int tid, int expire_date) : CECIvtrItem(tid, expire_date)
{
	m_iCID	= ICID_GIFT_BAG_LOTTERY_DELIVER;

	//	Get database data
	elementdataman* pDB = g_pGame->GetElementDataMan();
	DATA_TYPE DataType;
	m_pDBEssence = (GIFT_BAG_LOTTERY_DELIVER_ESSENCE*)pDB->get_data_ptr(tid, ID_SPACE_ESSENCE, DataType);
	
	m_iPileLimit	= m_pDBEssence->pile_num_max;
	m_iPrice		= m_pDBEssence->price;
	m_iShopPrice	= m_pDBEssence->shop_price;
//	m_iLevelReq		= m_pDBEssence->require_level;
//	m_iProfReq		= m_pDBEssence->character_combo_id;
	m_iGenderReq	= m_pDBEssence->require_gender;
	m_iProcType		= m_pDBEssence->proc_type;
	m_iEquipMask	= 0;
	m_bUseable		= false;	// why false? 因为如果不是false，打开包裹如果不能使用会有不断的提示
	m_bNeedUpdate	= false;
}

CECIvtrGiftBagLotteryDeliver::CECIvtrGiftBagLotteryDeliver(const CECIvtrGiftBagLotteryDeliver& s) : CECIvtrItem(s)
{
	m_pDBEssence	= s.m_pDBEssence;
	m_iGenderReq	= s.m_iGenderReq;
	m_Essence		= s.m_Essence;
}

CECIvtrGiftBagLotteryDeliver::~CECIvtrGiftBagLotteryDeliver()
{
}

//	Set item detail information
bool CECIvtrGiftBagLotteryDeliver::SetItemInfo(BYTE* pInfoData, int iDataLen)
{
	if( iDataLen == 0 )
		memset(&m_Essence, 0, sizeof(m_Essence));
	else
		memcpy(&m_Essence, pInfoData, sizeof(m_Essence));
	
	CECIvtrItem::SetItemInfo(pInfoData, iDataLen);
	return CECIvtrItem::SetItemInfo(pInfoData, iDataLen);
}

//	Get item icon file name
const char* CECIvtrGiftBagLotteryDeliver::GetIconFile()
{
	return g_pGame->GetDataPath(m_pDBEssence->file_icon);
}

//	Get item name
const wchar_t* CECIvtrGiftBagLotteryDeliver::GetName()
{
	return m_pDBEssence->name;
}

//	Get item cool time
int CECIvtrGiftBagLotteryDeliver::GetCoolTime(int* piMax/* NULL */)
{
	return 0;
}

//	Get item description text
const wchar_t* CECIvtrGiftBagLotteryDeliver::GetNormalDesc(bool bRepair)
{
	m_strDesc = _AL("");

	//	Try to build item description
	CECStringTab* pDescTab = g_pGame->GetItemDesc();
	CECStringTab* pFixMsg = g_pGame->GetFixedMsgTab();
	int red = ITEMDESC_COL_RED;
	int white = ITEMDESC_COL_WHITE;
	int name_color = DecideNameCol();
	CECHostPlayer* pHost = g_pGame->GetGameRun()->GetHostPlayer();
	if (m_iCount > 1)
		AddDescText(name_color, true, pDescTab->GetWideString(ITEMDESC_NAMENUMBER), GetName(), m_iCount);
	else
		AddDescText(name_color, true, pDescTab->GetWideString(ITEMDESC_NAME), GetName());
	
	AddStateDesc(pDescTab);

	//	Level requirement
	int iHostLevel = pHost->GetBasicProps().iLevel;	
	int iEverMaxLevel = iHostLevel;

	for (int i = 0; i< pHost->GetRebornCount(); ++i)
	{
		if (iEverMaxLevel < pHost->GetRebornInfo()[i].level)
			iEverMaxLevel = pHost->GetRebornInfo()[i].level;
	}
	if (m_pDBEssence->level_everLimit)
	{
		int col = red;
		if (iEverMaxLevel >= m_pDBEssence->level_everLimit)
			col = white;
		AddDescText(col,true, pDescTab->GetWideString(ITEMDESC_EVER_LEVELREQ), m_pDBEssence->level_everLimit);
	}

	if(m_pDBEssence->level_required > 0 && m_pDBEssence->level_upperLimit > 0)
	{
		int col = red;
		if(iHostLevel >= m_pDBEssence->level_required && iHostLevel <= m_pDBEssence->level_upperLimit)
			col = white;
		AddDescText(col, true, pDescTab->GetWideString(ITEMDESC_LEVELUPPERLIMIT), m_pDBEssence->level_required, m_pDBEssence->level_upperLimit);
	}
	else if(m_pDBEssence->level_required == 0 && m_pDBEssence->level_upperLimit > 0 )
	{
		int col = red;
		if(iHostLevel >= m_pDBEssence->level_required && iHostLevel <= m_pDBEssence->level_upperLimit)
			col = white;
		AddDescText(col, true, pDescTab->GetWideString(ITEMDESC_LEVELUPPERLIMIT), 1, m_pDBEssence->level_upperLimit);
	}
	else if(m_pDBEssence->level_required >0 && m_pDBEssence->level_upperLimit == 0 )
	{
		int col = red;
		if(iHostLevel >= m_pDBEssence->level_required)
			col = white;
		AddDescText(col, true, pDescTab->GetWideString(ITEMDESC_LEVELREQ), m_pDBEssence->level_required);
 	}
	
	// gender requirment
	BuildGenderReqDesc();

	if (m_pDBEssence->renascence_count > 0)
	{
		int col = (pHost->GetRebornCount() >= m_pDBEssence->renascence_count) ? ITEMDESC_COL_WHITE : ITEMDESC_COL_RED;
		AddDescText(col, true, pDescTab->GetWideString(ITEMDESC_RENASCENCE_REQUIRED), pDescTab->GetWideString(ITEMDESC_RENASCENCE_1+m_pDBEssence->renascence_count-1));
	}
	
	if (m_pDBEssence->money_need)
	{
		ACHAR szPrice[256];

		GetPriceText(szPrice, m_pDBEssence->money_need);
		AddDescText(white, true, pDescTab->GetWideString(ITEMDESC_GIFT_BAG_NEED_MONEY), szPrice);
	}

	AddDescText(white, true, pDescTab->GetWideString(ITEMDESC_GIFT_BAG_NEED_SPACE), m_pDBEssence->num_object);

	AddExpireTimeDesc();	
	//	Price
	AddPriceDesc(white, bRepair);

	AddExtDescText();
	return m_strDesc;
}

//	Check item use condition
bool CECIvtrGiftBagLotteryDeliver::CheckUseCondition()
{
	if (!m_pDBEssence)
		return false;
	CECHostPlayer* pHost = g_pGame->GetGameRun()->GetHostPlayer();
	if(!pHost)
		return false;
	CECStringTab* pDescTab = g_pGame->GetItemDesc();
	//	Check level requirement
	int iHostLevel = pHost->GetBasicProps().iLevel;
	int iEverMaxLevel = iHostLevel;

	for (int i = 0; i< pHost->GetRebornCount(); ++i)
	{
		if (iEverMaxLevel < pHost->GetRebornInfo()[i].level)
			iEverMaxLevel = pHost->GetRebornInfo()[i].level;
	}
	if (m_pDBEssence->level_everLimit && m_pDBEssence->level_everLimit > iEverMaxLevel)
	{
		g_pGame->GetGameRun()->AddFixedMessage(FIXMSG_LEVELTOOLOW);
		return false;		
	}
	if (m_pDBEssence->level_required && pHost->GetBasicProps().iLevel < m_pDBEssence->level_required)
	{
		g_pGame->GetGameRun()->AddFixedMessage(FIXMSG_LEVELTOOLOW);
		return false;
	}
	if (m_pDBEssence->level_upperLimit && pHost->GetBasicProps().iLevel > m_pDBEssence->level_upperLimit)
	{
		g_pGame->GetGameRun()->AddFixedMessage(FIXMSG_LEVELTOOHIGH);
		return false;
 	}
	if (m_iGenderReq != 2 && pHost->GetGender() != m_iGenderReq)
	{
		g_pGame->GetGameRun()->AddFixedMessage(FIXMSG_GIFT_BAG_GENDER_LIMIT, m_iGenderReq==0? pDescTab->GetWideString(ITEMDESC_MALE):pDescTab->GetWideString(ITEMDESC_FEMALE));
		return false;
	}
	if (pHost->GetRebornCount() < m_pDBEssence->renascence_count)
	{
		g_pGame->GetGameRun()->AddFixedMessage(FIXMSG_GIFT_BAG_REBORN_LIMIT);
		return false;
	}
	if (((pHost->GetProfession() < 64)&&((m_pDBEssence->character_combo_id & (UINT64(1)<< pHost->GetProfession())) == 0))  ||
		((pHost->GetProfession() >= 64)&&((m_pDBEssence->character_combo_id2 & (UINT64(1)<< (pHost->GetProfession() - 64))) == 0)) ) // Modified 2011-07-18.
	{
		g_pGame->GetGameRun()->AddFixedMessage(FIXMSG_GIFT_BAG_PREF_LIMIT);
		return false;
	}
	if ( m_pDBEssence->god_devil_mask !=0 && (pHost->GetCultivation() & m_pDBEssence->god_devil_mask) == 0)
	{
		g_pGame->GetGameRun()->AddFixedMessage(FIXMSG_GIFT_BAG_CULTI_LIMIT);
		return false;
	}
	if ( m_pDBEssence->id1_object_need )
	{
		if(pHost->GetPack()->GetItemTotalNum(m_pDBEssence->id1_object_need ) < (int)(m_pDBEssence->id1_object_num) )
		{
			g_pGame->GetGameRun()->AddFixedMessage(FIXMSG_GIFT_BAG_LACK_ITEM);
			return false;
		}
	}
	if (m_pDBEssence->id2_object_need )
	{
		if(pHost->GetPack()->GetItemTotalNum(m_pDBEssence->id2_object_need) < (int)(m_pDBEssence->id2_object_num) )
		{	
			g_pGame->GetGameRun()->AddFixedMessage(FIXMSG_GIFT_BAG_LACK_ITEM);
			return false;
		}
	}
	if (pHost->GetMoneyAmount() < m_pDBEssence->money_need)
	{
		g_pGame->GetGameRun()->AddFixedMessage(FIXMSG_GIFT_BAG_LACK_MONEY);
		return false;
	}
	if( (int)(m_pDBEssence->num_object) > pHost->GetPack()->GetEmptySlotNum())
	{
		g_pGame->GetGameRun()->AddFixedMessage(FIXMSG_GIFT_BAG_LACK_SPACE);
		return false;
	}

	if (!CheckCrossUseCondition())
		return false;

	return true;
}

//	Get drop model for shown
const char * CECIvtrGiftBagLotteryDeliver::GetDropModel()
{
	return g_pGame->GetDataPath(m_pDBEssence->file_matter);
}

//	Build gender requirement description
void CECIvtrGiftBagLotteryDeliver::BuildGenderReqDesc()
{
	if (m_iGenderReq == 2)
		return;		//	Both male and female can equip

	CECStringTab* pDescTab = g_pGame->GetItemDesc();
	CECHostPlayer* pHost = g_pGame->GetGameRun()->GetHostPlayer();

	int col;

	//	Gender requirement
	CECStringTab* pFixMsg = g_pGame->GetFixedMsgTab();
	if (pHost->GetGender() != m_iGenderReq)
		col = ITEMDESC_COL_RED;
	else
		col = ITEMDESC_COL_WHITE;

	AddDescText(col, false, pDescTab->GetWideString(ITEMDESC_GENDERREQ));
	m_strDesc += _AL(" ");
	if (m_iGenderReq == 0)
		AddDescText(col, true, pFixMsg->GetWideString(FIXMSG_GENDER_MALE));
	else	//	if (m_iGenderReq == 1)
		AddDescText(col, true, pFixMsg->GetWideString(FIXMSG_GENDER_FEMALE));
}


///////////////////////////////////////////////////////////////////////////
//	
//	Implement CECIvtrLotteryTangYuan
//	
///////////////////////////////////////////////////////////////////////////

CECIvtrLotteryTangYuan::CECIvtrLotteryTangYuan(int tid, int expire_date) : CECIvtrItem(tid, expire_date)
{
	m_iCID	= ICID_LOTTERY_TANGYUAN;

	//	Get database data
	elementdataman* pDB = g_pGame->GetElementDataMan();
	DATA_TYPE DataType;
	m_pDBEssence = (LOTTERY_TANGYUAN_ITEM_ESSENCE*)pDB->get_data_ptr(tid, ID_SPACE_ESSENCE, DataType);
	memset(&m_Essence, 0, sizeof(m_Essence));
	
	m_iPileLimit	= m_pDBEssence->pile_num_max;
	m_iPrice		= m_pDBEssence->price;
	m_iShopPrice	= m_pDBEssence->shop_price;
//	m_iLevelReq		= m_pDBEssence->require_level;
//	m_iProfReq		= m_pDBEssence->character_combo_id;
	m_iProcType		= m_pDBEssence->proc_type;
	m_iEquipMask	= 0;
	m_bUseable		= false;	// why false? 因为如果不是false，打开包裹如果不能使用会有不断的提示
	m_bNeedUpdate	= false;
}

CECIvtrLotteryTangYuan::CECIvtrLotteryTangYuan(const CECIvtrLotteryTangYuan& s) : CECIvtrItem(s)
{
	m_pDBEssence	= s.m_pDBEssence;
	m_Essence		= s.m_Essence;
}

CECIvtrLotteryTangYuan::~CECIvtrLotteryTangYuan()
{
}

//	Set item detail information
bool CECIvtrLotteryTangYuan::SetItemInfo(BYTE* pInfoData, int iDataLen)
{
	if( iDataLen == 0 )
		memset(&m_Essence, 0, sizeof(m_Essence));
	else
	{
		int iOld = m_Essence.exp;
		memcpy(&m_Essence, pInfoData, sizeof(m_Essence));
#ifndef _INVENTORY_DESC_MAN
		if( (m_Essence.exp-iOld)>100 )
		{
			PAUIDIALOG pDlg =  g_pGame->GetGameRun()->GetUIManager()->GetInGameUIMan()->GetDialog("Win_Lottery5");
			if(pDlg && pDlg->IsShow())
			{
				m_lstShowExp.AddTail(new int((m_Essence.exp-iOld)/100));
			}
		}
#endif
	}
	
	return CECIvtrItem::SetItemInfo(pInfoData, iDataLen);
}

//	Get item icon file name
const char* CECIvtrLotteryTangYuan::GetIconFile()
{
	return g_pGame->GetDataPath(m_pDBEssence->file_icon);
}

//	Get item name
const wchar_t* CECIvtrLotteryTangYuan::GetName()
{
	return m_pDBEssence->name;
}

//	Get item cool time
int CECIvtrLotteryTangYuan::GetCoolTime(int* piMax/* NULL */)
{
	return 0;
}

//	Get item description text
const wchar_t* CECIvtrLotteryTangYuan::GetNormalDesc(bool bRepair)
{
	m_strDesc = _AL("");

	//	Try to build item description
	CECStringTab* pDescTab = g_pGame->GetItemDesc();
	CECStringTab* pFixMsg = g_pGame->GetFixedMsgTab();
	int red = ITEMDESC_COL_RED;
	int white = ITEMDESC_COL_WHITE;
	int name_color = DecideNameCol();
	CECHostPlayer* pHost = g_pGame->GetGameRun()->GetHostPlayer();
	if (m_iCount > 1)
		AddDescText(name_color, true, pDescTab->GetWideString(ITEMDESC_NAMENUMBER), GetName(), m_iCount);
	else
		AddDescText(name_color, true, pDescTab->GetWideString(ITEMDESC_NAME), GetName());
	
	AddDescText(white, true, pDescTab->GetWideString(ITEMDESC_TANGYUAN_LEVEL), GetLevel());
	AddDescText(white, true, pDescTab->GetWideString(ITEMDESC_TANGYUAN_EXP), m_Essence.exp);
	if (m_Essence.ticket>0)
	{
		int itemId = m_pDBEssence->group_gifts[(m_Essence.ticket-1)/4].gifts[(m_Essence.ticket-1)%4].id;
		int itemNum = m_pDBEssence->group_gifts[(m_Essence.ticket-1)/4].gifts[(m_Essence.ticket-1)%4].num;
		CECIvtrItem *pItem = CECIvtrItem::CreateItem(itemId, 0, 1);
		if(pItem)
		{
			ACString strText;
			strText.Format(pDescTab->GetWideString(ITEMDESC_TANGYUAN_AWARD), pItem->GetName());
			if (itemNum>1)
			{
				ACString strTemp;
				strTemp.Format(_AL("x%d"), itemNum);
				strText += strTemp;
			}
			m_strDesc += strText;
			delete pItem;
		}
		m_strDesc += _AL("\\r");
	}

//	AddDescText(white, true, pDescTab->GetWideString(ITEMDESC_GIFT_BAG_NEED_SPACE), m_pDBEssence->num_object);

	AddStateDesc(pDescTab);
	AddExpireTimeDesc();	
	//	Price
	AddPriceDesc(white, bRepair);

	AddExtDescText();
	return m_strDesc;
}

//	Get drop model for shown
const char * CECIvtrLotteryTangYuan::GetDropModel()
{
	return g_pGame->GetDataPath(m_pDBEssence->file_matter);
}

int CECIvtrLotteryTangYuan::GetLevel()
{
	int i;
	for (i=0; i<7; ++i)
	{
		if (m_Essence.exp<m_pDBEssence->exp_level[i])
		{
			return i+1;
		}
	}
	
	return 8;
}

int CECIvtrLotteryTangYuan::ShowOneExp()
{
	if(m_lstShowExp.GetCount()>0)
	{
		int* iPop = m_lstShowExp.RemoveHead();
		int iRet = *iPop;
		delete iPop;
		return iRet;
	}
	else
		return -1;

}


///////////////////////////////////////////////////////////////////////////
//	
//	Implement CECIvtrVIPCard
//	
///////////////////////////////////////////////////////////////////////////

CECIvtrVIPCard::CECIvtrVIPCard(int tid, int expire_date) : CECIvtrItem(tid, expire_date)
{
	m_iCID	= ICID_VIP_CARD;

	//	Get database data
	elementdataman* pDB = g_pGame->GetElementDataMan();
	DATA_TYPE DataType;
	m_pDBEssence = (VIP_CARD_ESSENCE*)pDB->get_data_ptr(tid, ID_SPACE_ESSENCE, DataType);
	
	m_iPileLimit	= m_pDBEssence->pile_num_max;
	m_iPrice		= m_pDBEssence->price;
	m_iShopPrice	= m_pDBEssence->shop_price;
	m_iProcType		= m_pDBEssence->proc_type;
	m_iEquipMask	= 0;
	m_bUseable		= false;
	m_bNeedUpdate	= false;
}

CECIvtrVIPCard::CECIvtrVIPCard(const CECIvtrVIPCard& s) : CECIvtrItem(s)
{
	m_pDBEssence	= s.m_pDBEssence;
}

CECIvtrVIPCard::~CECIvtrVIPCard()
{
}

//	Set item detail information
bool CECIvtrVIPCard::SetItemInfo(BYTE* pInfoData, int iDataLen)
{
	return CECIvtrItem::SetItemInfo(pInfoData, iDataLen);
}

//	Get item icon file name
const char* CECIvtrVIPCard::GetIconFile()
{
	return g_pGame->GetDataPath(m_pDBEssence->file_icon);
}

//	Get item name
const wchar_t* CECIvtrVIPCard::GetName()
{
	return m_pDBEssence->name;
}

//	Get item cool time
int CECIvtrVIPCard::GetCoolTime(int* piMax/* NULL */)
{
	return 0;
}

//	Get item description text
const wchar_t* CECIvtrVIPCard::GetNormalDesc(bool bRepair)
{
	m_strDesc = _AL("");

	//	Try to build item description
	CECStringTab* pDescTab = g_pGame->GetItemDesc();
	CECStringTab* pFixMsg = g_pGame->GetFixedMsgTab();
	int red = ITEMDESC_COL_RED;
	int white = ITEMDESC_COL_WHITE;
	int name_color = DecideNameCol();
	CECHostPlayer* pHost = g_pGame->GetGameRun()->GetHostPlayer();
	if (m_iCount > 1)
		AddDescText(name_color, true, pDescTab->GetWideString(ITEMDESC_NAMENUMBER), GetName(), m_iCount);
	else
		AddDescText(name_color, true, pDescTab->GetWideString(ITEMDESC_NAME), GetName());

	AddStateDesc(pDescTab);
	AddExpireTimeDesc();
	//	Price
	AddPriceDesc(white, bRepair);

	AddExtDescText();
	return m_strDesc;
}

//	Check item use condition
bool CECIvtrVIPCard::CheckUseCondition()
{
	if (!m_pDBEssence)
		return false;
	if (!CheckCrossUseCondition())
		return false;
	return true;
}

//	Get drop model for shown
const char * CECIvtrVIPCard::GetDropModel()
{
	return g_pGame->GetDataPath(m_pDBEssence->file_matter);
}


///////////////////////////////////////////////////////////////////////////
//	
//	Implement CECIvtrMercenaryCredential
//	
///////////////////////////////////////////////////////////////////////////

CECIvtrMercenaryCredential::CECIvtrMercenaryCredential(int tid, int expire_date) : CECIvtrItem(tid, expire_date)
{
	m_iCID	= ICID_MERCENARY_CREDENTIAL;

	//	Get database data
	elementdataman* pDB = g_pGame->GetElementDataMan();
	DATA_TYPE DataType;
	m_pDBEssence = (MERCENARY_CREDENTIAL_ESSENCE*)pDB->get_data_ptr(tid, ID_SPACE_ESSENCE, DataType);
	memset(&m_Essence, 0, sizeof(m_Essence));
	
	m_iPileLimit	= m_pDBEssence->pile_num_max;
	m_iPrice		= m_pDBEssence->price;
	m_iShopPrice	= m_pDBEssence->shop_price;
	m_iProcType		= m_pDBEssence->proc_type;
	m_bUseable		= false;
}

CECIvtrMercenaryCredential::CECIvtrMercenaryCredential(const CECIvtrMercenaryCredential& s) : CECIvtrItem(s)
{
	m_pDBEssence	= s.m_pDBEssence;
	m_Essence		= s.m_Essence;
}

CECIvtrMercenaryCredential::~CECIvtrMercenaryCredential()
{
}

//	Set item detail information
bool CECIvtrMercenaryCredential::SetItemInfo(BYTE* pInfoData, int iDataLen)
{
	CECIvtrItem::SetItemInfo(pInfoData, iDataLen);
	if (iDataLen == sizeof(m_Essence))
		memcpy(&m_Essence, pInfoData, sizeof(m_Essence));
	return true;
}

//	Get item icon file name
const char* CECIvtrMercenaryCredential::GetIconFile()
{
	return g_pGame->GetDataPath(m_pDBEssence->file_icon);
}

//	Get item name
const wchar_t* CECIvtrMercenaryCredential::GetName()
{
	return m_pDBEssence->name;
}

//	Get item cool time
int CECIvtrMercenaryCredential::GetCoolTime(int* piMax/* NULL */)
{
	return 0;
}

//	Get item description text
const wchar_t* CECIvtrMercenaryCredential::GetNormalDesc(bool bRepair)
{
	if (m_bNeedUpdate)
		return NULL;

	m_strDesc = _AL("");

	//	Try to build item description
	CECStringTab* pDescTab = g_pGame->GetItemDesc();
	int red = ITEMDESC_COL_RED;
	int white = ITEMDESC_COL_WHITE;
	int name_color = DecideNameCol();
	CECHostPlayer* pHost = g_pGame->GetGameRun()->GetHostPlayer();
	if (m_iCount > 1)
		AddDescText(name_color, true, pDescTab->GetWideString(ITEMDESC_NAMENUMBER), GetName(), m_iCount);
	else
		AddDescText(name_color, true, pDescTab->GetWideString(ITEMDESC_NAME), GetName());
	
	if (m_Essence.faction_id)
	{
		Faction_Info * pInfo = g_pGame->GetFactionMan()->GetFaction(m_Essence.faction_id);
		if(pInfo)
			AddDescText(white, true, pDescTab->GetWideString(ITEMDESC_MERCENARY_CREDENTIAL_FACTION),pInfo->GetName());
	}

	if (m_Essence.territory_id)
	{
		if (m_strTerritoryName.IsEmpty())
		{
			vector<DOMAIN_INFO> *info = globaldata_getdomaininfos2();
			for(int i = 0; i < (int)info->size(); ++i )
			{
				if ((*info)[i].id == m_Essence.territory_id)
					m_strTerritoryName = (*info)[i].name;
			}
		}
		else
			AddDescText(white, true, pDescTab->GetWideString(ITEMDESC_MERCENARY_CREDENTIAL_TERRITORY), m_strTerritoryName);
	}
	
	AddStateDesc(pDescTab);
	AddExpireTimeDesc();
	//	Price
	AddPriceDesc(white, bRepair);

	AddExtDescText();
	return m_strDesc;
}

//	Check item use condition
bool CECIvtrMercenaryCredential::CheckUseCondition()
{
	if (!m_pDBEssence)
		return false;
	if (!CheckCrossUseCondition())
		return false;
	return true;
}

//	Get drop model for shown
const char * CECIvtrMercenaryCredential::GetDropModel()
{
	return g_pGame->GetDataPath(m_pDBEssence->file_matter);
}



///////////////////////////////////////////////////////////////////////////
//	
//	Implement CECIvtrTelePortation
//	
///////////////////////////////////////////////////////////////////////////

CECIvtrTelePortation::CECIvtrTelePortation(int tid, int expire_date) : CECIvtrItem(tid, expire_date)
{
	m_iCID	= ICID_TELEPORTATION;

	//	Get database data
	elementdataman* pDB = g_pGame->GetElementDataMan();
	DATA_TYPE DataType;
	m_pDBEssence = (TELEPORTATION_ESSENCE*)pDB->get_data_ptr(tid, ID_SPACE_ESSENCE, DataType);
	memset(&m_Essence, 0, sizeof(m_Essence));
	
	m_iPileLimit	= m_pDBEssence->pile_num_max;
	m_iPrice		= m_pDBEssence->price;
	m_iShopPrice	= m_pDBEssence->shop_price;
	m_iProcType		= m_pDBEssence->proc_type;
	m_bUseable		= false;
}

CECIvtrTelePortation::CECIvtrTelePortation(const CECIvtrTelePortation& s) : CECIvtrItem(s)
{
	m_pDBEssence	= s.m_pDBEssence;
	m_Essence		= s.m_Essence;
}

CECIvtrTelePortation::~CECIvtrTelePortation()
{
}

//	Set item detail information
bool CECIvtrTelePortation::SetItemInfo(BYTE* pInfoData, int iDataLen)
{
	CECIvtrItem::SetItemInfo(pInfoData, iDataLen);
	if(iDataLen < sizeof(m_Essence.ess))
		return false;

	if( iDataLen >= sizeof(m_Essence.ess) )
		memcpy(&m_Essence.ess, pInfoData, sizeof(m_Essence.ess));
	pInfoData += sizeof(m_Essence.ess);
	iDataLen -= sizeof(m_Essence.ess);
	// 添加星盘标注文字信息
	if (iDataLen==(m_Essence.ess.num*(int)sizeof(IVTR_TELEPORTATION_ESSENCE::coordinate_point)+16))
	{
		memcpy(&m_Essence.memo, pInfoData, 16);
		pInfoData += 16;
		iDataLen -= 16;
	}

	m_Essence.points.clear();
	int num = m_Essence.ess.num;
	for (int i = 0; i<m_Essence.ess.num; ++i )
	{
		if (iDataLen < sizeof(IVTR_TELEPORTATION_ESSENCE::coordinate_point))
			break;

		IVTR_TELEPORTATION_ESSENCE::coordinate_point point;
		point.inst = *(int*)(pInfoData + i*sizeof(IVTR_TELEPORTATION_ESSENCE::coordinate_point));
		point.x    = *(float*)(pInfoData + i*sizeof(IVTR_TELEPORTATION_ESSENCE::coordinate_point) + sizeof(int));
		point.y    = *(float*)(pInfoData + i*sizeof(IVTR_TELEPORTATION_ESSENCE::coordinate_point) + sizeof(int) + sizeof(float));
		point.z    = *(float*)(pInfoData + i*sizeof(IVTR_TELEPORTATION_ESSENCE::coordinate_point) + sizeof(int) + 2*sizeof(float));
		m_Essence.points.push_back(point);
		iDataLen -= sizeof(IVTR_TELEPORTATION_ESSENCE::coordinate_point);
	}

	return true;
}

//	Get item icon file name
const char* CECIvtrTelePortation::GetIconFile()
{
	return g_pGame->GetDataPath(m_pDBEssence->file_icon);
}

//	Get item name
const wchar_t* CECIvtrTelePortation::GetName()
{
	return m_pDBEssence->name;
}

//	Get item cool time
int CECIvtrTelePortation::GetCoolTime(int* piMax/* NULL */)
{
	return 0;
}

void CECIvtrTelePortation::AddValidTimeDesc()
{
	if( m_Essence.ess.time == 0 )
		return;

	int green = ITEMDESC_COL_GREEN;
	int red = ITEMDESC_COL_RED;
	int gray = ITEMDESC_COL_GRAY;

	CECStringTab* pDescTab = g_pGame->GetItemDesc();
	time_t timeLeft = m_Essence.ess.time - g_pGame->GetServerGMTTime();
	if( timeLeft < 0 ) timeLeft = 0;

	if( timeLeft > 24 * 3600 )
		AddDescText(green, true, pDescTab->GetWideString(ITEMDESC_VALIDTIME_DAY), timeLeft / (24 * 3600), (timeLeft % (24 * 3600)) / 3600);
	else if( timeLeft > 3600 )
	{
		AddDescText(red, true, pDescTab->GetWideString(ITEMDESC_VALIDTIME_HOUR_MIN), timeLeft / 3600, (timeLeft % 3600) / 60);
	}
	else if( timeLeft > 60 )
	{
		AddDescText(red, true, pDescTab->GetWideString(ITEMDESC_VALIDTIME_MIN_SEC), timeLeft / 60, timeLeft % 60);
	}
	else if ( timeLeft > 0)
	{
		AddDescText(red, true, pDescTab->GetWideString(ITEMDESC_VALIDTIME_SECOND), timeLeft);
	}
	else
	{
		AddDescText(gray, true, pDescTab->GetWideString(ITEMDESC_VALIDTIME_ZERO), timeLeft);
	}
}

//	Get item description text
const wchar_t* CECIvtrTelePortation::GetNormalDesc(bool bRepair)
{
	if (m_bNeedUpdate)
		return NULL;

	m_strDesc = _AL("");

	//	Try to build item description
	CECStringTab* pDescTab = g_pGame->GetItemDesc();
	int red = ITEMDESC_COL_RED;
	int white = ITEMDESC_COL_WHITE;
	int name_color = DecideNameCol();
	CECHostPlayer* pHost = g_pGame->GetGameRun()->GetHostPlayer();
	if (m_iCount > 1)
		AddDescText(name_color, true, pDescTab->GetWideString(ITEMDESC_NAMENUMBER), GetName(), m_iCount);
	else
		AddDescText(name_color, true, pDescTab->GetWideString(ITEMDESC_NAME), GetName());

	const ACHAR *cMemo = (const ACHAR *)m_Essence.memo;
	if (cMemo[0]!=0)
	{
		char cMemoInfo[18];
		memcpy(cMemoInfo, m_Essence.memo, 16);
		cMemoInfo[16] = 0;
		cMemoInfo[17] = 0;
		AddDescText(name_color, true, _AL("%s"), cMemoInfo);
	}

	AddStateDesc(pDescTab);	
	
	AddValidTimeDesc();
	
	AddExpireTimeDesc();
	//	Price
	AddPriceDesc(white, bRepair);

	AddExtDescText();
	return m_strDesc;
}

//	Check item use condition
bool CECIvtrTelePortation::CheckUseCondition()
{
	if (!m_pDBEssence)
		return false;
	if (!CheckCrossUseCondition())
		return false;
	return true;
}

//	Get drop model for shown
const char * CECIvtrTelePortation::GetDropModel()
{
	return g_pGame->GetDataPath(m_pDBEssence->file_matter);
}


///////////////////////////////////////////////////////////////////////////
//	
//	Implement CECIvtrTelePortationStone
//	
///////////////////////////////////////////////////////////////////////////

CECIvtrTelePortationStone::CECIvtrTelePortationStone(int tid, int expire_date) : CECIvtrItem(tid, expire_date)
{
	m_iCID	= ICID_TELEPORTATION_STONE;

	//	Get database data
	elementdataman* pDB = g_pGame->GetElementDataMan();
	DATA_TYPE DataType;
	m_pDBEssence = (TELEPORTATION_STONE_ESSENCE*)pDB->get_data_ptr(tid, ID_SPACE_ESSENCE, DataType);
	
	m_iPileLimit	= m_pDBEssence->pile_num_max;
	m_iPrice		= m_pDBEssence->price;
	m_iShopPrice	= m_pDBEssence->shop_price;
	m_iProcType		= m_pDBEssence->proc_type;
	m_bUseable		= false;
	m_bNeedUpdate	= false;
}

CECIvtrTelePortationStone::CECIvtrTelePortationStone(const CECIvtrTelePortationStone& s) : CECIvtrItem(s)
{
	m_pDBEssence	= s.m_pDBEssence;
}

CECIvtrTelePortationStone::~CECIvtrTelePortationStone()
{
}

//	Set item detail information
bool CECIvtrTelePortationStone::SetItemInfo(BYTE* pInfoData, int iDataLen)
{
	CECIvtrItem::SetItemInfo(pInfoData, iDataLen);
	return true;
}

//	Get item icon file name
const char* CECIvtrTelePortationStone::GetIconFile()
{
	return g_pGame->GetDataPath(m_pDBEssence->file_icon);
}

//	Get item name
const wchar_t* CECIvtrTelePortationStone::GetName()
{
	return m_pDBEssence->name;
}

//	Get item cool time
int CECIvtrTelePortationStone::GetCoolTime(int* piMax/* NULL */)
{
	return 0;
}

//	Get item description text
const wchar_t* CECIvtrTelePortationStone::GetNormalDesc(bool bRepair)
{
	if (m_bNeedUpdate)
		return NULL;

	m_strDesc = _AL("");

	//	Try to build item description
	CECStringTab* pDescTab = g_pGame->GetItemDesc();
	int red = ITEMDESC_COL_RED;
	int white = ITEMDESC_COL_WHITE;
	int green = ITEMDESC_COL_GREEN;
	int name_color = DecideNameCol();
	CECHostPlayer* pHost = g_pGame->GetGameRun()->GetHostPlayer();
	if (m_iCount > 1)
		AddDescText(name_color, true, pDescTab->GetWideString(ITEMDESC_NAMENUMBER), GetName(), m_iCount);
	else
		AddDescText(name_color, true, pDescTab->GetWideString(ITEMDESC_NAME), GetName());
	
	AddDescText(green, true, pDescTab->GetWideString(ITEMDESC_VALIDTIME_DAY2), m_pDBEssence->day);
	
	AddStateDesc(pDescTab);
	AddExpireTimeDesc();
	//	Price
	AddPriceDesc(white, bRepair);

	AddExtDescText();
	return m_strDesc;
}

//	Check item use condition
bool CECIvtrTelePortationStone::CheckUseCondition()
{
	if (!m_pDBEssence)
		return false;
	if (!CheckCrossUseCondition())
		return false;
	return true;
}

//	Get drop model for shown
const char * CECIvtrTelePortationStone::GetDropModel()
{
	return g_pGame->GetDataPath(m_pDBEssence->file_matter);
}



///////////////////////////////////////////////////////////////////////////
//	
//	Implement CECIvtrUpgradeEquipStone
//	
///////////////////////////////////////////////////////////////////////////

CECIvtrUpgradeEquipStone::CECIvtrUpgradeEquipStone(int tid, int expire_date) : CECIvtrItem(tid, expire_date)
{
	m_iCID	= ICID_UPGRADE_EQUIP_STONE;

	//	Get database data
	elementdataman* pDB = g_pGame->GetElementDataMan();
	DATA_TYPE DataType;
	m_pDBEssence = (UPGRADE_EQUIP_STONE_ESSENCE*)pDB->get_data_ptr(tid, ID_SPACE_ESSENCE, DataType);
	
	m_iPileLimit	= m_pDBEssence->pile_num_max;
	m_iPrice		= m_pDBEssence->price;
	m_iShopPrice	= m_pDBEssence->shop_price;
	m_iProcType		= m_pDBEssence->proc_type;
	m_bUseable		= false;
	m_bNeedUpdate	= false;
}

CECIvtrUpgradeEquipStone::CECIvtrUpgradeEquipStone(const CECIvtrUpgradeEquipStone& s) : CECIvtrItem(s)
{
	m_pDBEssence	= s.m_pDBEssence;
}

CECIvtrUpgradeEquipStone::~CECIvtrUpgradeEquipStone()
{
}

//	Set item detail information
bool CECIvtrUpgradeEquipStone::SetItemInfo(BYTE* pInfoData, int iDataLen)
{
	CECIvtrItem::SetItemInfo(pInfoData, iDataLen);
	return true;
}

//	Get item icon file name
const char* CECIvtrUpgradeEquipStone::GetIconFile()
{
	return g_pGame->GetDataPath(m_pDBEssence->file_icon);
}

//	Get item name
const wchar_t* CECIvtrUpgradeEquipStone::GetName()
{
	return m_pDBEssence->name;
}

//	Get item cool time
int CECIvtrUpgradeEquipStone::GetCoolTime(int* piMax/* NULL */)
{
	return 0;
}

//	Get item description text
const wchar_t* CECIvtrUpgradeEquipStone::GetNormalDesc(bool bRepair)
{
	if (m_bNeedUpdate)
		return NULL;

	m_strDesc = _AL("");

	//	Try to build item description
	CECStringTab* pDescTab = g_pGame->GetItemDesc();
	int red = ITEMDESC_COL_RED;
	int white = ITEMDESC_COL_WHITE;
	int green = ITEMDESC_COL_GREEN;
	int name_color = DecideNameCol();
	CECHostPlayer* pHost = g_pGame->GetGameRun()->GetHostPlayer();
	if (m_iCount > 1)
		AddDescText(name_color, true, pDescTab->GetWideString(ITEMDESC_NAMENUMBER), GetName(), m_iCount);
	else
		AddDescText(name_color, true, pDescTab->GetWideString(ITEMDESC_NAME), GetName());

	AddStateDesc(pDescTab);
	AddExpireTimeDesc();
	//	Price
	AddPriceDesc(white, bRepair);

	AddExtDescText();
	return m_strDesc;
}

//	Check item use condition
bool CECIvtrUpgradeEquipStone::CheckUseCondition()
{
	if (!m_pDBEssence)
		return false;
	if (!CheckCrossUseCondition())
		return false;
	
	return true;
}

//	Get drop model for shown
const char * CECIvtrUpgradeEquipStone::GetDropModel()
{
	return g_pGame->GetDataPath(m_pDBEssence->file_matter);
}

///////////////////////////////////////////////////////////////////////////
//	
//	Implement CECIvtrBoothFigure
//	
///////////////////////////////////////////////////////////////////////////

CECIvtrBoothFigure::CECIvtrBoothFigure(int tid, int expire_date) : CECIvtrItem(tid, expire_date)
{
	m_iCID = ICID_BOOTH_FIGURE;
	
	//	Get database data
	elementdataman* pDB = g_pGame->GetElementDataMan();
	DATA_TYPE DataType;
	m_pDBEssence = (BOOTH_FIGURE_ITEM_ESSENCE*)pDB->get_data_ptr(tid, ID_SPACE_ESSENCE, DataType);
	
	m_iPileLimit	= m_pDBEssence->pile_num_max;
	m_iPrice		= m_pDBEssence->price;
	m_iShopPrice	= m_pDBEssence->shop_price;
	m_iProcType		= m_pDBEssence->proc_type;
	m_iEquipMask	= 0;
	m_bUseable		= true;
	m_dwUseFlags	= USE_PERSIST;
}

CECIvtrBoothFigure::CECIvtrBoothFigure(const CECIvtrBoothFigure &s) : CECIvtrItem(s)
{
	m_pDBEssence = s.m_pDBEssence;
}

CECIvtrBoothFigure::~CECIvtrBoothFigure()
{
}

//	Set item detail information
bool CECIvtrBoothFigure::SetItemInfo(BYTE* pInfoData, int iDataLen)
{
	CECIvtrItem::SetItemInfo(pInfoData, iDataLen);
	return true;
}

//	Get item icon file name
const char* CECIvtrBoothFigure::GetIconFile()
{
	return g_pGame->GetDataPath(m_pDBEssence->file_icon);
}

//	Get item name
const wchar_t* CECIvtrBoothFigure::GetName()
{
	return m_pDBEssence->name;
}

//	Get item cool time
int CECIvtrBoothFigure::GetCoolTime(int *piMax /*NULL*/)
{
	return 0;
}

//	Get item description text
const wchar_t* CECIvtrBoothFigure::GetNormalDesc(bool bRepair)
{
	if (m_bNeedUpdate)
		return NULL;

	m_strDesc = _AL("");

	//	Try to build item description
	CECStringTab* pDescTab = g_pGame->GetItemDesc();
	int red = ITEMDESC_COL_RED;
	int white = ITEMDESC_COL_WHITE;
	int name_color = DecideNameCol();

	if (m_iCount > 1)
		AddDescText(name_color, true, pDescTab->GetWideString(ITEMDESC_NAMENUMBER), GetName(), m_iCount);
	else
		AddDescText(name_color, true, pDescTab->GetWideString(ITEMDESC_NAME), GetName());

	AddStateDesc(pDescTab);
	AddExpireTimeDesc();

	//	Price
	AddPriceDesc(white, bRepair);

	//	Extend description
	AddExtDescText();

	return m_strDesc;
}

//	Get drop model for shown
const char* CECIvtrBoothFigure::GetDropModel()
{
	return g_pGame->GetDataPath(m_pDBEssence->file_matter);
}

///////////////////////////////////////////////////////////////////////////
//	
//	Implement CECIvtrFlagBuffItem
//	
///////////////////////////////////////////////////////////////////////////

CECIvtrFlagBuffItem::CECIvtrFlagBuffItem(int tid, int expire_date) : CECIvtrItem(tid, expire_date)
{
	m_iCID = ICID_FLAG_BUFF;
	
	//	Get database data
	elementdataman* pDB = g_pGame->GetElementDataMan();
	DATA_TYPE DataType;
	m_pDBEssence = (FLAG_BUFF_ITEM_ESSENCE*)pDB->get_data_ptr(tid, ID_SPACE_ESSENCE, DataType);
	
	m_iPileLimit	= m_pDBEssence->pile_num_max;
	m_iPrice		= m_pDBEssence->price;
	m_iShopPrice	= m_pDBEssence->shop_price;
	m_iProcType		= m_pDBEssence->proc_type;
	m_iEquipMask	= 0;
	m_bUseable		= true;
	m_dwUseFlags	= USE_PERSIST;
}

CECIvtrFlagBuffItem::CECIvtrFlagBuffItem(const CECIvtrFlagBuffItem &s) : CECIvtrItem(s)
{
	m_pDBEssence = s.m_pDBEssence;
}

CECIvtrFlagBuffItem::~CECIvtrFlagBuffItem()
{
}

//	Set item detail information
bool CECIvtrFlagBuffItem::SetItemInfo(BYTE* pInfoData, int iDataLen)
{
	CECIvtrItem::SetItemInfo(pInfoData, iDataLen);
	return true;
}

//	Get item icon file name
const char* CECIvtrFlagBuffItem::GetIconFile()
{
	return g_pGame->GetDataPath(m_pDBEssence->file_icon);
}

//	Get item name
const wchar_t* CECIvtrFlagBuffItem::GetName()
{
	return m_pDBEssence->name;
}

//	Get item cool time
int CECIvtrFlagBuffItem::GetCoolTime(int *piMax /*NULL*/)
{
	CECHostPlayer* pHost = g_pGame->GetGameRun()->GetHostPlayer();
	return pHost ? pHost->GetCoolTime(COOLDOWN_INDEX_BATTLE_FLAG, piMax) : 0;
}

//	Get item description text
const wchar_t* CECIvtrFlagBuffItem::GetNormalDesc(bool bRepair)
{
	if (m_bNeedUpdate)
		return NULL;
	
	m_strDesc = _AL("");
	
	//	Try to build item description
	CECStringTab* pDescTab = g_pGame->GetItemDesc();
	int red = ITEMDESC_COL_RED;
	int white = ITEMDESC_COL_WHITE;
	int name_color = DecideNameCol();
	
	if (m_iCount > 1)
		AddDescText(name_color, true, pDescTab->GetWideString(ITEMDESC_NAMENUMBER), GetName(), m_iCount);
	else
		AddDescText(name_color, true, pDescTab->GetWideString(ITEMDESC_NAME), GetName());

	AddStateDesc(pDescTab);
	AddExpireTimeDesc();
	
	//	Price
	AddPriceDesc(white, bRepair);
	
	//	Extend description
	AddExtDescText();
	
	return m_strDesc;
}

//	Get drop model for shown
const char* CECIvtrFlagBuffItem::GetDropModel()
{
	return g_pGame->GetDataPath(m_pDBEssence->file_matter);
}

///////////////////////////////////////////////////////////////////////////
//	
//	Implement CECIvtrAstrology
//	
///////////////////////////////////////////////////////////////////////////

CECIvtrAstrology::CECIvtrAstrology(int tid, int expire_date) : CECIvtrItem(tid, expire_date)
{
	m_iCID = ICID_ASTROLOGY;
	
	//	Get database data
	elementdataman* pDB = g_pGame->GetElementDataMan();
	DATA_TYPE DataType;
	m_pDBEssence = (XINGZUO_ITEM_ESSENCE*)pDB->get_data_ptr(tid, ID_SPACE_ESSENCE, DataType);

	memset(&m_Essence, 0, sizeof(m_Essence));
	m_Essence.level = -1; // 策划需求等级从0开始, 所以这里初始化为-1
	
	m_iPileLimit	= m_pDBEssence->pile_num_max;
	m_iPrice		= m_pDBEssence->price;
	m_iShopPrice	= m_pDBEssence->shop_price;
	m_iProcType		= m_pDBEssence->proc_type;
	m_iEquipMask	= 0;
	m_bUseable		= true;
	m_dwUseFlags	= USE_PERSIST;
}

CECIvtrAstrology::CECIvtrAstrology(const CECIvtrAstrology &s) : CECIvtrItem(s)
{
	m_pDBEssence = s.m_pDBEssence;
	m_Essence = s.m_Essence;
}

CECIvtrAstrology::~CECIvtrAstrology()
{
}

//	Set item detail information
bool CECIvtrAstrology::SetItemInfo(BYTE* pInfoData, int iDataLen)
{
	CECIvtrItem::SetItemInfo(pInfoData, iDataLen);

	if(iDataLen < sizeof(m_Essence))
		return false;
	
	memcpy(&m_Essence, pInfoData, sizeof(m_Essence));

	return true;
}

//	Get item icon file name
const char* CECIvtrAstrology::GetIconFile()
{
	return g_pGame->GetDataPath(m_pDBEssence->file_icon);
}

//	Get item name
const wchar_t* CECIvtrAstrology::GetName()
{
	return m_pDBEssence->name;
}

//	Get item cool time
int CECIvtrAstrology::GetCoolTime(int *piMax /*NULL*/)
{
	return 0;
}

//	Get item description text
const wchar_t* CECIvtrAstrology::GetNormalDesc(bool bRepair)
{
	if (m_bNeedUpdate)
		return NULL;
	
	m_strDesc = _AL("");
	
	//	Try to build item description
	CECStringTab* pDescTab = g_pGame->GetItemDesc();
	int red = ITEMDESC_COL_RED;
	int white = ITEMDESC_COL_WHITE;
	int name_color = DecideNameCol();
	
	if (m_iCount > 1)
		AddDescText(name_color, true, pDescTab->GetWideString(ITEMDESC_NAMENUMBER), GetName(), m_iCount);
	else
		AddDescText(name_color, true, pDescTab->GetWideString(ITEMDESC_NAME), GetName());

	//可镶嵌部位，目前限定在武器、帽子、衣服、鞋子上面，如果扩展，这里也需要修改，并且需要将部位名字抽取出来放到itemdesc.txt中
	if (m_pDBEssence->equip_mask & EQUIP_MASK_WEAPON)
		AddDescText(white, true, _AL("(%s)"), pDescTab->GetWideString(ITEMDESC_WEAPON));
	else if (m_pDBEssence->equip_mask & EQUIP_MASK_HEAD)
		AddDescText(white, true, _AL("(%s)"), pDescTab->GetWideString(ITEMDESC_HAT));
	else if (m_pDBEssence->equip_mask & EQUIP_MASK_BODY)
		AddDescText(white, true, _AL("(%s)"), pDescTab->GetWideString(ITEMDESC_CLOTHES));
	else if (m_pDBEssence->equip_mask & EQUIP_MASK_FOOT)
		AddDescText(white, true, _AL("(%s)"), pDescTab->GetWideString(ITEMDESC_SHOE));

	if (m_pDBEssence->reborn_count == 0)
		AddDescText(white, m_Essence.level<=0, pDescTab->GetWideString(ITEMDESC_ASTROLOGY_NOTFLY));
	else
		AddDescText(white, m_Essence.level<=0, pDescTab->GetWideString(ITEMDESC_ASTROLOGY_FLY));

	if (m_Essence.level >= 0)
	{
		if (m_Essence.level > 0)
			AddDescText(white, true, pDescTab->GetWideString(ITEMDESC_ASTROLOGY_LEVEL), m_Essence.level);
		AddDescText(white, true, pDescTab->GetWideString(ITEMDESC_ASTROLOGY_EQUIPLV_LIMIT), GetEquipLevelLimit(m_Essence.level));

		for (int i = 0; i < 6; ++ i)
		{
			if (m_Essence.prop[i].id != 0)
			{
				int value = m_Essence.prop[i].value;
				AddDescText(white, false, pDescTab->GetWideString(ITEMDESC_EQUIPADDON_61+m_Essence.prop[i].id));
				AddDescText(ITEMDESC_COL_GREEN, true,  _AL(" +%s"), 
					CECIvtrRune::GetPropString(m_Essence.prop[i].id, int(value+value*m_Essence.level*0.25f)));
			}
		}

		AddDescText(GetScoreColor(m_Essence.score), true, pDescTab->GetWideString(ITEMDESC_ASTROLOGY_SCORE), m_Essence.score);
	}

	AddStateDesc(pDescTab);
	AddExpireTimeDesc();
	
	//	Price
	AddPriceDesc(white, bRepair);
	
	//	Extend description
	AddExtDescText();
	
	return m_strDesc;
}

//	Get drop model for shown
const char* CECIvtrAstrology::GetDropModel()
{
	return g_pGame->GetDataPath(m_pDBEssence->file_matter);
}

// 颜色显示同元婴
int CECIvtrAstrology::GetScoreColor(int score)
{
	int n = 0;
	if (score <= 50)
		n = 0;
	else if (score <= 100)
		n = 1;
	else if (score <= 200)
		n = 2;
	else if (score <= 300)
		n = 3;
	else if (score <= 500)
		n = 4;
	else if (score <= 800)
		n = 5;
	else
		n = 6;

	return ITEMDESC_RUNECOLOR1 + n;
}

// 获取镶嵌要求的装备等级
int CECIvtrAstrology::GetEquipLevelLimit(int level)
{
	switch(level)
	{
	case 0:
	case 1:
	case 2:
		return 60;
	case 3:
	case 4:
		return 75;
	case 5:
	case 6:
		return 90;
	case 7:
	case 8:
		return 105;
	case 9:
		return 120;
	case 10:
		return 135;
	default:
		break;
	}
	return 135;
}


///////////////////////////////////////////////////////////////////////////
//	
//	Implement CECIvtrAstrologyEnergyItem
//	
///////////////////////////////////////////////////////////////////////////

CECIvtrAstrologyEnergyItem::CECIvtrAstrologyEnergyItem(int tid, int expire_date) : CECIvtrItem(tid, expire_date)
{
	m_iCID = ICID_ASTROLOGY_ENERGY_ITEM;
	
	//	Get database data
	elementdataman* pDB = g_pGame->GetElementDataMan();
	DATA_TYPE DataType;
	m_pDBEssence = (XINGZUO_ENERGY_ITEM_ESSENCE*)pDB->get_data_ptr(tid, ID_SPACE_ESSENCE, DataType);
	
	m_iPileLimit	= m_pDBEssence->pile_num_max;
	m_iPrice		= m_pDBEssence->price;
	m_iShopPrice	= m_pDBEssence->shop_price;
	m_iProcType		= m_pDBEssence->proc_type;
	m_iEquipMask	= 0;
	m_bUseable		= true;
	m_dwUseFlags	= USE_PERSIST;
}

CECIvtrAstrologyEnergyItem::CECIvtrAstrologyEnergyItem(const CECIvtrAstrologyEnergyItem &s) : CECIvtrItem(s)
{
	m_pDBEssence = s.m_pDBEssence;
}

CECIvtrAstrologyEnergyItem::~CECIvtrAstrologyEnergyItem()
{
}

//	Set item detail information
bool CECIvtrAstrologyEnergyItem::SetItemInfo(BYTE* pInfoData, int iDataLen)
{
	return CECIvtrItem::SetItemInfo(pInfoData, iDataLen);
}

//	Get item icon file name
const char* CECIvtrAstrologyEnergyItem::GetIconFile()
{
	return g_pGame->GetDataPath(m_pDBEssence->file_icon);
}

//	Get item name
const wchar_t* CECIvtrAstrologyEnergyItem::GetName()
{
	return m_pDBEssence->name;
}

//	Get item cool time
int CECIvtrAstrologyEnergyItem::GetCoolTime(int *piMax /*NULL*/)
{
	return 0;
}

//	Get item description text
const wchar_t* CECIvtrAstrologyEnergyItem::GetNormalDesc(bool bRepair)
{
	if (m_bNeedUpdate)
		return NULL;

	m_strDesc = _AL("");

	//	Try to build item description
	CECStringTab* pDescTab = g_pGame->GetItemDesc();
	int white = ITEMDESC_COL_WHITE;
	int name_color = DecideNameCol();
	CECHostPlayer* pHost = g_pGame->GetGameRun()->GetHostPlayer();

	if (m_iCount > 1)
		AddDescText(name_color, true, pDescTab->GetWideString(ITEMDESC_NAMENUMBER), GetName(), m_iCount);
	else
		AddDescText(name_color, true, pDescTab->GetWideString(ITEMDESC_NAME), GetName());

	AddStateDesc(pDescTab);
	AddExpireTimeDesc();

	AddDescText(white, true, pDescTab->GetWideString(ITEMDESC_ASTROLOGY_ENERGY_VALUE), m_pDBEssence->energy);

	//	Price
	AddPriceDesc(white, bRepair);

	//	Extend description
	AddExtDescText();

	return m_strDesc;
}

//	Get drop model for shown
const char* CECIvtrAstrologyEnergyItem::GetDropModel()
{
	return g_pGame->GetDataPath(m_pDBEssence->file_matter);
}



///////////////////////////////////////////////////////////////////////////
//	
//	Implement CECIvtrAgentCombinedItem
//	
///////////////////////////////////////////////////////////////////////////

CECIvtrAgentCombinedItem::CECIvtrAgentCombinedItem(int tid, int expire_date) : CECIvtrItem(tid, expire_date)
{
	m_iCID = ICID_AGENTCOMBINED;
	
	//	Get database data
	elementdataman* pDB = g_pGame->GetElementDataMan();
	DATA_TYPE DataType;
	m_pDBEssence = (CASH_MEDIC_MERGE_ITEM_ESSENCE*)pDB->get_data_ptr(tid, ID_SPACE_ESSENCE, DataType);
	
	m_iPileLimit	= m_pDBEssence->pile_num_max;
	m_iPrice		= m_pDBEssence->price;
	m_iShopPrice	= m_pDBEssence->shop_price;
	m_iProcType		= m_pDBEssence->proc_type;
	
	m_iLevelReq		= m_pDBEssence->require_level;
	m_iEquipMask	= 0;
	m_bUseable		= true;
}

CECIvtrAgentCombinedItem::CECIvtrAgentCombinedItem(const CECIvtrAgentCombinedItem &s) : CECIvtrItem(s)
{
	m_pDBEssence = s.m_pDBEssence;
	m_Essence		= s.m_Essence;
	m_iLevelReq		= s.m_iLevelReq;
}

CECIvtrAgentCombinedItem::~CECIvtrAgentCombinedItem()
{
}


//	Get item icon file name
const char* CECIvtrAgentCombinedItem::GetIconFile()
{
	return g_pGame->GetDataPath(m_pDBEssence->file_icon);
}

//	Get item name
const wchar_t* CECIvtrAgentCombinedItem::GetName()
{
	return m_pDBEssence->name;
}

int CECIvtrAgentCombinedItem::GetCoolTime(int* piMax)
{
	CECHostPlayer* pHost = g_pGame->GetGameRun()->GetHostPlayer();
	return pHost ? pHost->GetCoolTime(COOLDOWN_INDEX_BOTTLE_HP + m_pDBEssence->type, piMax) : 0;
}

//	Get item description text
const wchar_t* CECIvtrAgentCombinedItem::GetNormalDesc(bool bRepair)
{
	if (m_bNeedUpdate)
		return NULL;

	m_strDesc = _AL("");

	//	Try to build item description
	CECStringTab* pDescTab = g_pGame->GetItemDesc();
	int white = ITEMDESC_COL_WHITE;
	int red = ITEMDESC_COL_RED;
	int name_color = DecideNameCol();
	CECHostPlayer* pHost = g_pGame->GetGameRun()->GetHostPlayer();

	if (m_iCount > 1)
		AddDescText(name_color, true, pDescTab->GetWideString(ITEMDESC_NAMENUMBER), GetName(), m_iCount);
	else
		AddDescText(name_color, true, pDescTab->GetWideString(ITEMDESC_NAME), GetName());

	if (m_pDBEssence->renascence_count > 0)
	{
		int col = (pHost->GetRebornCount() >= m_pDBEssence->renascence_count) ? ITEMDESC_COL_WHITE : ITEMDESC_COL_RED;
		AddDescText(col, true, pDescTab->GetWideString(ITEMDESC_RENASCENCE_REQUIRED), pDescTab->GetWideString(ITEMDESC_RENASCENCE_1+m_pDBEssence->renascence_count-1));
	}
	
	//	Level requirement
	if (m_iLevelReq)
	{
		int col = g_pGame->GetGameRun()->GetHostPlayer()->GetBasicProps().iLevel >= m_iLevelReq ? white : red;
		AddDescText(col, true, pDescTab->GetWideString(ITEMDESC_LEVELREQ), m_iLevelReq);
	}
	AddStateDesc(pDescTab);
	AddExpireTimeDesc();
	if(GetDBEssence()->type == 0 || GetDBEssence()->type == 3)
	{
		AddDescText(name_color, true, pDescTab->GetWideString(ITEMDESC_HP_VALUE),m_Essence.capacity);
	}
	else if(GetDBEssence()->type == 1 || GetDBEssence()->type == 4)
	{
		AddDescText(name_color, true,pDescTab->GetWideString(ITEMDESC_MP_VALUE),m_Essence.capacity);
	}
	else if(GetDBEssence()->type == 2 || GetDBEssence()->type == 5)
	{
		AddDescText(name_color, true,pDescTab->GetWideString(ITEMDESC_YUANLI_VALUE),m_Essence.capacity);
	}	
	
	//	Price
	AddPriceDesc(white, bRepair);

	//	Extend description
	AddExtDescText();

	return m_strDesc;
}

//	Get drop model for shown
const char* CECIvtrAgentCombinedItem::GetDropModel()
{
	return g_pGame->GetDataPath(m_pDBEssence->file_matter);
}

//	Set item detail information
bool CECIvtrAgentCombinedItem::SetItemInfo(BYTE* pInfoData, int iDataLen)
{

	CECIvtrItem::SetItemInfo(pInfoData, iDataLen);

	if( iDataLen != sizeof(int) )
		return false;
	m_Essence.capacity = *(int *)pInfoData;

	return true;
}

//	Check item use condition
bool CECIvtrAgentCombinedItem::CheckUseCondition()
{
	CECHostPlayer* pHost = g_pGame->GetGameRun()->GetHostPlayer();

	if (pHost->GetBasicProps().iLevel < GetLevelRequirement())
		return false;
	
	if (pHost->GetRebornCount() < m_pDBEssence->renascence_count)	//增加对转生次数的判断(飞升) Added 2012-03-29.
	{
		return false;
	}
	if (!CheckCrossUseCondition())
		return false;

	return true;
}


///////////////////////////////////////////////////////////////////////////
//	
//	Implement CECSalePromotionItem
//	
///////////////////////////////////////////////////////////////////////////

CECSalePromotionItem::CECSalePromotionItem(int tid, int expire_date) : CECIvtrItem(tid, expire_date)
{
	m_iCID = ICID_SALE_PROMOTION_ITEM;
	
	//	Get database data
	elementdataman* pDB = g_pGame->GetElementDataMan();
	DATA_TYPE DataType;
	m_pDBEssence = (SALE_PROMOTION_ITEM_ESSENCE*)pDB->get_data_ptr(tid, ID_SPACE_ESSENCE, DataType);
	
	m_iPileLimit	= m_pDBEssence->pile_num_max;
	m_iPrice		= m_pDBEssence->price;
	m_iShopPrice	= m_pDBEssence->shop_price;
	m_iProcType		= m_pDBEssence->proc_type;
	m_iEquipMask	= 0;
	m_bUseable		= true;
	m_dwUseFlags	= USE_PERSIST;
}

CECSalePromotionItem::CECSalePromotionItem(const CECSalePromotionItem &s) : CECIvtrItem(s)
{
	m_pDBEssence = s.m_pDBEssence;
	m_Essence = s.m_Essence;
}

CECSalePromotionItem::~CECSalePromotionItem()
{
}

//	Set item detail information
bool CECSalePromotionItem::SetItemInfo(BYTE* pInfoData, int iDataLen)
{
	CECIvtrItem::SetItemInfo(pInfoData, iDataLen);

	if(iDataLen < sizeof(m_Essence))
		return false;
	
	memcpy(&m_Essence, pInfoData, sizeof(m_Essence));

	return true;
}

//	Get item icon file name
const char* CECSalePromotionItem::GetIconFile()
{
	return g_pGame->GetDataPath(m_pDBEssence->file_icon);
}

//	Get item name
const wchar_t* CECSalePromotionItem::GetName()
{
	return m_pDBEssence->name;
}

//	Get item cool time
int CECSalePromotionItem::GetCoolTime(int *piMax /*NULL*/)
{
	return 0;
}

//	Get item description text
const wchar_t* CECSalePromotionItem::GetNormalDesc(bool bRepair)
{
	if (m_bNeedUpdate)
		return NULL;

	m_strDesc = _AL("");

	//	Try to build item description
	CECStringTab* pDescTab = g_pGame->GetItemDesc();
	int white = ITEMDESC_COL_WHITE;
	int name_color = DecideNameCol();
	CECHostPlayer* pHost = g_pGame->GetGameRun()->GetHostPlayer();

	if (m_iCount > 1)
		AddDescText(name_color, true, pDescTab->GetWideString(ITEMDESC_NAMENUMBER), GetName(), m_iCount);
	else
		AddDescText(name_color, true, pDescTab->GetWideString(ITEMDESC_NAME), GetName());

	AddStateDesc(pDescTab);		
	AddExpireTimeDesc();

	//	Price
	AddPriceDesc(white, bRepair);

	//	Extend description
	AddExtDescText();

	return m_strDesc;
}

//	Get drop model for shown
const char* CECSalePromotionItem::GetDropModel()
{
	return g_pGame->GetDataPath(m_pDBEssence->file_matter);
}


///////////////////////////////////////////////////////////////////////////
//	
//	Implement CECIvtrGiftPack
//	
///////////////////////////////////////////////////////////////////////////

CECIvtrGiftPack::CECIvtrGiftPack(int tid, int expire_date) : CECIvtrItem(tid, expire_date)
{
	m_iCID = ICID_GIFT_PACK;
	
	//	Get database data
	elementdataman* pDB = g_pGame->GetElementDataMan();
	DATA_TYPE DataType;
	m_pDBEssence = (GIFT_PACK_ITEM_ESSENCE*)pDB->get_data_ptr(tid, ID_SPACE_ESSENCE, DataType);
	
	m_iPileLimit	= m_pDBEssence->pile_num_max;
	m_iPrice		= m_pDBEssence->price;
	m_iShopPrice	= m_pDBEssence->shop_price;
	m_iProcType		= m_pDBEssence->proc_type;
	m_bUseable		= true;
	m_dwUseFlags	= USE_PERSIST;
}

CECIvtrGiftPack::CECIvtrGiftPack(const CECIvtrGiftPack &s) : CECIvtrItem(s)
{
	m_pDBEssence = s.m_pDBEssence;
	m_Essence = s.m_Essence;
}

CECIvtrGiftPack::~CECIvtrGiftPack()
{
}

//	Set item detail information
bool CECIvtrGiftPack::SetItemInfo(BYTE* pInfoData, int iDataLen)
{
	CECIvtrItem::SetItemInfo(pInfoData, iDataLen);
	
	if(iDataLen < sizeof(m_Essence))
		return false;
	
	memcpy(&m_Essence, pInfoData, sizeof(m_Essence));

	return true;
}

//	Get item icon file name
const char* CECIvtrGiftPack::GetIconFile()
{
	return g_pGame->GetDataPath(m_pDBEssence->file_icon);
}

//	Get item name
const wchar_t* CECIvtrGiftPack::GetName()
{
	return m_pDBEssence->name;
}

//	Get drop model for shown
const char* CECIvtrGiftPack::GetDropModel()
{
	return g_pGame->GetDataPath(m_pDBEssence->file_matter);
}

//	Get item description text
const wchar_t* CECIvtrGiftPack::GetNormalDesc(bool bRepair)
{
	if (m_bNeedUpdate)
		return NULL;

	m_strDesc = _AL("");

	//	Try to build item description
	CECStringTab* pDescTab = g_pGame->GetItemDesc();
	int white = ITEMDESC_COL_WHITE;
	int name_color = DecideNameCol();
	CECHostPlayer* pHost = g_pGame->GetGameRun()->GetHostPlayer();

	if (m_iCount > 1)
		AddDescText(name_color, true, pDescTab->GetWideString(ITEMDESC_NAMENUMBER), GetName(), m_iCount);
	else
		AddDescText(name_color, true, pDescTab->GetWideString(ITEMDESC_NAME), GetName());

	AddDescText(white, true, pDescTab->GetWideString(ITEM_DESC_GIFT_PACK_SCORE), m_Essence.apoint);
	AddStateDesc(pDescTab);
	AddExpireTimeDesc();

	AddDescText(white, true, pDescTab->GetWideString(ITEM_DESC_GIFT_PACK_1), m_Essence.apoint/100000);
	AddDescText(white, true, pDescTab->GetWideString(ITEM_DESC_GIFT_PACK_2), (m_Essence.apoint%100000)/10000);
	AddDescText(white, true, pDescTab->GetWideString(ITEM_DESC_GIFT_PACK_3), (m_Essence.apoint%10000)/1000);
	AddDescText(white, true, pDescTab->GetWideString(ITEM_DESC_GIFT_PACK_4), (m_Essence.apoint%1000)/100);


	//	Price
	AddPriceDesc(white, bRepair);

	//	Extend description
	AddExtDescText();

	return m_strDesc;
}


///////////////////////////////////////////////////////////////////////////
//	
//	Implement CECIvtrPropAddMaterial
//	基础属性增益丹药合成材料
///////////////////////////////////////////////////////////////////////////

CECIvtrPropAddMaterial::CECIvtrPropAddMaterial(int tid, int expire_date) : CECIvtrItem(tid, expire_date)
{
	m_iCID = ICID_PROP_ADD_MATERIAL_ITEM;
	
	//	Get database data
	elementdataman* pDB = g_pGame->GetElementDataMan();
	DATA_TYPE DataType;
	m_pDBEssence = (PROP_ADD_MATERIAL_ITEM_ESSENCE*)pDB->get_data_ptr(tid, ID_SPACE_ESSENCE, DataType);
	
	m_iPileLimit	= m_pDBEssence->pile_num_max;
	m_iPrice		= m_pDBEssence->price;
	m_iShopPrice	= m_pDBEssence->shop_price;
	m_iProcType		= m_pDBEssence->proc_type;
	m_iEquipMask	= 0;
	m_bUseable		= true;
	m_dwUseFlags	= USE_PERSIST;
}

CECIvtrPropAddMaterial::CECIvtrPropAddMaterial(const CECIvtrPropAddMaterial &s) : CECIvtrItem(s)
{
	m_pDBEssence = s.m_pDBEssence;
}

CECIvtrPropAddMaterial::~CECIvtrPropAddMaterial()
{
}

//	Get item icon file name
const char* CECIvtrPropAddMaterial::GetIconFile()
{
	return g_pGame->GetDataPath(m_pDBEssence->file_icon);
}

//	Get item name
const wchar_t* CECIvtrPropAddMaterial::GetName()
{
	return m_pDBEssence->name;
}

//	Get item description text
const wchar_t* CECIvtrPropAddMaterial::GetNormalDesc(bool bRepair)
{
	if (m_bNeedUpdate)
		return NULL;

	m_strDesc = _AL("");

	//	Try to build item description
	CECStringTab* pDescTab = g_pGame->GetItemDesc();
	int white = ITEMDESC_COL_WHITE;
	int name_color = DecideNameCol();
	CECHostPlayer* pHost = g_pGame->GetGameRun()->GetHostPlayer();

	if (m_iCount > 1)
		AddDescText(name_color, true, pDescTab->GetWideString(ITEMDESC_NAMENUMBER), GetName(), m_iCount);
	else
		AddDescText(name_color, true, pDescTab->GetWideString(ITEMDESC_NAME), GetName());

	AddStateDesc(pDescTab);
	AddExpireTimeDesc();

	//	Price
	AddPriceDesc(white, bRepair);

	//	Extend description
	AddExtDescText();

	return m_strDesc;
}

//	Get drop model for shown
const char* CECIvtrPropAddMaterial::GetDropModel()
{
	return g_pGame->GetDataPath(m_pDBEssence->file_matter);
}


///////////////////////////////////////////////////////////////////////////
//	
//	Implement CECIvtrPropAddItem
//	基础属性增益丹药合成材料
///////////////////////////////////////////////////////////////////////////

CECIvtrPropAddItem::CECIvtrPropAddItem(int tid, int expire_date) : CECIvtrItem(tid, expire_date)
{
	m_iCID = ICID_PROP_ADD_ITEM;
	
	//	Get database data
	elementdataman* pDB = g_pGame->GetElementDataMan();
	DATA_TYPE DataType;
	m_pDBEssence = (PROP_ADD_ITEM_ESSENCE*)pDB->get_data_ptr(tid, ID_SPACE_ESSENCE, DataType);
	
	m_iPileLimit	= m_pDBEssence->pile_num_max;
	m_iPrice		= m_pDBEssence->price;
	m_iShopPrice	= m_pDBEssence->shop_price;
	m_iProcType		= m_pDBEssence->proc_type;
	m_iEquipMask	= 0;
	m_bUseable		= true;
	m_dwUseFlags	= USE_PERSIST;
}

CECIvtrPropAddItem::CECIvtrPropAddItem(const CECIvtrPropAddItem &s) : CECIvtrItem(s)
{
	m_pDBEssence = s.m_pDBEssence;
}

CECIvtrPropAddItem::~CECIvtrPropAddItem()
{
}

//	Get item icon file name
const char* CECIvtrPropAddItem::GetIconFile()
{
	return g_pGame->GetDataPath(m_pDBEssence->file_icon);
}

//	Get item name
const wchar_t* CECIvtrPropAddItem::GetName()
{
	return m_pDBEssence->name;
}

//	Get item description text
const wchar_t* CECIvtrPropAddItem::GetNormalDesc(bool bRepair)
{
	if (m_bNeedUpdate)
		return NULL;

	m_strDesc = _AL("");

	//	Try to build item description
	CECStringTab* pDescTab = g_pGame->GetItemDesc();
	int white = ITEMDESC_COL_WHITE;
	int name_color = DecideNameCol();
	CECHostPlayer* pHost = g_pGame->GetGameRun()->GetHostPlayer();

	if (m_iCount > 1)
		AddDescText(name_color, true, pDescTab->GetWideString(ITEMDESC_NAMENUMBER), GetName(), m_iCount);
	else
		AddDescText(name_color, true, pDescTab->GetWideString(ITEMDESC_NAME), GetName());

	AddStateDesc(pDescTab);
	AddExpireTimeDesc();
	
	// PROP_ADD_DAMAGE
	if (m_pDBEssence->prop_add[0]>0)
		AddDescText(ITEMDESC_COL_LIGHTBLUE, true, _AL("%s"), CECIvtrEquip::GetEquipAddonDesc(0, 3, m_pDBEssence->prop_add[0], 0, 0));
	
	// PROP_ADD_DEF
	if (m_pDBEssence->prop_add[1]>0)
		AddDescText(ITEMDESC_COL_LIGHTBLUE, true, _AL("%s"), CECIvtrEquip::GetEquipAddonDesc(0, 4, m_pDBEssence->prop_add[1], 0, 0));
	
	// PROP_ADD_HP
	if (m_pDBEssence->prop_add[2]>0)
		AddDescText(ITEMDESC_COL_LIGHTBLUE, true, _AL("%s"), CECIvtrEquip::GetEquipAddonDesc(0, 1, m_pDBEssence->prop_add[2], 0, 0));
	
	// PROP_ADD_MP
	if (m_pDBEssence->prop_add[3]>0)
		AddDescText(ITEMDESC_COL_LIGHTBLUE, true, _AL("%s"), CECIvtrEquip::GetEquipAddonDesc(0, 2, m_pDBEssence->prop_add[3], 0, 0));
	
	// PROP_ADD_ATTACK
	if (m_pDBEssence->prop_add[4]>0)
		AddDescText(ITEMDESC_COL_LIGHTBLUE, true, _AL("%s"), CECIvtrEquip::GetEquipAddonDesc(0, 5, m_pDBEssence->prop_add[4], 0, 0));
	
	// PROP_ADD_ARMOR
	if (m_pDBEssence->prop_add[5]>0)
		AddDescText(ITEMDESC_COL_LIGHTBLUE, true, _AL("%s"), CECIvtrEquip::GetEquipAddonDesc(0, 6, m_pDBEssence->prop_add[5], 0, 0));
	
	// PROP_ADD_CRIT_RATE
	if (m_pDBEssence->prop_add[6]>0)
	{
		float value = float(m_pDBEssence->prop_add[6])/1000.f;
		AddDescText(ITEMDESC_COL_LIGHTBLUE, true, _AL("%s"), CECIvtrEquip::GetEquipAddonDesc(0, 7, *(int*)&value, 0, 0));
	}
	

	// PROP_ADD_CRIT_ANTI
	if (m_pDBEssence->prop_add[7]>0)
	{
		float value = float(m_pDBEssence->prop_add[7])/1000.f;
		AddDescText(ITEMDESC_COL_LIGHTBLUE, true, _AL("%s"), CECIvtrEquip::GetEquipAddonDesc(0, 41, *(int*)&value, 0, 0));
	}
	
	// PROP_ADD_CRIT_DAMAGE
	if (m_pDBEssence->prop_add[8]>0)
	{
		float value = float(m_pDBEssence->prop_add[8])/1000.f;
		AddDescText(ITEMDESC_COL_LIGHTBLUE, true, _AL("%s"), CECIvtrEquip::GetEquipAddonDesc(0, 8, *(int*)&value, 0, 0));
	}
	
	// PROP_ADD_CRIT_DAMAGE_ANTI
	if (m_pDBEssence->prop_add[9]>0)
	{
		float value = float(m_pDBEssence->prop_add[9])/1000.f;
		AddDescText(ITEMDESC_COL_LIGHTBLUE, true, _AL("%s"), CECIvtrEquip::GetEquipAddonDesc(0, 42, *(int*)&value, 0, 0));
	}
	
	// PROP_ADD_SKILL_ATTACK_RATE
	if (m_pDBEssence->prop_add[10]>0)
	{
		float value = float(m_pDBEssence->prop_add[10])/1000.f;
		AddDescText(ITEMDESC_COL_LIGHTBLUE, true, _AL("%s"), CECIvtrEquip::GetEquipAddonDesc(0, 44, *(int*)&value, 0, 0));
	}
	
	// PROP_ADD_SKILL_ARMOR_RATE
	if (m_pDBEssence->prop_add[11]>0)
	{
		float value = float(m_pDBEssence->prop_add[11])/1000.f;
		AddDescText(ITEMDESC_COL_LIGHTBLUE, true, _AL("%s"), CECIvtrEquip::GetEquipAddonDesc(0, 43, *(int*)&value, 0, 0));
	}

	//	Price
	AddPriceDesc(white, bRepair);

	//	Extend description
	AddExtDescText();

	return m_strDesc;
}

//	Get drop model for shown
const char* CECIvtrPropAddItem::GetDropModel()
{
	return g_pGame->GetDataPath(m_pDBEssence->file_matter);
}

///////////////////////////////////////////////////////////////////////////
//	
//	Implement CECIvtrPropAddMaterial
//	基础属性增益丹药合成材料
///////////////////////////////////////////////////////////////////////////

CECIvtrRune2013::CECIvtrRune2013(int tid, int expire_date) : CECIvtrItem(tid, expire_date)
{
	m_iCID = ICID_RUNE2013_ITEM;
	
	//	Get database data
	elementdataman* pDB = g_pGame->GetElementDataMan();
	DATA_TYPE DataType;
	m_pDBEssence = (RUNE2013_ITEM_ESSENCE*)pDB->get_data_ptr(tid, ID_SPACE_ESSENCE, DataType);
	
	m_iPileLimit	= m_pDBEssence->pile_num_max;
	m_iPrice		= m_pDBEssence->price;
	m_iShopPrice	= m_pDBEssence->shop_price;
	m_iProcType		= m_pDBEssence->proc_type;
	m_iEquipMask	= 0;
	m_bUseable		= true;
	m_dwUseFlags	= USE_PERSIST;

	memset(&m_Essence, 0, sizeof(m_Essence));
}

CECIvtrRune2013::CECIvtrRune2013(const CECIvtrRune2013 &s) : CECIvtrItem(s)
{
	m_pDBEssence = s.m_pDBEssence;
	m_Essence = s.m_Essence;
}

CECIvtrRune2013::~CECIvtrRune2013()
{
}

//	Get item icon file name
const char* CECIvtrRune2013::GetIconFile()
{
	return g_pGame->GetDataPath(m_pDBEssence->file_icon);
}

//	Get item name
const wchar_t* CECIvtrRune2013::GetName()
{
	return m_pDBEssence->name;
}

//	Get item description text
const wchar_t* CECIvtrRune2013::GetNormalDesc(bool bRepair)
{
	if (m_bNeedUpdate)
		return NULL;

	m_strDesc = _AL("");

	//	Try to build item description
	CECStringTab* pDescTab = g_pGame->GetItemDesc();
	int white = ITEMDESC_COL_WHITE;
	int name_color = DecideNameCol();
	CECHostPlayer* pHost = g_pGame->GetGameRun()->GetHostPlayer();

	if (m_iCount > 1)
		AddDescText(name_color, true, pDescTab->GetWideString(ITEMDESC_NAMENUMBER), GetName(), m_iCount);
	else
		AddDescText(name_color, true, pDescTab->GetWideString(ITEMDESC_NAME), GetName());

	AddStateDesc(pDescTab);
	AddExpireTimeDesc();

	AddDescText(white, false, pDescTab->GetWideString(ITEMDESC_ASTROLOGY_LEVEL), GetLevel());
	m_strDesc += _AL("    ");

	int nNextLevelExp = GetNextLevelExp();
	if (nNextLevelExp >= 0)
		AddDescText(white, true, pDescTab->GetWideString(ITEMDESC_RUNE2013EXP), min(GetMaxLevelExp(), m_Essence.exp), nNextLevelExp);
	else
		AddDescText(white, true, pDescTab->GetWideString(ITEMDESC_RUNE2013EXP2), min(GetMaxLevelExp(), m_Essence.exp));
	
	AddDescText(name_color, true, pDescTab->GetWideString(ITEMDESC_PLAYERPROPERTY1+m_pDBEssence->prop_type-PROP_TYPE_DAMAGE), GetPropString());

	//	Price
	AddPriceDesc(white, bRepair);

	//	Extend description
	AddExtDescText();

	return m_strDesc;
}

//	Get drop model for shown
const char* CECIvtrRune2013::GetDropModel()
{
	return g_pGame->GetDataPath(m_pDBEssence->file_matter);
}

//	Set item detail information
bool CECIvtrRune2013::SetItemInfo(BYTE* pInfoData, int iDataLen)
{
	CECIvtrItem::SetItemInfo(pInfoData, iDataLen);
	
	if(iDataLen < sizeof(m_Essence))
		return false;
	
	memcpy(&m_Essence, pInfoData, sizeof(m_Essence));

	return true;
}

// 当前等级
int CECIvtrRune2013::GetLevel()
{
	int level = 0;
	elementdataman* pDB = g_pGame->GetElementDataMan();
	DATA_TYPE DataType;
	RUNE2013_CONFIG* pCfg = (RUNE2013_CONFIG*)pDB->get_data_ptr(753, ID_SPACE_CONFIG, DataType);
	if (pCfg || DataType == DT_RUNE2013_CONFIG)
	{
		int quality = m_pDBEssence->quality;
		int exp = m_Essence.exp;
		
		for (int i = 0; i < 10; ++ i)
		{
			if (pCfg->lvup_exp[quality][i] > exp)
			{
				break;
			}
			if (i < 10)
				level++;
		}
	}
	return level;
}

// 获取下一级经验, -1表示已满
int CECIvtrRune2013::GetNextLevelExp()
{
	elementdataman* pDB = g_pGame->GetElementDataMan();
	DATA_TYPE DataType;
	RUNE2013_CONFIG* pCfg = (RUNE2013_CONFIG*)pDB->get_data_ptr(753, ID_SPACE_CONFIG, DataType);
	if (pCfg || DataType == DT_RUNE2013_CONFIG)
	{
		int quality = m_pDBEssence->quality;
		
		for (int i = 0; i < 10; ++ i)
		{
			if (pCfg->lvup_exp[quality][i] > m_Essence.exp)
			{
				return pCfg->lvup_exp[quality][i];
			}
		}
	}
	return -1;
}

// 获取最高级别经验
int CECIvtrRune2013::GetMaxLevelExp()
{
	elementdataman* pDB = g_pGame->GetElementDataMan();
	DATA_TYPE DataType;
	RUNE2013_CONFIG* pCfg = (RUNE2013_CONFIG*)pDB->get_data_ptr(753, ID_SPACE_CONFIG, DataType);
	if (pCfg || DataType == DT_RUNE2013_CONFIG)
	{
		return pCfg->lvup_exp[m_pDBEssence->quality][9];
	}
	return 0;
}

// 获取属性字符串
ACString CECIvtrRune2013::GetPropString()
{
	int level = GetLevel(); 

	int add_value = 0; 
	float add_value2 = 0;	//千分数 (暴击率)
	//float add_value3 = 0.f;	//浮点数 (暴击伤害)
	add_value = int(m_pDBEssence->add_value * level  + 0.1);
	add_value2 = m_pDBEssence->add_value * level;
	//add_value3 = m_pDBEssence->add_value * level;

	ACString szText;

	switch(m_pDBEssence->prop_type)
	{
		case PROP_TYPE_DAMAGE:
		case PROP_TYPE_DEF:
		case PROP_TYPE_HP:
		case PROP_TYPE_MP:
		case PROP_TYPE_ATTACK:
		case PROP_TYPE_ARMOR:
		case PROP_TYPE_RESISTANCE_1:
		case PROP_TYPE_RESISTANCE_2:
		case PROP_TYPE_RESISTANCE_3:
		case PROP_TYPE_RESISTANCE_4:
		case PROP_TYPE_RESISTANCE_5:
		case PROP_TYPE_CULT_DEFANCE_1:
		case PROP_TYPE_CULT_DEFANCE_2:
		case PROP_TYPE_CULT_DEFANCE_3:
		case PROP_TYPE_CULT_ATTACK_1:
		case PROP_TYPE_CULT_ATTACK_2:
		case PROP_TYPE_CULT_ATTACK_3:
		case PROP_TYPE_SKILLED_1:
		case PROP_TYPE_SKILLED_2:
		case PROP_TYPE_SKILLED_3:
		case PROP_TYPE_SKILLED_4:
		case PROP_TYPE_SKILLED_5:
		case PROP_TYPE_TOUGHNESS_1:
		case PROP_TYPE_TOUGHNESS_2:
		case PROP_TYPE_TOUGHNESS_3:
		case PROP_TYPE_TOUGHNESS_4:
		case PROP_TYPE_TOUGHNESS_5:
			szText.Format(_AL("%d"), add_value);
			break;
		case PROP_TYPE_CRIT_RATE:
		case PROP_TYPE_CRIT_ANTI:
			szText.Format(_AL("%.2f%%"), add_value2);
			break;
		case PROP_TYPE_CRIT_DAMAGE:
		case PROP_TYPE_CRIT_DAMAGE_ANTI:
		case PROP_TYPE_SKILL_ATTACK_RATE:
		case PROP_TYPE_SKILL_ARMOR_RATE:
		case PROP_TYPE_MOVE_SPEED:
		case PROP_TYPE_FLY_SPEED:
			szText.Format(_AL("%.2f"), add_value2);
			break;
		default:
			break;
	}

	return szText;
}




///////////////////////////////////////////////////////////////////////////
//	
//	Implement CECIvtrIndividualityItem
//	
///////////////////////////////////////////////////////////////////////////

CECIvtrIndividualityItem::CECIvtrIndividualityItem(int tid, int expire_date) : CECIvtrItem(tid, expire_date)
{
	m_iCID = ICID_INDIVIDUALITY_ITEM;

	//	Get database data
	elementdataman* pDB = g_pGame->GetElementDataMan();
	DATA_TYPE DataType;
	m_pDBEssence = (COLORANT_ITEM_ESSENCE*)pDB->get_data_ptr(tid, ID_SPACE_ESSENCE, DataType);

	m_iPileLimit	= m_pDBEssence->pile_num_max;
	m_iPrice		= m_pDBEssence->price;
	m_iShopPrice	= m_pDBEssence->shop_price;
	m_iProcType		= m_pDBEssence->proc_type;

	m_iEquipMask	= 0;
	m_bUseable		= true;
}

CECIvtrIndividualityItem::CECIvtrIndividualityItem(const CECIvtrIndividualityItem &s) : CECIvtrItem(s)
{
	m_pDBEssence = s.m_pDBEssence;
}

CECIvtrIndividualityItem::~CECIvtrIndividualityItem()
{
}


//	Get item icon file name
const char* CECIvtrIndividualityItem::GetIconFile()
{
	return g_pGame->GetDataPath(m_pDBEssence->file_icon);
}

//	Get item name
const wchar_t* CECIvtrIndividualityItem::GetName()
{
	return m_pDBEssence->name;
}


//	Get item description text
const wchar_t* CECIvtrIndividualityItem::GetNormalDesc(bool bRepair)
{
	if (m_bNeedUpdate)
		return NULL;

	m_strDesc = _AL("");

	//	Try to build item description
	CECStringTab* pDescTab = g_pGame->GetItemDesc();
	int white = ITEMDESC_COL_WHITE;
	int red = ITEMDESC_COL_RED;
	int name_color = DecideNameCol();
	//CECHostPlayer* pHost = g_pGame->GetGameRun()->GetHostPlayer();

	if (m_iCount > 1)
		AddDescText(name_color, true, pDescTab->GetWideString(ITEMDESC_NAMENUMBER), GetName(), m_iCount);
	else
		AddDescText(name_color, true, pDescTab->GetWideString(ITEMDESC_NAME), GetName());

	AddStateDesc(pDescTab);
	AddExpireTimeDesc();
	//	Price
	AddPriceDesc(white, bRepair);

	//	Extend description
	AddExtDescText();

	return m_strDesc;
}

//	Get drop model for shown
const char* CECIvtrIndividualityItem::GetDropModel()
{
	return g_pGame->GetDataPath(m_pDBEssence->file_matter);
}

//	Set item detail information
bool CECIvtrIndividualityItem::SetItemInfo(BYTE* pInfoData, int iDataLen)
{

	if( iDataLen != sizeof(int) )
		return false;
	//m_Essence.capacity = *(int *)pInfoData;


	CECIvtrItem::SetItemInfo(pInfoData, iDataLen);
	return true;
}

//	Check item use condition
bool CECIvtrIndividualityItem::CheckUseCondition()
{
	CECHostPlayer* pHost = g_pGame->GetGameRun()->GetHostPlayer();

	if (!CheckCrossUseCondition())
		return false;

	return true;
}
