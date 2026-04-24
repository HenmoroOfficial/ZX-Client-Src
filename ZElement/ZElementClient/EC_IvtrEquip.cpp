/*
 * FILE: EC_IvtrEquip.cpp
 *
 * DESCRIPTION: 
 *
 * CREATED BY: Duyuxin, 2004/11/19
 *
 * HISTORY: 
 *
 * Copyright (c) 2004 Archosaur Studio, All Rights Reserved.
 */

#include "EC_Global.h"
#include "EC_IvtrEquip.h"
#include "EC_IvtrTypes.h"
#include "EC_FixedMsg.h"
#include "EC_RTDebug.h"
#ifdef _INVENTORY_DESC_MAN //Added by Ford.W 2010-07-16.//Modified 2010-07-22.
#include "EC_Game_HintTool.h"
#include "EC_GameRun_HintTool.h"
#include "EC_HostPlayer_HintTool.h"
#include "EC_Player_HintTool.h"
#else
#include "EC_Game.h"
#include "EC_GameRun.h"
#include "EC_HostPlayer.h"
#endif
#include "EC_UIManager.h"
#include "EC_GameUIMan.h"
#include "AFI.h"
#include "EC_Inventory.h"
#include "EC_IvtrStone.h"
#include "ElementSkill.h"
#include "elementdataman.h"
#include "EC_GameRecord.h"
#include "ConfigFromLua.h"
#ifndef _INVENTORY_DESC_MAN		//Added by Ford.W 2010-09-09.
#include "EC_Faction.h"
#else
#include "EC_Faction_HintTool.h"
#endif

#include "AChar.h"
#include "AAssist.h"

#include "AUI\\AUICTranslate.h"
#include "AUI\\AUICommon.h"

#include "ElementSkill.h"

#include "EC_IvtrConsume.h"
#include "EC_IvtrRune.h"

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
//	Implement CECIvtrEquip
//	
///////////////////////////////////////////////////////////////////////////

CECIvtrEquip::CECIvtrEquip(int tid, int expire_date) : CECIvtrItem(tid, expire_date)
{
	m_iCID		= ICID_EQUIP;

	//Initialize the member viariable.
	memset(&m_Essence, 0, sizeof(m_Essence)); //Added by Ford.W 2010-09-06.
	//在构造该装备物品时，初始化特殊值-1 Added2011-03-18.
	memset(&m_Essence.gem_slot_shape[0], -1, MAX_GEM_SLOT*sizeof(char));
	memset(&m_Essence.gem_embeded[0], -1, MAX_GEM_SLOT*sizeof(int));
	
	//	Get database data
	elementdataman* pDB = g_pGame->GetElementDataMan();
	DATA_TYPE DataType;
	m_pDBEssence	= (EQUIPMENT_ESSENCE*)pDB->get_data_ptr(tid, ID_SPACE_ESSENCE, DataType);

	if( m_pDBEssence && DataType == DT_EQUIPMENT_ESSENCE )
	{
		m_pDBMajorType	= (EQUIPMENT_MAJOR_TYPE*)pDB->get_data_ptr(m_pDBEssence->id_major_type, ID_SPACE_ESSENCE, DataType);
		m_pDBSubType	= (EQUIPMENT_SUB_TYPE*)pDB->get_data_ptr(m_pDBEssence->id_sub_type, ID_SPACE_ESSENCE, DataType);

//		memset(&m_Essence, 0, sizeof(m_Essence)); //Canceled by Ford.W 2010-09-06.

		m_iPileLimit	= m_pDBEssence->pile_num_max;
		m_iPrice		= m_pDBEssence->price;
		m_iShopPrice	= m_pDBEssence->shop_price;
		m_iLevelReq		= m_pDBEssence->require_level;
		m_iLevelReqDrop = 0;
		m_iProfReq		= m_pDBEssence->character_combo_id;
		m_iProfReq2		= m_pDBEssence->character_combo_id2; // Added 2011-07-18.
		m_iGenderReq	= m_pDBEssence->require_gender;
		m_iRaceReq		= m_pDBEssence->require_race;
		m_iRebornReq	= m_pDBEssence->renascence_count;
		m_iEquipMask	= m_pDBEssence->equip_mask;
		m_iProcType		= m_pDBEssence->proc_type;
	}
	else
		m_pDBEssence = NULL;
}

CECIvtrEquip::CECIvtrEquip(const CECIvtrEquip& s) : CECIvtrItem(s)
{
	m_pDBMajorType	= s.m_pDBMajorType;
	m_pDBSubType	= s.m_pDBSubType;
	m_pDBEssence	= s.m_pDBEssence;
	m_Essence		= s.m_Essence;
	m_iLevelReq		= s.m_iLevelReq;
	m_iLevelReqDrop	= s.m_iLevelReqDrop;
	m_iProfReq		= s.m_iProfReq;
	m_iProfReq2		= s.m_iProfReq2; // Added 2011-07-18.
	m_iGenderReq	= s.m_iGenderReq;
	m_iRaceReq		= s.m_iRaceReq;
	m_iRebornReq	= s.m_iRebornReq;
	m_strMaker		= s.GetMakerName();
}

CECIvtrEquip::~CECIvtrEquip()
{
}

//	Set item detail information
bool CECIvtrEquip::SetItemInfo(BYTE* pInfoData, int iDataLen)
{
	CECIvtrItem::SetItemInfo(pInfoData, iDataLen);
	
	if( iDataLen >= sizeof(m_Essence) )//全部数据， 包括新加的星座相关数据//Modified 2012-08-27.
	{
		memcpy(&m_Essence, pInfoData, sizeof(m_Essence));
	}
	else if( iDataLen >= sizeof(IVTR_ESSENCE_EQUIPMENT_NEW_FOR_GEM) )//之前的全部数据, 包括与宝石镶嵌相关的数据//Modified 2012-08-27.
	{
		memcpy(&m_Essence, pInfoData, sizeof(IVTR_ESSENCE_EQUIPMENT_NEW_FOR_GEM));
	}
	else if( iDataLen >= sizeof(IVTR_ESSENCE_EQUIPMENT_NEW) )//之前的全部数据
	{
		memcpy(&m_Essence, pInfoData, sizeof(IVTR_ESSENCE_EQUIPMENT_NEW));
	}
	else if (iDataLen >= sizeof(m_Essence.old_data))//老数据
	{
		memcpy(&m_Essence, pInfoData, sizeof(m_Essence.old_data));
	}
		
	if( m_Essence.old_data.num_pstone )
	{
		DATA_TYPE dt;
		PSTONE_ESSENCE * pPStone = (PSTONE_ESSENCE *) g_pGame->GetElementDataMan()->get_data_ptr(m_Essence.old_data.id_pstone,
			ID_SPACE_ESSENCE, dt);

		if( dt == DT_PSTONE_ESSENCE )
		{
			if( pPStone->type == 1 )
			{
				m_iLevelReqDrop = m_Essence.old_data.num_pstone * 1;
			}
		}
	}

	return true;
}

void CECIvtrEquip::UpdateSpritePower(short dec, int result)
{
	m_Essence.cur_spirit_power -= dec;
	if (m_Essence.cur_spirit_power != result)
		m_Essence.cur_spirit_power = result;
}

GNET::Octets CECIvtrEquip::gen_item_info()
{
	return GNET::Octets(&m_Essence, sizeof(m_Essence));
}

//	Get item icon file name
const char* CECIvtrEquip::GetIconFile()
{
	return g_pGame->GetDataPath(m_pDBEssence->file_icon);
}

//	Get item name
const wchar_t* CECIvtrEquip::GetName()
{
	return m_pDBEssence->name;
}

//	Get equipment type
int CECIvtrEquip::GetEquipmentType() const
{
	return m_pDBEssence ? m_pDBEssence->equip_type : EQUIP_UNKNOWN;
}

//	Get scaled item
INT64 CECIvtrEquip::GetScaledPrice()
{
	if (m_iScaleType != SCALE_SELL)
		return CECIvtrItem::GetScaledPrice();

	int iPrice = m_iPrice * m_iCount;
	return iPrice;
}

//	Get item description for booth buying
const wchar_t* CECIvtrEquip::GetBoothBuyDesc()
{
	m_strDesc = _AL("");

	CECStringTab* pDescTab = g_pGame->GetItemDesc();

	int white = ITEMDESC_COL_WHITE;

	//	Item name
	AddDescText(white, true, pDescTab->GetWideString(ITEMDESC_NAME), GetName());

	//	Price
	AddPriceDesc(white, false);

	return m_strDesc;
}

//	Add price description
void CECIvtrEquip::AddPriceDesc(int col, bool bRepair)
{
	CECStringTab* pDescTab = g_pGame->GetItemDesc();
	if (bRepair)
		AddDescText(col, false, pDescTab->GetWideString(ITEMDESC_REPAIRCOST), GetRepairCost());
	else
		CECIvtrItem::AddPriceDesc(col, bRepair);
}

//	Decide equipment name color
int CECIvtrEquip::DecideNameCol()
{
	int col = ITEMDESC_COL_WHITE;

	if( m_Essence.old_data.num_estone )
		col = ITEMDESC_EQUIPSTONE_COLOR1 + m_Essence.old_data.num_estone - 1;
	
	if (col == ITEMDESC_COL_WHITE)
		col = CECIvtrItem::DecideNameCol();

	return col;
}

