/********************************************************************
	created:	2007/04/28
	author:		kuiwu
	
	purpose:	the helper 
	Copyright (C) 2007 - All Rights Reserved
*********************************************************************/



#include "EC_Helper.h"
#include <assert.h>
#include "luastate.h"
#include "LuaUtil.h"
#include "LuaScript.h"
#include <AString.h>
#include "ScriptValue.h"
//#include "LuaGameAPI.h"
using namespace LuaBind;
#include <AAssist.h>
#include "EC_Monster.h"
#include "ExpTypes.h"
#include "DlgTask.h"
#include <vector.h>
using namespace abase;

#include "EC_Game.h"
#include "EC_GameRun.h"
#include "EC_HostPlayer.h"
#include "EC_Global.h"
#include "EC_Matter.h"

#include "aabbcd.h"
#include "EC_CDR.h"
#include "AUI\\CSplit.h"


//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

#define  AI_TBL_NAME    "AI_Helper"
#define  LUA_AIHELPER_FILE   "Interfaces\\script\\AI_Helper.lua"
#define  LOCAL_DIR "userdata\\helper"

CECHelper::CECHelper()
:m_pState(NULL),
 m_bTransferEnable(false),
 m_bRunning(false),
 m_bAnswerQuestion(false),
 m_pMonsterSelector(NULL),
 m_pMatterSelector(NULL)
{
	m_Counter.SetPeriod(200);
	m_TransferCounter.SetPeriod(1800000);
//	m_TransferCounter.SetPeriod(120000);
}

CECHelper::~CECHelper()
{
	Release();
}


bool CECHelper::Init()
{
	m_pState = g_LuaStateMan.GetAIState();
	if (!m_pState)
	{
		a_LogOutput(1, "CECHelper::Init, null lua state\n");
		return false;
	}


	//currently, no need to lock the luastate  [4/28/2007 kuiwu]

// 	CLuaScript   tmpScript(LUA_AIHELPER_FILE, m_pState);
// 	CLuaScript   * pScript = g_LuaFileMan.Register(tmpScript);
// 	if (pScript->Execute() == CLuaScript::EXECUTE_FAIL)
// 	{
// 		assert(0);
// 		g_LuaFileMan.UnRegister(LUA_AIHELPER_FILE, m_pState);
// 		AString msg;
// 		msg.Format("preload %s fail\n", LUA_AIHELPER_FILE);
// 		LUA_DEBUG_INFO(msg);
// 		return false;
// 	}

	CLuaScript   * pScript = m_pState->RegisterFile(LUA_AIHELPER_FILE);
	if (!pScript)
	{
		return false;
	}
	

	Call(m_pState->GetVM(), AI_TBL_NAME, "Init");

	m_pMonsterSelector = new CMonsterSelector;
	m_pMatterSelector = new CMatterSelector;
	
	return true;
}

void CECHelper::Reset()
{
	vector<CScriptValue>   args;
	vector<CScriptValue>   results;
	Call(m_pState->GetVM(), AI_TBL_NAME, "Reset", args, results);
	Stop();
}

void CECHelper::Start()
{
	vector<CScriptValue>   args;
	vector<CScriptValue>   results;
	Call(m_pState->GetVM(), AI_TBL_NAME, "Start", args, results);
	m_bRunning = true; 
	m_TransferCounter.Reset();
}

void CECHelper::Release()
{
	if (m_pState&& m_pState->GetVM())
	{
		ReleaseTbl(m_pState->GetVM(), AI_TBL_NAME);
		m_pState->UnRegister(LUA_AIHELPER_FILE);
	}

	if (m_pMatterSelector)
	{
		delete m_pMatterSelector;
		m_pMatterSelector = NULL;
	}

	if (m_pMonsterSelector)
	{
		delete	m_pMonsterSelector;
		m_pMonsterSelector = NULL;
	}
	
//	g_LuaFileMan.UnRegister(LUA_AIHELPER_FILE, m_pState);
}

