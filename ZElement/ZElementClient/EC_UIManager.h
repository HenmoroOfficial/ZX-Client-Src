/*
 * FILE: EC_UIManager.h
 *
 * DESCRIPTION: 
 *
 * CREATED BY: Duyuxin, 2004/8/27
 *
 * HISTORY: 
 *
 * Copyright (c) 2004 Archosaur Studio, All Rights Reserved.
 */

#pragma once
#ifndef BACK_VERSION
#include "ECRObjBase.h"
#endif
///////////////////////////////////////////////////////////////////////////
//	
//	Define and Macro
//	
///////////////////////////////////////////////////////////////////////////


///////////////////////////////////////////////////////////////////////////
//	
//	Types and Global variables
//	
///////////////////////////////////////////////////////////////////////////

class A3DEngine;
class AUILuaManager;
class CECLoginUIMan;
class CECGameUIMan;
class CECScriptMan;
class CECPlayerImage;

///////////////////////////////////////////////////////////////////////////
//	
//	Declare of Global functions
//	
///////////////////////////////////////////////////////////////////////////


///////////////////////////////////////////////////////////////////////////
//	
//	Class CECUIManager
//	
///////////////////////////////////////////////////////////////////////////

#ifdef BACK_VERSION
class CECUIManager
#else
class CECUIManager : public ECRObjBase
#endif
{
public:		//	Types

	//	UI manager index
	enum
	{
		UIMAN_LOGIN = 0,	//	Login UI manager
		UIMAN_INGAME,		//	In game UI manager
	};

public:		//	Constructor and Destructor

	CECUIManager();
	virtual ~CECUIManager();

public:		//	Attributes

public:		//	Operations

	//	Initalize manager
	bool Init(A3DEngine* pA3DEngine, int iInitUIMan);
	//	Release manager
	void Release(void);

	//	Tick routine
	bool Tick(DWORD dwDeltaTime);
	//	Render routine
	bool Render();

	//	Change current UI manager
	bool ChangeCurUIManager(int iUIMan);
	//	Deal windows message
	bool DealWindowsMessage(UINT message, WPARAM wParam, LPARAM lParam);

	//	Get current UI manager
	int GetCurrentUIMan() { return m_iCurUIMan; }
	AUILuaManager* GetCurrentUIManPtr() { return m_pCurUIMan; }
	//	If current UI manager is LOGIN manager, this function return it's interface
	CECLoginUIMan* GetLoginUIMan();
	//	If current UI manager is INGAME manager, this function return it's interface
	CECGameUIMan* GetInGameUIMan();
	//	Get script help manager
	CECScriptMan* GetScriptMan() { return m_pScriptMan; }

	//  Get player image
	CECPlayerImage* GetPlayerImage() { return m_pPlayerImage; }

	bool UIControlCursor();
	void ShowErrorMsg(const ACHAR *pszMsg, const char *pszName="");
#ifndef BACK_VERSION
	virtual bool Render(const ECRENDERPARAM* pRenderParam);
#endif
protected:	//	Attributes

	A3DEngine*		m_pA3DEngine;	//	A3D engine object
	AUILuaManager*	m_pCurUIMan;	//	Current UI manager
	int				m_iCurUIMan;	//	Current UI manager index	
	
	// help system
	CECScriptMan * m_pScriptMan;
	CECPlayerImage * m_pPlayerImage;

protected:	//	Operations

	//	Release current UI manager
	void ReleaseCurUIManager();
};

///////////////////////////////////////////////////////////////////////////
//	
//	Inline functions
//	
///////////////////////////////////////////////////////////////////////////

