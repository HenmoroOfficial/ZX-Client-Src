/*
 * FILE: EC_HTCHMisc.cpp
 *
 * DESCRIPTION: 
 *
 * CREATED BY: lihengju, 2009/8/19
 *
 * HISTORY: 
 *
 * Copyright (c) 2004 Archosaur Studio, All Rights Reserved.
 */

#include "EC_Global.h"
#include "EC_HomeTownPrtcHandler.h"
#include "EC_HTPDataType.h"
#include "EC_Game.h"
//#include "EC_HomeTown.h"
#include "EC_GameRun.h"
#include "EC_UIManager.h"
#include "EC_GameUIMan.h"
#include "EC_HostPlayer.h"
#include "EC_Inventory.h"
#include "EC_IvtrItem.h"
#include "EC_HTC2SRawCmds.h"

#include "gnoctets.h"
#include "hometowndatasend.hpp"

#include "DlgModule.h"
#include "DlgGardenInventory.h"
#include "DlgGardenMan.h"
#include "DlgGardenMoney.h"
#include "DlgGardenQShop.h"

///////////////////////////////////////////////////////////////////////////
//	
//	Define and Macro
//	
///////////////////////////////////////////////////////////////////////////

#define new A_DEBUG_NEW

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
//	Implement CECHomeTownPrtcHandler
//	
///////////////////////////////////////////////////////////////////////////

#define ASSERT_SIZE(EXP) if (!(EXP)) return 0
#define ASSERT_SIZE_MIN() ASSERT_SIZE(iDataSize>=size_min)

static int Unmarshal(const char *pData, int iDataSize, struct HOMETOWN::farm_info &farm)
{
	using namespace HOMETOWN;
	int size_min = sizeof(struct farm_player)+2*sizeof(int)+3*sizeof(size_t);
	ASSERT_SIZE_MIN();
	farm.state = *(int *)pData;
	pData += sizeof(int);
	farm.protect_period += *(int *)pData;
	pData += sizeof(int);
	farm.owner = *(struct farm_player *)pData;
	pData += sizeof(struct farm_player);

	size_t count = *(size_t *)pData;
	pData += sizeof(size_t);
	if (count > 0)
	{
		size_min += sizeof(struct field_info)*count;
		ASSERT_SIZE_MIN();

		for (size_t i = 0; i < count; ++i)
		{
			farm.fields.push_back(*(struct field_info *)pData);
			pData += sizeof(struct field_info);
		}
	}
	count = *(size_t *)pData;
	pData += sizeof(size_t);
	if (count > 0)
	{
		for (size_t i = 0; i < count; ++i)
		{
			farm.farm_dynamic.push_back(Octets());
			Octets &oct = farm.farm_dynamic.back();
			size_t oct_len = *(size_t *)pData;
			size_min += sizeof(size_t)+oct_len;
			ASSERT_SIZE_MIN();
			pData += sizeof(size_t);
			oct.insert(oct.end(), pData, oct_len);
			pData += oct_len;
		}
	}
	count = *(size_t *)pData;
	pData += sizeof(size_t);
	if (count > 0)
	{
		for (size_t i = 0; i < count; ++i)
		{
			farm.action_dynamic.push_back(Octets());
			Octets &oct = farm.action_dynamic.back();
			size_t oct_len = *(size_t *)pData;
			size_min += sizeof(size_t)+oct_len;
			ASSERT_SIZE_MIN();
			pData += sizeof(size_t);
			oct.insert(oct.end(), pData, oct_len);
			pData += oct_len;
		}
	}
	ASSERT_SIZE(iDataSize==size_min);
	return 1;
}

static int Unmarshal(const char * pData, int iDataSize, struct HOMETOWN::pocket_info &pocket)
{
	using namespace HOMETOWN;
	int size_min = sizeof(int)+sizeof(size_t);
	ASSERT_SIZE_MIN();
	pocket.capacity = *(int *)pData;
	pData += sizeof(int);
	size_t count = *(size_t *)pData;
	pData += sizeof(size_t);
	if (count > 0)
	{
		for (size_t i = 0; i < count; ++i)
		{
			pocket.items.push_back(item_data());
			item_data &data = pocket.items.back();
			data.id = *(unsigned int *)pData;
			pData += sizeof(unsigned int);
			data.pos = *(int *)pData;
			pData += sizeof(int);
			data.count = *(int *)pData;
			pData += sizeof(int);
			data.max_count = *(int *)pData;
			pData += sizeof(int);
			size_t oct_len = *(size_t *)pData;
			pData += sizeof(size_t);
			size_min += sizeof(unsigned int)+3*sizeof(int)+sizeof(size_t)+oct_len;
			ASSERT_SIZE_MIN();
			data.content.insert(data.content.end(), pData, oct_len);
			pData += oct_len;
		}
	}
	ASSERT_SIZE(iDataSize==size_min);
	return 1;
}

