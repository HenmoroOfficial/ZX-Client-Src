/*
 * FILE: EC_DealInventory.cpp
 *
 * DESCRIPTION: 
 *
 * CREATED BY: Duyuxin, 2004/12/22
 *
 * HISTORY: 
 *
 * Copyright (c) 2004 Archosaur Studio, All Rights Reserved.
 */

#include "EC_Global.h"
#include "EC_DealInventory.h"
#include "EC_IvtrItem.h"
#include "EC_IvtrTypes.h"
#include "EC_RTDebug.h"
#include "EC_Game.h"

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
//	Implement CECDealInventory
//	
///////////////////////////////////////////////////////////////////////////

CECDealInventory::CECDealInventory()
{
	m_iMoney = 0;
}

CECDealInventory::~CECDealInventory()
{
}

//	Initalize object
bool CECDealInventory::Init(int iSize)
{
	Resize(iSize);
	return true;
}

//	Release object
void CECDealInventory::Release()
{
	//	Release all items
	for (int i=0; i < m_aItems.GetSize(); i++)
	{
		if (m_aItems[i] && m_aItemInfo[i].bDelete)
			delete m_aItems[i];
	}

	m_aItems.RemoveAll();
	m_aItemInfo.RemoveAll();
}

//	Resize inventory
void CECDealInventory::Resize(int iNewSize)
{
	//	Resize item object array
	int i, iOldSize = m_aItems.GetSize();
	if (iOldSize > iNewSize)
	{
		ASSERT(iOldSize <= iNewSize);
		return;
	}

	m_aItems.SetSize(iNewSize, 10);

	//	Clear new slots
	for (i=iOldSize; i < iNewSize; i++)
		m_aItems[i] = NULL;

	//	Resize item info. array
	m_aItemInfo.SetSize(iNewSize, 10);

	//	Clear new slots
	for (i=iOldSize; i < iNewSize; i++)
	{
		m_aItemInfo[i].iOrigin	= 0;
		m_aItemInfo[i].iAmount	= 0;
		m_aItemInfo[i].bDelete	= false;
		m_aItemInfo[i].iFlag	= 0;
	}
}

//	Get a item from inventory
CECIvtrItem* CECDealInventory::GetItem(int iSlot)
{
	if (iSlot < 0 || iSlot >= m_aItems.GetSize())
	{
		ASSERT(0);
		return NULL;
	}

	return m_aItems[iSlot];
}

//	Add an item. This function is used when trade with other players
bool CECDealInventory::AddItem(CECIvtrItem* pItem, int iOrigin, int iAmount,
						bool bDelete/* false */)
{
	int i, iEmpty = -1;

	//	Check whether there is an item came from the same original posiiton
	for (i=0; i < m_aItems.GetSize(); i++)
	{
		if (!m_aItems[i])
		{
			if (iEmpty < 0)
				iEmpty = i;
		}
		else if (m_aItemInfo[i].iOrigin == iOrigin)
		{
			if (bDelete)
				delete pItem;

			m_aItemInfo[i].iAmount += iAmount;
			return true;
		}
	}

	//	Add to the first empty slot
	if (iEmpty < 0)
	{
		g_pGame->RuntimeDebugInfo(RTDCOL_WARNING, _AL("Deal package is full"));
		return false;
	}

	m_aItems[iEmpty] = pItem;
	m_aItemInfo[iEmpty].iOrigin = iOrigin;
	m_aItemInfo[iEmpty].iAmount = iAmount;
	m_aItemInfo[iEmpty].bDelete = bDelete;
	m_aItemInfo[iEmpty].iFlag	= 0;

	return true;
}

