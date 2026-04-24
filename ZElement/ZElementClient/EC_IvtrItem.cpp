/*
 * FILE: EC_IvtrItem.cpp
 *
 * DESCRIPTION: 
 *
 * CREATED BY: Duyuxin, 2004/11/18
 *
 * HISTORY: 
 *
 * Copyright (c) 2004 Archosaur Studio, All Rights Reserved.
 */

#include "EC_Global.h"
#include "EC_IvtrItem.h"
#include "EC_IvtrMaterial.h"
#include "EC_IvtrConsume.h"
#include "EC_IvtrTaskItem.h"
#include "EC_IvtrScroll.h"
#include "EC_IvtrStone.h"
#include "EC_IvtrTalismanMain.h"
#include "EC_IvtrRune.h"
#ifndef _INVENTORY_DESC_MAN //Added by Ford.W 2010-07-16.//Modified 2010-07-22
#include "EC_Game.h"
#include "EC_GameRun.h"
#include "EC_GameSession.h"
#include "EC_HostPlayer.h"
#else
#include "EC_Game_HintTool.h"
#include "EC_GameRun_HintTool.h"
#include "EC_GameSession_HintTool.h"
#include "EC_HostPlayer_HintTool.h"
#endif
#include "EC_Resource.h"
#include "EC_FixedMsg.h"
#include "EC_IvtrDestroyingEssence.h"
#include "EC_StringTab.h"
#include "EC_IvtrHTItem.h"
#include "EC_GameUIMan.h"
#include "EC_UIManager.h"

#include "AAssist.h"
#include "ATime.h"
#include "AChar.h"
#include "ACounter.h"

#include "elementdataman.h"
#include "itemdataman.h"
#include "hometowntemplate.h"

#include "Network/IOLib/gnoctets.h"
#include "Network/IOLib/gnsecure.h"

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
//	Implement CECIvtrItem
//	
///////////////////////////////////////////////////////////////////////////

CECIvtrItem::CECIvtrItem(int tid, int expire_date)
{
	m_iCID			= ICID_ITEM;
	m_tid			= tid;
	m_expire_date	= expire_date;
	m_iState		= 0;
	m_iCount		= 0;
	m_iPileLimit	= 1;
	m_iPrice		= 1;
	m_iShopPrice	= 1;
	m_bNeedUpdate	= true;
	m_bUpdating		= false;
	m_dwUptTime		= 0;
	m_iEquipMask	= 0;
	m_bEmbeddable	= false;
	m_bUseable		= false;
	m_bFrozen		= false;
	m_bNetFrozen	= false;
	m_iProcType		= 0;
	m_dwUseFlags	= 0;
	m_fPriceScale	= PLAYER_PRICE_SCALE;
	m_iScaleType	= SCALE_SELL;
	m_pDescIvtr		= NULL;
	memset(m_infoMD5, 0, 16);
	m_bMD5OK		= true;
}

CECIvtrItem::CECIvtrItem(const CECIvtrItem& s)
{
	m_iCID			= s.m_iCID;
	m_tid			= s.m_tid;
	m_expire_date	= s.m_expire_date;
	m_iState		= s.m_iState;
	m_iCount		= s.m_iCount;
	m_iPileLimit	= s.m_iPileLimit;
	m_iPrice		= s.m_iPrice;
	m_iShopPrice	= s.m_iShopPrice;
	m_iEquipMask	= s.m_iEquipMask;
	m_bEmbeddable	= s.m_bEmbeddable;
	m_bUseable		= s.m_bUseable;
	m_bFrozen		= false;
	m_bNetFrozen	= false;
	m_dwUseFlags	= s.m_dwUseFlags;
	m_iScaleType	= s.m_iScaleType;
	m_fPriceScale	= s.m_fPriceScale;
	m_bNeedUpdate	= s.m_bNeedUpdate;
	m_iProcType		= s.m_iProcType;
	m_pDescIvtr		= s.m_pDescIvtr;
	memcpy(m_infoMD5, s.m_infoMD5, 16);
	m_bUpdating		= false;
	m_dwUptTime		= 0;
	m_bMD5OK		= s.m_bMD5OK;
}

