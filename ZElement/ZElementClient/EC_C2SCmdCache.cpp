/*
 * FILE: EC_C2SCmdCache.cpp
 *
 * DESCRIPTION: 
 *
 * CREATED BY: Duyuxin, 2005/1/20
 *
 * HISTORY: 
 *
 * Copyright (c) 2005 Archosaur Studio, All Rights Reserved.
 */

#include "EC_Global.h"
#include "EC_C2SCmdCache.h"
#include "EC_GameSession.h"
#include "EC_Game.h"
#include "EC_GameRun.h"
#include "EC_World.h"
#include "EC_HostPlayer.h"
#include "EC_IvtrItem.h"
#include "EC_Inventory.h"
#include "EC_GPDataType.h"
#include "defence/stackchecker.h"

//#include <ACounter.h>

#include "AAssist.h"

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
//	Implement CECC2SCmdCache
//	
///////////////////////////////////////////////////////////////////////////

CECC2SCmdCache::CECC2SCmdCache()
{
	m_idLastPickUpItem	= 0;
	m_idLastSelTarget	= 0;
	m_bGetExpProps		= false;
	m_bEnterSanctuary	= false;

	CECC2SCmdCache::InitCounters();
}

CECC2SCmdCache::~CECC2SCmdCache()
{
	//	Release all counters
	CounterTable::iterator it = m_CounterMap.begin();
	for (; it != m_CounterMap.end(); ++it)
		delete it->second;
}

//	Initialize counters
bool CECC2SCmdCache::InitCounters()
{
	using namespace C2S;

	//	'Use item' command time counter
	CECCounter* pCnt = new CECCounter;
	pCnt->SetPeriod(200);
	pCnt->Reset(true);
	m_CounterMap[USE_ITEM] = pCnt;

	//	'Pickup item' command time counter
	pCnt = new CECCounter;
	pCnt->SetPeriod(500);
	m_CounterMap[PICKUP] = pCnt;

	//	'Select target' command time counter
	pCnt = new CECCounter;
	pCnt->SetPeriod(250);
	m_CounterMap[SELECT_TARGET] = pCnt;

	//	'Get extend properties' command time counter
	pCnt = new CECCounter;
	pCnt->SetPeriod(2000);
	pCnt->Reset(true);
	m_CounterMap[GET_EXT_PROP] = pCnt;

	//	'Get reborn pet properties' command time counter
	pCnt = new CECCounter;
	pCnt->SetPeriod(2000);
	pCnt->Reset(true);
	m_CounterMap[GET_SUMMON_PETPROP] = pCnt;

	//	'Cast skill' command time counter
	pCnt = new CECCounter;
	pCnt->SetPeriod(200);
	pCnt->Reset(true);
	m_CounterMap[CAST_SKILL] = pCnt;

	//	'Revive ask' command time counter
	pCnt = new CECCounter;
	pCnt->SetPeriod(500);
	pCnt->Reset(true);
	m_CounterMap[REVIVE_VILLAGE] = pCnt;
	
	//	'Enter sanctuary' command time counter
	pCnt = new CECCounter;
	pCnt->SetPeriod(2000);
	m_CounterMap[ENTER_SANCTUARY] = pCnt;
	
	//	'Enter instance' command time counter
	pCnt = new CECCounter;
	pCnt->SetPeriod(2000);
	pCnt->Reset(true);
	m_CounterMap[ENTER_INSTANCE] = pCnt;

	//	'Rush fly' command time counter
	pCnt = new CECCounter;
	pCnt->SetPeriod(500);
	pCnt->Reset(true);
	m_CounterMap[ACTIVE_RUSH_FLY] = pCnt;

	//	'Cancel action' command time counter
	pCnt = new CECCounter;
	pCnt->SetPeriod(200);
	pCnt->Reset(true);
	m_CounterMap[CANCEL_ACTION] = pCnt;

	//	'Hello' command time counter
	pCnt = new CECCounter;
	pCnt->SetPeriod(2000);
	pCnt->Reset(true);
	m_CounterMap[SEVNPC_HELLO] = pCnt;
	
	return true;
}

