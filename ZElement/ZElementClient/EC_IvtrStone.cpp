/*
 * FILE: EC_IvtrStones.cpp
 *
 * DESCRIPTION: 
 *
 * CREATED BY: Hedi, 2006/4/27
 *
 * HISTORY: 
 *
 * Copyright (c) 2005 Archosaur Studio, All Rights Reserved.
 */

#include "EC_Global.h"
#include "EC_IvtrStone.h"
#ifndef _INVENTORY_DESC_MAN //Added by Ford.W 2010-07-22.
#include "EC_Game.h"
#else
#include "EC_Game_HintTool.h"
#endif
#include "EC_FixedMsg.h"
#include "EC_IvtrEquip.h"
#include "EC_GameUIMan.h"
#include "EC_UIManager.h"
#include "EC_GameRun.h"
#include "AFI.h"
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
//	Implement CECIvtrEStone
//	
///////////////////////////////////////////////////////////////////////////

CECIvtrEStone::CECIvtrEStone(int tid, int expire_date) : CECIvtrItem(tid, expire_date)
{
	m_iCID	= ICID_ESTONE;

	//	Get database data
	elementdataman* pDB = g_pGame->GetElementDataMan();
	DATA_TYPE DataType;
	m_pDBEssence	= (ESTONE_ESSENCE*)pDB->get_data_ptr(tid, ID_SPACE_ESSENCE, DataType);
	
	m_iPileLimit	= m_pDBEssence->pile_num_max;
	m_iPrice		= m_pDBEssence->price;
	m_iShopPrice	= m_pDBEssence->shop_price;
	m_iProcType		= m_pDBEssence->proc_type;
	m_iEquipMask	= 0;

	m_bNeedUpdate	= false;
}

CECIvtrEStone::CECIvtrEStone(const CECIvtrEStone& s) : CECIvtrItem(s)
{
	m_pDBEssence	= s.m_pDBEssence;
}

CECIvtrEStone::~CECIvtrEStone()
{
}

//	Set item detail information
bool CECIvtrEStone::SetItemInfo(BYTE* pInfoData, int iDataLen)
{
	CECIvtrItem::SetItemInfo(pInfoData, iDataLen);
	return true;
}

//	Get item icon file name
const char* CECIvtrEStone::GetIconFile()
{
	return g_pGame->GetDataPath(m_pDBEssence->file_icon);
}

//	Get item name
const wchar_t* CECIvtrEStone::GetName()
{
	return m_pDBEssence->name;
}

//	Get item description text
const wchar_t* CECIvtrEStone::GetNormalDesc(bool bRepair)
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
const char * CECIvtrEStone::GetDropModel()
{
	return g_pGame->GetDataPath(m_pDBEssence->file_matter);
}

///////////////////////////////////////////////////////////////////////////
//	
//	Implement CECIvtrPStone
//	
///////////////////////////////////////////////////////////////////////////

CECIvtrPStone::CECIvtrPStone(int tid, int expire_date) : CECIvtrItem(tid, expire_date)
{
	m_iCID	= ICID_PSTONE;

	//	Get database data
	elementdataman* pDB = g_pGame->GetElementDataMan();
	DATA_TYPE DataType;
	m_pDBEssence	= (PSTONE_ESSENCE*)pDB->get_data_ptr(tid, ID_SPACE_ESSENCE, DataType);
	
	m_iPileLimit	= m_pDBEssence->pile_num_max;
	m_iPrice		= m_pDBEssence->price;
	m_iShopPrice	= m_pDBEssence->shop_price;
	m_iProcType		= m_pDBEssence->proc_type;
	m_iEquipMask	= 0;

	m_bNeedUpdate	= false;
}

CECIvtrPStone::CECIvtrPStone(const CECIvtrPStone& s) : CECIvtrItem(s)
{
	m_pDBEssence	= s.m_pDBEssence;
}

CECIvtrPStone::~CECIvtrPStone()
{
}

//	Set item detail information
bool CECIvtrPStone::SetItemInfo(BYTE* pInfoData, int iDataLen)
{
	CECIvtrItem::SetItemInfo(pInfoData, iDataLen);
	return true;
}

//	Get item icon file name
const char* CECIvtrPStone::GetIconFile()
{
	return g_pGame->GetDataPath(m_pDBEssence->file_icon);
}

//	Get item name
const wchar_t* CECIvtrPStone::GetName()
{
	return m_pDBEssence->name;
}

//	Get item description text
const wchar_t* CECIvtrPStone::GetNormalDesc(bool bRepair)
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

	DATA_TYPE dt;
	if( m_pDBEssence->type == 0 )
	{
		EQUIPMENT_ADDON * pAddOn = (EQUIPMENT_ADDON *) g_pGame->GetElementDataMan()->get_data_ptr(m_pDBEssence->effect_addon_id,
			ID_SPACE_ADDON, dt);
		if( dt == DT_EQUIPMENT_ADDON && pAddOn->type > 0 )
		{
			AddDescText(white, true, _AL("%s%s"), pDescTab->GetWideString(ITEMDESC_PSTONEADDON_PREFIX), CECIvtrEquip::GetEquipAddonDesc(pAddOn->id, pAddOn->type, pAddOn->param1, pAddOn->param2, pAddOn->param3, 1));
		}
	}
	else if( m_pDBEssence->type == 1 )
	{
		AddDescText(white, true, pDescTab->GetWideString(ITEMDESC_LEVELREQ_DROP), 1);
	}


	//	Price
	AddPriceDesc(white, bRepair);

	//	Extend description
	AddExtDescText();

	return m_strDesc;
}

