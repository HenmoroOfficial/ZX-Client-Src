/*
 * FILE: EC_IvtrScroll.cpp
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
#include "EC_IvtrScroll.h"
#include "EC_FixedMsg.h"
#ifndef _INVENTORY_DESC_MAN //Added by Ford.W 2010-07-22.
#include "EC_Game.h"
#include "EC_GameRun.h"
#include "EC_HostPlayer.h"
#else
#include "EC_Game_HintTool.h"
#include "EC_GameRun_HintTool.h"
#include "EC_HostPlayer_HintTool.h"
#endif
//#include "EC_ManNPC.h"
//#include "EC_Monster.h"
#include "EC_World.h"
#include "EC_Instance.h"
#include "EL_Precinct.h"
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
//	Implement CECIvtrSkillTome
//	
///////////////////////////////////////////////////////////////////////////

CECIvtrSkillTome::CECIvtrSkillTome(int tid, int expire_date) : CECIvtrItem(tid, expire_date)
{
	m_iCID	= ICID_SKILLTOME;

	//	Get database data
	elementdataman* pDB = g_pGame->GetElementDataMan();
	DATA_TYPE DataType;
	m_pDBEssence	= (SKILLTOME_ESSENCE*)pDB->get_data_ptr(tid, ID_SPACE_ESSENCE, DataType);
	m_pDBSubType	= (SKILLTOME_SUB_TYPE*)pDB->get_data_ptr(m_pDBEssence->id_sub_type, ID_SPACE_ESSENCE, DataType);

	m_iPileLimit	= m_pDBEssence->pile_num_max;
	m_iPrice		= m_pDBEssence->price;
	m_iShopPrice	= m_pDBEssence->shop_price;
	m_iProcType		= m_pDBEssence->proc_type;
	m_iEquipMask	= 0;

	m_bNeedUpdate	= false;
}

CECIvtrSkillTome::CECIvtrSkillTome(const CECIvtrSkillTome& s) : CECIvtrItem(s)
{
	m_pDBEssence	= s.m_pDBEssence;
	m_pDBSubType	= s.m_pDBSubType;
}

CECIvtrSkillTome::~CECIvtrSkillTome()
{
}

//	Set item detail information
bool CECIvtrSkillTome::SetItemInfo(BYTE* pInfoData, int iDataLen)
{
	CECIvtrItem::SetItemInfo(pInfoData, iDataLen);
	return true;
}

//	Get item icon file name
const char* CECIvtrSkillTome::GetIconFile()
{
	return g_pGame->GetDataPath(m_pDBEssence->file_icon);
}

//	Get item name
const wchar_t* CECIvtrSkillTome::GetName()
{
	return m_pDBEssence->name;
}

//	Get item description text
const wchar_t* CECIvtrSkillTome::GetNormalDesc(bool bRepair)
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

	//	Price
	AddPriceDesc(white, bRepair);

	//	Extend description
	AddExtDescText();

	return m_strDesc;
}

//	Get drop model for shown
const char * CECIvtrSkillTome::GetDropModel()
{
	return g_pGame->GetDataPath(m_pDBEssence->file_matter);
}

///////////////////////////////////////////////////////////////////////////
//	
//	Implement CECIvtrTownScroll
//	
///////////////////////////////////////////////////////////////////////////

CECIvtrTownScroll::CECIvtrTownScroll(int tid, int expire_date) : CECIvtrItem(tid, expire_date)
{
	m_iCID	= ICID_TOWNSCROLL;

	//	Get database data
	elementdataman* pDB = g_pGame->GetElementDataMan();
	DATA_TYPE DataType;
	m_pDBEssence = (TOWNSCROLL_ESSENCE*)pDB->get_data_ptr(tid, ID_SPACE_ESSENCE, DataType);

	m_iPileLimit	= m_pDBEssence->pile_num_max;
	m_iPrice		= m_pDBEssence->price;
	m_iShopPrice	= m_pDBEssence->shop_price;
	m_iProcType		= m_pDBEssence->proc_type;
	m_iEquipMask	= 0;
	m_bUseable		= true;
	m_dwUseFlags	= USE_PERSIST;
	tag				= 0;
	x				= 0;
	z				= 0;

	m_bNeedUpdate	= (m_pDBEssence->blank == 0);
}

CECIvtrTownScroll::CECIvtrTownScroll(const CECIvtrTownScroll& s) : CECIvtrItem(s)
{
	m_pDBEssence	= s.m_pDBEssence;
	tag				= s.tag;
	x				= s.x;
	z				= s.z;
//	m_bNeedUpdate	= (m_pDBEssence->blank == 0);
}

CECIvtrTownScroll::~CECIvtrTownScroll()
{
}

//	Set item detail information
bool CECIvtrTownScroll::SetItemInfo(BYTE* pInfoData, int iDataLen)
{
	CECIvtrItem::SetItemInfo(pInfoData, iDataLen);

	struct INFO
	{
		int tag;
		float x;
		float y;
		float z;
	};

	if (iDataLen == sizeof(INFO))
	{
		INFO* pInfo = (INFO*)pInfoData;
		tag		= pInfo->tag;
		x		= pInfo->x;
		z		= pInfo->z;
	}

	return true;
}

//	Get item icon file name
const char* CECIvtrTownScroll::GetIconFile()
{
	return g_pGame->GetDataPath(m_pDBEssence->file_icon);
}

//	Get item name
const wchar_t* CECIvtrTownScroll::GetName()
{
	return m_pDBEssence->name;
}

//	Get item cool time
int CECIvtrTownScroll::GetCoolTime(int* piMax/* NULL */)
{
	return 0;
}

