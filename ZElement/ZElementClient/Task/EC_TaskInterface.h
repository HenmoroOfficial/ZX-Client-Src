/*
 * FILE: EC_TaskInterface.h
 *
 * DESCRIPTION: 
 *
 * CREATED BY: Duyuxin, 2005/2/3
 *
 * HISTORY: 
 *
 * Copyright (c) 2005 Archosaur Studio, All Rights Reserved.
 */

#pragma once

#include "TaskInterface.h"
#include "TaskClient.h"

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

class CECHostPlayer;
class ATaskTemplMan;

///////////////////////////////////////////////////////////////////////////
//	
//	Declare of Global functions
//	
///////////////////////////////////////////////////////////////////////////

ATaskTemplMan* GetTaskTemplMan();

///////////////////////////////////////////////////////////////////////////
//	
//	Class CECTaskInterface
//	
///////////////////////////////////////////////////////////////////////////

struct talk_proc;
class CECTaskInterface : public TaskInterface
{
public:		//	Types

public:		//	Constructor and Destructor

	CECTaskInterface(CECHostPlayer* pHost);
	virtual ~CECTaskInterface();

public:		//	Attributes

public:		//	Operations

	//	Initialize object
	bool Init(void* pActiveListBuf, int iActiveListLen, void* pFinishedListBuf, int iFinishedListLen, void* pFinishedTimeListBuf, int iFinishedTimeListLen);
	//	Release object
	void Release();

	virtual unsigned long GetPlayerLevel();
	virtual int GetTalismanValue();
	virtual void* GetActiveTaskList();
	virtual void* GetFinishedTaskList();
	virtual void* GetFinishedTimeList();
	virtual int GetTaskItemCount(unsigned long ulTaskItem);
	virtual int GetCommonItemCount(unsigned long ulCommonItem);
	virtual int GetReinforceItemCount(unsigned long ulItemId, unsigned long ulLevel, unsigned char cond_type);
	virtual bool IsInFaction();
	virtual int GetFactionLev();
	virtual int GetFactionRole();
	virtual unsigned long GetGoldNum();
	virtual long GetReputation();
	virtual long GetRegionReputation(int index);
	virtual unsigned long GetCurPeriod();
	virtual unsigned long GetPlayerId();
	virtual unsigned long GetPlayerRace();
	virtual unsigned long GetPlayerLifeAgainCnt(); //获得转生次数
	virtual unsigned long GetPlayerLifeAgainOccup(int nLifeAgainIndex); //根据转生索引获得对应转生职业
	virtual unsigned long GetPlayerOccupation();
	virtual long GetPKValue();
	virtual bool IsGM();
	virtual bool HasTitle(short title);
	virtual void TakeAwayTitle(short title) {}
	virtual void DeliverPKValue(long lVal) {}
	virtual void ResetPKValue() {}

	virtual unsigned long* GetTaskMask() { return NULL; }
	virtual void DeliverGold(unsigned long ulGoldNum, unsigned long) {}
	virtual void DeliverExperience(int64_t ulExp, unsigned long) {}
	virtual void DeliverSP(unsigned long ulSP, unsigned long) {}
	virtual void DeliverReputation(long ulReputation, unsigned long) {}
	virtual void DeliverRegionReputation(int index, long lValue, unsigned long ulPlayerID = 0) {}
	virtual void TakeAwayGold(unsigned long ulNum) {}
	virtual void TakeAwayTaskItem(unsigned long ulTemplId, unsigned long ulNum) {}
	virtual void TakeAwayCommonItem(unsigned long ulTemplId, unsigned long ulNum) {}
	virtual void TakeAwayReinforceItem(unsigned long ulTemplId, unsigned long ulItemLevel, unsigned char cond_type) {}
	virtual bool IsDeliverLegal() { return true; }
	virtual bool CanDeliverCommonItem(unsigned long);
	virtual bool CanDeliverTaskItem(unsigned long);
	virtual void DeliverCommonItem(unsigned long ulItem, unsigned long ulNum, bool bBind, long lPeriod, unsigned long) {}
	virtual void DeliverTaskItem(unsigned long ulItem, unsigned long ulNum, unsigned long) {}
	virtual void DeliverReinforceItem(unsigned long ulItemId, unsigned long ulLevel, bool bBind, long lPeriod) {}
	virtual long GetFactionContribution();
	virtual void DeliverFactionContribution(long lVal) {}
	virtual int GetFamilyContribution();
	virtual int GetBattleScore();
	virtual int GetSJBattleScore();
	virtual int GetTeamMemberNum();
	virtual void GetTeamMemberInfo(int nIndex, task_team_member_info* pInfo);
	virtual unsigned long GetTeamMemberId(int nIndex);
	virtual bool IsCaptain();
	virtual bool IsInTeam();
	virtual bool IsMale();
	virtual bool IsMarried();
	virtual unsigned long GetPos(float pos[3]);
	virtual void NotifyServer(const void* pBuf, size_t sz);
	virtual bool HasLivingSkill(unsigned long ulSkill);
	virtual long GetLivingSkillProficiency(unsigned long ulSkill);
	virtual int  GetPetCon();
	virtual int  GetPetCiv();
	virtual void GetSpecailAwardInfo(special_award* p);
	virtual int GetProduceSkillLev();
	virtual void SetProduceSkillFirstLev() {}
	virtual int GetProduceSkillExp();
	virtual void AddProduceSkillExp(int nExp) {}