void CECHelper::Tick(DWORD dwDelta)
{
	if (!m_pState || !m_pState->GetVM())
	{
		return;
	}
	
	if( g_pGame->GetGameRun()->GetHostPlayer()->IsFlying() )
	{
		if (m_Counter.IncCounter(dwDelta))
		{
			vector<CScriptValue>   args;
			vector<CScriptValue>   results;
			args.push_back(CScriptValue((double)m_Counter.GetCounter()));
			Call(m_pState->GetVM(), AI_TBL_NAME, "TickFly", args, results);
			m_Counter.Reset();
		}
		return;
	}

	if (m_bRunning )
	{
		if(m_bTransferEnable && m_TransferCounter.IncCounter(dwDelta) )
		{
			// 天人合一状态下，将背包中的特定物品定时转移到随身包裹中
			g_pGame->GetGameRun()->GetHostPlayer()->MoveItemPack2Pocket();
			m_TransferCounter.Reset();
		}
		if( m_Counter.IncCounter(dwDelta))
		{
			vector<CScriptValue>   args;
			vector<CScriptValue>   results;
			args.push_back(CScriptValue((double)m_Counter.GetCounter()));
			Call(m_pState->GetVM(), AI_TBL_NAME, "Tick", args, results);
			m_Counter.Reset();

			//DumpStack(m_pState->GetVM(), __FILE__, __LINE__);

		}
	}
	
}

int CECHelper::GetHpDelay()
{
	vector<CScriptValue> args;
	vector<CScriptValue> results;
	Call(m_pState->GetVM(), AI_TBL_NAME, "GetHpDelay", args, results);
	
	
	if (results.size() == 1 && results[0].m_Type == CScriptValue::SVT_NUMBER)
	{
		return results[0].GetInt();
	}
	else
	{
		a_LogOutput(1, "CECHelper::GetHpDelay, returns error!");
		return 0;
	}

}

void CECHelper::SetHpDelay(int delay)
{
	vector<CScriptValue> args;
	vector<CScriptValue> results;
	args.push_back(CScriptValue((double)delay));
	Call(m_pState->GetVM(), AI_TBL_NAME, "SetHpDelay", args, results);
}

int CECHelper::GetHpRate()
{
	vector<CScriptValue> args;
	vector<CScriptValue> results;
	Call(m_pState->GetVM(), AI_TBL_NAME, "GetHpRate", args, results);
	
	
	if (results.size() == 1 && results[0].m_Type == CScriptValue::SVT_NUMBER)
	{
		return results[0].GetInt();
	}
	else
	{
		a_LogOutput(1, "CECHelper::GetHpRate, returns error!");
		return 0;
	}

}

void CECHelper::SetHpRate(int rate)
{
	vector<CScriptValue> args;
	vector<CScriptValue> results;
	args.push_back(CScriptValue((double)rate));
	Call(m_pState->GetVM(), AI_TBL_NAME, "SetHpRate", args, results);
}




int CECHelper::GetMpDelay()
{
	vector<CScriptValue> args;
	vector<CScriptValue> results;
	Call(m_pState->GetVM(), AI_TBL_NAME, "GetMpDelay", args, results);
	
	
	if (results.size() == 1 && results[0].m_Type == CScriptValue::SVT_NUMBER)
	{
		return results[0].GetInt();
	}
	else
	{
		a_LogOutput(1, "CECHelper::GetMpDelay, returns error!");
		return 0;
	}

}

void CECHelper::SetMpDelay(int delay)
{
	vector<CScriptValue> args;
	vector<CScriptValue> results;
	args.push_back(CScriptValue((double)delay));
	Call(m_pState->GetVM(), AI_TBL_NAME, "SetMpDelay", args, results);
}