//	Get drop model for shown
const char * CECIvtrPStone::GetDropModel()
{
	return g_pGame->GetDataPath(m_pDBEssence->file_matter);
}

///////////////////////////////////////////////////////////////////////////
//	
//	Implement CECIvtrSStone
//	
///////////////////////////////////////////////////////////////////////////

CECIvtrSStone::CECIvtrSStone(int tid, int expire_date) : CECIvtrItem(tid, expire_date)
{
	m_iCID	= ICID_SSTONE;

	//	Get database data
	elementdataman* pDB = g_pGame->GetElementDataMan();
	DATA_TYPE DataType;
	m_pDBEssence	= (SSTONE_ESSENCE*)pDB->get_data_ptr(tid, ID_SPACE_ESSENCE, DataType);
	
	m_iPileLimit	= m_pDBEssence->pile_num_max;
	m_iPrice		= m_pDBEssence->price;
	m_iShopPrice	= m_pDBEssence->shop_price;
	m_iProcType		= m_pDBEssence->proc_type;
	m_iEquipMask	= 0;

	m_bNeedUpdate	= false;
}

CECIvtrSStone::CECIvtrSStone(const CECIvtrSStone& s) : CECIvtrItem(s)
{
	m_pDBEssence	= s.m_pDBEssence;
}

CECIvtrSStone::~CECIvtrSStone()
{
}

//	Set item detail information
bool CECIvtrSStone::SetItemInfo(BYTE* pInfoData, int iDataLen)
{
	CECIvtrItem::SetItemInfo(pInfoData, iDataLen);
	return true;
}

//	Get item icon file name
const char* CECIvtrSStone::GetIconFile()
{
	return g_pGame->GetDataPath(m_pDBEssence->file_icon);
}

//	Get item name
const wchar_t* CECIvtrSStone::GetName()
{
	return m_pDBEssence->name;
}

//	Get item description text
const wchar_t* CECIvtrSStone::GetNormalDesc(bool bRepair)
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
const char * CECIvtrSStone::GetDropModel()
{
	return g_pGame->GetDataPath(m_pDBEssence->file_matter);
}


///////////////////////////////////////////////////////////////////////////
//	
//	Implement CECIvtrRefineTicket
//	
///////////////////////////////////////////////////////////////////////////

CECIvtrRefineTicket::CECIvtrRefineTicket(int tid, int expire_date) : CECIvtrItem(tid, expire_date)
{
	m_iCID	= ICID_REFINETICKET;

	//	Get database data
	elementdataman* pDB = g_pGame->GetElementDataMan();
	DATA_TYPE DataType;
	m_pDBEssence	= (REFINE_TICKET_ESSENCE*)pDB->get_data_ptr(tid, ID_SPACE_ESSENCE, DataType);
	
	m_iPileLimit	= m_pDBEssence->pile_num_max;
	m_iPrice		= m_pDBEssence->price;
	m_iShopPrice	= m_pDBEssence->shop_price;
	m_iProcType		= m_pDBEssence->proc_type;
	m_iEquipMask	= 0;

	m_bNeedUpdate	= false;
}

CECIvtrRefineTicket::CECIvtrRefineTicket(const CECIvtrRefineTicket& s) : CECIvtrItem(s)
{
	m_pDBEssence	= s.m_pDBEssence;
}

CECIvtrRefineTicket::~CECIvtrRefineTicket()
{
}

//	Set item detail information
bool CECIvtrRefineTicket::SetItemInfo(BYTE* pInfoData, int iDataLen)
{
	CECIvtrItem::SetItemInfo(pInfoData, iDataLen);
	return true;
}

//	Get item icon file name
const char* CECIvtrRefineTicket::GetIconFile()
{
	return g_pGame->GetDataPath(m_pDBEssence->file_icon);
}

//	Get item name
const wchar_t* CECIvtrRefineTicket::GetName()
{
	return m_pDBEssence->name;
}

//	Get item description text
const wchar_t* CECIvtrRefineTicket::GetNormalDesc(bool bRepair)
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

	/*
	if( m_pDBEssence->ext_reserved_prob )
	{
		AddDescText(white, true, pDescTab->GetWideString(ITEMDESC_REFINETICKET1), m_pDBEssence->ext_reserved_prob * 100.0f);
	}

	if( m_pDBEssence->ext_succeed_prob )
	{
		AddDescText(white, true, pDescTab->GetWideString(ITEMDESC_REFINETICKET2), m_pDBEssence->ext_succeed_prob * 100.0f);
	}
	*/

	//	Price
	AddPriceDesc(white, bRepair);

	//	Extend description
	AddExtDescText();

	return m_strDesc;
}

//	Get drop model for shown
const char * CECIvtrRefineTicket::GetDropModel()
{
	return g_pGame->GetDataPath(m_pDBEssence->file_matter);
}

///////////////////////////////////////////////////////////////////////////
//	
//	Implement CECIvtrOfflineTrustee
//	
///////////////////////////////////////////////////////////////////////////

