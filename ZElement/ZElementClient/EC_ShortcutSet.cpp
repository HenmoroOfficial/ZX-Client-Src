/*
 * FILE: EC_ShortcutSet.cpp
 *
 * DESCRIPTION: 
 *
 * CREATED BY: Duyuxin, 2005/1/5
 *
 * HISTORY: 
 *
 * Copyright (c) 2005 Archosaur Studio, All Rights Reserved.
 */

#include "EC_Global.h"
#include "EC_ShortcutSet.h"
#include "EC_Shortcut.h"
#include "EC_Game.h"
#include "EC_GameRun.h"
#include "EC_HostPlayer.h"
#include "EC_Skill.h"
#include "EC_IvtrItem.h"
#include "EC_Inventory.h"

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
//	Implement CECShortcutSet
//	
///////////////////////////////////////////////////////////////////////////

CECShortcutSet::CECShortcutSet()
{
}

CECShortcutSet::~CECShortcutSet()
{
}

//	Initalize object
bool CECShortcutSet::Init(int iSize)
{
	m_aShortcuts.SetSize(iSize, 10);
	for (int i=0; i < iSize; i++)
		m_aShortcuts[i] = NULL;

	return true;
}

//	Release object
void CECShortcutSet::Release()
{
	//	Release all items
	for (int i=0; i < m_aShortcuts.GetSize(); i++)
	{
		if (m_aShortcuts[i])
			delete m_aShortcuts[i];
	}

	m_aShortcuts.RemoveAll();
}

//	Create a item shortcut at specified position
bool CECShortcutSet::CreateItemShortcut(int iSlot, int iIvtr, int iIvtrSlot, CECIvtrItem* pItem)
{
	CECSCItem* pItemSC = new CECSCItem;
	if (!pItemSC)
		return false;

	if (!pItemSC->Init(iIvtr, iIvtrSlot, pItem))
	{
		delete pItemSC;
		a_LogOutput(1, "CECShortcutSet::CreateItemShortcut, Failed to initialize item shortcut");
		return false;
	}

	SetShortcut(iSlot, pItemSC);
	return true;
}

//	Create a skill shortcut at specified position
bool CECShortcutSet::CreateSkillShortcut(int iSlot, CECSkill* pSkill)
{
	CECSCSkill* pSkillSC = new CECSCSkill;
	if (!pSkillSC)
		return false;

	if (!pSkillSC->Init(pSkill))
	{
		delete pSkillSC;
		a_LogOutput(1, "CECShortcutSet::CreateSkillShortcut, Failed to initialize skill shortcut");
		return false;
	}

	SetShortcut(iSlot, pSkillSC);
	return true;
}

//	Create a skill shortcut can't be used currently at specified position
bool CECShortcutSet::CreateSkillDumbShortcut(int iSlot, int id)
{
	CECSCSkillDumb* pSkillDumbSC = new CECSCSkillDumb;
	if (!pSkillDumbSC)
		return false;
	
	if (!pSkillDumbSC->Init(id))
	{
		delete pSkillDumbSC;
		a_LogOutput(1, "CECShortcutSet::CreateSkillDumbShortcut, Failed to initialize skill dumb shortcut");
		return false;
	}
	
	SetShortcut(iSlot, pSkillDumbSC);
	return true;
}

//	Create a skill group at specified position
bool CECShortcutSet::CreateSkillGroupShortcut(int iSlot, int iGroupIdx)
{
	CECSCSkillGrp* pSkillGrpSC = new CECSCSkillGrp;
	if (!pSkillGrpSC)
		return false;

	if (!pSkillGrpSC->Init(iGroupIdx))
	{
		delete pSkillGrpSC;
		a_LogOutput(1, "CECShortcutSet::CreateSkillGroupShortcut, Failed to initialize skill group shortcut");
		return false;
	}

	SetShortcut(iSlot, pSkillGrpSC);
	return true;
}