int CECHelper::GetMpRate()
{
	vector<CScriptValue> args;
	vector<CScriptValue> results;
	Call(m_pState->GetVM(), AI_TBL_NAME, "GetMpRate", args, results);
	
	
	if (results.size() == 1 && results[0].m_Type == CScriptValue::SVT_NUMBER)
	{
		return results[0].GetInt();
	}
	else
	{
		a_LogOutput(1, "CECHelper::GetMpRate, returns error!");
		return 0;
	}

}

void CECHelper::SetMpRate(int delay)
{
	vector<CScriptValue> args;
	vector<CScriptValue> results;
	args.push_back(CScriptValue((double)delay));
	Call(m_pState->GetVM(), AI_TBL_NAME, "SetMpRate", args, results);
}


int CECHelper::GetSkillDelay(int index)
{
	vector<CScriptValue> args;
	vector<CScriptValue> results;

	a_Clamp(index, 1, 9);
	args.push_back(CScriptValue((double)index));
	Call(m_pState->GetVM(), AI_TBL_NAME, "GetSkillDelay", args, results);
	
	
	if (results.size() == 1 && results[0].m_Type == CScriptValue::SVT_NUMBER)
	{
		return results[0].GetInt();
	}
	else
	{
		a_LogOutput(1, "CECHelper::GetSkillDelay, returns error!");
		return 0;
	}

}

void CECHelper::SetSkillDelay(int index, int delay)
{
	vector<CScriptValue> args;
	vector<CScriptValue> results;

	args.push_back(CScriptValue((double)index));
	args.push_back(CScriptValue((double)delay));
	Call(m_pState->GetVM(), AI_TBL_NAME, "SetSkillDelay", args, results);
}

bool CECHelper::IsSkillDisabled()
{
	vector<CScriptValue> args;
	vector<CScriptValue> results;

	Call(m_pState->GetVM(), AI_TBL_NAME, "IsSkillDisabled", args, results);
	
	
	if (results.size() == 1 && results[0].m_Type == CScriptValue::SVT_BOOL)
	{
		return results[0].GetBool();
	}
	else
	{
		a_LogOutput(1, "CECHelper::IsSkillDisabled, returns error!");
		return false;
	}

}

void CECHelper::SetSkillDisabled(bool bDisabled)
{
	vector<CScriptValue> args;
	vector<CScriptValue> results;

	args.push_back(CScriptValue(bDisabled));
	Call(m_pState->GetVM(), AI_TBL_NAME, "SetSkillDisabled", args, results);
}

int CECHelper::GetAnchorRange()
{
	vector<CScriptValue> args;
	vector<CScriptValue> results;
	Call(m_pState->GetVM(), AI_TBL_NAME, "GetAnchorRange", args, results);

	if (results.size() == 1 && results[0].m_Type == CScriptValue::SVT_NUMBER)
	{
		return results[0].GetInt();
	}
	else
	{
		a_LogOutput(1, "CECHelper::GetAnchorRange, returns error!");
		return 0;
	}

}

void CECHelper::SetAnchorRange(int range)
{
	vector<CScriptValue> args;
	vector<CScriptValue> results;

	args.push_back(CScriptValue((double)range));

	Call(m_pState->GetVM(), AI_TBL_NAME, "SetAnchorRange", args, results);
}

int CECHelper::GetUseItemDelay(int index)
{
	vector<CScriptValue> args;
	vector<CScriptValue> results;

	a_Clamp(index, 1, 3);
	args.push_back(CScriptValue((double)index));
	Call(m_pState->GetVM(), AI_TBL_NAME, "GetUseItemDelay", args, results);
	
	
	if (results.size() == 1 && results[0].m_Type == CScriptValue::SVT_NUMBER)
	{
		return results[0].GetInt();
	}
	else
	{
		a_LogOutput(1, "CECHelper::GetUseItemDelay, returns error!");
		return 0;
	}
}

