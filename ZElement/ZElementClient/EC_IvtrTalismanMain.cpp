/*
 * FILE: EC_IvtrTalismanMain.cpp
 *
 * DESCRIPTION: 
 *
 * CREATED BY: Hedi, 2007/4/17
 *
 * HISTORY: 
 *
 * Copyright (c) 2007 Archosaur Studio, All Rights Reserved.
 */

#include "EC_Global.h"
#include "EC_IvtrEquip.h"
#include "EC_IvtrTalismanMain.h"
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
#include "EC_Inventory.h"
#include "elementdataman.h"
#include "EC_GameRecord.h"
#include "EC_GameSession.h"
#include "AFilePackage.h"
#include "ConfigFromLua.h"
#include "EC_Skill.h"

#include "AChar.h"
#include "AAssist.h"

#include "AUI\\AUICTranslate.h"
#include "AUI\\AUICommon.h"

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
//	Implement CECIvtrTalismanMain
//	
///////////////////////////////////////////////////////////////////////////

CECIvtrTalismanMain::CECIvtrTalismanMain(int tid, int expire_date) : CECIvtrEquip(tid, expire_date)
{
	m_iCID		= ICID_TALISMAN_MAINPART;
	
	//	Get database data
	elementdataman* pDB = g_pGame->GetElementDataMan();
	DATA_TYPE DataType;
	m_pDBEssence	= (TALISMAN_MAINPART_ESSENCE*)pDB->get_data_ptr(tid, ID_SPACE_ESSENCE, DataType);

	memset(&m_Essence, 0, sizeof(m_Essence));

	m_iPileLimit	= m_pDBEssence->pile_num_max;
	m_iPrice		= m_pDBEssence->price;
	m_iShopPrice	= m_pDBEssence->shop_price;
	m_iLevelReq		= m_pDBEssence->require_level;
	
	m_iProfReq		= m_pDBEssence->character_combo_id;
	m_iProfReq2		= m_pDBEssence->character_combo_id2;
	m_iLevelReqDrop = 0;
	m_iGenderReq	= m_pDBEssence->require_gender;
	m_iRaceReq		= m_pDBEssence->require_race;
	m_iRebornReq	= m_pDBEssence->renascence_count;
	m_iProcType		= m_pDBEssence->proc_type;

	m_iEquipMask	= m_pDBEssence->is_aircraft ? EQUIP_MASK_WING : EQUIP_MASK_MAIN_TALISMAN;
}

CECIvtrTalismanMain::CECIvtrTalismanMain(const CECIvtrTalismanMain& s) : CECIvtrEquip(s)
{
	m_pDBEssence	= s.m_pDBEssence;
	m_Essence		= s.m_Essence;
	m_iLevelReq		= s.m_iLevelReq;
	m_iLevelReqDrop	= s.m_iLevelReqDrop;	
	m_iProfReq		= s.m_iProfReq;
	m_iGenderReq	= s.m_iGenderReq;
	m_iRaceReq		= s.m_iRaceReq;
	m_iRebornReq	= s.m_iRebornReq;
	m_strMaker		= s.GetMakerName();
}

CECIvtrTalismanMain::~CECIvtrTalismanMain()
{
}

//	Set item detail information
bool CECIvtrTalismanMain::SetItemInfo(BYTE* pInfoData, int iDataLen)
{
#ifndef _INVENTORY_DESC_MAN	//客户端gs发过来信息时的版本
	CECIvtrItem::SetItemInfo(pInfoData, iDataLen);
	
	if( iDataLen >= sizeof(m_Essence.ess) )
		memcpy(&m_Essence.ess, pInfoData, sizeof(m_Essence.ess));

	m_Essence.addons.clear();
	m_Essence.addons2012.clear();
	pInfoData += sizeof(m_Essence.ess);
	iDataLen -= sizeof(m_Essence.ess);
	if( iDataLen >= (int)sizeof(int) )
	{
		int size_addon = (*(int *)pInfoData) / sizeof(int);
		pInfoData += sizeof(int);
		iDataLen -= sizeof(int);

		for(int i=0; i<size_addon && iDataLen >= (int)sizeof(int); i++)
		{
			m_Essence.addons.push_back(*(int*)pInfoData);
			pInfoData += sizeof(int);
			iDataLen -= sizeof(int);
		}
	}
	
	if( iDataLen >= (int)sizeof(int) )
	{
		int size_addon = (*(int *)pInfoData);
		pInfoData += sizeof(int);
		iDataLen -= sizeof(int);
		
		for(int i=0; i<size_addon && iDataLen >= (int)sizeof(int); i++)
		{
			m_Essence.addons2012.push_back((int)(*(float*)pInfoData));
			pInfoData += sizeof(float);
			iDataLen -= sizeof(float);
		}
	}

	return true;
#else	//---------------------------------------------------------------------
//小工具版本，法宝新增功能以后，物品信息需要重新解析
	return SetItemFromMailInfo( pInfoData, iDataLen );
#endif
}

bool CECIvtrTalismanMain::SetItemFromMailInfo(BYTE* pInfoData, int iDataLen)
{
	CECIvtrItem::SetItemInfo(pInfoData, iDataLen);
	
	if( iDataLen >= sizeof(m_Essence.ess) )
		memcpy(&m_Essence.ess, pInfoData, sizeof(m_Essence.ess));
	
	m_Essence.addons.clear();
	m_Essence.addons2012.clear();
	pInfoData += sizeof(m_Essence.ess);
	iDataLen -= sizeof(m_Essence.ess);
	if( iDataLen >= (int)sizeof(int) )
	{
		int size_addon = (*(int *)pInfoData) / sizeof(int);
		pInfoData += sizeof(int);
		iDataLen -= sizeof(int);
		
		for(int i=0; i<size_addon && iDataLen >= (int)sizeof(int); i++)
		{
			m_Essence.addons.push_back(*(int*)pInfoData);
			pInfoData += sizeof(int);
			iDataLen -= sizeof(int);
		}
	}
	
	if( iDataLen >= (int)sizeof(int) )
	{
		int size_addon = (*(int *)pInfoData);
		size_addon /= sizeof(float);
		pInfoData += sizeof(int);
		iDataLen -= sizeof(int);
	
		int start = 96;
		int end = 119;
		if (size_addon<=end)
		{
			end = size_addon - 1;
		}
		for(int i=start; i<=end; i++)
		{
			if(iDataLen>=4)
				m_Essence.addons2012.push_back((int)(*(float*)(pInfoData+i*sizeof(float))));
			else
				m_Essence.addons2012.push_back(0);
			iDataLen -= sizeof(float);
		}
	}
	
	return true;
}