//	Remove all un-sent commands in cache
void CECC2SCmdCache::RemoveAllCmds()
{
	ALISTPOSITION pos = m_UseItemCmdList.GetHeadPosition();
	while (pos)
	{
		C2S::cmd_use_item* pCmd = m_UseItemCmdList.GetNext(pos);
		delete pCmd;
	}

	m_UseItemCmdList.RemoveAll();
	m_CounterMap[C2S::USE_ITEM]->Reset(true);

	m_bGetExpProps = false;
	m_CounterMap[C2S::GET_EXT_PROP]->Reset(true);

	m_bEnterSanctuary = false;
	m_CounterMap[C2S::ENTER_SANCTUARY]->Reset(true);	
}

//	Tick routine
bool CECC2SCmdCache::Tick(DWORD dwDeltaTime)
{
	DWORD dwRealTime = g_pGame->GetRealTickTime();

	CounterTable::iterator it = m_CounterMap.begin();
	for (; it != m_CounterMap.end(); ++it)
		((CECCounter*)it->second)->IncCounter(dwRealTime);

	//	Try to send 'use item' command in cache
	SendCachedCmdUseItem();

	//	Try to send 'get extend properties' command
	SendCachedCmdGetExtProp();

	if (m_bEnterSanctuary)
	{
		CECCounter* pCnt = m_CounterMap[C2S::ENTER_SANCTUARY];
		if (pCnt->IsFull())
		{
			c2s_SendCmdEnterSanctuary();
			m_bEnterSanctuary = false;
		}
	}

	return true;
}

//	Send 'use item' command
void CECC2SCmdCache::SendCachedCmdUseItem()
{
	CECCounter* pCnt = m_CounterMap[C2S::USE_ITEM];
	if (!pCnt->IsFull() || !m_UseItemCmdList.GetCount())
		return;

	pCnt->Reset();

	//	Send the first item
	C2S::cmd_use_item* pCmd = m_UseItemCmdList.GetHead();
	c2s_SendCmdUseItem(pCmd->where, (BYTE)pCmd->index, pCmd->item_id, pCmd->byCount);
	delete pCmd;
	m_UseItemCmdList.RemoveHead();
}

//	Send cached 'get extend properties' command
void CECC2SCmdCache::SendCachedCmdGetExtProp()
{
	CECCounter* pCnt = m_CounterMap[C2S::GET_EXT_PROP];
	if (!m_bGetExpProps || !pCnt->IsFull())
		return;

	pCnt->Reset();

	m_bGetExpProps = false;
	c2s_SendCmdGetExtProps();
}

/*	Send use item command

	The strategy to send 'use item' command: 

	1. if enough time has passed since last using time, send command directly.
	2. if 1 wasn't met and command cache is empty, put command into cache
	3. if 1, 2 weren't met, look through command cache, if found a command using
	   the item in the same slot, increase counter of item will be used and 
	   jump out of function.
    4. if 1, 2, 3 weren't met, add command to cache.
*/
void CECC2SCmdCache::SendCmdUseItem(BYTE byPackage, BYTE bySlot, int tid, BYTE byCount)
{
	using namespace C2S;

	CECCounter* pCnt = m_CounterMap[USE_ITEM];
	if (!m_UseItemCmdList.GetCount() && pCnt->IsFull())
	{
		pCnt->Reset();
		c2s_SendCmdUseItem(byPackage, bySlot, tid, byCount);
		return;
	}

	if (!m_UseItemCmdList.GetCount())
	{
		cmd_use_item* pCmd = new cmd_use_item;
		if (!pCmd)
			return;

		pCmd->where		= byPackage;
		pCmd->index		= bySlot;
		pCmd->item_id	= tid;
		pCmd->byCount	= byCount;
		m_UseItemCmdList.AddTail(pCmd);
		return;
	}

	ALISTPOSITION pos = m_UseItemCmdList.GetHeadPosition();
	while (pos)
	{
		cmd_use_item* pCmd = m_UseItemCmdList.GetAt(pos);

		if (pCmd->where == byPackage && pCmd->index == bySlot)
		{
			CECHostPlayer* pHost = g_pGame->GetGameRun()->GetHostPlayer();
			CECInventory* pPack = pHost->GetPack(byPackage);
			if (!pPack) 
				return;

			CECIvtrItem* pItem = pPack->GetItem(bySlot);
			if (!pItem || !pItem->CheckUseCondition())
				return;

			int iTotal = pCmd->byCount + byCount;
			if (iTotal >= pItem->GetCount())
				iTotal = pItem->GetCount();

			a_ClampRoof(iTotal, 255);

			pCmd->byCount = (BYTE)iTotal;
			break;
		}

		m_UseItemCmdList.GetNext(pos);
	}

	if (!pos)
	{
		cmd_use_item* pCmd = new cmd_use_item;
		if (!pCmd)
			return;

		pCmd->where		= byPackage;
		pCmd->index		= bySlot;
		pCmd->item_id	= tid;
		pCmd->byCount	= byCount;
		m_UseItemCmdList.AddTail(pCmd);
	}

	//	Try to send command in cache
	SendCachedCmdUseItem();
}

