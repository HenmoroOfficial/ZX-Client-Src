/*
 * FILE: EC_Inventory.h
 *
 * DESCRIPTION: 
 *
 * CREATED BY: Duyuxin, 2004/11/18
 *
 * HISTORY: 
 *
 * Copyright (c) 2004 Archosaur Studio, All Rights Reserved.
 */

#pragma once

#include "AArray.h"
#include "AAssist.h"

///////////////////////////////////////////////////////////////////////////
//	
//	Define and Macro
//	
///////////////////////////////////////////////////////////////////////////


///////////////////////////////////////////////////////////////////////////
//	
//	Types and Global variables
//	
///////////////////////////////////////////////////////////////////////////

class CECIvtrItem;

namespace S2C
{
	struct cmd_own_ivtr_info;
	struct cmd_own_ivtr_detail_info;
	struct repurchase_inv_data;
}

namespace GNET
{
	class Octets;
	class GamedataSend;
}

namespace HOMETOWN
{
	struct pocket_info;
}
///////////////////////////////////////////////////////////////////////////
//	
//	Declare of Global functions
//	
///////////////////////////////////////////////////////////////////////////


///////////////////////////////////////////////////////////////////////////
//	
//	Class CECInventory
//	
///////////////////////////////////////////////////////////////////////////

class CECInventory
{
public:		//	Types

public:		//	Constructor and Destructor

	CECInventory();
	virtual ~CECInventory();

public:		//	Attributes

public:		//	Operations

	//	Initalize object
	virtual bool Init(int iSize);
	//	Release object
	virtual void Release();
	//	Remove all items
	virtual void RemoveAllItems();
	//	Resize inventory
	virtual void Resize(int iNewSize);

	//	Put a item into inventory
	CECIvtrItem* PutItem(int iSlot, CECIvtrItem* pItem);
	//	Get a item from inventory
	CECIvtrItem* GetItem(int iSlot, bool bRemove=false);
	//	Set a item into inventory and release old item at this position automatically
	void SetItem(int iSlot, CECIvtrItem* pItem);
	//	Put a item into inventory, merge it with same kind item when necessary
	bool MergeItem(int tid, int state, int iExpireDate, int iAmount, int* piLastSlot, int* piLastAmount);
	//  Put a home town item into inventory, merge it with same kind item when necessary
	bool MergeHomeTownItem(int tid, int state, int iAmount, int* piLastSlot, int* piLastAmount);
	//	Exchange item in inventory
	void ExchangeItem(int iSlot1, int iSlot2);
	//	Reset all inventory items
	bool ResetItems(const S2C::cmd_own_ivtr_info& Info);
	bool ResetItems(const S2C::cmd_own_ivtr_detail_info& Info);
	bool ResetItems(const S2C::repurchase_inv_data& Info);
	bool ResetItems(const HOMETOWN::pocket_info& info);
	bool ResetWithDetailData(int iIvtrSize, void* pData, int iDataLen);
	GNET::GamedataSend gen_item_detail_data(unsigned char pack_id);
	//	Move item to another slot which contains the same kind of item
	bool MoveItem(int iSrc, int iDest, int iAmount);
	//	Remove item from invertory
	bool RemoveItem(int iSlot, int iAmount);
	//	Find an item with specified id
	int FindItem(int idItem);
	//	Get total number of specified item
	int GetItemTotalNum(int idItem, bool bPermanentOnly=false);
	//	Search a empty slot
	int SearchEmpty();
	//  Search a empty slot in specific range
	int SearchEmpty(int iBegin, int iEnd);
	//	Get empty slots number
	int GetEmptySlotNum();
	//	Get empty slots number in specific range
	int GetEmptySlotNum(int iBegin, int iEnd);
	//	Check whether ONE item can be put into this inventory or not
	bool CanAddOneItem(int idItem);

	//	Unfreeze all items
	void UnfreezeAllItems();
	//	Get cost of repairing all items
	int GetRepairAllItemCost();
	//	Repair all items
	void RepairAllItems();
	//	Check whether this inventory has disrepair item
	bool HasDisrepairItem();

	//	Get inventory size
	int GetSize() { return m_aItems.GetSize(); }

protected:	//	Attributes

	APtrArray<CECIvtrItem*>	m_aItems;	//	Item array

protected:	//	Operations

};

///////////////////////////////////////////////////////////////////////////
//	
//	Class CECNPCInventory
//	
///////////////////////////////////////////////////////////////////////////

class CECNPCInventory : public CECInventory
{
public:		//	Types

public:		//	Constructor and Destructor

	CECNPCInventory() {}
	virtual ~CECNPCInventory() {}

public:		//	Attributes

public:		//	Operations

	//	Set inventory name
	void SetName(const ACHAR* szName) { m_strName = szName; }
	//	Get inventory name
	const ACHAR* GetName() { return m_strName; }

protected:	//	Attributes

	ACString	m_strName;		//	Inventory name

protected:	//	Operations

};

///////////////////////////////////////////////////////////////////////////
//	
//	Inline functions
//	
///////////////////////////////////////////////////////////////////////////



struct ReputationItem
{
	unsigned int	id_goods;	
	struct
	{
		unsigned int	repu_id_req;
		unsigned int	repu_req_value;
		unsigned int	repu_id_consume;
		unsigned int	repu_consume_value;
	} repu_required_pre[3];
};
///////////////////////////////////////////////////////////////////////////
//	
//	Class CECNPCInventory
//	
///////////////////////////////////////////////////////////////////////////

class CECReputationInventory : public CECInventory
{
public:		//	Types

public:		//	Constructor and Destructor

	CECReputationInventory() {}
	virtual ~CECReputationInventory() {}

public:		//	Attributes

public:		//	Operations

	//	Set inventory name
	void SetName(const ACHAR* szName) { m_strName = szName; }
	//	Get inventory name
	const ACHAR* GetName() { return m_strName; }
	struct ReputationItem goods[48];
protected:	//	Attributes

	ACString	m_strName;		//	Inventory name
protected:	//	Operations

};

