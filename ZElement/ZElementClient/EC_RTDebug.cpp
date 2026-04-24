/*
 * FILE: EC_RTDebug.cpp
 *
 * DESCRIPTION: 
 *
 * CREATED BY: Duyuxin, 2004/9/9
 *
 * HISTORY: 
 *
 * Copyright (c) 2004 Archosaur Studio, All Rights Reserved.
 */

#include "EC_Global.H"
#include "EC_RTDebug.h"
#include "EC_Game.h"
#include "EC_Configs.h"
#include "EC_Resource.h"

#include "A3DDevice.h"
#include "A3DFont.h"
#include "A3DViewport.h"
#include "A3DEngine.h"
#include "AChar.h"

#ifdef BACK_VERSION
#include "EC_Viewport.h"
#else
#include "ECViewport.h"
#endif

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


///////////////////////////////////////////////////////////////////////////
//	
//	Implement CECRTDebug
//	
///////////////////////////////////////////////////////////////////////////

CECRTDebug::CECRTDebug()
{
	m_pA3DFont		= NULL;
	m_iMaxVisStr	= 20;
	m_iLineSpace	= 16;
	m_dwRTimeCnt	= 0;
	m_dwRTimeInter	= 5000;
}

CECRTDebug::~CECRTDebug()
{
}

//	Initialize object
bool CECRTDebug::Init(int iMaxVisStr)
{
	m_iMaxVisStr = iMaxVisStr;

	if (!(m_pA3DFont = g_pGame->GetFont(RES_FONT_TITLE)))
		m_pA3DFont = g_pGame->GetA3DEngine()->GetSystemFont();

	return true;
}

//	Release object
void CECRTDebug::Release()
{
	EnterCriticalSection(&g_csRTDebug);

	//	Release all resources
	ALISTPOSITION pos = m_DbgStrList.GetHeadPosition();
	while (pos)
	{
		DBGSTR* pStr = m_DbgStrList.GetNext(pos);
		delete pStr;
	}

	m_DbgStrList.RemoveAll();

	LeaveCriticalSection(&g_csRTDebug);
}

//	Output a debug string
void CECRTDebug::OutputDebugInfo(DWORD dwCol, const ACHAR* szMsg)
{
	if (!szMsg || !a_strlen(szMsg))
		return;

	DBGSTR* pStr = new DBGSTR;
	if (!pStr)
		return;

	//	Roll other strings up
	RollStringsUp(true);

	pStr->dwCol		= dwCol;
	pStr->strMsg	= szMsg;
	pStr->iLine		= 0;
	pStr->iLevel	= 1;

	EnterCriticalSection(&g_csRTDebug);
	m_DbgStrList.AddHead(pStr);
	LeaveCriticalSection(&g_csRTDebug);
}

//	Output a notify message
void CECRTDebug::OutputNotifyMessage(DWORD dwCol, const ACHAR* szMsg)
{
	if (!szMsg || !a_strlen(szMsg))
		return;

	DBGSTR* pStr = new DBGSTR;
	if (!pStr)
		return;

	//	Roll other strings up
	RollStringsUp(true);

	pStr->dwCol		= dwCol;
	pStr->strMsg	= szMsg;
	pStr->iLine		= 0;
	pStr->iLevel	= 2;

	EnterCriticalSection(&g_csRTDebug);
	m_DbgStrList.AddHead(pStr);
	LeaveCriticalSection(&g_csRTDebug);
}

//	Roll strings up
void CECRTDebug::RollStringsUp(bool bClearCnt)
{
	EnterCriticalSection(&g_csRTDebug);

	ALISTPOSITION pos = m_DbgStrList.GetHeadPosition();
	while (pos)
	{
		ALISTPOSITION posCur = pos;
		DBGSTR* pStr = m_DbgStrList.GetNext(pos);
		pStr->iLine++;

		if (pStr->iLine >= m_iMaxVisStr)
		{
			delete pStr;
			m_DbgStrList.RemoveAt(posCur);
		}
	}

	if (bClearCnt)
		m_dwRTimeCnt = 0;

	LeaveCriticalSection(&g_csRTDebug);
}

//	Tick routine
bool CECRTDebug::Tick(DWORD dwDeltaTime)
{
	m_dwRTimeCnt += dwDeltaTime;
	
	if (m_dwRTimeCnt >= m_dwRTimeInter)
	{
		m_dwRTimeCnt -= m_dwRTimeInter;

		RollStringsUp(false);
	}

	return true;
}

//	Render routine
bool CECRTDebug::Render(CECViewport* pViewport)
{
	if (!m_pA3DFont || !g_pGame->GetConfigs()->GetRTDebugLevel())
		return true;

	g_pGame->GetA3DDevice()->SetZWriteEnable(false);

	EnterCriticalSection(&g_csRTDebug);

	A3DVIEWPORTPARAM* pvp = pViewport->GetA3DViewport()->GetParam();

	int iLevel = g_pGame->GetConfigs()->GetRTDebugLevel();
	int x = pvp->X + 10;
	int y = pvp->Y + pvp->Height - 200 - m_iLineSpace;

	ALISTPOSITION pos = m_DbgStrList.GetHeadPosition();
	while (pos)
	{
		DBGSTR* pStr = m_DbgStrList.GetNext(pos);
		if (pStr->iLevel < iLevel)
			continue;

		int oy = y - pStr->iLine * m_iLineSpace;
		if (oy < (int)pvp->Y)
			break;

		m_pA3DFont->TextOut(x, oy, pStr->strMsg, pStr->dwCol);
	}

	LeaveCriticalSection(&g_csRTDebug);

	m_pA3DFont->Flush();
	g_pGame->GetA3DDevice()->SetZWriteEnable(true);

	return true;
}