//	Build add-ons properties description
void CECIvtrEquip::BuildAddOnPropDesc()
{
	int	id_addons[32];
	int iNumAddon = 0;
	int i = 0;
	if( m_pDBEssence->id_addon1 )
		id_addons[iNumAddon ++] = m_pDBEssence->id_addon1;
	if( m_pDBEssence->id_addon2 )
		id_addons[iNumAddon ++] = m_pDBEssence->id_addon2;
	if( m_pDBEssence->id_addon3 )
		id_addons[iNumAddon ++] = m_pDBEssence->id_addon3;
	if( m_pDBEssence->id_addon4 )
		id_addons[iNumAddon ++] = m_pDBEssence->id_addon4;
	if( m_pDBEssence->id_addon5 )
		id_addons[iNumAddon ++] = m_pDBEssence->id_addon5;
	
	if (m_Essence.old_data.id_addon1)
		id_addons[iNumAddon ++] = m_Essence.old_data.id_addon1;

	if (m_Essence.old_data.id_addon2)
		id_addons[iNumAddon ++] = m_Essence.old_data.id_addon2;

	// 精炼附加属性
	for (int j=0; j<3; ++j)
	{
		if (m_pDBEssence->hidden_addon[j].level <= m_Essence.old_data.num_estone)
			id_addons[iNumAddon ++] = m_pDBEssence->hidden_addon[j].addon;
	}

	if (!iNumAddon)
		return;

	CECStringTab* pDescTab = g_pGame->GetItemDesc();
	int color = ITEMDESC_COL_LIGHTBLUE;

	for (i=0; i < iNumAddon; i++)
	{
		DATA_TYPE dt;
		EQUIPMENT_ADDON * pAddOn = (EQUIPMENT_ADDON *) g_pGame->GetElementDataMan()->get_data_ptr(id_addons[i], ID_SPACE_ADDON, dt);
		if( dt != DT_EQUIPMENT_ADDON )
			continue;

		AddDescText(color, true, _AL("%s"), GetEquipAddonDesc(pAddOn->id, pAddOn->type, pAddOn->param1, pAddOn->param2, pAddOn->param3, 1));
	}
}

// 装备隐藏属性的显示
void CECIvtrEquip::BuildHiddenAddOn()
{
	abase::vector<Level2Addon> vecLevel2Addon;

	size_t i;
	for (i =0; i<2; ++i)
	{
		Level2Addon level2addon;
		level2addon.level = m_pDBEssence->hidden_prop[i].level;
		level2addon.addon = 0;
		if(m_pDBEssence->hidden_prop[i].level && m_pDBEssence->hidden_prop[i].level > m_Essence.old_data.num_estone)
		{
			for (int j=0; j<5; ++j)
			{
				if (m_pDBEssence->hidden_prop[i].prop[j].probability == 1.0f)
				{					
					level2addon.addon = m_pDBEssence->hidden_prop[i].prop[j].id;
					break;
				}
			}
			if (level2addon.addon == 0)
				level2addon.addon= -1;     // 有随机属性
			if (vecLevel2Addon.size() == 0)
				vecLevel2Addon.push_back(level2addon);
			else
			{
				size_t m;
				for(m=0; m<vecLevel2Addon.size(); ++m)
				{
					if (level2addon.level < vecLevel2Addon[m].level)
					{
						vecLevel2Addon.insert(vecLevel2Addon.begin()+m, level2addon);
						break;
					}
				}
				if (m == vecLevel2Addon.size())
					vecLevel2Addon.push_back(level2addon);
			}
		}
	}

	for (i =0; i <3; i++)
	{
		Level2Addon level2addon;
		if (m_pDBEssence->hidden_addon[i].level > m_Essence.old_data.num_estone)
		{
			level2addon.level = m_pDBEssence->hidden_addon[i].level;
			level2addon.addon = m_pDBEssence->hidden_addon[i].addon;

			if (vecLevel2Addon.size() == 0)
				vecLevel2Addon.push_back(level2addon);
			else
			{
				size_t m;
				for(m=0; m<vecLevel2Addon.size(); ++m)
				{
					if (level2addon.level < vecLevel2Addon[m].level)
					{
						vecLevel2Addon.insert(vecLevel2Addon.begin()+m, level2addon);
						break;
					}	
				}
				if (m == vecLevel2Addon.size())
					vecLevel2Addon.push_back(level2addon);
			}
		}
	}

	CECStringTab* pDescTab = g_pGame->GetItemDesc();
	int color = ITEMDESC_COL_GRAY;

	for(i =0; i<vecLevel2Addon.size(); ++i)
	{
		if (vecLevel2Addon[i].addon > 0)
		{
			DATA_TYPE dt;
			EQUIPMENT_ADDON * pAddOn = (EQUIPMENT_ADDON *) g_pGame->GetElementDataMan()->get_data_ptr(vecLevel2Addon[i].addon, ID_SPACE_ADDON, dt);
			if( dt != DT_EQUIPMENT_ADDON )
				continue;
			AddDescText(color, true, _AL("(+%d)%s"), vecLevel2Addon[i].level, GetEquipAddonDesc(pAddOn->id, pAddOn->type, pAddOn->param1, pAddOn->param2, pAddOn->param3, 1));
		}
		else if (vecLevel2Addon[i].addon < 0)
		{
			AddDescText(color, true, pDescTab->GetWideString(ITEMDESC_HIDDEN_ADDON), vecLevel2Addon[i].level);
		}
	}
}

//	Build gender requirement description
void CECIvtrEquip::BuildGenderReqDesc()
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

void CECIvtrEquip::BuildRaceReqDesc()
{
	if(m_iRaceReq == 2)
		return;

	CECStringTab* pDescTab = g_pGame->GetItemDesc();
	CECHostPlayer* pHost = g_pGame->GetGameRun()->GetHostPlayer();

	int col;

	CECStringTab* pFixMsg = g_pGame->GetFixedMsgTab();
	if( (pHost->GetRace() == RACE_HUMAN && m_iRaceReq == 0) ||
		( pHost->GetRace() == RACE_ORC && m_iRaceReq ==1) ||
		(pHost->GetRace() == RACE_EVILKIND && m_iRaceReq == 3))
		col = ITEMDESC_COL_WHITE;
	else
		col = ITEMDESC_COL_RED;

	AddDescText(col, false, pDescTab->GetWideString(ITEMDESC_RACEREQ));
	m_strDesc += _AL(" ");
	if(m_iRaceReq ==0)
		AddDescText(col, true, pFixMsg->GetWideString(FIXMSG_RACE_HUMAN));
	else if(m_iRaceReq == 1)
		AddDescText(col, true, pFixMsg->GetWideString(FIXMSG_RACE_ORC));
	else if(m_iRaceReq == 3)
		AddDescText(col, true, pFixMsg->GetWideString(FIXMSG_RACE_MONSTER));
}

void CECIvtrEquip::BuildTerrReqDesc()
{
	if (m_pDBEssence->require_territory == 0)
		return;
	
	CECStringTab* pDescTab = g_pGame->GetItemDesc();
	CECHostPlayer* pHost = g_pGame->GetGameRun()->GetHostPlayer();
	// 社稷需求
	int iTerrReq = m_pDBEssence->require_territory;
	int col = ITEMDESC_COL_RED;
	Faction_Info* pInfo = g_pGame->GetFactionMan()->GetFaction(pHost->GetFactionID());
	if (pInfo)
	{
		col = pInfo->GetTerritoryScore() >= iTerrReq ? ITEMDESC_COL_WHITE : ITEMDESC_COL_RED;
	}
	int iIndex = 0;
	switch(iTerrReq)
	{
	case 10:
		iIndex = 1;
		break;
	case 20:
		iIndex = 2;
		break;
	case 40:
		iIndex = 3;
		break;
	case 70:
		iIndex = 4;
		break;
	case 100:
		iIndex = 5;
		break;
	case 130:
		iIndex = 6;
		break;
	case 160:
		iIndex = 7;
		break;
	case 190:
		iIndex = 8;
		break;
	case 230:
		iIndex = 9;
		break;
	case 270:
		iIndex = 10;
		break;
	case 310:
		iIndex = 11;
		break;
	case 350:
		iIndex = 12;
		break;
	case 390:
		iIndex = 13;
		break;
	case 760:
		iIndex = 14;
		break;
	default:
		iIndex = 14;
	}
	AddDescText(col, true, pDescTab->GetWideString(ITEMDESC_TERRITORY_REQ), pDescTab->GetWideString(ITEMDESC_TERRITORY_REQ+iIndex));

}

//	Get item description text
const wchar_t* CECIvtrEquip::GetNormalDesc(bool bRepair)
{
	const wchar_t* szDesc = _AL("");

	szDesc = GetEquipmentDesc(bRepair);
	return szDesc;
}

