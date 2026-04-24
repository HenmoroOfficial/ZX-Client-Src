/*
 * FILE: EC_HTC2SRawCmds.cpp
 *
 * DESCRIPTION: home town command types
 *
 * CREATED BY: lihengju, 2009/8/19
 *
 * HISTORY: 
 *
 * Copyright (c) 2004 Archosaur Studio, All Rights Reserved.
 */

#include "EC_Global.h"
#include "EC_GameSession.h"
#include "EC_HTPDataType.h"
#include "EC_Game.h"

///////////////////////////////////////////////////////////////////////////
//	
//	Define and Macro
//	
///////////////////////////////////////////////////////////////////////////

//	Fixed length home town command structure
#define FIXLEN_HOMETOWNCMD(byCmd, byComp, content_type) \
	using namespace HT_C2S; \
	using namespace HOMETOWN; \
	using namespace HT_C2S::CMD; \
	struct \
	{ \
		hometown_c2s_cmd_header hthd; \
		char content[sizeof (content_type)]; \
	} htcmd; \
	htcmd.hthd.cmd_type = byCmd; \
	htcmd.hthd.compo_id = byComp;	\
    content_type* pHTCmd = (content_type*)(&htcmd.content); \
	int iSize = sizeof (hometown_c2s_cmd_header) + sizeof (content_type);

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
using namespace HT_C2S;
using namespace HT_C2S::CMD;

static inline void _SendHTNakeCommand(int iCmd, int compo)
{
	using namespace HT_C2S;
	using namespace HT_C2S::CMD;

	int iSize = sizeof (hometown_c2s_cmd_header);
	BYTE* pBuf = new BYTE [iSize];
	if (!pBuf)
		return;
	
	((hometown_c2s_cmd_header*)pBuf)->cmd_type = iCmd;
	((hometown_c2s_cmd_header*)pBuf)->compo_id = compo;
	g_pGame->GetGameSession()->SendHomeTownData(pBuf, iSize);

	delete [] pBuf;
}

///////////////////////////////////////////////////////////////////////////
//	
//	Implement
//	
///////////////////////////////////////////////////////////////////////////

//	Implementation of home town commands