CECIvtrOfflineTrustee::CECIvtrOfflineTrustee(int tid, int expire_date) : CECIvtrItem(tid, expire_date)
{
	m_iCID	= ICID_OFFLINE_TRUSTEE;

	//	Get database data
	elementdataman* pDB = g_pGame->GetElementDataMan();
	DATA_TYPE DataType;
	m_pDBEssence	= (OFFLINE_TRUSTEE_ESSENCE*)pDB->get_data_ptr(tid, ID_SPACE_ESSENCE, DataType);
	
	m_iPileLimit	= m_pDBEssence->pile_num_max;
	m_iPrice		= m_pDBEssence->price;
	m_iShopPrice	= m_pDBEssence->shop_price;
	m_iProcType		= m_pDBEssence->proc_type;
	m_iEquipMask	= 0;
	m_bUseable		= true;
	m_bNeedUpdate	= false;
}

CECIvtrOfflineTrustee::CECIvtrOfflineTrustee(const CECIvtrOfflineTrustee& s) : CECIvtrItem(s)
{
	m_pDBEssence	= s.m_pDBEssence;
}

CECIvtrOfflineTrustee::~CECIvtrOfflineTrustee()
{
}

//	Set item detail information
bool CECIvtrOfflineTrustee::SetItemInfo(BYTE* pInfoData, int iDataLen)
{
	CECIvtrItem::SetItemInfo(pInfoData, iDataLen);
	return true;
}

//	Get item icon file name
const char* CECIvtrOfflineTrustee::GetIconFile()
{
	return g_pGame->GetDataPath(m_pDBEssence->file_icon);
}

//	Get item name
const wchar_t* CECIvtrOfflineTrustee::GetName()
{
	return m_pDBEssence->name;
}

//	Get item description text
const wchar_t* CECIvtrOfflineTrustee::GetNormalDesc(bool bRepair)
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
const char * CECIvtrOfflineTrustee::GetDropModel()
{
	return g_pGame->GetDataPath(m_pDBEssence->file_matter);
}


///////////////////////////////////////////////////////////////////////////
//	
//	Implement CECIvtrChangeShapeStone
//	
///////////////////////////////////////////////////////////////////////////

CECIvtrChangeShapeStone::CECIvtrChangeShapeStone(int tid, int expire_date) : CECIvtrItem(tid, expire_date)
{
	m_iCID	= ICID_CHANGE_SHAPE_STONE;

	//	Get database data
	elementdataman* pDB = g_pGame->GetElementDataMan();
	DATA_TYPE DataType;
	m_pDBEssence	= (CHANGE_SHAPE_STONE_ESSENCE*)pDB->get_data_ptr(tid, ID_SPACE_ESSENCE, DataType);
	
	m_iPileLimit	= m_pDBEssence->pile_num_max;
	m_iPrice		= m_pDBEssence->price;
	m_iShopPrice	= m_pDBEssence->shop_price;
	m_iProcType		= m_pDBEssence->proc_type;
	m_iEquipMask	= 0;
	m_bUseable		= true;
	m_bNeedUpdate	= false;
}

CECIvtrChangeShapeStone::CECIvtrChangeShapeStone(const CECIvtrChangeShapeStone& s) : CECIvtrItem(s)
{
	m_pDBEssence	= s.m_pDBEssence;
}

CECIvtrChangeShapeStone::~CECIvtrChangeShapeStone()
{
}

//	Set item detail information
bool CECIvtrChangeShapeStone::SetItemInfo(BYTE* pInfoData, int iDataLen)
{
	CECIvtrItem::SetItemInfo(pInfoData, iDataLen);
	return true;
}

//	Get item icon file name
const char* CECIvtrChangeShapeStone::GetIconFile()
{
	return g_pGame->GetDataPath(m_pDBEssence->file_icon);
}

//	Get item name
const wchar_t* CECIvtrChangeShapeStone::GetName()
{
	return m_pDBEssence->name;
}

//	Get item description text
const wchar_t* CECIvtrChangeShapeStone::GetNormalDesc(bool bRepair)
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
const char * CECIvtrChangeShapeStone::GetDropModel()
{
	return g_pGame->GetDataPath(m_pDBEssence->file_matter);
}



///////////////////////////////////////////////////////////////////////////////
//宝石物品类的实现函数
//主要实现：宝石物品类的构造函数、获得Hint的描述函数
///////////////////////////////////////////////////////////////////////////////
CECIvtrGem::CECIvtrGem( int tid, int expire_date ) : CECIvtrItem(tid, expire_date)
{
	m_iCID	= ICID_GEM;

	//	Get database data
	elementdataman* pDB = g_pGame->GetElementDataMan();
	DATA_TYPE DataType;
	m_pDBEssence	= (GEM_ESSENCE*)pDB->get_data_ptr(tid, ID_SPACE_ESSENCE, DataType);
	
	m_iPileLimit	= m_pDBEssence->pile_num_max;
	m_iPrice		= m_pDBEssence->price;
	m_iShopPrice	= m_pDBEssence->shop_price;
	m_iProcType		= m_pDBEssence->proc_type;
	m_iEquipMask	= 0;

	m_bNeedUpdate	= false;

	m_bSuitShapeType = false;
}