//	Build equipment description text
const wchar_t* CECIvtrEquip::GetEquipmentDesc(bool bRepair)
{
	if (m_bNeedUpdate)
		return NULL;

	m_strDesc = _AL("");

	// Build addon desc and save it.
	BuildAddOnPropDesc();
	AWString strAddon = m_strDesc;
	m_strDesc = _AL("");

	CECStringTab* pDescTab = g_pGame->GetItemDesc();
	CECStringTab* pFixMsg = g_pGame->GetFixedMsgTab();
	CECHostPlayer* pHost = g_pGame->GetGameRun()->GetHostPlayer();
	
	int lblue = ITEMDESC_COL_LIGHTBLUE;
	int white = ITEMDESC_COL_WHITE;
	int red = ITEMDESC_COL_RED;
	int gray = ITEMDESC_COL_GRAY;
	int namecol = DecideNameCol();
	int col = white;

	EQUIPMENT_ESSENCE enhanced;
	int iEnhancedVal = 0;
	memset(&enhanced, 0, sizeof(enhanced));
	if( m_Essence.old_data.num_estone )
	{
		DATA_TYPE dt;
		ESTONE_ESSENCE * pEStone = (ESTONE_ESSENCE *) g_pGame->GetElementDataMan()->get_data_ptr(m_Essence.old_data.id_estone,
			ID_SPACE_ESSENCE, dt);

		if( dt == DT_ESTONE_ESSENCE )
		{
			for(int i=0; i<4; i++)
			{
				if( (pEStone->effects[i].equip_mask & GetDBEssence()->equip_mask) && pEStone->effects[i].effect_addon_type > 0 )
				{
					EnhanceWithEquipAddOn(enhanced, pEStone->effects[i].effect_addon_type, m_pDBEssence->ehanced_value[m_Essence.old_data.num_estone - 1]);
					iEnhancedVal = m_pDBEssence->ehanced_value[m_Essence.old_data.num_estone-1];
					break;
				}
			}
		}
	}

	//	Item name
	AddDescText(namecol, false, pDescTab->GetWideString(ITEMDESC_NAME), GetName());

	if( m_Essence.old_data.num_estone )
		AddDescText(namecol, false, pDescTab->GetWideString(ITEMDESC_ENAME), m_Essence.old_data.num_estone);

	if( m_Essence.old_data.num_pstone )	
		AddDescText(namecol, false, pDescTab->GetWideString(ITEMDESC_PNAME), m_Essence.old_data.num_pstone);

	AddDescText(namecol, true, _AL(""));

	//添加装备-饰品的插槽和镶嵌宝石悬浮提示信息 Added 2011-03-18.
#ifndef _INVENTORY_DESC_MAN //处理小工具所需代码和游戏本身代码 Added 2011-05-24
	CECGameUIMan *pGameUIMan = g_pGame->GetGameRun()->GetUIManager()->GetInGameUIMan();
#endif
	for( int nIndex = 0; nIndex < MAX_GEM_SLOT; ++nIndex )
	{
		if( -1 != m_Essence.gem_slot_shape[nIndex] && m_Essence.reserved1 == 0 )//判断该插槽是否已经被鉴定，未被鉴定时为-1  // 目前如果reserved1 != 0 代表时装染色物品
		{
			if( -1 == m_Essence.gem_embeded[nIndex] )//该插槽还没有镶嵌宝石
			{
				//显示形状属性名字
#ifndef _INVENTORY_DESC_MAN //处理小工具所需代码和游戏本身代码 Added 2011-05-24
				AddDescImage(false, CECGameUIMan::ICON_HINT_GEM, CECIvtrItem::HINT_GEM_SLOTSHAPE1+m_Essence.gem_slot_shape[nIndex]);
#endif
				int index = ITEMDESC_GEM_SLOT_SHAPE1 + m_Essence.gem_slot_shape[nIndex];
				AddDescText(ITEMDESC_COL_GRAY, true, pDescTab->GetWideString(ITEMDESC_SLOT_TYPE), pDescTab->GetWideString(index));
			}
			else//已经镶嵌了宝石
			{
#ifndef _INVENTORY_DESC_MAN //处理小工具所需代码和游戏本身代码 Added 2011-05-24
				CECIvtrGem* pGem = (CECIvtrGem*)CECIvtrItem::CreateItem(m_Essence.gem_embeded[nIndex], 0, 1);
				if (pGem)
				{
					AString strName;
					af_GetFileTitle(g_pGame->GetDataPath(pGem->GetDBEssence()->file_shape_icon), strName);
					strName.MakeLower();
					AddDescImage(false, CECGameUIMan::ICON_HINT_GEM, pGameUIMan->GetIconId(CECGameUIMan::ICONS_GEM16, strName));
					delete pGem;
				}
#endif
				DATA_TYPE dt;
				GEM_ESSENCE* pGemEssence = (GEM_ESSENCE *)g_pGame->GetElementDataMan()->get_data_ptr(m_Essence.gem_embeded[nIndex], ID_SPACE_ESSENCE, dt);
/*				if( dt != DT_GEM_ESSENCE )
				{
					AddDescText(ITEMDESC_COL_GRAY, false, pDescTab->GetWideString(ITEMDESC_NAME), _AL("未知宝石"));
				}
				else
				{
					AddDescText(ITEMDESC_COL_LIGHTBLUE, false, pDescTab->GetWideString(ITEMDESC_NAME), pGemEssence->name);
				}

*/				int nCurIndex;
				for( nCurIndex = 0; nCurIndex < 6; ++nCurIndex )
				{
					//首先获得该附加属性的模板
					DATA_TYPE dt;	
					EQUIPMENT_ADDON * pAddOn = NULL;
					if( 0 == pGemEssence->addon_property[nCurIndex].addon_type )//基本属性
					{
						pAddOn = (EQUIPMENT_ADDON *) g_pGame->GetElementDataMan()->get_data_ptr(pGemEssence->addon_property[nCurIndex].addon_id, ID_SPACE_ADDON, dt);
						if( dt != DT_EQUIPMENT_ADDON )
						{
							continue;
						}
						//根据获得的附加属性模板添加描述
						AddDescText(ITEMDESC_COL_LIGHTBLUE, true, _AL("%s"), GetEquipAddonDesc(pAddOn->id, pAddOn->type, pAddOn->param1, pAddOn->param2, pAddOn->param3, 1));
					}
					
				}
				
				//显示插槽属性
				//显示形状属性名字
				//AddDescImage(false, CECGameUIMan::ICON_HINT_GEM, CECIvtrItem::HINT_GEM_SLOTSHAPE1+m_Essence.gem_slot_shape[nIndex]);
				int iColor = (pGemEssence->shape_type==m_Essence.gem_slot_shape[nIndex] ||
					0==m_Essence.gem_slot_shape[nIndex] || 0==pGemEssence->shape_type) ? ITEMDESC_COL_LIGHTBLUE : ITEMDESC_COL_GRAY;
				for( nCurIndex = 0; nCurIndex < 6; ++nCurIndex )
				{
					//首先获得该附加属性的模板
					DATA_TYPE dt;	
					EQUIPMENT_ADDON * pAddOn = NULL;
					if( 1 == pGemEssence->addon_property[nCurIndex].addon_type )//镶嵌属性
					{
						pAddOn = (EQUIPMENT_ADDON *) g_pGame->GetElementDataMan()->get_data_ptr(pGemEssence->addon_property[nCurIndex].addon_id, ID_SPACE_ADDON, dt);
						if( dt != DT_EQUIPMENT_ADDON )
						{
							continue;
						}
						int index = ITEMDESC_GEM_SLOT_SHAPE1 + m_Essence.gem_slot_shape[nIndex];
						//根据获得的附加属性模板添加描述
						AddDescText(iColor, true, pDescTab->GetWideString(ITEMDESC_SLOT_AWARD), pDescTab->GetWideString(index),
							GetEquipAddonDesc(pAddOn->id, pAddOn->type, pAddOn->param1, pAddOn->param2, pAddOn->param3, 1));
					}
					
				}
				
			}
		}
		
	}


	// 如果进行了血祭
	if( m_Essence.master_id)
	{
		AddBleedSacrificeDesc(iEnhancedVal);
	}

	//  镶嵌器魄的属性加成描述
	AddSoulEmbedDesc();
	
	// 器魄套装描述
	if (m_Essence.soul_rune_word != 0)
		AddSoulSuiteDesc();	

	// 打孔和星座描述
	if (m_Essence.curr_hole != 0)
		AddAstrologyDesc();

	AddStateDesc(pDescTab);

	if (m_pDBEssence->proc_type & (1 << 4))
		AddDescText(white, true, _AL("%s"), pDescTab->GetWideString(ITEMDESC_CANT_TRADE));

	AddExpireTimeDesc();

	//	Sub class name
	AddDescText(white, true, pDescTab->GetWideString(ITEMDESC_CLASSNAME), m_pDBSubType->name);

	//	level
	if( m_pDBEssence->show_level[0] )
		AddDescText(-1, true, _AL("%s"), m_pDBEssence->show_level);

	//	Attack distance
	if( m_pDBEssence->attack_range )
		AddDescText(white, true, pDescTab->GetWideString(ITEMDESC_ATKDISTANCE), m_pDBEssence->attack_range);

	//	damage
	if (enhanced.min_dmg || enhanced.max_dmg)
	{
		AddDescText(lblue, true, pDescTab->GetWideString(ITEMDESC_PHYDAMAGE), m_pDBEssence->min_dmg + enhanced.min_dmg, m_pDBEssence->max_dmg + enhanced.max_dmg);
	}
	else if (m_pDBEssence->min_dmg || m_pDBEssence->max_dmg)
	{
		AddDescText(white, true, pDescTab->GetWideString(ITEMDESC_PHYDAMAGE), m_pDBEssence->min_dmg, m_pDBEssence->max_dmg);
	}

	//	Physical defence
	if (enhanced.defence)
	{
		AddDescText(lblue, true, pDescTab->GetWideString(ITEMDESC_PHYDEFENCE), m_pDBEssence->defence + enhanced.defence);
	}
	else if (m_pDBEssence->defence)
	{
		AddDescText(white, true, pDescTab->GetWideString(ITEMDESC_PHYDEFENCE), m_pDBEssence->defence);
	}

	//	hp
	if( enhanced.hp )
		AddDescText(lblue, true, pDescTab->GetWideString(ITEMDESC_HPEXTRA), m_pDBEssence->hp + enhanced.hp);
	else if( m_pDBEssence->hp )
		AddDescText(white, true, pDescTab->GetWideString(ITEMDESC_HPEXTRA), m_pDBEssence->hp);

	//	mp
	if( enhanced.mp )
		AddDescText(lblue, true, pDescTab->GetWideString(ITEMDESC_MPEXTRA), m_pDBEssence->mp + enhanced.mp);
	else if( m_pDBEssence->mp )
		AddDescText(white, true, pDescTab->GetWideString(ITEMDESC_MPEXTRA), m_pDBEssence->mp);
	
	//	dp 神力 Added 2011-08-05.
	if ( enhanced.dp )
	{
		AddDescText(lblue, true, pDescTab->GetWideString(ITEMDESC_DPEXTRA), m_pDBEssence->dp + enhanced.dp);
	}
	else if ( m_pDBEssence->dp )
	{
		AddDescText(white, true, pDescTab->GetWideString(ITEMDESC_DPEXTRA), m_pDBEssence->dp);
	}

	//	deity_power 神圣力 Added 2011-08-05.
	if ( enhanced.deity_power )
	{
		AddDescText(lblue, true, pDescTab->GetWideString(ITEMDESC_DEITY_POWER), m_pDBEssence->deity_power + enhanced.deity_power);
	}
	else if ( m_pDBEssence->deity_power )
	{
		AddDescText(white, true, pDescTab->GetWideString(ITEMDESC_DEITY_POWER), m_pDBEssence->deity_power );
	}

	//	Attack rating
	if (enhanced.attack)
	{
		AddDescText(lblue, true, pDescTab->GetWideString(ITEMDESC_ATKRATING), m_pDBEssence->attack + enhanced.attack);
	}
	else if (m_pDBEssence->attack)
	{
		AddDescText(white, true, pDescTab->GetWideString(ITEMDESC_ATKRATING), m_pDBEssence->attack);
	}

	//	Dodge
	if (enhanced.armor)
	{
		AddDescText(lblue, true, pDescTab->GetWideString(ITEMDESC_DODGE), m_pDBEssence->armor + enhanced.armor);
	}
	else if (m_pDBEssence->armor)
	{
		AddDescText(white, true, pDescTab->GetWideString(ITEMDESC_DODGE), m_pDBEssence->armor);
	}

	if (m_pDBEssence->renascence_count > 0)
	{
		int col = (pHost->GetRebornCount() >= m_pDBEssence->renascence_count) ? ITEMDESC_COL_WHITE : ITEMDESC_COL_RED;
		AddDescText(col, true, pDescTab->GetWideString(ITEMDESC_RENASCENCE_REQUIRED), pDescTab->GetWideString(ITEMDESC_RENASCENCE_1+m_pDBEssence->renascence_count-1));
	}
	// race requirement
	BuildRaceReqDesc();
	//	Profession requirement
	if (m_pDBEssence->sect_mask == 0)
		AddProfReqDesc(m_iProfReq, m_iProfReq2);
	else
	{
		static const unsigned int uAllSect = (1 << NUM_SECT) - 1;
		static const unsigned int uHumanSect = (1 << NUM_HUMAN_SECT) - 1;
		static const unsigned int uOrcSect = uAllSect - uHumanSect;
		CECPlayer::SECT_LEVEL2_INFO* pInfo = CECPlayer::GetSectInfoByProf(pHost->GetProfession());

//		if (m_pDBEssence->sect_mask != uAllSect)
		if ( (m_pDBEssence->require_race == 0 && m_pDBEssence->sect_mask == uHumanSect) ||
			 (m_pDBEssence->require_race == 1 && m_pDBEssence->sect_mask == uOrcSect) ||
			 (m_pDBEssence->require_race == 2 && m_pDBEssence->sect_mask == uAllSect))
		{
			// 不显示
		}
		else
		{
			int col = (pInfo && (pInfo->mask_sect & m_pDBEssence->sect_mask)) ? ITEMDESC_COL_WHITE : ITEMDESC_COL_RED;
			AddDescText(col, false, pDescTab->GetWideString(ITEMDESC_SECT_REQUIRE));

			for (int i = 0; i < NUM_SECT; i++)
			{
				if (m_pDBEssence->sect_mask & (1 << i))
				{
					m_strDesc += _AL(" ");
					AddDescText(col, false, pFixMsg->GetWideString(FIXMSG_SECT_1 + i));
				}
			}

			m_strDesc += _AL("\\r");
		}

//      前世门派 隔世门派 跨世门派 悬浮提示先去掉
// 		if (m_pDBEssence->renascence_count > 0)
// 		{
// //			if (m_pDBEssence->sect_mask_1 != uAllSect)
// 			if ( (m_pDBEssence->require_race == 0 && m_pDBEssence->sect_mask_1 == uHumanSect) ||
// 				 (m_pDBEssence->require_race == 1 && m_pDBEssence->sect_mask_1 == uOrcSect) ||
// 				 (m_pDBEssence->require_race == 2 && m_pDBEssence->sect_mask_1 == uAllSect))
// 			{
// 				// 不显示
// 			}
// 			else
// 			{
// 				AddDescText(ITEMDESC_COL_WHITE, false, pDescTab->GetWideString(ITEMDESC_SECT1_REQUIRE));
// 
// 				for (int i = 0; i < NUM_SECT; i++)
// 				{
// 					if (m_pDBEssence->sect_mask_1 & (1 << i))
// 					{
// 						m_strDesc += _AL(" ");
// 						AddDescText(ITEMDESC_COL_WHITE, false, pFixMsg->GetWideString(FIXMSG_SECT_1 + i));
// 					}
// 				}
// 
// 				m_strDesc += _AL("\\r");
// 			}
// 		}
// 
// 		if (m_pDBEssence->renascence_count > 1)
// 		{
// //			if (m_pDBEssence->sect_mask_2 != uAllSect)
// 			if ( (m_pDBEssence->require_race == 0 && m_pDBEssence->sect_mask_2 == uHumanSect) ||
// 				 (m_pDBEssence->require_race == 1 && m_pDBEssence->sect_mask_2 == uOrcSect) ||
// 				 (m_pDBEssence->require_race == 2 && m_pDBEssence->sect_mask_2 == uAllSect))
// 			{
// 				// 不显示
// 			}
// 			else
// 			{
// 				AddDescText(ITEMDESC_COL_WHITE, false, pDescTab->GetWideString(ITEMDESC_SECT2_REQUIRE));
// 
// 				for (int i = 0; i < NUM_SECT; i++)
// 				{
// 					if (m_pDBEssence->sect_mask_2 & (1 << i))
// 					{
// 						m_strDesc += _AL(" ");
// 						AddDescText(ITEMDESC_COL_WHITE, false, pFixMsg->GetWideString(FIXMSG_SECT_1 + i));
// 					}
// 				}
// 
// 				m_strDesc += _AL("\\r");
// 			}
// 		}
// 
// 		if (m_pDBEssence->renascence_count > 2)
// 		{
// //			if (m_pDBEssence->sect_mask_3 != uAllSect)
// 			if ( (m_pDBEssence->require_race == 0 && m_pDBEssence->sect_mask_3 == uHumanSect) ||
// 				 (m_pDBEssence->require_race == 1 && m_pDBEssence->sect_mask_3 == uOrcSect) ||
// 				 (m_pDBEssence->require_race == 2 && m_pDBEssence->sect_mask_3 == uAllSect))
// 			{
// 				// 不显示
// 			}
// 			else
// 			{
// 				AddDescText(ITEMDESC_COL_WHITE, false, pDescTab->GetWideString(ITEMDESC_SECT3_REQUIRE));
// 
// 				for (int i = 0; i < NUM_SECT; i++)
// 				{
// 					if (m_pDBEssence->sect_mask_3 & (1 << i))
// 					{
// 						m_strDesc += _AL(" ");
// 						AddDescText(ITEMDESC_COL_WHITE, false, pFixMsg->GetWideString(FIXMSG_SECT_1 + i));
// 					}
// 				}
// 
// 				m_strDesc += _AL("\\r");
// 			}
// 		}

		if (m_pDBEssence->require_level2 > 0)
		{
			int col = (pInfo && (pInfo->level2 >= m_pDBEssence->require_level2)) ? ITEMDESC_COL_WHITE : ITEMDESC_COL_RED;
			AddDescText(col, true, pDescTab->GetWideString(ITEMDESC_LEVEL2_REQUIRE), pFixMsg->GetWideString(FIXMSG_LEVEL2_1 + m_pDBEssence->require_level2));
		}
	}

	if (m_pDBEssence->god_devil_mask)
	{
		int count = 0;
		AWString str;

		for (int i = 0; i < 6; i++)
		{
			if (m_pDBEssence->god_devil_mask & (1 << i))
			{
				str += pFixMsg->GetWideString(FIXMSG_GOD_DEVIL_1+i);
				count++;
			}
		}

		if (count >= 2)
			str += pDescTab->GetWideString(ITEMDESC_GOD_DEVIL_SUFFIX2+count-2);

		AddDescText((pHost->GetCultivation() & m_pDBEssence->god_devil_mask) ? white : red, true, pDescTab->GetWideString(ITEMDESC_GOD_DEVIL_REQUIRE), str);
	}

	if (m_pDBEssence->title_require)
	{
		CConfigFromLua* pConfigFromLua = g_pGame->GetConfigFromLua();
		CConfigFromLua::TitleDefTab::iterator it = pConfigFromLua->m_TitleDef.find(m_pDBEssence->title_require);

		if (it != pConfigFromLua->m_TitleDef.end())
		{
			AWString title = *it.value();

			if (title.GetLength() >= 7)
			{
				if (title[0] == _AL('^'))
					title = title.Right(title.GetLength() - 7);
			}
			// 过滤图形称号中的图片路径
			if(title.Find(_AL("#image")) >=0 || title.Find(_AL("&image")) >=0 || title.Find(_AL("#anim")) >=0 || title.Find(_AL("&anim")) >=0)
			{
				int i = 0;
				while(title[i] != _AL('#') && title[i] != _AL('&')) i++;
				title = title.Left(i);
			}
			AddDescText(pHost->HasTitle(m_pDBEssence->title_require) ? white : red, true, pDescTab->GetWideString(ITEMDESC_TITLE_REQUIRE), title);
		}
	}

	for (int n = 0; n < 2; n++)
	{
		if (m_pDBEssence->require_repu[n].index >= 0 && m_pDBEssence->require_repu[n].num > 0)
		{	
			col = pHost->GetRegionReputation(m_pDBEssence->require_repu[n].index) >= m_pDBEssence->require_repu[n].num ? white : red;
			AddDescText(col, true, pDescTab->GetWideString(ITEMDESC_EQUIP_REPU_REQUIRE),
				pDescTab->GetWideString(ITEMDESC_EQUIP_REPU1+m_pDBEssence->require_repu[n].index), m_pDBEssence->require_repu[n].num);
		}
	}

	//	Level requirment
	int iLevelReq = m_iLevelReq - m_iLevelReqDrop;
	col = pHost->GetBasicProps().iLevel >= iLevelReq ? white : red;
	AddDescText(col, true, pDescTab->GetWideString(ITEMDESC_LEVELREQ), iLevelReq);
	
	BuildTerrReqDesc();

	
	// 器魄，灵性等级
	if (m_pDBEssence->blood_sacrifice && m_pDBEssence->soul_level)
	{
		AddDescText(lblue, true, pDescTab->GetWideString(ITEMDESC_SOUL_LEVEL), m_pDBEssence->soul_level);
	}

	//	Gender requirement
	BuildGenderReqDesc();

	//	Add addon properties
	if (strAddon.GetLength())
		m_strDesc += strAddon;

	//	Display pstone and sstone stones properties
	if( m_Essence.old_data.num_pstone )
	{
		DATA_TYPE dt;
		PSTONE_ESSENCE * pPStone = (PSTONE_ESSENCE *) g_pGame->GetElementDataMan()->get_data_ptr(m_Essence.old_data.id_pstone,
			ID_SPACE_ESSENCE, dt);

		if( dt == DT_PSTONE_ESSENCE )
		{
			if( pPStone->type == 0 )
			{
				EQUIPMENT_ADDON * pAddOn = (EQUIPMENT_ADDON *) g_pGame->GetElementDataMan()->get_data_ptr(pPStone->effect_addon_id,
					ID_SPACE_ADDON, dt);
				if( dt == DT_EQUIPMENT_ADDON && pAddOn->type > 0 )
				{
					AddDescText(lblue, true, _AL("%s%s"), pDescTab->GetWideString(ITEMDESC_PSTONEADDON_PREFIX), GetEquipAddonDesc(pAddOn->id, pAddOn->type, pAddOn->param1, pAddOn->param2, pAddOn->param3, m_Essence.old_data.num_pstone));
				}
			}
			else if( pPStone->type == 1 )
			{
				AddDescText(lblue, true, pDescTab->GetWideString(ITEMDESC_LEVELREQ_DROP), m_Essence.old_data.num_pstone * 1);
			}
		}
	}

	BuildHiddenAddOn();
	//	Price
	AddPriceDesc(white, bRepair);

	//	Maker's info
	if (m_strMaker.GetLength())
	{
		m_strDesc += _AL("\\r");
		AddDescText(ITEMDESC_COL_GREEN, false, pDescTab->GetWideString(ITEMDESC_MADEFROM), m_strMaker);
	}

	//	Suite description
	AddSuiteDesc();
	
	//	Extend description
	AddExtDescText();

	if(GetNewEssenceForGem().reserved1 > 0)
	{
		CECIvtrIndividualityItem* pIndividuality = (CECIvtrIndividualityItem*)CECIvtrItem::CreateItem(GetNewEssenceForGem().reserved1, 0, 1);
		if(pIndividuality)
		{
			m_strDesc += _AL("\\r");
			AddDescText(ITEMDESC_COL_GREEN, false, pDescTab->GetWideString(ITEMDESC_INDIVIDUAITY_COLOR), pIndividuality->GetName());
			if(pIndividuality->GetDBEssence()->quality > 0)
			{
				m_strDesc += _AL("\\r");
				AddDescText(ITEMDESC_COL_GREEN, false, pDescTab->GetWideString(ITEMDESC_INDIVIDUAITY_QUALITY),pDescTab->GetWideString((FACEDESC_QUALITY1 + pIndividuality->GetDBEssence()->quality) ));
			}
			delete pIndividuality;
		}
	}
	if(GetDBEssence()->color == 1)
	{
		m_strDesc += _AL("\\r");
		AddDescText(ITEMDESC_COL_GREEN, false, pDescTab->GetWideString(ITEMDESC_INDIVIDUAITY_OKDYE));
	}
	return m_strDesc;
}

