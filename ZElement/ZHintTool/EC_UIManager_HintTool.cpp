/*
 * FILE: EC_UIManager.cpp
 *
 * DESCRIPTION: 
 *
 * CREATED BY: Duyuxin, 2004/8/27
 *
 * HISTORY: 
 *
 * Copyright (c) 2004 Archosaur Studio, All Rights Reserved.
 */

#include "EC_Global.h"
#include "EC_UIManager_HintTool.h"
//#include "EC_LoginUIMan.h"
#include "EC_GameUIMan_HintTool.h"
#include "EC_Game_HintTool.h"
#include "EC_GameRun_HintTool.h"
//#include "EC_Configs.h"
//#include "ECScriptManInGame.h"
//#include "EC_PlayerImage.h"

#include <A3DEngine.h>

#define new A_DEBUG_NEW

///////////////////////////////////////////////////////////////////////////
//	
//	Define and Macro
//	
///////////////////////////////////////////////////////////////////////////


///////////////////////////////////////////////////////////////////////////
//	
//	Reference to External variables and functions
//	
///////////////////////////////////////////////////////////////////////////


///////////////////////////////////////////////////////////////////////////
//	
//	Local Types and Variables and Global variables
//	
///////////////////////////////////////////////////////////////////////////


///////////////////////////////////////////////////////////////////////////
//	
//	Local functions
//	
///////////////////////////////////////////////////////////////////////////

/*
AUILuaManager*  GetAuiManager()
{
	return g_pGame->GetGameRun()->GetUIManager()->GetCurrentUIManPtr();
}

///////////////////////////////////////////////////////////////////////////
//	
//	Implement CECUIManager
//	
///////////////////////////////////////////////////////////////////////////
*/
CECUIManager::CECUIManager()
{
	m_pA3DEngine	= NULL;
	m_pCurUIMan		= NULL;
	m_iCurUIMan		= -1;
/*	m_pScriptMan	= NULL;
	m_pPlayerImage	= NULL;*/
}

CECUIManager::~CECUIManager()
{
}

//	Initalize manager
bool CECUIManager::Init(A3DEngine* pA3DEngine, int iInitUIMan)
{
	m_pA3DEngine	= pA3DEngine;
	m_iCurUIMan		= iInitUIMan;

/*	if (!ChangeCurUIManager(iInitUIMan))
	{
		a_LogOutput(1, "CECUIManager::Init, Failed to create initial UI manager");
		return false;
	}

	m_pPlayerImage = new CECPlayerImage;
	m_pPlayerImage->Init();
*/
	return true;
}

