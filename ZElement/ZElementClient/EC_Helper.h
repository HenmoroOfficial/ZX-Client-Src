/********************************************************************
	created:	2007/04/28
	author:		kuiwu
	
	purpose:	the helper 
	Copyright (C) 2007 - All Rights Reserved
*********************************************************************/

#pragma once

#include "EC_Counter.h"
#include "a3dvector.h"
#include <AWString.h>
#include <vector.h>

class CLuaState;
class CMonsterSelector;
class CECMonster;
class CECMatter;

class CMatterSelector
{
public:
	enum
	{
		MATTER_ALL,		  //all
		MATTER_NONE,	  //none
		MATTER_INCLUDE,   //use include list
		MATTER_EXCLUDE,	  //use exclude list
	};

	typedef abase::vector<AWString>  IncludeList;
	typedef abase::vector<AWString>  ExcludeList;

	CMatterSelector();
	~CMatterSelector();
	
	IncludeList * GetIncludeList() 
	{
		return &m_IncludeList;
	}
	ExcludeList * GetExcludeList() 
	{
		return &m_ExcludeList;
	}

	bool IsTarget(CECMatter * pMatter);

	int  GetSelectType() const
	{
		return m_SelectType;
	}
	void SetSelectType(int t)
	{
		m_SelectType = t;
	}
	void SetAutoPick(int mask, int lev1, int lev2)
	{
		m_AutoPickMask = mask;
		m_iAutoPickLevel1 = lev1;
		m_iAutoPickLevel2 = lev2;
	}
	void GetAutoPick(int &mask, int &lev1, int &lev2)
	{
		mask = m_AutoPickMask;
		lev1 = m_iAutoPickLevel1;
		lev2 = m_iAutoPickLevel2;
	}
	void SetAutoUnPick(int mask, int lev1, int lev2)
	{
		m_AutoUnPickMask = mask;
		m_iAutoUnPickLevel1 = lev1;
		m_iAutoUnPickLevel2 = lev2;
	}
	void GetAutoUnPick(int &mask, int &lev1, int &lev2)
	{
		mask = m_AutoUnPickMask;
		lev1 = m_iAutoUnPickLevel1;
		lev2 = m_iAutoUnPickLevel2;
	}
	int GetCountLimit() const
	{
		return m_CountLimit;
	}

	bool Load(int playerId, int listType);
	bool Save(int playerId, int listType);
private:
	IncludeList  m_IncludeList;
	ExcludeList  m_ExcludeList;
	int          m_SelectType;
	int          m_CountLimit;
	int			 m_AutoPickMask;
	int			 m_iAutoPickLevel1;
	int			 m_iAutoPickLevel2;
	int			 m_AutoUnPickMask;
	int			 m_iAutoUnPickLevel1;
	int			 m_iAutoUnPickLevel2;
	
	bool _Find(abase::vector<AWString>& list, const AWString& str);
	bool IsAutoPickItem(int tid);
	bool IsNotAutoPickItem(int tid);
};

class CMonsterSelector
{
public:
	enum
	{
		//MONSTER_NONE   = 0x0,
		MONSTER_NORMAL = 0x1,
		MONSTER_ELITE  = 0x2,
		MONSTER_BOSS   = 0x4,
	};

	typedef abase::vector<AWString>  ExcludeList;
	CMonsterSelector();
	virtual ~CMonsterSelector();
	bool IsTarget(CECMonster * pMonster);

	int     GetTargetType() const {return m_TargetType;}
	void    SetTargetType(int type) {m_TargetType = type;}
	bool	IsFullHp() const {return m_bFullHp;}
	void    SetFullHp(bool bFull) {m_bFullHp = bFull;}
	bool    IsTaskOnly() const {return m_bTaskOnly;}
	void    SetTaskOnly(bool bTask) {m_bTaskOnly = bTask;}
	
	int		GetAttackRange() const { return m_iAttackRange; }
	void	SetAttackRange(int range) { m_iAttackRange = range; }

	ExcludeList * GetExcludeList() 
	{
		return &m_ExcludeList;
	}
	bool LoadMonsterList(int playerId);
	bool SaveMonsterList(int playerId);

	bool Load(int playerId);
	bool Save(int playerId);

private:
	int        m_TargetType;
	bool       m_bFullHp;
	bool       m_bTaskOnly;
	int		   m_iAttackRange;   // 攻击范围
	int		   m_CountLimit;
	ExcludeList  m_ExcludeList;
	
	bool _Find(abase::vector<AWString>& list, const AWString& str);
	bool       _IsLineClear(const A3DVECTOR3& start, const A3DVECTOR3& end);

};



class CECHelper  
{
public:
	CECHelper();
	virtual ~CECHelper();
	
	bool Init();
	void Release();
	void Reset();
	
	void Tick(DWORD dwDelta);;
	void Start();
	void Stop()  {m_bRunning = false;}
	bool IsRunning() const {return m_bRunning;}

	int  GetHpDelay();
	void SetHpDelay(int delay);
	int  GetHpRate();
	void SetHpRate(int rate);

	int  GetMpDelay();
	void SetMpDelay(int delay);
	int  GetMpRate();
	void SetMpRate(int rate);

	//index from 1 to 7
	int  GetSkillDelay(int index);
	void SetSkillDelay(int index, int delay);
	
	bool IsSkillDisabled();
	void SetSkillDisabled(bool bDisabled);

	CMatterSelector * GetMatterSelector() const {return m_pMatterSelector;}
	CMonsterSelector * GetMonsterSelector() const{return m_pMonsterSelector;}
	
	bool IsAnswerQuestion() const {return m_bAnswerQuestion;}
	void SetAnswerQuestion(bool bAnswer) {m_bAnswerQuestion = bAnswer;}
	
	int GetAnchorRange();
	void SetAnchorRange(int range);

	void SetTransferEnable(bool bEnable){ m_bTransferEnable = bEnable; }
	bool GetTransferEnable() { return m_bTransferEnable; }
	
	int GetUseItemDelay(int index);   
	void SetUseItemDelay(int index, int delay);

	bool Load(int playerId);
	bool Save(int playerId);
private:
	CLuaState * m_pState;
	CECCounter  m_Counter;
	CECCounter  m_TransferCounter; // 天人合一状态下，定期的将背包中的特定物品转移到随身包裹中
	bool		m_bTransferEnable; // 天人合一状态下，定期的将背包中的特定物品转移到随身包裹中功能是否开启
	bool        m_bRunning;
	bool        m_bAnswerQuestion;

	CMonsterSelector	* m_pMonsterSelector;
	CMatterSelector		* m_pMatterSelector;
};