//	Get one Addmon desc
const wchar_t* CECIvtrEquip::GetEquipAddonDesc(int idAddOn, int typeAddOn, int param1, int param2, int param3, int count)
{
	if (idAddOn)
	{
		const wchar_t* szAddonDesc = g_pGame->GetAddOnDesc()->GetWideString(idAddOn);

		if (szAddonDesc)
			return szAddonDesc;
	}

	static wchar_t szInfo[256];
	const wchar_t * szSkillName;

	CECStringTab* pDescTab = g_pGame->GetItemDesc();
	switch(typeAddOn)
	{
	case 1:	// 增加生命上限固定值
		a_sprintf(szInfo, pDescTab->GetWideString(ITEMDESC_EQUIPADDON_1), param1 * count);
		break;

	case 2: // 增加魔力上限固定值
		a_sprintf(szInfo, pDescTab->GetWideString(ITEMDESC_EQUIPADDON_2), param1 * count);
		break;

	case 3: // 增加攻击力固定值
		a_sprintf(szInfo, pDescTab->GetWideString(ITEMDESC_EQUIPADDON_3), param1 * count);
		break;

	case 4: // 增加防御力固定值
		a_sprintf(szInfo, pDescTab->GetWideString(ITEMDESC_EQUIPADDON_4), param1 * count);
		break;

	case 5: // 增加命中固定值
		a_sprintf(szInfo, pDescTab->GetWideString(ITEMDESC_EQUIPADDON_5), param1 * count);
		break;

	case 6: // 增加闪避固定值
		a_sprintf(szInfo, pDescTab->GetWideString(ITEMDESC_EQUIPADDON_6), param1 * count);
		break;

	case 7: // 增加致命一击率固定值
		a_sprintf(szInfo, pDescTab->GetWideString(ITEMDESC_EQUIPADDON_7), *(float *)&param1 * 100.0f * count);
		break;

	case 8: // 增加致命一击伤害固定值
		a_sprintf(szInfo, pDescTab->GetWideString(ITEMDESC_EQUIPADDON_8), *(float *)&param1 * 100.0f * count);
		break;

	case 9: // skill ext 0
		a_sprintf(szInfo, pDescTab->GetWideString(ITEMDESC_EQUIPADDON_9), param1 * count);
		break;

	case 10: // skill ext 1
		a_sprintf(szInfo, pDescTab->GetWideString(ITEMDESC_EQUIPADDON_10), param1 * count);
		break;

	case 11: // skill ext 2
		a_sprintf(szInfo, pDescTab->GetWideString(ITEMDESC_EQUIPADDON_11), param1 * count);
		break;

	case 12: // skill ext 3
		a_sprintf(szInfo, pDescTab->GetWideString(ITEMDESC_EQUIPADDON_12), param1 * count);
		break;

	case 13: // skill ext 4
		a_sprintf(szInfo, pDescTab->GetWideString(ITEMDESC_EQUIPADDON_13), param1 * count);
		break;

	case 14: // skill ext 5
		a_sprintf(szInfo, pDescTab->GetWideString(ITEMDESC_EQUIPADDON_14), param1 * count);
		break;

	case 15: // 增加生命上限百分比
		a_sprintf(szInfo, pDescTab->GetWideString(ITEMDESC_EQUIPADDON_15), (*(float *)&param1) * 100.0f * count);
		break;

	case 16: // 增加魔力上限百分比
		a_sprintf(szInfo, pDescTab->GetWideString(ITEMDESC_EQUIPADDON_16), (*(float *)&param1) * 100.0f * count);
		break;

	case 17: // 增加攻击力百分比
		a_sprintf(szInfo, pDescTab->GetWideString(ITEMDESC_EQUIPADDON_17), (*(float *)&param1) * 100.0f * count);
		break;

	case 18: // 增加防御力百分比
		a_sprintf(szInfo, pDescTab->GetWideString(ITEMDESC_EQUIPADDON_18), (*(float *)&param1) * 100.0f * count);
		break;

	case 19: // skill percent 0
		a_sprintf(szInfo, pDescTab->GetWideString(ITEMDESC_EQUIPADDON_19), (*(float *)&param1) * 100.0f * count);
		break;

	case 20: // skill percent 1
		a_sprintf(szInfo, pDescTab->GetWideString(ITEMDESC_EQUIPADDON_20), (*(float *)&param1) * 100.0f * count);
		break;

	case 21: // skill percent 2
		a_sprintf(szInfo, pDescTab->GetWideString(ITEMDESC_EQUIPADDON_21), (*(float *)&param1) * 100.0f * count);
		break;

	case 22: // skill percent 3
		a_sprintf(szInfo, pDescTab->GetWideString(ITEMDESC_EQUIPADDON_22), (*(float *)&param1) * 100.0f * count);
		break;

	case 23: // skill percent 4
		a_sprintf(szInfo, pDescTab->GetWideString(ITEMDESC_EQUIPADDON_23), (*(float *)&param1) * 100.0f * count);
		break;

	case 24: // skill percent 5
		a_sprintf(szInfo, pDescTab->GetWideString(ITEMDESC_EQUIPADDON_24), (*(float *)&param1) * 100.0f * count);
		break;

	case 25: // 增加移动速度
		a_sprintf(szInfo, pDescTab->GetWideString(ITEMDESC_EQUIPADDON_25), *(float *)&param1 * count);
		break;

	case 26: // 增加生命恢复速度百分比
		a_sprintf(szInfo, pDescTab->GetWideString(ITEMDESC_EQUIPADDON_26), (*(float *)&param1) * 100.0f * count);
		break;

	case 27: // 增加魔力恢复速度百分比
		a_sprintf(szInfo, pDescTab->GetWideString(ITEMDESC_EQUIPADDON_27), (*(float *)&param1) * 100.0f * count);
		break;

	case 28: // 经验获得增加固定值
		a_sprintf(szInfo, pDescTab->GetWideString(ITEMDESC_EQUIPADDON_28), param1 * count);
		break;

	case 29: // 经验获得增加百分比
		a_sprintf(szInfo, pDescTab->GetWideString(ITEMDESC_EQUIPADDON_29), (*(float *)&param1) * 100.0f * count);
		break;

	case 30: // 金钱获得增加固定值
		a_sprintf(szInfo, pDescTab->GetWideString(ITEMDESC_EQUIPADDON_30), param1 * count);
		break;

	case 31: // 金钱获得增加百分比
		a_sprintf(szInfo, pDescTab->GetWideString(ITEMDESC_EQUIPADDON_31), (*(float *)&param1) * 100.0f * count);
		break;

	case 32: // 减少伤害固定值
		a_sprintf(szInfo, pDescTab->GetWideString(ITEMDESC_EQUIPADDON_32), param1 * count);
		break;

	case 33: // 减少伤害百分比
		a_sprintf(szInfo, pDescTab->GetWideString(ITEMDESC_EQUIPADDON_33), (*(float *)&param1) * 100.0f * count);
		break;

	case 34: // 增加某某(全部)技能等级
		szSkillName = g_pGame->GetSkillDesc()->GetWideString(param1 * 10);
		if( !szSkillName || szSkillName[0] == 0 )
			szSkillName = pDescTab->GetWideString(ITEMDESC_ALL);
		a_sprintf(szInfo, pDescTab->GetWideString(ITEMDESC_EQUIPADDON_34), szSkillName, param2 * count);
		break;

	case 35: // 降低某某（全部）全部技能冷却时间
		szSkillName = g_pGame->GetSkillDesc()->GetWideString(param1 * 10);
		if( !szSkillName || szSkillName[0] == 0 )
			szSkillName = pDescTab->GetWideString(ITEMDESC_ALL);
		a_sprintf(szInfo, pDescTab->GetWideString(ITEMDESC_EQUIPADDON_35), szSkillName, (*(float *)&param2) * count);
		break;

	case 36: // 附加伤害
		a_sprintf(szInfo, pDescTab->GetWideString(ITEMDESC_EQUIPADDON_36), param1 * count);
		break;

	case 37: // 骑乘速度增加
		a_sprintf(szInfo, pDescTab->GetWideString(ITEMDESC_EQUIPADDON_37), *(float *)&param1 * count);
		break;

	case 38: // 增加一个某等级的技能
		szSkillName = g_pGame->GetSkillDesc()->GetWideString(param1 * 10);
		if( !szSkillName || szSkillName[0] == 0 )
			szSkillName = _AL("");
		a_sprintf(szInfo, pDescTab->GetWideString(ITEMDESC_EQUIPADDON_38), szSkillName, param2);
		break;

	case 40: // 增加飞行速度
		a_sprintf(szInfo, pDescTab->GetWideString(ITEMDESC_EQUIPADDON_40), *(float *)&param1 * count);
		break;
		
	case 41:
		a_sprintf(szInfo, pDescTab->GetWideString(ITEMDESC_EQUIPADDON_41), (*(float *)&param1) * 100.0f * count);
		break;

	case 42:
		a_sprintf(szInfo, pDescTab->GetWideString(ITEMDESC_EQUIPADDON_42), (*(float *)&param1) * 100.0f * count);
		break;

	case 43:
		a_sprintf(szInfo, pDescTab->GetWideString(ITEMDESC_EQUIPADDON_43), (*(float *)&param1) * 100.0f * count);
		break;

	case 44:
		a_sprintf(szInfo, pDescTab->GetWideString(ITEMDESC_EQUIPADDON_44), (*(float *)&param1) * 100.0f * count);
		break;

	case 45:
		szSkillName = g_pGame->GetSkillDesc()->GetWideString(param1 * 10);
		if( !szSkillName || szSkillName[0] == 0 )
			szSkillName = pDescTab->GetWideString(ITEMDESC_ALL);
		a_sprintf(szInfo, pDescTab->GetWideString(ITEMDESC_EQUIPADDON_45), szSkillName, (*(float *)&param2) * count);
		break;

	case 46:
		a_sprintf(szInfo, pDescTab->GetWideString(ITEMDESC_EQUIPADDON_46), param1 * count);
		break;

	case 47:
		a_sprintf(szInfo, pDescTab->GetWideString(ITEMDESC_EQUIPADDON_47), (*(float *)&param1) * 100.0f * count);
		break;

	case 48:
		a_sprintf(szInfo, pDescTab->GetWideString(ITEMDESC_EQUIPADDON_48), (*(float *)&param1) * 100.0f * count);
		break;
		
	case 49:
		a_sprintf(szInfo, pDescTab->GetWideString(ITEMDESC_EQUIPADDON_49), (*(float *)&param1) * 100.0f * count);
		break;

	case 50:
		a_sprintf(szInfo, pDescTab->GetWideString(ITEMDESC_EQUIPADDON_50), (*(float *)&param1) * 100.0f * count);
		break;

	case 51:
		a_sprintf(szInfo, pDescTab->GetWideString(ITEMDESC_EQUIPADDON_51), (*(float *)&param1) * 100.0f * count);
		break;

	case 52:
		a_sprintf(szInfo, pDescTab->GetWideString(ITEMDESC_EQUIPADDON_52), (*(float *)&param1) * 100.0f * count);
		break;

	case 53:
		a_sprintf(szInfo, pDescTab->GetWideString(ITEMDESC_EQUIPADDON_53), (*(float *)&param1) * 100.0f * count);
		break;

	case 54:
		a_sprintf(szInfo, pDescTab->GetWideString(ITEMDESC_EQUIPADDON_54), (*(float *)&param1) * 100.0f * count);
		break;

	case 55:
		a_sprintf(szInfo, pDescTab->GetWideString(ITEMDESC_EQUIPADDON_55), (*(float *)&param1) * 100.0f * count);
		break;

	case 56:
		a_sprintf(szInfo, pDescTab->GetWideString(ITEMDESC_EQUIPADDON_56), (*(float *)&param1) * 100.0f * count);
		break;

	case 57:
		a_sprintf(szInfo, pDescTab->GetWideString(ITEMDESC_EQUIPADDON_57), (*(float *)&param1) * 100.0f * count);
		break;

	case 58:
		a_sprintf(szInfo, pDescTab->GetWideString(ITEMDESC_EQUIPADDON_58), (*(float *)&param1) * 100.0f * count);
		break;

	case 59:
		a_sprintf(szInfo, pDescTab->GetWideString(ITEMDESC_EQUIPADDON_59), (*(float *)&param1) * 100.0f * count);
		break;
		
	case 60:
		a_sprintf(szInfo, pDescTab->GetWideString(ITEMDESC_EQUIPADDON_60), (*(float *)&param1) * 100.0f * count);
		break;
	case 61:	//增加一个附加永久技能
		szSkillName = g_pGame->GetSkillDesc()->GetWideString(param1 * 10);
		if( !szSkillName || szSkillName[0] == 0 )
		{
			szSkillName = _AL("");
		}
		a_sprintf(szInfo, pDescTab->GetWideString(ITEMDESC_EQUIPADDON_61), szSkillName, param2);
		break;
	case 62:
		szSkillName = g_pGame->GetSkillDesc()->GetWideString(param1 * 10);
		if( !szSkillName || szSkillName[0] == 0 )
			szSkillName = pDescTab->GetWideString(ITEMDESC_ALL);
		a_sprintf(szInfo, pDescTab->GetWideString(ITEMDESC_EQUIPADDON_62), szSkillName);
		break;
	default:	//需要特殊处理？？？
		break;
	}

	return szInfo;
}

