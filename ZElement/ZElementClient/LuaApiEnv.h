/********************************************************************
  created:	   4/7/2009  
  author:      liudong  
  description:  envionment for lua api
  Copyright (c) , All Rights Reserved.
*********************************************************************/
#pragma once

#include <AWString.h>
#include <vector.h>

class CECGame;
class CECGameRun;
class CECWorld;
class CECGameSession;
class CECHostPlayer;
class CECUIManager;
class CECGameUIMan;

class CLuaApiEnv
{
public:

	struct sObjectInfo
	{
		int    id;
		double ratioHP;
		double ratioMP;
		sObjectInfo()
		{
			Clear();
		}
		void Clear()
		{
			id		= 0;
			ratioHP	= 0.0;
			ratioMP	= 0.0;	
		}
	};

public:
	//name.
	static bool GetMonsterName(AWString& name, const unsigned int tid);							//获取怪物名字。
	static bool GetItemName(AWString& name, const unsigned int tid);							//获取物品名字（通过背包）。
	static bool GetTaskName(AWString& name, const unsigned int id);								//获取任务名字。
	static bool GetBattleName(AWString& name, const unsigned int tid);							//获取战场名字。
	static bool GetReputationName(AWString& name, const unsigned int id);						//获取声望名字。
	static bool GetSkillName(AWString& name, const unsigned int id);							//获取技能名字。
	static bool GetProduceName(AWString& name, const unsigned int tid, const unsigned int index);//获取生产物品名字，传入参数：配方id，第几个物品, index:zero based。
//	static bool GetMoneyBagTypeName(AWString& name, const unsigned int id);						//获取跑商交易币名字。
	static bool GetProfName(AWString& name, const unsigned int id);								//获取职业名字。
	static bool GetAchievementName(AWString& name, const unsigned int id);						//获取成就名字。
	
	//pet 
	static bool GetPetProp(sObjectInfo& info);
	static int  UsePetSkill(const int targetType, const int indexSkill);//return skill id.
	static bool CheckPetHPItem();
	static bool CheckPetMPItem();
	static void HealPetHP();
	static void HealPetMP();
	static void GetPetBuff(abase::vector<int>& vecBuff);
	//判断宠物技能状态。 参数(slot):佣兵技能栏index，0-7.；参数bCanAutoCast:返回是否能自动释放；返回技能id。
	static int CheckPetSkillCastCondition(bool& bCanAutoCast, const int iSkillSlot);
	
	//host player
	static void GetHostProp(sObjectInfo& info);
	static bool GetTargetProp(sObjectInfo& info);
	static void GetHostBuff(abase::vector<int>& vecBuff);
	static void GetTargetBuff(abase::vector<int>& vecBuff);
	
	//utility
	static void AddChatMsg(const wchar_t* pMsg);

protected:
	static CECGame * GetGame();
	static CECGameRun * GetGameRun();
	static CECWorld * GetWorld();
	static CECGameSession * GetGameSession();
	static CECHostPlayer * GetHostPlayer();
	static CECGameUIMan * GetGameUIMan();
};