GNET::Octets CECIvtrTalismanMain::gen_item_info()
{
	return GNET::Octets(&m_Essence, sizeof(m_Essence));
}

//	Get item icon file name
const char* CECIvtrTalismanMain::GetIconFile()
{
	return g_pGame->GetDataPath(m_pDBEssence->file_icon);
}

//	Get item name
const wchar_t* CECIvtrTalismanMain::GetName()
{
	return m_pDBEssence->name;
}

//	Get drop model for shown
const char * CECIvtrTalismanMain::GetDropModel()
{
	return g_pGame->GetDataPath(m_pDBEssence->file_matter);
}

//	Build add-ons properties description
void CECIvtrTalismanMain::BuildAddOnPropDesc()
{
	CECStringTab* pDescTab = g_pGame->GetItemDesc();
	int color = ITEMDESC_COL_LIGHTBLUE;

	const wchar_t * szSkillName;
	{
		if(m_Essence.addons.size()==0)
			return;
		DATA_TYPE dt;
		EQUIPMENT_ADDON * pAddOn = (EQUIPMENT_ADDON *) g_pGame->GetElementDataMan()->get_data_ptr(m_Essence.addons[0], ID_SPACE_ADDON, dt);
		if( dt == DT_EQUIPMENT_ADDON )
			AddDescText(color, true, _AL("%s"), GetEquipAddonDesc(pAddOn->id, pAddOn->type, pAddOn->param1, pAddOn->param2, pAddOn->param3, 1));
	}

	size_t i;
	// 镶嵌技能
	for (i=TYPE_NEWADDON_ID_1; i<=TYPE_NEWADDON_ID_5; ++i)
	{
		if (m_Essence.addons2012.size()<=i)
			break;
		if (m_Essence.addons2012[i]==0)
			continue;

		DATA_TYPE dt;
		EQUIPMENT_ADDON * pAddOn = (EQUIPMENT_ADDON *) g_pGame->GetElementDataMan()->get_data_ptr(m_Essence.addons2012[i], ID_SPACE_ADDON, dt);
		if( dt != DT_EQUIPMENT_ADDON )
			continue;
		
		if (pAddOn->type == 38)
		{
			szSkillName = g_pGame->GetSkillDesc()->GetWideString(pAddOn->param1 * 10);
			if( !szSkillName || szSkillName[0] == 0 )
				szSkillName = _AL("");
			AddDescText(color, true, pDescTab->GetWideString(ITEMDESC_EQUIPADDON_SKILL_EXTRA), szSkillName, pAddOn->param2);
		}
	}

	for (i=1; i<m_Essence.addons.size(); i++)
	{
		DATA_TYPE dt;
		EQUIPMENT_ADDON * pAddOn = (EQUIPMENT_ADDON *) g_pGame->GetElementDataMan()->get_data_ptr(m_Essence.addons[i], ID_SPACE_ADDON, dt);
		if( dt != DT_EQUIPMENT_ADDON )
			continue;

		if (pAddOn->type == 39)
			continue;

		AddDescText(color, true, _AL("%s"), GetEquipAddonDesc(pAddOn->id, pAddOn->type, pAddOn->param1, pAddOn->param2, pAddOn->param3, 1));
	}
}

void CECIvtrTalismanMain::BuildNewAddOnPropDesc()
{
	CECStringTab* pDescTab = g_pGame->GetItemDesc();
	int color = ITEMDESC_COL_LIGHTBLUE;
	
	size_t i;
	for (i=TYPE_REFINE_0; i<=TYPE_REFINE_8; ++i)
	{
		if(m_Essence.addons2012.size()<=i)
			break;
		if(m_Essence.addons2012[i]==0)
			continue;
		CECSkill *pSkill = new CECSkill(m_Essence.addons2012[i], GetFixSkillLevel());
		pSkill->SetItem(this);
		m_strDesc += pSkill->GetDesc(GetFixSkillLevel(), false);
		m_strDesc += _AL("\\r");
		delete pSkill;
	}
}