//	Enhanced equip with add on
void CECIvtrEquip::EnhanceWithEquipAddOn(EQUIPMENT_ESSENCE& essence, int typeAddOn, int value)
{
	switch(typeAddOn)
	{
	case 1:	// 增加生命上限固定值
		essence.hp += value;
		break;

	case 2: // 增加魔力上限固定值
		essence.mp += value;
		break;

	case 3: // 增加攻击力固定值
		essence.min_dmg += value;
		essence.max_dmg += value;
		break;

	case 4: // 增加防御力固定值
		essence.defence += value;
		break;

	case 5: // 增加命中固定值
		essence.attack += value;
		break;

	case 6: // 增加闪避固定值
		essence.armor += value;
		break;

	case 7: // 增加致命一击率固定值
		break;

	case 8: // 增加致命一击伤害固定值
		break;

	case 9: // skill ext 0
		break;

	case 10: // skill ext 1
		break;

	case 11: // skill ext 2
		break;

	case 12: // skill ext 3
		break;

	case 13: // skill ext 4
		break;

	case 14: // skill ext 5
		break;

	case 15: // 增加生命上限百分比
		essence.hp = (int)(essence.hp * (1.0f + *(float*)&value));
		break;

	case 16: // 增加魔力上限百分比
		essence.mp = (int)(essence.mp * (1.0f + *(float*)&value));
		break;

	case 17: // 增加攻击力百分比
		essence.min_dmg = (int)(essence.min_dmg * (1.0f + *(float*)&value));
		essence.max_dmg = (int)(essence.max_dmg * (1.0f + *(float*)&value));
		break;

	case 18: // 增加防御力百分比
		essence.defence = (int)(essence.defence * (1.0f + *(float*)&value));
		break;

	case 19: // skill percent 0
		break;

	case 20: // skill percent 1
		break;

	case 21: // skill percent 2
		break;

	case 22: // skill percent 3
		break;

	case 23: // skill percent 4
		break;

	case 24: // skill percent 5
		break;

	case 25: // 增加移动速度
		break;

	case 26: // 增加生命恢复速度百分比
		break;

	case 27: // 增加魔力恢复速度百分比
		break;

	case 28: // 经验获得增加固定值
		break;

	case 29: // 经验获得增加百分比
		break;

	case 30: // 金钱获得增加固定值
		break;

	case 31: // 金钱获得增加百分比
		break;

	case 32: // 减少伤害固定值
		break;

	case 33: // 减少伤害百分比
		break;

	case 34: // 增加某某(全部)技能等级
		break;

	case 35: // 降低某某（全部）全部技能冷却时间
		break;

	case 36: // 附加伤害
		break;
	}

	return;
}

