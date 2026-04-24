/*
 * FILE: EC_IvtrRune.h
 *
 * DESCRIPTION: 
 *
 * CREATED BY: Fu Chonggang, 2012/4/20
 *
 * HISTORY: 
 *
 * Copyright (c) 2012 ZhuXian Studio, All Rights Reserved.
 */

#pragma once

#include "EC_IvtrEquip.h"
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

struct RUNE_EQUIP_ESSENCE;
struct RUNE_SLOT_ESSENCE;

///////////////////////////////////////////////////////////////////////////
//	
//	Declare of Global functions
//	
///////////////////////////////////////////////////////////////////////////


///////////////////////////////////////////////////////////////////////////
//	
//	Class CECIvtrRune
//	
///////////////////////////////////////////////////////////////////////////

class CECIvtrRune : public CECIvtrEquip
{
public:		//	Types

public:		//	Constructor and Destructor

	CECIvtrRune(int tid, int expire_date);
	CECIvtrRune(const CECIvtrRune& s);
	virtual ~CECIvtrRune();

public:		//	Attributes

public:		//	Operations

	//	Set item detail information
	virtual bool SetItemInfo(BYTE* pInfoData, int iDataLen);
	//	Get item icon file name
	virtual const char* GetIconFile();
	//	Get item name
	virtual const wchar_t* GetName();
	//	Clone item
	virtual CECIvtrItem* Clone() { return new CECIvtrRune(*this); }
	//	Get drop model for shown
	virtual const char * GetDropModel();

	int		GetRuneSuiteId() { return m_iRuneSuiteId; }
	
	// 星座的附加属性也是一样的规则，所以这个方法给星座共用 by wuweixin
	static ACString GetPropString(int id, int value);

	//	Get database data
	const RUNE_ENHANCED_ESSENCE& GetEssence() { return m_Essence; }
	const RUNE_EQUIP_ESSENCE* GetDBEssence() { return m_pDBEssence; }

protected:	//	Operations

	//	Data in database
	RUNE_EQUIP_ESSENCE*		m_pDBEssence;
	RUNE_ENHANCED_ESSENCE	m_Essence;
	
	int						m_iRuneSuiteId;

protected:	//	Operations

	//	Get item description text
	virtual const wchar_t* GetNormalDesc(bool bRepair);
	virtual int DecideNameCol();
};


///////////////////////////////////////////////////////////////////////////
//	
//	Class CECIvtrRuneSlot
//	
///////////////////////////////////////////////////////////////////////////

class CECIvtrRuneSlot : public CECIvtrItem
{
public:		//	Types
	
public:		//	Constructor and Destructor
	
	CECIvtrRuneSlot(int tid, int expire_date);
	CECIvtrRuneSlot(const CECIvtrRuneSlot& s);
	virtual ~CECIvtrRuneSlot();
	
public:		//	Attributes
	
public:		//	Operations
	
	//	Set item detail information
	virtual bool SetItemInfo(BYTE* pInfoData, int iDataLen);
	//	Get item icon file name
	virtual const char* GetIconFile();
	//	Get item name
	virtual const wchar_t* GetName();
	//	Clone item
	virtual CECIvtrItem* Clone() { return new CECIvtrRuneSlot(*this); }
	//	Get drop model for shown
	virtual const char * GetDropModel();
	
	const RUNE_SLOT_ESSENCE* GetDBEssence() { return m_pDBEssence; }
protected:	//	Operations

	//	Data in database
	RUNE_SLOT_ESSENCE*		m_pDBEssence;
		
protected:	//	Operations
	
	//	Get item description text
	virtual const wchar_t* GetNormalDesc(bool bRepair);
};