//	Create an inventory item
CECIvtrItem* CECIvtrItem::CreateItem(int tid, int expire_date, int iCount, int idSpace/* 0 */)
{
	CECIvtrItem* pItem = NULL;

	//	Get item data type
	DATA_TYPE DataType = g_pGame->GetElementDataMan()->get_data_type(tid, (ID_SPACE)idSpace);

	switch (DataType)
	{
	case DT_EQUIPMENT_ESSENCE:		pItem = new CECIvtrEquip(tid, expire_date);			break;
	case DT_TALISMAN_MAINPART_ESSENCE:pItem = new CECIvtrTalismanMain(tid, expire_date);break;
	case DT_MEDICINE_ESSENCE:		pItem = new CECIvtrMedicine(tid, expire_date);		break;
	case DT_MATERIAL_ESSENCE:		pItem = new CECIvtrMaterial(tid, expire_date);		break;
	case DT_SKILLTOME_ESSENCE:		pItem = new CECIvtrSkillTome(tid, expire_date);		break;
	case DT_TRANSMITROLL_ESSENCE:	pItem = new CECIvtrTransmitRoll(tid, expire_date);	break;
	case DT_LUCKYROLL_ESSENCE:		pItem = new CECIvtrLuckyRoll(tid, expire_date);		break;
	case DT_TOWNSCROLL_ESSENCE:		pItem = new CECIvtrTownScroll(tid, expire_date);		break;
	case DT_REVIVESCROLL_ESSENCE:	pItem = new CECIvtrRevScroll(tid, expire_date);		break;
	case DT_TASKMATTER_ESSENCE:		pItem = new CECIvtrTaskItem(tid, expire_date);		break;
	case DT_TASKDICE_ESSENCE:		pItem = new CECIvtrTaskDice(tid, expire_date);		break;
	case DT_TASKNORMALMATTER_ESSENCE:	pItem = new CECIvtrTaskNmMatter(tid, expire_date);	break;
	case DT_GM_GENERATOR_ESSENCE:	pItem = new CECIvtrGMGenerator(tid, expire_date);	break;
	case DT_RECIPE_ESSENCE:			pItem = new CECIvtrRecipe(tid, expire_date);			break;
	case DT_FIREWORKS_ESSENCE:		pItem = new CECIvtrFirework(tid, expire_date);		break;
	case DT_ESTONE_ESSENCE:			pItem = new CECIvtrEStone(tid, expire_date);			break;
	case DT_PSTONE_ESSENCE:			pItem = new CECIvtrPStone(tid, expire_date);			break;
	case DT_SSTONE_ESSENCE:			pItem = new CECIvtrSStone(tid, expire_date);			break;
	case DT_REFINE_TICKET_ESSENCE:	pItem = new CECIvtrRefineTicket(tid, expire_date);	break;
	case DT_OFFLINE_TRUSTEE_ESSENCE:pItem = new CECIvtrOfflineTrustee(tid, expire_date);break;
	case DT_RECIPEROLL_ESSENCE:		pItem = new CECIvtrRecipeRoll(tid, expire_date);		break;
	case DT_DOUBLE_EXP_ESSENCE:		pItem = new CECIvtrDoubleExp(tid, expire_date);		break;
	case DT_DESTROYING_ESSENCE:		pItem = new CECIvtrDestroyingEssence(tid, expire_date); break;
	case DT_SKILLMATTER_ESSENCE:	pItem = new CECIvtrSkillMat(tid, expire_date); break;
	case DT_VEHICLE_ESSENCE:		pItem = new CECIvtrVehicle(tid, expire_date); break;
	case DT_COUPLE_JUMPTO_ESSENCE:	pItem = new CECIvtrCoupleJumpTo(tid, expire_date); break;
	case DT_LOTTERY_ESSENCE:		pItem = new CECIvtrLottery(tid, expire_date); break;
	case DT_CAMRECORDER_ESSENCE:	pItem = new CECCamRecorder(tid, expire_date); break;
	case DT_TEXT_FIREWORKS_ESSENCE:	pItem = new CECIvtrTextFirework(tid, expire_date);		break;
	case DT_TALISMAN_EXPFOOD_ESSENCE:	pItem = new CECIvtrTalismanExpFood(tid, expire_date);		break;
	case DT_TALISMAN_MERGEKATALYST_ESSENCE:	pItem = new CECIvtrTalismanMergeKatalyst(tid, expire_date);		break;
	case DT_TALISMAN_ENERGYFOOD_ESSENCE:	pItem = new CECIvtrTalismanEnergyFood(tid, expire_date);		break;
	case DT_SPEAKER_ESSENCE:		pItem = new CECIvtrSpeaker(tid, expire_date); break;
	case DT_SIEGE_ARTILLERY_SCROLL_ESSENCE: pItem = new CECIvtrArtilleryScroll(tid, expire_date); break;
	case DT_PET_BEDGE_ESSENCE:		pItem = new CECPetBedge(tid, expire_date); break;
	case DT_PET_FOOD_ESSENCE:		pItem = new CECPetFood(tid, expire_date); break;
	case DT_PET_SKILL_ESSENCE:		pItem = new CECPetSkill(tid, expire_date); break;
	case DT_PET_ARMOR_ESSENCE:		pItem = new CECPetArmor(tid, expire_date); break;
	case DT_PET_AUTO_FOOD_ESSENCE:	pItem = new CECPetAutoFood(tid, expire_date); break;
	case DT_PET_REFINE_ESSENCE:		pItem = new CECPetRefine(tid, expire_date); break;
	case DT_PET_ASSIST_REFINE_ESSENCE:	pItem = new CECPetAssistRefine(tid, expire_date); break;
	case DT_BOOK_ESSENCE:			pItem = new CECIvtrBook(tid, expire_date); break;
	case DT_EQUIP_SOUL_ESSENCE:		pItem = new CECIvtrEquipSoul(tid, expire_date); break;
	case DT_SPECIAL_NAME_ITEM_ESSENCE:		pItem = new CECIvtrSpecialNameItem(tid, expire_date); break;
	case DT_GIFT_BAG_ESSENCE:		pItem = new CECIvtrGiftBag(tid, expire_date);	break;
	case DT_VIP_CARD_ESSENCE:		pItem = new CECIvtrVIPCard(tid, expire_date);	break;
	case DT_CHANGE_SHAPE_CARD_ESSENCE:		pItem = new CECIvtrChangeShapeCard(tid, expire_date); break;
	case DT_CHANGE_SHAPE_STONE_ESSENCE:		pItem = new CECIvtrChangeShapeStone(tid, expire_date); break;
	case DT_MERCENARY_CREDENTIAL_ESSENCE:	pItem = new CECIvtrMercenaryCredential(tid, expire_date);break;
	case DT_TELEPORTATION_ESSENCE:	pItem = new CECIvtrTelePortation(tid, expire_date); break;
	case DT_TELEPORTATION_STONE_ESSENCE:	pItem = new CECIvtrTelePortationStone(tid, expire_date); break;
	case DT_UPGRADE_EQUIP_STONE_ESSENCE:	pItem = new CECIvtrUpgradeEquipStone(tid, expire_date);	break;
	case DT_LOTTERY2_ESSENCE:				pItem = new CECIvtrLottery2(tid, expire_date);	break;
	case DT_GEM_ESSENCE:					pItem = new CECIvtrGem(tid, expire_date); break;
	case DT_GEM_SEAL_ESSENCE:				pItem = new CECIvtrGemSeal(tid, expire_date); break;
	case DT_GEM_DUST_ESSENCE:				pItem = new CECIvtrGemDust(tid, expire_date); break;
	case DT_GENERAL_ARTICLE_ESSENCE:		pItem = new CECIvtrGeneralArticle(tid, expire_date); break;
	case DT_LOTTERY3_ESSENCE:				pItem = new CECIvtrLottery3(tid, expire_date); break; //Added 2011-07-27.
	case DT_SMALL_ONLINE_GIFT_BAG_ESSENCE:	pItem = new CECIvtrSmallGiftBag( tid, expire_date ); break;	//Added 2012-03-23.
	case DT_SCROLL_UNLOCK_ESSENCE:			pItem = new CECIvtrScrollUnlockItem( tid, expire_date ); break;	//Added 2012-03-23.
	case DT_SCROLL_DIG_COUNT_INC_ESSENCE:	pItem = new CECIvtrScrollDigCountIncItem( tid, expire_date ); break;//Added 2012-03-23.
	case DT_RUNE_EQUIP_ESSENCE:				pItem = new CECIvtrRune(tid, expire_date); break;
	case DT_RUNE_SLOT_ESSENCE:				pItem = new CECIvtrRuneSlot(tid, expire_date); break;
	case DT_BOOTH_FIGURE_ITEM_ESSENCE:		pItem = new CECIvtrBoothFigure(tid, expire_date); break;
	case DT_FLAG_BUFF_ITEM_ESSENCE:			pItem = new CECIvtrFlagBuffItem(tid, expire_date); break;
	case DT_XINGZUO_ITEM_ESSENCE:			pItem = new CECIvtrAstrology(tid, expire_date); break;
	case DT_XINGZUO_ENERGY_ITEM_ESSENCE:	pItem = new CECIvtrAstrologyEnergyItem(tid, expire_date); break;
	case DT_CASH_MEDIC_MERGE_ITEM_ESSENCE:	pItem = new CECIvtrAgentCombinedItem(tid, expire_date); break;// 2012-10-16 添加 add by zy	
	case DT_GIFT_BAG_LOTTERY_DELIVER_ESSENCE: pItem = new CECIvtrGiftBagLotteryDeliver(tid, expire_date); break;// 2012-12-21
	case DT_LOTTERY_TANGYUAN_ITEM_ESSENCE:	pItem = new CECIvtrLotteryTangYuan(tid, expire_date); break;// 2012-12-26
	case DT_SALE_PROMOTION_ITEM_ESSENCE:	pItem = new CECSalePromotionItem(tid, expire_date);	break;
	case DT_GIFT_PACK_ITEM_ESSENCE:			pItem = new CECIvtrGiftPack(tid, expire_date);	break;
	case DT_PROP_ADD_MATERIAL_ITEM_ESSENCE:	pItem = new CECIvtrPropAddMaterial(tid, expire_date); break;
	case DT_PROP_ADD_ITEM_ESSENCE:			pItem = new CECIvtrPropAddItem(tid, expire_date); break;
	case DT_RUNE2013_ITEM_ESSENCE:			pItem = new CECIvtrRune2013(tid, expire_date); break;
	case DT_COLORANT_ITEM_ESSENCE:	pItem = new CECIvtrIndividualityItem(tid, expire_date); break;
	default:
	{
	//	ASSERT(0);
		pItem = new CECIvtrUnknown(tid);
	}
	}

	pItem->SetCount(iCount);
	pItem->UpdateInfoMD5();

	return pItem;
}

