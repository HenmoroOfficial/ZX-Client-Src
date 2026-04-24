/*
 * FILE: EC_IvtrStones.h
 *
 * DESCRIPTION: 
 *
 * CREATED BY: Hedi, 2006/4/27
 *
 * HISTORY: 
 *
 * Copyright (c) 2005 Archosaur Studio, All Rights Reserved.
 */

#pragma once

#include "EC_IvtrItem.h"
#include "EC_IvtrTypes.h"

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

struct ESTONE_ESSENCE;
struct PSTONE_ESSENCE;
struct SSTONE_ESSENCE;
struct REFINE_TICKET_ESSENCE;
struct OFFLINE_TRUSTEE_ESSENCE;
struct CHANGE_SHAPE_STONE_ESSENCE;
struct GEM_ESSENCE;
struct GEM_SEAL_ESSENCE;
struct GEM_DUST_ESSENCE;
struct GENERAL_ARTICLE_ESSENCE;
struct SMALL_ONLINE_GIFT_BAG_ESSENCE;
struct SCROLL_UNLOCK_ESSENCE;
struct SCROLL_DIG_COUNT_INC_ESSENCE;
///////////////////////////////////////////////////////////////////////////
//	
//	Declare of Global functions
//	
///////////////////////////////////////////////////////////////////////////


///////////////////////////////////////////////////////////////////////////
//	
//	Class CECIvtrEStone
//	
///////////////////////////////////////////////////////////////////////////

class CECIvtrEStone : public CECIvtrItem
{
public:		//	Types

public:		//	Constructor and Destructor

	CECIvtrEStone(int tid, int expire_date);
	CECIvtrEStone(const CECIvtrEStone& s);
	virtual ~CECIvtrEStone();

public:		//	Attributes

public:		//	Operations

	//	Set item detail information
	virtual bool SetItemInfo(BYTE* pInfoData, int iDataLen);
	//	Get item icon file name
	virtual const char* GetIconFile();
	//	Get item name
	virtual const wchar_t* GetName();
	//	Clone item
	virtual CECIvtrItem* Clone() { return new CECIvtrEStone(*this); }
	//	Get drop model for shown
	virtual const char * GetDropModel();

	//	Get database data
	const ESTONE_ESSENCE* GetDBEssence() { return m_pDBEssence; }

protected:	//	Operations

	//	Data in database
	ESTONE_ESSENCE*		m_pDBEssence;

protected:	//	Operations

	//	Get item description text
	virtual const wchar_t* GetNormalDesc(bool bRepair);
};

///////////////////////////////////////////////////////////////////////////
//	
//	Class CECIvtrPStone
//	
///////////////////////////////////////////////////////////////////////////

class CECIvtrPStone : public CECIvtrItem
{
public:		//	Types

public:		//	Constructor and Destructor

	CECIvtrPStone(int tid, int expire_date);
	CECIvtrPStone(const CECIvtrPStone& s);
	virtual ~CECIvtrPStone();

public:		//	Attributes

public:		//	Operations

	//	Set item detail information
	virtual bool SetItemInfo(BYTE* pInfoData, int iDataLen);
	//	Get item icon file name
	virtual const char* GetIconFile();
	//	Get item name
	virtual const wchar_t* GetName();
	//	Clone item
	virtual CECIvtrItem* Clone() { return new CECIvtrPStone(*this); }
	//	Get drop model for shown
	virtual const char * GetDropModel();

	//	Get database data
	const PSTONE_ESSENCE* GetDBEssence() { return m_pDBEssence; }

protected:	//	Operations

	//	Data in database
	PSTONE_ESSENCE*		m_pDBEssence;

protected:	//	Operations

	//	Get item description text
	virtual const wchar_t* GetNormalDesc(bool bRepair);
};

///////////////////////////////////////////////////////////////////////////
//	
//	Class CECIvtrSStone
//	
///////////////////////////////////////////////////////////////////////////

class CECIvtrSStone : public CECIvtrItem
{
public:		//	Types

public:		//	Constructor and Destructor

	CECIvtrSStone(int tid, int expire_date);
	CECIvtrSStone(const CECIvtrSStone& s);
	virtual ~CECIvtrSStone();

public:		//	Attributes

public:		//	Operations

	//	Set item detail information
	virtual bool SetItemInfo(BYTE* pInfoData, int iDataLen);
	//	Get item icon file name
	virtual const char* GetIconFile();
	//	Get item name
	virtual const wchar_t* GetName();
	//	Clone item
	virtual CECIvtrItem* Clone() { return new CECIvtrSStone(*this); }
	//	Get drop model for shown
	virtual const char * GetDropModel();

	//	Get database data
	const SSTONE_ESSENCE* GetDBEssence() { return m_pDBEssence; }

protected:	//	Operations

