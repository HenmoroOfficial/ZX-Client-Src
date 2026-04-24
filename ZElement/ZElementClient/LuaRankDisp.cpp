/********************************************************************
	created:	2006/08/25
	author:		kuiwu
	
	purpose:	use lua to config rank display
	Copyright (C) 2006 - All Rights Reserved
*********************************************************************/

#include <AString.h>
#include <ABaseDef.h>
#include "LuaRankDisp.h"
#include "ScriptValue.h"
#include "LuaState.h"
#include "LuaScript.h"


//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////
#define  LUA_TBL_RANK_DISP   "rank_disp"
#define  LUA_RANK_DISP_FILE  "Interfaces\\script\\rank_disp_u.lua"
using namespace LuaBind;


CLuaRankDisp::CLuaRankDisp()
{
	m_pState =  g_LuaStateMan.GetConfigState();  //use config state?
}

CLuaRankDisp::~CLuaRankDisp()
{
	Release();
}

 

bool CLuaRankDisp::Init()
{
	assert(m_pState);


	m_pState->Lock();
	
	
	CLuaScript * pScript = m_pState->RegisterFile(LUA_RANK_DISP_FILE);

	if (!pScript)
	{
		m_pState->Unlock();
		return false;
	}

	m_pState->Unlock();
	return  true;
}

void CLuaRankDisp::Release()
{
	abase::vector<AString> rmTbls;
	rmTbls.push_back(LUA_TBL_RANK_DISP);
	m_pState->Lock();
	m_pState->UnRegister(LUA_RANK_DISP_FILE, &rmTbls);
	m_pState->Unlock();
}


// bool  CLuaRankDisp::GetDisp(int id, int rank, int last_rank, const AWString& name, int prof, __int64 val, AWString& disp)
// {
// 	assert(m_pState);
// 	m_pState->Lock();
// 	lua_State * L = m_pState->GetVM();
// 	
// 	int n = lua_gettop(L);
// 
// 	Push(TypeWrapper<int>(), L, id);
// 	Push(TypeWrapper<int>(), L, rank);
// 	Push(TypeWrapper<int>(), L, last_rank);
// 	char aName[256];
// 	wc2utf8(name, aName, 256);
// 	Push(TypeWrapper<const char*>(), L, aName);
// 	Push(TypeWrapper<int>(), L, prof);
// 	Push(TypeWrapper<__int64>(), L, val);
// 
// 	bool rt = LuaBind::Call(L, LUA_TBL_RANK_DISP, "GetDescription",  6, 1);
// 
// 	if (rt)
// 	{
// 		if (!CheckValue(TypeWrapper<const wchar_t *>(), L, 1, disp, __FILE__, __LINE__))
// 		{
// 			rt = false;
// 		}
// 		Pop(L, 1);
// 	}
// 
// 	lua_settop(L, n);
// 	m_pState->Unlock();
// 	return rt;
// }


bool  CLuaRankDisp::GetDisp(int id, int rank, int last_rank, const AWString& name, int prof, __int64 val, int val2 , AWString& disp)
{
	assert(m_pState);

	abase::vector<CScriptValue> args;
	abase::vector<CScriptValue> results;

	args.push_back(CScriptValue((double)id));
	args.push_back(CScriptValue((double)rank));
	args.push_back(CScriptValue((double)last_rank));
	CScriptString str;
	str.SetAWString(name);
	args.push_back(CScriptValue(str));
	args.push_back(CScriptValue((double)prof));	
	args.push_back(CScriptValue((double)val));	
	args.push_back(CScriptValue((double)val2));
	
	if (!m_pState->LockCall(LUA_TBL_RANK_DISP, "GetDescription", args, results))
	{
		return false;
	}
	 
	if (results.size() != 1 || results[0].m_Type != CScriptValue::SVT_STRING)
	{
		return false;
	}
	
	results[0].RetrieveAWString(disp);
	return true;
}