void CECHelper::SetUseItemDelay(int index, int delay)
{
	vector<CScriptValue> args;
	vector<CScriptValue> results;

	args.push_back(CScriptValue((double)index));
	args.push_back(CScriptValue((double)delay));
	Call(m_pState->GetVM(), AI_TBL_NAME, "SetUseItemDelay", args, results);
}

bool CECHelper::Save(int playerId)
{
	abase::vector<AWString> * list = NULL;
	AString file_name;
	CreateDirectoryA(LOCAL_DIR, NULL);
	AString sub_dir;
	sub_dir.Format("%s\\%d", LOCAL_DIR, playerId);
	CreateDirectoryA(sub_dir, NULL);
	file_name.Format("%s\\talisman.ini", sub_dir);
	AString strKey, strValue;
	int i;
	strValue.Format("%d", GetHpDelay());
	WritePrivateProfileStringA("Settings", "HpDelay", strValue, file_name);
	strValue.Format("%d", GetHpRate());
	WritePrivateProfileStringA("Settings", "HpRate", strValue, file_name);
	strValue.Format("%d", GetMpDelay());
	WritePrivateProfileStringA("Settings", "MpDelay", strValue, file_name);
	strValue.Format("%d", GetMpRate());
	WritePrivateProfileStringA("Settings", "MpRate", strValue, file_name);
	strValue.Format("%d", IsAnswerQuestion());
	WritePrivateProfileStringA("Settings", "AnswerQuestion", strValue, file_name);
	strValue.Format("%d", GetAnchorRange());
	WritePrivateProfileStringA("Settings", "AnchorRange", strValue, file_name);
	strValue.Format("%d", IsSkillDisabled());
	WritePrivateProfileStringA("Settings", "SkillDisabled", strValue, file_name);
	for(i = 1; i <= 9; i++)
	{
		strKey.Format("SkillDelay%d", i);
		strValue.Format("%d", GetSkillDelay(i));
		WritePrivateProfileStringA("Settings", strKey, strValue, file_name);
	}
	for(i=1; i<=3; ++i)
	{
		strKey.Format("UseItemDelay%d", i);
		strValue.Format("%d", GetUseItemDelay(i));
		WritePrivateProfileStringA("Settings", strKey, strValue, file_name);
	}
	return true;
}

bool CECHelper::Load(int playerId)
{
	AString strKey, strValue;
	AString file_name;
	AString sub_dir;
	sub_dir.Format("%s\\%d", LOCAL_DIR, playerId);
	file_name.Format("%s\\talisman.ini", sub_dir);
	int i;
	SetHpDelay(GetPrivateProfileIntA("Settings", "HpDelay", GetHpDelay(), file_name));
	SetHpRate(GetPrivateProfileIntA("Settings", "HpRate", GetHpRate(), file_name));
	SetMpDelay(GetPrivateProfileIntA("Settings", "MpDelay", GetMpDelay(), file_name));
	SetMpRate(GetPrivateProfileIntA("Settings", "MpRate", GetMpRate(), file_name));
	SetAnswerQuestion(GetPrivateProfileIntA("Settings", "AnswerQuestion", IsAnswerQuestion(), file_name) == 1);
	SetAnchorRange(GetPrivateProfileIntA("Settings", "AnchorRange", GetAnchorRange(), file_name));
	SetSkillDisabled(GetPrivateProfileIntA("Settings", "SkillDisabled", IsSkillDisabled(), file_name) == 1);
	for(i = 1; i <= 9; i++)
	{
		strKey.Format("SkillDelay%d", i);
		strValue.Format("%d", GetSkillDelay(i));
		SetSkillDelay(i, GetPrivateProfileIntA("Settings", strKey, GetSkillDelay(i), file_name));
	}
	for(i = 1; i<=3; ++i)
	{
		strKey.Format("UseItemDelay%d", i);
		strValue.Format("%d", GetSkillDelay(i));
		SetUseItemDelay(i, GetPrivateProfileIntA("Settings", strKey, GetUseItemDelay(i), file_name));
	}
	
	return true;
}