//	Get item description text
const wchar_t* CECIvtrTownScroll::GetNormalDesc(bool bRepair)
{
	if (m_bNeedUpdate)
 		return NULL;

	m_strDesc = _AL("");

	//	Try to build item description
	CECStringTab* pDescTab = g_pGame->GetItemDesc();
	int white = ITEMDESC_COL_WHITE;
	int earthyellow = ITEMDESC_COL_EARTHYELLOW;
	int name_color = DecideNameCol();

	if (m_iCount > 1)
		AddDescText(name_color, true, pDescTab->GetWideString(ITEMDESC_NAMENUMBER), GetName(), m_iCount);
	else
		AddDescText(name_color, true, pDescTab->GetWideString(ITEMDESC_NAME), GetName());

	AddStateDesc(pDescTab);
	AddExpireTimeDesc();

	if (m_pDBEssence->blank == 0)
	{
		CECInstance* pInstance = g_pGame->GetGameRun()->GetInstance(tag);

		if (pInstance)
			AddDescText(name_color, false, pDescTab->GetWideString(ITEMDESC_TOWNSCROLL), pInstance->GetName(), (int)x, (int)z);
	}

	//	Price
	AddPriceDesc(white, bRepair);
	
	CECInstance* pInstance = g_pGame->GetGameRun()->GetInstance(tag);
	if(pInstance)
	{
		CELPrecinct* pPrecinct = g_pGame->GetGameRun()->GetWorld()->GetPrecinctSet()->IsPointIn(
						x, z, pInstance->GetID());
		if(pPrecinct)
		{
			m_strDesc += _AL("\\r");
 			AddDescText(earthyellow, true, pDescTab->GetWideString(ITEMDESC_ANGLE_BRACKET), pPrecinct->GetName());
		}
	}
	//	Extend description
	AddExtDescText();



	return m_strDesc;
}

//	Get drop model for shown
const char * CECIvtrTownScroll::GetDropModel()
{
	return g_pGame->GetDataPath(m_pDBEssence->file_matter);
}

///////////////////////////////////////////////////////////////////////////
//	
//	Implement CECIvtrRevScroll
//	
///////////////////////////////////////////////////////////////////////////

