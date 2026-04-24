/*
 * FILE: EC_IvtrMaterial.cpp
 *
 * DESCRIPTION: 
 *
 * CREATED BY: Fu Chonggang, 2012/4/20
 *
 * HISTORY: 
 *
 * Copyright (c) 2012 ZhuXian Studio, All Rights Reserved.
 */

#include "EC_Global.h"
#include "EC_IvtrRune.h"
#ifndef _INVENTORY_DESC_MAN //Added by Ford.W 2010-07-22.
#include "EC_Game.h"
#include "EC_GameRun.h"
#include "EC_HostPlayer.h"
#else
#include "EC_Game_HintTool.h"
#include "EC_GameRun_HintTool.h"
#include "EC_HostPlayer_HintTool.h"
#endif
#include "EC_FixedMsg.h"
#include "EC_GameUIMan.h"
#include "EC_UIManager.h"
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
//	Implement CECIvtrRune
//	
///////////////////////////////////////////////////////////////////////////

CECIvtrRune::CECIvtrRune(int tid, int expire_date) : CECIvtrEquip(tid, expire_date)
{
	m_iCID	= ICID_RUNE;

	//	Get database data
	elementdataman* pDB = g_pGame->GetElementDataMan();
	DATA_TYPE DataType;
	m_pDBEssence	= (RUNE_EQUIP_ESSENCE*)pDB->get_data_ptr(tid, ID_SPACE_ESSENCE, DataType);

	memset(&m_Essence, 0, sizeof(m_Essence));

	m_iLevelReqDrop = 0;
	m_iLevelReq		= m_pDBEssence->require_level;
	m_iRebornReq	= m_pDBEssence->renascence_count;
	m_iPileLimit	= m_pDBEssence->pile_num_max;
	m_iPrice		= m_pDBEssence->price;
	m_iShopPrice	= m_pDBEssence->shop_price;
	m_iProcType		= m_pDBEssence->proc_type;
	m_iEquipMask	= EQUIP_MASK_AUX4;

	m_bNeedUpdate	= false;
}

CECIvtrRune::CECIvtrRune(const CECIvtrRune& s) : CECIvtrEquip(s)
{
	m_pDBEssence	= s.m_pDBEssence;
	m_Essence		= s.m_Essence;
}

CECIvtrRune::~CECIvtrRune()
{
}

//	Set item detail information
bool CECIvtrRune::SetItemInfo(BYTE* pInfoData, int iDataLen)
{
	CECIvtrItem::SetItemInfo(pInfoData, iDataLen);

	if(iDataLen < sizeof(m_Essence))
		return false;
	
	memcpy(&m_Essence, pInfoData, sizeof(m_Essence));
	int iQuality = m_Essence.quality;
	if(iQuality==0)
		m_Essence.quality = -1;
	else if (iQuality<21)
		m_Essence.quality = 0;
	else if (iQuality<41)
		m_Essence.quality = 1;
	else if (iQuality<56)
		m_Essence.quality = 2;
	else if (iQuality<71)
		m_Essence.quality = 3;
	else if (iQuality<86)
		m_Essence.quality = 4;
	else if (iQuality<96)
		m_Essence.quality = 5;
	else
		m_Essence.quality = 6;

	// 查找合适符语套装属性
	DATA_TYPE dtType;
	elementdataman* pDataman = g_pGame->GetElementDataMan();
	int id = pDataman->get_first_data_id(ID_SPACE_CONFIG , dtType);
	m_iRuneSuiteId = 0;
	while(id)
	{
		const void* pData = pDataman->get_data_ptr(id , ID_SPACE_CONFIG , dtType);
		if(dtType == DT_RUNE_COMB_PROPERTY)
		{
			const RUNE_COMB_PROPERTY* pConfig = (const RUNE_COMB_PROPERTY*)pData;
			int i, j;
			if (pConfig->max_ids==m_Essence.hole)
			{
				for (i=0; i<pConfig->max_ids; ++i)
				{
					for (j=0; j<m_Essence.hole; ++j)
					{
						if (GetEssence().rune_stone[j]==(int)pConfig->ids[i])
						{
							break;
						}
					}
					if (j==m_Essence.hole)
					{
						break;
					}
				}
				if (i==pConfig->max_ids)
				{
					m_iRuneSuiteId = pConfig->id;
					break;
				}
			}
		}
		id = pDataman->get_next_data_id(ID_SPACE_CONFIG , dtType);
	}
	
	return true;
}

//	Get item icon file name
const char* CECIvtrRune::GetIconFile()
{
	return g_pGame->GetDataPath(m_pDBEssence->file_icon);
}

//	Get item name
const wchar_t* CECIvtrRune::GetName()
{
	if (m_Essence.level>0 && m_Essence.quality>=0)
	{
		return g_pGame->GetItemDesc()->GetWideString(ITEMDESC_RUNENAME1+m_Essence.quality);
	}
	else
		return m_pDBEssence->name;
}