//helper selector
CMonsterSelector::CMonsterSelector()
:m_TargetType(CMonsterSelector::MONSTER_NORMAL),
m_bFullHp(false),
m_bTaskOnly(false),
m_iAttackRange(20),
m_CountLimit(100)
{

}

CMonsterSelector::~CMonsterSelector()
{

}

/*
//method1
bool CMonsterSelector::_IsLineClear(const A3DVECTOR3& start, const A3DVECTOR3& end)
{
	using namespace AutoMove;

	if (!g_AutoMoveAgent.GetMoveMap())
	{
		return true;
	}

	A3DPOINT2 start2d, end2d;
	start2d =	 g_AutoMoveAgent.GetMoveMap()->Trans3DTo2D(start);
	end2d   =    g_AutoMoveAgent.GetMoveMap()->Trans3DTo2D(end);
	A3DPOINT2 dir2d(end2d - start2d);

	CLine line;
	line.Init(APointF((float)start2d.x, (float)start2d.y), APointF((float)dir2d.x, (float)dir2d.y));
	
	APointF cur2dF;
	A3DPOINT2 cur2d;
	cur2dF = line.Next();
	cur2d.x = (int)cur2dF.x;
	cur2d.y = (int)cur2dF.y;
	while (cur2d != end2d)
	{
		if (!g_AutoMoveAgent.GetMoveMap()->IsPosReachable(cur2d))
		{
			return false;
		}
		
		cur2dF = line.Next();
		cur2d.x = (int)cur2dF.x;
		cur2d.y = (int)cur2dF.y;
	}
	
		
	return true;
}
*/
//method2
bool CMonsterSelector::_IsLineClear(const A3DVECTOR3& start, const A3DVECTOR3& end)
{
	using namespace CHBasedCD;
	
	const float up = 0.3f;

	BrushTraceInfo  info;
	A3DVECTOR3  from(start);
	from.y += up;
	A3DVECTOR3 to(end);
	to.y += up;
	A3DVECTOR3  delta(to - from);

	
	info.Init(from, delta, A3DVECTOR3(0), true);

	return	!AABBCollideWithBrush(&info);
}

bool CMonsterSelector::IsTarget(CECMonster * pMonster)
{
	if (m_bFullHp)
	{
		ROLEBASICPROP& rbp =	pMonster->GetBasicProps();
		ROLEEXTPROP&   rep =    pMonster->GetExtendProps();

		if (rbp.iCurHP != rep.max_hp )
		{
			return false;
		}
	}

	if (m_bTaskOnly)
	{
		if (CDlgTask::m_mapTaskMonster.end() == CDlgTask::m_mapTaskMonster.find(pMonster->GetTemplateID()))
		{
			return false;
		}
	}

	if (!_IsLineClear(g_pGame->GetGameRun()->GetHostPlayer()->GetPos(), pMonster->GetPos()))
	{
		return false;
	}

	if(_Find(m_ExcludeList, pMonster->GetName()) )
	{
		return false;
	}

	int level = pMonster->GetDBEssence()->show_level;
	if (((m_TargetType & MONSTER_NORMAL) == MONSTER_NORMAL)&& (level == 0) )
	{
		return true;
	}

	if (((m_TargetType & MONSTER_ELITE) == MONSTER_ELITE)&& (level < 4) && (level > 0) )
	{
		return true;
	}

	if (((m_TargetType & MONSTER_BOSS) == MONSTER_BOSS)&& (level == 4)  )
	{
		return true;
	}

	return false;
}

