/********************************************************************
  created:	   4/7/2009  
  author:      liudong  
  description: 
  Copyright (c) , All Rights Reserved.
*********************************************************************/

#include "LuaGameAPI.h"
#include "LuaAPI.h"
#include "LuaState.h"
#include "LuaUtil.h"
#include "LuaApiEnv.h"
#include "EC_Achievement.h"
//#include "ShellSDK/EncStr.h"

#include <vector.h>

using namespace abase;
using namespace LuaBind;

void GetMonsterName(vector<CScriptValue>& args, vector<CScriptValue>& results)
{
	if (args.size() < 1 || args[0].m_Type != CScriptValue::SVT_NUMBER)
	{
		LUA_DEBUG_INFO("GetMonsterName, wrong args\n");
		return;
	}

	unsigned int tid = (unsigned int)args[0].GetDouble();
	AWString name;
	if (CLuaApiEnv::GetMonsterName(name, tid))
	{
		CScriptValue var;
		var.SetVal(name);
		results.push_back(var);
	}
}
IMPLEMENT_SCRIPT_API(GetMonsterName);

void GetItemName(vector<CScriptValue>& args, vector<CScriptValue>& results)
{
	if (args.size() < 1 || args[0].m_Type != CScriptValue::SVT_NUMBER)
	{
		LUA_DEBUG_INFO("GetItemName, wrong args\n");
		return;
	}

	unsigned int tid = (unsigned int)args[0].GetDouble();
	AWString name;
	if (CLuaApiEnv::GetItemName(name, tid))
	{
		CScriptValue var;
		var.SetVal(name);
		results.push_back(var);
	}
}
IMPLEMENT_SCRIPT_API(GetItemName);

void GetTaskName(vector<CScriptValue>& args, vector<CScriptValue>& results)
{
	if (args.size() < 1 || args[0].m_Type != CScriptValue::SVT_NUMBER)
	{
		LUA_DEBUG_INFO("GetTaskName, wrong args\n");
		return;
	}

	unsigned int id = (unsigned int)args[0].GetDouble();
	AWString name;
	if (CLuaApiEnv::GetTaskName(name, id))
	{
		CScriptValue var;
		var.SetVal(name);
		results.push_back(var);
	}
}
IMPLEMENT_SCRIPT_API(GetTaskName);

void GetReputationName(vector<CScriptValue>& args, vector<CScriptValue>& results)
{
	if (args.size() < 1 || args[0].m_Type != CScriptValue::SVT_NUMBER)
	{
		LUA_DEBUG_INFO("GetReputationName, wrong args\n");
		return;
	}

	unsigned int id = (unsigned int)args[0].GetDouble();
	AWString name;
	if (CLuaApiEnv::GetReputationName(name, id))
	{
		CScriptValue var;
		var.SetVal(name);
		results.push_back(var);
	}
}
IMPLEMENT_SCRIPT_API(GetReputationName);

void GetSkillName(vector<CScriptValue>& args, vector<CScriptValue>& results)
{
	if (args.size() < 1 || args[0].m_Type != CScriptValue::SVT_NUMBER)
	{
		LUA_DEBUG_INFO("GetSkillName, wrong args\n");
		return;
	}

	unsigned int id = (unsigned int)args[0].GetDouble();
	AWString name;
	if (CLuaApiEnv::GetSkillName(name, id))
	{
		CScriptValue var;
		var.SetVal(name);
		results.push_back(var);
	}
}
IMPLEMENT_SCRIPT_API(GetSkillName);

void GetProduceName(vector<CScriptValue>& args, vector<CScriptValue>& results)
{
	if (args.size() < 2 || 
		args[0].m_Type != CScriptValue::SVT_NUMBER ||
		args[1].m_Type != CScriptValue::SVT_NUMBER)
	{
		LUA_DEBUG_INFO("GetProduceName, wrong args\n");
		return;
	}

	unsigned int tid	= (unsigned int)args[0].GetDouble();
	unsigned int index	= (unsigned int)args[1].GetDouble()-1;//C is Zero based.
	AWString name;
	if (CLuaApiEnv::GetProduceName(name, tid, index))
	{
		CScriptValue var;
		var.SetVal(name);
		results.push_back(var);
	}
}
IMPLEMENT_SCRIPT_API(GetProduceName);


void GetProfName(vector<CScriptValue>& args, vector<CScriptValue>& results)
{
	if (args.size() < 1 || args[0].m_Type != CScriptValue::SVT_NUMBER)
	{
		LUA_DEBUG_INFO("GetProfName, wrong args\n");
		return;
	}

	unsigned int id = (unsigned int)args[0].GetDouble();
	AWString name;
	if (CLuaApiEnv::GetProfName(name, id))
	{
		CScriptValue var;
		var.SetVal(name);
		results.push_back(var);
	}
}
IMPLEMENT_SCRIPT_API(GetProfName);

//获取成就中某个条件的整数数据。
void GetAchCondDataInt(vector<CScriptValue>& args, vector<CScriptValue>& results)
{
	if (args.size() < 1 || args[0].m_Type != CScriptValue::SVT_USERDATA)
	{
		LUA_DEBUG_INFO("GetAchCondDataInt, wrong args\n");
		return;
	}

	const sAchievementCond* pCond = (const sAchievementCond*)args[0].GetUserData();
	if (pCond)
	{
		int data;
		if (pCond->GetDataInt(data))
		{
			results.push_back((double)data);
		}
	}
}
IMPLEMENT_SCRIPT_API(GetAchCondDataInt);

void GetAchievementName(vector<CScriptValue>& args, vector<CScriptValue>& results)
{
	if (args.size() < 1 || args[0].m_Type != CScriptValue::SVT_NUMBER)
	{
		LUA_DEBUG_INFO("GetAchievementName, wrong args\n");
		return;
	}

	unsigned int id = (unsigned int)args[0].GetDouble();
	AWString name;
	if (CLuaApiEnv::GetAchievementName(name, id))
	{
		CScriptValue var;
		var.SetVal(name);
		results.push_back(var);
	}
}
IMPLEMENT_SCRIPT_API(GetAchievementName);


void InitLuaAchievementApi()
{
	REGISTER_SCRIPT_API(GameAI, GetMonsterName);
	REGISTER_SCRIPT_API(GameAI, GetItemName);
	REGISTER_SCRIPT_API(GameAI, GetTaskName);
	REGISTER_SCRIPT_API(GameAI, GetReputationName);
	REGISTER_SCRIPT_API(GameAI, GetSkillName);
	REGISTER_SCRIPT_API(GameAI, GetProduceName);
	REGISTER_SCRIPT_API(GameAI, GetProfName);
	REGISTER_SCRIPT_API(GameAI, GetAchievementName);
	REGISTER_SCRIPT_API(GameAI, GetAchCondDataInt);
}