//	Get item description text
const wchar_t* CECIvtrTalismanMain::GetNormalDesc(bool bRepair)
{
	if (m_bNeedUpdate)
		return NULL;

	// Build addon desc and save it.
	m_strDesc = _AL("");
	BuildAddOnPropDesc();
	AWString strAddon = m_strDesc;
	m_strDesc = _AL("");
	BuildNewAddOnPropDesc();
	AWString strAddonNew = m_strDesc;
	m_strDesc = _AL("");

	CECStringTab* pDescTab = g_pGame->GetItemDesc();
	CECStringTab* pFixMsg = g_pGame->GetFixedMsgTab();
	CECHostPlayer* pHost = g_pGame->GetGameRun()->GetHostPlayer();

	//	Try to build item description
	int white = ITEMDESC_COL_WHITE;
	int darkgold = ITEMDESC_COL_DARKGOLD;
	int green	= ITEMDESC_COL_GREEN;
	int name_color = DecideNameCol();

	if (m_iCount > 1)
		AddDescText(name_color, true, pDescTab->GetWideString(ITEMDESC_NAMENUMBER), GetName(), m_iCount);
	else
		AddDescText(name_color, true, pDescTab->GetWideString(ITEMDESC_NAME), GetName());

	if (m_Essence.addons2012.size()>TYPE_REBORN_COUNT && m_Essence.addons2012[TYPE_REBORN_COUNT]>0)
	{
		int col = (pHost->GetRebornCount() > 0) ? ITEMDESC_COL_PURPLE : ITEMDESC_COL_RED;
		AddDescText(col, true, _AL("%s"), pDescTab->GetWideString(ITEMDESC_RENASCENCE_1));
	}

	AddStateDesc(pDescTab);

	if (m_pDBEssence->proc_type & (1 << 4))
		AddDescText(white, true, _AL("%s"), pDescTab->GetWideString(ITEMDESC_CANT_TRADE));

	AddExpireTimeDesc();

	//	show level
	if( m_pDBEssence->show_level[0] )
		AddDescText(white, true, _AL("%s"), m_pDBEssence->show_level);

	//	special addon 39
	for (size_t n = 0; n < m_Essence.addons.size(); n++)
	{
		DATA_TYPE dt;
		EQUIPMENT_ADDON* pAddOn = (EQUIPMENT_ADDON*) g_pGame->GetElementDataMan()->get_data_ptr(m_Essence.addons[n], ID_SPACE_ADDON, dt);

		if (dt == DT_EQUIPMENT_ADDON && pAddOn->type == 39)
		{
			AddDescText(ITEMDESC_COL_LIGHTBLUE, true, _AL("%s"), pAddOn->name);
			break;
		}
	}

	//	owner
	if( m_Essence.ess.master_id )
	{	
		AUICTranslate trans;
		ACHAR szText[512];
		AUI_ConvertChatString((ACHAR*)(m_Essence.ess.master_name), szText, false);
		AWString strMasterName = trans.ReverseTranslate(szText);		// 名字中有'\\'，转为'\\'+'\\'，因为updatehint的时候会调用translate
		
		if (m_pDBEssence->is_aircraft)
			AddDescText(white, true, pDescTab->GetWideString(ITEMDESC_FLYSWORD_OWNER), strMasterName);
		else
			AddDescText(white, true, pDescTab->GetWideString(ITEMDESC_TALISMAN_OWNER), strMasterName);
	}

	if (m_pDBEssence->is_aircraft)
	{
		int index;

		if (m_Essence.ess.level < 10)
			index = 0;
		else if (m_Essence.ess.level < 20)
			index = 1;
		else if (m_Essence.ess.level < 30)
			index = 2;
		else
			index = 3;

		AddDescText(white, true, pDescTab->GetWideString(ITEMDESC_FLYSWORD_STAR), pDescTab->GetWideString(ITEMDESC_FLYSWORD_STAR1 + index));
	}
	else if (m_Essence.ess.level)
		AddDescText(white, true, pDescTab->GetWideString(ITEMDESC_TALISMAN_LEVEL), m_Essence.ess.level);

	//	Quality
	if (m_pDBEssence->is_aircraft)
	{
		AddDescText(name_color, true, pDescTab->GetWideString(ITEMDESC_FLYSWORD_QUALITY),
			pDescTab->GetWideString(ITEMDESC_FLYSWORD_QUALITY0 + ((m_Essence.ess.quality >> 26) & 0xf)));
	}
	else if (m_Essence.ess.quality)
	{
		AddDescText(white, true, pDescTab->GetWideString(ITEMDESC_TALISMAN_QUALITY), 
			pDescTab->GetWideString(ITEMDESC_TALISMAN_QUALITYLEVEL0 + ((m_Essence.ess.quality >> 26) & 0x3)),
			pDescTab->GetWideString(ITEMDESC_TALISMAN_HOUR0 + ((m_Essence.ess.quality >> 22) & 0xf)),
			pDescTab->GetWideString(ITEMDESC_TALISMAN_MINUTE0 + ((m_Essence.ess.quality >> 20) & 0x3)),
			pDescTab->GetWideString(ITEMDESC_TALISMAN_ORG0 + ((m_Essence.ess.quality >> 16) & 0xf))
			);
	}

	//	Max Energy
	if( m_Essence.ess.stamina )
		AddDescText(white, true, pDescTab->GetWideString(ITEMDESC_TALISMAN_MAXENERGY), m_Essence.ess.stamina);

	//	current exp
	if (m_pDBEssence->is_aircraft)
	{
		if (m_Essence.ess.level > 0)
			AddDescText(white, true, pDescTab->GetWideString(ITEMDESC_FLYSWORD_EXP), CECPlayer::GetTalismanTotalExp(m_Essence.ess.level) + m_Essence.ess.exp);

		AddDescText(white, true, pDescTab->GetWideString(ITEMDESC_FLY_SPEED), m_pDBEssence->fly_speed);
	}
	else if (m_Essence.ess.exp)
		AddDescText(white, true, pDescTab->GetWideString(ITEMDESC_TALISMAN_EXP), m_Essence.ess.exp);
	
	DWORD quality_trend = (m_Essence.ess.quality >> 12) & 0xf;
	
	if (m_pDBEssence->is_aircraft==0 && quality_trend!=1) // 1:不显示 0:随机 2:最小攻击 ... 13:防御
	{
		if (quality_trend==0)
			AddDescText(darkgold, true, pDescTab->GetWideString(ITEMDESC_TALISMAN_TREND), pDescTab->GetWideString(ITEMDESC_TALISMAN_TREND0));
		else
			AddDescText(darkgold, true, pDescTab->GetWideString(ITEMDESC_TALISMAN_TREND), pDescTab->GetWideString(ITEMDESC_TALISMAN_TREND0+quality_trend-1));
	}

	//	damage
	if( (m_Essence.ess.damage_high || m_Essence.ess.damage_low) && m_Essence.ess.attack_enhance )
	{
		AddDescText(quality_trend == 2 || quality_trend == 3 ? darkgold : white, false, pDescTab->GetWideString(ITEMDESC_PHYDAMAGE), m_Essence.ess.damage_low - m_Essence.ess.attack_enhance, m_Essence.ess.damage_high - m_Essence.ess.attack_enhance);
		AddDescText(green, true, pDescTab->GetWideString(ITEMDESC_ADD_NUM), m_Essence.ess.attack_enhance);
	}
	else if(m_Essence.ess.damage_high || m_Essence.ess.damage_low)
	{
		AddDescText(quality_trend == 2 || quality_trend == 3 ? darkgold : white, true, pDescTab->GetWideString(ITEMDESC_PHYDAMAGE), m_Essence.ess.damage_low - m_Essence.ess.attack_enhance, m_Essence.ess.damage_high - m_Essence.ess.attack_enhance);
	}

	//	Physical defence
//	if( m_Essence.ess.defense )
// 		AddDescText(quality_trend == 13 ? darkgold : white, true, pDescTab->GetWideString(ITEMDESC_PHYDEFENCE), m_Essence.ess.defense);
	
	//	hp
	if( m_Essence.ess.hp && m_Essence.ess.hp_enhance )
	{
		AddDescText(quality_trend == 4 ? darkgold : white, false, pDescTab->GetWideString(ITEMDESC_HPEXTRA), m_Essence.ess.hp - m_Essence.ess.hp_enhance);
		AddDescText(green, true, pDescTab->GetWideString(ITEMDESC_ADD_NUM), m_Essence.ess.hp_enhance);
	}
	else if( m_Essence.ess.hp )
	{		
		AddDescText(quality_trend == 4 ? darkgold : white, true, pDescTab->GetWideString(ITEMDESC_HPEXTRA), m_Essence.ess.hp - m_Essence.ess.hp_enhance);
	}
	//	mp
	if( m_Essence.ess.mp && m_Essence.ess.mp_enhance)
	{
		AddDescText(quality_trend == 5 ? darkgold : white, false, pDescTab->GetWideString(ITEMDESC_MPEXTRA), m_Essence.ess.mp - m_Essence.ess.mp_enhance);
		AddDescText(green, true, pDescTab->GetWideString(ITEMDESC_ADD_NUM), m_Essence.ess.mp_enhance);
	}
	else if( m_Essence.ess.mp )
	{
		AddDescText(quality_trend == 5 ? darkgold : white, true, pDescTab->GetWideString(ITEMDESC_MPEXTRA), m_Essence.ess.mp - m_Essence.ess.mp_enhance);
	}
	//	Attack rating
//	if (m_Essence.ess.attack)
// 		AddDescText(quality_trend == 6 ? darkgold : white, true, pDescTab->GetWideString(ITEMDESC_ATKRATING), m_Essence.ess.attack);
	
	//	Dodge
//	if (m_Essence.ess.armor)
// 		AddDescText(quality_trend == 7 ? darkgold : white, true, pDescTab->GetWideString(ITEMDESC_DODGE), m_Essence.ess.armor);
	
	for(int i=0; i<5; i++)
	{
		if( m_Essence.ess.resistance[i] )
			AddDescText(quality_trend == 8 + i ? darkgold : white, true, pDescTab->GetWideString(ITEMDESC_TALISMAN_RESIST_1 + i), m_Essence.ess.resistance[i]);
	}

	if (m_pDBEssence->renascence_count > 0)
	{
		int col = (pHost->GetRebornCount() >= m_pDBEssence->renascence_count) ? ITEMDESC_COL_WHITE : ITEMDESC_COL_RED;
		AddDescText(col, false, pDescTab->GetWideString(ITEMDESC_RENASCENCE_REQUIRED), pDescTab->GetWideString(ITEMDESC_RENASCENCE_1+m_pDBEssence->renascence_count-1));
	}

	BuildRaceReqDesc();

	//	Profession requirement
	if (m_pDBEssence->sect_mask == 0)
		AddProfReqDesc(m_iProfReq, m_iProfReq2); // Modified 2011-07-18.
	else
	{
		static const unsigned int uAllSect = (1<<NUM_SECT) -1;
		static const unsigned int uHumanSect = (1 << NUM_HUMAN_SECT) - 1;
		static const unsigned int uOrcSect = uAllSect - uHumanSect;
		CECPlayer::SECT_LEVEL2_INFO* pInfo = CECPlayer::GetSectInfoByProf(pHost->GetProfession());

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

		AddDescText(white, true, pDescTab->GetWideString(ITEMDESC_GOD_DEVIL_REQUIRE), str);
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

			AddDescText(pHost->HasTitle(m_pDBEssence->title_require) ? white : ITEMDESC_COL_RED, true, pDescTab->GetWideString(ITEMDESC_TITLE_REQUIRE), title);
		}
	}

	//	Level requirment
	int iLevelReq = m_iLevelReq;
	int col = pHost->GetBasicProps().iLevel >= iLevelReq ? ITEMDESC_COL_WHITE : ITEMDESC_COL_RED;
	AddDescText(col, true, pDescTab->GetWideString(ITEMDESC_LEVELREQ), iLevelReq);
	
	//	Gender requirement
	BuildGenderReqDesc();

	//	Add addon properties
	if (strAddon.GetLength())
		m_strDesc += strAddon;
	
	//	Price
	AddPriceDesc(white, bRepair);

	//	Suite description
	AddSuiteDesc();

	//	Extend description
	AddExtDescText();

	if (strAddonNew)
	{
		m_strDesc += _AL("\t");
		m_strDesc += strAddonNew;
	}

	return m_strDesc;
}

