/*
 * FILE: EC_Inventory.cpp
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
#include "EC_Inventory.h"
#include "EC_IvtrItem.h"
#include "EC_IvtrEquip.h"
#include "EC_Game.h"
#include "EC_RTDebug.h"
#include "EC_GPDataType.h"
#include "EC_GameRecord.h"
#include "hometowntemplate.h"
#include "EC_HTPDataType.h"

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
//	Implement CECInventory
//	
///////////////////////////////////////////////////////////////////////////

CECInventory::CECInventory()
{
}

CECInventory::~CECInventory()
{
}

//	Initalize object
bool CECInventory::Init(int iSize)
{
	Resize(iSize);
	return true;
}

//	Release object
void CECInventory::Release()
{
	//	Release all items
	for (int i=0; i < m_aItems.GetSize(); i++)
	{
		if (m_aItems[i])
			delete m_aItems[i];
	}

	m_aItems.RemoveAll();
}

//	Resize inventory
void CECInventory::Resize(int iNewSize)
{
	int iOldSize = m_aItems.GetSize();
	if (iOldSize > iNewSize)
	{
		ASSERT(iOldSize <= iNewSize);
		return;
	}

	m_aItems.SetSize(iNewSize, 10);

	//	Clear new slots
	for (int i=iOldSize; i < iNewSize; i++)
		m_aItems[i] = NULL;
}

//	Put a item into inventory
//	Return the old item in specified slot
CECIvtrItem* CECInventory::PutItem(int iSlot, CECIvtrItem* pItem)
{
	if (iSlot < 0 || iSlot >= m_aItems.GetSize())
	{
		ASSERT(0);
		return NULL;
	}

	CECIvtrItem* pOldItem = m_aItems[iSlot];
	m_aItems[iSlot] = pItem;
	return pOldItem;
}

//	Set a item into inventory and release old item at this position automatically
//	If NULL is passed to pItem, this function can be used to delete existing item
void CECInventory::SetItem(int iSlot, CECIvtrItem* pItem)
{
	if (iSlot < 0 || iSlot >= m_aItems.GetSize())
	{
		ASSERT(0);
		return;
	}

	if (m_aItems[iSlot])
	{
	//	g_pGame->RuntimeDebugInfo(RTDCOL_WARNING, _AL("Inventory item was deleted forcedly"));
		delete m_aItems[iSlot];
	}

	m_aItems[iSlot] = pItem;
}

//	Get a item from inventory
CECIvtrItem* CECInventory::GetItem(int iSlot, bool bRemove/* false */)
{
	if (iSlot < 0 || iSlot >= m_aItems.GetSize())
	{
		ASSERT(0);
		return NULL;
	}

	CECIvtrItem* pItem = m_aItems[iSlot];
	if (bRemove)
		m_aItems[iSlot] = NULL;

	return pItem;
}

//	Exchange item in inventory
void CECInventory::ExchangeItem(int iSlot1, int iSlot2)
{
	if (iSlot1 < 0 || iSlot1 >= m_aItems.GetSize() ||
		iSlot2 < 0 || iSlot2 >= m_aItems.GetSize())
	{
		ASSERT(0);
		return;
	}

	if (iSlot1 != iSlot2)
	{
		CECIvtrItem* pItem = m_aItems[iSlot1];
		m_aItems[iSlot1] = m_aItems[iSlot2];
		m_aItems[iSlot2] = pItem;
	}
}

/*	Put items into inventory, merge it with same kind item when necessary

	Return true if all item have been put into this inventory. Return false
	means all or part amount of item weren't put into.

	tid: item's template id
	iAmount: total amount of item will be put into inventory
	piLastSlot (out): if true is returned, this is index of the last slot 
		item was put into.
	piSlotAmount (out): if true is returned, this is total amount of item in slot 
		after items were merged.
*/
bool CECInventory::MergeItem(int tid, int state, int iExpireDate, int iAmount, int* piLastSlot, int* piSlotAmount)
{
	int iFirstEmpty = -1;

	for (int i=0; i < m_aItems.GetSize(); i++)
	{
		CECIvtrItem* pSlotItem = m_aItems[i];
		if (pSlotItem)
		{
			int iNumMerge = pSlotItem->MergeItem(tid, state, iAmount);
			iAmount -= iNumMerge;

			if (!iAmount)
			{
				*piLastSlot	  = i;
				*piSlotAmount = pSlotItem->GetCount();
				return true;
			}
		}
		else if (iFirstEmpty < 0)
			iFirstEmpty = i;
	}

	if (iFirstEmpty < 0 || !iAmount)
	{
		ASSERT(iFirstEmpty >= 0 && iAmount);
		return false;
	}

	CECIvtrItem* pItem = CECIvtrItem::CreateItem(tid, iExpireDate, iAmount);
	pItem->SetStateFlag(state);
	pItem->UpdateInfoMD5();

	m_aItems[iFirstEmpty] = pItem;

	*piLastSlot	  = iFirstEmpty;
	*piSlotAmount = iAmount;

	return true;
}