//	Duplicate a shortcut to specified position
bool CECShortcutSet::CreateClonedShortcut(int iSlot, CECShortcut* pShortcut)
{
	CECShortcut* pDest = pShortcut->Clone();
	if (!pDest)
		return false;

	SetShortcut(iSlot, pDest);
	return true;
}

bool CECShortcutSet::CreateTargetShortcut(int iSlot, int nType, int id, int level,int prof, int feishengcnt, int cultivation)
{
	CECSCTarget* pSCTarget = new CECSCTarget;
	if(!pSCTarget)
		return false;

	if(!pSCTarget->Init(nType, id, level, prof, feishengcnt, cultivation))
	{
		delete pSCTarget;
		a_LogOutput(1, "CECShortcutSet::CreateTargetShortcut, Failed to initialize target shortcut");
		return false;
	}

	SetShortcut(iSlot, pSCTarget);
	return true;
}

//	Put a shortcut into set
CECShortcut* CECShortcutSet::PutShortcut(int iSlot, CECShortcut* pShortcut)
{
	if (iSlot < 0 || iSlot >= m_aShortcuts.GetSize())
		return NULL;

	CECShortcut* pOldShortcut = m_aShortcuts[iSlot];
	m_aShortcuts[iSlot] = pShortcut;
	return pOldShortcut;
}

//	Get a shortcut from set
CECShortcut* CECShortcutSet::GetShortcut(int iSlot, bool bRemove/* false */)
{
	if (iSlot < 0 || iSlot >= m_aShortcuts.GetSize())
		return NULL;

	CECShortcut* pShortcut = m_aShortcuts[iSlot];
	if (bRemove)
		m_aShortcuts[iSlot] = NULL;

	return pShortcut;
}

//	Set a shortcut into set and release old shortcut at this position automatically
void CECShortcutSet::SetShortcut(int iSlot, CECShortcut* pShortcut)
{
	if (iSlot < 0 || iSlot >= m_aShortcuts.GetSize())
		return;

	if (m_aShortcuts[iSlot])
		delete m_aShortcuts[iSlot];

	m_aShortcuts[iSlot] = pShortcut;
}

//	Exchange shortcut in inventory
void CECShortcutSet::ExchangeShortcut(int iSlot1, int iSlot2)
{
	if (iSlot1 < 0 || iSlot1 >= m_aShortcuts.GetSize() ||
		iSlot2 < 0 || iSlot2 >= m_aShortcuts.GetSize())
		return;

	if (iSlot1 != iSlot2)
	{
		CECShortcut* pShortcut = m_aShortcuts[iSlot1];
		m_aShortcuts[iSlot1] = m_aShortcuts[iSlot2];
		m_aShortcuts[iSlot2] = pShortcut;
	}
}

//	Remove all shortcuts
void CECShortcutSet::RemoveAllShortcuts()
{
	for (int i=0; i < m_aShortcuts.GetSize(); i++)
	{
		if (m_aShortcuts[i])
		{
			delete m_aShortcuts[i];
			m_aShortcuts[i] = NULL;
		}
	}
}

//	Update item shortcut when item position changed
void CECShortcutSet::UpdateMovedItemSC(int tidItem, int iSrcIvtr, int iSrcSlot, int iDstIvtr, int iDstSlot)
{
	for (int i=0; i < m_aShortcuts.GetSize(); i++)
	{
		CECShortcut* pSC = m_aShortcuts[i];
		if (!pSC || pSC->GetType() != CECShortcut::SCT_ITEM)
			continue;

		CECSCItem* pItemSC = (CECSCItem*)pSC;
		if (pItemSC->GetInventory() == iSrcIvtr && pItemSC->GetIvtrSlot() == iSrcSlot)
		{
			ASSERT(pItemSC->GetItemTID() == tidItem);
			pItemSC->MoveItem(iDstIvtr, iDstSlot);
		}
	}
}