bool CMonsterSelector::Save(int playerId)
{
	abase::vector<AWString> * list = NULL;
	AString file_name;
	CreateDirectoryA(LOCAL_DIR, NULL);
	AString sub_dir;
	sub_dir.Format("%s\\%d", LOCAL_DIR, playerId);
	CreateDirectoryA(sub_dir, NULL);
	file_name.Format("%s\\talisman.ini", sub_dir);
	AString strKey, strValue;
	strValue.Format("%d", GetTargetType());
	WritePrivateProfileStringA("Settings", "TargetType", strValue, file_name);
	strValue.Format("%d", IsFullHp());
	WritePrivateProfileStringA("Settings", "FullHp", strValue, file_name);
	strValue.Format("%d", IsTaskOnly());
	WritePrivateProfileStringA("Settings", "TaskOnly", strValue, file_name);

	return true;
}

bool CMonsterSelector::Load(int playerId)
{
	AString strKey, strValue;
	AString file_name;
	AString sub_dir;
	sub_dir.Format("%s\\%d", LOCAL_DIR, playerId);
	file_name.Format("%s\\talisman.ini", sub_dir);
	SetTargetType(GetPrivateProfileIntA("Settings", "TargetType", GetTargetType(), file_name));
	SetFullHp(GetPrivateProfileIntA("Settings", "FullHp", IsFullHp(), file_name) == 1);
	SetTaskOnly(GetPrivateProfileIntA("Settings", "TaskOnly", IsTaskOnly(), file_name) == 1);

	return true;
}

bool CMonsterSelector::_Find(abase::vector<AWString>& list, const AWString& str)
{
	abase::vector<AWString>::iterator it;
	for (it = list.begin(); it != list.end(); ++it)
	{
		if (str.Find(*it) >= 0)
		{
			break;
		}
	}

	return (it != list.end());

}

bool CMonsterSelector::LoadMonsterList(int playerId)
{	
	abase::vector<AWString> * list = &m_ExcludeList;
	AString file_name;
	file_name.Format("%s\\%d\\excludemonster.txt", LOCAL_DIR, playerId);

	if (!list)
	{
		return false;
	}

	list->clear();

	AFile afile;
	if (!afile.Open(file_name, AFILE_OPENEXIST|AFILE_TEXT|AFILE_TEMPMEMORY))
	{
		return false;
	}

	int count = 0;

	char line[AFILE_LINEMAXLEN];
	DWORD  read_len;
	while (afile.ReadLine(line, AFILE_LINEMAXLEN, &read_len) && (count < m_CountLimit))
	{
		list->push_back(AS2WC(line));
		++count;
	}
	afile.Close();
	return true;
}

bool CMonsterSelector::SaveMonsterList(int playerId)
{
	abase::vector<AWString> * list = &m_ExcludeList;
	AString file_name;
	CreateDirectoryA(LOCAL_DIR, NULL);
	AString sub_dir;
	sub_dir.Format("%s\\%d", LOCAL_DIR, playerId);
	CreateDirectoryA(sub_dir, NULL);

	file_name.Format("%s\\excludemonster.txt", sub_dir);
/* 空的也需要保存
	if (!list|| list->empty())
	{
		return false;
	}
*/	AFile afile;
	if (!afile.Open(file_name, AFILE_CREATENEW|AFILE_TEXT|AFILE_TEMPMEMORY))
	{
		return false;
	}
	
	int count = 0;
	
	while (count < m_CountLimit && count < (int)list->size())
	{
		AString str(WC2AS(list->at(count)));
		afile.WriteLine(str);
		count++;
	}
	afile.Close();
	return true;
}

CMatterSelector::CMatterSelector()
:m_SelectType(CMatterSelector::MATTER_ALL),
 m_CountLimit(100),
 m_AutoPickMask(0),
 m_iAutoPickLevel1(1),
 m_iAutoPickLevel2(1),
 m_AutoUnPickMask(0),
 m_iAutoUnPickLevel1(1),
 m_iAutoUnPickLevel2(1)
{
	
}

CMatterSelector::~CMatterSelector()
{

}