bool CECInventory::MergeHomeTownItem(int tid, int state, int iAmount, int* piLastSlot, int* piSlotAmount)
{
	int iFirstEmpty = -1;

	for (int i=0; i < m_aItems.GetSize(); i++)
	{
		CECIvtrItem* pSlotItem = m_aItems[i];
		if (pSlotItem)
		{
			int iNumMerge = pSlotItem->MergeItem(tid, state, iAmount);
			iAmount -= iNumMerge;

			if (!iAmount)
			{
				*piLastSlot	  = i;
				*piSlotAmount = pSlotItem->GetCount();
				return true;
			}
		}
		else if (iFirstEmpty < 0)
			iFirstEmpty = i;
	}

	if (iFirstEmpty < 0 || !iAmount)
	{
		ASSERT(iFirstEmpty >= 0 && iAmount);
		return false;
	}

	CECIvtrItem* pItem = CECIvtrItem::CreateHomeTownItem(tid, iAmount);
	m_aItems[iFirstEmpty] = pItem;

	*piLastSlot	  = iFirstEmpty;
	*piSlotAmount = iAmount;

	return true;
}

//	Reset all inventory items
bool CECInventory::ResetItems(const S2C::cmd_own_ivtr_info& Info)
{
	//	Release all old items
	RemoveAllItems();

	if (m_aItems.GetSize() != Info.ivtr_size)
		Resize(Info.ivtr_size);

	if (!Info.content_length)
	{
		ASSERT(Info.content_length);
		return false;
	}

	try
	{
		CECDataReader dr(Info.content, Info.content_length);

		for (int i=0; i < m_aItems.GetSize(); i++)
		{
			int tidItem = dr.Read_int();
			if (tidItem < 0)
			{
				SetItem(i, NULL);
			}
			else
			{
				int expire_date = dr.Read_int();
				int state = dr.Read_int();
				int iAmount = dr.Read_int();
				if (iAmount > 0)
				{
					//	TODO: Check if every item needs to be updated
					CECIvtrItem* pItem = CECIvtrItem::CreateItem(tidItem, expire_date, iAmount);
					pItem->SetStateFlag(state);
					pItem->UpdateInfoMD5();
					SetItem(i, pItem);
				}
			}
		}
	}

	catch (CECException& e)
	{
		ASSERT(0);
		a_LogOutput(1, "CECInventory::ResetItems, data read error (%d)", e.GetType());
		return false;
	}

	return true;
}

bool CECInventory::ResetItems(const S2C::cmd_own_ivtr_detail_info& Info)
{
	return ResetWithDetailData(Info.ivtr_size, Info.content, Info.content_length);
}

bool CECInventory::ResetItems(const S2C::repurchase_inv_data& Info)
{
	return ResetWithDetailData(Info.ivtr_size, Info.content, Info.content_length);
}

bool CECInventory::ResetItems(const HOMETOWN::pocket_info& info)
{
	RemoveAllItems();
	if (info.capacity > 0)
	{
		if (m_aItems.GetSize() != info.capacity)
			Resize(info.capacity);
	}

	for (int i = 0; i< (int) info.items.size(); ++i)
	{
		const HOMETOWN::item_data& data = info.items[i];
		CECIvtrItem* pItem = CECIvtrItem::CreateHomeTownItem(data.id, data.count);
		if(pItem)
		{
			if(data.content.size())
				pItem->SetItemInfo((BYTE*)data.content.begin(), data.content.size());
			else
				pItem->SetItemInfo(NULL, 0);
			
			pItem->UpdateInfoMD5();
			SetItem(data.pos, pItem);
		}

	}
	return true;
}