//	Create an inventory item
CECIvtrItem* CECIvtrItem::CreateHomeTownItem(int tid, int iCount, int idSpace/* 0 */)
{
	using namespace HOMETOWN;
	CECIvtrItem* pItem = NULL;
	//	Get item data type
	HOMETOWN::DATA_TYPE DataType = g_pGame->GetHomeTownDataMan()->get_data_type(tid, (HOMETOWN::ID_SPACE)idSpace);

	switch (DataType)
	{
	case DT_CROP_ESSENCE:		pItem = new CECIvtrHTCrop(tid);							break;
	case DT_FRUIT_ESSENCE:		pItem = new CECIvtrHTFruit(tid);						break;
	case DT_TOOL_ESSENCE:		pItem = new CECIvtrHTTool(tid);							break;
	default:
		pItem = new CECIvtrUnknown(tid);								break;
	}

	pItem->SetCount(iCount);
	pItem->UpdateInfoMD5();
	return pItem;
}
//	Check whether item2 is item1's candidate
bool CECIvtrItem::IsCandidate(int tid1, int tid2)
{
	elementdataman* pDB = g_pGame->GetElementDataMan();

	//	medicines and talisman energy food have candidates
	DATA_TYPE DataType1, DataType2;
	const void* pEssence1 = pDB->get_data_ptr(tid1, ID_SPACE_ESSENCE, DataType1);
	const void* pEssence2 = pDB->get_data_ptr(tid2, ID_SPACE_ESSENCE, DataType2);
	if( DataType1 != DataType2 )
		return false;

	switch(DataType1)
	{
	case DT_MEDICINE_ESSENCE:
		return ((const MEDICINE_ESSENCE*)pEssence1)->type == ((const MEDICINE_ESSENCE*)pEssence2)->type ? true : false;

	case DT_TALISMAN_ENERGYFOOD_ESSENCE:
		return true;

	default:
		return false;

	}
}