bool CMatterSelector::IsTarget(CECMatter * pMatter)
{
	bool ret = true;
	switch(m_SelectType)
	{
	case MATTER_ALL:
		{
			ret = true;
			break;
		}
	case MATTER_NONE:
		{
			ret = false;
			break;
		}
	case MATTER_EXCLUDE:
		{
			ret = ( (!_Find(m_ExcludeList, pMatter->GetName())) && (!IsNotAutoPickItem(pMatter->GetTemplateID())) );
			break;
		}
	case MATTER_INCLUDE:
		{
			ret = (_Find(m_IncludeList, pMatter->GetName()) || IsAutoPickItem(pMatter->GetTemplateID()) );
			break;
		}
	default:
		break;
	}
	
	return ret;
}

bool CMatterSelector::_Find(abase::vector<AWString>& list, const AWString& str)
{
	abase::vector<AWString>::iterator it;
	for (it = list.begin(); it != list.end(); ++it)
	{
		if (str.Find(*it) >= 0)
		{
			break;
		}
	}

	return (it != list.end());

}

bool CMatterSelector::IsAutoPickItem(int tid)
{
	for (int i= 0; i < 4; ++i)
	{
		if (m_AutoPickMask & (1<<i))
		{
			int level = g_pGame->GetAutoPickItemLevel(i, tid);
			if ((i ==0 && level >= m_iAutoPickLevel1) || (i ==1 && level >= m_iAutoPickLevel2) || (i ==2 && level >0 ) || (i==3 && level > 0))
				return true;
		}
	}
	return false;
}

bool CMatterSelector::IsNotAutoPickItem(int tid)
{
	for (int i= 0; i < 4; ++i)
	{
		if (m_AutoUnPickMask & (1<<i))
		{
			int level = g_pGame->GetAutoPickItemLevel(i, tid);
			if (level>0)
				if ((i ==0 && level <= m_iAutoUnPickLevel1) || (i ==1 && level <= m_iAutoUnPickLevel2) || i ==2 || i==3 )
					return true;
		}
	}
	return false;
}

bool IsUnicode(char* strPath)
{
	FILE* fp = fopen(strPath, "rb");
	if (fp)
	{
		ACHAR firstChar;
		size_t sz = fread(&firstChar, sizeof(ACHAR), 1, fp);
		fclose(fp);
		return firstChar == 0xfeff;
	}
	return false;
}

bool CMatterSelector::Load(int playerId, int listType)
{
	AString file_name;
	AString strKey, strValue;
	AString sub_dir;
	sub_dir.Format("%s\\%d", LOCAL_DIR, playerId);
	file_name.Format("%s\\talisman.ini", sub_dir);
	m_AutoPickMask = GetPrivateProfileIntA("Settings", "AutoPickMask", m_AutoPickMask, file_name);
	m_iAutoPickLevel1 = GetPrivateProfileIntA("Settings", "AutoPickLev1", m_iAutoPickLevel1, file_name);
	m_iAutoPickLevel2 = GetPrivateProfileIntA("Settings", "AutoPickLev2", m_iAutoPickLevel2, file_name);
	m_AutoUnPickMask = GetPrivateProfileIntA("Settings", "AutoUnPickMask", m_AutoUnPickMask, file_name);
	m_iAutoUnPickLevel1 = GetPrivateProfileIntA("Settings", "AutoUnPickLev1", m_iAutoUnPickLevel1, file_name);
	m_iAutoUnPickLevel2 = GetPrivateProfileIntA("Settings", "AutoUnPickLev2", m_iAutoUnPickLevel2, file_name);
	m_SelectType = GetPrivateProfileIntA("Settings", "MatterSelectType", m_SelectType, file_name);
	
	abase::vector<AWString> * list = NULL;
	
	if (listType == MATTER_EXCLUDE)
	{
		list = &m_ExcludeList;
		file_name.Format("%s\\%d\\exclude.txt", LOCAL_DIR, playerId);
	}
	else if (listType == MATTER_INCLUDE)
	{
		list = &m_IncludeList;
		file_name.Format("%s\\%d\\include.txt", LOCAL_DIR, playerId);
	}

	if (!list)
	{
		return false;
	}

	list->clear();

	AFile afile;
	if (!IsUnicode(file_name.GetBuffer(0)))
	{
		AFile afile;
		if (!afile.Open(file_name, AFILE_OPENEXIST|AFILE_TEXT|AFILE_TEMPMEMORY))
		{
			return false;
		}
		
		int count = 0;
		char line[AFILE_LINEMAXLEN];
		DWORD  read_len;
		while (afile.ReadLine(line, AFILE_LINEMAXLEN, &read_len) && (count < m_CountLimit))
		{
			list->push_back(AS2WC(line));
			++count;
		}
		afile.Close();
	}
	else
	{
		if (!afile.Open(file_name, AFILE_OPENEXIST|AFILE_BINARY|AFILE_TEMPMEMORY))
		{
			return false;
		}

		int count = 0;
		char buf[8192];
		DWORD readsize = 8192;
		DWORD  read_len;
		if(afile.Read(buf, 8192, &read_len) && read_len>=4)
		{
			ACString strText((ACHAR*)(buf+2), (read_len-4)/2);
			CSplit s(strText);
			CSPLIT_STRING_VECTOR vec = s.Split(_AL("\r"));
			for (UINT i=0; i<vec.size(); i++)
			{
				list->push_back(vec[i]);
				++count;
				if (count >= m_CountLimit)
				{
					break;
				}
			}
		}
		afile.Close();
	}

	return true;
}

