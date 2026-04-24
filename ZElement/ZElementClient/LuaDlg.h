/********************************************************************
  created:	   19/4/2006  
  filename:	   LuaDlg.h
  author:      Wangkuiwu  
  description: 
  Copyright (c) , All Rights Reserved.
*********************************************************************/
#ifdef LUA_UI

#pragma  once

#include "AUI\\AUIDialog.h"
#include "LuaScript.h"


#ifndef WM_MOUSEWHEEL
#define WM_MOUSEWHEEL                   0x020A
#endif
#ifndef GET_KEYSTATE_WPARAM
#define GET_KEYSTATE_WPARAM(wParam)     (LOWORD(wParam))
#endif
#ifndef GET_WHEEL_DELTA_WPARAM
#define WHEEL_DELTA                     120
#define GET_WHEEL_DELTA_WPARAM(wParam)  ((short)HIWORD(wParam))
#endif


class CLuaDlg  :public AUIDialog, public CLuaScript
{
public:
	CLuaDlg();
	virtual ~CLuaDlg();
	bool Release();
	bool Render();

	virtual	bool LuaInit(const char * szScript);
	virtual	bool LuaRelease();

protected:
	bool OnCommandMap(const char *szCommand);
	bool OnEventMap(UINT uMsg, WPARAM wParam, LPARAM lParam, PAUIOBJECT pObj);

	bool OnInitDialog();
	void OnTick();
	void OnShowDialog();
	bool InitCustomizeControl(const char *szObjectType, AScriptFile* pASF);
	
	bool m_bRenderCB;
	bool m_bTickCB;
	
};


#endif