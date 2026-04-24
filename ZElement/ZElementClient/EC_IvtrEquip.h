/*
 * FILE: EC_IvtrEquip.h
 *
 * DESCRIPTION: 
 *
 * CREATED BY: Duyuxin, 2004/11/19
 *
 * HISTORY: 
 *
 * Copyright (c) 2004 Archosaur Studio, All Rights Reserved.
 */

#pragma once

#include "EC_IvtrItem.h"
#include "EC_IvtrTypes.h"

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

struct EQUIPMENT_ESSENCE;
struct EQUIPMENT_SUB_TYPE;
struct EQUIPMENT_MAJOR_TYPE;

namespace GNET
{
	class Octets;
}

///////////////////////////////////////////////////////////////////////////
//	
//	Declare of Global functions
//	
///////////////////////////////////////////////////////////////////////////


///////////////////////////////////////////////////////////////////////////
//	
//	Class CECIvtrEquip
//	
///////////////////////////////////////////////////////////////////////////

class CECIvtrEquip : public CECIvtrItem
{
public:		//	Types

	//	Equipment type
	enum
	{
		EQUIP_UNKNOWN = -1,
		EQUIP_WEAPON = 0,
		EQUIP_ARMOR,
		EQUIP_DECORATION,
		EQUIP_FASHION,
		EQUIP_WING,
		EQUIP_TALISMAN_EXT,
		EQUIP_SPEAKER,
	};

	struct Level2Addon 
	{
		int level;
		int addon;
	};

public:		//	Constructor and Destructor

	CECIvtrEquip(int tid, int expire_date);
	CECIvtrEquip(const CECIvtrEquip& s);
	virtual ~CECIvtrEquip();

public:		//	Attributes

public:		//	Operations

	//	Set item detail information
	virtual bool SetItemInfo(BYTE* pInfoData, int iDataLen);
	virtual GNET::Octets gen_item_info();
	//	Get scaled item
	virtual INT64 GetScaledPrice();
	//	Get item icon file name
	virtual const char* GetIconFile();
	//	Get item name
	virtual const wchar_t* GetName();
	//	Clone item
	virtual CECIvtrItem* Clone() { return new CECIvtrEquip(*this); }
	//	Get item description for booth buying
	virtual const wchar_t* GetBoothBuyDesc();

	//	Get equip requirement
	int GetLevelRequirement() const;
	UINT64 GetProfessionRequirement() const { return m_iProfReq; }
	UINT64 GetProfessionRequirement2() const { return m_iProfReq2; } // 职业ID扩展后新增加字段Added 2011-08-04.
	int GetGenderRequirement() const { return m_iGenderReq; }
	int GetRebornRequirement() const { return m_iRebornReq; }
	int GetStrengthRequirement() const { return 0; }
	int GetAgilityRequirement() const { return 0; }
	int GetVitalityRequirement() const { return 0; }
	int GetEnergyRequirement() const { return 0; }
	//	Get current endurance
	int GetCurEndurance() const { return 1; }
	//	Get maximum endurance
	int GetMaxEndurance() const { return 1; }
	//	Add current endurance
	int AddCurEndurance(int iValue) { return 1; }
	//	Add deadly strike rate provided by this equipment
	int GetDeadlyStrikeRate() { return 0; }
	//	Check whether this equipment belongs to a suite
	int GetSuiteID();

	//	Is thie item need repairing ?
	bool NeedRepair() { return false; }
	//	Repair
	void Repair() {}
	//	Get raw repair cost
	virtual float GetRawRepairCost() const { return 0.0f; }
	//	Get repair cost
	virtual int GetRepairCost() const { return 0; }
	//	Get equipment type
	virtual int GetEquipmentType() const;

	//	Get hole number
	int GetHoleNum() { return 0; }
	//	Get empty hole number
	int GetEmptyHoleNum() { return 0; }
	//	Get item id in specified hole
	int GetHoleItem(int n) { return 0; }
	//	Get maker info
	const ACHAR* GetMakerName() const { return m_strMaker; }
	
	void UpdateSpritePower(short dec, int result);
	