//	Add an item freely. This function is used when trade with NPCs
//	Return the number of items which cannot be added to package (package is full)
int CECDealInventory::AddItemFree(CECIvtrItem* pItem, int iOrigin, int iAmount, unsigned int* repu_id /*NULL*/, unsigned int* repu_consume /*NULL*/)
{
	int iRemain = iAmount;

	for (int i=0; i < m_aItems.GetSize(); i++)
	{
		if (!m_aItems[i])
		{
			int iSlotNum = iRemain;
			if (iSlotNum > pItem->GetPileLimit())
				iSlotNum = pItem->GetPileLimit();

			m_aItems[i] = pItem;
			m_aItemInfo[i].iOrigin	= iOrigin;
			m_aItemInfo[i].iAmount	= iSlotNum;
			m_aItemInfo[i].bDelete	= false;
			m_aItemInfo[i].iFlag	= 0;

			iRemain -= iSlotNum;

			if(repu_id && repu_consume)
				for(int j = 0; j < 3; j++)
				{
					m_aItemInfo[i].repu_id_consume[j] = repu_id[j];
					m_aItemInfo[i].repu_consume_value[j] = repu_consume[j];
				}
		}
		else if (m_aItemInfo[i].iOrigin == iOrigin &&
			m_aItemInfo[i].iAmount < pItem->GetPileLimit())
		{
			int iSlotNum = iRemain;
			if (iSlotNum > pItem->GetPileLimit() - m_aItemInfo[i].iAmount)
				iSlotNum = pItem->GetPileLimit() - m_aItemInfo[i].iAmount;

			m_aItemInfo[i].iAmount += iSlotNum;

			iRemain -= iSlotNum;
		}

		if (!iRemain)
			break;
	}

	return iRemain;
}

//	Add an booth item. This function is used when open booth
int CECDealInventory::AddBoothItem(CECIvtrItem* pItem, int iOrigin, int iAmount, int iUnitPrice)
{
	int i;

	//	Search for empty slot
	for (i=0; i < m_aItems.GetSize(); i++)
	{
		if (!m_aItems[i])
			break;
	}

	if (i >= m_aItems.GetSize())
		return -1;

	//	Clone item
	CECIvtrItem* pClonedItem = pItem->Clone();
	if (!pClonedItem)
	{
		a_LogOutput(1, "CECDealInventory::AddBoothItem, Failed to clone item");
		return -1;
	}

	m_aItems[i]	= pClonedItem;

	pClonedItem->SetPriceScale(CECIvtrItem::SCALE_BOOTH, 1.0f);
	pClonedItem->SetUnitPrice(iUnitPrice);
	pClonedItem->SetAmount(iAmount);

	m_aItemInfo[i].iOrigin	= iOrigin;
	m_aItemInfo[i].iAmount	= iAmount;
	m_aItemInfo[i].bDelete	= true;
	m_aItemInfo[i].iFlag	= 0;

	return i;
}

//	Set an booth item. This function is used when visite else player's booth
bool CECDealInventory::AddEPBoothItem(int idItem, int iExpireDate, int iState, int iOrigin, int iAmount, 
							int iUnitPrice, BYTE* pData, int iDataLen)
{
	int i;

	//	Search an empty slot
	for (i=0; i < m_aItems.GetSize(); i++)
	{
		if (!m_aItems[i])
			break;
	}

	if (i >= m_aItems.GetSize())
		return false;

	CECIvtrItem* pItem = CECIvtrItem::CreateItem(idItem, iExpireDate, iAmount);
	if (!pItem)
		return false;

	if (pData && iDataLen)
		pItem->SetItemInfo(pData, iDataLen);
	else
		pItem->SetEmptyInfo();
	pItem->SetPriceScale(CECIvtrItem::SCALE_BOOTH, 1.0f);
	pItem->SetUnitPrice(iUnitPrice);
	pItem->SetExpireDate(iExpireDate);
	pItem->SetAmount(iAmount);
	pItem->SetStateFlag(iState);
	pItem->UpdateInfoMD5();

	m_aItems[i] = pItem;

	m_aItemInfo[i].iOrigin	= iOrigin;
	m_aItemInfo[i].iAmount	= iAmount;
	m_aItemInfo[i].bDelete	= true;
	m_aItemInfo[i].iFlag	= 0;
	
	return true;
}

