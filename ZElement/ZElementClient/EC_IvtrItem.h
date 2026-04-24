/*
 * FILE: EC_IvtrItem.h
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

#include "AWString.h"
#include "AAssist.h"

///////////////////////////////////////////////////////////////////////////
//	
//	Define and Macro
//	
///////////////////////////////////////////////////////////////////////////

#define ITEM_STATE_BINDED			0x1
#define ITEM_STATE_BINDAFTEREQUIP	0x2
#define ITEM_STATE_LOCKED			0x4

namespace GNET
{
	class Octets;
}

///////////////////////////////////////////////////////////////////////////
//	
//	Types and Global variables
//	
///////////////////////////////////////////////////////////////////////////


///////////////////////////////////////////////////////////////////////////
//	
//	Declare of Global functions
//	
///////////////////////////////////////////////////////////////////////////


///////////////////////////////////////////////////////////////////////////
//	
//	Class CECIvtrItem
//	
///////////////////////////////////////////////////////////////////////////

class CECInventory;
class CECStringTab;

class CECIvtrItem
{
public:		//	Types

	//	Inventory item class ID
	enum
	{
		ICID_ITEM	= -100,
		ICID_EQUIP	= 0,
		ICID_MATERIAL,
		ICID_MEDICINE,
		ICID_REVSCROLL,
		ICID_SKILLTOME,
		ICID_TOWNSCROLL,
		ICID_TASKITEM,
		ICID_ESTONE,
		ICID_PSTONE,
		ICID_SSTONE,
		ICID_TASKDICE,
		ICID_TASKNMMATTER,
		ICID_ERRORITEM,
		ICID_GM_GENERATOR,
		ICID_RECIPE,
		ICID_FIREWORK,
		ICID_REFINETICKET,
		ICID_RECIPEROLL,
		ICID_TRANSMITROLL,
		ICID_LUCKYROLL,
		ICID_DOUBLEEXP,
		ICID_DESTROYINGESSENCE,
		ICID_SKILLMATTER,
		ICID_VEHICLE,
		ICID_COUPLE_JUMPTO,
		ICID_LOTTERY,
		ICID_CAMRECORDER,
		ICID_TEXT_FIREWORK,
		ICID_TALISMAN_MAINPART,
		ICID_TALISMAN_EXPFOOD,
		ICID_TALISMAN_MERGEKATALYST,
		ICID_TALISMAN_ENERGYFOOD,
		ICID_SPAKER,
		ICID_ARTILLERY_SCROLL,
		ICID_PET_BEDGE,
		ICID_PET_FOOD,
		ICID_PET_SKILL,
		ICID_PET_ARMOR,
		ICID_PET_AUTO_FOOD,
		ICID_PET_REFINE,
		ICID_PET_ASSIST_REFINE,
		ICID_BOOK,
		ICID_OFFLINE_TRUSTEE,
		ICID_EQUIP_SOUL,
		ICID_SPECIAL_NAME_ITEM,
		ICID_GIFT_BAG,
		ICID_VIP_CARD,
		ICID_CHANGE_SHAPE_CARD,
		ICID_CHANGE_SHAPE_STONE,
		ICID_HOMETOWN_TOOL,
		ICID_HOMETOWN_FRUIT,
		ICID_MERCENARY_CREDENTIAL,
		ICID_TELEPORTATION,
		ICID_TELEPORTATION_STONE,
		ICID_UPGRADE_EQUIP_STONE,
		ICID_LOTTERY2,
		ICID_GEM,//Added 2011-03-15.
		ICID_GEM_SEAL,
		ICID_GEM_DUST,
		ICID_GENERAL_ARTICLE,	//Added 2011-03-17.
		ICID_LOTTERY3,			//Added 2011-07-27.
		ICID_SMALL_ONLINE_GIFT_BAG,	//Added 2012-03-23.
		ICID_SCROLL_UNLOCK,		//Added 2012-03-23.
		ICID_SCROLL_DIG_COUNT_INC,	//Added 2012-03-23.
		ICID_RUNE,					// 装备元魂
		ICID_RUNE_SLOT,				// 元魂符文
		ICID_BOOTH_FIGURE,
		ICID_FLAG_BUFF,
		ICID_ASTROLOGY,				// 星座
		ICID_ASTROLOGY_ENERGY_ITEM,	// 星座精力石
		ICID_AGENTCOMBINED,			// 药剂合并
		ICID_GIFT_BAG_LOTTERY_DELIVER,	// 彩票赠送的礼包
		ICID_LOTTERY_TANGYUAN,	// 汤圆彩票
		ICID_SALE_PROMOTION_ITEM,	// 台历道具
		ICID_GIFT_PACK,
		ICID_PROP_ADD_MATERIAL_ITEM,	// 基础属性增益丹药合成材料
		ICID_PROP_ADD_ITEM,				// 基础属性增益丹药
		ICID_RUNE2013_ITEM,				// 150-160级符文
		ICID_INDIVIDUALITY_ITEM,				// 个性合并物品
	};

	//	Item price scale type
	enum
	{
		SCALE_BUY = 1,	//	Buy from NPC
		SCALE_SELL,		//	Sell to NPC
		SCALE_BOOTH,	//	Booth item
		SCALE_MAKE,		//	Make item
	};
	
	//	Description type
	enum
	{
		DESC_NORMAL = 0,
		DESC_BOOTHBUY,
		DESC_REPAIR,
		DESC_REWARD,
	};
	
	//	Item use conditions
	enum
	{
		USE_ATKTARGET	= 0x0001,	//	Attack target
		USE_PERSIST		= 0x0002,	//	Persist some time
		USE_TARGET		= 0x0004,	//	Normal target
	};

	//	Proc-type
	enum
	{
		PROC_DROPWHENDIE	= 0x0001,	//	死亡时是否掉落
		PROC_DROPPABLE		= 0x0002,	//	是否可以扔在地上
		PROC_SELLABLE		= 0x0004,	//	是否可以卖给NPC
		PROC_LOG			= 0x0008,	//	记录详细Log信息
		PROC_TRADEABLE		= 0x0010,	//	是否可以玩家间交易
		PROC_TASK			= 0x0020,	//	是否任务相关物品
		PROC_EQUIPBIND		= 0x0040,	//  是否装备绑定
		PROC_RESERVED		= 0x0080,	//  预留
		PROC_GUID			= 0x0100,   //  是否产生GUID
		PROC_RECYCLE		= 0x0200,	//  是否可以回收（战场中使用）
		PROC_CROSS_TRADEABLE= 0x0400,	//  跨服可以交易
		PROC_CROSS_ONLY		= 0x0800,	//  仅在跨服可用
		PROC_CROSS_FORBID	= 0x1000,	//  跨服不可使用
		PROC_MOVE_STORAGE   = 0x2000,   //  不可放入仓库
	};
	
	enum
	{
		HINT_GEM_SLOTSHAPE1 = 5,
	};

public:		//	Constructor and Destructor

	CECIvtrItem(int tid, int expire_date);
	CECIvtrItem(const CECIvtrItem& s);
	virtual ~CECIvtrItem() {}

public:		//	Attributes

public:		//	Operations

	//	Create an inventory item
	static CECIvtrItem* CreateItem(int tid, int expire_date, int iCount, int idSpace=0);
	//	Create item from text
	static CECIvtrItem* Unserialize(const ACHAR* szText);
	//	Check whether item2 is item1's candidate
	static bool IsCandidate(int tid1, int tid2);
	static bool IsCandidate(int tid1, CECIvtrItem* pItem2, bool bFirstCandidate = true);
	//	Get scaled price of specified count item
	static INT64 GetScaledPrice(int iUnitPrice, int iCount, int iScaleType, float fScale);
	//  Create an inventory item, only for home town
	static CECIvtrItem* CreateHomeTownItem(int tid, int iCount, int idSpace=0);

	//	Set item detail information
	virtual bool SetItemInfo(BYTE* pInfoData, int iDataLen);
	//	Set item info to empty things, this will be called when MsgHstOwnItemInfo return with NULL content or zero content_length
	virtual bool SetEmptyInfo() { m_bNeedUpdate = false; return true; }

	//	Get item default information from database
	virtual void DefaultInfo() {}
	//	Get item icon file name
	virtual const char* GetIconFile();
	//	Get item name
	virtual const wchar_t* GetName();
	//	Use item
	virtual bool Use() { return true; }
	//	Get scaled price of one item
	virtual INT64 GetScaledPrice();
	//	Clone item
	virtual CECIvtrItem* Clone() { return new CECIvtrItem(*this); }
	//	Get item cool time
	virtual int GetCoolTime(int* piMax=NULL) { return 0; }
	//	Check item use condition
	virtual bool CheckUseCondition() { return IsUseable() && CheckCrossUseCondition(); }
	//	Get drop model for shown
	virtual const char * GetDropModel();

	//	Get item description text
	const wchar_t* GetDesc(int iDescType=DESC_NORMAL, CECInventory* pInventory=NULL);
	//	Get item name string with color
	AWString GetColor();

	//	Merge item amount with another same kind item
	int MergeItem(int tid, int state, int iAmount);
	//	Add item amount
	int AddAmount(int iAmount);
	//	Set amount
	void SetAmount(int iAmount) { m_iCount = iAmount; }
	//	Set expire date
	void SetExpireDate(int iExpireDate) { m_expire_date = iExpireDate; }
		//	Set state flag
	void SetStateFlag(int iState) { m_iState = iState; }
	//	Get state flag
	int GetStateFlag() const { return m_iState; }

	//	Can this item be equipped to specified position ?
	bool CanEquippedTo(int iSlot) const { return (m_iEquipMask & (1 << iSlot)) ? true : false; }

	//	Set / Get item properties
	int GetClassID() const { return m_iCID; }
	int GetTemplateID() const { return m_tid; }
	int GetExpireDate() const { return m_expire_date; }
	int GetCount() const { return m_iCount; }
	void SetCount(int iCount) { m_iCount = iCount; }
	int GetPileLimit() const { return m_iPileLimit; }
	int GetUnitPrice() const { return m_iPrice; }
	void SetUnitPrice(int iPrice) { m_iPrice = iPrice; }
	int GetShopPrice() const { return m_iShopPrice; }
	int GetEquipMask() const { return m_iEquipMask; }
	float GetPriceScale() const { return m_fPriceScale; }
	void SetPriceScale(int iType, float fScale) { m_iScaleType = iType; m_fPriceScale = fScale; }

	bool IsEmbeddable() const { return m_bEmbeddable; }
	bool IsUseable() const { return m_bMD5OK ? m_bUseable : false; }
	bool IsEquipment() const { return m_bMD5OK ? (m_iEquipMask ? true : false) : false; }
	bool IsFrozen() const { return m_bMD5OK ? (m_bFrozen || m_bNetFrozen) : true; }
	bool IsTradeable() const { 
		if( !m_bMD5OK ) return false;
		return ((m_iProcType & PROC_TRADEABLE) || 
			(GetStateFlag() & ITEM_STATE_BINDED)) ? false : true; }
	bool IsSellable() const { 
		if( !m_bMD5OK ) return false;
		return ((m_iProcType & PROC_SELLABLE)) ? false : true; }
	bool IsDropable() const {
		if(!m_bMD5OK) return false;
		return ((m_iProcType& PROC_DROPPABLE)|| 
			(GetStateFlag() & ITEM_STATE_BINDED))? false: true; }

	// 在跨服中可交易
	bool IsCrossTradeable() const
	{
		if (!m_bMD5OK) return false;
		return (m_iProcType & PROC_CROSS_TRADEABLE)? true:false; 
	}
	// 仅在跨服服务器可用
	bool IsCrossOnly() const
	{
		if (!m_bMD5OK) return false;
		return (m_iProcType & PROC_CROSS_ONLY) ? true:false;
	}
	// 跨服服务器禁用
	bool IsCrossForbid() const
	{
		if (!m_bMD5OK) return false;
		return (m_iProcType & PROC_CROSS_FORBID)? true:false;
	}
	
	bool CheckCrossUseCondition() const;
	
	bool IsLocked() const { return (GetStateFlag() & ITEM_STATE_LOCKED) ? true : false; }

	//	Does item data needs to be updated ?
	bool NeedUpdate() { return m_bNeedUpdate; }
	void SetNeedUpdate(bool bFlag) { m_bNeedUpdate = bFlag; }

	//	Get item's detail data from server
	void GetDetailDataFromSev(int iPack, int iSlot);
	//	Get item's detail data from local database
	void GetDetailDataFromLocal();
	//	Set local frozen flag
	void Freeze(bool bFreeze) { m_bFrozen = bFreeze; }
	//	Set net frozen flag
	void NetFreeze(bool bFreeze) { m_bNetFrozen = bFreeze; }

	//	Get use condition flags
	DWORD GetUseFlags() { return m_dwUseFlags; }
	bool Use_AtkTarget() { return (m_dwUseFlags & USE_ATKTARGET) ? true : false; }
	bool Use_Persist() { return (m_dwUseFlags & USE_PERSIST) ? true : false; }
	bool Use_Target() { return (m_dwUseFlags & USE_TARGET) ? true : false; }

	static void GetPriceText(ACHAR szText[256], INT64 nPrice);
	ACString Serialize() const;
	virtual GNET::Octets gen_item_info();

	//	Update item info's md5
	void UpdateInfoMD5();
	//	Verify if md5 of this item is valid, return true if still valid and false if invalid
	bool VerifyInfoMD5();
	//  this item Can move to trash box
	bool CanMoveToTrash();

	//Added by Ford.W 2010-08-10.
	//for mini tool of xunbao173.
	virtual int GetEstone(){ return -1; };
	//Added end.

protected:	//	Attributes

	int			m_iCID;			//	Class ID
	int			m_tid;			//	Template id
	int			m_expire_date;	//	Expiration date
	int			m_iState;		//	Item state flags
	int			m_iCount;		//	Item count
	int			m_iPileLimit;	//	Pile limit number
	int			m_iPrice;		//	Item unit price
	int			m_iShopPrice;	//	Shop price
	int			m_iEquipMask;	//	Equip mask
	bool		m_bEmbeddable;	//	true, embeddable item
	bool		m_bUseable;		//	true, item can be use
	bool		m_bFrozen;		//	Frozen flag set by local reason (cannot be moved, equipped and used)
	bool		m_bNetFrozen;	//	Frozen flag set by net reason (cannot be moved, equipped and used)
	DWORD		m_dwUseFlags;	//	Use condition flags
	int			m_iProcType;	//	proc-type flag

	int			m_iScaleType;	//	Item price scale type
	float		m_fPriceScale;	//	Price scale

	bool		m_bNeedUpdate;	//	true, detail data needs to updated
	bool		m_bUpdating;	//	true, being updating detail data
	DWORD		m_dwUptTime;	//	Time when updating request was sent
	AWString	m_strDesc;		//	Item description

	BYTE		m_infoMD5[16];	//	Item info's md5
	bool		m_bMD5OK;		//	flag indicates whether or not md5 is verified ok

	CECInventory*	m_pDescIvtr;	//	Inventory only used to get item description

protected:	//	Operations

	//	Get item description text
	virtual const wchar_t* GetNormalDesc(bool bRepair) { return m_strDesc.GetLength() ? m_strDesc : NULL; }
	//	Get item description text for booth buying
	virtual const wchar_t* GetBoothBuyDesc() { return GetNormalDesc(false); }
	//	Get item description text for rewarding
	virtual const wchar_t* GetRewardDesc();
	//	Add price description
	virtual void AddPriceDesc(int col, bool bRepair);
	//	Add profession requirment description
	virtual void AddProfReqDesc(UINT64 iProfReq, UINT64 iProfReq2 ); // 职业ID扩展后需要两个64位变量Modified 2011-07-18.
	//	Get item name color
	virtual int DecideNameCol();

	//	Add content to description string
	void AddDescText(int iCol, bool bRet, const ACHAR* szText, ...);
	//  Add image to description string
	void AddDescImage(bool bRet, int iIndex, int iFrame);
	//	Add extent description to description string
	void AddExtDescText();
	//	Add expire time desc
	void AddExpireTimeDesc();
	//	Trim the last '\r' in description string
	void TrimLastReturn();
	//	Calculate Info md5
	void CalculateInfoMD5(BYTE md5[16]);
	//	Add state desc
	void AddStateDesc(CECStringTab* pDescTab);
	
	virtual ACString _Serialize() const { return ACString(); }
	virtual bool _Unserialize(const ACHAR* szText) { m_bNeedUpdate = false; return true; }
};

///////////////////////////////////////////////////////////////////////////
//	
//	Class CECIvtrUnknown
//	
///////////////////////////////////////////////////////////////////////////

class CECIvtrUnknown : public CECIvtrItem
{
public:		//	Types

public:		//	Constructor and Destructor

	CECIvtrUnknown(int tid);
	CECIvtrUnknown(const CECIvtrUnknown& s);
	virtual ~CECIvtrUnknown() {}

public:		//	Attributes

public:		//	Operations

	//	Get item icon file name
	virtual const char* GetIconFile();
	//	Get item name
	virtual const wchar_t* GetName();
	//	Clone item
	virtual CECIvtrItem* Clone() { return new CECIvtrUnknown(*this); }

protected:	//	Attributes

protected:	//	Operations

	//	Get item description text
	virtual const wchar_t* GetNormalDesc(bool bRepair);
};

///////////////////////////////////////////////////////////////////////////
//	
//	Inline functions
//	
///////////////////////////////////////////////////////////////////////////


