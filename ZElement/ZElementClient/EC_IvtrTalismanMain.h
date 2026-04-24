/*
 * FILE: EC_IvtrTalismanMain.h
 *
 * DESCRIPTION: 
 *
 * CREATED BY: Hedi, 2007/4/17
 *
 * HISTORY: 
 *
 * Copyright (c) 2007 Archosaur Studio, All Rights Reserved.
 */

#pragma once

#include "EC_IvtrItem.h"
#include "EC_IvtrEquip.h"
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

struct TALISMAN_MAINPART_ESSENCE;
struct SPEAKER_ESSENCE;
struct CHANGE_SHAPE_CARD_ESSENCE;

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
//	Class CECIvtrTalismanMain
//	
///////////////////////////////////////////////////////////////////////////

class CECIvtrTalismanMain : public CECIvtrEquip
{
public:		//	Types

public:		//	Constructor and Destructor

	CECIvtrTalismanMain(int tid, int expire_date);
	CECIvtrTalismanMain(const CECIvtrTalismanMain& s);
	virtual ~CECIvtrTalismanMain();
	
	enum Param_Type
	{
		TYPE_REFINE_LEVEL	= 0,	// 法宝技能等级。对应脚本中96，以下顺延	// 弃用
		TYPE_REFINE_0	,			// 技能id，等于m_Essence.addons[0]		// 弃用
		TYPE_REFINE_1	,			// 影响TYPE_REFINE_0的8个技能id
		TYPE_REFINE_2	,
		TYPE_REFINE_3	,
		TYPE_REFINE_4	,
		TYPE_REFINE_5	,
		TYPE_REFINE_6	,
		TYPE_REFINE_7	,
		TYPE_REFINE_8	,
		TYPE_REFINE_TMP_1,			// 法宝融合后生成的技能id，玩家可选择是否接受
		TYPE_REFINE_TMP_2,
		TYPE_REFINE_TMP_3,
		TYPE_REFINE_TMP_4,
		TYPE_REFINE_TMP_5,
		TYPE_REFINE_TMP_6,
		TYPE_REFINE_TMP_7,
		TYPE_REFINE_TMP_8,
		TYPE_NEWADDON_ID_1,			// 114	镶嵌的技能，addon id
		TYPE_NEWADDON_ID_2,
		TYPE_NEWADDON_ID_3,
		TYPE_NEWADDON_ID_4,
		TYPE_NEWADDON_ID_5,
		TYPE_REBORN_COUNT,			// 119  飞升次数
	};

public:		//	Attributes

public:		//	Operations

	//	Set item detail information
	virtual bool SetItemInfo(BYTE* pInfoData, int iDataLen);
	// 仿照服务器的处理方式
	bool SetItemFromMailInfo(BYTE* pInfoData, int iDataLen);
	virtual GNET::Octets gen_item_info();
	//	Get scaled item
	virtual INT64 GetScaledPrice();
	//	Get item icon file name
	virtual const char* GetIconFile();
	//	Get item name
	virtual const wchar_t* GetName();
	//	Clone item
	virtual CECIvtrItem* Clone() { return new CECIvtrTalismanMain(*this); }
	//	Get item description for booth buying
	virtual const wchar_t* GetBoothBuyDesc();
	//	Get equipment type
	virtual int GetEquipmentType() const { return CECIvtrEquip::EQUIP_UNKNOWN; }

	void SetNewExp(int new_exp) { m_Essence.ess.exp = new_exp; UpdateInfoMD5(); }
	int GetRebornCount() { return m_Essence.addons2012.size()>TYPE_REBORN_COUNT ? m_Essence.addons2012[TYPE_REBORN_COUNT]:0; }
	int GetInfo(int index) { return (int)m_Essence.addons2012.size()>index ? m_Essence.addons2012[index]:0; }
	int GetFixSkill();
	int GetFixSkillLevel();

	void BuildNewAddOnPropDesc();