//	Remove an item
void CECDealInventory::RemoveItem(int iOrigin, int iAmount)
{
	for (int i=0; i < m_aItems.GetSize(); i++)
	{
		if (m_aItems[i] && m_aItemInfo[i].iOrigin == iOrigin)
		{
			if (iAmount < 0 || iAmount >= m_aItemInfo[i].iAmount)
			{
				if (m_aItemInfo[i].bDelete)
					delete m_aItems[i];

				m_aItems[i] = NULL;
			}
			else
			{
				m_aItemInfo[i].iAmount -= iAmount;

				if (m_aItemInfo[i].bDelete)
					m_aItems[i]->AddAmount(-iAmount);
			}

			return;
		}
	}
}

void CECDealInventory::RemoveItemByFlag(int iFlag, int iAmount)
{
	for (int i=0; i < m_aItems.GetSize(); i++)
	{
		if (m_aItems[i] && m_aItemInfo[i].iFlag == iFlag)
		{
			if (iAmount < 0 || iAmount >= m_aItemInfo[i].iAmount)
			{
				if (m_aItemInfo[i].bDelete)
					delete m_aItems[i];

				m_aItems[i] = NULL;
			}
			else
			{	
				m_aItemInfo[i].iAmount -= iAmount;

				if (m_aItemInfo[i].bDelete)
					m_aItems[i]->AddAmount(-iAmount);
			}

			return;
		}
	}
}

void CECDealInventory::RemoveItemByIndex(int iIndex, int iAmount)
{
	if (iIndex < 0 || iIndex >= m_aItems.GetSize())
	{
		ASSERT(0);
		return;
	}

	if (m_aItems[iIndex])
	{
		if (iAmount < 0 || iAmount >= m_aItemInfo[iIndex].iAmount)
		{
			if (m_aItemInfo[iIndex].bDelete)
				delete m_aItems[iIndex];

			m_aItems[iIndex] = NULL;
		}
		else
		{
			m_aItemInfo[iIndex].iAmount -= iAmount;

			if (m_aItemInfo[iIndex].bDelete)
				m_aItems[iIndex]->AddAmount(-iAmount);
		}
	}
}

//	Get item's index in inventory according to item's original position
int CECDealInventory::GetItemIndex(int iOrigin)
{
	for (int i=0; i < m_aItems.GetSize(); i++)
	{
		if (m_aItems[i] && m_aItemInfo[i].iOrigin == iOrigin)
			return i;
	}

	return -1;
}

//	Get item's index through item's flag
int CECDealInventory::GetItemIndexByFlag(int iFlag)
{
	for (int i=0; i < m_aItems.GetSize(); i++)
	{
		if (m_aItems[i] && m_aItemInfo[i].iFlag == iFlag)
			return i;
	}

	return -1;
}

//	Set flag in item info.
void CECDealInventory::SetItemInfoFlag(int iIndex, int iFlag)
{
	if (iIndex < 0 || iIndex >= m_aItems.GetSize())
	{
		ASSERT(0);
		return;
	}

	m_aItemInfo[iIndex].iFlag = iFlag;
}

//	Remove all items
void CECDealInventory::RemoveAllItems()
{
	for (int i=0; i < m_aItems.GetSize(); i++)
	{
		if (m_aItems[i])
		{
			if (m_aItemInfo[i].bDelete)
				delete m_aItems[i];

			m_aItems[i] = NULL;
		}
	}

	m_iMoney = 0;
}

//	Unfreeze all items
void CECDealInventory::UnfreezeAllItems()
{
	//	Release all items
	for (int i=0; i < m_aItems.GetSize(); i++)
	{
		if (m_aItems[i])
			m_aItems[i]->Freeze(false);
	}
}

//	Search a empty slot
int CECDealInventory::SearchEmpty()
{
	for (int i=0; i < m_aItems.GetSize(); i++)
	{
		if (!m_aItems[i])
			return i;
	}

	return -1;
}

//	Get empty slots number
int CECDealInventory::GetEmptySlotNum()
{
	int i, iCount=0;

	for (i=0; i < m_aItems.GetSize(); i++)
	{
		if (!m_aItems[i])
			iCount++;
	}

	return iCount;
}