//	Get item description text
const wchar_t* CECIvtrRune::GetNormalDesc(bool bRepair)
{
	m_strDesc = _AL("");

	//	Try to build item description
	CECHostPlayer* pHost = g_pGame->GetGameRun()->GetHostPlayer();
	CECStringTab* pDescTab = g_pGame->GetItemDesc();
	int white = ITEMDESC_COL_WHITE;
	int name_color = DecideNameCol();
	int col = white;
	ACString strName = pDescTab->GetWideString(ITEMDESC_RUNEUNOPEN);
	if (m_Essence.level>0 && m_Essence.quality>=0)
	{
		strName = pDescTab->GetWideString(ITEMDESC_RUNENAME1+m_Essence.quality);
	}
	AddDescText(name_color, true, pDescTab->GetWideString(ITEMDESC_NAME), strName);
	
	AddStateDesc(pDescTab);
	m_strDesc += _AL("\\r");
	AddExpireTimeDesc();

	// 元婴
	AddDescText(white, true, pDescTab->GetWideString(ITEMDESC_RUNENAME));
	if (m_Essence.level>0)
	{
		// 品质
		if(m_Essence.quality>=0)
			AddDescText(name_color, true, pDescTab->GetWideString(ITEMDESC_FLYSWORD_QUALITY), 
			pDescTab->GetWideString(ITEMDESC_RUNEQUALITY1+m_Essence.quality));
		// 元魂等级
		AddDescText(white, TRUE, pDescTab->GetWideString(ITEMDESC_RUNELEVELREQ), m_Essence.level);
		// 经验
		AddDescText(white, TRUE, pDescTab->GetWideString(ITEMDESC_RUNEXP), m_Essence.exp);
	}

	int iLevelReq = m_iLevelReq - m_iLevelReqDrop;
	col = pHost->GetBasicProps().iLevel >= iLevelReq ? white : ITEMDESC_COL_RED;
	AddDescText(col, true, pDescTab->GetWideString(ITEMDESC_LEVELREQ), iLevelReq);

	if (m_Essence.level==0)
	{
		AddDescText(white, true, pDescTab->GetWideString(ITEMDESC_PET_UN_IDENTIFY));
		m_strDesc += _AL("\\r");
		AddDescText(name_color, false, pDescTab->GetWideString(ITEMDESC_RUNEDESCORIG));
	}
	else
	{
		// 最多8个属性
		int i;
		for (i=0; i<8; ++i)
		{
			if (m_Essence.prop[i].id>0)
			{
				AddDescText(white, false, pDescTab->GetWideString(ITEMDESC_EQUIPADDON_61+m_Essence.prop[i].id));
				AddDescText(ITEMDESC_COL_GREEN, true,  _AL(" +%s"), GetPropString(m_Essence.prop[i].id, m_Essence.prop[i].value*m_Essence.level));
			}
		}

		// 2-5个孔的情况
		m_strDesc += _AL("\\r");
		if (m_Essence.hole>0)
		{
			CECGameUIMan *pGameUIMan = g_pGame->GetGameRun()->GetUIManager()->GetInGameUIMan();
			for (int i=0; i<m_Essence.hole; ++i)
			{
				if (m_Essence.rune_stone[i])
				{
					CECIvtrItem* pItem = CECIvtrItem::CreateItem(m_Essence.rune_stone[i], 0, 1);
					if (pItem)
					{
						AString strName;
						af_GetFileTitle(pItem->GetIconFile(), strName);
						strName.MakeLower();
#ifndef _INVENTORY_DESC_MAN		//寻宝网不需要，Added 2012-05-18.
						AddDescImage(false, CECGameUIMan::ICON_HINT_RUNE, pGameUIMan->GetIconId(CECGameUIMan::ICONS_RUNE, strName));
#endif
						AddDescText(ITEMDESC_COL_YELLOW, true, pItem->GetName());
						delete pItem;
					}
				}
				else
					AddDescText(ITEMDESC_COL_LIGHTBLUE, true, pDescTab->GetWideString(ITEMDESC_RUNESLOTEMPTY));
			}
		}

		// 孔的属性是否激活，激活则显示
		m_strDesc += _AL("\\r");
		if (m_iRuneSuiteId>0)
		{
			elementdataman* pDataman = g_pGame->GetElementDataMan();
			DATA_TYPE dtType = DT_INVALID;
			const void* pData = pDataman->get_data_ptr(m_iRuneSuiteId,ID_SPACE_CONFIG, dtType);
			if(dtType == DT_RUNE_COMB_PROPERTY)
			{
				const RUNE_COMB_PROPERTY* pRuneSoul = (const RUNE_COMB_PROPERTY*)pData;
				// 神通属性
				AddDescText(ITEMDESC_COL_YELLOW, true, pDescTab->GetWideString(ITEMDESC_RUNE_SUITE_PROP), pRuneSoul->name);
				for (i=0; i<9; ++i)
				{
					if(pRuneSoul->addons[i]>0)
					{
						const void *pDataProp = pDataman->get_data_ptr(pRuneSoul->addons[i], ID_SPACE_ADDON, dtType);
						if (dtType != DT_EQUIPMENT_ADDON)
						{
							//ASSERT(dtType == DT_EQUIPMENT_ADDON);
							continue;
						}
						
						const EQUIPMENT_ADDON* pAddOn = (const EQUIPMENT_ADDON*)pDataProp;
						AddDescText(ITEMDESC_COL_LIGHTBLUE, true, _AL("%s"), GetEquipAddonDesc(pAddOn->id, pAddOn->type, pAddOn->param1, pAddOn->param2, pAddOn->param3, 1));
					}
				}
			}
		}

		m_strDesc += _AL("\\r");
		AddDescText(name_color, true, pDescTab->GetWideString(ITEMDESC_RUNEDESC1+m_Essence.quality));
		m_strDesc += _AL("\\r");
		AddDescText(name_color, false, pDescTab->GetWideString(ITEMDESC_RUNEDESCHINT));
	}

	//	Extend description
	AddExtDescText();

	return m_strDesc;
}