int CECHomeTownPrtcHandler:: OnHTCmd_GetCompoList(const HTCMDINFO& htci)
{
	using namespace HT_S2C;
	using namespace HT_S2C::CMD;
	compo_list* pCmd = (compo_list*)htci.pData;
	CECGameUIMan *pGameUI = g_pGame->GetGameRun()->GetUIManager()->GetInGameUIMan();
	((CDlgModule*)pGameUI->GetDialog("Win_Option"))->OnPrtc_ModuleList(pCmd);

	return 1;
}
int CECHomeTownPrtcHandler:: OnHTCmd_AddCompo(const HTCMDINFO& htci)
{
	using namespace HT_S2C;
	using namespace HT_S2C::CMD;
	//compo_add* pCmd = (compo_add*)htci.pData;
	struct farm_info farm;
	
	char* pCmd = (char *)htci.pData;
	if (!Unmarshal(pCmd, htci.iDataSize, farm)) return 0;
	CECGameUIMan *pGameUI = g_pGame->GetGameRun()->GetUIManager()->GetInGameUIMan();

	return 1;
}
int CECHomeTownPrtcHandler:: OnHTCmd_ExgMoney(const HTCMDINFO& htci)
{
	using namespace HT_S2C;
	using namespace HT_S2C::CMD;
	exchg_money* pCmd = (exchg_money*)htci.pData;
	CECGameUIMan *pGameUI = g_pGame->GetGameRun()->GetUIManager()->GetInGameUIMan();
	ACString strText;
	if (pCmd->type)
	{
		strText.Format(pGameUI->GetStringFromTable(9107), (int)(pCmd->hometown_money - CDlgModule::GetModuleMoney()));
	}
	else
	{
		strText.Format(pGameUI->GetStringFromTable(9108), (int)(pCmd->compo_money - CDlgGardenMan::GetGardenMoney()));
	}
	pGameUI->AddChatMessage(strText, GP_CHAT_LOCAL);
	CDlgModule::SetModuleMoney(pCmd->hometown_money);
	CDlgGardenMan::SetGardenMoney(pCmd->compo_money);
	((CDlgGardenMan*)pGameUI->GetDialog("Win_Garden"))->Show(true);

	return 1;
}
int CECHomeTownPrtcHandler:: OnHTCmd_LevelUp(const HTCMDINFO& htci)
{
	using namespace HT_S2C;
	using namespace HT_S2C::CMD;
	struct farm_info farm;
	char* pCmd = (char *)htci.pData;
	if (!Unmarshal(pCmd, htci.iDataSize, farm)) return 0;
	CECGameUIMan *pGameUI = g_pGame->GetGameRun()->GetUIManager()->GetInGameUIMan();
	((CDlgGardenMan*)pGameUI->GetDialog("Win_Garden"))->OnPrtc_LevelUp((char *)&farm);

	return 1;
}
int CECHomeTownPrtcHandler:: OnHTCmd_BuyItem(const HTCMDINFO& htci)
{
	using namespace HT_S2C;
	using namespace HT_S2C::CMD;
	buy_item* pCmd = (buy_item*)htci.pData;
	CECGameUIMan *pGameUI = g_pGame->GetGameRun()->GetUIManager()->GetInGameUIMan();
	CECInventory* pInventory = g_pGame->GetGameRun()->GetHostPlayer()->GetHTFarmPack();
	int iLastSlot, iSlotNum;
	if (!pInventory->MergeHomeTownItem(pCmd->itemid, 0, pCmd->count, &iLastSlot, &iSlotNum))
	{
		ASSERT(0);
		return 0;
	}
	if (CDlgGardenMan::GetOwnerId() == g_pGame->GetGameRun()->GetHostPlayer()->GetCharacterID())
		CDlgGardenMan::SetGardenCharm(CDlgGardenMan::GetGardenCharm()-pCmd->cost1);
	CDlgGardenMan::SetGardenMoney(CDlgGardenMan::GetGardenMoney()-pCmd->cost2);

	return 1;
}
int CECHomeTownPrtcHandler:: OnHTCmd_UseItem(const HTCMDINFO& htci)
{
	using namespace HT_S2C;
	using namespace HT_S2C::CMD;
	use_item* pCmd = (use_item*)htci.pData;

	CECInventory* pPack = g_pGame->GetGameRun()->GetHostPlayer()->GetHTFarmPack();
	if (!pPack) return 0;

	CECIvtrItem* pItem = (CECIvtrItem*)pPack->GetItem(pCmd->pos);
	if (!pItem || pItem->GetTemplateID() != pCmd->itemid)
	{
		ASSERT(0);
		return 0;
	}
	pItem->Use();
	pPack->RemoveItem(pCmd->pos,1);
	if (pCmd->target_type==FARM_TARGET_FARM)
		htc2s_SendCmdViewFarm(pCmd->roleid);
	else if (pCmd->target_type==FARM_TARGET_FIELD)
		htc2s_SendCmdViewFarmField(pCmd->roleid, pCmd->field_id);
	return 1;
}
int CECHomeTownPrtcHandler:: OnHTCmd_SellItem(const HTCMDINFO& htci)
{
	using namespace HT_S2C;
	using namespace HT_S2C::CMD;
	sell_item* pCmd = (sell_item*)htci.pData;

	CECInventory* pPack = g_pGame->GetGameRun()->GetHostPlayer()->GetHTFarmPack();
	if (!pPack) return 0;

	CECIvtrItem* pItem = (CECIvtrItem*)pPack->GetItem(pCmd->pos);
	if (!pItem || pItem->GetTemplateID() != pCmd->itemid)
	{
		ASSERT(0);
		return 0;
	}
	if (pCmd->count)
		pPack->RemoveItem(pCmd->pos,pCmd->count);
	CDlgGardenMan::SetGardenMoney(CDlgGardenMan::GetGardenMoney() + pCmd->earning);
	return 1;
}
int CECHomeTownPrtcHandler:: OnHTCmd_GetPackage(const HTCMDINFO& htci)
{
	using namespace HT_S2C;
	using namespace HT_S2C::CMD;
	struct pocket_info pocket;
	char* pCmd = (char *)htci.pData;
	if (!Unmarshal(pCmd, htci.iDataSize, pocket)) return 0;
	CECInventory* pHTInventory = g_pGame->GetGameRun()->GetHostPlayer()->GetHTFarmPack();
	pHTInventory->ResetItems(pocket);
	return 1;
}
int CECHomeTownPrtcHandler:: OnHTCmd_ViewFarm(const HTCMDINFO& htci)
{
	using namespace HT_S2C;
	using namespace HT_S2C::CMD;

	ASSERT_SIZE(htci.iDataSize>=sizeof(int));
	struct farm_view data;
	char* pCmd = (char *)htci.pData;
	data.roleid = *(int *)pCmd;
	pCmd += sizeof(int);

	if (!Unmarshal(pCmd, htci.iDataSize-sizeof(int), data.farm))
		return 0;

	CECGameUIMan *pGameUI = g_pGame->GetGameRun()->GetUIManager()->GetInGameUIMan();
	((CDlgGardenMan*)pGameUI->GetDialog("Win_Garden"))->OnPrtc_ViewFarm(&data);

	return 1;
}
int CECHomeTownPrtcHandler:: OnHTCmd_ViewFarmField(const HTCMDINFO& htci)
{
	using namespace HT_S2C;
	using namespace HT_S2C::CMD;
	
	ASSERT_SIZE(htci.iDataSize>=sizeof(int));
	struct farm_view_field *data = (farm_view_field*)htci.pData;
	
	CECGameUIMan *pGameUI = g_pGame->GetGameRun()->GetUIManager()->GetInGameUIMan();
	((CDlgGardenMan*)pGameUI->GetDialog("Win_Garden"))->OnPrtc_ViewFarmField(data);
	
	return 1;
}
int CECHomeTownPrtcHandler:: OnHTCmd_SowField(const HTCMDINFO& htci)
{
	using namespace HT_S2C;
	using namespace HT_S2C::CMD;
	farm_sow_field* pCmd = (farm_sow_field*)htci.pData;
	CECGameUIMan *pGameUI = g_pGame->GetGameRun()->GetUIManager()->GetInGameUIMan();
	((CDlgGardenMan*)pGameUI->GetDialog("Win_Garden"))->OnPrtc_SowField(pCmd);

	return 1;
}
int CECHomeTownPrtcHandler:: OnHTCmd_PestField(const HTCMDINFO& htci)
{
	using namespace HT_S2C;
	using namespace HT_S2C::CMD;
	farm_pest_field* pCmd = (farm_pest_field*)htci.pData;
	CECGameUIMan *pGameUI = g_pGame->GetGameRun()->GetUIManager()->GetInGameUIMan();
	((CDlgGardenMan*)pGameUI->GetDialog("Win_Garden"))->OnPrtc_PestField(pCmd);

	return 1;
}

