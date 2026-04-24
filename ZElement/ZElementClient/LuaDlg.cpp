/********************************************************************
  created:	   19/4/2006  
  filename:	   LuaDlg.cpp
  author:      Wangkuiwu  
  description: 
  Copyright (c) , All Rights Reserved.
*********************************************************************/

#ifdef LUA_UI


#include "LuaDlg.h"
#include "LuaBind.h"
#include "LuaUtil.h"
#include "AUI\\AStringWithWildcard.h"
#include "AUI\\AUICommon.h"
#include "EC_UIPlayer.h"
#include "LuaManager.h"
#include "LuaState.h"

#define DLG_DEBUG

CLuaDlg::CLuaDlg()
:m_bRenderCB(false), m_bTickCB(false)
{

	
}

CLuaDlg::~CLuaDlg()
{

}


bool CLuaDlg::LuaInit(const char * szScript)
{

	if (!CLuaScript::LuaInit(szScript))
	{
		return false;
	}
	
	lua_State * L = m_pLuaState->GetVM();
	//save this
	LuaBind::SetField(L, m_szName, "this", LuaBind::TypeWrapper<void *>(),  this);
	
	//check render&tick
	if (!LuaBind::GetTable(L, m_szName))
	{
		return false;
	}
	LuaBind::GetField(L, "Render");
	//m_bRenderCB = LuaBind::Match(LuaBind::TypeWrapper<LuaBind::LuaFunc>(), LuaBind::g_State, -1);
	m_bRenderCB = LuaBind::Match(L, -1, LUA_TFUNCTION);
	LuaBind::Pop(L, 1);
	LuaBind::GetField(L, "OnTick");
	//m_bTickCB = LuaBind::Match(LuaBind::TypeWrapper<LuaBind::LuaFunc>(), LuaBind::g_State, -1);
	m_bTickCB = LuaBind::Match(L, -1, LUA_TFUNCTION);
	LuaBind::Pop(L, 1);
	LuaBind::Pop(L, 1);
	
	//let lua  init dialog
	LuaBind::Call(L, m_szName, "OnInitDialog" );

	//debug  By kuiwu. [18/4/2006]
#ifdef DLG_DEBUG
	LuaBind::DumpStack(L, __FILE__, __LINE__);
#endif

	return true;
}

bool CLuaDlg::Release()
{
	//give scipt a chance to release
	LuaBind::Call(m_pLuaState->GetVM(), m_szName, "OnDestroy");
	
	if(!AUIDialog::Release())
	{
		return false;
	}
	return LuaRelease();
}

bool CLuaDlg::LuaRelease()
{
	lua_State * L = m_pLuaState->GetVM();
	LuaBind::ReleaseTbl(L, m_szName);

	CLuaScript::LuaRelease();

#ifdef DLG_DEBUG
	LuaBind::DumpStack(L, __FILE__, __LINE__);
#endif

	return true;
}

bool CLuaDlg::OnInitDialog()
{
	if (!AUIDialog::OnInitDialog())
	{
		return false;
	}

	CLuaScript::SetLuaState(g_LuaStateMan.GetUIState());
	
	//char path[200];
	//sprintf(path, "Interfaces\\script\\%s.lua",  m_szName);
	AString path;
	path.Format("Interfaces\\script\\%s.lua",  m_szName);
	TLuaFile * pLuaFile = new TLuaFile(path, this);
	if (!pLuaFile->Update(true))
	{
		delete pLuaFile;
		return false;
	}
	//move it to func LuaInit, and we can modify the script at runtime
	//LuaBind::Call(m_szName, "OnInitDialog" );

	g_LuaFileMan.AddScript(pLuaFile);
	
	return true;

}


bool CLuaDlg::OnCommandMap(const char *szCommand)
{
	lua_State * L = m_pLuaState->GetVM();
	LuaBind::Push(LuaBind::TypeWrapper<const char *>(), L, szCommand);
	LuaBind::Call(L, m_szName, "OnEventMap", 1);
	return true;
}