//	Reset inventory with detail item data
bool CECInventory::ResetWithDetailData(int iIvtrSize, void* pData, int iDataLen)
{
	//	Release all old items
	RemoveAllItems();

	if (iIvtrSize > 0)
	{
		if (m_aItems.GetSize() != iIvtrSize)
			Resize(iIvtrSize);
	}

	if (!iDataLen)
	{
		ASSERT(iDataLen);
		return true;
	}

	try
	{
		CECDataReader dr(pData, iDataLen);

		int iNumItem = dr.Read_int();
		for (int i=0; i < iNumItem; i++)
		{
			int iIndex = dr.Read_int();
			if (iIndex < 0)
				continue;

			int tid			= dr.Read_int();
			int expire_date = dr.Read_int();
			int state		= dr.Read_int();
			int iAmount		= dr.Read_int();
			WORD wDataLen	= dr.Read_WORD();

			BYTE* pData = NULL;
			if (wDataLen)
				pData = (BYTE*)dr.Read_Data(wDataLen);

			CECIvtrItem* pItem = CECIvtrItem::CreateItem(tid, expire_date, iAmount);
			if (pItem)
			{
				pItem->SetStateFlag(state);

				if (pData)
					pItem->SetItemInfo(pData, (int)wDataLen);
				else
				{
					// clear need update flag
					((CECIvtrItem *)pItem)->SetItemInfo(NULL, 0);
				}
				pItem->UpdateInfoMD5();
				SetItem(iIndex, pItem);
			}
		}
	}

	catch (CECException& e)
	{
		ASSERT(0);
		a_LogOutput(1, "CECInventory::ResetItemsWithDetailData, data read error (%d)", e.GetType());
		return false;
	}

	return true;
}

GNET::GamedataSend CECInventory::gen_item_detail_data(unsigned char pack_id)
{
	using namespace S2C;

	GamedataSend gds;
	GNET::Octets& o = gds.data;
	o.reserve(1024);
	cmd_header h;
	h.cmd = OWN_IVTR_DETAIL_DATA;
	add_to_octets<cmd_header>(o, h);

	add_to_octets<unsigned char>(o, pack_id);
	add_to_octets<unsigned char>(o, m_aItems.GetSize());
	int offset = o.size();
	size_t length = 0;
	add_to_octets<size_t>(o, length);
	add_to_octets<int>(o, m_aItems.GetSize());

	for (int i = 0; i < m_aItems.GetSize(); i++)
	{
		CECIvtrItem* pItem = m_aItems[i];

		if (pItem)
		{
			add_to_octets<int>(o, i);
			add_to_octets<int>(o, pItem->GetTemplateID());
			add_to_octets<int>(o, pItem->GetExpireDate());
			add_to_octets<int>(o, pItem->GetStateFlag());
			add_to_octets<int>(o, pItem->GetCount());
			GNET::Octets o1 = pItem->gen_item_info();
			add_to_octets<unsigned short>(o, o1.size());

			if (o1.size())
				o.insert(o.end(), o1.begin(), o1.size());
		}
		else
			add_to_octets<int>(o, -1);
	}

	length = o.size() - offset - sizeof(size_t);
	memcpy((char*)o.begin() + offset, &length, sizeof(length));
	return gds;
}

//	Move item to another slot which contains the same kind of item
bool CECInventory::MoveItem(int iSrc, int iDest, int iAmount)
{
	if (iSrc < 0 || iSrc >= m_aItems.GetSize() ||
		iDest < 0 || iDest >= m_aItems.GetSize())
	{
		ASSERT(0);
		return false;
	}

	CECIvtrItem* pSrcItem = m_aItems[iSrc];
	CECIvtrItem* pDstItem = m_aItems[iDest];

	if (!pSrcItem)
	{
		ASSERT(pSrcItem);
		return false;
	}

	if (!pDstItem)
	{
	//	pDstItem = CECIvtrItem::CreateItem(pSrcItem->GetTemplateID(), 0, iAmount);
		pDstItem = pSrcItem->Clone();
		pDstItem->SetCount(iAmount);
		SetItem(iDest, pDstItem);
	}
	else
	{
		ASSERT(pSrcItem->GetTemplateID() == pDstItem->GetTemplateID());
		int iNumAdded = pDstItem->MergeItem(pSrcItem->GetTemplateID(), pSrcItem->GetStateFlag(), iAmount);
		ASSERT(iNumAdded == iAmount);
	}

	RemoveItem(iSrc, iAmount);

	return true;
}

//	Remove item from invertory
bool CECInventory::RemoveItem(int iSlot, int iAmount)
{
	if (iSlot < 0 || iSlot >= m_aItems.GetSize())
	{
		ASSERT(0);
		return false;
	}

	CECIvtrItem* pItem = m_aItems[iSlot];
	if (!pItem)
		return true;

	pItem->AddAmount(-iAmount);

	if (pItem->GetCount() <= 0)
		SetItem(iSlot, NULL);	//	Remove all items

	return true;
}