//拷贝构造函数
CECIvtrGem::CECIvtrGem(const CECIvtrGem& s) : CECIvtrItem(s)
{
	m_pDBEssence	= s.m_pDBEssence;
}

//析构函数
CECIvtrGem::~CECIvtrGem()
{
}

//从gs服务器获得相应的数据，更新该物品
bool CECIvtrGem::SetItemInfo(BYTE* pInfoData, int iDataLen)
{
	CECIvtrItem::SetItemInfo(pInfoData, iDataLen);
	return true;	
}

//获得该物品的图标路径
const char* CECIvtrGem::GetIconFile()
{
	return g_pGame->GetDataPath(m_pDBEssence->file_icon);
}

//获得该宝石的形状图标路径
const char* CECIvtrGem::GetShapeIconFile()
{
	return g_pGame->GetDataPath(m_pDBEssence->file_shape_icon);
}

//获得该物品的名称
const wchar_t* CECIvtrGem::GetName()
{
	return m_pDBEssence->name;
}

//获得该物品掉落时的简化模型路径
const char * CECIvtrGem::GetDropModel()
{
	return g_pGame->GetDataPath(m_pDBEssence->file_matter);
}

//获得该物品的基本悬浮提示描述
const wchar_t* CECIvtrGem::GetNormalDesc(bool bRepair)
{
	m_strDesc = _AL("");//初始化

	CECStringTab* pDescTab = g_pGame->GetItemDesc();
	int white = ITEMDESC_COL_WHITE;
	int name_color = DecideNameCol();

	//添加物品名字描述
	if (m_iCount > 1)
	{
		AddDescText(name_color, true, pDescTab->GetWideString(ITEMDESC_NAMENUMBER), GetName(), m_iCount);
	}
	else
	{
		AddDescText(name_color, true, pDescTab->GetWideString(ITEMDESC_NAME), GetName());
	}

	//添加宝石品级描述
	AddDescText(ITEMDESC_COL_RED, true, pDescTab->GetWideString(ITEMDESC_GEM_GRADE), pDescTab->GetWideString(ITEMDESC_NUMBER_ZERO+m_pDBEssence->grade));
		
	AddStateDesc(pDescTab);

	//添加附加属性描述
	// 基础属性
	int i;
	for( i = 0; i < 6; ++i)//悲剧的magic number: 6....强烈需要去除这种恶心的东西。
	{
		if(m_pDBEssence->addon_property[i].addon_type==1) // 附加属性
			continue;
		//首先获得该附加属性的模板
		DATA_TYPE dt;
		EQUIPMENT_ADDON * pAddOn = (EQUIPMENT_ADDON *) g_pGame->GetElementDataMan()->get_data_ptr(m_pDBEssence->addon_property[i].addon_id, ID_SPACE_ADDON, dt);
		if( dt != DT_EQUIPMENT_ADDON )
		{
			continue;
		}

		//设置字体颜色
		int color = ITEMDESC_COL_LIGHTBLUE;
		//根据获得的附加属性模板添加描述
		AddDescText(color, true, _AL("%s"), CECIvtrEquip::GetEquipAddonDesc(pAddOn->id, pAddOn->type, pAddOn->param1, pAddOn->param2, pAddOn->param3, 1));
	}

	// 插槽类型
#ifndef _INVENTORY_DESC_MAN		//=======================================================小工具不包含GameUIMan的相关逻辑
	AString strName;
	af_GetFileTitle(g_pGame->GetDataPath(m_pDBEssence->file_shape_icon), strName);
	strName.MakeLower();
	CECGameUIMan *pGameUIMan = g_pGame->GetGameRun()->GetUIManager()->GetInGameUIMan();
	AddDescImage(false, CECGameUIMan::ICON_HINT_GEM, pGameUIMan->GetIconId(CECGameUIMan::ICONS_GEM16, strName));
#endif							//=======================================================
	int index = ITEMDESC_GEM_SLOT_SHAPE1 + m_pDBEssence->shape_type;
	AddDescText(ITEMDESC_COL_WHITE, true, pDescTab->GetWideString(ITEMDESC_SLOT_TYPE), pDescTab->GetWideString(index));

	// 镶嵌属性
	for( i = 0; i < 6; ++i )//悲剧的magic number: 6....强烈需要去除这种恶心的东西。
	{
		if(m_pDBEssence->addon_property[i].addon_type==0) // 基础属性
			continue;
		//首先获得该附加属性的模板
		DATA_TYPE dt;
		EQUIPMENT_ADDON * pAddOn = (EQUIPMENT_ADDON *) g_pGame->GetElementDataMan()->get_data_ptr(m_pDBEssence->addon_property[i].addon_id, ID_SPACE_ADDON, dt);
		if( dt != DT_EQUIPMENT_ADDON )
		{
			continue;
		}
		
		//设置字体颜色
		int color = ITEMDESC_COL_GRAY;
		if(m_bSuitShapeType)
			color = ITEMDESC_COL_LIGHTBLUE;
		else
			m_bSuitShapeType = false;
		//根据获得的附加属性模板添加描述
		AddDescText(color, true, _AL("%s"), CECIvtrEquip::GetEquipAddonDesc(pAddOn->id, pAddOn->type, pAddOn->param1, pAddOn->param2, pAddOn->param3, 1));
	}

	//添加过期时间描述
	AddExpireTimeDesc();

	//添加物品价格描述
	AddPriceDesc(white, bRepair);

	//添加额外描述
	AddExtDescText();

	//返回最终的格式化后的Hint描述字符串
	return m_strDesc;
}