ACString CECIvtrTalismanMain::_Serialize() const
{
	const size_t sz = sizeof(m_Essence.ess)+m_Essence.addons.size()*sizeof(int);

	if (sz > 256)
		return ACString();

	DWORD c_sz = 1024;
	char buf[1024];
	unsigned char out[1024];
	memcpy(buf, &m_Essence.ess, sizeof(m_Essence.ess));

	if (m_Essence.addons.size())
		memcpy(buf+sizeof(m_Essence.ess), m_Essence.addons.begin(), m_Essence.addons.size()*sizeof(int));

	AFilePackage::Compress((unsigned char*)buf, sz, out, &c_sz);
	ACString str = BufferMakeString(out, c_sz);
	return str;
}

bool CECIvtrTalismanMain::_Unserialize(const ACHAR* szText)
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

	if (sz_u < sizeof(m_Essence.ess))
	{
		delete[] p;
		return false;
	}

	memcpy(&m_Essence.ess, buf, sizeof(m_Essence.ess));
	sz_u -= sizeof(m_Essence.ess);

	if (sz_u)
	{
		sz_u /= sizeof(int);
		int* p1 = (int*)(buf + sizeof(m_Essence.ess));

		for (size_t i = 0; i < sz_u; i++)
			m_Essence.addons.push_back(p1[i]);
	}

	delete[] p;
	m_bNeedUpdate = false;
	return true;
}