//	Remove all items
void CECInventory::RemoveAllItems()
{
	//	Release all items
	for (int i=0; i < m_aItems.GetSize(); i++)
	{
		if (m_aItems[i])
		{
			delete m_aItems[i];
			m_aItems[i] = NULL;
		}
	}
}

//	Unfreeze all items
void CECInventory::UnfreezeAllItems()
{
	//	Release all items
	for (int i=0; i < m_aItems.GetSize(); i++)
	{
		if (m_aItems[i])
			m_aItems[i]->Freeze(false);
	}
}

//	Check whether this inventory has disrepair item
bool CECInventory::HasDisrepairItem()
{
	for (int i=0; i < m_aItems.GetSize(); i++)
	{
		CECIvtrItem* pItem = m_aItems[i];
		if (pItem && pItem->IsEquipment())
		{
			if (((CECIvtrEquip*)pItem)->NeedRepair())
				return true;
		}
	}

	return false;
}

//	Get cost of repairing all items
int CECInventory::GetRepairAllItemCost()
{
	float fCost = 0.0f;

	for (int i=0; i < m_aItems.GetSize(); i++)
	{
		CECIvtrItem* pItem = m_aItems[i];
		if (pItem && pItem->IsEquipment())
			fCost += ((CECIvtrEquip*)pItem)->GetRawRepairCost();
	}

	int iCost = (int)fCost;
	return iCost;
}

//	Repair all items
void CECInventory::RepairAllItems()
{
	for (int i=0; i < m_aItems.GetSize(); i++)
	{
		CECIvtrItem* pItem = m_aItems[i];
		if (pItem && pItem->IsEquipment())
			((CECIvtrEquip*)pItem)->Repair();
	}
}

//	Find an item with specified id
//	Return item's position
int CECInventory::FindItem(int idItem)
{
	for (int i=0; i < m_aItems.GetSize(); i++)
	{
		CECIvtrItem* pItem = m_aItems[i];
		if (pItem && pItem->GetTemplateID() == idItem)
			return i;
	}

	return -1;
}

//	Get total number of specified item
int CECInventory::GetItemTotalNum(int idItem, bool bPermanentOnly)
{
	int iCount = 0;

	for (int i=0; i < m_aItems.GetSize(); i++)
	{
		CECIvtrItem* pItem = m_aItems[i];
		if (pItem && pItem->GetTemplateID() == idItem)
		{
			if (!bPermanentOnly || pItem->GetExpireDate()==0)
			{
				iCount += pItem->GetCount();
			}
		}
	}

	return iCount;
}

//	Search a empty slot
int CECInventory::SearchEmpty()
{
	for (int i=0; i < m_aItems.GetSize(); i++)
	{
		if (!m_aItems[i])
			return i;
	}

	return -1;
}

//	Search a empty slot in specific range
int CECInventory::SearchEmpty(int iBegin, int iEnd)
{
	if (iBegin < 0 || iBegin >= m_aItems.GetSize() || iBegin > iEnd)
	{
		return -1;
	}
	if(iEnd >= m_aItems.GetSize())
		iEnd = m_aItems.GetSize() - 1;
	for(int i = iBegin; i <= iEnd; i++)
	{
		if(!m_aItems[i])
			return i;
	}
	
	return -1;
}

//	Check whether ONE item can be put into this inventory or not
bool CECInventory::CanAddOneItem(int idItem)
{
	for (int i=0; i < m_aItems.GetSize(); i++)
	{
		CECIvtrItem* pItem = m_aItems[i];
		if (!pItem)
			return true;

		if (pItem->GetTemplateID() == idItem && pItem->GetCount() + 1 <= pItem->GetPileLimit())
			return true;
	}

	return false;
}

//	Get empty slots number
int CECInventory::GetEmptySlotNum()
{
	int i, iCount=0;

	for (i=0; i < m_aItems.GetSize(); i++)
	{
		if (!m_aItems[i])
			iCount++;
	}

	return iCount;
}

//	Get empty slots number in specific range
int CECInventory::GetEmptySlotNum(int iBegin, int iEnd)
{
	if(iBegin < 0 || iBegin >= m_aItems.GetSize() || iBegin > iEnd)
	{
		return 0;
	}
	if(iEnd >= m_aItems.GetSize())
		iEnd = m_aItems.GetSize() - 1;

	int i, iCount = 0;
	for(i = iBegin; i <= iEnd; i++)
	{
		if(!m_aItems[i])
			iCount++;
	}

	return iCount;
}