///////////////////////////////////////////////////////////////////////////////
//宝石魔印的实现函数
//主要实现：宝石魔印物品的构造函数、获得该物品的悬浮提示描述
///////////////////////////////////////////////////////////////////////////////
CECIvtrGemSeal::CECIvtrGemSeal( int tid, int expire_date ) : CECIvtrItem(tid, expire_date)
{
	m_iCID	= ICID_GEM_SEAL;

	//	Get database data
	elementdataman* pDB = g_pGame->GetElementDataMan();
	DATA_TYPE DataType;
	m_pDBEssence	= (GEM_SEAL_ESSENCE*)pDB->get_data_ptr(tid, ID_SPACE_ESSENCE, DataType);
	
	m_iPileLimit	= m_pDBEssence->pile_num_max;
	m_iPrice		= m_pDBEssence->price;
	m_iShopPrice	= m_pDBEssence->shop_price;
	m_iProcType		= m_pDBEssence->proc_type;
	m_iEquipMask	= 0;

	m_bNeedUpdate	= false;

}

//拷贝构造函数
CECIvtrGemSeal::CECIvtrGemSeal(const CECIvtrGemSeal& s) : CECIvtrItem(s)
{
	m_pDBEssence	= s.m_pDBEssence;
}

//析构函数
CECIvtrGemSeal::~CECIvtrGemSeal()
{
}

//从gs服务器获得相应的数据，更新该物品
bool CECIvtrGemSeal::SetItemInfo(BYTE* pInfoData, int iDataLen)
{
	CECIvtrItem::SetItemInfo(pInfoData, iDataLen);
	return true;	
}

//获得该物品的图标路径
const char* CECIvtrGemSeal::GetIconFile()
{
	return g_pGame->GetDataPath(m_pDBEssence->file_icon);
}

//获得该物品的名称
const wchar_t* CECIvtrGemSeal::GetName()
{
	return m_pDBEssence->name;
}

//获得该物品掉落时的简化模型路径
const char * CECIvtrGemSeal::GetDropModel()
{
	return g_pGame->GetDataPath(m_pDBEssence->file_matter);
}

//获得该物品的基本悬浮提示描述
const wchar_t* CECIvtrGemSeal::GetNormalDesc(bool bRepair)
{
	m_strDesc = _AL("");//初始化

	CECStringTab* pDescTab = g_pGame->GetItemDesc();
	int white = ITEMDESC_COL_WHITE;
	int name_color = DecideNameCol();

	//添加物品名字描述
	if (m_iCount > 1)
	{
		AddDescText(name_color, true, pDescTab->GetWideString(ITEMDESC_NAMENUMBER), GetName(), m_iCount);
	}
	else
	{
		AddDescText(name_color, true, pDescTab->GetWideString(ITEMDESC_NAME), GetName());
	}

	//添加魔印的形状属性
	int index = ITEMDESC_GEM_SLOT_SHAPE1 + m_pDBEssence->shape_type;
	AddDescText(ITEMDESC_COL_RED, true, pDescTab->GetWideString(ITEMDESC_GEM_SEAL_SHAPE), pDescTab->GetWideString(index));

	AddStateDesc(pDescTab);

	//添加过期时间描述
	AddExpireTimeDesc();

	//添加物品价格描述
	AddPriceDesc(white, bRepair);

	//添加额外描述
	AddExtDescText();

	//返回最终的格式化后的Hint描述字符串
	return m_strDesc;
}


///////////////////////////////////////////////////////////////////////////////
//宝石粉尘的实现函数
//主要实现：宝石粉尘的构造函数、获得物品当前悬浮提示描述函数
///////////////////////////////////////////////////////////////////////////////
CECIvtrGemDust::CECIvtrGemDust( int tid, int expire_date ) : CECIvtrItem(tid, expire_date)
{
	m_iCID	= ICID_GEM_DUST;

	//	Get database data
	elementdataman* pDB = g_pGame->GetElementDataMan();
	DATA_TYPE DataType;
	m_pDBEssence	= (GEM_DUST_ESSENCE*)pDB->get_data_ptr(tid, ID_SPACE_ESSENCE, DataType);
	
	m_iPileLimit	= m_pDBEssence->pile_num_max;
	m_iPrice		= m_pDBEssence->price;
	m_iShopPrice	= m_pDBEssence->shop_price;
	m_iProcType		= m_pDBEssence->proc_type;
	m_iEquipMask	= 0;

	m_bNeedUpdate	= false;

}

//拷贝构造函数
CECIvtrGemDust::CECIvtrGemDust(const CECIvtrGemDust& s) : CECIvtrItem(s)
{
	m_pDBEssence	= s.m_pDBEssence;
}

//析构函数
CECIvtrGemDust::~CECIvtrGemDust()
{
}

//从gs服务器获得相应的数据，更新该物品
bool CECIvtrGemDust::SetItemInfo(BYTE* pInfoData, int iDataLen)
{
	CECIvtrItem::SetItemInfo(pInfoData, iDataLen);
	return true;	
}