CECIvtrRevScroll::CECIvtrRevScroll(int tid, int expire_date) : CECIvtrItem(tid, expire_date)
{
	m_iCID	= ICID_REVSCROLL;

	//	Get database data
	elementdataman* pDB = g_pGame->GetElementDataMan();
	DATA_TYPE DataType;
	m_pDBEssence = (REVIVESCROLL_ESSENCE*)pDB->get_data_ptr(tid, ID_SPACE_ESSENCE, DataType);

	m_iPileLimit	= m_pDBEssence->pile_num_max;
	m_iPrice		= m_pDBEssence->price;
	m_iShopPrice	= m_pDBEssence->shop_price;
	m_iProcType		= m_pDBEssence->proc_type;
	m_iEquipMask	= 0;
	m_bUseable		= true;

	m_bNeedUpdate	= false;
}

CECIvtrRevScroll::CECIvtrRevScroll(const CECIvtrRevScroll& s) : CECIvtrItem(s)
{
	m_pDBEssence	= s.m_pDBEssence;
}

CECIvtrRevScroll::~CECIvtrRevScroll()
{
}

//	Set item detail information
bool CECIvtrRevScroll::SetItemInfo(BYTE* pInfoData, int iDataLen)
{
	CECIvtrItem::SetItemInfo(pInfoData, iDataLen);
	return true;
}

//	Get item icon file name
const char* CECIvtrRevScroll::GetIconFile()
{
	return g_pGame->GetDataPath(m_pDBEssence->file_icon);
}

//	Get item name
const wchar_t* CECIvtrRevScroll::GetName()
{
	return m_pDBEssence->name;
}

//	Get item cool time
int CECIvtrRevScroll::GetCoolTime(int* piMax/* NULL */)
{
	return 0;
}

//	Get item description text
const wchar_t* CECIvtrRevScroll::GetNormalDesc(bool bRepair)
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

	//	Price
	AddPriceDesc(white, bRepair);

	//	Extend description
	AddExtDescText();

	return m_strDesc;
}

//	Get drop model for shown
const char * CECIvtrRevScroll::GetDropModel()
{
	return g_pGame->GetDataPath(m_pDBEssence->file_matter);
}

///////////////////////////////////////////////////////////////////////////
//	
//	Implement CECIvtrGMGenerator
//	
///////////////////////////////////////////////////////////////////////////

CECIvtrGMGenerator::CECIvtrGMGenerator(int tid, int expire_date) : CECIvtrItem(tid, expire_date)
{
	m_iCID	= ICID_GM_GENERATOR;

	//	Get database data
	elementdataman* pDB = g_pGame->GetElementDataMan();
	DATA_TYPE DataType;
	m_pDBEssence	= (GM_GENERATOR_ESSENCE*)pDB->get_data_ptr(tid, ID_SPACE_ESSENCE, DataType);
	m_pDBType		= (GM_GENERATOR_TYPE*)pDB->get_data_ptr(m_pDBEssence->id_type, ID_SPACE_ESSENCE, DataType);

	m_iPileLimit	= m_pDBEssence->pile_num_max;
	m_iPrice		= 0;
	m_iShopPrice	= 0;
	m_iProcType		= m_pDBEssence->proc_type;
	m_iEquipMask	= 0;
	m_bUseable		= true;

	m_bNeedUpdate	= false;
}

CECIvtrGMGenerator::CECIvtrGMGenerator(const CECIvtrGMGenerator& s) : CECIvtrItem(s)
{
	m_pDBEssence	= s.m_pDBEssence;
	m_pDBType		= s.m_pDBType;
}

CECIvtrGMGenerator::~CECIvtrGMGenerator()
{
}

//	Set item detail information
bool CECIvtrGMGenerator::SetItemInfo(BYTE* pInfoData, int iDataLen)
{
	CECIvtrItem::SetItemInfo(pInfoData, iDataLen);
	return true;
}

//	Get item icon file name
const char* CECIvtrGMGenerator::GetIconFile()
{
	return g_pGame->GetDataPath(m_pDBEssence->file_icon);
}

//	Get item name
const wchar_t* CECIvtrGMGenerator::GetName()
{
	return m_pDBEssence->name;
}

//	Get item cool time
int CECIvtrGMGenerator::GetCoolTime(int* piMax/* NULL */)
{
	return 0;
}