ACString CECIvtrEquip::_Serialize() const
{
	return ItemMakeString(&m_Essence);
}

bool CECIvtrEquip::_Unserialize(const ACHAR* szText)
{
	ItemFromString(szText, &m_Essence);
	m_bNeedUpdate = false;

	if( m_Essence.old_data.num_pstone )
	{
		DATA_TYPE dt;
		PSTONE_ESSENCE * pPStone = (PSTONE_ESSENCE *) g_pGame->GetElementDataMan()->get_data_ptr(m_Essence.old_data.id_pstone,
			ID_SPACE_ESSENCE, dt);

		if( dt == DT_PSTONE_ESSENCE )
		{
			if( pPStone->type == 1 )
			{
				m_iLevelReqDrop = m_Essence.old_data.num_pstone * 1;
			}
		}
	}
	return true;
}

int CECIvtrEquip::GetLevelRequirement() const
{ 
	return m_iLevelReq - m_iLevelReqDrop; 
}

//	Get drop model for shown
const char * CECIvtrEquip::GetDropModel()
{
	return g_pGame->GetDataPath(m_pDBEssence->file_matter);
}

//	Check whether this equipment belongs to a suite
int CECIvtrEquip::GetSuiteID()
{
	CECGame::SuiteEquipTable& SuiteTab = g_pGame->GetSuiteEquipTable();
	CECGame::SuiteEquipTable::pair_type pair = SuiteTab.get(m_tid);
	return pair.second ? *pair.first : 0;
}