//	Get scaled item
INT64 CECIvtrTalismanMain::GetScaledPrice()
{
	if (m_iScaleType != SCALE_SELL)
		return CECIvtrItem::GetScaledPrice();

	INT64 iPrice = m_iPrice * m_iCount;
	return iPrice;
}

//	Add price description
void CECIvtrTalismanMain::AddPriceDesc(int col, bool bRepair)
{
	if (!IsEquipment() && bRepair)
	{
		TrimLastReturn();
		return;
	}

	CECStringTab* pDescTab = g_pGame->GetItemDesc();
	if (m_iScaleType == SCALE_BOOTH)
	{
		ACHAR szPrice[256];
		GetPriceText(szPrice, m_iPrice);
		AddDescText(col, false, pDescTab->GetWideString(ITEMDESC_UNITPRICE), szPrice);
	}
	else if (m_iScaleType == SCALE_SELL && m_iCount > 1)
	{
		ACHAR szPrice1[256];
		GetPriceText(szPrice1, m_iPrice);
		ACHAR szPrice2[256];
		GetPriceText(szPrice2, GetScaledPrice());
		AddDescText(col, false, pDescTab->GetWideString(ITEMDESC_PRICEWITHTOTAL), szPrice1, szPrice2);
	}
	else
	{
		ACHAR szPrice[256];
		GetPriceText(szPrice, GetScaledPrice());
		AddDescText(col, false, pDescTab->GetWideString(ITEMDESC_PRICE), szPrice);
	}
}

//	Get item description for booth buying
const wchar_t* CECIvtrTalismanMain::GetBoothBuyDesc()
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

//	Decide equipment name color
int CECIvtrTalismanMain::DecideNameCol()
{
	int col = CECIvtrItem::DecideNameCol();
	return col;
}

int CECIvtrTalismanMain::GetFixSkill() 
{
	int iRet = 0;
	if (m_Essence.addons.size()==0 || m_Essence.addons[0]==0)
		return iRet;
	
	DATA_TYPE dt;
	EQUIPMENT_ADDON * pAddOn = (EQUIPMENT_ADDON *) g_pGame->GetElementDataMan()->get_data_ptr(m_Essence.addons[0], ID_SPACE_ADDON, dt);
	if( dt != DT_EQUIPMENT_ADDON )
		return iRet;
	
	if (pAddOn->type != 38)
		return iRet;
	
	return pAddOn->param1;
}

int CECIvtrTalismanMain::GetFixSkillLevel() 
{
	int iRet = 0;
	if (m_Essence.addons.size()==0 || m_Essence.addons[0]==0)
		return iRet;
	
	DATA_TYPE dt;
	EQUIPMENT_ADDON * pAddOn = (EQUIPMENT_ADDON *) g_pGame->GetElementDataMan()->get_data_ptr(m_Essence.addons[0], ID_SPACE_ADDON, dt);
	if( dt != DT_EQUIPMENT_ADDON )
		return iRet;
	
	if (pAddOn->type != 38)
		return iRet;
	
	return pAddOn->param2;
}

///////////////////////////////////////////////////////////////////////////
//	
//	Implement CECIvtrSpeaker
//	
///////////////////////////////////////////////////////////////////////////

CECIvtrSpeaker::CECIvtrSpeaker(int tid, int expire_date) : CECIvtrEquip(tid, expire_date)
{
	m_iCID		= ICID_SPAKER;
	
	//	Get database data
	elementdataman* pDB = g_pGame->GetElementDataMan();
	DATA_TYPE DataType;
	m_pDBEssence	= (SPEAKER_ESSENCE*)pDB->get_data_ptr(tid, ID_SPACE_ESSENCE, DataType);

	m_iPileLimit	= m_pDBEssence->pile_num_max;
	m_iPrice		= m_pDBEssence->price;
	m_iShopPrice	= m_pDBEssence->shop_price;
	m_iLevelReq		= 0;
	
	m_iProfReq		= 0;
	m_iLevelReqDrop = 0;
	m_iGenderReq	= 0;
	m_iRaceReq		= 0;
	m_iRebornReq	= 0;
	m_iProcType		= m_pDBEssence->proc_type;

	m_iEquipMask	= EQUIP_MASK_SPEAKER;
}

CECIvtrSpeaker::CECIvtrSpeaker(const CECIvtrSpeaker& s) : CECIvtrEquip(s)
{
	m_pDBEssence	= s.m_pDBEssence;
	m_iLevelReq		= s.m_iLevelReq;
	m_iLevelReqDrop	= s.m_iLevelReqDrop;	
	m_iProfReq		= s.m_iProfReq;
	m_iGenderReq	= s.m_iGenderReq;
	m_iRaceReq		= s.m_iRaceReq;
	m_iRebornReq	= s.m_iRebornReq;
	m_strMaker		= s.GetMakerName();
}

CECIvtrSpeaker::~CECIvtrSpeaker()
{
}

//	Set item detail information
bool CECIvtrSpeaker::SetItemInfo(BYTE* pInfoData, int iDataLen)
{
	CECIvtrItem::SetItemInfo(pInfoData, iDataLen);
	
	return true;
}

GNET::Octets CECIvtrSpeaker::gen_item_info()
{
	return GNET::Octets();
}

//	Get item icon file name
const char* CECIvtrSpeaker::GetIconFile()
{
	return g_pGame->GetDataPath(m_pDBEssence->file_icon);
}

//	Get item name
const wchar_t* CECIvtrSpeaker::GetName()
{
	return m_pDBEssence->name;
}

//	Get drop model for shown
const char * CECIvtrSpeaker::GetDropModel()
{
	return g_pGame->GetDataPath(m_pDBEssence->file_matter);
}

//	Build add-ons properties description
void CECIvtrSpeaker::BuildAddOnPropDesc()
{
}

