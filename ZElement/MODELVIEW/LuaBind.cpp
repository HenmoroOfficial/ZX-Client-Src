/********************************************************************
  created:	   19/4/2006  
  filename:	   LuaBind.cpp
  author:      Wangkuiwu  
  description: 
  Copyright (c) , All Rights Reserved.
*********************************************************************/

#include "LuaBind.h"
#include "LuaUtil.h"

#ifdef LUA_UI
#include "LuaDlgAPI.h"
#endif

#include "LuaState.h"
#include "ScriptValue.h"



//#define  LUA_TESTBED

#ifdef LUA_TESTBED
#include "LuaRankDisp.h"
#include "LuaMemTbl.h"
#include "LuaScript.h"
#endif



namespace LuaBind
{

bool PreLoad()
{

#ifdef LUA_UI
	if (!PreLoadUI(g_LuaStateMan.GetUIState()))
	{
		return false;
	}
#endif

	return  true;

}

#ifdef LUA_TESTBED

void CPrint(abase::vector<CScriptValue>& args, abase::vector<CScriptValue>& results)
{
	if (args.size()> 0 && args[0].m_Type == CScriptValue::SVT_STRING)
	{
		LUA_DEBUG_INFO(args[0].m_String.GetDataBuffer());
	}
}

IMPLEMENT_SCRIPT_API(CPrint);

void test_api()
{
   REGISTER_SCRIPT_API(CApi, CPrint);
}


void test_call_lua()
{
	g_LuaStateMan.GetConfigState()->Lock();
   	CLuaScript * pTestScript = new CLuaScript(g_LuaStateMan.GetConfigState());
	TLuaFile   * pLuaFile = new TLuaFile("Interfaces\\script\\test_val.lua", pTestScript);
	pLuaFile->Update(true);
	
	lua_State * L = g_LuaStateMan.GetConfigState()->GetVM();
	abase::vector<CScriptValue> args;
	CScriptValue val;

	val.m_Type = CScriptValue::SVT_STRING;
	val.m_String = "hello";
	args.push_back(val);
	
	val.m_Type = CScriptValue::SVT_STRING;
	val.m_String = " world";
	args.push_back(val);

	val.m_Type = CScriptValue::SVT_BOOL;
	val.m_Bool  = false;
	args.push_back(val);


	abase::vector<CScriptValue> rets;


	LuaBind::Call(L, "test_val", "GetInfo", args, rets);

	//just to make sure
	LUA_DEBUG_INFO("return from lua\n");
	for(int i = 0; i < (int)rets.size(); ++i)
	{
		DumpValue(rets[i]);
	}
	delete pTestScript;
	delete pLuaFile;
	pTestScript = 0;
	pLuaFile = 0;

	LuaBind::DumpStack(L, __FILE__, __LINE__);
	g_LuaStateMan.GetConfigState()->Unlock();

}

#endif


bool Init()
{

	
	
	if (!g_LuaStateMan.Init())
	{
		return false;
	}
	 

	
	//InitAIApi(g_LuaStateMan.GetAIState()->GetVM());

	if (!PreLoad())
	{
	  return false;
	}
	


	//////////////////////////////////////////////////////////////////////////
	//test lua memory table
// 	CLuaMemTbl  memTbl;
// 	const char * szTblName = "Ecm0123";
// 	memTbl.Init(szTblName);
// 	abase::vector<AString> vargs;
// 	vargs.push_back("i");
// 	vargs.push_back("j");
// 	const char * szBody = "local k = i + j;\nreturn k;";
// 	memTbl.AddMethod("Test", vargs, szBody);
// 	
// 	lua_State * L = memTbl.GetState()->GetVM();
// 	int rt;
// 	lua_pushnumber(L, 22);
// 	lua_pushnumber(L, 11);
// 	memTbl.Call("Test", 2, 1);
// 	rt = luaL_checkint(L, 1);
// 	lua_pop(L, 1);
// 
// 	//memTbl.Release();
// 	memTbl.RemoveMethod("Test");
// 
// 	lua_pushnumber(L, 22);
// 	lua_pushnumber(L, 11);
// 	using namespace LuaBind;
// 	if (memTbl.Call("Test", 2, 1))
// 	{
// 		CheckValue(TypeWrapper<int>(), L, 1, rt);
// 		void * pp;
// 		CheckValue(TypeWrapper<LuaUserData>(), L, 1, pp);
// 				
// 		lua_pop(L, 1);
// 	}

// 	CLuaRankDisp  rankdisp;
// 	rankdisp.Init();
// 	abase::vector<AWString> cat;
// 	abase::vector<RankSubCategory> subCat;
// 	rankdisp.GetCategoryName(cat);
// 	rankdisp.GetSubCategory(cat[0], subCat);
// 
// 	AWString name = L"°¢½ð±Ì";
// 	AWString disp;
// 	rankdisp.GetDisp(123, 1, 2, name, 11, 22, disp);
// 
//  	lua_State * L = g_LuaStateMan.GetConfigState()->GetVM();	
//  	LuaBind::DumpStack(L, __FILE__, __LINE__);

#ifdef LUA_TESTBED
	test_api();
	lua_State * L = g_LuaStateMan.GetConfigState()->GetVM();
	Reg2Script(L);
	test_call_lua();
#endif
	//////////////////////////////////////////////////////////////////////////
		


	
	//lua_checkstack(g_LuaStateMan.m_MasterState, 100);
	//DumpStack(g_LuaStateMan.m_MasterState, __FILE__, __LINE__);
	//DumpStack(g_LuaStateMan.GetConfigState()->GetState(), __FILE__, __LINE__);

	return true;
}

void Release()
{

	g_LuaStateMan.Release();

	
}


}