int CECHomeTownPrtcHandler:: OnHTCmd_WaterField(const HTCMDINFO& htci)
{
	using namespace HT_S2C;
	using namespace HT_S2C::CMD;
	farm_water_field* pCmd = (farm_water_field*)htci.pData;
	CECGameUIMan *pGameUI = g_pGame->GetGameRun()->GetUIManager()->GetInGameUIMan();
	((CDlgGardenMan*)pGameUI->GetDialog("Win_Garden"))->OnPrtc_WaterField(pCmd);

	return 1;
}
int CECHomeTownPrtcHandler:: OnHTCmd_PlowField(const HTCMDINFO& htci)
{
	using namespace HT_S2C;
	using namespace HT_S2C::CMD;
	level_up* pCmd = (level_up*)htci.pData;
	CECGameUIMan *pGameUI = g_pGame->GetGameRun()->GetUIManager()->GetInGameUIMan();
	((CDlgGardenMan*)pGameUI->GetDialog("Win_Garden"))->OnPrtc_PlowField(pCmd);

	return 1;
}
int CECHomeTownPrtcHandler:: OnHTCmd_WeedField(const HTCMDINFO& htci)
{
	using namespace HT_S2C;
	using namespace HT_S2C::CMD;
	farm_weed_field* pCmd = (farm_weed_field*)htci.pData;
	CECGameUIMan *pGameUI = g_pGame->GetGameRun()->GetUIManager()->GetInGameUIMan();
	((CDlgGardenMan*)pGameUI->GetDialog("Win_Garden"))->OnPrtc_WeedField(pCmd);

	return 1;
}
int CECHomeTownPrtcHandler:: OnHTCmd_HarvestField(const HTCMDINFO& htci)
{
	using namespace HT_S2C;
	using namespace HT_S2C::CMD;
	farm_harvest_field* pCmd = (farm_harvest_field*)htci.pData;
	CECGameUIMan *pGameUI = g_pGame->GetGameRun()->GetUIManager()->GetInGameUIMan();
	((CDlgGardenMan*)pGameUI->GetDialog("Win_Garden"))->OnPrtc_HarvestField(pCmd);

	return 1;
}
int CECHomeTownPrtcHandler:: OnHTCmd_StealField(const HTCMDINFO& htci)
{
	using namespace HT_S2C;
	using namespace HT_S2C::CMD;
	farm_steal_field* pCmd = (farm_steal_field*)htci.pData;
	CECGameUIMan *pGameUI = g_pGame->GetGameRun()->GetUIManager()->GetInGameUIMan();
	((CDlgGardenMan*)pGameUI->GetDialog("Win_Garden"))->OnPrtc_StealField(pCmd);

	return 1;
}
int CECHomeTownPrtcHandler:: OnHTCmd_ListLoveFields(const HTCMDINFO& htci)
{
	using namespace HT_S2C;
	using namespace HT_S2C::CMD;
	farm_list_love_field* pCmd = (farm_list_love_field*)htci.pData;
	CECGameUIMan *pGameUI = g_pGame->GetGameRun()->GetUIManager()->GetInGameUIMan();
	((CDlgGardenMan*)pGameUI->GetDialog("Win_Garden"))->OnPrtc_ListLoveFields(pCmd);

	return 1;
}
int CECHomeTownPrtcHandler:: OnHTCmd_ListMatureFields(const HTCMDINFO& htci)
{
	using namespace HT_S2C;
	using namespace HT_S2C::CMD;
	farm_list_mature_field* pCmd = (farm_list_mature_field*)htci.pData;
	CECGameUIMan *pGameUI = g_pGame->GetGameRun()->GetUIManager()->GetInGameUIMan();
	((CDlgGardenMan*)pGameUI->GetDialog("Win_Garden"))->OnPrtc_ListMatureFields(pCmd);

	return 1;
}