	//	Get essence data
	const IVTR_ESSENCE_TALISMAN_MAIN& GetEssence() { return m_Essence; }
	const TALISMAN_MAINPART_ESSENCE* GetDBEssence() const { return m_pDBEssence; }

protected:	//	Attributes
	TALISMAN_MAINPART_ESSENCE*	m_pDBEssence;	// Equipment essence
	IVTR_ESSENCE_TALISMAN_MAIN	m_Essence;		// talisman mainpart essence data from server

protected:	//	Operations
	//	Add price description
	virtual void AddPriceDesc(int col, bool bRepair);
	//	Build add-ons properties description
	void BuildAddOnPropDesc();
	//	Decide equipment name color
	virtual int DecideNameCol();
	//	Get item description text
	virtual const wchar_t* GetNormalDesc(bool bRepair);
	//	Get drop model for shown
	virtual const char * GetDropModel();
	
	ACString _Serialize() const;
	bool _Unserialize(const ACHAR* szText);
};


///////////////////////////////////////////////////////////////////////////
//	
//	Class CECIvtrSpeaker
//	
///////////////////////////////////////////////////////////////////////////

class CECIvtrSpeaker : public CECIvtrEquip
{
public:		//	Types

public:		//	Constructor and Destructor

	CECIvtrSpeaker(int tid, int expire_date);
	CECIvtrSpeaker(const CECIvtrSpeaker& s);
	virtual ~CECIvtrSpeaker();

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
	virtual CECIvtrItem* Clone() { return new CECIvtrSpeaker(*this); }
	//	Get item description for booth buying
	virtual const wchar_t* GetBoothBuyDesc();
	//	Get equipment type
	virtual int GetEquipmentType() const { return CECIvtrEquip::EQUIP_SPEAKER; }

	//	Get essence data
	const SPEAKER_ESSENCE* GetDBEssence() const { return m_pDBEssence; }

protected:	//	Attributes
	SPEAKER_ESSENCE*	m_pDBEssence;	// Equipment essence
	
protected:	//	Operations
	//	Add price description
	virtual void AddPriceDesc(int col, bool bRepair);
	//	Build add-ons properties description
	void BuildAddOnPropDesc();
	//	Decide equipment name color
	virtual int DecideNameCol();
	//	Get item description text
	virtual const wchar_t* GetNormalDesc(bool bRepair);
	//	Get drop model for shown
	virtual const char * GetDropModel();
	
	ACString _Serialize() const;
	bool _Unserialize(const ACHAR* szText);
};


///////////////////////////////////////////////////////////////////////////
//	
//	Class CECIvtrChangeShapeCard
//	
///////////////////////////////////////////////////////////////////////////

class CECIvtrChangeShapeCard : public CECIvtrEquip
{
public:		//	Types

public:		//	Constructor and Destructor

	CECIvtrChangeShapeCard(int tid, int expire_date);
	CECIvtrChangeShapeCard(const CECIvtrChangeShapeCard& s);
	virtual ~CECIvtrChangeShapeCard();

public:		//	Attributes

public:		//	Operations

	//	Set item detail information
	virtual bool SetItemInfo(BYTE* pInfoData, int iDataLen);
	//	Get item icon file name
	virtual const char* GetIconFile();
	//	Get item name
	virtual const wchar_t* GetName();
	//	Clone item
	virtual CECIvtrItem* Clone() { return new CECIvtrChangeShapeCard(*this); }
	//	Get item cool time
	virtual int GetCoolTime(int* piMax=NULL);
	//	Check item use condition
	virtual bool CheckUseCondition();
	//	Get drop model for shown
	virtual const char * GetDropModel();

	const CHANGE_SHAPE_CARD_ESSENCE* GetDBEssence() { return m_pDBEssence; }
	const IVTR_ESSENCE_CHANGE_SAHPE_CARD& GetEssence() {return m_Essence; }

	void UpdateDuration(int dec, int result );

	void UpdataExp(int newexp);

protected:	//	Attributes

	//	Data in database
	CHANGE_SHAPE_CARD_ESSENCE*		m_pDBEssence;
	IVTR_ESSENCE_CHANGE_SAHPE_CARD	m_Essence;
	unsigned int					m_iMonsterFaction;
protected:	//	Operations

	//	Get item description text
	virtual const wchar_t* GetNormalDesc(bool bRepair);
	void BuildAddOnPropDesc();
	virtual int DecideNameCol();	
	ACString _Serialize() const;
	bool _Unserialize(const ACHAR* szText);
};

///////////////////////////////////////////////////////////////////////////
//	
//	Inline functions
//	
///////////////////////////////////////////////////////////////////////////