//	Check whether pItem2 is item1's candidate
bool CECIvtrItem::IsCandidate(int tid1, CECIvtrItem* pItem2, bool bFirstCandidate)
{
	ASSERT(pItem2);

	elementdataman* pDB = g_pGame->GetElementDataMan();
	DATA_TYPE DataType;
	
	//	medicines and talisman energy food have candidates
	switch(pItem2->GetClassID())
	{
	case CECIvtrItem::ICID_MEDICINE:
		{
			const MEDICINE_ESSENCE* pEssence1 = (const MEDICINE_ESSENCE*)pDB->get_data_ptr(tid1, ID_SPACE_ESSENCE, DataType);
			if (DataType != DT_MEDICINE_ESSENCE)
				return false;			
			if(bFirstCandidate)
			{
				if(pEssence1->type == ((CECIvtrMedicine*)pItem2)->GetDBEssence()->type )
					return true;
				else 
					return false;
			}
			else
			{
				if( pEssence1->type <= 5 &&  ((CECIvtrMedicine*)pItem2)->GetDBEssence()->type <= 5 && abs(pEssence1->type - ((CECIvtrMedicine*)pItem2)->GetDBEssence()->type )== 3)  //此处的3与药品类型值的差值一致，如果类型值改变，此处也要修改
					return true;
				//  加上了补充神力的药，类型为6和7，分别为瞬回和持续回DP
				else if ((pEssence1->type == 6 && ((CECIvtrMedicine*)pItem2)->GetDBEssence()->type == 7 ) || (pEssence1->type == 7 && ((CECIvtrMedicine*)pItem2)->GetDBEssence()->type == 6 ))
					return true;
				else
					return false;
			}
//			return pEssence1->type == ((CECIvtrMedicine*)pItem2)->GetDBEssence()->type ? true : false;
		}
	case CECIvtrItem::ICID_TALISMAN_ENERGYFOOD:
		{
			const TALISMAN_ENERGYFOOD_ESSENCE* pEssence1 = (const TALISMAN_ENERGYFOOD_ESSENCE*)pDB->get_data_ptr(tid1, ID_SPACE_ESSENCE, DataType);
			if (DataType != DT_TALISMAN_ENERGYFOOD_ESSENCE)
				return false;
			if(bFirstCandidate)
			{
				CECIvtrTalismanEnergyFood* pFood = (CECIvtrTalismanEnergyFood*)pItem2;
				if(pFood->GetEssence().energy_left < pFood->GetDBEssence()->energy_total )
					return true;
				else
					return false;
			}
			else
			{
				return true;
			}
			//return DataType == DT_TALISMAN_ENERGYFOOD_ESSENCE ? true : false;
		} 
	case CECIvtrItem::ICID_PET_AUTO_FOOD:
		{
			const PET_AUTO_FOOD_ESSENCE* pEssence1 = (const PET_AUTO_FOOD_ESSENCE*)pDB->get_data_ptr(tid1, ID_SPACE_ESSENCE, DataType);
			return DataType == DT_PET_AUTO_FOOD_ESSENCE? true : false;
		}
	default:
		return false;
	}
}

//	Get scaled price of item
INT64 CECIvtrItem::GetScaledPrice(int iUnitPrice, int iCount, int iScaleType, float fScale)
{
	if (!iCount)
		return 0;

	INT64 iPrice = 0;

	switch (iScaleType)
	{
	case SCALE_BUY:
		
		iPrice = (INT64)((INT64)iUnitPrice * fScale + 0.5f);
		/*if (iPrice >= 1000)
			iPrice = ((iPrice + 99) / 100) * 100;
		else if (iPrice >= 100)
			iPrice = ((iPrice + 9) / 10) * 10;*/

		iPrice *= iCount;
		break;

	case SCALE_SELL:

		iPrice = (INT64)((INT64)iUnitPrice * iCount * fScale + 0.5f);
		break;

	case SCALE_BOOTH:
	case SCALE_MAKE:
		
		iPrice = (INT64)iUnitPrice * iCount;
		break;

	default:
		
		iPrice = (INT64)iUnitPrice * iCount;
		break;
	}
	
	return iPrice;
}

