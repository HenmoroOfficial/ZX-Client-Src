/*
 * FILE: EC_CameraPath.cpp
 *
 * DESCRIPTION: 
 *
 * CREATED BY: Xiao Zhou, 2008/12/10
 *
 * HISTORY: 
 *
 * Copyright (c) 2008 Archosaur Studio, All Rights Reserved.
 */

#include "EC_Global.h"
#include "EC_CameraPath.h"
#include "EC_Game.h"
#include "EC_GameRun.h"
#include "EC_World.h"
#include "EC_Scene.h"
#include "EC_Configs.h"
#include "EC_UIManager.h"
#include "EC_GameUIMan.h"
#include "EC_HostPlayer.h"
#include "EC_InputCtrl.h"
#include "EC_HPWork.h"
#include "EC_HPWorkTrace.h"
#include "EC_HPWorkMove.h"
#include "EC_HPWorkAutoMove.h"
#include "DlgCameraPath.h"

#ifdef BACK_VERSION
#include "EC_Viewport.h"
#else
#include "ECViewport.h"
#endif
#include "A3DCamera.h"

#include "AUI/AUIDialog.h"

#include "LuaState.h"
#include "LuaUtil.h"
#include "ScriptValue.h"
#include "LuaFuncCommon.h"


///////////////////////////////////////////////////////////////////////////
//	
//	Define and Macro
//	
///////////////////////////////////////////////////////////////////////////

#define	new A_DEBUG_NEW
#define CAMERAPATH_TBL_NAME ("CameraPath")
const static unsigned int INVALID_CAMERA_PATH_INDEX = (unsigned int)-1;

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


///////////////////////////////////////////////////////////////////////////
//	
//	Implement CECCameraPath
//	
///////////////////////////////////////////////////////////////////////////

CECCameraPath::CECCameraPath():m_dwTickTimeExp(10)
{
	m_bLastShowUI = true;
	m_bUsingCameraPath = false;
	m_pathIndexDelayed = INVALID_CAMERA_PATH_INDEX;
	m_counter.SetPeriod(0);
	m_counter.Reset(false);
}

void CECCameraPath::Tick(DWORD dwDeltaTime)
{
	_ProcessDelayedCamera();
	if (IsUsingCameraPath())
	{
		if(!m_CameraPathWalker.IsWalking())
		{
			if(!m_counter.IsFull())
			{
				m_counter.IncCounter(dwDeltaTime);
			}
			else
			{
				m_counter.Reset(false);
				EndCameraPath();
			}
		}else
		{
			m_dwTickTimeExp.AddData(dwDeltaTime);
			DWORD deltaTime = m_dwTickTimeExp.GetExpectation();
			m_CameraPathWalker.Tick(deltaTime);	
		}
	}
} 

A3DVECTOR3 CECCameraPath::GetDir()
{
	return m_CameraPathWalker.GetDir();
}

A3DVECTOR3 CECCameraPath::GetPos()
{
	return m_CameraPathWalker.GetPos();
}

float CECCameraPath::GetRatio()
{
	return m_CameraPathWalker.GetRatio();
}

float CECCameraPath::GetFov()
{
	return m_CameraPathWalker.GetFov();
}

CECBezier* CECCameraPath::GetBezier(DWORD dwPathIndex)
{
	return (CECBezier*)g_pGame->GetGameRun()->GetWorld()->GetCutsceneBezier(dwPathIndex);
//	return (CECBezier*)g_pGame->GetGameRun()->GetWorld()->GetScene()->GetCameraPathObject(dwPathIndex);
}

bool CECCameraPath::StartCameraPath(DWORD dwPathIndex, DWORD* pIDBezier, DWORD dwWaitMilliseconds)
{
	//end other camera path playing now.
	EndCameraPath();

	CECBezier* pBezier = GetBezier(dwPathIndex);
	if (!pBezier)
	{
		a_LogOutput(1, ("CECCameraPath::StartCameraPath, error camera path id %d\n"), m_id);
		return false;
	}
 	m_pathIndex = dwPathIndex;

	m_CameraPathWalker.BindBezier(pBezier);
	m_CameraPathWalker.StartWalk(false, true);
	m_bUsingCameraPath = true;
	_OnStartCamera(dwPathIndex);	
	
//	实现预加载，加载完场景后才开始漫游
// 	A3DVECTOR3 vPos = pBezier->GetPoint(0)->GetPos();
//	g_pGame->GetGameRun()->GetWorld()->SetLoadCenter(vPos);

	return true;
}

void CECCameraPath::EndCameraPath()
{
	if (m_bUsingCameraPath)
		m_bUsingCameraPath = false;

	m_pathIndexDelayed = INVALID_CAMERA_PATH_INDEX;
	m_counter.Reset(false);
}

bool CECCameraPath::IsUsingCameraPath()
{
	return m_bUsingCameraPath;
}

bool CECCameraPath::HasDelayedCameraPath()
{ 
	return (m_pathIndexDelayed!=INVALID_CAMERA_PATH_INDEX) ;
}

void CECCameraPath::_DelayCameraPath(const unsigned int pathIndex)
{
	//just delay on camera path, discard last one.
	m_pathIndexDelayed = pathIndex;
}

void CECCameraPath::_ProcessDelayedCamera()
{
	if (CECUIManager::UIMAN_LOGIN==g_pGame->GetGameRun()->GetUIManager()->GetCurrentUIMan()) // || g_pGame->GetConfigs()->IsNCfgReady())
	{
		StartCameraPath(m_pathIndexDelayed);
	}
}

void CECCameraPath::_OnStartCamera(DWORD dwPathIndex)
{
	m_dwTickTimeExp.Clear();
}
