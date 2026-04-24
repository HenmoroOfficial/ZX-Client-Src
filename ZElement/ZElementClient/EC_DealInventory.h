/*
 * FILE: EC_DealInventory.h
 *
 * DESCRIPTION: 
 *
 * CREATED BY: Duyuxin, 2004/12/22
 *
 * HISTORY: 
 *
 * Copyright (c) 2004 Archosaur Studio, All Rights Reserved.
 */

#pragma once

#include "EC_Inventory.h"
#include "AArray.h"

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

///////////////////////////////////////////////////////////////////////////
//	
//	Declare of Global functions
//	
///////////////////////////////////////////////////////////////////////////


///////////////////////////////////////////////////////////////////////////
//	
//	Class CECDealInventory
//	
///////////////////////////////////////////////////////////////////////////

class CECDealInventory
{
public:		//	Types

	//	Item extra information
	struct ITEMINFO
	{
		int		iOrigin;	//	Original position in player's normal pack
		int		iAmount;	//	Amount
		bool	bDelete;	//	true, item object can be released
		int		iFlag;		//	Item flag
		
		unsigned int	repu_id_consume[3];		//	Used for reputation shop
		unsigned int	repu_consume_value[3];
	};

public:		//	Constructor and Destructor

	CECDealInventory();
	virtual ~CECDealInventory();

public:		//	Attributes

public:		//	Operations

	//	Initalize object
	bool Init(int iSize);
	//	Release object
	void Release();
	//	Remove all items
	void RemoveAllItems();
	//	Resize inventory
	void Resize(int iNewSize);

	//	Get a item from inventory
	CECIvtrItem* GetItem(int iSlot);
	//	Add an item. This function is used when trade with other players
	bool AddItem(CECIvtrItem* pItem, int iOrigin, int iAmount, bool bDelete=false);
	//	Add an item freely. This function is used when trade with NPCs
	int AddItemFree(CECIvtrItem* pItem, int iOrigin, int iAmount, unsigned int* repu_id = NULL, unsigned int* repu_consume = NULL);
	//	Add an booth item. This function is used when open booth
	int AddBoothItem(CECIvtrItem* pItem, int iOrigin, int iAmount, int iUnitPrice);
	//	Set an booth item. This function is used when visite else player's booth
	bool AddEPBoothItem(int idItem, int iExpireDate, int iState, int iOrigin, int iAmount, int iUnitPrice, BYTE* pData, int iDataLen);
	//	Remove an item
	void RemoveItem(int iOrigin, int iAmount);
	void RemoveItemByFlag(int iFlag, int iAmount);
	void RemoveItemByIndex(int iIndex, int iAmount);
	//	Get item's index in inventory according to item's original position
	int GetItemIndex(int iOrigin);
	//	Get item's index through item's flag
	int GetItemIndexByFlag(int iFlag);
	//	Set flag in item info.
	void SetItemInfoFlag(int iIndex, int iFlag);
	//	Unfreeze all items
	void UnfreezeAllItems();
	//	Search a empty slot
	int SearchEmpty();
	//	Get empty slots number
	int GetEmptySlotNum();

	//	Add money
	int AddMoney(int iMoney) { m_iMoney += iMoney; return m_iMoney; }
	//	Get money
	int GetMoney() { return m_iMoney; }
	//	Get item extra information
	const ITEMINFO& GetItemInfo(int n) { return m_aItemInfo[n]; }
	//	Get inventory size
	int GetSize() { return m_aItems.GetSize(); }

protected:	//	Attributes

	int		m_iMoney;

	APtrArray<CECIvtrItem*>		m_aItems;		//	Item object array
	AArray<ITEMINFO, ITEMINFO&>	m_aItemInfo;	//	Item extra information

protected:	//	Operations

};

///////////////////////////////////////////////////////////////////////////
//	
//	Inline functions
//	
///////////////////////////////////////////////////////////////////////////