//	Set item detail information
bool CECIvtrItem::SetItemInfo(BYTE* pInfoData, int iDataLen)
{
	m_bNeedUpdate	= false;
	m_bUpdating		= false;

	return true;
}

//	Merge item amount with another same kind item
//	Return the number of item which was merged.
int CECIvtrItem::MergeItem(int tid, int state, int iAmount)
{
	//	If item type is different or this item has been full, return.
	if (tid != m_tid || m_iState != state || m_iCount >= m_iPileLimit)
		return 0;

	int iNumAdd = iAmount;
	if (m_iCount + iNumAdd > m_iPileLimit)
		iNumAdd = m_iPileLimit - m_iCount;

	m_iCount += iNumAdd;

	return iNumAdd;
}

//	Add item amount. Return new amount of item
int CECIvtrItem::AddAmount(int iAmount)
{
	m_iCount += iAmount;
	a_Clamp(m_iCount, 0, m_iPileLimit);
	return m_iCount;
}

//	Get item icon file name
const char* CECIvtrItem::GetIconFile()
{
	return "Unknown.dds";
}

//	Get item name
const wchar_t* CECIvtrItem::GetName()
{
	CECStringTab* pDescTab = g_pGame->GetItemDesc();
	return pDescTab->GetWideString(ITEMDESC_ERRORITEM);
}

//	Get item's detail data from server
void CECIvtrItem::GetDetailDataFromSev(int iPack, int iSlot)
{
	if (!m_bNeedUpdate)
		return;

	if (m_bUpdating)
	{
		//	Update request has been sent
		if (a_GetTime() < m_dwUptTime + 5000)
			return;

		//	So long time has passed, update response hasn't arrived, ask again
	}

	m_dwUptTime = a_GetTime();
	m_bUpdating = true;

	g_pGame->GetGameSession()->c2s_CmdGetItemInfo((BYTE)iPack, (BYTE)iSlot);
}

//	Get item's detail data from local database
void CECIvtrItem::GetDetailDataFromLocal()
{
	itemdataman* pItemDataMan = g_pGame->GetItemDataMan();
	item_data* pData = (item_data*)pItemDataMan->get_item_for_sell(m_tid);
	if (pData)
		SetItemInfo((BYTE*)pData->item_content, pData->content_length);	
	UpdateInfoMD5();
}

INT64 CECIvtrItem::GetScaledPrice()
{
	int iPrice = m_iScaleType == SCALE_BUY ? m_iShopPrice : m_iPrice;
	return GetScaledPrice(iPrice, m_iCount, m_iScaleType, m_fPriceScale);
}

//	Trim the last '\r' in description string
void CECIvtrItem::TrimLastReturn()
{
	int iLen = m_strDesc.GetLength();
	if (iLen >= 2 && m_strDesc[iLen-2] == '\\' && m_strDesc[iLen-1] == 'r')
		m_strDesc.CutRight(2);
}

//	Add content to description string
void CECIvtrItem::AddDescText(int iCol, bool bRet, const ACHAR* szText, ...)
{
	CECStringTab* pDescTab = g_pGame->GetItemDesc();

	//	Add color
	if ((iCol >= ITEMDESC_COL_WHITE && iCol <= ITEMDESC_COL_CYANINE) || 
		(iCol >= ITEMDESC_EQUIPSTONE_COLOR1 && iCol <= ITEMDESC_EQUIPSTONE_COLOR20) || 
		(iCol>=ITEMDESC_COL_EARTHYELLOW && iCol <= ITEMDESC_COL_RESERVE9)  || 
		(iCol>=ITEMDESC_RUNECOLOR1 && iCol <= ITEMDESC_RUNECOLOR7) )
	{
		const wchar_t* szCol = pDescTab->GetWideString(iCol);
		m_strDesc += szCol;
	}

	ACHAR szLine[256];

	va_list argList;
	va_start(argList, szText);
	a_vsprintf(szLine, szText, argList);
	va_end(argList);
	
	m_strDesc += szLine;

	if (bRet)
		m_strDesc += _AL("\\r");
}

//	Add image to description string
void CECIvtrItem::AddDescImage(bool bRet, int iIndex, int iFrame)
{
#ifndef _INVENTORY_DESC_MAN //处理小工具所需代码和游戏本身代码 Added 2011-05-24
	EditBoxItemBase item(enumEIImage);
	item.SetImageIndex(iIndex);
	item.SetImageFrame(iFrame);
	AddDescText(0, bRet, (ACHAR)AUICOMMON_ITEM_CODE_START + item.Serialize() + _AL(" "));
#endif
}

//	Add extend description to description string
void CECIvtrItem::AddExtDescText()
{
	const wchar_t* szExtDesc = g_pGame->GetItemExtDesc(m_tid);
	if (!szExtDesc || !szExtDesc[0])
		return;

	m_strDesc += _AL("\\r\\r");
	m_strDesc += szExtDesc;
}

