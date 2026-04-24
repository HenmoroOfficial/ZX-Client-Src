/*
 * FILE: OptionsPage.cpp
 *
 * DESCRIPTION: 
 *
 * CREATED BY: Duyuxin, 2004/6/19
 *
 * HISTORY: 
 *
 * Copyright (c) 2004 Archosaur Studio, All Rights Reserved.
 */

#include "Global.h"
#include "OptionsPage.h"
#include "RollUpPanel.h"

///////////////////////////////////////////////////////////////////////////
//	
//	Define and Macro
//	
///////////////////////////////////////////////////////////////////////////

//#define new A_DEBUG_NEW
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
//	Implement COptionsPage
//	
///////////////////////////////////////////////////////////////////////////

BEGIN_MESSAGE_MAP(COptionsPage, CRollUpWnd)
	//{{AFX_MSG_MAP(COptionsPage)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

COptionsPage::COptionsPage()
{
}

COptionsPage::~COptionsPage()
{
}

BOOL COptionsPage::Create(LPCTSTR lpszClassName, LPCTSTR lpszWindowName, DWORD dwStyle, const RECT& rect, CWnd* pParentWnd, UINT nID, CCreateContext* pContext) 
{
	if (!CRollUpWnd::Create(lpszClassName, lpszWindowName, dwStyle, rect, pParentWnd, nID, pContext))
	{
		g_Log.Log("COptionsPage::Create, Failed to create terrain page");
		return FALSE;
	}

	//	Create option panel
	m_OptionPanel.Create(IDD_OPTIONS, this);
	if (CreatePanel("一般设置", 0, &m_OptionPanel) < 0)
	{
		g_Log.Log("COptionsPage::Create, Failed to create option panel");
		return FALSE;
	}

	//	Create render option panel
	m_RenderPanel.Create(IDD_RENDEROPT, this);
	if (CreatePanel("渲染", 0, &m_RenderPanel) < 0)
	{
		g_Log.Log("COptionsPage::Create, Failed to create render option panel");
		return FALSE;
	}
	
	m_BrushSetPanel.Create(IDD_BRUSH_SET,this);
	if (CreatePanel("画刷设置", 0, &m_BrushSetPanel) < 0)
	{
		g_Log.Log("COptionsPage::Create, Failed to create brush option panel");
		return FALSE;
	}

	m_PostEffectPanel.Create(IDD_DIALOG_POSTEFFECT,this);
	if (CreatePanel("屏幕后处理", 0, &m_PostEffectPanel) < 0)
	{
		g_Log.Log("COptionsPage::Create, Failed to create post effect option panel");
		return FALSE;
	}

	m_FogPanel.Create(IDD_FOG,this);
	if (CreatePanel("雾参数", 0, &m_FogPanel) < 0)
	{
		g_Log.Log("COptionsPage::Create, Failed to create fog panel");
		return FALSE;
	}
	
	//	Show panels
	int aIndices[NUM_PANEL] = {PANEL_OPTION, PANEL_RENDER, PANEL_BRUSH, PANEL_POST, PANEL_FOG};
	bool aOpenFlags[NUM_PANEL] = {false, false, true, false, false};
	ShowPanels(aIndices, aOpenFlags, NUM_PANEL);

	return TRUE;
}

//	Release resources
void COptionsPage::Release()
{
}

//	Initlaize device objects
bool COptionsPage::InitDeviceObjects()
{
	m_RenderPanel.InitDeviceObjects();
	return true;
}
