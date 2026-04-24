/*
 * FILE: EC_IvtrSkillTome.h
 *
 * DESCRIPTION: 
 *
 * CREATED BY: Duyuxin, 2005/1/4
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

struct SKILLTOME_ESSENCE;
struct SKILLTOME_SUB_TYPE;
struct TOWNSCROLL_ESSENCE;
struct REVIVESCROLL_ESSENCE;
struct GM_GENERATOR_TYPE;
struct GM_GENERATOR_ESSENCE;
struct WAR_TANKCALLIN_ESSENCE;
struct TRANSMITROLL_ESSENCE;
struct LUCKYROLL_ESSENCE;
struct SIEGE_ARTILLERY_SCROLL_ESSENCE;

///////////////////////////////////////////////////////////////////////////
//	
//	Declare of Global functions
//	
///////////////////////////////////////////////////////////////////////////


///////////////////////////////////////////////////////////////////////////
//	
//	Class CECIvtrSkillTome
//	
///////////////////////////////////////////////////////////////////////////

class CECIvtrSkillTome : public CECIvtrItem
{
public:		//	Types

public:		//	Constructor and Destructor

	CECIvtrSkillTome(int tid, int expire_date);
	CECIvtrSkillTome(const CECIvtrSkillTome& s);
	virtual ~CECIvtrSkillTome();

public:		//	Attributes

public:		//	Operations

	//	Set item detail information
	virtual bool SetItemInfo(BYTE* pInfoData, int iDataLen);
	//	Get item icon file name
	virtual const char* GetIconFile();
	//	Get item name
	virtual const wchar_t* GetName();
	//	Clone item
	virtual CECIvtrItem* Clone() { return new CECIvtrSkillTome(*this); }
	//	Get drop model for shown
	virtual const char * GetDropModel();

	const SKILLTOME_ESSENCE* GetDBEssence() { return m_pDBEssence; }
	const SKILLTOME_SUB_TYPE* GetDBSubType() { return m_pDBSubType; }

protected:	//	Attributes

	//	Data in database
	SKILLTOME_ESSENCE*		m_pDBEssence;
	SKILLTOME_SUB_TYPE*		m_pDBSubType;

protected:	//	Operations

	//	Get item description text
	virtual const wchar_t* GetNormalDesc(bool bRepair);
};

///////////////////////////////////////////////////////////////////////////
//	
//	Class CECIvtrTownScroll
//	
///////////////////////////////////////////////////////////////////////////

class CECIvtrTownScroll : public CECIvtrItem
{
public:		//	Types

public:		//	Constructor and Destructor

	CECIvtrTownScroll(int tid, int expire_date);
	CECIvtrTownScroll(const CECIvtrTownScroll& s);
	virtual ~CECIvtrTownScroll();

public:		//	Attributes

	int tag;
	float x, z;

public:		//	Operations

	//	Set item detail information
	virtual bool SetItemInfo(BYTE* pInfoData, int iDataLen);
	//	Get item icon file name
	virtual const char* GetIconFile();
	//	Get item name
	virtual const wchar_t* GetName();
	//	Clone item
	virtual CECIvtrItem* Clone() { return new CECIvtrTownScroll(*this); }
	//	Get item cool time
	virtual int GetCoolTime(int* piMax=NULL);
	//	Get drop model for shown
	virtual const char * GetDropModel();

	const TOWNSCROLL_ESSENCE* GetDBEssence() { return m_pDBEssence; }

protected:	//	Attributes

	//	Data in database
	TOWNSCROLL_ESSENCE*	m_pDBEssence;

protected:	//	Operations

	//	Get item description text
	virtual const wchar_t* GetNormalDesc(bool bRepair);
};

///////////////////////////////////////////////////////////////////////////
//	
//	Class CECIvtrRevScroll
//	
///////////////////////////////////////////////////////////////////////////

class CECIvtrRevScroll : public CECIvtrItem
{
public:		//	Types

public:		//	Constructor and Destructor

	CECIvtrRevScroll(int tid, int expire_date);
	CECIvtrRevScroll(const CECIvtrRevScroll& s);
	virtual ~CECIvtrRevScroll();

public:		//	Attributes

public:		//	Operations

	//	Set item detail information
	virtual bool SetItemInfo(BYTE* pInfoData, int iDataLen);
	//	Get item icon file name
	virtual const char* GetIconFile();
	//	Get item name
	virtual const wchar_t* GetName();
	//	Clone item
	virtual CECIvtrItem* Clone() { return new CECIvtrRevScroll(*this); }
	//	Get item cool time
	virtual int GetCoolTime(int* piMax=NULL);
	//	Get drop model for shown
	virtual const char * GetDropModel();

	const REVIVESCROLL_ESSENCE* GetDBEssence() { return m_pDBEssence; }

protected:	//	Attributes

	//	Data in database
	REVIVESCROLL_ESSENCE*	m_pDBEssence;

protected:	//	Operations

	//	Get item description text
	virtual const wchar_t* GetNormalDesc(bool bRepair);
};

///////////////////////////////////////////////////////////////////////////
//	
//	Class CECIvtrGMGenerator
//	
///////////////////////////////////////////////////////////////////////////

class CECIvtrGMGenerator : public CECIvtrItem
{
public:		//	Types

public:		//	Constructor and Destructor

	CECIvtrGMGenerator(int tid, int expire_date);
	CECIvtrGMGenerator(const CECIvtrGMGenerator& s);
	virtual ~CECIvtrGMGenerator();

public:		//	Attributes

public:		//	Operations

	//	Set item detail information
	virtual bool SetItemInfo(BYTE* pInfoData, int iDataLen);
	//	Get item icon file name
	virtual const char* GetIconFile();
	//	Get item name
	virtual const wchar_t* GetName();
	//	Clone item
	virtual CECIvtrItem* Clone() { return new CECIvtrGMGenerator(*this); }
	//	Get item cool time
	virtual int GetCoolTime(int* piMax=NULL);
	//	Check item use condition
	virtual bool CheckUseCondition();
	//	Get drop model for shown
	virtual const char * GetDropModel();

	//	Get database data
	const GM_GENERATOR_TYPE* GetDBMajorType() { return m_pDBType; }
	const GM_GENERATOR_ESSENCE* GetDBEssence() { return m_pDBEssence; }

protected:	//	Attributes

	//	Data in database
	GM_GENERATOR_TYPE*		m_pDBType;
	GM_GENERATOR_ESSENCE*	m_pDBEssence;

protected:	//	Operations

	//	Get item description text
	virtual const wchar_t* GetNormalDesc(bool bRepair);
};

///////////////////////////////////////////////////////////////////////////
//	
//	Class CECIvtrTownScroll
//	
///////////////////////////////////////////////////////////////////////////

class CECIvtrTransmitRoll : public CECIvtrItem
{
public:		//	Types

public:		//	Constructor and Destructor

	CECIvtrTransmitRoll(int tid, int expire_date);
	CECIvtrTransmitRoll(const CECIvtrTransmitRoll& s);
	virtual ~CECIvtrTransmitRoll();

public:		//	Attributes

public:		//	Operations

	//	Set item detail information
	virtual bool SetItemInfo(BYTE* pInfoData, int iDataLen);
	//	Get item icon file name
	virtual const char* GetIconFile();
	//	Get item name
	virtual const wchar_t* GetName();
	//	Clone item
	virtual CECIvtrItem* Clone() { return new CECIvtrTransmitRoll(*this); }
	//	Get item cool time
	virtual int GetCoolTime(int* piMax=NULL);
	//	Get drop model for shown
	virtual const char * GetDropModel();

	const TRANSMITROLL_ESSENCE* GetDBEssence() { return m_pDBEssence; }

protected:	//	Attributes

	//	Data in database
	TRANSMITROLL_ESSENCE*	m_pDBEssence;

protected:	//	Operations

	//	Get item description text
	virtual const wchar_t* GetNormalDesc(bool bRepair);
};

class CECIvtrLuckyRoll : public CECIvtrItem
{
public:		//	Types

public:		//	Constructor and Destructor

	CECIvtrLuckyRoll(int tid, int expire_date);
	CECIvtrLuckyRoll(const CECIvtrLuckyRoll& s);
	virtual ~CECIvtrLuckyRoll();

public:		//	Attributes

public:		//	Operations

	//	Set item detail information
	virtual bool SetItemInfo(BYTE* pInfoData, int iDataLen);
	//	Get item icon file name
	virtual const char* GetIconFile();
	//	Get item name
	virtual const wchar_t* GetName();
	//	Clone item
	virtual CECIvtrItem* Clone() { return new CECIvtrLuckyRoll(*this); }
	//	Get item cool time
	virtual int GetCoolTime(int* piMax=NULL);
	//	Get drop model for shown
	virtual const char * GetDropModel();

	const LUCKYROLL_ESSENCE* GetDBEssence() { return m_pDBEssence; }

protected:	//	Attributes

	//	Data in database
	LUCKYROLL_ESSENCE*	m_pDBEssence;

protected:	//	Operations

	//	Get item description text
	virtual const wchar_t* GetNormalDesc(bool bRepair);
};

class CECIvtrArtilleryScroll : public CECIvtrItem
{
public:		//	Types

public:		//	Constructor and Destructor

	CECIvtrArtilleryScroll(int tid, int expire_date);
	CECIvtrArtilleryScroll(const CECIvtrArtilleryScroll& s);
	virtual ~CECIvtrArtilleryScroll();

public:		//	Attributes

public:		//	Operations

	//	Set item detail information
	virtual bool SetItemInfo(BYTE* pInfoData, int iDataLen);
	//	Get item icon file name
	virtual const char* GetIconFile();
	//	Get item name
	virtual const wchar_t* GetName();
	//	Clone item
	virtual CECIvtrItem* Clone() { return new CECIvtrArtilleryScroll(*this); }
	//	Get item cool time
	virtual int GetCoolTime(int* piMax=NULL);
	//	Get drop model for shown
	virtual const char * GetDropModel();

	const SIEGE_ARTILLERY_SCROLL_ESSENCE* GetDBEssence() { return m_pDBEssence; }

protected:	//	Attributes

	//	Data in database
	SIEGE_ARTILLERY_SCROLL_ESSENCE*	m_pDBEssence;

protected:	//	Operations

	//	Get item description text
	virtual const wchar_t* GetNormalDesc(bool bRepair);
};

///////////////////////////////////////////////////////////////////////////
//	
//	Inline functions
//	
///////////////////////////////////////////////////////////////////////////


