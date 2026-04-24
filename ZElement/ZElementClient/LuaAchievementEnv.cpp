/********************************************************************
  created:	   4/7/2009  
  author:      liudong  
  description:  lua api env in achievement.
  Copyright (c) , All Rights Reserved.
*********************************************************************/

#include "LuaApiEnv.h"

#include "EC_Global.h"
#include "EC_Game.h"
#include "EC_GameRun.h"
#include "EC_World.h"
#include "EC_IvtrItem.h"
#include "EC_ManPlayer.h"
#include "EC_HostPlayer.h"
#include "EC_GPDataType.h"
#include "EC_RTDebug.h"
#include "EC_Achievement.h"
#include "EC_Skill.h"
#include "EC_FixedMsg.h"

#include "ExpTypes.h"
#include "elementdataman.h"
#include "TaskTempl.h"
#include "TaskTemplMan.h"

//////////////////////////////////////////////////////////////////////////
//local functions


//////////////////////////////////////////////////////////////////////////
//CLuaApiEnv functions.
bool CLuaApiEnv::GetMonsterName(AWString& name, const unsigned int tid)
{
	const wchar_t* pName = NULL;
	DATA_TYPE DataType;
	const void* pData = g_pGame->GetElementDataMan()->get_data_ptr(tid, ID_SPACE_ESSENCE, DataType);
	if (pData)
	{
		if (DT_MONSTER_ESSENCE==DataType)
		{
			const MONSTER_ESSENCE* pEssence = (const MONSTER_ESSENCE*)pData;
			pName = pEssence->name;
		}else if (DT_NPC_ESSENCE==DataType)
		{
			const NPC_ESSENCE* pEssence = (const NPC_ESSENCE*)pData;
			pName = pEssence->name;
		}
	}
	if (pName && pName[0])
	{
		name = pName;
	}else
	{
		name = _AL("");
	}
	return name.GetLength()>0;
}

bool CLuaApiEnv::GetItemName(AWString& name, const unsigned int tid)
{
	const wchar_t* pName = NULL;
	CECIvtrItem* pItem = CECIvtrItem::CreateItem(tid,0,1,ID_SPACE_ESSENCE);
	if (pItem && CECIvtrItem::ICID_ERRORITEM!=pItem->GetClassID())
	{
		pName = pItem->GetName();
	}
	if(pName && pName[0])
	{
		name = pName;
	}else
	{
		name = _AL("");
	}
	if (pItem)
	{
		delete pItem;
	}
	return name.GetLength()>0;
}

bool CLuaApiEnv::GetTaskName(AWString& name, const unsigned int id)
{
	const wchar_t* pName = NULL;
	ATaskTemplMan *pMan = g_pGame->GetTaskTemplateMan();
	if (pMan)
	{
		ATaskTempl* pTask = pMan->GetTaskTemplByID(id);
		if (pTask)
		{
			pName = (const ACHAR*)pTask->m_szName;
		}
	}
	if(pName && pName[0])
	{
		name = pName;
	}else
	{
		name = _AL("");
	}
	return name.GetLength()>0;
}

//bool CLuaApiEnv::GetBattleName(AWString& name, const unsigned int tid)
//{
//	const wchar_t* pName = NULL;
//	DATA_TYPE DataType;
//	const void* pData = g_pGame->GetElementDataMan()->get_data_ptr(tid, ID_SPACE_CONFIG, DataType);
//	if (pData && DataType == DT_INSTANCE_CONFIG)
//	{
//		pName = ((const INSTANCE_CONFIG*)pData)->name;
//	}
//	if(pName && pName[0])
//	{
//		name = pName;
//	}else
//	{
//		name = _AL("");
//	}
//	return name.GetLength()>0;
// }

//bool CLuaApiEnv::GetReputationName(AWString& name, const unsigned int id)
//{
//	AWString key;
//	key.Format(_AL("SECT_REPU%d"),id);
//	return CLuaResource::GetString(name,key);
// }
bool CLuaApiEnv::GetReputationName(AWString& name, const unsigned int id)
{
	CECStringTab* pStrTab = g_pGame->GetFixedMsgTab();
	name = pStrTab->GetWideString(FIXMSG_REGION_REPU_1 + id);
	return name.GetLength() > 0;
}

bool CLuaApiEnv::GetSkillName(AWString& name, const unsigned int id)
{
	CECSkill* pSkill = GetHostPlayer()->GetSkill(id);
	const wchar_t* pName = pSkill->GetName();
	if(pName && pName[0])
	{
		name = pName;
	}else
	{
		name = _AL("");
	}
	return name.GetLength()>0;
}

bool CLuaApiEnv::GetProduceName(AWString& name, const unsigned int tid, const unsigned int index)
{
	DATA_TYPE DataType;
	const void* pData = g_pGame->GetElementDataMan()->get_data_ptr(tid, ID_SPACE_RECIPE, DataType);
	if (pData && DataType == DT_RECIPE_ESSENCE)
	{
		const RECIPE_ESSENCE* pEssence = (const RECIPE_ESSENCE*)pData;
		const int NUM = sizeof(pEssence->products)/sizeof(pEssence->products[0]);
		if (index<NUM)
		{
			GetItemName(name,pEssence->products[index].id_to_make);
			return true;
		}
	}
	name = _AL("");
	return false;
}
//
//bool CLuaApiEnv::GetMoneyBagTypeName(AWString& name, const unsigned int id)
//{
//	AWString key;
//	key.Format(_AL("MONEY_BAG_TYPE_%d"),id+1);
//	return CLuaResource::GetString(name,key);
// }

//bool CLuaApiEnv::GetProfName(AWString& name, const unsigned int id)
//{
//	AWString key;
//	key.Format(_AL("PROF_%d"),id+1);
//	return CLuaResource::GetString(name,key);
// }

bool CLuaApiEnv::GetProfName(AWString& name, const unsigned int id)
{
	CECStringTab* pStrTab = g_pGame->GetFixedMsgTab();
	name = pStrTab->GetWideString(FIXMSG_PROF_1 + id);
	return name.GetLength() > 0;
}

bool CLuaApiEnv::GetAchievementName(AWString& name, const unsigned int id)
{
	name = _AL("");
	CECAchievementMan* pMan = g_pGame->GetGameRun()->GetHostPlayer()->GetAchievementMan();
	if (pMan)
	{
		sAchievement* pAhievement = pMan->GetAchievement(id);
		if (pAhievement)
		{
			name = pAhievement->name;
		}
	}
	return name.GetLength()>0;
}