	//	Data in database
	SSTONE_ESSENCE*		m_pDBEssence;

protected:	//	Operations

	//	Get item description text
	virtual const wchar_t* GetNormalDesc(bool bRepair);
};

///////////////////////////////////////////////////////////////////////////
//	
//	Class CECIvtrRefineTicket
//	
///////////////////////////////////////////////////////////////////////////

class CECIvtrRefineTicket : public CECIvtrItem
{
public:		//	Types

public:		//	Constructor and Destructor

	CECIvtrRefineTicket(int tid, int expire_date);
	CECIvtrRefineTicket(const CECIvtrRefineTicket& s);
	virtual ~CECIvtrRefineTicket();

public:		//	Attributes

public:		//	Operations

	//	Set item detail information
	virtual bool SetItemInfo(BYTE* pInfoData, int iDataLen);
	//	Get item icon file name
	virtual const char* GetIconFile();
	//	Get item name
	virtual const wchar_t* GetName();
	//	Clone item
	virtual CECIvtrItem* Clone() { return new CECIvtrRefineTicket(*this); }
	//	Get drop model for shown
	virtual const char * GetDropModel();

	//	Get database data
	const REFINE_TICKET_ESSENCE* GetDBEssence() { return m_pDBEssence; }

protected:	//	Operations

	//	Data in database
	REFINE_TICKET_ESSENCE*		m_pDBEssence;

protected:	//	Operations

	//	Get item description text
	virtual const wchar_t* GetNormalDesc(bool bRepair);
};

///////////////////////////////////////////////////////////////////////////
//	
//	Class CECIvtrOfflineTrustee
//	
///////////////////////////////////////////////////////////////////////////

class CECIvtrOfflineTrustee : public CECIvtrItem
{
public:		//	Types

public:		//	Constructor and Destructor

	CECIvtrOfflineTrustee(int tid, int expire_date);
	CECIvtrOfflineTrustee(const CECIvtrOfflineTrustee& s);
	virtual ~CECIvtrOfflineTrustee();

public:		//	Attributes

public:		//	Operations

	//	Set item detail information
	virtual bool SetItemInfo(BYTE* pInfoData, int iDataLen);
	//	Get item icon file name
	virtual const char* GetIconFile();
	//	Get item name
	virtual const wchar_t* GetName();
	//	Clone item
	virtual CECIvtrItem* Clone() { return new CECIvtrOfflineTrustee(*this); }
	//	Get drop model for shown
	virtual const char * GetDropModel();

	//	Get database data
	const OFFLINE_TRUSTEE_ESSENCE* GetDBEssence() { return m_pDBEssence; }

protected:	//	Operations

	//	Data in database
	OFFLINE_TRUSTEE_ESSENCE*		m_pDBEssence;

protected:	//	Operations

	//	Get item description text
	virtual const wchar_t* GetNormalDesc(bool bRepair);
};


///////////////////////////////////////////////////////////////////////////
//	
//	Class CECIvtrChangeShapeStone
//	
///////////////////////////////////////////////////////////////////////////

class CECIvtrChangeShapeStone : public CECIvtrItem
{
public:		//	Types

public:		//	Constructor and Destructor

	CECIvtrChangeShapeStone(int tid, int expire_date);
	CECIvtrChangeShapeStone(const CECIvtrChangeShapeStone& s);
	virtual ~CECIvtrChangeShapeStone();

public:		//	Attributes

public:		//	Operations

	//	Set item detail information
	virtual bool SetItemInfo(BYTE* pInfoData, int iDataLen);
	//	Get item icon file name
	virtual const char* GetIconFile();
	//	Get item name
	virtual const wchar_t* GetName();
	//	Clone item
	virtual CECIvtrItem* Clone() { return new CECIvtrChangeShapeStone(*this); }
	//	Get drop model for shown
	virtual const char * GetDropModel();

	//	Get database data
	const CHANGE_SHAPE_STONE_ESSENCE* GetDBEssence() { return m_pDBEssence; }

protected:	//	Operations

	//	Data in database
	CHANGE_SHAPE_STONE_ESSENCE*		m_pDBEssence;

protected:	//	Operations

	//	Get item description text
	virtual const wchar_t* GetNormalDesc(bool bRepair);
};



///////////////////////////////////////////////////////////////////////////////
//宝石物品类定义：CECIvtrGem
//Created Date: 2011-03-15.
///////////////////////////////////////////////////////////////////////////////
class CECIvtrGem : public CECIvtrItem
{
public:		//	Types

public:		//	Constructor and Destructor

	CECIvtrGem(int tid, int expire_date);
	CECIvtrGem(const CECIvtrGem& s);
	virtual ~CECIvtrGem();

public:		//	Attributes

public:		//	Operations

