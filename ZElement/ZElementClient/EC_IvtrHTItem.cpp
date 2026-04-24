/*
 * FILE: EC_IvtrHTItem.cpp
 *
 * DESCRIPTION: 
 *
 * CREATED BY: Lihengju, 2009/8/29
 *
 * HISTORY: 
 *
 * Copyright (c) 2005 Archosaur Studio, All Rights Reserved.
 */

#include "EC_Global.h"
#include "EC_IvtrHTItem.h"
#ifndef _INVENTORY_DESC_MAN //Added by Ford.W 2010-07-22.
#include "EC_Game.h"
#else
#include "EC_Game_HintTool.h"
#endif
#include "EC_FixedMsg.h"
#include "hometowntemplate.h"
#include "EC_HTPDataType.h"

#define new A_DEBUG_NEW

using namespace HOMETOWN;
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
//	Implement CECIvtrHTTool
//	
///////////////////////////////////////////////////////////////////////////

CECIvtrHTTool::CECIvtrHTTool(int tid) : CECIvtrItem(tid, 0)
{
	m_iCID	= ICID_HOMETOWN_TOOL;

	//	Get database data
	HometownTemplate* pDB = g_pGame->GetHomeTownDataMan();
	HOMETOWN::DATA_TYPE DataType;
	m_pDBEssence	= (TOOL_ESSENCE*)pDB->get_data_ptr(tid, HOMETOWN::ID_SPACE_ESSENCE, DataType, (const HOMETOWN::TOOL_ESSENCE *&)m_pDBEssence);
	
	m_iPileLimit	= m_pDBEssence->pile_limit;
//	m_iPrice		= m_pDBEssence->price;
//	m_iShopPrice	= m_pDBEssence->shop_price;
//	m_iProcType		= m_pDBEssence->proc_type;
//	m_iEquipMask	= 0;

	m_bNeedUpdate	= false;
}

CECIvtrHTTool::CECIvtrHTTool(const CECIvtrHTTool& s) : CECIvtrItem(s)
{
	m_pDBEssence	= s.m_pDBEssence;
}

CECIvtrHTTool::~CECIvtrHTTool()
{
}

//	Set item detail information
bool CECIvtrHTTool::SetItemInfo(BYTE* pInfoData, int iDataLen)
{
	CECIvtrItem::SetItemInfo(pInfoData, iDataLen);
	return true;
}

// 	Get item icon file name
const char* CECIvtrHTTool::GetIconFile()
{
	return g_pGame->GetHomeTownIconPath()->GetANSIString(m_pDBEssence->icon);
}

//	Get item name
const wchar_t* CECIvtrHTTool::GetName()
{
	return m_pDBEssence->name;
}

int CECIvtrHTTool::GetTargetType()
{
	int i=0;
	bool bWholeFarm = false;
	HometownTemplate* pDB = g_pGame->GetHomeTownDataMan();
	HOMETOWN::DATA_TYPE DataType;
	const HOMETOWN::TOOL_FUNC* tf;
	tf = (HOMETOWN::TOOL_FUNC*)pDB->get_data_ptr(m_pDBEssence->func_ids[0], HOMETOWN::ID_SPACE_FUNC, DataType, tf);
	assert(DataType==DT_TOOL_FUNC);

	if (tf->type==TFT_FARM_HIDE)
		return HOMETOWN::FARM_TARGET_SELF;
	else if (tf->type==TFT_FARM_SPEEDGROW || tf->type==TFT_FARM_PROTECT)
		return HOMETOWN::FARM_TARGET_FARM;
	else if (tf->type==TFT_FIELD_PROTECT || tf->type==TFT_FIELD_SPEEDGROW)
		return HOMETOWN::FARM_TARGET_FIELD;
	assert(0);
	return 	-1;
}

//	Get item description text
const wchar_t* CECIvtrHTTool::GetNormalDesc(bool bRepair)
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

	const wchar_t* szExtDesc = g_pGame->GetHomeTownDesc()->GetWideString(m_tid);

	m_strDesc += _AL("\\r");
	if(szExtDesc)
		AddDescText(white, true, szExtDesc);
	ACString strText;
	AddDescText(white, true, pDescTab->GetWideString(ITEMDESC_LEVELREQ), m_pDBEssence->required_level);
	AddDescText(white, true, pDescTab->GetWideString(ITEMDESC_GARDEN_TOOL_CHARMREQ), m_pDBEssence->charm_price);
	AddDescText(white, true, pDescTab->GetWideString(ITEMDESC_GARDEN_TOOL_MONEYREQ), m_pDBEssence->money_price);
//	strText.Format(_AL("%d"), m_pDBEssence->price);
	//	Price
//	AddPriceDesc(white, bRepair);


	return m_strDesc;
}


///////////////////////////////////////////////////////////////////////////
//	
//	Implement CECIvtrHTFruit
//	
///////////////////////////////////////////////////////////////////////////

CECIvtrHTFruit::CECIvtrHTFruit(int tid) : CECIvtrItem(tid, 0)
{
	m_iCID	= ICID_HOMETOWN_FRUIT;

	//	Get database data
	HometownTemplate* pDB = g_pGame->GetHomeTownDataMan();
	HOMETOWN::DATA_TYPE DataType;
	m_pDBEssence	= (FRUIT_ESSENCE*)pDB->get_data_ptr(tid, HOMETOWN::ID_SPACE_ESSENCE, DataType, (const HOMETOWN::FRUIT_ESSENCE *&)m_pDBEssence);
	
	m_iPileLimit	= m_pDBEssence->pile_limit;
//	m_iPrice		= m_pDBEssence->price;
//	m_iShopPrice	= m_pDBEssence->shop_price;
//	m_iProcType		= m_pDBEssence->proc_type;
//	m_iEquipMask	= 0;

	m_bNeedUpdate	= false;
}

