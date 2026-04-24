/*
 * FILE: EC_IvtrHTItem.h
 *
 * DESCRIPTION: 
 *
 * CREATED BY: Lihengju, 2009/8/29
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
namespace HOMETOWN
{
	struct TOOL_ESSENCE;
	struct FRUIT_ESSENCE;
	struct CROP_ESSENCE;
}
///////////////////////////////////////////////////////////////////////////
//	
//	Declare of Global functions
//	
///////////////////////////////////////////////////////////////////////////


///////////////////////////////////////////////////////////////////////////
//	
//	Class CECIvtrHTTool 
//	
///////////////////////////////////////////////////////////////////////////

class CECIvtrHTTool : public CECIvtrItem
{
public:		//	Types
	
	enum
	{
		HTTOOL_UNKNOWN	= -1,
		HTTOOL_HT		= 0,
		HTTOOL_PLANT,
	};

public:		//	Constructor and Destructor

	CECIvtrHTTool (int tid);
	CECIvtrHTTool (const CECIvtrHTTool & s);
	virtual ~CECIvtrHTTool ();

public:		//	Attributes

public:		//	Operations

	//	Set item detail information
	virtual bool SetItemInfo(BYTE* pInfoData, int iDataLen);
	//	Get item icon file name
	virtual const char* GetIconFile();
	//	Get item name
	virtual const wchar_t* GetName();
	//	Clone item
	virtual CECIvtrItem* Clone() { return new CECIvtrHTTool(*this); }

	int GetTargetType();
	//	Get drop model for shown
//	virtual const char * GetDropModel();

	//	Get database data
	const HOMETOWN::TOOL_ESSENCE* GetDBEssence() { return m_pDBEssence; }

protected:	//	Operations

	//	Data in database
	HOMETOWN::TOOL_ESSENCE*		m_pDBEssence;

protected:	//	Operations

	//	Get item description text
	virtual const wchar_t* GetNormalDesc(bool bRepair);
};


///////////////////////////////////////////////////////////////////////////
//	
//	Class CECIvtrHTFruit 
//	
///////////////////////////////////////////////////////////////////////////

class CECIvtrHTFruit : public CECIvtrItem
{
public:		//	Types

public:		//	Constructor and Destructor

	CECIvtrHTFruit (int tid);
	CECIvtrHTFruit (const CECIvtrHTFruit & s);
	virtual ~CECIvtrHTFruit ();

public:		//	Attributes

public:		//	Operations

	//	Set item detail information
	virtual bool SetItemInfo(BYTE* pInfoData, int iDataLen);
	//	Get item icon file name
	virtual const char* GetIconFile();
	//	Get item name
	virtual const wchar_t* GetName();
	//	Clone item
	virtual CECIvtrItem* Clone() { return new CECIvtrHTFruit(*this); }
	//	Get drop model for shown
//	virtual const char * GetDropModel();

	//	Get database data
	const HOMETOWN::FRUIT_ESSENCE* GetDBEssence() { return m_pDBEssence; }

protected:	//	Operations

	//	Data in database
	HOMETOWN::FRUIT_ESSENCE*		m_pDBEssence;

protected:	//	Operations

	//	Get item description text
	virtual const wchar_t* GetNormalDesc(bool bRepair);
};

///////////////////////////////////////////////////////////////////////////
//	
//	Class CECIvtrHTCrop 
//	
///////////////////////////////////////////////////////////////////////////

class CECIvtrHTCrop : public CECIvtrItem
{
public:		//	Types
	
public:		//	Constructor and Destructor
	
	CECIvtrHTCrop (int tid);
	CECIvtrHTCrop (const CECIvtrHTCrop & s);
	virtual ~CECIvtrHTCrop ();
	
public:		//	Attributes
	
public:		//	Operations
	
	//	Set item detail information
	virtual bool SetItemInfo(BYTE* pInfoData, int iDataLen);
	//	Get item icon file name
	virtual const char* GetIconFile();
	//	Get item name
	virtual const wchar_t* GetName();
	//	Clone item
	virtual CECIvtrItem* Clone() { return new CECIvtrHTCrop(*this); }
	//	Get drop model for shown
	//	virtual const char * GetDropModel();
	
	//	Get database data
	const HOMETOWN::CROP_ESSENCE* GetDBEssence() { return m_pDBEssence; }
	
protected:	//	Operations
	
	//	Data in database
	HOMETOWN::CROP_ESSENCE*		m_pDBEssence;
	
protected:	//	Operations
	
	//	Get item description text
	virtual const wchar_t* GetNormalDesc(bool bRepair);
};