//	Update item shortcut when item removed
void CECShortcutSet::UpdateRemovedItemSC(int tidItem, int iIvtr, int iSlot, int iSameItem)
{
	for (int i=0; i < m_aShortcuts.GetSize(); i++)
	{
		CECShortcut* pSC = m_aShortcuts[i];
		if (!pSC || pSC->GetType() != CECShortcut::SCT_ITEM)
			continue;

		CECSCItem* pItemSC = (CECSCItem*)pSC;
		if (pItemSC->GetInventory() == iIvtr && pItemSC->GetIvtrSlot() == iSlot)
		{
			ASSERT(pItemSC->GetItemTID() == tidItem);

			if (pItemSC->GetAutoFindFlag() && iSameItem >= 0)
				pItemSC->MoveItem(iIvtr, iSameItem);
			else
				SetShortcut(i, NULL);
		}
	}
}

//	Update item shortcut when two items exchanged
void CECShortcutSet::UpdateExchangedItemSC(int tidItem1, int iIvtr1, int iSlot1, 
										   int tidItem2, int iIvtr2, int iSlot2)
{
	for (int i=0; i < m_aShortcuts.GetSize(); i++)
	{
		CECShortcut* pSC = m_aShortcuts[i];
		if (!pSC || pSC->GetType() != CECShortcut::SCT_ITEM)
			continue;
		
		CECSCItem* pItemSC = (CECSCItem*)pSC;

		if (pItemSC->GetInventory() == iIvtr1 && pItemSC->GetIvtrSlot() == iSlot1)
		{
			ASSERT(pItemSC->GetItemTID() == tidItem1);
			pItemSC->MoveItem(iIvtr2, iSlot2);
		}
		else if (pItemSC->GetInventory() == iIvtr2 && pItemSC->GetIvtrSlot() == iSlot2)
		{
			ASSERT(pItemSC->GetItemTID() == tidItem2);
			pItemSC->MoveItem(iIvtr1, iSlot1);
		}
	}
}

//	Save shortcut configs to specified buffer
bool CECShortcutSet::SaveConfigData(void* pDataBuf, int* piSize)
{
	int iTotalSize = 0;
	BYTE* pData = (BYTE*)pDataBuf;

	int iTargetType = 0;

	for (int i=0; i < m_aShortcuts.GetSize(); i++)
	{
		CECShortcut* pSC = m_aShortcuts[i];
		if (!pSC)
			continue;

		iTotalSize += sizeof (int) * 2;

		//	Record shortcut's position and type
		if (pDataBuf)
		{
			*((int*)pData) = i;
			pData += sizeof (int);
			*((int*)pData) = pSC->GetType();
			pData += sizeof (int);
		}

		switch (pSC->GetType())
		{
		case CECShortcut::SCT_COMMAND:
			
			iTotalSize += sizeof (int) * 2;
			if (pDataBuf)
			{
				*((int*)pData) = ((CECSCCommand*)pSC)->GetCommandID();
				pData += sizeof (int);
				*((int*)pData) = (int)((CECSCCommand*)pSC)->GetParam();
				pData += sizeof (int);
			}

			break;

		case CECShortcut::SCT_SKILL:
			
			iTotalSize += sizeof (int);
			if (pDataBuf)
			{
				*((int*)pData) = ((CECSCSkill*)pSC)->GetSkill()->GetSkillID();
				pData += sizeof (int);
			}

			break;

		case CECShortcut::SCT_ITEM:
			
			iTotalSize += sizeof (int) * 3;
			if (pDataBuf)
			{
				*((int*)pData) = ((CECSCItem*)pSC)->GetInventory();
				pData += sizeof (int);
				*((int*)pData) = ((CECSCItem*)pSC)->GetIvtrSlot();
				pData += sizeof (int);
				*((int*)pData) = ((CECSCItem*)pSC)->GetItemTID();
				pData += sizeof (int);
			}

			break;
			
		case CECShortcut::SCT_SKILLGRP:

			iTotalSize += sizeof (int);
			if (pDataBuf)
			{
				*((int*)pData) = ((CECSCSkillGrp*)pSC)->GetGroupIndex();
				pData += sizeof (int);
			}

			break;
			
		case CECShortcut::SCT_SKILLDUMB:
			
			iTotalSize += sizeof (int);
			if (pDataBuf)
			{
				*((int*)pData) = ((CECSCSkillDumb*)pSC)->GetSkillId();
				pData += sizeof (int);
			}
			
			break;

		case CECShortcut::SCT_TARGET:
			
			iTargetType = ((CECSCTarget*)pSC)->GetTargetType();
			
			if(iTargetType == CECSCTarget::TT_PLAYER)
			{
				iTotalSize += sizeof (int)*6;
				if (pDataBuf)
				{
					*((int*)pData) = iTargetType;
					pData += sizeof (int);

					*((int*)pData) = ((CECSCTarget*)pSC)->GetTargetID();
					pData += sizeof (int);

					*((int*)pData) = ((CECSCTarget*)pSC)->GetTargetLevel();
					pData += sizeof (int);

					*((int*)pData) = ((CECSCTarget*)pSC)->GetTargetProfession();
					pData += sizeof (int);

					*((int*)pData) = ((CECSCTarget*)pSC)->GetFeishengCnt();
					pData += sizeof (int);

					*((int*)pData) = ((CECSCTarget*)pSC)->GetCultivation();
					pData += sizeof (int);
				}
			}
			else if(iTargetType == CECSCTarget::TT_MONSTER)
			{
				iTotalSize += sizeof (int)*3;
				if (pDataBuf)
				{
					*((int*)pData) = iTargetType;
					pData += sizeof (int);

					*((int*)pData) = ((CECSCTarget*)pSC)->GetTargetID();
					pData += sizeof (int);

					*((int*)pData) = ((CECSCTarget*)pSC)->GetTargetLevel();
					pData += sizeof (int);
				}
			}
			
			break;

		default:
			ASSERT(0);
			break;
		}
	}

	//	Add the end flag
	iTotalSize += sizeof (int);
	if (pDataBuf)
	{
		*((int*)pData) = -1;
		pData += sizeof (int);
	}
	
	if (piSize)
		*piSize = iTotalSize;

	return true;
}