//	Release manager
void CECUIManager::Release(void)
{
//	A3DRELEASE(m_pPlayerImage)

	ReleaseCurUIManager();
}
/*
//	Change current UI manager
bool CECUIManager::ChangeCurUIManager(int iUIMan)
{
	if (iUIMan != m_iCurUIMan)
		ReleaseCurUIManager();

	if (iUIMan < 0)
		return true;

	if (iUIMan == UIMAN_LOGIN)
	{
		CECLoginUIMan* pLoginUI = new CECLoginUIMan;
		if (!pLoginUI)
		{
			glb_ErrorOutput(ECERR_NOTENOUGHMEMORY, "CECUIManager::ChangeCurUIManager", __LINE__);
			return false;
		}
		m_pCurUIMan = pLoginUI;
		if (!pLoginUI->Init(m_pA3DEngine, m_pA3DEngine->GetA3DDevice(), g_pGame->GetConfigs()->GetLoginUIDcf()))
		{
			m_pCurUIMan = NULL;
			::MessageBoxA(NULL, "无法加载游戏数据文件！\n此错误可能是更新不完全所致，请重新进行更新或进行版本验证！", "错误", MB_OK | MB_ICONSTOP);
			glb_ErrorOutput(ECERR_FAILEDTOCALL, "CECUIManager::ChangeCurUIManager", __LINE__);
			return false;
		}
	}
	else if (iUIMan == UIMAN_INGAME)
	{
		CECGameUIMan* pInGameUI = new CECGameUIMan;
		if (!pInGameUI)
		{
			glb_ErrorOutput(ECERR_NOTENOUGHMEMORY, "CECUIManager::ChangeCurUIManager", __LINE__);
			return false;
		}
		m_pCurUIMan = pInGameUI; 
		if (!pInGameUI->Init(m_pA3DEngine, m_pA3DEngine->GetA3DDevice(), g_pGame->GetConfigs()->GetInGameUIDcf()))
		{
			m_pCurUIMan = NULL;
			::MessageBoxA(NULL, "无法加载游戏数据文件！\n此错误可能是更新不完全所致，请重新进行更新或进行版本验证！", "错误", MB_OK | MB_ICONSTOP);
			glb_ErrorOutput(ECERR_FAILEDTOCALL, "CECUIManager::ChangeCurUIManager", __LINE__);
			return false;
		}
	}
	else if (iUIMan == UIMAN_LOADING )
	{
		AUILuaManager* pLoadingUI = new AUILuaManager;
		if (!pLoadingUI)
		{
			glb_ErrorOutput(ECERR_NOTENOUGHMEMORY, "CECUIManager::ChangeCurUIManager", __LINE__);
			return false;
		}

		m_pCurUIMan = pLoadingUI;
		if (!pLoadingUI->Init(m_pA3DEngine, m_pA3DEngine->GetA3DDevice(), g_pGame->GetConfigs()->GetLoadingUIDcf()))
		{
			m_pCurUIMan = NULL;
			::MessageBoxA(NULL, "无法加载游戏数据文件！\n此错误可能是更新不完全所致，请重新进行更新或进行版本验证！", "错误", MB_OK | MB_ICONSTOP);
			glb_ErrorOutput(ECERR_FAILEDTOCALL, "CECUIManager::ChangeCurUIManager", __LINE__);
			return false;
		}
	}
	else
	{
		ASSERT(0);
		return false;
	}

	m_iCurUIMan	= iUIMan;

	
	// init help system
	if (iUIMan == UIMAN_LOGIN)
	{
		ASSERT(!m_pScriptMan);
		// create help system for login ui
		// not used now
	}
	else if (iUIMan == UIMAN_INGAME)
	{
		ASSERT(!m_pScriptMan);
		
// 		if (!(m_pScriptMan = new CECScriptManInGame))
// 		{
// 			glb_ErrorOutput(ECERR_NOTENOUGHMEMORY, "CECUIManager::ChangeCurUIManager", __LINE__);
// 			return false;
// 		}
// 		
// 		if (!m_pScriptMan->Init())
// 		{
// 			delete m_pScriptMan;
// 			m_pScriptMan = NULL;
// 			a_LogOutput(1, "CECUIManager::ChangeCurUIManager, Failed to initialize Help system for game.");
// 			return false;
// 		}
	}
	else if (iUIMan == UIMAN_LOADING)
	{
		ASSERT(!m_pScriptMan);
		// create help system for login ui
		// not used now
	}

	return true;
}
*/
//	Release current UI manager
void CECUIManager::ReleaseCurUIManager()
{
	if (m_iCurUIMan < 0)
	{
		ASSERT(!m_pCurUIMan);
		return;
	}

	ASSERT(m_pCurUIMan);
	m_pCurUIMan->Release();
	delete m_pCurUIMan;
	m_pCurUIMan = NULL;
	m_iCurUIMan = -1;

//	A3DRELEASE(m_pScriptMan);
}
/*
//	Deal windows message
bool CECUIManager::DealWindowsMessage(UINT message, WPARAM wParam, LPARAM lParam)
{
	if (!m_pCurUIMan)
		return false;

	bool bRet = m_pCurUIMan->DealWindowsMessage(message, wParam, lParam);

	return bRet;
}

//	Tick routine
bool CECUIManager::Tick(DWORD dwDeltaTime)
{
	if (m_pCurUIMan)
		m_pCurUIMan->Tick(dwDeltaTime);

	// help system
	if (m_pScriptMan)
		m_pScriptMan->Tick(dwDeltaTime);

	return true;
}

//	Render routine
bool CECUIManager::Render()
{
	if (m_pCurUIMan)
		m_pCurUIMan->Render();

	return true;
}

//	If current UI manager is LOGIN manager, this function return it's interface
CECLoginUIMan* CECUIManager::GetLoginUIMan()
{
	if (m_pCurUIMan && m_iCurUIMan == UIMAN_LOGIN)
		return (CECLoginUIMan*)m_pCurUIMan;
	else
		return NULL;
}
*/
//	If current UI manager is INGAME manager, this function return it's interface
CECGameUIMan* CECUIManager::GetInGameUIMan()
{
	if (m_pCurUIMan && m_iCurUIMan == UIMAN_INGAME)
		return (CECGameUIMan*)m_pCurUIMan;
	else
		return NULL;
}
/*
bool CECUIManager::UIControlCursor()
{
	if( m_pCurUIMan )
	{
		if( m_iCurUIMan == UIMAN_LOGIN )
			return ((CECLoginUIMan *)m_pCurUIMan)->UIControlCursor();
		else if( m_iCurUIMan == UIMAN_INGAME )
			return ((CECGameUIMan *)m_pCurUIMan)->UIControlCursor();
	}
	return false;
}
*/
//void CECUIManager::ShowErrorMsg(const ACHAR *pszMsg, const char *pszName/* "" */)
/*{
	if( m_pCurUIMan )
	{
		if( m_iCurUIMan == UIMAN_LOGIN )
			((CECLoginUIMan *)m_pCurUIMan)->ShowErrorMsg(pszMsg, pszName);
		else if( m_iCurUIMan == UIMAN_INGAME )
			((CECGameUIMan *)m_pCurUIMan)->ShowErrorMsg(pszMsg, pszName);
	}
}
*/