	//	Set item detail information
	virtual bool SetItemInfo(BYTE* pInfoData, int iDataLen);
	//	Get item icon file name
	virtual const char* GetIconFile();
	//获得该宝石的形状图标路径
	const char* GetShapeIconFile();
	//	Get item name
	virtual const wchar_t* GetName();
	//	Clone item
	virtual CECIvtrItem* Clone() { return new CECIvtrGem(*this); }
	//	Get drop model for shown
	virtual const char * GetDropModel();

	void SetSuitShapeType(bool suit) { m_bSuitShapeType = suit; }

	//	Get database data
	const GEM_ESSENCE* GetDBEssence() { return m_pDBEssence; }

protected:	//	Operations

	//	Data in database
	GEM_ESSENCE*		m_pDBEssence;
	bool				m_bSuitShapeType;

protected:	//	Operations

	//	Get item description text
	virtual const wchar_t* GetNormalDesc(bool bRepair);
};


///////////////////////////////////////////////////////////////////////////////
//宝石魔印 物品类定义：CECIvtrGemSeal
//Created Date: 2011-03-15.
///////////////////////////////////////////////////////////////////////////////
class CECIvtrGemSeal : public CECIvtrItem
{
public:		//	Types

public:		//	Constructor and Destructor

	CECIvtrGemSeal(int tid, int expire_date);
	CECIvtrGemSeal(const CECIvtrGemSeal& s);
	virtual ~CECIvtrGemSeal();

public:		//	Attributes

public:		//	Operations

	//	Set item detail information
	virtual bool SetItemInfo(BYTE* pInfoData, int iDataLen);
	//	Get item icon file name
	virtual const char* GetIconFile();
	//	Get item name
	virtual const wchar_t* GetName();
	//	Clone item
	virtual CECIvtrItem* Clone() { return new CECIvtrGemSeal(*this); }
	//	Get drop model for shown
	virtual const char * GetDropModel();

	//	Get database data
	const GEM_SEAL_ESSENCE* GetDBEssence() { return m_pDBEssence; }

protected:	//	Operations

	//	Data in database
	GEM_SEAL_ESSENCE*		m_pDBEssence;

protected:	//	Operations

	//	Get item description text
	virtual const wchar_t* GetNormalDesc(bool bRepair);
};


///////////////////////////////////////////////////////////////////////////////
//宝石粉尘 物品类定义：CECIvtrGemDust
//Created Date: 2011-03-15.
///////////////////////////////////////////////////////////////////////////////
class CECIvtrGemDust : public CECIvtrItem
{
public:		//	Types

public:		//	Constructor and Destructor

	CECIvtrGemDust(int tid, int expire_date);
	CECIvtrGemDust(const CECIvtrGemDust& s);
	virtual ~CECIvtrGemDust();

public:		//	Attributes

public:		//	Operations

	//	Set item detail information
	virtual bool SetItemInfo(BYTE* pInfoData, int iDataLen);
	//	Get item icon file name
	virtual const char* GetIconFile();
	//	Get item name
	virtual const wchar_t* GetName();
	//	Clone item
	virtual CECIvtrItem* Clone() { return new CECIvtrGemDust(*this); }
	//	Get drop model for shown
	virtual const char * GetDropModel();

	//	Get database data
	const GEM_DUST_ESSENCE* GetDBEssence() { return m_pDBEssence; }

protected:	//	Operations

	//	Data in database
	GEM_DUST_ESSENCE*		m_pDBEssence;

protected:	//	Operations

	//	Get item description text
	virtual const wchar_t* GetNormalDesc(bool bRepair);
};


///////////////////////////////////////////////////////////////////////////////
//通用道具 物品类定义：CECIvtrGeneralArticle
//Created Date: 2011-03-17.
///////////////////////////////////////////////////////////////////////////////
class CECIvtrGeneralArticle : public CECIvtrItem
{
public:		//	Types

public:		//	Constructor and Destructor

	CECIvtrGeneralArticle(int tid, int expire_date);
	CECIvtrGeneralArticle(const CECIvtrGeneralArticle& s);
	virtual ~CECIvtrGeneralArticle();

public:		//	Attributes

public:		//	Operations

	//	Set item detail information
	virtual bool SetItemInfo(BYTE* pInfoData, int iDataLen);
	//	Get item icon file name
	virtual const char* GetIconFile();
	//	Get item name
	virtual const wchar_t* GetName();
	//	Clone item
	virtual CECIvtrItem* Clone() { return new CECIvtrGeneralArticle(*this); }
	//	Get drop model for shown
	virtual const char * GetDropModel();

	//	Get database data
	const GENERAL_ARTICLE_ESSENCE* GetDBEssence() { return m_pDBEssence; }

protected:	//	Operations