void CECIvtrItem::GetPriceText(ACHAR szText[256], INT64 nPrice)
{
	CECGameUIMan* pGameUI = g_pGame->GetGameRun()->GetUIManager()->GetInGameUIMan();
	INT64 nGold = nPrice / 10000;
	INT64 nSilver = (nPrice / 100) % 100;
	INT64 nCopper = nPrice % 100;
	ACString strText;
	
	if( nPrice == 0 )
	{
		a_sprintf(szText, _AL("%d"), 0);		
		return;
	}
	
	szText[0] = 0;
	CECStringTab* pDescTab = g_pGame->GetItemDesc();
	
	ACHAR szTemp[256];
	if( nGold )
	{
		pGameUI->Int64ToStr(nGold, strText);
		a_sprintf(szTemp, _AL("^ffffff %s %s"), strText.GetBuffer(0), pDescTab->GetWideString(ITEMDESC_PRICETAG_GOLD));
		a_strcat(szText, szTemp);
	}
	if( nSilver )
	{
		pGameUI->Int64ToStr(nSilver, strText);
		a_sprintf(szTemp, _AL("^ffffff %s %s"), strText.GetBuffer(0), pDescTab->GetWideString(ITEMDESC_PRICETAG_SILVER));
		a_strcat(szText, szTemp);
	}
	if( nCopper )
	{
		pGameUI->Int64ToStr(nCopper, strText);
		a_sprintf(szTemp, _AL("^ffffff %s %s"), strText.GetBuffer(0), pDescTab->GetWideString(ITEMDESC_PRICETAG_COPPER));
		a_strcat(szText, szTemp);
	}
}

//	Add price description
void CECIvtrItem::AddPriceDesc(int col, bool bRepair)
{
	if (!IsEquipment() && bRepair)
	{
		TrimLastReturn();
		return;
	}

	CECStringTab* pDescTab = g_pGame->GetItemDesc();
	if (m_iScaleType == SCALE_BOOTH)
	{
		if (m_iPrice == 0)
			return;

		ACHAR szPrice[256];
		GetPriceText(szPrice, m_iPrice);
		AddDescText(col, false, pDescTab->GetWideString(ITEMDESC_UNITPRICE), szPrice);
	}
	else if (m_iScaleType == SCALE_SELL && m_iCount > 1)
	{
		if (m_iPrice == 0)
			return;

		ACHAR szPrice1[256];
		GetPriceText(szPrice1, m_iPrice);
		ACHAR szPrice2[256];
		GetPriceText(szPrice2, GetScaledPrice());
		AddDescText(col, false, pDescTab->GetWideString(ITEMDESC_PRICEWITHTOTAL), szPrice1, szPrice2);
	}
	else
	{
		ACHAR szPrice[256];
		INT64 nPrice = GetScaledPrice();

		if (nPrice == 0)
			return;

		GetPriceText(szPrice, nPrice);
		AddDescText(col, false, pDescTab->GetWideString(ITEMDESC_PRICE), szPrice);
	}
}

//	Add profession requirment description
void CECIvtrItem::AddProfReqDesc(UINT64 iProfReq, UINT64 iProfReq2) // Modified 2011-07-18.
{
	const int PROF_BIT_NUM = 64; // 职业ID扩展后，所添加中间常量Added 2011-07-18.
	static int iAllProf = (1 << NUM_PROFESSION) - 1; //这种操作的真实意图是什么？在NUM_PROFESSION = 64时
													 //iAllProf永远等于-1-->补码下，就是每位都是1
	if (((iProfReq & iAllProf) == iAllProf) && (iProfReq2 & iAllProf) == iAllProf) //职业ID扩展后，需要考虑2个变量
		return;	//	All profession permit equirement

	CECStringTab* pDescTab = g_pGame->GetItemDesc();
	CECHostPlayer* pHost = g_pGame->GetGameRun()->GetHostPlayer();
	CECStringTab* pFixMsg = g_pGame->GetFixedMsgTab();

	int col = ( ((pHost->GetProfession() < PROF_BIT_NUM)&&(iProfReq & (UINT64(1) << pHost->GetProfession()))) ||
				((pHost->GetProfession() >= PROF_BIT_NUM)&&(iProfReq2 & (UINT64(1) << (pHost->GetProfession() - PROF_BIT_NUM)))) ) ? ITEMDESC_COL_WHITE : ITEMDESC_COL_RED;
	AddDescText(col, false, pDescTab->GetWideString(ITEMDESC_PROFESSIONREQ));

	for (int i=0; i < PROF_BIT_NUM; i++)
	{
		if (iProfReq & (UINT64(1) << i))
		{
			m_strDesc += _AL(" ");
			AddDescText(col, false, pFixMsg->GetWideString(FIXMSG_PROF_1+i));
		}
	}

	//对于另一个64位职业掩码，需要同样的处理
	for ( int j = 0; j < PROF_BIT_NUM; ++j )
	{
		if ( iProfReq2 & (UINT64(1) << j) )
		{
			m_strDesc += _AL(" ");
			AddDescText(col, false, pFixMsg->GetWideString(FIXMSG_PROF_65+j));
		}
	}

	m_strDesc += _AL("\\r");
}

