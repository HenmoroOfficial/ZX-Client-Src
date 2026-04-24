/*
 * FILE: EC_Skill.h
 *
 * DESCRIPTION: 
 *
 * CREATED BY: Duyuxin, 2005/1/17
 *
 * HISTORY: 
 *
 * Copyright (c) 2005 Archosaur Studio, All Rights Reserved.
 */

#pragma once

#include "ElementSkill.h"

///////////////////////////////////////////////////////////////////////////
//	
//	Define and Macro
//	
///////////////////////////////////////////////////////////////////////////

class CECIvtrTalismanMain;

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

class CECSkillStr : public GNET::SkillStr
{
public:

	virtual wchar_t* Find(int id) const;
};

///////////////////////////////////////////////////////////////////////////
//	
//	Class CECSkill
//	
///////////////////////////////////////////////////////////////////////////

class CECSkill
{
public:		//	Types

	//	Skill type
	enum
	{
		TYPE_ATTACK		= 1,  // 主动攻击
		TYPE_BLESS		= 2,  // 主动祝福
		TYPE_CURSE		= 3,  // 主动诅咒
		TYPE_JUMP		= 4,  // 轻功
		TYPE_FACTIONBLESS =5,	//阵营祝福
		TYPE_COMBINESKILL =6,	//组合技

//		TYPE_RUSH		= 6,  // 冲锋，冲锋在ISCHARGE里面判断，没有作为一个单独的技能类型存在

		TYPE_PASSIVE	= 11, // 被动
		TYPE_ENABLED	= 12, // 武器附加
		TYPE_RUNE		= 13, // 物品技能
		TYPE_LIVE		= 14, // 生产
	};

	//	Range type
	enum
	{
		RANGE_POINT = 0,	//	点 
		RANGE_LINE,			//	线 
		RANGE_SELFSPHERE,	//	自身为中心的球
		RANGE_TARGETSPHERE, //	目标为中心的球
		RANGE_TAPER,		//	圆锥
		RANGE_SLEF,			//	自身
	};

	// 技能的种类，是按照技能的构建的目录来区分的

	enum
	{
        OCCUPATION_TALISMAN     = 130, //法宝技能
        OCCUPATION_FAMILY       = 131, //家族技能
		OCCUPATION_REBORN       = 146, //转生技能
        OCCUPATION_SECT         = 147, //师徒技能
        OCCUPATION_TAOIST       = 148, //道心技能
        OCCUPATION_EVIL         = 149, //魔性技能
        OCCUPATION_BUDDHA       = 150, //佛缘技能
		OCCUPATION_EQUIP		= 151, //装备技能
        OCCUPATION_SPOUSE       = 152, //夫妻技能
		OCCUPATION_LIFE			= 166, //生活技能
	};

public:		//	Constructor and Destructor

	CECSkill(int id, int iLevel);
	virtual ~CECSkill();

public:		//	Attributes

public:		//	Operations

	//	Tick routine
	void Tick();
	//	Skill level up
	void LevelUp();

	//	Start into cooling state
	void StartCooling(int iTotalTime, int iStartCnt);
	//	Ready to be cast ?
	bool ReadyToCast() { return !m_bCooling; }
	//	Get cooling time counter
	int GetCoolingCnt() { return m_iCoolCnt; }
	//	Get total cooling time
	int GetCoolingTime() { return m_iCoolTime; }
	//	Start charging
	void StartCharging(int iChargeMax);
	//  Start chanting
	void StartChanting();
	//	End charging
	void EndCharging();
	//	Get charging flag
	bool IsCharging() { return m_bCharging; }
	//  Get chanting flag
	bool IsChanting() { return m_bChanting; }
	bool IsEmission() { return m_bEmission; }
	void SetStamp(char stamp) { m_cStamp = stamp; }
	char GetStamp() { return m_cStamp; }
	//	Get charging counter
	int GetChargingCnt() { return m_iChargeCnt; }
	//	Get charging maximum count
	int GetChargingMax() { return m_iChargeMax; }
	//	Change full
	bool ChargeFull();
	//	Check cast condition
	int CheckCastCondition(GNET::UseRequirement& req);

	//	Get skill ID
	int GetSkillID() { return m_idSkill; }
	//	Get skill level
	int GetSkillLevel();
	int GetSkillMaxLevel();
	void SetAddonLevel(int iLevel);
	int GetAddonLevel();
	void SetCommonLevel(int iLevel);
	void SetLevel(int lev) { m_pSkillCore->SetLevel(lev); }
	void SetProficiency(int prof) { m_pSkillCore->SetProficiency(prof);}
	//	Get skill icon file
	const char* GetIconFile() { return m_pSkillCore->GetIcon(); }
	const wchar_t* GetName();
	const wchar_t* GetDesc() { return GetDesc(GetSkillLevel()); }
	const wchar_t* GetDesc(int iLevel, bool bNextLv=true);
	const short* GetTalent() { return m_pSkillCore->GetTalent(); } 
	int GetExecuteTime() { return m_pSkillCore->GetExecuteTime(); }
	int GetType() { return m_pSkillCore->GetType(); }
	int GetCategory() { return m_pSkillCore->GetCategory(); }
	bool IsCategory(int iCategory) { return (GetCategory() & iCategory) == iCategory; }
	short GetTalentSize() { return m_pSkillCore->GetTalentSize(); } 
	int GetCoreCoolingTime() { return m_pSkillCore->GetCoolingTime(); }
	int GetRangeType() { return m_pSkillCore->GetRangeType(); }
	float GetCastRange(float fAtkDist) { return m_pSkillCore->GetPrayRange(fAtkDist); }
	int GetTargetType() { return m_pSkillCore->GetTargetType(); }
	int GetShowOrder() { return 0; }//return m_pSkillCore->GetShowOrder(); }
	bool IsChargeable() { return m_pSkillCore->IsWarmup(); }
	const char* GetNativeName() { return m_pSkillCore->GetNativeName(); }
	bool ValidWeapon(int idWeapon) { return true; } // return m_pSkillCore->ValidWeapon(idWeapon); }
	bool ValidShape(int iShape) { return true; } // return m_pSkillCore->IsValidForm((char)iShape); }
	bool ChangeToMelee() { return m_pSkillCore->IsAutoAttack(); }
	bool IsInstant() { return m_pSkillCore->IsInstant(); }
	bool IsDurative() { return m_pSkillCore->IsDurative(); }
	int GetJunior() { return 0; } // m_pSkillCore->GetJunior(); }
	bool IsExtra() { return m_bExtra; }
	void SetExtra(bool b) { m_bExtra = b; }
	bool IsUseProficiency() { return m_pSkillCore->IsUseProficiency(); }