//	Get item description text
const wchar_t* CECIvtrGMGenerator::GetNormalDesc(bool bRepair)
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

	//	Extend description
	AddExtDescText();

	return m_strDesc;
}

//	Check item use condition
bool CECIvtrGMGenerator::CheckUseCondition()
{
	CECHostPlayer* pHost = g_pGame->GetGameRun()->GetHostPlayer();
	return pHost->IsGM();
}

//	Get drop model for shown
const char * CECIvtrGMGenerator::GetDropModel()
{
	return g_pGame->GetDataPath(m_pDBEssence->file_matter);
}

///////////////////////////////////////////////////////////////////////////
//	
//	Implement CECIvtrTransmitRoll
//	
///////////////////////////////////////////////////////////////////////////

CECIvtrTransmitRoll::CECIvtrTransmitRoll(int tid, int expire_date) : CECIvtrItem(tid, expire_date)
{
	m_iCID	= ICID_TRANSMITROLL;

	//	Get database data
	elementdataman* pDB = g_pGame->GetElementDataMan();
	DATA_TYPE DataType;
	m_pDBEssence = (TRANSMITROLL_ESSENCE*)pDB->get_data_ptr(tid, ID_SPACE_ESSENCE, DataType);

	m_iPileLimit	= m_pDBEssence->pile_num_max;
	m_iPrice		= m_pDBEssence->price;
	m_iShopPrice	= m_pDBEssence->shop_price;
	m_iProcType		= m_pDBEssence->proc_type;
	m_iEquipMask	= 0;
	m_bUseable		= true;
	
	m_bNeedUpdate	= false;
}

CECIvtrTransmitRoll::CECIvtrTransmitRoll(const CECIvtrTransmitRoll& s) : CECIvtrItem(s)
{
	m_pDBEssence	= s.m_pDBEssence;
}

CECIvtrTransmitRoll::~CECIvtrTransmitRoll()
{
}

//	Set item detail information
bool CECIvtrTransmitRoll::SetItemInfo(BYTE* pInfoData, int iDataLen)
{
	CECIvtrItem::SetItemInfo(pInfoData, iDataLen);
	return true;
}

//	Get item icon file name
const char* CECIvtrTransmitRoll::GetIconFile()
{
	return g_pGame->GetDataPath(m_pDBEssence->file_icon);
}

//	Get item name
const wchar_t* CECIvtrTransmitRoll::GetName()
{
	return m_pDBEssence->name;
}

//	Get item cool time
int CECIvtrTransmitRoll::GetCoolTime(int* piMax/* NULL */)
{
	CECHostPlayer* pHost = g_pGame->GetGameRun()->GetHostPlayer();
	return pHost ? pHost->GetCoolTime(GP_CT_TRANSMIT_ROLL, piMax) : 0;
}

//	Get item description text
const wchar_t* CECIvtrTransmitRoll::GetNormalDesc(bool bRepair)
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

	//	Price
	AddPriceDesc(white, bRepair);

	//	Extend description
	AddExtDescText();

	return m_strDesc;
}

//	Get drop model for shown
const char * CECIvtrTransmitRoll::GetDropModel()
{
	return g_pGame->GetDataPath(m_pDBEssence->file_matter);
}

///////////////////////////////////////////////////////////////////////////
//	
//	Implement CECIvtrTransmitRoll
//	
///////////////////////////////////////////////////////////////////////////

CECIvtrLuckyRoll::CECIvtrLuckyRoll(int tid, int expire_date) : CECIvtrItem(tid, expire_date)
{
	m_iCID	= ICID_LUCKYROLL;

	//	Get database data
	elementdataman* pDB = g_pGame->GetElementDataMan();
	DATA_TYPE DataType;
	m_pDBEssence = (LUCKYROLL_ESSENCE*)pDB->get_data_ptr(tid, ID_SPACE_ESSENCE, DataType);

	m_iPileLimit	= m_pDBEssence->pile_num_max;
	m_iPrice		= m_pDBEssence->price;
	m_iShopPrice	= m_pDBEssence->shop_price;
	m_iProcType		= m_pDBEssence->proc_type;
	m_iEquipMask	= 0;
	m_bUseable		= false;
	m_bNeedUpdate	= false;
}

