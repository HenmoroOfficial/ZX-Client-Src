/********************************************************************
	created:	2010/7/8
	author:		lihengju
	
	purpose:	try to rewrite the helper in c++, then we can seal the interfaces for the lua
	Copyright (C) 2006 - All Rights Reserved
*********************************************************************/

#pragma once

#include "EC_Counter.h"
#include "a3dvector.h"
#include <AWString.h>
#include <vector.h>
//#include "EC_Helper.h"

class CMonsterSelector;
class CMatterSelector;


class CECAutoHelper
{
public:	// Types
	enum
	{
		Cnt_HealHP = 0,
		Cnt_HealMP,
		Cnt_HealDP,
		Cnt_HealStamina,
		Cnt_SelectMonster,
		Cnt_PickUp,
		Cnt_UseSkill1,
		Cnt_UseSkill2,
		Cnt_UseSkill3,
		Cnt_UseSkill4,
		Cnt_UseSkill5,
		Cnt_UseSkill6,
		Cnt_UseSkill7,
		Cnt_UseSkill8,
		Cnt_UseSkill9,
		Cnt_UseItem1,
		Cnt_UseItem2,
		Cnt_UseItem3,
		Cnt_ReportCnt,
		Cnt_AutoMove,
		Cnt_Obstacle,
		NUM_COUNTER,
	};
	
public: // Constructor and Destructor
	CECAutoHelper();
	~CECAutoHelper();


public: // Attributes
	
public:	// Operations

	bool Init();
	void Release();
	void Reset();
	
	void Tick(DWORD dwDelta);;
	void Start();
	void Stop()  {m_bRunning = false;}
	bool IsRunning() const {return m_bRunning;}

	int  GetHpDelay();
	void SetHpDelay(int delay);
	int  GetHpRate() { return m_iHpRate;}
	void SetHpRate(int rate){ m_iHpRate = rate; }

	int  GetMpDelay(){ return m_Counters[Cnt_HealMP].GetPeriod(); }
	void SetMpDelay(int delay){ m_Counters[Cnt_HealMP].SetPeriod(delay); }
	int  GetMpRate() { return m_iMpRate; }
	void SetMpRate(int rate) { m_iMpRate = rate; }
	
	int  GetDpDelay(){ return m_Counters[Cnt_HealDP].GetPeriod(); }
	void SetDpDelay(int delay){ m_Counters[Cnt_HealDP].SetPeriod(delay); }
	int  GetDpRate() { return m_iDpRate; }
	void SetDpRate(int rate) { m_iDpRate = rate; }

	//index from 1 to 7
	int  GetSkillDelay(int index) { return m_Counters[Cnt_UseSkill1+index-1].GetPeriod(); }
	void SetSkillDelay(int index, int delay) { m_Counters[Cnt_UseSkill1+index-1].SetPeriod(delay); }
	
	bool IsSkillDisabled() { return m_bSkillDisabled; }
	void SetSkillDisabled(bool bDisabled) { m_bSkillDisabled = bDisabled; }

	CMatterSelector * GetMatterSelector() const {return m_pMatterSelector;}
	CMonsterSelector * GetMonsterSelector() const{return m_pMonsterSelector;}
	
	bool IsAnswerQuestion() const {return m_bAnswerQuestion;}
	void SetAnswerQuestion(bool bAnswer) {m_bAnswerQuestion = bAnswer;}
	
	int GetAnchorRange(){ return m_iCurRangeChoosen; }
	void SetAnchorRange(int range);

	void SetTransferEnable(bool bEnable){ m_bTransferEnable = bEnable; }
	bool GetTransferEnable() { return m_bTransferEnable; }
	
	int GetUseItemDelay(int index) { return m_Counters[Cnt_UseItem1+index-1].GetPeriod(); }
	void SetUseItemDelay(int index, int delay) { m_Counters[Cnt_UseItem1+index-1].SetPeriod(delay); }

	bool Load(int playerId);
	bool Save(int playerId);
	
//	bool IsWatchSkill() { return m_bWatchSkill; } 
	void SetLiuDaoSkillCnt(bool bRecnt){ m_LiuDaoSkillCnt.Reset(bRecnt); };
	
protected:	// Operations

	bool IsPickingUp();
	bool IsAutoMoving();
	bool IsOperating();
	int	 GetDrops(float curRange);
	bool IsUsingSkill();
	bool IsPreparingSkill();
	void UseTalismanItem(int index);
	bool UseSkill();
	bool UseItem();
	void TickFly(DWORD dwDelta);
	void TickAuto(DWORD dwDelta);
	bool Heal();
	bool IsAttcking();
	void ProcessObstacleMonster(DWORD dwDelta);
	void SetSkillCooling();

private: // Attributes

	CECCounter  m_Counter;
	CECCounter  m_TransferCounter;	// 天人合一状态下，定期的将背包中的特定物品转移到随身包裹中
	CECCounter	m_PickUpCnt;		// 捡东西的计时器，捡东西的时间10s如果超出，则先做别的事情
	CECCounter  m_ObstacleCnt;		// 遇到障碍怪的情况
	CECCounter	m_PickUpAdjustCnt;	// 拾取物品失败后的调整计时
	CECCounter  m_LiuDaoSkillCnt;	// 六道类技能计时
	bool		m_bTransferEnable;	// 天人合一状态下，定期的将背包中的特定物品转移到随身包裹中功能是否开启
	bool        m_bRunning;
	bool        m_bAnswerQuestion;

	CMonsterSelector	* m_pMonsterSelector;
	CMatterSelector		* m_pMatterSelector;

	bool		m_bSkillDisabled;
	bool		m_bUsingSkill;
	bool		m_bPickingUp;
	bool		m_bAutoMoving;
	bool		m_bNewTarget;		// 刚刚选中了一个新的目标
	int			m_iCurStatus;		// 当前状态
	int			m_idTarget;			// 当前的目标
	int			m_iCurRangeChoosen;
	float		m_fCurRange;		// 当前选择的范围
	CECCounter	m_Counters[NUM_COUNTER];
	A3DVECTOR3  m_vStartPos;

	int			m_iHpRate;
	int			m_iMpRate;
	int			m_iDpRate;
	float		m_fStaminaRate;
	int			m_iCurTargetHp;		// 当前选中怪的血量
	int			m_iBadID;			// 障碍怪
	int         m_iPickupNum;		// 当前可以拾取的物品的数目
	int			m_iPickupTimes;		// 尝试捡起物品的次数，如果多次拾取失败，则停止拾取
	bool		m_bPickupAdjust;	// 因为拾取失败进行的时间上的调整，隔10s不再拾取
	bool		m_bCastASkill;		// 释放了一个技能，等一个tick，等待服务器的返回
	int			m_iWaitSkill;		// 等待技能返回的时间，避免重复释放冷却时间为0的技能
	
};