//	Get item description text for rewarding
const wchar_t* CECIvtrItem::GetRewardDesc()
{
	CECStringTab* pDescTab = g_pGame->GetItemDesc();
	int white = ITEMDESC_COL_WHITE;

	//	Name
	m_strDesc = _AL("");
	AddDescText(white, false, pDescTab->GetWideString(ITEMDESC_NAME), GetName());
	
	//	Extend description
	AddExtDescText();

	return m_strDesc;
}

void CECIvtrItem::AddExpireTimeDesc()
{
	if( m_expire_date == 0 )
		return;

	int green = ITEMDESC_COL_GREEN;
	int yellow = ITEMDESC_COL_YELLOW;
	int gold = ITEMDESC_COL_DARKGOLD;
	int red = ITEMDESC_COL_RED;
	CECStringTab* pDescTab = g_pGame->GetItemDesc();

#ifndef _INVENTORY_DESC_MAN //用于客户端逻辑 Added 2012-06-21.
	time_t timeLeft = m_expire_date - g_pGame->GetServerGMTTime();
	if( timeLeft < 0 ) timeLeft = 0;

	if( timeLeft > 24 * 3600 )
		AddDescText(green, true, pDescTab->GetWideString(ITEMDESC_EXPIRETIME_DAY), timeLeft / (24 * 3600), (timeLeft % (24 * 3600)) / 3600);
	else if( timeLeft > 3600 )
	{
		AddDescText(yellow, true, pDescTab->GetWideString(ITEMDESC_EXPIRETIME_HOUR_MIN), timeLeft / 3600, (timeLeft % 3600) / 60);
	}
	else if( timeLeft > 60 )
	{
		AddDescText(gold, true, pDescTab->GetWideString(ITEMDESC_EXPIRETIME_MIN_SEC), timeLeft / 60, timeLeft % 60);
	}
	else
	{
		AddDescText(red, true, pDescTab->GetWideString(ITEMDESC_EXPIRETIME_SECOND), timeLeft);
	}
#else				//用于寻宝网小工具
	time_t curTime = time(NULL);
	tm tm1 = *gmtime(&curTime);
	tm tm2 = *localtime(&curTime);
	int nZone = tm2.tm_hour - tm1.tm_hour;
	time_t expTime = m_expire_date;
	expTime -= nZone * 3600;
	tm* newTime = localtime(&expTime);
//	const int MAX_BUFF_SIZE = 256;
//	ACHAR tmpBuff[MAX_BUFF_SIZE];
//	memset( tmpBuff, 0, sizeof(ACHAR) * MAX_BUFF_SIZE );
//	wcsftime( tmpBuff, MAX_BUFF_SIZE, _AL("到期时间：%F"), newTime );
	AddDescText( red, true, _AL("到期时间：%d-%d-%d %02d:%02d:%02d"), 1900 + newTime->tm_year, 1 + newTime->tm_mon, newTime->tm_mday, newTime->tm_hour, newTime->tm_min, newTime->tm_sec );
#endif
}

CECIvtrItem* CECIvtrItem::Unserialize(const ACHAR* szText)
{
	szText = wcschr(szText, L'[');
	if (szText == NULL)
		return NULL;
	szText++;
	int tid = _wtoi(szText);
	szText = wcschr(szText, L']');
	if (szText == NULL)
		return NULL;

	szText = wcschr(szText, L'[');
	if (szText == NULL)
		return NULL;

	szText++;
	int expire_date = _wtoi(szText);
	szText = wcschr(szText, L']');
	if (szText == NULL)
		return NULL;

	szText = wcschr(szText, L'[');
	if (szText == NULL)
		return NULL;

	szText++;
	int iState = _wtoi(szText);
	szText = wcschr(szText, L']');
	if (szText == NULL)
		return NULL;

	szText++;
	CECIvtrItem* pItem = CreateItem(tid, expire_date, 1);

	if (pItem == NULL)
		return NULL;

	if (!pItem->_Unserialize(szText))
	{
		delete pItem;
		return NULL;
	}

	pItem->SetStateFlag(iState);
	pItem->UpdateInfoMD5();
	return pItem;
}

ACString CECIvtrItem::Serialize() const
{
	ACHAR buf[64];	   
	if( !m_bMD5OK )
	{
		buf[0] = 0;
		return buf;
	}

	wsprintf(buf, _AL("[%d][%d][%d]"), m_tid, m_expire_date, m_iState);
	return buf + _Serialize();
}

GNET::Octets CECIvtrItem::gen_item_info()
{
	return GNET::Octets();
}

const char * CECIvtrItem::GetDropModel()
{ 
	return "Models\\Error\\Error.ecm"; 
}

void CECIvtrItem::CalculateInfoMD5(BYTE md5[16])
{
	// first we serialize the item to get the content for md5
	ACString strInfo = Serialize();
	GNET::Octets input((const wchar_t*)strInfo, strInfo.GetLength() * sizeof(ACHAR));
	GNET::Octets output = GNET::MD5Hash::Digest(input);
	memcpy(md5, output.begin(), 16);
}