//	Get item description text
const wchar_t* CECIvtrSpeaker::GetNormalDesc(bool bRepair)
{
	if (m_bNeedUpdate)
		return NULL;

	// Build addon desc and save it.
	m_strDesc = _AL("");
	BuildAddOnPropDesc();
	AWString strAddon = m_strDesc;

	m_strDesc = _AL("");

	CECStringTab* pDescTab = g_pGame->GetItemDesc();
	CECStringTab* pFixMsg = g_pGame->GetFixedMsgTab();
	CECHostPlayer* pHost = g_pGame->GetGameRun()->GetHostPlayer();

	//	Try to build item description
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

	//	Suite description
	AddSuiteDesc();

	//	Extend description
	AddExtDescText();

	return m_strDesc;
}

ACString CECIvtrSpeaker::_Serialize() const
{
	return CECIvtrItem::_Serialize();
}

bool CECIvtrSpeaker::_Unserialize(const ACHAR* szText)
{
	return CECIvtrItem::_Unserialize(szText);
}

//	Get scaled item
INT64 CECIvtrSpeaker::GetScaledPrice()
{
	if (m_iScaleType != SCALE_SELL)
		return CECIvtrItem::GetScaledPrice();

	INT64 iPrice = m_iPrice * m_iCount;
	return iPrice;
}

//	Add price description
void CECIvtrSpeaker::AddPriceDesc(int col, bool bRepair)
{
	if (!IsEquipment() && bRepair)
	{
		TrimLastReturn();
		return;
	}

	CECStringTab* pDescTab = g_pGame->GetItemDesc();
	if (m_iScaleType == SCALE_BOOTH)
	{
		ACHAR szPrice[256];
		GetPriceText(szPrice, m_iPrice);
		AddDescText(col, false, pDescTab->GetWideString(ITEMDESC_UNITPRICE), szPrice);
	}
	else if (m_iScaleType == SCALE_SELL && m_iCount > 1)
	{
		ACHAR szPrice1[256];
		GetPriceText(szPrice1, m_iPrice);
		ACHAR szPrice2[256];
		GetPriceText(szPrice2, GetScaledPrice());
		AddDescText(col, false, pDescTab->GetWideString(ITEMDESC_PRICEWITHTOTAL), szPrice1, szPrice2);
	}
	else
	{
		ACHAR szPrice[256];
		GetPriceText(szPrice, GetScaledPrice());
		AddDescText(col, false, pDescTab->GetWideString(ITEMDESC_PRICE), szPrice);
	}
}

//	Get item description for booth buying
const wchar_t* CECIvtrSpeaker::GetBoothBuyDesc()
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

//	Decide equipment name color
int CECIvtrSpeaker::DecideNameCol()
{
	int col = CECIvtrItem::DecideNameCol();
	return col;
}


///////////////////////////////////////////////////////////////////////////
//	
//	Implement CECIvtrChangeShapeCard
//	
///////////////////////////////////////////////////////////////////////////

CECIvtrChangeShapeCard::CECIvtrChangeShapeCard(int tid, int expire_date) : CECIvtrEquip(tid, expire_date)
{
	m_iCID	= ICID_CHANGE_SHAPE_CARD;

	//	Get database data
	elementdataman* pDB = g_pGame->GetElementDataMan();
	DATA_TYPE DataType;
	m_pDBEssence = (CHANGE_SHAPE_CARD_ESSENCE*)pDB->get_data_ptr(tid, ID_SPACE_ESSENCE, DataType);
	memset(&m_Essence, 0, sizeof(m_Essence));

	m_iProfReq		= m_pDBEssence->character_combo_id;
	m_iProfReq2		= m_pDBEssence->character_combo_id2; // Added 2011-07-18.	
	m_iLevelReq		= m_pDBEssence->require_level;
	m_iGenderReq	= m_pDBEssence->require_gender;
	m_iRaceReq		= 0;
	m_iRebornReq	= m_pDBEssence->renascence_count;
	m_iPileLimit	= m_pDBEssence->pile_num_max;
	m_iPrice		= m_pDBEssence->price;
	m_iShopPrice	= m_pDBEssence->shop_price;
	m_iProcType		= m_pDBEssence->proc_type;
	m_iLevelReqDrop	= 0;
	m_iEquipMask	= EQUIP_MASK_FASHION_NOSE;

	// 获取对应的怪物阵营 只是为了显示
	const void * pData = pDB->get_data_ptr(m_pDBEssence->ref_temp_id, ID_SPACE_CONFIG, DataType);
	if (DataType == DT_CHANGE_SHAPE_PROP_CONFIG)
	{
		CHANGE_SHAPE_PROP_CONFIG* pConfig = (CHANGE_SHAPE_PROP_CONFIG*)pData;
		const void* pData2 = pDB->get_data_ptr(pConfig->monster_id, ID_SPACE_ESSENCE, DataType);
		if(DataType == DT_MONSTER_ESSENCE)
		{
			MONSTER_ESSENCE* pMonster = (MONSTER_ESSENCE*)pData2;
			for (int i = 0; i< 32; ++i)
			{
				if( pMonster->monster_faction & (1<<i))
				{
					m_iMonsterFaction = i;
					break;
				}	
			}
		}
	}
}

CECIvtrChangeShapeCard::CECIvtrChangeShapeCard(const CECIvtrChangeShapeCard& s) : CECIvtrEquip(s)
{
	m_pDBEssence	= s.m_pDBEssence;
	m_Essence		= s.m_Essence;
	m_iLevelReq		= s.m_iLevelReq;	
	m_iProfReq		= s.m_iProfReq;
	m_iGenderReq	= s.m_iGenderReq;
	m_iRaceReq		= s.m_iRaceReq;
	m_iRebornReq	= s.m_iRebornReq;
	m_iLevelReqDrop	= s.m_iLevelReqDrop;
	m_iMonsterFaction = s.m_iMonsterFaction;
}

CECIvtrChangeShapeCard::~CECIvtrChangeShapeCard()
{
}