//获得该物品的图标路径
const char* CECIvtrGemDust::GetIconFile()
{
	return g_pGame->GetDataPath(m_pDBEssence->file_icon);
}

//获得该物品的名称
const wchar_t* CECIvtrGemDust::GetName()
{
	return m_pDBEssence->name;
}

//获得该物品掉落时的简化模型路径
const char * CECIvtrGemDust::GetDropModel()
{
	return g_pGame->GetDataPath(m_pDBEssence->file_matter);
}

//获得该物品的基本悬浮提示描述
const wchar_t* CECIvtrGemDust::GetNormalDesc(bool bRepair)
{
	m_strDesc = _AL("");//初始化

	CECStringTab* pDescTab = g_pGame->GetItemDesc();
	int white = ITEMDESC_COL_WHITE;
	int name_color = DecideNameCol();

	//添加物品名字描述
	if (m_iCount > 1)
	{
		AddDescText(name_color, true, pDescTab->GetWideString(ITEMDESC_NAMENUMBER), GetName(), m_iCount);
	}
	else
	{
		AddDescText(name_color, true, pDescTab->GetWideString(ITEMDESC_NAME), GetName());
	}

	AddStateDesc(pDescTab);

	//添加过期时间描述
	AddExpireTimeDesc();

	//添加物品价格描述
	AddPriceDesc(white, bRepair);

	//添加额外描述
	AddExtDescText();

	//返回最终的格式化后的Hint描述字符串
	return m_strDesc;
}


///////////////////////////////////////////////////////////////////////////////
//通用道具的实现函数
//主要实现：通用道具的构造函数、获得物品当前悬浮提示描述函数
///////////////////////////////////////////////////////////////////////////////
CECIvtrGeneralArticle::CECIvtrGeneralArticle( int tid, int expire_date ) : CECIvtrItem(tid, expire_date)
{
	m_iCID	= ICID_GENERAL_ARTICLE;

	//	Get database data
	elementdataman* pDB = g_pGame->GetElementDataMan();
	DATA_TYPE DataType;
	m_pDBEssence	= (GENERAL_ARTICLE_ESSENCE*)pDB->get_data_ptr(tid, ID_SPACE_ESSENCE, DataType);
	
	m_iPileLimit	= m_pDBEssence->pile_num_max;
	m_iPrice		= m_pDBEssence->price;
	m_iShopPrice	= m_pDBEssence->shop_price;
	m_iProcType		= m_pDBEssence->proc_type;
	m_iEquipMask	= 0;

	m_bNeedUpdate	= false;

}

//拷贝构造函数
CECIvtrGeneralArticle::CECIvtrGeneralArticle(const CECIvtrGeneralArticle& s) : CECIvtrItem(s)
{
	m_pDBEssence	= s.m_pDBEssence;
}

//析构函数
CECIvtrGeneralArticle::~CECIvtrGeneralArticle()
{
}

//从gs服务器获得相应的数据，更新该物品
bool CECIvtrGeneralArticle::SetItemInfo(BYTE* pInfoData, int iDataLen)
{
	CECIvtrItem::SetItemInfo(pInfoData, iDataLen);
	return true;	
}

//获得该物品的图标路径
const char* CECIvtrGeneralArticle::GetIconFile()
{
	return g_pGame->GetDataPath(m_pDBEssence->file_icon);
}

//获得该物品的名称
const wchar_t* CECIvtrGeneralArticle::GetName()
{
	return m_pDBEssence->name;
}

//获得该物品掉落时的简化模型路径
const char * CECIvtrGeneralArticle::GetDropModel()
{
	return g_pGame->GetDataPath(m_pDBEssence->file_matter);
}

//获得该物品的基本悬浮提示描述
const wchar_t* CECIvtrGeneralArticle::GetNormalDesc(bool bRepair)
{
	m_strDesc = _AL("");//初始化

	CECStringTab* pDescTab = g_pGame->GetItemDesc();
	int white = ITEMDESC_COL_WHITE;
	int name_color = DecideNameCol();

	//添加物品名字描述
	if (m_iCount > 1)
	{
		AddDescText(name_color, true, pDescTab->GetWideString(ITEMDESC_NAMENUMBER), GetName(), m_iCount);
	}
	else
	{
		AddDescText(name_color, true, pDescTab->GetWideString(ITEMDESC_NAME), GetName());
	}

	AddStateDesc(pDescTab);

	//添加过期时间描述
	AddExpireTimeDesc();

	//添加物品价格描述
	AddPriceDesc(white, bRepair);

	//添加额外描述
	AddExtDescText();

	//返回最终的格式化后的Hint描述字符串
	return m_strDesc;
}


///////////////////////////////////////////////////////////////////////////////
//在线奖励小礼包的实现函数	Added 2012-03-23.
//主要实现：在线奖励小礼包的构造函数、获得物品当前悬浮提示描述函数
///////////////////////////////////////////////////////////////////////////////
CECIvtrSmallGiftBag::CECIvtrSmallGiftBag( int tid, int expire_date ) : CECIvtrItem(tid, expire_date)
{
	m_iCID	= ICID_SMALL_ONLINE_GIFT_BAG;

	//	Get database data
	elementdataman* pDB = g_pGame->GetElementDataMan();
	DATA_TYPE DataType;
	m_pDBEssence	= (SMALL_ONLINE_GIFT_BAG_ESSENCE*)pDB->get_data_ptr(tid, ID_SPACE_ESSENCE, DataType);
	
	m_iPileLimit	= m_pDBEssence->pile_num_max;
	m_iPrice		= m_pDBEssence->price;
	m_iShopPrice	= m_pDBEssence->shop_price;
	m_iProcType		= m_pDBEssence->proc_type;
	m_iEquipMask	= 0;

	m_bUseable		= true;	//物品能否直接使用
	m_bNeedUpdate	= false;

}