	/* 家族信息 */
	virtual bool IsInFamily();
	virtual int GetFamilyID();
	virtual int GetFamilySkillLevel(int nIndex);
	virtual int GetFamilySkillProficiency(int nIndex);
	virtual int GetFamilyMonsterRecord(int nIndex);
	virtual int GetFamilyCommonValue(int nIndex);
	virtual void DeliverFamilyExperience(int nFamilyID, int64_t ulExp) {}
	virtual bool IsMaxInvSize();
	virtual void CameraMove(unsigned long ulCutsceneIndex);
	virtual void PlayAnimation(unsigned long ulAnimationID);
	virtual void PlayAwardGfx(unsigned long ulGfxID);
	virtual void PlayTinyGame(unsigned long ulGameID);


	virtual bool IsMaster();
	virtual bool IsPrentice();
	virtual void ResetRegionReputation(int index) {}
	virtual unsigned long GetCultivation();
	virtual void SetCultivation(unsigned long ulCult) {}
	virtual void ClearCultSkill(unsigned long ulCult) {}
	virtual void ClearNormalSkill() {}
	virtual void ClearTalentSkill() {}
	virtual unsigned long GetInMasterLev();
	virtual int GetTreasureConsumed();
	virtual int GetTotalCaseAdd();	   //充值元宝数
	virtual bool IsAchievementFinish(unsigned short id);
	
	virtual unsigned long GetAchievePoint();	// 获取成就点数
	virtual unsigned long GetFriendNum();		// 获取好友数量
	virtual int GetCircleGroupId();
	virtual unsigned long GetCircleGroupPoints();	// 获取圈子积分	
	virtual unsigned long GetLastLogoutTime();		// 获取上次下线时间
	
	virtual unsigned long GetTerritoryScore();		// 获取领土战积分


	// 变身
	virtual int GetTransformID();
	virtual int GetTransformLevel();
	virtual int GetTransformExpLevel();
	virtual void SetTransform(int iID, int iDuration, int iLevel, int iExpLevel, bool bCover) {}
	
	virtual bool IsInCrossServer();
	virtual int GetItemSlotCnt(unsigned long ulItemTemplID, unsigned long ulItemNum);

	virtual unsigned long GetFengshenLevel();
	virtual bool IsFengshenExpFull();
	virtual void DeliverFengshenExp(unsigned long ulExp, unsigned long ulPlayerID) {}
	virtual void DeliverFengshenDujie(unsigned long ulPlayerID) {}

	/* 国战 */
	virtual bool HasKing();		// 是否有国王
	virtual bool IsKing();		// 玩家是否国王
	virtual bool IsQueen();		// 玩家是否王后
	virtual bool IsGeneral();	// 玩家是否将军
	virtual bool IsOfficial();	// 玩家是否大臣
	virtual bool IsMember();	// 玩家是否国王帮成员
	virtual bool IsGuard();		// 玩家是否亲卫
	virtual int  GetKingScore();// 获取国王积分

	//帮派基地属性
	virtual int GetBuildingLevel(int iBuildId);
	virtual int GetFactionGoldNote();
	virtual void DeliverFactionGoldNote(int iGoldNoteNum) {}
	virtual void TakeAwayFactionGoldNote(int iGoldNum) {}

	virtual int GetFactionGrass();
	virtual void DeliverFactionGrass(int iGrassNum) {}
	virtual void TakeAwayFactionGrass(int iGrasNum) {}

	virtual int GetFactionMine();
	virtual void DeliverFactionMine(int iMineNum) {}
	virtual void TakeAwayFactionMine(int iMineNum) {}

	virtual int GetFactionMonsterFood();
	virtual void DeliverFactionMonsterFood(int iMonsterFoodNum) {}
	virtual void TakeAwayFactionMosnterFood(int iMonsterFoodNum) {}

	virtual int GetFactionMonsterCore();
	virtual void DeliverFactionMonsterCore(int iMonsterCoreNum) {}
	virtual void TakeAwayFactionMonsterCore(int iMonsterCoreNum) {}

	virtual int GetFactionMoney();
	virtual void DeliverFactionMoney(int iMoneyNum) {}
	virtual void TakeAwayFactionMoney(int iMoneyNum) {}

	virtual void OnFBaseBuildingProgress(unsigned long task_id, int ivalue) {}

	virtual int GetBuildLevelInConstruct();

	virtual bool InInterAction(int iInterObjId);

	//检查当前角色的创建时间长度 Added 2011-04-11.
	virtual unsigned long GetCreateRoleTimeDuration();

protected:	//	Attributes

	CECHostPlayer*	m_pHost;
	void*			m_pActiveListBuf;	//	Active task list buffer
	void*			m_pFinishedListBuf;	//	Finished task list buffer
	void*			m_pFinishedTimeListBuf; //	Finished time list buffer

protected:	//	Operations

};

///////////////////////////////////////////////////////////////////////////
//	
//	Inline functions
//	
///////////////////////////////////////////////////////////////////////////