//	Set item detail information
bool CECIvtrChangeShapeCard::SetItemInfo(BYTE* pInfoData, int iDataLen)
{
	CECIvtrItem::SetItemInfo(pInfoData, iDataLen);

	if(iDataLen < sizeof(m_Essence.ess))
		return false;

	if( iDataLen >= sizeof(m_Essence.ess) )
		memcpy(&m_Essence.ess, pInfoData, sizeof(m_Essence.ess));

	m_Essence.addons.clear();
	iDataLen -= sizeof(m_Essence.ess);
	if( iDataLen >= (int)sizeof(int) )
	{
		iDataLen -= sizeof(int);

		int size_addon = *(int *)(pInfoData + sizeof(m_Essence.ess)) / sizeof(int);
		int * addon_id = (int *)(pInfoData + sizeof(m_Essence.ess) + sizeof(int));

		for(int i=0; i<size_addon && iDataLen >= (int)sizeof(int); i++)
		{
			m_Essence.addons.push_back(*addon_id);

			addon_id ++;
			iDataLen -= sizeof(int);
		}
	}	
	return true;
}

//	Get item icon file name
const char* CECIvtrChangeShapeCard::GetIconFile()
{
	return g_pGame->GetDataPath(m_pDBEssence->file_icon);
}

//	Get item name
const wchar_t* CECIvtrChangeShapeCard::GetName()
{
	return m_pDBEssence->name;
}

//	Get item cool time
int CECIvtrChangeShapeCard::GetCoolTime(int* piMax/* NULL */)
{
	return 0;
}
//	Build add-ons properties description
void CECIvtrChangeShapeCard::BuildAddOnPropDesc()
{
	CECStringTab* pDescTab = g_pGame->GetItemDesc();
	int color = ITEMDESC_COL_LIGHTBLUE;

	for (size_t i=0; i<m_Essence.addons.size(); i++)
	{
		DATA_TYPE dt;
		EQUIPMENT_ADDON * pAddOn = (EQUIPMENT_ADDON *) g_pGame->GetElementDataMan()->get_data_ptr(m_Essence.addons[i], ID_SPACE_ADDON, dt);
		if( dt != DT_EQUIPMENT_ADDON )
			continue;

		if (pAddOn->type == 39)
			continue;

		AddDescText(color, true, _AL("%s"), GetEquipAddonDesc(pAddOn->id, pAddOn->type, pAddOn->param1, pAddOn->param2, pAddOn->param3, 1));
	}
}
int CECIvtrChangeShapeCard::DecideNameCol()
{
	//白1-3 绿4-6   蓝7-9 紫10-12 黄13-15 暗金16-20
	static int col[6]=
	{
		ITEMDESC_COL_WHITE,
		ITEMDESC_COL_GREEN,
		ITEMDESC_COL_POOLBLUE,
		ITEMDESC_COL_PURPLE,
		ITEMDESC_COL_YELLOW,
		ITEMDESC_COL_DARKGOLD,
	};

	int level = m_Essence.ess.level;
	ASSERT( level >= 1);
	int index = (level-1)/3;
	if (index > 5)
		index = 5;
	return col[index];
}

