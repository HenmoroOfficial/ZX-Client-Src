/********************************************************************
	created:	2009/6/23
	author:		lihengju
	
	purpose:	use lua event to trigger help system
	Copyright (C) 2006 - All Rights Reserved
*********************************************************************/
#include "EC_Global.h"
#include "LuaEvent.h"
#include "LuaState.h"
#include "LuaUtil.h"
#include "EC_UIManager.h"
#include "EC_Global.h"
#include "EC_GameRun.h"
#include "EC_HostPlayer.h"
#include "EC_Secretary.h"
#include "EC_Game.h"
#include "EC_GameUIMan.h"
#include "LuaTriggerTask.h"
#include "AUI\\AUIDialog.h"

#define SECRETARYSPECIALIDMAP "SecretarySpecialIDMap"

#define SENDLUAFIRSTEVENT2(event) \
	CECSecretary* pSecretary = g_pGame->GetGameRun()->GetSecretary();\
	CLuaTriggerTask* pTriggerTask = g_pGame->GetLuaTriggerTask();\
	pTriggerTask->ProcessTriggerTask((int)LEVT_##event, 0);\
	if(!pSecretary->CheckEventState(CECSecretary::event))\
	return;\
	CECGameUIMan * pUIMan = g_pGame->GetGameRun()->GetUIManager()->GetInGameUIMan();\
	if (pUIMan && pUIMan->IsImageHelpOpen())\
	{\
	abase::vector<CScriptValue> args;\
	args.push_back(CScriptValue((double)LEVT_##event));\
	args.push_back(CScriptValue((double)LEVT_##event));\
	pUIMan->SendLuaEvent(args);\
	pSecretary->SetEventState(CECSecretary::event);\
	}

#define SENDLUAFIRSTEVENT(event) \
	CECSecretary* pSecretary = g_pGame->GetGameRun()->GetSecretary();\
	CLuaTriggerTask* pTriggerTask = g_pGame->GetLuaTriggerTask();\
	pTriggerTask->ProcessTriggerTask((int)LEVT_##event, 0);\
	if(!pSecretary->IsEnable())\
	{\
		pSecretary->SetEventState(CECSecretary::event);\
		return;\
	}\
	if(!pSecretary->CheckEventState(CECSecretary::event))\
		return;\
	CECGameUIMan * pUIMan = g_pGame->GetGameRun()->GetUIManager()->GetInGameUIMan();\
	if (pUIMan)\
	{\
		abase::vector<CScriptValue> args;\
		args.push_back(CScriptValue((double)LEVT_##event));\
		args.push_back(CScriptValue((double)LEVT_##event));\
		pUIMan->SendLuaEvent(args);\
		pSecretary->SetEventState(CECSecretary::event);\
	}

#define SENDLUAFIRSTEVENTARG(event, id) \
	CECGameUIMan * pUIMan = g_pGame->GetGameRun()->GetUIManager()->GetInGameUIMan();\
	if (pUIMan)\
	{\
		abase::vector<CScriptValue> args;\
		args.push_back(CScriptValue((double)LEVT_##event));\
		args.push_back(CScriptValue((double)LEVT_##event));\
		args.push_back(CScriptValue((double)id));\
		pUIMan->SendLuaEvent(args);\
	}

#define SENDLUAFIRSTEVENTARG2(event, id) \
	CECGameUIMan * pUIMan = g_pGame->GetGameRun()->GetUIManager()->GetInGameUIMan();\
	if (pUIMan && pUIMan->IsImageHelpOpen())\
	{\
	abase::vector<CScriptValue> args;\
	args.push_back(CScriptValue((double)LEVT_##event));\
	args.push_back(CScriptValue((double)LEVT_##event));\
	args.push_back(CScriptValue((double)id));\
	pUIMan->SendLuaEvent(args);\
	}

#define PROCESSTRIGGERTASKARG(event, id)\
	CLuaTriggerTask* pTriggerTask = g_pGame->GetLuaTriggerTask();\
	pTriggerTask->ProcessTriggerTask((int)LEVT_##event, id);\

namespace LuaEvent
{
	void FirstEnterGame()
	{
/*		CECSecretary* pSecretary = g_pGame->GetGameRun()->GetSecretary();
		CLuaTriggerTask* pTriggerTask = g_pGame->GetLuaTriggerTask();
		pTriggerTask->ProcessTriggerTask((int)LEVT_FIRST_ENTER_GAME, 0);
		if(!pSecretary->CheckEventState(CECSecretary::FIRST_ENTER_GAME))
			return;
		CECGameUIMan * pUIMan = g_pGame->GetGameRun()->GetUIManager()->GetInGameUIMan();
		if (pUIMan)
		{
			abase::vector<CScriptValue> args;
			args.push_back(CScriptValue((double)LEVT_FIRST_ENTER_GAME));
			args.push_back(CScriptValue((double)LEVT_FIRST_ENTER_GAME));
			pUIMan->SendLuaEvent(args);
			pSecretary->SetEventState(CECSecretary::FIRST_ENTER_GAME);
		}
*///		SENDLUAFIRSTEVENT2(FIRST_ENTER_GAME);
//		if(pUIMan)
//			g_pGame->GetGameRun()->GetSecretary()->SetLastEvent(LEVT_FIRST_ENTER_GAME);
	}
	void PlayerIsMoving()
	{
//		CECSecretary* pSecret = g_pGame->GetGameRun()->GetSecretary();
//		if(pSecret->GetLastEvent() != LEVT_FIRST_ENTER_GAME )
//			return;
		SENDLUAFIRSTEVENT(PLAYER_IS_MOVING)
// 		if (pUIMan)
// 		{
// 			g_pGame->GetGameRun()->GetSecretary()->SetLastEvent(LEVT_PLAYER_IS_MOVING);
// 		}
	} 

	void PlayerIsAdjustOrient()
	{
//		CECSecretary* pSecret = g_pGame->GetGameRun()->GetSecretary();
//		if(pSecret->GetLastEvent() != LEVT_PLAYER_IS_MOVING )
//			return;
		SENDLUAFIRSTEVENT(PLAYER_ADJUST_ORIENT)
// 		if (pUIMan)
// 		{
// 			g_pGame->GetGameRun()->GetSecretary()->SetLastEvent(LEVT_PLAYER_ADJUST_ORIENT);
// 		}
	}


	void FirstLockVision()
	{
		SENDLUAFIRSTEVENT(PLAYER_LOCK_VISION)		
	}

	void FirstEmotionAction()
	{
		SENDLUAFIRSTEVENT(FIRST_EMOTION_ACTION)	
	}

	void FirstEmotionWord()
	{
		SENDLUAFIRSTEVENT(FIRST_EMOTION_WORD)	
	}

	void FirstCameraMode()
	{
		SENDLUAFIRSTEVENT(FIRST_CAMERA_MODE)	
	}
	void FirstSafeLock()
	{
		SENDLUAFIRSTEVENT(FIRST_SAFELOCK)	
	}
	void FirstBossKey()
	{
		SENDLUAFIRSTEVENT(FIRST_BOSSKEY)	
	}

	void FirstTalkNPC()
	{
	//	FirstTalkNPC2();
		SENDLUAFIRSTEVENT(FIRST_TALK_NPC)
	}
	
	void FirstTalkNPC2()
	{
		SENDLUAFIRSTEVENT2(FIRST_TALK_NPC2)
	}
	void FirstAcceptMainTask()
	{
		SENDLUAFIRSTEVENT(FIRST_TASK_MAIN)	
	}
	void FirstAcceptTravelTask()
	{
		SENDLUAFIRSTEVENT(FIRST_TASK_TRAVEL)	
	}
	void FirstAcceptRiskTask()
	{
		SENDLUAFIRSTEVENT(FIRST_TASK_RISK)	
	}

	void FirstAcceptClassicalTask()
	{
		SENDLUAFIRSTEVENT(FIRST_TASK_CLASSICAL)	
	}
	void FirstAcceptChanllengeTask()
	{
		SENDLUAFIRSTEVENT(FIRST_TASK_CHANLLENGE)	
	}
	void FirstAcceptKillMonsterTask()
	{
		SENDLUAFIRSTEVENT(FIRST_TASK_KILL_MONSTER)	
	}

	void FirstAcceptForgeTask()
	{
		SENDLUAFIRSTEVENT(FIRST_TASK_FORGE)	
	}
	
	void FirstAcceptHeritTask()
	{
		SENDLUAFIRSTEVENT(FIRST_TASK_HERIT)	
	}
	void FirstAcceptAsceticTask()
	{
		SENDLUAFIRSTEVENT(FIRST_TASK_ASCETIC)	
	}
	void FirstAcceptLupinTask()
	{
		SENDLUAFIRSTEVENT(FIRST_TASK_LUPIN)	
	}
	void FirstAcceptTeacherTask()
	{
		SENDLUAFIRSTEVENT(FIRST_TASK_TEACHER)	
	}
	void FirstAcceptGuildTask()
	{
		SENDLUAFIRSTEVENT(FIRST_TASK_GUILD)	
	}
	void FirstAcceptTeasureTask()
	{
		SENDLUAFIRSTEVENT(FIRST_TASK_TREASURE)	
	}

	void FirstAcceptAdventureTask()
	{
		SENDLUAFIRSTEVENT(FIRST_TASK_ADVENTURE)	
	}
	void FirstAcceptLoveTask()
	{
		SENDLUAFIRSTEVENT(FIRST_TASK_LOVE)	
	}
	// 查看任务
	void FirstViewTask()
	{
		SENDLUAFIRSTEVENT(FIRST_TASK_READ)	
	}
	// 采集物品
	void FirstCollectItem()
	{
		SENDLUAFIRSTEVENT(FIRST_OBJECT_PICK)	
	}
	
	void FirstTaskFail()
	{
		SENDLUAFIRSTEVENT(FIRST_TASK_FAIL)	
	}
	void FirstTaskFull()
	{
		SENDLUAFIRSTEVENT(FIRST_TASK_LIMIT)	
	}
	void FirstTraceTask()
	{
		SENDLUAFIRSTEVENT(FIRST_TASK_TRACK)	
	}
	void FirstAutoMove()
	{
		//FirstAutoMove2();
		SENDLUAFIRSTEVENT(FIRST_MAP_AUTOTRACK)	
	}
	void FirstAutoMove2()
	{
		SENDLUAFIRSTEVENT2(FIRST_MAP_AUTOTRACK2)	
	}
	void FirstOpenMap()
	{
		SENDLUAFIRSTEVENT(FIRST_MAP_OPEN)	
	}
	void FirstFinishTask()
	{
		SENDLUAFIRSTEVENT(FIRST_TASK_FINISH)	
	}
	void FirstTaskAward()
	{
		SENDLUAFIRSTEVENT(FIRST_TASK_AWARD)	
	}
	void FirstSearchLatestTask()
	{
		SENDLUAFIRSTEVENT(FIRST_TASK_SEARCH_LATEST)	
	}
	void FirstOpenPack()
	{
		SENDLUAFIRSTEVENT(FIRST_BAG_OPEN)	
	}
	void FirstEquipItem()
	{
		SENDLUAFIRSTEVENT(FIRST_ITEM_EQUIP)	
	}
	void FirstUseMedicine()
	{
		SENDLUAFIRSTEVENT(FIRST_MEDICINE_USE)	
	}

	void FirstObtainSpecialItem(int tid)
	{
		//FirstObtainSpecialItem2(tid);
		PROCESSTRIGGERTASKARG(FIRST_ITEM_SPECIAL, tid);
		abase::vector<CScriptValue> args;
		abase::vector<CScriptValue> results;
		int index = 0;
		args.push_back((double)tid);
		CLuaState* pState = g_LuaStateMan.GetAIState();
		if (!pState)
		{
			ASSERT(0);
			return;
		}
		pState->Call(SECRETARYSPECIALIDMAP, "GetSpecialIdIndex", args, results);
		
		if (results.size() == 1 && results[0].m_Type == CScriptValue::SVT_NUMBER)
		{
			index = results[0].GetInt();
 		}
		if (index == 0)
			return;

		CECSecretary* pSecretary = g_pGame->GetGameRun()->GetSecretary();
		if (!pSecretary->CheckSpecialState(index))
			return;
		
		// 第一次获取物品的高亮显示
		g_pGame->GetGameRun()->GetUIManager()->GetInGameUIMan()->GetItemEmphase(tid);

		if(!pSecretary->IsEnable())
		{
			pSecretary->SetSpecialState(index);
			return;
		}
		SENDLUAFIRSTEVENTARG(FIRST_ITEM_SPECIAL, tid)
		pSecretary->SetSpecialState(index);
	}
	
	void FirstObtainSpecialItem2(int tid)
	{
		abase::vector<CScriptValue> args;
		abase::vector<CScriptValue> results;
		int index = 0;
		args.push_back((double)tid);
		CLuaState* pState = g_LuaStateMan.GetAIState();
		if (!pState)
		{
			ASSERT(0);
			return;
		}
		pState->Call(SECRETARYSPECIALIDMAP, "GetSpecialIdImageIndex", args, results);
		
		if (results.size() == 1 && results[0].m_Type == CScriptValue::SVT_NUMBER)
		{
			index = results[0].GetInt();
		}
		if (index == 0)
			return;
		
		int indexItem;
		pState->Call(SECRETARYSPECIALIDMAP, "GetSpecialIdIndex", args, results);
		
		if (results.size() == 1 && results[0].m_Type == CScriptValue::SVT_NUMBER)
		{
			indexItem = results[0].GetInt();
		}
		if (indexItem == 0)
			return;

		CECSecretary* pSecretary = g_pGame->GetGameRun()->GetSecretary();
		if (!pSecretary->CheckSpecialState(indexItem))
			return;

		SENDLUAFIRSTEVENTARG2(FIRST_ITEM_SPECIAL2, tid)
	}

	void FirstNeatenPack()
	{
		SENDLUAFIRSTEVENT(FIRST_BAG_NEATEN)	
	}
	void FirstExpandPack()
	{
		SENDLUAFIRSTEVENT(FIRST_BAG_EXPAND)	
	}

	void FirstSplitItem()
	{
		SENDLUAFIRSTEVENT(FIRST_ITEM_SPILT)	
	}
	// 第一次摆摊
	void FirstBooth()
	{
		SENDLUAFIRSTEVENT(FIRST_BAG_SELL)	
	}
	// 第一次交易
	void FirstTrade()
	{
		SENDLUAFIRSTEVENT(FIRST_EXCHANGE)	
	}
	// 第一次元宝交易
	void FirstCashTrade()
	{
		SENDLUAFIRSTEVENT(FIRST_CASH_EXCHANGE)	
	}

	void FirstOpenTrash()
	{
		SENDLUAFIRSTEVENT(FIRST_DEPOT_OPEN)	
	}
	void FirstExpandTrash()
	{
		SENDLUAFIRSTEVENT(FIRST_DEPOT_EXPAND)	
	}
	// 第一次装备坐骑
	void FirstEquipHorse()
	{
		SENDLUAFIRSTEVENT(FIRST_HORSE_EQUIP)	
	}

	void FirstLockItem()
	{
		SENDLUAFIRSTEVENT(FIRST_ITEM_LOCK)	
	}
	void FirstOpenPetPack()
	{
		SENDLUAFIRSTEVENT(FIRST_BAG_PET)	
	}
	void FirstOpenFashionPack()
	{
		SENDLUAFIRSTEVENT(FIRST_BAG_FASHDRESS)	
	}
	// 第一次释放特殊技能
	void FirstCastSpecialSkill(int skillid)
	{

		PROCESSTRIGGERTASKARG(FIRST_SKILL_SPECIAL,skillid);
		abase::vector<CScriptValue> args;
		abase::vector<CScriptValue> results;
		int index = 0;
		args.push_back((double)skillid);
		CLuaState* pState = g_LuaStateMan.GetAIState();
		if (!pState)
		{
			ASSERT(0);
			return;
		}
		pState->Call(SECRETARYSPECIALIDMAP, "GetSpecialIdIndex", args, results);
		
		if (results.size() == 1 && results[0].m_Type == CScriptValue::SVT_NUMBER)
		{
			index = results[0].GetInt();
 		}
		if (index == 0)
			return;

		CECSecretary* pSecretary = g_pGame->GetGameRun()->GetSecretary();
		if(!pSecretary->IsEnable())
		{
			pSecretary->SetSpecialState(index);
			return;
		}
		if (!pSecretary->CheckSpecialState(index))
			return;
		SENDLUAFIRSTEVENTARG(FIRST_SKILL_SPECIAL,skillid)
		pSecretary->SetSpecialState(index);
	}

	void FirstAddSkillPoint()
	{
		SENDLUAFIRSTEVENT(FIRST_SKILL_POINT_ADD)	
	}
	// 编辑连续技
	void FirstEditContinuousSkill()
	{
		SENDLUAFIRSTEVENT(FIRST_SKILL_EDIT)	
	}
	// 洗点
	void FirstClearSkill()
	{
		SENDLUAFIRSTEVENT(FIRST_SKILL_CLEAR)	
	}

	void FirstAttack()
	{
		SENDLUAFIRSTEVENT(FIRST_ATTACK)	
	}
	void FirstPK()
	{
		SENDLUAFIRSTEVENT(FIRST_PK)	
	}
	// pk 设置
	void FirstPKSetting()
	{
		SENDLUAFIRSTEVENT(FIRST_PK_SETTING)	
	}
	// pk值 增加
	void FirstIncPKValue()
	{
		SENDLUAFIRSTEVENT(FIRST_PK_UP)	
	}

	void FirstDecPKValue()
	{
		SENDLUAFIRSTEVENT(FIRST_PK_DOWN)	
	}

	void FirstBUFF()
	{
		SENDLUAFIRSTEVENT(FIRST_BUFF)	
	}
	// 第一次hp小于50%
	void FirstHalfHP()
	{
		SENDLUAFIRSTEVENT(FIRST_HP)	
	}

	void FirstDie()
	{
		SENDLUAFIRSTEVENT(FIRST_DIE)	
	}
	void FirstPunishDie()
	{
		SENDLUAFIRSTEVENT(FIRST_DIE_PUNISH)	
	}
	void FirstSitDown()
	{
		SENDLUAFIRSTEVENT(FIRST_REST)	
	}
	void FirstKillMonster()
	{
		SENDLUAFIRSTEVENT(FIRST_KILL_MONSTER)	
	}

	void FirstPickUpItem()
	{
		SENDLUAFIRSTEVENT(FIRST_PICKUP)	
	}
	void FirstDiscardItem()
	{
		SENDLUAFIRSTEVENT(FIRST_ABANDON)	
	}
	void FirstOpenChat()
	{
		SENDLUAFIRSTEVENT(FIRST_CHAT_OPEN)	
	}
	// 第一次选中特殊属性的怪物
	void FirstClickSpecialMonster(int id)
	{
		PROCESSTRIGGERTASKARG(FIRST_MONSTER_SPECIAL, id)
		abase::vector<CScriptValue> args;
		abase::vector<CScriptValue> results;
		int index = 0;
		args.push_back((double)id);
		CLuaState* pState = g_LuaStateMan.GetAIState();
		if (!pState)
		{
			ASSERT(0);
			return;
		}
		pState->Call(SECRETARYSPECIALIDMAP, "GetSpecialIdIndex", args, results);
		
		if (results.size() == 1 && results[0].m_Type == CScriptValue::SVT_NUMBER)
		{
			index = results[0].GetInt();
 		}
		if (index == 0)
			return;

		CECSecretary* pSecretary = g_pGame->GetGameRun()->GetSecretary();
		if(!pSecretary->IsEnable())
		{
			pSecretary->SetSpecialState(index);
			return;
		}
		if (!pSecretary->CheckSpecialState(index))
			return;
		SENDLUAFIRSTEVENTARG(FIRST_MONSTER_SPECIAL, id)
		pSecretary->SetSpecialState(index);
	}
	void FirstEnterNewMap()
	{
		SENDLUAFIRSTEVENT(FIRST_ENTER_MAP_NEW)	
	}
	void FirstTransmit()
	{
		SENDLUAFIRSTEVENT(FIRST_MAP_TRANSFER)	
	}

	void FirstTeam()
	{
		SENDLUAFIRSTEVENT(FIRST_TEAM_SETUP)	
	}

	void FirstTeamSetting()
	{
		SENDLUAFIRSTEVENT(FIRST_TEAM_SETTING)	
	}
	void FirstFactionBattle()
	{
		SENDLUAFIRSTEVENT(FIRST_BATTLE_GUILD)	
	}
	// 第一次参加城战
	void FirstCityBattle()
	{
		SENDLUAFIRSTEVENT(FIRST_BATTLE_CITY)	
	}
	
	void FirstObtainTalisman()
	{
		SENDLUAFIRSTEVENT(FIRST_TALISMAN_MAIN)	
	}
	void FirstObtainTalismanPart()
	{
		SENDLUAFIRSTEVENT(FIRST_TALISMAN_VICE)	
	}
	// 第一次使用内挂
	void FirstRunHelper()
	{
		SENDLUAFIRSTEVENT(FIRST_TALISMAN_START)	
	}
	//
	void FirstTalismanUpgrade()
	{
		SENDLUAFIRSTEVENT(FIRST_TALISMAN_UPDATE)	
	}
	// 获得法宝灵媒
	void FirstObtainTalismanMedium()
	{
		SENDLUAFIRSTEVENT(FIRST_TALISMAN_MEDIUM)	
	}
	// 聚气
	void FirstRefineTalisman()
	{
		SENDLUAFIRSTEVENT(FIRST_TALISMAN_REFINE)	
	}
	// 灌魔
	void FirstEnchantTalisman()
	{
		SENDLUAFIRSTEVENT(FIRST_TALISMAN_ENCHANT)	
	}
	// 归元
	void FirstCleanTalisman()
	{
		SENDLUAFIRSTEVENT(FIRST_TALISMAN_CLEAN)	
	}
	// 血炼
	void FirstCombineTalisman()
	{
		SENDLUAFIRSTEVENT(FIRST_TALISMAN_COMBINE)	
	}

	void FirstAddFriend()
	{
		SENDLUAFIRSTEVENT(FIRST_FRIEND_ADD)	
	}
	// 相依相偎
	void FirstHug()
	{
		SENDLUAFIRSTEVENT(FIRST_HUG)	
	}

	void FirstMarry()
	{
		SENDLUAFIRSTEVENT(FIRST_MARRY)	
	}
	
	void FirstCreateFamily()
	{
		SENDLUAFIRSTEVENT(FIRST_FAMILY_CREATE)	
	}
	void FirstJoinFamily()
	{
		SENDLUAFIRSTEVENT(FIRST_FAMILY_JOIN)	
	}
	void FirstCreateFaction()
	{
		SENDLUAFIRSTEVENT(FIRST_GUILD_CREATE)	
	}
	void FirstJoinFaction()
	{
		SENDLUAFIRSTEVENT(FIRST_GUILD_JOIN)	
	}

	void FirstDeleteFriend()
	{
		SENDLUAFIRSTEVENT(FIRST_FRIEND_DELETE)	
	}
	void FirstOpenBlackList()
	{
		SENDLUAFIRSTEVENT(FIRST_BLACKLIST)	
	}
	void FirstOpenEnemy()
	{
		SENDLUAFIRSTEVENT(FIRST_ENEMY)	
	}
	// 拜师
	void FirstBeStudent()
	{
		SENDLUAFIRSTEVENT(FIRST_STUDENT)	
	}
	// 出师
	void FirstLeaveMaster()
	{
		SENDLUAFIRSTEVENT(FIRST_STUDENT_OUT)	
	}
	// 收徒
	void FirstbeMaster()
	{
		SENDLUAFIRSTEVENT(FIRST_TEACHER)	
	}
	// 收徒数量上限增加
	void IncDispileUpperLimit()
	{
		SENDLUAFIRSTEVENT(FIRST_STUDENT_ADD)	
	}

	void FirstOpenSNS()
	{
		SENDLUAFIRSTEVENT(FIRST_FRIEND_SEEK)	
	}
	// 狭义值
	void FirstObtainErrantry()
	{
		SENDLUAFIRSTEVENT(FIRST_LUPIN_POINT)	
	}
	void FirstObtainPet()
	{
		SENDLUAFIRSTEVENT(FIRST_PET_GET)	
	}
	void FirstObtainSpecialPet(int id)
	{
		PROCESSTRIGGERTASKARG(FIRST_PET_SPECIAL, id)
		abase::vector<CScriptValue> args;
		abase::vector<CScriptValue> results;
		int index = 0;
		args.push_back((double)id);
		CLuaState* pState = g_LuaStateMan.GetAIState();
		if (!pState)
		{
			ASSERT(0);
			return;
		}
		pState->Call(SECRETARYSPECIALIDMAP, "GetSpecialIdIndex", args, results);
		
		if (results.size() == 1 && results[0].m_Type == CScriptValue::SVT_NUMBER)
		{
			index = results[0].GetInt();
 		}
		if (index ==0)
			return;

		CECSecretary* pSecretary = g_pGame->GetGameRun()->GetSecretary();
		if(!pSecretary->IsEnable())
		{
			pSecretary->SetSpecialState(index);
			return;
		}
		if (!pSecretary->CheckSpecialState(index))
			return;
		SENDLUAFIRSTEVENTARG(FIRST_PET_SPECIAL, id)
		pSecretary->SetSpecialState(index);
	}
	
	void FirstUsePet()
	{
		SENDLUAFIRSTEVENT(FIRST_PET_USE)	
	}
	// 第一次获得宠物装备
	void FirstObtainPetEquip()
	{
		SENDLUAFIRSTEVENT(FIRST_PET_EQUIPMENT)	
	}
	void FirstEnhancePetEquip()
	{
		SENDLUAFIRSTEVENT(FIRST_PET_EQUIPMENT_STRENGHTEN)	
	}

	void FirstTamePet()
	{
		SENDLUAFIRSTEVENT(FIRST_PET_TAME)	
	}
	void FirstRefinePet()
	{
		SENDLUAFIRSTEVENT(FIRST_PET_COMBINE)	
	}
	void FirstIncPetStar()
	{
		SENDLUAFIRSTEVENT(FIRST_PET_STAR_UP)	
	}
	void FirstChangePetShape()
	{
		SENDLUAFIRSTEVENT(FIRST_PET_CHANGE_IMAGE)	
	}
	void FirstChangePetName()
	{
		SENDLUAFIRSTEVENT(FIRST_PET_CHANGE_NAME)	
	}
	// 宠物模式切换
	void FirstChangePetMode()
	{
		SENDLUAFIRSTEVENT(FIRST_PET_CHANGE_MODE)	
	}
	
	void FirstFeedPet()
	{
		SENDLUAFIRSTEVENT(FIRST_PET_FEED)	
	}

	void FirstPetWeak()
	{
		SENDLUAFIRSTEVENT(FIRST_PET_WEAK)	
	}
	void FirstPetHungry()
	{
		SENDLUAFIRSTEVENT(FIRST_PET_HUNGRY)	
	}
	void FirstPetDie()
	{
		SENDLUAFIRSTEVENT(FIRST_PET_DIE)	
	}
	void FirstPetLvUp()
	{
		SENDLUAFIRSTEVENT(FIRST_PET_LV_UP)	
	}
	void FirstPetCollect()
	{
		SENDLUAFIRSTEVENT(FIRST_PET_COLLECT)	
	}
	void FirstPetProduce()
	{
		SENDLUAFIRSTEVENT(FIRST_PET_PRODUCE)	
	}
	// 第一次捕捉宠物
	void FirstCatchPet()
	{
		SENDLUAFIRSTEVENT(FIRST_PET_CATCH)	
	}

	void FirstObtainFlySword()
	{
		SENDLUAFIRSTEVENT(FIRST_FLY_GET)	
	}
	void FirstFly()
	{
		SENDLUAFIRSTEVENT(FIRST_FLY_MODE)	
	}
	void FirstFlyFight()
	{
		SENDLUAFIRSTEVENT(FIRST_FLY_FIGHT_MODE)	
	}
	// 第一次提高飞行法宝成长度
	void FirstFlySwordGrowthRateUp()
	{
		SENDLUAFIRSTEVENT(FIRST_FLY_LV_UP)	
	}
	void FirstFlySowrdSpeedUp()
	{
		SENDLUAFIRSTEVENT(FIRST_FLY_SPEED_UP)	
	}
	// 第一次炼器
	void FirstHarden()
	{
		SENDLUAFIRSTEVENT(FIRST_ENCHASE_LV)	
	}
	// 灌魔
	void FirstEnhancePoint()
	{
		SENDLUAFIRSTEVENT(FIRST_ENCHASE_POINT)	
	}
	// 血祭
	void FirstBleedScarifice()
	{
		SENDLUAFIRSTEVENT(FIRST_BLOOD_ENCHANSE)	
	}
	// 兑换装备
	void FirstExchangeEquip()
	{
		SENDLUAFIRSTEVENT(FIRST_EQUIP_EXCHANGE)	
	}

	void FirstBreakEquip()
	{
		SENDLUAFIRSTEVENT(FIRST_EQUIP_BREAK)	
	}
	void FirstHideEquip()
	{
		SENDLUAFIRSTEVENT(FIRST_EQUIP_HIDE)	
	}

	void FirstBindEquip()
	{
		SENDLUAFIRSTEVENT(FIRST_EQUIP_BIND)	
	}
	void FirstDepartEquip()
	{
		SENDLUAFIRSTEVENT(FIRST_EQUIP_DEPART)	
	}
	void FirstDestroyEquip()
	{
		SENDLUAFIRSTEVENT(FIRST_EQUIP_DESTROY)	
	}
	void FirstSuitEquip()
	{
		SENDLUAFIRSTEVENT(FIRST_EQUIP_SUIT)	
	}
	void FirstSendMail()
	{
		SENDLUAFIRSTEVENT(FIRST_MAIL)	
	}
	void FirstReceiveMail()
	{
		SENDLUAFIRSTEVENT(FIRST_MAIL)	
	}
	// 第一次加入门派
	void FirstJoinSect(int prof)
	{
		CECSecretary* pSecretary = g_pGame->GetGameRun()->GetSecretary();
		PROCESSTRIGGERTASKARG(FIRST_DESC_JOIN, prof);

		if(!pSecretary->IsEnable())
		{
			return;
		}
		SENDLUAFIRSTEVENTARG(FIRST_DESC_JOIN, prof)	
	}

	void LevelUp(int level)
	{
		//LevelUp2(level);
		CECSecretary* pSecretary = g_pGame->GetGameRun()->GetSecretary();
		PROCESSTRIGGERTASKARG(LV_UP, level)

		if(!pSecretary->IsEnable())
		{
			return;
		}
		SENDLUAFIRSTEVENTARG(LV_UP, level)
	}
	
	void LevelUp2(int level)
	{
		SENDLUAFIRSTEVENTARG2(LV_UP2, level)
	}
	void FirstOpenQshop()
	{
		SENDLUAFIRSTEVENT(FIRST_QSHOP_OPEN)	
	}
	void FirstObtainQshopItem()
	{
		SENDLUAFIRSTEVENT(QSHOP_ITEM_GET)	
	}
	void FirstLottery()
	{
		SENDLUAFIRSTEVENT(FIRST_RAFFLE)	
	}
	void FirstOpenQshop2()
	{
		SENDLUAFIRSTEVENT(FIRST_POPSHOP)	
	}
	void FirstIncReputation()
	{
		SENDLUAFIRSTEVENT(FIRST_CREDIT_ADD)	
	}
	void RankDropOut()
	{
		SENDLUAFIRSTEVENT(RANK_PERSON_DROPOUT)	
	}
	
	void RepuRankDropOut()
	{
		SENDLUAFIRSTEVENT(RANK_PERSONALL_DROPOUT)	
	}

	void FirstHideElsePlayer()
	{
		SENDLUAFIRSTEVENT(FIRST_HIDE_ELSEPLAYER)			
	}

	void FirstDeityLevelUp()
	{
		CECSecretary* pSecretary = g_pGame->GetGameRun()->GetSecretary();
		CECGameUIMan * pUIMan = g_pGame->GetGameRun()->GetUIManager()->GetInGameUIMan();
		if (pSecretary->CheckEventState(CECSecretary::FIRST_DEITY_POPHELP))
		{
			//pUIMan->GetDialog("Win_Fengshen_Soul_Help")->Show(true);
			AString strDlg = "Win_Fengshen_Soul_Help";
			pUIMan->CommonMessage("Win_HelpInfo","open",(WPARAM)(&strDlg),0);
			pSecretary->SetEventState(CECSecretary::FIRST_DEITY_POPHELP);
		}
	
		int iIndex = CECSecretary::FIRST_DEITY_LEVELUP;
		if(!pSecretary->CheckEventState(iIndex))
			return;
		pUIMan->CommonMessage("Win_Character", "set_flash", 0, 0);
	}

	void AcceptScriptCompleteTask(int iTask)
	{
		g_pGame->GetGameRun()->GetHostPlayer()->AddScriptFinishTask(iTask);
	}

	void FirstSpecialOperation(int iEvent)
	{
		CECSecretary* pSecretary = g_pGame->GetGameRun()->GetSecretary();
		int iIndex = iEvent - LEVT_FIRST_ENTER_GAME;
		// 触发任务，开启或者完成任务
		CLuaTriggerTask* pTriggerTask = g_pGame->GetLuaTriggerTask();
		pTriggerTask->ProcessTriggerTask(iEvent, 0);

		if(!pSecretary->IsEnable())
		{
			pSecretary->SetEventState(iIndex);
			return;
		}

		if(!pSecretary->CheckEventState(iIndex))
			return;
		CECGameUIMan * pUIMan = g_pGame->GetGameRun()->GetUIManager()->GetInGameUIMan();
		if (pUIMan)
		{
			abase::vector<CScriptValue> args;
			args.push_back(CScriptValue((double)iEvent));
			args.push_back(CScriptValue((double)iEvent));
			pUIMan->SendLuaEvent(args);
			pSecretary->SetEventState(iIndex);
		}		

	}

	void FirstSpecialOperationWithArg(int iEvent, int arg)
	{
		CLuaTriggerTask* pTriggerTask = g_pGame->GetLuaTriggerTask();
		pTriggerTask->ProcessTriggerTask(iEvent, arg);

		abase::vector<CScriptValue> args;
		abase::vector<CScriptValue> results;
		int index = 0;
		args.push_back((double)arg);
		CLuaState* pState = g_LuaStateMan.GetAIState();
		if (!pState)
		{
			ASSERT(0);
			return;
		}
		pState->Call(SECRETARYSPECIALIDMAP, "GetSpecialIdIndex", args, results);
		
		if (results.size() == 1 && results[0].m_Type == CScriptValue::SVT_NUMBER)
		{
			index = results[0].GetInt();
 		}
		if (index == 0)
			return;

		CECSecretary* pSecretary = g_pGame->GetGameRun()->GetSecretary();
		if(!pSecretary->IsEnable())
		{
			pSecretary->SetSpecialState(index);
			return;
		}
		if (!pSecretary->CheckSpecialState(index))
			return;

		CECGameUIMan * pUIMan = g_pGame->GetGameRun()->GetUIManager()->GetInGameUIMan();
		if (pUIMan)
		{
			abase::vector<CScriptValue> args;
			args.push_back(CScriptValue((double)iEvent));
			args.push_back(CScriptValue((double)iEvent));
			args.push_back(CScriptValue((double)arg));
			pUIMan->SendLuaEvent(args);
		}

		pSecretary->SetSpecialState(index);
	}
		
}