CECIvtrHTFruit::CECIvtrHTFruit(const CECIvtrHTFruit& s) : CECIvtrItem(s)
{
	m_pDBEssence	= s.m_pDBEssence;
}

CECIvtrHTFruit::~CECIvtrHTFruit()
{
}

//	Set item detail information
bool CECIvtrHTFruit::SetItemInfo(BYTE* pInfoData, int iDataLen)
{
	CECIvtrItem::SetItemInfo(pInfoData, iDataLen);
	return true;
}

//	Get item icon file name
const char* CECIvtrHTFruit::GetIconFile()
{
	return g_pGame->GetHomeTownIconPath()->GetANSIString(m_pDBEssence->icon);
}

//	Get item name
const wchar_t* CECIvtrHTFruit::GetName()
{
	return m_pDBEssence->name;
}

//	Get item description text
const wchar_t* CECIvtrHTFruit::GetNormalDesc(bool bRepair)
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

	const wchar_t* szExtDesc = g_pGame->GetHomeTownDesc()->GetWideString(m_tid);
	
	m_strDesc += _AL("\\r");
	if(szExtDesc)
		AddDescText(white, true, szExtDesc);
	ACString strText;
	strText.Format(_AL("%d"), m_pDBEssence->price);
	AddDescText(white, true, pDescTab->GetWideString(ITEMDESC_PRICE), strText);
	//	Price
//	AddPriceDesc(white, bRepair);


	return m_strDesc;
}


///////////////////////////////////////////////////////////////////////////
//	
//	Implement CECIvtrHTCrop
//	
///////////////////////////////////////////////////////////////////////////

CECIvtrHTCrop::CECIvtrHTCrop(int tid) : CECIvtrItem(tid, 0)
{
	m_iCID	= ICID_HOMETOWN_FRUIT;
	
	//	Get database data
	HometownTemplate* pDB = g_pGame->GetHomeTownDataMan();
	HOMETOWN::DATA_TYPE DataType;
	m_pDBEssence	= (CROP_ESSENCE*)pDB->get_data_ptr(tid, HOMETOWN::ID_SPACE_ESSENCE, DataType, (const HOMETOWN::CROP_ESSENCE *&)m_pDBEssence);
	
	m_iPileLimit	= 0;
	//	m_iPrice		= m_pDBEssence->price;
	//	m_iShopPrice	= m_pDBEssence->shop_price;
	//	m_iProcType		= m_pDBEssence->proc_type;
	//	m_iEquipMask	= 0;
	
	m_bNeedUpdate	= false;
}

CECIvtrHTCrop::CECIvtrHTCrop(const CECIvtrHTCrop& s) : CECIvtrItem(s)
{
	m_pDBEssence	= s.m_pDBEssence;
}

CECIvtrHTCrop::~CECIvtrHTCrop()
{
}

//	Set item detail information
bool CECIvtrHTCrop::SetItemInfo(BYTE* pInfoData, int iDataLen)
{
	CECIvtrItem::SetItemInfo(pInfoData, iDataLen);
	return true;
}

//	Get item icon file name
const char* CECIvtrHTCrop::GetIconFile()
{
	return g_pGame->GetHomeTownIconPath()->GetANSIString(m_pDBEssence->icon_seed);
}

//	Get item name
const wchar_t* CECIvtrHTCrop::GetName()
{
	return m_pDBEssence->name;
}

//	Get item description text
const wchar_t* CECIvtrHTCrop::GetNormalDesc(bool bRepair)
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
	
	const wchar_t* szExtDesc = g_pGame->GetHomeTownDesc()->GetWideString(m_tid);
	
	m_strDesc += _AL("\\r");
	if(szExtDesc)
		m_strDesc += szExtDesc;

	if (m_pDBEssence->type == 0)
		AddDescText(white, true, pDescTab->GetWideString(ITEMDESC_GARDEN_CROP_USUAL));
	else
		AddDescText(white, true, pDescTab->GetWideString(ITEMDESC_GARDEN_CROP_FINE));
	AddDescText(white, true, pDescTab->GetWideString(ITEMDESC_LEVELREQ), m_pDBEssence->required_level);
	AddDescText(white, true, pDescTab->GetWideString(ITEMDESC_GARDEN_CROP_FRUITNUM), m_pDBEssence->fruit_min, m_pDBEssence->fruit_max);
	ACString strText;
	strText.Format(_AL("%d"),m_pDBEssence->seed_price);
	AddDescText(white, true, pDescTab->GetWideString(ITEMDESC_PRICE), strText);
	if (m_pDBEssence->grow_period>60*60*24)
		AddDescText(white, true, pDescTab->GetWideString(ITEMDESC_GARDEN_CROP_GROWPERIODDAY), (m_pDBEssence->grow_period+60*60*24-1)/(60*60*24));
	else
		AddDescText(white, true, pDescTab->GetWideString(ITEMDESC_GARDEN_CROP_GROWPERIODHOUR), (m_pDBEssence->grow_period+60*60-1)/(60*60));
	
	if (m_pDBEssence->protect_period>60*60)
		AddDescText(white, true, pDescTab->GetWideString(ITEMDESC_GARDEN_CROP_PROTECTPERIODHOUR), (m_pDBEssence->protect_period+60*60-1)/(60*60));
	else
		AddDescText(white, true, pDescTab->GetWideString(ITEMDESC_GARDEN_CROP_PROTECTPERIODMIN), (m_pDBEssence->protect_period+60-1)/(60));
	AddDescText(white, true, pDescTab->GetWideString(ITEMDESC_GARDEN_CROP_STEALLIMIT), m_pDBEssence->steal_limit);

	return m_strDesc;
}
