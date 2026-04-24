/********************************************************************
  created:	   19/4/2006  
  filename:	   LuaBind.cpp
  author:      Wangkuiwu  
  description: 
  Copyright (c) , All Rights Reserved.
*********************************************************************/

#include "LuaBind.h"
#include "LuaUtil.h"


#include "LuaState.h"
#include "ScriptValue.h"
#include "LuaGameAPI.h"
#include "aui\\LuaDlgAPI.h"
#include "EC_Achievement.h"



const	char *  ai_preload_scripts[] = 
{
		"Interfaces\\script\\utility.lua",
		"Interfaces\\script\\constant.lua",
		"Interfaces\\script\\text.lua",
		"Interfaces\\script\\AchievementCfg.lua",
		"Interfaces\\script\\interfaces\\dlgtemplate.lua",
		"Interfaces\\script\\interfaces\\uimantemplate.lua",
		"Interfaces\\script\\interfaces\\TowerData.lua",
		"Interfaces\\script\\interfaces\\TGllkData.lua",
		"Interfaces\\script\\interfaces\\SecretaryData.lua",
		"Interfaces\\script\\cutscene.lua",
		NULL
};



namespace LuaBind
{





bool LuaPreLoad(const char ** preload_scripts, CLuaState * pState)
{

	pState->Lock(); //??? need lock?

	int i = 0;
	bool ret = true;
	while (preload_scripts[i])
	{
		CLuaScript * pScript = pState->RegisterFile(preload_scripts[i]);
		if (!pScript)
		{
			AString strMsg;
			strMsg.Format("failed to register lua: %s", preload_scripts[i]);
			MessageBoxA(NULL, strMsg, "lua error", MB_OK | MB_ICONSTOP);
			ret = false;
			break;
		}
		++i;
	}		
	
	pState->Unlock();
	return  ret;
}


bool Init(bool bDbg)
{

	if (!g_LuaStateMan.Init(bDbg))
	{
		return false;
	}
	 

	
	InitAIApi(g_LuaStateMan.GetAIState());
	InitDlgApi(g_LuaStateMan.GetAIState());

	if (!LuaPreLoad(ai_preload_scripts, g_LuaStateMan.GetAIState()))
	{
	  return false;
	}
	
	bool bResult = CECAchievementMan::PreLoadDataFromScript();
	ASSERT(bResult);
	
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
