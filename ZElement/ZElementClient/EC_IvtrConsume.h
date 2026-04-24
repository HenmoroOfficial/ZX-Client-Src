/*
 * FILE: EC_IvtrConsume.h
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

#include "EC_IvtrEquip.h"
#include "EC_IvtrTypes.h"
#include "itemdataman.h"
#include "AList2.h"

///////////////////////////////////////////////////////////////////////////
//	
//	Define and Macro
//	
///////////////////////////////////////////////////////////////////////////

namespace GNET
{
	class Octets;
}

///////////////////////////////////////////////////////////////////////////
//	
//	Types and Global variables
//	
///////////////////////////////////////////////////////////////////////////

struct MEDICINE_MAJOR_TYPE;
struct MEDICINE_SUB_TYPE;
struct MEDICINE_ESSENCE;
struct FIREWORKS_ESSENCE;
struct RECIPEROLL_ESSENCE;
struct RECIPE_ESSENCE;
struct DOUBLE_EXP_ESSENCE;
struct SKILLMATTER_ESSENCE;
struct VEHICLE_ESSENCE;
struct COUPLE_JUMPTO_ESSENCE;
struct LOTTERY_ESSENCE;
struct CAMRECORDER_ESSENCE;
struct TEXT_FIREWORKS_ESSENCE;
struct TALISMAN_EXPFOOD_ESSENCE;
struct TALISMAN_MERGEKATALYST_ESSENCE;
struct TALISMAN_ENERGYFOOD_ESSENCE;
struct PET_BEDGE_ESSENCE;
struct PET_FOOD_ESSENCE;
struct PET_SKILL_ESSENCE;
struct PET_ARMOR_ESSENCE;
struct EQUIP_SOUL_ESSENCE;
struct CHANGE_SHAPE_CARD_ESSENCE;
struct MERCENARY_CREDENTIAL_ESSENCE;
struct FLAG_BUFF_ITEM_ESSENCE;
///////////////////////////////////////////////////////////////////////////
//	
//	Declare of Global functions
//	
///////////////////////////////////////////////////////////////////////////


///////////////////////////////////////////////////////////////////////////
//	
//	Class CECIvtrMedicine
//	
///////////////////////////////////////////////////////////////////////////

class CECIvtrMedicine : public CECIvtrItem
{
public:		//	Types

public:		//	Constructor and Destructor

	CECIvtrMedicine(int tid, int expire_date);
	CECIvtrMedicine(const CECIvtrMedicine& s);
	virtual ~CECIvtrMedicine();

public:		//	Attributes

public:		//	Operations

	//	Set item detail information
	virtual bool SetItemInfo(BYTE* pInfoData, int iDataLen);
	virtual GNET::Octets gen_item_info();
	//	Get item icon file name
	virtual const char* GetIconFile();
	//	Get item name
	virtual const wchar_t* GetName();
	//	Clone item
	virtual CECIvtrItem* Clone() { return new CECIvtrMedicine(*this); }
	//	Get item cool time
	virtual int GetCoolTime(int* piMax=NULL);
	//	Check item use condition
	virtual bool CheckUseCondition();
	//	Get drop model for shown
	virtual const char * GetDropModel();

	//	Get database data
	const MEDICINE_MAJOR_TYPE* GetDBMajorType() { return m_pDBMajorType; }
	const MEDICINE_SUB_TYPE* GetDBSubType() { return m_pDBSubType; }
	const MEDICINE_ESSENCE* GetDBEssence() { return m_pDBEssence; }
	const IVTR_ESSENCE_MEDICINE& GetEssence() { return m_Essence; }
	int GetLevelRequirement() const { return m_iLevelReq; }
	
protected:	//	Operations

	//	Data in database
	MEDICINE_MAJOR_TYPE*	m_pDBMajorType;
	MEDICINE_SUB_TYPE*		m_pDBSubType;
	MEDICINE_ESSENCE*		m_pDBEssence;

	IVTR_ESSENCE_MEDICINE	m_Essence;

	int						m_iLevelReq;

protected:	//	Operations

	//	Get item description text
	virtual const wchar_t* GetNormalDesc(bool bRepair);
	//	Get item description for booth buying
	virtual const wchar_t* GetBoothBuyDesc();

	//	Build medicine effect description
	void BuildEffectDesc();

	ACString _Serialize() const;
	bool _Unserialize(const ACHAR* szText);
};

///////////////////////////////////////////////////////////////////////////
//	
//	Class CECIvtrFirework
//	
///////////////////////////////////////////////////////////////////////////

class CECIvtrFirework : public CECIvtrItem
{
public:		//	Types

public:		//	Constructor and Destructor

	CECIvtrFirework(int tid, int expire_date);
	CECIvtrFirework(const CECIvtrFirework& s);
	virtual ~CECIvtrFirework();

public:		//	Attributes

public:		//	Operations

	//	Set item detail information
	virtual bool SetItemInfo(BYTE* pInfoData, int iDataLen);
	//	Get item icon file name
	virtual const char* GetIconFile();
	//	Get item name
	virtual const wchar_t* GetName();
	//	Clone item
	virtual CECIvtrItem* Clone() { return new CECIvtrFirework(*this); }
	//	Get item cool time
	virtual int GetCoolTime(int* piMax=NULL);
	//	Get drop model for shown
	virtual const char * GetDropModel();

	const FIREWORKS_ESSENCE* GetDBEssence() { return m_pDBEssence; }

protected:	//	Attributes

	//	Data in database
	FIREWORKS_ESSENCE*		m_pDBEssence;

protected:	//	Operations

	//	Get item description text
	virtual const wchar_t* GetNormalDesc(bool bRepair);
};

///////////////////////////////////////////////////////////////////////////
//	
//	Class CECIvtrTextFirework
//	
///////////////////////////////////////////////////////////////////////////

class CECIvtrTextFirework : public CECIvtrItem
{
public:		//	Types

public:		//	Constructor and Destructor

	CECIvtrTextFirework(int tid, int expire_date);
	CECIvtrTextFirework(const CECIvtrTextFirework& s);
	virtual ~CECIvtrTextFirework();

public:		//	Attributes

public:		//	Operations

	//	Set item detail information
	virtual bool SetItemInfo(BYTE* pInfoData, int iDataLen);
	//	Get item icon file name
	virtual const char* GetIconFile();
	//	Get item name
	virtual const wchar_t* GetName();
	//	Clone item
	virtual CECIvtrItem* Clone() { return new CECIvtrTextFirework(*this); }
	//	Get item cool time
	virtual int GetCoolTime(int* piMax=NULL);
	//	Get drop model for shown
	virtual const char * GetDropModel();

	const TEXT_FIREWORKS_ESSENCE* GetDBEssence() { return m_pDBEssence; }

protected:	//	Attributes

	//	Data in database
	TEXT_FIREWORKS_ESSENCE*		m_pDBEssence;

protected:	//	Operations

	//	Get item description text
	virtual const wchar_t* GetNormalDesc(bool bRepair);
};

///////////////////////////////////////////////////////////////////////////
//	
//	Class CECIvtrRecipeRoll
//	
///////////////////////////////////////////////////////////////////////////

class CECIvtrRecipeRoll : public CECIvtrItem
{
public:		//	Types

public:		//	Constructor and Destructor

	CECIvtrRecipeRoll(int tid, int expire_date);
	CECIvtrRecipeRoll(const CECIvtrRecipeRoll& s);
	virtual ~CECIvtrRecipeRoll();

public:		//	Attributes

public:		//	Operations

	//	Set item detail information
	virtual bool SetItemInfo(BYTE* pInfoData, int iDataLen);
	//	Get item icon file name
	virtual const char* GetIconFile();
	//	Get item name
	virtual const wchar_t* GetName();
	//	Clone item
	virtual CECIvtrItem* Clone() { return new CECIvtrRecipeRoll(*this); }
	//	Get item cool time
	virtual int GetCoolTime(int* piMax=NULL);
	//	Check item use condition
	virtual bool CheckUseCondition();
	//	Get drop model for shown
	virtual const char * GetDropModel();

	const RECIPEROLL_ESSENCE* GetDBEssence() { return m_pDBEssence; }

protected:	//	Attributes

	//	Data in database
	RECIPEROLL_ESSENCE*		m_pDBEssence;
	RECIPE_ESSENCE*			m_pRelatedRecipe;

protected:	//	Operations

	//	Get item description text
	virtual const wchar_t* GetNormalDesc(bool bRepair);
};

///////////////////////////////////////////////////////////////////////////
//	
//	Class CECIvtrDoubleExp
//	
///////////////////////////////////////////////////////////////////////////

class CECIvtrDoubleExp : public CECIvtrItem
{
public:		//	Types

public:		//	Constructor and Destructor

	CECIvtrDoubleExp(int tid, int expire_date);
	CECIvtrDoubleExp(const CECIvtrDoubleExp& s);
	virtual ~CECIvtrDoubleExp();

public:		//	Attributes

public:		//	Operations

	//	Set item detail information
	virtual bool SetItemInfo(BYTE* pInfoData, int iDataLen);
	//	Get item icon file name
	virtual const char* GetIconFile();
	//	Get item name
	virtual const wchar_t* GetName();
	//	Clone item
	virtual CECIvtrItem* Clone() { return new CECIvtrDoubleExp(*this); }
	//	Get item cool time
	virtual int GetCoolTime(int* piMax=NULL);
	//	Check item use condition
	virtual bool CheckUseCondition();
	//	Get drop model for shown
	virtual const char * GetDropModel();

	const DOUBLE_EXP_ESSENCE* GetDBEssence() { return m_pDBEssence; }

protected:	//	Attributes

	//	Data in database
	DOUBLE_EXP_ESSENCE*		m_pDBEssence;

protected:	//	Operations

	//	Get item description text
	virtual const wchar_t* GetNormalDesc(bool bRepair);
};

///////////////////////////////////////////////////////////////////////////
//	
//	Class CECIvtrSkillMat
//	
///////////////////////////////////////////////////////////////////////////

class CECIvtrSkillMat : public CECIvtrItem
{
public:		//	Types

public:		//	Constructor and Destructor

	CECIvtrSkillMat(int tid, int expire_date);
	CECIvtrSkillMat(const CECIvtrSkillMat& s);
	virtual ~CECIvtrSkillMat();

public:		//	Attributes

public:		//	Operations

	//	Set item detail information
	virtual bool SetItemInfo(BYTE* pInfoData, int iDataLen);
	//	Get item icon file name
	virtual const char* GetIconFile();
	//	Get item name
	virtual const wchar_t* GetName();
	//	Clone item
	virtual CECIvtrItem* Clone() { return new CECIvtrSkillMat(*this); }
	//	Get item cool time
	virtual int GetCoolTime(int* piMax=NULL);
	//	Check item use condition
	virtual bool CheckUseCondition();
	//	Get drop model for shown
	virtual const char * GetDropModel();

	const SKILLMATTER_ESSENCE* GetDBEssence() { return m_pDBEssence; }

protected:	//	Attributes

	//	Data in database
	SKILLMATTER_ESSENCE*	m_pDBEssence;

protected:	//	Operations

	//	Get item description text
	virtual const wchar_t* GetNormalDesc(bool bRepair);
};

///////////////////////////////////////////////////////////////////////////
//	
//	Class CECIvtrVehicle
//	
///////////////////////////////////////////////////////////////////////////

class CECIvtrVehicle : public CECIvtrItem
{
public:		//	Types

public:		//	Constructor and Destructor

	CECIvtrVehicle(int tid, int expire_date);
	CECIvtrVehicle(const CECIvtrVehicle& s);
	virtual ~CECIvtrVehicle();

public:		//	Attributes

public:		//	Operations

	//	Set item detail information
	virtual bool SetItemInfo(BYTE* pInfoData, int iDataLen);
	//	Get item icon file name
	virtual const char* GetIconFile();
	//	Get item name
	virtual const wchar_t* GetName();
	//	Clone item
	virtual CECIvtrItem* Clone() { return new CECIvtrVehicle(*this); }
	//	Get item cool time
	virtual int GetCoolTime(int* piMax=NULL);
	//	Check item use condition
	virtual bool CheckUseCondition();
	//	Get drop model for shown
	virtual const char * GetDropModel();

	const VEHICLE_ESSENCE* GetDBEssence() { return m_pDBEssence; }

protected:	//	Attributes

	//	Data in database
	VEHICLE_ESSENCE*	m_pDBEssence;
	abase::vector<int>	m_addons;

protected:	//	Operations

	//	Get item description text
	virtual const wchar_t* GetNormalDesc(bool bRepair);
	ACString _Serialize() const;
	bool _Unserialize(const ACHAR* szText);	
};

///////////////////////////////////////////////////////////////////////////
//	
//	Class CECIvtrCoupleJumpTo
//	
///////////////////////////////////////////////////////////////////////////

class CECIvtrCoupleJumpTo : public CECIvtrItem
{
public:		//	Types

public:		//	Constructor and Destructor

	CECIvtrCoupleJumpTo(int tid, int expire_date);
	CECIvtrCoupleJumpTo(const CECIvtrCoupleJumpTo& s);
	virtual ~CECIvtrCoupleJumpTo();

public:		//	Attributes

public:		//	Operations

	//	Set item detail information
	virtual bool SetItemInfo(BYTE* pInfoData, int iDataLen);
	//	Get item icon file name
	virtual const char* GetIconFile();
	//	Get item name
	virtual const wchar_t* GetName();
	//	Clone item
	virtual CECIvtrItem* Clone() { return new CECIvtrCoupleJumpTo(*this); }
	//	Get item cool time
	virtual int GetCoolTime(int* piMax=NULL);
	//	Check item use condition
	virtual bool CheckUseCondition();
	//	Get drop model for shown
	virtual const char * GetDropModel();

	const COUPLE_JUMPTO_ESSENCE* GetDBEssence() { return m_pDBEssence; }

protected:	//	Attributes

	//	Data in database
	COUPLE_JUMPTO_ESSENCE*	m_pDBEssence;

protected:	//	Operations

	//	Get item description text
	virtual const wchar_t* GetNormalDesc(bool bRepair);
};

///////////////////////////////////////////////////////////////////////////
//	
//	Class CECIvtrLottery
//	
///////////////////////////////////////////////////////////////////////////

class CECIvtrLottery : public CECIvtrItem
{
public:		//	Types

public:		//	Constructor and Destructor

	CECIvtrLottery(int tid, int expire_date);
	CECIvtrLottery(const CECIvtrLottery& s);
	virtual ~CECIvtrLottery();

public:		//	Attributes

public:		//	Operations

	//	Set item detail information
	virtual bool SetItemInfo(BYTE* pInfoData, int iDataLen);
	virtual GNET::Octets gen_item_info();
	virtual bool SetEmptyInfo();
	//	Get item icon file name
	virtual const char* GetIconFile();
	//	Get item name
	virtual const wchar_t* GetName();
	//	Clone item
	virtual CECIvtrItem* Clone() { return new CECIvtrLottery(*this); }
	//	Get item cool time
	virtual int GetCoolTime(int* piMax=NULL);
	//	Check item use condition
	virtual bool CheckUseCondition();
	//	Get drop model for shown
	virtual const char * GetDropModel();

	const LOTTERY_ESSENCE* GetDBEssence() { return m_pDBEssence; }
	const IVTR_ESSENCE_LOTTERY& GetEssence() { return m_Essence; }

protected:	//	Attributes

	//	Data in database
	LOTTERY_ESSENCE*		m_pDBEssence;
	IVTR_ESSENCE_LOTTERY	m_Essence;
	ACString				m_strRank;
protected:	//	Operations

	//	Get item description text
	virtual const wchar_t* GetNormalDesc(bool bRepair);
	ACString _Serialize() const;
	bool _Unserialize(const ACHAR* szText);
	void GetAwardRank();
};

///////////////////////////////////////////////////////////////////////////
//	
//	Class CECIvtrLottery2
//	
///////////////////////////////////////////////////////////////////////////

class CECIvtrLottery2 : public CECIvtrItem
{
public:		//	Types

public:		//	Constructor and Destructor

	CECIvtrLottery2(int tid, int expire_date);
	CECIvtrLottery2(const CECIvtrLottery2& s);
	virtual ~CECIvtrLottery2();

public:		//	Attributes

public:		//	Operations

	//	Set item detail information
	virtual bool SetItemInfo(BYTE* pInfoData, int iDataLen);

	//	Get item icon file name
	virtual const char* GetIconFile();
	//	Get item name
	virtual const wchar_t* GetName();
	//	Clone item
	virtual CECIvtrItem* Clone() { return new CECIvtrLottery2(*this); }
	//	Get item cool time
	virtual int GetCoolTime(int* piMax=NULL);
	//	Check item use condition
	virtual bool CheckUseCondition();
	//	Get drop model for shown
	virtual const char * GetDropModel();

	const LOTTERY2_ESSENCE* GetDBEssence() { return m_pDBEssence; }

protected:	//	Attributes

	//	Data in database
	LOTTERY2_ESSENCE*		m_pDBEssence;

protected:	//	Operations

	//	Get item description text
	virtual const wchar_t* GetNormalDesc(bool bRepair);
// 	ACString _Serialize() const;
// 	bool _Unserialize(const ACHAR* szText);
//	void GetAwardRank();
};

///////////////////////////////////////////////////////////////////////////
//
//CECIvtrLottery3类的定义(新彩票)
//
///////////////////////////////////////////////////////////////////////////
class CECIvtrLottery3 : public CECIvtrItem
{
public:		//constructor and destructor
	CECIvtrLottery3(int tid, int expire_date);
	CECIvtrLottery3(const CECIvtrLottery3& lottery);
	virtual ~CECIvtrLottery3();

public:		//operations
	//	Set item detail information
	virtual bool SetItemInfo(BYTE* pInfoData, int iDataLen);

	//	Get item icon file name
	virtual const char* GetIconFile();
	//	Get item name
	virtual const wchar_t* GetName();
	//	Clone item
	virtual CECIvtrItem* Clone() { return new CECIvtrLottery3(*this); }	
	//	Get item cool time
	virtual int GetCoolTime(int* piMax=NULL);
	//	Check item use condition
	virtual bool CheckUseCondition();
	//	Get drop model for shown
	virtual const char * GetDropModel();

	const LOTTERY3_ESSENCE* GetDBEssence() { return m_pDBEssence; }

protected:	//attributes
	LOTTERY3_ESSENCE*		m_pDBEssence;

protected:	//operations
	virtual const wchar_t* GetNormalDesc(bool bRepair);
};

///////////////////////////////////////////////////////////////////////////
//	
//	Class CECCamRecorder
//	
///////////////////////////////////////////////////////////////////////////

class CECCamRecorder : public CECIvtrItem
{
public:		//	Types

public:		//	Constructor and Destructor

	CECCamRecorder(int tid, int expire_date);
	CECCamRecorder(const CECCamRecorder& s);
	virtual ~CECCamRecorder();

public:		//	Attributes

public:		//	Operations

	//	Set item detail information
	virtual bool SetItemInfo(BYTE* pInfoData, int iDataLen);
	//	Get item icon file name
	virtual const char* GetIconFile();
	//	Get item name
	virtual const wchar_t* GetName();
	//	Clone item
	virtual CECIvtrItem* Clone() { return new CECCamRecorder(*this); }
	//	Get item cool time
	virtual int GetCoolTime(int* piMax=NULL);
	//	Check item use condition
	virtual bool CheckUseCondition();
	//	Get drop model for shown
	virtual const char * GetDropModel();

	const CAMRECORDER_ESSENCE* GetDBEssence() { return m_pDBEssence; }

protected:	//	Attributes

	//	Data in database
	CAMRECORDER_ESSENCE*		m_pDBEssence;

protected:	//	Operations

	//	Get item description text
	virtual const wchar_t* GetNormalDesc(bool bRepair);
};

///////////////////////////////////////////////////////////////////////////
//	
//	Class CECIvtrTalismanExpFood
//	
///////////////////////////////////////////////////////////////////////////

class CECIvtrTalismanExpFood : public CECIvtrItem
{
public:		//	Types

public:		//	Constructor and Destructor

	CECIvtrTalismanExpFood(int tid, int expire_date);
	CECIvtrTalismanExpFood(const CECIvtrTalismanExpFood& s);
	virtual ~CECIvtrTalismanExpFood();

public:		//	Attributes

public:		//	Operations

	//	Set item detail information
	virtual bool SetItemInfo(BYTE* pInfoData, int iDataLen);
	//	Get item icon file name
	virtual const char* GetIconFile();
	//	Get item name
	virtual const wchar_t* GetName();
	//	Clone item
	virtual CECIvtrItem* Clone() { return new CECIvtrTalismanExpFood(*this); }
	//	Get drop model for shown
	virtual const char * GetDropModel();

	const TALISMAN_EXPFOOD_ESSENCE* GetDBEssence() { return m_pDBEssence; }

protected:	//	Attributes

	//	Data in database
	TALISMAN_EXPFOOD_ESSENCE*		m_pDBEssence;

protected:	//	Operations

	//	Get item description text
	virtual const wchar_t* GetNormalDesc(bool bRepair);
};

///////////////////////////////////////////////////////////////////////////
//	
//	Class CECIvtrTalismanMergeKatalyst
//	
///////////////////////////////////////////////////////////////////////////

class CECIvtrTalismanMergeKatalyst : public CECIvtrItem
{
public:		//	Types

public:		//	Constructor and Destructor

	CECIvtrTalismanMergeKatalyst(int tid, int expire_date);
	CECIvtrTalismanMergeKatalyst(const CECIvtrTalismanMergeKatalyst& s);
	virtual ~CECIvtrTalismanMergeKatalyst();

public:		//	Attributes

public:		//	Operations

	//	Set item detail information
	virtual bool SetItemInfo(BYTE* pInfoData, int iDataLen);
	//	Get item icon file name
	virtual const char* GetIconFile();
	//	Get item name
	virtual const wchar_t* GetName();
	//	Clone item
	virtual CECIvtrItem* Clone() { return new CECIvtrTalismanMergeKatalyst(*this); }
	//	Get drop model for shown
	virtual const char * GetDropModel();

	const TALISMAN_MERGEKATALYST_ESSENCE* GetDBEssence() { return m_pDBEssence; }

protected:	//	Attributes

	//	Data in database
	TALISMAN_MERGEKATALYST_ESSENCE*		m_pDBEssence;

protected:	//	Operations

	//	Get item description text
	virtual const wchar_t* GetNormalDesc(bool bRepair);
};

///////////////////////////////////////////////////////////////////////////
//	
//	Class CECIvtrTalismanEnergyFood
//	
///////////////////////////////////////////////////////////////////////////

class CECIvtrTalismanEnergyFood : public CECIvtrItem
{
public:		//	Types

public:		//	Constructor and Destructor

	CECIvtrTalismanEnergyFood(int tid, int expire_date);
	CECIvtrTalismanEnergyFood(const CECIvtrTalismanEnergyFood& s);
	virtual ~CECIvtrTalismanEnergyFood();

public:		//	Attributes

public:		//	Operations

	//	Set item detail information
	virtual bool SetItemInfo(BYTE* pInfoData, int iDataLen);
	//	Get item icon file name
	virtual const char* GetIconFile();
	//	Get item name
	virtual const wchar_t* GetName();
	//	Clone item
	virtual CECIvtrItem* Clone() { return new CECIvtrTalismanEnergyFood(*this); }
	//	Get drop model for shown
	virtual const char * GetDropModel();
	//	Check item use condition
	virtual bool CheckUseCondition();

	const TALISMAN_ENERGYFOOD_ESSENCE* GetDBEssence() { return m_pDBEssence; }
	const IVTR_ESSENCE_TALISMAN_ENERGYFOOD& GetEssence() { return m_Essence; }

protected:	//	Attributes
	IVTR_ESSENCE_TALISMAN_ENERGYFOOD		m_Essence;

	//	Data in database
	TALISMAN_ENERGYFOOD_ESSENCE*		m_pDBEssence;

protected:	//	Operations

	//	Get item description text
	virtual const wchar_t* GetNormalDesc(bool bRepair);

	ACString _Serialize() const;
	bool _Unserialize(const ACHAR* szText);
};

///////////////////////////////////////////////////////////////////////////
//	
//	Class CECPetBedge
//	
///////////////////////////////////////////////////////////////////////////

class CECPetBedge : public CECIvtrItem
{
public:		//	Types

	struct PET_SKILL_COOLDOWN
	{
		int	iCoolCnt;
		int	iCoolMax;
		int iCoolFinishTime;
	};

public:		//	Constructor and Destructor

	CECPetBedge(int tid, int expire_date);
	CECPetBedge(const CECPetBedge& s);
	virtual ~CECPetBedge();

public:		//	Attributes

public:		//	Operations

	//	Set item detail information
	virtual bool SetItemInfo(BYTE* pInfoData, int iDataLen);
	virtual GNET::Octets gen_item_info();
	//	Get item icon file name
	virtual const char* GetIconFile();
	//	Get item name
	virtual const wchar_t* GetName();
	//	Clone item
	virtual CECIvtrItem* Clone() { return new CECPetBedge(*this); }
	//	Get item cool time
	virtual int GetCoolTime(int* piMax=NULL);
	//	Check item use condition
	virtual bool CheckUseCondition();
	//	Get drop model for shown
	virtual const char * GetDropModel();

	const PET_BEDGE_ESSENCE* GetDBEssence() { return m_pDBEssence; }
	itemdataman::_pet_bedge_essence& GetEssence() { return m_Essence; }
	void RefreshCoolDown(DWORD dwTick);
	//	Set skill cool time
	void SetSkillCoolTime(int iCoolIdx, int iTime);
	//	Get skill cool time
	int GetSkillCoolTime(int iSkillIdx, int* piMax=NULL);
	
	ACString GetOwnerName();

protected:	//	Attributes

	//	Data in database
	PET_BEDGE_ESSENCE*		m_pDBEssence;
	itemdataman::_pet_bedge_essence m_Essence;
	PET_SKILL_COOLDOWN m_CoolDown[8];

protected:	//	Operations

	//	Get item description text
	virtual const wchar_t* GetNormalDesc(bool bRepair);

	ACString _Serialize() const;
	bool _Unserialize(const ACHAR* szText);
};

///////////////////////////////////////////////////////////////////////////
//	
//	Class CECPetFood
//	
///////////////////////////////////////////////////////////////////////////

class CECPetFood : public CECIvtrItem
{
public:		//	Types

public:		//	Constructor and Destructor

	CECPetFood(int tid, int expire_date);
	CECPetFood(const CECPetFood& s);
	virtual ~CECPetFood();

public:		//	Attributes

public:		//	Operations

	//	Set item detail information
	virtual bool SetItemInfo(BYTE* pInfoData, int iDataLen);
	//	Get item icon file name
	virtual const char* GetIconFile();
	//	Get item name
	virtual const wchar_t* GetName();
	//	Clone item
	virtual CECIvtrItem* Clone() { return new CECPetFood(*this); }
	//	Get item cool time
	virtual int GetCoolTime(int* piMax=NULL);
	//	Check item use condition
	virtual bool CheckUseCondition();
	//	Get drop model for shown
	virtual const char * GetDropModel();

	const PET_FOOD_ESSENCE* GetDBEssence() { return m_pDBEssence; }

protected:	//	Attributes

	//	Data in database
	PET_FOOD_ESSENCE*		m_pDBEssence;

protected:	//	Operations

	//	Get item description text
	virtual const wchar_t* GetNormalDesc(bool bRepair);
};
 
class CECPetAutoFood : public CECIvtrItem
{
public:		//	Types

public:		//	Constructor and Destructor

	CECPetAutoFood(int tid, int expire_date);
	CECPetAutoFood(const CECPetAutoFood& s);
	virtual ~CECPetAutoFood();

public:		//	Attributes

public:		//	Operations

	//	Set item detail information
	virtual bool SetItemInfo(BYTE* pInfoData, int iDataLen);
	//	Get item icon file name
	virtual const char* GetIconFile();
	//	Get item name
	virtual const wchar_t* GetName();
	//	Clone item
	virtual CECIvtrItem* Clone() { return new CECPetAutoFood(*this); }
	//	Get item cool time
	virtual int GetCoolTime(int* piMax=NULL);
	//	Check item use condition
	virtual bool CheckUseCondition();
	//	Get drop model for shown
	virtual const char * GetDropModel();

	const PET_AUTO_FOOD_ESSENCE* GetDBEssence() { return m_pDBEssence; }
	int GetCurValue() { return cur_value; }
	ACString _Serialize() const;
	bool _Unserialize(const ACHAR* szText);	

protected:	//	Attributes

	//	Data in database
	PET_AUTO_FOOD_ESSENCE* m_pDBEssence;
	int cur_value;

protected:	//	Operations

	//	Get item description text
	virtual const wchar_t* GetNormalDesc(bool bRepair);
};

class CECPetRefine : public CECIvtrItem
{
public:		//	Types

public:		//	Constructor and Destructor

	CECPetRefine(int tid, int expire_date);
	CECPetRefine(const CECPetRefine& s);
	virtual ~CECPetRefine();

public:		//	Attributes

public:		//	Operations

	//	Set item detail information
	virtual bool SetItemInfo(BYTE* pInfoData, int iDataLen);
	//	Get item icon file name
	virtual const char* GetIconFile();
	//	Get item name
	virtual const wchar_t* GetName();
	//	Clone item
	virtual CECIvtrItem* Clone() { return new CECPetRefine(*this); }
	//	Get item cool time
	virtual int GetCoolTime(int* piMax=NULL);
	//	Check item use condition
	virtual bool CheckUseCondition();
	//	Get drop model for shown
	virtual const char * GetDropModel();

	const PET_REFINE_ESSENCE* GetDBEssence() { return m_pDBEssence; }

protected:	//	Attributes

	//	Data in database
	PET_REFINE_ESSENCE* m_pDBEssence;

protected:	//	Operations

	//	Get item description text
	virtual const wchar_t* GetNormalDesc(bool bRepair);
};

class CECPetAssistRefine : public CECIvtrItem
{
public:		//	Types

public:		//	Constructor and Destructor

	CECPetAssistRefine(int tid, int expire_date);
	CECPetAssistRefine(const CECPetAssistRefine& s);
	virtual ~CECPetAssistRefine();

public:		//	Attributes

public:		//	Operations

	//	Set item detail information
	virtual bool SetItemInfo(BYTE* pInfoData, int iDataLen);
	//	Get item icon file name
	virtual const char* GetIconFile();
	//	Get item name
	virtual const wchar_t* GetName();
	//	Clone item
	virtual CECIvtrItem* Clone() { return new CECPetAssistRefine(*this); }
	//	Get item cool time
	virtual int GetCoolTime(int* piMax=NULL);
	//	Check item use condition
	virtual bool CheckUseCondition();
	//	Get drop model for shown
	virtual const char * GetDropModel();

	const PET_ASSIST_REFINE_ESSENCE* GetDBEssence() { return m_pDBEssence; }

protected:	//	Attributes

	//	Data in database
	PET_ASSIST_REFINE_ESSENCE* m_pDBEssence;

protected:	//	Operations

	//	Get item description text
	virtual const wchar_t* GetNormalDesc(bool bRepair);
};

///////////////////////////////////////////////////////////////////////////
//	
//	Class CECPetSkill
//	
///////////////////////////////////////////////////////////////////////////

class CECPetSkill : public CECIvtrItem
{
public:		//	Types

public:		//	Constructor and Destructor

	CECPetSkill(int tid, int expire_date);
	CECPetSkill(const CECPetSkill& s);
	virtual ~CECPetSkill();

public:		//	Attributes

public:		//	Operations

	//	Set item detail information
	virtual bool SetItemInfo(BYTE* pInfoData, int iDataLen);
	//	Get item icon file name
	virtual const char* GetIconFile();
	//	Get item name
	virtual const wchar_t* GetName();
	//	Clone item
	virtual CECIvtrItem* Clone() { return new CECPetSkill(*this); }
	//	Get item cool time
	virtual int GetCoolTime(int* piMax=NULL);
	//	Check item use condition
	virtual bool CheckUseCondition();
	//	Get drop model for shown
	virtual const char * GetDropModel();

	const PET_SKILL_ESSENCE* GetDBEssence() { return m_pDBEssence; }

protected:	//	Attributes

	//	Data in database
	PET_SKILL_ESSENCE*		m_pDBEssence;

protected:	//	Operations

	//	Get item description text
	virtual const wchar_t* GetNormalDesc(bool bRepair);
};

///////////////////////////////////////////////////////////////////////////
//	
//	Class CECPetArmor
//	
///////////////////////////////////////////////////////////////////////////

class CECPetArmor : public CECIvtrItem
{
public:		//	Types

public:		//	Constructor and Destructor

	CECPetArmor(int tid, int expire_date);
	CECPetArmor(const CECPetArmor& s);
	virtual ~CECPetArmor();

public:		//	Attributes

public:		//	Operations

	//	Set item detail information
	virtual bool SetItemInfo(BYTE* pInfoData, int iDataLen);
	//	Get item icon file name
	virtual const char* GetIconFile();
	//	Get item name
	virtual const wchar_t* GetName();
	//	Clone item
	virtual CECIvtrItem* Clone() { return new CECPetArmor(*this); }
	//	Get item cool time
	virtual int GetCoolTime(int* piMax=NULL);
	//	Check item use condition
	virtual bool CheckUseCondition();
	//	Get drop model for shown
	virtual const char * GetDropModel();

	const PET_ARMOR_ESSENCE* GetDBEssence() { return m_pDBEssence; }
	itemdataman::_pet_armor_essence& GetEssence() { return m_Essence; }

protected:	//	Attributes

	//	Data in database
	PET_ARMOR_ESSENCE*		m_pDBEssence;
	itemdataman::_pet_armor_essence m_Essence;

protected:	//	Operations

	//	Get item description text
	virtual const wchar_t* GetNormalDesc(bool bRepair);
	
	ACString _Serialize() const;
	bool _Unserialize(const ACHAR* szText);
};

///////////////////////////////////////////////////////////////////////////
//	
//	Class CECIvtrBook
//	
///////////////////////////////////////////////////////////////////////////

class CECIvtrBook : public CECIvtrItem
{
public:		//	Types

public:		//	Constructor and Destructor

	CECIvtrBook(int tid, int expire_date);
	CECIvtrBook(const CECIvtrBook& s);
	virtual ~CECIvtrBook();

public:		//	Attributes

public:		//	Operations

	//	Set item detail information
	virtual bool SetItemInfo(BYTE* pInfoData, int iDataLen);
	//	Get item icon file name
	virtual const char* GetIconFile();
	//	Get item name
	virtual const wchar_t* GetName();
	//	Clone item
	virtual CECIvtrItem* Clone() { return new CECIvtrBook(*this); }
	//	Get item cool time
	virtual int GetCoolTime(int* piMax=NULL);
	//	Check item use condition
	virtual bool CheckUseCondition();
	//	Get drop model for shown
	virtual const char * GetDropModel();

	const BOOK_ESSENCE* GetDBEssence() { return m_pDBEssence; }

protected:	//	Attributes

	//	Data in database
	BOOK_ESSENCE*		m_pDBEssence;

protected:	//	Operations

	//	Get item description text
	virtual const wchar_t* GetNormalDesc(bool bRepair);
};

///////////////////////////////////////////////////////////////////////////
//	
//	Class CECIvtrEquipSoul
//	
///////////////////////////////////////////////////////////////////////////

class CECIvtrEquipSoul : public CECIvtrItem
{
public:		//	Types

public:		//	Constructor and Destructor

	CECIvtrEquipSoul(int tid, int expire_date);
	CECIvtrEquipSoul(const CECIvtrEquipSoul& s);
	virtual ~CECIvtrEquipSoul();

public:		//	Attributes

public:		//	Operations

	//	Set item detail information
	virtual bool SetItemInfo(BYTE* pInfoData, int iDataLen);
	//	Get item icon file name
	virtual const char* GetIconFile();
	//	Get item name
	virtual const wchar_t* GetName();
	//	Clone item
	virtual CECIvtrItem* Clone() { return new CECIvtrEquipSoul(*this); }
	//	Get item cool time
	virtual int GetCoolTime(int* piMax=NULL);
	//	Check item use condition
	virtual bool CheckUseCondition();
	//	Get drop model for shown
	virtual const char * GetDropModel();

	const EQUIP_SOUL_ESSENCE* GetDBEssence() { return m_pDBEssence; }

protected:	//	Attributes

	//	Data in database
	EQUIP_SOUL_ESSENCE*		m_pDBEssence;

protected:	//	Operations

	//	Get item description text
	virtual const wchar_t* GetNormalDesc(bool bRepair);
	void BuildRaceReqDesc();
};

///////////////////////////////////////////////////////////////////////////
//	
//	Class CECIvtrSpecialNameItem
//	
///////////////////////////////////////////////////////////////////////////

class CECIvtrSpecialNameItem : public CECIvtrItem
{
public:		//	Types

public:		//	Constructor and Destructor

	CECIvtrSpecialNameItem(int tid, int expire_date);
	CECIvtrSpecialNameItem(const CECIvtrSpecialNameItem& s);
	virtual ~CECIvtrSpecialNameItem();

public:		//	Attributes

public:		//	Operations

	//	Set item detail information
	virtual bool SetItemInfo(BYTE* pInfoData, int iDataLen);
	//	Get item icon file name
	virtual const char* GetIconFile();
	//	Get item name
	virtual const wchar_t* GetName();
	//	Clone item
	virtual CECIvtrItem* Clone() { return new CECIvtrSpecialNameItem(*this); }
	//	Get item cool time
	virtual int GetCoolTime(int* piMax=NULL);
	//	Check item use condition
	virtual bool CheckUseCondition();
	//	Get drop model for shown
	virtual const char * GetDropModel();

	const SPECIAL_NAME_ITEM_ESSENCE* GetDBEssence() { return m_pDBEssence; }
	const IVTR_ESSENCE_SPECIALNAME& GetEssence(){ return m_Essence; }
protected:	//	Attributes

	//	Data in database
	SPECIAL_NAME_ITEM_ESSENCE*		m_pDBEssence;
	IVTR_ESSENCE_SPECIALNAME		m_Essence;
	ACHAR							m_strSpecialName[128];
protected:	//	Operations

	//	Get item description text
	virtual const wchar_t* GetNormalDesc(bool bRepair);
};

///////////////////////////////////////////////////////////////////////////
//	
//	Class CECIvtrGiftBag
//	
///////////////////////////////////////////////////////////////////////////

class CECIvtrGiftBag : public CECIvtrItem
{
public:		//	Types

public:		//	Constructor and Destructor

	CECIvtrGiftBag(int tid, int expire_date);
	CECIvtrGiftBag(const CECIvtrGiftBag& s);
	virtual ~CECIvtrGiftBag();

public:		//	Attributes

public:		//	Operations

	//	Set item detail information
	virtual bool SetItemInfo(BYTE* pInfoData, int iDataLen);
	//	Get item icon file name
	virtual const char* GetIconFile();
	//	Get item name
	virtual const wchar_t* GetName();
	//	Clone item
	virtual CECIvtrItem* Clone() { return new CECIvtrGiftBag(*this); }
	//	Get item cool time
	virtual int GetCoolTime(int* piMax=NULL);
	//	Check item use condition
	virtual bool CheckUseCondition();
	//	Get drop model for shown
	virtual const char * GetDropModel();

	const GIFT_BAG_ESSENCE* GetDBEssence() { return m_pDBEssence; }

protected:	//	Attributes

	//	Data in database
	GIFT_BAG_ESSENCE*		m_pDBEssence;
	int						m_iGenderReq;

protected:	//	Operations

	//	Get item description text
	virtual const wchar_t* GetNormalDesc(bool bRepair);
	void BuildGenderReqDesc();
};

///////////////////////////////////////////////////////////////////////////
//	
//	Class CECIvtrGiftBagLotteryDeliver
//	
///////////////////////////////////////////////////////////////////////////

class CECIvtrGiftBagLotteryDeliver : public CECIvtrItem
{
public:		//	Types
	
public:		//	Constructor and Destructor
	
	CECIvtrGiftBagLotteryDeliver(int tid, int expire_date);
	CECIvtrGiftBagLotteryDeliver(const CECIvtrGiftBagLotteryDeliver& s);
	virtual ~CECIvtrGiftBagLotteryDeliver();
	
public:		//	Attributes
	
public:		//	Operations
	
	//	Set item detail information
	virtual bool SetItemInfo(BYTE* pInfoData, int iDataLen);
	//	Get item icon file name
	virtual const char* GetIconFile();
	//	Get item name
	virtual const wchar_t* GetName();
	//	Clone item
	virtual CECIvtrItem* Clone() { return new CECIvtrGiftBagLotteryDeliver(*this); }
	//	Get item cool time
	virtual int GetCoolTime(int* piMax=NULL);
	//	Check item use condition
	virtual bool CheckUseCondition();
	//	Get drop model for shown
	virtual const char * GetDropModel();
	
	const GIFT_BAG_LOTTERY_DELIVER_ESSENCE* GetDBEssence() { return m_pDBEssence; }
	const IVTR_ESSENCE_GIFTBAT_LOTTERYDELIVER& GetEssence() { return m_Essence; }
	
protected:	//	Attributes
	
	//	Data in database
	GIFT_BAG_LOTTERY_DELIVER_ESSENCE*	m_pDBEssence;
	IVTR_ESSENCE_GIFTBAT_LOTTERYDELIVER	m_Essence;
	int						m_iGenderReq;
	
protected:	//	Operations
	
	//	Get item description text
	virtual const wchar_t* GetNormalDesc(bool bRepair);
	void BuildGenderReqDesc();
};

///////////////////////////////////////////////////////////////////////////
//	
//	Class CECIvtrLotteryTangYuan
//	
///////////////////////////////////////////////////////////////////////////

class CECIvtrLotteryTangYuan : public CECIvtrItem
{
public:		//	Types
	
public:		//	Constructor and Destructor
	
	CECIvtrLotteryTangYuan(int tid, int expire_date);
	CECIvtrLotteryTangYuan(const CECIvtrLotteryTangYuan& s);
	virtual ~CECIvtrLotteryTangYuan();
	
public:		//	Attributes
	
public:		//	Operations
	
	//	Set item detail information
	virtual bool SetItemInfo(BYTE* pInfoData, int iDataLen);
	//	Get item icon file name
	virtual const char* GetIconFile();
	//	Get item name
	virtual const wchar_t* GetName();
	//	Clone item
	virtual CECIvtrItem* Clone() { return new CECIvtrLotteryTangYuan(*this); }
	//	Get item cool time
	virtual int GetCoolTime(int* piMax=NULL);
	//	Check item use condition
	virtual bool CheckUseCondition() { return true; }
	//	Get drop model for shown
	virtual const char * GetDropModel();

	int GetLevel();
	
	const LOTTERY_TANGYUAN_ITEM_ESSENCE* GetDBEssence() { return m_pDBEssence; }
	const IVTR_ESSENCE_LOTTERY_TANGYUAN& GetEssence() { return m_Essence; }

	int ShowOneExp();
	
protected:	//	Attributes
	
	//	Data in database
	LOTTERY_TANGYUAN_ITEM_ESSENCE*	m_pDBEssence;
	IVTR_ESSENCE_LOTTERY_TANGYUAN	m_Essence;
	APtrList<int*> m_lstShowExp;
	
protected:	//	Operations
	
	//	Get item description text
	virtual const wchar_t* GetNormalDesc(bool bRepair);
	void BuildGenderReqDesc();
};


///////////////////////////////////////////////////////////////////////////
//	
//	Class CECIvtrVIPCard
//	
///////////////////////////////////////////////////////////////////////////

class CECIvtrVIPCard : public CECIvtrItem
{
public:		//	Types

public:		//	Constructor and Destructor

	CECIvtrVIPCard(int tid, int expire_date);
	CECIvtrVIPCard(const CECIvtrVIPCard& s);
	virtual ~CECIvtrVIPCard();

public:		//	Attributes

public:		//	Operations

	//	Set item detail information
	virtual bool SetItemInfo(BYTE* pInfoData, int iDataLen);
	//	Get item icon file name
	virtual const char* GetIconFile();
	//	Get item name
	virtual const wchar_t* GetName();
	//	Clone item
	virtual CECIvtrItem* Clone() { return new CECIvtrVIPCard(*this); }
	//	Get item cool time
	virtual int GetCoolTime(int* piMax=NULL);
	//	Check item use condition
	virtual bool CheckUseCondition();
	//	Get drop model for shown
	virtual const char * GetDropModel();

	const VIP_CARD_ESSENCE* GetDBEssence() { return m_pDBEssence; }

protected:	//	Attributes

	//	Data in database
	VIP_CARD_ESSENCE*		m_pDBEssence;

protected:	//	Operations

	//	Get item description text
	virtual const wchar_t* GetNormalDesc(bool bRepair);
};

///////////////////////////////////////////////////////////////////////////
//	
//	Class CECIvtrMercenaryCredential
//	
///////////////////////////////////////////////////////////////////////////

class CECIvtrMercenaryCredential : public CECIvtrItem
{
public:		//	Types

public:		//	Constructor and Destructor

	CECIvtrMercenaryCredential(int tid, int expire_date);
	CECIvtrMercenaryCredential(const CECIvtrMercenaryCredential& s);
	virtual ~CECIvtrMercenaryCredential();

public:		//	Attributes

public:		//	Operations

	//	Set item detail information
	virtual bool SetItemInfo(BYTE* pInfoData, int iDataLen);
	//	Get item icon file name
	virtual const char* GetIconFile();
	//	Get item name
	virtual const wchar_t* GetName();
	//	Clone item
	virtual CECIvtrItem* Clone() { return new CECIvtrMercenaryCredential(*this); }
	//	Get item cool time
	virtual int GetCoolTime(int* piMax=NULL);
	//	Check item use condition
	virtual bool CheckUseCondition();
	//	Get drop model for shown
	virtual const char * GetDropModel();

	const MERCENARY_CREDENTIAL_ESSENCE* GetDBEssence() { return m_pDBEssence; }
	const IVTR_ESSENCE_MERCENARY_CREDENTIAL& GetEssence() { return m_Essence; }

protected:	//	Attributes

	//	Data in database
	MERCENARY_CREDENTIAL_ESSENCE*		m_pDBEssence;
	IVTR_ESSENCE_MERCENARY_CREDENTIAL   m_Essence;
	ACString							m_strTerritoryName;
protected:	//	Operations

	//	Get item description text
	virtual const wchar_t* GetNormalDesc(bool bRepair);
};


///////////////////////////////////////////////////////////////////////////
//	
//	Class CECIvtrTelePortation
//	
///////////////////////////////////////////////////////////////////////////

class CECIvtrTelePortation : public CECIvtrItem
{
public:		//	Types

public:		//	Constructor and Destructor

	CECIvtrTelePortation(int tid, int expire_date);
	CECIvtrTelePortation(const CECIvtrTelePortation& s);
	virtual ~CECIvtrTelePortation();

public:		//	Attributes

public:		//	Operations

	//	Set item detail information
	virtual bool SetItemInfo(BYTE* pInfoData, int iDataLen);
	//	Get item icon file name
	virtual const char* GetIconFile();
	//	Get item name
	virtual const wchar_t* GetName();
	//	Clone item
	virtual CECIvtrItem* Clone() { return new CECIvtrTelePortation(*this); }
	//	Get item cool time
	virtual int GetCoolTime(int* piMax=NULL);
	//	Check item use condition
	virtual bool CheckUseCondition();
	//	Get drop model for shown
	virtual const char * GetDropModel();

	const TELEPORTATION_ESSENCE* GetDBEssence() { return m_pDBEssence; }
	const IVTR_TELEPORTATION_ESSENCE& GetEssence() { return m_Essence; }

protected:	//	Attributes

	//	Data in database
	TELEPORTATION_ESSENCE*		m_pDBEssence;
	IVTR_TELEPORTATION_ESSENCE  m_Essence;

protected:	//	Operations

	//	Get item description text
	virtual const wchar_t* GetNormalDesc(bool bRepair);
	void  AddValidTimeDesc();
};


///////////////////////////////////////////////////////////////////////////
//	
//	Class CECIvtrTelePortationStone
//	
///////////////////////////////////////////////////////////////////////////

class CECIvtrTelePortationStone : public CECIvtrItem
{
public:		//	Types

public:		//	Constructor and Destructor

	CECIvtrTelePortationStone(int tid, int expire_date);
	CECIvtrTelePortationStone(const CECIvtrTelePortationStone& s);
	virtual ~CECIvtrTelePortationStone();

public:		//	Attributes

public:		//	Operations

	//	Set item detail information
	virtual bool SetItemInfo(BYTE* pInfoData, int iDataLen);
	//	Get item icon file name
	virtual const char* GetIconFile();
	//	Get item name
	virtual const wchar_t* GetName();
	//	Clone item
	virtual CECIvtrItem* Clone() { return new CECIvtrTelePortationStone(*this); }
	//	Get item cool time
	virtual int GetCoolTime(int* piMax=NULL);
	//	Check item use condition
	virtual bool CheckUseCondition();
	//	Get drop model for shown
	virtual const char * GetDropModel();

	const TELEPORTATION_STONE_ESSENCE* GetDBEssence() { return m_pDBEssence; }

protected:	//	Attributes

	//	Data in database
	TELEPORTATION_STONE_ESSENCE*		m_pDBEssence;

protected:	//	Operations

	//	Get item description text
	virtual const wchar_t* GetNormalDesc(bool bRepair);
};


///////////////////////////////////////////////////////////////////////////
//	
//	Class CECIvtrUpgradeEquipStone
//	
///////////////////////////////////////////////////////////////////////////

class CECIvtrUpgradeEquipStone : public CECIvtrItem
{
public:		//	Types

public:		//	Constructor and Destructor

	CECIvtrUpgradeEquipStone(int tid, int expire_date);
	CECIvtrUpgradeEquipStone(const CECIvtrUpgradeEquipStone& s);
	virtual ~CECIvtrUpgradeEquipStone();

public:		//	Attributes

public:		//	Operations

	//	Set item detail information
	virtual bool SetItemInfo(BYTE* pInfoData, int iDataLen);
	//	Get item icon file name
	virtual const char* GetIconFile();
	//	Get item name
	virtual const wchar_t* GetName();
	//	Clone item
	virtual CECIvtrItem* Clone() { return new CECIvtrUpgradeEquipStone(*this); }
	//	Get item cool time
	virtual int GetCoolTime(int* piMax=NULL);
	//	Check item use condition
	virtual bool CheckUseCondition();
	//	Get drop model for shown
	virtual const char * GetDropModel();

	const UPGRADE_EQUIP_STONE_ESSENCE* GetDBEssence() { return m_pDBEssence; }

protected:	//	Attributes

	//	Data in database
	UPGRADE_EQUIP_STONE_ESSENCE*		m_pDBEssence;

protected:	//	Operations

	//	Get item description text
	virtual const wchar_t* GetNormalDesc(bool bRepair);
};

//////////////////////////////////////////////////////////////////////////
//	
//	Class CECIvtrBoothFigure
//	
///////////////////////////////////////////////////////////////////////////

class CECIvtrBoothFigure : public CECIvtrItem
{
public:		//	Types

public:		//	Constructor and Destructor

		CECIvtrBoothFigure(int tid, int expire_date);
		CECIvtrBoothFigure(const CECIvtrBoothFigure &s);
		~CECIvtrBoothFigure();
		
public:		//	Attributes

public:		//	Operations

	//	Set item detail information
	virtual	bool SetItemInfo(BYTE* pInfoData, int iDataLen);
	//	Get item icon file name
	virtual const char* GetIconFile();
	//	Get item name
	virtual const wchar_t* GetName();
	//	Clone item
	virtual CECIvtrItem* Clone() {return new CECIvtrBoothFigure(*this);}
	//	Get item cool time
	virtual int GetCoolTime(int* piMax = NULL);
	//	Get drop model for shown
	virtual const char* GetDropModel();

	const BOOTH_FIGURE_ITEM_ESSENCE* GetDBEssence() {return m_pDBEssence;}

protected:		//	Attributes

	//	Data in database
	BOOTH_FIGURE_ITEM_ESSENCE*			m_pDBEssence;

protected:		//Operations

	// Get item description text
	virtual const wchar_t* GetNormalDesc(bool bRepair);
};

//////////////////////////////////////////////////////////////////////////
//	
//	Class CECIvtrBoothFigure
//	
///////////////////////////////////////////////////////////////////////////

class CECIvtrFlagBuffItem : public CECIvtrItem
{
public:		//	Types
	
public:		//	Constructor and Destructor
	
	CECIvtrFlagBuffItem(int tid, int expire_date);
	CECIvtrFlagBuffItem(const CECIvtrFlagBuffItem &s);
	~CECIvtrFlagBuffItem();
	
public:		//	Attributes
	
public:		//	Operations
	
	//	Set item detail information
	virtual	bool SetItemInfo(BYTE* pInfoData, int iDataLen);
	//	Get item icon file name
	virtual const char* GetIconFile();
	//	Get item name
	virtual const wchar_t* GetName();
	//	Clone item
	virtual CECIvtrItem* Clone() {return new CECIvtrFlagBuffItem(*this);}
	//	Get item cool time
	virtual int GetCoolTime(int* piMax = NULL);
	//	Get drop model for shown
	virtual const char* GetDropModel();
	
	const FLAG_BUFF_ITEM_ESSENCE* GetDBEssence() {return m_pDBEssence;}
	
protected:		//	Attributes
	
	//	Data in database
	FLAG_BUFF_ITEM_ESSENCE*			m_pDBEssence;
	
protected:		//Operations
	
	// Get item description text
	virtual const wchar_t* GetNormalDesc(bool bRepair);
};

//////////////////////////////////////////////////////////////////////////
//	
//	Class CECIvtrAstrology
//	
///////////////////////////////////////////////////////////////////////////

class CECIvtrAstrology : public CECIvtrItem
{
public:		//	Types
	
public:		//	Constructor and Destructor
	
	CECIvtrAstrology(int tid, int expire_date);
	CECIvtrAstrology(const CECIvtrAstrology &s);
	~CECIvtrAstrology();
	
public:		//	Attributes
	
public:		//	Operations
	
	//	Set item detail information
	virtual	bool SetItemInfo(BYTE* pInfoData, int iDataLen);
	//	Get item icon file name
	virtual const char* GetIconFile();
	//	Get item name
	virtual const wchar_t* GetName();
	//	Clone item
	virtual CECIvtrItem* Clone() {return new CECIvtrAstrology(*this);}
	//	Get item cool time
	virtual int GetCoolTime(int* piMax = NULL);
	//	Get drop model for shown
	virtual const char* GetDropModel();
	
	const XINGZUO_ITEM_ESSENCE* GetDBEssence() {return m_pDBEssence;}
	const IVTR_ASTROLOGY_ESSENCE& GetEssence() {return m_Essence;}

	// 获取镶嵌要求的装备等级
	static int GetEquipLevelLimit(int level);

	// 获取评分颜色
	static int GetScoreColor(int score);
	
protected:		//	Attributes
	
	//	Data in database
	XINGZUO_ITEM_ESSENCE*			m_pDBEssence;
	IVTR_ASTROLOGY_ESSENCE			m_Essence;
	
protected:		//Operations
	
	// Get item description text
	virtual const wchar_t* GetNormalDesc(bool bRepair);
};


//////////////////////////////////////////////////////////////////////////
//	
//	Class CECIvtrAstrology
//	
///////////////////////////////////////////////////////////////////////////

class CECIvtrAstrologyEnergyItem : public CECIvtrItem
{
public:		//	Types
	
public:		//	Constructor and Destructor
	
	CECIvtrAstrologyEnergyItem(int tid, int expire_date);
	CECIvtrAstrologyEnergyItem(const CECIvtrAstrologyEnergyItem &s);
	~CECIvtrAstrologyEnergyItem();
	
public:		//	Attributes
	
public:		//	Operations
	
	//	Set item detail information
	virtual	bool SetItemInfo(BYTE* pInfoData, int iDataLen);
	//	Get item icon file name
	virtual const char* GetIconFile();
	//	Get item name
	virtual const wchar_t* GetName();
	//	Clone item
	virtual CECIvtrItem* Clone() {return new CECIvtrAstrologyEnergyItem(*this);}
	//	Get item cool time
	virtual int GetCoolTime(int* piMax = NULL);
	//	Get drop model for shown
	virtual const char* GetDropModel();
	
	const XINGZUO_ENERGY_ITEM_ESSENCE* GetDBEssence() {return m_pDBEssence;}
	
protected:		//	Attributes
	
	//	Data in database
	XINGZUO_ENERGY_ITEM_ESSENCE*			m_pDBEssence;
	
protected:		//Operations
	
	// Get item description text
	virtual const wchar_t* GetNormalDesc(bool bRepair);
};


//////////////////////////////////////////////////////////////////////////
//	
//	Class CECIvtrAgentCombined
//	2012-10-16 合并药剂  by zy
///////////////////////////////////////////////////////////////////////////

class CECIvtrAgentCombinedItem : public CECIvtrItem
{
public:		//	Types
	
public:		//	Constructor and Destructor
	
	CECIvtrAgentCombinedItem(int tid, int expire_date);
	CECIvtrAgentCombinedItem(const CECIvtrAgentCombinedItem &s);
	~CECIvtrAgentCombinedItem();
	
public:		//	Attributes
	
public:		//	Operations
	


	//	Get item icon file name
	virtual const char* GetIconFile();
	//	Get item name
	virtual const wchar_t* GetName();
	//	Clone item
	virtual CECIvtrItem* Clone() {return new CECIvtrAgentCombinedItem(*this);}
	//	Get item cool time
	virtual int GetCoolTime(int* piMax=NULL);

	//	Get drop model for shown
	virtual const char* GetDropModel();
		//	Set item detail information
	virtual bool SetItemInfo(BYTE* pInfoData, int iDataLen);
	virtual bool CheckUseCondition();
	const CASH_MEDIC_MERGE_ITEM_ESSENCE* GetDBEssence() { return m_pDBEssence; }
	const IVTR_ESSENCE_BOTTLE_ESSENCE& GetEssence() { return m_Essence; }
	int GetLevelRequirement() const { return m_iLevelReq; }
protected:		//	Attributes
	
	//	Data in database
	CASH_MEDIC_MERGE_ITEM_ESSENCE*			m_pDBEssence;
	IVTR_ESSENCE_BOTTLE_ESSENCE m_Essence;
protected:		//Operations
	
	int						m_iLevelReq;
	// Get item description text
	virtual const wchar_t* GetNormalDesc(bool bRepair);
};

//////////////////////////////////////////////////////////////////////////
//	
//	Class CECSalePromotionItem
//	台历道具
///////////////////////////////////////////////////////////////////////////

class CECSalePromotionItem : public CECIvtrItem
{
public:		//	Types
	
public:		//	Constructor and Destructor
	
	CECSalePromotionItem(int tid, int expire_date);
	CECSalePromotionItem(const CECSalePromotionItem &s);
	~CECSalePromotionItem();
	
public:		//	Attributes
	
public:		//	Operations
	
	//	Set item detail information
	virtual	bool SetItemInfo(BYTE* pInfoData, int iDataLen);
	//	Get item icon file name
	virtual const char* GetIconFile();
	//	Get item name
	virtual const wchar_t* GetName();
	//	Clone item
	virtual CECIvtrItem* Clone() {return new CECSalePromotionItem(*this);}
	//	Get item cool time
	virtual int GetCoolTime(int* piMax = NULL);
	//	Get drop model for shown
	virtual const char* GetDropModel();
	
	const SALE_PROMOTION_ITEM_ESSENCE* GetDBEssence() {return m_pDBEssence;}
	const IVTR_SALE_PROMOTION_ITEM_ESSENCE& GetEssence() { return m_Essence; }
	
protected:		//	Attributes
	
	//	Data in database
	SALE_PROMOTION_ITEM_ESSENCE*			m_pDBEssence;
	IVTR_SALE_PROMOTION_ITEM_ESSENCE		m_Essence;
	
protected:		//Operations
	
	// Get item description text
	virtual const wchar_t* GetNormalDesc(bool bRepair);
};

//////////////////////////////////////////////////////////////////////////
//	
//	Class CECIvtrGiftPack
//	消费大礼包
///////////////////////////////////////////////////////////////////////////

class CECIvtrGiftPack : public CECIvtrItem
{
public:		//	Types
	
public:		//	Constructor and Destructor
	
	CECIvtrGiftPack(int tid, int expire_date);
	CECIvtrGiftPack(const CECIvtrGiftPack &s);
	~CECIvtrGiftPack();
	
public:		//	Attributes
	
public:		//	Operations
	
	//	Set item detail information
	virtual	bool SetItemInfo(BYTE* pInfoData, int iDataLen);
	//	Get item icon file name
	virtual const char* GetIconFile();
	//	Get item name
	virtual const wchar_t* GetName();
	//	Clone item
	virtual CECIvtrItem* Clone() {return new CECIvtrGiftPack(*this);}
	//	Get drop model for shown
	virtual const char* GetDropModel();
	
	const GIFT_PACK_ITEM_ESSENCE* GetDBEssence() {return m_pDBEssence;}
	const IVTR_GIFT_PACK_ESSENCE& GetEssence() { return m_Essence; }
	
protected:		//	Attributes
	
	//	Data in database
	GIFT_PACK_ITEM_ESSENCE*			m_pDBEssence;
	IVTR_GIFT_PACK_ESSENCE			m_Essence;
	
protected:		//Operations
	
	// Get item description text
	virtual const wchar_t* GetNormalDesc(bool bRepair);
};


//////////////////////////////////////////////////////////////////////////
//	
//	Class CECIvtrPropAddMaterial
//	基础属性增益丹药合成材料
///////////////////////////////////////////////////////////////////////////

class CECIvtrPropAddMaterial : public CECIvtrItem
{
public:		//	Types
	
public:		//	Constructor and Destructor
	
	CECIvtrPropAddMaterial(int tid, int expire_date);
	CECIvtrPropAddMaterial(const CECIvtrPropAddMaterial &s);
	~CECIvtrPropAddMaterial();
	
public:		//	Attributes
	
public:		//	Operations
	
	//	Get item icon file name
	virtual const char* GetIconFile();
	//	Get item name
	virtual const wchar_t* GetName();
	//	Clone item
	virtual CECIvtrItem* Clone() {return new CECIvtrPropAddMaterial(*this);}
	//	Get drop model for shown
	virtual const char* GetDropModel();
	
	const PROP_ADD_MATERIAL_ITEM_ESSENCE* GetDBEssence() {return m_pDBEssence;}
	
protected:		//	Attributes
	
	//	Data in database
	PROP_ADD_MATERIAL_ITEM_ESSENCE*			m_pDBEssence;
	
protected:		//Operations
	
	// Get item description text
	virtual const wchar_t* GetNormalDesc(bool bRepair);
};

//////////////////////////////////////////////////////////////////////////
//	
//	Class CECIvtrPropAddItem
//	基础属性增益丹药
///////////////////////////////////////////////////////////////////////////

class CECIvtrPropAddItem : public CECIvtrItem
{
public:		//	Types
	
public:		//	Constructor and Destructor
	
	CECIvtrPropAddItem(int tid, int expire_date);
	CECIvtrPropAddItem(const CECIvtrPropAddItem &s);
	~CECIvtrPropAddItem();
	
public:		//	Attributes
	
public:		//	Operations
	
	//	Get item icon file name
	virtual const char* GetIconFile();
	//	Get item name
	virtual const wchar_t* GetName();
	//	Clone item
	virtual CECIvtrItem* Clone() {return new CECIvtrPropAddItem(*this);}
	//	Get drop model for shown
	virtual const char* GetDropModel();
	
	const PROP_ADD_ITEM_ESSENCE* GetDBEssence() {return m_pDBEssence;}
	
protected:		//	Attributes
	
	//	Data in database
	PROP_ADD_ITEM_ESSENCE*			m_pDBEssence;
	
protected:		//Operations
	
	// Get item description text
	virtual const wchar_t* GetNormalDesc(bool bRepair);
};

//////////////////////////////////////////////////////////////////////////
//	
//	Class CECIvtrRune2013
//	2013.5版本, 150-160级符文
///////////////////////////////////////////////////////////////////////////

class CECIvtrRune2013 : public CECIvtrItem
{
public:		//	Types
	
public:		//	Constructor and Destructor
	
	CECIvtrRune2013(int tid, int expire_date);
	CECIvtrRune2013(const CECIvtrRune2013 &s);
	~CECIvtrRune2013();
	
public:		//	Attributes
	
public:		//	Operations
	//	Set item detail information
	virtual bool SetItemInfo(BYTE* pInfoData, int iDataLen);
	//	Get item icon file name
	virtual const char* GetIconFile();
	//	Get item name
	virtual const wchar_t* GetName();
	//	Clone item
	virtual CECIvtrItem* Clone() {return new CECIvtrRune2013(*this);}
	//	Get drop model for shown
	virtual const char* GetDropModel();
	
	const RUNE2013_ITEM_ESSENCE* GetDBEssence() {return m_pDBEssence;}
	const IVTR_FUWEN_ESSENCE& GetEssence() { return m_Essence; }

	// 当前等级
	int GetLevel();

	// 获取下一级经验, -1表示已满
	int GetNextLevelExp();

	// 获取最高级别经验
	int GetMaxLevelExp();

	// 获取属性字符串
	ACString GetPropString();
	
protected:		//	Attributes
	
	//	Data in database
	RUNE2013_ITEM_ESSENCE*			m_pDBEssence;
	IVTR_FUWEN_ESSENCE				m_Essence;
	
protected:		//Operations
	
	// Get item description text
	virtual const wchar_t* GetNormalDesc(bool bRepair);
};



//////////////////////////////////////////////////////////////////////////
//	
//	Class CECIvtrAgentCombined
//	2013-7-15 染色合计  by zy
///////////////////////////////////////////////////////////////////////////

class CECIvtrIndividualityItem : public CECIvtrItem
{
public:		//	Types

public:		//	Constructor and Destructor

	CECIvtrIndividualityItem(int tid, int expire_date);
	CECIvtrIndividualityItem(const CECIvtrIndividualityItem &s);
	~CECIvtrIndividualityItem();

public:		//	Attributes

public:		//	Operations



	//	Get item icon file name
	virtual const char* GetIconFile();
	//	Get item name
	virtual const wchar_t* GetName();
	//	Clone item
	virtual CECIvtrItem* Clone() {return new CECIvtrIndividualityItem(*this);}

	//	Get drop model for shown
	virtual const char* GetDropModel();
	//	Set item detail information
	virtual bool SetItemInfo(BYTE* pInfoData, int iDataLen);
	virtual bool CheckUseCondition();
	const COLORANT_ITEM_ESSENCE* GetDBEssence() { return m_pDBEssence; }
protected:		//	Attributes

	//	Data in database
	COLORANT_ITEM_ESSENCE*			m_pDBEssence;
protected:		//Operations
	// Get item description text
	virtual const wchar_t* GetNormalDesc(bool bRepair);
};
