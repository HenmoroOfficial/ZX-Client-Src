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
#include "EC_UIManager.h"
#include "EC_LoginUIMan.h"
#include "EC_GameUIMan.h"
#include "EC_Game.h"
#include "EC_GameRun.h"
#include "EC_Configs.h"
#include "ECScriptManInGame.h"
#include "EC_PlayerImage.h"

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

float fProgCnt = 0.0f;
float fLoginUIInc = 20.0f / 60.0f;
float fGameUIInc = 220.0f / 1000.0f;

///////////////////////////////////////////////////////////////////////////
//	
//	Local functions
//	
///////////////////////////////////////////////////////////////////////////


AUILuaManager*  GetAuiManager()
{
	return g_pGame->GetGameRun()->GetUIManager()->GetCurrentUIManPtr();
}

void LoadingCallBack()
{
	if( g_pGame->GetGameRun()->GetUIManager()->GetCurrentUIMan() == CECUIManager::UIMAN_LOGIN )
		fProgCnt += fLoginUIInc;
	else
		fProgCnt += fGameUIInc;
	int iStep = (int)fProgCnt;
	if (iStep)
	{
		g_pGame->GetGameRun()->StepLoadProgress(iStep);
		fProgCnt -= (float)iStep;
	}
}

///////////////////////////////////////////////////////////////////////////
//	
//	Implement CECUIManager
//	
///////////////////////////////////////////////////////////////////////////

CECUIManager::CECUIManager()
{
	m_pA3DEngine	= NULL;
	m_pCurUIMan		= NULL;
	m_iCurUIMan		= -1;
	m_pScriptMan	= NULL;
	m_pPlayerImage	= NULL;
}

CECUIManager::~CECUIManager()
{
}

//	Initalize manager
bool CECUIManager::Init(A3DEngine* pA3DEngine, int iInitUIMan)
{
	m_pA3DEngine	= pA3DEngine;
	m_iCurUIMan		= iInitUIMan;

	if (!ChangeCurUIManager(iInitUIMan))
	{
		a_LogOutput(1, "CECUIManager::Init, Failed to create initial UI manager");
		return false;
	}

	m_pPlayerImage = new CECPlayerImage;
	m_pPlayerImage->Init();

	return true;
}

//	Release manager
void CECUIManager::Release(void)
{
	A3DRELEASE(m_pPlayerImage)

	ReleaseCurUIManager();
}

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
		fProgCnt = 0.0f;
		pLoginUI->SetLoadingCallback(LoadingCallBack);
		if (!pLoginUI->Init(m_pA3DEngine, m_pA3DEngine->GetA3DDevice(), g_pGame->GetConfigs()->GetLoginUIDcf()))
		{
			m_pCurUIMan = NULL;
			::MessageBoxA(NULL, "无法加载游戏数据文件！\n此错误可能是更新不完全所致，请重新进行更新或进行版本验证！", "错误", MB_OK | MB_ICONSTOP);
			glb_ErrorOutput(ECERR_FAILEDTOCALL, "CECUIManager::ChangeCurUIManager", __LINE__);
			return false;
		}
		g_pGame->GetConfigs()->EnableGlobalFog(false);	// 老地图会用默认雾，暂时关掉
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
		fProgCnt = 0.0f;
		pInGameUI->SetLoadingCallback(LoadingCallBack);
		if (!pInGameUI->Init(m_pA3DEngine, m_pA3DEngine->GetA3DDevice(), g_pGame->GetConfigs()->GetInGameUIDcf()))
		{
			m_pCurUIMan = NULL;
			::MessageBoxA(NULL, "无法加载游戏数据文件：界面相关文件！\n此错误可能是更新不完全所致，请重新进行更新或进行版本验证！", "错误", MB_OK | MB_ICONSTOP);
			glb_ErrorOutput(ECERR_FAILEDTOCALL, "CECUIManager::ChangeCurUIManager", __LINE__);
			return false;
		}
		g_pGame->GetConfigs()->EnableGlobalFog(true);
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

	return true;
}

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

	A3DRELEASE(m_pScriptMan);
}

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

#ifndef BACK_VERSION
bool CECUIManager::Render( const ECRENDERPARAM* pRenderParam )
{
	return Render();
}
#endif

//	If current UI manager is LOGIN manager, this function return it's interface
CECLoginUIMan* CECUIManager::GetLoginUIMan()
{
	if (m_pCurUIMan && m_iCurUIMan == UIMAN_LOGIN)
		return (CECLoginUIMan*)m_pCurUIMan;
	else
		return NULL;
}

//	If current UI manager is INGAME manager, this function return it's interface
CECGameUIMan* CECUIManager::GetInGameUIMan()
{
	if (m_pCurUIMan && m_iCurUIMan == UIMAN_INGAME)
		return (CECGameUIMan*)m_pCurUIMan;
	else
		return NULL;
}

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

void CECUIManager::ShowErrorMsg(const ACHAR *pszMsg, const char *pszName/* "" */)
{
	if( m_pCurUIMan )
	{
		if( m_iCurUIMan == UIMAN_LOGIN )
			((CECLoginUIMan *)m_pCurUIMan)->ShowErrorMsg(pszMsg, pszName);
		else if( m_iCurUIMan == UIMAN_INGAME )
			((CECGameUIMan *)m_pCurUIMan)->ShowErrorMsg(pszMsg, pszName);
	}
}