void htc2s_SendCmdGetCompoList(int roleid)
{
	FIXLEN_HOMETOWNCMD(COMPO_LIST, FARM_COMPONENT, compo_list)
	pHTCmd->roleid = roleid;
	g_pGame->GetGameSession()->SendHomeTownData(&htcmd, iSize);	
}
void htc2s_SendCmdAddCompo(int id)
{
	_SendHTNakeCommand(HT_C2S::COMPO_ADD, id);
}
void htc2s_SendCmdExgMoney(BYTE type, __int64 money)
{
	FIXLEN_HOMETOWNCMD(EXCHG_MONEY, FARM_COMPONENT, exchg_money)
	pHTCmd->type  = type;
	pHTCmd->money = money;
	g_pGame->GetGameSession()->SendHomeTownData(&htcmd, iSize);	
}
void htc2s_SendCmdLevelUp()
{
	_SendHTNakeCommand(HT_C2S::LEVEL_UP, HT_C2S::FARM_COMPONENT);
}
void htc2s_SendCmdBuyItem(int itemid, int count)
{
	FIXLEN_HOMETOWNCMD(BUY_ITEM, FARM_COMPONENT, buy_item)
	pHTCmd->itemid = itemid;
	pHTCmd->count	= count;
	g_pGame->GetGameSession()->SendHomeTownData(&htcmd, iSize);	
}
void htc2s_SendCmdUseItem(int pos, int itemid, int target_type, int roleid, int fieldid)
{
	FIXLEN_HOMETOWNCMD(USE_ITEM, FARM_COMPONENT, use_item)
	pHTCmd->pos = pos;
	pHTCmd->itemid = itemid;
	pHTCmd->target_type = target_type;
	pHTCmd->roleid = roleid;
	pHTCmd->field_id = fieldid;
	g_pGame->GetGameSession()->SendHomeTownData(&htcmd, iSize);	
}
void htc2s_SendCmdSellItem(int pos, int itemid, int count)
{
	FIXLEN_HOMETOWNCMD(SELL_ITEM, FARM_COMPONENT, sell_item)
	pHTCmd->pos = pos;
	pHTCmd->itemid = itemid;
	pHTCmd->count = count;
	g_pGame->GetGameSession()->SendHomeTownData(&htcmd, iSize);	
}
void htc2s_SendCmdGetPackage()
{
	_SendHTNakeCommand(HT_C2S::PACKAGE_LIST, HT_C2S::FARM_COMPONENT);
}
void htc2s_SendCmdViewFarm(int roleid)
{
	FIXLEN_HOMETOWNCMD(FARM_VIEW, FARM_COMPONENT, farm_view);
	pHTCmd->roleid = roleid;
	g_pGame->GetGameSession()->SendHomeTownData(&htcmd, iSize);	
}
void htc2s_SendCmdViewFarmField(int roleid, int fieldid)
{
	FIXLEN_HOMETOWNCMD(FARM_VIEW_FIELD, FARM_COMPONENT, farm_view_field);
	pHTCmd->roleid = roleid;
	pHTCmd->field_id = fieldid;
	g_pGame->GetGameSession()->SendHomeTownData(&htcmd, iSize);	
}
void htc2s_SendCmdPlowField(int roleid, int fieldid, int emotion)
{
	FIXLEN_HOMETOWNCMD(FARM_PLOW_FIELD, FARM_COMPONENT, farm_plow_field);
	pHTCmd->roleid = roleid;
	pHTCmd->field_id = fieldid;
	pHTCmd->emotion = emotion;
	g_pGame->GetGameSession()->SendHomeTownData(&htcmd, iSize);	
}
void htc2s_SendCmdWaterField(int roleid, int fieldid, int emotion)
{
	FIXLEN_HOMETOWNCMD(FARM_WATER_FIELD, FARM_COMPONENT, farm_water_field);
	pHTCmd->roleid = roleid;
	pHTCmd->field_id = fieldid;
	pHTCmd->emotion = emotion;
	g_pGame->GetGameSession()->SendHomeTownData(&htcmd, iSize);	
}
void htc2s_SendCmdSowField(int roleid, int fieldid, int seedid)
{
	FIXLEN_HOMETOWNCMD(FARM_SOW_FIELD, FARM_COMPONENT, farm_sow_field);
	pHTCmd->roleid = roleid;
	pHTCmd->field_id = fieldid;
	pHTCmd->seed_id  = seedid;
	g_pGame->GetGameSession()->SendHomeTownData(&htcmd, iSize);	
}
void htc2s_SendCmdPestField(int roleid, int fieldid, int emotion)
{
	FIXLEN_HOMETOWNCMD(FARM_PEST_FIELD, FARM_COMPONENT, farm_pest_field);
	pHTCmd->roleid = roleid;
	pHTCmd->field_id = fieldid;
	pHTCmd->emotion = emotion;
	g_pGame->GetGameSession()->SendHomeTownData(&htcmd, iSize);	
}
void htc2s_SendCmdWeedField(int roleid, int fieldid, int emotion)
{
	FIXLEN_HOMETOWNCMD(FARM_WEED_FIELD, FARM_COMPONENT, farm_weed_field);
	pHTCmd->roleid = roleid;
	pHTCmd->field_id = fieldid;
	pHTCmd->emotion = emotion;
	g_pGame->GetGameSession()->SendHomeTownData(&htcmd, iSize);	
}
void htc2s_SendCmdHarvestField(int roleid, int fieldid, int emotion)
{
	FIXLEN_HOMETOWNCMD(FARM_HARVEST_FIELD, FARM_COMPONENT, farm_harvest_field);
	pHTCmd->roleid = roleid;
	pHTCmd->field_id = fieldid;
	g_pGame->GetGameSession()->SendHomeTownData(&htcmd, iSize);	
}
void htc2s_SendCmdStealField(int roleid, int fieldid, int emotion)
{
	FIXLEN_HOMETOWNCMD(FARM_STEAL_FIELD, FARM_COMPONENT, farm_steal_field);
	pHTCmd->roleid = roleid;
	pHTCmd->field_id = fieldid;
	pHTCmd->emotion = emotion;
	g_pGame->GetGameSession()->SendHomeTownData(&htcmd, iSize);	
}
void htc2s_SendCmdListLoveFields()
{
	_SendHTNakeCommand(HT_C2S::FARM_LIST_LOVE_FIELD, HT_C2S::FARM_COMPONENT);
}
void htc2s_SendCmdListMatureFields()
{
	_SendHTNakeCommand(HT_C2S::FARM_LIST_MATURE_FIELD,HT_C2S::FARM_COMPONENT);
}