/*	Send 'pick up item' command

	The strategy to send 'pick up item' command: 

	1. if the item is just the one which was to be picked up, check whether
	   enough time has passed since last command was sent. If true, send
	   command again, otherwise just throw command
    2. if the item isn't the one which was to be picked up, send command
	   directly.
*/
void CECC2SCmdCache::SendCmdPickUp(int idItem, int tid)
{
	StackChecker::ACTrace(5);

	CECCounter* pCnt = m_CounterMap[C2S::PICKUP];
	if (m_idLastPickUpItem != idItem || pCnt->IsFull())
	{
		pCnt->Reset();
		c2s_SendCmdPickup(idItem, tid);
		m_idLastPickUpItem = idItem;
	}
}

//	Send 'select target' command
//	The strategy to send 'select target' command is like as SendCmdPickUp()
void CECC2SCmdCache::SendCmdSelectTarget(int id)
{
	CECCounter* pCnt = m_CounterMap[C2S::SELECT_TARGET];
	if (m_idLastSelTarget != id || pCnt->IsFull())
	{
		pCnt->Reset();
		c2s_SendCmdSelectTarget(id);
		m_idLastSelTarget = id;
	}
}

//	Send 'get extend properties' command
void CECC2SCmdCache::SendCmdExtProps()
{
	//	If there is already a request waiting, return directly
	if (m_bGetExpProps)
		return;

	CECCounter* pCnt = m_CounterMap[C2S::GET_EXT_PROP];
	if (pCnt->IsFull())
	{
		pCnt->Reset();
		c2s_SendCmdGetExtProps();
		return;
	}

	m_bGetExpProps = true;
}

//	Send 'get extend properties' command
void CECC2SCmdCache::SendCmdPetProps()
{
	CECCounter* pCnt = m_CounterMap[C2S::GET_SUMMON_PETPROP];
	if (pCnt->IsFull())
	{
		pCnt->Reset();
		c2s_SendCmdGetPetProps();
		return;
	}
}

//	Send 'revive ask' command
void CECC2SCmdCache::SendCmdReviveVillage()
{
	CECCounter* pCnt = m_CounterMap[C2S::REVIVE_VILLAGE];
	if (pCnt->IsFull())
	{
		pCnt->Reset();
		c2s_SendCmdReviveVillage();
	}
}

//	Send 'revive ask' command
void CECC2SCmdCache::SendCmdReviveItem()
{
	//	This command merge time counter with REVIVE_VILLAGE
	CECCounter* pCnt = m_CounterMap[C2S::REVIVE_VILLAGE];
	if (pCnt->IsFull())
	{
		pCnt->Reset();
		c2s_SendCmdReviveItem();
	}
}

