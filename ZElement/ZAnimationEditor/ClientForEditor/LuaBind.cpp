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
//#include "LuaGameAPI.h"
//#include "aui\\LuaDlgAPI.h"
//#include "EC_Achievement.h"



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
	 

	return true;
}

void Release()
{
	g_LuaStateMan.Release();
}


}