	bool IsWeapon() const { return GetEquipmentType() == EQUIP_WEAPON; }
	bool IsArmor() const { return GetEquipmentType() == EQUIP_ARMOR; }
	bool IsDecoration() const { return GetEquipmentType() == EQUIP_DECORATION; }
	bool IsFashion() const { return GetEquipmentType() == EQUIP_FASHION; }
	bool IsTalismanMain() const { return GetClassID() == ICID_TALISMAN_MAINPART ; }
	bool IsRune() const { return GetClassID() == ICID_RUNE; }
//	bool IsTalismanExt() const { return GetEquipmentType() == EQUIP_TALISMAN_EXT; }
	bool IsSpeaker() const { return GetEquipmentType() == EQUIP_SPEAKER; }
	bool IsChangeShapeCard() const { return GetClassID() == ICID_CHANGE_SHAPE_CARD; }
	//	Get essence data
	const IVTR_ESSENCE_EQUIPMENT_NEW& GetNewEssence() { return m_Essence; }
	const IVTR_ESSENCE_EQUIPMENT& GetEssence(){ return m_Essence.old_data; }
	const IVTR_ESSENCE_EQUIPMENT_NEW_FOR_GEM& GetNewEssenceForGem() { return m_Essence; }
	const IVTR_ESSENCE_EQUIPMENT_NEW_FOR_ASTROLOGY& GetNewEssenceForAstrology() { return m_Essence; }

	//	Get database data
	const EQUIPMENT_MAJOR_TYPE* GetDBMajorType() const { return m_pDBMajorType; }
	const EQUIPMENT_SUB_TYPE* GetDBSubType() const { return m_pDBSubType; }
	const EQUIPMENT_ESSENCE* GetDBEssence() const { return m_pDBEssence; }

	//Added by Ford.W 2010-08-10.
	//for mini tool of xunbao173.
	virtual int GetEstone(){ return m_Essence.old_data.num_estone; };
	//Added end.

protected:	//	Attributes

	EQUIPMENT_MAJOR_TYPE*	m_pDBMajorType;
	EQUIPMENT_SUB_TYPE*		m_pDBSubType;
	EQUIPMENT_ESSENCE*		m_pDBEssence;	//	Equipment essence

	IVTR_ESSENCE_EQUIPMENT_NEW_FOR_ASTROLOGY	m_Essence;		// equipment essence data from server //Modified 2012-08-27.

	int			m_iLevelReq;			//	Level requirement
	UINT64		m_iProfReq;				//	Profession requirement
	UINT64		m_iProfReq2;			//	Profession requirement -->职业ID扩展后，需要再添加一个变量来满足掩码需求 Added 2011-07-18.
	int			m_iGenderReq;			//	Gender requirement
	int			m_iRaceReq;				//  Race requirement
	ACString	m_strMaker;				//	Maker's name
	int			m_iLevelReqDrop;		//	Drop of level requirement
	int			m_iRebornReq;			//  Reborn count requirement

protected:	//	Operations

	//	Add price description
	virtual void AddPriceDesc(int col, bool bRepair);
	//	Decide equipment name color
	virtual int DecideNameCol();	
	//	Get item description text
	virtual const wchar_t* GetNormalDesc(bool bRepair);
	//	Get drop model for shown
	virtual const char * GetDropModel();
	
	//	Build all description for equipment
	const wchar_t* GetEquipmentDesc(bool bRepair);
	//	Convert float percent property parameter to int value
	int VisualizeFloatPercent(int p) { return (int)(*(float*)&p * 100.0f + 0.5f); }
	//	Convert endurance real value to displaying value
	int VisualizeEndurance(int v) { return 1; }
	//	Build add-ons properties description
	void BuildAddOnPropDesc();
	//	Build gender requirement description
	void BuildGenderReqDesc();
	//	Get item description text for suite information
	void AddSuiteDesc();
	//  器魄套装的激活属性加成描述
	void AddSoulSuiteDesc();
	//  打孔、星座属性加成描述
	void AddAstrologyDesc();
	//  血祭附加属性
	void AddBleedSacrificeDesc(int val);
	//  镶嵌器魄后的属性加成
	void AddSoulEmbedDesc();
	// Build race requirement description
	void BuildRaceReqDesc();
	//  隐藏属性的显示
	void BuildHiddenAddOn();
	//  社稷需求
	void BuildTerrReqDesc();
	virtual ACString _Serialize() const;
	virtual bool _Unserialize(const ACHAR* szText);

public:
	//	Get Addon desc
	static const wchar_t* GetEquipAddonDesc(int idAddon, int typeAddOn, int param1, int param2, int param3, int count=1);
	//	Enhanced equip with add on
	static void EnhanceWithEquipAddOn(EQUIPMENT_ESSENCE& essence, int typeAddOn, int value);
};

///////////////////////////////////////////////////////////////////////////
//	
//	Inline functions
//	
///////////////////////////////////////////////////////////////////////////