// soul suite addon description
void CECIvtrEquip::AddSoulSuiteDesc()
{
	int i = 0;
	int lblue = ITEMDESC_COL_LIGHTBLUE;
	int gray  = ITEMDESC_COL_GRAY;
	//	Get suite info
	DATA_TYPE DataType = DT_INVALID;
	elementdataman* pDataMan = g_pGame->GetElementDataMan();
	CECStringTab* pDescTab = g_pGame->GetItemDesc();

	CECGame::SuiteEquipTable& SuiteTab = g_pGame->GetSuiteEquipTable();
	CECGame::SuiteEquipTable::iterator pos;

	for( i=0; i<5; ++i)
	{
		if(m_Essence.soul[i]==0)
			return;
	}
	
	int soul_type;
	int min_soul_level;
	int min_soul_id;

	const void* pData = pDataMan->get_data_ptr(m_Essence.soul[0],ID_SPACE_ESSENCE, DataType);
	if(DataType != DT_EQUIP_SOUL_ESSENCE)
		return;
	const EQUIP_SOUL_ESSENCE* pEquipSoul = (const EQUIP_SOUL_ESSENCE*)pData;
	soul_type = pEquipSoul->soul_type;
	min_soul_level = pEquipSoul->soul_level;
	min_soul_id = pEquipSoul->id; 

	for ( i=0; i<5; ++i)
	{
		const void* pData = pDataMan->get_data_ptr(m_Essence.soul[i],ID_SPACE_ESSENCE, DataType);
		if(DataType != DT_EQUIP_SOUL_ESSENCE)
			return;
		const EQUIP_SOUL_ESSENCE* pEquipSoul = (const EQUIP_SOUL_ESSENCE*)pData;
		if(pEquipSoul->soul_type != soul_type)
			return;
		if(pEquipSoul->soul_level < min_soul_level)
		{
			min_soul_level = pEquipSoul->soul_level;
			min_soul_id = pEquipSoul->id;
		}
	}

	CECGame::SuiteEquipTable::pair_type pair = SuiteTab.get(min_soul_id);
	if (!pair.second)
		return;
	int idSuite = *pair.first;

	pData = pDataMan->get_data_ptr(idSuite, ID_SPACE_ESSENCE, DataType);
	if(DataType != DT_SUITE_ESSENCE)
		return;
	const SUITE_ESSENCE* pSuiteEss = (const SUITE_ESSENCE*)pData;
	if(pSuiteEss->equip_soul_suite ==0 || pSuiteEss->max_equips != 5)
		return;
	
	int idProp = (int)(pSuiteEss->addons[pSuiteEss->max_equips -2].id);
	if (!idProp)
		return;
	pData = pDataMan->get_data_ptr(idProp, ID_SPACE_ADDON, DataType);
	if(DataType != DT_EQUIPMENT_ADDON)
	{
		ASSERT(DataType == DT_EQUIPMENT_ADDON);
		return;
	}
	//器魄套装名字
	//AddDescText(lblue, true, _AL("%s (%d)"), pSuiteEss->name, pSuiteEss->max_equips);
	AddDescText(m_Essence.cur_spirit_power>0? lblue:gray, false, _AL("(%s)"), pDescTab->GetWideString(ITEMDESC_ACTIVATE_SOUL_SUITE));
	const EQUIPMENT_ADDON* pAddOn = (const EQUIPMENT_ADDON*)pData;

	AddDescText(m_Essence.cur_spirit_power>0? lblue:gray, true, _AL(" %s"), GetEquipAddonDesc(pAddOn->id, pAddOn->type, pAddOn->param1, pAddOn->param2, pAddOn->param3, 1));

}

void CECIvtrEquip::AddAstrologyDesc()
{
	CECGameUIMan *pGameUIMan = g_pGame->GetGameRun()->GetUIManager()->GetInGameUIMan();
	CECStringTab* pDescTab = g_pGame->GetItemDesc();
	int nPropColor = g_pGame->GetGameRun()->GetHostPlayer()->GetAstrologyEnergy() > 0 ? ITEMDESC_COL_GREEN : ITEMDESC_COL_GRAY;
	ACString szValue;

	for (int i = 0; i < m_Essence.curr_hole; ++ i)
	{
		if (m_Essence.hole[i].astrology_id != 0) // 已经镶嵌星座
		{
			int level = m_Essence.hole[i].level;
			int score = m_Essence.hole[i].score;
			CECIvtrAstrology* pAstrology = (CECIvtrAstrology*)CECIvtrItem::CreateItem(m_Essence.hole[i].astrology_id, 0, 1);
			if (pAstrology)
			{
				AString strName;
				af_GetFileTitle(g_pGame->GetDataPath(pAstrology->GetDBEssence()->file_icon), strName);
				strName.MakeLower();
				AddDescImage(false, CECGameUIMan::ICON_HINT_GEM, pGameUIMan->GetIconId(CECGameUIMan::ICONS_ASTROLOGY, strName));
				AddDescText(ITEMDESC_COL_WHITE, level <= 0, pAstrology->GetDBEssence()->name);
				delete pAstrology;

				if (level > 0)
					AddDescText(ITEMDESC_COL_WHITE, true, _AL(" +%d"), level);
			}

			const ASTROLOGY_PROP* props = m_Essence.hole[i].prop;
			for (int j = 0; j < 5; ++ j)
			{
				const ASTROLOGY_PROP& prop = props[j];
				if (prop.id == 0)
					break;

				int value = prop.value;
				AddDescText(nPropColor, false, pDescTab->GetWideString(ITEMDESC_EQUIPADDON_61+prop.id));
				AddDescText(nPropColor, true, _AL("+%s"), CECIvtrRune::GetPropString(prop.id, int(value+value*level*0.25f)));
			}

			AddDescText(CECIvtrAstrology::GetScoreColor(score), 
				true, pDescTab->GetWideString(ITEMDESC_ASTROLOGY_SCORE), score);
		}
		else // 只打孔, 未镶嵌星座
		{
			AddDescImage(false, CECGameUIMan::ICON_HINT_GEM, pGameUIMan->GetIconId(CECGameUIMan::ICONS_ASTROLOGY, "hole")); // TODO: "hole"
		
			if (m_pDBEssence->equip_mask & EQUIP_MASK_HEAD)
				AddDescText(ITEMDESC_COL_GRAY, true, pDescTab->GetWideString(ITEMDESC_ASTROLOGY_HOLE_HEAD));
			if (m_pDBEssence->equip_mask & EQUIP_MASK_WEAPON)
				AddDescText(ITEMDESC_COL_GRAY, true, pDescTab->GetWideString(ITEMDESC_ASTROLOGY_HOLE_WEAPON));
			if (m_pDBEssence->equip_mask & EQUIP_MASK_BODY)
				AddDescText(ITEMDESC_COL_GRAY, true, pDescTab->GetWideString(ITEMDESC_ASTROLOGY_HOLE_BODY));
			if (m_pDBEssence->equip_mask & EQUIP_MASK_FOOT)
				AddDescText(ITEMDESC_COL_GRAY, true, pDescTab->GetWideString(ITEMDESC_ASTROLOGY_HOLE_SHOES));
		}
	}

}