	//	Data in database
	GENERAL_ARTICLE_ESSENCE*		m_pDBEssence;

protected:	//	Operations

	//	Get item description text
	virtual const wchar_t* GetNormalDesc(bool bRepair);
};


///////////////////////////////////////////////////////////////////////////////
//在线奖励小礼包 物品类定义：CECIvtrSmallGiftBag
//Created Date: 2012-03-23.
///////////////////////////////////////////////////////////////////////////////
class CECIvtrSmallGiftBag : public CECIvtrItem
{
public:		//	Types

public:		//	Constructor and Destructor

	CECIvtrSmallGiftBag(int tid, int expire_date);
	CECIvtrSmallGiftBag(const CECIvtrSmallGiftBag& s);
	virtual ~CECIvtrSmallGiftBag();

public:		//	Attributes

public:		//	Operations

	//	Set item detail information
	virtual bool SetItemInfo(BYTE* pInfoData, int iDataLen);
	//	Get item icon file name
	virtual const char* GetIconFile();
	//	Get item name
	virtual const wchar_t* GetName();
	//	Clone item
	virtual CECIvtrItem* Clone() { return new CECIvtrSmallGiftBag(*this); }
	//	Get drop model for shown
	virtual const char * GetDropModel();

	//	Get database data
	const SMALL_ONLINE_GIFT_BAG_ESSENCE* GetDBEssence() { return m_pDBEssence; }

protected:	//	Operations

	//	Data in database
	SMALL_ONLINE_GIFT_BAG_ESSENCE*		m_pDBEssence;

protected:	//	Operations

	//	Get item description text
	virtual const wchar_t* GetNormalDesc(bool bRepair);
};


///////////////////////////////////////////////////////////////////////////////
//诛仙古卷挖宝解锁道具 物品类定义：CECIvtrScrollUnlockItem
//Created Date: 2012-03-23.
///////////////////////////////////////////////////////////////////////////////
class CECIvtrScrollUnlockItem : public CECIvtrItem
{
public:		//	Types

public:		//	Constructor and Destructor

	CECIvtrScrollUnlockItem(int tid, int expire_date);
	CECIvtrScrollUnlockItem(const CECIvtrScrollUnlockItem& s);
	virtual ~CECIvtrScrollUnlockItem();

public:		//	Attributes

public:		//	Operations

	//	Set item detail information
	virtual bool SetItemInfo(BYTE* pInfoData, int iDataLen);
	//	Get item icon file name
	virtual const char* GetIconFile();
	//	Get item name
	virtual const wchar_t* GetName();
	//	Clone item
	virtual CECIvtrItem* Clone() { return new CECIvtrScrollUnlockItem(*this); }
	//	Get drop model for shown
	virtual const char * GetDropModel();

	//	Get database data
	const SCROLL_UNLOCK_ESSENCE* GetDBEssence() { return m_pDBEssence; }

protected:	//	Operations

	//	Data in database
	SCROLL_UNLOCK_ESSENCE*		m_pDBEssence;

protected:	//	Operations

	//	Get item description text
	virtual const wchar_t* GetNormalDesc(bool bRepair);
};

///////////////////////////////////////////////////////////////////////////////
//诛仙古卷挖宝增加探索次数道具 物品类定义：CECIvtrScrollDigCountIncItem
//Created Date: 2012-03-23.
///////////////////////////////////////////////////////////////////////////////
class CECIvtrScrollDigCountIncItem : public CECIvtrItem
{
public:		//	Types

public:		//	Constructor and Destructor

	CECIvtrScrollDigCountIncItem(int tid, int expire_date);
	CECIvtrScrollDigCountIncItem(const CECIvtrScrollDigCountIncItem& s);
	virtual ~CECIvtrScrollDigCountIncItem();

public:		//	Attributes

public:		//	Operations

	//	Set item detail information
	virtual bool SetItemInfo(BYTE* pInfoData, int iDataLen);
	//	Get item icon file name
	virtual const char* GetIconFile();
	//	Get item name
	virtual const wchar_t* GetName();
	//	Clone item
	virtual CECIvtrItem* Clone() { return new CECIvtrScrollDigCountIncItem(*this); }
	//	Get drop model for shown
	virtual const char * GetDropModel();

	//	Get database data
	const SCROLL_DIG_COUNT_INC_ESSENCE* GetDBEssence() { return m_pDBEssence; }

protected:	//	Operations

	//	Data in database
	SCROLL_DIG_COUNT_INC_ESSENCE*		m_pDBEssence;

protected:	//	Operations

	//	Get item description text
	virtual const wchar_t* GetNormalDesc(bool bRepair);
};


///////////////////////////////////////////////////////////////////////////
//	
//	Inline functions
//	
///////////////////////////////////////////////////////////////////////////