//拷贝构造函数
CECIvtrSmallGiftBag::CECIvtrSmallGiftBag(const CECIvtrSmallGiftBag& s) : CECIvtrItem(s)
{
	m_pDBEssence	= s.m_pDBEssence;
}

//析构函数
CECIvtrSmallGiftBag::~CECIvtrSmallGiftBag()
{
}

//从gs服务器获得相应的数据，更新该物品
bool CECIvtrSmallGiftBag::SetItemInfo(BYTE* pInfoData, int iDataLen)
{
	CECIvtrItem::SetItemInfo(pInfoData, iDataLen);
	return true;	
}

//获得该物品的图标路径
const char* CECIvtrSmallGiftBag::GetIconFile()
{
	return g_pGame->GetDataPath(m_pDBEssence->file_icon);
}

//获得该物品的名称
const wchar_t* CECIvtrSmallGiftBag::GetName()
{
	return m_pDBEssence->name;
}

//获得该物品掉落时的简化模型路径
const char * CECIvtrSmallGiftBag::GetDropModel()
{
	return g_pGame->GetDataPath(m_pDBEssence->file_matter);
}

//获得该物品的基本悬浮提示描述
const wchar_t* CECIvtrSmallGiftBag::GetNormalDesc(bool bRepair)
{
	m_strDesc = _AL("");//初始化

	CECStringTab* pDescTab = g_pGame->GetItemDesc();
	int white = ITEMDESC_COL_WHITE;
	int name_color = DecideNameCol();

	//添加物品名字描述
	if (m_iCount > 1)
	{
		AddDescText(name_color, true, pDescTab->GetWideString(ITEMDESC_NAMENUMBER), GetName(), m_iCount);
	}
	else
	{
		AddDescText(name_color, true, pDescTab->GetWideString(ITEMDESC_NAME), GetName());
	}

	//特殊信息，添加该小礼包中都有哪些物品，格式：物品名称*数量
	const int nItemCount = 16;
	CECIvtrItem* pIvtrItem = NULL;
	for ( int i = 0; i < nItemCount; ++i )
	{
		if ( m_pDBEssence->item_configs[i].item_id > 0 )
		{
			pIvtrItem = CECIvtrItem::CreateItem(m_pDBEssence->item_configs[i].item_id, 0, 1);
			if ( !pIvtrItem )
			{
				continue;
			}
			if ( m_pDBEssence->item_configs[i].item_count > 1 )
			{
				AddDescText(ITEMDESC_COL_YELLOW, true, _AL("%s×%d"), pIvtrItem->GetName(), m_pDBEssence->item_configs[i].item_count);
			}
			else
			{
				AddDescText(ITEMDESC_COL_YELLOW, true, _AL("%s"), pIvtrItem->GetName() );
			}
			delete pIvtrItem;
			pIvtrItem = NULL;
		}
	}

	AddStateDesc(pDescTab);

	//添加过期时间描述
	AddExpireTimeDesc();

	//添加物品价格描述
	AddPriceDesc(white, bRepair);

	//添加额外描述
	AddExtDescText();

	//返回最终的格式化后的Hint描述字符串
	return m_strDesc;
}



///////////////////////////////////////////////////////////////////////////////
//诛仙古卷挖宝解锁道具的实现函数	Added 2012-03-23.
//主要实现：诛仙古卷挖宝解锁道具的构造函数、获得物品当前悬浮提示描述函数
///////////////////////////////////////////////////////////////////////////////
CECIvtrScrollUnlockItem::CECIvtrScrollUnlockItem( int tid, int expire_date ) : CECIvtrItem(tid, expire_date)
{
	m_iCID	= ICID_SCROLL_UNLOCK;

	//	Get database data
	elementdataman* pDB = g_pGame->GetElementDataMan();
	DATA_TYPE DataType;
	m_pDBEssence	= (SCROLL_UNLOCK_ESSENCE*)pDB->get_data_ptr(tid, ID_SPACE_ESSENCE, DataType);
	
	m_iPileLimit	= m_pDBEssence->pile_num_max;
	m_iPrice		= m_pDBEssence->price;
	m_iShopPrice	= m_pDBEssence->shop_price;
	m_iProcType		= m_pDBEssence->proc_type;
	m_iEquipMask	= 0;

	m_bUseable		= true;	//物品能否直接使用
	m_bNeedUpdate	= false;

}

//拷贝构造函数
CECIvtrScrollUnlockItem::CECIvtrScrollUnlockItem(const CECIvtrScrollUnlockItem& s) : CECIvtrItem(s)
{
	m_pDBEssence	= s.m_pDBEssence;
}

//析构函数
CECIvtrScrollUnlockItem::~CECIvtrScrollUnlockItem()
{
}