CECIvtrLuckyRoll::CECIvtrLuckyRoll(const CECIvtrLuckyRoll& s) : CECIvtrItem(s)
{
	m_pDBEssence	= s.m_pDBEssence;
}

CECIvtrLuckyRoll::~CECIvtrLuckyRoll()
{
}

//	Set item detail information
bool CECIvtrLuckyRoll::SetItemInfo(BYTE* pInfoData, int iDataLen)
{
	CECIvtrItem::SetItemInfo(pInfoData, iDataLen);
	return true;
}

//	Get item icon file name
const char* CECIvtrLuckyRoll::GetIconFile()
{
	return g_pGame->GetDataPath(m_pDBEssence->file_icon);
}

//	Get item name
const wchar_t* CECIvtrLuckyRoll::GetName()
{
	return m_pDBEssence->name;
}

//	Get item cool time
int CECIvtrLuckyRoll::GetCoolTime(int* piMax/* NULL */)
{
	return 0;
}

//	Get item description text
const wchar_t* CECIvtrLuckyRoll::GetNormalDesc(bool bRepair)
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

	//	Price
	AddPriceDesc(white, bRepair);

	//	Extend description
	AddExtDescText();

	return m_strDesc;
}

//	Get drop model for shown
const char * CECIvtrLuckyRoll::GetDropModel()
{
	return g_pGame->GetDataPath(m_pDBEssence->file_matter);
}

///////////////////////////////////////////////////////////////////////////
//	
//	Implement CECIvtrArtilleryScroll
//	
///////////////////////////////////////////////////////////////////////////

CECIvtrArtilleryScroll::CECIvtrArtilleryScroll(int tid, int expire_date) : CECIvtrItem(tid, expire_date)
{
	m_iCID	= ICID_ARTILLERY_SCROLL;

	//	Get database data
	elementdataman* pDB = g_pGame->GetElementDataMan();
	DATA_TYPE DataType;
	m_pDBEssence = (SIEGE_ARTILLERY_SCROLL_ESSENCE*)pDB->get_data_ptr(tid, ID_SPACE_ESSENCE, DataType);

	m_iPileLimit	= m_pDBEssence->pile_num_max;
	m_iPrice		= m_pDBEssence->price;
	m_iShopPrice	= m_pDBEssence->shop_price;
	m_iProcType		= m_pDBEssence->proc_type;
	m_iEquipMask	= 0;
	m_bUseable		= true;
	m_bNeedUpdate	= false;
}

CECIvtrArtilleryScroll::CECIvtrArtilleryScroll(const CECIvtrArtilleryScroll& s) : CECIvtrItem(s)
{
	m_pDBEssence	= s.m_pDBEssence;
}

CECIvtrArtilleryScroll::~CECIvtrArtilleryScroll()
{
}

//	Set item detail information
bool CECIvtrArtilleryScroll::SetItemInfo(BYTE* pInfoData, int iDataLen)
{
	CECIvtrItem::SetItemInfo(pInfoData, iDataLen);
	return true;
}

//	Get item icon file name
const char* CECIvtrArtilleryScroll::GetIconFile()
{
	return g_pGame->GetDataPath(m_pDBEssence->file_icon);
}

//	Get item name
const wchar_t* CECIvtrArtilleryScroll::GetName()
{
	return m_pDBEssence->name;
}

//	Get item cool time
int CECIvtrArtilleryScroll::GetCoolTime(int* piMax/* NULL */)
{
	return 0;
}

//	Get item description text
const wchar_t* CECIvtrArtilleryScroll::GetNormalDesc(bool bRepair)
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

	//	Price
	AddPriceDesc(white, bRepair);

	//	Extend description
	AddExtDescText();

	return m_strDesc;
}

//	Get drop model for shown
const char * CECIvtrArtilleryScroll::GetDropModel()
{
	return g_pGame->GetDataPath(m_pDBEssence->file_matter);
}