bool CLuaDlg::OnEventMap(UINT uMsg, WPARAM wParam, LPARAM lParam, PAUIOBJECT pObj)
{

	lua_State * L = m_pLuaState->GetVM();
	int nArgs = 0;
	const char * name = (pObj)? (pObj->GetName()) : ("");
	switch(uMsg) 
	{
	case WM_MOUSEWHEEL:
		{
			LuaBind::Push(LuaBind::TypeWrapper<int>(), L, uMsg);
			++nArgs;
			short	fwKeys = GET_KEYSTATE_WPARAM(wParam);
			short   zDelta = GET_WHEEL_DELTA_WPARAM(wParam);
			LuaBind::Push(LuaBind::TypeWrapper<int>(), L, fwKeys);
			++nArgs;
			LuaBind::Push(LuaBind::TypeWrapper<int>(), L, zDelta);
			++nArgs;
			short    lo  = (short) LOWORD(lParam);  //maybe x 
			short    hi  = (short) HIWORD(lParam);  //maybe y
			LuaBind::Push(LuaBind::TypeWrapper<int>(), L, lo);
			++nArgs;
			LuaBind::Push(LuaBind::TypeWrapper<int>(), L, hi);
			++nArgs;
			LuaBind::Push(LuaBind::TypeWrapper<const char *>(), L, name);
			++nArgs;
			//LuaBind::Call(m_szName, "OnMouseWheel", 6);
		}
		break;
	
	default:
		{
			LuaBind::Push(LuaBind::TypeWrapper<int>(), L, uMsg);
			++nArgs;
			LuaBind::Push(LuaBind::TypeWrapper<int>(), L, wParam);
			++nArgs;
			short    lo  = (short) LOWORD(lParam);  //maybe x 
			short    hi  = (short) HIWORD(lParam);  //maybe y
			LuaBind::Push(LuaBind::TypeWrapper<int>(), L, lo);
			++nArgs;
			LuaBind::Push(LuaBind::TypeWrapper<int>(), L, hi);
			++nArgs;
			LuaBind::Push(LuaBind::TypeWrapper<const char *>(),L, name);
			++nArgs;
			//LuaBind::Call(m_szName, "OnEventMap", 5);		
		}
		break;
	}

	LuaBind::Call(L, m_szName, "OnEventMap", nArgs);		

	return true;
}

void CLuaDlg::OnTick()
{
	if (m_bTickCB)
	{
		LuaBind::Call(m_pLuaState->GetVM(), m_szName, "OnTick");
	}
}

bool CLuaDlg::Render()
{
	if (m_bRenderCB && IsShow() )
	{
		LuaBind::Call(m_pLuaState->GetVM(), m_szName, "Render");
	}
	return AUIDialog::Render();
}

void CLuaDlg::OnShowDialog()
{
	AUIDialog::OnShowDialog();
	LuaBind::Call(m_pLuaState->GetVM(), m_szName, "OnShowDialog");
}

bool CLuaDlg::InitCustomizeControl(const char *szObjectType, AScriptFile* pASF)
{
	bool bval;
	if( 0 == strcmpi(szObjectType, "PLAYERDEMO") )
	{
		CECUIPlayer * pObject = new CECUIPlayer;
		if( !pObject ) return false;
		pObject->SetParent(this);

		AUIOBJECT_SETPROPERTY p;
		strcpy(p.c, szObjectType);
		pObject->SetProperty("Object Type", &p);
		bval = pObject->Init(m_pA3DEngine, m_pA3DDevice, pASF);
		if( !bval ) return AUI_ReportError(__LINE__, 1, "CLuaDlg::InitCustomizeControl, Failed to init control AUICustomize");
		bval = AddObjectToList(pObject);
		if( !bval ) return AUI_ReportError(__LINE__, 1, "CLuaDlg::InitCustomizeControl, Failed to call AddObjectToList()");

		return true;
	}
	
	
	return AUIDialog::InitCustomizeControl(szObjectType, pASF);
}

#endif