	int GetRequiredLevel() { return m_pSkillCore->GetRequiredLevel(); }
	int GetRequiredBook() { return 0; } // m_pSkillCore->GetRequiredBook(); }
	int GetRequiredSkill() { return m_pSkillCore->GetRequiredSkill(); }
	//  释放技能需要消耗物品
	bool IsNeedItem() { return m_pSkillCore->IsNeedItem(); }
	int GetItem1Id(){ return m_pSkillCore->GetItem1Id(); }
	int GetItem2Id(){ return m_pSkillCore->GetItem2Id(); }
	int GetItem1Num(){ return m_pSkillCore->GetItem1Num(); }
	int GetItem2Num(){ return m_pSkillCore->GetItem2Num(); }
	int GetItemRelation(){ return m_pSkillCore->GetItemRelation();}
	bool IsCharge()  { return m_pSkillCore->IsCharge(); } //注意：这个接口只是判断该技能是否能瞬移-->不是真正的冲锋
	bool IsChargeToTarget() { return m_pSkillCore->IsChargeToTarget(); } // 判断该技能能否冲锋Added 2011-07-22.
	const char* GetCircleGfxName() { return m_pSkillCore->GetCycleGfx(); }	//	获得该技能的阵法特效名称，目前专用于阵法技能 Added 2011-08-02.
	float GetNoTargetSkillRadius() const { return m_pSkillCore->GetRadius(); }	//获得无目标释放技能的攻击半径 Added 2012-05-07.
	// 获取技能种类
	int GetSkillClass() { return m_pSkillCore->GetOccupation(); }
	// 组合技能
	int GetSkillElemsNum() { return m_pSkillCore->GetSkillElemsNum(); }
	int GetSkillElem(int index) { return m_pSkillCore->GetSkillElem(index); }
	int GetComboSkillCapacity(int index) { return m_pSkillCore->GetComboSkillCapacity(index); }
	
	bool IsCombineSkill() { return m_pSkillCore->IsComboSkill(); }
	
	void SetPlayerProps(GNET::PlayerProps& props);

	bool IsGuide() { return m_pSkillCore->IsGuide(); }	// 是否为引导类技能
	
	bool IsNoSummonLimitSkill() { return m_pSkillCore->IsNoSummonLimitSkill(); }	// 判断召唤兽限制技能
	int GetSummonLimitSkillId() { return m_pSkillCore->GetSummonLimitSkillId(); }

	float GetDarkLightType() { return m_pSkillCore->GetDarkLightType(); }
	bool IsCastInPreSkill() { return m_pSkillCore->IsCastInPreSkill(); }
	int GetPreCastSkillId() { return m_pSkillCore->GetPreCastSkillId(); }	

	int  GetMaxTargetCnt() { return m_pSkillCore->GetMaxTargetCnt(); }
	
	bool IsBlinkFaceDir() { return m_pSkillCore->IsBlinkFaceDir();}
	float GetChargeDist() { return m_pSkillCore->GetChargeDist();}
	
	float GetEffectDistance() { return m_pSkillCore->GetEffectdistance();}
	
	int  GetFollowSkill()	{ return m_pSkillCore->GetFollowSkill();}

	void SetItem(CECIvtrTalismanMain *pItem) { m_pItem=pItem; }

	bool CanCastTriggerSkill() { return m_bCanCastTriggerSkill;	}	//  能否释放永久冷却技能

	void SetTriggerSkillCnt(int iCnt) { m_iTriggerSkillCnt = iCnt;	}	// 设置永久冷却技能可用期时间

protected:	//	Attributes

	GNET::ElementSkill*	m_pSkillCore;

	int		m_idSkill;		//	Skill ID
	int		m_iCoolCnt;		//	Cooling time counter
	int		m_iCoolTime;	//	Total cooling time
	bool	m_bCooling;		//	In cooling state
	int		m_iChargeCnt;	//	Charging time counter
	int		m_iChargeMax;	//	Charging time maximum count value
	bool	m_bCharging;	//	In charging state
	bool	m_bChanting;	//  蓄力技能的正在吟唱
	bool	m_bEmission;	//  蓄力技能的最终释放阶段
	char	m_cStamp;
	bool	m_bExtra;

	// 2012.8.31 法宝技能描述依赖法宝的属性
	// 为了不修改以前接口添加
	ACString m_strDesc;
	CECIvtrTalismanMain	*m_pItem;
	
	int		m_iTriggerSkillCnt;		// 永久冷却技能可用期计时
	bool	m_bCanCastTriggerSkill;	// 能否释放永久冷却技能

protected:	//	Operations

};

///////////////////////////////////////////////////////////////////////////
//	
//	Inline functions
//	
///////////////////////////////////////////////////////////////////////////