//	Load shortcut configs from specified buffer
bool CECShortcutSet::LoadConfigData(const void* pDataBuf, int* piSize, DWORD dwVer)
{
	BYTE* pData = (BYTE*)pDataBuf;

	CECGameRun* pGameRun = g_pGame->GetGameRun();
	CECHostPlayer* pHost = pGameRun->GetHostPlayer();
	ASSERT(pHost);

	while (1)
	{
		int iSlot = *(int*)pData;
		pData += sizeof (int);
		if (iSlot < 0)
			break;	//	Meet end flag

		int iSCType = *(int*)pData;
		pData += sizeof (int);

		switch (iSCType)
		{
		case CECShortcut::SCT_COMMAND:
		{
			int iCommand = *(int*)pData;
			pData += sizeof (int);

			int iParam = 0;
			if (dwVer >= 2)
			{
				iParam = *(int*)pData;
				pData += sizeof (int);
			}

			CECSCCommand* pCmdSC = NULL;
			if (iCommand == CECSCCommand::CMD_PLAYPOSE)
				pCmdSC = pGameRun->GetPoseCmdShortcut(iParam);
			else if (iCommand == CECSCCommand::CMD_PLAYACTIVEPOSE)
				pCmdSC = pGameRun->GetActivePoseCmdShortcuts(iParam);
			else
				pCmdSC = pGameRun->GetCmdShortcut(iCommand);

		//	ASSERT(pCmdSC);
			if (pCmdSC)
				CreateClonedShortcut(iSlot, pCmdSC);
			
			break;
		}
		case CECShortcut::SCT_SKILL:
		case CECShortcut::SCT_SKILLDUMB:
		{
			int idSkill = *(int*)pData;
			pData += sizeof (int);
			CECSkill* pSkill = pHost->GetPositiveSkillByID(idSkill, true);
		//	ASSERT(pSkill);
			if (pSkill)
			{
				CreateSkillShortcut(iSlot, pSkill);
			}
			else
			{
				pSkill = new CECSkill(idSkill, 0);
				// 触发技能
				if (pSkill->IsCategory(GNET::CATEGORY_TRIGGERED))
					CreateSkillShortcut(iSlot, pSkill);
				else	// 家族技能等跨服没有的技能
					CreateSkillDumbShortcut(iSlot, idSkill);

				delete pSkill;
				pSkill = NULL;
			}

			break;
		}
		case CECShortcut::SCT_ITEM:
		{
			int iPack = *(int*)pData;
			pData += sizeof (int);
			int iIvtrSlot = *(int*)pData;
			pData += sizeof (int);
			int idItem = *(int*)pData;
			pData += sizeof (int);

			CECInventory* pPack = pHost->GetPack(iPack);
			if (!pPack)
			{
				ASSERT(0);
				return false;
			}

			CECIvtrItem* pItem = pPack->GetItem(iIvtrSlot);
		//	ASSERT(pItem && pItem->GetTemplateID() == idItem);
			if (pItem)
				CreateItemShortcut(iSlot, iPack, iIvtrSlot, pItem);

			break;
		}
		case CECShortcut::SCT_SKILLGRP:
		{
			if (dwVer >= 3)
			{
				int iGroupIdx = *(int*)pData;
				pData += sizeof (int);

				if (iGroupIdx >= 0)
					CreateSkillGroupShortcut(iSlot, iGroupIdx);
			}
			else
			{
				ASSERT(0);
				return false;
			}
			
			break;
		}
		case CECShortcut::SCT_TARGET:
		{
			int nTargetType = *(int*)pData;
			pData += sizeof (int);

			if(nTargetType == CECSCTarget::TT_PLAYER)
			{
				// id, level, prof, feisheng
				int id = *(int*)pData;
				pData += sizeof (int);

				int level = *(int*)pData;
				pData += sizeof (int);

				int prof = *(int*)pData;
				pData += sizeof (int);

				int feishengCnt = *(int*)pData;
				pData += sizeof (int);

				int cultivation = *(int*)pData;
				pData += sizeof (int);

				CreateTargetShortcut(iSlot, nTargetType, id, level, prof, feishengCnt, cultivation);

			}
			else if(nTargetType == CECSCTarget::TT_MONSTER)
			{
				// id, level
				int id = *(int*)pData;
				pData += sizeof (int);

				int level = *(int*)pData;
				pData += sizeof (int);

				CreateTargetShortcut(iSlot, nTargetType, id, level, 0,0, 0);
			}
			else
			{
				ASSERT(0);
				return false;
			}

			break;
		}
		default:
			ASSERT(0);
			return false;
		}
	}

	if (piSize)
		*piSize = pData - (BYTE*)pDataBuf;

	return true;
}