int CECIvtrRune::DecideNameCol()
{
	int col = ITEMDESC_RUNECOLOR2;

	if (m_Essence.level>0)
	{
		col = ITEMDESC_RUNECOLOR1+m_Essence.quality;
	}

	return col;
}

//	Get drop model for shown
const char * CECIvtrRune::GetDropModel()
{
	return g_pGame->GetDataPath(m_pDBEssence->file_matter);
}

/*
value值由于需要乘以等级，所以精度要求比较大，所以value做了特殊处理（*100）
prop=true表示是资质显示，保留的精度要比prop=false高
*/
ACString CECIvtrRune::GetPropString(int id, int value)
{
	ACString strText;
	switch (id)
	{
	case 1:	// 气血
	case 2:	// 真气
	case 3:	// 攻击
	case 4:	// 防御
	case 5:	// 命中
	case 6:	// 躲闪
	case 9:	// 元力
	case 20:// 减伤
	case 22:// 抗性
	case 23:
	case 24:
	case 25:
	case 26:
	case 27:
	case 33:
	{
		strText.Format(_AL("%.2f"), value/100.0);
		break;
	}
	case 7:	// 技能命中
	case 8:	// 技能躲闪
	{
		strText.Format(_AL("%.3f"), float(value) / 1000);
		break;
	}
	case 10:// 致命一击率	千分数？小数
	case 12:// 减免致命一击率
	case 11:	//致命一击伤害
	case 13:	//减免致命一击伤害
	case 14:	//御仙魔佛
	case 15:
	case 16:
	case 17:	//克仙魔佛
	case 18:
	case 19:
	case 21:
	case 28:
	case 29:
	case 30:
	case 31:
	case 32:
	{
		strText.Format(_AL("%.3f%%"), float(value) / 1000.0f);
		break;
	}
//		strText.Format(_AL("%.1f%%"), value * 100 + 0.001);
//		break;
	default:
		strText = _AL("error!!!");
	}

	return strText;
}


///////////////////////////////////////////////////////////////////////////
//	
//	Implement CECIvtrRuneSlot
//	
///////////////////////////////////////////////////////////////////////////

CECIvtrRuneSlot::CECIvtrRuneSlot(int tid, int expire_date) : CECIvtrItem(tid, expire_date)
{
	m_iCID	= ICID_RUNE_SLOT;
	
	//	Get database data
	elementdataman* pDB = g_pGame->GetElementDataMan();
	DATA_TYPE DataType;
	m_pDBEssence	= (RUNE_SLOT_ESSENCE*)pDB->get_data_ptr(tid, ID_SPACE_ESSENCE, DataType);
	
	m_iPileLimit	= m_pDBEssence->pile_num_max;
	m_iPrice		= m_pDBEssence->price;
	m_iShopPrice	= m_pDBEssence->shop_price;
	m_iProcType		= m_pDBEssence->proc_type;
	m_iEquipMask	= 0;
	
	m_bNeedUpdate	= false;
}

CECIvtrRuneSlot::CECIvtrRuneSlot(const CECIvtrRuneSlot& s) : CECIvtrItem(s)
{
	m_pDBEssence	= s.m_pDBEssence;
}

CECIvtrRuneSlot::~CECIvtrRuneSlot()
{
}

//	Set item detail information
bool CECIvtrRuneSlot::SetItemInfo(BYTE* pInfoData, int iDataLen)
{
	CECIvtrItem::SetItemInfo(pInfoData, iDataLen);
	
	// TODO
	return true;
}

//	Get item icon file name
const char* CECIvtrRuneSlot::GetIconFile()
{
	return g_pGame->GetDataPath(m_pDBEssence->file_icon);
}

//	Get item name
const wchar_t* CECIvtrRuneSlot::GetName()
{
	return m_pDBEssence->name;
}

//	Get item description text
const wchar_t* CECIvtrRuneSlot::GetNormalDesc(bool bRepair)
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
const char * CECIvtrRuneSlot::GetDropModel()
{
	return g_pGame->GetDataPath(m_pDBEssence->file_matter);
}