//	Send 'cast skill' command
void CECC2SCmdCache::SendCmdCastSkill(int idSkill, BYTE byPVPMask, int iNumTarget, int* aTargets, A3DVECTOR3* pPos)
{
	CECCounter* pCnt = m_CounterMap[C2S::CAST_SKILL];
	if (pCnt->IsFull())
	{
		pCnt->Reset();
//		g_pGame->RuntimeDebugInfo(0xffffffff, _AL("Cast skill: %d"), GetTickCount());
// 		DWORD time = ACounter::GetMilliSecondNow();
// 		AString str;
// 		str.Format("cast skill start time : %d\n", time);
// 		OutputDebugStringA(str);

		c2s_SendCmdCastSkill(idSkill, byPVPMask, iNumTarget, aTargets, pPos);
	}
}

//	Send 'cast instant skill' command
void CECC2SCmdCache::SendCmdCastInstantSkill(int idSkill, BYTE byPVPMask, int iNumTarget, int* aTargets, A3DVECTOR3* pPos)
{
	CECCounter* pCnt = m_CounterMap[C2S::CAST_SKILL];
	if (pCnt->IsFull())
	{
		pCnt->Reset();
		c2s_SendCmdCastInstantSkill(idSkill, byPVPMask, iNumTarget, aTargets, pPos);
	}
}

void CECC2SCmdCache::SendCmdCastApoiseSkill(int idSkill, BYTE bySkillType, int item_id, short item_index, BYTE force_attack, int iNumTarget, int* aTargets)
{
	CECCounter* pCnt = m_CounterMap[C2S::CAST_SKILL];
	if (pCnt->IsFull())
	{
		pCnt->Reset();
		c2s_SendCmdCastApoiseSkill(idSkill, bySkillType, item_id, item_index, force_attack, iNumTarget, aTargets);
	}
}

void CECC2SCmdCache::SendCmdTransfigureSkill(int idSkill, BYTE bySkillType, BYTE byPVPMask, int iNumTarget, int* aTargets, A3DVECTOR3* pPos)
{
	CECCounter* pCnt = m_CounterMap[C2S::CAST_SKILL];
	if (pCnt->IsFull())
	{
		pCnt->Reset();
		c2s_SendCmdCastTransfigureSkill(idSkill, bySkillType, byPVPMask, iNumTarget, aTargets, pPos);
	}
}

void CECC2SCmdCache::SendCmdChargeSkill(int idSkill, BYTE byPVPMask, int idTarget, const A3DVECTOR3& vPos, int iNumTarget, int* aTargets)
{
	CECCounter* pCnt = m_CounterMap[C2S::CAST_SKILL];
	if (pCnt->IsFull())
	{
		pCnt->Reset();
		c2s_SendCmdCastChargeSkill(idSkill, byPVPMask, idTarget, vPos, iNumTarget, aTargets);
	}
}

//	Send 'enter sanctuary' command
void CECC2SCmdCache::SendCmdEnterSanctuary()
{
	//	Delay this command some time
	m_bEnterSanctuary = true;
	CECCounter* pCnt = m_CounterMap[C2S::ENTER_SANCTUARY];
	pCnt->Reset();
}

//	Send 'enter instance' commnad
void CECC2SCmdCache::SendCmdEnterInstance(int iTransIdx, int idInst)
{
	CECCounter* pCnt = m_CounterMap[C2S::ENTER_INSTANCE];
	if (pCnt->IsFull())
	{
		pCnt->Reset();
		c2s_SendCmdEnterInstance(iTransIdx, idInst);
	}
}

void CECC2SCmdCache::SendCmdActiveRushFly(bool bActive)
{
	CECCounter* pCnt = m_CounterMap[C2S::ACTIVE_RUSH_FLY];
	if (pCnt->IsFull())
	{
		pCnt->Reset();
		c2s_SendCmdActiveRushFly(bActive);
	}
}

void CECC2SCmdCache::SendCmdCancelAction()
{
	CECCounter* pCnt = m_CounterMap[C2S::CANCEL_ACTION];
	if (pCnt->IsFull())
	{
		pCnt->Reset();
		c2s_SendCmdCancelAction();
	}
}

void CECC2SCmdCache::SendCmdNPCSevHello(int nid)
{
	CECCounter* pCnt = m_CounterMap[C2S::SEVNPC_HELLO];
	if (pCnt->IsFull())
	{
		pCnt->Reset();
		c2s_SendCmdNPCSevHello(nid);
	}
}


