/********************************************************************
	created:	2006/08/16
	author:		kuiwu
	
	purpose:	get config info from lua script
	Copyright (C) 2006 - All Rights Reserved
*********************************************************************/
#pragma warning ( disable : 4284 )


#include <assert.h>
#include "LuaState.h"
#include "LuaUtil.h"
#include "LuaScript.h"

#include "ConfigFromLua.h"

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

#define  TITLE_DIR_NAME  "title_dir"
#define  TITLE_DEF_NAME   "title_definition"
#define  LUA_TITLE_FILE  "Interfaces\\script\\config\\title_def_u.lua"

CConfigFromLua::CConfigFromLua()
:m_TitleDef(256), m_TitleDesc(256)
{
	m_pState = NULL;
}

CConfigFromLua::~CConfigFromLua()
{
	Release();
}

bool CConfigFromLua::Init()
{
	m_pState = g_LuaStateMan.GetConfigState();
	assert(m_pState);
	
	m_pState->Lock();

	
	CLuaScript * pScript = m_pState->RegisterFile(LUA_TITLE_FILE);
	
	if (!pScript)
	{
		m_pState->Unlock();
		return false;
	}

	bool rt = _GetTitleDefTab();
	
// 	int count = m_TitleDef.size();
// 	m_TitleDef.clear();
// 	m_TitleDesc.clear();
// 	_GetTitleDefTab0();
// 	int count0 = m_TitleDesc.size();

	assert(rt);
	rt = _GetTitleDir();
	assert(rt);

	m_pState->Unlock();
	return	rt;



}

void CConfigFromLua::Release()
{
	m_TitleDef.clear();
	m_TitleDesc.clear();
	
	abase::vector<AString> rmTbls;

	rmTbls.push_back(TITLE_DEF_NAME);
	rmTbls.push_back(TITLE_DIR_NAME);

	m_pState->Lock();
	m_pState->UnRegister(LUA_TITLE_FILE, &rmTbls);
	m_pState->Unlock();
	
}


// bool CConfigFromLua::_GetTitleDefTab0()
// {
// 	using namespace LuaBind;
// 
// 	assert(m_pState);
// 	lua_State * L = m_pState->GetVM();
// 
// #define  TITLE_TAB_NAME   "title_definition"
// 	if (!GetTable(L, TITLE_TAB_NAME))
// 	{
// 		return false;
// 	}
// 
// 	
// 	
// 	Push(TypeWrapper<LuaNil>(), L);
// 	while (Next(L))
// 	{
// 		//key, {id, note, desc}
// 		
// 		int restore_idx = lua_gettop(L);  //save the stack pos 
// 		
// 		RawGetField(L, "id");
// 		int id;
// 		if (!CheckValue(TypeWrapper<int>(), L, -1, id, __FILE__, __LINE__))
// 		{
// 			//fail to get id
// 			lua_settop(L, restore_idx);
// 			Pop(L, 1);  // discard the record
// 			a_LogOutput(1, "CConfigFromLua::_GetTitleDefTab, fail to get id");
// 			continue;   // try to get the next record
// 		}
// 		Pop(L, 1);
// 
// 		RawGetField(L, "note");
// 		AWString note;
// 		if (!CheckValue(TypeWrapper<const WCHAR*>(), L, -1, note, __FILE__, __LINE__))
// 		{
// 			//fail to get note
// 			lua_settop(L, restore_idx);
// 			a_LogOutput(1, "CConfigFromLua::_GetTitleDefTab, fail to get id's(%d) note", id);
// 		}
// 		else
// 		{
// 			m_TitleDef.put(id, note);
// 			Pop(L, 1);  //pop the note
// 		}
// 		
// 		RawGetField(L, "desc");
// 		AWString desc;
// 		if (!CheckValue(TypeWrapper<const WCHAR*>(), L, -1, desc, __FILE__, __LINE__))
// 		{
// 			//fail to get note
// 			lua_settop(L, restore_idx);
// 			a_LogOutput(1, "CConfigFromLua::_GetTitleDefTab, fail to get id's(%d) desc", id);
// 		}
// 		else
// 		{
// 			m_TitleDesc.put(id, desc);
// 			Pop(L, 1);  //pop the note
// 		}
// 		
// 		Pop(L, 1);
// 	}
// 	
// 	Pop(L, 1);
// 	return true;
// }

bool CConfigFromLua::_GetTitleDefTab()
{



	abase::vector<CScriptValue> args;
	abase::vector<CScriptValue> results;
	
	if (!m_pState->Call(TITLE_DEF_NAME, "GetTitleDef", args, results))
	{
		return false;
	}
	 
	CScriptValue& ret0 = results[0];

	unsigned int i, j;
	for (i = 0; i < ret0.m_ArrVal.size(); i++)
	{
		abase::vector<CScriptValue>& ret0ik = ret0.m_ArrVal[i].m_ArrKey;
		abase::vector<CScriptValue>& ret0iv = ret0.m_ArrVal[i].m_ArrVal;
		int id;
		AWString note;
		AWString desc;

		for (j = 0; j < ret0ik.size(); j++)
		{
			CScriptValue& ret0ijk = ret0ik[j];
			if (ret0ijk.m_Type != CScriptValue::SVT_STRING)
			{
				return false;
			}
			AString key; 
			ret0ijk.RetrieveAString(key);
			if (strcmp(key, "id") == 0)
			{
				id = ret0iv[j].GetInt();
			}
			else if (strcmp(key, "note") == 0)
			{
				ret0iv[j].RetrieveAWString(note);	
			}
			else if (strcmp(key, "desc") == 0)
			{
				ret0iv[j].RetrieveAWString(desc);
			}
			else
			{
				return false;
			}
		}

		m_TitleDef.put(id, note);
		m_TitleDesc.put(id, desc);

	}


	return true;
}



bool CConfigFromLua::_GetTitleDir()
{
	abase::vector<CScriptValue> args;

	if (!m_pState->Call(TITLE_DIR_NAME, "GetDir", args, m_TitleDir))
	{
		a_LogOutput(1, "CConfigFromLua::_GetTitleDir, fail");
		return false;
	}
	 
	return true;
}