void CECIvtrItem::UpdateInfoMD5()
{
	CalculateInfoMD5(m_infoMD5);
	m_bMD5OK = true;
}

bool CECIvtrItem::VerifyInfoMD5()
{
#ifndef _DEBUG
	BYTE md5[16];

	CalculateInfoMD5(md5);
	if( memcmp(m_infoMD5, md5, 16) != 0 )
	{
		m_bMD5OK = false;
		return false;
	}
#endif

	m_bMD5OK = true;
	return true;
}

const wchar_t* CECIvtrItem::GetDesc(int iDescType, CECInventory* pInventory)
{
	if( !VerifyInfoMD5() )
	{
		m_strDesc = _AL("");
		CECStringTab* pDescTab = g_pGame->GetItemDesc();
		AddDescText(ITEMDESC_COL_RED, true, pDescTab->GetWideString(ITEMDESC_FATALERROR));
		return m_strDesc;
	}

	m_pDescIvtr = pInventory;

	if (iDescType == DESC_BOOTHBUY)
		return GetBoothBuyDesc();
	else if (iDescType == DESC_REWARD)
		return GetRewardDesc();
	else
		return GetNormalDesc(iDescType == DESC_REPAIR);
}

//	Decide equipment name color
int CECIvtrItem::DecideNameCol()
{
	int iIndex = g_pGame->GetItemNameColorIdx(m_tid);
	return iIndex + ITEMDESC_COL_WHITE;
}

//	Get item color
AWString CECIvtrItem::GetColor()
{
	AWString strColName;
	int iCol = DecideNameCol();
	CECStringTab* pDescTab = g_pGame->GetItemDesc();
	const wchar_t* szCol = pDescTab->GetWideString(iCol);

	if (szCol)
		strColName = szCol;

	return strColName;
}

void CECIvtrItem::AddStateDesc(CECStringTab* pDescTab)
{
	if (m_iState & ITEM_STATE_BINDED)
		AddDescText(ITEMDESC_COL_DARKGOLD, true, pDescTab->GetWideString(ITEMDESC_EQUIP_BINDED));
	else if (m_iState & ITEM_STATE_BINDAFTEREQUIP)
		AddDescText(ITEMDESC_COL_DARKGOLD, true, 
		pDescTab->GetWideString(m_iCID==ICID_EQUIP ? ITEMDESC_EQUIP_BINDAFTERUSE : ITEMDESC_ITEM_BINDAFTERUSE));

	if (m_iState & ITEM_STATE_LOCKED)
		AddDescText(ITEMDESC_COL_DARKGOLD, true, pDescTab->GetWideString(ITEMDESC_LOCKED));
	
	if (m_iProcType & PROC_DROPWHENDIE)
		AddDescText(ITEMDESC_COL_DARKGOLD, true, pDescTab->GetWideString(ITEMDESC_NO_DROP_WHEN_DIE));

	if (m_iProcType & PROC_SELLABLE)
		AddDescText(ITEMDESC_COL_DARKGOLD, true, pDescTab->GetWideString(ITEMDESC_NOT_SELLABLE));
}

// 是否可以移到仓库中
// 目前的VIP卡不能移到仓库中
bool CECIvtrItem::CanMoveToTrash()
{
	if(m_iCID == ICID_VIP_CARD)
		return false;

	return (m_iProcType & PROC_MOVE_STORAGE)? false:true;
}

bool CECIvtrItem::CheckCrossUseCondition() const
{
	CECHostPlayer* pHost = g_pGame->GetGameRun()->GetHostPlayer();
	if (!pHost)
		return false;
	if (pHost->GetSrcServerID()!= 0 && IsCrossForbid())
		return false;
	if (pHost->GetSrcServerID()== 0 && IsCrossOnly())
		return false;
	return true;
}
///////////////////////////////////////////////////////////////////////////
//	
//	Implement CECIvtrUnknown
//	
///////////////////////////////////////////////////////////////////////////

CECIvtrUnknown::CECIvtrUnknown(int tid) : CECIvtrItem(tid, 0)
{
	m_iCID = ICID_ERRORITEM;

	m_bNeedUpdate	= false;
	m_bUpdating		= false;
}

CECIvtrUnknown::CECIvtrUnknown(const CECIvtrUnknown& s) : CECIvtrItem(s)
{
}

//	Get item icon file name
const char* CECIvtrUnknown::GetIconFile()
{
	return "Unknown.dds";
}

//	Get item name
const wchar_t* CECIvtrUnknown::GetName()
{
	CECStringTab* pDescTab = g_pGame->GetItemDesc();
	return pDescTab->GetWideString(ITEMDESC_ERRORITEM);
}

//	Get item description text
const wchar_t* CECIvtrUnknown::GetNormalDesc(bool bRepair)
{
	m_strDesc = _AL("");
	CECStringTab* pDescTab = g_pGame->GetItemDesc();
	AddStateDesc(pDescTab);
	AddDescText(ITEMDESC_COL_WHITE, false, pDescTab->GetWideString(ITEMDESC_ERRORITEM));
	AddDescText(ITEMDESC_COL_WHITE, false, _AL("(%d)"), m_tid);
	return m_strDesc;
}