bool CMatterSelector::Save(int playerId, int listType)
{
	abase::vector<AWString> * list = NULL;
	AString file_name;
	CreateDirectoryA(LOCAL_DIR, NULL);
	AString sub_dir;
	sub_dir.Format("%s\\%d", LOCAL_DIR, playerId);
	CreateDirectoryA(sub_dir, NULL);

	file_name.Format("%s\\talisman.ini", sub_dir);
	AString strKey, strValue;
	strValue.Format("%d", m_AutoPickMask);
	WritePrivateProfileStringA("Settings", "AutoPickMask", strValue, file_name);
	strValue.Format("%d", m_iAutoPickLevel1);
	WritePrivateProfileStringA("Settings", "AutoPickLev1", strValue, file_name);
	strValue.Format("%d", m_iAutoPickLevel2);
	WritePrivateProfileStringA("Settings", "AutoPickLev2", strValue, file_name);
	strValue.Format("%d", m_AutoUnPickMask);
	WritePrivateProfileStringA("Settings", "AutoUnPickMask", strValue, file_name);
	strValue.Format("%d", m_iAutoUnPickLevel1);
	WritePrivateProfileStringA("Settings", "AutoUnPickLev1", strValue, file_name);
	strValue.Format("%d", m_iAutoUnPickLevel2);
	WritePrivateProfileStringA("Settings", "AutoUnPickLev2", strValue, file_name);
	strValue.Format("%d", m_SelectType);
	WritePrivateProfileStringA("Settings", "MatterSelectType", strValue, file_name);

	if (listType == MATTER_EXCLUDE)
	{
		list = &m_ExcludeList;
		file_name.Format("%s\\exclude.txt", sub_dir);
	}
	else if (listType == MATTER_INCLUDE)
	{
		list = &m_IncludeList;
		file_name.Format("%s\\include.txt", sub_dir);
	}
	
	FILE* afile = fopen(file_name, "wb");
	if (!afile)
	{
		return false;
	}
	
	unsigned short magic = 0xfeff;
 	fwrite(&magic, sizeof(magic), 1, afile);

	int count = 0;
	while (count < m_CountLimit && count < (int)list->size())
	{
		fwprintf(afile, _AL("%s\r"),list->at(count));
		count++;
	}
	
	fclose(afile);

	return true;
}

#undef  AI_TBL_NAME
#undef  LOCAL_DIR