//	Get item description text
const wchar_t* CECIvtrChangeShapeCard::GetNormalDesc(bool bRepair)
{
	if(m_bNeedUpdate)
		return NULL;

	m_strDesc = _AL("");
	BuildAddOnPropDesc();
	AWString strAddon = m_strDesc;
	m_strDesc = _AL("");

	//	Try to build item description
	CECStringTab* pDescTab = g_pGame->GetItemDesc();
	CECStringTab* pFixMsg = g_pGame->GetFixedMsgTab();
	int red = ITEMDESC_COL_RED;
	int white = ITEMDESC_COL_WHITE;
	int yellow = ITEMDESC_COL_YELLOW;
	int green = ITEMDESC_COL_GREEN;
	int lightblue = ITEMDESC_COL_LIGHTBLUE;
	int darkgold	= ITEMDESC_COL_DARKGOLD;


	int name_color = DecideNameCol();
	CECHostPlayer* pHost = g_pGame->GetGameRun()->GetHostPlayer();
	if (m_iCount > 1)
		AddDescText(name_color, true, pDescTab->GetWideString(ITEMDESC_NAMENUMBER), GetName(), m_iCount);
	else
		AddDescText(name_color, true, pDescTab->GetWideString(ITEMDESC_NAME), GetName());
	int monster_faction = 0;

	AddDescText(white, true, pDescTab->GetWideString(ITEMDESC_MONSTER_FACTION), pDescTab->GetWideString(ITEMDESC_MONSTER_FACTION1 + m_iMonsterFaction));
	AddDescText(white, true, pDescTab->GetWideString(ITEMDESC_CARD_LEVEL), m_Essence.ess.level);

	if (m_Essence.ess.master_id)
	{
		AddDescText(yellow, true, pDescTab->GetWideString(ITEMDESC_CARD_OWENER), m_Essence.ess.master_name);
		if (m_Essence.ess.stamina == 0)
			AddDescText(red, true, pDescTab->GetWideString(ITEMDESC_WAKAN), m_Essence.ess.stamina, m_Essence.ess.max_stamina);
		else
			AddDescText(white, true, pDescTab->GetWideString(ITEMDESC_WAKAN), m_Essence.ess.stamina, m_Essence.ess.max_stamina);

		// 幻化契合度，最大为5阶
		if (m_Essence.ess.exp_level ==  5 )
			AddDescText(white, true, pDescTab->GetWideString(ITEMDESC_WAKAN_CONSISTENT2),  m_Essence.ess.exp_level);			
		else if (m_Essence.ess.max_exp )
		{
			AddDescText(white, true, pDescTab->GetWideString(ITEMDESC_WAKAN_CONSISTENT),  m_Essence.ess.exp_level, (int)((float)m_Essence.ess.exp*100/(float)m_Essence.ess.max_exp));				
		}
		// 品级
		int col = white;
		switch(m_Essence.ess.quality)
		{
		case 1:
			col = white;
			break;
		case 2:
			col = green;
			break;
		case 3:
			col = lightblue;
			break;
		case 4:
			col = darkgold;
			break;
		case 5:
			col = red;
			break;
		default:
			col = white;
			break;
		}
		AddDescText(col, true, pDescTab->GetWideString(ITEMDESC_CARD_GRADE), pDescTab->GetWideString(ITEMDESC_CARD_GRADE+m_Essence.ess.quality));
		if (m_Essence.ess.damage_high)
			AddDescText(white, true, pDescTab->GetWideString(ITEMDESC_PHYDAMAGE), m_Essence.ess.damage_low, m_Essence.ess.damage_high);
		if (m_Essence.ess.defense)
			AddDescText(white, true, pDescTab->GetWideString(ITEMDESC_EQUIPADDON_4), m_Essence.ess.defense);
		if (m_Essence.ess.hp)
			AddDescText(white, true, pDescTab->GetWideString(ITEMDESC_EQUIPADDON_1), m_Essence.ess.hp);
		if (m_Essence.ess.mp)
			AddDescText(white, true, pDescTab->GetWideString(ITEMDESC_EQUIPADDON_2), m_Essence.ess.mp);
		if (m_Essence.ess.attack)
			AddDescText(white, true, pDescTab->GetWideString(ITEMDESC_EQUIPADDON_5), m_Essence.ess.attack);
		if (m_Essence.ess.armor)
			AddDescText(white, true, pDescTab->GetWideString(ITEMDESC_EQUIPADDON_6), m_Essence.ess.armor);

		for(int i=0; i< 6; ++i)
		{
			if(m_Essence.ess.resistance[i])
				AddDescText(white, true, pDescTab->GetWideString(ITEMDESC_EQUIPADDON_9+i), m_Essence.ess.resistance[i]);
		}
	}
	else
		AddDescText(white, true, pDescTab->GetWideString(ITEMDESC_NO_BOND));

	//  Gender requirement
	BuildGenderReqDesc();

	if (m_pDBEssence->renascence_count > 0)
	{
		int col = (pHost->GetRebornCount() >= m_pDBEssence->renascence_count) ? ITEMDESC_COL_WHITE : ITEMDESC_COL_RED;
		AddDescText(col, true, pDescTab->GetWideString(ITEMDESC_RENASCENCE_REQUIRED), pDescTab->GetWideString(ITEMDESC_RENASCENCE_1+m_pDBEssence->renascence_count-1));
	}
	//  Sect requirement
	if (m_pDBEssence->require_sect == 0)
		AddProfReqDesc(m_iProfReq, m_iProfReq2); // Modified 2011-07-18.
	else
	{
		static const unsigned int uAllSect = (1 << NUM_SECT) - 1;
		static const unsigned int uHumanSect = (1 << NUM_HUMAN_SECT) - 1;
		static const unsigned int uOrcSect = uAllSect - uHumanSect;
		CECPlayer::SECT_LEVEL2_INFO* pInfo = CECPlayer::GetSectInfoByProf(pHost->GetProfession());

		if ( ( m_pDBEssence->require_sect == uHumanSect) ||
			 (m_pDBEssence->require_sect == uOrcSect) ||
			 (m_pDBEssence->require_sect == uAllSect))
		{
			// 不显示
		}
		else
		{
			int col = (pInfo && (pInfo->mask_sect & m_pDBEssence->require_sect)) ? ITEMDESC_COL_WHITE : ITEMDESC_COL_RED;
			AddDescText(col, false, pDescTab->GetWideString(ITEMDESC_SECT_REQUIRE));

			for (int i = 0; i < NUM_SECT; i++)
			{
				if (m_pDBEssence->require_sect & (1 << i))
				{
					m_strDesc += _AL(" ");
					AddDescText(col, false, pFixMsg->GetWideString(FIXMSG_SECT_1 + i));
				}
			}

			m_strDesc += _AL("\\r");
		}
	}
	//	Level requirement
	if (m_pDBEssence->require_level)
	{
		int col = g_pGame->GetGameRun()->GetHostPlayer()->GetBasicProps().iLevel >= m_pDBEssence->require_level ? white : red;
		AddDescText(col, true, pDescTab->GetWideString(ITEMDESC_LEVELREQ), m_pDBEssence->require_level);
	}
	
	AddStateDesc(pDescTab);
	AddExpireTimeDesc();
	//	Add addon properties
	if (m_Essence.ess.master_id && strAddon.GetLength())
		m_strDesc += strAddon;
	//	Price
	AddPriceDesc(white, bRepair);

	AddExtDescText();
	return m_strDesc;
}

//	Check item use condition
bool CECIvtrChangeShapeCard::CheckUseCondition()
{
	if (!m_pDBEssence)
		return false;
	if (!CheckCrossUseCondition())
		return false;
	return true;
}

//	Get drop model for shown
const char * CECIvtrChangeShapeCard::GetDropModel()
{
	return g_pGame->GetDataPath(m_pDBEssence->file_matter);
}

void CECIvtrChangeShapeCard::UpdateDuration(int dec, int result )
{
	m_Essence.ess.stamina -= dec;
	if( m_Essence.ess.stamina != result)
		m_Essence.ess.stamina = result;
	UpdateInfoMD5();
}

void CECIvtrChangeShapeCard::UpdataExp(int newexp)
{
	m_Essence.ess.exp = newexp;
	UpdateInfoMD5();
}

ACString CECIvtrChangeShapeCard::_Serialize() const
{
	const size_t sz = sizeof(m_Essence.ess)+m_Essence.addons.size()*sizeof(int);

	if (sz > 256)
		return ACString();

	DWORD c_sz = 1024;
	char buf[1024];
	unsigned char out[1024];
	memcpy(buf, &m_Essence.ess, sizeof(m_Essence.ess));

	if (m_Essence.addons.size())
		memcpy(buf+sizeof(m_Essence.ess), m_Essence.addons.begin(), m_Essence.addons.size()*sizeof(int));

	AFilePackage::Compress((unsigned char*)buf, sz, out, &c_sz);
	ACString str = BufferMakeString(out, c_sz);
	return str;
}

bool CECIvtrChangeShapeCard::_Unserialize(const ACHAR* szText)
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

	if (sz_u < sizeof(m_Essence.ess))
	{
		delete[] p;
		return false;
	}

	memcpy(&m_Essence.ess, buf, sizeof(m_Essence.ess));
	sz_u -= sizeof(m_Essence.ess);

	if (sz_u)
	{
		sz_u /= sizeof(int);
		int* p1 = (int*)(buf + sizeof(m_Essence.ess));

		for (size_t i = 0; i < sz_u; i++)
			m_Essence.addons.push_back(p1[i]);
	}

	delete[] p;
	m_bNeedUpdate = false;
	return true;
}