//从gs服务器获得相应的数据，更新该物品
bool CECIvtrScrollUnlockItem::SetItemInfo(BYTE* pInfoData, int iDataLen)
{
	CECIvtrItem::SetItemInfo(pInfoData, iDataLen);
	return true;	
}

//获得该物品的图标路径
const char* CECIvtrScrollUnlockItem::GetIconFile()
{
	return g_pGame->GetDataPath(m_pDBEssence->file_icon);
}

//获得该物品的名称
const wchar_t* CECIvtrScrollUnlockItem::GetName()
{
	return m_pDBEssence->name;
}

//获得该物品掉落时的简化模型路径
const char * CECIvtrScrollUnlockItem::GetDropModel()
{
	return g_pGame->GetDataPath(m_pDBEssence->file_matter);
}

//获得该物品的基本悬浮提示描述
const wchar_t* CECIvtrScrollUnlockItem::GetNormalDesc(bool bRepair)
{
	m_strDesc = _AL("");//初始化

	CECStringTab* pDescTab = g_pGame->GetItemDesc();
	int white = ITEMDESC_COL_WHITE;
	int name_color = DecideNameCol();

	//添加物品名字描述
	if (m_iCount > 1)
	{
		AddDescText(name_color, true, pDescTab->GetWideString(ITEMDESC_NAMENUMBER), GetName(), m_iCount);
	}
	else
	{
		AddDescText(name_color, true, pDescTab->GetWideString(ITEMDESC_NAME), GetName());
	}

	AddStateDesc(pDescTab);

	//添加过期时间描述
	AddExpireTimeDesc();

	//添加物品价格描述
	AddPriceDesc(white, bRepair);

	//添加额外描述
	AddExtDescText();

	//返回最终的格式化后的Hint描述字符串
	return m_strDesc;
}


///////////////////////////////////////////////////////////////////////////////
//诛仙古卷挖宝增加探索次数道具的实现函数	Added 2012-03-23.
//主要实现：诛仙古卷挖宝增加探索次数道具的构造函数、获得物品当前悬浮提示描述函数
///////////////////////////////////////////////////////////////////////////////
CECIvtrScrollDigCountIncItem::CECIvtrScrollDigCountIncItem( int tid, int expire_date ) : CECIvtrItem(tid, expire_date)
{
	m_iCID	= ICID_SCROLL_DIG_COUNT_INC;

	//	Get database data
	elementdataman* pDB = g_pGame->GetElementDataMan();
	DATA_TYPE DataType;
	m_pDBEssence	= (SCROLL_DIG_COUNT_INC_ESSENCE*)pDB->get_data_ptr(tid, ID_SPACE_ESSENCE, DataType);
	
	m_iPileLimit	= m_pDBEssence->pile_num_max;
	m_iPrice		= m_pDBEssence->price;
	m_iShopPrice	= m_pDBEssence->shop_price;
	m_iProcType		= m_pDBEssence->proc_type;
	m_iEquipMask	= 0;

	m_bUseable		= true;	//物品能否直接使用
	m_bNeedUpdate	= false;

}

//拷贝构造函数
CECIvtrScrollDigCountIncItem::CECIvtrScrollDigCountIncItem(const CECIvtrScrollDigCountIncItem& s) : CECIvtrItem(s)
{
	m_pDBEssence	= s.m_pDBEssence;
}

//析构函数
CECIvtrScrollDigCountIncItem::~CECIvtrScrollDigCountIncItem()
{
}

//从gs服务器获得相应的数据，更新该物品
bool CECIvtrScrollDigCountIncItem::SetItemInfo(BYTE* pInfoData, int iDataLen)
{
	CECIvtrItem::SetItemInfo(pInfoData, iDataLen);
	return true;	
}

//获得该物品的图标路径
const char* CECIvtrScrollDigCountIncItem::GetIconFile()
{
	return g_pGame->GetDataPath(m_pDBEssence->file_icon);
}

//获得该物品的名称
const wchar_t* CECIvtrScrollDigCountIncItem::GetName()
{
	return m_pDBEssence->name;
}

//获得该物品掉落时的简化模型路径
const char * CECIvtrScrollDigCountIncItem::GetDropModel()
{
	return g_pGame->GetDataPath(m_pDBEssence->file_matter);
}

//获得该物品的基本悬浮提示描述
const wchar_t* CECIvtrScrollDigCountIncItem::GetNormalDesc(bool bRepair)
{
	m_strDesc = _AL("");//初始化

	CECStringTab* pDescTab = g_pGame->GetItemDesc();
	int white = ITEMDESC_COL_WHITE;
	int name_color = DecideNameCol();

	//添加物品名字描述
	if (m_iCount > 1)
	{
		AddDescText(name_color, true, pDescTab->GetWideString(ITEMDESC_NAMENUMBER), GetName(), m_iCount);
	}
	else
	{
		AddDescText(name_color, true, pDescTab->GetWideString(ITEMDESC_NAME), GetName());
	}

	AddStateDesc(pDescTab);

	//添加过期时间描述
	AddExpireTimeDesc();

	//添加物品价格描述
	AddPriceDesc(white, bRepair);

	//添加额外描述
	AddExtDescText();

	//返回最终的格式化后的Hint描述字符串
	return m_strDesc;
}