//	Get item description text for suite information
void CECIvtrEquip::AddSuiteDesc()
{
	int idSuite = GetSuiteID();
	if (!idSuite)
		return;	//	This equipment isn't one of any suite

	//	Get suite info
	DATA_TYPE DataType = DT_INVALID;
	elementdataman* pDataMan = g_pGame->GetElementDataMan();
	const void* pData = pDataMan->get_data_ptr(idSuite, ID_SPACE_ESSENCE, DataType);
	if (DataType != DT_SUITE_ESSENCE)
	{
		ASSERT(DataType == DT_SUITE_ESSENCE);
		return;
	}

	const SUITE_ESSENCE* pSuiteEss = (const SUITE_ESSENCE*)pData;
	CECHostPlayer* pHostPlayer = g_pGame->GetGameRun()->GetHostPlayer();

	//	Colors
	int lblue = ITEMDESC_COL_LIGHTBLUE;
	int green = ITEMDESC_COL_GREEN;
	int gray = ITEMDESC_COL_GRAY;
	int white = ITEMDESC_COL_WHITE;
	int yellow = ITEMDESC_COL_YELLOW;
	int iNameCol = white;

	//	Save current description
	AWString strCurDesc = m_strDesc;
	int i, j;

	// we always show detail, and we decide the suite information according to host player's equipment inventory;
	//	Maximum number of suite items
	enum
	{
		MAX_NUM = 14
	};

	struct SUITEITEM
	{
		bool bEnable;
		int tid;
		wchar_t szName[32];

	} aSuiteItems[MAX_NUM];

	//	Fill suite item array
	for (i=0; i < pSuiteEss->max_equips; i++)
	{
		SUITEITEM& item = aSuiteItems[i];
		item.bEnable = false;
		item.tid = (int)pSuiteEss->equipments[i].id;
		item.szName[0] = '\0';

		//	Get item name
		CECIvtrItem* pEquipItem = CECIvtrItem::CreateItem(item.tid, 0, 1);
		if (pEquipItem)
		{
			a_strcpy(item.szName, pEquipItem->GetName());
			delete pEquipItem;
		}
		else
		{
			ASSERT(0);
			item.tid = 0;
		}
	}

	//	Get equipped suite item list
	int iItemCnt, aEquipped[MAX_NUM];
	iItemCnt = pHostPlayer->GetEquippedSuiteItem(idSuite, aEquipped);
	
	m_strDesc = _AL("\\r");

	//	Update suite item status
	for (i=0; i < MAX_NUM; i++)
	{
		SUITEITEM& item = aSuiteItems[i];

		for (j=0; j < iItemCnt; j++)
		{
			if (item.tid == aEquipped[j])
			{
				item.bEnable = true;
				break;
			}
		}
	}

	CECStringTab* pDescTab = g_pGame->GetItemDesc();
	
	iNameCol = pSuiteEss->max_equips == iItemCnt ? yellow : white;
	if( iItemCnt <= 1 )
		iNameCol = gray;

	//	Change color
	AddDescText(iNameCol, true, pDescTab->GetWideString(ITEMDESC_SUITE_PROP));
	
	//	Build suite addon properties at first
	for (i=2; i<=pSuiteEss->max_equips; i++)
	{
		int idProp = (int)pSuiteEss->addons[i-2].id;
		if (!idProp)
			continue;

		pData = pDataMan->get_data_ptr(idProp, ID_SPACE_ADDON, DataType);
		if (DataType != DT_EQUIPMENT_ADDON)
		{
			ASSERT(DataType == DT_EQUIPMENT_ADDON);
			continue;
		}

		const EQUIPMENT_ADDON* pAddOn = (const EQUIPMENT_ADDON*)pData;

		int col = i <= iItemCnt ? lblue : gray;
		AddDescText(col, false, _AL("(%d) "), i);
		AddDescText(col, true, _AL("%s"), GetEquipAddonDesc(pAddOn->id, pAddOn->type, pAddOn->param1, pAddOn->param2, pAddOn->param3, 1));
	}
	
	iNameCol = pSuiteEss->max_equips == iItemCnt ? yellow : white;
	if( iItemCnt == 0 )
		iNameCol = gray;
	//	Add suite name
	AddDescText(iNameCol, true, _AL("%s (%d / %d)"), pSuiteEss->name, iItemCnt, pSuiteEss->max_equips);
	
	//	List suite item names
	for (i=0; i < pSuiteEss->max_equips; i++)
	{
		SUITEITEM& item = aSuiteItems[i];
		if (!item.tid)
			continue;

		int col = item.bEnable ? green : gray;
		bool bRet = (i == pSuiteEss->max_equips-1) ? false : true;

		//	Add item name
		AddDescText(col, bRet, _AL("  %s"), item.szName);
	}

	m_strDesc = strCurDesc + m_strDesc;
}

void CECIvtrEquip::AddBleedSacrificeDesc(int val)
{
	int darkgold = ITEMDESC_COL_DARKGOLD;
	int green   = ITEMDESC_COL_GREEN;
	int gray	= ITEMDESC_COL_GRAY;
	CECStringTab* pDescTab = g_pGame->GetItemDesc();

	AUICTranslate trans;
	ACHAR szText[512];
	AUI_ConvertChatString((ACHAR*)(m_Essence.master_name), szText, false);
	AWString strMasterName = trans.ReverseTranslate(szText);		// 名字中有'\\'，转为'\\'+'\\'，因为updatehint的时候会调用translate

	AddDescText(darkgold, true, pDescTab->GetWideString(ITEMDESC_EQUIP_HOST), strMasterName);
	AddDescText(m_Essence.cur_spirit_power>0? green:gray, true, pDescTab->GetWideString(ITEMDESC_SOUL_VALUE), m_Essence.cur_spirit_power, m_Essence.max_spirit_power);

	AddDescText(m_Essence.cur_spirit_power>0? darkgold:gray, false, _AL ("( %s )"), pDescTab->GetWideString(ITEMDESC_BLOOD_SACRIFICE));

	switch(m_pDBEssence->equip_mask)
	{
	case EQUIP_MASK_BODY:	
		AddDescText(m_Essence.cur_spirit_power>0? darkgold:gray, true, pDescTab->GetWideString(ITEMDESC_EQUIPADDON_4), (unsigned int)(val*0.05f+25));
		break;
	case EQUIP_MASK_FOOT:
		AddDescText(m_Essence.cur_spirit_power>0? darkgold:gray, true, pDescTab->GetWideString(ITEMDESC_EQUIPADDON_1), (unsigned int)(val*0.1f+100));
		break;
	case EQUIP_MASK_HEAD:
		AddDescText(m_Essence.cur_spirit_power>0? darkgold:gray, true, pDescTab->GetWideString(ITEMDESC_EQUIPADDON_2), (unsigned int)(val*0.1f+100));
		break;
	case EQUIP_MASK_WEAPON:
		AddDescText(m_Essence.cur_spirit_power>0? darkgold:gray, true, pDescTab->GetWideString(ITEMDESC_EQUIPADDON_3), (unsigned int)(val*0.05f+25));
		break;
	}
}
// 镶嵌器魄之后的属性加成
void CECIvtrEquip::AddSoulEmbedDesc()
{
	int i = 0;
	DATA_TYPE dt = DT_INVALID;
	elementdataman* pDataMan = g_pGame->GetElementDataMan();
	CECStringTab* pDescTab = g_pGame->GetItemDesc();
	int lblue = ITEMDESC_COL_LIGHTBLUE;
	int gray  = ITEMDESC_COL_GRAY;
	for (i =0; i<5; ++i)
	{
		if(m_Essence.soul[i])
		{
			EQUIP_SOUL_ESSENCE* pSoul = (EQUIP_SOUL_ESSENCE*) pDataMan->get_data_ptr(m_Essence.soul[i], ID_SPACE_ESSENCE, dt);
			if (dt != DT_EQUIP_SOUL_ESSENCE )
			{
				ASSERT(dt == DT_EQUIP_SOUL_ESSENCE);
				return;
			}
			EQUIPMENT_ADDON* pAddOn = (EQUIPMENT_ADDON*) g_pGame->GetElementDataMan()->get_data_ptr(pSoul->id_addon1, ID_SPACE_ADDON, dt);
			if (dt == DT_EQUIPMENT_ADDON)
			{	
				AddDescText(m_Essence.cur_spirit_power>0? lblue:gray, false, pDescTab->GetWideString(ITEMDESC_EQUIP_SOUL2), pDescTab->GetWideString(ITEMDESC_GOLD+i));
				AddDescText(m_Essence.cur_spirit_power>0? lblue:gray, false, pSoul->name);
				AddDescText(m_Essence.cur_spirit_power>0? lblue:gray, true, _AL(" %s"), CECIvtrEquip::GetEquipAddonDesc(pAddOn->id, pAddOn->type, pAddOn->param1, pAddOn->param2, pAddOn->param3, 1));
			}		
		}
	}
}