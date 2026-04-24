/********************************************************************
  created:	   19/4/2006  
  filename:	   LuaBind.cpp
  author:      Wangkuiwu  
  description: 
  Copyright (c) , All Rights Reserved.
*********************************************************************/
#include "StdAfx.h"
#include "LuaBind.h"

#include "aui\\AUILuaManager.h"
#include "LuaState.h"
#include "LuaScript.h"
#include <AString.h>
#include "LuaUtil.h"


void Lua_ReportErrorFunc( const char *szMsg)
{
	int len = strlen(szMsg);
	WCHAR *pBuffer = new WCHAR[len + 1];
	if(pBuffer)
	{
		int r2 = MultiByteToWideChar(CP_ACP,0,szMsg,len,pBuffer,len + 1);
		pBuffer[r2] = 0;
		AfxMessageBox(pBuffer);
		delete []pBuffer;
	}
}

namespace LuaBind
{

	bool Init()
	{
		
		if (!g_LuaStateMan.Init())
		{
			return false;
		}
		RegisterLuaErrHandler(Lua_ReportErrorFunc);
		return true;
	}
	
	void Release()
	{
		g_LuaStateMan.Release();
	}
	
}