//	Replace skill id in skill shortcuts
void CECShortcutSet::ReplaceSkillID(int idOld, CECSkill* pNewSkill)
{
	for (int i=0; i < m_aShortcuts.GetSize(); i++)
	{
		CECShortcut* pSC = m_aShortcuts[i];
		if (!pSC || pSC->GetType() != CECShortcut::SCT_SKILL)
			continue;
		
		//	Replace skill
		CECSCSkill* pSkillSC = (CECSCSkill*)pSC;
		CECSkill* pSkill = pSkillSC->GetSkill();
		if (pSkill->GetSkillID() == idOld)
		{
			if (pNewSkill)	//	Replace skill
				pSkillSC->SetSkill(pNewSkill);
			else	//	Remove skill
				SetShortcut(i, NULL);
		}
	}
}

//	Tick routine
bool CECShortcutSet::Tick(DWORD dwDeltaTime)
{
	for (int i=0; i < m_aShortcuts.GetSize(); i++)
	{
		CECShortcut* pSC = m_aShortcuts[i];
		if (!pSC || pSC->GetType() != CECShortcut::SCT_SKILL)
			continue;
		
		CECSkill* pSkill = ((CECSCSkill*)pSC)->GetSkill();
		if ( pSkill->GetSkillLevel()<=0 && !pSkill->IsCategory(GNET::CATEGORY_TRIGGERED) )
			SetShortcut(i, NULL);
	}

	return true;
}

