/********************************************************************
  created:	   9/5/2006  
  filename:	   LuaDlgAPI.h
  author:      Wangkuiwu  
  description: 
  Copyright (c) , All Rights Reserved.
*********************************************************************/
#ifdef LUA_UI

#pragma  once

typedef struct lua_State lua_State;
class CLuaState;

namespace LuaBind
{
	void InitDlgApi(lua_State * L);
	bool PreLoadUI(CLuaState * pUIState);
}

#endif